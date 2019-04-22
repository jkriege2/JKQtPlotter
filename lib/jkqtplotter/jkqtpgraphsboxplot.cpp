/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/jkqtpgraphsboxplot.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;








JKQTPBoxplotVerticalGraph::JKQTPBoxplotVerticalGraph(JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    posColumn=-1;
    medianColumn=-1;
    meanColumn=-1;
    minColumn=-1;
    maxColumn=-1;
    percentile25Column=-1;
    percentile75Column=-1;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=0.4;
    meanSymbol=JKQTPPlus;
    meanSymbolWidth=1;
    meanSymbolSize=12;
    sortData=Unsorted;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=parent->getPlotStyle(parentPlotStyle).fillColor();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
        meanSymbolSize=parent->getPlotStyle(parentPlotStyle).symbolSize();
        meanSymbolWidth=parent->getPlotStyle(parentPlotStyle).symbolLineWidthF();
    }

}


JKQTPBoxplotVerticalGraph::JKQTPBoxplotVerticalGraph(JKQTPlotter* parent):
    JKQTPGraph(parent)
{
    posColumn=-1;
    medianColumn=-1;
    meanColumn=-1;
    minColumn=-1;
    maxColumn=-1;
    percentile25Column=-1;
    percentile75Column=-1;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=0.4;
    meanSymbol=JKQTPPlus;
    meanSymbolWidth=1;
    meanSymbolSize=12;
    sortData=Unsorted;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=parent->getPlotStyle(parentPlotStyle).fillColor();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
        meanSymbolSize=parent->getPlotStyle(parentPlotStyle).symbolSize();
        meanSymbolWidth=parent->getPlotStyle(parentPlotStyle).symbolLineWidthF();
    }

}


void JKQTPBoxplotVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotVerticalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);


    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=datastore->getColumn(posColumn).getRows();
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;



    //bool first=false;

    // 1. step find width of boxplots:
    double boxwidth_real=0;
    bool bwfirst=true;
    intSortData();
    for (int iii=imin+1; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv0=transformX(datastore->get(posColumn,i-1));
        double xv=transformX(datastore->get(posColumn,i));
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
            double xv=datastore->get(posColumn,i);
            double p25v=datastore->get(percentile25Column,i);
            double p75v=datastore->get(percentile75Column,i);
            double minv=datastore->get(minColumn,i);
            double maxv=datastore->get(maxColumn,i);
            double medianv=datastore->get(medianColumn,i);
            double mean=transformY(datastore->get(meanColumn,i));

            QVector<QLineF> lines_p, lines_pw;

            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(p25v) &&
                JKQTPIsOKFloat(p75v) && JKQTPIsOKFloat(minv) &&
                JKQTPIsOKFloat(maxv) && JKQTPIsOKFloat(medianv) ) {

                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(p);
                painter.setBrush(b);
                //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
                double x=transformX(xv);
                double p25=transformY(p25v);
                double p75=transformY(p75v);
                double min=transformY(minv);
                double max=transformY(maxv);
                double median=transformY(medianv);

                double xn=x+1;
                if (i+1<imax) xn=transformX(datastore->get(posColumn,i+1));
                else if (i-1>=0) xn=transformX(datastore->get(posColumn,i-1));
                else xn=x+1;


                double w=((boxwidth_real>0)?boxwidth_real:(fabs(xn-x)))*boxWidth;
                double minstop=p25;
                double maxstop=p75;
                if (percentile25Column<0 && medianColumn>=0) minstop=median;
                else if (percentile25Column<0 && meanColumn>=0) minstop=mean;
                else if (percentile25Column<0 && maxColumn>=0) minstop=max;
                if (percentile75Column<0 && medianColumn>=0) maxstop=median;
                else if (percentile75Column<0 && meanColumn>=0) maxstop=mean;
                else if (percentile75Column<0 && minColumn>=0) maxstop=min;
                double xma=x+w/2.0;
                double xmi=x-w/2.0;
                double xma4=x+w/4.0;
                double xmi4=x-w/4.0;

                if (imax<=0) {
                    xma=transformX(xv+boxWidth/2.0);
                    xmi=transformX(xv-boxWidth/2.0);
                    xma4=transformX(xv+boxWidth/4.0);
                    xmi4=transformX(xv-boxWidth/4.0);
                }

                if (minColumn>=0) {
                    lines_p.append(QLineF(xmi4, min, xma4, min));
                    lines_pw.append(QLineF(x, min, x, minstop));
                }
                if (maxColumn>=0) {
                    lines_p.append(QLineF(xmi4, max, xma4, max));
                    lines_pw.append(QLineF(x, max, x, maxstop));
                }

                if (percentile25Column>=0 && percentile75Column>=0) painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
                if (medianColumn>=0) lines_p.append(QLineF(xmi+p.widthF()/2.0, median, xma-p.widthF()/2.0, median));
                if (meanColumn>=0 && JKQTPIsOKFloat(mean)) {
                    JKQTPPlotSymbol(painter, x, mean, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->getLineWidthMultiplier()), color, fillColor);
                }


                painter.setPen(p);
                if (lines_p.size()>0) painter.drawLines(lines_p);
                painter.setPen(pw);
                if (lines_pw.size()>0) painter.drawLines(lines_pw);
            

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
    int imax=datastore->getColumn(posColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,i);
        if (JKQTPIsOKFloat(xv)) {
            double xn=xv+1;
            if (i+1<imax) xn=datastore->get(posColumn,i+1);
            else if (i-1>=0) xn=datastore->get(posColumn,i-1);
            else xn=xv+1;
            double delta=fabs(xn-xv);
            double w=delta*boxWidth;
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
    int imax=datastore->getColumn(medianColumn).getRows();
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

bool JKQTPBoxplotVerticalGraph::usesColumn(int c) const
{
    return (c==meanColumn)||(c==posColumn)||(c==medianColumn)||(c==minColumn)||(c==maxColumn)||(c==percentile25Column)||(c==percentile75Column);
}

void JKQTPBoxplotVerticalGraph::setDataSortOrder(int __value) {
    sortData=(DataSortOrder)__value;
    if (__value>0) sortData=Sorted;
}


void JKQTPBoxplotVerticalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double x=rect.left()+rect.width()/2.0;
    double xma=x+rect.width()/2.5;
    double xmi=x-rect.width()/2.5;
    double min=rect.bottom();
    double max=rect.top();
    double median=max+rect.height()/2.0;
    double w=rect.width()/1.8;
    double p25=max+0.75*rect.height();
    double p75=max+0.25*rect.height();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
    }
    
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
    painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
    painter.setPen(pw);
    painter.drawLine(QLineF(x, max, x, p75));
    painter.drawLine(QLineF(x, min, x, p25));
    
}

QColor JKQTPBoxplotVerticalGraph::getKeyLabelColor() const {
    return color;
}




void JKQTPBoxplotHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double y=rect.top()+rect.height()/2.0;
    double yma=y+rect.height()/2.5;
    double ymi=y-rect.height()/2.5;
    double min=rect.left();
    double max=rect.right();
    double median=max-rect.width()/2.0;
    double w=rect.height()/1.8;
    double p25=min+0.75*rect.width();
    double p75=min+0.25*rect.width();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(p75, ymi, fabs(p75-p25), fabs(yma-ymi)));
    }
    
    painter.drawLine(QLineF(median, ymi, median, yma));
    painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
    painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
    painter.setPen(pw);
    painter.drawLine(QLineF(max, y, p75, y));
    painter.drawLine(QLineF(min, y, p25, y));
    
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
    int imax=datastore->getColumn(medianColumn).getRows();
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
    int imax=datastore->getColumn(posColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,i);
        double xn=xv+1;
        if (i+1<imax) xn=datastore->get(posColumn,i+1);
        else if (i-1>=0) xn=datastore->get(posColumn,i-1);
        else xn=xv+1;
        double delta=fabs(xn-xv);
        double w=delta*boxWidth;
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

