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
    QFontMetricsF fm(currentEv.getFont(parentMathText), painter.device());

    getChild()->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    overallHeight=overallHeight*1.2;//+fm.ascent()*0.1;
    baselineHeight=baselineHeight*1.2;//+fm.ascent()*0.1;
    width=width+fm.boundingRect("A").width()*2; // 1.53
}

double JKQTMathTextSqrtNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0, baselineHeight=0, overallHeight=0, sp=0;
    getChild()->getSize(painter, currentEv, width, baselineHeight, overallHeight, sp);
    QFont f=currentEv.getFont(parentMathText);
    QFont fsmall=f;
    QFontMetricsF fm(f, painter.device());
    double w=fm.boundingRect("A").width();
    double a=baselineHeight*1.15;
    double d=overallHeight-baselineHeight;
    //painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(currentEv.color);
    p.setWidthF(fm.lineWidth());
    //painter.setPen(p);
    QPainterPath path;
    if (w>0) {
        path.moveTo(x+0.1*w, y-0.4*a);
        path.lineTo(x+0.33*w, y-0.4*a);
        path.lineTo( x+0.66*w, y+0.5*d);
        path.lineTo(x+w, y-a);
    }
    if (degree!=2) {
        fsmall.setPointSizeF(fsmall.pointSizeF()/2.0);
        fsmall.setItalic(false);
        painter.setFont(fsmall);
        painter.drawText(QPointF(x+0.33*w, y-0.55*a), QLocale::c().toString(degree));
    }
    //painter.restore();
    double xnew=getChild()->draw(painter, x+1.2*w, y, currentEv);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(p);
    if (w>0) {
        path.lineTo( xnew+0.2*w, y-a);
        path.lineTo(xnew+0.2*w, y-0.8*a);
        painter.drawPath(path);
    }

    return xnew+0.33*w;
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


