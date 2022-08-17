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



#include "jkqtmathtext/nodes/jkqtmathtextlistnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsymbolnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextbracenode.h"
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


JKQTMathTextHorizontalListNode::JKQTMathTextHorizontalListNode(JKQTMathText* _parent):
    JKQTMathTextMultiChildNode(_parent)
{
    nodes.clear();
    // these operations cause sub/sup script to be typeset over/under the operator, not right besides!
}

JKQTMathTextHorizontalListNode::~JKQTMathTextHorizontalListNode() {
    clearChildrenImpl(true);
}

QString JKQTMathTextHorizontalListNode::getTypeName() const
{
    return "MTHorizontalListNode";
}

JKQTMathTextNodeSize JKQTMathTextHorizontalListNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    JKQTMathTextNodeSize outSize;
    const QFont f=currentEv.getFont(parentMathText);
    const QFontMetricsF fm(f);
    const double subsupershift=fm.xHeight()*parentMathText->getOperatorsubsuperDistanceFactor();
    const double subsuperextrawidth=fm.boundingRect('x').width()*parentMathText->getOperatorsubsuperExtraSpaceFactor();
    const double subsuperSpecialModeAscent=fm.ascent()*parentMathText->getSubsuperModeSelectionBySizeFactor();
    const double subsuperSpecialModeDecent=fm.descent()*parentMathText->getSubsuperModeSelectionBySizeFactor();
    const double spaceWidth=fm.boundingRect(' ').width();

    //QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());


    double xnew=0;
    //bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        JKQTMathTextSymbolNode::NodeSize prevNodeSize;
        JKQTMathTextNodeSize* prevNodeSizePtrForSubscript=nullptr;
        JKQTMathTextNodeSize* prevNodeSizePtrForSuperscript=nullptr;

        if (i>0) {
            JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i-1]);
            if (symbN) prevNodeSize=symbN->getSymbolSize(painter, currentEv);
            else prevNodeSize = nodes[i-1]->getSize(painter, currentEv);
            const double prevAscent=prevNodeSize.baselineHeight;
            const double prevDescent=prevNodeSize.overallHeight-prevNodeSize.baselineHeight;
            const bool shouldUseSpecialSubscriptMode=prevAscent>=subsuperSpecialModeAscent;
            const bool shouldUseSpecialSuperscriptMode=prevDescent>=subsuperSpecialModeDecent;
            if (shouldUseSpecialSubscriptMode)  prevNodeSizePtrForSubscript=&prevNodeSize;
            if (shouldUseSpecialSuperscriptMode)  prevNodeSizePtrForSuperscript=&prevNodeSize;
        }
        const double subscript_xcorrection=prevNodeSize.baselineXCorrection+fm.lineWidth()*0.5;

        JKQTMathTextSuperscriptNode* nodeI_SuperScript=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i]);
        JKQTMathTextSubscriptNode* nodeI_SubScript=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i]);

        bool doDraw=true;
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (nodeI_SuperScript) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                JKQTMathTextSubscriptNode* nodeISucc_SubScript=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1]);
                if (nodeISucc_SubScript) { // is this subscript?
                    const JKQTMathTextNodeSize ns=nodeI_SuperScript->getSizeWithSpecialPlacement(painter, currentEv, prevNodeSizePtrForSuperscript);

                    if (ns.baselineHeight>outSize.baselineHeight) {
                        outSize.overallHeight=outSize.overallHeight+ns.baselineHeight-outSize.baselineHeight;
                        outSize.baselineHeight=ns.baselineHeight;
                        outSize.strikeoutPos=ns.strikeoutPos;
                    }
                    if (outSize.baselineHeight+ns.getDescent()>outSize.overallHeight) {
                        outSize.overallHeight=outSize.baselineHeight+ns.getDescent();
                        outSize.strikeoutPos=ns.strikeoutPos;
                    }

                    i++;
                    const JKQTMathTextNodeSize ns2=nodeISucc_SubScript->getSizeWithSpecialPlacement(painter, currentEv, prevNodeSizePtrForSubscript);
                    //qDebug()<<"super_sub:   sub: "<<nodes[i]->getTypeName()<<"  w2="<<w2<<" bh"<<bh<<" oh="<<oh<<" sp="<<sp;
                    if (ns2.baselineHeight>outSize.baselineHeight) {
                        outSize.overallHeight=outSize.overallHeight+ns2.baselineHeight-outSize.baselineHeight;
                        outSize.baselineHeight=ns2.baselineHeight;
                        outSize.strikeoutPos=ns2.strikeoutPos;
                    }
                    if (outSize.baselineHeight+ns2.getDescent()>outSize.overallHeight) {
                        outSize.overallHeight=outSize.baselineHeight+ns2.getDescent();
                        outSize.strikeoutPos=ns2.strikeoutPos;
                    }
                    xnew+=qMax(ns.width+spaceWidth, ns2.width+subscript_xcorrection);

                    doDraw=false;
                    //qDebug()<<"### super+sub";
                    //qDebug()<<"### subsupop: super+sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (nodeI_SubScript) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                JKQTMathTextSuperscriptNode* nodeISucc_SuperScript=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1]);
                if (nodeISucc_SuperScript) { // is this subscript?
                    const JKQTMathTextNodeSize ns=nodeI_SubScript->getSizeWithSpecialPlacement(painter, currentEv, prevNodeSizePtrForSubscript);
                    if (ns.baselineHeight>outSize.baselineHeight) {
                        outSize.overallHeight=outSize.overallHeight+ns.baselineHeight-outSize.baselineHeight;
                        outSize.baselineHeight=ns.baselineHeight;
                        outSize.strikeoutPos=ns.strikeoutPos;
                    }
                    if (outSize.baselineHeight+ns.overallHeight-ns.baselineHeight>outSize.overallHeight) {
                        outSize.overallHeight=outSize.baselineHeight+ns.overallHeight-ns.baselineHeight;
                        outSize.strikeoutPos=ns.strikeoutPos;
                    }

                    i++;
                    const JKQTMathTextNodeSize ns2=nodeISucc_SuperScript->getSizeWithSpecialPlacement(painter, currentEv, prevNodeSizePtrForSuperscript);
                    if (ns2.baselineHeight>outSize.baselineHeight) {
                        outSize.overallHeight=outSize.overallHeight+ns2.baselineHeight-outSize.baselineHeight;
                        outSize.baselineHeight=ns2.baselineHeight;
                        outSize.strikeoutPos=ns2.strikeoutPos;
                    }
                    if (outSize.baselineHeight+ns2.overallHeight-ns2.baselineHeight>outSize.overallHeight) {
                        outSize.overallHeight=outSize.baselineHeight+ns2.overallHeight-ns2.baselineHeight;
                        outSize.strikeoutPos=ns2.strikeoutPos;
                    }
                    xnew+=qMax(ns.width+subscript_xcorrection, ns2.width+spaceWidth);


                    doDraw=false;
                    //qDebug()<<"### sub+super";
                    //qDebug()<<"### subsupop: sub+super1   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            } else {
                const JKQTMathTextNodeSize ns=nodes[i]->getSize(painter, currentEv);
                if (ns.baselineHeight>outSize.baselineHeight) {
                    outSize.overallHeight=outSize.overallHeight+ns.baselineHeight-outSize.baselineHeight;
                    outSize.baselineHeight=ns.baselineHeight;
                    outSize.strikeoutPos=ns.strikeoutPos;
                }
                if (outSize.baselineHeight+ns.getDescent()>outSize.overallHeight) {
                    outSize.overallHeight=outSize.baselineHeight+ns.getDescent();
                    outSize.strikeoutPos=ns.strikeoutPos;
                }
                xnew+=ns.width+subscript_xcorrection;
                doDraw=false;

            }
        } else {
            if (nodes[i]->isSubSuperscriptAboveBelowNode()) {
                JKQTMathTextSubscriptNode* subn=nullptr;
                JKQTMathTextSuperscriptNode* supn=nullptr;
                JKQTMathTextSubscriptNode* subn2=nullptr;
                JKQTMathTextSuperscriptNode* supn2=nullptr;
                if (i+1<nodes.size()) {
                    subn=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1]);
                    supn=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1]);
                }
                if (i+2<nodes.size()) {
                    subn2=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+2]);
                    supn2=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+2]);
                }
                //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                if ((subn && supn2) || (subn2 && supn)) { // is this subscript and superscript  or superscript and subscript?
                    if (!subn) subn=subn2;
                    if (!supn) supn=supn2;
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    const JKQTMathTextNodeSize ns=nodes[i]->getSize(painter, currentEv);
                    //qDebug()<<"sub_super:   node: "<<nodes[i]->getTypeName()<<"  ns.width="<<ns.width<<" bh"<<ns.baselineHeight<<" oh="<<ns.overallHeight<<" sp="<<ns.strikeoutPos;
                    const JKQTMathTextNodeSize sub=subn->getChild()->getSize(painter, ev);
                    //qDebug()<<"sub_super:   node: "<<subn->getTypeName()<<"  w2="<<w2<<" bh2"<<bh2<<" oh2="<<oh2<<" sp2="<<sp2;
                    const JKQTMathTextNodeSize sup=supn->getChild()->getSize(painter, ev);
                    //qDebug()<<"sub_super:   node: "<<supn->getTypeName()<<"  w3="<<w3<<" bh3"<<bh3<<" oh3="<<oh3<<" sp3="<<sp3;
                    const double descent1=ns.overallHeight-ns.baselineHeight;
                    //double d1=ns.overallHeight-ns.baselineHeight;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    const double neww=qMax(qMax(ns.width, sub.width+prevNodeSize.subSuperAboveBelowXCorrection), sup.width+prevNodeSize.subSuperAboveBelowXCorrection)+subsuperextrawidth;
                    const double newBaselineHeight=ns.baselineHeight+sup.overallHeight+subsupershift;
                    if (newBaselineHeight>outSize.baselineHeight) {
                        const double extraBaselineHeight=newBaselineHeight-outSize.baselineHeight;
                        outSize.baselineHeight=newBaselineHeight;
                        outSize.overallHeight=outSize.overallHeight+extraBaselineHeight;
                    }
                    const double newDescent=descent1+sub.overallHeight+subsupershift;
                    if (newDescent>outSize.overallHeight-outSize.baselineHeight) {
                        const double extraDescent=newDescent-(outSize.overallHeight-outSize.baselineHeight);
                        outSize.overallHeight=outSize.overallHeight+extraDescent;
                    }

                    i++;
                    i++;
                    doDraw=false;
                    xnew+=neww;
                    //qDebug()<<"### subsupop: sub+super2   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;

                } else if (subn) { // is this subscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    JKQTMathTextSymbolNode::NodeSize nodeSize;
                    if (symbN) {
                        nodeSize=symbN->getSymbolSize(painter, currentEv);
                    } else {
                        nodeSize=nodes[i]->getSize(painter, currentEv);
                    }
                    const  JKQTMathTextNodeSize sub=subn->getChild()->getSize(painter, ev);
                    const double descent1=nodeSize.getDescent();
                    //double d1=ns.overallHeight-ns.baselineHeight;
                    //double d2=oh2-bh2;

                    const double newDescent=descent1+sub.overallHeight+subsupershift;
                    if (newDescent>outSize.overallHeight-outSize.baselineHeight) {
                        const double extraDescent=newDescent-(outSize.overallHeight-outSize.baselineHeight);
                        outSize.overallHeight=outSize.overallHeight+extraDescent;
                    }
                    if (nodeSize.baselineHeight>outSize.baselineHeight) {
                        outSize.overallHeight=outSize.overallHeight+(nodeSize.baselineHeight-outSize.baselineHeight);
                        outSize.baselineHeight=nodeSize.baselineHeight;
                    }
                    const double neww=qMax(nodeSize.width, sub.width+nodeSize.subSuperAboveBelowXCorrection)+subsuperextrawidth;
                    i++;
                    doDraw=false;
                    xnew+=neww;
                    //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                } else if (supn) { // is this superscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    JKQTMathTextSymbolNode::NodeSize nodeSize;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        nodeSize=symbN->getSymbolSize(painter, currentEv);
                    } else {
                        nodeSize=nodes[i]->getSize(painter, currentEv);
                    }
                    const double descent1=nodeSize.getDescent();
                    const JKQTMathTextNodeSize sup=supn->getChild()->getSize(painter, ev);
                    //double d1=ns.overallHeight-ns.baselineHeight;
                    //double d2=oh2-bh2;

                    const double newBaselineHeight=nodeSize.baselineHeight+sup.overallHeight+subsupershift;
                    if (newBaselineHeight>outSize.baselineHeight) {
                        const double extraBaselineHeight=newBaselineHeight-outSize.baselineHeight;
                        outSize.baselineHeight=newBaselineHeight;
                        outSize.overallHeight=outSize.overallHeight+extraBaselineHeight;
                    }
                    if (descent1>outSize.overallHeight-outSize.baselineHeight) {
                        outSize.overallHeight=outSize.baselineHeight+descent1;
                    }

                    const double neww=qMax(nodeSize.width, sup.width+nodeSize.subSuperAboveBelowXCorrection)+subsuperextrawidth;
                    i++;
                    doDraw=false;
                    xnew+=neww;
                    //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            JKQTMathTextNodeSize ns;
            if (nodeI_SubScript) ns=nodeI_SubScript->getSizeWithSpecialPlacement(painter, currentEv, prevNodeSizePtrForSubscript);
            else if (nodeI_SuperScript) ns=nodeI_SuperScript->getSizeWithSpecialPlacement(painter, currentEv, prevNodeSizePtrForSuperscript);
            else ns=nodes[i]->getSize(painter, currentEv);
            const double cDescent=ns.getDescent();

            //qDebug()<<"### else:  ns.baselineHeight="<<ns.baselineHeight<<" baselineHeight="<<baselineHeight<<"   oh="<<oh<<" overallHeight="<<overallHeight;
            if (ns.baselineHeight>outSize.baselineHeight) {
                outSize.overallHeight=outSize.overallHeight-outSize.baselineHeight+ns.baselineHeight;
                outSize.baselineHeight=ns.baselineHeight;
                outSize.strikeoutPos=ns.strikeoutPos;
            }
            if (outSize.baselineHeight+cDescent>outSize.overallHeight) {
                outSize.overallHeight=outSize.baselineHeight+cDescent;
                outSize.strikeoutPos=ns.strikeoutPos;
            }
            outSize.baselineXCorrection=ns.baselineXCorrection;
            outSize.topXCorrection=ns.topXCorrection;
            //qDebug()<<"### subsupop: else   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;

            xnew+=ns.width;
            //qDebug()<<i<<xnew;
        }
         //wasBrace=dynamic_cast<JKQTMathTextBraceNode*>(nodes[i]);
    }
    outSize.width=xnew;

    return outSize;
}

double JKQTMathTextHorizontalListNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
    doDrawBoxes(painter, x, y, currentEv);
    double ynew=y;
    double xnew=x;
    //qDebug()<<"listNode: "<<currentEv.fontSize;
    const QFontMetricsF fm(currentEv.getFont(parentMathText));
    const double subsupershift=fm.xHeight()*parentMathText->getOperatorsubsuperDistanceFactor();
    const double subsuperextrawidth=fm.boundingRect('x').width()*parentMathText->getOperatorsubsuperExtraSpaceFactor();
    const double subsuperSpecialModeAscent=fm.ascent()*parentMathText->getSubsuperModeSelectionBySizeFactor();
    const double subsuperSpecialModeDecent=fm.descent()*parentMathText->getSubsuperModeSelectionBySizeFactor();
    for (int i=0; i<nodes.size(); i++) {
        bool doDraw=true;

        JKQTMathTextSymbolNode::NodeSize prevNodeSize;
        JKQTMathTextNodeSize* prevNodeSizePtrForSubscript=nullptr;
        JKQTMathTextNodeSize* prevNodeSizePtrForSuperscript=nullptr;

        if (i>0) {
            JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i-1]);
            if (symbN) {
                prevNodeSize=symbN->getSymbolSize(painter, currentEv);
            } else {
                prevNodeSize=nodes[i-1]->getSize(painter, currentEv);
            }
            const double prevAscent=prevNodeSize.baselineHeight;
            const double prevDescent=prevNodeSize.overallHeight-prevNodeSize.baselineHeight;
            const bool shouldUseSpecialSubscriptMode=prevAscent>=subsuperSpecialModeAscent;
            const bool shouldUseSpecialSuperscriptMode=prevDescent>=subsuperSpecialModeDecent;
            if (shouldUseSpecialSubscriptMode)  prevNodeSizePtrForSubscript=&prevNodeSize;
            if (shouldUseSpecialSuperscriptMode)  prevNodeSizePtrForSuperscript=&prevNodeSize;
        }
        const double subscript_xcorrection=prevNodeSize.baselineXCorrection+fm.lineWidth()*0.5;

        JKQTMathTextSuperscriptNode* nodeI_SuperScript=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i]);
        JKQTMathTextSubscriptNode* nodeI_SubScript=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i]);

        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (nodeI_SuperScript) {

            if (i+1<nodes.size()) { // is there one mor node behind?
                JKQTMathTextSubscriptNode* nodeISucc_SubScript=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1]);
                if (nodeISucc_SubScript) { // is this subscript?

                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    const double xnewidth=nodeI_SuperScript->drawWithSpecialPlacement(painter, xnew, ynew, currentEv, prevNodeSizePtrForSuperscript);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    const double xnew2=nodeISucc_SubScript->drawWithSpecialPlacement(painter, xnew+subscript_xcorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
                    //i++;
                    xnew=qMax(xnewidth, xnew2);
                    doDraw=false;
                }
            }
        } else if (nodeI_SubScript) {
            if (i+1<nodes.size()) { // is there one more node behind?
                JKQTMathTextSuperscriptNode* nodeISucc_SuperScript=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1]);
                if (nodeISucc_SuperScript) { // is this subscript?
                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    double xnewidth=nodeI_SubScript->drawWithSpecialPlacement(painter, xnew+subscript_xcorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodeISucc_SuperScript->drawWithSpecialPlacement(painter, xnew, ynew, currentEv, prevNodeSizePtrForSuperscript);
                    //i++;
                    xnew=qMax(xnewidth, xnew2);
                    doDraw=false;
                }
            } else {
                xnew=nodeI_SubScript->drawWithSpecialPlacement(painter, xnew+subscript_xcorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
                doDraw=false;
            }
        } else {

            if (nodes[i]->isSubSuperscriptAboveBelowNode()) {
                JKQTMathTextSubscriptNode* subn=nullptr;
                JKQTMathTextSuperscriptNode* supn=nullptr;
                JKQTMathTextSubscriptNode* subn2=nullptr;
                JKQTMathTextSuperscriptNode* supn2=nullptr;
                if (i+1<nodes.size()) {
                    subn=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1]);
                    supn=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1]);
                }
                if (i+2<nodes.size()) {
                    subn2=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+2]);
                    supn2=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+2]);
                }
                //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                if ((subn && supn2) || (subn2 && supn)) { // is this subscript and superscript  or superscript and subscript?
                    if (!subn) subn=subn2;
                    if (!supn) supn=supn2;
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    JKQTMathTextSymbolNode::NodeSize nodeSize;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        nodeSize=symbN->getSymbolSize(painter, currentEv);
                    } else {
                        nodeSize=nodes[i]->getSize(painter, currentEv);
                    }


                    const JKQTMathTextNodeSize sub=subn->getChild()->getSize(painter, ev);
                    const JKQTMathTextNodeSize sup=supn->getChild()->getSize(painter, ev);
                    const double descent1=nodeSize.getDescent();
                    //double d2=oh2-bh2;
                    const double descent3=sup.getDescent();

                    const double neww=qMax(qMax(nodeSize.width, sub.width), sup.width)+subsuperextrawidth;
                    //double xnewidth=
                    const double xn1=nodes[i]->draw(painter, xnew+(neww-nodeSize.width)/2.0, ynew, currentEv);
                    i++;
                    //double xnew2=
                    const double xnsub=subn->getChild()->draw(painter, xnew+(neww-sub.width)/2.0-nodeSize.subSuperAboveBelowXCorrection, ynew+sub.baselineHeight+descent1+subsupershift, ev);
                    i++;
                    //double xnew3=
                    const double xnsup=supn->getChild()->draw(painter, xnew+(neww-sup.width)/2.0+nodeSize.subSuperAboveBelowXCorrection, ynew-nodeSize.baselineHeight-descent3-subsupershift, ev);
                    doDraw=false;
                    xnew=qMax(qMax(xn1, xnsub), xnsup)+subsuperextrawidth/2.0;
                } else if (subn) { // is this subscript and no following superscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    JKQTMathTextSymbolNode::NodeSize nodeSize;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        nodeSize=symbN->getSymbolSize(painter, currentEv);
                    } else {
                        nodeSize=nodes[i]->getSize(painter, currentEv);
                    }
                    const JKQTMathTextNodeSize sub=subn->getChild()->getSize(painter, ev);
                    const double descent1=nodeSize.getDescent();
                    //double d2=oh2-bh2;

                    const double neww=qMax(nodeSize.width, sub.width)+subsuperextrawidth;
                    //double xnewidth=
                    const double xn1=nodes[i]->draw(painter, xnew+(neww-nodeSize.width)/2.0, ynew, currentEv);
                    i++;
                    //double xnew2=
                    const double xnsub=subn->getChild()->draw(painter, xnew+(neww-sub.width)/2.0-nodeSize.subSuperAboveBelowXCorrection, ynew+sub.baselineHeight+descent1+subsupershift, ev)+subsupershift;
                    doDraw=false;
                    //xnew+=w;
                    xnew=qMax(xnsub, xn1)+subsuperextrawidth/2.0;
                } else if (supn) { // is this superscript and no following subscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    JKQTMathTextSymbolNode::NodeSize nodeSize;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        nodeSize=symbN->getSymbolSize(painter, currentEv);
                    } else {
                        nodeSize=nodes[i]->getSize(painter, currentEv);
                    }
                    const JKQTMathTextNodeSize sup=supn->getChild()->getSize(painter, ev);
                    //double d1=ns.overallHeight-ns.baselineHeight;
                    //double d2=oh2-bh2;
                    const double descent3=sup.getDescent();

                    const double neww=qMax(nodeSize.width, sup.width)+subsuperextrawidth;
                    //double xnewidth=
                    const double xn1=nodes[i]->draw(painter, xnew+(neww-nodeSize.width)/2.0, ynew, currentEv);
                    i++;
                    //double xnew3=
                    const double xnsup=supn->getChild()->draw(painter, xnew+(neww-sup.width)/2.0+nodeSize.subSuperAboveBelowXCorrection, ynew-nodeSize.baselineHeight-descent3-subsupershift, ev);
                    doDraw=false;
                    xnew=qMax(xn1, xnsup)+subsuperextrawidth/2.0;
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            if (nodeI_SuperScript) xnew=nodeI_SuperScript->drawWithSpecialPlacement(painter, xnew, ynew, currentEv, prevNodeSizePtrForSuperscript);
            else if (nodeI_SubScript) xnew=nodeI_SubScript->drawWithSpecialPlacement(painter, xnew+subscript_xcorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
            else xnew=nodes[i]->draw(painter, xnew, ynew, currentEv);
        }
    }
    return xnew;
}

