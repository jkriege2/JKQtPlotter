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



#include "jkqtplotter/graphs/jkqtppeakstream.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;




JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(JKQTBasePlotter *parent):
    JKQTPSingleColumnGraph(parent)
{
    baseline=0;
    yPeaks=true;
    peakHeight=1;
    drawBaseline=true;
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
}



JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(JKQTPlotter *parent):
    JKQTPPeakStreamGraph(parent->getPlotter())
{
}


bool JKQTPPeakStreamGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    if (yPeaks) {
        return getDataMinMax(dataColumn, minx, maxx, smallestGreaterZero);
    } else {
        minx=qMin(baseline, baseline+peakHeight);
        maxx=qMax(baseline, baseline+peakHeight);
        return true;
        //smallestGreaterZero=qMax(double(0.0), qMin(baseline, baseline+peakHeight));
    }
}

bool JKQTPPeakStreamGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (!yPeaks) {
        return getDataMinMax(dataColumn, miny, maxy, smallestGreaterZero);
    } else {
        miny=qMin(baseline, baseline+peakHeight);
        maxy=qMax(baseline, baseline+peakHeight);
        return true;
        //smallestGreaterZero=qMax(double(0.0), qMin(baseline, baseline+peakHeight));
    }
}

void JKQTPPeakStreamGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPPeakStreamGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;
    if (dataColumn<0) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        p.setCapStyle(Qt::FlatCap);

        int imax=static_cast<int>(datastore->getRows(static_cast<size_t>(dataColumn)));
        int imin=0;
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;

        QVector<QLineF> lines;

        if (yPeaks) {
            if (drawBaseline) {
                lines<<QLineF(transform(parent->getXMin(), baseline), transform(parent->getXMax(), baseline));
            }
            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                int i=qBound<int>(imin, getDataIndex(static_cast<int>(iii)), imax);
                const double xv=datastore->get(dataColumn,static_cast<size_t>(i));
                if (JKQTPIsOKFloat(xv)) {
                    lines<<QLineF(transform(xv, baseline), transform(xv, baseline+peakHeight));
                    addHitTestData(xv, (baseline+peakHeight)/2.0,iii, datastore);
                }
            }
        } else {
            if (drawBaseline) {
                lines<<QLineF(transform(baseline, parent->getYMin()), transform(baseline, parent->getYMax()));
            }
            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                int i=qBound<int>(imin, getDataIndex(iii), imax);
                const double yv=datastore->get(dataColumn,static_cast<size_t>(i));
                if (JKQTPIsOKFloat(yv)) {
                    lines<<QLineF(transform(baseline, yv), transform(baseline+peakHeight, yv));
                    addHitTestData((baseline+peakHeight)/2.0, yv,iii, datastore);
                }
            }
        }

        painter.setPen(p);
        if (lines.size()>0) painter.drawLines(lines);


    }

    drawErrorsAfter(painter);
}

void JKQTPPeakStreamGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getKeyLinePen(painter, rect, parent);
    painter.setPen(p);
    if (yPeaks) {
        p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,qMin(p.widthF(), rect.width()/10.0)));
        if (drawBaseline) {
            if (peakHeight>=0) painter.drawLine(rect.bottomLeft(), rect.bottomRight());
            else painter.drawLine(rect.topLeft(), rect.topRight());
        }
        painter.drawLine(QPointF(rect.left()+rect.width()*0.1, rect.top()),QPointF( rect.left()+rect.width()*0.1, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.55, rect.top()), QPointF(rect.left()+rect.width()*0.55, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.75, rect.top()), QPointF(rect.left()+rect.width()*0.75, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.9, rect.top()), QPointF(rect.left()+rect.width()*0.9, rect.bottom()));
    } else {
        p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,qMin(p.widthF(), rect.height()/15.0)));
        if (drawBaseline) {
            if (peakHeight>=0) painter.drawLine(rect.bottomLeft(), rect.topLeft());
            else painter.drawLine(rect.bottomRight(), rect.topRight());
        }
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.1), QPointF(rect.right(), rect.top()+rect.height()*0.1));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.55), QPointF(rect.right(), rect.top()+rect.height()*0.55));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.75), QPointF(rect.right(), rect.top()+rect.height()*0.75));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.9), QPointF(rect.right(), rect.top()+rect.height()*0.9));
    }

    
}

QColor JKQTPPeakStreamGraph::getKeyLabelColor() const
{
    return getLineColor();
}

void JKQTPPeakStreamGraph::setColor(QColor col)
{
    setLineColor(col);
}

void JKQTPPeakStreamGraph::setBaseline(double __value)
{
    this->baseline = __value;
}

double JKQTPPeakStreamGraph::getBaseline() const
{
    return this->baseline;
}

void JKQTPPeakStreamGraph::setPeakHeight(double __value)
{
    this->peakHeight = __value;
}

double JKQTPPeakStreamGraph::getPeakHeight() const
{
    return this->peakHeight;
}

void JKQTPPeakStreamGraph::setYPeaks(bool __value)
{
    this->yPeaks = __value;
}

bool JKQTPPeakStreamGraph::getYPeaks() const
{
    return this->yPeaks;
}

void JKQTPPeakStreamGraph::setDrawBaseline(bool __value)
{
    this->drawBaseline = __value;
}

bool JKQTPPeakStreamGraph::getDrawBaseline() const
{
    return this->drawBaseline;
}






