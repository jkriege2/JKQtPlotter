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




JKQTMathTextListNode::JKQTMathTextListNode(JKQTMathText* _parent):
    JKQTMathTextNode(_parent)
{
    nodes.clear();
    // these operations cause sub/sup script to be typeset over/under the operator, not right besides!
    subsupOperations<<"sum"<<"prod"<<"coprod"
               <<"bigcap"<<"bigcup"<<"bigvee"<<"bighat"
               <<"int"<<"iint"<<"iiint"<<"oint"<<"oiint"<<"oiiint"
               <<"mod"<<"median"<<"max"<<"min"<<"argmax"<<"argmin"<<"sup"<<"inf"
               <<"liminf"<<"limsup"<<"lim"<<"max"<<"min";
}

JKQTMathTextListNode::~JKQTMathTextListNode() {
    for (int i=0; i<nodes.size(); i++) {
        delete nodes[i];
    }
    nodes.clear();
}

QString JKQTMathTextListNode::getTypeName() const
{
    return "MTlistNode";
}

void JKQTMathTextListNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    width=0;
    overallHeight=0;
    baselineHeight=0;
    strikeoutPos=0;
    QFontMetricsF fm(currentEv.getFont(parent));
    //QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());


    double xnew=0;
    bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        JKQTMathTextNodeSize prevNodeSize;
        JKQTMathTextNodeSize* prevNodeSizePtr=nullptr;

        if (i>0 && wasBrace) {
            nodes[i-1]->getSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos);
            prevNodeSizePtr=&prevNodeSize;
        }


        bool doDraw=true;
        JKQTMathTextSymbolNode* smb=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1, w2, oh, bh, sp;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp, prevNodeSizePtr);

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
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp, prevNodeSizePtr);
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
                    xnew+=qMax(w1+fm.boundingRect(' ').width(), w2);

                    doDraw=false;
                    //qDebug()<<"### super+sub";
                    //qDebug()<<"### subsupop: super+sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1, w2, oh, bh, sp;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp, prevNodeSizePtr);
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
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp, prevNodeSizePtr);
                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }
                    xnew+=qMax(w1, w2+fm.boundingRect(' ').width());


                    doDraw=false;
                    //qDebug()<<"### sub+super";
                    //qDebug()<<"### subsupop: sub+super1   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (smb) {
            QString s=smb->getSymbolName();
            if (subsupOperations.contains(s)) {
                JKQTMathTextSubscriptNode* subn=nullptr;
                if (i+1<nodes.size()) subn=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1]);
                JKQTMathTextSuperscriptNode* supn=nullptr;
                if (i+2<nodes.size()) supn=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+2]);
                //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                if (subn && supn) { // is this subscript and superscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                    double w1=0, w2=0, w3=0;
                    double oh1=0, oh2=0, oh3=0;
                    double bh1=0, bh2=0, bh3=0;
                    double sp1=0, sp2=0, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    //qDebug()<<"sub_super:   node: "<<nodes[i]->getTypeName()<<"  w1="<<w1<<" bh"<<bh1<<" oh="<<oh1<<" sp="<<sp1;
                    subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp2);
                    //qDebug()<<"sub_super:   node: "<<subn->getTypeName()<<"  w2="<<w2<<" bh2"<<bh2<<" oh2="<<oh2<<" sp2="<<sp2;
                    supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp3);
                    //qDebug()<<"sub_super:   node: "<<supn->getTypeName()<<"  w3="<<w3<<" bh3"<<bh3<<" oh3="<<oh3<<" sp3="<<sp3;
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    double w=qMax(qMax(w1, w2), w3)+fm.boundingRect(' ').width();

                    double oh=oh1+oh2+oh3;
                    double bh=bh1+oh3;
                    if (oh>overallHeight) overallHeight=oh;
                    if (bh>baselineHeight) { baselineHeight=bh; strikeoutPos=sp1; }
                    if (oh-bh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+oh-bh;
                    }

                    i++;
                    i++;
                    doDraw=false;
                    xnew+=w;
                    //qDebug()<<"### subsupop: sub+super2   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                /*} else if (subn && supn) { // is this subscript and superscript?
                    MTenvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                    double w1=0, w2=0, w3=0;
                    double oh1=0, oh2=0, oh3=0;
                    double bh1=0, bh2=0, bh3=0;
                    double sp1=0, sp2=0, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp3);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    double w=qMax(w1, w3)+fm.boundingRect(' ').width();

                    double oh=oh1+oh3;
                    double bh=bh1+oh3;
                    if (oh>overallHeight) overallHeight=oh;
                    if (bh>baselineHeight) { baselineHeight=bh; strikeoutPos=sp1; }
                    if (oh-bh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+oh-bh;
                    }

                    i++;
                    i++;
                    doDraw=false;
                    xnew+=w;
                    //qDebug()<<"### subsupop: sub+super";*/
                } else if (subn) { // is this subscript?
                    JKQTMathTextEnvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                    double w1=0, w2=0;
                    double oh1=0, oh2=0;
                    double bh1=0, bh2=0;
                    double sp1=0, sp2=0;//, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp2);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;

                    double oh=oh1+oh2;
                    double sh=oh1-bh1+oh2*1.1;
                    if (oh>overallHeight) overallHeight=oh;
                    if (bh1>baselineHeight) baselineHeight=bh1;
                    if (sh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+sh;
                    }
                    double w=qMax(w1, w2)+fm.boundingRect(' ').width();
                    i++;
                    doDraw=false;
                    xnew+=w;
                    //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            double w, oh, bh, sp;
            nodes[i]->getSize(painter, currentEv, w, bh, oh, sp, prevNodeSizePtr);


            //qDebug()<<"### else:  bh="<<bh<<" baselineHeight="<<baselineHeight<<"   oh="<<oh<<" overallHeight="<<overallHeight;
            if (bh>baselineHeight) {
                overallHeight=overallHeight+bh-baselineHeight;
                baselineHeight=bh;
                strikeoutPos=sp;
            }
            if (baselineHeight+oh-bh>overallHeight) {
                overallHeight=baselineHeight+oh-bh;
                strikeoutPos=sp;
            }
            //qDebug()<<"### subsupop: else   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;

            xnew+=w;
            //qDebug()<<i<<xnew;
        }
         wasBrace=dynamic_cast<JKQTMathTextBraceNode*>(nodes[i]);
    }
    width=xnew;
}

double JKQTMathTextListNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double ynew=y;
    double xnew=x;
    //qDebug()<<"listNode: "<<currentEv.fontSize;
    QFontMetricsF fm(currentEv.getFont(parent));
    bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        bool doDraw=true;

        JKQTMathTextNodeSize prevNodeSize;
        JKQTMathTextNodeSize* prevNodeSizePtr=nullptr;

        if (i>0 && wasBrace) {
            nodes[i-1]->getSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos);
            prevNodeSizePtr=&prevNodeSize;
        }


        JKQTMathTextSymbolNode* smb=dynamic_cast<JKQTMathTextSymbolNode*>(nodes[i]);
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i])) {

            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1])) { // is this subscript?

                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            }
        } else if (dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            }
        } else {
            if (smb) {
                QString s=smb->getSymbolName();
                if (subsupOperations.contains(s)) {
                    JKQTMathTextSubscriptNode* subn=nullptr;
                    if (i+1<nodes.size()) subn=dynamic_cast<JKQTMathTextSubscriptNode*>(nodes[i+1]);
                    JKQTMathTextSuperscriptNode* supn=nullptr;
                    if (i+2<nodes.size()) supn=dynamic_cast<JKQTMathTextSuperscriptNode*>(nodes[i+2]);
                    //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                    if (subn && supn) { // is this subscript and superscript?
                        JKQTMathTextEnvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                        double w1=0, w2=0, w3=0;
                        double oh1=0, oh2=0, oh3=0;
                        double bh1=0, bh2=0, bh3=0, sp;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp);
                        supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp);
                        double d1=oh1-bh1;
                        //double d2=oh2-bh2;
                        double d3=oh3-bh3;

                        double w=qMax(qMax(w1, w2), w3);
                        //double xnew1=
                        double xn1=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew2=
                        double xn2=subn->getChild()->draw(painter, xnew+(w-w2)/2.0, ynew+bh2+d1, ev);
                        i++;
                        //double xnew3=
                        double xn3=supn->getChild()->draw(painter, xnew+(w-w3)/2.0, ynew-bh1-d3-fm.xHeight()/4.0, ev);
                        doDraw=false;
                        xnew=qMax(qMax(xn1, xn2), xn3)+fm.boundingRect(' ').width();
                    } else if (subn) { // is this subscript and not superscript?
                        JKQTMathTextEnvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                        double w1=0, w2=0;
                        double oh1=0, oh2=0;
                        double bh1=0, bh2=0, sp=0;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp);
                        double d1=oh1-bh1;
                        //double d2=oh2-bh2;

                        double w=qMax(w1, w2);
                        //double xnew1=
                        double xn2=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew2=
                        double xn1=subn->getChild()->draw(painter, xnew+(w-w2)/2.0, ynew+bh2+d1, ev)+fm.boundingRect(' ').width();
                        doDraw=false;
                        //xnew+=w;
                        xnew=qMax(xn1, xn2);
                    } else if (supn) { // is this subscript and superscript?
                        JKQTMathTextEnvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                        double w1=0,  w3=0;
                        double oh1=0,  oh3=0;
                        double bh1=0,  bh3=0, sp;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp);
                        //double d1=oh1-bh1;
                        //double d2=oh2-bh2;
                        double d3=oh3-bh3;

                        double w=qMax(w1, w3);
                        //double xnew1=
                        double xn1=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew3=
                        double xn3=supn->getChild()->draw(painter, xnew+(w-w3)/2.0, ynew-bh1-d3-fm.xHeight()/4.0, ev);
                        doDraw=false;
                        xnew=qMax(xn1, xn3)+fm.boundingRect(' ').width();
                    }
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            xnew=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
        }
        wasBrace=dynamic_cast<JKQTMathTextBraceNode*>(nodes[i]);
    }
    return xnew;
}

bool JKQTMathTextListNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    bool ok=true;
    for (int i=0; i<nodes.size(); i++) {
        QString h="";
        ok = ok && nodes[i]->toHtml(h, currentEv, defaultEv);
        html=html+h;
    }
    return ok;
}

void JKQTMathTextListNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->setDrawBoxes(draw);
    }
}

QList<JKQTMathTextNode *> JKQTMathTextListNode::getNodes() const {
    return this->nodes;
}


