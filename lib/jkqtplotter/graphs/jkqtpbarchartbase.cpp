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



#include "jkqtplotter/graphs/jkqtpbarchartbase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter.h"

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;






JKQTPBarGraphBase::JKQTPBarGraphBase(JKQTBasePlotter* parent):
    JKQTPXYBaselineGraph(parent),
    width(0.9), shift(0),
    m_fillMode(FillMode::SingleFilling),
    m_lineColorDerivationModeForSpecialFill(parent->getCurrentPlotterStyle().graphsStyle.barchartStyle.graphColorDerivationMode),
    rectRadiusAtBaseline(0),rectRadiusAtValue(0)
{
    initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Barchart);
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Barchart);
    m_fillStyleBelow.initFillStyleInvertedColor(this);
    rectRadiusAtBaseline= parent->getCurrentPlotterStyle().graphsStyle.barchartStyle.defaultRectRadiusAtBaseline;
    rectRadiusAtValue= parent->getCurrentPlotterStyle().graphsStyle.barchartStyle.defaultRectRadiusAtValue;
}


JKQTPBarGraphBase::JKQTPBarGraphBase(JKQTPlotter* parent):
    JKQTPBarGraphBase(parent->getPlotter())
{
}

void JKQTPBarGraphBase::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePenForRects(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    //int y=rect.top()+rect.height()/2.0;
    painter.setPen(p);
    painter.setBrush(b);
    painter.drawRect(rect);

}

QColor JKQTPBarGraphBase::getKeyLabelColor() const {
    return getFillColor();
}


void JKQTPBarGraphBase::autoscaleBarWidthAndShift(double maxWidth, double shrinkFactor)
{
    if (parent) {
        double cntH=0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPPlotElement* g=parent->getGraph(i);
            JKQTPBarGraphBase* gb=qobject_cast<JKQTPBarGraphBase*>(g);
            if (gb && considerForAutoscaling(gb)) {
                cntH++;
            }

        }

        double widthH=1.0/cntH*maxWidth*shrinkFactor;
        double dH=maxWidth/(cntH);
        double h=0.1+dH/2.0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPPlotElement* g=parent->getGraph(i);
            JKQTPBarGraphBase* gb=qobject_cast<JKQTPBarGraphBase*>(g);
            if (gb && considerForAutoscaling(gb)) {
                if (cntH>1) {
                    gb->width=widthH;
                    gb->shift=h-0.5;
                    h=h+dH;
                } else {
                    gb->width=maxWidth;
                    gb->shift=0.0;
                }
            }

        }
    }
}

void JKQTPBarGraphBase::autoscaleBarWidthAndShiftSeparatedGroups(double groupWidth) {
    autoscaleBarWidthAndShift(groupWidth, 1);
}



void JKQTPBarGraphBase::setColor(QColor c)
{
    setFillColor(c);
    setLineColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.barchartStyle.graphColorDerivationMode, c));
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
    m_fillStyleBelow.initFillStyleInvertedColor(this);
}

void JKQTPBarGraphBase::setBarPositionColumn(int column)
{
    setKeyColumn(column);
}

void JKQTPBarGraphBase::setBarPositionColumn(size_t column)
{
    setKeyColumn(static_cast<int>(column));
}

void JKQTPBarGraphBase::setBarHeightColumn(int column)
{
    setValueColumn(column);
}

void JKQTPBarGraphBase::setBarHeightColumn(size_t column)
{
    setValueColumn(static_cast<int>(column));
}

JKQTPBarGraphBase::FillBrushFunctor JKQTPBarGraphBase::constructFillBrushFunctor() const
{
    if (m_fillMode==FillMode::FunctorFilling) return m_fillBrushFunctor;
    if (m_fillMode==FillMode::TwoColorFilling) return [](double , double value, JKQTPEnhancedPainter &painter, JKQTPBarGraphBase* graph) {
        if (value<graph->getBaseline()) return graph->fillStyleBelow().getFillBrush(painter, graph->getParent());
        return graph->getFillBrush(painter, graph->getParent());
    };
    else return [](double, double, JKQTPEnhancedPainter &painter, JKQTPBarGraphBase* graph) {
        return graph->getFillBrush(painter, graph->getParent());
    };
}

void JKQTPBarGraphBase::setShift(double __value)
{
    this->shift = __value;
}

double JKQTPBarGraphBase::getShift() const
{
    return this->shift;
}

void JKQTPBarGraphBase::setWidth(double __value)
{
    this->width = __value;
}

double JKQTPBarGraphBase::getWidth() const
{
    return this->width;
}

void JKQTPBarGraphBase::setFillColor_and_darkenedColor(QColor fill, int colorDarker)
{
    setFillColor(fill);
    setLineColor(fill.darker(colorDarker));
}

int JKQTPBarGraphBase::getBarPositionColumn() const
{
    return getKeyColumn();
}

int JKQTPBarGraphBase::getBarHeightColumn() const
{
    return getValueColumn();
}

JKQTPGraphFillStyleMixin &JKQTPBarGraphBase::fillStyleBelow()
{
    return m_fillStyleBelow;
}

const JKQTPGraphFillStyleMixin &JKQTPBarGraphBase::fillStyleBelow() const
{
    return m_fillStyleBelow;
}

JKQTPBarGraphBase::FillMode JKQTPBarGraphBase::getFillMode() const
{
    return m_fillMode;
}

