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



#include "jkqtplotter/graphs/jkqtpboxplot.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;




JKQTPBoxplotVerticalGraph::JKQTPBoxplotVerticalGraph(JKQTBasePlotter* parent):
    JKQTPBoxplotGraphBase(parent)
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


JKQTPBoxplotVerticalGraph::JKQTPBoxplotVerticalGraph(JKQTPlotter* parent):
    JKQTPBoxplotVerticalGraph(parent->getPlotter())
{

}


void JKQTPBoxplotVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotVerticalGraph::draw");
#endif
    clearHitTestData();
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;    

    drawErrorsBefore(painter);

    int imax=static_cast<int>(datastore->getRows(static_cast<size_t>(posColumn)));
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    reserveHitTestData(imax-imin);


    //bool first=false;

    // 1. step find width of boxplots:
    double boxwidth_real=0;
    bool bwfirst=true;
    intSortData();
    for (int iii=imin+1; iii<imax; iii++) {
        int i=qBound(imin+1, getDataIndex(iii), imax);
        double xv0=transformX(datastore->get(static_cast<size_t>(posColumn),static_cast<size_t>(i-1)));
        double xv=transformX(datastore->get(static_cast<size_t>(posColumn),static_cast<size_t>(i)));
        if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xv0)) {
            if (bwfirst) {
                boxwidth_real=fabs(xv-xv0);
                bwfirst=false;
            } else {
                if (fabs(xv-xv0)>0) boxwidth_real=qMin(fabs(xv-xv0), boxwidth_real);
            }
        }
    }

    // 2. plot:
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        for (int i=imin; i<imax; i++) {
            const double xv=datastore->get(posColumn,static_cast<size_t>(i));
            const double p25v=datastore->get(percentile25Column,static_cast<size_t>(i));
            const double p75v=datastore->get(percentile75Column,static_cast<size_t>(i));
            const double minv=datastore->get(minColumn,static_cast<size_t>(i));
            const double maxv=datastore->get(maxColumn,static_cast<size_t>(i));
            const double medianv=datastore->get(medianColumn,static_cast<size_t>(i));
            const double medConf=datastore->get(medianConfidenceColumn,static_cast<size_t>(i));
            const double meanv=datastore->get(meanColumn,static_cast<size_t>(i));

            if (posColumn>=0 && JKQTPIsOKFloat(xv) ) {

                // collect single-value labels for hitTest()-data at the bottom of this loop!
                QStringList labelValues, labelNames;
                int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
                labelNames<<"pos";
                labelValues<<jkqtp_floattolatexqstr(xv, 3);
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<jkqtp_floattolatexqstr(minv, 3); labMin=labelValues.size()-1; }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<jkqtp_floattolatexqstr(p25v, 3); labQ25=labelValues.size()-1; }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) {
                    if (medianConfidenceColumn>=0 && JKQTPIsOKFloat(medConf)) {
                        labelNames<<"\\median"; labelValues<<(jkqtp_floattolatexqstr(medianv, 3)+"\\:{\\pm}\\:"+jkqtp_floattolatexqstr(medConf, 3)); labMedian=labelValues.size()-1;
                    } else {
                        labelNames<<"\\median"; labelValues<<jkqtp_floattolatexqstr(medianv, 3); labMedian=labelValues.size()-1;
                    }
                }
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<jkqtp_floattolatexqstr(meanv, 3); labMean=labelValues.size()-1; }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<jkqtp_floattolatexqstr(p75v, 3); labQ75=labelValues.size()-1; }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<jkqtp_floattolatexqstr(maxv, 3); labMax=labelValues.size()-1; }

                const double x=transformX(xv);
                const double p25=transformY(p25v);
                const double p75=transformY(p75v);
                const double min=transformY(minv);
                const double max=transformY(maxv);
                const double median=transformY(medianv);
                const double medianu=transformY(medianv-medConf/2.0);
                const double mediano=transformY(medianv+medConf/2.0);
                const double mean=transformY(meanv);


                double w=(useRelativeBoxWidth && boxwidth_real>0)?(boxwidth_real*getBoxWidthRelative()):parent->pt2px(painter,getBoxWidthAbsolute());
                double xma=x+w/2.0;
                double xmi=x-w/2.0;

                plotVerticalBoxplot(parent, painter, x, xmi, xma, min, p25, median, p75, max, mean, medianu, mediano);


                // add hit-test graph points
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(xv, meanv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(xv, medianv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(xv, minv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(xv, maxv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(xv, p25v, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(xv, p75v, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                //first=true;
            }
        }
    }


    drawErrorsAfter(painter);
}


bool JKQTPBoxplotVerticalGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (posColumn<0) return false;
    if (parent==nullptr) return false;

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

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv)) {
            double xn=xv+1;
            if (i+1<imax) xn=datastore->get(posColumn,i+1);
            else if (i-1>=0) xn=datastore->get(posColumn,i-1);
            else xn=xv+1;
            double delta=fabs(xn-xv);
            double w=delta*getBoxWidthRelative();
            double xma=xv+w;
            double xmi=xv-w;
            if (start || xma>maxx) maxx=xma;
            if (start || xmi<minx) minx=xmi;
            double xvsgz;
            xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPBoxplotVerticalGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (percentile25Column<0 && percentile75Column<0 && medianColumn<0 && minColumn<0 && maxColumn<0) return false;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax= static_cast<int>(datastore->getRows(medianColumn));
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double p25=datastore->get(percentile25Column,i);
        double p75=datastore->get(percentile75Column,i);
        double min=datastore->get(minColumn,i);
        double max=datastore->get(maxColumn,i);
        double median=datastore->get(medianColumn,i);
        if (JKQTPIsOKFloat(median)) {
            if (start || median>maxy) maxy=median;
            if (start || median<miny) miny=median;
            if (JKQTPIsOKFloat(p25) &&p25>maxy) maxy=p25;
            if (JKQTPIsOKFloat(p25) &&p25<miny) miny=p25;
            if (JKQTPIsOKFloat(p75) &&p75>maxy) maxy=p75;
            if (JKQTPIsOKFloat(p75) &&p75<miny) miny=p75;
            if (JKQTPIsOKFloat(min) &&min>maxy) maxy=min;
            if (JKQTPIsOKFloat(min) &&min<miny) miny=min;
            if (JKQTPIsOKFloat(max) &&max>maxy) maxy=max;
            if (JKQTPIsOKFloat(max) &&max<miny) miny=max;

            double xvsgz;
            xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

void JKQTPBoxplotVerticalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    plotVerticalKeyMarker(parent, painter, rect);
}




JKQTPBoxplotHorizontalGraph::JKQTPBoxplotHorizontalGraph(JKQTBasePlotter *parent):
    JKQTPBoxplotGraphBase(parent)
{
}

JKQTPBoxplotHorizontalGraph::JKQTPBoxplotHorizontalGraph(JKQTPlotter *parent):
    JKQTPBoxplotHorizontalGraph(parent->getPlotter())
{
}
void JKQTPBoxplotHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    plotHorizontalKeyMarker(parent, painter, rect);
    
}

