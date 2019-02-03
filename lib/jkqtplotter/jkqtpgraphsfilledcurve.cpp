/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtplotter/jkqtpgraphsfilledcurve.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;







JKQTPFilledCurveXGraph::JKQTPFilledCurveXGraph(JKQTBasePlotter* parent):
    JKQTPXYGraph(parent)
{
    baseline=0.0;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    drawLine=true;
    fillStyle=Qt::SolidPattern;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        fillColor=color.lighter();
    }
}

JKQTPFilledCurveXGraph::JKQTPFilledCurveXGraph(JKQTPlotter *parent):
    JKQTPXYGraph(parent)
{
    baseline=0.0;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    drawLine=true;
    fillStyle=Qt::SolidPattern;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        fillColor=color.lighter();
    }
}

void JKQTPFilledCurveXGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPFilledCurveXGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=static_cast<int>(qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows()));
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QPainterPath pl, pf;

    double xold=-1;
    //double yold=-1;
//    double x0=transformX(baseline);
//    if (parent->getXAxis()->isLogAxis()) {
//        if (baseline>0 && baseline>parent->getXAxis()->getMin()) x0=transformX(baseline);
//        else x0=transformX(parent->getXAxis()->getMin());
//    }
    double y0=transformY(baseline);
    if (parent->getYAxis()->isLogAxis()) {
        y0=transformY(parent->getYAxis()->getMin());
        if (baseline>0 && baseline>parent->getYAxis()->getMin()) y0=transformY(baseline);
        else y0=transformY(parent->getYAxis()->getMin());
    }
    bool first=true;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=transformX(xv); bool xok=JKQTPIsOKFloat(x);
            double y=transformY(yv); bool yok=JKQTPIsOKFloat(y);

            if (!first) {
                if (xok&&yok) {
                    pf.lineTo(x, y);
                    if (drawLine) pl.lineTo(x, y);
                    xold=x;
                    //yold=y;
                } else if (xok&&!yok){
                    pf.lineTo(x, y0);
                    if (drawLine) pl.lineTo(x, y0);
                    xold=x;
                    //yold=y0;
                }
            } else {
                if (xok&&yok) {
                    if (drawLine) pl.moveTo(x,y);
                    pf.moveTo(x, y0);
                    pf.lineTo(x, y);
                    xold=x;
                    //yold=y;
                    first=false;
                } else if (xok&&!yok) {
                    if (drawLine) pl.moveTo(x,y0);
                    pf.moveTo(x, y0);
                    xold=x;
                    //yold=y0;
                    first=false;
                }
            }

        }
    }
    pf.lineTo(xold, y0);
    pf.closeSubpath();
    if (drawLine) {
        painter.save();

        if (drawSelectionLine) {
            QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
            QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
            if (selectionLineColor!=Qt::transparent) {
                penSelection.setColor(selectionLineColor);
            }
            penSelection.setWidthF(penSelection.widthF()*3.0);
            painter.setPen(penSelection);
            painter.drawPath(pl);
        }


        painter.setPen(p);
        painter.drawPath(pl);
        painter.restore();
    }
    painter.setBrush(b);
    painter.setPen(np);
    painter.drawPath(pf);

    painter.restore();

    drawErrorsAfter(painter);
}

void JKQTPFilledCurveXGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (drawLine) painter.setPen(p);
    painter.setBrush(b);
    painter.drawRect(rect);
    painter.restore();
}

QColor JKQTPFilledCurveXGraph::getKeyLabelColor() {
    return color;
}














JKQTPFilledCurveYGraph::JKQTPFilledCurveYGraph(JKQTBasePlotter* parent):
    JKQTPFilledCurveXGraph(parent)
{
}

JKQTPFilledCurveYGraph::JKQTPFilledCurveYGraph(JKQTPlotter *parent):
    JKQTPFilledCurveXGraph(parent)
{

}

void JKQTPFilledCurveYGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPFilledCurveYGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    p.setJoinStyle(Qt::RoundJoin);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=static_cast<int>(qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows()));
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QPainterPath pl, pf;

    //double xold=-1;
    double yold=-1;
    double x0=transformX(baseline);
    if (parent->getXAxis()->isLogAxis()) {
        if (baseline>0 && baseline>parent->getXAxis()->getMin()) x0=transformX(baseline);
        else x0=transformX(parent->getXAxis()->getMin());
    }
    /*double y0=transformY(baseline);
    if (parent->getYAxis()->isLogAxis()) {
        y0=transformY(parent->getYAxis()->getMin());
        if (baseline>0 && baseline>parent->getYAxis()->getMin()) y0=transformY(baseline);
        else y0=transformY(parent->getYAxis()->getMin());
    }*/
    bool first=true;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=transformX(xv); bool xok=JKQTPIsOKFloat(x);
            double y=transformY(yv); bool yok=JKQTPIsOKFloat(y);

            if (!first) {
                if (xok&&yok) {
                    pf.lineTo(x, y);
                    if (drawLine) pl.lineTo(x, y);
                    //xold=x;
                    yold=y;
                } else if (!xok&&yok){
                    pf.lineTo(x0, y);
                    if (drawLine) pl.lineTo(x0, y);
                    //xold=x0;
                    yold=y;
                }
            } else {
                if (xok&&yok) {
                    if (drawLine) pl.moveTo(x,y);
                    pf.moveTo(x0, y);
                    pf.lineTo(x, y);
                    //xold=x;
                    yold=y;
                    first=false;
                } else if (!xok&&yok) {
                    if (drawLine) pl.moveTo(x0,y);
                    pf.moveTo(x0, y);
                    //xold=x0;
                    yold=y;
                    first=false;
                }
            }
        }
    }
    pf.lineTo(x0, yold);
    pf.closeSubpath();
    if (drawLine) {
        painter.save();
        if (drawSelectionLine) {
            QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
            QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
            if (selectionLineColor!=Qt::transparent) {
                penSelection.setColor(selectionLineColor);
            }
            penSelection.setWidthF(penSelection.widthF()*3.0);
            painter.setPen(penSelection);
            painter.drawPath(pl);
        }

        painter.setPen(p);
        painter.drawPath(pl);
        painter.restore();
    }
    painter.setBrush(b);
    painter.setPen(np);
    painter.drawPath(pf);

    painter.restore();

    drawErrorsAfter(painter);
}









QPen JKQTPFilledCurveXGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}


QBrush JKQTPFilledCurveXGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}


JKQTPFilledCurveXErrorGraph::JKQTPFilledCurveXErrorGraph(JKQTBasePlotter *parent):
    JKQTPFilledCurveXGraph(parent), JKQTPYGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

JKQTPFilledCurveXErrorGraph::JKQTPFilledCurveXErrorGraph(JKQTPlotter *parent):
    JKQTPFilledCurveXGraph(parent), JKQTPYGraphErrors()
{
    setErrorColorFromGraphColor(color);
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
    JKQTPFilledCurveYGraph(parent), JKQTPXGraphErrors()
{
    setErrorColorFromGraphColor(color);
}

JKQTPFilledCurveYErrorGraph::JKQTPFilledCurveYErrorGraph(JKQTPlotter *parent):
    JKQTPFilledCurveYGraph(parent), JKQTPXGraphErrors()
{
    setErrorColorFromGraphColor(color);
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
    JKQTPXYGraph(parent)
{
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    drawLine=true;
    fillStyle=Qt::SolidPattern;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        fillColor=color.lighter();
    }
}

JKQTPFilledVerticalRangeGraph::JKQTPFilledVerticalRangeGraph(JKQTPlotter *parent):
    JKQTPXYGraph(parent)
{
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    drawLine=true;
    fillStyle=Qt::SolidPattern;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
        fillColor=color.lighter();
    }
}

bool JKQTPFilledVerticalRangeGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=static_cast<int>(qMin(qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows()), datastore->getColumn(static_cast<size_t>(yColumn2)).getRows()));
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        double yv2=datastore->get(static_cast<size_t>(yColumn2),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(yv)) {
            if (start || yv>maxy) maxy=yv;
            if (start || yv<miny) miny=yv;
            double xvsgz;
            xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
        if (JKQTPIsOKFloat(yv2)) {
            if (start || yv2>maxy) maxy=yv2;
            if (start || yv2<miny) miny=yv2;
            double xvsgz;
            xvsgz=yv2; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPFilledVerticalRangeGraph::usesColumn(int column) const
{
    return JKQTPXYGraph::usesColumn(column)||(column==yColumn2);
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

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=static_cast<int>(qMin(qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows()), datastore->getColumn(static_cast<size_t>(yColumn2)).getRows()));
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    // upper points are added to poly_all, lower points to plow
    // then plow points are added to poly_all in vewerse order
    // then the whole thing is drawn
    QPolygonF poly_all, phigh, plow;

    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        double yv2=datastore->get(static_cast<size_t>(yColumn2),static_cast<size_t>(i));
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv)) {
            double x=transformX(xv); bool xok=JKQTPIsOKFloat(x);
            double y=transformY(yv); bool yok=JKQTPIsOKFloat(y);
            double y2=transformY(yv2); bool y2ok=JKQTPIsOKFloat(y2);

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
    painter.setBrush(b);
    painter.setPen(np);
    painter.drawPolygon(poly_all);

    painter.restore();
    if (drawLine) {
        painter.save();

        if (drawSelectionLine) {
            QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
            QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
            if (selectionLineColor!=Qt::transparent) {
                penSelection.setColor(selectionLineColor);
            }
            penSelection.setWidthF(penSelection.widthF()*3.0);
            painter.setPen(penSelection);
            painter.drawPolyline(phigh);
            painter.drawPolyline(plow);
        }


        painter.setPen(p);
        painter.drawPolyline(phigh);
        painter.drawPolyline(plow);
        painter.restore();
    }

    drawErrorsAfter(painter);
}

void JKQTPFilledVerticalRangeGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save();
    QRectF r=rect;
    r.setHeight(r.height()/2.0);
    r.moveTo(r.x(), r.y()+r.height()-1);
    painter.fillRect(r, getBrush(painter));
    if (drawLine) {
        painter.setPen(getLinePen(painter));
        painter.drawLine(QLineF(r.topLeft(), r.topRight()));
    }
    painter.restore();
}

QColor JKQTPFilledVerticalRangeGraph::getKeyLabelColor()
{
    return color;
}

QBrush JKQTPFilledVerticalRangeGraph::getBrush(JKQTPEnhancedPainter &/*painter*/) const
{
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPFilledVerticalRangeGraph::getLinePen(JKQTPEnhancedPainter &painter) const
{
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}