JKQTPBoxplotHorizontalGraph::JKQTPBoxplotHorizontalGraph(JKQTBasePlotter *parent):
    JKQTPBoxplotVerticalGraph(parent)
{
}

JKQTPBoxplotHorizontalGraph::JKQTPBoxplotHorizontalGraph(JKQTPlotter *parent):
    JKQTPBoxplotVerticalGraph(parent)
{
}

void JKQTPBoxplotHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotHorizontalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);


    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=datastore->getColumn(posColumn).getRows();
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
            double yv=datastore->get(posColumn,i);
            double p25v=datastore->get(percentile25Column,i);
            double p75v=datastore->get(percentile75Column,i);
            double minv=datastore->get(minColumn,i);
            double maxv=datastore->get(maxColumn,i);
            double medianv=datastore->get(medianColumn,i);
            double mean=transformX(datastore->get(meanColumn,i));

            QVector<QLineF> lines_p, lines_pw;
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (posColumn>=0 && JKQTPIsOKFloat(yv) &&
                JKQTPIsOKFloat(p25v) &&
                JKQTPIsOKFloat(p75v) &&
                JKQTPIsOKFloat(minv) &&
                JKQTPIsOKFloat(maxv) &&
                JKQTPIsOKFloat(medianv)) {

                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(p);
                painter.setBrush(b);


                double p25=transformX(p25v);
                double p75=transformX(p75v);
                double min=transformX(minv);
                double max=transformX(maxv);
                double median=transformX(medianv);


                //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
                double y=transformY(yv);
                double minstop=p25;
                double maxstop=p75;
                if (percentile25Column<0 && medianColumn>=0) minstop=median;
                else if (percentile25Column<0 && maxColumn>=0) minstop=max;
                else if (percentile25Column<0 && meanColumn>=0) minstop=mean;
                if (percentile75Column<0 && medianColumn>=0) maxstop=median;
                else if (percentile75Column<0 && minColumn>=0) maxstop=min;
                else if (percentile75Column<0 && meanColumn>=0) maxstop=mean;

                double yn=y+1;
                if (i+1<imax) yn=transformY(datastore->get(posColumn,i+1));
                else if (i-1>=0) yn=transformY(datastore->get(posColumn,i-1));
                else yn=y+1;
                double delta=fabs(yn-y);
                double w=((boxwidth_real>0)?boxwidth_real:(delta))*boxWidth;
                double yma=y-w/2.0;
                double ymi=y+w/2.0;
                double yma4=y+w/4.0;
                double ymi4=y-w/4.0;

                if (imax<=1) {
                    ymi=transformY(yv+boxWidth/2.0);
                    yma=transformY(yv-boxWidth/2.0);
                    yma4=transformY(yv+boxWidth/4.0);
                    ymi4=transformY(yv-boxWidth/4.0);
                }
                if (minColumn>=0) {
                    lines_p.append(QLineF(min, ymi4, min, yma4));
                    lines_pw.append(QLineF(min, y, minstop, y));
                }
                if (maxColumn>=0) {
                    lines_p.append(QLineF(max, ymi4, max, yma4));
                    lines_pw.append(QLineF(max, y, maxstop, y));
                }
                if (percentile25Column>=0 && percentile75Column>=0) painter.drawRect(QRectF(p25, qMin(yma,ymi), fabs(p75-p25), fabs(yma-ymi)));
                if (medianColumn>=0) lines_p.append(QLineF(median, ymi-p.widthF()/2.0, median, yma+p.widthF()/2.0));

                if (meanColumn>=0 && JKQTPIsOKFloat(mean)) {
                    JKQTPPlotSymbol(painter, mean, y, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->getLineWidthMultiplier()), color, fillColor);
                }

                //first=true;
                painter.setPen(p);
                if (lines_p.size()>0) painter.drawLines(lines_p);
                painter.setPen(pw);
                if (lines_pw.size()>0) painter.drawLines(lines_pw);
            
            }

        }

    }

    drawErrorsAfter(painter);
}





















