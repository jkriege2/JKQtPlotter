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



#include "jkqtplotter/graphs/jkqtpimpulses.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;






JKQTPImpulsesGraphBase::JKQTPImpulsesGraphBase(JKQTBasePlotter* parent):
    JKQTPXYBaselineGraph(parent),
    drawSymbols(false),
    m_drawBaseline(parent->getCurrentPlotterStyle().graphsStyle.impulseStyle.drawBaseline)
{
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Impulses);
    initSymbolStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Impulses);
    m_baselineStyle=*this;
}

QColor JKQTPImpulsesGraphBase::getKeyLabelColor() const {
    return getLineColor();
}

void JKQTPImpulsesGraphBase::setColor(QColor c)
{
    setLineColor(c);
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.impulseStyle.fillColorDerivationMode, c));
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
    m_baselineStyle.setLineColor(c);
}

void JKQTPImpulsesGraphBase::setDrawSymbols(bool __value)
{
    drawSymbols=__value;
}

bool JKQTPImpulsesGraphBase::getDrawSymbols() const
{
    return drawSymbols;
}

void JKQTPImpulsesGraphBase::setDrawBaseline(bool __value)
{
    m_drawBaseline=__value;
}

bool JKQTPImpulsesGraphBase::getDrawBaseline() const
{
    return this->m_drawBaseline;
}

JKQTPGraphLineStyleMixin &JKQTPImpulsesGraphBase::baselineStyle()
{
    return m_baselineStyle;
}

const JKQTPGraphLineStyleMixin &JKQTPImpulsesGraphBase::baselineStyle() const
{
    return m_baselineStyle;
}

bool JKQTPImpulsesGraphBase::getValuesMinMax(double &mmin, double &mmax, double &smallestGreaterZero)
{
    mmin=0;
    mmax=0;
    smallestGreaterZero=0;
    if (getBaseline()>0) {
        smallestGreaterZero=getBaseline();
        mmin=getBaseline();
        mmax=getBaseline();
    }

    if (getKeyColumn()<0 || getValueColumn()<0) return false;

    const size_t datacol=static_cast<size_t>(getValueColumn());

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0, imax=0;
    if (getIndexRange(imin, imax)) {


        for (int i=imin; i<imax; i++) {
            double yv=getBaseline();
            if (JKQTPIsOKFloat(yv)) {
                if (yv>mmax) mmax=yv;
                if (yv<mmin) mmin=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            }
            yv=datastore->get(datacol,static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yv)) {
                if (yv>mmax) mmax=yv;
                if (yv<mmin) mmin=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            }
        }
        return true;
    }
    return false;
}

bool JKQTPImpulsesGraphBase::getPositionsMinMax(double &mmin, double &mmax, double &smallestGreaterZero)
{
    bool start=true;
    mmin=0;
    mmax=0;
    smallestGreaterZero=0;

    if (getKeyColumn()<0 || getValueColumn()<0) return false;

    const size_t poscol=static_cast<size_t>(getKeyColumn());

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0, imax=0;
    if (getIndexRange(imin, imax)) {
        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(poscol,static_cast<size_t>(i));


            if (JKQTPIsOKFloat(xv)) {

                if (start || xv>mmax) mmax=xv;
                if (start || xv<mmin) mmin=xv;
                double xvsgz;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
    return false;
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

        int imax=0;
        int imin=0;
        double bottom=-1e6;
        double top=1e6;
        bool firstXY=true;
        if (getIndexRange(imin, imax)) {

            double x0=transformX(getBaseline());
            if (getXAxis()->isLogAxis()) {
                if (getBaseline()>0 && getBaseline()>getXAxis()->getMin()) x0=transformX(getBaseline());
                else x0=transformX(getXAxis()->getMin());
            }

            QVector<QLineF> lines;
            QVector<QPointF> points;
            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                const int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                    const double x=transformX(xv);
                    const double y=transformY(yv);
                    if (firstXY) {
                        bottom=y;
                        top=y;
                    } else {
                        bottom=qMax(bottom, y);
                        top=qMin(top, y);
                    }
                    firstXY=false;
                    lines.append(QLineF(x0, y, x, y));
                    points.append(QPointF(x,y));

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
        if (getDrawBaseline() && top!=bottom) {
            painter.setPen(baselineStyle().getLinePen(painter, parent));
            const double xb=transformX(getBaseline());
            if (JKQTPIsOKFloat(xb)) painter.drawLine(xb,top,xb,bottom);
        }

    }
    drawErrorsAfter(painter);
}

void JKQTPImpulsesHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) {


    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getKeyLinePen(painter, rect, parent);
    p.setCapStyle(Qt::FlatCap);
    painter.setPen(p);

    const int y=rect.top()+rect.height()/2.0;
    painter.drawLine(rect.left(), y, rect.right(), y);
}

int JKQTPImpulsesHorizontalGraph::getKeyColumn() const
{
    return getYColumn();
}

int JKQTPImpulsesHorizontalGraph::getValueColumn() const
{
    return getXColumn();
}

bool JKQTPImpulsesHorizontalGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    return getValuesMinMax(minx, maxx, smallestGreaterZero);
}

bool JKQTPImpulsesHorizontalGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    return getPositionsMinMax(miny, maxy, smallestGreaterZero);
}

void JKQTPImpulsesHorizontalGraph::setKeyColumn(int __value)
{
    setYColumn(__value);
}

void JKQTPImpulsesHorizontalGraph::setValueColumn(int __value)
{
    setXColumn(__value);
}












JKQTPImpulsesVerticalGraph::JKQTPImpulsesVerticalGraph(JKQTBasePlotter* parent):
    JKQTPImpulsesGraphBase(parent)
{
}

JKQTPImpulsesVerticalGraph::JKQTPImpulsesVerticalGraph(JKQTPlotter *parent):
    JKQTPImpulsesVerticalGraph(parent->getPlotter())
{

}

void JKQTPImpulsesVerticalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) {


    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getKeyLinePen(painter, rect, parent);
    p.setCapStyle(Qt::FlatCap);
    painter.setPen(p);
    const int x=rect.left()+rect.width()/2.0;
    painter.drawLine(x, rect.bottom(), x, rect.top());

}

bool JKQTPImpulsesVerticalGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    return getPositionsMinMax(minx, maxx, smallestGreaterZero);
}

bool JKQTPImpulsesVerticalGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    return getValuesMinMax(miny, maxy, smallestGreaterZero);
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

        int imax=0;
        int imin=0;
        double left=-1e6;
        double right=1e6;
        bool firstXY=true;
        if (getIndexRange(imin, imax)) {



            double y0=transformY(getBaseline());
            if (getYAxis()->isLogAxis()) {
                if (getBaseline()>0 && getBaseline()>getYAxis()->getMin()) y0=transformY(getBaseline());
                else y0=transformY(getYAxis()->getMin());
            }
            QVector<QLineF> lines;
            QVector<QPointF> points;
            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                const int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) ) {
                    const double x=transformX(xv);
                    const double y=transformY(yv);
                    if (firstXY) {
                        left=x;
                        right=x;
                    } else {
                        left=qMin(left, x);
                        right=qMax(right, x);
                    }
                    firstXY=false;


                    lines.append(QLineF(x, y0, x, y));
                    points.append(QPointF(x,y));

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
        if (getDrawBaseline() && left!=right) {
            painter.setPen(baselineStyle().getLinePen(painter, parent));
            const double yb=transformY(getBaseline());
            if (JKQTPIsOKFloat(yb)) painter.drawLine(left, yb, right, yb);
        }
    }

    drawErrorsAfter(painter);
}






JKQTPImpulsesHorizontalErrorGraph::JKQTPImpulsesHorizontalErrorGraph(JKQTBasePlotter *parent):
    JKQTPImpulsesHorizontalGraph(parent)
{
    setErrorColorFromGraphColor(getLineColor());
    initErrorStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Impulses);
}

JKQTPImpulsesHorizontalErrorGraph::JKQTPImpulsesHorizontalErrorGraph(JKQTPlotter *parent):
    JKQTPImpulsesHorizontalErrorGraph(parent->getPlotter())
{

}

bool JKQTPImpulsesHorizontalErrorGraph::usesColumn(int c) const
{
    return JKQTPImpulsesHorizontalGraph::usesColumn(c)|| JKQTPXGraphErrors::errorUsesColumn(c);
}

bool JKQTPImpulsesHorizontalErrorGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    if (xErrorColumn<0 || xErrorStyle==JKQTPNoError) {
        return getMinMaxWithBaseline(xColumn, minx, maxx, smallestGreaterZero);
    } else {
        return getMinMaxWithErrorsAndBaseline(xColumn, xErrorColumn, xErrorColumnLower, xErrorSymmetric, minx, maxx, smallestGreaterZero);
    }

}

int JKQTPImpulsesHorizontalErrorGraph::getErrorColumn() const
{
    return getXErrorColumn();
}

int JKQTPImpulsesHorizontalErrorGraph::getErrorColumnLower() const
{
    return getXErrorColumnLower();
}

JKQTPErrorPlotstyle JKQTPImpulsesHorizontalErrorGraph::getErrorStyle() const
{
    return getXErrorStyle();
}

bool JKQTPImpulsesHorizontalErrorGraph::getErrorSymmetric() const
{
    return getXErrorSymmetric();
}

void JKQTPImpulsesHorizontalErrorGraph::setErrorSymmetric(bool __value)
{
    setXErrorSymmetric(__value);
}

void JKQTPImpulsesHorizontalErrorGraph::setErrorStyle(JKQTPErrorPlotstyle __value)
{
    setXErrorStyle(__value);
}

void JKQTPImpulsesHorizontalErrorGraph::setErrorColumn(size_t column)
{
    setXErrorColumn(column);
}

void JKQTPImpulsesHorizontalErrorGraph::setErrorColumn(int column)
{
    setXErrorColumn(column);
}

void JKQTPImpulsesHorizontalErrorGraph::setErrorColumnLower(int column)
{
    setXErrorColumnLower(column);
}

void JKQTPImpulsesHorizontalErrorGraph::setErrorColumnLower(size_t column)
{
    setXErrorColumnLower(column);
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
    initErrorStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Impulses);
}

JKQTPImpulsesVerticalErrorGraph::JKQTPImpulsesVerticalErrorGraph(JKQTPlotter *parent):
    JKQTPImpulsesVerticalErrorGraph(parent->getPlotter())
{

}

int JKQTPImpulsesVerticalErrorGraph::getErrorColumn() const
{
    return getYErrorColumn();
}

int JKQTPImpulsesVerticalErrorGraph::getErrorColumnLower() const
{
    return getYErrorColumnLower();
}

JKQTPErrorPlotstyle JKQTPImpulsesVerticalErrorGraph::getErrorStyle() const
{
    return getYErrorStyle();
}

bool JKQTPImpulsesVerticalErrorGraph::getErrorSymmetric() const
{
    return getYErrorSymmetric();
}

void JKQTPImpulsesVerticalErrorGraph::setErrorSymmetric(bool __value)
{
    setYErrorSymmetric(__value);
}

void JKQTPImpulsesVerticalErrorGraph::setErrorStyle(JKQTPErrorPlotstyle __value)
{
    setYErrorStyle(__value);
}

void JKQTPImpulsesVerticalErrorGraph::setErrorColumn(size_t column)
{
    setYErrorColumn(column);
}

void JKQTPImpulsesVerticalErrorGraph::setErrorColumn(int column)
{
    setYErrorColumn(column);
}

void JKQTPImpulsesVerticalErrorGraph::setErrorColumnLower(int column)
{
    setYErrorColumnLower(column);
}

void JKQTPImpulsesVerticalErrorGraph::setErrorColumnLower(size_t column)
{
    setYErrorColumnLower(column);
}

bool JKQTPImpulsesVerticalErrorGraph::usesColumn(int c) const
{
    return JKQTPImpulsesVerticalGraph::usesColumn(c)|| JKQTPYGraphErrors::errorUsesColumn(c);
}

bool JKQTPImpulsesVerticalErrorGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (yErrorColumn<0 || yErrorStyle==JKQTPNoError) {
        return getMinMaxWithBaseline(yColumn, miny, maxy, smallestGreaterZero);
    } else {
        return getMinMaxWithErrorsAndBaseline(yColumn, yErrorColumn, yErrorColumnLower, yErrorSymmetric, miny, maxy, smallestGreaterZero);
    }
}


void JKQTPImpulsesVerticalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);

}
