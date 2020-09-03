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



#include "jkqtplotter/graphs/jkqtpviolinplot.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;










JKQTPViolinplotVerticalElement::JKQTPViolinplotVerticalElement(JKQTBasePlotter* parent):
    JKQTPPlotObject(DrawAsGraphicElement, parent)
{
    pos=JKQTP_NAN;
    median=JKQTP_NAN;
    mean=JKQTP_NAN;
    min=JKQTP_NAN;
    max=JKQTP_NAN;
    drawMean=false;
    drawMinMax=false;
    violinPositionColumn=-1;
    violinFrequencyColumn=-1;

    initViolinplotStyle(parent, parentPlotStyle);
    setMeanSymbolType(JKQTPPlus);

}

JKQTPViolinplotVerticalElement::JKQTPViolinplotVerticalElement(JKQTPlotter* parent):
    JKQTPViolinplotVerticalElement(parent->getPlotter())
{
}

void JKQTPViolinplotVerticalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPViolinplotVerticalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        const double xv=pos;
        const double minv=min;
        const double maxv=max;
        const double medianv=median;
        const double meanv=mean;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) ) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            const double x=transformX(xv);
            const double min=transformY(minv);
            const double max=transformY(maxv);
            const double median=transformY(medianv);
            const double mean=transformY(meanv);

            // collect single-value labels for hitTest()-data at the bottom of this loop!
            QStringList labelValues, labelNames;
            int labMedian=-1, labMean=-1, labMin=-1, labMax=-1;
            labelNames<<"pos";
            labelValues<<jkqtp_floattolatexqstr(xv, 3);
            if (JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<jkqtp_floattolatexqstr(minv, 3); labMin=labelValues.size()-1; }
            if (JKQTPIsOKFloat(medianv)) {
                labelNames<<"\\median"; labelValues<<jkqtp_floattolatexqstr(medianv, 3); labMedian=labelValues.size()-1;
            }
            if (JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<jkqtp_floattolatexqstr(meanv, 3); labMean=labelValues.size()-1; }
            if (JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<jkqtp_floattolatexqstr(maxv, 3); labMax=labelValues.size()-1; }

            double w=parent->pt2px(painter,getViolinWidthAbsolute());
            double xma=x+w/2.0;
            double xmi=x-w/2.0;

            QVector<double> violPos;
            if (violinPositionColumn>=0) violPos=transformY(getParent()->getDatastore()->getData(static_cast<size_t>(violinPositionColumn)));
            QVector<double> violFreq;
            if (violinFrequencyColumn>=0) violFreq=getParent()->getDatastore()->getData(static_cast<size_t>(violinFrequencyColumn));

            plotVerticalViolinplot(parent, painter, x, xmi, xma, violPos, violFreq, (drawMinMax)?min:JKQTP_NAN, (drawMedian)?median:JKQTP_NAN, (drawMinMax)?max:JKQTP_NAN, (drawMean)?mean:JKQTP_NAN);

        
            if (drawMedian && JKQTPIsOKFloat(medianv)) {
                if (JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(xv, medianv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
            if (drawMinMax) {
                if (JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(xv, minv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(xv, maxv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        

            if (drawMean && JKQTPIsOKFloat(meanv)) {
                if (JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(xv, meanv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }


        }
    }

    //drawErrorsAfter(painter);
}


bool JKQTPViolinplotVerticalElement::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    double xv=pos;
    double x=transformX(xv);
    double xma=backtransformX(x-parent->pt2px(getViolinWidthAbsolute(), 72));
    double xmi=backtransformX(x+parent->pt2px(getViolinWidthAbsolute(), 72));
    if (xmi>xma) qSwap(xmi,xma);
    maxx=xma;
    minx=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPViolinplotVerticalElement::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false ;


        if (drawMedian) {
            maxy=median;
            miny=median;
        }
        if (min>maxy) maxy=min;
        if (drawMinMax && (min<miny)) miny=min;
        if (drawMinMax && (max>maxy)) maxy=max;
        if (drawMinMax && (max<miny)) miny=max;

        double xvsgz;
        xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
        return true;
}

void JKQTPViolinplotVerticalElement::setPos(double __value)
{
    this->pos = __value;
}

double JKQTPViolinplotVerticalElement::getPos() const
{
    return this->pos;
}

void JKQTPViolinplotVerticalElement::setMedian(double __value)
{
    if (this->median != __value) {
        this->median = __value;
        drawMedian=true;
    }
}

double JKQTPViolinplotVerticalElement::getMedian() const
{
    return this->median;
}


void JKQTPViolinplotVerticalElement::setMean(double __value)
{
    if (this->mean != __value) {
        this->mean = __value;
        drawMean=true;
    }
}

double JKQTPViolinplotVerticalElement::getMean() const
{
    return this->mean;
}

void JKQTPViolinplotVerticalElement::setMin(double __value)
{
    if (this->min != __value) {
        this->min = __value;
        drawMinMax=true;
    }
}

double JKQTPViolinplotVerticalElement::getMin() const
{
    return this->min;
}

void JKQTPViolinplotVerticalElement::setMax(double __value)
{
    if (this->max != __value) {
        this->max = __value;
        drawMinMax=true;
    }
}

double JKQTPViolinplotVerticalElement::getMax() const
{
    return this->max;
}


void JKQTPViolinplotVerticalElement::setDrawMean(bool __value)
{
    this->drawMean = __value;
}

bool JKQTPViolinplotVerticalElement::getDrawMean() const
{
    return this->drawMean;
}

void JKQTPViolinplotVerticalElement::setDrawMedian(bool __value)
{
    this->drawMedian = __value;
}

bool JKQTPViolinplotVerticalElement::getDrawMedian() const
{
    return this->drawMedian;
}

void JKQTPViolinplotVerticalElement::setDrawMinMax(bool __value)
{
    this->drawMinMax = __value;
}

bool JKQTPViolinplotVerticalElement::getDrawMinMax() const
{
    return this->drawMinMax;
}

void JKQTPViolinplotVerticalElement::setViolinPositionColumn(int __value)
{
    violinPositionColumn=__value;
}

void JKQTPViolinplotVerticalElement::setViolinPositionColumn(size_t __value)
{
    violinPositionColumn=static_cast<int>(__value);
}

int JKQTPViolinplotVerticalElement::getViolinPositionColumn() const
{
    return violinPositionColumn;
}

void JKQTPViolinplotVerticalElement::setViolinFrequencyColumn(int __value)
{
    violinFrequencyColumn=__value;
}

void JKQTPViolinplotVerticalElement::setViolinFrequencyColumn(size_t __value)
{
    violinFrequencyColumn=static_cast<int>(__value);
}

int JKQTPViolinplotVerticalElement::getViolinFrequencyColumn() const
{
    return violinFrequencyColumn;
}


void JKQTPViolinplotVerticalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    plotVerticalKeyMarker(parent, painter, rect);
}

QColor JKQTPViolinplotVerticalElement::getKeyLabelColor() const {
    return getLineColor();
}

void JKQTPViolinplotVerticalElement::setColor(QColor c)
{
    setViolinplotColor(c, getParent());
}




void JKQTPViolinplotHorizontalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    plotHorizontalKeyMarker(parent, painter, rect);
}

bool JKQTPViolinplotHorizontalElement::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;


    if (drawMedian) {
        maxx=median;
        minx=median;
    }
    if (min>maxx) maxx=min;
    if (drawMinMax && (min<minx)) minx=min;
    if (drawMinMax && (max>maxx)) maxx=max;
    if (drawMinMax && (max<minx)) minx=max;

    double xvsgz;
    xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPViolinplotHorizontalElement::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    double xv=pos;
    double x=transformY(xv);
    double xma=backtransformY(x-parent->pt2px(getViolinWidthAbsolute(), 72));
    double xmi=backtransformY(x+parent->pt2px(getViolinWidthAbsolute(), 72));
    if (xmi>xma) qSwap(xmi,xma);
    maxy=xma;
    miny=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;

}

JKQTPViolinplotHorizontalElement::JKQTPViolinplotHorizontalElement(JKQTBasePlotter *parent):
    JKQTPViolinplotVerticalElement(parent)
{
}

JKQTPViolinplotHorizontalElement::JKQTPViolinplotHorizontalElement(JKQTPlotter *parent):
    JKQTPViolinplotHorizontalElement(parent->getPlotter())
{
}

void JKQTPViolinplotHorizontalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPViolinplotHorizontalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        const double yv=pos;
        const double minv=min;
        const double maxv=max;
        const double medianv=median;
        const double meanv=mean;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(yv) ) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            const double y=transformY(yv);
            const double min=transformX(minv);
            const double max=transformX(maxv);
            const double median=transformX(medianv);
            const double mean=transformX(meanv);

            // collect single-value labels for hitTest()-data at the bottom of this loop!
            QStringList labelValues, labelNames;
            int labMedian=-1, labMean=-1, labMin=-1, labMax=-1;
            labelNames<<"pos";
            labelValues<<jkqtp_floattolatexqstr(yv, 3);
            if (JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<jkqtp_floattolatexqstr(minv, 3); labMin=labelValues.size()-1; }
            if (JKQTPIsOKFloat(medianv)) {
                labelNames<<"\\median"; labelValues<<jkqtp_floattolatexqstr(medianv, 3); labMedian=labelValues.size()-1;
            }
            if (JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<jkqtp_floattolatexqstr(meanv, 3); labMean=labelValues.size()-1; }
            if (JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<jkqtp_floattolatexqstr(maxv, 3); labMax=labelValues.size()-1; }

            double w=parent->pt2px(painter,getViolinWidthAbsolute());
            double yma=y+w/2.0;
            double ymi=y-w/2.0;

            QVector<double> violPos;
            if (violinPositionColumn>=0) violPos=transformX(getParent()->getDatastore()->getData(static_cast<size_t>(violinPositionColumn)));
            QVector<double> violFreq;
            if (violinFrequencyColumn>=0) violFreq=getParent()->getDatastore()->getData(static_cast<size_t>(violinFrequencyColumn));

            plotHorizontalViolinplot(parent, painter, y, ymi, yma, violPos, violFreq, (drawMinMax)?min:JKQTP_NAN, (drawMedian)?median:JKQTP_NAN, (drawMinMax)?max:JKQTP_NAN, (drawMean)?mean:JKQTP_NAN);

        
            if (drawMedian && JKQTPIsOKFloat(medianv)) {
                 if (JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(medianv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }            }
            if (drawMinMax) {
                if (JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(minv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(maxv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        

            if (drawMean && JKQTPIsOKFloat(meanv)) {
                if (JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(meanv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }

        }
    }

    //drawErrorsAfter(painter);
}




