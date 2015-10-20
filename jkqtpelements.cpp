/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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



#include "jkqtpelements.h"
#include "jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtptools.h"
#include "jkqtpimageelements.h"
#include "jkqtpbaseelements.h"
#include "jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPgraph::JKQTPgraph(JKQtBasePlotter* parent):
    QObject(parent)
{
    datarange_end=datarange_start=-1;
    title="";
    visible=true;
    setParent(parent);
}

JKQTPgraph::JKQTPgraph(JKQtPlotter *parent):
    QObject(parent->get_plotter())
{
    datarange_end=datarange_start=-1;
    title="";
    visible=true;
    setParent(parent->get_plotter());
}

QPointF JKQTPgraph::transform(const QPointF& x) {
    if (xAxis&&yAxis) return QPointF(xAxis->x2p(x.x()), yAxis->x2p(x.y()));
    return QPointF();
}

QPointF JKQTPgraph::backTransform(const QPointF& x) {
    if (xAxis&&yAxis) return QPointF(xAxis->p2x(x.x()), yAxis->p2x(x.y()));
    return QPointF();
}

void JKQTPgraph::setParent(JKQtBasePlotter* parent) {
    this->parent=parent;
    if (parent) {
        this->xAxis=parent->getXAxis();
        this->yAxis=parent->getYAxis();
    } else {
        xAxis=NULL;
        yAxis=NULL;
    }
    QObject::setParent(parent);
}

void JKQTPgraph::setParent(JKQtPlotter *parent)
{
    setParent(parent->get_plotter());
}

QVector<QPointF> JKQTPgraph::transform(const QVector<QPointF>& x) {
    QVector<QPointF> res;
    for (int i=0; i<x.size(); i++) {
        res.append(transform(x[i]));
    }
    return res;
}

QPainterPath JKQTPgraph::transformToLinePath(const QVector<QPointF>& x) {
    QPainterPath res;
    if (x.size()>0) {
        res.moveTo(transform(x[0]));
        for (int i=1; i<x.size(); i++) {
            res.lineTo(transform(x[i]));
        }
    }
    return res;
}

bool JKQTPgraph::getDataMinMax(int column, double &minx, double &maxx, double &smallestGreaterZero)
{
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(column).getRows();
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
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

void JKQTPgraph::getOutsideSize(JKQTPEnhancedPainter& /*painter*/, int& leftSpace, int& rightSpace, int& topSpace, int& bottomspace) {
    leftSpace=0;
    rightSpace=0;
    topSpace=0;
    bottomspace=0;
}

void JKQTPgraph::drawOutside(JKQTPEnhancedPainter& /*painter*/, QRect /*leftSpace*/, QRect /*rightSpace*/, QRect /*topSpace*/, QRect /*bottomSpace*/) {
    /*qDebug()<<"outside("<<title<<"): left="<<leftSpace<<"   right="<<rightSpace<<"   top="<<topSpace<<"   bottom="<<bottomSpace;
    painter.save();
    painter.setPen(QColor("black"));
    painter.drawRect(leftSpace);
    painter.setPen(QColor("red"));
    painter.drawRect(topSpace);
    painter.setPen(QColor("blue"));
    painter.drawRect(rightSpace);
    painter.setPen(QColor("green"));
    painter.drawRect(bottomSpace);
    painter.restore();*/
}

bool JKQTPgraph::usesColumn(int /*column*/)
{
    return false;
}






JKQTPxyGraph::JKQTPxyGraph(JKQtBasePlotter* parent):
    JKQTPgraph(parent)
{
    sortData=Unsorted;
    xColumn=-1;
    yColumn=-1;


}

JKQTPxyGraph::JKQTPxyGraph(JKQtPlotter *parent):
    JKQTPgraph(parent)
{
    sortData=Unsorted;
    xColumn=-1;
    yColumn=-1;

}

bool JKQTPxyGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==NULL)  return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(xColumn,i);
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

bool JKQTPxyGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(yColumn,i);
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

bool JKQTPxyGraph::usesColumn(int column)
{
    return (column==xColumn)||(column==yColumn);
}




JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(JKQtBasePlotter *parent):
    JKQTPgraph(parent)
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

JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(int dataColumn, JKQtBasePlotter *parent):
    JKQTPgraph(parent)
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

JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQtBasePlotter *parent):
    JKQTPgraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=lineWidth;
    parentPlotStyle=-1;
}


JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(JKQtPlotter *parent):
    JKQTPgraph(parent)
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

JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(int dataColumn, JKQtPlotter *parent):
    JKQTPgraph(parent)
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

JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQtPlotter *parent):
    JKQTPgraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=lineWidth;
    parentPlotStyle=-1;
}

JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, JKQtPlotter *parent):
    JKQTPgraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=style;
    this->lineWidth=2.0;
    parentPlotStyle=-1;
}

JKQTPsingleColumnGraph::JKQTPsingleColumnGraph(int dataColumn, QColor color, JKQtPlotter *parent):
    JKQTPgraph(parent)
{
    sortData=Unsorted;
    this->dataColumn=dataColumn;
    this->color=color;
    this->style=Qt::SolidLine;
    this->lineWidth=2.0;
    parentPlotStyle=-1;
}
QColor JKQTPsingleColumnGraph::getKeyLabelColor()
{
    return color;
}

bool JKQTPsingleColumnGraph::usesColumn(int c)
{
    return c==dataColumn;
}

QPen JKQTPsingleColumnGraph::getLinePen(JKQTPEnhancedPainter& painter) const
{
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

void JKQTPsingleColumnGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==NULL)  return ;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(dataColumn).getRows();
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPsingleColumnGraph::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(dataColumn,i);
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());


    }
}



















JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(JKQtBasePlotter *parent):
    JKQTPsingleColumnGraph(parent)
{
    baseline=0;
    yPeaks=true;
    peakHeight=1;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQtBasePlotter *parent):
    JKQTPsingleColumnGraph(dataColumn, color, Qt::SolidLine, 2.0, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQtBasePlotter *parent):
    JKQTPsingleColumnGraph(dataColumn, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
}


JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(JKQtPlotter *parent):
    JKQTPsingleColumnGraph(parent)
{
    baseline=0;
    yPeaks=true;
    peakHeight=1;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQtPlotter *parent):
    JKQTPsingleColumnGraph(dataColumn, color, Qt::SolidLine, 2.0, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQtPlotter *parent):
    JKQTPsingleColumnGraph(dataColumn, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
}

bool JKQTPPeakStreamGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    if (yPeaks) {
        return getDataMinMax(dataColumn, minx, maxx, smallestGreaterZero);
    } else {
        minx=qMin(baseline, baseline+peakHeight);
        maxx=qMax(baseline, baseline+peakHeight);
        return true;
        //smallestGreaterZero=qMax(double(0.0), qMin(baseline, baseline+peakHeight));
    }
    return false;
}

bool JKQTPPeakStreamGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (!yPeaks) {
        return getDataMinMax(dataColumn, miny, maxy, smallestGreaterZero);
    } else {
        miny=qMin(baseline, baseline+peakHeight);
        maxy=qMax(baseline, baseline+peakHeight);
        return true;
        //smallestGreaterZero=qMax(double(0.0), qMin(baseline, baseline+peakHeight));
    }
    return false;
}

void JKQTPPeakStreamGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPPeakStreamGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=getLinePen(painter);
    p.setCapStyle(Qt::FlatCap);

    int imax=datastore->getColumn(dataColumn).getRows();
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<QLineF> lines;

    if (yPeaks) {
        if (drawBaseline) {
            lines<<QLineF(transform(parent->getXMin(), baseline), transform(parent->getXMax(), baseline));
        }
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound(imin, getDataIndex(iii), imax);
            const double xv=datastore->get(dataColumn,i);
            if (JKQTPIsOKFloat(xv)) {
                lines<<QLineF(transform(xv, baseline), transform(xv, baseline+peakHeight));
            }
        }
    } else {
        if (drawBaseline) {
            lines<<QLineF(transform(baseline, parent->getYMin()), transform(baseline, parent->getYMax()));
        }
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound(imin, getDataIndex(iii), imax);
            const double yv=datastore->get(dataColumn,i);
            if (JKQTPIsOKFloat(yv)) {
                lines<<QLineF(transform(baseline, yv), transform(baseline+peakHeight, yv));
            }
        }
    }

    painter.setPen(p);
    if (lines.size()>0) painter.drawLines(lines);


    painter.restore();

    drawErrorsAfter(painter);
}

void JKQTPPeakStreamGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save();
    QPen p=getLinePen(painter);
    painter.setPen(p);
    if (yPeaks) {
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,qMin(parent->pt2px(painter, p.widthF()), rect.width()/10.0)));
        if (drawBaseline) {
            if (peakHeight>=0) painter.drawLine(rect.bottomLeft(), rect.bottomRight());
            else painter.drawLine(rect.topLeft(), rect.topRight());
        }
        painter.drawLine(QPointF(rect.left()+rect.width()*0.1, rect.top()),QPointF( rect.left()+rect.width()*0.1, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.55, rect.top()), QPointF(rect.left()+rect.width()*0.55, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.75, rect.top()), QPointF(rect.left()+rect.width()*0.75, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.9, rect.top()), QPointF(rect.left()+rect.width()*0.9, rect.bottom()));
    } else {
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,qMin(parent->pt2px(painter, p.widthF()), rect.height()/15.0)));
        if (drawBaseline) {
            if (peakHeight>=0) painter.drawLine(rect.bottomLeft(), rect.topLeft());
            else painter.drawLine(rect.bottomRight(), rect.topRight());
        }
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.1), QPointF(rect.right(), rect.top()+rect.height()*0.1));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.55), QPointF(rect.right(), rect.top()+rect.height()*0.55));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.75), QPointF(rect.right(), rect.top()+rect.height()*0.75));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.9), QPointF(rect.right(), rect.top()+rect.height()*0.9));
    }

    painter.restore();
}









JKQTPgraphErrors::JKQTPgraphErrors(QColor graphColor) {
    errorColor=graphColor.darker();
    errorStyle=Qt::SolidLine;
    errorWidth=2;
    errorFillColor=graphColor.lighter();
    errorColor.setAlphaF(0.5);
    errorFillStyle=Qt::SolidPattern;
    errorbarSize=7;
}