JKQTPBoxplotVerticalElement::JKQTPBoxplotVerticalElement(JKQTBasePlotter* parent):
    JKQTPPlotObject(parent)
{
    pos=0;
    median=0;
    mean=0;
    min=-1;
    max=1;
    drawMean=true;
    drawMinMax=true;
    percentile25=-0.75;
    percentile75=0.75;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=16;
    meanSymbol=JKQTPPlus;
    meanSymbolWidth=1;
    meanSymbolSize=12;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=parent->getPlotStyle(parentPlotStyle).fillColor();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
        meanSymbolSize=parent->getPlotStyle(parentPlotStyle).symbolSize();
        meanSymbolWidth=parent->getPlotStyle(parentPlotStyle).symbolLineWidthF();
    }

}

JKQTPBoxplotVerticalElement::JKQTPBoxplotVerticalElement(JKQTPlotter* parent):
    JKQTPPlotObject(parent)
{
    pos=0;
    median=0;
    mean=0;
    min=-1;
    max=1;
    drawMean=true;
    drawMinMax=true;
    percentile25=-0.75;
    percentile75=0.75;
    color=QColor("red");
    fillColor=QColor("white");
    fillStyle=Qt::SolidPattern;
    whiskerStyle=Qt::SolidLine;
    lineWidth=1;
    boxWidth=16;
    meanSymbol=JKQTPPlus;
    meanSymbolWidth=1;
    meanSymbolSize=12;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=parent->getPlotStyle(parentPlotStyle).fillColor();
        whiskerStyle=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
        meanSymbolSize=parent->getPlotStyle(parentPlotStyle).symbolSize();
        meanSymbolWidth=parent->getPlotStyle(parentPlotStyle).symbolLineWidthF();
    }

}
void JKQTPBoxplotVerticalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotVerticalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=painter.pen();
        p.setColor(color);
        p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
        p.setJoinStyle(Qt::RoundJoin);
        p.setJoinStyle(Qt::RoundJoin);
        p.setCapStyle(Qt::RoundCap);
        QPen pw=p;
        pw.setStyle(whiskerStyle);
        QPen np(Qt::NoPen);

        QBrush b=painter.brush();
        b.setColor(fillColor);
        b.setStyle(fillStyle);


        double xv=pos;
        double p25v=percentile25;
        double p75v=percentile75;
        double minv=min;
        double maxv=max;
        double medianv=median;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(p25v) &&
            JKQTPIsOKFloat(p75v) && JKQTPIsOKFloat(minv) &&
            JKQTPIsOKFloat(maxv) && JKQTPIsOKFloat(medianv) ) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            double x=transformX(xv);
            double p25=transformY(p25v);
            double p75=transformY(p75v);
            double min=transformY(minv);
            double max=transformY(maxv);
            double median=transformY(medianv);

            double w=boxWidth;
            double xma=x+w/2.0;
            double xmi=x-w/2.0;

            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setBrush(b);
                painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
            }
        
            if (drawMedian) {
                painter.drawLine(QLineF(xmi, median, xma, median));
            }
            if (drawMinMax) {
                painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
                painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
                painter.setPen(pw);
                painter.drawLine(QLineF(x, max, x, p75));
                painter.drawLine(QLineF(x, min, x, p25));
            }
        

            if (drawMean) {
                double mean=transformY(this->mean);
                JKQTPPlotSymbol(painter, x, mean, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->getLineWidthMultiplier()), color, fillColor);
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
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double x=rect.left()+rect.width()/2.0;
    double xma=x+rect.width()/2.5;
    double xmi=x-rect.width()/2.5;
    double min=rect.bottom();
    double max=rect.top();
    double median=max+rect.height()/2.0;
    double w=rect.width()/1.8;
    double p25=max+0.75*rect.height();
    double p75=max+0.25*rect.height();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
    }
    
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
    painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
    painter.setPen(pw);
    painter.drawLine(QLineF(x, max, x, p75));
    painter.drawLine(QLineF(x, min, x, p25));
    
}

