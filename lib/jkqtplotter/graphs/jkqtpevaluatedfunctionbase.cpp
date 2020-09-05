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


