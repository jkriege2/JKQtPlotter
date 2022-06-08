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



#include "jkqtmathtext/nodes/jkqtmathtextdecoratednode.h"
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







JKQTMathTextDecoratedNode::JKQTMathTextDecoratedNode(JKQTMathText* _parent, JKQTMathTextDecoration decoration, JKQTMathTextNode* child):
    JKQTMathTextNode(_parent)
{
    this->child=child;
    this->decoration=decoration;
}

JKQTMathTextDecoratedNode::~JKQTMathTextDecoratedNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathTextDecoratedNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    const QFontMetricsF fm(currentEv.getFont(parent), painter.device());

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    const double italic_xcorrection=getNonItalicXCorretion(painter, width, currentEv, child);
    const double decoheightfactor=parent->getDecorationHeightFactor();
    const double deco_miniwidth=((decoration==MTDtilde||decoration==MTDbreve)?fm.boundingRect("~").width():fm.boundingRect("^").width())-italic_xcorrection;

    const double decoAboveAscent_yposdelta=fm.ascent()*(1.0+2.0*decoheightfactor);
    const double decoAboveBaselineheight_yposdelta=baselineHeight*(1.0+decoheightfactor);


    const double descent=overallHeight-baselineHeight;
    baselineHeight=decoAboveBaselineheight_yposdelta;
    if (decoration==MTDbar) {
        baselineHeight=std::max<double>(baselineHeight, decoAboveAscent_yposdelta);
    }
    overallHeight=baselineHeight+descent;
    width=std::max<double>(deco_miniwidth,width);
}

double JKQTMathTextDecoratedNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    double width=0, baselineHeight=0, overallHeight=0, strikeoutPos=0;
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    const QFont font=ev.getFont(parent);
    const QFontMetricsF fm(font, painter.device());
    const double width_X=fm.boundingRect("X").width();
    const double width_x=fm.boundingRect("x").width();
    const double width_dot=fm.boundingRect(".").width()/2.0;
    const double decoheightfactor=parent->getDecorationHeightFactor();
    const double deco_ypos=y-baselineHeight*(1.0+decoheightfactor);
    const double decoAboveAscent_ypos=y-fm.ascent()*(1.0+decoheightfactor);
    const double strike_ypos=y-baselineHeight/2.0;
    const double decobelow_ypos=y+qMax((overallHeight-baselineHeight)*(1.0+decoheightfactor), fm.xHeight()*decoheightfactor);
    const double deco_height=decoheightfactor*baselineHeight;
    const double italic_xcorrection=getNonItalicXCorretion(painter, width, ev, child);
    const double deco_xoffset=parent->getDecorationWidthReductionXFactor()*width_X/2.0;
    const double deco_width=std::max<double>(width_x*0.5,width-2.0*deco_xoffset-italic_xcorrection);
    const double deco_vecwidth=width_x*0.33;
    const double deco_miniwidth=((decoration==MTDtilde||decoration==MTDbreve)?fm.boundingRect("~").width():fm.boundingRect("^").width())-italic_xcorrection;
    const double decotop_xcenter=x+italic_xcorrection+(width-italic_xcorrection)/2.0;
    const double decotop_xstart=decotop_xcenter-deco_width/2.0;
    const double decotop_xend=decotop_xcenter+deco_width/2.0;
    const double decobot_xstart=x;
    const double decobot_xend=x+width-italic_xcorrection;
    const double decobot_xcenter=(decobot_xstart+decobot_xend)/2.0;



    QPen pold=painter.pen();
    QPen p=pold;
    p.setColor(ev.color);
    p.setWidthF(qMax(parent->ABS_MIN_LINEWIDTH, fm.lineWidth()));//ceil(currentEv.fontSize/16.0));

    double xnew=child->draw(painter, x, y, ev);

    if (decoration==MTDvec) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ypos)<<QPointF(decotop_xend, deco_ypos)<<QPointF(decotop_xend-deco_vecwidth, deco_ypos-deco_height*2.0/3.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDoverline) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, deco_ypos, decotop_xend, deco_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDbar) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, decoAboveAscent_ypos, decotop_xend, decoAboveAscent_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleoverline) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, deco_ypos, decotop_xend, deco_ypos);
        if (l.length()>0) painter.drawLine(l);
        const QLineF l2(decotop_xstart, deco_ypos-2.0*p.widthF(), decotop_xend, deco_ypos-2.0*p.widthF());
        if (l2.length()>0) painter.drawLine(l2);
        painter.setPen(pold);
    } else if (decoration==MTDunderline) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, decobelow_ypos, decobot_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleunderline) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, decobelow_ypos, decobot_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        QLineF l2(decobot_xstart, decobelow_ypos+2.0*p.widthF(), decobot_xend, decobelow_ypos+2.0*p.widthF());
        if (l2.length()>0) painter.drawLine(l2);
        painter.setPen(pold);
    } else if (decoration==MTDarrow) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, deco_ypos+deco_height/2.0, decotop_xend, deco_ypos+deco_height/2.0);
        if (l.length()>0) painter.drawLine(l);
        QPolygonF poly;
        poly<<QPointF(decotop_xend-deco_vecwidth, deco_ypos)<<QPointF(decotop_xend, deco_ypos+deco_height/2.0)<<QPointF(decotop_xend-deco_vecwidth, deco_ypos+deco_height);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDhat) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xcenter-0.5*deco_miniwidth, deco_ypos+deco_height/3.0)<<QPointF(decotop_xcenter, deco_ypos)<<QPointF(decotop_xcenter+0.5*deco_miniwidth, deco_ypos+deco_height/3.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDwidehat) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ypos+deco_height/2.0)<<QPointF(decotop_xcenter, deco_ypos+deco_height/3.0)<<QPointF(decotop_xend, deco_ypos+deco_height/2.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);

    } else if (decoration==MTDcheck) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xcenter-0.5*deco_miniwidth, deco_ypos)<<QPointF(decotop_xcenter, deco_ypos+deco_height/3.0)<<QPointF(decotop_xcenter+0.5*deco_miniwidth, deco_ypos);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDwidecheck) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ypos-deco_height/2.0)<<QPointF(decotop_xcenter, deco_ypos+deco_height/3.0)<<QPointF(decotop_xend, deco_ypos-deco_height/2.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDbreve) {
        painter.setPen(p);
        QPainterPath path;
        const double x0=decotop_xcenter-deco_miniwidth/2.0;
        const double x1=decotop_xcenter+deco_miniwidth/2.0;
        path.moveTo(x0, deco_ypos);
        path.cubicTo(x0, deco_ypos+deco_height,  x1, deco_ypos+deco_height    , x1, deco_ypos);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDtilde) {
        painter.setPen(p);
        QPainterPath path;
        const double x0=decotop_xcenter-deco_miniwidth/2.0;
        const double x1=decotop_xcenter+deco_miniwidth/2.0;
        path.moveTo(x0, deco_ypos);
        path.cubicTo((x0+x1)/2.0, deco_ypos+deco_height,  (x0+x1)/2.0, deco_ypos-deco_height    , x1, deco_ypos);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDwidetilde) {
        painter.setPen(p);
        QPainterPath path;
        const double x0=decotop_xstart;
        const double x1=decotop_xend;
        path.moveTo(x0, deco_ypos);
        path.cubicTo((x0+x1)/2.0, deco_ypos+deco_height,  (x0+x1)/2.0, deco_ypos-deco_height    , x1, deco_ypos);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDocirc) {
        painter.setPen(p);
        const QBrush bold=painter.brush();
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QPointF(decotop_xcenter, deco_ypos), width_dot/1.5, width_dot/1.5);
        painter.setBrush(bold);
        painter.setPen(pold);
    } else if (decoration==MTDdot) {
        p.setWidthF(0.01);
        painter.setPen(p);
        QBrush b=painter.brush();
        const QBrush bold=b;
        b.setColor(ev.color);
        b.setStyle(Qt::SolidPattern);
        painter.setBrush(b);
        painter.drawEllipse(QPointF(decotop_xcenter, deco_ypos), width_dot/2.0, width_dot/2.0);
        painter.setBrush(bold);
        painter.setPen(pold);
    } else if (decoration==MTDddot) {
        p.setWidthF(0.01);
        painter.setPen(p);
        QBrush b=painter.brush();
        const QBrush bold=b;
        b.setColor(ev.color);
        b.setStyle(Qt::SolidPattern);
        painter.setBrush(b);
        painter.drawEllipse(QPointF(decotop_xcenter-width_dot, deco_ypos), width_dot/2.0, width_dot/2.0);
        painter.drawEllipse(QPointF(decotop_xcenter+width_dot, deco_ypos), width_dot/2.0, width_dot/2.0);
        painter.setBrush(bold);
        painter.setPen(pold);
        painter.setBrush(bold);
    } else if (decoration==MTDstrike) {
        painter.setPen(p);
        const QLineF l((decotop_xstart+decobot_xstart)/2.0, strike_ypos, (decotop_xend+decobot_xend)/2.0, strike_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDcancel) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, decobelow_ypos, decotop_xend, deco_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDbcancel) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, deco_ypos, decotop_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDxcancel) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, deco_ypos, decotop_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        const QLineF l1(decobot_xstart, decobelow_ypos, decotop_xend, deco_ypos);
        if (l1.length()>0) painter.drawLine(l1);
        painter.setPen(pold);
    }

    /*painter.setPen(QPen(Qt::red, 1.5));
    painter.drawLine(QLineF(x, deco_ypos, xnew, deco_ypos));
    painter.setPen(QPen(Qt::green, 1.5));
    painter.drawLine(QLineF(deco_xstart, deco_ypos+2, deco_xend, deco_ypos+2));
    painter.drawEllipse(QPointF(deco_xpos_center, deco_ypos+2), 5, 5);
    painter.setPen(pold);*/
    return xnew;
}

bool JKQTMathTextDecoratedNode::toHtml(QString &/*html*/, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) {
    //QString f;
    //JKQTMathTextEnvironment ev=currentEv;

    //bool ok=child->toHtml(html, ev, defaultEv);

    return false;
}

void JKQTMathTextDecoratedNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}

QString JKQTMathTextDecoratedNode::getTypeName() const
{
    return "MTdecoratedNode";
}

JKQTMathTextNode *JKQTMathTextDecoratedNode::getChild() const {
    return this->child;
}

JKQTMathTextDecoration JKQTMathTextDecoratedNode::getDecoration() const {
    return this->decoration;
}


