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
#include "jkqtcommon/jkqtpdebuggingtools.h"
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

JKQTMathTextNodeSize JKQTMathTextBraceNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    return getSizeInternalAndBrace(painter, currentEv);
}

JKQTMathTextBraceNode::NodeSize JKQTMathTextBraceNode::getSizeInternalAndBrace(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
    NodeSize s;
    const NodeSize childSize=getChild()->getSize(painter, currentEv);
    const QFont f=currentEv.getFont(parentMathText);
    const double minChildHeight=JKQTMathTextGetTightBoundingRect(f, "l", painter.device()).height();

    double cAscentAboveStrike=0;
    double cDescentBelowStrike=0;
    cAscentAboveStrike=qMax(minChildHeight-childSize.strikeoutPos, childSize.baselineHeight-childSize.strikeoutPos);
    cDescentBelowStrike=qMax(childSize.strikeoutPos, childSize.getDescent()+childSize.strikeoutPos);

    //qDebug()<<"getSizeInternalAndBrace(): showOpeningBrace="<<showOpeningBrace<<", openbrace="<<openbrace<<", showClosingBrace="<<showClosingBrace<<", closebrace="<<closebrace;
    //qDebug()<<"getSizeInternalAndBrace(): child: baselineHeight="<<baselineHeight<<", strikeoutPos="<<strikeoutPos<<", overallHeight="<<overallHeight;
    //qDebug()<<"getSizeInternalAndBrace(): child: cAscentAboveStrike="<<cAscentAboveStrike<<", cDescentBelowStrike="<<cDescentBelowStrike;

    const double heightAboveBelowStrike=qMax(cAscentAboveStrike, cDescentBelowStrike);

    s.width=childSize.width;
    s.strikeoutPos=childSize.strikeoutPos;
    s.baselineHeight=childSize.strikeoutPos+heightAboveBelowStrike*parentMathText->getBraceFactor();
    s.overallHeight=2.0*heightAboveBelowStrike*parentMathText->getBraceFactor(); //fm.height();

    //qDebug()<<"getSizeInternalAndBrace(): heightAboveBelowStrike="<<heightAboveBelowStrike<<", baselineHeight="<<baselineHeight<<", overallHeight="<<overallHeight;

    calcBraceSizes(s, painter, currentEv, childSize);

    if (openbrace!=MTBTNone && openbrace!=MTBTAny) s.width+=s.openBraceWidth;
    if (closebrace!=MTBTNone && closebrace!=MTBTAny) s.width+=s.closeBraceWidth;
    return s;
}

double JKQTMathTextBraceNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextBraceNode[]::draw()"));
#endif
    //std::cout<<"drawing brace-node: '"<<openbrace.toStdString()<<"' ... '"<<closebrace.toStdString()<<"'\n";

    const NodeSize nodesize=getSizeInternalAndBrace(painter, currentEv);
    doDrawBoxes(painter, x, y, nodesize);
    const QFont f=currentEv.getFont(parentMathText);

    const double lw=JKQTMathTextGetFontLineWidth(f, painter.device());

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
        const double xbrace1=xnew+lw*2.0;
        const double xbrace2=xnew+qMin(paren_fraction*nodesize.openBraceWidth, nodesize.openBraceWidth-lw);
        const double xbraceC=xnew+nodesize.openBraceWidth/2.0;
        if (openbrace==MTBTParenthesis) {
            QPainterPath path;
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
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
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
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
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight)-lw/2.0;
            const double y2=y-nodesize.baselineHeight+lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        } else if (openbrace==MTBTTopCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace2-xbrace1);
            const double y1=y-nodesize.baselineHeight+lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y1+dx);
            painter.drawPath(path);
        } else if (openbrace==MTBTBottomCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace2-xbrace1);
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight)-lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y1-dx);
            painter.drawPath(path);
        } else if (openbrace==MTBTCurlyBracket) {
            const QPainterPath path=JKQTMathTextMakeHBracePath(0,0,nodesize.overallHeight, nodesize.openBraceWidth*paren_fraction, p.widthF());
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(xbraceC, y-nodesize.baselineHeight+nodesize.overallHeight/2.0);
            painter.rotate(90);
            QPen plocal=p;
            plocal.setWidthF(0.0001);
            painter.fillPath(path, QBrush(currentEv.color));
        } else if (openbrace==MTBTFloorBracket) {
            QPainterPath path;
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight)-lw/2.0;
            const double y2=y-nodesize.baselineHeight;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        } else if (openbrace==MTBTCeilBracket) {
            QPainterPath path;
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight+lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace1, y2);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        } else if (openbrace==MTBTSingleLine) {
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
            const QLineF l(xbraceC, y1, xbraceC, y2);
            QPen plocal=p;
            plocal.setWidthF(plocal.widthF()*absnorm_linewidth_factor);
            painter.setPen(plocal);
            if (l.length()>0) painter.drawLine(l);
            painter.setPen(p);
        } else if (openbrace==MTBTDoubleLine) {
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
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
            xnew=xnew+nodesize.openBraceWidth;
        }
    }

    painter.setPen(pold);

    xnew= getChild()->draw(painter, xnew, y, currentEv);

    {
        bool showClosingBrace=true;
        const double xbrace1=qMax(xnew+nodesize.closeBraceWidth-paren_fraction*nodesize.closeBraceWidth, xnew+lw);
        const double xbrace2=xnew+nodesize.closeBraceWidth-lw*2.0;
        const double xbraceC=xnew+nodesize.closeBraceWidth/2.0;
        painter.setPen(p);
        if (closebrace==MTBTParenthesis) {
            QPainterPath path;
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
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
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
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
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight)-lw/2.0;
            const double y2=y-nodesize.baselineHeight+lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        } else if (closebrace==MTBTBottomCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace1-xbrace2);
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight)-lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y1-dx);
            painter.drawPath(path);
        } else if (closebrace==MTBTTopCorner) {
            QPainterPath path;
            const double dx=fabs(xbrace1-xbrace2);
            const double y1=y-nodesize.baselineHeight+lw/2.0;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y1+dx);
            painter.drawPath(path);
        } else if (closebrace==MTBTCurlyBracket) {
            const QPainterPath path=JKQTMathTextMakeHBracePath(0,0,nodesize.overallHeight, nodesize.closeBraceWidth*paren_fraction, p.widthF());
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(xbraceC, y-nodesize.baselineHeight+nodesize.overallHeight/2.0);
            painter.rotate(270);
            QPen plocal=p;
            plocal.setWidthF(0.0001);
            painter.fillPath(path, QBrush(currentEv.color));
        } else if (closebrace==MTBTFloorBracket) {
            QPainterPath path;
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight)-lw/2.0;
            const double y2=y-nodesize.baselineHeight;
            path.moveTo(xbrace1, y1);
            path.lineTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            painter.drawPath(path);
        } else if (closebrace==MTBTCeilBracket) {
            QPainterPath path;
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight+lw/2.0;
            path.moveTo(xbrace2, y1);
            path.lineTo(xbrace2, y2);
            path.lineTo(xbrace1, y2);
            painter.drawPath(path);
        } else if (closebrace==MTBTSingleLine) {
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
            const QLineF l(xbraceC, y1, xbraceC, y2);
            QPen plocal=p;
            plocal.setWidthF(plocal.widthF()*absnorm_linewidth_factor);
            painter.setPen(plocal);
            if (l.length()>0) painter.drawLine(l);
            painter.setPen(p);
        } else if (closebrace==MTBTDoubleLine) {
            const double y1=y+(nodesize.overallHeight-nodesize.baselineHeight);
            const double y2=y-nodesize.baselineHeight;
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
            xnew=xnew+nodesize.closeBraceWidth;
        }
    }

    return xnew;
}

