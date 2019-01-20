/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
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

JKQTPPlotElement::JKQTPPlotElement(JKQTPLotter *parent):
    QObject(parent->get_plotter())
{
    title="";
    visible=true;
    setParent(parent);
}

JKQTPPlotElement::~JKQTPPlotElement()
{

}

JKQTPGraph::JKQTPGraph(JKQTBasePlotter* parent):
    JKQTPPlotElement(parent)
{
    datarange_start=datarange_end=-1;
}

JKQTPGraph::JKQTPGraph(JKQTPLotter *parent):
    JKQTPPlotElement(parent)
{
    datarange_start=datarange_end=-1;
}

JKQTPGraph::~JKQTPGraph()
{

}

QImage JKQTPPlotElement::generateKeyMarker(QSize size)
{
    QImage img(size.width(),size.height(),QImage::Format_ARGB32);
    if (parent) img.fill(parent->get_keyBackgroundColor());
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





void JKQTPPlotElement::setParent(JKQTBasePlotter* parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPPlotElement::setParent(JKQTPLotter *parent)
{
    setParent(parent->get_plotter());
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
    // interpret data ranges
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
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
    return parent->get_xAxis()->x2p(x);
}

double JKQTPPlotElement::transformY(double y) const {
    return parent->get_yAxis()->x2p(y);
}

double JKQTPPlotElement::backtransformX(double x) const {
    return parent->get_xAxis()->p2x(x);
}

double JKQTPPlotElement::backtransformY(double y) const {
    return parent->get_yAxis()->p2x(y);
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

JKQTPXYGraph::JKQTPXYGraph(JKQTPLotter *parent):
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
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<int>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<int>(imax));
    }
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
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<int>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<int>(imax));
    }
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

void JKQTPXYGraph::set_sortData(int __value) {
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


JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(JKQTPLotter *parent):
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
    }

}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, JKQTPLotter *parent):
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
    }
}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQTPLotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=lineWidth;
    parentPlotStyle=-1;
}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, JKQTPLotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=2.0;
    parentPlotStyle=-1;
}

JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(int dataColumn, QColor color, JKQTPLotter *parent):
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

void JKQTPSingleColumnGraph::set_sortData(int __value) {
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
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
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
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<int>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<int>(imax));
    }
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













JKQTPGraphErrors::JKQTPGraphErrors(QColor graphColor) {
    errorColor=graphColor.darker();
    errorStyle=Qt::SolidLine;
    errorWidth=2;
    errorFillColor=graphColor.lighter();
    errorColor.setAlphaF(0.5);
    errorFillStyle=Qt::SolidPattern;
    errorbarSize=7;
}

JKQTPGraphErrors::~JKQTPGraphErrors()
{

}



