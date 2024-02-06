/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/graphs/jkqtpevaluatedfunctionbase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;




JKQTPEvaluatedFunctionGraphBase::JKQTPEvaluatedFunctionGraphBase(JKQTBasePlotter* parent):
    JKQTPGraph(parent),
    parameterColumn(-1),
    minSamples(50),
    maxRefinementDegree(5),
    slopeTolerance(0.005),
    minPixelPerSample(32),
    dataCleanupMaxAllowedAngleDegree(0.2),
    displaySamplePoints(false)
{
    data.clear();
    iparams.clear();
}

JKQTPEvaluatedFunctionGraphBase::JKQTPEvaluatedFunctionGraphBase(JKQTPlotter* parent):
    JKQTPEvaluatedFunctionGraphBase(parent->getPlotter())
{

}

JKQTPEvaluatedFunctionGraphBase::~JKQTPEvaluatedFunctionGraphBase()
{
    data.clear();
}


void JKQTPEvaluatedFunctionGraphBase::drawSamplePoints(JKQTPEnhancedPainter& painter, QColor graphColor) {
    QColor c=graphColor;
    c.setHsv(fmod(c.hue()+90, 360), c.saturation(), c.value());
    painter.save(); auto __finalpaintsamplepoints=JKQTPFinally([&painter]() {painter.restore();});
    for (const auto& d: data) {
        if (JKQTPIsOKFloat(d.x()) && JKQTPIsOKFloat(d.y())) {
            JKQTPPlotSymbol(painter, d.x(), d.y(), JKQTPCross, 6,1*parent->getLineWidthMultiplier(), c, QColor(Qt::transparent), QGuiApplication::font().family());
        }
    }
}



void JKQTPEvaluatedFunctionGraphBase::setMinSamples(const unsigned int &__value)
{
    this->minSamples = __value;
}

unsigned int JKQTPEvaluatedFunctionGraphBase::getMinSamples() const
{
    return this->minSamples;
}

void JKQTPEvaluatedFunctionGraphBase::setMaxRefinementDegree(const unsigned int &__value)
{
    this->maxRefinementDegree = __value;
}

unsigned int JKQTPEvaluatedFunctionGraphBase::getMaxRefinementDegree() const
{
    return this->maxRefinementDegree;
}

void JKQTPEvaluatedFunctionGraphBase::setSlopeTolerance(double __value)
{
    this->slopeTolerance = __value;
}

double JKQTPEvaluatedFunctionGraphBase::getSlopeTolerance() const
{
    return this->slopeTolerance;
}

void JKQTPEvaluatedFunctionGraphBase::setMinPixelPerSample(double __value)
{
    this->minPixelPerSample = __value;
}

double JKQTPEvaluatedFunctionGraphBase::getMinPixelPerSample() const
{
    return this->minPixelPerSample;
}

void JKQTPEvaluatedFunctionGraphBase::setDataCleanupMaxAllowedAngleDegree(double __value)
{
    dataCleanupMaxAllowedAngleDegree=__value;
}

double JKQTPEvaluatedFunctionGraphBase::getDataCleanupMaxAllowedAngleDegree() const
{
    return dataCleanupMaxAllowedAngleDegree;
}

void JKQTPEvaluatedFunctionGraphBase::setDisplaySamplePoints(bool __value)
{
    this->displaySamplePoints = __value;
}

void JKQTPEvaluatedFunctionGraphBase::createPlotData(bool collectParams)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPEvaluatedFunctionWithErrorsGraphBase[%1]::createPlotData()").arg(title));
#endif
    data.clear();
    if (collectParams) collectParameters();
    PlotFunctorSpec plotfunc= buildPlotFunctorSpec();

    if (plotfunc.isValid()) {
        std::function<QPointF(double)> fTransformedFunc= std::bind([plotfunc](const JKQTPPlotElement* plot, double t) -> QPointF { return plot->transform(plotfunc.func(t)); }, this, std::placeholders::_1);

        JKQTPAdaptiveFunctionGraphEvaluator evaluator(fTransformedFunc, minSamples, maxRefinementDegree, slopeTolerance, minPixelPerSample);
        data=evaluator.evaluate(plotfunc.range_start, plotfunc.range_end);
        data=JKQTPSimplyfyLineSegemnts(data, dataCleanupMaxAllowedAngleDegree);
    }
}

bool JKQTPEvaluatedFunctionGraphBase::getDisplaySamplePoints() const
{
    return this->displaySamplePoints;
}

bool JKQTPEvaluatedFunctionGraphBase::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
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

bool JKQTPEvaluatedFunctionGraphBase::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
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


void JKQTPEvaluatedFunctionGraphBase::collectParameters()
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
        int i=iparams.size()-1;
        while (i>=0 && !JKQTPIsOKFloat(iparams[i])) {
            iparams.remove(i,1);
            i--;
        }

    }
}


void JKQTPEvaluatedFunctionGraphBase::setParams(const QVector<double> &params)
{
    iparams=params;
}

