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



#include "jkqtplotter/graphs/jkqtpfinancial.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter.h"

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;



QSet<JKQTBasePlotter*> JKQTPFinancialGraph::parentsAlreadySeen={};

void JKQTPFinancialGraph::clearColorAssignStore() {
    parentsAlreadySeen.clear();
}


JKQTPFinancialGraph::JKQTPFinancialGraph(JKQTBasePlotter* parent):
    JKQTPXGraph(parent),
    graphType(FinancialGraphType::CandleStick),
    openColumn(-1),
    closeColumn(-1),
    highColumn(-1),
    lowColumn(-1),
    width(0.8),
    shift(0)

{
    //qDebug()<<"1: pos: "<<m_lineStylePositive.getLineColor()<<", fill: "<<m_fillStylePositive.getFillColor();
    //qDebug()<<"1: neg: "<<m_lineStyleNegative.getLineColor()<<", fill: "<<m_fillStyleNegative.getFillColor();
    m_fillStylePositive.initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::FinancialPositive);
    m_fillStyleNegative.initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::FinancialNegative);

    m_lineStylePositive.initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::FinancialPositive);
    m_lineStyleNegative.initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::FinancialNegative);
    //qDebug()<<"2: pos: "<<m_lineStylePositive.getLineColor()<<", fill: "<<m_fillStylePositive.getFillColor();
    //qDebug()<<"2: neg: "<<m_lineStyleNegative.getLineColor()<<", fill: "<<m_fillStyleNegative.getFillColor();



    if (parent && !parentsAlreadySeen.contains(parent)) {
        // ensure, the first graph in a plot has default red/green colors
        // further plots will have colors
        //qDebug()<<"FIRST!"<<parent;
        m_fillStylePositive.setFillColor(parent->getCurrentPlotterStyle().graphsStyle.financialStyle.positiveDefaultColor);
        m_fillStyleNegative.setFillColor(parent->getCurrentPlotterStyle().graphsStyle.financialStyle.negativeDefaultColor);
        m_lineStylePositive.setLineColor(parent->getCurrentPlotterStyle().graphsStyle.financialStyle.positiveDefaultColor);
        m_lineStyleNegative.setLineColor(parent->getCurrentPlotterStyle().graphsStyle.financialStyle.negativeDefaultColor);
    }
    parentsAlreadySeen.insert(parent);

    //qDebug()<<"3: pos: "<<m_lineStylePositive.getLineColor()<<", fill: "<<m_fillStylePositive.getFillColor();
    //qDebug()<<"3: neg: "<<m_lineStyleNegative.getLineColor()<<", fill: "<<m_fillStyleNegative.getFillColor();
}


JKQTPFinancialGraph::JKQTPFinancialGraph(JKQTPlotter* parent):
    JKQTPFinancialGraph(parent->getPlotter())
{
}

void JKQTPFinancialGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& r) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    const QPen pp=m_lineStylePositive.getKeyLinePen(painter, r, parent);
    const QPen pn=m_lineStyleNegative.getKeyLinePen(painter, r, parent);
    const QBrush bp=m_fillStylePositive.getFillBrush(painter, parent);
    const QBrush bn=m_fillStyleNegative.getFillBrush(painter, parent);


    if (graphType==CandleStick) {
        painter.setPen(pp);
        painter.setBrush(bp);
        QRectF rect=r;
        rect.setWidth(r.width()-pp.widthF());
        rect.setHeight(r.height()*0.8);
        rect.setX(rect.x()+pp.widthF()/2.0);
        rect.setY(rect.y()+r.height()*0.1);
        painter.drawPolygon(QPolygonF()<<rect.topLeft()<<rect.topRight()<<rect.bottomLeft());
        painter.drawLine(r.center().x(),r.top(),r.center().x(),rect.top());
        painter.setPen(pn);
        painter.setBrush(bn);
        painter.drawPolygon(QPolygonF()<<rect.topRight()<<rect.bottomRight()<<rect.bottomLeft());
        painter.drawLine(r.center().x(),r.bottom(),r.center().x(),rect.bottom());

    } else if (graphType==OHLC) {
        painter.setPen(pp);
        painter.setBrush(bp);
        painter.drawLine(r.center().x(),r.top(),r.center().x(),r.center().y());
        painter.drawLine(r.center().x(),r.top()+r.height()/4.0,r.center().x()+r.width()/4,r.top()+r.height()/4.0);
        painter.setPen(pn);
        painter.setBrush(bn);
        painter.drawLine(r.center().x(),r.center().y(),r.center().x(),r.bottom());
        painter.drawLine(r.center().x(),r.top()+r.height()*0.8,r.center().x()-r.width()/4,r.top()+r.height()*0.8);
    }

}

