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



#include "jkqtmathtext/nodes/jkqtmathtextmatrixnode.h"
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







JKQTMathTextMatrixNode::JKQTMathTextMatrixNode(JKQTMathText* _parent, QVector<QVector<JKQTMathTextNode*> > children):
    JKQTMathTextNode(_parent)
{
    this->lines=children.size();
    this->columns=0;
    for (int i=0; i<children.size(); i++) {
        if (children[i].size()>this->columns) this->columns=children[i].size();
    }
    this->children=children;
}

JKQTMathTextMatrixNode::~JKQTMathTextMatrixNode() {
    for (int i=0; i<children.size(); i++) {
        for (int j=0; j<children[i].size(); j++) {
            delete children[i].at(j);
        }
    }
    children.clear();
}

QString JKQTMathTextMatrixNode::getTypeName() const
{
    return "MTmatrixNode";
}

void JKQTMathTextMatrixNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTMathTextEnvironment ev1=currentEv;

    double xh=fm.strikeOutPos();//fm.xHeight();
    //double Ah=fm.ascent();
    double xw=fm.boundingRect("x").width();

    //ev1.fontSize=ev1.fontSize*parent->getFracFactor();


    QVector<double> colwidth, rowheight;
    //QVector<QVector<double> > widths, heights, baselines;

    double width1=0, baselineHeight1=0, overallHeight1=0, strikeoutPos1=0;
    //widths.resize(lines);
    colwidth.resize(columns); for (int i=0; i<columns; i++) colwidth[i]=0;
    rowheight.resize(lines);
    //heights=baselines=widths;
    for (int i=0; i<lines; i++) {
        rowheight[i]=0;
        //widths[i].resize(columns);
        //baselines[i]=heights[i]=widths[i];
        for (int j=0; j<children[i].size(); j++) {
            children[i].at(j)->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos1);
            /*widths[i].operator[](j)=width1;
            baselines[i].operator[](j)=baselineHeight;
            heights[i].operator[](j)=overallHeight1;*/
            if (overallHeight1>rowheight[i]) rowheight[i]=overallHeight1;
            if (width1>colwidth[j]) colwidth[j]=width1;
        }
    }


    overallHeight=(lines-1)*xw/2.0;
    width=columns*xw;
    for (int i=0; i<columns; i++) width=width+colwidth[i];
    for (int i=0; i<lines; i++) overallHeight=overallHeight+rowheight[i];
    baselineHeight=overallHeight/2.0+xh;
    strikeoutPos=xh;
}

double JKQTMathTextMatrixNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);

    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTMathTextEnvironment ev1=currentEv;

    double xh=fm.strikeOutPos();//fm.xHeight();
    //double Ah=fm.ascent();
    double xw=fm.boundingRect("x").width();

    //ev1.fontSize=ev1.fontSize;*parent->getFracFactor();


    QVector<double> colwidth, rowheight, rowascent;
    //QVector<QVector<double> > widths, heights, baselines;

    double width1=0, baselineHeight1=0, overallHeight1=0, strikeoutPos=0;
    //widths.resize(lines);
    colwidth.resize(columns); for (int i=0; i<columns; i++) colwidth[i]=0;
    rowheight.resize(lines);
    rowascent.resize(lines);
    //heights=baselines=widths;
    for (int i=0; i<lines; i++) {
        rowheight[i]=0;
        rowascent[i]=0;
        //widths[i].resize(columns);
        //baselines[i]=heights[i]=widths[i];
        for (int j=0; j<children[i].size(); j++) {
            if (children[i].at(j)!=nullptr) children[i].at(j)->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos);
            /*widths[i].operator[](j)=width1;
            baselines[i].operator[](j)=baselineHeight;
            heights[i].operator[](j)=overallHeight1;*/
            if (overallHeight1>rowheight[i]) rowheight[i]=overallHeight1;
            if (baselineHeight1>rowascent[i]) rowascent[i]=baselineHeight1;
            if (width1>colwidth[j]) colwidth[j]=width1;
        }
    }

    double overallHeight=(lines-1)*xw/2.0;
    double width=(columns)*xw;
    for (int i=0; i<columns; i++) width=width+colwidth[i];
    for (int i=0; i<lines; i++) overallHeight=overallHeight+rowheight[i];
    double baselineHeight=overallHeight/2.0+xh;

    double xx=x;
    double yy=y-baselineHeight;
    if (lines>0) yy=yy+rowascent[0];
    for (int i=0; i<lines; i++) {
        xx=x;
        for (int j=0; j<children[i].size(); j++) {
            children[i].at(j)->draw(painter, xx, yy, ev1);
            xx=xx+colwidth[j]+xw;
        }

        if (i<lines-1) yy=yy+(rowheight[i]-rowascent[i])+xw/2.0+rowascent[i+1];
    }


    return x+width;
}

bool JKQTMathTextMatrixNode::toHtml(QString &/*html*/, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/)
{
    return false;
}

QVector<QVector<JKQTMathTextNode *> > JKQTMathTextMatrixNode::getChildren() const {
    return this->children;
}

int JKQTMathTextMatrixNode::getColumns() const {
    return this->columns;
}

int JKQTMathTextMatrixNode::getLines() const {
    return this->lines;
}

void JKQTMathTextMatrixNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    for (int i=0; i<lines; i++) {
        for (int j=0; j<children[i].size(); j++) {
            children[i].at(j)->setDrawBoxes(draw);
        }
    }

}