void JKQTPEvaluatedFunctionGraphBase::setCopiedParams(const double *params, int N)
{
    QVector<double> v;
    for (int i=0; i<N; i++) { v<<params[i]; }
    setParams(v);
}

void JKQTPEvaluatedFunctionGraphBase::setParamsV(double p1) {
    QVector<double> p;
    p<<p1;
    setParams(p);
}

void JKQTPEvaluatedFunctionGraphBase::setParamsV(double p1, double p2) {
    QVector<double> p;
    p<<p1<<p2;
    setParams(p);
}

void JKQTPEvaluatedFunctionGraphBase::setParamsV(double p1, double p2, double p3) {
    QVector<double> p;
    p<<p1<<p2<<p3;
    setParams(p);
}

void JKQTPEvaluatedFunctionGraphBase::setParamsV(double p1, double p2, double p3, double p4) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4;
    setParams(p);
}

void JKQTPEvaluatedFunctionGraphBase::setParamsV(double p1, double p2, double p3, double p4, double p5) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4<<p5;
    setParams(p);
}

void JKQTPEvaluatedFunctionGraphBase::setParameterColumn(int __value)
{
    this->parameterColumn = __value;
}

int JKQTPEvaluatedFunctionGraphBase::getParameterColumn() const
{
    return this->parameterColumn;
}

void JKQTPEvaluatedFunctionGraphBase::setParameterColumn(size_t __value) {
    this->parameterColumn = static_cast<int>(__value);
}

const QVector<double>& JKQTPEvaluatedFunctionGraphBase::getInternalParams() const {
    return iparams;
}

QVector<double> &JKQTPEvaluatedFunctionGraphBase::getInternalParams()
{
    return iparams;
}

bool JKQTPEvaluatedFunctionGraphBase::usesColumn(int c) const
{
    return (c==parameterColumn);
}

JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec::PlotFunctorSpec():
    func(),
    range_start(0),
    range_end(0)
{

}

bool JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec::isValid() const
{
    return static_cast<bool>(func) && (fabs(range_end-range_start)>0);
}




JKQTPEvaluatedFunctionWithErrorsGraphBase::JKQTPEvaluatedFunctionWithErrorsGraphBase(JKQTBasePlotter *parent):
    JKQTPEvaluatedFunctionGraphBase(parent),
    errorParameterColumn(-1)
{

}

JKQTPEvaluatedFunctionWithErrorsGraphBase::JKQTPEvaluatedFunctionWithErrorsGraphBase(JKQTPlotter *parent):
    JKQTPEvaluatedFunctionWithErrorsGraphBase(parent->getPlotter())
{

}

JKQTPEvaluatedFunctionWithErrorsGraphBase::~JKQTPEvaluatedFunctionWithErrorsGraphBase()
{

}

const QVector<double>& JKQTPEvaluatedFunctionWithErrorsGraphBase::getInternalErrorParams() const {
    return ierrorparams;
}

QVector<double>& JKQTPEvaluatedFunctionWithErrorsGraphBase::getInternalErrorParams() {
    return ierrorparams;
}

bool JKQTPEvaluatedFunctionWithErrorsGraphBase::usesColumn(int c) const
{
    return JKQTPEvaluatedFunctionGraphBase::usesColumn(c)||(c==errorParameterColumn);
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParams(const QVector<double> &errorParams)
{
    ierrorparams=errorParams;
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParameterColumn(int __value)
{
    this->errorParameterColumn = __value;
}

int JKQTPEvaluatedFunctionWithErrorsGraphBase::getErrorParameterColumn() const
{
    return this->errorParameterColumn;
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParameterColumn(size_t __value) {
    this->errorParameterColumn = static_cast<int>(__value);
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParamsV(double p1)
{
    QVector<double> p;
    p<<p1;
    setErrorParams(p);
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParamsV(double p1, double p2)
{
    QVector<double> p;
    p<<p1<<p2;
    setErrorParams(p);
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParamsV(double p1, double p2, double p3)
{
    QVector<double> p;
    p<<p1<<p2<<p3;
    setErrorParams(p);
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParamsV(double p1, double p2, double p3, double p4)
{
    QVector<double> p;
    p<<p1<<p2<<p3<<p4;
    setErrorParams(p);
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::setErrorParamsV(double p1, double p2, double p3, double p4, double p5)
{
    QVector<double> p;
    p<<p1<<p2<<p3<<p4<<p5;
    setErrorParams(p);
}

void JKQTPEvaluatedFunctionWithErrorsGraphBase::collectParameters()
{
    JKQTPEvaluatedFunctionGraphBase::collectParameters();

    if (parent && errorParameterColumn>=0) {
        ierrorparams.clear();
        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax= static_cast<int>(datastore->getRows(errorParameterColumn));

        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(errorParameterColumn,i);
            ierrorparams<<xv;
        }
        int i=ierrorparams.size()-1;
        while (i>=0 && !JKQTPIsOKFloat(ierrorparams[i])) {
            ierrorparams.remove(i,1);
            i--;
        }
    }
}