void JKQTPgraphErrors::intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, long long datarange_start, long long datarange_end, int xErrorColumnLower, int yErrorColumnLower, bool xErrorSymmetric, bool yErrorSymmetric, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    //std::cout<<"JKQTPgraphErrors::intPlotXYErrorIndicators(p, "<<parent<<", "<<xColumn<<", "<<yColumn<<", "<<xErrorColumn<<", "<<yErrorColumn<<", "<<xErrorStyle<<", "<<yErrorStyle<<", ...)\n";
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    if ((yErrorStyle==JKQTPnoError) && (xErrorStyle==JKQTPnoError)) return;
    bool visX=(xErrorStyle!=JKQTPnoError)&&(xErrorColumn>=0||xErrorColumnLower>=0)&&(xColumn>=0)&&(yColumn>=0);
    bool visY=(yErrorStyle!=JKQTPnoError)&&(yErrorColumn>=0||yErrorColumnLower>=0)&&(xColumn>=0)&&(yColumn>=0);
    if (!visX&&!visY) return;
    //std::cout<<"   JKQTPgraphErrors::intPlotXYErrorIndicators(p, "<<parent<<", "<<xColumn<<", "<<yColumn<<", "<<xErrorColumn<<", "<<yErrorColumn<<", ...)\n";

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

    unsigned long long imaxx=0, imaxy=0;
    if (xColumn>=0) imaxx=datastore->getColumn(xColumn).getRows();
    if (yColumn>=0) imaxy=datastore->getColumn(yColumn).getRows();
    int imax=qMin(imaxx, imaxy);
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
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
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
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
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);

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

        double xe=0;   if (xErrorStyle!=JKQTPnoError && xErrorColumn>=0) { xe=datastore->get(xErrorColumn,i); plotupperbarx=true; }
        double ye=0;   if (yErrorStyle!=JKQTPnoError && yErrorColumn>=0) { ye=datastore->get(yErrorColumn,i); plotupperbary=true; }
        double xl=0;   if (xErrorSymmetric) { xl=xe; plotlowerbarx=plotupperbarx||(xl>0); }
                       else if (xErrorStyle!=JKQTPnoError && xErrorColumnLower>=0) { xl=datastore->get(xErrorColumnLower,i); plotlowerbarx=true; }
        double yl=0;   if (yErrorSymmetric) { yl=ye; plotlowerbary=plotupperbary||(yl>0); }
                       else if (yErrorStyle!=JKQTPnoError && yErrorColumnLower>=0) { yl=datastore->get(yErrorColumnLower,i); plotlowerbary=true; }
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(xe) && JKQTPIsOKFloat(ye) && JKQTPIsOKFloat(xl) && JKQTPIsOKFloat(yl)) {
            double x=xAxis->x2p(xv+xrelshift*deltax); bool xok=JKQTPIsOKFloat(x);
            double y=yAxis->x2p(yv+yrelshift*deltay); bool yok=JKQTPIsOKFloat(y);
            QColor terrCol=errorColor;
            QColor terrFillCol=errorFillColor;
            defaultErrorColor = defaultErrorColor && !this->intPlotXYErrorIndicatorsGetColor(painter, parent, xAxis, yAxis, xColumn, yColumn, xErrorColumn, yErrorColumn, xErrorStyle, yErrorStyle, i, terrCol, terrFillCol);

            // x-errorpolygons
            if (/*pastFirst &&*/ (xErrorStyle==JKQTPerrorPolygons || xErrorStyle==JKQTPerrorBarsPolygons || xErrorStyle==JKQTPerrorSimpleBarsPolygons)) {
                //double xl1m=xmold;
                //double xl1p=xpold;
                //double yl1=yold;
                double xl2m=xAxis->x2p(xv+xrelshift*deltax-xl);
                double xl2p=xAxis->x2p(xv+xrelshift*deltax+xe);
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
            if (/*pastFirst &&*/ (yErrorStyle==JKQTPerrorPolygons || yErrorStyle==JKQTPerrorBarsPolygons || yErrorStyle==JKQTPerrorSimpleBarsPolygons)) {
                //double yl1m=ymold;
                //double yl1p=ypold;
                //double xl1=xold;
                double yl2m=yAxis->x2p(yv+yrelshift*deltay-yl);
                double yl2p=yAxis->x2p(yv+yrelshift*deltay+ye);
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
            if ((xErrorColumn>=0 || xErrorColumnLower>=0) && (xErrorStyle==JKQTPerrorBars || xErrorStyle==JKQTPerrorBarsLines|| xErrorStyle==JKQTPerrorBarsPolygons
                || xErrorStyle==JKQTPerrorSimpleBars || xErrorStyle==JKQTPerrorSimpleBarsLines|| xErrorStyle==JKQTPerrorSimpleBarsPolygons)) {
                    double x0=xAxis->x2p(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                    double x1=xAxis->x2p(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                    painter.save();
                    QPen pp=p;
                    pp.setColor(terrCol);
                    painter.setPen(pp);
                    if (x0ok&&x1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x0, y, x1, y));
                        if (xErrorStyle==JKQTPerrorBars || xErrorStyle==JKQTPerrorBarsLines|| xErrorStyle==JKQTPerrorBarsPolygons) {
                            if (plotlowerbarx) painter.drawLine(QLineF(x0,y-ebs_px/2.0,x0,y+ebs_px/2.0));
                            if (plotupperbarx) painter.drawLine(QLineF(x1,y-ebs_px/2.0,x1,y+ebs_px/2.0));
                        }
                    } else if (x0ok&&!x1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x0, y, x, y));
                        if (xErrorStyle==JKQTPerrorBars || xErrorStyle==JKQTPerrorBarsLines|| xErrorStyle==JKQTPerrorBarsPolygons) {
                            if (plotlowerbarx) painter.drawLine(QLineF(x0,y-ebs_px/2.0,x0,y+ebs_px/2.0));
                        }
                        if (x0<x) painter.drawLine(QLineF(x,y,xAxis->x2p(parent->getXMax()),y));
                        else painter.drawLine(QLineF(x,y,xAxis->x2p(parent->getXMin()),y));
                    } else if (!x0ok&&x1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x1, y, x, y));
                        if (xErrorStyle==JKQTPerrorBars || xErrorStyle==JKQTPerrorBarsLines|| xErrorStyle==JKQTPerrorBarsPolygons) {
                            if (plotupperbarx) painter.drawLine(QLineF(x1,y-ebs_px/2.0,x1,y+ebs_px/2.0));
                        }
                        if (x1<x) painter.drawLine(QLineF(x,y,xAxis->x2p(parent->getXMin()),y));
                        else painter.drawLine(QLineF(x,y,xAxis->x2p(parent->getXMax()),y));
                    }
                    painter.restore();
                }
                // y-errorbars
                if ((yErrorColumn>=0 || yErrorColumnLower>=0) && (yErrorStyle==JKQTPerrorBars || yErrorStyle==JKQTPerrorBarsLines || yErrorStyle==JKQTPerrorBarsPolygons
                    || yErrorStyle==JKQTPerrorSimpleBars || yErrorStyle==JKQTPerrorSimpleBarsLines || yErrorStyle==JKQTPerrorSimpleBarsPolygons)) {
                    double y0=yAxis->x2p(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                    double y1=yAxis->x2p(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                    painter.save();
                    QPen pp=p;
                    pp.setColor(terrCol);
                    painter.setPen(pp);
                    if (y0ok&&y1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x, y0, x, y1));
                        if (yErrorStyle==JKQTPerrorBars || yErrorStyle==JKQTPerrorBarsLines || yErrorStyle==JKQTPerrorBarsPolygons) {
                            if (plotlowerbary) painter.drawLine(QLineF(x-ebs_px/2.0,y0,x+ebs_px/2.0,y0));
                            if (plotupperbary) painter.drawLine(QLineF(x-ebs_px/2.0,y1,x+ebs_px/2.0,y1));
                        }
                    } else if (y0ok&&!y1ok&&xok&&yok) {   // upper errorbar OK, lower errorbar NAN
                        painter.drawLine(QLineF(x, y0, x, y));
                        if (yErrorStyle==JKQTPerrorBars || yErrorStyle==JKQTPerrorBarsLines || yErrorStyle==JKQTPerrorBarsPolygons) {
                            if (plotlowerbary) painter.drawLine(QLineF(x-ebs_px/2.0,y0,x+ebs_px/2.0,y0));
                        }
                        if (y0<y) painter.drawLine(QLineF(x,y,x,yAxis->x2p(parent->getYMin())));
                        else painter.drawLine(QLineF(x,y,x,yAxis->x2p(parent->getYMax()))); // inverted axis!
                    } else if (!y0ok&&y1ok&&xok&&yok) {
                        painter.drawLine(QLineF(x, y1, x, y));
                        if (yErrorStyle==JKQTPerrorBars || yErrorStyle==JKQTPerrorBarsLines || yErrorStyle==JKQTPerrorBarsPolygons) {
                            if (plotupperbary) painter.drawLine(QLineF(x-ebs_px/2.0,y1,x+ebs_px/2.0,y1));
                        }
                        if (y1<y) painter.drawLine(QLineF(x,y,x,yAxis->x2p(parent->getYMax())));
                        else painter.drawLine(QLineF(x,y,x,yAxis->x2p(parent->getYMin())));
                    }

                    painter.restore();
                }

                // error boxes
                if (yErrorStyle==JKQTPerrorBoxes || xErrorStyle==JKQTPerrorBoxes || yErrorStyle==JKQTPerrorEllipses || xErrorStyle==JKQTPerrorEllipses ) {
                    double y0=yAxis->x2p(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                    double y1=yAxis->x2p(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                    double x0=xAxis->x2p(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                    double x1=xAxis->x2p(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                    painter.save();
                    QPen pp=p;
                    pp.setColor(terrCol);
                    painter.setPen(pp);
                    QBrush bb=b;
                    bb.setColor(terrFillCol);
                    painter.setBrush(bb);

                    QRectF errRect=QRectF(QPointF(x0,y0), QPointF(x1,y1));
                    if ((y0ok&&y1ok)||(x0ok&&x1ok)) {
                        if (yErrorStyle==JKQTPerrorEllipses || xErrorStyle==JKQTPerrorEllipses) painter.drawEllipse(errRect);
                        else painter.drawRect(errRect);
                    }

                    painter.restore();
                }            //}

            // x-errorlines
            if (pastFirst && (xErrorStyle==JKQTPerrorLines || xErrorStyle==JKQTPerrorBarsLines || xErrorStyle==JKQTPerrorSimpleBarsLines)) {
                double xl1m=xmold;
                double xl1p=xpold;
                double yl1=yold;
                double xl2m=xAxis->x2p(xv+xrelshift*deltax-xl);
                double xl2p=xAxis->x2p(xv+xrelshift*deltax+xe);
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
            if (pastFirst && (yErrorStyle==JKQTPerrorLines || yErrorStyle==JKQTPerrorBarsLines || yErrorStyle==JKQTPerrorSimpleBarsLines)) {
                double yl1m=ymold;
                double yl1p=ypold;
                double xl1=xold;
                double yl2m=yAxis->x2p(yv+yrelshift*deltay-yl);
                double yl2p=yAxis->x2p(yv+yrelshift*deltay+ye);
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
            xmold=xAxis->x2p(xv+xrelshift*deltax-xl);
            xpold=xAxis->x2p(xv+xrelshift*deltax+xe);
            yold=y;
            ymold=yAxis->x2p(yv+yrelshift*deltay-yl);
            ypold=yAxis->x2p(yv+yrelshift*deltay+ye);
        }
    }
    // x-errorpolygons
    if ((polyXTopPoints.size()>0 || polyXBottomPoints.size()>0) && (xErrorStyle==JKQTPerrorPolygons || xErrorStyle==JKQTPerrorBarsPolygons || xErrorStyle==JKQTPerrorSimpleBarsPolygons)) {
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
    if ((polyYTopPoints.size()>0 || polyYBottomPoints.size()>0) && (yErrorStyle==JKQTPerrorPolygons || yErrorStyle==JKQTPerrorBarsPolygons || yErrorStyle==JKQTPerrorSimpleBarsPolygons)) {
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

bool JKQTPgraphErrors::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, JKQtBasePlotter */*parent*/, JKQTPcoordinateAxis* /*xAxis*/, JKQTPcoordinateAxis* /*yAxis*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPerrorPlotstyle /*xErrorStyle*/, JKQTPerrorPlotstyle /*yErrorStyle*/, long long /*index*/, QColor &/*errorColor*/, QColor &/*errorFillColor*/)
{
    return false;
}

double JKQTPgraphErrors::getXErrorU(int /*i*/, JKQTPdatastore */*ds*/) const
{
    return 0.0;
}

double JKQTPgraphErrors::getXErrorL(int /*i*/, JKQTPdatastore */*ds*/) const
{
    return 0.0;
}

double JKQTPgraphErrors::getYErrorU(int /*i*/, JKQTPdatastore */*ds*/) const
{
    return 0.0;
}

double JKQTPgraphErrors::getYErrorL(int/* i*/, JKQTPdatastore */*ds*/) const
{
    return 0.0;
}







JKQTPxGraphErrors::JKQTPxGraphErrors(QColor graphColor):
    JKQTPgraphErrors(graphColor)
{
    xErrorSymmetric=true;
    xErrorColumn=-1;
    xErrorStyle=JKQTPnoError;
    xErrorColumnLower=-1;
}

void JKQTPxGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis *xAxis, JKQTPcoordinateAxis *yAxis, int xColumn, int yColumn, long long datarange_start, long long datarange_end, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    intPlotXYErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, xErrorColumn, -1, xErrorStyle, JKQTPnoError, datarange_start, datarange_end, xErrorColumnLower, -1, xErrorSymmetric, true, xrelshift, yrelshift, dataorder);
}

double JKQTPxGraphErrors::getXErrorU(int i, JKQTPdatastore *ds) const
{
    if (ds && xErrorColumn>=0) {
        return ds->get(xErrorColumn, i);
    }
    return 0.0;
}

double JKQTPxGraphErrors::getXErrorL(int i, JKQTPdatastore *ds) const
{
    if (ds) {
        if (xErrorSymmetric) { if (xErrorColumn>=0) return ds->get(xErrorColumn, i); }
        else if (xErrorColumnLower>=0) return ds->get(xErrorColumnLower, i);
    }
    return 0.0;
}







JKQTPyGraphErrors::JKQTPyGraphErrors(QColor graphColor):
    JKQTPgraphErrors(graphColor)
{
    yErrorColumn=-1;
    yErrorSymmetric=true;
    yErrorStyle=JKQTPnoError;
    yErrorColumnLower=-1;
}

void JKQTPyGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, long long datarange_start, long long datarange_end, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    intPlotXYErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, -1, yErrorColumn, JKQTPnoError, yErrorStyle, datarange_start, datarange_end, -1, yErrorColumnLower, true, yErrorSymmetric, xrelshift, yrelshift, dataorder);
}

double JKQTPyGraphErrors::getYErrorU(int i, JKQTPdatastore *ds) const
{
    if (ds && yErrorColumn>=0) {
        return ds->get(yErrorColumn, i);
    }
    return 0.0;
}

double JKQTPyGraphErrors::getYErrorL(int i, JKQTPdatastore *ds) const
{
    if (ds) {
        if (yErrorSymmetric) { if (yErrorColumn>=0) return ds->get(yErrorColumn, i); }
        else if (yErrorColumnLower>=0) return ds->get(yErrorColumnLower, i);
    }
    return 0.0;
}







JKQTPxyGraphErrors::JKQTPxyGraphErrors(QColor graphColor):
    JKQTPgraphErrors(graphColor)
{
    xErrorSymmetric=true;
    yErrorSymmetric=true;
    yErrorColumn=-1;
    yErrorStyle=JKQTPnoError;
    xErrorColumn=-1;
    xErrorStyle=JKQTPnoError;
    xErrorColumnLower=-1;
    yErrorColumnLower=-1;

}

void JKQTPxyGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, long long datarange_start, long long datarange_end, double xrelshift, double yrelshift, const  QVector<int>* dataorder) {
    this->intPlotXYErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, xErrorColumn, yErrorColumn, xErrorStyle, yErrorStyle, datarange_start, datarange_end, xErrorColumnLower, yErrorColumnLower, xErrorSymmetric, yErrorSymmetric, xrelshift, yrelshift, dataorder);
}

double JKQTPxyGraphErrors::getXErrorU(int i, JKQTPdatastore *ds) const
{
    if (ds && xErrorColumn>=0) {
        return ds->get(xErrorColumn, i);
    }
    return 0.0;
}

double JKQTPxyGraphErrors::getXErrorL(int i, JKQTPdatastore *ds) const
{
    if (ds && xErrorColumn>=0) {
        if (xErrorSymmetric) { if (xErrorColumn>=0) return ds->get(xErrorColumn, i); }
        else if (xErrorColumnLower>=0) return ds->get(xErrorColumnLower, i);
    }
    return 0.0;
}

double JKQTPxyGraphErrors::getYErrorU(int i, JKQTPdatastore *ds) const
{
    if (ds && yErrorColumn>=0) {
        return ds->get(yErrorColumn, i);
    }
    return 0.0;
}

double JKQTPxyGraphErrors::getYErrorL(int i, JKQTPdatastore *ds) const
{
    if (ds && yErrorColumn>=0) {
        if (yErrorSymmetric) { if (yErrorColumn>=0) return ds->get(yErrorColumn, i); }
        else if (yErrorColumnLower>=0) return ds->get(yErrorColumnLower, i);
    }
    return 0.0;
}












JKQTPxyLineGraph::JKQTPxyLineGraph(JKQtPlotter* parent):
    JKQTPxyGraph(parent)
{
    sortData=JKQTPxyLineGraph::Unsorted;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    symbolSize=12;
    symbolWidth=1;
    symbol=JKQTPnoSymbol;
    drawLine=true;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
    fillColor=color;
}

JKQTPxyLineGraph::JKQTPxyLineGraph(JKQtBasePlotter* parent):
    JKQTPxyGraph(parent)
{
    sortData=JKQTPxyLineGraph::Unsorted;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    symbolSize=12;
    symbolWidth=1;
    symbol=JKQTPnoSymbol;
    drawLine=true;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
    fillColor=color;
}

void JKQTPxyLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPxyLineGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    //qDebug()<<"JKQTPxyLineGraph::draw();";

    drawErrorsBefore(painter);

    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<1;
    painter.save();
    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<2;

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);


    QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
    QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
    if (selectionLineColor!=Qt::transparent) {
        penSelection.setColor(selectionLineColor);
    }
    penSelection.setWidthF(penSelection.widthF()*3.0);


    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<3<<" imin="<<imin<<" imax="<<imax;
    //double xold=-1;
    //double yold=-1;
    //bool first=false;
    //QVector<QLineF> lines;
    QPolygonF linesP;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        double x=xAxis->x2p(xv);
        double y=yAxis->x2p(yv);
        //qDebug()<<"JKQTPxyLineGraph::draw(): (xv, yv) =    ( "<<xv<<", "<<yv<<" )";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
            QColor symbColor=color;
            QColor symbFillColor=fillColor;// symbColor.lighter();

            if (drawSelectionLine && symbol!=JKQTPnoSymbol) {
                plotSymbol(painter, x, y, JKQTPfilledCircle, parent->pt2px(painter, symbolSize*1.5), parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), penSelection.color(), penSelection.color());
            }
            plotSymbol(painter, x, y, symbol, parent->pt2px(painter, symbolSize), parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), symbColor, symbFillColor);
            /*if (drawLine && first) {
                double xl1=xold;
                double yl1=yold;
                double xl2=x;
                double yl2=y;
                lines.append(QLineF(xl1, yl1, xl2, yl2));
            }*/
            if (drawLine) {
                linesP<<QPointF(x,y);

            }
//            xold=x;
//            yold=y;
//            first=true;
        }
    }
    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<4<<" lines="<<lines.size();
    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<5<<"  p="<<painter.pen();
    if (linesP.size()>0) {
        if (drawSelectionLine) {
            painter.setPen(penSelection);
            //painter.drawLines(lines);
            painter.drawPolyline(linesP);
        }
        painter.setPen(p);
        //painter.drawLines(lines);
        painter.drawPolyline(linesP);
    }
    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<6;
    painter.restore();
    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<7;
    drawErrorsAfter(painter);
    //qDebug()<<"JKQTPxyLineGraph::draw() ... done";
}

void JKQTPxyLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    const double minSize=qMin(rect.width(), rect.height());
    const double maxSize=qMax(rect.width(), rect.height());
    double symbolSize=parent->pt2px(painter, this->symbolSize);
    if (symbolSize>minSize*0.9) symbolSize=minSize*0.9;
    double symbolWidth=parent->pt2px(painter, this->symbolWidth*parent->get_lineWidthMultiplier());
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;
    double lineWidth=parent->pt2px(painter, this->lineWidth*parent->get_lineWidthMultiplier());
    if (lineWidth>0.5*maxSize) lineWidth=0.5*maxSize;

    painter.save();
    QPen p=painter.pen();
    p.setColor(getKeyLabelColor());
    p.setStyle(style);
    p.setWidthF(lineWidth);
    painter.setPen(p);
    int y=rect.top()+rect.height()/2.0;
    if (drawLine) painter.drawLine(rect.left(), y, rect.right(), y);
    plotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, symbol, symbolSize, symbolWidth, getKeyLabelColor(), fillColor);
    painter.restore();
}

