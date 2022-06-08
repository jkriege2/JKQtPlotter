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



#include "jkqtmathtext/nodes/jkqtmathtextbracenode.h"
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
#include <QPainterPath>




JKQTMathTextBraceNode::JKQTMathTextBraceNode(JKQTMathText* _parent, const QString& openbrace, const QString& closebrace, JKQTMathTextNode* child, bool showRightBrace):
    JKQTMathTextSingleChildNode(child, _parent)
{
    this->openbrace=openbrace;
    this->closebrace=closebrace;
    this->showRightBrace=showRightBrace;
}


JKQTMathTextBraceNode::~JKQTMathTextBraceNode() {
}

void JKQTMathTextBraceNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {

    const JKQTMathTextEnvironment ev=currentEv;
    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    double bracewidth=0, braceheight=0;
    getBraceWidth(painter, ev, baselineHeight, overallHeight, bracewidth, braceheight);

    bracewidth=bracewidth/parentMathText->getBraceShrinkFactor();

    baselineHeight=/*qMin(baselineHeight, braceheight)*/ baselineHeight*parentMathText->getBraceFactor();
    overallHeight=qMax(overallHeight, braceheight)*parentMathText->getBraceFactor(); //fm.height();

    width=width+bracewidth*2.0;


}

double JKQTMathTextBraceNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    //std::cout<<"drawing brace-node: '"<<openbrace.toStdString()<<"' ... '"<<closebrace.toStdString()<<"'\n";
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;

    double width=0;
    double baselineHeight=0;
    double overallHeight=0, strikeoutPos=0;

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    double bracewidth=0, braceheight=0;
    getBraceWidth(painter, ev, baselineHeight, overallHeight, bracewidth, braceheight);

    double cWidth=0;
    double cBaselineHeight=0;
    double cOverallHeight=0, cstrikeoutPos=0;

    getSize(painter, currentEv, cWidth, cBaselineHeight, cOverallHeight, cstrikeoutPos);

    const double lw=qMax(0.25,ceil(currentEv.fontSize/16.0));//fm.lineWidth();

    double xnew=x+lw;

    const QPen pold=painter.pen();
    QPen p=pold;
    p.setWidthF(lw);
    p.setColor(currentEv.color);
    painter.setPen(p);
    double brace_fraction=0.85;
    if (openbrace=="(") {
        QPainterPath path;
        const double y1=y+(cOverallHeight-cBaselineHeight);
        const double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.cubicTo(xnew, (y1+y2)/2.0+fabs(y1-y2)/6.0, xnew, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="[") {
        QPainterPath path;
        const double y1=y+(cOverallHeight-cBaselineHeight);
        const double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew+lw/2.0, y1);
        path.lineTo(xnew+lw/2.0, y2);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="{") {
        QPainterPath path=JKQTMathTextMakeHBracePath(0,0,cOverallHeight, bracewidth*brace_fraction);
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(xnew+bracewidth*(1.0-brace_fraction), y-cBaselineHeight+cOverallHeight/2.0);
        painter.rotate(90);
        painter.drawPath(path);

    } else if (openbrace=="_") {
        QPainterPath path;
        const double y1=y+(cOverallHeight-cBaselineHeight);
        const double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew, y1);
        path.lineTo(xnew, y2);
        painter.drawPath(path);
    } else if (openbrace=="~") {
        QPainterPath path;
        const double y1=y+(cOverallHeight-cBaselineHeight);
        const double y2=y-cBaselineHeight;
        path.moveTo(xnew, y1);
        path.lineTo(xnew, y2);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="|") {
        const double y1=y+(cOverallHeight-cBaselineHeight);
        const double y2=y-cBaselineHeight;
        QLineF l(xnew+brace_fraction*bracewidth, y1, xnew+brace_fraction*bracewidth, y2);
        if (l.length()>0) painter.drawLine(l);
    } else if (openbrace=="#" || openbrace=="||") {
        const double y1=y+(cOverallHeight-cBaselineHeight);
        const double y2=y-cBaselineHeight;
        QLineF l(xnew+brace_fraction*bracewidth, y1, xnew+brace_fraction*bracewidth, y2);
        if (l.length()>0) painter.drawLine(l);
        l=QLineF(xnew+brace_fraction*bracewidth-1.5*lw, y1, xnew+brace_fraction*bracewidth-1.5*lw, y2);
        if (l.length()>0) painter.drawLine(l);
    } else if (openbrace=="<") {
        QPainterPath path;
        const double y1=y+(cOverallHeight-cBaselineHeight);
        const double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew, (y2+y1)/2.0);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    }

    painter.setPen(pold);

    xnew= child->draw(painter, xnew+bracewidth/parentMathText->getBraceShrinkFactor()-lw, y, currentEv)+lw;

    if (showRightBrace) {
        painter.setPen(p);
        if (closebrace==")") {
            QPainterPath path;
            const double y1=y+(cOverallHeight-cBaselineHeight);
            const double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.cubicTo(xnew+bracewidth, (y1+y2)/2.0+fabs(y1-y2)/6.0, xnew+bracewidth, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="]") {
            QPainterPath path;
            const double y1=y+(cOverallHeight-cBaselineHeight);
            const double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth-lw/2.0, y1);
            path.lineTo(xnew+bracewidth-lw/2.0, y2);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="}") {
            QPainterPath path=JKQTMathTextMakeHBracePath(0,0,cOverallHeight, bracewidth*brace_fraction);
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(xnew+bracewidth*brace_fraction, y-cBaselineHeight+cOverallHeight/2.0);
            painter.rotate(270);
            painter.drawPath(path);

        } else if (closebrace=="_") {
            QPainterPath path;
            const double y1=y+(cOverallHeight-cBaselineHeight);
            const double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth, y1);
            path.lineTo(xnew+bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="~") {
            QPainterPath path;
            const double y1=y+(cOverallHeight-cBaselineHeight);
            const double y2=y-cBaselineHeight;
            path.moveTo(xnew+bracewidth, y1);
            path.lineTo(xnew+bracewidth, y2);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="|") {
            const double y1=y+(cOverallHeight-cBaselineHeight);
            const double y2=y-cBaselineHeight;
            QLineF l(xnew+(1.0-brace_fraction)*bracewidth, y1, xnew+(1.0-brace_fraction)*bracewidth, y2);
            if (l.length()>0) painter.drawLine(l);
        } else if (closebrace=="#" || closebrace=="||") {
            const double y1=y+(cOverallHeight-cBaselineHeight);
            const double y2=y-cBaselineHeight;
            QLineF l(xnew+(1.0-brace_fraction)*bracewidth, y1, xnew+(1.0-brace_fraction)*bracewidth, y2);
            if (l.length()>0) painter.drawLine(l);
            l=QLineF(xnew+(1.0-brace_fraction)*bracewidth+1.5*lw, y1, xnew+(1.0-brace_fraction)*bracewidth+1.5*lw, y2);
            if (l.length()>0) painter.drawLine(l);
        } else if (closebrace==">") {
            QPainterPath path;
            const double y1=y+(cOverallHeight-cBaselineHeight);
            const double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth, (y2+y1)/2.0);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        }
        painter.setPen(pold);
    }

    //qDebug()<<" ==> "<<bc<<fm.boundingRect(bc).width();
    return xnew+bracewidth/parentMathText->getBraceShrinkFactor()-lw;
}

