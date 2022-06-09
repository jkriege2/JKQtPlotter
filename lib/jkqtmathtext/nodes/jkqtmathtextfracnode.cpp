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







JKQTMathTextFracNode::JKQTMathTextFracNode(JKQTMathText* _parent, JKQTMathTextNode* child_top, JKQTMathTextNode* child_bottom, JKQTMathTextFracMode mode):
    JKQTMathTextDualChildNode(child_top, child_bottom, _parent)
{
    this->mode=mode;
}

JKQTMathTextFracNode::~JKQTMathTextFracNode() {
}

QString JKQTMathTextFracNode::getTypeName() const
{
    return "MTfracNode";
}

void JKQTMathTextFracNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    const QFont f=currentEv.getFont(parentMathText);
    const QFontMetricsF fm(f, painter.device());
    JKQTMathTextEnvironment ev1=currentEv;
    JKQTMathTextEnvironment ev2=currentEv;

    const double xheight=fm.xHeight(); //tightBoundingRect("x").height();
    const double line_ascent=xheight/2.0;
    const double Mheight=JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height();//fm.ascent();
    const double xwidth=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width();
    const double qheight=JKQTMathTextGetTightBoundingRect(f, "q", painter.device()).height();//fm.ascent();

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUnderbraceFactor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUndersetFactor();
    } else  if (mode==MTFMfrac || mode==MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor();
        ev2.fontSize=ev2.fontSize*getFracScalingFactor();
    } else  if (mode==MTFMtfrac || mode==MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor()*0.7;
        ev2.fontSize=ev2.fontSize*getFracScalingFactor()*0.7;
    }


    double width1=0, baselineHeight1=0, overallHeight1=0, strikeoutPos1=0;
    double width2=0, baselineHeight2=0, overallHeight2=0, strikeoutPos2=0;
    child1->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos1);
    child2->getSize(painter, ev2, width2, baselineHeight2, overallHeight2, strikeoutPos2);


    const double maxHeight=qMax(overallHeight1,overallHeight2);
    const bool _isBraceParentNearerThanFrac=isBraceParentNearerThanFrac();
    const double height1OrMaxHeight=(_isBraceParentNearerThanFrac)?maxHeight:overallHeight1;
    const double height2OrMaxHeight=(_isBraceParentNearerThanFrac)?maxHeight:overallHeight2;
    overallHeight=0;
    baselineHeight=0;
    width=0;
    if (mode==MTFMfrac || mode==MTFMdfrac || mode==MTFMtfrac || mode==MTFMstackrel) {
        const double top_ascent=line_ascent+xheight*parentMathText->getFracShiftFactor();
        const double bot_ascent=line_ascent-xheight*parentMathText->getFracShiftFactor();
        // here we use maxHeight (as LaTeX does) so braces are centered around the xHieght!!!
        // if there are no braces, we can use the actual height
        const double newascent=height1OrMaxHeight+top_ascent;
        const double newdescent=height2OrMaxHeight-bot_ascent;
        width=qMax(width1, width2);
        if (mode!=MTFMstackrel) width+=xwidth/2.0;
        strikeoutPos=line_ascent;

        overallHeight=newascent+newdescent;
        baselineHeight=newascent;

    } else if (mode==MTFMstfrac || mode==MTFMsfrac) {
        const double top_ascent=line_ascent;
        // here we use maxHeight (as LaTeX does) so braces are centered around the xHieght!!!
        // if there are no braces, we can use the actual height
        const double newascent=height1OrMaxHeight+top_ascent;
        const double newdescent=qMax(height2OrMaxHeight-baselineHeight2, qheight-xheight);
        width=width1+width2+xwidth/2.0;
        strikeoutPos=line_ascent;

        overallHeight=newascent+newdescent;
        baselineHeight=newascent;
    } else if (mode==MTFMunderbrace) {
        overallHeight=overallHeight1+overallHeight2+Mheight/2.0;
        baselineHeight=baselineHeight1;
        width=qMax(width1, width2)+xwidth;
        strikeoutPos=line_ascent;
    } else if (mode==MTFMoverbrace) {
        overallHeight=overallHeight1+overallHeight2+Mheight/2.0;
        baselineHeight=baselineHeight1+overallHeight2+Mheight/2.0;
        width=qMax(width1, width2)+xwidth;
        strikeoutPos=line_ascent;
    } else if (mode==MTFMunderset) {
        overallHeight=overallHeight1+overallHeight2+xheight/6.0;
        baselineHeight=baselineHeight1;
        width=qMax(width1, width2)+xwidth;
        strikeoutPos=line_ascent;
    } else if (mode==MTFMoverset) {
        overallHeight=overallHeight1+overallHeight2+xheight/6.0;
        baselineHeight=baselineHeight1+overallHeight2+xheight/6.0;
        width=qMax(width1, width2)+xwidth;
        strikeoutPos=line_ascent;
    }
}

int JKQTMathTextFracNode::getNestingLevel(bool sameType) const
{
    QList<const JKQTMathTextFracNode*> parents=getParents<JKQTMathTextFracNode>();
    int cnt=0;
    for (auto& p: parents) {
        if (p && p->getMode()==getMode()) cnt++;
    }
    return cnt;
}

double JKQTMathTextFracNode::getFracScalingFactor() const
{
    if (mode!=MTFMdfrac) {
        const int level=getNestingLevel(true);
        if (level>=1) return parentMathText->getFracNestedFactor();
    }
    return parentMathText->getFracFactor();
}

