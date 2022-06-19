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



#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
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


// --------------------------------------------------------------------------------------------------
// -- implementation of the JKQTMathTextNode's methods
// --------------------------------------------------------------------------------------------------


JKQTMathTextNode::JKQTMathTextNode(JKQTMathText *_parent):
    parentMathText(_parent),
    parentNode(nullptr),
    drawBoxes(false)
{

}

JKQTMathTextNode::~JKQTMathTextNode()
= default;

void JKQTMathTextNode::getSize(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize)
{
    double w=width, b=baselineHeight, o=overallHeight, s=strikeoutPos;
    getSizeInternal(painter, currentEv, w, b, o, s, prevNodeSize);

    if (w<1e5) width=w;
    if (b<1e5) baselineHeight=b;
    if (o<1e5) overallHeight=o;
    if (s<1e5) strikeoutPos=s;
}


double JKQTMathTextNode::getNonItalicXCorretion(QPainter &painter, double width_potentiallyitalic, const JKQTMathTextEnvironment &ev_potentiallyitalic, JKQTMathTextNode* child) const
{
    double italic_xcorrection=0.0;
    if (ev_potentiallyitalic.italic) {
        JKQTMathTextEnvironment ev_nonitalic=ev_potentiallyitalic;
        ev_nonitalic.italic=false;
        double width_nonitalic=0, baselineHeight_nonitalic=0, overallHeight_nonitalic=0, strikeoutPos_nonitalic=0;
        child->getSize(painter, ev_nonitalic, width_nonitalic, baselineHeight_nonitalic, overallHeight_nonitalic, strikeoutPos_nonitalic);
        italic_xcorrection=width_potentiallyitalic-width_nonitalic;
    }
    return italic_xcorrection;
}


bool JKQTMathTextNode::toHtml(QString &/*html*/, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) {
    return false;
}

bool JKQTMathTextNode::getDrawBoxes() const {
    return this->drawBoxes;
}


void JKQTMathTextNode::doDrawBoxes(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) {
    if (drawBoxes) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        double w, oh, bh, sp;
        getSize(painter, currentEv, w, bh, oh, sp);
        QPen p=painter.pen();
        p.setColor("lightcoral");
        p.setWidthF(0.5);
        painter.setPen(p);
        QRectF r(x, y-bh, w, oh);
        painter.drawRect(r);
        p.setColor("lightblue");
        painter.setPen(p);
        if (w>0) painter.drawLine(QLineF(x, y, x+w, y));
        p.setColor("pink");
        p.setStyle(Qt::DashLine);
        painter.setPen(p);
        if (w>0) painter.drawLine(QLineF(x, y-sp, x+w, y-sp));
        p.setColor("green");
        p.setStyle(Qt::SolidLine);
        painter.setPen(p);
        painter.drawEllipse(QRectF(x-3.0,y-3.0,6.0,6.0));
        p.setColor("lightgreen");
        painter.setPen(p);
        painter.drawLine(QLineF(x-2.0, y, x+2.0, y));
        painter.drawLine(QLineF(x, y-2, x, y+2.0));

    }
}

void JKQTMathTextNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
}

QString JKQTMathTextNode::getTypeName() const
{
    return "JKQTMathTextNode";
}

void JKQTMathTextNode::setParentNode(JKQTMathTextNode *node)
{
    parentNode=node;
    if (node) parentMathText=node->parentMathText;
}

JKQTMathTextNode *JKQTMathTextNode::getParentNode()
{
    return parentNode;
}

const JKQTMathTextNode *JKQTMathTextNode::getParentNode() const
{
    return parentNode;
}

JKQTMathTextSingleChildNode::JKQTMathTextSingleChildNode(JKQTMathTextNode *_child, JKQTMathText *parentMathText):
    JKQTMathTextNode(parentMathText),
    child(_child)
{
    if (child) child->setParentNode(this);
}

JKQTMathTextSingleChildNode::~JKQTMathTextSingleChildNode()
{
    JKQTMathTextNode* c=child; child=nullptr;
    if (c) delete c;
}

JKQTMathTextNode *JKQTMathTextSingleChildNode::getChild()
{
    return child;
}

const JKQTMathTextNode *JKQTMathTextSingleChildNode::getChild() const
{
    return child;
}

JKQTMathTextNode *JKQTMathTextSingleChildNode::replaceChild(JKQTMathTextNode *newChild)
{
    JKQTMathTextNode* c=child;
    child=newChild;
    child->setParentNode(this);
    return c;
}

