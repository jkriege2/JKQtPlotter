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




QString JKQTMathTextBraceNode::BraceType2String(BraceType type) {
    switch(type) {
    case MTBTAngleBracket:
        return "angle_bracket";
    case MTBTSquareBracket:
        return "square_bracket";
    case MTBTCeilBracket:
        return "ceil_bracket";
    case MTBTCurlyBracket:
        return "curly_bracket";
    case MTBTDoubleLine:
        return "double_line";
    case MTBTFloorBracket:
        return "floor_bracket";
    case MTBTParenthesis:
        return "parenhesis";
    case MTBTSingleLine:
        return "single_line";
    }
    return "unknown";
}

JKQTMathTextBraceNode::BraceType JKQTMathTextBraceNode::TokenNameString2TokenType(const QString &type)
{
    if (type=="(" || type==")") return MTBTParenthesis;
    if (type=="[" || type=="]") return MTBTSquareBracket;
    if (type=="{" || type=="}") return MTBTCurlyBracket;
    if (type=="|") return MTBTSingleLine;
    if (type=="||" || type=="#") return MTBTDoubleLine;
    if (type=="<" || type==">" || type=="langle" || type=="rangle") return MTBTAngleBracket;
    if (type=="_" || type=="lfloor" || type=="rfloor") return MTBTFloorBracket;
    if (type=="~" || type=="lceil" || type=="rceil") return MTBTCeilBracket;
    return MTBTNone;
}

JKQTMathTextBraceNode::JKQTMathTextBraceNode(JKQTMathText* _parent, JKQTMathTextBraceNode::BraceType openbrace, JKQTMathTextBraceNode::BraceType closebrace, JKQTMathTextNode* child, bool showOpeningBrace, bool showClosingBrace):
    JKQTMathTextSingleChildNode(child, _parent)
{
    this->openbrace=openbrace;
    this->closebrace=closebrace;
    this->showClosingBrace=showClosingBrace;
    this->showOpeningBrace=showOpeningBrace;
}


JKQTMathTextBraceNode::~JKQTMathTextBraceNode() {
}

void JKQTMathTextBraceNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    double braceWidth=0, braceHeight=0;
    getSizeInternalAndBrace(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos, braceWidth, braceHeight, prevNodeSize);
}

void JKQTMathTextBraceNode::getSizeInternalAndBrace(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, double &bracewidth, double &braceheight, const JKQTMathTextNodeSize */*prevNodeSize*/)
{

    const JKQTMathTextEnvironment ev=currentEv;
    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    const double cAscentAboveStrike=baselineHeight-strikeoutPos;
    const double cDescentBelowStrike=overallHeight-baselineHeight+strikeoutPos;

    //qDebug()<<"getSizeInternalAndBrace(): showOpeningBrace="<<showOpeningBrace<<", openbrace="<<openbrace<<", showClosingBrace="<<showClosingBrace<<", closebrace="<<closebrace;
    //qDebug()<<"getSizeInternalAndBrace(): child: baselineHeight="<<baselineHeight<<", strikeoutPos="<<strikeoutPos<<", overallHeight="<<overallHeight;
    //qDebug()<<"getSizeInternalAndBrace(): child: cAscentAboveStrike="<<cAscentAboveStrike<<", cDescentBelowStrike="<<cDescentBelowStrike;

    const double heightAboveBelowStrike=qMax(cAscentAboveStrike, cDescentBelowStrike);


    baselineHeight=strikeoutPos+heightAboveBelowStrike*parentMathText->getBraceFactor();
    overallHeight=2.0*heightAboveBelowStrike*parentMathText->getBraceFactor(); //fm.height();

    //qDebug()<<"getSizeInternalAndBrace(): heightAboveBelowStrike="<<heightAboveBelowStrike<<", baselineHeight="<<baselineHeight<<", overallHeight="<<overallHeight;

    bracewidth=0;
    braceheight=0;
    getBraceSize(painter, ev, baselineHeight, overallHeight, bracewidth, braceheight);

    bracewidth=bracewidth/parentMathText->getBraceShrinkFactor();

    if (showOpeningBrace) width+=bracewidth;
    if (showClosingBrace) width+=bracewidth;
}

double JKQTMathTextBraceNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    //std::cout<<"drawing brace-node: '"<<openbrace.toStdString()<<"' ... '"<<closebrace.toStdString()<<"'\n";
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;

    double nodeWidth=0;
    double nodeBaselineHeight=0;
    double nodeOverallHeight=0, nodeStrikeoutPos=0;
    double bracewidth=0, braceheight=0;
    getSizeInternalAndBrace(painter, currentEv, nodeWidth, nodeBaselineHeight, nodeOverallHeight, nodeStrikeoutPos, bracewidth, braceheight);


    const double lw=qMax(0.25,ceil(currentEv.fontSize/16.0));//fm.lineWidth();

    double xnew=x;

    const QPen pold=painter.pen();
    QPen p=pold;
    p.setWidthF(lw);
    p.setColor(currentEv.color);
    painter.setPen(p);
    const double paren_fraction=0.85;
    const double brace_fraction=0.65;
    if (showOpeningBrace) {
        const double xbrace1=xnew+lw;
        const double xbrace2=qMin(xnew+paren_fraction*bracewidth, xnew+bracewidth-lw/2.0);
        const double xbrace2s=qMin(xnew+brace_fraction*bracewidth, xnew+bracewidth-lw/2.0);
        if (openbrace==MTBTParenthesis) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace2, y1);
            path.cubicTo(xbrace1, (y1+y2)/2.0+fabs(y1-y2)/6.0, xbrace1, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xbrace2, y2);
            painter.drawPath(path);
        } else if (openbrace==JKQTMathTextBraceNode::MTBTSquareBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace2s, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            path.lineTo(xbrace2s, y2);
            painter.drawPath(path);
        } else if (openbrace==JKQTMathTextBraceNode::MTBTCurlyBracket) {
            QPainterPath path=JKQTMathTextMakeHBracePath(0,0,nodeOverallHeight, bracewidth*brace_fraction);
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate((xbrace1+xbrace2)/2.0, y-nodeBaselineHeight+nodeOverallHeight/2.0);
            painter.rotate(90);
            painter.drawPath(path);

        } else if (openbrace==JKQTMathTextBraceNode::MTBTFloorBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace2s, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        } else if (openbrace==JKQTMathTextBraceNode::MTBTCeilBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            path.lineTo(xbrace2s, y2);
            painter.drawPath(path);
        } else if (openbrace==JKQTMathTextBraceNode::MTBTSingleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QLineF l(xbrace1, y1, xbrace1, y2);
            if (l.length()>0) painter.drawLine(l);
        } else if (openbrace==JKQTMathTextBraceNode::MTBTDoubleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QLineF l(xbrace1, y1, xbrace1, y2);
            if (l.length()>0) painter.drawLine(l);
            const QLineF l2(xbrace1+1.5*lw, y1, xbrace1+1.5*lw, y2);
            if (l2.length()>0) painter.drawLine(l2);
        } else if (openbrace==JKQTMathTextBraceNode::MTBTAngleBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, (y2+y1)/2.0);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        }
        xnew=xnew+bracewidth;
    }

    painter.setPen(pold);

    xnew= child->draw(painter, xnew, y, currentEv);

    if (showClosingBrace) {
        const double xbrace1=qMax(xnew+bracewidth-paren_fraction*bracewidth, xnew+lw/2.0);
        const double xbrace1s=qMax(xnew+bracewidth-brace_fraction*bracewidth, xnew+lw/2.0);
        const double xbrace2=xnew+bracewidth-lw;
        painter.setPen(p);
        if (closebrace==JKQTMathTextBraceNode::MTBTParenthesis) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace1, y1);
            path.cubicTo(xbrace2, (y1+y2)/2.0+fabs(y1-y2)/6.0, xbrace2, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xbrace1, y2);
            painter.drawPath(path);
        } else if (closebrace==JKQTMathTextBraceNode::MTBTSquareBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace1s, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            path.lineTo(xbrace1s, y2);
            painter.drawPath(path);
        } else if (closebrace==JKQTMathTextBraceNode::MTBTCurlyBracket) {
            QPainterPath path=JKQTMathTextMakeHBracePath(0,0,nodeOverallHeight, bracewidth*brace_fraction);
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate((xbrace1+xbrace2)/2.0, y-nodeBaselineHeight+nodeOverallHeight/2.0);
            painter.rotate(270);
            painter.drawPath(path);

        } else if (closebrace==JKQTMathTextBraceNode::MTBTFloorBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace1s, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        } else if (closebrace==JKQTMathTextBraceNode::MTBTCeilBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            path.lineTo(xbrace1s, y2);
            painter.drawPath(path);
        } else if (closebrace==JKQTMathTextBraceNode::MTBTSingleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QLineF l(xbrace2, y1, xbrace2, y2);
            if (l.length()>0) painter.drawLine(l);
        } else if (closebrace==JKQTMathTextBraceNode::MTBTDoubleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QLineF l(xbrace2, y1, xbrace2, y2);
            if (l.length()>0) painter.drawLine(l);
            const QLineF l2(xbrace2-1.5*lw, y1, xbrace2-1.5*lw, y2);
            if (l2.length()>0) painter.drawLine(l2);
        } else if (closebrace==JKQTMathTextBraceNode::MTBTAngleBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, (y2+y1)/2.0);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        }
        painter.setPen(pold);
        xnew=xnew+bracewidth;
    }

    //qDebug()<<" ==> "<<bc<<fm.boundingRect(bc).width();
    return xnew;
}

