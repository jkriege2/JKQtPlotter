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



#include "jkqtplotter/jkqtpelements.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpimageelements.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;









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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=getLinePen(painter);
    p.setCapStyle(Qt::FlatCap);

    int imax=datastore->getColumn(dataColumn).getRows();
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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









JKQTPxyLineGraph::JKQTPxyLineGraph(JKQtPlotter* parent):
    JKQTPxyGraph(parent)
{
    sortData=JKQTPxyGraph::Unsorted;
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
    sortData=JKQTPxyGraph::Unsorted;
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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

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


    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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

        if (parent==nullptr) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, static_cast<long long>(imax));
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, static_cast<long long>(imax));
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
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))+getXErrorU(i, datastore);
            if (JKQTPIsOKFloat(xv)) {
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))-getXErrorL(i, datastore);
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

        if (parent==nullptr) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, static_cast<long long>(imax));
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, static_cast<long long>(imax));
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))+getYErrorU(i, datastore);
            if (JKQTPIsOKFloat(yv)) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))-getYErrorL(i, datastore);
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

void JKQTPxyLineErrorGraph::drawErrorsBefore(JKQTPEnhancedPainter &painter)
{
    intSortData();
    if (sortData==JKQTPxyGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(Qt::SolidLine);
    p.setCapStyle(Qt::FlatCap);

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(Qt::SolidLine);
    p.setCapStyle(Qt::FlatCap);

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

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

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

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

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

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

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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











JKQTPstepVerticalGraph::JKQTPstepVerticalGraph(JKQtBasePlotter *parent):
    JKQTPstepHorizontalGraph(parent)
{
}

JKQTPstepVerticalGraph::JKQTPstepVerticalGraph(JKQtPlotter *parent):
    JKQTPstepHorizontalGraph(parent)
{

}

void JKQTPstepVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPstepVerticalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

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

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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

bool JKQTPhorizontalRange::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    smallestGreaterZero=minx=maxx=0;
    return false;
}

bool JKQTPhorizontalRange::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    miny=rangeMin;
    maxy=rangeMax;
    smallestGreaterZero=0;
    if (rangeMin>0) smallestGreaterZero=rangeMin;
    return true;
}

QColor JKQTPhorizontalRange::getKeyLabelColor()
{
    return color;
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

bool JKQTPverticalRange::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    minx=rangeMin;
    maxx=rangeMax;
    smallestGreaterZero=0;
    if (rangeMin>0) smallestGreaterZero=rangeMin;
    return true;
}

bool JKQTPverticalRange::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    smallestGreaterZero=miny=maxy=0;
    return false;
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

bool JKQTPimpulsesHorizontalErrorGraph::usesColumn(int c)
{
    return JKQTPimpulsesHorizontalGraph::usesColumn(c)|| JKQTPxGraphErrors::errorUsesColumn(c);
}

void JKQTPimpulsesHorizontalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPxyGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);
}

JKQTPimpulsesVerticalErrorGraph::JKQTPimpulsesVerticalErrorGraph(JKQtBasePlotter *parent):
    JKQTPimpulsesVerticalGraph(parent), JKQTPyGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

JKQTPimpulsesVerticalErrorGraph::JKQTPimpulsesVerticalErrorGraph(JKQtPlotter *parent):
    JKQTPimpulsesVerticalGraph(parent), JKQTPyGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

bool JKQTPimpulsesVerticalErrorGraph::usesColumn(int c)
{
    return JKQTPimpulsesVerticalGraph::usesColumn(c)|| JKQTPyGraphErrors::errorUsesColumn(c);
}

void JKQTPimpulsesVerticalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPxyGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

}

JKQTPfilledCurveXErrorGraph::JKQTPfilledCurveXErrorGraph(JKQtBasePlotter *parent):
    JKQTPfilledCurveXGraph(parent), JKQTPyGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

JKQTPfilledCurveXErrorGraph::JKQTPfilledCurveXErrorGraph(JKQtPlotter *parent):
    JKQTPfilledCurveXGraph(parent), JKQTPyGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

bool JKQTPfilledCurveXErrorGraph::usesColumn(int c)
{
    return JKQTPfilledCurveXGraph::usesColumn(c)|| JKQTPyGraphErrors::errorUsesColumn(c);
}

void JKQTPfilledCurveXErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPxyGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

}

JKQTPfilledCurveYErrorGraph::JKQTPfilledCurveYErrorGraph(JKQtBasePlotter *parent):
    JKQTPfilledCurveYGraph(parent), JKQTPxGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

JKQTPfilledCurveYErrorGraph::JKQTPfilledCurveYErrorGraph(JKQtPlotter *parent):
    JKQTPfilledCurveYGraph(parent), JKQTPxGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

bool JKQTPfilledCurveYErrorGraph::usesColumn(int c)
{
    return JKQTPfilledCurveYGraph::usesColumn(c)|| JKQTPxGraphErrors::errorUsesColumn(c);
}

void JKQTPfilledCurveYErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPxyGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

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
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

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


    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<long long>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<long long>(imax));
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
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
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
        if (parent==nullptr) return;
        JKQTPdatastore* datastore=parent->getDatastore();
        if (datastore==nullptr) return;
        if (colorColumn<0) return;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        int imin=0;
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, static_cast<long long>(imax));
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, static_cast<long long>(imax));
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
    if (parent==nullptr) return symbolSize;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return symbolSize;
    if (sizeColumn<0) return symbolSize;
    if (i>=(int64_t)datastore->getColumn(sizeColumn).getRows()) return symbolSize;
    return datastore->get(sizeColumn,i);
}

QColor JKQTPxyParametrizedScatterGraph::getLocalColor(int i)
{
    if (parent==nullptr) return color;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return color;
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
    if (parent==nullptr) return symbol;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return symbol;
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

        if (parent==nullptr) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, static_cast<long long>(imax));
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, static_cast<long long>(imax));
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
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))+getXErrorU(i, datastore);
            double xvv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))-getXErrorL(i, datastore);
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

        if (parent==nullptr) return false;

        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        // interpret data ranges
        if (datarange_start>-1) {
            imin=qMin(datarange_start, static_cast<long long>(imax));
        }
        if (datarange_end>-1) {
            imax=qMin(datarange_end, static_cast<long long>(imax));
        }
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))+getYErrorU(i, datastore);
            double yvv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))-getYErrorL(i, datastore);
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

void JKQTPxyParametrizedErrorScatterGraph::drawErrorsBefore(JKQTPEnhancedPainter &painter)
{
    intSortData();
    if (sortData==JKQTPxyGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);
}

bool JKQTPxyParametrizedErrorScatterGraph::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, JKQtBasePlotter */*parent*/, JKQTPcoordinateAxis* /*xAxis*/, JKQTPcoordinateAxis* /*yAxis*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPerrorPlotstyle /*xErrorStyle*/, JKQTPerrorPlotstyle /*yErrorStyle*/, long long index, QColor &/*errorColor*/, QColor &/*errorFillColor*/)
{
    QColor c=getLocalColor(index);
    errorColor=c.darker();
    errorFillColor=c.lighter();
    //qDebug()<<"intPlotXYErrorIndicatorsGetColor("<<index<<"): "<<errorColor;
    return true;
}


