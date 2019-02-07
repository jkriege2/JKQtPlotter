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



#include "jkqtplotter/jkqtpgraphsbarchart.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;






JKQTPBarVerticalGraph::JKQTPBarVerticalGraph(JKQTBasePlotter* parent):
    JKQTPXYGraph(parent)
{
    baseline=0.0;
    color=QColor("black");
    fillColor=QColor("red");
    style=Qt::SolidLine;
    lineWidth=1;
    fillStyle=Qt::SolidPattern;
    width=0.9;
    shift=0;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=parent->getPlotStyle(parentPlotStyle).fillColor();
        fillStyle=parent->getPlotStyle(parentPlotStyle).fillStyle();
        //style=parent->getPlotStyle(parentPlotStyle).style();
        lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
    }
}


JKQTPBarVerticalGraph::JKQTPBarVerticalGraph(JKQTPlotter* parent):
    JKQTPBarVerticalGraph(parent->getPlotter())
{
}

void JKQTPBarVerticalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    painter.setPen(p);
    painter.setBrush(b);
    painter.drawRect(rect);

}

QColor JKQTPBarVerticalGraph::getKeyLabelColor() {
    return fillColor;
}


void JKQTPBarVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBarHorizontalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;

    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

    //    double x0=transformX(0);
    //    if (parent->getXAxis()->isLogAxis()) x0=transformX(parent->getXAxis()->getMin());
        double y0=transformY(0);
        if (parent->getYAxis()->isLogAxis()) y0=transformY(parent->getYAxis()->getMin());
        double delta=1;
        double deltap=0;
        double deltam=0;
        intSortData();
        const bool hasStackPar=hasStackParent();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound(imin, getDataIndex(iii), imax);
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            int sr=datastore->getNextLowerIndex(xColumn, i);
            int lr=datastore->getNextHigherIndex(xColumn, i);
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            double yv0=y0;
            if (!qFuzzyIsNull(baseline)) yv0=transformY(baseline);
            if (hasStackPar) {
                double stackLastY=getParentStackedMax(i);
                const double yvold=yv;
                yv0=transformY(stackLastY)-(getLineWidth());
                yv=stackLastY+yvold;
            }
            if (sr<0 && lr<0) { // only one x-value
                deltam=0.5;
                deltap=0.5;
            } else if (lr<0) { // the right-most x-value
                deltap=deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
            } else if (sr<0) { // the left-most x-value
                deltam=deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
            } else {
                deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
                deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
            }
            //std::cout<<iii<<", \t"<<i<<", \t"<<sr<<", \t"<<lr<<", \t"<<deltam<<", \t"<<deltap<<"\n\n";
            delta=deltap+deltam;

            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=transformX(xv+shift*delta-width*deltam);
                double y=transformY(yv);
                double xx=transformX(xv+shift*delta+width*deltap);
                double yy=yv0;

                //std::cout<<"delta="<<delta<<"   x="<<x<<" y="<<y<<"   xx="<<xx<<" yy="<<yy<<std::endl;
                if (yy<y) { qSwap(y,yy); }
                if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(xx) && JKQTPIsOKFloat(y) && JKQTPIsOKFloat(yy)) {
                    painter.setBrush(b);
                    painter.setPen(p);
                    QRectF r(QPointF(x, y), QPointF(xx, yy));
                    painter.drawRect(r);

                }
            }
        }

    }
    drawErrorsAfter(painter);
}

bool JKQTPBarVerticalGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        int sr=datastore->getNextLowerIndex(xColumn, i);
        int lr=datastore->getNextHigherIndex(xColumn, i);
        double delta, deltap, deltam;

        if (sr<0 && lr<0) { // only one x-value
            deltam=0.5;
            deltap=0.5;
        } else if (lr<0) { // the right-most x-value
            deltap=deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
        } else if (sr<0) { // the left-most x-value
            deltam=deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
        } else {
            deltam=fabs(xv-datastore->get(xColumn,sr))/2.0;
            deltap=fabs(datastore->get(xColumn,lr)-xv)/2.0;
        }
        delta=deltap+deltam;

        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(delta) ) {

            if (start || xv+shift*delta+width*delta/2.0>maxx) maxx=xv+shift*delta+width*delta/2.0;
            if (start || xv+shift*delta-width*delta/2.0<minx) minx=xv+shift*delta-width*delta/2.0;
            double xvsgz;
            xvsgz=xv+shift*delta+width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xv+shift*delta-width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPBarVerticalGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;
    if (baseline>0) {
        smallestGreaterZero=baseline;
        miny=baseline;
        maxy=baseline;
    }

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double stack=0;
        double yv=baseline;
        if (!isHorizontal()) {
            stack=getParentStackedMax(i);
            yv=stack;
        }
        if (JKQTPIsOKFloat(yv)) {
            if (yv>maxy) maxy=yv;
            if (yv<miny) miny=yv;
            double xvsgz;
            xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
        }
        yv=stack+datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(yv)) {
            if (yv>maxy) maxy=yv;
            if (yv<miny) miny=yv;
            double xvsgz;
            xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
        }
    }
    return true;
}

