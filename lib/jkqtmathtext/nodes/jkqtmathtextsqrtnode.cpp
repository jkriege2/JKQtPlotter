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



#include "jkqtmathtext/nodes/jkqtmathtextsqrtnode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
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




JKQTMathTextSqrtNode::JKQTMathTextSqrtNode(JKQTMathText* _parent, JKQTMathTextNode* child, int degree):
    JKQTMathTextSingleChildNode(child, _parent)
{
    this->degree=degree;
}

JKQTMathTextSqrtNode::~JKQTMathTextSqrtNode() {
}

void JKQTMathTextSqrtNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    const QFontMetricsF fm(currentEv.getFont(parentMathText), painter.device());
    QFont fsmall=currentEv.getFont(parentMathText);
    fsmall.setPointSizeF(fsmall.pointSizeF()*parentMathText->getSqrtSmallFontFactor());
    fsmall.setItalic(false);
    const QFontMetricsF fmsmall(fsmall, painter.device());

    getChild()->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
    const double descent=overallHeight-baselineHeight;
    const double sqrtwidth=fm.boundingRect("X").width()*parentMathText->getSqrtWidthXFactor();
    const double newAscent=qMax(baselineHeight*parentMathText->getSqrtHeightFactor(), fm.ascent());
    const double newDescent=qMax(descent*parentMathText->getSqrtHeightFactor(), fm.descent());

    overallHeight=newAscent+newDescent;;
    baselineHeight=newAscent;
    width=width+sqrtwidth;
    if (degree!=2) {
        const QString degreetext=QLocale::c().toString(degree);
        const double degwidth=fmsmall.width(degreetext);
        const double smalltextIndent=0.6*sqrtwidth;
        if (degwidth>smalltextIndent) width=width+(degwidth-smalltextIndent);
    }
}

double JKQTMathTextSqrtNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);

    const QFont f=currentEv.getFont(parentMathText);
    QFont fsmall=f;
    const QFontMetricsF fm(f, painter.device());
    fsmall.setPointSizeF(fsmall.pointSizeF()*parentMathText->getSqrtSmallFontFactor());
    fsmall.setItalic(false);
    const QFontMetricsF fmsmall(fsmall, painter.device());

    double width=0, baselineHeight=0, overallHeight=0, strikeoutPos=0;
    getChild()->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
    const double descent=overallHeight-baselineHeight;
    const double sqrtwidth=fm.boundingRect("X").width()*parentMathText->getSqrtWidthXFactor();
    const double newAscent=qMax(baselineHeight*parentMathText->getSqrtHeightFactor(), fm.ascent());
    const double newDescent=qMax(descent*parentMathText->getSqrtHeightFactor(), fm.descent());
    const double linewidth=fm.lineWidth();
    const double tinyhookSize=sqrtwidth*0.1;
    const double smalltextIndent=0.6*sqrtwidth;
    const QString degreetext=(degree!=2)?QLocale::c().toString(degree):"";
    const double degwidth=fmsmall.width(degreetext);
    const double degheight=fmsmall.boundingRect(degreetext).height();
    const double degree_overwidth=(degwidth>smalltextIndent)?(degwidth-smalltextIndent):0.0;

    //painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(currentEv.color);
    p.setWidthF(linewidth);
    p.setCapStyle(Qt::RoundCap);
    p.setJoinStyle(Qt::RoundJoin);
    //painter.setPen(p);

    double xnew=getChild()->draw(painter, x+sqrtwidth+degree_overwidth, y, currentEv);

    const bool useAltForm=overallHeight>4.0*sqrtwidth;
    const double y_tinyhooktop=y-strikeoutPos;
    const double y_bottom=y+newDescent-linewidth/2.0;
    const double y_top=y-newAscent+linewidth/2.0;
    const double x_start=x+degree_overwidth+linewidth/2.0;
    const double x_tinyhooktop=x_start+tinyhookSize;
    const double x_hookbottom=(!useAltForm)?(x_start+0.33*sqrtwidth):(x_start+0.66*sqrtwidth);
    const double x_hooktop=(!useAltForm)?(x_start+sqrtwidth):x_hookbottom;
    const double x_smalltextend=x_start+smalltextIndent;
    const double y_smalltext=y_top+fmsmall.ascent()+(fabs(y_top-(y_tinyhooktop-linewidth))-degheight)/2.0;
    if (sqrtwidth>0) {
        QPainterPath path;
        path.moveTo(x_start, y_tinyhooktop+tinyhookSize);
        path.lineTo(x_tinyhooktop, y_tinyhooktop);
        path.lineTo(x_hookbottom, y_bottom);
        path.lineTo(x_hooktop, y_top);
        path.lineTo(xnew-linewidth/2.0, y_top);
        path.moveTo(x_tinyhooktop,y_tinyhooktop);
        path.lineTo(x_tinyhooktop+linewidth*0.8, y_tinyhooktop-linewidth*0.8);
        path.lineTo(x_hookbottom, y_bottom-2.0*linewidth);
        painter.setPen(p);
        painter.drawPath(path);
    }
    if (degree!=2) {
        painter.setFont(fsmall);
        painter.drawText(QPointF(x_smalltextend-degwidth, y_smalltext), degreetext);
    }

    return xnew;
}

bool JKQTMathTextSqrtNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    html=html+"&radic;<span style=\"text-decoration:overline\">";
    bool ok=getChild()->toHtml(html, currentEv, defaultEv);
    html=html+"&nbsp;</span>";
    return ok;
}

QString JKQTMathTextSqrtNode::getTypeName() const
{
    return "MTsqrtNode";
}

int JKQTMathTextSqrtNode::getDegree() const {
    return this->degree;
}


