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



#include "jkqtplotter/graphs/jkqtpfilledcurve.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;



JKQTPFilledCurveGraphBase::JKQTPFilledCurveGraphBase(JKQTBasePlotter *parent):
    JKQTPXYBaselineGraph(parent), m_fillMode(FillMode::SingleFilling)
{
    parentPlotStyle=-1;
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);
    initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);
    m_fillStyleBelow.initFillStyleInvertedColor(this);
    setFillCurve(true);
    setDrawLine(true);
}

QColor JKQTPFilledCurveGraphBase::getKeyLabelColor() const
{
    return getLineColor();
}

void JKQTPFilledCurveGraphBase::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setWidthF(getKeyLineWidthPx(painter, rect, parent));
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    const double y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (getDrawLine()) painter.setPen(p);
    painter.setBrush(b);
    if (getFillCurve()) {
        if (getFillMode()==FillMode::SingleFilling) {
            painter.drawRect(rect);
        } else {
            QBrush belowB=fillStyleBelow().getFillBrush(painter, parent);
            QPolygonF p,pb;
            p<<rect.topLeft()<<rect.topRight()<<rect.bottomRight();
            pb<<rect.topLeft()<<rect.bottomRight()<<rect.bottomLeft();
            painter.drawPolygon(p);
            painter.setBrush(belowB);
            painter.drawPolygon(pb);
            painter.setBrush(b);
        }
    }
    if (!getFillCurve() && getDrawLine()) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
}

JKQTPGraphFillStyleMixin &JKQTPFilledCurveGraphBase::fillStyleBelow()
{
    return m_fillStyleBelow;
}

const JKQTPGraphFillStyleMixin &JKQTPFilledCurveGraphBase::fillStyleBelow() const
{
    return m_fillStyleBelow;
}

JKQTPFilledCurveGraphBase::FillMode JKQTPFilledCurveGraphBase::getFillMode() const
{
    return m_fillMode;
}

void JKQTPFilledCurveGraphBase::setColor(QColor c)
{
    setLineColor(c);
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.filledStyle.fillColorDerivationMode, c));
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
    m_fillStyleBelow.initFillStyleInvertedColor(this);
}

void JKQTPFilledCurveGraphBase::setFillMode(FillMode mode)
{
    m_fillMode=mode;
}




JKQTPFilledCurveXGraph::JKQTPFilledCurveXGraph(JKQTBasePlotter* parent):
    JKQTPFilledCurveGraphBase(parent)
{

}

JKQTPFilledCurveXGraph::JKQTPFilledCurveXGraph(JKQTPlotter *parent):
    JKQTPFilledCurveXGraph(parent->getPlotter())
{

}

void JKQTPFilledCurveXGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPFilledCurveXGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    QPen p=getLinePen(painter, parent);
    QPen ph=getHighlightingLinePen(painter, parent);
    //QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    QBrush b_below=fillStyleBelow().getFillBrush(painter, parent);

    int imax=0;
    int imin=0;

    if (getIndexRange(imin, imax)) {


        QPainterPath pl, pf;

        double xold=-1;
        //double yold=-1;
        double y0=transformY(getBaseline());
        if (parent->getYAxis()->isLogAxis()) {
            y0=transformY(parent->getYAxis()->getMin());
            if (getBaseline()>0 && getBaseline()>parent->getYAxis()->getMin()) y0=transformY(getBaseline());
            else y0=transformY(parent->getYAxis()->getMin());
        }
        bool subsequentItem=false;
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            const int i=qBound(imin, getDataIndex(iii), imax);
            const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                const double x=transformX(xv);
                const double y=transformY(yv);
                if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
                    if (subsequentItem) {
                        pf.lineTo(x, y);
                        if (getDrawLine()) {
                            pl.lineTo(x, y);
                        }
                    } else {
                        if (getDrawLine()) pl.moveTo(x,y);
                        pf.moveTo(x, y0);
                        pf.lineTo(x, y);
                    }
                    xold=x;
                    //yold=y;
                    subsequentItem=true;
                }
            }
        }
        if (getFillCurve()) {
            pf.lineTo(xold, y0);
            pf.closeSubpath();
        }
        painter.save();
        auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        if (getFillCurve()) {
            if (getFillMode()==FillMode::SingleFilling) {
                painter.fillPath(pf, b);
            } else if (getFillMode()==FillMode::TwoColorFilling) {
                QRectF rAbove=pf.boundingRect();
                rAbove.setBottom(y0);
                QPainterPath pAbove;
                pAbove.addRect(rAbove);
                QRectF rBelow=pf.boundingRect();
                rBelow.setTop(y0);
                QPainterPath pBelow;
                pBelow.addRect(rBelow);
                QPainterPath pfa=pf.intersected(pAbove);
                QPainterPath pfb=pf.intersected(pBelow);
                painter.fillPath(pfa, b);
                painter.fillPath(pfb, b_below);
            }
        }

        if (isHighlighted()) {
            painter.setBrush(QBrush(Qt::transparent));
            painter.setPen(ph);
            painter.drawPath(pl);
        }

        if (getDrawLine()) {
            painter.setBrush(QBrush(Qt::transparent));
            painter.setPen(p);
            painter.drawPath(pl);
        }

    }

    drawErrorsAfter(painter);
}

JKQTPFilledCurveYGraph::JKQTPFilledCurveYGraph(JKQTBasePlotter* parent):
    JKQTPFilledCurveGraphBase(parent)
{
}

JKQTPFilledCurveYGraph::JKQTPFilledCurveYGraph(JKQTPlotter *parent):
    JKQTPFilledCurveYGraph(parent->getPlotter())
{

}

void JKQTPFilledCurveYGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPSpecialLineVerticalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    QPen p=getLinePen(painter, parent);
    QPen ph=getHighlightingLinePen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    QBrush b_below=fillStyleBelow().getFillBrush(painter, parent);

    int imax=0;
    int imin=0;

    if (getIndexRange(imin, imax)) {


        QPainterPath pl, pf;

        //double xold=-1;
        double yold=-1;
        double x0=transformX(getBaseline());
        if (parent->getXAxis()->isLogAxis()) {
            if (getBaseline()>0 && getBaseline()>parent->getXAxis()->getMin()) x0=transformX(getBaseline());
            else x0=transformX(parent->getXAxis()->getMin());
        }
        bool first=false;
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            const int i=qBound(imin, getDataIndex(iii), imax);
            const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                const double x=transformX(xv);
                const double y=transformY(yv);
                if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
                    if (first) {

                        pf.lineTo(x, y);
                        if (getDrawLine()) {
                            pl.lineTo(x, y);
                        }
                    } else {
                        if (getDrawLine()) pl.moveTo(x,y);
                        pf.moveTo(x0, y);
                        pf.lineTo(x, y);
                    }
                    //xold=x;
                    yold=y;
                    first=true;
                }
            }
        }
        pf.lineTo(x0, yold);
        pf.closeSubpath();
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        if (getFillCurve()) {
            if (getFillMode()==FillMode::SingleFilling) {
                painter.fillPath(pf, b);
            } else if (getFillMode()==FillMode::TwoColorFilling) {
                QRectF rAbove=pf.boundingRect();
                rAbove.setLeft(x0);
                QPainterPath pAbove;
                pAbove.addRect(rAbove);
                QRectF rBelow=pf.boundingRect();
                rBelow.setRight(x0);
                QPainterPath pBelow;
                pBelow.addRect(rBelow);
                QPainterPath pfa=pf.intersected(pAbove);
                QPainterPath pfb=pf.intersected(pBelow);
                painter.fillPath(pfa, b);
                painter.fillPath(pfb, b_below);
            }
        }

        if (isHighlighted()) {
            painter.setBrush(QBrush(Qt::transparent));
            painter.setPen(ph);
            painter.drawPath(pl);
        }

        if (getDrawLine()) {
            painter.setBrush(QBrush(Qt::transparent));
            painter.setPen(p);
            painter.drawPath(pl);
        }

    }

    drawErrorsAfter(painter);
}