QColor JKQTPxyLineGraph::getKeyLabelColor() {
    return color;
}


bool JKQTPxyLineErrorGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero) {
    if (xErrorColumn<0 || xErrorStyle==JKQTPnoError) {
        return JKQTPxyLineGraph::getXMinMax(minx, maxx, smallestGreaterZero);
    } else {
        bool start=true;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        if (parent==NULL) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, (long long)imax);
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, (long long)imax);
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double xvsgz;
            double xv=datastore->get(xColumn,i)+getXErrorU(i, datastore);
            if (JKQTPIsOKFloat(xv)) {
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                xv=datastore->get(xColumn,i)-getXErrorL(i, datastore);
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

bool JKQTPxyLineErrorGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero) {
    if (yErrorColumn<0 || yErrorStyle==JKQTPnoError) {
        return JKQTPxyLineGraph::getYMinMax(miny, maxy, smallestGreaterZero);
    } else {
        bool start=true;
        miny=0;
        maxy=0;
        smallestGreaterZero=0;

        if (parent==NULL) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, (long long)imax);
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, (long long)imax);
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(yColumn,i)+getYErrorU(i, datastore);
            if (JKQTPIsOKFloat(yv)) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                yv=datastore->get(yColumn,i)-getYErrorL(i, datastore);
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

bool JKQTPxyLineErrorGraph::usesColumn(int c)
{
    return JKQTPxyLineGraph::usesColumn(c)||JKQTPxyGraphErrors::errorUsesColumn(c);
}










JKQTPimpulsesHorizontalGraph::JKQTPimpulsesHorizontalGraph(JKQtBasePlotter* parent):
    JKQTPxyGraph(parent)
{
    baseline=0;
    color=QColor("red");
    lineWidth=3;
    parentPlotStyle=-1;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        color=parent->getPlotStyle(parentPlotStyle).color();
    }
}

JKQTPimpulsesHorizontalGraph::JKQTPimpulsesHorizontalGraph(JKQtPlotter* parent):
    JKQTPxyGraph(parent)
{
    baseline=0;
    color=QColor("red");
    lineWidth=3;
    parentPlotStyle=-1;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        color=parent->getPlotStyle(parentPlotStyle).color();
    }
}
void JKQTPimpulsesHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPimpulsesHorizontalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(Qt::SolidLine);
    p.setCapStyle(Qt::FlatCap);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    //double xold=-1;
    //double yold=-1;
    double x0=xAxis->x2p(baseline);
    if (parent->getXAxis()->isLogAxis()) {
        if (baseline>0 && baseline>parent->getXAxis()->getMin()) x0=xAxis->x2p(baseline);
        else x0=xAxis->x2p(parent->getXAxis()->getMin());
    }
//    double y0=yAxis->x2p(baseline);
//    if (parent->getYAxis()->isLogAxis()) {
//        y0=yAxis->x2p(parent->getYAxis()->getMin());
//        if (baseline>0 && baseline>parent->getYAxis()->getMin()) y0=yAxis->x2p(baseline);
//        else y0=yAxis->x2p(parent->getYAxis()->getMin());
//    }
    //bool first=false;
    QVector<QLineF> lines;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv);
            double y=yAxis->x2p(yv);


            lines.append(QLineF(x0, y, x, y));

//            xold=x;
//            yold=y;
            //first=true;
        }
    }
    painter.setPen(p);
    if (lines.size()>0) painter.drawLines(lines);
    painter.restore();

    drawErrorsAfter(painter);
}

void JKQTPimpulsesHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {


    painter.save();
    QPen p=painter.pen();
    p.setColor(color);
    p.setStyle(Qt::SolidLine);
    p.setCapStyle(Qt::FlatCap);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,qMin(parent->pt2px(painter, qMax(.01, lineWidth)), rect.height()/2.0)*parent->get_lineWidthMultiplier()));
    painter.setPen(p);
    int y=rect.top()+rect.height()/2.0;
    painter.drawLine(rect.left(), y, rect.right(), y);
    painter.restore();
}

QColor JKQTPimpulsesHorizontalGraph::getKeyLabelColor() {
    return color;
}











JKQTPimpulsesVerticalGraph::JKQTPimpulsesVerticalGraph(JKQtBasePlotter* parent):
    JKQTPimpulsesHorizontalGraph(parent)
{
}

JKQTPimpulsesVerticalGraph::JKQTPimpulsesVerticalGraph(JKQtPlotter *parent):
    JKQTPimpulsesHorizontalGraph(parent)
{

}

void JKQTPimpulsesVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPimpulsesVerticalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(Qt::SolidLine);
    p.setCapStyle(Qt::FlatCap);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    //double xold=-1;
    //double yold=-1;
    //bool first=false;
//    double x0=xAxis->x2p(baseline);
//    if (parent->getXAxis()->isLogAxis()) {
//        if (baseline>0 && baseline>parent->getXAxis()->getMin()) x0=xAxis->x2p(baseline);
//        else x0=xAxis->x2p(parent->getXAxis()->getMin());
//    }
    double y0=yAxis->x2p(baseline);
    if (parent->getYAxis()->isLogAxis()) {
        y0=yAxis->x2p(parent->getYAxis()->getMin());
        if (baseline>0 && baseline>parent->getYAxis()->getMin()) y0=yAxis->x2p(baseline);
        else y0=yAxis->x2p(parent->getYAxis()->getMin());
    }
    QVector<QLineF> lines;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) ) {
            double x=xAxis->x2p(xv);
            double y=yAxis->x2p(yv);


            lines.append(QLineF(x, y0, x, y));

            //xold=x;
            //yold=y;
            //first=true;
        }
    }
    painter.setPen(p);
    if (lines.size()>0) painter.drawLines(lines);
    painter.restore();

    drawErrorsAfter(painter);
}



















JKQTPfilledCurveXGraph::JKQTPfilledCurveXGraph(JKQtBasePlotter* parent):
    JKQTPxyGraph(parent)
{
    baseline=0.0;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    drawLine=true;
    fillStyle=Qt::SolidPattern;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        fillColor=color.lighter();
    }
}

JKQTPfilledCurveXGraph::JKQTPfilledCurveXGraph(JKQtPlotter *parent):
    JKQTPxyGraph(parent)
{
    baseline=0.0;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    drawLine=true;
    fillStyle=Qt::SolidPattern;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        fillColor=color.lighter();
    }
}

void JKQTPfilledCurveXGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPfilledCurveXGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QPainterPath pl, pf;

    double xold=-1;
    //double yold=-1;
//    double x0=xAxis->x2p(baseline);
//    if (parent->getXAxis()->isLogAxis()) {
//        if (baseline>0 && baseline>parent->getXAxis()->getMin()) x0=xAxis->x2p(baseline);
//        else x0=xAxis->x2p(parent->getXAxis()->getMin());
//    }
    double y0=yAxis->x2p(baseline);
    if (parent->getYAxis()->isLogAxis()) {
        y0=yAxis->x2p(parent->getYAxis()->getMin());
        if (baseline>0 && baseline>parent->getYAxis()->getMin()) y0=yAxis->x2p(baseline);
        else y0=yAxis->x2p(parent->getYAxis()->getMin());
    }
    bool first=true;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv); bool xok=JKQTPIsOKFloat(x);
            double y=yAxis->x2p(yv); bool yok=JKQTPIsOKFloat(y);

            if (!first) {
                if (xok&&yok) {
                    pf.lineTo(x, y);
                    if (drawLine) pl.lineTo(x, y);
                    xold=x;
                    //yold=y;
                } else if (xok&&!yok){
                    pf.lineTo(x, y0);
                    if (drawLine) pl.lineTo(x, y0);
                    xold=x;
                    //yold=y0;
                }
            } else {
                if (xok&&yok) {
                    if (drawLine) pl.moveTo(x,y);
                    pf.moveTo(x, y0);
                    pf.lineTo(x, y);
                    xold=x;
                    //yold=y;
                    first=false;
                } else if (xok&&!yok) {
                    if (drawLine) pl.moveTo(x,y0);
                    pf.moveTo(x, y0);
                    xold=x;
                    //yold=y0;
                    first=false;
                }
            }

        }
    }
    pf.lineTo(xold, y0);
    pf.closeSubpath();
    if (drawLine) {
        painter.save();

        if (drawSelectionLine) {
            QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
            QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
            if (selectionLineColor!=Qt::transparent) {
                penSelection.setColor(selectionLineColor);
            }
            penSelection.setWidthF(penSelection.widthF()*3.0);
            painter.setPen(penSelection);
            painter.drawPath(pl);
        }


        painter.setPen(p);
        painter.drawPath(pl);
        painter.restore();
    }
    painter.setBrush(b);
    painter.setPen(np);
    painter.drawPath(pf);

    painter.restore();

    drawErrorsAfter(painter);
}

void JKQTPfilledCurveXGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (drawLine) painter.setPen(p);
    painter.setBrush(b);
    painter.drawRect(rect);
    painter.restore();
}

QColor JKQTPfilledCurveXGraph::getKeyLabelColor() {
    return color;
}














JKQTPfilledCurveYGraph::JKQTPfilledCurveYGraph(JKQtBasePlotter* parent):
    JKQTPfilledCurveXGraph(parent)
{
}

JKQTPfilledCurveYGraph::JKQTPfilledCurveYGraph(JKQtPlotter *parent):
    JKQTPfilledCurveXGraph(parent)
{

}

void JKQTPfilledCurveYGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPfilledCurveYGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    p.setJoinStyle(Qt::RoundJoin);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QPainterPath pl, pf;

    //double xold=-1;
    double yold=-1;
    double x0=xAxis->x2p(baseline);
    if (parent->getXAxis()->isLogAxis()) {
        if (baseline>0 && baseline>parent->getXAxis()->getMin()) x0=xAxis->x2p(baseline);
        else x0=xAxis->x2p(parent->getXAxis()->getMin());
    }
    double y0=yAxis->x2p(baseline);
    if (parent->getYAxis()->isLogAxis()) {
        y0=yAxis->x2p(parent->getYAxis()->getMin());
        if (baseline>0 && baseline>parent->getYAxis()->getMin()) y0=yAxis->x2p(baseline);
        else y0=yAxis->x2p(parent->getYAxis()->getMin());
    }
    bool first=true;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv); bool xok=JKQTPIsOKFloat(x);
            double y=yAxis->x2p(yv); bool yok=JKQTPIsOKFloat(y);

            if (!first) {
                if (xok&&yok) {
                    pf.lineTo(x, y);
                    if (drawLine) pl.lineTo(x, y);
                    //xold=x;
                    yold=y;
                } else if (!xok&&yok){
                    pf.lineTo(x0, y);
                    if (drawLine) pl.lineTo(x0, y);
                    //xold=x0;
                    yold=y;
                }
            } else {
                if (xok&&yok) {
                    if (drawLine) pl.moveTo(x,y);
                    pf.moveTo(x, y0);
                    pf.lineTo(x, y);
                    //xold=x;
                    yold=y;
                    first=false;
                } else if (!xok&&yok) {
                    if (drawLine) pl.moveTo(x0,y);
                    pf.moveTo(x0, y);
                    //xold=x0;
                    yold=y;
                    first=false;
                }
            }
        }
    }
    pf.lineTo(x0, yold);
    pf.closeSubpath();
    if (drawLine) {
        painter.save();
        if (drawSelectionLine) {
            QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
            QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
            if (selectionLineColor!=Qt::transparent) {
                penSelection.setColor(selectionLineColor);
            }
            penSelection.setWidthF(penSelection.widthF()*3.0);
            painter.setPen(penSelection);
            painter.drawPath(pl);
        }

        painter.setPen(p);
        painter.drawPath(pl);
        painter.restore();
    }
    painter.setBrush(b);
    painter.setPen(np);
    painter.drawPath(pf);

    painter.restore();

    drawErrorsAfter(painter);
}













JKQTPboxplotVerticalGraph::JKQTPboxplotVerticalGraph(JKQtBasePlotter* parent):
    JKQTPgraph(parent)
{
    posColumn=-1;
    medianColumn=-1;
    meanColumn=-1;
    minColumn=-1;
    maxColumn=-1;
    percentile25Column=-1;
    percentile75Column=-1;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=0.4;
    meanSymbol=JKQTPplus;
    meanSymbolWidth=1;
    meanSymbolSize=12;
    sortData=Unsorted;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
    }

}


JKQTPboxplotVerticalGraph::JKQTPboxplotVerticalGraph(JKQtPlotter* parent):
    JKQTPgraph(parent)
{
    posColumn=-1;
    medianColumn=-1;
    meanColumn=-1;
    minColumn=-1;
    maxColumn=-1;
    percentile25Column=-1;
    percentile75Column=-1;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=0.4;
    meanSymbol=JKQTPplus;
    meanSymbolWidth=1;
    meanSymbolSize=12;
    sortData=Unsorted;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
    }

}


void JKQTPboxplotVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPboxplotVerticalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);


    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=datastore->getColumn(posColumn).getRows();
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;



    //bool first=false;

    // 1. step find width of boxplots:
    double boxwidth_real=0;
    bool bwfirst=true;
    intSortData();
    for (int iii=imin+1; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv0=xAxis->x2p(datastore->get(posColumn,i-1));
        double xv=xAxis->x2p(datastore->get(posColumn,i));
        if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xv0)) {
            if (bwfirst) {
                boxwidth_real=fabs(xv-xv0);
                bwfirst=false;
            } else {
                if (fabs(xv-xv0)>0) boxwidth_real=qMin(fabs(xv-xv0), boxwidth_real);
            }
        }
    }
    // 2. plot:
    painter.save();
    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,i);
        double p25v=datastore->get(percentile25Column,i);
        double p75v=datastore->get(percentile75Column,i);
        double minv=datastore->get(minColumn,i);
        double maxv=datastore->get(maxColumn,i);
        double medianv=datastore->get(medianColumn,i);
        double mean=yAxis->x2p(datastore->get(meanColumn,i));

        QVector<QLineF> lines_p, lines_pw;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(p25v) &&
            JKQTPIsOKFloat(p75v) && JKQTPIsOKFloat(minv) &&
            JKQTPIsOKFloat(maxv) && JKQTPIsOKFloat(medianv) ) {

            painter.save();
            painter.setPen(p);
            painter.setBrush(b);
            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            double x=xAxis->x2p(xv);
            double p25=yAxis->x2p(p25v);
            double p75=yAxis->x2p(p75v);
            double min=yAxis->x2p(minv);
            double max=yAxis->x2p(maxv);
            double median=yAxis->x2p(medianv);

            double xn=x+1;
            if (i+1<imax) xn=xAxis->x2p(datastore->get(posColumn,i+1));
            else if (i-1>=0) xn=xAxis->x2p(datastore->get(posColumn,i-1));
            else xn=x+1;


            double w=((boxwidth_real>0)?boxwidth_real:(fabs(xn-x)))*boxWidth;
            double minstop=p25;
            double maxstop=p75;
            if (percentile25Column<0 && medianColumn>=0) minstop=median;
            else if (percentile25Column<0 && meanColumn>=0) minstop=mean;
            else if (percentile25Column<0 && maxColumn>=0) minstop=max;
            if (percentile75Column<0 && medianColumn>=0) maxstop=median;
            else if (percentile75Column<0 && meanColumn>=0) maxstop=mean;
            else if (percentile75Column<0 && minColumn>=0) maxstop=min;
            double xma=x+w/2.0;
            double xmi=x-w/2.0;
            double xma4=x+w/4.0;
            double xmi4=x-w/4.0;

            if (imax<=0) {
                xma=xAxis->x2p(xv+boxWidth/2.0);
                xmi=xAxis->x2p(xv-boxWidth/2.0);
                xma4=xAxis->x2p(xv+boxWidth/4.0);
                xmi4=xAxis->x2p(xv-boxWidth/4.0);
            }

            if (minColumn>=0) {
                lines_p.append(QLineF(xmi4, min, xma4, min));
                lines_pw.append(QLineF(x, min, x, minstop));
            }
            if (maxColumn>=0) {
                lines_p.append(QLineF(xmi4, max, xma4, max));
                lines_pw.append(QLineF(x, max, x, maxstop));
            }

            if (percentile25Column>=0 && percentile75Column>=0) painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
            if (medianColumn>=0) lines_p.append(QLineF(xmi+p.widthF()/2.0, median, xma-p.widthF()/2.0, median));
            if (meanColumn>=0 && JKQTPIsOKFloat(mean)) {
                plotSymbol(painter, x, mean, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->get_lineWidthMultiplier()), color, fillColor);
            }


            painter.setPen(p);
            if (lines_p.size()>0) painter.drawLines(lines_p);
            painter.setPen(pw);
            if (lines_pw.size()>0) painter.drawLines(lines_pw);
            painter.restore();

            //first=true;
        }
    }
    painter.restore();


    drawErrorsAfter(painter);
}


bool JKQTPboxplotVerticalGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (posColumn<0) return false;
    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(posColumn).getRows();
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,i);
        if (JKQTPIsOKFloat(xv)) {
            double xn=xv+1;
            if (i+1<imax) xn=datastore->get(posColumn,i+1);
            else if (i-1>=0) xn=datastore->get(posColumn,i-1);
            else xn=xv+1;
            double delta=fabs(xn-xv);
            double w=delta*boxWidth;
            double xma=xv+w;
            double xmi=xv-w;
            if (start || xma>maxx) maxx=xma;
            if (start || xmi<minx) minx=xmi;
            double xvsgz;
            xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPboxplotVerticalGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (percentile25Column<0 && percentile75Column<0 && medianColumn<0 && minColumn<0 && maxColumn<0) return false;

    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(medianColumn).getRows();
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double p25=datastore->get(percentile25Column,i);
        double p75=datastore->get(percentile75Column,i);
        double min=datastore->get(minColumn,i);
        double max=datastore->get(maxColumn,i);
        double median=datastore->get(medianColumn,i);
        if (JKQTPIsOKFloat(median)) {
            if (start || median>maxy) maxy=median;
            if (start || median<miny) miny=median;
            if (JKQTPIsOKFloat(p25) &&p25>maxy) maxy=p25;
            if (JKQTPIsOKFloat(p25) &&p25<miny) miny=p25;
            if (JKQTPIsOKFloat(p75) &&p75>maxy) maxy=p75;
            if (JKQTPIsOKFloat(p75) &&p75<miny) miny=p75;
            if (JKQTPIsOKFloat(min) &&min>maxy) maxy=min;
            if (JKQTPIsOKFloat(min) &&min<miny) miny=min;
            if (JKQTPIsOKFloat(max) &&max>maxy) maxy=max;
            if (JKQTPIsOKFloat(max) &&max<miny) miny=max;

            double xvsgz;
            xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPboxplotVerticalGraph::usesColumn(int c)
{
    return (c==meanColumn)||(c==posColumn)||(c==medianColumn)||(c==minColumn)||(c==maxColumn)||(c==percentile25Column)||(c==percentile75Column);
}


void JKQTPboxplotVerticalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double x=rect.left()+rect.width()/2.0;
    double xma=x+rect.width()/2.5;
    double xmi=x-rect.width()/2.5;
    double min=rect.bottom();
    double max=rect.top();
    double median=max+rect.height()/2.0;
    double w=rect.width()/1.8;
    double p25=max+0.75*rect.height();
    double p75=max+0.25*rect.height();

    painter.setPen(p);
    painter.save();
    painter.setBrush(b);
    painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
    painter.restore();
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
    painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
    painter.setPen(pw);
    painter.drawLine(QLineF(x, max, x, p75));
    painter.drawLine(QLineF(x, min, x, p25));
    painter.restore();
}

QColor JKQTPboxplotVerticalGraph::getKeyLabelColor() {
    return color;
}




void JKQTPboxplotHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double y=rect.top()+rect.height()/2.0;
    double yma=y+rect.height()/2.5;
    double ymi=y-rect.height()/2.5;
    double min=rect.left();
    double max=rect.right();
    double median=max-rect.width()/2.0;
    double w=rect.height()/1.8;
    double p25=min+0.75*rect.width();
    double p75=min+0.25*rect.width();

    painter.setPen(p);
    painter.save();
    painter.setBrush(b);
    painter.drawRect(QRectF(p75, ymi, fabs(p75-p25), fabs(yma-ymi)));
    painter.restore();
    painter.drawLine(QLineF(median, ymi, median, yma));
    painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
    painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
    painter.setPen(pw);
    painter.drawLine(QLineF(max, y, p75, y));
    painter.drawLine(QLineF(min, y, p25, y));
    painter.restore();
}

bool JKQTPboxplotHorizontalGraph::getXMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (percentile25Column<0 && percentile75Column<0 && medianColumn<0 && minColumn<0 && maxColumn<0) return false;

    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(medianColumn).getRows();
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double p25=datastore->get(percentile25Column,i);
        double p75=datastore->get(percentile75Column,i);
        double min=datastore->get(minColumn,i);
        double max=datastore->get(maxColumn,i);
        double median=datastore->get(medianColumn,i);
        if (JKQTPIsOKFloat(median)) {
            if (start || median>maxy) maxy=median;
            if (start || median<miny) miny=median;
            if (JKQTPIsOKFloat(p25)&&p25>maxy) maxy=p25;
            if (JKQTPIsOKFloat(p25)&&p25<miny) miny=p25;
            if (JKQTPIsOKFloat(p75)&&p75>maxy) maxy=p75;
            if (JKQTPIsOKFloat(p75)&&p75<miny) miny=p75;
            if (JKQTPIsOKFloat(min)&&min>maxy) maxy=min;
            if (JKQTPIsOKFloat(min)&&min<miny) miny=min;
            if (JKQTPIsOKFloat(max)&&max>maxy) maxy=max;
            if (JKQTPIsOKFloat(max)&&max<miny) miny=max;
            double xvsgz;
            xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPboxplotHorizontalGraph::getYMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (posColumn<0) return false;
    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(posColumn).getRows();
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,i);
        double xn=xv+1;
        if (i+1<imax) xn=datastore->get(posColumn,i+1);
        else if (i-1>=0) xn=datastore->get(posColumn,i-1);
        else xn=xv+1;
        double delta=fabs(xn-xv);
        double w=delta*boxWidth;
        double xma=xv+w;
        double xmi=xv-w;
        if (JKQTPIsOKFloat(xma) && JKQTPIsOKFloat(xmi) ) {
            if (start || xma>maxx) maxx=xma;
            if (start || xmi<minx) minx=xmi;
            double xvsgz;
            xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

void JKQTPboxplotHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPboxplotHorizontalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);


    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=datastore->getColumn(posColumn).getRows();
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    // 1. step find width of boxplots:
    double boxwidth_real=0;
    bool bwfirst=true;
    intSortData();
    for (int iii=imin+1; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv0=yAxis->x2p(datastore->get(posColumn,i-1));
        double xv=yAxis->x2p(datastore->get(posColumn,i));
        if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xv0)) {
            if (bwfirst) {
                boxwidth_real=fabs(xv-xv0);
                bwfirst=false;
            } else {
                if (fabs(xv-xv0)>0) boxwidth_real=qMin(fabs(xv-xv0), boxwidth_real);
            }
        }
    }
    // 2. plot:
    painter.save();

    //bool first=false;
    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(posColumn,i);
        double p25v=datastore->get(percentile25Column,i);
        double p75v=datastore->get(percentile75Column,i);
        double minv=datastore->get(minColumn,i);
        double maxv=datastore->get(maxColumn,i);
        double medianv=datastore->get(medianColumn,i);
        double mean=xAxis->x2p(datastore->get(meanColumn,i));

        QVector<QLineF> lines_p, lines_pw;
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (posColumn>=0 && JKQTPIsOKFloat(yv) &&
            JKQTPIsOKFloat(p25v) &&
            JKQTPIsOKFloat(p75v) &&
            JKQTPIsOKFloat(minv) &&
            JKQTPIsOKFloat(maxv) &&
            JKQTPIsOKFloat(medianv)) {

            painter.save();
            painter.setPen(p);
            painter.setBrush(b);


            double p25=xAxis->x2p(p25v);
            double p75=xAxis->x2p(p75v);
            double min=xAxis->x2p(minv);
            double max=xAxis->x2p(maxv);
            double median=xAxis->x2p(medianv);


            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            double y=yAxis->x2p(yv);
            double minstop=p25;
            double maxstop=p75;
            if (percentile25Column<0 && medianColumn>=0) minstop=median;
            else if (percentile25Column<0 && maxColumn>=0) minstop=max;
            else if (percentile25Column<0 && meanColumn>=0) minstop=mean;
            if (percentile75Column<0 && medianColumn>=0) maxstop=median;
            else if (percentile75Column<0 && minColumn>=0) maxstop=min;
            else if (percentile75Column<0 && meanColumn>=0) maxstop=mean;

            double yn=y+1;
            if (i+1<imax) yn=yAxis->x2p(datastore->get(posColumn,i+1));
            else if (i-1>=0) yn=yAxis->x2p(datastore->get(posColumn,i-1));
            else yn=y+1;
            double delta=fabs(yn-y);
            double w=((boxwidth_real>0)?boxwidth_real:(delta))*boxWidth;
            double yma=y-w/2.0;
            double ymi=y+w/2.0;
            double yma4=y+w/4.0;
            double ymi4=y-w/4.0;

            if (imax<=1) {
                ymi=yAxis->x2p(yv+boxWidth/2.0);
                yma=yAxis->x2p(yv-boxWidth/2.0);
                yma4=yAxis->x2p(yv+boxWidth/4.0);
                ymi4=yAxis->x2p(yv-boxWidth/4.0);
            }
            if (minColumn>=0) {
                lines_p.append(QLineF(min, ymi4, min, yma4));
                lines_pw.append(QLineF(min, y, minstop, y));
            }
            if (maxColumn>=0) {
                lines_p.append(QLineF(max, ymi4, max, yma4));
                lines_pw.append(QLineF(max, y, maxstop, y));
            }
            if (percentile25Column>=0 && percentile75Column>=0) painter.drawRect(QRectF(p25, qMin(yma,ymi), fabs(p75-p25), fabs(yma-ymi)));
            if (medianColumn>=0) lines_p.append(QLineF(median, ymi-p.widthF()/2.0, median, yma+p.widthF()/2.0));

            if (meanColumn>=0 && JKQTPIsOKFloat(mean)) {
                plotSymbol(painter, mean, y, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->get_lineWidthMultiplier()), color, fillColor);
            }

            //first=true;
            painter.setPen(p);
            if (lines_p.size()>0) painter.drawLines(lines_p);
            painter.setPen(pw);
            if (lines_pw.size()>0) painter.drawLines(lines_pw);
            painter.restore();
        }

    }

    painter.restore();

    drawErrorsAfter(painter);
}





















JKQTPboxplotVerticalElement::JKQTPboxplotVerticalElement(JKQtBasePlotter* parent):
    JKQTPgraph(parent)
{
    pos=0;
    median=0;
    mean=0;
    min=-1;
    max=1;
    drawMean=true;
    drawMinMax=true;
    percentile25=-0.75;
    percentile75=0.75;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=0.4;
    meanSymbol=JKQTPplus;
    meanSymbolWidth=1;
    meanSymbolSize=12;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
    }

}

JKQTPboxplotVerticalElement::JKQTPboxplotVerticalElement(JKQtPlotter* parent):
    JKQTPgraph(parent)
{
    pos=0;
    median=0;
    mean=0;
    min=-1;
    max=1;
    drawMean=true;
    drawMinMax=true;
    percentile25=-0.75;
    percentile75=0.75;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=0.4;
    meanSymbol=JKQTPplus;
    meanSymbolWidth=1;
    meanSymbolSize=12;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
    }

}
void JKQTPboxplotVerticalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPboxplotVerticalElement::draw");
#endif
    if (parent==NULL) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);


    double xv=pos;
    double p25v=percentile25;
    double p75v=percentile75;
    double minv=min;
    double maxv=max;
    double medianv=median;

    //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
    if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(p25v) &&
        JKQTPIsOKFloat(p75v) && JKQTPIsOKFloat(minv) &&
        JKQTPIsOKFloat(maxv) && JKQTPIsOKFloat(medianv) ) {

        //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
        double x=xAxis->x2p(xv);
        double p25=yAxis->x2p(p25v);
        double p75=yAxis->x2p(p75v);
        double min=yAxis->x2p(minv);
        double max=yAxis->x2p(maxv);
        double median=yAxis->x2p(medianv);

        double w=boxWidth;
        double xma=x+w/2.0;
        double xmi=x-w/2.0;

        painter.save();
        painter.setPen(p);
        painter.save();
        painter.setBrush(b);
        painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
        painter.restore();
        if (drawMedian) {
            painter.drawLine(QLineF(xmi, median, xma, median));
        }
        if (drawMinMax) {
            painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
            painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
            painter.setPen(pw);
            painter.drawLine(QLineF(x, max, x, p75));
            painter.drawLine(QLineF(x, min, x, p25));
        }
        painter.restore();

        if (drawMean) {
            double mean=yAxis->x2p(this->mean);
            plotSymbol(painter, x, mean, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->get_lineWidthMultiplier()), color, fillColor);
        }


    }
    painter.restore();

    drawErrorsAfter(painter);
}


bool JKQTPboxplotVerticalElement::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false;

    double xv=pos;
    double w=boxWidth;
    double xma=xv+w;
    double xmi=xv-w;
    maxx=xma;
    minx=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPboxplotVerticalElement::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false ;


        double p25=percentile25;
        double p75=percentile75;
        if (drawMedian) {
            maxy=median;
            miny=median;
        } else {
            maxy=p75;
            miny=p25;
        }
        if (p25>maxy) maxy=p25;
        if (p25<miny) miny=p25;
        if (p75>maxy) maxy=p75;
        if (p75<miny) miny=p75;
        if (min>maxy) maxy=min;
        if (drawMinMax && (min<miny)) miny=min;
        if (drawMinMax && (max>maxy)) maxy=max;
        if (drawMinMax && (max<miny)) miny=max;

        double xvsgz;
        xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
        return true;
}


void JKQTPboxplotVerticalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double x=rect.left()+rect.width()/2.0;
    double xma=x+rect.width()/2.5;
    double xmi=x-rect.width()/2.5;
    double min=rect.bottom();
    double max=rect.top();
    double median=max+rect.height()/2.0;
    double w=rect.width()/1.8;
    double p25=max+0.75*rect.height();
    double p75=max+0.25*rect.height();

    painter.setPen(p);
    painter.save();
    painter.setBrush(b);
    painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
    painter.restore();
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
    painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
    painter.setPen(pw);
    painter.drawLine(QLineF(x, max, x, p75));
    painter.drawLine(QLineF(x, min, x, p25));
    painter.restore();
}

QColor JKQTPboxplotVerticalElement::getKeyLabelColor() {
    return color;
}




void JKQTPboxplotHorizontalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double y=rect.top()+rect.height()/2.0;
    double yma=y+rect.height()/2.5;
    double ymi=y-rect.height()/2.5;
    double min=rect.left();
    double max=rect.right();
    double median=max-rect.width()/2.0;
    double w=rect.height()/1.8;
    double p25=min+0.75*rect.width();
    double p75=min+0.25*rect.width();

    painter.setPen(p);
    painter.save();
    painter.setBrush(b);
    painter.drawRect(QRectF(p75, ymi, fabs(p75-p25), fabs(yma-ymi)));
    painter.restore();
    painter.drawLine(QLineF(median, ymi, median, yma));
    painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
    painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
    painter.setPen(pw);
    painter.drawLine(QLineF(max, y, p75, y));
    painter.drawLine(QLineF(min, y, p25, y));
    painter.restore();
}

bool JKQTPboxplotHorizontalElement::getXMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false;


        double p25=percentile25;
        double p75=percentile75;
        if (drawMedian) {
            maxy=median;
            miny=median;
        } else {
            maxy=p75;
            miny=p25;
        }
        if (p25>maxy) maxy=p25;
        if (p25<miny) miny=p25;
        if (p75>maxy) maxy=p75;
        if (p75<miny) miny=p75;
        if (min>maxy) maxy=min;
        if (drawMinMax && (min<miny)) miny=min;
        if (drawMinMax && (max>maxy)) maxy=max;
        if (drawMinMax && (max<miny)) miny=max;

        double xvsgz;
        xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
        return true;
}

bool JKQTPboxplotHorizontalElement::getYMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false;

    double xv=pos;
    double w=boxWidth;
    double xma=xv+w;
    double xmi=xv-w;
    maxx=xma;
    minx=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

void JKQTPboxplotHorizontalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPboxplotHorizontalElement::draw");
#endif
    if (parent==NULL) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    double yv=pos;
    double p25v=percentile25;
    double p75v=percentile75;
    double minv=min;
    double maxv=max;
    double medianv=median;

    //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
    if (JKQTPIsOKFloat(yv) &&
        JKQTPIsOKFloat(p25v) &&
        JKQTPIsOKFloat(p75v) &&
        JKQTPIsOKFloat(minv) &&
        JKQTPIsOKFloat(maxv) &&
        JKQTPIsOKFloat(medianv)) {

        //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
        double y=yAxis->x2p(yv);
        double p25=xAxis->x2p(p25v);
        double p75=xAxis->x2p(p75v);
        double min=xAxis->x2p(minv);
        double max=xAxis->x2p(maxv);
        double median=xAxis->x2p(medianv);

        double w=boxWidth;
        double yma=y+w/2.0;
        double ymi=y-w/2.0;

        painter.save();
        painter.setPen(p);
        painter.save();
        painter.setBrush(b);
        painter.drawRect(QRectF(p25, ymi, fabs(p75-p25), fabs(yma-ymi)));
        painter.restore();
        if (drawMedian) {
            painter.drawLine(QLineF(median, ymi, median, yma));
        }
        if (drawMinMax) {
            painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
            painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
            painter.setPen(pw);
            painter.drawLine(QLineF(max, y, p75, y));
            painter.drawLine(QLineF(min, y, p25, y));
        }
        painter.restore();

        if (drawMean) {
            double mean=yAxis->x2p(this->mean);
            plotSymbol(painter, mean, y, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->get_lineWidthMultiplier()), color, fillColor);
        }

    }
    painter.restore();

    drawErrorsAfter(painter);
}






double JKQTPxFunctionLineGraphPolynomial(double x, void* param) {
    double res=0;
    QVector<double>* d=static_cast<QVector<double>*>(param);
    if (d && d->size()>0) {
        res=d->value(0,0);
        double xx=x;
        for (int i=1; i<d->size(); i++) {
            res=res+d->value(i,0)*xx;
            xx=xx*x;
        }
    }

    return res;
}

double JKQTPxFunctionLineGraphExponential(double x, void* param) {
    double res=0;
    QVector<double>* d=static_cast<QVector<double>*>(param);
    if (d) {
        if (d->size()>=3) {
            res=d->value(0,0)+d->value(1,0)*exp(x/d->value(2,0));
        } else if (d->size()>=2) {
            res=d->value(0,0)*exp(x/d->value(1,0));
        }
    }
    return res;
}

double JKQTPxFunctionLineGraphPowerLaw(double x, void* param) {
    double res=0;
    QVector<double>* d=static_cast<QVector<double>*>(param);
    if (d) {
        if (d->size()>=3) {
            res=d->value(0,0)+d->value(1,0)*pow(x, d->value(2,1));
        } else if (d->size()>=2) {
            res=d->value(0,0)*pow(x, d->value(1,1));
        } else if (d->size()>=1) {
            res=pow(x, d->value(0,1));
        }

    }
    return res;
}


JKQTPxFunctionLineGraph::JKQTPxFunctionLineGraph(JKQtBasePlotter* parent):
    JKQTPgraph(parent)
{
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
    drawLine=true;
    fillCurve=false;
    plotFunction=NULL;
    params=NULL;
    minSamples=10;
    maxRefinementDegree=7;
    slopeTolerance=0.005;
    minPixelPerSample=32;
    plotRefinement=true;
    displaySamplePoints=false;
    data=NULL;

    drawErrorPolygons=false;
    drawErrorLines=false;
    errorPlotFunction=NULL;
    errorParams=NULL;
    errorColor=color.lighter();
    errorFillColor=color.lighter();
    errorStyle=Qt::SolidLine;
    errorLineWidth=1;
    errorFillStyle=Qt::SolidPattern;

    parameterColumn=-1;
    errorParameterColumn=-1;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
        errorColor=color.lighter();
        errorFillColor=color.lighter();
        errorStyle=style;
    }
    fillColor.setAlphaF(0.5);
    errorFillColor.setAlphaF(0.5);
}

JKQTPxFunctionLineGraph::JKQTPxFunctionLineGraph(JKQtPlotter* parent):
    JKQTPgraph(parent)
{
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
    drawLine=true;
    fillCurve=false;
    plotFunction=NULL;
    params=NULL;
    minSamples=10;
    maxRefinementDegree=7;
    slopeTolerance=0.005;
    minPixelPerSample=32;
    plotRefinement=true;
    displaySamplePoints=false;
    data=NULL;

    drawErrorPolygons=false;
    drawErrorLines=false;
    errorPlotFunction=NULL;
    errorParams=NULL;
    errorColor=color.lighter();
    errorFillColor=color.lighter();
    errorStyle=Qt::SolidLine;
    errorLineWidth=1;
    errorFillStyle=Qt::SolidPattern;

    parameterColumn=-1;
    errorParameterColumn=-1;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
        errorColor=color.lighter();
        errorFillColor=color.lighter();
        errorStyle=style;
    }
    fillColor.setAlphaF(0.5);
    errorFillColor.setAlphaF(0.5);
}


JKQTPxFunctionLineGraph::~JKQTPxFunctionLineGraph() {
    clearData();
}

void JKQTPxFunctionLineGraph::clearData() {
    while (data!=NULL) {
        doublePair* d=data;
        data=data->next;
        delete d;
    }
    data=NULL;
}


void JKQTPxFunctionLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    int y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (drawLine) painter.setPen(p);
    painter.setBrush(b);
    if (fillCurve) painter.drawRect(rect);
    if (!fillCurve & drawLine) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    painter.restore();
}

QColor JKQTPxFunctionLineGraph::getKeyLabelColor() {
    return color;
}

void JKQTPxFunctionLineGraph::createPlotData(bool collectParams) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPxFunctionLineGraph[%1]::createPlotData()").arg(title));
#endif
    clearData();
    if (collectParams) collectParameters();

    if (parent==NULL) return;
    if (plotFunction==NULL) return;

    double xmin=parent->getXMin();
    double xmax=parent->getXMax();
    double pxmin=xAxis->x2p(xmin);
    double pxmax=xAxis->x2p(xmax);
    double delta0=(pxmax-pxmin)/(double)minSamples;
    //double logdelta0=(log(xmax)-log(xmin))/(double)minSamples;

    // initially sample function
    doublePair* d=new doublePair;
    d->x=xmin;
    d->f=plotFunction(xmin, params);
    d->next=NULL;
    data=d;
    /*if (parent && parent->getXAxis()->isLogAxis()) {
        for (double x=log(xmin)+logdelta0; x<log(xmax); x=x+logdelta0) {
            d->next = new doublePair;
            d->next->x=exp(x+((double)rand()/(double)RAND_MAX-0.5)*delta0/2.0);
            d->next->f=plotFunction(d->next->x, params);
            d->next->next=NULL;
            doublePair* dd=d;
            d=d->next;
            refine(dd, d);
        }
    } else {*/
    QVector<double>* dv=static_cast<QVector<double>*>(params);
    if (plotFunction==JKQTPxFunctionLineGraphPolynomial && dv && dv->size()<=2) {
        // we only need the first and last datapoint
    } else {
        for (double x=pxmin+delta0; x<pxmax; x=x+delta0) {
            d->next = new doublePair;
            d->next->x=parent->p2x(x+((double)rand()/(double)RAND_MAX-0.5)*delta0/2.0);
            d->next->f=plotFunction(d->next->x, params);
            d->next->next=NULL;
            doublePair* dd=d;
            d=d->next;
            refine(dd, d);
        }
    }
    //}
    d->next = new doublePair;
    d->next->x=xmax;
    d->next->f=plotFunction(xmax, params);
    d->next->next=NULL;
    refine(d, d->next);

}

void JKQTPxFunctionLineGraph::collectParameters()
{
    if (parent && parameterColumn>=0) {
        iparams.clear();
        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=datastore->getColumn(parameterColumn).getRows();

        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(parameterColumn,i);
            iparams<<xv;
        }
        //qDebug()<<"iparams_beforeclean:";
        //for (int i=0; i<iparams.size(); i++) qDebug()<<iparams[i];
        int i=iparams.size()-1;
        while (i>=0 && !JKQTPIsOKFloat(iparams[i])) {
            iparams.remove(i,1);
            i--;
        }

        //qDebug()<<"iparams:";
        //for (i=0; i<iparams.size(); i++) qDebug()<<iparams[i];

        params=&iparams;
    }
    if (parent && errorParameterColumn>=0) {
        ierrorparams.clear();
        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=datastore->getColumn(errorParameterColumn).getRows();

        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(errorParameterColumn,i);
            ierrorparams<<xv;
        }
        int i=ierrorparams.size()-1;
        while (i>=0 && !JKQTPIsOKFloat(ierrorparams[i])) {
            ierrorparams.remove(i,1);
            i--;
        }

        errorParams=&ierrorparams;
    }
}

void JKQTPxFunctionLineGraph::refine(doublePair* a, doublePair* b, unsigned int degree) {
    if (degree>=maxRefinementDegree) return;
    double ax=xAxis->x2p(a->x);
    double af=xAxis->x2p(a->f);
    double bx=xAxis->x2p(b->x);
    double bf=xAxis->x2p(b->f);

    double delta=bx - ax;
    //double logdelta=log(bx) - log(ax);
    double xmid=ax+(delta)/2.0;
    /*if (parent && parent->getXAxis()->isLogAxis()) {
        xmid=log(a->x)+(logdelta)/2.0;
        xmid=xmid+((double)rand()/(double)RAND_MAX-0.5)*delta/5.0;
        xmid=exp(xmid);
    } else {*/
        xmid=xmid+((double)rand()/(double)RAND_MAX-0.5)*delta/5.0; // shake by 10%
    //}
    double realxmid=parent->p2x(xmid);
    double realfmid=plotFunction(realxmid, params);
    double fmid=yAxis->x2p(realfmid);
    double a1=(fmid - af)/(xmid - ax);
    double a2=(bf - fmid)/(bx - xmid);
    //std::cout<<std::string(degree*2, ' ')<<"refine( ["<<a->x<<", "<<a->f<<"], ["<<xmid<<", "<<fmid<<"],   ["<<b->x<<", "<<b->f<<"] ): a1="<<a1<<",  a2="<<a2<<"  acrit="<<abs(a2/a1)-1.0<<"\n";
    //std::cout<<std::string(degree*2, ' ')<<"refine(): a1="<<a1<<",  a2="<<a2<<"  acrit="<<fabs(a2-a1)<<"\n";
    if (fabs(a2-a1)>slopeTolerance || delta>minPixelPerSample) {
        doublePair* dmid = new doublePair;
        dmid->x=realxmid;
        dmid->f=realfmid;
        a->next=dmid;
        dmid->next=b;
        refine(a, dmid, degree+1);
        refine(dmid, b, degree+1);
    }
}


void JKQTPxFunctionLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPxFunctionLineGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    //qDebug()<<"start plot\n";
    createPlotData();
    //qDebug()<<"plot data created\n";

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QPen ep=painter.pen();
    ep.setColor(errorColor);
    ep.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, errorLineWidth*parent->get_lineWidthMultiplier())));
    ep.setStyle(errorStyle);
    ep.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    QBrush eb=painter.brush();
    eb.setColor(errorFillColor);
    eb.setStyle(errorFillStyle);


//    double xold=-1;
//    double yold=-1;
//    double ypeold=-1;
//    double ymeold=-1;

//    double x0=xAxis->x2p(0);
//    if (parent->getXAxis()->isLogAxis()) x0=xAxis->x2p(parent->getXAxis()->getMin());
    double y0=yAxis->x2p(0);
    if (parent->getYAxis()->isLogAxis()) y0=yAxis->x2p(parent->getYAxis()->getMin());
    bool first=false;
    doublePair* d=data;
    //QPainterPath pa, pfill;
    //QPainterPath pel, pef;
    QPolygonF filledPolygon, linePolygon, errorLineTop, errorLineBottom;
    QList<QPointF> epTop, epBottom;
    double yami=qMin(yAxis->x2p(parent->getYAxis()->getMin()),yAxis->x2p(parent->getYAxis()->getMax()));
    double yama=qMax(yAxis->x2p(parent->getYAxis()->getMin()),yAxis->x2p(parent->getYAxis()->getMax()));
    double dypix=fabs(yama-yami);
    yami=yami-2*dypix;
    yama=yama+2*dypix;
    while (d!=NULL) {

        double xv=d->x;
        double yv=d->f;
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv);
            double y=yAxis->x2p(yv);
            double ype=0, yme=0;
            if ((drawErrorLines || drawErrorPolygons) && (errorPlotFunction!=NULL)) {
                double e=errorPlotFunction(xv, errorParams);
                ype=yAxis->x2p(yv+e);
                yme=yAxis->x2p(yv-e);
                ype=qBound(yami, ype, yama);
                yme=qBound(yami, yme, yama);
            }

            y=qBound(yami, y, yama);

            if (fillCurve) {
                if (!first) filledPolygon<<QPointF(x, y0);
                filledPolygon<<QPointF(x, y);
                if (!d->next) filledPolygon<<QPointF(x, y0);
            }

            if (drawErrorPolygons && (errorPlotFunction!=NULL)) {
                epTop<<QPointF(x, ype);
                epBottom<<QPointF(x, yme);
            }

            if (drawLine) {
                linePolygon<<QPointF(x, y);
            }

            if (drawErrorLines && (errorPlotFunction!=NULL)) {
                errorLineTop<<QPointF(x, ype);
                errorLineBottom<<QPointF(x, yme);
            }

