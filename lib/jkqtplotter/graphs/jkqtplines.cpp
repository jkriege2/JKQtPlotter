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



#include "jkqtplotter/graphs/jkqtplines.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <QMarginsF>
#include <iostream>
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;











JKQTPXYLineGraph::JKQTPXYLineGraph(JKQTPlotter* parent):
    JKQTPXYLineGraph(parent->getPlotter())
{
}

JKQTPXYLineGraph::JKQTPXYLineGraph(JKQTBasePlotter* parent):
    JKQTPXYGraph(parent),
    drawLine(true),
    drawLineInForeground(true)
{
    sortData=JKQTPXYGraph::Unsorted;

    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
    initSymbolStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
}

void JKQTPXYLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPXYLineGraph::draw");
#endif
    if (parent==nullptr) return;
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //qDebug()<<"JKQTPXYLineGraph::draw();";

    drawErrorsBefore(painter);
    {
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<1;
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<2;

        const QPen p=getLinePen(painter, parent);
        const QPen penSelection=getHighlightingLinePen(painter, parent);
        const auto symType=getSymbolType();
        const double xmin=transformX(parent->getXAxis()->getMin());
        const double xmax=transformX(parent->getXAxis()->getMax());
        const double ymin=transformY(parent->getYAxis()->getMin());
        const double ymax=transformY(parent->getYAxis()->getMax());
        const double symbolSize=parent->pt2px(painter, getSymbolSize());
        const QMarginsF clipMargins=(symType==JKQTPNoSymbol)?QMarginsF(0,0,0,0):QMarginsF(symbolSize,symbolSize,symbolSize,symbolSize);
        const QRectF cliprect=QRectF(qMin(xmin,xmax),qMin(ymin,ymax),fabs(xmax-xmin),fabs(ymax-ymin))+clipMargins;


        int imax=0;
        int imin=0;
        if (getIndexRange(imin, imax)) {


            QVector<QPointF> symbols;
            QList<QPolygonF> vec_linesP;
            vec_linesP.push_back(QPolygonF());
            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                const int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                const double x=transformX(xv);
                const double y=transformY(yv);
                //qDebug()<<"JKQTPXYLineGraph::draw(): (xv, yv) =    ( "<<xv<<", "<<yv<<" )";
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {

                    //if (isHighlighted() && getSymbolType()!=JKQTPNoSymbol) {
                        //JKQTPPlotSymbol(painter, x, y, JKQTPFilledCircle, parent->pt2px(painter, symbolSize*1.5), parent->pt2px(painter, symbolWidth*parent->getLineWidthMultiplier()), penSelection.color(), penSelection.color(),getSymbolFont());
                    //}
                    if ((!parent->getXAxis()->isLogAxis() || xv>0.0) && (!parent->getYAxis()->isLogAxis() || yv>0.0) ) {
                        if (symType!=JKQTPNoSymbol && cliprect.contains(x,y)) {
                            if (drawLineInForeground) plotStyledSymbol(parent, painter, x, y);
                            else symbols.push_back({x,y});
                        }
                        if (drawLine) {
                            vec_linesP.last() << QPointF(x,y);
                        }
                    } else {
                        if (drawLine) {
                            if (vec_linesP.size()==0 || vec_linesP.last().size()>0)
                            vec_linesP.push_back(QPolygonF());
                        }
                    }
                }
            }
            //qDebug()<<"JKQTPXYLineGraph::draw(): "<<4<<" lines="<<lines.size();
            //qDebug()<<"JKQTPXYLineGraph::draw(): "<<5<<"  p="<<painter.pen();
            if (drawLine) {
                //qDebug()<<"JKQTPXYLineGraph::draw(): vec_linesP.size()=="<<vec_linesP.size();

                QList<QPolygonF> linesToDraw;
                if (getUseNonvisibleLineCompression()) linesToDraw=JKQTPClipPolyLines(JKQTPSimplifyPolyLines(vec_linesP, getNonvisibleLineCompressionAgressiveness()), cliprect);
                else linesToDraw=JKQTPClipPolyLines(vec_linesP, cliprect);
                //qDebug()<<"JKQTPXYLineGraph::draw(): linesToDraw.size()=="<<linesToDraw.size()<<", clip: x="<<xmin<<".."<<xmax<<", y="<<ymin<<".."<<ymax;
                for (const auto &linesP : linesToDraw) {
                    //qDebug()<<"JKQTPXYLineGraph::draw():   linesPFromV.size()=="<<linesPFromV.size()<<"   useNonvisibleLineCompression="<<getUseNonvisibleLineCompression();
                    //qDebug()<<"JKQTPXYLineGraph::draw():     --> linesP.size()=="<<linesP.size();
                    if (linesP.size()>0) {
                        if (isHighlighted()) {
                            painter.setPen(penSelection);
                            painter.drawPolylineFast(linesP);
                        }
                        painter.setPen(p);
                        painter.drawPolylineFast(linesP);
                    }
                }
            }
            for (const auto& s: symbols) {
                plotStyledSymbol(parent, painter, s.x(), s.y());
            }
            //qDebug()<<"JKQTPXYLineGraph::draw(): "<<6;
        }
    }
    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<7;
    drawErrorsAfter(painter);
    //qDebug()<<"JKQTPXYLineGraph::draw() ... done";
}

void JKQTPXYLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setWidthF(getKeyLineWidthPx(painter,rect,parent));
    painter.setPen(p);
    double y=rect.top()+rect.height()/2.0;
    if (drawLine && !drawLineInForeground) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    JKQTPPlotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, getSymbolType(), getKeySymbolSizePx(painter, rect, parent), getKeySymbolLineWidthPx(painter, rect, parent), getKeyLabelColor(), getSymbolFillColor(),getSymbolFont());
    if (drawLine && drawLineInForeground) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
}

QColor JKQTPXYLineGraph::getKeyLabelColor() const {
    return getSymbolColor();
}

void JKQTPXYLineGraph::setDrawLine(bool __value)
{
    this->drawLine = __value;
}

bool JKQTPXYLineGraph::getDrawLine() const
{
    return this->drawLine;
}

void JKQTPXYLineGraph::setDrawLineInForeground(bool __value)
{
    drawLineInForeground=__value;
}

bool JKQTPXYLineGraph::getDrawLineInForeground() const
{
    return drawLineInForeground;
}

void JKQTPXYLineGraph::setColor(QColor c)
{
    setLineColor(c);
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.fillColorDerivationMode, c));
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
}



JKQTPXYLineErrorGraph::JKQTPXYLineErrorGraph(JKQTBasePlotter *parent):
    JKQTPXYLineGraph(parent)
{
    setErrorColorFromGraphColor(getSymbolColor());
    initErrorStyle(parent, parentPlotStyle);
}

JKQTPXYLineErrorGraph::JKQTPXYLineErrorGraph(JKQTPlotter *parent):
    JKQTPXYLineErrorGraph(parent->getPlotter())
{

}

bool JKQTPXYLineErrorGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero) {
    if (xErrorColumn<0 || xErrorStyle==JKQTPNoError) {
        return JKQTPXYLineGraph::getXMinMax(minx, maxx, smallestGreaterZero);
    } else {
        bool start=true;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        if (parent==nullptr) return false;

        const JKQTPDatastore* datastore=parent->getDatastore();
        int imax=0;
        int imin=0;
        if (getIndexRange(imin, imax)) {
            for (int i=imin; i<imax; i++) {
                double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))+getXErrorU(i, datastore);
                if (JKQTPIsOKFloat(xv)) {
                    if (start || xv>maxx) maxx=xv;
                    if (start || xv<minx) minx=xv;
                    const double xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                    start=false;
                }
                xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))-getXErrorL(i, datastore);
                if (JKQTPIsOKFloat(xv)) {
                    if (start || xv>maxx) maxx=xv;
                    if (start || xv<minx) minx=xv;
                    const double xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                    start=false;
                }
            }
            return !start;
        }
    }
    return false;
}

bool JKQTPXYLineErrorGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero) {
    if (yErrorColumn<0 || yErrorStyle==JKQTPNoError) {
        return JKQTPXYLineGraph::getYMinMax(miny, maxy, smallestGreaterZero);
    } else {
        bool start=true;
        miny=0;
        maxy=0;
        smallestGreaterZero=0;

        if (parent==nullptr) return false;

        const JKQTPDatastore* datastore=parent->getDatastore();
        int imax=0;
        int imin=0;
        if (getIndexRange(imin, imax)) {
            for (int i=imin; i<imax; i++) {
                double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))+getYErrorU(i, datastore);
                if (JKQTPIsOKFloat(yv)) {
                    if (start || yv>maxy) maxy=yv;
                    if (start || yv<miny) miny=yv;
                    const double xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                    start=false;
                }
                yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))-getYErrorL(i, datastore);
                if (JKQTPIsOKFloat(yv)) {
                    if (start || yv>maxy) maxy=yv;
                    if (start || yv<miny) miny=yv;
                    const double xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                    start=false;
                }
            }
            return !start;
        }
    }
    return false;
}

bool JKQTPXYLineErrorGraph::usesColumn(int c) const
{
    return JKQTPXYLineGraph::usesColumn(c)||JKQTPXYGraphErrors::errorUsesColumn(c);
}

void JKQTPXYLineErrorGraph::drawErrorsBefore(JKQTPEnhancedPainter &painter)
{
    intSortData();
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);

}










