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



#include "jkqtmathtext/nodes/jkqtmathtexttextnode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>





JKQTMathTextTextBaseNode::JKQTMathTextTextBaseNode(JKQTMathText *parent, const QString &text_):
    JKQTMathTextNode(parent),
    text(text_)
{

}

JKQTMathTextTextBaseNode::~JKQTMathTextTextBaseNode()
{

}

QString JKQTMathTextTextBaseNode::getText() const
{
    return text;
}

void JKQTMathTextTextBaseNode::drawString(QPainter &painter, const JKQTMathTextEnvironment &currentEv, double x, double y, const QString &txt) const {
    const QFont f=currentEv.getFont(parentMathText);
    drawString(painter, currentEv, f, x, y, txt);
}

void JKQTMathTextTextBaseNode::drawString(QPainter &painter, const JKQTMathTextEnvironment &currentEv, const QFont &f, double x, double y, const QString &txt) const
{
    const QFontMetricsF fm(f, painter.device());
    const QPen p(currentEv.color, fm.lineWidth()/2.0, Qt::SolidLine);
    painter.setPen(p);
    if (currentEv.font==MTEblackboard && parentMathText->isFontBlackboardSimulated()) {
        QPainterPath path;
        path.addText(QPointF(x, y), f, txt);
        path.addText(QPointF(x+fm.lineWidth()/2.0, y), f, txt);
        painter.drawPath(path);
    } else {
        painter.setFont(f);
        painter.drawText(QPointF(x, y), txt);
    }
}

QString JKQTMathTextTextBaseNode::textTransform(const QString &text, const JKQTMathTextEnvironment &/*currentEv*/) const
{
    return  text;
}


bool JKQTMathTextTextBaseNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    html=html
            +currentEv.toHtmlStart(defaultEv, parentMathText)
            +textTransform(text, currentEv).toHtmlEscaped()
            +currentEv.toHtmlAfter(defaultEv, parentMathText);
    return true;
}




JKQTMathTextTextNode::JKQTMathTextTextNode(JKQTMathText* _parent, const QString& textIn, bool addWhitespace, bool stripInnerWhitepace):
    JKQTMathTextTextBaseNode(_parent, "")
{
    QString textTransformed=textIn;

    if (stripInnerWhitepace) {
        textTransformed="";
        for (int i=0; i<textIn.size(); i++) {
            if (!textIn[i].isSpace()) textTransformed+=textIn[i];
        }
    }

    text=textTransformed;
    // strip all whitespace from left
    while (text.size()>1 && text[0].isSpace()) {
        text=text.right(text.size()-1);
    }
    if (addWhitespace && (text.size()>0) && (!text[text.size()-1].isSpace())) text=text+" ";
    //qDebug()<<"JKQTMathTextTextNode( text="<<text<<" addWhitespace="<<addWhitespace<<")   [=> this->text="<<this->text<<"]";
}

JKQTMathTextTextNode::~JKQTMathTextTextNode() = default;

void JKQTMathTextTextNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    QStringList textpart;
    QList<bool> fontForcedUpright;
    QList<double> textpartXPos;
    getSizeInternalAndData(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos,textpart, fontForcedUpright, textpartXPos);
}

