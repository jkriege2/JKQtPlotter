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



#include "jkqtmathtext/nodes/jkqtmathtextfracnode.h"
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







JKQTMathTextFracNode::JKQTMathTextFracNode(JKQTMathText* _parent, JKQTMathTextNode* child_top, JKQTMathTextNode* child_bottom, JKQTMathTextFracMode mode):
    JKQTMathTextNode(_parent)
{
    this->child1=child_top;
    this->child2=child_bottom;
    this->mode=mode;
}

JKQTMathTextFracNode::~JKQTMathTextFracNode() {
    if (child1!=nullptr) delete child1;
    if (child2!=nullptr) delete child2;
}

QString JKQTMathTextFracNode::getTypeName() const
{
    return "MTfracNode";
}

void JKQTMathTextFracNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTMathTextEnvironment ev1=currentEv;
    JKQTMathTextEnvironment ev2=currentEv;

    double xh=fm.xHeight(); //tightBoundingRect("x").height();
    double sp=xh;
    double Ah=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parent), "M", painter.device()).height();//fm.ascent();
    double xw=fm.boundingRect("x").width();

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parent->getUnderbraceFactor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parent->getUndersetFactor();
    } else  if (mode==MTFMfrac || mode==MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor();
        ev2.fontSize=ev2.fontSize*parent->getFracFactor();
    } else  if (mode==MTFMtfrac || mode==MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor()*0.7;
        ev2.fontSize=ev2.fontSize*parent->getFracFactor()*0.7;
    }


    double width1=0, baselineHeight1=0, overallHeight1=0, strikeoutPos1=0;
    double width2=0, baselineHeight2=0, overallHeight2=0, strikeoutPos2=0;
    child1->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos1);
    child2->getSize(painter, ev2, width2, baselineHeight2, overallHeight2, strikeoutPos2);

    overallHeight=0;
    baselineHeight=0;
    width=0;
    if (mode==MTFMfrac || mode==MTFMdfrac || mode==MTFMtfrac) {
        //std::cout<<"\nxh="<<xh;
        //std::cout<<"\n  baselineHeight1="<<baselineHeight1<<",  overallHeight1="<<overallHeight1;
        //std::cout<<"\n  baselineHeight2="<<baselineHeight2<<",  overallHeight2="<<overallHeight2<<std::endl;
        //overallHeight=overallHeight1+overallHeight2+sp*(2.0*parent->getFracShiftFactor());
        //baselineHeight=overallHeight1+xh*(2.0*parent->getFracShiftFactor());
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->getFracShiftFactor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->getFracShiftFactor());
        //std::cout<<"=>  baselineHeight="<<baselineHeight<<",  overallHeight="<<overallHeight<<std::endl;
        width=qMax(width1, width2)+ xw;
        strikeoutPos=sp;
    } else if (mode==MTFMstfrac || mode==MTFMsfrac) {
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->getFracShiftFactor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->getFracShiftFactor());
        width=width1+width2+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMstackrel) {
        //overallHeight=overallHeight1+overallHeight2+sp*(2.0*parent->getFracShiftFactor());
        //baselineHeight=overallHeight1+xh*(2.0*parent->getFracShiftFactor());
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->getFracShiftFactor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->getFracShiftFactor());
        width=qMax(width1, width2)+ xw;
        strikeoutPos=sp;
    } else if (mode==MTFMunderbrace) {
        overallHeight=overallHeight1+overallHeight2+Ah/2.0;
        baselineHeight=baselineHeight1;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMoverbrace) {
        overallHeight=overallHeight1+overallHeight2+Ah/2.0;
        baselineHeight=baselineHeight1+overallHeight2+Ah/2.0;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMunderset) {
        overallHeight=overallHeight1+overallHeight2+xh/6.0;
        baselineHeight=baselineHeight1;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMoverset) {
        overallHeight=overallHeight1+overallHeight2+xh/6.0;
        baselineHeight=baselineHeight1+overallHeight2+xh/6.0;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    }
}

double JKQTMathTextFracNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    QFont f=currentEv.getFont(parent);
    QFontMetricsF fm(f, painter.device());
    JKQTMathTextEnvironment ev1=currentEv;
    JKQTMathTextEnvironment ev2=currentEv;


    double xh=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height(); //fm.xHeight();
    double xw=fm.boundingRect("x").width();
    double lw=qMax(0.0,ceil(currentEv.fontSize/16.0));//fm.lineWidth();
    double Ah=JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height();//fm.ascent();
    double bw=Ah/2.0;

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parent->getUnderbraceFactor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parent->getUndersetFactor();
    } else  if (mode==MTFMfrac || mode==MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor();
        ev2.fontSize=ev2.fontSize*parent->getFracFactor();
    } else  if (mode==MTFMtfrac || mode==MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor()*0.7;
        ev2.fontSize=ev2.fontSize*parent->getFracFactor()*0.7;
    }


    double width1=0, baselineHeight1=0, overallHeight1=0;//, strikeoutPos1=0;
    double width2=0, baselineHeight2=0, overallHeight2=0, strikeoutPos=0;
    child1->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos);
    child2->getSize(painter, ev2, width2, baselineHeight2, overallHeight2, strikeoutPos);
    double ascent1=baselineHeight1;
    double descent1=overallHeight1-baselineHeight1;
    double ascent2=baselineHeight2;
    double descent2=overallHeight2-baselineHeight2;

    double yline=y-xh*0.5;


    //double overallHeight=overallHeight1+overallHeight2+xh;
    //double baselineHeight=3.0*xh/2.0+overallHeight1;
    double width=qMax(width1, width2);

    QPen p=painter.pen();
    p.setColor(ev1.color);
    p.setStyle(Qt::SolidLine);
    p.setWidthF(qMax(parent->ABS_MIN_LINEWIDTH, lw));
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(p);
    if (mode==MTFMfrac || mode==MTFMdfrac || mode==MTFMtfrac) {
        QLineF l(x+xw/4.0, yline, x+width+xw/2.0, yline);
        if (l.length()>0) painter.drawLine(l);
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, yline-xh*(parent->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, yline+xh*(parent->getFracShiftFactor())+ascent2, ev2);
    } else if (mode==MTFMstfrac || mode==MTFMsfrac) {
        child1->draw(painter, x, yline-descent1, ev1);
        child2->draw(painter, x+width+xw, yline+ascent2, ev2);
        QLineF l(x+width+1.2*xw, yline-descent1-ascent1, x+width-0.2*xw, yline+ascent1+descent1);
        if (l.length()>0) painter.drawLine(l);
    } else if (mode==MTFMstackrel) {
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, yline-xh*(parent->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, yline+xh*(parent->getFracShiftFactor())+ascent2, ev2);
    } else if (mode==MTFMunderset) {
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y+descent1+xh/6.0+ascent2, ev2);
    } else if (mode==MTFMunderbrace) {
        double ybrace=y+descent1+bw/2.0;
        QPainterPath path=JKQTMathTextMakeHBracePath(x+xw/2.0+(width1)/2.0, ybrace, width, bw);
        painter.drawPath(path);

        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y+descent1+bw+ascent2, ev2);
    } else if (mode==MTFMoverset) {
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y-ascent1-xh/6.0-descent2, ev2);
    } else if (mode==MTFMoverbrace) {
        double ybrace=y-ascent1-bw/2.0;

        {
            painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(x+xw/2.0+(width1)/2.0, ybrace);
            painter.rotate(180);
            QPainterPath path=JKQTMathTextMakeHBracePath(0,0, width, bw);
            painter.drawPath(path);
        }

        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y-ascent1-bw-descent2, ev2);
    }


    if (mode==MTFMstackrel) return x+width+ xw;
    else if (mode==MTFMstfrac || mode==MTFMsfrac) return x+width+width2+xw;
    else return x+width+xw;

}

bool JKQTMathTextFracNode::toHtml(QString &/*html*/, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) {
    bool ok=false;



    return ok;
}

void JKQTMathTextFracNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child1->setDrawBoxes(draw);
    child2->setDrawBoxes(draw);

}

JKQTMathTextNode *JKQTMathTextFracNode::getChild1() const {
    return this->child1;
}

JKQTMathTextNode *JKQTMathTextFracNode::getChild2() const {
    return this->child2;
}

JKQTMathTextFracMode JKQTMathTextFracNode::getMode() const {
    return this->mode;
}



