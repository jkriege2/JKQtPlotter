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



#include "jkqtmathtext/nodes/jkqtmathtextsubsupernode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>






JKQTMathTextSuperscriptNode::JKQTMathTextSuperscriptNode(JKQTMathText* _parent, JKQTMathTextNode* child):
    JKQTMathTextSingleChildNode(child,_parent)
{
}

JKQTMathTextSuperscriptNode::~JKQTMathTextSuperscriptNode() {
}

JKQTMathTextNodeSize JKQTMathTextSuperscriptNode::getSizeWithSpecialPlacement(QPainter& painter, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const{
    JKQTMathTextNodeSize s;
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();
    const QFont fnt=currentEv.getFont(parentMathText);
    //const QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());
    const JKQTMathTextNodeSize cs=getChild()->getSize(painter, ev);
    const double childDescent=cs.getDescent();
    const QRectF tbr=JKQTMathTextGetTightBoundingRect(fnt, "x", painter.device());
    const double xh=tbr.height();

    double shiftToChildBottom=parentMathText->getSuperShiftFactor()*xh;

    if (prevNodeSizeForSpecialPlacement!=nullptr) {
        const double modifiedShift=prevNodeSizeForSpecialPlacement->baselineHeight-childDescent-parentMathText->getSpecialSuperShiftFactor()*cs.baselineHeight-childDescent;
        if (modifiedShift>shiftToChildBottom) shiftToChildBottom=modifiedShift;
    }

    s.baselineHeight=s.overallHeight=cs.overallHeight+shiftToChildBottom;
    s.width=cs.width;
    if (prevNodeSizeForSpecialPlacement!=nullptr) s.strikeoutPos=prevNodeSizeForSpecialPlacement->strikeoutPos;
    else s.strikeoutPos=JKQTMathTextGetFontStrikoutPos(fnt, painter.device());
    return s;
}

JKQTMathTextNodeSize JKQTMathTextSuperscriptNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv)  const
{
    return getSizeWithSpecialPlacement(painter, currentEv, nullptr);
}

double JKQTMathTextSuperscriptNode::drawWithSpecialPlacement(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSuperscriptNode[]::draw()"));
#endif
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();

    const JKQTMathTextNodeSize cs=getChild()->getSize(painter, ev);
    const QFont fnt=currentEv.getFont(parentMathText);
    const QRectF tbr=JKQTMathTextGetTightBoundingRect(fnt, "x", painter.device());
    const double xh=tbr.height();
    //qDebug()<<"x="<<x<<" prevNodeSizeForSpecialPlacement="<<prevNodeSizeForSpecialPlacement<<" font="<<currentEv.getFont(parentMathText);
    //QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());
    const double childDescent=cs.overallHeight-cs.baselineHeight;
    double shiftToChildBottom=parentMathText->getSuperShiftFactor()*xh;

    if (prevNodeSizeForSpecialPlacement!=nullptr) {
        const double modifiedShift=prevNodeSizeForSpecialPlacement->baselineHeight-childDescent-parentMathText->getSpecialSuperShiftFactor()*cs.baselineHeight-childDescent;
        if (modifiedShift>shiftToChildBottom) shiftToChildBottom=modifiedShift;
    }

    double xx=x;

    return getChild()->draw(painter, xx, y-(shiftToChildBottom+childDescent), ev);
}

double JKQTMathTextSuperscriptNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv) const
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSuperscriptNode[]::draw()"));
#endif
    return drawWithSpecialPlacement(painter, x, y, currentEv, nullptr);
}


QString JKQTMathTextSuperscriptNode::getTypeName() const
{
    return "MTsuperscriptNode";
}


bool JKQTMathTextSuperscriptNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const
{
    html=html+"<sup>";
    bool ok=getChild()->toHtml(html, currentEv, defaultEv);
    html=html+"</sup>";
    return ok;
}







JKQTMathTextSubscriptNode::JKQTMathTextSubscriptNode(JKQTMathText* _parent, JKQTMathTextNode* child):
    JKQTMathTextSingleChildNode(child, _parent)
{
}

