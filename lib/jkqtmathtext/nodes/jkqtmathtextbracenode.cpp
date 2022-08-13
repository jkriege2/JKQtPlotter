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




JKQTMathTextBraceNode::JKQTMathTextBraceNode(JKQTMathText* _parent, JKQTMathTextBraceType openbrace, JKQTMathTextBraceType closebrace, JKQTMathTextNode* child):
    JKQTMathTextSingleChildNode(child, _parent)
{
    this->openbrace=openbrace;
    this->closebrace=closebrace;
}


JKQTMathTextBraceNode::~JKQTMathTextBraceNode() {
}

void JKQTMathTextBraceNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    double braceWidth=0, braceHeight=0;
    getSizeInternalAndBrace(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos, braceWidth, braceHeight, prevNodeSize);
}

void JKQTMathTextBraceNode::getSizeInternalAndBrace(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, double &bracewidth, double &braceheight, const JKQTMathTextNodeSize */*prevNodeSize*/)
{
    width=0;
    baselineHeight=0;
    overallHeight=0;
    strikeoutPos=0;
    getChild()->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
    const QFontMetricsF fm(currentEv.getFont(parentMathText), painter.device());
    const double minChildHeight=fm.tightBoundingRect("l").height();

    double cAscentAboveStrike=0;
    double cDescentBelowStrike=0;
    cAscentAboveStrike=qMax(minChildHeight-strikeoutPos, baselineHeight-strikeoutPos);
    cDescentBelowStrike=qMax(strikeoutPos, overallHeight-baselineHeight+strikeoutPos);

    //qDebug()<<"getSizeInternalAndBrace(): showOpeningBrace="<<showOpeningBrace<<", openbrace="<<openbrace<<", showClosingBrace="<<showClosingBrace<<", closebrace="<<closebrace;
    //qDebug()<<"getSizeInternalAndBrace(): child: baselineHeight="<<baselineHeight<<", strikeoutPos="<<strikeoutPos<<", overallHeight="<<overallHeight;
    //qDebug()<<"getSizeInternalAndBrace(): child: cAscentAboveStrike="<<cAscentAboveStrike<<", cDescentBelowStrike="<<cDescentBelowStrike;

    const double heightAboveBelowStrike=qMax(cAscentAboveStrike, cDescentBelowStrike);


    baselineHeight=strikeoutPos+heightAboveBelowStrike*parentMathText->getBraceFactor();
    overallHeight=2.0*heightAboveBelowStrike*parentMathText->getBraceFactor(); //fm.height();

    //qDebug()<<"getSizeInternalAndBrace(): heightAboveBelowStrike="<<heightAboveBelowStrike<<", baselineHeight="<<baselineHeight<<", overallHeight="<<overallHeight;

    bracewidth=0;
    braceheight=0;
    getBraceSize(painter, currentEv, baselineHeight, overallHeight, bracewidth, braceheight);

    if (openbrace!=MTBTNone && openbrace!=MTBTAny) width+=bracewidth;
    if (closebrace!=MTBTNone && closebrace!=MTBTAny) width+=bracewidth;
}

double JKQTMathTextBraceNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    //std::cout<<"drawing brace-node: '"<<openbrace.toStdString()<<"' ... '"<<closebrace.toStdString()<<"'\n";
    doDrawBoxes(painter, x, y, currentEv);

    double nodeWidth=0;
    double nodeBaselineHeight=0;
    double nodeOverallHeight=0, nodeStrikeoutPos=0;
    double bracewidth=0, braceheight=0;
    getSizeInternalAndBrace(painter, currentEv, nodeWidth, nodeBaselineHeight, nodeOverallHeight, nodeStrikeoutPos, bracewidth, braceheight);
    const QFontMetricsF fm(currentEv.getFont(parentMathText));

    const double lw=fm.lineWidth();

    double xnew=x;

    const QPen pold=painter.pen();
    QPen p=pold;
    p.setWidthF(lw);
    p.setColor(currentEv.color);
    p.setCapStyle(Qt::FlatCap);
    p.setJoinStyle(Qt::MiterJoin);
    painter.setPen(p);
    const double paren_fraction=0.75;
    const double absnorm_linewidth_factor=0.75;
    const double paren_topwidth=lw*0.75;
    const double paren_centerwidth=lw*1.75;
    const double angle_centerwidth=lw*1.55;
    {
        bool showOpeningBrace=true;
        const double xbrace1=xnew+lw;
        const double xbrace2=xnew+qMin(paren_fraction*bracewidth, bracewidth-lw/2.0);
        const double xbraceC=xnew+bracewidth/2.0;
        if (openbrace==MTBTParenthesis) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QPointF pb1(xbrace2-paren_topwidth/2.0, y1);
            const QPointF pbc1(xbrace1-paren_centerwidth/2.0, (y1+y2)/2.0+fabs(y1-y2)/6.0);
            const QPointF ptc1(xbrace1-paren_centerwidth/2.0, (y1+y2)/2.0-fabs(y1-y2)/6.0);
            const QPointF pt1(xbrace2-paren_topwidth/2.0, y2);
            const QPointF pt2(xbrace2+paren_topwidth/2.0, y2);
            const QPointF ptc2(xbrace1+paren_centerwidth/2.0, (y1+y2)/2.0-fabs(y1-y2)/6.0);
            const QPointF pbc2(xbrace1+paren_centerwidth/2.0, (y1+y2)/2.0+fabs(y1-y2)/6.0);
            const QPointF pb2(xbrace2+paren_topwidth/2.0, y1);
            path.moveTo(pb1);
            path.cubicTo(pbc1, ptc1, pt1);
            path.lineTo(pt2);
            path.cubicTo(ptc2, pbc2, pb2);
            path.closeSubpath();
            painter.fillPath(path, QBrush(currentEv.color, Qt::SolidPattern));
            /*painter.setPen("blue");
            painter.drawLine(pb1,pbc1);
            painter.drawLine(pt1,ptc1);
            painter.drawLine(pb2,pbc2);
            painter.drawLine(pt2,ptc2);*/
        } else if (openbrace==MTBTAngleBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const double yc=(y1+y2)/2.0;
            const QPointF pb1(xbrace2-paren_topwidth/2.0, y1);
            const QPointF pc1(xbrace1-angle_centerwidth/2.0, yc);
            const QPointF pt1(xbrace2-paren_topwidth/2.0, y2);
            const QPointF pt2(xbrace2+paren_topwidth/2.0, y2);
            const QPointF pc2(xbrace1+angle_centerwidth/2.0, yc);
            const QPointF pb2(xbrace2+paren_topwidth/2.0, y1);
            path.moveTo(pb1);
            path.lineTo(pc1);
            path.lineTo(pt1);
            path.lineTo(pt2);
            path.lineTo(pc2);
            path.lineTo(pb2);
            path.closeSubpath();
            painter.fillPath(path, QBrush(currentEv.color, Qt::SolidPattern));
        } else if (openbrace==MTBTSquareBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight)-lw/2.0;
            const double y2=y-nodeBaselineHeight+lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        } else if (openbrace==MTBTTopCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace2-xbrace1);
            const double y1=y-nodeBaselineHeight+lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y1+dx);
            painter.drawPath(path);
        } else if (openbrace==MTBTBottomCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace2-xbrace1);
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight)-lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y1-dx);
            painter.drawPath(path);
        } else if (openbrace==MTBTCurlyBracket) {
            const QPainterPath path=JKQTMathTextMakeHBracePath(0,0,nodeOverallHeight, bracewidth*paren_fraction, p.widthF());
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(xbraceC, y-nodeBaselineHeight+nodeOverallHeight/2.0);
            painter.rotate(90);
            QPen plocal=p;
            plocal.setWidthF(0.0001);
            painter.fillPath(path, QBrush(currentEv.color));
        } else if (openbrace==MTBTFloorBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight)-lw/2.0;
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        } else if (openbrace==MTBTCeilBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight+lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        } else if (openbrace==MTBTSingleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QLineF l(xbraceC, y1, xbraceC, y2);
            QPen plocal=p;
            plocal.setWidthF(plocal.widthF()*absnorm_linewidth_factor);
            painter.setPen(plocal);
            if (l.length()>0) painter.drawLine(l);
            painter.setPen(p);
        } else if (openbrace==MTBTDoubleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            QPen plocal=p;
            plocal.setWidthF(plocal.widthF()*absnorm_linewidth_factor);
            painter.setPen(plocal);
            const QLineF l(xbraceC-lw, y1, xbraceC-lw, y2);
            if (l.length()>0) painter.drawLine(l);
            const QLineF l2(xbraceC+lw, y1, xbraceC+lw, y2);
            if (l2.length()>0) painter.drawLine(l2);
            painter.setPen(p);
        } else {
            showOpeningBrace=false;
        }
        if (showOpeningBrace) {
            xnew=xnew+bracewidth;
        }
    }

    painter.setPen(pold);

    xnew= getChild()->draw(painter, xnew, y, currentEv);

    {
        bool showClosingBrace=true;
        const double xbrace1=qMax(xnew+bracewidth-paren_fraction*bracewidth, xnew+lw/2.0);
        const double xbrace2=xnew+bracewidth-lw;
        const double xbraceC=xnew+bracewidth/2.0;
        painter.setPen(p);
        if (closebrace==MTBTParenthesis) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QPointF pb1(xbrace1-paren_topwidth/2.0, y1);
            const QPointF pbc1(xbrace2-paren_centerwidth/2.0, (y1+y2)/2.0+fabs(y1-y2)/6.0);
            const QPointF ptc1(xbrace2-paren_centerwidth/2.0, (y1+y2)/2.0-fabs(y1-y2)/6.0);
            const QPointF pt1(xbrace1-paren_topwidth/2.0, y2);
            const QPointF pt2(xbrace1+paren_topwidth/2.0, y2);
            const QPointF ptc2(xbrace2+paren_centerwidth/2.0, (y1+y2)/2.0-fabs(y1-y2)/6.0);
            const QPointF pbc2(xbrace2+paren_centerwidth/2.0, (y1+y2)/2.0+fabs(y1-y2)/6.0);
            const QPointF pb2(xbrace1+paren_topwidth/2.0, y1);
            path.moveTo(pb1);
            path.cubicTo(pbc1, ptc1, pt1);
            path.lineTo(pt2);
            path.cubicTo(ptc2, pbc2, pb2);
            path.closeSubpath();
            painter.fillPath(path, QBrush(currentEv.color, Qt::SolidPattern));
            /*painter.setPen("blue");
            painter.drawLine(pb1,pbc1);
            painter.drawLine(pt1,ptc1);
            painter.drawLine(pb2,pbc2);
            painter.drawLine(pt2,ptc2);*/
        } else if (closebrace==MTBTAngleBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const double yc=(y1+y2)/2.0;
            const QPointF pb1(xbrace1-paren_topwidth/2.0, y1);
            const QPointF pc1(xbrace2-angle_centerwidth/2.0, yc);
            const QPointF pt1(xbrace1-paren_topwidth/2.0, y2);
            const QPointF pt2(xbrace1+paren_topwidth/2.0, y2);
            const QPointF pc2(xbrace2+angle_centerwidth/2.0, yc);
            const QPointF pb2(xbrace1+paren_topwidth/2.0, y1);
            path.moveTo(pb1);
            path.lineTo(pc1);
            path.lineTo(pt1);
            path.lineTo(pt2);
            path.lineTo(pc2);
            path.lineTo(pb2);
            path.closeSubpath();
            painter.fillPath(path, QBrush(currentEv.color, Qt::SolidPattern));
        } else if (closebrace==MTBTSquareBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight)-lw/2.0;
            const double y2=y-nodeBaselineHeight+lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        } else if (closebrace==MTBTBottomCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace1-xbrace2);
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight)-lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y1-dx);
            painter.drawPath(path);
        } else if (closebrace==MTBTTopCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace1-xbrace2);
            const double y1=y-nodeBaselineHeight+lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y1+dx);
            painter.drawPath(path);
        } else if (closebrace==MTBTCurlyBracket) {
            const QPainterPath path=JKQTMathTextMakeHBracePath(0,0,nodeOverallHeight, bracewidth*paren_fraction, p.widthF());
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(xbraceC, y-nodeBaselineHeight+nodeOverallHeight/2.0);
            painter.rotate(270);
            QPen plocal=p;
            plocal.setWidthF(0.0001);
            painter.fillPath(path, QBrush(currentEv.color));
        } else if (closebrace==MTBTFloorBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight)-lw/2.0;
            const double y2=y-nodeBaselineHeight;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        } else if (closebrace==MTBTCeilBracket) {
            QPainterPath path;
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight+lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        } else if (closebrace==MTBTSingleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QLineF l(xbraceC, y1, xbraceC, y2);
            QPen plocal=p;
            plocal.setWidthF(plocal.widthF()*absnorm_linewidth_factor);
            painter.setPen(plocal);
            if (l.length()>0) painter.drawLine(l);
            painter.setPen(p);
        } else if (closebrace==MTBTDoubleLine) {
            const double y1=y+(nodeOverallHeight-nodeBaselineHeight);
            const double y2=y-nodeBaselineHeight;
            const QLineF l(xbraceC-lw, y1, xbraceC-lw, y2);
            QPen plocal=p;
            plocal.setWidthF(plocal.widthF()*absnorm_linewidth_factor);
            painter.setPen(plocal);
            if (l.length()>0) painter.drawLine(l);
            const QLineF l2(xbraceC+lw, y1, xbraceC+lw, y2);
            if (l2.length()>0) painter.drawLine(l2);
            painter.setPen(p);
        } else {
            showClosingBrace=false;
        }
        painter.setPen(pold);
        if (showClosingBrace) {
            xnew=xnew+bracewidth;
        }
    }

    //qDebug()<<" ==> "<<bc<<fm.boundingRect(bc).width();
    return xnew;
}

