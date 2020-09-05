/*
    Copyright (c) 2020-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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





JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(JKQTBasePlotter* parent):
    JKQTPEvaluatedFunctionWithParamsGraphBase(parent)
{
    tmin=0.0;
    tmax=1.0;

    initLineStyle(parent, parentPlotStyle);

}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(JKQTPlotter* parent):
    JKQTPXYFunctionLineGraph(parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpSimpleParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraph(parent)
{
    tmin=tmin_;
    tmax=tmax_;
    title=title_;
    plotFunction=jkqtpParametricCurveFunctionType();
    simplePlotFunction=f;
    data.clear();
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpSimpleParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(f, title_, tmin_, tmax_, parent->getPlotter())
{

}


JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpSimpleParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraph(parent)
{
    tmin=tmin_;
    tmax=tmax_;
    title=title_;
    plotFunction=jkqtpParametricCurveFunctionType();
    simplePlotFunction=std::move(f);
    data.clear();
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpSimpleParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(std::forward<jkqtpSimpleParametricCurveFunctionType>(f), title_, tmin_, tmax_, parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraph(parent)
{
    tmin=tmin_;
    tmax=tmax_;
    title=title_;
    simplePlotFunction=jkqtpSimpleParametricCurveFunctionType();
    plotFunction=std::move(f);
    data.clear();
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(jkqtpParametricCurveFunctionType &&f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(std::forward<jkqtpParametricCurveFunctionType>(f), title_, tmin_, tmax_, parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTBasePlotter *parent):
    JKQTPXYFunctionLineGraph(parent)
{
    tmin=tmin_;
    tmax=tmax_;
    title=title_;
    simplePlotFunction=jkqtpSimpleParametricCurveFunctionType();
    plotFunction=std::move(f);
    data.clear();
}

JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(const jkqtpParametricCurveFunctionType &f, const QString &title_, double tmin_, double tmax_, JKQTPlotter *parent):
    JKQTPXYFunctionLineGraph(f, title_, tmin_, tmax_, parent->getPlotter())
{

}

JKQTPXYFunctionLineGraph::~JKQTPXYFunctionLineGraph() {
    data.clear();
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


void JKQTPXYFunctionLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);
    const double y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    painter.setPen(p);
    painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
}

QColor JKQTPXYFunctionLineGraph::getKeyLabelColor() const {
    return getLineColor();
}


void JKQTPXYFunctionLineGraph::createPlotData(bool collectParams) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPXYFunctionLineGraph[%1]::createPlotData()").arg(title));
#endif
    data.clear();
    if (collectParams) collectParameters();

    if (parent==nullptr) return;
    if (!plotFunction && !simplePlotFunction) return;

    jkqtpSimpleParametricCurveFunctionType func;
    if (plotFunction) func=std::bind(plotFunction, std::placeholders::_1, getInternalParams());
    else if (simplePlotFunction) func=simplePlotFunction;

    jkqtpSimpleParametricCurveFunctionType fTransformedFunc= std::bind([&](const JKQTPPlotElement* plot, double t) -> QPointF { return plot->transform(func(t)); }, this, std::placeholders::_1);

    JKQTPAdaptiveFunctionGraphEvaluator evaluator(fTransformedFunc, minSamples, maxRefinementDegree, slopeTolerance, minPixelPerSample);
    data=evaluator.evaluate(tmin, tmax);
    data=JKQTPSimplyfyLineSegemnts(data, dataCleanupMaxAllowedAngleDegree);
}


void JKQTPXYFunctionLineGraph::draw(JKQTPEnhancedPainter& painter) {
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



double JKQTPXYFunctionLineGraph::getTMin() const
{
    return tmin;
}

double JKQTPXYFunctionLineGraph::getTMax() const
{
    return tmax;
}

void JKQTPXYFunctionLineGraph::setTMin(double val)
{
    tmin=val;
}

void JKQTPXYFunctionLineGraph::setTMax(double val)
{
    tmax=val;
}

QPair<double, double> JKQTPXYFunctionLineGraph::getTRange() const
{
    return QPair<double, double>(tmin,tmax);
}

void JKQTPXYFunctionLineGraph::setTRange(double tmin_, double tmax_)
{
    tmin=tmin_;
    tmax=tmax_;
}

void JKQTPXYFunctionLineGraph::setTRange(const QPair<double, double> &range)
{
    tmin=range.first;
    tmax=range.second;
}