void JKQTMathTextHorizontalListNode::addChild(JKQTMathTextNode *n) {
    n->setParentNode(this);
    nodes.append(n);
}

bool JKQTMathTextHorizontalListNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
    bool ok=true;
    for (int i=0; i<nodes.size(); i++) {
        QString h="";
        ok = ok && nodes[i]->toHtml(h, currentEv, defaultEv);
        html=html+h;
    }
    return ok;
}

QList<JKQTMathTextNode *> JKQTMathTextHorizontalListNode::getChildren()  {
    return this->nodes;
}


int JKQTMathTextHorizontalListNode::childCount() const
{
    return nodes.size();
}


void JKQTMathTextHorizontalListNode::clearChildrenImpl(bool deleteChildren)
{
    if (deleteChildren) {
        for (int i=0; i<nodes.size(); i++) {
            delete nodes[i];
        }
    }
    nodes.clear();
}

void JKQTMathTextHorizontalListNode::clearChildren(bool deleteChildren)
{
    clearChildrenImpl(deleteChildren);
}

void JKQTMathTextHorizontalListNode::deleteChild(int i)
{
    if (i>=0 && i<nodes.size()) {
        delete nodes[i];
        nodes.removeAt(i);
    }
}

JKQTMathTextNode *JKQTMathTextHorizontalListNode::getChild(int i)
{
    return nodes[i];
}

