/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/graphs/jkqtpimpulses.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;






JKQTPImpulsesGraphBase::JKQTPImpulsesGraphBase(JKQTBasePlotter* parent):
    JKQTPXYBaselineGraph(parent), drawSymbols(false)
{
    initLineStyle(parent, parentPlotStyle);
    initSymbolStyle(parent, parentPlotStyle);
    setLineWidth(3);
}

QColor JKQTPImpulsesGraphBase::getKeyLabelColor() const {
    return getLineColor();
}

void JKQTPImpulsesGraphBase::setColor(QColor c)
{
    setLineColor(c);
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
}

void JKQTPImpulsesGraphBase::setDrawSymbols(bool __value)
{
    drawSymbols=__value;
}

bool JKQTPImpulsesGraphBase::getDrawSymbols() const
{
    return drawSymbols;
}


JKQTPImpulsesHorizontalGraph::JKQTPImpulsesHorizontalGraph(JKQTBasePlotter* parent):
    JKQTPImpulsesGraphBase(parent)
{

}

JKQTPImpulsesHorizontalGraph::JKQTPImpulsesHorizontalGraph(JKQTPlotter* parent):
    JKQTPImpulsesHorizontalGraph(parent->getPlotter())
{
}

void JKQTPImpulsesHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPImpulsesHorizontalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        p.setCapStyle(Qt::FlatCap);

        int imax=static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));
        int imin=0;
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        //double xold=-1;
        //double yold=-1;
        double x0=transformX(getBaseline());
        if (parent->getXAxis()->isLogAxis()) {
            if (getBaseline()>0 && getBaseline()>parent->getXAxis()->getMin()) x0=transformX(getBaseline());
            else x0=transformX(parent->getXAxis()->getMin());
        }
    //    double y0=transformY(getBaseline());
    //    if (parent->getYAxis()->isLogAxis()) {
    //        y0=transformY(parent->getYAxis()->getMin());
    //        if (getBaseline()>0 && getBaseline()>parent->getYAxis()->getMin()) y0=transformY(getBaseline());
    //        else y0=transformY(parent->getYAxis()->getMin());
    //    }
        //bool first=false;
        QVector<QLineF> lines;
        QVector<QPointF> points;
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound(imin, getDataIndex(iii), imax);
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=transformX(xv);
                double y=transformY(yv);


                lines.append(QLineF(x0, y, x, y));
                points.append(QPointF(x,y));
    //            xold=x;
    //            yold=y;
                //first=true;
            }
        }
        painter.setPen(p);
        if (lines.size()>0) painter.drawLines(lines);
        if (drawSymbols && points.size()>0) {
            painter.save(); auto __finalpaintsym=JKQTPFinally([&painter]() {painter.restore();});
            for (auto& p: points) {
                plotStyledSymbol(parent, painter, p.x(), p.y());
            }
        }

    }
    drawErrorsAfter(painter);
}

void JKQTPImpulsesHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {


    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setCapStyle(Qt::FlatCap);
    painter.setPen(p);

    const int y=rect.top()+rect.height()/2.0;
    painter.drawLine(rect.left(), y, rect.right(), y);
}












JKQTPImpulsesVerticalGraph::JKQTPImpulsesVerticalGraph(JKQTBasePlotter* parent):
    JKQTPImpulsesGraphBase(parent)
{
}

JKQTPImpulsesVerticalGraph::JKQTPImpulsesVerticalGraph(JKQTPlotter *parent):
    JKQTPImpulsesVerticalGraph(parent->getPlotter())
{

}

void JKQTPImpulsesVerticalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {


    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setCapStyle(Qt::FlatCap);
    painter.setPen(p);
    const int x=rect.left()+rect.width()/2.0;
    painter.drawLine(x, rect.bottom(), x, rect.top());

}

void JKQTPImpulsesVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPImpulsesVerticalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        p.setCapStyle(Qt::FlatCap);

        int imax=static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));
        int imin=0;
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
    //    double x0=transformX(getBaseline());
    //    if (parent->getXAxis()->isLogAxis()) {
    //        if (getBaseline()>0 && getBaseline()>parent->getXAxis()->getMin()) x0=transformX(getBaseline());
    //        else x0=transformX(parent->getXAxis()->getMin());
    //    }
        double y0=transformY(getBaseline());
        if (parent->getYAxis()->isLogAxis()) {
            y0=transformY(parent->getYAxis()->getMin());
            if (getBaseline()>0 && getBaseline()>parent->getYAxis()->getMin()) y0=transformY(getBaseline());
            else y0=transformY(parent->getYAxis()->getMin());
        }
        QVector<QLineF> lines;
        QVector<QPointF> points;
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound(imin, getDataIndex(iii), imax);
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) ) {
                double x=transformX(xv);
                double y=transformY(yv);


                lines.append(QLineF(x, y0, x, y));
                points.append(QPointF(x,y));
                //xold=x;
                //yold=y;
                //first=true;
            }
        }
        painter.setPen(p);
        if (lines.size()>0) painter.drawLines(lines);
        if (drawSymbols && points.size()>0) {
            painter.save(); auto __finalpaintsym=JKQTPFinally([&painter]() {painter.restore();});
            for (auto& p: points) {
                plotStyledSymbol(parent, painter, p.x(), p.y());
            }
        }
    }

    drawErrorsAfter(painter);
}






JKQTPImpulsesHorizontalErrorGraph::JKQTPImpulsesHorizontalErrorGraph(JKQTBasePlotter *parent):
    JKQTPImpulsesHorizontalGraph(parent)
{
    setErrorColorFromGraphColor(getLineColor());
    initErrorStyle(parent, parentPlotStyle);
}

JKQTPImpulsesHorizontalErrorGraph::JKQTPImpulsesHorizontalErrorGraph(JKQTPlotter *parent):
    JKQTPImpulsesHorizontalErrorGraph(parent->getPlotter())
{

}

bool JKQTPImpulsesHorizontalErrorGraph::usesColumn(int c) const
{
    return JKQTPImpulsesHorizontalGraph::usesColumn(c)|| JKQTPXGraphErrors::errorUsesColumn(c);
}

void JKQTPImpulsesHorizontalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);
}

JKQTPImpulsesVerticalErrorGraph::JKQTPImpulsesVerticalErrorGraph(JKQTBasePlotter *parent):
    JKQTPImpulsesVerticalGraph(parent)
{
    setErrorColorFromGraphColor(getLineColor());
    initErrorStyle(parent, parentPlotStyle);
}

JKQTPImpulsesVerticalErrorGraph::JKQTPImpulsesVerticalErrorGraph(JKQTPlotter *parent):
    JKQTPImpulsesVerticalErrorGraph(parent->getPlotter())
{

}

bool JKQTPImpulsesVerticalErrorGraph::usesColumn(int c) const
{
    return JKQTPImpulsesVerticalGraph::usesColumn(c)|| JKQTPYGraphErrors::errorUsesColumn(c);
}

void JKQTPImpulsesVerticalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);

}
