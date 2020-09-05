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

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;




JKQTPXYFunctionLineGraph::JKQTPXYFunctionLineGraph(JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    tmin=0.0;
    tmax=1.0;
    params=nullptr;
    minSamples=100;
    maxRefinementDegree=7;
    slopeTolerance=0.005;
    minPixelPerSample=32;
    plotRefinement=true;
    displaySamplePoints=false;

    initLineStyle(parent, parentPlotStyle);

    parameterColumn=-1;

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

void JKQTPXYFunctionLineGraph::setParams(void *__value)
{
    if (this->params != __value) {
        this->params = __value;
        data.clear();
    }
}

void *JKQTPXYFunctionLineGraph::getParams() const
{
    return this->params;
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

bool JKQTPXYFunctionLineGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    if (data.size()==0) createPlotData();
    if (data.size()>0){
        bool start=true;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        for (auto const& d: data) {
            if (JKQTPIsOKFloat(d.x())) {
                if (start || d.x()>maxx) maxx=d.x();
                if (start || d.x()<minx) minx=d.x();
                double xvsgz;
                xvsgz=d.x(); SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    } else {
        smallestGreaterZero=minx=maxx=0; return false;
    }
}

bool JKQTPXYFunctionLineGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (data.size()==0) createPlotData();
    if (data.size()>0){
        bool start=true;
        miny=0;
        maxy=0;
        smallestGreaterZero=0;

        for (auto const& d: data) {
            if (JKQTPIsOKFloat(d.y())) {
                if (start || d.y()>maxy) maxy=d.y();
                if (start || d.y()<miny) miny=d.y();
                double xvsgz;
                xvsgz=d.x(); SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    } else {
        smallestGreaterZero=miny=maxy=0; return false;
    }
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
    if (plotFunction) func=std::bind(plotFunction, std::placeholders::_1, params);
    else if (simplePlotFunction) func=simplePlotFunction;

    jkqtpSimpleParametricCurveFunctionType fTransformedFunc= std::bind([&](const JKQTPPlotElement* plot, double t) -> QPointF { return plot->transform(func(t)); }, this, std::placeholders::_1);

    JKQTPAdaptiveFunctionGraphEvaluator evaluator(fTransformedFunc, minSamples, maxRefinementDegree, slopeTolerance, minPixelPerSample);
    data=evaluator.evaluate(tmin, tmax);

}

void JKQTPXYFunctionLineGraph::collectParameters()
{
    if (parent && parameterColumn>=0) {
        iparams.clear();
        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=static_cast<int>(datastore->getRows(parameterColumn));

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


        QColor c=getLineColor();
        c.setHsv(fmod(c.hue()+90, 360), c.saturation(), c.value());
        if (displaySamplePoints) {
            painter.save(); auto __finalpaintsamplepoints=JKQTPFinally([&painter]() {painter.restore();});
            for (const auto& d: data) {
                if (JKQTPIsOKFloat(d.x()) && JKQTPIsOKFloat(d.x())) {
                    JKQTPPlotSymbol(painter, d.x(), d.y(), JKQTPCross, 6,1*parent->getLineWidthMultiplier(), c, QColor(Qt::transparent));
                }
            }

        }
    }
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}





void JKQTPXYFunctionLineGraph::setParams(const QVector<double> &params)
{
    iparams=params;
    setParams(&iparams);
}

void JKQTPXYFunctionLineGraph::setCopiedParams(const double *params, int N)
{
    QVector<double> v;
    for (int i=0; i<N; i++) { v<<params[i]; }
    setParams(v);
}

void JKQTPXYFunctionLineGraph::setParamsV(double p1) {
    QVector<double> p;
    p<<p1;
    setParams(p);
}

void JKQTPXYFunctionLineGraph::setParamsV(double p1, double p2) {
    QVector<double> p;
    p<<p1<<p2;
    setParams(p);
}

void JKQTPXYFunctionLineGraph::setParamsV(double p1, double p2, double p3) {
    QVector<double> p;
    p<<p1<<p2<<p3;
    setParams(p);
}

void JKQTPXYFunctionLineGraph::setParamsV(double p1, double p2, double p3, double p4) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4;
    setParams(p);
}

void JKQTPXYFunctionLineGraph::setParamsV(double p1, double p2, double p3, double p4, double p5) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4<<p5;
    setParams(p);
}


void JKQTPXYFunctionLineGraph::setParameterColumn(int __value)
{
    this->parameterColumn = __value;
}

int JKQTPXYFunctionLineGraph::getParameterColumn() const
{
    return this->parameterColumn;
}

void JKQTPXYFunctionLineGraph::setParameterColumn(size_t __value) {
    this->parameterColumn = static_cast<int>(__value);
}


QVector<double> JKQTPXYFunctionLineGraph::getInternalParams() const {
    return iparams;
}

void JKQTPXYFunctionLineGraph::setMinSamples(const unsigned int &__value)
{
    this->minSamples = __value;
}

unsigned int JKQTPXYFunctionLineGraph::getMinSamples() const
{
    return this->minSamples;
}

void JKQTPXYFunctionLineGraph::setMaxRefinementDegree(const unsigned int &__value)
{
    this->maxRefinementDegree = __value;
}

unsigned int JKQTPXYFunctionLineGraph::getMaxRefinementDegree() const
{
    return this->maxRefinementDegree;
}

void JKQTPXYFunctionLineGraph::setSlopeTolerance(double __value)
{
    this->slopeTolerance = __value;
}

double JKQTPXYFunctionLineGraph::getSlopeTolerance() const
{
    return this->slopeTolerance;
}

void JKQTPXYFunctionLineGraph::setMinPixelPerSample(double __value)
{
    this->minPixelPerSample = __value;
}

double JKQTPXYFunctionLineGraph::getMinPixelPerSample() const
{
    return this->minPixelPerSample;
}

void JKQTPXYFunctionLineGraph::setPlotRefinement(bool __value)
{
    this->plotRefinement = __value;
}

bool JKQTPXYFunctionLineGraph::getPlotRefinement() const
{
    return this->plotRefinement;
}

void JKQTPXYFunctionLineGraph::setDisplaySamplePoints(bool __value)
{
    this->displaySamplePoints = __value;
}

bool JKQTPXYFunctionLineGraph::getDisplaySamplePoints() const
{
    return this->displaySamplePoints;
}


bool JKQTPXYFunctionLineGraph::usesColumn(int c) const
{
    return (c==parameterColumn);
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