const JKQTMathTextNode *JKQTMathTextHorizontalListNode::getChild(int i) const
{
    return nodes[i];
}

JKQTMathTextNode *JKQTMathTextHorizontalListNode::replaceChild(int i, JKQTMathTextNode *newChild)
{
    JKQTMathTextNode* c=nodes[i];
    nodes[i]=newChild;
    newChild->setParentNode(this);
    return c;
}


JKQTMathTextVerticalListNode::JKQTMathTextVerticalListNode(JKQTMathText *_parent, JKQTMathTextHorizontalAlignment _alignment, double _linespacingFactor, JKQTMathTextLineSpacingMode spacingMode_, JKQTMathTextVerticalOrientation _verticalOrientation):
    JKQTMathTextMultiChildNode(_parent),
    alignment(_alignment),
    lineSpacingFactor(_linespacingFactor),
    verticalOrientation(_verticalOrientation),
    spacingMode(spacingMode_)
{
    nodes.clear();
    // these operations cause sub/sup script to be typeset over/under the operator, not right besides!
}

JKQTMathTextVerticalListNode::~JKQTMathTextVerticalListNode() {
    clearChildrenImpl(true);
}

QString JKQTMathTextVerticalListNode::getTypeName() const
{
    return "MTVerticalListNode";
}

