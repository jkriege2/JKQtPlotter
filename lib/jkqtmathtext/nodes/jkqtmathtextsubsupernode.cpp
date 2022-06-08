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
    JKQTMathTextNode(_parent)
{
    this->child=child;
}

JKQTMathTextSuperscriptNode::~JKQTMathTextSuperscriptNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathTextSuperscriptNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    QRectF tbr=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parent), "M", painter.device());
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    double shift=parent->getSuperShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->baselineHeight>tbr.height()) {
        shift=prevNodeSize->baselineHeight-(overallHeight-baselineHeight)-shift;
    }

    double yshift=shift+overallHeight-baselineHeight;
    baselineHeight=overallHeight=overallHeight+shift;
    strikeoutPos=strikeoutPos-yshift;
    if (currentEv.italic && prevNodeSize==nullptr) width=width+double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();
}

double JKQTMathTextSuperscriptNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();

    double cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos;
    child->getSize(painter, ev, cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos);

    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    QRectF tbr=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parent), "M", painter.device());
    double shift=parent->getSuperShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->baselineHeight>tbr.height()) {
        shift=prevNodeSize->baselineHeight-(cOverallHeight-cBaselineHeight)-shift;
    }

    double yshift=shift+cOverallHeight-cBaselineHeight;
    double xx=x;
    if (currentEv.italic && prevNodeSize==nullptr) xx=xx+double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();

    return child->draw(painter, xx, y-yshift, ev);//+0.5*fm.boundingRect("A").width();
}

JKQTMathTextNode *JKQTMathTextSuperscriptNode::getChild() const {
    return this->child;
}


QString JKQTMathTextSuperscriptNode::getTypeName() const
{
    return "MTsuperscriptNode";
}


bool JKQTMathTextSuperscriptNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv)
{
    html=html+"<sup>";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"</sup>";
    return ok;
}

void JKQTMathTextSuperscriptNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}







JKQTMathTextSubscriptNode::JKQTMathTextSubscriptNode(JKQTMathText* _parent, JKQTMathTextNode* child):
    JKQTMathTextNode(_parent)
{
    this->child=child;
}

JKQTMathTextSubscriptNode::~JKQTMathTextSubscriptNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathTextSubscriptNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();

    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);

    QFontMetricsF fm(ev.getFont(parent), painter.device());
    QRectF tbr=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parent), "M", painter.device());
    double shift=parent->getSubShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->overallHeight-prevNodeSize->baselineHeight>shift) {
        shift=-1.0*(prevNodeSize->overallHeight-prevNodeSize->baselineHeight-shift);
    }

    double yshift=baselineHeight-shift;
    baselineHeight=shift;
    strikeoutPos=fm.strikeOutPos()+yshift;
    if (currentEv.italic && prevNodeSize==nullptr) width=width-double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();
}

double JKQTMathTextSubscriptNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();
    QFontMetricsF fm(ev.getFont(parent), painter.device());
    QRectF tbr=JKQTMathTextGetTightBoundingRect(currentEv.getFont(parent), "M", painter.device());

    double width=0, baselineHeight=0, overallHeight=0, strikeoutPos=0;
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    double shift=parent->getSubShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->overallHeight-prevNodeSize->baselineHeight>shift) {
        //qDebug()<<"oldshift="<<shift<<", prevNodeSize->overallHeight="<<prevNodeSize->overallHeight<<", prevNodeSize->baselineHeight="<<prevNodeSize->baselineHeight;
        shift=-1.0*(prevNodeSize->overallHeight-prevNodeSize->baselineHeight-shift);
        //qDebug()<<"newshift="<<shift;
    }

    double yshift=baselineHeight-shift;
    //qDebug()<<"baselineHeight="<<baselineHeight<<", overallHeight="<<overallHeight<<", strikeoutPos="<<strikeoutPos;
    //qDebug()<<"shift="<<shift<<", yshift="<<yshift;
    double xx=x;
    if (currentEv.italic && prevNodeSize==nullptr) xx=xx-double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();
    return child->draw(painter, xx, y+yshift, ev);//+0.5*fm.boundingRect("A").width();
}

QString JKQTMathTextSubscriptNode::getTypeName() const
{
    return "MTsubscriptNode";
}

JKQTMathTextNode *JKQTMathTextSubscriptNode::getChild() const {
    return this->child;
}


bool JKQTMathTextSubscriptNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    html=html+"<sub>";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"</sub>";
    return ok;
}

void JKQTMathTextSubscriptNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}