//            xold=x;
//            yold=y;
//            ypeold=ype;
//            ymeold=yme;
            first=true;
        }
        d=d->next;
    }
    if (drawErrorPolygons) {
        painter.save();
        painter.setBrush(eb);
        painter.setPen(np);
        QPolygonF poly;
        //poly << QPointF(xold, ypeold) << QPointF(x, ype)<< QPointF(x, yme) << QPointF(xold, ymeold) ;
        for (int i=0; i<epTop.size(); i++) {
            poly<<epTop[i];
        }
        for (int i=epBottom.size()-1; i>=0; i--) {
            poly<<epBottom[i];
        }
        painter.drawPolygon(poly, Qt::OddEvenFill);
        painter.restore();
    }
    if (fillCurve) {
        painter.save();
        painter.setBrush(b);
        painter.setPen(np);
        painter.drawPolygon(filledPolygon, Qt::OddEvenFill);
        painter.restore();
    }
    if (drawLine) {
        painter.save();
        painter.setPen(p);
        painter.drawPolyline(linePolygon);
        painter.restore();
    }

    if (drawErrorLines && (errorPlotFunction!=NULL)) {
        painter.save();
        painter.setPen(ep);
        painter.drawPolyline(errorLineTop);
        painter.drawPolyline(errorLineBottom);
        painter.restore();

    }


    QColor c=color;
    c.setHsv(fmod(color.hue()+90, 360), color.saturation(), color.value());
    d=data;
    if (displaySamplePoints) {
        painter.save();
        while (d!=NULL) {
            double xv=d->x;
            double yv=d->f;
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=xAxis->x2p(xv);
                double y=yAxis->x2p(yv);
                plotSymbol(painter, x, y, JKQTPcross, 6,1*parent->get_lineWidthMultiplier(), c, QColor(Qt::transparent));
            }
            d=d->next;
        }
        painter.restore();
    }
    painter.restore();
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}















void JKQTPyFunctionLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPyFunctionLineGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    //std::cout<<"start plot\n";
    createPlotData();
    //std::cout<<"plot data created\n";

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QPen ep=painter.pen();
    ep.setColor(errorColor);
    ep.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, errorLineWidth*parent->get_lineWidthMultiplier())));
    ep.setStyle(errorStyle);
    ep.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    QBrush eb=painter.brush();
    eb.setColor(errorFillColor);
    eb.setStyle(errorFillStyle);


    double xold=-1;
    double yold=-1;
    double xpeold=-1;
    double xmeold=-1;

    double x0=xAxis->x2p(0);
    if (parent->getXAxis()->isLogAxis()) x0=xAxis->x2p(parent->getXAxis()->getMin());
//    double y0=yAxis->x2p(0);
//    if (parent->getYAxis()->isLogAxis()) y0=yAxis->x2p(parent->getYAxis()->getMin());
    bool first=false;
    doublePair* d=data;

    while (d!=NULL) {
        double yv=d->x;
        double xv=d->f;
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv);
            double y=yAxis->x2p(yv);
            double xpe=0, xme=0;
            if ((drawErrorLines || drawErrorPolygons) && (errorPlotFunction!=NULL)) {
                double e=errorPlotFunction(xv, errorParams);
                xpe=xAxis->x2p(xv+e);
                xme=xAxis->x2p(xv-e);
            }

            if (first) {
                double xl1=xold;
                double yl1=yold;
                double xl2=x;
                double yl2=y;

                if (fillCurve) {
                    painter.save();
                    painter.setBrush(b);
                    painter.setPen(np);
                    QPolygonF poly;
                    poly << QPointF(xl1, yl1) << QPointF(xl2, yl2) << QPointF(x0, yl2) << QPointF(x0, yl1);
                    painter.drawConvexPolygon(poly);
                    painter.restore();
                    /*pfill.lineTo(x, y);
                    if (d->next==NULL) { // last datapoint
                        pfill.lineTo(x, y0);
                    }*/
                }

                if (drawErrorPolygons && (errorPlotFunction!=NULL)) {
                    painter.save();
                    painter.setBrush(eb);
                    painter.setPen(np);
                    QPolygonF poly;
                    poly << QPointF(xpeold, yold) << QPointF(xpe, y)<< QPointF(xme, y) << QPointF(xmeold, yold) ;
                    painter.drawConvexPolygon(poly);
                    painter.restore();
                }

                if (drawLine) {
                    painter.setPen(p);
                    //pa.lineTo(x, y);
                    painter.drawLine(QLineF(xl1, yl1, xl2, yl2));
                }

                if (drawErrorLines && (errorPlotFunction!=NULL)) {
                    painter.setPen(ep);
                    painter.drawLine(QLineF(xpeold, yold, xpe, y));
                    painter.drawLine(QLineF(xmeold, yold, xme, y));
                }

                //std::cout<<"line ("<<xl1<<", "<<yl1<<") -- ("<<xl2<<", "<<yl2<<")"<<std::endl;
            } /*else {
                if (drawLine) {
                    pa.moveTo(x, y);
                }
                if (fillCurve) {
                    pfill.moveTo(x, y0);
                    pfill.lineTo(x, y);
                }
            }*/
            xold=x;
            yold=y;
            xpeold=xpe;
            xmeold=xme;
            first=true;
        }
        d=d->next;
    }
    /*if (fillCurve) {
        pfill.closeSubpath();
        painter.save();
        painter.setBrush(b);
        painter.setPen(np);
        painter.drawPath(pfill);
        painter.restore();
    }

    if (drawLine) {
        painter.setPen(p);
        painter.drawPath(pa);
        painter.restore();
    }*/

    QColor c=color;
    c.setHsv(fmod(color.hue()+90, 360), color.saturation(), color.value());
    d=data;
    if (displaySamplePoints) while (d!=NULL) {
        double yv=d->x;
        double xv=d->f;
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv);
            double y=yAxis->x2p(yv);
            plotSymbol(painter, x, y, JKQTPcross, 6, 1*parent->get_lineWidthMultiplier(), c, QColor(Qt::transparent));
        }
        d=d->next;
    }
    painter.restore();
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}


void JKQTPyFunctionLineGraph::createPlotData(bool /*collectParams*/) {
    clearData();

    if (parent==NULL) return;
    if (plotFunction==NULL) return;

    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    double delta0=(ymax-ymin)/(double)minSamples;

    // initially sample function
    doublePair* d=new doublePair;
    d->x=ymin;
    d->f=plotFunction(ymin, params);
    d->next=NULL;
    data=d;
    for (double y=ymin+delta0; y<ymax; y=y+delta0) {
        d->next = new doublePair;
        d->next->x=y+((double)rand()/(double)RAND_MAX-0.5)*delta0/2.0;
        d->next->f=plotFunction(d->next->x, params);
        d->next->next=NULL;
        doublePair* dd=d;
        d=d->next;
        refine(dd, d);
    }
    d->next = new doublePair;
    d->next->x=ymax;
    d->next->f=plotFunction(ymax, params);
    d->next->next=NULL;
    refine(d, d->next);

}





JKQTPstepHorizontalGraph::JKQTPstepHorizontalGraph(JKQtBasePlotter* parent):
    JKQTPxyGraph(parent)
{
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
    drawLine=true;
    fillCurve=true;
    valuesCentered=false;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
}


JKQTPstepHorizontalGraph::JKQTPstepHorizontalGraph(JKQtPlotter* parent):
    JKQTPxyGraph(parent)
{
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
    drawLine=true;
    fillCurve=true;
    valuesCentered=false;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
}


void JKQTPstepHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    int y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (drawLine) painter.setPen(p);
    painter.setBrush(b);
    if (fillCurve) painter.drawRect(rect);
    if (!fillCurve & drawLine) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    painter.restore();
}

QColor JKQTPstepHorizontalGraph::getKeyLabelColor() {
    return color;
}


void JKQTPstepHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPstepHorizontalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QPainterPath pl, pf;

    double xold=-1;
    double yold=-1;
//    double xstart=-1;
//    double ystart=-1;
    double x0=xAxis->x2p(0);
    if (parent->getXAxis()->isLogAxis()) x0=xAxis->x2p(parent->getXAxis()->getMin());
//    double y0=yAxis->x2p(0);
//    if (parent->getYAxis()->isLogAxis()) y0=yAxis->x2p(parent->getYAxis()->getMin());
    bool subsequentItem=false;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv);
            double y=yAxis->x2p(yv);
            if (subsequentItem) {
                //double xl1=xold;
                //double yl1=yold;
                //double xl2=x;
                //double yl2=y;

                if (valuesCentered) {
                    double d=(x-xold);
                    pf.lineTo(xold+d/2.0, yold);
                    pf.lineTo(xold+d/2.0, y);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(xold+d/2.0, yold);
                        pl.lineTo(xold+d/2.0, y);
                        pl.lineTo(x, y);
                    }
                } else {
                    pf.lineTo(x, yold);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(x, yold);
                        pl.lineTo(x, y);
                    }
                }

                //std::cout<<"line ("<<xl1<<", "<<yl1<<") -- ("<<xl2<<", "<<yl2<<")"<<std::endl;
            } else {
                if (drawLine) pl.moveTo(x,y);
                pf.moveTo(x0, y);
                pf.lineTo(x, y);
                //xstart=x;
                //ystart=y0;
            }
            xold=x;
            yold=y;
            subsequentItem=true;
        }
    }
    if (fillCurve) {
        pf.lineTo(x0, yold);
        pf.closeSubpath();
    }
    painter.save();

    if (fillCurve) {
        painter.fillPath(pf, b);
    }

    if (drawLine) {
        painter.setBrush(QBrush(Qt::transparent));
        painter.setPen(p);
        painter.drawPath(pl);
    }
    painter.restore();

    drawErrorsAfter(painter);
}











void JKQTPstepVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPstepVerticalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QPainterPath pl, pf;

    double xold=-1;
    double yold=-1;
//    double x0=xAxis->x2p(0);
//    if (parent->getXAxis()->isLogAxis()) x0=xAxis->x2p(parent->getXAxis()->getMin());
    double y0=yAxis->x2p(0);
    if (parent->getYAxis()->isLogAxis()) y0=yAxis->x2p(parent->getYAxis()->getMin());
    bool first=false;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv);
            double y=yAxis->x2p(yv);
            if (first) {
                //double xl1=xold;
                //double yl1=yold;
                //double xl2=x;
                //double yl2=y;

                if (valuesCentered) {
                    double d=(y-yold);
                    pf.lineTo(xold, yold+d/2.0);
                    pf.lineTo(x, yold+d/2.0);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(xold, yold+d/2.0);
                        pl.lineTo(x, yold+d/2.0);
                        pl.lineTo(x, y);
                    }
                } else {
                    pf.lineTo(xold, y);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(xold, y);
                        pl.lineTo(x, y);
                    }
                }

                //std::cout<<"line ("<<xl1<<", "<<yl1<<") -- ("<<xl2<<", "<<yl2<<")"<<std::endl;
            } else {
                if (drawLine) pl.moveTo(x,y);
                pf.moveTo(x, y0);
                pf.lineTo(x, y);
            }
            xold=x;
            yold=y;
            first=true;
        }
    }
    pf.lineTo(xold, y0);
    pf.closeSubpath();
    painter.save();
    if (drawLine) {
        painter.setPen(p);
        painter.drawPath(pl);
    }
    painter.setBrush(b);
    painter.setPen(np);
    painter.drawPath(pf);

    painter.restore();

    drawErrorsAfter(painter);
}











JKQTPbarHorizontalGraph::JKQTPbarHorizontalGraph(JKQtBasePlotter* parent):
    JKQTPxyGraph(parent), JKQTPxyGraphErrors()
{
    baseline=0.0;
    color=QColor("black");
    fillColor=QColor("red");
    style=Qt::SolidLine;
    lineWidth=1;
    fillStyle=Qt::SolidPattern;
    width=0.9;
    shift=0;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        fillColor=parent->getPlotStyle(parentPlotStyle).color();
    }
    setErrorColorFromGraphColor(color);
}


JKQTPbarHorizontalGraph::JKQTPbarHorizontalGraph(JKQtPlotter* parent):
    JKQTPxyGraph(parent), JKQTPxyGraphErrors()
{
    baseline=0.0;
    color=QColor("black");
    fillColor=QColor("red");
    style=Qt::SolidLine;
    lineWidth=1;
    fillStyle=Qt::SolidPattern;
    width=0.9;
    shift=0;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        fillColor=parent->getPlotStyle(parentPlotStyle).color();
    }
    setErrorColorFromGraphColor(color);
}

void JKQTPbarHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    painter.setPen(p);
    painter.setBrush(b);
    painter.drawRect(rect);
    painter.restore();
}

QColor JKQTPbarHorizontalGraph::getKeyLabelColor() {
    return fillColor;
}


void JKQTPbarHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPbarHorizontalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end+1, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;
    painter.save();

//    double x0=xAxis->x2p(0);
//    if (parent->getXAxis()->isLogAxis()) x0=xAxis->x2p(parent->getXAxis()->getMin());
    double y0=yAxis->x2p(0);
    if (parent->getYAxis()->isLogAxis()) y0=yAxis->x2p(parent->getYAxis()->getMin());
    double delta=1;
    double deltap=0;
    double deltam=0;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        long long sr=datastore->getNextLowerIndex(xColumn, i, datarange_start, datarange_end);
        long long lr=datastore->getNextHigherIndex(xColumn, i, datarange_start, datarange_end);
        double yv=datastore->get(yColumn,i);
//        if (imin==imax) { // only one x-value
//            deltam=0.5;
//            deltap=0.5;
//        } else if (i==imax-1) { // the right-most x-value
//            deltap=deltam=fabs(xv-datastore->get(xColumn,i-1))/2.0;
//        } else if (i==imin) { // the left-most x-value
//            deltam=deltap=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
//        } else {
//            deltam=fabs(xv-datastore->get(xColumn,i-1))/2.0;
//            deltap=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
//        }
        if (sr<0 && lr<0) { // only one x-value
            deltam=0.5;
            deltap=0.5;
        } else if (lr<0) { // the right-most x-value
            deltap=deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
        } else if (sr<0) { // the left-most x-value
            deltam=deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
        } else {
            deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
            deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
        }
        //std::cout<<iii<<", \t"<<i<<", \t"<<sr<<", \t"<<lr<<", \t"<<deltam<<", \t"<<deltap<<"\n\n";
        delta=deltap+deltam;

        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=xAxis->x2p(xv+shift*delta-width*deltam);
            double y=yAxis->x2p(yv);
            double xx=xAxis->x2p(xv+shift*delta+width*deltap);
            double yy=y0;
            if (!qFuzzyIsNull(baseline)) yy=yAxis->x2p(baseline);
            //std::cout<<"delta="<<delta<<"   x="<<x<<" y="<<y<<"   xx="<<xx<<" yy="<<yy<<std::endl;
            if (yy<y) { qSwap(y,yy); }
            if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(xx) && JKQTPIsOKFloat(y) && JKQTPIsOKFloat(yy)) {
                painter.setBrush(b);
                painter.setPen(p);
                QRectF r(QPointF(x, y), QPointF(xx, yy));
                painter.drawRect(r);

            }
        }
    }

    painter.restore();
    drawErrorsAfter(painter);
}

bool JKQTPbarHorizontalGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(xColumn,i);
        long long sr=datastore->getNextLowerIndex(xColumn, i, datarange_start, datarange_end);
        long long lr=datastore->getNextHigherIndex(xColumn, i, datarange_start, datarange_end);
        double delta, deltap, deltam;