void JKQTPGraphErrors::intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int datarange_start, int datarange_end, int xErrorColumnLower, int yErrorColumnLower, bool xErrorSymmetric, bool yErrorSymmetric, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    //std::cout<<"JKQTPGraphErrors::intPlotXYErrorIndicators(p, "<<parent<<", "<<xColumn<<", "<<yColumn<<", "<<xErrorColumn<<", "<<yErrorColumn<<", "<<xErrorStyle<<", "<<yErrorStyle<<", ...)\n";
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    if ((yErrorStyle==JKQTPNoError) && (xErrorStyle==JKQTPNoError)) return;
    bool visX=(xErrorStyle!=JKQTPNoError)&&(xErrorColumn>=0||xErrorColumnLower>=0)&&(xColumn>=0)&&(yColumn>=0);
    bool visY=(yErrorStyle!=JKQTPNoError)&&(yErrorColumn>=0||yErrorColumnLower>=0)&&(xColumn>=0)&&(yColumn>=0);
    if (!visX&&!visY) return;
    //std::cout<<"   JKQTPGraphErrors::intPlotXYErrorIndicators(p, "<<parent<<", "<<xColumn<<", "<<yColumn<<", "<<xErrorColumn<<", "<<yErrorColumn<<", ...)\n";

    QBrush b=painter.brush();
    b.setColor(errorFillColor);
    b.setStyle(errorFillStyle);
    QPen p=painter.pen();
    p.setColor(errorColor);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, errorWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(errorStyle);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    painter.setPen(p);

    unsigned int imaxx=0, imaxy=0;
    if (xColumn>=0) imaxx=datastore->getColumn(static_cast<size_t>(xColumn)).getRows();
    if (yColumn>=0) imaxy=datastore->getColumn(static_cast<size_t>(yColumn)).getRows();
    int imax=qMin(imaxx, imaxy);
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<int>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<int>(imax));
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    double xold=-1;
    double xpold=-1;
    double xmold=-1;
    double yold=-1;
    double ypold=-1;
    double ymold=-1;
    bool pastFirst=false;
    double ebs_px=parent->pt2px(painter, errorbarSize);
    QPolygonF polyX, polyY;
    QList<QPointF> polyXTopPoints, polyXBottomPoints, polyYTopPoints, polyYBottomPoints;
    QList<QColor> errFC, errC;
    bool defaultErrorColor=true;


    /*
     *    double delta=1;
    double deltap=0;
    double deltam=0;
    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        if (imin==imax) { // only one x-value
            deltam=0.5;
            deltap=0.5;
        } else if (i==imax-1) { // the right-most x-value
            deltap=deltam=fabs(xv-datastore->get(xColumn,i-1))/2.0;
        } else if (i==imin) { // the left-most x-value
            deltam=deltap=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
        } else {
            deltam=fabs(xv-datastore->get(xColumn,i-1))/2.0;
            deltap=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
        }
        delta=deltap+deltam;
     **/

    for (int iii=imin; iii<imax; iii++) {
        int i=iii;
        if (dataorder) i=dataorder->value(iii, iii);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));

        double deltax=1;
        double deltapx=0;
        double deltamx=0;
        if (imin==imax) { // only one x-value
            deltamx=0.5;
            deltapx=0.5;
        } else if (i==imax-1&& i-1>=0) { // the right-most x-value
            deltapx=deltamx=fabs(xv-datastore->get(xColumn,i-1))/2.0;
        } else if (i==imin && i+1<imax) { // the left-most x-value
            deltamx=deltapx=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
        } else {
            if (i-1>=0) deltamx=fabs(xv-datastore->get(xColumn,i-1))/2.0;
            if (i+1<imax) deltapx=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
        }
        deltax=deltapx+deltamx;

        double deltay=1;
        double deltapy=0;
        double deltamy=0;
        if (imin==imax) { // only one y-value
            deltamy=0.5;
            deltapy=0.5;
        } else if (i==imax-1&& i-1>=0) { // the right-most y-value
            deltapy=deltamy=fabs(yv-datastore->get(yColumn,i-1))/2.0;
        } else if (i==imin && i+1<imax) { // the left-most y-value
            deltamy=deltapy=fabs(datastore->get(yColumn,i+1)-yv)/2.0;
        } else {
            if (i-1>=0) deltamy=fabs(yv-datastore->get(yColumn,i-1))/2.0;
            if (i+1<imax) deltapy=fabs(datastore->get(yColumn,i+1)-yv)/2.0;
        }
        deltay=deltapy+deltamy;

        bool plotlowerbarx=false;
        bool plotupperbarx=false;
        bool plotlowerbary=false;
        bool plotupperbary=false;

        double xe=0;   if (xErrorStyle!=JKQTPNoError && xErrorColumn>=0) { xe=datastore->get(xErrorColumn,i); plotupperbarx=true; }
        double ye=0;   if (yErrorStyle!=JKQTPNoError && yErrorColumn>=0) { ye=datastore->get(yErrorColumn,i); plotupperbary=true; }
        double xl=0;   if (xErrorSymmetric) { xl=xe; plotlowerbarx=plotupperbarx||(xl>0); }
                       else if (xErrorStyle!=JKQTPNoError && xErrorColumnLower>=0) { xl=datastore->get(xErrorColumnLower,i); plotlowerbarx=true; }
        double yl=0;   if (yErrorSymmetric) { yl=ye; plotlowerbary=plotupperbary||(yl>0); }
                       else if (yErrorStyle!=JKQTPNoError && yErrorColumnLower>=0) { yl=datastore->get(yErrorColumnLower,i); plotlowerbary=true; }
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(xe) && JKQTPIsOKFloat(ye) && JKQTPIsOKFloat(xl) && JKQTPIsOKFloat(yl)) {
            double x=parentGraph->transformX(xv+xrelshift*deltax); bool xok=JKQTPIsOKFloat(x);
            double y=parentGraph->transformY(yv+yrelshift*deltay); bool yok=JKQTPIsOKFloat(y);
            QColor terrCol=errorColor;
            QColor terrFillCol=errorFillColor;
            defaultErrorColor = defaultErrorColor && !this->intPlotXYErrorIndicatorsGetColor(painter, parent, parentGraph, xColumn, yColumn, xErrorColumn, yErrorColumn, xErrorStyle, yErrorStyle, i, terrCol, terrFillCol);

            // x-errorpolygons
            if (/*pastFirst &&*/ (xErrorStyle==JKQTPErrorPolygons || xErrorStyle==JKQTPErrorBarsPolygons || xErrorStyle==JKQTPErrorSimpleBarsPolygons)) {
                //double xl1m=xmold;
                //double xl1p=xpold;
                //double yl1=yold;
                double xl2m=parentGraph->transformX(xv+xrelshift*deltax-xl);
                double xl2p=parentGraph->transformX(xv+xrelshift*deltax+xe);
                double yl2=y;
                /*painter.save();
                painter.setBrush(b);
                painter.setPen(QPen(Qt::NoPen));
                QPolygonF poly;
                poly << QPointF(xl1p, yl1) << QPointF(xl1m, yl1) << QPointF(xl2m, yl2) << QPointF(xl2p, yl2);
                painter.drawConvexPolygon(poly);
                painter.restore();*/
                polyXTopPoints<<QPointF(xl2p, yl2);
                polyXBottomPoints<<QPointF(xl2m, yl2);
                errFC<<terrFillCol;
                errC<<terrCol;
            }

            // y-errorpolygons
            if (/*pastFirst &&*/ (yErrorStyle==JKQTPErrorPolygons || yErrorStyle==JKQTPErrorBarsPolygons || yErrorStyle==JKQTPErrorSimpleBarsPolygons)) {
                //double yl1m=ymold;
                //double yl1p=ypold;
                //double xl1=xold;
                double yl2m=parentGraph->transformY(yv+yrelshift*deltay-yl);
                double yl2p=parentGraph->transformY(yv+yrelshift*deltay+ye);
                double xl2=x;
                /*painter.save();
                painter.setBrush(b);
                painter.setPen(QPen(Qt::NoPen));
                QPolygonF poly;
                poly << QPointF(xl1, yl1m) << QPointF(xl2, yl2m) << QPointF(xl2, yl2p) << QPointF(xl1, yl1p);
                painter.drawConvexPolygon(poly);
                painter.restore();*/
                polyYTopPoints<<QPointF(xl2, yl2p);
                polyYBottomPoints<<QPointF(xl2, yl2m);
                errFC<<terrFillCol;
                errC<<terrCol;
            }


            //if (xv>=xmin && xv<=xmax && yv>=ymin && yv<=ymax) {
                //x-errorbars
            if ((xErrorColumn>=0 || xErrorColumnLower>=0) && (xErrorStyle==JKQTPErrorBars || xErrorStyle==JKQTPErrorBarsLines|| xErrorStyle==JKQTPErrorBarsPolygons
                || xErrorStyle==JKQTPErrorSimpleBars || xErrorStyle==JKQTPErrorSimpleBarsLines|| xErrorStyle==JKQTPErrorSimpleBarsPolygons)) {
                    double x0=parentGraph->transformX(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                    double x1=parentGraph->transformX(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                    painter.save();
                    QPen pp=p;
                    pp.setColor(terrCol);
                    painter.setPen(pp);
                    if (x0ok&&x1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x0, y, x1, y));
                        if (xErrorStyle==JKQTPErrorBars || xErrorStyle==JKQTPErrorBarsLines|| xErrorStyle==JKQTPErrorBarsPolygons) {
                            if (plotlowerbarx) painter.drawLine(QLineF(x0,y-ebs_px/2.0,x0,y+ebs_px/2.0));
                            if (plotupperbarx) painter.drawLine(QLineF(x1,y-ebs_px/2.0,x1,y+ebs_px/2.0));
                        }
                    } else if (x0ok&&!x1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x0, y, x, y));
                        if (xErrorStyle==JKQTPErrorBars || xErrorStyle==JKQTPErrorBarsLines|| xErrorStyle==JKQTPErrorBarsPolygons) {
                            if (plotlowerbarx) painter.drawLine(QLineF(x0,y-ebs_px/2.0,x0,y+ebs_px/2.0));
                        }
                        if (x0<x) painter.drawLine(QLineF(x,y,parentGraph->transformX(parent->getXMax()),y));
                        else painter.drawLine(QLineF(x,y,parentGraph->transformX(parent->getXMin()),y));
                    } else if (!x0ok&&x1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x1, y, x, y));
                        if (xErrorStyle==JKQTPErrorBars || xErrorStyle==JKQTPErrorBarsLines|| xErrorStyle==JKQTPErrorBarsPolygons) {
                            if (plotupperbarx) painter.drawLine(QLineF(x1,y-ebs_px/2.0,x1,y+ebs_px/2.0));
                        }
                        if (x1<x) painter.drawLine(QLineF(x,y,parentGraph->transformX(parent->getXMin()),y));
                        else painter.drawLine(QLineF(x,y,parentGraph->transformX(parent->getXMax()),y));
                    }
                    painter.restore();
                }
                // y-errorbars
                if ((yErrorColumn>=0 || yErrorColumnLower>=0) && (yErrorStyle==JKQTPErrorBars || yErrorStyle==JKQTPErrorBarsLines || yErrorStyle==JKQTPErrorBarsPolygons
                    || yErrorStyle==JKQTPErrorSimpleBars || yErrorStyle==JKQTPErrorSimpleBarsLines || yErrorStyle==JKQTPErrorSimpleBarsPolygons)) {
                    double y0=parentGraph->transformY(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                    double y1=parentGraph->transformY(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                    painter.save();
                    QPen pp=p;
                    pp.setColor(terrCol);
                    painter.setPen(pp);
                    if (y0ok&&y1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x, y0, x, y1));
                        if (yErrorStyle==JKQTPErrorBars || yErrorStyle==JKQTPErrorBarsLines || yErrorStyle==JKQTPErrorBarsPolygons) {
                            if (plotlowerbary) painter.drawLine(QLineF(x-ebs_px/2.0,y0,x+ebs_px/2.0,y0));
                            if (plotupperbary) painter.drawLine(QLineF(x-ebs_px/2.0,y1,x+ebs_px/2.0,y1));
                        }
                    } else if (y0ok&&!y1ok&&xok&&yok) {   // upper errorbar OK, lower errorbar NAN
                        painter.drawLine(QLineF(x, y0, x, y));
                        if (yErrorStyle==JKQTPErrorBars || yErrorStyle==JKQTPErrorBarsLines || yErrorStyle==JKQTPErrorBarsPolygons) {
                            if (plotlowerbary) painter.drawLine(QLineF(x-ebs_px/2.0,y0,x+ebs_px/2.0,y0));
                        }
                        if (y0<y) painter.drawLine(QLineF(x,y,x,parentGraph->transformY(parent->getYMin())));
                        else painter.drawLine(QLineF(x,y,x,parentGraph->transformY(parent->getYMax()))); // inverted axis!
                    } else if (!y0ok&&y1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x, y1, x, y));
                        if (yErrorStyle==JKQTPErrorBars || yErrorStyle==JKQTPErrorBarsLines || yErrorStyle==JKQTPErrorBarsPolygons) {
                            if (plotupperbary) painter.drawLine(QLineF(x-ebs_px/2.0,y1,x+ebs_px/2.0,y1));
                        }
                        if (y1<y) painter.drawLine(QLineF(x,y,x,parentGraph->transformY(parent->getYMax())));
                        else painter.drawLine(QLineF(x,y,x,parentGraph->transformY(parent->getYMin())));
                    }

                    painter.restore();
                }

                // error boxes
                if (yErrorStyle==JKQTPErrorBoxes || xErrorStyle==JKQTPErrorBoxes || yErrorStyle==JKQTPErrorEllipses || xErrorStyle==JKQTPErrorEllipses ) {
                    double y0=parentGraph->transformY(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                    double y1=parentGraph->transformY(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                    double x0=parentGraph->transformX(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                    double x1=parentGraph->transformX(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                    painter.save();
                    QPen pp=p;
                    pp.setColor(terrCol);
                    painter.setPen(pp);
                    QBrush bb=b;
                    bb.setColor(terrFillCol);
                    painter.setBrush(bb);

                    QRectF errRect=QRectF(QPointF(x0,y0), QPointF(x1,y1));
                    if ((y0ok&&y1ok)||(x0ok&&x1ok)) {
                        if (yErrorStyle==JKQTPErrorEllipses || xErrorStyle==JKQTPErrorEllipses) painter.drawEllipse(errRect);
                        else painter.drawRect(errRect);
                    }

                    painter.restore();
                }            //}

            // x-errorlines
            if (pastFirst && (xErrorStyle==JKQTPErrorLines || xErrorStyle==JKQTPErrorBarsLines || xErrorStyle==JKQTPErrorSimpleBarsLines)) {
                double xl1m=xmold;
                double xl1p=xpold;
                double yl1=yold;
                double xl2m=parentGraph->transformX(xv+xrelshift*deltax-xl);
                double xl2p=parentGraph->transformX(xv+xrelshift*deltax+xe);
                double yl2=y;
                painter.save();
                QPen pp=p;
                pp.setColor(terrCol);
                painter.setPen(pp);
                if (JKQTPIsOKFloat(xl1m)&&JKQTPIsOKFloat(yl1)&&JKQTPIsOKFloat(xl2m)&&JKQTPIsOKFloat(yl2)) {
                    painter.drawLine(QLineF(xl1m, yl1, xl2m, yl2));
                }
                if (JKQTPIsOKFloat(xl1p)&&JKQTPIsOKFloat(yl1)&&JKQTPIsOKFloat(xl2p)&&JKQTPIsOKFloat(yl2)) {
                    painter.drawLine(QLineF(xl1p, yl1, xl2p, yl2));
                }
                painter.restore();
            }


            // y-errorlines
            if (pastFirst && (yErrorStyle==JKQTPErrorLines || yErrorStyle==JKQTPErrorBarsLines || yErrorStyle==JKQTPErrorSimpleBarsLines)) {
                double yl1m=ymold;
                double yl1p=ypold;
                double xl1=xold;
                double yl2m=parentGraph->transformY(yv+yrelshift*deltay-yl);
                double yl2p=parentGraph->transformY(yv+yrelshift*deltay+ye);
                double xl2=x;
                painter.save();
                QPen pp=p;
                pp.setColor(terrCol);
                painter.setPen(pp);
                if (JKQTPIsOKFloat(xl1)&&JKQTPIsOKFloat(yl1m)&&JKQTPIsOKFloat(xl2)&&JKQTPIsOKFloat(yl2m)) {
                    painter.drawLine(QLineF(xl1, yl1m, xl2, yl2m));
                }
                if (JKQTPIsOKFloat(xl1)&&JKQTPIsOKFloat(yl1p)&&JKQTPIsOKFloat(xl2)&&JKQTPIsOKFloat(yl2p)) {
                    painter.drawLine(QLineF(xl1, yl1p, xl2, yl2p));
                }
                painter.restore();
            }



            pastFirst=true;
            xold=x;
            xmold=parentGraph->transformX(xv+xrelshift*deltax-xl);
            xpold=parentGraph->transformX(xv+xrelshift*deltax+xe);
            yold=y;
            ymold=parentGraph->transformY(yv+yrelshift*deltay-yl);
            ypold=parentGraph->transformY(yv+yrelshift*deltay+ye);
        }
    }
    // x-errorpolygons
    if ((polyXTopPoints.size()>0 || polyXBottomPoints.size()>0) && (xErrorStyle==JKQTPErrorPolygons || xErrorStyle==JKQTPErrorBarsPolygons || xErrorStyle==JKQTPErrorSimpleBarsPolygons)) {
        painter.save();
        painter.setBrush(b);
        painter.setPen(QPen(Qt::NoPen));
        QPolygonF poly;
        //poly << QPointF(xl1p, yl1) << QPointF(xl1m, yl1) << QPointF(xl2m, yl2) << QPointF(xl2p, yl2);
        for (int i=0; i<polyXTopPoints.size(); i++) {
            poly<<polyXTopPoints[i];
        }
        for (int i=polyXBottomPoints.size()-1; i>=0; i--) {
            poly<<polyXBottomPoints[i];
        }
        painter.drawConvexPolygon(poly);
        painter.restore();
    }
    if ((polyYTopPoints.size()>0 || polyYBottomPoints.size()>0) && (yErrorStyle==JKQTPErrorPolygons || yErrorStyle==JKQTPErrorBarsPolygons || yErrorStyle==JKQTPErrorSimpleBarsPolygons)) {
        painter.save();
        painter.setBrush(b);
        painter.setPen(QPen(Qt::NoPen));
        QPolygonF poly;
        //poly << QPointF(xl1p, yl1) << QPointF(xl1m, yl1) << QPointF(xl2m, yl2) << QPointF(xl2p, yl2);
        for (int i=0; i<polyYTopPoints.size(); i++) {
            poly<<polyYTopPoints[i];
        }
        for (int i=polyYBottomPoints.size()-1; i>=0; i--) {
            poly<<polyYBottomPoints[i];
        }
        painter.drawConvexPolygon(poly);
        painter.restore();
    }
    //std::cout<<"end\n";
}

bool JKQTPGraphErrors::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, JKQTBasePlotter * /*parent*/, JKQTPGraph* /*parentGraph*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPErrorPlotstyle /*xErrorStyle*/, JKQTPErrorPlotstyle /*yErrorStyle*/, int /*index*/, QColor &/*errorColor*/, QColor &/*errorFillColor*/)
{
    return false;
}