QColor JKQTPFinancialGraph::getKeyLabelColor() const {
    return m_lineStylePositive.getLineColor();
}

void JKQTPFinancialGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPFinancialGraph::draw");
#endif
    if (parent==nullptr) return;
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    const QPen pp=m_lineStylePositive.getLinePenForRects(painter, parent);
    const QPen pn=m_lineStyleNegative.getLinePenForRects(painter, parent);
    const QBrush bp=m_fillStylePositive.getFillBrush(painter, parent);
    const QBrush bn=m_fillStyleNegative.getFillBrush(painter, parent);

    int imax=0;
    int imin=0;
    double left=-1e6;
    double right=1e6;
    bool firstXY=true;
    if (getIndexRange(imin, imax)) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        double delta=1;
        double deltap=0;
        double deltam=0;
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            const int i=qBound(imin, getDataIndex(iii), imax);
            const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            const int sr=datastore->getNextLowerIndex(xColumn, i);
            const int lr=datastore->getNextHigherIndex(xColumn, i);
            const double vO=datastore->get(static_cast<size_t>(openColumn),static_cast<size_t>(i));
            const double vC=datastore->get(static_cast<size_t>(closeColumn),static_cast<size_t>(i));
            const double vL=datastore->get(static_cast<size_t>(lowColumn),static_cast<size_t>(i));
            const double vH=datastore->get(static_cast<size_t>(highColumn),static_cast<size_t>(i));
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

            if (JKQTPIsOKFloat(xv)) {
                const double x=transformX(xv+shift*delta);
                const double xm=transformX(xv+shift*delta-qMin(width*deltam,width*deltap));
                const double xp=transformX(xv+shift*delta+qMin(width*deltam,width*deltap));
                const double yO=transformY(vO);
                const double yC=transformY(vC);
                const double yL=[&]() { double y=transformY(vL); if (!JKQTPIsOKFloat(y)) y=transformY(qMin(vO,vC)); return y; }();
                const double yH=[&]() { double y=transformY(vH); if (!JKQTPIsOKFloat(y)) y=transformY(qMax(vO,vC)); return y; }();
                if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(xm) && JKQTPIsOKFloat(xp) && JKQTPIsOKFloat(yC) && JKQTPIsOKFloat(yO)) {
                    const bool isPos=(vC>=vO);
                    if (firstXY) {
                        left=xm;
                        right=xp;
                    } else {
                        left=qMin(left, xm);
                        right=qMax(right, xp);
                    }
                    firstXY=false;

                    if (isPos) {
                        painter.setPen(pp);
                        painter.setBrush(bp);
                    } else {
                        painter.setPen(pn);
                        painter.setBrush(bn);
                    }
                    switch(graphType) {
                    case OHLC:
                        painter.drawLine(QLineF(x,yH,x,yL));
                        painter.drawLine(QLineF(xm,yO,x,yO));
                        painter.drawLine(QLineF(xp,yC,x,yC));
                        break;
                    case CandleStick: {
                            const QRectF rec(xm, qMin(yO,yC),xp-xm,fabs(yO-yC));
                            painter.drawLine(QLineF(x,yH,x,rec.top()));
                            painter.drawLine(QLineF(x,yL,x,rec.bottom()));
                            painter.drawRect(rec);
                        }
                        break;
                    }
                }
            }
        }
    }
    drawErrorsAfter(painter);
}