//        if (imin==imax) { // only one x-value
//            deltam=0.5;
//            deltap=0.5;
//        } else if (i==imax-1) { // the right-most x-value
//            deltap=deltam=fabs(xv-datastore->get(xColumn,i-1))/2.0;
//        } else if (i==imin) { // the left-most x-value
//            deltam=deltap=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
//        } else {
//            deltam=fabs(xv-datastore->get(xColumn,i-1))/2.0;
//            deltap=fabs(datastore->get(xColumn,i+1)-xv)/2.0;
//        }

        if (sr<0 && lr<0) { // only one x-value
            deltam=0.5;
            deltap=0.5;
        } else if (lr<0) { // the right-most x-value
            deltap=deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
        } else if (sr<0) { // the left-most x-value
            deltam=deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
        } else {
            deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
            deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
        }
        delta=deltap+deltam;

        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(delta) ) {

            if (start || xv+shift*delta+width*delta/2.0>maxx) maxx=xv+shift*delta+width*delta/2.0;
            if (start || xv+shift*delta-width*delta/2.0<minx) minx=xv+shift*delta-width*delta/2.0;
            double xvsgz;
            xvsgz=xv+shift*delta+width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xv+shift*delta-width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPbarHorizontalGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {


    if (yErrorColumn<0 || yErrorStyle==JKQTPnoError) {
        return JKQTPxyGraph::getYMinMax(miny, maxy, smallestGreaterZero);
    } else {
        bool start=false;
        miny=baseline;
        maxy=baseline;
        smallestGreaterZero=0;
        if (baseline>0) smallestGreaterZero=baseline;

        if (parent==NULL) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, (long long)imax);
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, (long long)imax);
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;


        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(yColumn,i)+getYErrorU(i, datastore);
            double yvv=datastore->get(yColumn,i)-getYErrorL(i, datastore);
            if (JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(yvv) ) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                if (start || yvv>maxy) maxy=yvv;
                if (start || yvv<miny) miny=yvv;
                xvsgz=yvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

void JKQTPbarHorizontalGraph::autoscaleBarWidthAndShift(double maxWidth, double shrinkFactor)
{
    if (parent) {
        double cntH=0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPgraph* g=parent->getGraph(i);
            JKQTPbarHorizontalGraph* gb=qobject_cast<JKQTPbarHorizontalGraph*>(g);
            if (gb && gb->isHorizontal()==isHorizontal()) {
                cntH++;
            }

        }

        double widthH=1.0/cntH*maxWidth*shrinkFactor;
        double dH=maxWidth/(cntH);
        double h=0.1+dH/2.0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPgraph* g=parent->getGraph(i);
            JKQTPbarHorizontalGraph* gb=qobject_cast<JKQTPbarHorizontalGraph*>(g);
            if (gb && gb->isHorizontal()==isHorizontal()) {
                if (cntH>1) {
                    gb->width=widthH;
                    gb->shift=h-0.5;
                    h=h+dH;
                } else {
                    gb->width=maxWidth;
                    gb->shift=0.0;
                }
            }

        }
    }
}










void JKQTPbarVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPbarVerticalGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end+1, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    double x0=xAxis->x2p(0);
    if (parent->getXAxis()->isLogAxis()) x0=xAxis->x2p(parent->getXAxis()->getMin());
//    double y0=yAxis->x2p(0);
//    if (parent->getYAxis()->isLogAxis()) y0=yAxis->x2p(parent->getYAxis()->getMin());
    double delta=1;
    double deltap=0;
    double deltam=0;
    painter.save();
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        long long sr=datastore->getNextLowerIndex(yColumn, i, datarange_start, datarange_end);
        long long lr=datastore->getNextHigherIndex(yColumn, i, datarange_start, datarange_end);
//        if (imin==imax) { // only one x-value
//            deltam=0.5;
//            deltap=0.5;
//        } else if (i==imax-1) { // the right-most x-value
//            deltap=deltam=fabs(yv-datastore->get(yColumn,i-1))/2.0;
//        } else if (i==imin) { // the left-most x-value
//            deltam=deltap=fabs(datastore->get(yColumn,i+1)-yv)/2.0;
//        } else {
//            deltam=fabs(yv-datastore->get(yColumn,i-1))/2.0;
//            deltap=fabs(datastore->get(yColumn,i+1)-yv)/2.0;
//        }

        if (sr<0 && lr<0) { // only one y-value
            deltam=0.5;
            deltap=0.5;
        } else if (lr<0) { // the right-most y-value
            deltap=deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
        } else if (sr<0) { // the left-most y-value
            deltam=deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
        } else {
            deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
            deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
        }
        delta=deltap+deltam;

        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=x0;
            if (!qFuzzyIsNull(baseline)) x=xAxis->x2p(baseline);
            double y=yAxis->x2p(yv+shift*delta+width*deltap);
            double xx=xAxis->x2p(xv);
            double yy=yAxis->x2p(yv+shift*delta-width*deltam);
            if (x>xx) { qSwap(x,xx); }
            //qDebug()<<"delta="<<delta<<"   x="<<x<<" y="<<y<<"   xx="<<xx<<" yy="<<yy;
            //qDebug()<<"xv="<<xv<<"   x0="<<x0<<"   x="<<x<<"..."<<xx;
            if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(xx) && JKQTPIsOKFloat(y) && JKQTPIsOKFloat(yy)) {
                painter.setBrush(b);
                painter.setPen(p);
                QRectF r(QPointF(x, y), QPointF(xx, yy));
                painter.drawRect(r);
            }
        }
    }

    painter.restore();
    drawErrorsAfter(painter);
}

bool JKQTPbarVerticalGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {

    if (xErrorColumn<0 || xErrorStyle==JKQTPnoError) {
        return JKQTPxyGraph::getXMinMax(minx, maxx, smallestGreaterZero);
    } else {
        bool start=false;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        if (parent==NULL) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, (long long)imax);
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, (long long)imax);
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double xvsgz;
            double xv=datastore->get(xColumn,i)+getXErrorU(i, datastore);
            double xvv=datastore->get(xColumn,i)-getXErrorL(i, datastore);
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xvv) ) {
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                if (start || xvv>maxx) maxx=xvv;
                if (start || xvv<minx) minx=xvv;
                xvsgz=xvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

bool JKQTPbarVerticalGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==NULL) return false;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(yColumn,i);
        double delta, deltap, deltam;
        long long sr=datastore->getNextLowerIndex(yColumn, i, datarange_start, datarange_end);
        long long lr=datastore->getNextHigherIndex(yColumn, i, datarange_start, datarange_end);
//        if (imin==imax) { // only one x-value
//            deltam=0.5;
//            deltap=0.5;
//        } else if (i==imax-1) { // the right-most x-value
//            deltap=deltam=fabs(yv-datastore->get(yColumn,i-1))/2.0;
//        } else if (i==imin) { // the left-most x-value
//            deltam=deltap=fabs(datastore->get(yColumn,i+1)-yv)/2.0;
//        } else {
//            deltam=fabs(yv-datastore->get(yColumn,i-1))/2.0;
//            deltap=fabs(datastore->get(yColumn,i+1)-yv)/2.0;
//        }

        if (sr<0 && lr<0) { // only one y-value
            deltam=0.5;
            deltap=0.5;
        } else if (lr<0) { // the right-most y-value
            deltap=deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
        } else if (sr<0) { // the left-most y-value
            deltam=deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
        } else {
            deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
            deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
        }
        delta=deltap+deltam;
        if (JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(delta) ) {

            if (start || yv+shift*delta+width*delta/2.0>maxy) maxy=yv+shift*delta+width*delta/2.0;
            if (start || yv+shift*delta-width*delta/2.0<miny) miny=yv+shift*delta-width*delta/2.0;
            double xvsgz;
            xvsgz=yv+shift*delta+width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=yv+shift*delta-width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPbarVerticalGraph::isHorizontal() const
{
    return false;
}










JKQTPhorizontalRange::JKQTPhorizontalRange(JKQtBasePlotter* parent):
    JKQTPgraph(parent)
{
    color=QColor("red").darker();
    fillColor=QColor("red").lighter();
    style=Qt::DotLine;
    lineWidth=1;
    fillStyle=Qt::SolidPattern;
    centerColor=QColor("red");
    centerStyle=Qt::SolidLine;
    centerLineWidth=2;
    sizeMin=0;
    sizeMax=1;
    unlimitedSizeMin=true;
    unlimitedSizeMax=true;
    invertedRange=false;



    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        centerColor=parent->getPlotStyle(parentPlotStyle).color();
        color=centerColor.darker();
        fillColor=centerColor.lighter();
    }
    fillColor.setAlphaF(0.5);

    rangeMin=rangeMax=0;
    rangeCenter=0;
    plotCenterLine=true;
    plotRange=true;
    plotRangeLines=true;
    fillRange=true;
}


JKQTPhorizontalRange::JKQTPhorizontalRange(JKQtPlotter* parent):
    JKQTPgraph(parent)
{
    color=QColor("red").darker();
    fillColor=QColor("red").lighter();
    style=Qt::DotLine;
    lineWidth=1;
    fillStyle=Qt::SolidPattern;
    centerColor=QColor("red");
    centerStyle=Qt::SolidLine;
    centerLineWidth=2;
    sizeMin=0;
    sizeMax=1;
    unlimitedSizeMin=true;
    unlimitedSizeMax=true;
    invertedRange=false;



    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        centerColor=parent->getPlotStyle(parentPlotStyle).color();
        color=centerColor.darker();
        fillColor=centerColor.lighter();
    }
    fillColor.setAlphaF(0.5);

    rangeMin=rangeMax=0;
    rangeCenter=0;
    plotCenterLine=true;
    plotRange=true;
    plotRangeLines=true;
    fillRange=true;
}
void JKQTPhorizontalRange::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPhorizontalRange::draw");
#endif
    double c=yAxis->x2p(rangeCenter);
    double cmi=yAxis->x2p(rangeMin);
    double cma=yAxis->x2p(rangeMax);
    double smi=yAxis->x2p(parent->getYAxis()->getMin());
    double sma=yAxis->x2p(parent->getYAxis()->getMax());
    double mi=xAxis->x2p(parent->getXAxis()->getMin());
    double ma=xAxis->x2p(parent->getXAxis()->getMax());
    if (!unlimitedSizeMin) mi=xAxis->x2p(sizeMin);
    if (!unlimitedSizeMax) ma=xAxis->x2p(sizeMax);
    //std::cout<<"hor: rangeMin="<<rangeMin<<" -> "<<cmi<<std::endl;
    //std::cout<<"hor: rangeMax="<<rangeMax<<" -> "<<cma<<std::endl;
    painter.save();
    if (plotRange && (cmi!=cma) && JKQTPIsOKFloat(mi) && JKQTPIsOKFloat(ma)) {
        QPen p=painter.pen();
        QPen np(Qt::NoPen);
        p.setColor(color);
        p.setStyle(style);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
        QBrush nb(Qt::NoBrush);
        QBrush b=painter.brush();
        b.setColor(fillColor);
        b.setStyle(fillStyle);
        QRectF rect(QPointF(mi, cmi), QPointF(ma, cma));
        if (fillRange) {
            painter.setPen(np);
            painter.setBrush(b);
            if (invertedRange) {
                if (JKQTPIsOKFloat(cmi)) {
                    QRectF rectl(QPointF(mi, smi), QPointF(ma, cmi));
                    painter.drawRect(rectl);
                }
                if (JKQTPIsOKFloat(cma)) {
                    QRectF rectr(QPointF(mi, cma), QPointF(ma, sma));
                    painter.drawRect(rectr);
                }
            } else {
                if (JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) painter.drawRect(rect);
            }
        }

        //std::cout<<"hor: rangeMin.rect.top="<<rect.top()<<std::endl;
        //std::cout<<"hor: rangeMax.rect.bottom="<<rect.bottom()<<std::endl;

        if (plotRangeLines && JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) {
            painter.setPen(p);
            painter.setBrush(nb);
            painter.drawLine(rect.left(), rect.top(), rect.right(), rect.top());
            painter.drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
        }

    }
    if (plotCenterLine && JKQTPIsOKFloat(c) && JKQTPIsOKFloat(mi) && JKQTPIsOKFloat(ma)) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, centerLineWidth*parent->get_lineWidthMultiplier())));
        painter.setPen(p);
        painter.drawLine(QLineF(mi, c, ma, c));
    }
    painter.restore();
}

void JKQTPhorizontalRange::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush nb(Qt::NoBrush);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    if (plotRange) {
        painter.setPen(np);
        painter.setBrush(b);
        if (fillRange) painter.drawRect(rect);
        painter.setPen(p);
        painter.setBrush(nb);
        if (plotRangeLines) {
            painter.drawLine(rect.left(), rect.top(), rect.right(), rect.top());
            painter.drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
        }
    }

    if (plotCenterLine) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        painter.setPen(p);
        int y=rect.top()+rect.height()/2.0;
        painter.drawLine(rect.left(), y, rect.right(), y);
    }
    painter.restore();
}

void JKQTPhorizontalRange::setDrawCenterLineOnly()
{
    plotCenterLine=true;
    plotRange=false;
}










JKQTPverticalRange::JKQTPverticalRange(JKQtBasePlotter* parent):
    JKQTPhorizontalRange(parent)
{
}

JKQTPverticalRange::JKQTPverticalRange(JKQtPlotter* parent):
    JKQTPhorizontalRange(parent)
{
}

void JKQTPverticalRange::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPverticalRange::draw");
#endif
    double c=xAxis->x2p(rangeCenter);
    double cmi=xAxis->x2p(rangeMin);
    double cma=xAxis->x2p(rangeMax);
    double mi=yAxis->x2p(parent->getYAxis()->getMin());
    double ma=yAxis->x2p(parent->getYAxis()->getMax());
    double smi=xAxis->x2p(parent->getXAxis()->getMin());
    double sma=xAxis->x2p(parent->getXAxis()->getMax());
    if (!unlimitedSizeMin) mi=yAxis->x2p(sizeMin);
    if (!unlimitedSizeMax) ma=yAxis->x2p(sizeMax);
    painter.save();
    if (plotRange && (cmi!=cma) && JKQTPIsOKFloat(mi) && JKQTPIsOKFloat(ma)) {
        QPen p=painter.pen();
        QPen np(Qt::NoPen);
        p.setColor(color);
        p.setStyle(style);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
        QBrush nb(Qt::NoBrush);
        QBrush b=painter.brush();
        b.setColor(fillColor);
        b.setStyle(fillStyle);
        QRectF rect(QPointF(cmi, mi), QPointF(cma, ma));
        if (fillRange) {
            painter.setPen(np);
            painter.setBrush(b);
            if (invertedRange) {
                if (JKQTPIsOKFloat(cmi)) {
                    QRectF rectl(QPointF(smi,mi), QPointF(cmi,ma));
                    painter.drawRect(rectl);
                }
                if (JKQTPIsOKFloat(cma)) {
                    QRectF rectr(QPointF(cma,mi), QPointF(sma,ma));
                    painter.drawRect(rectr);
                }
            } else {
                if (JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) painter.drawRect(rect);
            }
        }
        if (plotRangeLines && JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) {
            painter.setPen(p);
            painter.setBrush(nb);
            painter.drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
            painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
        }

    }
    if (plotCenterLine&& JKQTPIsOKFloat(c) && JKQTPIsOKFloat(ma) && JKQTPIsOKFloat(mi)) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, centerLineWidth*parent->get_lineWidthMultiplier())));
        painter.setPen(p);
        painter.drawLine(QLineF(c, mi, c, ma));
    }
    painter.restore();
}

void JKQTPverticalRange::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush nb(Qt::NoBrush);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    if (plotRange) {
        painter.setPen(np);
        painter.setBrush(b);
        if (fillRange) painter.drawRect(rect);
        painter.setPen(p);
        painter.setBrush(nb);
        if (plotRangeLines) {
            painter.drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
            painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
        }
    }
    if (plotCenterLine) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        painter.setPen(p);
        int y=rect.left()+rect.width()/2.0;
        painter.drawLine(y, rect.top(), y, rect.bottom());
    }
    painter.restore();
}


QPen JKQTPxyLineGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

QPen JKQTPxyLineGraph::getSymbolPen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*symbolWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

QBrush JKQTPxyLineGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    return b;
}

void JKQTPxyGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==NULL)  return ;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPxyLineGraph::SortedX) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(xColumn,i);
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());


    } else if (sortData==JKQTPxyLineGraph::SortedY) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(yColumn,i);
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());
    }
}

QPen JKQTPimpulsesHorizontalGraph::getPen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    return p;

}


QPen JKQTPfilledCurveXGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}


QBrush JKQTPfilledCurveXGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPboxplotVerticalGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

void JKQTPboxplotVerticalGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==NULL)  return ;

    JKQTPdatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(posColumn).getRows();
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPboxplotVerticalGraph::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(posColumn,i);
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());


    }
}

QBrush JKQTPboxplotVerticalGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QBrush JKQTPboxplotVerticalElement::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPboxplotVerticalElement::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}


QBrush JKQTPxFunctionLineGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPxFunctionLineGraph::getLinePen(JKQTPEnhancedPainter &painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

QBrush JKQTPxFunctionLineGraph::getErrorBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(errorFillColor);
    b.setStyle(errorFillStyle);
    return b;
}

QPen JKQTPxFunctionLineGraph::getErrorLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(errorColor);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*errorLineWidth)));
    p.setStyle(errorStyle);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

QBrush JKQTPstepHorizontalGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPstepHorizontalGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

QBrush JKQTPbarHorizontalGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPbarHorizontalGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

QBrush JKQTPhorizontalRange::getBrush(JKQTPEnhancedPainter& /*painter*/ ) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPhorizontalRange::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

void JKQTPxFunctionLineGraph::set_params(const QVector<double> &params)
{
    iparams=params;
    set_params(&iparams);
}

void JKQTPxFunctionLineGraph::set_copiedParams(const double *params, int N)
{
    QVector<double> v;
    for (int i=0; i<N; i++) { v<<params[i]; }
    set_params(v);
}

void JKQTPxFunctionLineGraph::set_errorParams(const QVector<double> &errorParams)
{
    ierrorparams=errorParams;
    set_errorParams(&ierrorparams);
}


void JKQTPxFunctionLineGraph::setSpecialFunction(JKQTPxFunctionLineGraph::SpecialFunction function)
{
    if (function==JKQTPxFunctionLineGraph::Polynomial) set_plotFunction(JKQTPxFunctionLineGraphPolynomial);
    else if (function==JKQTPxFunctionLineGraph::Exponential) set_plotFunction(JKQTPxFunctionLineGraphExponential);
    else if (function==JKQTPxFunctionLineGraph::PowerLaw) set_plotFunction(JKQTPxFunctionLineGraphPowerLaw);
}

QVector<double> JKQTPxFunctionLineGraph::get_internalParams() const {
    return iparams;
}
QVector<double> JKQTPxFunctionLineGraph::get_internalErrorParams() const {
    return ierrorparams;
}

bool JKQTPxGraphErrors::errorUsesColumn(int c)
{
    return c==(xErrorColumn) || (c==xErrorColumnLower);
}

bool JKQTPgraphErrors::errorUsesColumn(int /*c*/)
{
    return false;
}

void JKQTPgraphErrors::setErrorColorFromGraphColor(QColor graphColor)
{
    errorColor=graphColor.darker();
    errorFillColor=graphColor.lighter();
    //errorColor.setAlphaF(0.5);
}

bool JKQTPyGraphErrors::errorUsesColumn(int c)
{
    return (c==yErrorColumn) || (c==yErrorColumnLower);
}

bool JKQTPxyGraphErrors::errorUsesColumn(int c)
{
    return (c==xErrorColumn)||(c==yErrorColumn)||(c==xErrorColumnLower)||(c==yErrorColumnLower);
}

bool JKQTPxFunctionLineGraph::usesColumn(int c)
{
    return (c==parameterColumn)||(c==errorParameterColumn);
}

bool JKQTPimpulsesHorizontalErrorGraph::usesColumn(int c)
{
    return JKQTPimpulsesHorizontalGraph::usesColumn(c)|| JKQTPxGraphErrors::errorUsesColumn(c);
}

bool JKQTPimpulsesVerticalErrorGraph::usesColumn(int c)
{
    return JKQTPimpulsesVerticalGraph::usesColumn(c)|| JKQTPyGraphErrors::errorUsesColumn(c);
}

bool JKQTPfilledCurveXErrorGraph::usesColumn(int c)
{
    return JKQTPfilledCurveXGraph::usesColumn(c)|| JKQTPyGraphErrors::errorUsesColumn(c);
}

bool JKQTPfilledCurveYErrorGraph::usesColumn(int c)
{
    return JKQTPfilledCurveYGraph::usesColumn(c)|| JKQTPxGraphErrors::errorUsesColumn(c);
}


JKQTPxyParametrizedScatterGraph::JKQTPxyParametrizedScatterGraph(JKQtBasePlotter *parent):
    JKQTPxyLineGraph(parent),
    JKQTPColorPaletteTools(parent)
{
    sizeColumn=-1;
    colorColumn=-1;
    symbolColumn=-1;
    palette=JKQTPMathImageMATLAB;
    colorColumnContainsRGB=false;
    symbol=JKQTPfilledCircle;
    drawLine=false;

    gridModeForSymbolSize=false;
    gridDeltaX=1;
    gridDeltaY=1;
    gridSymbolFractionSize=0.9;
}

JKQTPxyParametrizedScatterGraph::JKQTPxyParametrizedScatterGraph(JKQtPlotter *parent):
    JKQTPxyLineGraph(parent),
    JKQTPColorPaletteTools(parent->get_plotter())
{
    sizeColumn=-1;
    colorColumn=-1;
    symbolColumn=-1;
    palette=JKQTPMathImageMATLAB;
    colorColumnContainsRGB=false;
    symbol=JKQTPfilledCircle;
    drawLine=false;

    gridModeForSymbolSize=false;
    gridDeltaX=1;
    gridDeltaY=1;
    gridSymbolFractionSize=0.9;
}
void JKQTPxyParametrizedScatterGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPxyParametrizedScatterGraph::draw");
#endif
    if (parent==NULL) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return;

    cbGetDataMinMax(intColMin, intColMax);

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);


    QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
    QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
    if (selectionLineColor!=Qt::transparent) {
        penSelection.setColor(selectionLineColor);
    }
    penSelection.setWidthF(penSelection.widthF()*3.0);


    int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, (long long)imax);
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, (long long)imax);
    }
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    //qDebug()<<"JKQTPxyLineGraph::draw(): "<<3<<" imin="<<imin<<" imax="<<imax;
    double xold=-1;
    double yold=-1;
    bool first=false;
    QVector<QLineF> lines;
    QPolygonF linesP;
    QVector<QColor> linecols;
    QVector<QColor> linecolss;
    intSortData();
    double specSymbSize=0;
    bool hasSpecSymbSize=false;
    painter.save();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(xColumn,i);
        double yv=datastore->get(yColumn,i);
        double x=xAxis->x2p(xv);
        double y=yAxis->x2p(yv);
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
            double symbSize= parent->pt2px(painter, getLocalSymbolSize(i));

            if (gridModeForSymbolSize) {
                if (!hasSpecSymbSize) {
                    double sSX= fabs(xAxis->x2p( xv+gridDeltaX*gridSymbolFractionSize/2.0)-xAxis->x2p( xv-gridDeltaX*gridSymbolFractionSize/2.0));
                    double sSY= fabs(yAxis->x2p( yv+gridDeltaY*gridSymbolFractionSize/2.0)-yAxis->x2p( yv-gridDeltaY*gridSymbolFractionSize/2.0));
                    hasSpecSymbSize=true;
                    specSymbSize=qMin(sSX,sSY);
                }
                symbSize=specSymbSize;
            }
            QColor symbColor=getLocalColor(i);
            QColor symbFillColor=symbColor.lighter();
            //qDebug()<<i<<symbolSize<<symbColor;
            if (first && drawLine) {
                double xl1=xold;
                double yl1=yold;
                double xl2=x;
                double yl2=y;

                if (drawSelectionLine) {
                    if (colorColumn>=0) linecolss<<symbColor.lighter();
                    else linecolss<<selectionLineColor;
                }
                linecols<<symbColor;
                lines<<QLineF(xl1, yl1, xl2, yl2);
                linesP<<QPointF(x,y);
            }

            if (drawSelectionLine && symbol!=JKQTPnoSymbol && symbolColumn<0) {
                plotSymbol(painter, x, y, JKQTPfilledCircle,symbSize, parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), penSelection.color(), penSelection.color());
            } else {
                plotSymbol(painter, x, y, getLocalSymbolType(i), symbSize, parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), symbColor, symbFillColor);
            }


            xold=x;
            yold=y;
            first=true;
        }
    }
    painter.restore();

    if (lines.size()>0) {
        painter.save();
        if (drawSelectionLine) {
            QPen pp=penSelection;
            if (colorColumn>=0) {
                for (int i=0; i<lines.size(); i++) {
                    pp.setColor(linecolss.value(i, selectionLineColor));
                    painter.setPen(pp);
                    painter.drawLine(lines[i]);
                }
            } else {
                pp.setColor(selectionLineColor);
                painter.setPen(pp);
                painter.drawPolyline(linesP);
            }
        }
        QPen pp=p;
        if (colorColumn>=0) {
            for (int i=0; i<lines.size(); i++) {
                pp.setColor(linecols.value(i, selectionLineColor));
                painter.setPen(pp);
                painter.drawLine(lines[i]);
            }
        } else {
            pp.setColor(selectionLineColor);
            painter.setPen(pp);
            painter.drawPolyline(linesP);
        }
        painter.restore();
    }
    painter.restore();
    drawErrorsAfter(painter);
}

QColor JKQTPxyParametrizedScatterGraph::getKeyLabelColor()
{
    return getLocalColor(-1);
}

void JKQTPxyParametrizedScatterGraph::setParent(JKQtBasePlotter *parent)
{
    JKQTPxyLineGraph::setParent(parent);
    cbSetParent(parent);
}

void JKQTPxyParametrizedScatterGraph::getOutsideSize(JKQTPEnhancedPainter &painter, int &leftSpace, int &rightSpace, int &topSpace, int &bottomSpace)
{
    JKQTPxyLineGraph::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar&& colorColumn>=0) cbGetOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPxyParametrizedScatterGraph::drawOutside(JKQTPEnhancedPainter &painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace)
{
    JKQTPxyLineGraph::drawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar&& colorColumn>=0) cbDrawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPxyParametrizedScatterGraph::cbGetDataMinMax(double &dmin, double &dmax)
{
    if (autoImageRange) {
        dmin=dmax=0;
        if (parent==NULL) return;
        JKQTPdatastore* datastore=parent->getDatastore();
        if (datastore==NULL) return;
        if (colorColumn<0) return;
        int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
        int imin=0;
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, (long long)imax);
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, (long long)imax);
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        bool first=true;
        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(colorColumn,i);
            if (first) {
                dmin=dmax=xv;
                first=false;
            } else {
                dmin=qMin(xv, dmin);
                dmax=qMax(xv, dmax);
            }
        }

    } else {
        dmin=imageMin;
        dmax=imageMax;
    }

}

bool JKQTPxyParametrizedScatterGraph::usesColumn(int c)
{
    return (c==colorColumn) || (c==sizeColumn) || (c==symbolColumn) || JKQTPxyLineGraph::usesColumn(c);
}

double JKQTPxyParametrizedScatterGraph::getInternalDataMin() const
{
    return 0;
}

double JKQTPxyParametrizedScatterGraph::getInternalDataMax() const
{
    return 0;
}

double JKQTPxyParametrizedScatterGraph::getLocalSymbolSize(int i)
{
    if (parent==NULL) return symbolSize;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return symbolSize;
    if (sizeColumn<0) return symbolSize;
    if (i>=(int64_t)datastore->getColumn(sizeColumn).getRows()) return symbolSize;
    return datastore->get(sizeColumn,i);
}

QColor JKQTPxyParametrizedScatterGraph::getLocalColor(int i)
{
    if (parent==NULL) return color;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return color;
    if (colorColumn<0) return color;
    if (colorColumnContainsRGB) {
        if (i<0 || i>=(int64_t)datastore->getColumn(colorColumn).getRows()) return color;
        //QRgb rgb=
        return QRgb(round(datastore->get(colorColumn,i)));
    } else {
        QImage img;
        double colorval=0;
        if (i>=0 && i<(int64_t)datastore->getColumn(colorColumn).getRows()) colorval=datastore->get(colorColumn,i);
        double colMin=0;
        double colMax=0;
        if (intColMin==intColMax) {
            colMin=0;
            colMax=datastore->getColumn(colorColumn).getRows()-1;
        } else {
            colMin=intColMin;
            colMax=intColMax;
        }
        JKQTPimagePlot_array2image(&colorval, 1, 1, img, palette, colMin, colMax);
        return img.pixel(0,0);
    }

}

JKQTPgraphSymbols JKQTPxyParametrizedScatterGraph::getLocalSymbolType(int i)
{
    if (parent==NULL) return symbol;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==NULL) return symbol;
    if (symbolColumn<0) return symbol;
    if (i>=(int64_t)datastore->getColumn(symbolColumn).getRows()) return symbol;
    int id=floor(datastore->get(symbolColumn,i));
    if (id<0 || id>JKQTPmaxSymbolID) return symbol;
    return JKQTPgraphSymbols(id);

}


bool JKQTPxyParametrizedErrorScatterGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    if (xErrorColumn<0 || xErrorStyle==JKQTPnoError) {
        return JKQTPxyLineGraph::getXMinMax(minx, maxx, smallestGreaterZero);
    } else {
        bool start=true;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        if (parent==NULL) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, (long long)imax);
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, (long long)imax);
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double xvsgz;
            double xv=datastore->get(xColumn,i)+getXErrorU(i, datastore);
            double xvv=datastore->get(xColumn,i)-getXErrorL(i, datastore);
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xvv) ) {
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                if (start || xvv>maxx) maxx=xvv;
                if (start || xvv<minx) minx=xvv;
                xvsgz=xvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

bool JKQTPxyParametrizedErrorScatterGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (yErrorColumn<0 || yErrorStyle==JKQTPnoError) {
        return JKQTPxyLineGraph::getYMinMax(miny, maxy, smallestGreaterZero);
    } else {
        bool start=true;
        miny=0;
        maxy=0;
        smallestGreaterZero=0;

        if (parent==NULL) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(xColumn).getRows(), datastore->getColumn(yColumn).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, (long long)imax);
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, (long long)imax);
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(yColumn,i)+getYErrorU(i, datastore);
            double yvv=datastore->get(yColumn,i)-getYErrorL(i, datastore);
            if (JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(yvv) ) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                if (start || yvv>maxy) maxy=yvv;
                if (start || yvv<miny) miny=yvv;
                xvsgz=yvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

bool JKQTPxyParametrizedErrorScatterGraph::usesColumn(int c)
{
    return JKQTPxyParametrizedScatterGraph::usesColumn(c)||JKQTPxyGraphErrors::errorUsesColumn(c);

}

bool JKQTPxyParametrizedErrorScatterGraph::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, JKQtBasePlotter */*parent*/, JKQTPcoordinateAxis* /*xAxis*/, JKQTPcoordinateAxis* /*yAxis*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPerrorPlotstyle /*xErrorStyle*/, JKQTPerrorPlotstyle /*yErrorStyle*/, long long index, QColor &/*errorColor*/, QColor &/*errorFillColor*/)
{
    QColor c=getLocalColor(index);
    errorColor=c.darker();
    errorFillColor=c.lighter();
    //qDebug()<<"intPlotXYErrorIndicatorsGetColor("<<index<<"): "<<errorColor;
    return true;
}


bool JKQTPbarHorizontalGraph::isHorizontal() const
{
    return true;
}