void JKQTMathTextSingleChildNode::replaceAndDeleteChild(JKQTMathTextNode *newChild)
{
    JKQTMathTextNode* oldC=replaceChild(newChild);
    if (newChild!=oldC) {
        oldC->setParentNode(nullptr);
        delete oldC;
    }
}

bool JKQTMathTextSingleChildNode::hasChild() const
{
    return child!=nullptr;
}

void JKQTMathTextSingleChildNode::setDrawBoxes(bool draw)
{
    JKQTMathTextNode::setDrawBoxes(draw);
    if (child) child->setDrawBoxes(draw);
}


JKQTMathTextDualChildNode::JKQTMathTextDualChildNode(JKQTMathTextNode *_child1, JKQTMathTextNode *_child2, JKQTMathText *parentMathText):
    JKQTMathTextMultiChildNode(parentMathText),
    child1(_child1),
    child2(_child2)
{
    if (child1) child1->setParentNode(this);
    if (child2) child2->setParentNode(this);
}

JKQTMathTextDualChildNode::~JKQTMathTextDualChildNode()
{
    JKQTMathTextNode* c1=child1; child1=nullptr;
    JKQTMathTextNode* c2=child2; child2=nullptr;
    if (c1) delete c1;
    if (c2) delete c2;
}

JKQTMathTextNode *JKQTMathTextDualChildNode::getChild1()
{
    return child1;
}

const JKQTMathTextNode *JKQTMathTextDualChildNode::getChild1() const
{
    return child1;
}

JKQTMathTextNode *JKQTMathTextDualChildNode::getChild2()
{
    return child2;
}

const JKQTMathTextNode *JKQTMathTextDualChildNode::getChild2() const
{
    return child2;
}

JKQTMathTextNode *JKQTMathTextDualChildNode::getChild(int i)
{
    if (i==0) return child1;
    if (i==1) return child2;
    return nullptr;
}


const JKQTMathTextNode *JKQTMathTextDualChildNode::getChild(int i) const
{
    if (i==0) return child1;
    if (i==1) return child2;
    return nullptr;
}

JKQTMathTextNode *JKQTMathTextDualChildNode::replaceChild(int i, JKQTMathTextNode *newChild)
{
    if (i==0) {
        JKQTMathTextNode* c=child1;
        child1=newChild;
        child1->setParentNode(this);
        return c;
    }
    if (i==1) {
        JKQTMathTextNode* c=child2;
        child2=newChild;
        child2->setParentNode(this);
        return c;
    }
    return nullptr;
}

int JKQTMathTextDualChildNode::childCount() const
{
    return 2;
}

void JKQTMathTextDualChildNode::clearChildren(bool deleteChildren)
{
    JKQTMathTextNode* c1=child1; child1=nullptr;
    if (c1) delete c1;
    JKQTMathTextNode* c2=child2; child2=nullptr;
    if (c2) delete c2;
}

JKQTMathTextMultiChildNode::JKQTMathTextMultiChildNode(JKQTMathText *parentMathText):
    JKQTMathTextNode(parentMathText)
{

}

JKQTMathTextMultiChildNode::~JKQTMathTextMultiChildNode()
{
}

QList<JKQTMathTextNode *> JKQTMathTextMultiChildNode::getChildren()
{
    QList<JKQTMathTextNode *> l;
    for (int i=0; i<childCount(); i++) {
        l.append(getChild(i));
    }
    return l;
}

QList<const JKQTMathTextNode *> JKQTMathTextMultiChildNode::getChildren() const
{
    QList<const JKQTMathTextNode *> l;
    for (int i=0; i<childCount(); i++) {
        l.append(getChild(i));
    }
    return l;
}

void JKQTMathTextMultiChildNode::replaceAndDeleteChild(int i, JKQTMathTextNode *newChild)
{
    JKQTMathTextNode* oldC=replaceChild(i, newChild);
    if (newChild!=oldC) {
        oldC->setParentNode(nullptr);
        delete oldC;
    }
}

void JKQTMathTextMultiChildNode::setDrawBoxes(bool draw)
{
    JKQTMathTextNode::setDrawBoxes(draw);
    for (int i=0; i<childCount(); i++) {
        JKQTMathTextNode* c=getChild(i);
        if (c) c->setDrawBoxes(draw);
    }

}