bool JKQTMathTextBraceNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    QString ob;
    QString cb;
    if (openbrace==MTBTAngleBracket) ob="&lang;";
    else if (openbrace==MTBTFloorBracket) ob="&lfloor;";
    else if (openbrace==MTBTCeilBracket) ob="&lceil;";
    else if (openbrace==MTBTParenthesis) ob="(";
    else if (openbrace==MTBTSquareBracket) ob="[";
    else if (openbrace==MTBTCurlyBracket) ob="{";
    else if (openbrace==MTBTSingleLine) ob="|";
    else if (openbrace==MTBTDoubleLine) ob="||";
    else if (openbrace==MTBTTopCorner) ob="&ulcorner;";
    else if (openbrace==MTBTBottomCorner) ob="&llcorner;";
    if (closebrace==MTBTAngleBracket) cb="&rang;";
    else if (closebrace==MTBTFloorBracket) cb="&rfloor;";
    else if (closebrace==MTBTCeilBracket) cb="&rceil;";
    else if (closebrace==MTBTParenthesis) cb=")";
    else if (closebrace==MTBTSquareBracket) cb="]";
    else if (closebrace==MTBTCurlyBracket) cb="}";
    else if (closebrace==MTBTSingleLine) cb="|";
    else if (closebrace==MTBTDoubleLine) cb="||";
    else if (closebrace==MTBTTopCorner) ob="&urcorner;";
    else if (closebrace==MTBTBottomCorner) ob="&lrcorner;";


    html=html+ob;

    bool ok=getChild()->toHtml(html, currentEv, defaultEv);

    html=html+cb;

    return ok;
}