void JKQTMathTextTextNode::getSizeInternalAndData(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, QStringList &textpart, QList<bool> &fontForcedUpright, QList<double> &textpartXPos)
{
    textpart.clear();
    fontForcedUpright.clear();
    const QString txt=textTransform(text, currentEv);
    if (currentEv.insideMath && currentEv.insideMathForceDigitsUpright) {
        splitTextForMathMode(txt, textpart, fontForcedUpright);
    } else {
        textpart.append(text);
        fontForcedUpright.append(false);
    }


    const QFont f=currentEv.getFont(parentMathText);
    const QFont fnonItalic=JKQTMathTextGetNonItalic(f);
    const QFontMetricsF fmNonItalic(fnonItalic, painter.device());
    const QFontMetricsF fm(f, painter.device());
#if (QT_VERSION>=QT_VERSION_CHECK(5, 15, 0))
    const double sp=fm.horizontalAdvance(' ');
#else
    const double sp=fm.width(' ');
#endif
    width=0;
    double ascent=0;
    double descent=0;
    for (int i=0; i<textpart.size(); i++) {
        const QRectF br=(fontForcedUpright[i]) ? fmNonItalic.boundingRect(textpart[i]) : fm.boundingRect(textpart[i]);
        const QRectF tbr=(fontForcedUpright[i]) ? JKQTMathTextGetTightBoundingRect(fnonItalic, textpart[i], painter.device()) : JKQTMathTextGetTightBoundingRect(f, textpart[i], painter.device());
        textpartXPos.append(width);
        width+=br.width();
        if (textpart[i].size()>0 && textpart[i].at(textpart[i].size()-1).isSpace()) {
            // this correction is necessary, because it seems that QFontMetricsF::boundingRect() ignores trailing spaces
            width+=sp;
        }
        const double thisAscent=-tbr.top();
        const double thisDescent=tbr.bottom();
        ascent=qMax(ascent, thisAscent);
        descent=qMax(descent, thisDescent);
    }
    overallHeight=(ascent+descent); //fm.height();
    baselineHeight=ascent;
    strikeoutPos=fm.strikeOutPos();
}

void JKQTMathTextTextNode::splitTextForMathMode(const QString &txt, QStringList &textpart, QList<bool> &fontForcedUpright)
{
    auto isForcedUprightChar=[](const QChar& c) {
        return c.isDigit()
                || c=='(' || c=='[' || c=='|' || c==']' || c==')' || c=='<' || c=='>'|| c=='{' || c=='}' || c=='|'
                || c==QChar(0x2329) || c==QChar(0x232A) || c==QChar(0x2308) || c==QChar(0x2309) || c==QChar(0x230A) || c==QChar(0x230B);
    };

    textpart.clear();
    fontForcedUpright.clear();
    QString currentSection="";
    bool currentSectionForcedUpright=false;
    int i=0;
    while (i<txt.size()) {
        const QChar c=txt[i];
        const bool CisForcedUprightChar=isForcedUprightChar(c);
        const bool CisForcedUprightCharExt=CisForcedUprightChar||(c=='.')||(c==',');
        if (currentSection.size()==0) {
            // start new section
            currentSectionForcedUpright=CisForcedUprightChar;
            currentSection+=c;
        } else {
            // existing section
            if (CisForcedUprightCharExt==currentSectionForcedUpright) {
                // continue current section
                currentSection+=c;
            } else {
                // start new section
                textpart.append(currentSection);
                fontForcedUpright.append(currentSectionForcedUpright);
                currentSection=c;
                currentSectionForcedUpright=CisForcedUprightChar;
            }
        }
        i++;
    }
    if (currentSection.size()>0) {
        textpart.append(currentSection);
        fontForcedUpright.append(currentSectionForcedUpright);
    }
}

double JKQTMathTextTextNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0;
    double baselineHeight=0;
    double overallHeight=0;
    double sp=0;
    QStringList textpart;
    QList<bool> fontForcedUpright;
    QList<double> textpartXPos;
    getSizeInternalAndData(painter, currentEv, width, baselineHeight, overallHeight, sp, textpart, fontForcedUpright, textpartXPos);


    const QFont f=currentEv.getFont(parentMathText);
    const QFont fnonItalic=JKQTMathTextGetNonItalic(f);
    const QFontMetricsF fm(f, painter.device());
    const QFontMetricsF fmNonItalic(fnonItalic, painter.device());

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setFont(f);

    //qDebug()<<"JKQTMathTextTextNode: text="<<text<<" font="<<f;


    for (int i=0; i<textpart.size(); i++) {
        if (fontForcedUpright[i]) drawString(painter, currentEv, fnonItalic, x+textpartXPos[i], y, textpart[i]);
        else drawString(painter, currentEv, f, x+textpartXPos[i], y, textpart[i]);
    }

    return x+width;
}



QString JKQTMathTextTextNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextTextNode(")+text+")";
}

