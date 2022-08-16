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


QHash<QString, JKQTMathTextFracNode::FracType> JKQTMathTextFracNode::instructions;


void JKQTMathTextFracNode::fillInstructions()
{
    if (instructions.size()>0) return;
    instructions["frac"]=MTFMfrac;
    instructions["dfrac"] = MTFMdfrac;
    instructions["cfrac"]=MTFMdfrac;
    instructions["sfrac"] = MTFMsfrac;
    instructions["slantfrac"] = MTFMsfrac;
    instructions["xfrac"]=MTFMsfrac;
    instructions["stfrac"] = MTFMstfrac;
    instructions["nicefrac"] = MTFMstfrac;
    instructions["slanttextfrac"] = MTFMstfrac;
    instructions["xtfrac"]=MTFMstfrac;
    instructions["tfrac"]=MTFMtfrac;
    instructions["stackrel"]=MTFMstackrel;
    instructions["underbrace"]=MTFMunderbrace;
    instructions["underbracket"]=MTFMunderbracket;
    instructions["underset"]=MTFMunderset;
    instructions["overbrace"]=MTFMoverbrace;
    instructions["overbracket"]=MTFMoverbracket;
    instructions["overset"]=MTFMoverset;
}





QString JKQTMathTextFracNode::FracType2String(JKQTMathTextFracNode::FracType mode)
{
    switch(mode) {
        case JKQTMathTextFracNode::MTFMfrac:
            return "frac";
        case JKQTMathTextFracNode::MTFMdfrac:
            return "dfrac";
        case JKQTMathTextFracNode::MTFMsfrac:
            return "sfrac";
        case JKQTMathTextFracNode::MTFMstfrac:
            return "stfrac";
        case JKQTMathTextFracNode::MTFMtfrac:
            return "tfrac";
        case JKQTMathTextFracNode::MTFMunderbrace:
            return "underbrace";
        case JKQTMathTextFracNode::MTFMoverbrace:
            return "overbrace";
        case JKQTMathTextFracNode::MTFMunderbracket:
            return "underbracket";
        case JKQTMathTextFracNode::MTFMoverbracket:
            return "overbracket";
        case JKQTMathTextFracNode::MTFMunderset:
            return "underset";
        case JKQTMathTextFracNode::MTFMoverset:
            return "overset";
        case JKQTMathTextFracNode::MTFMstackrel:
            return "stackrel";
    }
    return "unknown";
}

JKQTMathTextFracNode::FracType JKQTMathTextFracNode::InstructionName2FracType(const QString &mode)
{
    fillInstructions();
    return instructions.value(mode, MTFMfrac);
}

bool JKQTMathTextFracNode::supportsInstructionName(const QString &instructionName)
{
    fillInstructions();
    return instructions.contains(instructionName);
}



JKQTMathTextFracNode::JKQTMathTextFracNode(JKQTMathText* _parent, JKQTMathTextNode* child_top, JKQTMathTextNode* child_bottom, JKQTMathTextFracNode::FracType mode):
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