JKQTMathTextSubscriptNode::~JKQTMathTextSubscriptNode() {

}

JKQTMathTextNodeSize JKQTMathTextSubscriptNode::getSizeWithSpecialPlacement(QPainter& painter, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const {
    JKQTMathTextNodeSize s;
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();
    const QFont f(ev.getFont(parentMathText));
    //const QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());

    const JKQTMathTextNodeSize cs=getChild()->getSize(painter, ev);
    //const double childDescent=cs.overallHeight-cs.baselineHeight;
    double shift_to_childBaseline=cs.baselineHeight-parentMathText->getSubShiftFactor()*JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height();

    if (prevNodeSizeForSpecialPlacement!=nullptr) {
        //qDebug()<<"oldshift="<<shift<<", prevNodeSize->overallHeight="<<prevNodeSize->overallHeight<<", prevNodeSize->baselineHeight="<<prevNodeSize->baselineHeight;
        const double parentDescent=prevNodeSizeForSpecialPlacement->overallHeight-prevNodeSizeForSpecialPlacement->baselineHeight;
        const double newShift=parentDescent+parentMathText->getSpecialSubShiftFactor()*cs.baselineHeight;
        if (newShift>shift_to_childBaseline) shift_to_childBaseline=newShift;
        //qDebug()<<"newshift="<<shift;
    }

    s.baselineHeight=cs.baselineHeight-shift_to_childBaseline;
    s.overallHeight=cs.overallHeight;
    if (prevNodeSizeForSpecialPlacement!=nullptr) s.strikeoutPos=prevNodeSizeForSpecialPlacement->strikeoutPos;
    else s.strikeoutPos=JKQTMathTextGetFontStrikoutPos(f, painter.device());
    s.width=cs.width;
    return s;
}

JKQTMathTextNodeSize JKQTMathTextSubscriptNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
    return getSizeWithSpecialPlacement(painter, currentEv, nullptr);
}

double JKQTMathTextSubscriptNode::drawWithSpecialPlacement(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSubscriptNode[]::draw()"));
#endif
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();
    QFont f=ev.getFont(parentMathText);
    //const QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());

    const JKQTMathTextNodeSize cs=getChild()->getSize(painter, ev);
    double shift_to_childBaseline=cs.baselineHeight-parentMathText->getSubShiftFactor()*JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height();

    if (prevNodeSizeForSpecialPlacement!=nullptr) {
        //qDebug()<<"oldshift="<<shift<<", prevNodeSize->overallHeight="<<prevNodeSize->overallHeight<<", prevNodeSize->baselineHeight="<<prevNodeSize->baselineHeight;
        const double parentDescent=prevNodeSizeForSpecialPlacement->overallHeight-prevNodeSizeForSpecialPlacement->baselineHeight;
        const double newShift=parentDescent+parentMathText->getSpecialSubShiftFactor()*cs.baselineHeight;
        if (newShift>shift_to_childBaseline) shift_to_childBaseline=newShift;
        //qDebug()<<"newshift="<<shift;
    }

    //qDebug()<<"baselineHeight="<<baselineHeight<<", overallHeight="<<overallHeight<<", strikeoutPos="<<strikeoutPos;
    //qDebug()<<"shift="<<shift<<", yshift="<<yshift;
    double xx=x;
    return getChild()->draw(painter, xx, y+shift_to_childBaseline, ev);
}

double JKQTMathTextSubscriptNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv) const
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSubscriptNode[]::draw()"));
#endif
    return drawWithSpecialPlacement(painter, x, y, currentEv, nullptr);
}

QString JKQTMathTextSubscriptNode::getTypeName() const
{
    return "MTsubscriptNode";
}

bool JKQTMathTextSubscriptNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
    html=html+"<sub>";
    bool ok=getChild()->toHtml(html, currentEv, defaultEv);
    html=html+"</sub>";
    return ok;
}
