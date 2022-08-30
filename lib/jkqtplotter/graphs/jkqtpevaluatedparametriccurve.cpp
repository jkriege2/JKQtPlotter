/*
    Copyright (c) 2020-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/graphs/jkqtpevaluatedparametriccurve.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"





JKQTPXYFunctionLineGraphBase::JKQTPXYFunctionLineGraphBase(double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPEvaluatedFunctionGraphBase(parent),
    tmin(tmin_),
    tmax(tmax_)
{
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
    setMaxRefinementDegree(8);
}

JKQTPXYFunctionLineGraphBase::JKQTPXYFunctionLineGraphBase(double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraphBase(tmin_,tmax_,parent->getPlotter())
{

}

JKQTPXYFunctionLineGraphBase::JKQTPXYFunctionLineGraphBase(JKQTBasePlotter* parent):
    JKQTPXYFunctionLineGraphBase(0,1,parent)
{
}

JKQTPXYFunctionLineGraphBase::JKQTPXYFunctionLineGraphBase(JKQTPlotter* parent):
    JKQTPXYFunctionLineGraphBase(parent->getPlotter())
{

}


JKQTPXYFunctionLineGraphBase::~JKQTPXYFunctionLineGraphBase()
{

}



void JKQTPXYFunctionLineGraphBase::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    p.setWidthF(getKeyLineWidthPx(painter, rect, parent));
    QPen np(Qt::NoPen);
    const double y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    painter.setPen(p);
    painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
}

QColor JKQTPXYFunctionLineGraphBase::getKeyLabelColor() const {
    return getLineColor();
}


void JKQTPXYFunctionLineGraphBase::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPXYFunctionLineGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //qDebug()<<"start plot\n";
    createPlotData();
    //qDebug()<<"plot data created\n";

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        QPen np(Qt::NoPen);


        {
            painter.save(); auto __finalpaintline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            painter.drawPolyline(data);
        }


        if (displaySamplePoints) drawSamplePoints(painter, getLineColor());
    }
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}



double JKQTPXYFunctionLineGraphBase::getTMin() const
{
    return tmin;
}

double JKQTPXYFunctionLineGraphBase::getTMax() const
{
    return tmax;
}

void JKQTPXYFunctionLineGraphBase::setTMin(double val)
{
    tmin=val;
}

void JKQTPXYFunctionLineGraphBase::setTMax(double val)
{
    tmax=val;
}

QPair<double, double> JKQTPXYFunctionLineGraphBase::getTRange() const
{
    return QPair<double, double>(tmin,tmax);
}

void JKQTPXYFunctionLineGraphBase::setTRange(double tmin_, double tmax_)
{
    tmin=tmin_;
    tmax=tmax_;
}

void JKQTPXYFunctionLineGraphBase::setTRange(const QPair<double, double> &range)
{
    tmin=range.first;
    tmax=range.second;
}






JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(JKQTBasePlotter* parent):
    JKQTPXYFunctionLineGraphBase(parent)
{
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(JKQTPlotter* parent):
    JKQTPXYFunctionLineGraph(parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpSimpleParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraphBase(tmin_, tmax_, parent), plotFunction(), simplePlotFunction(f)
{
    setTitle(title_);
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpSimpleParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(f, title_, tmin_, tmax_, parent->getPlotter())
{

}


JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpSimpleParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraphBase(tmin_, tmax_, parent), plotFunction(), simplePlotFunction(std::move(f))
{
    setTitle(title_);
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpSimpleParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(std::forward<jkqtpSimpleParametricCurveFunctionType>(f), title_, tmin_, tmax_, parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraphBase(tmin_, tmax_, parent), plotFunction(std::move(f)), simplePlotFunction()
{
    setTitle(title_);
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(std::forward<jkqtpParametricCurveFunctionType>(f), title_, tmin_, tmax_, parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraphBase(tmin_, tmax_, parent), plotFunction(f), simplePlotFunction()
{
    setTitle(title_);
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(f, title_, tmin_, tmax_, parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::~JKQTPXYFunctionLineGraph() {

}



void JKQTPXYFunctionLineGraph::setPlotFunctionFunctor(const jkqtpParametricCurveFunctionType &__value)
{
    simplePlotFunction=jkqtpSimpleParametricCurveFunctionType();
    plotFunction = __value;
    data.clear();
}

void JKQTPXYFunctionLineGraph::setPlotFunctionFunctor(const jkqtpSimpleParametricCurveFunctionType &__value)
{
    plotFunction=jkqtpParametricCurveFunctionType();
    simplePlotFunction=__value;
    data.clear();
}

void JKQTPXYFunctionLineGraph::setPlotFunctionFunctor(jkqtpParametricCurveFunctionType &&__value)
{
    simplePlotFunction=jkqtpSimpleParametricCurveFunctionType();
    plotFunction = std::move(__value);
    data.clear();
}

void JKQTPXYFunctionLineGraph::setPlotFunctionFunctor(jkqtpSimpleParametricCurveFunctionType &&__value)
{
    plotFunction=jkqtpParametricCurveFunctionType();
    simplePlotFunction=std::move(__value);
    data.clear();
}

jkqtpParametricCurveFunctionType JKQTPXYFunctionLineGraph::getPlotFunctionFunctor() const
{
    return plotFunction;
}

jkqtpSimpleParametricCurveFunctionType JKQTPXYFunctionLineGraph::getSimplePlotFunction() const
{
    return simplePlotFunction;
}

bool JKQTPXYFunctionLineGraph::isSimplePlotFunction() const
{
    return !static_cast<bool>(plotFunction) && static_cast<bool>(simplePlotFunction);
}


JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec JKQTPXYFunctionLineGraph::buildPlotFunctorSpec()
{
    JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec spec;

    if (!plotFunction && !simplePlotFunction) return spec; // return invalid spec!

    // range over which to evaluate func
    spec.range_start=tmin;
    spec.range_end=tmax;

    // the actual function to use
    if (plotFunction) spec.func=std::bind(plotFunction, std::placeholders::_1, getInternalParams());
    else if (simplePlotFunction) spec.func=simplePlotFunction;

    return spec;
}

