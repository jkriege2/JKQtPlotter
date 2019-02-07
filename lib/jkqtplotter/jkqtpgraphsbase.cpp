/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtpgraphsbase.h"


#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPPlotElement::JKQTPPlotElement(JKQTBasePlotter* parent):
    QObject(parent)
{
    title="";
    visible=true;
    setParent(parent);
}

JKQTPPlotElement::JKQTPPlotElement(JKQTPlotter *parent):
    QObject(parent->getPlotter())
{
    title="";
    visible=true;
    setParent(parent);
}

JKQTPGraph::JKQTPGraph(JKQTBasePlotter* parent):
    JKQTPPlotElement(parent)
{
}

JKQTPGraph::JKQTPGraph(JKQTPlotter *parent):
    JKQTPPlotElement(parent)
{

}

QImage JKQTPPlotElement::generateKeyMarker(QSize size)
{
    QImage img(size.width(),size.height(),QImage::Format_ARGB32);
    if (parent) img.fill(parent->getKeyBackgroundColor());
    {
        JKQTPEnhancedPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
        QRectF rect(0,0,size.width(),size.height());
        drawKeyMarker(painter, rect);
    }
    return img;
}

void JKQTPPlotElement::setTitle(const QString &__value)
{
    this->title = __value;
}

QString JKQTPPlotElement::getTitle() const
{
    return this->title;
}

void JKQTPPlotElement::setVisible(bool __value)
{
    this->visible = __value;
}

bool JKQTPPlotElement::isVisible() const
{
    return this->visible;
}





void JKQTPPlotElement::setParent(JKQTBasePlotter* parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPPlotElement::setParent(JKQTPlotter *parent)
{
    setParent(parent->getPlotter());
}


bool JKQTPGraph::getDataMinMax(int column, double &minx, double &maxx, double &smallestGreaterZero)
{
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(column).getRows();

    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(column,i);
        if (start || xv>maxx) maxx=xv;
        if (start || xv<minx) minx=xv;
        double xvsgz;
        xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
        start=false;
    }
    return !start;
}

void JKQTPPlotElement::getOutsideSize(JKQTPEnhancedPainter& /*painter*/, int& leftSpace, int& rightSpace, int& topSpace, int& bottomspace) {
    leftSpace=0;
    rightSpace=0;
    topSpace=0;
    bottomspace=0;
}

void JKQTPPlotElement::drawOutside(JKQTPEnhancedPainter& /*painter*/, QRect /*leftSpace*/, QRect /*rightSpace*/, QRect /*topSpace*/, QRect /*bottomSpace*/) {

}

double JKQTPPlotElement::transformX(double x) const {
    return parent->getXAxis()->x2p(x);
}

double JKQTPPlotElement::transformY(double y) const {
    return parent->getYAxis()->x2p(y);
}

double JKQTPPlotElement::backtransformX(double x) const {
    return parent->getXAxis()->p2x(x);
}

double JKQTPPlotElement::backtransformY(double y) const {
    return parent->getYAxis()->p2x(y);
}


bool JKQTPGraph::usesColumn(int /*column*/) const
{
    return false;
}

void JKQTPGraph::drawErrorsBefore(JKQTPEnhancedPainter &)
{

}

void JKQTPGraph::drawErrorsAfter(JKQTPEnhancedPainter &)
{

}

QVector<QPointF> JKQTPPlotElement::transform(const QVector<QPointF> &x) {
    QVector<QPointF> res;
    for (int i=0; i<x.size(); i++) {
        res.append(transform(x[i]));
    }
    return res;
}

QPainterPath JKQTPPlotElement::transformToLinePath(const QVector<QPointF> &x) {
    QPainterPath res;
    if (x.size()>0) {
        res.moveTo(transform(x[0]));
        for (int i=1; i<x.size(); i++) {
            res.lineTo(transform(x[i]));
        }
    }
    return res;
}




JKQTPXYGraph::JKQTPXYGraph(JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    xColumn=-1;
    yColumn=-1;


}

JKQTPXYGraph::JKQTPXYGraph(JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    xColumn=-1;
    yColumn=-1;

}

bool JKQTPXYGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr)  return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv)) {
            if (start || xv>maxx) maxx=xv;
            if (start || xv<minx) minx=xv;
            double xvsgz;
            xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPXYGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(yv)) {
            if (start || yv>maxy) maxy=yv;
            if (start || yv<miny) miny=yv;
            double xvsgz;
            xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPXYGraph::usesColumn(int column) const
{
    return (column==xColumn)||(column==yColumn);
}

void JKQTPXYGraph::setDataSortOrder(int __value) {
    sortData=(DataSortOrder)__value;
}




JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(JKQTBasePlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    dataColumn=-1;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
    }

}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, JKQTBasePlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    parentPlotStyle=-1;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();

    }
}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQTBasePlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=lineWidth;
    parentPlotStyle=-1;
}


JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    dataColumn=-1;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();

    }

}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    parentPlotStyle=-1;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();

    }
}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=lineWidth;
    parentPlotStyle=-1;
}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=2.0;
    parentPlotStyle=-1;
}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, QColor color, JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=Qt::SolidLine;
    this->lineWidth=2.0;
    parentPlotStyle=-1;
}
QColor JKQTPSingleColumnGraph::getKeyLabelColor()
{
    return color;
}

void JKQTPSingleColumnGraph::setDataSortOrder(int __value) {
    sortData=(DataSortOrder)__value;
    if (__value>0) sortData=Sorted;
}

bool JKQTPSingleColumnGraph::usesColumn(int c) const
{
    return c==dataColumn;
}

QPen JKQTPSingleColumnGraph::getLinePen(JKQTPEnhancedPainter& painter) const
{
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

void JKQTPSingleColumnGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(dataColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPSingleColumnGraph::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(dataColumn,i);
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());


    }
}















void JKQTPXYGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPXYLineGraph::SortedX) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());


    } else if (sortData==JKQTPXYLineGraph::SortedY) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());
    }
}



JKQTPPlotObject::JKQTPPlotObject(JKQTBasePlotter *parent):
    JKQTPPlotElement(parent)
{

}

JKQTPPlotObject::JKQTPPlotObject(JKQTPlotter *parent):
    JKQTPPlotElement(parent)
{

}

JKQTPPlotObject::~JKQTPPlotObject()
= default;
