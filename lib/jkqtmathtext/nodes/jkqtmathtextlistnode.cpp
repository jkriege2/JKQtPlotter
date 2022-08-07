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

void JKQTMathTextHorizontalListNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    width=0;
    overallHeight=0;
    baselineHeight=0;
    strikeoutPos=0;
    const QFontMetricsF fm(currentEv.getFont(parentMathText));
    const double subsupershift=fm.xHeight()*parentMathText->getOperatorsubsuperDistanceFactor();
    const double subsuperextrawidth=fm.boundingRect('x').width()*parentMathText->getOperatorsubsuperExtraSpaceFactor();
    const double subsuperSpecialModeAscent=fm.ascent()*parentMathText->getSubsuperModeSelectionBySizeFactor();
    const double subsuperSpecialModeDecent=fm.descent()*parentMathText->getSubsuperModeSelectionBySizeFactor();
    const double spaceWidth=fm.boundingRect(' ').width();
    //QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());


    double xnew=0;
    //bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        JKQTMathTextNodeSize prevNodeSize;
        JKQTMathTextNodeSize* prevNodeSizePtrForSubscript=nullptr;
        JKQTMathTextNodeSize* prevNodeSizePtrForSuperscript=nullptr;
        double subSuperXCorrection=0;
        double subBesidesXCorrection=0;

        if (i>0) {
            JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i-1]);
            if (symbN) symbN->getSymbolSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos, subSuperXCorrection, subBesidesXCorrection);
            else nodes[i-1]->getSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos);
            const double prevAscent=prevNodeSize.baselineHeight;
            const double prevDescent=prevNodeSize.overallHeight-prevNodeSize.baselineHeight;
            const bool shouldUseSpecialSubscriptMode=prevAscent>=subsuperSpecialModeAscent;
            const bool shouldUseSpecialSuperscriptMode=prevDescent>=subsuperSpecialModeDecent;
            if (shouldUseSpecialSubscriptMode)  prevNodeSizePtrForSubscript=&prevNodeSize;
            if (shouldUseSpecialSuperscriptMode)  prevNodeSizePtrForSuperscript=&prevNodeSize;
        }

        JKQTMathTextSuperscriptNode* nodeI_SuperScript=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i]);
        JKQTMathTextSubscriptNode* nodeI_SubScript=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i]);

        bool doDraw=true;
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (nodeI_SuperScript) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1=0, w2=0, oh=0, bh=0, sp=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp, prevNodeSizePtrForSuperscript);

                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }

                    i++;
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp, prevNodeSizePtrForSubscript);
                    //qDebug()<<"super_sub:   sub: "<<nodes[i]->getTypeName()<<"  w2="<<w2<<" bh"<<bh<<" oh="<<oh<<" sp="<<sp;
                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }
                    xnew+=qMax(w1+spaceWidth, w2+subBesidesXCorrection);

                    doDraw=false;
                    //qDebug()<<"### super+sub";
                    //qDebug()<<"### subsupop: super+sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (nodeI_SubScript) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1=0, w2=0, oh=0, bh=0, sp=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp, prevNodeSizePtrForSubscript);
                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }

                    i++;
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp, prevNodeSizePtrForSuperscript);
                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }
                    xnew+=qMax(w1+subBesidesXCorrection, w2+spaceWidth);


                    doDraw=false;
                    //qDebug()<<"### sub+super";
                    //qDebug()<<"### subsupop: sub+super1   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            } else {
                double w1=0, oh=0, bh=0, sp=0;
                nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp, prevNodeSizePtrForSubscript);
                if (bh>baselineHeight) {
                    overallHeight=overallHeight+bh-baselineHeight;
                    baselineHeight=bh;
                    strikeoutPos=sp;
                }
                if (baselineHeight+oh-bh>overallHeight) {
                    overallHeight=baselineHeight+oh-bh;
                    strikeoutPos=sp;
                }
                xnew+=w1-subBesidesXCorrection;
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
                    double w1=0, wsub=0, wsup=0;
                    double oh1=0, ohsub=0, ohsup=0;
                    double bh1=0, bhsub=0, bhsup=0;
                    double sp1=0, spsub=0, spsup=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    //qDebug()<<"sub_super:   node: "<<nodes[i]->getTypeName()<<"  w1="<<w1<<" bh"<<bh1<<" oh="<<oh1<<" sp="<<sp1;
                    subn->getChild()->getSize(painter, ev, wsub, bhsub, ohsub, spsub);
                    //qDebug()<<"sub_super:   node: "<<subn->getTypeName()<<"  w2="<<w2<<" bh2"<<bh2<<" oh2="<<oh2<<" sp2="<<sp2;
                    supn->getChild()->getSize(painter, ev, wsup, bhsup, ohsup, spsup);
                    //qDebug()<<"sub_super:   node: "<<supn->getTypeName()<<"  w3="<<w3<<" bh3"<<bh3<<" oh3="<<oh3<<" sp3="<<sp3;
                    const double descent1=oh1-bh1;
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    const double neww=qMax(qMax(w1, wsub+subSuperXCorrection), wsup+subSuperXCorrection)+subsuperextrawidth;
                    const double newBaselineHeight=bh1+ohsup+subsupershift;
                    if (newBaselineHeight>baselineHeight) {
                        const double extraBaselineHeight=newBaselineHeight-baselineHeight;
                        baselineHeight=newBaselineHeight;
                        overallHeight=overallHeight+extraBaselineHeight;
                    }
                    const double newDescent=descent1+ohsub+subsupershift;
                    if (newDescent>overallHeight-baselineHeight) {
                        const double extraDescent=newDescent-(overallHeight-baselineHeight);
                        overallHeight=overallHeight+extraDescent;
                    }

                    i++;
                    i++;
                    doDraw=false;
                    xnew+=neww;
                    //qDebug()<<"### subsupop: sub+super2   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;

                } else if (subn) { // is this subscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    double w1=0, wsub=0;
                    double oh1=0, ohsub=0;
                    double bh1=0, bhsub=0;
                    double sp1=0, spsub=0;//, sp3=0;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        symbN->getSymbolSize(painter, currentEv, w1, bh1, oh1, sp1, subSuperXCorrection, subBesidesXCorrection);
                    } else {
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    }
                    subn->getChild()->getSize(painter, ev, wsub, bhsub, ohsub, spsub);
                    const double descent1=oh1-bh1;
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;

                    const double newDescent=descent1+ohsub+subsupershift;
                    if (newDescent>overallHeight-baselineHeight) {
                        const double extraDescent=newDescent-(overallHeight-baselineHeight);
                        overallHeight=overallHeight+extraDescent;
                    }
                    if (bh1>baselineHeight) {
                        overallHeight=overallHeight+(bh1-baselineHeight);
                        baselineHeight=bh1;
                    }
                    const double neww=qMax(w1, wsub+subSuperXCorrection)+subsuperextrawidth;
                    i++;
                    doDraw=false;
                    xnew+=neww;
                    //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                } else if (supn) { // is this superscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    double w1=0, wsup=0;
                    double oh1=0, ohsup=0;
                    double bh1=0, bhsup=0;
                    double sp1=0, spsup=0;//, sp3=0;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        symbN->getSymbolSize(painter, currentEv, w1, bh1, oh1, sp1, subSuperXCorrection, subBesidesXCorrection);
                    } else {
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    }
                    const double descent1=oh1-bh1;
                    supn->getChild()->getSize(painter, ev, wsup, bhsup, ohsup, spsup);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;

                    const double newBaselineHeight=bh1+ohsup+subsupershift;
                    if (newBaselineHeight>baselineHeight) {
                        const double extraBaselineHeight=newBaselineHeight-baselineHeight;
                        baselineHeight=newBaselineHeight;
                        overallHeight=overallHeight+extraBaselineHeight;
                    }
                    if (descent1>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+descent1;
                    }

                    const double neww=qMax(w1, wsup+subSuperXCorrection)+subsuperextrawidth;
                    i++;
                    doDraw=false;
                    xnew+=neww;
                    //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            double w=0, oh=0, bh=0, sp=0;
            if (nodeI_SubScript) nodes[i]->getSize(painter, currentEv, w, bh, oh, sp, prevNodeSizePtrForSubscript);
            else if (nodeI_SuperScript) nodes[i]->getSize(painter, currentEv, w, bh, oh, sp, prevNodeSizePtrForSuperscript);
            else nodes[i]->getSize(painter, currentEv, w, bh, oh, sp, nullptr);
            const double cDescent=oh-bh;

            //qDebug()<<"### else:  bh="<<bh<<" baselineHeight="<<baselineHeight<<"   oh="<<oh<<" overallHeight="<<overallHeight;
            if (bh>baselineHeight) {
                overallHeight=overallHeight-baselineHeight+bh;
                baselineHeight=bh;
                strikeoutPos=sp;
            }
            if (baselineHeight+cDescent>overallHeight) {
                overallHeight=baselineHeight+cDescent;
                strikeoutPos=sp;
            }
            //qDebug()<<"### subsupop: else   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;

            xnew+=w;
            //qDebug()<<i<<xnew;
        }
         //wasBrace=dynamic_cast<JKQTMathTextBraceNode*>(nodes[i]);
    }
    width=xnew;
}

double JKQTMathTextHorizontalListNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
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

        JKQTMathTextNodeSize prevNodeSize;
        JKQTMathTextNodeSize* prevNodeSizePtrForSubscript=nullptr;
        JKQTMathTextNodeSize* prevNodeSizePtrForSuperscript=nullptr;
        double subSuperXCorrection=0;
        double subBesidesXCorrection=0;

        if (i>0) {
            JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i-1]);
            if (symbN) {
                symbN->getSymbolSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos, subSuperXCorrection, subBesidesXCorrection);
            } else {
                nodes[i-1]->getSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos);
            }
            const double prevAscent=prevNodeSize.baselineHeight;
            const double prevDescent=prevNodeSize.overallHeight-prevNodeSize.baselineHeight;
            const bool shouldUseSpecialSubscriptMode=prevAscent>=subsuperSpecialModeAscent;
            const bool shouldUseSpecialSuperscriptMode=prevDescent>=subsuperSpecialModeDecent;
            if (shouldUseSpecialSubscriptMode)  prevNodeSizePtrForSubscript=&prevNodeSize;
            if (shouldUseSpecialSuperscriptMode)  prevNodeSizePtrForSuperscript=&prevNodeSize;
        }

        JKQTMathTextSuperscriptNode* nodeI_SuperScript=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i]);
        JKQTMathTextSubscriptNode* nodeI_SubScript=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i]);

        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (nodeI_SuperScript) {

            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1])) { // is this subscript?

                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtrForSuperscript);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew-subSuperXCorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            }
        } else if (nodeI_SubScript) {
            if (i+1<nodes.size()) { // is there one more node behind?
                if (dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew-subSuperXCorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtrForSuperscript);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            } else {
                xnew=nodes[i]->draw(painter, xnew-subBesidesXCorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
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
                    double w1=0, wsub=0, wsup=0;
                    double oh1=0, ohsub=0, ohsup=0;
                    double bh1=0, bhsub=0, bhsup=0, spsub, spsup, sp;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        symbN->getSymbolSize(painter, currentEv, w1, bh1, oh1, sp, subSuperXCorrection, subBesidesXCorrection);
                    } else {
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                    }


                    subn->getChild()->getSize(painter, ev, wsub, bhsub, ohsub, spsub);
                    supn->getChild()->getSize(painter, ev, wsup, bhsup, ohsup, spsup);
                    const double descent1=oh1-bh1;
                    //double d2=oh2-bh2;
                    const double descent3=ohsup-bhsup;

                    const double neww=qMax(qMax(w1, wsub), wsup)+subsuperextrawidth;
                    //double xnew1=
                    const double xn1=nodes[i]->draw(painter, xnew+(neww-w1)/2.0, ynew, currentEv);
                    i++;
                    //double xnew2=
                    const double xnsub=subn->getChild()->draw(painter, xnew+(neww-wsub)/2.0-subSuperXCorrection, ynew+bhsub+descent1+subsupershift, ev);
                    i++;
                    //double xnew3=
                    const double xnsup=supn->getChild()->draw(painter, xnew+(neww-wsup)/2.0+subSuperXCorrection, ynew-bh1-descent3-subsupershift, ev);
                    doDraw=false;
                    xnew=qMax(qMax(xn1, xnsub), xnsup)+subsuperextrawidth/2.0;
                } else if (subn) { // is this subscript and no following superscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    double w1=0, wsub=0;
                    double oh1=0, ohsub=0;
                    double bh1=0, bhsub=0, sp=0, spsub=0;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        symbN->getSymbolSize(painter, currentEv, w1, bh1, oh1, sp, subSuperXCorrection, subBesidesXCorrection);
                    } else {
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                    }
                    subn->getChild()->getSize(painter, ev, wsub, bhsub, ohsub, spsub);
                    const double descent1=oh1-bh1;
                    //double d2=oh2-bh2;

                    const double neww=qMax(w1, wsub)+subsuperextrawidth;
                    //double xnew1=
                    const double xn1=nodes[i]->draw(painter, xnew+(neww-w1)/2.0, ynew, currentEv);
                    i++;
                    //double xnew2=
                    const double xnsub=subn->getChild()->draw(painter, xnew+(neww-wsub)/2.0-subSuperXCorrection, ynew+bhsub+descent1+subsupershift, ev)+subsupershift;
                    doDraw=false;
                    //xnew+=w;
                    xnew=qMax(xnsub, xn1)+subsuperextrawidth/2.0;
                } else if (supn) { // is this superscript and no following subscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parentMathText->getOperatorsubsuperSizeFactor();
                    double w1=0,  wsup=0;
                    double oh1=0,  ohsup=0;
                    double bh1=0,  bhsup=0, sp, spsup;
                    JKQTMathTextSymbolNode* symbN=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
                    if (symbN) {
                        symbN->getSymbolSize(painter, currentEv, w1, bh1, oh1, sp, subSuperXCorrection, subBesidesXCorrection);
                    } else {
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                    }
                    supn->getChild()->getSize(painter, ev, wsup, bhsup, ohsup, spsup);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    const double descent3=ohsup-bhsup;

                    const double neww=qMax(w1, wsup)+subsuperextrawidth;
                    //double xnew1=
                    const double xn1=nodes[i]->draw(painter, xnew+(neww-w1)/2.0, ynew, currentEv);
                    i++;
                    //double xnew3=
                    const double xnsup=supn->getChild()->draw(painter, xnew+(neww-wsup)/2.0+subSuperXCorrection, ynew-bh1-descent3-subsupershift, ev);
                    doDraw=false;
                    xnew=qMax(xn1, xnsup)+subsuperextrawidth/2.0;
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            if (nodeI_SuperScript) xnew=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtrForSuperscript);
            else if (nodeI_SubScript) xnew=nodes[i]->draw(painter, xnew-subBesidesXCorrection, ynew, currentEv, prevNodeSizePtrForSubscript);
            else xnew=nodes[i]->draw(painter, xnew, ynew, currentEv, nullptr);
        }
    }
    return xnew;
}