JKQTPFilledCurveXErrorGraph::JKQTPFilledCurveXErrorGraph(JKQTBasePlotter *parent):
    JKQTPFilledCurveXGraph(parent)
{
    setErrorColorFromGraphColor(getLineColor());
    initErrorStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);
}

JKQTPFilledCurveXErrorGraph::JKQTPFilledCurveXErrorGraph(JKQTPlotter *parent):
    JKQTPFilledCurveXErrorGraph(parent->getPlotter())
{

}

bool JKQTPFilledCurveXErrorGraph::usesColumn(int c) const
{
    return JKQTPFilledCurveXGraph::usesColumn(c)|| JKQTPYGraphErrors::errorUsesColumn(c);
}

void JKQTPFilledCurveXErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);

}

JKQTPFilledCurveYErrorGraph::JKQTPFilledCurveYErrorGraph(JKQTBasePlotter *parent):
    JKQTPFilledCurveYGraph(parent)
{
    setErrorColorFromGraphColor(getLineColor());
    initErrorStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);

}

JKQTPFilledCurveYErrorGraph::JKQTPFilledCurveYErrorGraph(JKQTPlotter *parent):
    JKQTPFilledCurveYErrorGraph(parent->getPlotter())
{

}

bool JKQTPFilledCurveYErrorGraph::usesColumn(int c) const
{
    return JKQTPFilledCurveYGraph::usesColumn(c)|| JKQTPXGraphErrors::errorUsesColumn(c);
}

void JKQTPFilledCurveYErrorGraph::drawErrorsAfter(JKQTPEnhancedPainter &painter)
{
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);

}



JKQTPFilledVerticalRangeGraph::JKQTPFilledVerticalRangeGraph(JKQTBasePlotter *parent):
    JKQTPXYYGraph(parent)
{
    initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);
}

JKQTPFilledVerticalRangeGraph::JKQTPFilledVerticalRangeGraph(JKQTPlotter *parent):
    JKQTPFilledVerticalRangeGraph(parent->getPlotter())
{

}

void JKQTPFilledVerticalRangeGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPFilledCurveXGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        QPen ps=getHighlightingLinePen(painter, parent);
        QPen np(Qt::NoPen);

        QBrush b=getFillBrush(painter, parent);

        int imax=0;
        int imin=0;
        if (getIndexRange(imin, imax)) {

            // upper points are added to poly_all, lower points to plow
            // then plow points are added to poly_all in vewerse order
            // then the whole thing is drawn
            QPolygonF poly_all, phigh, plow;

            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                const double yv2=datastore->get(static_cast<size_t>(yColumn2),static_cast<size_t>(i));
                //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
                if (JKQTPIsOKFloat(xv)) {
                    const double x=transformX(xv); const bool xok=JKQTPIsOKFloat(x);
                    const double y=transformY(yv); const bool yok=JKQTPIsOKFloat(y);
                    const double y2=transformY(yv2); const bool y2ok=JKQTPIsOKFloat(y2);

                    if (xok&&yok) phigh.append(QPointF(x,y));
                    if (xok&&yok) poly_all.append(QPointF(x,y));
                    if (xok&&y2ok) plow.append(QPointF(x,y2));
                }
            }

            if (plow.size()>0) {
                for (int i=plow.size()-1; i>=0; i--) {
                    poly_all.append(plow[i]);
                }
            }

            if (getFillCurve()) {
                painter.setBrush(b);
                painter.setPen(np);
                painter.drawPolygon(poly_all);
            }


            if (getDrawLine()) {
                painter.save(); auto __finalpaintline=JKQTPFinally([&painter]() {painter.restore();});

                if (isHighlighted()) {

                    painter.setPen(ps);
                    painter.drawPolylineFast(phigh);
                    painter.drawPolylineFast(plow);
                }


                painter.setPen(p);
                painter.drawPolylineFast(phigh);
                painter.drawPolylineFast(plow);

            }
        }
    }
    drawErrorsAfter(painter);
}

void JKQTPFilledVerticalRangeGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QRectF r=rect;
    r.setHeight(r.height()/2.0);
    r.moveTo(r.x(), r.y()+r.height()-1);
    painter.fillRect(r, getFillBrush(painter, parent));
    if (getDrawLine()) {
        painter.setPen(getKeyLinePen(painter, rect, parent));
        painter.drawLine(QLineF(r.topLeft(), r.topRight()));
    }

}

QColor JKQTPFilledVerticalRangeGraph::getKeyLabelColor() const
{
    return getLineColor();
}




JKQTPFilledHorizontalRangeGraph::JKQTPFilledHorizontalRangeGraph(JKQTBasePlotter *parent):
    JKQTPXXYGraph(parent)
{
    initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Filled);
}

JKQTPFilledHorizontalRangeGraph::JKQTPFilledHorizontalRangeGraph(JKQTPlotter *parent):
    JKQTPFilledHorizontalRangeGraph(parent->getPlotter())
{

}

void JKQTPFilledHorizontalRangeGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPFilledCurveXGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        QPen ps=getHighlightingLinePen(painter, parent);
        QPen np(Qt::NoPen);

        QBrush b=getFillBrush(painter, parent);

        int imax=0;
        int imin=0;
        if (getIndexRange(imin, imax)) {

            // upper points are added to poly_all, lower points to plow
            // then plow points are added to poly_all in vewerse order
            // then the whole thing is drawn
            QPolygonF poly_all, phigh, plow;

            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                const double xv2=datastore->get(static_cast<size_t>(xColumn2),static_cast<size_t>(i));
                //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
                if (JKQTPIsOKFloat(xv)) {
                    const double x=transformX(xv); const bool xok=JKQTPIsOKFloat(x);
                    const double y=transformY(yv); const bool yok=JKQTPIsOKFloat(y);
                    const double x2=transformX(xv2); const bool x2ok=JKQTPIsOKFloat(x2);

                    if (xok&&yok) phigh.append(QPointF(x,y));
                    if (xok&&yok) poly_all.append(QPointF(x,y));
                    if (xok&&x2ok) plow.append(QPointF(x2,y));
                }
            }

            if (plow.size()>0) {
                for (int i=plow.size()-1; i>=0; i--) {
                    poly_all.append(plow[i]);
                }
            }

            if (getFillCurve()) {
                painter.setBrush(b);
                painter.setPen(np);
                painter.drawPolygon(poly_all);
            }


            if (getDrawLine()) {
                painter.save(); auto __finalpaintline=JKQTPFinally([&painter]() {painter.restore();});

                if (isHighlighted()) {

                    painter.setPen(ps);
                    painter.drawPolylineFast(phigh);
                    painter.drawPolylineFast(plow);
                }


                painter.setPen(p);
                painter.drawPolylineFast(phigh);
                painter.drawPolylineFast(plow);

            }
        }
    }
    drawErrorsAfter(painter);
}

void JKQTPFilledHorizontalRangeGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QRectF r=rect;
    r.setHeight(r.height()/2.0);
    r.moveTo(r.x(), r.y()+r.height()-1);
    painter.fillRect(r, getFillBrush(painter, parent));
    if (getDrawLine()) {
        painter.setPen(getKeyLinePen(painter, rect, parent));
        painter.drawLine(QLineF(r.topLeft(), r.topRight()));
    }

}

QColor JKQTPFilledHorizontalRangeGraph::getKeyLabelColor() const
{
    return getLineColor();
}

