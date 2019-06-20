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



#include "jkqtplotter/jkqtpgraphsfilledcurve.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;







JKQTPFilledCurveXGraph::JKQTPFilledCurveXGraph(JKQTBasePlotter* parent):
    JKQTPSpecialLineHorizontalGraph(parent)
{
    setDrawLine(true);
    setDrawSymbols(false);
    setFillCurve(true);
    setSpecialLineType(JKQTPDirectLine);
}

JKQTPFilledCurveXGraph::JKQTPFilledCurveXGraph(JKQTPlotter *parent):
    JKQTPFilledCurveXGraph(parent->getPlotter())
{

}



JKQTPFilledCurveYGraph::JKQTPFilledCurveYGraph(JKQTBasePlotter* parent):
    JKQTPSpecialLineVerticalGraph(parent)
{
    setDrawLine(true);
    setDrawSymbols(false);
    setFillCurve(true);
    setSpecialLineType(JKQTPDirectLine);
}

JKQTPFilledCurveYGraph::JKQTPFilledCurveYGraph(JKQTPlotter *parent):
    JKQTPFilledCurveYGraph(parent->getPlotter())
{

}



JKQTPFilledCurveXErrorGraph::JKQTPFilledCurveXErrorGraph(JKQTBasePlotter *parent):
    JKQTPFilledCurveXGraph(parent)
{
    setErrorColorFromGraphColor(getLineColor());
    initErrorStyle(parent, parentPlotStyle);
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
    initErrorStyle(parent, parentPlotStyle);

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
    JKQTPXYGraph(parent)
{
    drawLine=true;
    initFillStyle(parent, parentPlotStyle);
    initLineStyle(parent, parentPlotStyle);
}

JKQTPFilledVerticalRangeGraph::JKQTPFilledVerticalRangeGraph(JKQTPlotter *parent):
    JKQTPFilledVerticalRangeGraph(parent->getPlotter())
{

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
    int imax=static_cast<int>(qMin(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))), datastore->getRows(static_cast<size_t>(yColumn2))));
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
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        QPen ps=getHighlightingLinePen(painter, parent);
        QPen np(Qt::NoPen);

        QBrush b=getFillBrush(painter, parent);

        int imax=static_cast<int>(qMin(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))), datastore->getRows(static_cast<size_t>(yColumn2))));
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


        if (drawLine) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

            if (isHighlighted()) {

                painter.setPen(ps);
                painter.drawPolyline(phigh);
                painter.drawPolyline(plow);
            }


            painter.setPen(p);
            painter.drawPolyline(phigh);
            painter.drawPolyline(plow);

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
    if (drawLine) {
        painter.setPen(getLinePen(painter, parent));
        painter.drawLine(QLineF(r.topLeft(), r.topRight()));
    }

}

QColor JKQTPFilledVerticalRangeGraph::getKeyLabelColor() const
{
    return getLineColor();
}

void JKQTPFilledVerticalRangeGraph::setYColumn2(int __value)
{
    this->yColumn2 = __value;
}

int JKQTPFilledVerticalRangeGraph::getYColumn2() const
{
    return this->yColumn2;
}

void JKQTPFilledVerticalRangeGraph::setYColumn2(size_t __value) {
    this->yColumn2 = static_cast<int>(__value);
}

void JKQTPFilledVerticalRangeGraph::setDrawLine(bool __value)
{
    drawLine=__value;
}

bool JKQTPFilledVerticalRangeGraph::getDrawLine() const
{
    return drawLine;
}