double JKQTPBarGraphBase::getRectRadiusAtValue() const
{
    return rectRadiusAtValue;
}

double JKQTPBarGraphBase::getRectRadiusAtBaseline() const
{
    return rectRadiusAtBaseline;
}

JKQTPBarGraphBase::FillBrushFunctor &JKQTPBarGraphBase::getFillBrushFunctor() {
    return m_fillBrushFunctor;
}

const JKQTPBarGraphBase::FillBrushFunctor &JKQTPBarGraphBase::getFillBrushFunctor() const {
    return m_fillBrushFunctor;
}

void JKQTPBarGraphBase::setFillMode(FillMode mode)
{
    m_fillMode=mode;
}

void JKQTPBarGraphBase::setFillBrushFunctor(const FillBrushFunctor &f) {
    m_fillBrushFunctor=f;
}

void JKQTPBarGraphBase::setFillBrushFunctor(FillBrushFunctor &&f) {
    m_fillBrushFunctor=std::forward<FillBrushFunctor>(f);
}

void JKQTPBarGraphBase::setFillBrushFunctor(const SimpleFillBrushFunctor &f) {
    m_fillBrushFunctor=SimpleFillBrushFunctorAdaptor(f);
}

void JKQTPBarGraphBase::setFillBrushFunctor(SimpleFillBrushFunctor &&f) {
    m_fillBrushFunctor=SimpleFillBrushFunctorAdaptor(std::forward<SimpleFillBrushFunctor>(f));
}

double JKQTPBarGraphBase::getParentStackedMax(int /*index*/) const
{
    return getBaseline();
}

bool JKQTPBarGraphBase::hasStackParent() const
{
    return false;
}

bool JKQTPBarGraphBase::getValuesMinMax(double &mmin, double &mmax, double &smallestGreaterZero)
{
    mmin=0;
    mmax=0;
    smallestGreaterZero=0;
    if (getBaseline()>0) {
        smallestGreaterZero=getBaseline();
        mmin=getBaseline();
        mmax=getBaseline();
    }

    if (getBarPositionColumn()<0 || getBarHeightColumn()<0) return false;

    const size_t datacol=static_cast<size_t>(getBarHeightColumn());

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0, imax=0;
    if (getIndexRange(imin, imax)) {


        for (int i=imin; i<imax; i++) {
            double stack=0;
            double yv=getBaseline();
            const double boxstart=getParentStackedMax(i);
            if (hasStackParent()) {
                stack=boxstart;
                yv=stack;
            }
            if (JKQTPIsOKFloat(yv)) {
                if (yv>mmax) mmax=yv;
                if (yv<mmin) mmin=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            }
            yv=stack+datastore->get(datacol,static_cast<size_t>(i));
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

bool JKQTPBarGraphBase::getPositionsMinMax(double &mmin, double &mmax, double &smallestGreaterZero)
{
    bool start=true;
    mmin=0;
    mmax=0;
    smallestGreaterZero=0;

    if (getBarPositionColumn()<0 || getBarHeightColumn()<0) return false;

    const size_t poscol=static_cast<size_t>(getBarPositionColumn());

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0, imax=0;
    if (getIndexRange(imin, imax)) {
        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(poscol,static_cast<size_t>(i));
            int sr=datastore->getNextLowerIndex(poscol, i);
            int lr=datastore->getNextHigherIndex(poscol, i);
            double delta, deltap, deltam;

            if (sr<0 && lr<0) { // only one x-value
                deltam=0.5;
                deltap=0.5;
            } else if (lr<0) { // the right-most x-value
                deltap=deltam=fabs(xv-datastore->get(poscol,sr))/2.0;
            } else if (sr<0) { // the left-most x-value
                deltam=deltap=fabs(datastore->get(poscol,lr)-xv)/2.0;
            } else {
                deltam=fabs(xv-datastore->get(poscol,sr))/2.0;
                deltap=fabs(datastore->get(poscol,lr)-xv)/2.0;
            }
            delta=deltap+deltam;

            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(delta) ) {

                if (start || xv+shift*delta+width*delta/2.0>mmax) mmax=xv+shift*delta+width*delta/2.0;
                if (start || xv+shift*delta-width*delta/2.0<mmin) mmin=xv+shift*delta-width*delta/2.0;
                double xvsgz;
                xvsgz=xv+shift*delta+width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
                xvsgz=xv+shift*delta-width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
    return false;
}


void JKQTPBarGraphBase::setRectRadiusAtValue(double __value)
{
    rectRadiusAtValue=__value;
}

void JKQTPBarGraphBase::setRectRadiusAtBaseline(double __value)
{
    rectRadiusAtBaseline=__value;
}

void JKQTPBarGraphBase::setRectRadius(double all)
{
    setRectRadiusAtValue(all);
    setRectRadiusAtBaseline(all);
}

void JKQTPBarGraphBase::setRectRadius(double atValue, double atBaseline)
{
    setRectRadiusAtValue(atValue);
    setRectRadiusAtBaseline(atBaseline);
}

void JKQTPBarGraphBase::setLineColorDerivationModeForSpecialFill(const JKQTPColorDerivationMode &m)
{
    m_lineColorDerivationModeForSpecialFill=m;
}

JKQTPColorDerivationMode JKQTPBarGraphBase::getLineColorDerivationModeForSpecialFill() const
{
    return m_lineColorDerivationModeForSpecialFill;
}
