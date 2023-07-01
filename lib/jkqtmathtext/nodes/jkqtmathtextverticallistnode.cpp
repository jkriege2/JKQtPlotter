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



#include "jkqtmathtext/nodes/jkqtmathtextverticallistnode.h"
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

JKQTMathTextVerticalListNode::LayoutInfo JKQTMathTextVerticalListNode::calcLayout(QPainter &painter, JKQTMathTextEnvironment ev) const
{
    JKQTMathTextEnvironment currentEv=ev;
    LayoutInfo l;

    if (nodes.size()<=0) {
        return l;
    }

    // from now on we have at least one child node!!!

    QList<double> widths, heights, ascents, descents, strikeouts;
    double heightSum=0;
    QList<double> ysFromFirstLine; // y-position of each line, where the first line is always at y=0 (i.e. ysFromFirstLine[0]==0)
    double y=0;
    for (int i=0; i<nodes.size(); i++) {
        const QFontMetricsF fm(currentEv.getFont(parentMathText), painter.device());
        const double linespacing=fm.lineSpacing()*lineSpacingFactor;
        const double fleading=fm.leading();
        const double synLeading=fm.lineWidth();
        const double lineLeading=((fabs(fleading)>1e-6)?fleading:synLeading)*lineSpacingFactor;

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

        JKQTMathTextModifyEnvironmentNodeMixIn* modEnvNode=dynamic_cast<JKQTMathTextModifyEnvironmentNodeMixIn*>(nodes[i]);
        if (modEnvNode) {
            modEnvNode->modifyEnvironment(currentEv);
        }

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

double JKQTMathTextVerticalListNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment ev) const {
    JKQTMathTextEnvironment currentEv=ev;
    doDrawBoxes(painter, x, y, currentEv);
    const LayoutInfo l=calcLayout(painter, currentEv);
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->draw(painter, x+l.X.at(i).x(), y+l.X.at(i).y(), currentEv);
        JKQTMathTextModifyEnvironmentNodeMixIn* modEnvNode=dynamic_cast<JKQTMathTextModifyEnvironmentNodeMixIn*>(nodes[i]);
        if (modEnvNode) {
            modEnvNode->modifyEnvironment(currentEv);
        }
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
        JKQTMathTextModifyEnvironmentNodeMixIn* modEnvNode=dynamic_cast<JKQTMathTextModifyEnvironmentNodeMixIn*>(nodes[i]);
        if (modEnvNode) {
            modEnvNode->modifyEnvironment(currentEv);
        }
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