bool JKQTMathTextBraceNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    QString ob;
    QString cb;
    if (openbrace==JKQTMathTextBraceNode::MTBTAngleBracket) ob="&lang;";
    else if (openbrace==JKQTMathTextBraceNode::MTBTFloorBracket) ob="&lfloor;";
    else if (openbrace==JKQTMathTextBraceNode::MTBTCeilBracket) ob="&lceil;";
    else if (openbrace==JKQTMathTextBraceNode::MTBTParenthesis) ob="(";
    else if (openbrace==JKQTMathTextBraceNode::MTBTSquareBracket) ob="[";
    else if (openbrace==JKQTMathTextBraceNode::MTBTCurlyBracket) ob="{";
    else if (openbrace==JKQTMathTextBraceNode::MTBTSingleLine) ob="|";
    else if (openbrace==JKQTMathTextBraceNode::MTBTDoubleLine) ob="||";
    if (closebrace==JKQTMathTextBraceNode::MTBTAngleBracket) cb="&rang;";
    else if (closebrace==JKQTMathTextBraceNode::MTBTFloorBracket) cb="&rfloor;";
    else if (closebrace==JKQTMathTextBraceNode::MTBTCeilBracket) cb="&rceil;";
    else if (closebrace==JKQTMathTextBraceNode::MTBTParenthesis) cb=")";
    else if (closebrace==JKQTMathTextBraceNode::MTBTSquareBracket) cb="]";
    else if (closebrace==JKQTMathTextBraceNode::MTBTCurlyBracket) cb="}";
    else if (closebrace==JKQTMathTextBraceNode::MTBTSingleLine) cb="|";
    else if (closebrace==JKQTMathTextBraceNode::MTBTDoubleLine) cb="||";


    html=html+ob;

    bool ok=child->toHtml(html, currentEv, defaultEv);

    html=html+cb;

    return ok;
}

QString JKQTMathTextBraceNode::getTypeName() const
{
    return QLatin1String("MTbraceNode(")+BraceType2String(openbrace)+" "+BraceType2String(closebrace)+")";
}

JKQTMathTextBraceNode::BraceType JKQTMathTextBraceNode::getOpenbrace() const {
    return this->openbrace;
}

JKQTMathTextBraceNode::BraceType JKQTMathTextBraceNode::getClosebrace() const {
    return this->closebrace;
}

bool JKQTMathTextBraceNode::getShowClosingBrace() const {
    return this->showClosingBrace;
}

bool JKQTMathTextBraceNode::getShowOpeningBrace() const
{
    return showOpeningBrace;
}

void JKQTMathTextBraceNode::getBraceSize(QPainter &/*painter*/, JKQTMathTextEnvironment ev, double /*baselineHeight*/, double overallHeight, double &bracewidth, double &braceheight) const
{
    const double lw=qMax(0.25,ceil(ev.fontSize/12.0));
    braceheight=overallHeight*parentMathText->getBraceFactor();
    bracewidth=0.6*pow(braceheight, 0.6);
    if (openbrace==JKQTMathTextBraceNode::MTBTCurlyBracket || closebrace==JKQTMathTextBraceNode::MTBTCurlyBracket)  bracewidth=qMax(bracewidth, lw*3.5);

}

