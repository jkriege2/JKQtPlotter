/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)



    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtmathtext/nodes/jkqtmathtextverbatimnode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>






JKQTMathTextVerbatimNode::JKQTMathTextVerbatimNode(JKQTMathText *_parent, const QString& _text, bool visibleWhitespace_, JKQTMathTextHorizontalAlignment _alignment, double _linespacingFactor, JKQTMathTextVerticalOrientation _verticalOrientation, size_t tabSize_):
    JKQTMathTextTextBaseNode(_parent, _text),
    alignment(_alignment),
    lineSpacingFactor(_linespacingFactor),
    verticalOrientation(_verticalOrientation),
    visibleWhitespace(visibleWhitespace_),
    tabSize(tabSize_)
{

}

QString JKQTMathTextVerbatimNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextVerbatimNode");
}

JKQTMathTextHorizontalAlignment JKQTMathTextVerbatimNode::getAlignment() const
{
    return alignment;
}

JKQTMathTextVerticalOrientation JKQTMathTextVerbatimNode::getVerticalOrientation() const
{
    return verticalOrientation;
}

double JKQTMathTextVerbatimNode::getLineSpacingFactor() const
{
    return lineSpacingFactor;
}

bool JKQTMathTextVerbatimNode::getVisibleWhitespace() const
{
    return visibleWhitespace;
}

size_t JKQTMathTextVerbatimNode::getTabSize() const
{
    return tabSize;
}

double JKQTMathTextVerbatimNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv) const
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextVerbatimNode[]::draw()"));
#endif
    transformEnvironment(currentEv);
    const LayoutInfo l=calcLayout(painter, currentEv);
    doDrawBoxes(painter, x, y, l);
    QFont f=currentEv.getFont(parentMathText);
    f.setStyleStrategy(QFont::PreferDefault);
    f.setFixedPitch(true);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setFont(f);
    for (int i=0; i<l.lines.size(); i++) {
        painter.drawText(QPointF(x+l.X.at(i).x(), y+l.X.at(i).y()), l.lines.at(i));
    }
    return x+l.width;
}

bool JKQTMathTextVerbatimNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const
{
    transformEnvironment(currentEv);
    const bool isMultiLine=text.count('\n')>0;
    html+=currentEv.toHtmlStart(defaultEv, parentMathText);
    if (isMultiLine) {
        if (alignment==MTHALeft) {
            html+="<div align=\"left\">";
        } else if (alignment==MTHACentered) {
            html+="<div align=\"center\">";
        } else if (alignment==MTHARight) {
            html+="<div align=\"right\">";
        }
    }
    html+="<pre>";
    html+=textTransform(text, currentEv).toHtmlEscaped();
    html+="</pre>";
    if (isMultiLine) html+="</div>";
    html+=currentEv.toHtmlAfter(defaultEv, parentMathText);
    return true;
}

JKQTMathTextNodeSize JKQTMathTextVerbatimNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
    transformEnvironment(currentEv);
    return calcLayout(painter, currentEv);
}

void JKQTMathTextVerbatimNode::transformEnvironment(JKQTMathTextEnvironment &currentEv) const
{
    currentEv.font=MTEtypewriter;
}