bool JKQTMathTextFracNode::isBraceParentNearerThanFrac() const
{
    auto parents=getParents<JKQTMathTextNode>();
    for (const auto& p: parents) {
        if (p!=nullptr) {
            if (dynamic_cast<const JKQTMathTextBraceNode*>(p)!=nullptr) return true;
            if (dynamic_cast<const JKQTMathTextFracNode*>(p)!=nullptr) return false;
        }
    }
    return false;

}

double JKQTMathTextFracNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    const QFont f=currentEv.getFont(parentMathText);
    const QFontMetricsF fm(f, painter.device());
    JKQTMathTextEnvironment ev1=currentEv;
    JKQTMathTextEnvironment ev2=currentEv;


    const double xheight=fm.xHeight();
    const double xwidth=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width();
    const double linewideth=qMax(0.0,ceil(currentEv.fontSize/16.0));//fm.lineWidth();
    const double Mheight=JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height();//fm.ascent();
    const double qheight=JKQTMathTextGetTightBoundingRect(f, "q", painter.device()).height();//fm.ascent();
    const double bw=Mheight/2.0;

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUnderbraceFactor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUndersetFactor();
    } else  if (mode==MTFMfrac || mode==MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor();
        ev2.fontSize=ev2.fontSize*getFracScalingFactor();
    } else  if (mode==MTFMtfrac || mode==MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor()*0.7;
        ev2.fontSize=ev2.fontSize*getFracScalingFactor()*0.7;
    }


    double width1=0, baselineHeight1=0, overallHeight1=0;//, strikeoutPos1=0;
    double width2=0, baselineHeight2=0, overallHeight2=0, strikeoutPos=0;
    child1->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos);
    child2->getSize(painter, ev2, width2, baselineHeight2, overallHeight2, strikeoutPos);
    double ascent1=baselineHeight1;
    double descent1=overallHeight1-baselineHeight1;
    double ascent2=baselineHeight2;
    double descent2=overallHeight2-baselineHeight2;

    double yline=y-xheight*0.5;


    //double overallHeight=overallHeight1+overallHeight2+xh;
    //double baselineHeight=3.0*xh/2.0+overallHeight1;
    const double maxWidth=qMax(width1, width2);
    double deltaWidth=0;

    QPen p=painter.pen();
    p.setColor(ev1.color);
    p.setStyle(Qt::SolidLine);
    p.setWidthF(qMax(parentMathText->ABS_MIN_LINEWIDTH, linewideth));
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(p);
    if (mode==MTFMfrac || mode==MTFMdfrac || mode==MTFMtfrac) {
        deltaWidth=xwidth/2.0;
        const QLineF l(x+p.widthF(), yline, x+maxWidth+deltaWidth-p.widthF(), yline);
        if (l.length()>0) painter.drawLine(l);
        child1->draw(painter, x+deltaWidth/2.0+(maxWidth-width1)/2.0, yline-xheight*(parentMathText->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+deltaWidth/2.0+(maxWidth-width2)/2.0, yline+xheight*(parentMathText->getFracShiftFactor())+ascent2, ev2);
    } else if (mode==MTFMstackrel) {
        child1->draw(painter, x+(maxWidth-width1)/2.0, yline-xheight*(parentMathText->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+(maxWidth-width2)/2.0, yline+xheight*(parentMathText->getFracShiftFactor())+ascent2, ev2);
    } else if (mode==MTFMstfrac || mode==MTFMsfrac) {
        deltaWidth=xwidth/2.0;
        child1->draw(painter, x, yline, ev1);
        child2->draw(painter, x+width1+deltaWidth, y, ev2);
        const QLineF l(x+width1+deltaWidth, y-Mheight, x+width1, y+(qheight-xheight));
        if (l.length()>0) painter.drawLine(l);
    } else if (mode==MTFMunderset) {
        child1->draw(painter, x+xwidth/2.0+(maxWidth-width1)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-width2)/2.0, y+descent1+xheight/6.0+ascent2, ev2);
        deltaWidth=xwidth;
    } else if (mode==MTFMunderbrace) {
        double ybrace=y+descent1+bw/2.0;
        const QPainterPath path=JKQTMathTextMakeHBracePath(x+xwidth/2.0+(width1)/2.0, ybrace, maxWidth, bw);
        painter.drawPath(path);

        child1->draw(painter, x+xwidth/2.0+(maxWidth-width1)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-width2)/2.0, y+descent1+bw+ascent2, ev2);
        deltaWidth=xwidth;
    } else if (mode==MTFMoverset) {
        child1->draw(painter, x+xwidth/2.0+(maxWidth-width1)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-width2)/2.0, y-ascent1-xheight/6.0-descent2, ev2);
        deltaWidth=xwidth;
    } else if (mode==MTFMoverbrace) {
        const double ybrace=y-ascent1-bw/2.0;

        {
            painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(x+xwidth/2.0+(width1)/2.0, ybrace);
            painter.rotate(180);
            const QPainterPath path=JKQTMathTextMakeHBracePath(0,0, maxWidth, bw);
            painter.drawPath(path);
        }

        child1->draw(painter, x+xwidth/2.0+(maxWidth-width1)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-width2)/2.0, y-ascent1-bw-descent2, ev2);
        deltaWidth=xwidth;
    }


    if (mode==MTFMstfrac || mode==MTFMsfrac) return x+width1+width2+deltaWidth;
    else return x+maxWidth+deltaWidth;

}

bool JKQTMathTextFracNode::toHtml(QString &/*html*/, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) {
    bool ok=false;



    return ok;
}

JKQTMathTextFracMode JKQTMathTextFracNode::getMode() const {
    return this->mode;
}