void JKQTPBarVerticalGraph::autoscaleBarWidthAndShift(double maxWidth, double shrinkFactor)
{
    if (parent) {
        double cntH=0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPPlotElement* g=parent->getGraph(i);
            JKQTPBarVerticalGraph* gb=qobject_cast<JKQTPBarVerticalGraph*>(g);
            if (gb && gb->isHorizontal()==isHorizontal()) {
                cntH++;
            }

        }

        double widthH=1.0/cntH*maxWidth*shrinkFactor;
        double dH=maxWidth/(cntH);
        double h=0.1+dH/2.0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPPlotElement* g=parent->getGraph(i);
            JKQTPBarVerticalGraph* gb=qobject_cast<JKQTPBarVerticalGraph*>(g);
            if (gb && gb->isHorizontal()==isHorizontal()) {
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

void JKQTPBarVerticalGraph::autoscaleBarWidthAndShiftSeparatedGroups(double groupWidth) {
    autoscaleBarWidthAndShift(groupWidth, 1);
}










JKQTPBarHorizontalGraph::JKQTPBarHorizontalGraph(JKQTBasePlotter *parent):
    JKQTPBarVerticalGraph(parent)
{

}

JKQTPBarHorizontalGraph::JKQTPBarHorizontalGraph(JKQTPlotter *parent):
    JKQTPBarHorizontalGraph(parent->getPlotter())
{

}

void JKQTPBarHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBarVerticalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    double x0=transformX(0);
    if (parent->getXAxis()->isLogAxis()) x0=transformX(parent->getXAxis()->getMin());
//    double y0=transformY(0);
//    if (parent->getYAxis()->isLogAxis()) y0=transformY(parent->getYAxis()->getMin());
    double delta=1;
    double deltap=0;
    double deltam=0;
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        intSortData();
        const bool hasStackPar=hasStackParent();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound(imin, getDataIndex(iii), imax);
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            int sr=datastore->getNextLowerIndex(yColumn, i);
            int lr=datastore->getNextHigherIndex(yColumn, i);
            double xv0=x0;
            if (!qFuzzyIsNull(baseline)) xv0=transformX(baseline);
            if (hasStackPar) {
                double stackLastX=getParentStackedMax(i);
                const double xvold=xv;
                xv0=transformX(stackLastX)+(getLineWidth());
                xv=stackLastX+xvold;
            }


            if (sr<0 && lr<0) { // only one y-value
                deltam=0.5;
                deltap=0.5;
            } else if (lr<0) { // the right-most y-value
                deltap=deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
            } else if (sr<0) { // the left-most y-value
                deltam=deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
            } else {
                deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
                deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
            }
            delta=deltap+deltam;

            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=xv0;
                if (!qFuzzyIsNull(baseline)) x=transformX(baseline);
                double y=transformY(yv+shift*delta+width*deltap);
                double xx=transformX(xv);
                double yy=transformY(yv+shift*delta-width*deltam);
                if (x>xx) { qSwap(x,xx); }
                //qDebug()<<"delta="<<delta<<"   x="<<x<<" y="<<y<<"   xx="<<xx<<" yy="<<yy;
                //qDebug()<<"xv="<<xv<<"   x0="<<x0<<"   x="<<x<<"..."<<xx;
                if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(xx) && JKQTPIsOKFloat(y) && JKQTPIsOKFloat(yy)) {
                    painter.setBrush(b);
                    painter.setPen(p);
                    QRectF r(QPointF(x, y), QPointF(xx, yy));
                    painter.drawRect(r);
                }
            }
        }

    }
    drawErrorsAfter(painter);
}

bool JKQTPBarHorizontalGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;
    if (baseline>0) {
        smallestGreaterZero=baseline;
        minx=baseline;
        maxx=baseline;
    }

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(xColumn)).getRows());
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double stack=0;
        double xv=baseline;
        if (isHorizontal()) {
            stack=getParentStackedMax(i);
            xv=stack;
        }
        if (JKQTPIsOKFloat(xv)) {
            if (xv>maxx) maxx=xv;
            if (xv<minx) minx=xv;
            double xvsgz;
            xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
        }
        xv=stack+datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv)) {
            if (xv>maxx) maxx=xv;
            if (xv<minx) minx=xv;
            double xvsgz;
            xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
        }
    }
    return true;
}

bool JKQTPBarHorizontalGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        double delta, deltap, deltam;
        int sr=datastore->getNextLowerIndex(yColumn, i);
        int lr=datastore->getNextHigherIndex(yColumn, i);

        if (sr<0 && lr<0) { // only one y-value
            deltam=0.5;
            deltap=0.5;
        } else if (lr<0) { // the right-most y-value
            deltap=deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
        } else if (sr<0) { // the left-most y-value
            deltam=deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
        } else {
            deltam=fabs(yv-datastore->get(yColumn,sr))/2.0;
            deltap=fabs(datastore->get(yColumn,lr)-yv)/2.0;
        }
        delta=deltap+deltam;
        if (JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(delta) ) {

            if (start || yv+shift*delta+width*delta/2.0>maxy) maxy=yv+shift*delta+width*delta/2.0;
            if (start || yv+shift*delta-width*delta/2.0<miny) miny=yv+shift*delta-width*delta/2.0;
            double xvsgz;
            xvsgz=yv+shift*delta+width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=yv+shift*delta-width*delta/2.0; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPBarHorizontalGraph::isHorizontal() const
{
    return true;
}







QBrush JKQTPBarVerticalGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPBarVerticalGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

double JKQTPBarVerticalGraph::getParentStackedMax(int /*index*/) const
{
    return false;
}

bool JKQTPBarVerticalGraph::hasStackParent() const
{
    return false;
}

double JKQTPBarVerticalGraph::getStackedMax(int /*index*/) const
{
    return baseline;
}



bool JKQTPBarVerticalGraph::isHorizontal() const
{
    return false;
}

void JKQTPBarVerticalGraph::setFillColor_and_darkenedColor(QColor fill, int colorDarker)
{
    setFillColor(fill);
    setColor(fill.darker(colorDarker));
}

JKQTPBarHorizontalErrorGraph::JKQTPBarHorizontalErrorGraph(JKQTBasePlotter *parent):
    JKQTPBarHorizontalGraph(parent), JKQTPXGraphErrors(color, parent)
{
    setErrorColorFromGraphColor(color);
    if (parentPlotStyle>=0) setErrorStyleFromPen(parent->getPlotStyle(parentPlotStyle));
}

JKQTPBarHorizontalErrorGraph::JKQTPBarHorizontalErrorGraph(JKQTPlotter *parent):
    JKQTPBarHorizontalErrorGraph(parent->getPlotter())
{

}

bool JKQTPBarHorizontalErrorGraph::usesColumn(int c) const
{
    return JKQTPBarHorizontalGraph::usesColumn(c)|| JKQTPXGraphErrors::errorUsesColumn(c);
}

bool JKQTPBarHorizontalErrorGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    if (xErrorColumn<0 || xErrorStyle==JKQTPNoError) {
        return JKQTPBarHorizontalGraph::getXMinMax(minx, maxx, smallestGreaterZero);
    } else {
        bool start=false;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        if (parent==nullptr) return false;

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double xvsgz;
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))+getXErrorU(i, datastore);
            double xvv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))-getXErrorL(i, datastore);
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xvv) ) {
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                if (start || xvv>maxx) maxx=xvv;
                if (start || xvv<minx) minx=xvv;
                xvsgz=xvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

void JKQTPBarHorizontalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0.0, shift);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0.0, shift, &sortedIndices);
}

JKQTPBarVerticalErrorGraph::JKQTPBarVerticalErrorGraph(JKQTBasePlotter *parent):
    JKQTPBarVerticalGraph(parent), JKQTPYGraphErrors(color, parent)
{
    setErrorColorFromGraphColor(color);
    if (parentPlotStyle>=0) setErrorStyleFromPen(parent->getPlotStyle(parentPlotStyle));

}

JKQTPBarVerticalErrorGraph::JKQTPBarVerticalErrorGraph(JKQTPlotter *parent):
    JKQTPBarVerticalErrorGraph(parent->getPlotter())
{

}

bool JKQTPBarVerticalErrorGraph::usesColumn(int c) const
{
    return JKQTPBarVerticalGraph::usesColumn(c)|| JKQTPYGraphErrors::errorUsesColumn(c);
}

bool JKQTPBarVerticalErrorGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (yErrorColumn<0 || yErrorStyle==JKQTPNoError) {
        miny=0;
        maxy=0;
        smallestGreaterZero=0;
        if (baseline>0) {
            smallestGreaterZero=baseline;
            miny=baseline;
            maxy=baseline;
        }

        if (parent==nullptr) return false;

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        for (int i=imin; i<imax; i++) {
            double yv=baseline;
            if (JKQTPIsOKFloat(yv)) {
                if (yv>maxy) maxy=yv;
                if (yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            }
            yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yv)) {
                if (yv>maxy) maxy=yv;
                if (yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            }
        }
        return true;
    } else {
        bool start=false;
        miny=baseline;
        maxy=baseline;
        smallestGreaterZero=0;
        if (baseline>0) {
            smallestGreaterZero=baseline;
            miny=baseline;
            maxy=baseline;
        }

        if (parent==nullptr) return false;

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;


        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))+getYErrorU(i, datastore);
            double yvv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))-getYErrorL(i, datastore);
            if (JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(yvv) ) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                if (start || yvv>maxy) maxy=yvv;
                if (start || yvv<miny) miny=yvv;
                xvsgz=yvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

void JKQTPBarVerticalErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    //plotErrorIndicators(painter, parent, this, xColumn, yColumn, shift, 0.0);
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn, shift, 0.0);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, shift, 0, &sortedIndices);
}

JKQTPBarVerticalStackableGraph::JKQTPBarVerticalStackableGraph(JKQTBasePlotter *parent):
    JKQTPBarVerticalGraph(parent), stackParent(nullptr)
{

}

JKQTPBarVerticalStackableGraph::JKQTPBarVerticalStackableGraph(JKQTPlotter *parent):
    JKQTPBarVerticalStackableGraph(parent->getPlotter())
{

}

void JKQTPBarVerticalStackableGraph::stackUpon(JKQTPBarVerticalStackableGraph *parentGraph)
{
    stackParent=parentGraph;
}

void JKQTPBarVerticalStackableGraph::dontStackUpon()
{
    stackParent=nullptr;
}

double JKQTPBarVerticalStackableGraph::getParentStackedMax(int index) const
{
    if (stackParent) {
        return stackParent->getStackedMax(index);
    } else {
        return 0.0;
    }
}

bool JKQTPBarVerticalStackableGraph::hasStackParent() const
{
    return stackParent!=nullptr;
}

const JKQTPBarVerticalStackableGraph *JKQTPBarVerticalStackableGraph::getStackParent() const
{
    return stackParent;
}



double JKQTPBarVerticalStackableGraph::getStackedMax(int index) const
{
    double height=0;
    if (parent!=nullptr) {
        JKQTPDatastore* datastore=parent->getDatastore();
        height=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(index));
    }
    if (stackParent==nullptr) {
        return height;
    } else {
        return stackParent->getStackedMax(index)+height;
    }
}

JKQTPBarHorizontalStackableGraph::JKQTPBarHorizontalStackableGraph(JKQTBasePlotter *parent):
    JKQTPBarHorizontalGraph(parent), stackParent(nullptr)
{

}

JKQTPBarHorizontalStackableGraph::JKQTPBarHorizontalStackableGraph(JKQTPlotter *parent):
    JKQTPBarHorizontalStackableGraph(parent->getPlotter())
{

}

void JKQTPBarHorizontalStackableGraph::stackUpon(JKQTPBarHorizontalStackableGraph *parentGraph)
{
    stackParent=parentGraph;
}

void JKQTPBarHorizontalStackableGraph::dontStackUpon()
{
    stackParent=nullptr;
}

const JKQTPBarHorizontalStackableGraph *JKQTPBarHorizontalStackableGraph::getStackParent() const
{
    return stackParent;
}


double JKQTPBarHorizontalStackableGraph::getStackedMax(int index) const
{
    double height=0;
    if (parent!=nullptr) {
        JKQTPDatastore* datastore=parent->getDatastore();
        height=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(index));
    }
    if (stackParent==nullptr) {
        return height;
    } else {
        return stackParent->getStackedMax(index)+height;
    }
}

double JKQTPBarHorizontalStackableGraph::getParentStackedMax(int index) const
{
    if (stackParent) {
        return stackParent->getStackedMax(index);
    } else {
        return 0.0;
    }
}

bool JKQTPBarHorizontalStackableGraph::hasStackParent() const
{
    return stackParent!=nullptr;
}
