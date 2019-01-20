/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/jkqtpgraphsimpulses.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;





JKQTPImpulsesHorizontalGraph::JKQTPImpulsesHorizontalGraph(JKQTBasePlotter* parent):
    JKQTPXYGraph(parent)
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

JKQTPImpulsesHorizontalGraph::JKQTPImpulsesHorizontalGraph(JKQTPlotter* parent):
    JKQTPXYGraph(parent)
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
void JKQTPImpulsesHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPImpulsesHorizontalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
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

    //double xold=-1;
    //double yold=-1;
    double x0=transformX(baseline);
    if (parent->get_xAxis()->isLogAxis()) {
        if (baseline>0 && baseline>parent->get_xAxis()->getMin()) x0=transformX(baseline);
        else x0=transformX(parent->get_xAxis()->getMin());
    }
//    double y0=transformY(baseline);
//    if (parent->get_yAxis()->isLogAxis()) {
//        y0=transformY(parent->get_yAxis()->getMin());
//        if (baseline>0 && baseline>parent->get_yAxis()->getMin()) y0=transformY(baseline);
//        else y0=transformY(parent->get_yAxis()->getMin());
//    }
    //bool first=false;
    QVector<QLineF> lines;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=transformX(xv);
            double y=transformY(yv);


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

void JKQTPImpulsesHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {


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

QColor JKQTPImpulsesHorizontalGraph::getKeyLabelColor() {
    return color;
}











JKQTPImpulsesVerticalGraph::JKQTPImpulsesVerticalGraph(JKQTBasePlotter* parent):
    JKQTPImpulsesHorizontalGraph(parent)
{
}

JKQTPImpulsesVerticalGraph::JKQTPImpulsesVerticalGraph(JKQTPlotter *parent):
    JKQTPImpulsesHorizontalGraph(parent)
{

}

void JKQTPImpulsesVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPImpulsesVerticalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
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

    //double xold=-1;
    //double yold=-1;
    //bool first=false;
//    double x0=transformX(baseline);
//    if (parent->get_xAxis()->isLogAxis()) {
//        if (baseline>0 && baseline>parent->get_xAxis()->getMin()) x0=transformX(baseline);
//        else x0=transformX(parent->get_xAxis()->getMin());
//    }
    double y0=transformY(baseline);
    if (parent->get_yAxis()->isLogAxis()) {
        y0=transformY(parent->get_yAxis()->getMin());
        if (baseline>0 && baseline>parent->get_yAxis()->getMin()) y0=transformY(baseline);
        else y0=transformY(parent->get_yAxis()->getMin());
    }
    QVector<QLineF> lines;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) ) {
            double x=transformX(xv);
            double y=transformY(yv);


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





QPen JKQTPImpulsesHorizontalGraph::getPen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    return p;

}



JKQTPImpulsesHorizontalErrorGraph::JKQTPImpulsesHorizontalErrorGraph(JKQTBasePlotter *parent):
    JKQTPImpulsesHorizontalGraph(parent), JKQTPXGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

JKQTPImpulsesHorizontalErrorGraph::JKQTPImpulsesHorizontalErrorGraph(JKQTPlotter *parent):
    JKQTPImpulsesHorizontalGraph(parent), JKQTPXGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

bool JKQTPImpulsesHorizontalErrorGraph::usesColumn(int c) const
{
    return JKQTPImpulsesHorizontalGraph::usesColumn(c)|| JKQTPXGraphErrors::errorUsesColumn(c);
}

void JKQTPImpulsesHorizontalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);
}

JKQTPImpulsesVerticalErrorGraph::JKQTPImpulsesVerticalErrorGraph(JKQTBasePlotter *parent):
    JKQTPImpulsesVerticalGraph(parent), JKQTPYGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

JKQTPImpulsesVerticalErrorGraph::JKQTPImpulsesVerticalErrorGraph(JKQTPlotter *parent):
    JKQTPImpulsesVerticalGraph(parent), JKQTPYGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

bool JKQTPImpulsesVerticalErrorGraph::usesColumn(int c) const
{
    return JKQTPImpulsesVerticalGraph::usesColumn(c)|| JKQTPYGraphErrors::errorUsesColumn(c);
}

void JKQTPImpulsesVerticalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

}