JKQTMathTextNodeSize JKQTMathTextVerticalListNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    return calcLayout(painter, currentEv);
}

JKQTMathTextVerticalListNode::LayoutInfo JKQTMathTextVerticalListNode::calcLayout(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
    LayoutInfo l;
    const QFontMetricsF fm(currentEv.getFont(parentMathText));
    const double linespacing=fm.lineSpacing()*lineSpacingFactor;
    const double fleading=fm.leading();
    const double synLeading=fm.lineWidth();
    const double lineLeading=((fabs(fleading)>1e-6)?fleading:synLeading)*lineSpacingFactor;

    if (nodes.size()<=0) {
        return l;
    }

    // from now on we have at least one child node!!!

    QList<double> widths, heights, ascents, descents, strikeouts;
    double heightSum=0;
    QList<double> ysFromFirstLine; // y-position of each line, where the first line is always at y=0 (i.e. ysFromFirstLine[0]==0)
    double y=0;
    for (int i=0; i<nodes.size(); i++) {
        const JKQTMathTextNodeSize loc=nodes[i]->getSize(painter, currentEv);

        if (i==0) {
            heightSum=loc.baselineHeight;
        } else if (i>0) {
            double deltaLine=0;
            if (spacingMode==MTSMMinimalSpacing) {
                deltaLine=descents.last()+lineLeading+loc.baselineHeight;
            } else if (spacingMode==MTSMDefaultSpacing) {
                deltaLine=qMax(linespacing, descents.last()+lineLeading+loc.baselineHeight);
            }
            heightSum=heightSum+deltaLine;
            y=y+deltaLine;
        }
        widths<<loc.width;
        l.width=qMax(l.width, loc.width);
        heights<<loc.overallHeight;
        ascents<<loc.baselineHeight;
        descents<<(loc.overallHeight-loc.baselineHeight);
        strikeouts<<loc.strikeoutPos;
        ysFromFirstLine<<y;
    }
    heightSum+=descents.last();

    l.overallHeight=heightSum;
    double y0=0;
    if (verticalOrientation==MTVOTop) {
        l.baselineHeight=0;
        l.strikeoutPos=0;
        y0=ascents.first();
    } else if (verticalOrientation==MTVOFirstLine) {
        l.baselineHeight=ascents.first();
        l.strikeoutPos=strikeouts.first();
        y0=0;
    } else if (verticalOrientation==MTVOCentered) {
        l.baselineHeight=heightSum/2.0;
        l.strikeoutPos=heightSum/4.0;
        y0=-heightSum/2.0+ascents.first();
    } else if (verticalOrientation==MTVOLastLine) {
        l.baselineHeight=heightSum-descents.last();
        l.strikeoutPos=strikeouts.last();
        y0=-(heightSum-ascents.first()-descents.last());
    } else if (verticalOrientation==MTVOBottom) {
        l.baselineHeight=heightSum;
        l.strikeoutPos=0;
        y0=-(heightSum-ascents.first());
    }
    for (int i=0; i<nodes.size(); i++) {
        double x=0;
        if (alignment==MTHARight) x=l.width-widths[i];
        else if (alignment==MTHACentered) x=(l.width-widths[i])/2.0;
        l.X<<QPointF(x,ysFromFirstLine[i]+y0);
    }
    return l;
}

double JKQTMathTextVerticalListNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
    doDrawBoxes(painter, x, y, currentEv);
    const LayoutInfo l=calcLayout(painter, currentEv);
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->draw(painter, x+l.X.at(i).x(), y+l.X.at(i).y(), currentEv);
    }
    return x+l.width;
}

void JKQTMathTextVerticalListNode::addChild(JKQTMathTextNode *n) {
    n->setParentNode(this);
    nodes.append(n);
}

bool JKQTMathTextVerticalListNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
    bool ok=true;
    if (alignment==MTHALeft) {
        html+="<div align=\"left\">";
    } else if (alignment==MTHACentered) {
        html+="<div align=\"center\">";
    } else if (alignment==MTHARight) {
        html+="<div align=\"right\">";
    }
    for (int i=0; i<nodes.size(); i++) {
        QString h="";
        ok = ok && nodes[i]->toHtml(h, currentEv, defaultEv);
        if (i==0) html=html+h;
        else html=html+"<br/>"+h;
    }
    html+="</div>";
    return ok;
}

QList<JKQTMathTextNode *> JKQTMathTextVerticalListNode::getChildren()  {
    return this->nodes;
}


int JKQTMathTextVerticalListNode::childCount() const
{
    return nodes.size();
}


void JKQTMathTextVerticalListNode::clearChildrenImpl(bool deleteChildren)
{
    if (deleteChildren) {
        for (int i=0; i<nodes.size(); i++) {
            delete nodes[i];
        }
    }
    nodes.clear();
}