JKQTMathTextNodeSize JKQTMathTextFracNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    const QFont f=currentEv.getFont(parentMathText);
    const QFontMetricsF fm(f, painter.device());
    JKQTMathTextEnvironment ev1=currentEv;
    JKQTMathTextEnvironment ev2=currentEv;

    const double xheight=fm.xHeight(); //tightBoundingRect("x").height();
    const double line_ascent=xheight/2.0;
    //const double Mheight=JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height();//fm.ascent();
    const double xwidth=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width();
    const double qheight=JKQTMathTextGetTightBoundingRect(f, "q", painter.device()).height();//fm.ascent();
    const double braceheight=fm.xHeight()*parentMathText->getUnderbraceBraceSizeXFactor();
    const double braceseparation=fm.xHeight()*parentMathText->getUnderbraceSeparationXFactor();

    if (mode==JKQTMathTextFracNode::MTFMunderbrace || mode==JKQTMathTextFracNode::MTFMoverbrace||mode==JKQTMathTextFracNode::MTFMunderbracket || mode==JKQTMathTextFracNode::MTFMoverbracket) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUnderbraceFactor();
    } else if (mode==JKQTMathTextFracNode::MTFMunderset || mode==JKQTMathTextFracNode::MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUndersetFactor();
    } else  if (mode==JKQTMathTextFracNode::MTFMfrac || mode==JKQTMathTextFracNode::MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor();
        ev2.fontSize=ev2.fontSize*getFracScalingFactor();
    } else  if (mode==JKQTMathTextFracNode::MTFMtfrac || mode==JKQTMathTextFracNode::MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor()*0.7;
        ev2.fontSize=ev2.fontSize*getFracScalingFactor()*0.7;
    }

    const QFontMetricsF fmev1(ev1.getFont(parentMathText), painter.device());
    const QRectF AeTBR1=fmev1.tightBoundingRect("A");
    const double asc1=AeTBR1.height();
    const QFontMetricsF fmev2(ev2.getFont(parentMathText), painter.device());
    const QRectF AeTBR2=fmev2.tightBoundingRect("A");
    const double asc2=AeTBR2.height();

    JKQTMathTextNodeSize size1=child1->getSize(painter, ev1);
    JKQTMathTextNodeSize size2=child2->getSize(painter, ev2);
    if (asc1>size1.baselineHeight) {
        const double oldDescent=size1.overallHeight-size1.baselineHeight;
        size1.baselineHeight=asc1;
        size1.overallHeight=size1.baselineHeight+oldDescent;
    }
    if (asc2>size2.baselineHeight) {
        const double oldDescent=size2.overallHeight-size2.baselineHeight;
        size2.baselineHeight=asc2;
        size2.overallHeight=size2.baselineHeight+oldDescent;
    }
    const double descent1=size1.overallHeight-size1.baselineHeight;

    JKQTMathTextNodeSize size;
    size.overallHeight=0;
    size.baselineHeight=0;
    size.width=0;
    if (mode==JKQTMathTextFracNode::MTFMfrac || mode==JKQTMathTextFracNode::MTFMdfrac || mode==JKQTMathTextFracNode::MTFMtfrac || mode==JKQTMathTextFracNode::MTFMstackrel) {
        const double top_ascent=line_ascent+xheight*parentMathText->getFracShiftFactor();
        const double bot_ascent=line_ascent-xheight*parentMathText->getFracShiftFactor();
        const double newascent=size1.overallHeight+top_ascent;
        const double newdescent=size2.overallHeight-bot_ascent;
        size.width=qMax(size1.width, size2.width);
        if (mode!=JKQTMathTextFracNode::MTFMstackrel) size.width+=xwidth/2.0;
        size.strikeoutPos=line_ascent;

        size.overallHeight=newascent+newdescent;
        size.baselineHeight=newascent;

    } else if (mode==JKQTMathTextFracNode::MTFMstfrac || mode==JKQTMathTextFracNode::MTFMsfrac) {
        const double top_ascent=line_ascent;
        const double newascent=size1.overallHeight+top_ascent;
        const double newdescent=qMax(size2.overallHeight-size2.baselineHeight, qheight-xheight);
        size.width=size1.width+size2.width+xwidth*0.666;
        size.strikeoutPos=line_ascent;

        size.overallHeight=newascent+newdescent;
        size.baselineHeight=newascent;
    } else if (mode==JKQTMathTextFracNode::MTFMunderbrace || mode==JKQTMathTextFracNode::MTFMunderbracket) {
        const double newdescent=descent1+size2.overallHeight+braceheight+2.0*braceseparation;
        size.overallHeight=newdescent+size1.baselineHeight;
        size.baselineHeight=size1.baselineHeight;
        size.width=qMax(size1.width, size2.width)+xwidth;
        size.strikeoutPos=line_ascent;
    } else if (mode==JKQTMathTextFracNode::MTFMoverbrace || mode==JKQTMathTextFracNode::MTFMoverbracket) {
        size.overallHeight=size1.overallHeight+size2.overallHeight+braceheight+2.0*braceseparation;
        size.baselineHeight=size1.baselineHeight+size2.overallHeight+braceheight+2.0*braceseparation;
        size.width=qMax(size1.width, size2.width)+xwidth;
        size.strikeoutPos=line_ascent;
    } else if (mode==JKQTMathTextFracNode::MTFMunderset) {
        size.overallHeight=size1.overallHeight+size2.overallHeight+xheight/6.0;
        size.baselineHeight=size1.baselineHeight;
        size.width=qMax(size1.width, size2.width)+xwidth;
        size.strikeoutPos=line_ascent;
    } else if (mode==JKQTMathTextFracNode::MTFMoverset) {
        size.overallHeight=size1.overallHeight+size1.overallHeight+xheight/6.0;
        size.baselineHeight=size1.baselineHeight+size2.overallHeight+xheight/6.0;
        size.width=qMax(size1.width, size2.width)+xwidth;
        size.strikeoutPos=line_ascent;
    }
    return size;
}

int JKQTMathTextFracNode::getNestingLevel(bool /*sameType*/) const
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
    if (mode!=JKQTMathTextFracNode::MTFMdfrac) {
        const int level=getNestingLevel(true);
        if (level>=1) return parentMathText->getFracNestedFactor();
    }
    return parentMathText->getFracFactor();
}

double JKQTMathTextFracNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
    doDrawBoxes(painter, x, y, currentEv);
    const QFont f=currentEv.getFont(parentMathText);
    const QFontMetricsF fm(f, painter.device());
    JKQTMathTextEnvironment ev1=currentEv;
    JKQTMathTextEnvironment ev2=currentEv;


    const double xheight=fm.xHeight();
    const double xwidth=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width();
    const double linewideth=fm.lineWidth();
    const double Mheight=JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height();//fm.ascent();
    const double qheight=JKQTMathTextGetTightBoundingRect(f, "q", painter.device()).height();//fm.ascent();
    const double braceheight=fm.xHeight()*parentMathText->getUnderbraceBraceSizeXFactor();
    const double braceseparation=fm.xHeight()*parentMathText->getUnderbraceSeparationXFactor();

    if (mode==JKQTMathTextFracNode::MTFMunderbrace || mode==JKQTMathTextFracNode::MTFMoverbrace||mode==JKQTMathTextFracNode::MTFMunderbracket || mode==JKQTMathTextFracNode::MTFMoverbracket) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUnderbraceFactor();
    } else if (mode==JKQTMathTextFracNode::MTFMunderset || mode==JKQTMathTextFracNode::MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parentMathText->getUndersetFactor();
    } else  if (mode==JKQTMathTextFracNode::MTFMfrac || mode==JKQTMathTextFracNode::MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor();
        ev2.fontSize=ev2.fontSize*getFracScalingFactor();
    } else  if (mode==JKQTMathTextFracNode::MTFMtfrac || mode==JKQTMathTextFracNode::MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*getFracScalingFactor()*0.7;
        ev2.fontSize=ev2.fontSize*getFracScalingFactor()*0.7;
    }


    const QFontMetricsF fmev1(ev1.getFont(parentMathText), painter.device());
    const QRectF AeTBR1=fmev1.tightBoundingRect("A");
    const double asc1=AeTBR1.height();
    const QFontMetricsF fmev2(ev2.getFont(parentMathText), painter.device());
    const QRectF AeTBR2=fmev2.tightBoundingRect("A");
    const double asc2=AeTBR2.height();

    JKQTMathTextNodeSize size1=child1->getSize(painter, ev1);
    JKQTMathTextNodeSize size2=child2->getSize(painter, ev2);
    if (asc1>size1.baselineHeight) {
        const double oldDescent=size1.overallHeight-size1.baselineHeight;
        size1.baselineHeight=asc1;
        size1.overallHeight=size1.baselineHeight+oldDescent;
    }
    if (asc2>size2.baselineHeight) {
        const double oldDescent=size2.overallHeight-size2.baselineHeight;
        size2.baselineHeight=asc2;
        size2.overallHeight=size2.baselineHeight+oldDescent;
    }
    const double ascent1=size1.baselineHeight;
    const double descent1=size1.overallHeight-size1.baselineHeight;
    const double ascent2=size2.baselineHeight;
    const double descent2=size2.overallHeight-size2.baselineHeight;

    const double yline=y-xheight*0.5;


    //double overallHeight=size1.overallHeight+size2.overallHeight+xh;
    //double baselineHeight=3.0*xh/2.0+size1.overallHeight;
    const double maxWidth=qMax(size1.width, size2.width);
    double deltaWidth=0;

    QPen p=painter.pen();
    p.setColor(ev1.color);
    p.setStyle(Qt::SolidLine);
    p.setWidthF(qMax(parentMathText->ABS_MIN_LINEWIDTH, linewideth));
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(p);
    if (mode==JKQTMathTextFracNode::MTFMfrac || mode==JKQTMathTextFracNode::MTFMdfrac || mode==JKQTMathTextFracNode::MTFMtfrac) {
        deltaWidth=xwidth/2.0;
        const QLineF l(x+p.widthF()*2.0, yline, x+maxWidth+deltaWidth-p.widthF()*2.0, yline);
        if (l.length()>0) painter.drawLine(l);
        child1->draw(painter, x+deltaWidth/2.0+(maxWidth-size1.width)/2.0, yline-xheight*(parentMathText->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+deltaWidth/2.0+(maxWidth-size2.width)/2.0, yline+xheight*(parentMathText->getFracShiftFactor())+ascent2, ev2);
    } else if (mode==JKQTMathTextFracNode::MTFMstackrel) {
        child1->draw(painter, x+(maxWidth-size1.width)/2.0, yline-xheight*(parentMathText->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+(maxWidth-size2.width)/2.0, yline+xheight*(parentMathText->getFracShiftFactor())+ascent2, ev2);
    } else if (mode==JKQTMathTextFracNode::MTFMstfrac || mode==JKQTMathTextFracNode::MTFMsfrac) {
        deltaWidth=xwidth*0.666;
        child1->draw(painter, x, yline, ev1);
        child2->draw(painter, x+size1.width+deltaWidth, y, ev2);
        const QLineF l(x+size1.width+deltaWidth, y-Mheight, x+size1.width, y+(qheight-xheight));
        if (l.length()>0) painter.drawLine(l);
    } else if (mode==JKQTMathTextFracNode::MTFMunderset) {
        child1->draw(painter, x+xwidth/2.0+(maxWidth-size1.width)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-size2.width)/2.0, y+descent1+xheight/6.0+ascent2, ev2);
        deltaWidth=xwidth;
    } else if (mode==JKQTMathTextFracNode::MTFMunderbrace) {
        const double ybrace=y+descent1+braceseparation+braceheight/2.0;
        const double ybot=y+descent1+2.0*braceseparation+braceheight+ascent2;
        {
            const QPainterPath path=JKQTMathTextMakeHBracePath(x+xwidth/2.0+(size1.width)/2.0, ybrace, maxWidth, braceheight, p.width());
            QPen plocal=p;
            plocal.setWidthF(0.0001);
            painter.fillPath(path, QBrush(ev1.color));
        }
        child1->draw(painter, x+xwidth/2.0+(maxWidth-size1.width)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-size2.width)/2.0, ybot, ev2);
        deltaWidth=xwidth;
    } else if (mode==JKQTMathTextFracNode::MTFMunderbracket) {
        const double ybrace=y+descent1+braceseparation+braceheight/2.0;
        const double ybot=y+descent1+2.0*braceseparation+braceheight+ascent2;
        {
            QPainterPath path;
            const double y1=ybrace-braceheight/2.0;
            const double y2=ybrace+braceheight/2.0;
            const double x1=x+xwidth/2.0+p.width()/2.0;
            const double x2=x+xwidth/2.0+maxWidth-p.width()/2.0;
            path.moveTo(x1, y1);
            path.lineTo(x1, y2);
            path.lineTo(x2, y2);
            path.lineTo(x2, y1);
            painter.drawPath(path);
        }
        child1->draw(painter, x+xwidth/2.0+(maxWidth-size1.width)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-size2.width)/2.0, ybot, ev2);
        deltaWidth=xwidth;
    } else if (mode==JKQTMathTextFracNode::MTFMoverset) {
        child1->draw(painter, x+xwidth/2.0+(maxWidth-size1.width)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-size2.width)/2.0, y-ascent1-xheight/6.0-descent2, ev2);
        deltaWidth=xwidth;
    } else if (mode==JKQTMathTextFracNode::MTFMoverbrace) {
        const double ybrace=y-ascent1-braceheight/2.0-braceseparation;
        const double ytop=y-ascent1-2.0*braceseparation-braceheight-descent2;
        {
            painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(x+xwidth/2.0+(size1.width)/2.0, ybrace);
            painter.rotate(180);
            const QPainterPath path=JKQTMathTextMakeHBracePath(0,0, maxWidth, braceheight, p.widthF());
            QPen plocal=p;
            plocal.setWidthF(0.0001);
            painter.fillPath(path, QBrush(ev1.color));
        }

        child1->draw(painter, x+xwidth/2.0+(maxWidth-size1.width)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-size2.width)/2.0, ytop, ev2);
        deltaWidth=xwidth;
    } else if (mode==JKQTMathTextFracNode::MTFMoverbracket) {
        const double ybrace=y-ascent1-braceheight/2.0-braceseparation;
        const double ytop=y-ascent1-2.0*braceseparation-braceheight-descent2;
        {
            QPainterPath path;
            const double y1=ybrace+braceheight/2.0;
            const double y2=ybrace-braceheight/2.0;
            const double x1=x+xwidth/2.0+p.width()/2.0;
            const double x2=x+xwidth/2.0+maxWidth-p.width()/2.0;
            path.moveTo(x1, y1);
            path.lineTo(x1, y2);
            path.lineTo(x2, y2);
            path.lineTo(x2, y1);
            painter.drawPath(path);
        }
        child1->draw(painter, x+xwidth/2.0+(maxWidth-size1.width)/2.0, y, ev1);
        child2->draw(painter, x+xwidth/2.0+(maxWidth-size2.width)/2.0, ytop, ev2);
        deltaWidth=xwidth;
    }


    if (mode==JKQTMathTextFracNode::MTFMstfrac || mode==JKQTMathTextFracNode::MTFMsfrac) return x+size1.width+size2.width+deltaWidth;
    else return x+maxWidth+deltaWidth;

}

bool JKQTMathTextFracNode::toHtml(QString &/*html*/, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) const {
    bool ok=false;


    return ok;
}

JKQTMathTextFracNode::FracType JKQTMathTextFracNode::getMode() const {
    return this->mode;
}