double JKQTPGraphErrors::getXErrorU(int /*i*/, JKQTPDatastore * /*ds*/) const
{
    return 0.0;
}

double JKQTPGraphErrors::getXErrorL(int /*i*/, JKQTPDatastore * /*ds*/) const
{
    return 0.0;
}

double JKQTPGraphErrors::getYErrorU(int /*i*/, JKQTPDatastore * /*ds*/) const
{
    return 0.0;
}

double JKQTPGraphErrors::getYErrorL(int/* i*/, JKQTPDatastore * /*ds*/) const
{
    return 0.0;
}







JKQTPXGraphErrors::JKQTPXGraphErrors(QColor graphColor):
    JKQTPGraphErrors(graphColor)
{
    xErrorSymmetric=true;
    xErrorColumn=-1;
    xErrorStyle=JKQTPNoError;
    xErrorColumnLower=-1;
}

void JKQTPXGraphErrors::set_xErrorColumn(int __value) {
    if (this->xErrorColumn != __value) {
        this->xErrorColumn = __value;
        if (xErrorColumn>=0 && xErrorStyle==JKQTPNoError) xErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPXGraphErrors::set_xErrorColumnLower(int __value) {
    if (this->xErrorColumnLower != __value) {
        this->xErrorColumnLower = __value;
        if (xErrorColumnLower>=0 && xErrorStyle==JKQTPNoError) xErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPXGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph *parentGraph, int xColumn, int yColumn, int datarange_start, int datarange_end, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    intPlotXYErrorIndicators(painter, parent, parentGraph, xColumn, yColumn, xErrorColumn, -1, xErrorStyle, JKQTPNoError, datarange_start, datarange_end, xErrorColumnLower, -1, xErrorSymmetric, true, xrelshift, yrelshift, dataorder);
}

double JKQTPXGraphErrors::getXErrorU(int i, JKQTPDatastore *ds) const
{
    if (ds && xErrorColumn>=0) {
        return ds->get(xErrorColumn, i);
    }
    return 0.0;
}

double JKQTPXGraphErrors::getXErrorL(int i, JKQTPDatastore *ds) const
{
    if (ds) {
        if (xErrorSymmetric) { if (xErrorColumn>=0) return ds->get(xErrorColumn, i); }
        else if (xErrorColumnLower>=0) return ds->get(xErrorColumnLower, i);
    }
    return 0.0;
}







JKQTPYGraphErrors::JKQTPYGraphErrors(QColor graphColor):
    JKQTPGraphErrors(graphColor)
{
    yErrorColumn=-1;
    yErrorSymmetric=true;
    yErrorStyle=JKQTPNoError;
    yErrorColumnLower=-1;
}

void JKQTPYGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int datarange_start, int datarange_end, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    intPlotXYErrorIndicators(painter, parent, parentGraph, xColumn, yColumn, -1, yErrorColumn, JKQTPNoError, yErrorStyle, datarange_start, datarange_end, -1, yErrorColumnLower, true, yErrorSymmetric, xrelshift, yrelshift, dataorder);
}

double JKQTPYGraphErrors::getYErrorU(int i, JKQTPDatastore *ds) const
{
    if (ds && yErrorColumn>=0) {
        return ds->get(yErrorColumn, i);
    }
    return 0.0;
}

double JKQTPYGraphErrors::getYErrorL(int i, JKQTPDatastore *ds) const
{
    if (ds) {
        if (yErrorSymmetric) { if (yErrorColumn>=0) return ds->get(yErrorColumn, i); }
        else if (yErrorColumnLower>=0) return ds->get(yErrorColumnLower, i);
    }
    return 0.0;
}







JKQTPXYGraphErrors::JKQTPXYGraphErrors(QColor graphColor):
    JKQTPGraphErrors(graphColor)
{
    xErrorSymmetric=true;
    yErrorSymmetric=true;
    yErrorColumn=-1;
    yErrorStyle=JKQTPNoError;
    xErrorColumn=-1;
    xErrorStyle=JKQTPNoError;
    xErrorColumnLower=-1;
    yErrorColumnLower=-1;

}

void JKQTPXYGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int datarange_start, int datarange_end, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    this->intPlotXYErrorIndicators(painter, parent, parentGraph, xColumn, yColumn, xErrorColumn, yErrorColumn, xErrorStyle, yErrorStyle, datarange_start, datarange_end, xErrorColumnLower, yErrorColumnLower, xErrorSymmetric, yErrorSymmetric, xrelshift, yrelshift, dataorder);
}

double JKQTPXYGraphErrors::getXErrorU(int i, JKQTPDatastore *ds) const
{
    if (ds && xErrorColumn>=0) {
        return ds->get(xErrorColumn, i);
    }
    return 0.0;
}

double JKQTPXYGraphErrors::getXErrorL(int i, JKQTPDatastore *ds) const
{
    if (ds && xErrorColumn>=0) {
        if (xErrorSymmetric) { if (xErrorColumn>=0) return ds->get(xErrorColumn, i); }
        else if (xErrorColumnLower>=0) return ds->get(xErrorColumnLower, i);
    }
    return 0.0;
}

double JKQTPXYGraphErrors::getYErrorU(int i, JKQTPDatastore *ds) const
{
    if (ds && yErrorColumn>=0) {
        return ds->get(yErrorColumn, i);
    }
    return 0.0;
}

double JKQTPXYGraphErrors::getYErrorL(int i, JKQTPDatastore *ds) const
{
    if (ds && yErrorColumn>=0) {
        if (yErrorSymmetric) { if (yErrorColumn>=0) return ds->get(yErrorColumn, i); }
        else if (yErrorColumnLower>=0) return ds->get(yErrorColumnLower, i);
    }
    return 0.0;
}









void JKQTPXYGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<int>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<int>(imax));
    }
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


