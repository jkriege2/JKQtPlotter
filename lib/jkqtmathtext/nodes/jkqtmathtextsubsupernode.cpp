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

void JKQTMathTextSuperscriptNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();
    const QFontMetricsF fm(currentEv.getFont(parentMathText), painter.device());
    const QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());
    double cStrikeoutPos=0, cWidth=0, cBaselineHeight=0, cOverallHeight=0;
    getChild()->getSize(painter, ev, cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos);
    const double childDescent=cOverallHeight-cBaselineHeight;
    double shiftToChildBottom=parentMathText->getSuperShiftFactor()*fm.xHeight();

    if (prevNodeSize!=nullptr) {
        const double modifiedShift=prevNodeSize->baselineHeight-childDescent-parentMathText->getSpecialSuperShiftFactor()*cBaselineHeight-childDescent;
        if (modifiedShift>shiftToChildBottom) shiftToChildBottom=modifiedShift;
    }

    baselineHeight=overallHeight=cOverallHeight+shiftToChildBottom;
    width=cWidth;
    if (prevNodeSize!=nullptr) strikeoutPos=prevNodeSize->strikeoutPos;
    else strikeoutPos=fm.strikeOutPos();
    if (currentEv.italic && prevNodeSize==nullptr) width=width+double(fm.boundingRect(' ').width())*parentMathText->getItalicCorrectionFactor();
}

double JKQTMathTextSuperscriptNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();

    double cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos;
    getChild()->getSize(painter, ev, cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos);

    const QFontMetricsF fm(currentEv.getFont(parentMathText), painter.device());
    QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());
    const double childDescent=cOverallHeight-cBaselineHeight;
    double shiftToChildBottom=parentMathText->getSuperShiftFactor()*fm.xHeight();

    if (prevNodeSize!=nullptr) {
        const double modifiedShift=prevNodeSize->baselineHeight-childDescent-parentMathText->getSpecialSuperShiftFactor()*cBaselineHeight-childDescent;
        if (modifiedShift>shiftToChildBottom) shiftToChildBottom=modifiedShift;
    }

    double xx=x;
    if (currentEv.italic && prevNodeSize==nullptr) xx=xx+double(fm.boundingRect(' ').width())*parentMathText->getItalicCorrectionFactor();

    return getChild()->draw(painter, xx, y-(shiftToChildBottom+childDescent), ev);//+0.5*fm.boundingRect("A").width();
}


QString JKQTMathTextSuperscriptNode::getTypeName() const
{
    return "MTsuperscriptNode";
}


bool JKQTMathTextSuperscriptNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv)
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

void JKQTMathTextSubscriptNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();
    const QFontMetricsF fm(ev.getFont(parentMathText), painter.device());
    const QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());

    double cWidth=0, cBaselineHeight=0, cOverallHeight=0, cStrikeoutPos=0;
    getChild()->getSize(painter, ev, cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos);
    //const double childDescent=cOverallHeight-cBaselineHeight;
    double shift_to_childBaseline=cBaselineHeight-parentMathText->getSubShiftFactor()*fm.xHeight();

    if (prevNodeSize!=nullptr) {
        //qDebug()<<"oldshift="<<shift<<", prevNodeSize->overallHeight="<<prevNodeSize->overallHeight<<", prevNodeSize->baselineHeight="<<prevNodeSize->baselineHeight;
        const double parentDescent=prevNodeSize->overallHeight-prevNodeSize->baselineHeight;
        const double newShift=parentDescent+parentMathText->getSpecialSubShiftFactor()*cBaselineHeight;
        if (newShift>shift_to_childBaseline) shift_to_childBaseline=newShift;
        //qDebug()<<"newshift="<<shift;
    }

    baselineHeight=cBaselineHeight-shift_to_childBaseline;
    overallHeight=cOverallHeight;
    if (prevNodeSize!=nullptr) strikeoutPos=prevNodeSize->strikeoutPos;
    else strikeoutPos=fm.strikeOutPos();
    width=cWidth;
    if (currentEv.italic && prevNodeSize==nullptr) width=width-double(fm.boundingRect(' ').width())*parentMathText->getItalicCorrectionFactor();
}

double JKQTMathTextSubscriptNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parentMathText->getSubsuperSizeFactor();
    const QFontMetricsF fm(ev.getFont(parentMathText), painter.device());
    const QRectF tbr_of_letterM=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parentMathText), "M", painter.device());

    double cWidth=0, cBaselineHeight=0, cOverallHeight=0, cStrikeoutPos=0;
    getChild()->getSize(painter, ev, cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos);
    double shift_to_childBaseline=cBaselineHeight-parentMathText->getSubShiftFactor()*fm.xHeight();

    if (prevNodeSize!=nullptr) {
        //qDebug()<<"oldshift="<<shift<<", prevNodeSize->overallHeight="<<prevNodeSize->overallHeight<<", prevNodeSize->baselineHeight="<<prevNodeSize->baselineHeight;
        const double parentDescent=prevNodeSize->overallHeight-prevNodeSize->baselineHeight;
        const double newShift=parentDescent+parentMathText->getSpecialSubShiftFactor()*cBaselineHeight;
        if (newShift>shift_to_childBaseline) shift_to_childBaseline=newShift;
        //qDebug()<<"newshift="<<shift;
    }

    //qDebug()<<"baselineHeight="<<baselineHeight<<", overallHeight="<<overallHeight<<", strikeoutPos="<<strikeoutPos;
    //qDebug()<<"shift="<<shift<<", yshift="<<yshift;
    double xx=x;
    if (currentEv.italic && prevNodeSize==nullptr) xx=xx-double(fm.boundingRect(' ').width())*parentMathText->getItalicCorrectionFactor();
    return getChild()->draw(painter, xx, y+shift_to_childBaseline, ev);//+0.5*fm.boundingRect("A").width();
}

QString JKQTMathTextSubscriptNode::getTypeName() const
{
    return "MTsubscriptNode";
}

bool JKQTMathTextSubscriptNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    html=html+"<sub>";
    bool ok=getChild()->toHtml(html, currentEv, defaultEv);
    html=html+"</sub>";
    return ok;
}