bool JKQTMathTextBraceNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
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



JKQTMathTextBraceNode::NodeSize::NodeSize():
    JKQTMathTextNodeSize(),
    openBraceWidth(0.0),
    openBraceHeight(0.0),
    closeBraceWidth(0.0),
    closeBraceHeight(0.0)
{

}

JKQTMathTextBraceNode::NodeSize::NodeSize(const JKQTMathTextNodeSize &other):
    JKQTMathTextNodeSize(other),
    openBraceWidth(0.0),
    openBraceHeight(0.0),
    closeBraceWidth(0.0),
    closeBraceHeight(0.0)
{

}

JKQTMathTextBraceNode::NodeSize &JKQTMathTextBraceNode::NodeSize::operator=(const JKQTMathTextNodeSize &other)
{
    JKQTMathTextNodeSize::operator=(other);
    openBraceWidth=0.0;
    openBraceHeight=0.0;
    closeBraceWidth=0.0;
    closeBraceHeight=0.0;
    return *this;
}

JKQTMathTextBraceNode::NodeSize &JKQTMathTextBraceNode::NodeSize::operator=(const NodeSize &other)
{
    JKQTMathTextNodeSize::operator=(dynamic_cast<const JKQTMathTextNodeSize&>(other));
    openBraceWidth=other.openBraceWidth;
    openBraceHeight=other.openBraceHeight;
    closeBraceWidth=other.closeBraceWidth;
    closeBraceHeight=other.closeBraceHeight;
    return *this;
}

JKQTMathTextBraceNode::NodeSize::NodeSize(const NodeSize &other):
    JKQTMathTextNodeSize(dynamic_cast<const JKQTMathTextNodeSize&>(other)),
    openBraceWidth(other.openBraceWidth),
    openBraceHeight(other.openBraceHeight),
    closeBraceWidth(other.closeBraceWidth),
    closeBraceHeight(other.closeBraceHeight)
{

}


void JKQTMathTextBraceNode::calcBraceSizes(NodeSize& out, QPainter &painter, const JKQTMathTextEnvironment &ev, const JKQTMathTextNodeSize &childSize) const
{
    const QFont f=ev.getFont(parentMathText);
    const QSizeF openBraceS=calcBraceSize(f, painter.device(), openbrace, childSize);
    const QSizeF closeBraceS=calcBraceSize(f, painter.device(), closebrace, childSize);
    out.openBraceWidth=openBraceS.width();
    out.openBraceHeight=openBraceS.width();
    out.closeBraceWidth=closeBraceS.width();
    out.closeBraceHeight=closeBraceS.width();
}

QSizeF JKQTMathTextBraceNode::calcBraceSize(const QFont &f, QPaintDevice *pd, JKQTMathTextBraceType bracetype, const JKQTMathTextNodeSize &childSize) const
{
    double braceWidth=0.0;
    double braceHeight=0.0;
    const double lw=JKQTMathTextGetFontLineWidth(f, pd);
    const double dblline_distance=2.0*lw;
    braceHeight=childSize.overallHeight*parentMathText->getBraceFactor();
    braceWidth=lw*5.0;
    if (bracetype==MTBTCurlyBracket)  braceWidth=lw*6.5;
    if (bracetype==MTBTParenthesis)  braceWidth=lw*6.0;
    if (bracetype==MTBTDoubleLine)  braceWidth=dblline_distance+3.0*lw;
    if (bracetype==MTBTSingleLine)  braceWidth=3.0*lw;
    if (bracetype==MTBTSquareBracket || bracetype==MTBTCeilBracket || bracetype==MTBTFloorBracket)  braceWidth=7.0*lw;

    const double overSizeFactor=braceHeight/JKQTMathTextGetFontHeight(f, pd);
    if (overSizeFactor>1.2) braceWidth=braceWidth*sqrt(overSizeFactor);

    return QSizeF(braceWidth, braceHeight);
}