void JKQTMathTextHorizontalListNode::addChild(JKQTMathTextNode *n) {
    n->setParentNode(this);
    nodes.append(n);
}

bool JKQTMathTextHorizontalListNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
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


QString JKQTMathTextVerticalListNode::SpacingMode2String(SpacingMode mode)
{
    switch(mode) {
        case SMMinimal: return "minimal";
        default:
        case SMDefault: return "default";
    }
}

JKQTMathTextVerticalListNode::SpacingMode JKQTMathTextVerticalListNode::String2SpacingMode(QString tokenName)
{
    tokenName=tokenName.toLower().trimmed();
    if (tokenName=="default") return SMDefault;
    if (tokenName=="minimal" || tokenName=="min" || tokenName=="minimum") return SMMinimal;
    return SMDefault;
}

JKQTMathTextVerticalListNode::JKQTMathTextVerticalListNode(JKQTMathText *_parent, JKQTMathTextHorizontalAlignment _alignment, double _linespacingFactor, SpacingMode spacingMode_, JKQTMathTextVerticalOrientation _verticalOrientation):
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

void JKQTMathTextVerticalListNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    const  LayoutInfo l=calcLayout(painter, currentEv);
    width=l.width;
    overallHeight=l.overallHeight;
    baselineHeight=l.baselineHeight;
    strikeoutPos=l.strikeoutPos;
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
        double locWidth=0, locOverallHeight=0, locBaselineHeight=0, locStrikeoutPos=0;
        nodes[i]->getSize(painter, currentEv, locWidth, locBaselineHeight, locOverallHeight, locStrikeoutPos);

        if (i==0) {
            heightSum=locBaselineHeight;
        } else if (i>0) {
            double deltaLine=0;
            if (spacingMode==SMMinimal) {
                deltaLine=descents.last()+lineLeading+locBaselineHeight;
            } else if (spacingMode==SMDefault) {
                deltaLine=qMax(linespacing, descents.last()+lineLeading+locBaselineHeight);
            }
            heightSum=heightSum+deltaLine;
            y=y+deltaLine;
        }
        widths<<locWidth;
        l.width=qMax(l.width, locWidth);
        heights<<locOverallHeight;
        ascents<<locBaselineHeight;
        descents<<(locOverallHeight-locBaselineHeight);
        strikeouts<<locStrikeoutPos;
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

double JKQTMathTextVerticalListNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
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

bool JKQTMathTextVerticalListNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
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

JKQTMathTextVerticalListNode::SpacingMode JKQTMathTextVerticalListNode::getSpacingMode() const
{
    return spacingMode;
}

JKQTMathTextVerticalListNode::LayoutInfo::LayoutInfo():
    JKQTMathTextNodeSize(), X()
{}