bool JKQTPBoxplotHorizontalGraph::getXMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (percentile25Column<0 && percentile75Column<0 && medianColumn<0 && minColumn<0 && maxColumn<0) return false;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax= static_cast<int>(datastore->getRows(medianColumn));
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double p25=datastore->get(percentile25Column,i);
        double p75=datastore->get(percentile75Column,i);
        double min=datastore->get(minColumn,i);
        double max=datastore->get(maxColumn,i);
        double median=datastore->get(medianColumn,i);
        if (JKQTPIsOKFloat(median)) {
            if (start || median>maxy) maxy=median;
            if (start || median<miny) miny=median;
            if (JKQTPIsOKFloat(p25)&&p25>maxy) maxy=p25;
            if (JKQTPIsOKFloat(p25)&&p25<miny) miny=p25;
            if (JKQTPIsOKFloat(p75)&&p75>maxy) maxy=p75;
            if (JKQTPIsOKFloat(p75)&&p75<miny) miny=p75;
            if (JKQTPIsOKFloat(min)&&min>maxy) maxy=min;
            if (JKQTPIsOKFloat(min)&&min<miny) miny=min;
            if (JKQTPIsOKFloat(max)&&max>maxy) maxy=max;
            if (JKQTPIsOKFloat(max)&&max<miny) miny=max;
            double xvsgz;
            xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPBoxplotHorizontalGraph::getYMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (posColumn<0) return false;
    if (parent==nullptr) return false;

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

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,static_cast<size_t>(i));
        double xn=xv+1;
        if (i+1<imax) xn=datastore->get(posColumn,i+1);
        else if (i-1>=0) xn=datastore->get(posColumn,i-1);
        else xn=xv+1;
        double delta=fabs(xn-xv);
        double w=delta*getBoxWidthRelative();
        double xma=xv+w;
        double xmi=xv-w;
        if (JKQTPIsOKFloat(xma) && JKQTPIsOKFloat(xmi) ) {
            if (start || xma>maxx) maxx=xma;
            if (start || xmi<minx) minx=xmi;
            double xvsgz;
            xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}


void JKQTPBoxplotHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotHorizontalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);


    int imax=static_cast<int>(datastore->getRows(static_cast<size_t>(posColumn)));
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    // 1. step find width of boxplots:
    double boxwidth_real=0;
    bool bwfirst=true;
    intSortData();
    for (int iii=imin+1; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv0=transformY(datastore->get(posColumn,i-1));
        double xv=transformY(datastore->get(posColumn,i));
        if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xv0)) {
            if (bwfirst) {
                boxwidth_real=fabs(xv-xv0);
                bwfirst=false;
            } else {
                if (fabs(xv-xv0)>0) boxwidth_real=qMin(fabs(xv-xv0), boxwidth_real);
            }
        }
    }
    // 2. plot:
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        //bool first=false;
        for (int i=imin; i<imax; i++) {
            const double yv=datastore->get(posColumn,static_cast<size_t>(i));
            const double p25v=datastore->get(percentile25Column,static_cast<size_t>(i));
            const double p75v=datastore->get(percentile75Column,static_cast<size_t>(i));
            const double minv=datastore->get(minColumn,static_cast<size_t>(i));
            const double maxv=datastore->get(maxColumn,static_cast<size_t>(i));
            const double medianv=datastore->get(medianColumn,static_cast<size_t>(i));
            const double meanv=datastore->get(meanColumn,static_cast<size_t>(i));
            const double medConf=datastore->get(medianConfidenceColumn,static_cast<size_t>(i));

            if (posColumn>=0 && JKQTPIsOKFloat(yv) ) {


                // collect single-value labels for hitTest()-data at the bottom of this loop!
                QStringList labelValues, labelNames;
                int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
                labelNames<<"pos";
                labelValues<<jkqtp_floattolatexqstr(yv, 3);
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<jkqtp_floattolatexqstr(minv, 3); labMin=labelValues.size()-1; }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<jkqtp_floattolatexqstr(p25v, 3); labQ25=labelValues.size()-1; }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) {
                    if (medianConfidenceColumn>=0 && JKQTPIsOKFloat(medConf)) {
                        labelNames<<"\\median"; labelValues<<(jkqtp_floattolatexqstr(medianv, 3)+"\\:{\\pm}\\:"+jkqtp_floattolatexqstr(medConf, 3)); labMedian=labelValues.size()-1;
                    } else {
                        labelNames<<"\\median"; labelValues<<jkqtp_floattolatexqstr(medianv, 3); labMedian=labelValues.size()-1;
                    }
                }
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<jkqtp_floattolatexqstr(meanv, 3); labMean=labelValues.size()-1; }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<jkqtp_floattolatexqstr(p75v, 3); labQ75=labelValues.size()-1; }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<jkqtp_floattolatexqstr(maxv, 3); labMax=labelValues.size()-1; }



                const double y=transformY(yv);
                const double p25=transformX(p25v);
                const double p75=transformX(p75v);
                const double min=transformX(minv);
                const double max=transformX(maxv);
                const double median=transformX(medianv);
                const double medianu=transformX(medianv-medConf/2.0);
                const double mediano=transformX(medianv+medConf/2.0);
                const double mean=transformX(meanv);


                double w=(useRelativeBoxWidth && boxwidth_real>0)?(boxwidth_real*getBoxWidthRelative()):parent->pt2px(painter,getBoxWidthAbsolute());
                double yma=y+w/2.0;
                double ymi=y-w/2.0;


                plotHorizontalBoxplot(parent, painter, y, ymi, yma, min, p25, median, p75, max, mean, medianu, mediano);


                // add hit-test graph points
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(meanv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(medianv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(minv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(maxv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(p25v, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(p75v, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }

        }

    }

    drawErrorsAfter(painter);
}





















JKQTPBoxplotVerticalElement::JKQTPBoxplotVerticalElement(JKQTBasePlotter* parent):
    JKQTPBoxplotElementBase(parent)
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

JKQTPBoxplotVerticalElement::JKQTPBoxplotVerticalElement(JKQTPlotter* parent):
    JKQTPBoxplotVerticalElement(parent->getPlotter())
{
}

void JKQTPBoxplotVerticalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotVerticalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        const double xv=pos;
        const double p25v=percentile25;
        const double p75v=percentile75;
        const double minv=min;
        const double maxv=max;
        const double medianv=median;
        const double medConf=medianConfidenceIntervalWidth;
        const double meanv=mean;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) ) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            const double x=transformX(xv);
            const double p25=transformY(p25v);
            const double p75=transformY(p75v);
            const double min=transformY(minv);
            const double max=transformY(maxv);
            const double median=transformY(medianv);
            const double medianu=transformY(medianv-medConf/2.0);
            const double mediano=transformY(medianv+medConf/2.0);
            const double mean=transformY(meanv);

            // collect single-value labels for hitTest()-data at the bottom of this loop!
            QStringList labelValues, labelNames;
            int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
            labelNames<<"pos";
            labelValues<<jkqtp_floattolatexqstr(xv, 3);
            if (JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<jkqtp_floattolatexqstr(minv, 3); labMin=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<jkqtp_floattolatexqstr(p25v, 3); labQ25=labelValues.size()-1; }
            if (JKQTPIsOKFloat(medianv)) {
                if (JKQTPIsOKFloat(medConf)) {
                    labelNames<<"\\median"; labelValues<<(jkqtp_floattolatexqstr(medianv, 3)+"\\:{\\pm}\\:"+jkqtp_floattolatexqstr(medConf, 3)); labMedian=labelValues.size()-1;
                } else {
                    labelNames<<"\\median"; labelValues<<jkqtp_floattolatexqstr(medianv, 3); labMedian=labelValues.size()-1;
                }
            }
            if (JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<jkqtp_floattolatexqstr(meanv, 3); labMean=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<jkqtp_floattolatexqstr(p75v, 3); labQ75=labelValues.size()-1; }
            if (JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<jkqtp_floattolatexqstr(maxv, 3); labMax=labelValues.size()-1; }

            double w=parent->pt2px(painter,getBoxWidthAbsolute());
            double xma=x+w/2.0;
            double xmi=x-w/2.0;

            plotVerticalBoxplot(parent, painter, x, xmi, xma, (drawMinMax)?min:JKQTP_NAN, p25, (drawMedian)?median:JKQTP_NAN, p75, (drawMinMax)?max:JKQTP_NAN, (drawMean)?mean:JKQTP_NAN, (drawMedian&&drawNotch)?medianu:JKQTP_NAN, (drawMedian&&drawNotch)?mediano:JKQTP_NAN);

            if (JKQTPIsOKFloat(p25v) && JKQTPIsOKFloat(p75v)) {
                if (JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(xv, p25v, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(xv, p75v, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        
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


bool JKQTPBoxplotVerticalElement::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    double xv=pos;
    double w=0;
    double xma=xv+w;
    double xmi=xv-w;
    maxx=xma;
    minx=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPBoxplotVerticalElement::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false ;


        double p25=percentile25;
        double p75=percentile75;
        if (drawMedian) {
            maxy=median;
            miny=median;
        } else {
            maxy=p75;
            miny=p25;
        }
        if (p25>maxy) maxy=p25;
        if (p25<miny) miny=p25;
        if (p75>maxy) maxy=p75;
        if (p75<miny) miny=p75;
        if (min>maxy) maxy=min;
        if (drawMinMax && (min<miny)) miny=min;
        if (drawMinMax && (max>maxy)) maxy=max;
        if (drawMinMax && (max<miny)) miny=max;

        double xvsgz;
        xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
        return true;
}

void JKQTPBoxplotVerticalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    plotVerticalKeyMarker(parent, painter, rect);
}





JKQTPBoxplotHorizontalElement::JKQTPBoxplotHorizontalElement(JKQTBasePlotter *parent):
    JKQTPBoxplotElementBase(parent)
{
}

JKQTPBoxplotHorizontalElement::JKQTPBoxplotHorizontalElement(JKQTPlotter *parent):
    JKQTPBoxplotHorizontalElement(parent->getPlotter())
{
}

void JKQTPBoxplotHorizontalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    plotHorizontalKeyMarker(parent, painter, rect);
}

bool JKQTPBoxplotHorizontalElement::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;


    double p25=percentile25;
    double p75=percentile75;
    if (drawMedian) {
        maxx=median;
        minx=median;
    } else {
        maxx=p75;
        minx=p25;
    }
    if (p25>maxx) maxx=p25;
    if (p25<minx) minx=p25;
    if (p75>maxx) maxx=p75;
    if (p75<minx) minx=p75;
    if (min>maxx) maxx=min;
    if (drawMinMax && (min<minx)) minx=min;
    if (drawMinMax && (max>maxx)) maxx=max;
    if (drawMinMax && (max<minx)) minx=max;

    double xvsgz;
    xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPBoxplotHorizontalElement::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    double xv=pos;
    double w=0;
    double xma=xv+w;
    double xmi=xv-w;
    maxy=xma;
    miny=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;

}

void JKQTPBoxplotHorizontalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotHorizontalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        const double yv=pos;
        const double p25v=percentile25;
        const double p75v=percentile75;
        const double minv=min;
        const double maxv=max;
        const double medianv=median;
        const double medConf=medianConfidenceIntervalWidth;
        const double meanv=mean;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(yv) ) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            const double y=transformY(yv);
            const double p25=transformX(p25v);
            const double p75=transformX(p75v);
            const double min=transformX(minv);
            const double max=transformX(maxv);
            const double median=transformX(medianv);
            const double medianu=transformX(medianv-medConf/2.0);
            const double mediano=transformX(medianv+medConf/2.0);
            const double mean=transformX(meanv);

            // collect single-value labels for hitTest()-data at the bottom of this loop!
            QStringList labelValues, labelNames;
            int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
            labelNames<<"pos";
            labelValues<<jkqtp_floattolatexqstr(yv, 3);
            if (JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<jkqtp_floattolatexqstr(minv, 3); labMin=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<jkqtp_floattolatexqstr(p25v, 3); labQ25=labelValues.size()-1; }
            if (JKQTPIsOKFloat(medianv)) {
                if (JKQTPIsOKFloat(medConf)) {
                    labelNames<<"\\median"; labelValues<<(jkqtp_floattolatexqstr(medianv, 3)+"\\:{\\pm}\\:"+jkqtp_floattolatexqstr(medConf, 3)); labMedian=labelValues.size()-1;
                } else {
                    labelNames<<"\\median"; labelValues<<jkqtp_floattolatexqstr(medianv, 3); labMedian=labelValues.size()-1;
                }
            }
            if (JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<jkqtp_floattolatexqstr(meanv, 3); labMean=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<jkqtp_floattolatexqstr(p75v, 3); labQ75=labelValues.size()-1; }
            if (JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<jkqtp_floattolatexqstr(maxv, 3); labMax=labelValues.size()-1; }

            double w=parent->pt2px(painter,getBoxWidthAbsolute());
            double yma=y+w/2.0;
            double ymi=y-w/2.0;

            plotHorizontalBoxplot(parent, painter, y, ymi, yma, (drawMinMax)?min:JKQTP_NAN, p25, (drawMedian)?median:JKQTP_NAN, p75, (drawMinMax)?max:JKQTP_NAN, (drawMean)?mean:JKQTP_NAN, (drawMedian&&drawNotch)?medianu:JKQTP_NAN, (drawMedian&&drawNotch)?mediano:JKQTP_NAN);

            if (JKQTPIsOKFloat(p25v) && JKQTPIsOKFloat(p75v)) {
                if (JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(p25v, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(p75v, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        
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