QColor JKQTPBoxplotVerticalElement::getKeyLabelColor() const {
    return color;
}




void JKQTPBoxplotHorizontalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    QPen pw=p;
    pw.setStyle(whiskerStyle);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    double y=rect.top()+rect.height()/2.0;
    double yma=y+rect.height()/2.5;
    double ymi=y-rect.height()/2.5;
    double min=rect.left();
    double max=rect.right();
    double median=max-rect.width()/2.0;
    double w=rect.height()/1.8;
    double p25=min+0.75*rect.width();
    double p75=min+0.25*rect.width();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(p75, ymi, fabs(p75-p25), fabs(yma-ymi)));
    }
    
    painter.drawLine(QLineF(median, ymi, median, yma));
    painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
    painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
    painter.setPen(pw);
    painter.drawLine(QLineF(max, y, p75, y));
    painter.drawLine(QLineF(min, y, p25, y));
    
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

JKQTPBoxplotHorizontalElement::JKQTPBoxplotHorizontalElement(JKQTBasePlotter *parent):
    JKQTPBoxplotVerticalElement(parent)
{
}

JKQTPBoxplotHorizontalElement::JKQTPBoxplotHorizontalElement(JKQTPlotter *parent):
    JKQTPBoxplotVerticalElement(parent)
{
}

void JKQTPBoxplotHorizontalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotHorizontalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=painter.pen();
        p.setColor(color);
        p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
        p.setJoinStyle(Qt::RoundJoin);
        p.setJoinStyle(Qt::RoundJoin);
        p.setCapStyle(Qt::RoundCap);
        QPen pw=p;
        pw.setStyle(whiskerStyle);
        QPen np(Qt::NoPen);

        QBrush b=painter.brush();
        b.setColor(fillColor);
        b.setStyle(fillStyle);
        double yv=pos;
        double p25v=percentile25;
        double p75v=percentile75;
        double minv=min;
        double maxv=max;
        double medianv=median;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(yv) &&
            JKQTPIsOKFloat(p25v) &&
            JKQTPIsOKFloat(p75v) &&
            JKQTPIsOKFloat(minv) &&
            JKQTPIsOKFloat(maxv) &&
            JKQTPIsOKFloat(medianv)) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            double y=transformY(yv);
            double p25=transformX(p25v);
            double p75=transformX(p75v);
            double min=transformX(minv);
            double max=transformX(maxv);
            double median=transformX(medianv);

            double w= parent->pt2px(painter, boxWidth);
            double yma=y+w/2.0;
            double ymi=y-w/2.0;

            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setBrush(b);
                painter.drawRect(QRectF(p25, ymi, fabs(p75-p25), fabs(yma-ymi)));
            }
        
            if (drawMedian) {
                painter.drawLine(QLineF(median, ymi, median, yma));
            }
            if (drawMinMax) {
                painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
                painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
                painter.setPen(pw);
                painter.drawLine(QLineF(max, y, p75, y));
                painter.drawLine(QLineF(min, y, p25, y));
            }
        

            if (drawMean) {
                double mean=transformY(this->mean);
                JKQTPPlotSymbol(painter, mean, y, meanSymbol, parent->pt2px(painter, meanSymbolSize), parent->pt2px(painter, meanSymbolWidth*parent->getLineWidthMultiplier()), color, fillColor);
            }

        }
    }

    //drawErrorsAfter(painter);
}






QPen JKQTPBoxplotVerticalGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*lineWidth)));
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

void JKQTPBoxplotVerticalGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(posColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPBoxplotVerticalGraph::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(posColumn,i);
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());


    }
}

QBrush JKQTPBoxplotVerticalGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QBrush JKQTPBoxplotVerticalElement::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPBoxplotVerticalElement::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*lineWidth)));
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