void JKQTMathTextVerticalListNode::deleteChild(int i)
{
    if (i>=0 && i<nodes.size()) {
        delete nodes[i];
        nodes.removeAt(i);
    }
}
void JKQTMathTextVerticalListNode::clearChildren(bool deleteChildren)
{
    clearChildrenImpl(deleteChildren);
}

JKQTMathTextNode *JKQTMathTextVerticalListNode::getChild(int i)
{
    return nodes[i];
}

const JKQTMathTextNode *JKQTMathTextVerticalListNode::getChild(int i) const
{
    return nodes[i];
}

JKQTMathTextNode *JKQTMathTextVerticalListNode::replaceChild(int i, JKQTMathTextNode *newChild)
{
    JKQTMathTextNode* c=nodes[i];
    nodes[i]=newChild;
    newChild->setParentNode(this);
    return c;
}

JKQTMathTextHorizontalAlignment JKQTMathTextVerticalListNode::getAlignment() const
{
    return alignment;
}

JKQTMathTextVerticalOrientation JKQTMathTextVerticalListNode::getVerticalOrientation() const
{
    return verticalOrientation;
}

double JKQTMathTextVerticalListNode::getLineSpacingFactor() const
{
    return lineSpacingFactor;
}

JKQTMathTextLineSpacingMode JKQTMathTextVerticalListNode::getSpacingMode() const
{
    return spacingMode;
}