bool JKQTMathTextBraceNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    QString ob=openbrace;
    QString cb=closebrace;
    if (ob=="<") ob="&lang;";
    else if (ob=="_") ob="&lfloor;";
    else if (ob=="~") ob="&lceil;";
    else if (ob=="||" || ob=="#") ob="||";
    if (cb=="<") cb="&rang;";
    else if (cb=="_") cb="&rfloor;";
    else if (cb=="~") cb="&rceil;";
    else if (cb=="||" || cb=="#") cb="||";


    html=html+ob;

    bool ok=child->toHtml(html, currentEv, defaultEv);

    html=html+cb;

    return ok;
}

QString JKQTMathTextBraceNode::getTypeName() const
{
    return QLatin1String("MTbraceNode(")+openbrace+" "+closebrace+")";
}

QString JKQTMathTextBraceNode::getOpenbrace() const {
    return this->openbrace;
}

QString JKQTMathTextBraceNode::getClosebrace() const {
    return this->closebrace;
}

bool JKQTMathTextBraceNode::getShowRightBrace() const {
    return this->showRightBrace;
}

void JKQTMathTextBraceNode::getBraceWidth(QPainter &/*painter*/, JKQTMathTextEnvironment ev, double /*baselineHeight*/, double overallHeight, double &bracewidth, double &braceheight) const
{
    /*QFont evf=ev.getFont(parent);
    if (ev.insideMath) evf.setItalic(false);
    ev.italic=false;
    while (ev.fontSize<10*parent->getFontSize()) {
        const QFontMetricsF fme(evf, painter.device());
        if (fme.ascent()>overallHeight) break;
        ev.fontSize+=0.5;
        evf.setPointSizeF(ev.fontSize);
    }
    ev.fontSize=ev.fontSize*parent->getBraceFactor();
    evf.setPointSizeF(ev.fontSize);
    QFontMetricsF fm(evf, painter.device());
    QString bc="_X";
    bracewidth=fm.width("I")*parent->getBraceShrinkFactor();
    braceheight=parent->getTBR(evf, bc, painter.device()).height();*/
    double lw=qMax(0.25,ceil(ev.fontSize/12.0));
    braceheight=overallHeight*parentMathText->getBraceFactor();
    bracewidth=0.6*pow(braceheight, 0.6);
    if (openbrace=="{" || closebrace=="}")  bracewidth=qMax(bracewidth, lw*3.5);

}