JKQTMathTextVerbatimNode::LayoutInfo JKQTMathTextVerbatimNode::calcLayout(QPainter &painter, const JKQTMathTextEnvironment& currentEv) const
{
    LayoutInfo l;
    QFont f=currentEv.getFont(parentMathText);
    f.setStyleStrategy(QFont::PreferDefault);
    f.setFixedPitch(true);
    const qreal fascent=JKQTMathTextGetFontAscent(f, painter.device());
    const double linespacing=JKQTMathTextGetFontLineSpacing(f, painter.device())*lineSpacingFactor;
    const double fleading=JKQTMathTextGetFontLeading(f, painter.device());
    const double synLeading=JKQTMathTextGetFontLineWidth(f, painter.device());
    const double lineLeading=((fabs(fleading)>1e-6)?fleading:synLeading)*lineSpacingFactor;

    if (text.size()<=0) {
        return l;
    }
    l.lines=textTransform(text, currentEv).split('\n');

    // from now on we have at least one child node!!!

    QList<double> widths, heights, ascents, descents, strikeouts;
    double heightSum=0;
    QList<double> ysFromFirstLine; // y-position of each line, where the first line is always at y=0 (i.e. ysFromFirstLine[0]==0)
    double y=0;
    for (int i=0; i<l.lines.size(); i++) {

        if (i==0) {
            heightSum=fascent;
        } else if (i>0) {
            const double deltaLine=qMax(linespacing, descents.last()+lineLeading+fascent);
            heightSum=heightSum+deltaLine;
            y=y+deltaLine;
        }
        widths<<JKQTMathTextGetBoundingRect(f,l.lines[i],painter.device()).width();
        l.width=qMax(l.width, widths.last());
        heights<<JKQTMathTextGetFontHeight(f, painter.device());
        ascents<<fascent;
        descents<<JKQTMathTextGetFontDescent(f, painter.device());
        strikeouts<<JKQTMathTextGetFontStrikoutPos(f, painter.device());
        ysFromFirstLine<<y;
    }
    heightSum+=descents.last();

    l.overallHeight=heightSum;
    double y0=0;
    if (verticalOrientation==MTVOTop) {
        l.baselineHeight=0;
        l.strikeoutPos=0;
        y0=ascents.first();
    } else if (verticalOrientation==MTVOFirstLine) {
        l.baselineHeight=ascents.first();
        l.strikeoutPos=strikeouts.first();
        y0=0;
    } else if (verticalOrientation==MTVOCentered) {
        l.baselineHeight=heightSum/2.0;
        l.strikeoutPos=heightSum/4.0;
        y0=-heightSum/2.0+ascents.first();
    } else if (verticalOrientation==MTVOLastLine) {
        l.baselineHeight=heightSum-descents.last();
        l.strikeoutPos=strikeouts.last();
        y0=-(heightSum-ascents.first()-descents.last());
    } else if (verticalOrientation==MTVOBottom) {
        l.baselineHeight=heightSum;
        l.strikeoutPos=0;
        y0=-(heightSum-ascents.first());
    }
    for (int i=0; i<l.lines.size(); i++) {
        double x=0;
        if (alignment==MTHARight) x=l.width-widths[i];
        else if (alignment==MTHACentered) x=(l.width-widths[i])/2.0;
        l.X<<QPointF(x,ysFromFirstLine[i]+y0);
    }
    return l;
}

QString JKQTMathTextVerbatimNode::textTransform(const QString &text, const JKQTMathTextEnvironment &currentEv) const
{
    QFont f=currentEv.getFont(parentMathText);
    f.setStyleStrategy(QFont::PreferDefault);
    const QFontMetricsF fm(f);

    QString spRep=QChar(0xB7);
    if (!fm.inFont(spRep[0])) {
        spRep=QChar(0x2423);
    }
    QString tabRep=QString(tabSize,QChar(0x2192));
    if (!fm.inFont(tabRep[0])) {
        spRep=QString(tabSize,QChar(0xAC));
    }

    QString res=JKQTMathTextTextBaseNode::textTransform(text, currentEv);
    if (res.startsWith('\n')) res=res.right(res.size()-1);
    if (res.endsWith('\n')) res=res.left(res.size()-1);
    if (visibleWhitespace) {
        res.replace(' ', spRep);
        res.replace('\t', tabRep);
        return res;
    } else {
        res.replace('\t', QString(tabSize, ' '));
        return res;
    }
    return res;
}


JKQTMathTextVerbatimNode::LayoutInfo::LayoutInfo():
    JKQTMathTextNodeSize(), lines(), X()
{

}
