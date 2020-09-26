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



#include "jkqtplotter/graphs/jkqtpboxplotbase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;




JKQTPBoxplotGraphBase::JKQTPBoxplotGraphBase(JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    boxWidthRelative=0.4;
    useRelativeBoxWidth=true;
    posColumn=-1;
    medianColumn=-1;
    meanColumn=-1;
    minColumn=-1;
    maxColumn=-1;
    percentile25Column=-1;
    percentile75Column=-1;
    medianConfidenceColumn=-1;

    sortData=Unsorted;

    initBoxplotStyle(parent, parentPlotStyle);
    setMeanSymbolType(JKQTPPlus);
}


bool JKQTPBoxplotGraphBase::usesColumn(int c) const
{
    return (c==meanColumn)||(c==posColumn)||(c==medianColumn)||(c==minColumn)||(c==maxColumn)||(c==percentile25Column)||(c==percentile75Column);
}

void JKQTPBoxplotGraphBase::setDataSortOrder(JKQTPBoxplotGraphBase::DataSortOrder __value)
{
    this->sortData = __value;
}

JKQTPBoxplotGraphBase::DataSortOrder JKQTPBoxplotGraphBase::getDataSortOrder() const
{
    return this->sortData;
}

void JKQTPBoxplotGraphBase::setDataSortOrder(int __value) {
    sortData=static_cast<DataSortOrder>(__value);
    if (__value>0) sortData=Sorted;
}

void JKQTPBoxplotGraphBase::setPositionColumn(int __value)
{
    this->posColumn = __value;
}

int JKQTPBoxplotGraphBase::getPositionColumn() const
{
    return this->posColumn;
}

void JKQTPBoxplotGraphBase::setPositionColumn(size_t __value) {
    this->posColumn = static_cast<int>(__value);
}

void JKQTPBoxplotGraphBase::setMedianColumn(int __value)
{
    this->medianColumn = __value;
}

int JKQTPBoxplotGraphBase::getMedianColumn() const
{
    return this->medianColumn;
}

void JKQTPBoxplotGraphBase::setMedianColumn(size_t __value) {
    this->medianColumn = static_cast<int>(__value);
}

void JKQTPBoxplotGraphBase::setMeanColumn(int __value)
{
    this->meanColumn = __value;
}

int JKQTPBoxplotGraphBase::getMeanColumn() const
{
    return this->meanColumn;
}

void JKQTPBoxplotGraphBase::setMeanColumn(size_t __value) {
    this->meanColumn = static_cast<int>(__value);
}

void JKQTPBoxplotGraphBase::setMinColumn(int __value)
{
    this->minColumn = __value;
}

int JKQTPBoxplotGraphBase::getMinColumn() const
{
    return this->minColumn;
}

void JKQTPBoxplotGraphBase::setMinColumn(size_t __value) {
    this->minColumn = static_cast<int>(__value);
}

void JKQTPBoxplotGraphBase::setMaxColumn(int __value)
{
    this->maxColumn = __value;
}

int JKQTPBoxplotGraphBase::getMaxColumn() const
{
    return this->maxColumn;
}

void JKQTPBoxplotGraphBase::setMaxColumn(size_t __value) {
    this->maxColumn = static_cast<int>(__value);
}

void JKQTPBoxplotGraphBase::setPercentile25Column(int __value)
{
    this->percentile25Column = __value;
}

int JKQTPBoxplotGraphBase::getPercentile25Column() const
{
    return this->percentile25Column;
}

void JKQTPBoxplotGraphBase::setPercentile25Column(size_t __value) {
    this->percentile25Column = static_cast<int>(__value);
}

void JKQTPBoxplotGraphBase::setPercentile75Column(int __value)
{
    this->percentile75Column = __value;
}

int JKQTPBoxplotGraphBase::getPercentile75Column() const
{
    return this->percentile75Column;
}

void JKQTPBoxplotGraphBase::setPercentile75Column(size_t __value) {
    this->percentile75Column = static_cast<int>(__value);
}

int JKQTPBoxplotGraphBase::getMedianConfidenceColumn() const
{
    return medianConfidenceColumn;
}

void JKQTPBoxplotGraphBase::setMedianConfidenceColumn(size_t __value)
{
    medianConfidenceColumn=static_cast<int>(__value);
}

void JKQTPBoxplotGraphBase::setBoxWidthRelative(double __value)
{
    this->boxWidthRelative = __value;
}

double JKQTPBoxplotGraphBase::getBoxWidthRelative() const
{
    return this->boxWidthRelative;
}

void JKQTPBoxplotGraphBase::setUseRelativeBoxWidth(bool __value)
{
    useRelativeBoxWidth=__value;
}

bool JKQTPBoxplotGraphBase::getUseRelativeBoxWidth() const
{
    return useRelativeBoxWidth;
}

