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
    JKQTPGraph(parent)
{
    minSamples=50;
    maxRefinementDegree=5;
    slopeTolerance=0.005;
    minPixelPerSample=32;
    dataCleanupMaxAllowedAngleDegree=0.2;
    displaySamplePoints=false;
    data.clear();

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
            JKQTPPlotSymbol(painter, d.x(), d.y(), JKQTPCross, 6,1*parent->getLineWidthMultiplier(), c, QColor(Qt::transparent));
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



JKQTPEvaluatedFunctionWithParamsGraphBase::JKQTPEvaluatedFunctionWithParamsGraphBase(JKQTBasePlotter *parent):
    parameterColumn(-1)
{

}

JKQTPEvaluatedFunctionWithParamsGraphBase::JKQTPEvaluatedFunctionWithParamsGraphBase(JKQTPlotter *parent):
    JKQTPEvaluatedFunctionWithParamsGraphBase(parent->getPlotter())
{

}

JKQTPEvaluatedFunctionWithParamsGraphBase::~JKQTPEvaluatedFunctionWithParamsGraphBase()
{

}

void JKQTPEvaluatedFunctionWithParamsGraphBase::collectParameters()
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


void JKQTPEvaluatedFunctionWithParamsGraphBase::setParams(const QVector<double> &params)
{
    iparams=params;
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setCopiedParams(const double *params, int N)
{
    QVector<double> v;
    for (int i=0; i<N; i++) { v<<params[i]; }
    setParams(v);
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setParamsV(double p1) {
    QVector<double> p;
    p<<p1;
    setParams(p);
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setParamsV(double p1, double p2) {
    QVector<double> p;
    p<<p1<<p2;
    setParams(p);
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setParamsV(double p1, double p2, double p3) {
    QVector<double> p;
    p<<p1<<p2<<p3;
    setParams(p);
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setParamsV(double p1, double p2, double p3, double p4) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4;
    setParams(p);
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setParamsV(double p1, double p2, double p3, double p4, double p5) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4<<p5;
    setParams(p);
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setParameterColumn(int __value)
{
    this->parameterColumn = __value;
}

int JKQTPEvaluatedFunctionWithParamsGraphBase::getParameterColumn() const
{
    return this->parameterColumn;
}

void JKQTPEvaluatedFunctionWithParamsGraphBase::setParameterColumn(size_t __value) {
    this->parameterColumn = static_cast<int>(__value);
}

QVector<double> JKQTPEvaluatedFunctionWithParamsGraphBase::getInternalParams() const {
    return iparams;
}

bool JKQTPEvaluatedFunctionWithParamsGraphBase::usesColumn(int c) const
{
    return (c==parameterColumn);
}