void JKQTPFinancialGraph::autoscaleBoxWidthAndShift(double maxWidth, double shrinkFactor)
{
    if (parent) {
        double cntH=0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPPlotElement* g=parent->getGraph(i);
            JKQTPFinancialGraph* gb=qobject_cast<JKQTPFinancialGraph*>(g);
            if (gb && considerForAutoscaling(gb)) {
                cntH++;
            }

        }

        double widthH=1.0/cntH*maxWidth*shrinkFactor;
        double dH=maxWidth/(cntH);
        double h=0.1+dH/2.0;
        for (size_t i=0; i<parent->getGraphCount(); i++) {
            JKQTPPlotElement* g=parent->getGraph(i);
            JKQTPFinancialGraph* gb=qobject_cast<JKQTPFinancialGraph*>(g);
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

void JKQTPFinancialGraph::autoscaleBoxWidthAndShiftSeparatedGroups(double groupWidth) {
    autoscaleBoxWidthAndShift(groupWidth, 0.8);
}


void JKQTPFinancialGraph::setShift(double __value)
{
    this->shift = __value;
}

double JKQTPFinancialGraph::getShift() const
{
    return this->shift;
}

void JKQTPFinancialGraph::setWidth(double __value)
{
    this->width = __value;
}

double JKQTPFinancialGraph::getWidth() const
{
    return this->width;
}

JKQTPGraphFillStyleMixin &JKQTPFinancialGraph::fillStyleNegative()
{
    return m_fillStyleNegative;
}

const JKQTPGraphFillStyleMixin &JKQTPFinancialGraph::fillStyleNegative() const
{
    return m_fillStyleNegative;
}

JKQTPGraphFillStyleMixin &JKQTPFinancialGraph::fillStylePositive()
{
    return m_fillStylePositive;
}

const JKQTPGraphFillStyleMixin &JKQTPFinancialGraph::fillStylePositive() const
{
    return m_fillStylePositive;
}

JKQTPGraphLineStyleMixin &JKQTPFinancialGraph::lineStyleNegative()
{
    return m_lineStyleNegative;
}

const JKQTPGraphLineStyleMixin &JKQTPFinancialGraph::lineStyleNegative() const
{
    return m_lineStyleNegative;
}

JKQTPGraphLineStyleMixin &JKQTPFinancialGraph::lineStylePositive()
{
    return m_lineStylePositive;
}

const JKQTPGraphLineStyleMixin &JKQTPFinancialGraph::lineStylePositive() const
{
    return m_lineStylePositive;
}

bool JKQTPFinancialGraph::getIndexRange(int &imin, int &imax) const
{
    bool ok=JKQTPXGraph::getIndexRange(imin, imax);
    /*if (ok) {
        if (parent==nullptr)  return false;
        const JKQTPDatastore* datastore=parent->getDatastore();
        if (openColumn<0) return false;
        if (closeColumn<0) return false;
        if (highColumn<0) return false;
        if (lowColumn<0) return false;
        const int rowsO=static_cast<int>(datastore->getRows(static_cast<size_t>(openColumn)));
        const int rowsC=static_cast<int>(datastore->getRows(static_cast<size_t>(closeColumn)));
        const int rowsH=static_cast<int>(datastore->getRows(static_cast<size_t>(highColumn)));
        const int rowsL=static_cast<int>(datastore->getRows(static_cast<size_t>(lowColumn)));
        imax=qMin<int>(imax, rowsO);
        imax=qMin<int>(imax, rowsC);
        imax=qMin<int>(imax, rowsH);
        imax=qMin<int>(imax, rowsL);
    }*/
    return ok;

}

bool JKQTPFinancialGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    if (getIndexRange(imin, imax)) {


        for (int i=imin; i<imax; i++) {
            const double yvO=datastore->get(static_cast<size_t>(openColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yvO)) {
                if (start || yvO>maxy) maxy=yvO;
                if (start || yvO<miny) miny=yvO;
                double xvsgz;
                xvsgz=yvO; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
            const double yvC=datastore->get(static_cast<size_t>(closeColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yvC)) {
                if (start || yvC>maxy) maxy=yvC;
                if (start || yvC<miny) miny=yvC;
                double xvsgz;
                xvsgz=yvC; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
            const double yvL=datastore->get(static_cast<size_t>(lowColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yvL)) {
                if (start || yvL>maxy) maxy=yvL;
                if (start || yvL<miny) miny=yvL;
                double xvsgz;
                xvsgz=yvL; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
            const double yvH=datastore->get(static_cast<size_t>(highColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yvH)) {
                if (start || yvH>maxy) maxy=yvH;
                if (start || yvH<miny) miny=yvH;
                double xvsgz;
                xvsgz=yvH; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
    return false;
}

bool JKQTPFinancialGraph::usesColumn(int column) const
{
    return JKQTPXGraph::usesColumn(column)||(column==openColumn)||(column==closeColumn)||(column==highColumn)||(column==lowColumn);
}


int JKQTPFinancialGraph::getOpenColumn() const
{
    return openColumn;
}

int JKQTPFinancialGraph::getCloseColumn() const
{
    return closeColumn;
}

int JKQTPFinancialGraph::getHighColumn() const
{
    return highColumn;
}

int JKQTPFinancialGraph::getLowColumn() const
{
    return lowColumn;
}

JKQTPFinancialGraph::FinancialGraphType JKQTPFinancialGraph::getGraphType() const
{
    return graphType;
}

void JKQTPFinancialGraph::setGraphType(JKQTPFinancialGraph::FinancialGraphType t)
{
    graphType=t;
}

void JKQTPFinancialGraph::setColor(QColor cPositive, QColor cNegative)
{
    m_fillStylePositive.setFillColor(cPositive);
    m_fillStyleNegative.setFillColor(cNegative);
    m_lineStylePositive.setLineColor(cPositive);
    m_lineStyleNegative.setLineColor(cNegative);
}

void JKQTPFinancialGraph::setColor(QColor linePositive, QColor fillPositive, QColor lineNegative, QColor fillNegative)
{
    m_fillStylePositive.setFillColor(fillPositive);
    m_fillStyleNegative.setFillColor(fillNegative);
    m_lineStylePositive.setLineColor(linePositive);
    m_lineStyleNegative.setLineColor(lineNegative);
}

void JKQTPFinancialGraph::setCandlestickTwoColor(QColor cPositive, QColor cNegative)
{
    setGraphType(CandleStick);
    setColor(cPositive, cNegative);
}

void JKQTPFinancialGraph::setCandlestickTwoColor(QColor cPositive, QColor cNegative, QColor lineColor)
{
    setGraphType(CandleStick);
    setColor(lineColor, cPositive, lineColor, cNegative);
}

void JKQTPFinancialGraph::setCandlestickOneColor(QColor cLine)
{
    setGraphType(CandleStick);
    if (parent) setColor(cLine,parent->getCurrentPlotterStyle().plotBackgroundBrush.color(),cLine,cLine);
    else  setColor(cLine,Qt::transparent,cLine,cLine);
}

void JKQTPFinancialGraph::setOHLCTwoColor(QColor cPositive, QColor cNegative)
{
    setGraphType(OHLC);
    setColor(cPositive, cNegative);
}

void JKQTPFinancialGraph::setOpenColumn(int __value)
{
    openColumn=static_cast<int>(__value);
}

void JKQTPFinancialGraph::setOpenColumn(size_t __value)
{
    openColumn=static_cast<int>(__value);
}

void JKQTPFinancialGraph::setCloseColumn(int __value)
{
    closeColumn=static_cast<int>(__value);
}

void JKQTPFinancialGraph::setCloseColumn(size_t __value)
{
    closeColumn=static_cast<int>(__value);
}

void JKQTPFinancialGraph::setHighColumn(int __value)
{
    highColumn=static_cast<int>(__value);
}

void JKQTPFinancialGraph::setHighColumn(size_t __value)
{
    highColumn=static_cast<int>(__value);
}

void JKQTPFinancialGraph::setLowColumn(int __value)
{
    lowColumn=static_cast<int>(__value);
}

void JKQTPFinancialGraph::setLowColumn(size_t __value)
{
    lowColumn=static_cast<int>(__value);
}

bool JKQTPFinancialGraph::considerForAutoscaling(JKQTPFinancialGraph *other) const
{
    return (dynamic_cast<JKQTPFinancialGraph*>(other)!=nullptr);
}