QColor JKQTPBoxplotGraphBase::getKeyLabelColor() const {
    return getLineColor();
}

void JKQTPBoxplotGraphBase::setColor(QColor c)
{
    setBoxplotColor(c, getParent());
}



void JKQTPBoxplotGraphBase::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=static_cast<int>(datastore->getRows(static_cast<size_t>(posColumn)));
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPBoxplotGraphBase::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(posColumn,static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());


    }
}


















JKQTPBoxplotElementBase::JKQTPBoxplotElementBase(JKQTBasePlotter* parent):
    JKQTPPlotElement(parent)
{
    pos=JKQTP_NAN;
    median=JKQTP_NAN;
    mean=JKQTP_NAN;
    min=JKQTP_NAN;
    max=JKQTP_NAN;
    drawMean=false;
    drawMinMax=false;
    percentile25=JKQTP_NAN;
    percentile75=JKQTP_NAN;
    medianConfidenceIntervalWidth=JKQTP_NAN;
    drawNotch=false;

    initBoxplotStyle(parent, parentPlotStyle);
    setMeanSymbolType(JKQTPPlus);

}

JKQTPBoxplotElementBase::JKQTPBoxplotElementBase(JKQTPlotter* parent):
    JKQTPBoxplotElementBase(parent->getPlotter())
{
}


void JKQTPBoxplotElementBase::setPos(double __value)
{
    this->pos = __value;
}

double JKQTPBoxplotElementBase::getPos() const
{
    return this->pos;
}

void JKQTPBoxplotElementBase::setMedian(double __value)
{
    if (this->median != __value) {
        this->median = __value;
        drawMedian=true;
    }
}

double JKQTPBoxplotElementBase::getMedian() const
{
    return this->median;
}

void JKQTPBoxplotElementBase::setMedianConfidenceIntervalWidth(double __value)
{
    if (this->medianConfidenceIntervalWidth != __value) {
        this->medianConfidenceIntervalWidth = __value;
        drawNotch=true;
    }
}

double JKQTPBoxplotElementBase::getMedianConfidenceIntervalWidth() const
{
    return this->medianConfidenceIntervalWidth;
}

void JKQTPBoxplotElementBase::setMean(double __value)
{
    if (this->mean != __value) {
        this->mean = __value;
        drawMean=true;
    }
}

double JKQTPBoxplotElementBase::getMean() const
{
    return this->mean;
}

void JKQTPBoxplotElementBase::setMin(double __value)
{
    if (this->min != __value) {
        this->min = __value;
        drawMinMax=true;
    }
}

double JKQTPBoxplotElementBase::getMin() const
{
    return this->min;
}

void JKQTPBoxplotElementBase::setMax(double __value)
{
    if (this->max != __value) {
        this->max = __value;
        drawMinMax=true;
    }
}

double JKQTPBoxplotElementBase::getMax() const
{
    return this->max;
}

void JKQTPBoxplotElementBase::setPercentile25(double __value)
{
    this->percentile25 = __value;
}

double JKQTPBoxplotElementBase::getPercentile25() const
{
    return this->percentile25;
}

void JKQTPBoxplotElementBase::setPercentile75(double __value)
{
    this->percentile75 = __value;
}

void JKQTPBoxplotElementBase::setData(double pos, double min, double q75, double median, double mean, double q25, double max)
{
    this->pos=pos;
    this->min=min;
    this->max=max;
    drawMinMax=true;
    this->median=median;
    drawMedian=true;
    this->mean=mean;
    drawMean=true;
    this->percentile25=q25;
    this->percentile75=q75;
}

double JKQTPBoxplotElementBase::getPercentile75() const
{
    return this->percentile75;
}

void JKQTPBoxplotElementBase::setDrawMean(bool __value)
{
    this->drawMean = __value;
}

bool JKQTPBoxplotElementBase::getDrawMean() const
{
    return this->drawMean;
}

void JKQTPBoxplotElementBase::setDrawMedian(bool __value)
{
    this->drawMedian = __value;
}

bool JKQTPBoxplotElementBase::getDrawMedian() const
{
    return this->drawMedian;
}

void JKQTPBoxplotElementBase::setDrawMinMax(bool __value)
{
    this->drawMinMax = __value;
}

bool JKQTPBoxplotElementBase::getDrawMinMax() const
{
    return this->drawMinMax;
}

void JKQTPBoxplotElementBase::setDrawNotch(bool __value)
{
    drawNotch=__value;
}

bool JKQTPBoxplotElementBase::getDrawNotch() const
{
    return drawNotch;
}


QColor JKQTPBoxplotElementBase::getKeyLabelColor() const {
    return getLineColor();
}

void JKQTPBoxplotElementBase::setColor(QColor c)
{
    setBoxplotColor(c, getParent());
}