QString JKQTMathTextTextNode::textTransform(const QString &text, const JKQTMathTextEnvironment &currentEv) const
{
    QString txt=text;
    auto fnt=parentMathText->getFontData(currentEv.font, currentEv.insideMath);
    const QFontMetricsF fm(currentEv.getFont(parentMathText));
    if (fnt.second==MTFELatin1 || fnt.second==MTFEUnicode) {
        if (currentEv.insideMath) {
            txt="";
            for (int i=0; i<text.size(); i++) {
                QChar c=text[i];
                switch(c.unicode()) {
                    case '-':
                    if (fm.inFont(QChar(0x2212))) {
                        txt+=QString(QString(" ")+QChar(0x2212));
                    } else {
                        txt+=QString(QString(" -"));
                    }
                    break;
                    case '+': txt+=QString(QString(" +")); break;
                    case '<': txt+=QString(QString(" <")); break;
                    case '>': txt+=QString(QString(" >")); break;
                    case '=': txt+=QString(QString(" =")); break;
                    case ';': txt+=QString(QString("; ")); break;
                    case ',': txt+=QString(QString(", ")); break;
                    default: txt+=c; break;
                }
            }
            txt=txt.replace("  ", " ");
        }
    }

    return txt;
}



JKQTMathTextVerbatimNode::JKQTMathTextVerbatimNode(JKQTMathText *_parent, const QString& _text, bool visibleWhitespace_, JKQTMathTextHorizontalAlignment _alignment, double _linespacingFactor, JKQTMathTextVerticalOrientation _verticalOrientation):
    JKQTMathTextTextBaseNode(_parent, _text),
    alignment(_alignment),
    lineSpacingFactor(_linespacingFactor),
    verticalOrientation(_verticalOrientation),
    visibleWhitespace(visibleWhitespace_)
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

double JKQTMathTextVerbatimNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize *prevNodeSize)
{
    doDrawBoxes(painter, x, y, currentEv);
    transformEnvironment(currentEv);
    const LayoutInfo l=calcLayout(painter, currentEv);
    QFont f=currentEv.getFont(parentMathText);
    f.setStyleStrategy(QFont::PreferDefault);
    f.setFixedPitch(true);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    for (int i=0; i<l.lines.size(); i++) {
        drawString(painter, currentEv, f, x+l.X.at(i).x(), y+l.X.at(i).y(), l.lines.at(i));
    }
    return x+l.width;
}

bool JKQTMathTextVerbatimNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv)
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

void JKQTMathTextVerbatimNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, const JKQTMathTextNodeSize *prevNodeSize)
{
    transformEnvironment(currentEv);
    const  LayoutInfo l=calcLayout(painter, currentEv);
    width=l.width;
    overallHeight=l.overallHeight;
    baselineHeight=l.baselineHeight;
    strikeoutPos=l.strikeoutPos;
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
    const QFontMetricsF fm(f);
    const double linespacing=fm.lineSpacing()*lineSpacingFactor;
    const double fleading=fm.leading();
    const double synLeading=fm.lineWidth();
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
            heightSum=fm.ascent();
        } else if (i>0) {
            const double deltaLine=qMax(linespacing, descents.last()+lineLeading+fm.ascent());
            heightSum=heightSum+deltaLine;
            y=y+deltaLine;
        }
        widths<<fm.boundingRect(l.lines[i]).width();
        l.width=qMax(l.width, widths.last());
        heights<<fm.height();
        ascents<<fm.ascent();
        descents<<fm.descent();
        strikeouts<<fm.strikeOutPos();
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
    QString tabRep=QString(4,QChar(0x2192));
    if (!fm.inFont(tabRep[0])) {
        spRep=QString(4,QChar(0xAC));
    }

    QString res=JKQTMathTextTextBaseNode::textTransform(text, currentEv);
    if (res.startsWith('\n')) res=res.right(res.size()-1);
    if (res.endsWith('\n')) res=res.left(res.size()-1);
    if (visibleWhitespace) {
        res.replace(' ', spRep);
        res.replace('\t', tabRep);
        return res;
    }
    return res;
}


JKQTMathTextVerbatimNode::LayoutInfo::LayoutInfo():
    JKQTMathTextNodeSize(), lines(), X()
{

}