QString JKQTMathTextBraceNode::getTypeName() const
{
    return QLatin1String("MTbraceNode(")+JKQTMathTextBraceType2String(openbrace)+" "+JKQTMathTextBraceType2String(closebrace)+")";
}

JKQTMathTextBraceType JKQTMathTextBraceNode::getOpenbrace() const {
    return this->openbrace;
}

JKQTMathTextBraceType JKQTMathTextBraceNode::getClosebrace() const {
    return this->closebrace;
}

void JKQTMathTextBraceNode::getBraceSize(QPainter &painter, JKQTMathTextEnvironment ev, double /*baselineHeight*/, double overallHeight, double &bracewidth, double &braceheight) const
{
    const QFontMetricsF fm(ev.getFont(parentMathText), painter.device());
    const double lw=fm.lineWidth();
    const double dblline_distance=2.0*lw;
    braceheight=overallHeight*parentMathText->getBraceFactor();
    bracewidth=lw*5.0;
    if (openbrace==MTBTCurlyBracket || closebrace==MTBTCurlyBracket)  bracewidth=lw*6.5;
    if (openbrace==MTBTParenthesis || closebrace==MTBTParenthesis)  bracewidth=lw*6.0;
    if (openbrace==MTBTDoubleLine || closebrace==MTBTDoubleLine)  bracewidth=dblline_distance+3.0*lw;
    if (openbrace==MTBTSingleLine || closebrace==MTBTSingleLine)  bracewidth=3.0*lw;

    const double overSizeFactor=braceheight/fm.height();
    if (overSizeFactor>1.2) bracewidth=bracewidth*sqrt(overSizeFactor);
}