bool JKQTPXGraphErrors::errorUsesColumn(int c) const
{
    return c==(xErrorColumn) || (c==xErrorColumnLower);
}

bool JKQTPGraphErrors::errorUsesColumn(int /*c*/) const
{
    return false;
}

void JKQTPGraphErrors::setErrorColorFromGraphColor(QColor graphColor)
{
    errorColor=graphColor.darker();
    errorFillColor=graphColor.lighter();
    //errorColor.setAlphaF(0.5);
}

bool JKQTPYGraphErrors::errorUsesColumn(int c) const
{
    return (c==yErrorColumn) || (c==yErrorColumnLower);
}

void JKQTPYGraphErrors::set_yErrorColumn(int __value) {
    if (this->yErrorColumn != __value) {
        this->yErrorColumn = __value;
        if (yErrorColumn>=0 && yErrorStyle==JKQTPNoError) yErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPYGraphErrors::set_yErrorColumnLower(int __value) {
    if (this->yErrorColumnLower != __value) {
        this->yErrorColumnLower = __value;
        if (yErrorColumnLower>=0 && yErrorStyle==JKQTPNoError) yErrorStyle=JKQTPErrorBars;
    }
}

bool JKQTPXYGraphErrors::errorUsesColumn(int c) const
{
    return (c==xErrorColumn)||(c==yErrorColumn)||(c==xErrorColumnLower)||(c==yErrorColumnLower);
}

void JKQTPXYGraphErrors::set_xErrorColumn(int __value) {
    if (this->xErrorColumn != __value) {
        this->xErrorColumn = __value;
        if (xErrorColumn>=0 && xErrorStyle==JKQTPNoError) xErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPXYGraphErrors::set_xErrorColumnLower(int __value) {
    if (this->xErrorColumnLower != __value) {
        this->xErrorColumnLower = __value;
        if (xErrorColumnLower>=0 && xErrorStyle==JKQTPNoError) xErrorStyle=JKQTPErrorBars;
    } \
}

void JKQTPXYGraphErrors::set_yErrorColumn(int __value) {
    if (this->yErrorColumn != __value) {
        this->yErrorColumn = __value;
        if (yErrorColumn>=0 && yErrorStyle==JKQTPNoError) yErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPXYGraphErrors::set_yErrorColumnLower(int __value) {
    if (this->yErrorColumnLower != __value) {
        this->yErrorColumnLower = __value;
        if (yErrorColumnLower>=0 && yErrorStyle==JKQTPNoError) yErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPXYGraphErrors::set_xErrorColumn(size_t __value)
{
    set_xErrorColumn(static_cast<int>(__value));
}

void JKQTPXYGraphErrors::set_xErrorColumnLower(size_t __value)
{
    set_xErrorColumnLower(static_cast<int>(__value));
}

void JKQTPXYGraphErrors::set_yErrorColumn(size_t __value)
{
    set_yErrorColumn(static_cast<int>(__value));
}

void JKQTPXYGraphErrors::set_yErrorColumnLower(size_t __value)
{
    set_yErrorColumnLower(static_cast<int>(__value));
}


JKQTPPlotObject::JKQTPPlotObject(JKQTBasePlotter *parent):
    JKQTPPlotElement(parent)
{

}

JKQTPPlotObject::JKQTPPlotObject(JKQTPLotter *parent):
    JKQTPPlotElement(parent)
{

}

JKQTPPlotObject::~JKQTPPlotObject()
{

}
