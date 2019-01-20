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



#include "jkqtplotter/jkqtpgraphspeakstream.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;




JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(JKQTBasePlotter *parent):
    JKQTPSingleColumnGraph(parent)
{
    baseline=0;
    yPeaks=true;
    peakHeight=1;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQTBasePlotter *parent):
    JKQTPSingleColumnGraph(dataColumn, color, Qt::SolidLine, 2.0, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQTBasePlotter *parent):
    JKQTPSingleColumnGraph(dataColumn, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
}


JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(JKQTPlotter *parent):
    JKQTPSingleColumnGraph(parent)
{
    baseline=0;
    yPeaks=true;
    peakHeight=1;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQTPlotter *parent):
    JKQTPSingleColumnGraph(dataColumn, color, Qt::SolidLine, 2.0, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
}

JKQTPPeakStreamGraph::JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQTPlotter *parent):
    JKQTPSingleColumnGraph(dataColumn, parent)
{
    yPeaks=true;
    this->baseline=baseline;
    this->peakHeight=peakHeight;
    drawBaseline=true;
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

    painter.save();

    QPen p=getLinePen(painter);
    p.setCapStyle(Qt::FlatCap);

    int imax=static_cast<int>(datastore->getColumn(static_cast<size_t>(dataColumn)).getRows());
    int imin=0;
    // interpret data ranges
    if (datarange_start>-1) {
        imin=qMin(datarange_start, static_cast<int>(imax));
    }
    if (datarange_end>-1) {
        imax=qMin(datarange_end, static_cast<int>(imax));
    }
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
            const double xv=datastore->get(dataColumn,i);
            if (JKQTPIsOKFloat(xv)) {
                lines<<QLineF(transform(xv, baseline), transform(xv, baseline+peakHeight));
            }
        }
    } else {
        if (drawBaseline) {
            lines<<QLineF(transform(baseline, parent->getYMin()), transform(baseline, parent->getYMax()));
        }
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound<int>(imin, getDataIndex(iii), imax);
            const double yv=datastore->get(dataColumn,i);
            if (JKQTPIsOKFloat(yv)) {
                lines<<QLineF(transform(baseline, yv), transform(baseline+peakHeight, yv));
            }
        }
    }

    painter.setPen(p);
    if (lines.size()>0) painter.drawLines(lines);


    painter.restore();

    drawErrorsAfter(painter);
}

void JKQTPPeakStreamGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save();
    QPen p=getLinePen(painter);
    painter.setPen(p);
    if (yPeaks) {
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,qMin(parent->pt2px(painter, p.widthF()), rect.width()/10.0)));
        if (drawBaseline) {
            if (peakHeight>=0) painter.drawLine(rect.bottomLeft(), rect.bottomRight());
            else painter.drawLine(rect.topLeft(), rect.topRight());
        }
        painter.drawLine(QPointF(rect.left()+rect.width()*0.1, rect.top()),QPointF( rect.left()+rect.width()*0.1, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.55, rect.top()), QPointF(rect.left()+rect.width()*0.55, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.75, rect.top()), QPointF(rect.left()+rect.width()*0.75, rect.bottom()));
        painter.drawLine(QPointF(rect.left()+rect.width()*0.9, rect.top()), QPointF(rect.left()+rect.width()*0.9, rect.bottom()));
    } else {
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,qMin(parent->pt2px(painter, p.widthF()), rect.height()/15.0)));
        if (drawBaseline) {
            if (peakHeight>=0) painter.drawLine(rect.bottomLeft(), rect.topLeft());
            else painter.drawLine(rect.bottomRight(), rect.topRight());
        }
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.1), QPointF(rect.right(), rect.top()+rect.height()*0.1));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.55), QPointF(rect.right(), rect.top()+rect.height()*0.55));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.75), QPointF(rect.right(), rect.top()+rect.height()*0.75));
        painter.drawLine(QPointF(rect.left(), rect.top()+rect.height()*0.9), QPointF(rect.right(), rect.top()+rect.height()*0.9));
    }

    painter.restore();
}