void JKQTMathTextVerticalListNode::setAlignment(JKQTMathTextHorizontalAlignment value)
{
    alignment=value;
}

void JKQTMathTextVerticalListNode::setVerticalOrientation(JKQTMathTextVerticalOrientation value)
{
    verticalOrientation=value;
}

void JKQTMathTextVerticalListNode::setLineSpacingFactor(double value)
{
    lineSpacingFactor=value;
}

void JKQTMathTextVerticalListNode::setSpacingMode(JKQTMathTextLineSpacingMode value)
{
    spacingMode=value;
}

JKQTMathTextVerticalListNode::LayoutInfo::LayoutInfo():
    JKQTMathTextNodeSize(), X()
{}



JKQTMathTextVerticalListNode::LayoutInfo::LayoutInfo(const JKQTMathTextNodeSize &other):
    JKQTMathTextNodeSize(other),
    X()
{

}

JKQTMathTextVerticalListNode::LayoutInfo &JKQTMathTextVerticalListNode::LayoutInfo::operator=(const JKQTMathTextNodeSize &other)
{
    JKQTMathTextNodeSize::operator=(other);
    X.clear();
    return *this;
}

JKQTMathTextVerticalListNode::LayoutInfo &JKQTMathTextVerticalListNode::LayoutInfo::operator=(const LayoutInfo &other)
{
    JKQTMathTextNodeSize::operator=(dynamic_cast<const JKQTMathTextNodeSize&>(other));
    X=other.X;
    return *this;
}

JKQTMathTextVerticalListNode::LayoutInfo::LayoutInfo(const LayoutInfo &other):
    JKQTMathTextNodeSize(dynamic_cast<const JKQTMathTextNodeSize&>(other)),
    X(other.X)
{

}
