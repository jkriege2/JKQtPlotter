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



#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;











JKQTPXYLineGraph::JKQTPXYLineGraph(JKQTPlotter* parent):
    JKQTPXYGraph(parent)
{
    sortData=JKQTPXYGraph::Unsorted;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    symbolSize=12;
    symbolWidth=1;
    symbol=JKQTPNoSymbol;
    drawLine=true;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
    fillColor=color;
}

JKQTPXYLineGraph::JKQTPXYLineGraph(JKQTBasePlotter* parent):
    JKQTPXYGraph(parent)
{
    sortData=JKQTPXYGraph::Unsorted;
    drawSelectionLine=false;
    selectionLineColor=Qt::transparent;
    color=QColor("red");
    style=Qt::SolidLine;
    lineWidth=2;
    parentPlotStyle=-1;
    symbolSize=12;
    symbolWidth=1;
    symbol=JKQTPNoSymbol;
    drawLine=true;

    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
    fillColor=color;
}

void JKQTPXYLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPXYLineGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //qDebug()<<"JKQTPXYLineGraph::draw();";

    drawErrorsBefore(painter);

    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<1;
    painter.save();
    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<2;

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);


    QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
    QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
    if (selectionLineColor!=Qt::transparent) {
        penSelection.setColor(selectionLineColor);
    }
    penSelection.setWidthF(penSelection.widthF()*3.0);


    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<3<<" imin="<<imin<<" imax="<<imax;
    //double xold=-1;
    //double yold=-1;
    //bool first=false;
    //QVector<QLineF> lines;
    QPolygonF linesP;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        double x=transformX(xv);
        double y=transformY(yv);
        //qDebug()<<"JKQTPXYLineGraph::draw(): (xv, yv) =    ( "<<xv<<", "<<yv<<" )";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
            QColor symbColor=color;
            QColor symbFillColor=fillColor;// symbColor.lighter();

            if (drawSelectionLine && symbol!=JKQTPNoSymbol) {
                JKQTPPlotSymbol(painter, x, y, JKQTPFilledCircle, parent->pt2px(painter, symbolSize*1.5), parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), penSelection.color(), penSelection.color());
            }
            JKQTPPlotSymbol(painter, x, y, symbol, parent->pt2px(painter, symbolSize), parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), symbColor, symbFillColor);
            /*if (drawLine && first) {
                double xl1=xold;
                double yl1=yold;
                double xl2=x;
                double yl2=y;
                lines.append(QLineF(xl1, yl1, xl2, yl2));
            }*/
            if (drawLine) {
                linesP<<QPointF(x,y);

            }
//            xold=x;
//            yold=y;
//            first=true;
        }
    }
    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<4<<" lines="<<lines.size();
    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<5<<"  p="<<painter.pen();
    if (linesP.size()>0) {
        if (drawSelectionLine) {
            painter.setPen(penSelection);
            //painter.drawLines(lines);
            painter.drawPolyline(linesP);
        }
        painter.setPen(p);
        //painter.drawLines(lines);
        painter.drawPolyline(linesP);
    }
    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<6;
    painter.restore();
    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<7;
    drawErrorsAfter(painter);
    //qDebug()<<"JKQTPXYLineGraph::draw() ... done";
}

void JKQTPXYLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    const double minSize=qMin(rect.width(), rect.height());
    const double maxSize=qMax(rect.width(), rect.height());
    double symbolSize=parent->pt2px(painter, this->symbolSize);
    if (symbolSize>minSize*0.9) symbolSize=minSize*0.9;
    double symbolWidth=parent->pt2px(painter, this->symbolWidth*parent->get_lineWidthMultiplier());
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;
    double lineWidth=parent->pt2px(painter, this->lineWidth*parent->get_lineWidthMultiplier());
    if (lineWidth>0.5*maxSize) lineWidth=0.5*maxSize;

    painter.save();
    QPen p=painter.pen();
    p.setColor(getKeyLabelColor());
    p.setStyle(style);
    p.setWidthF(lineWidth);
    painter.setPen(p);
    double y=rect.top()+rect.height()/2.0;
    if (drawLine) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    JKQTPPlotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, symbol, symbolSize, symbolWidth, getKeyLabelColor(), fillColor);
    painter.restore();
}

QColor JKQTPXYLineGraph::getKeyLabelColor() {
    return color;
}


JKQTPXYLineErrorGraph::JKQTPXYLineErrorGraph(JKQTBasePlotter *parent):
    JKQTPXYLineGraph(parent), JKQTPXYGraphErrors()
{ setErrorColorFromGraphColor(color); }

JKQTPXYLineErrorGraph::JKQTPXYLineErrorGraph(JKQTPlotter *parent):
    JKQTPXYLineGraph(parent), JKQTPXYGraphErrors()
{ setErrorColorFromGraphColor(color);  }

bool JKQTPXYLineErrorGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero) {
    if (xErrorColumn<0 || xErrorStyle==JKQTPNoError) {
        return JKQTPXYLineGraph::getXMinMax(minx, maxx, smallestGreaterZero);
    } else {
        bool start=true;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        if (parent==nullptr) return false;

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

        for (int i=imin; i<imax; i++) {
            double xvsgz;
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))+getXErrorU(i, datastore);
            if (JKQTPIsOKFloat(xv)) {
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))-getXErrorL(i, datastore);
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
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

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))+getYErrorU(i, datastore);
            if (JKQTPIsOKFloat(yv)) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))-getYErrorL(i, datastore);
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

bool JKQTPXYLineErrorGraph::usesColumn(int c) const
{
    return JKQTPXYLineGraph::usesColumn(c)||JKQTPXYGraphErrors::errorUsesColumn(c);
}

void JKQTPXYLineErrorGraph::drawErrorsBefore(JKQTPEnhancedPainter &painter)
{
    intSortData();
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

}














JKQTPStepHorizontalGraph::JKQTPStepHorizontalGraph(JKQTBasePlotter* parent):
    JKQTPXYGraph(parent)
{
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
    drawLine=true;
    fillCurve=true;
    stepType=JKQTPStepLeft;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
}


JKQTPStepHorizontalGraph::JKQTPStepHorizontalGraph(JKQTPlotter* parent):
    JKQTPXYGraph(parent)
{
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
    drawLine=true;
    fillCurve=true;
    stepType=JKQTPStepLeft;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
    }
}


void JKQTPStepHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
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
    int y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (drawLine) painter.setPen(p);
    painter.setBrush(b);
    if (fillCurve) painter.drawRect(rect);
    if (!fillCurve & drawLine) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    painter.restore();
}

QColor JKQTPStepHorizontalGraph::getKeyLabelColor() {
    return color;
}


void JKQTPStepHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPStepHorizontalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

    QPainterPath pl, pf;

    double xold=-1;
    double yold=-1;
//    double xstart=-1;
//    double ystart=-1;
    //double x0=transformX(0);
    //if (parent->get_xAxis()->isLogAxis()) x0=transformX(parent->get_xAxis()->getMin());
    double y0=transformY(0);
    if (parent->get_yAxis()->isLogAxis()) y0=transformY(parent->get_yAxis()->getMin());
    bool subsequentItem=false;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=transformX(xv);
            double y=transformY(yv);
            if (subsequentItem) {
                //double xl1=xold;
                //double yl1=yold;
                //double xl2=x;
                //double yl2=y;

                if (stepType==JKQTPStepCenter) {
                    double d=(x-xold);
                    pf.lineTo(xold+d/2.0, yold);
                    pf.lineTo(xold+d/2.0, y);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(xold+d/2.0, yold);
                        pl.lineTo(xold+d/2.0, y);
                        pl.lineTo(x, y);
                    }
                } else if (stepType==JKQTPStepLeft) {
                    pf.lineTo(xold, y);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(xold, y);
                        pl.lineTo(x, y);
                    }
                } else if (stepType==JKQTPStepRight) {
                    pf.lineTo(x, yold);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(x, yold);
                        pl.lineTo(x, y);
                    }
                }

                //std::cout<<"line ("<<xl1<<", "<<yl1<<") -- ("<<xl2<<", "<<yl2<<")"<<std::endl;
            } else {
                if (drawLine) pl.moveTo(x,y);
                pf.moveTo(x, y0);
                pf.lineTo(x, y);
                //xstart=x;
                //ystart=y0;
            }
            xold=x;
            yold=y;
            subsequentItem=true;
        }
    }
    if (fillCurve) {
        pf.lineTo(xold, y0);
        pf.closeSubpath();
    }
    painter.save();

    if (fillCurve) {
        painter.fillPath(pf, b);
    }

    if (drawLine) {
        painter.setBrush(QBrush(Qt::transparent));
        painter.setPen(p);
        painter.drawPath(pl);
    }
    painter.restore();

    drawErrorsAfter(painter);
}











JKQTPStepVerticalGraph::JKQTPStepVerticalGraph(JKQTBasePlotter *parent):
    JKQTPStepHorizontalGraph(parent)
{
}

JKQTPStepVerticalGraph::JKQTPStepVerticalGraph(JKQTPlotter *parent):
    JKQTPStepHorizontalGraph(parent)
{

}

void JKQTPStepVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPStepVerticalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

    QPainterPath pl, pf;

    double xold=-1;
    double yold=-1;
    double x0=transformX(0);
    if (parent->get_xAxis()->isLogAxis()) x0=transformX(parent->get_xAxis()->getMin());
    //double y0=transformY(0);
    //if (parent->get_yAxis()->isLogAxis()) y0=transformY(parent->get_yAxis()->getMin());
    bool first=false;
    intSortData();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=transformX(xv);
            double y=transformY(yv);
            if (first) {
                //double xl1=xold;
                //double yl1=yold;
                //double xl2=x;
                //double yl2=y;

                if (stepType==JKQTPStepCenter) {
                    double d=(y-yold);
                    pf.lineTo(xold, yold+d/2.0);
                    pf.lineTo(x, yold+d/2.0);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(xold, yold+d/2.0);
                        pl.lineTo(x, yold+d/2.0);
                        pl.lineTo(x, y);
                    }
                } else if (stepType==JKQTPStepLeft) {
                    pf.lineTo(x, yold);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(x, yold);
                        pl.lineTo(x, y);
                    }
                } else if (stepType==JKQTPStepRight) {
                    pf.lineTo(xold, y);
                    pf.lineTo(x, y);
                    if (drawLine) {
                        pl.lineTo(xold, y);
                        pl.lineTo(x, y);
                    }
                }

                //std::cout<<"line ("<<xl1<<", "<<yl1<<") -- ("<<xl2<<", "<<yl2<<")"<<std::endl;
            } else {
                if (drawLine) pl.moveTo(x,y);
                pf.moveTo(x0, y);
                pf.lineTo(x, y);
            }
            xold=x;
            yold=y;
            first=true;
        }
    }
    pf.lineTo(x0, yold);
    pf.closeSubpath();
    painter.save();
    if (drawLine) {
        painter.setPen(p);
        painter.drawPath(pl);
    }
    painter.setBrush(b);
    painter.setPen(np);
    painter.drawPath(pf);

    painter.restore();

    drawErrorsAfter(painter);
}














JKQTPHorizontalRange::JKQTPHorizontalRange(JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    color=QColor("red").darker();
    fillColor=QColor("red").lighter();
    style=Qt::DotLine;
    lineWidth=1;
    fillStyle=Qt::SolidPattern;
    centerColor=QColor("red");
    centerStyle=Qt::SolidLine;
    centerLineWidth=2;
    sizeMin=0;
    sizeMax=1;
    unlimitedSizeMin=true;
    unlimitedSizeMax=true;
    invertedRange=false;



    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        centerColor=parent->getPlotStyle(parentPlotStyle).color();
        color=centerColor.darker();
        fillColor=centerColor.lighter();
    }
    fillColor.setAlphaF(0.5);

    rangeMin=rangeMax=0;
    rangeCenter=0;
    plotCenterLine=true;
    plotRange=true;
    plotRangeLines=true;
    fillRange=true;
}


JKQTPHorizontalRange::JKQTPHorizontalRange(JKQTPlotter* parent):
    JKQTPGraph(parent)
{
    color=QColor("red").darker();
    fillColor=QColor("red").lighter();
    style=Qt::DotLine;
    lineWidth=1;
    fillStyle=Qt::SolidPattern;
    centerColor=QColor("red");
    centerStyle=Qt::SolidLine;
    centerLineWidth=2;
    sizeMin=0;
    sizeMax=1;
    unlimitedSizeMin=true;
    unlimitedSizeMax=true;
    invertedRange=false;



    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        centerColor=parent->getPlotStyle(parentPlotStyle).color();
        color=centerColor.darker();
        fillColor=centerColor.lighter();
    }
    fillColor.setAlphaF(0.5);

    rangeMin=rangeMax=0;
    rangeCenter=0;
    plotCenterLine=true;
    plotRange=true;
    plotRangeLines=true;
    fillRange=true;
}
void JKQTPHorizontalRange::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPHorizontalRange::draw");
#endif
    double c=transformY(rangeCenter);
    double cmi=transformY(rangeMin);
    double cma=transformY(rangeMax);
    double smi=transformY(parent->get_yAxis()->getMin());
    double sma=transformY(parent->get_yAxis()->getMax());
    double mi=transformX(parent->get_xAxis()->getMin());
    double ma=transformX(parent->get_xAxis()->getMax());
    if (!unlimitedSizeMin) mi=transformX(sizeMin);
    if (!unlimitedSizeMax) ma=transformX(sizeMax);
    //std::cout<<"hor: rangeMin="<<rangeMin<<" -> "<<cmi<<std::endl;
    //std::cout<<"hor: rangeMax="<<rangeMax<<" -> "<<cma<<std::endl;
    painter.save();
    if (plotRange && (cmi!=cma) && JKQTPIsOKFloat(mi) && JKQTPIsOKFloat(ma)) {
        QPen p=painter.pen();
        QPen np(Qt::NoPen);
        p.setColor(color);
        p.setStyle(style);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
        QBrush nb(Qt::NoBrush);
        QBrush b=painter.brush();
        b.setColor(fillColor);
        b.setStyle(fillStyle);
        QRectF rect(QPointF(mi, cmi), QPointF(ma, cma));
        if (fillRange) {
            painter.setPen(np);
            painter.setBrush(b);
            if (invertedRange) {
                if (JKQTPIsOKFloat(cmi)) {
                    QRectF rectl(QPointF(mi, smi), QPointF(ma, cmi));
                    painter.drawRect(rectl);
                }
                if (JKQTPIsOKFloat(cma)) {
                    QRectF rectr(QPointF(mi, cma), QPointF(ma, sma));
                    painter.drawRect(rectr);
                }
            } else {
                if (JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) painter.drawRect(rect);
            }
        }

        //std::cout<<"hor: rangeMin.rect.top="<<rect.top()<<std::endl;
        //std::cout<<"hor: rangeMax.rect.bottom="<<rect.bottom()<<std::endl;

        if (plotRangeLines && JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) {
            painter.setPen(p);
            painter.setBrush(nb);
            painter.drawLine(rect.left(), rect.top(), rect.right(), rect.top());
            painter.drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
        }

    }
    if (plotCenterLine && JKQTPIsOKFloat(c) && JKQTPIsOKFloat(mi) && JKQTPIsOKFloat(ma)) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, centerLineWidth*parent->get_lineWidthMultiplier())));
        painter.setPen(p);
        painter.drawLine(QLineF(mi, c, ma, c));
    }
    painter.restore();
}

void JKQTPHorizontalRange::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush nb(Qt::NoBrush);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    if (plotRange) {
        painter.setPen(np);
        painter.setBrush(b);
        if (fillRange) painter.drawRect(rect);
        painter.setPen(p);
        painter.setBrush(nb);
        if (plotRangeLines) {
            painter.drawLine(rect.left(), rect.top(), rect.right(), rect.top());
            painter.drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
        }
    }

    if (plotCenterLine) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        painter.setPen(p);
        int y=rect.top()+rect.height()/2.0;
        painter.drawLine(rect.left(), y, rect.right(), y);
    }
    painter.restore();
}

bool JKQTPHorizontalRange::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    smallestGreaterZero=minx=maxx=0;
    return false;
}

bool JKQTPHorizontalRange::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    miny=rangeMin;
    maxy=rangeMax;
    smallestGreaterZero=0;
    if (rangeMin>0) smallestGreaterZero=rangeMin;
    return true;
}

QColor JKQTPHorizontalRange::getKeyLabelColor()
{
    return color;
}

void JKQTPHorizontalRange::setDrawCenterLineOnly()
{
    plotCenterLine=true;
    plotRange=false;
}










JKQTPVerticalRange::JKQTPVerticalRange(JKQTBasePlotter* parent):
    JKQTPHorizontalRange(parent)
{
}

JKQTPVerticalRange::JKQTPVerticalRange(JKQTPlotter* parent):
    JKQTPHorizontalRange(parent)
{
}

void JKQTPVerticalRange::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPVerticalRange::draw");
#endif
    double c=transformX(rangeCenter);
    double cmi=transformX(rangeMin);
    double cma=transformX(rangeMax);
    double mi=transformY(parent->get_yAxis()->getMin());
    double ma=transformY(parent->get_yAxis()->getMax());
    double smi=transformX(parent->get_xAxis()->getMin());
    double sma=transformX(parent->get_xAxis()->getMax());
    if (!unlimitedSizeMin) mi=transformY(sizeMin);
    if (!unlimitedSizeMax) ma=transformY(sizeMax);
    painter.save();
    if (plotRange && (cmi!=cma) && JKQTPIsOKFloat(mi) && JKQTPIsOKFloat(ma)) {
        QPen p=painter.pen();
        QPen np(Qt::NoPen);
        p.setColor(color);
        p.setStyle(style);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
        QBrush nb(Qt::NoBrush);
        QBrush b=painter.brush();
        b.setColor(fillColor);
        b.setStyle(fillStyle);
        QRectF rect(QPointF(cmi, mi), QPointF(cma, ma));
        if (fillRange) {
            painter.setPen(np);
            painter.setBrush(b);
            if (invertedRange) {
                if (JKQTPIsOKFloat(cmi)) {
                    QRectF rectl(QPointF(smi,mi), QPointF(cmi,ma));
                    painter.drawRect(rectl);
                }
                if (JKQTPIsOKFloat(cma)) {
                    QRectF rectr(QPointF(cma,mi), QPointF(sma,ma));
                    painter.drawRect(rectr);
                }
            } else {
                if (JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) painter.drawRect(rect);
            }
        }
        if (plotRangeLines && JKQTPIsOKFloat(cmi) && JKQTPIsOKFloat(cma)) {
            painter.setPen(p);
            painter.setBrush(nb);
            painter.drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
            painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
        }

    }
    if (plotCenterLine&& JKQTPIsOKFloat(c) && JKQTPIsOKFloat(ma) && JKQTPIsOKFloat(mi)) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, centerLineWidth*parent->get_lineWidthMultiplier())));
        painter.setPen(p);
        painter.drawLine(QLineF(c, mi, c, ma));
    }
    painter.restore();
}

void JKQTPVerticalRange::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush nb(Qt::NoBrush);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    //int y=rect.top()+rect.height()/2.0;
    if (plotRange) {
        painter.setPen(np);
        painter.setBrush(b);
        if (fillRange) painter.drawRect(rect);
        painter.setPen(p);
        painter.setBrush(nb);
        if (plotRangeLines) {
            painter.drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
            painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
        }
    }
    if (plotCenterLine) {
        QPen p=painter.pen();
        p.setColor(centerColor);
        p.setStyle(centerStyle);
        painter.setPen(p);
        int y=rect.left()+rect.width()/2.0;
        painter.drawLine(y, rect.top(), y, rect.bottom());
    }
    painter.restore();
}

bool JKQTPVerticalRange::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    minx=rangeMin;
    maxx=rangeMax;
    smallestGreaterZero=0;
    if (rangeMin>0) smallestGreaterZero=rangeMin;
    return true;
}

bool JKQTPVerticalRange::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    smallestGreaterZero=miny=maxy=0;
    return false;
}


QPen JKQTPXYLineGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

QPen JKQTPXYLineGraph::getSymbolPen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*symbolWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

QBrush JKQTPXYLineGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    return b;
}


QBrush JKQTPStepHorizontalGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPStepHorizontalGraph::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

QBrush JKQTPHorizontalRange::getBrush(JKQTPEnhancedPainter& /*painter*/ ) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPHorizontalRange::getLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}


JKQTPXYParametrizedScatterGraph::JKQTPXYParametrizedScatterGraph(JKQTBasePlotter *parent):
    JKQTPXYLineGraph(parent),
    JKQTPColorPaletteTools(parent)
{
    sizeColumn=-1;
    colorColumn=-1;
    symbolColumn=-1;
    linewidthColumn=-1;
    palette=JKQTPMathImageMATLAB;
    colorColumnContainsRGB=false;
    symbol=JKQTPFilledCircle;
    drawLine=false;

    gridModeForSymbolSize=false;
    gridDeltaX=1;
    gridDeltaY=1;
    gridSymbolFractionSize=0.9;
}

JKQTPXYParametrizedScatterGraph::JKQTPXYParametrizedScatterGraph(JKQTPlotter *parent):
    JKQTPXYLineGraph(parent),
    JKQTPColorPaletteTools(parent->get_plotter())
{
    sizeColumn=-1;
    colorColumn=-1;
    symbolColumn=-1;
    linewidthColumn=-1;
    palette=JKQTPMathImageMATLAB;
    colorColumnContainsRGB=false;
    symbol=JKQTPFilledCircle;
    drawLine=false;

    gridModeForSymbolSize=false;
    gridDeltaX=1;
    gridDeltaY=1;
    gridSymbolFractionSize=0.9;
}

void JKQTPXYParametrizedScatterGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPXYParametrizedScatterGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    cbGetDataMinMax(intColMin, intColMax);

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);


    QPen penSelection=p; penSelection.setStyle(Qt::SolidLine);
    QColor selcol=penSelection.color(); selcol.setAlphaF(0.5); penSelection.setColor(selcol);
    if (selectionLineColor!=Qt::transparent) {
        penSelection.setColor(selectionLineColor);
    }
    penSelection.setWidthF(penSelection.widthF()*3.0);


    int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<3<<" imin="<<imin<<" imax="<<imax;
    double xold=-1;
    double yold=-1;
    bool first=false;
    QVector<QLineF> lines;
    QPolygonF linesP;
    QVector<QColor> linecols;
    QVector<QColor> linecolss;
    QVector<double> linewidths;
    intSortData();
    double specSymbSize=0;
    bool hasSpecSymbSize=false;
    painter.save();
    for (int iii=imin; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        double x=transformX(xv);
        double y=transformY(yv);
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
            double symbSize= parent->pt2px(painter, getLocalSymbolSize(i));
            double lineW= parent->pt2px(painter, getLocalLineWidth(i));

            if (gridModeForSymbolSize) {
                if (!hasSpecSymbSize) {
                    double sSX= fabs(transformX( xv+gridDeltaX*gridSymbolFractionSize/2.0)-transformX( xv-gridDeltaX*gridSymbolFractionSize/2.0));
                    double sSY= fabs(transformY( yv+gridDeltaY*gridSymbolFractionSize/2.0)-transformY( yv-gridDeltaY*gridSymbolFractionSize/2.0));
                    hasSpecSymbSize=true;
                    specSymbSize=qMin(sSX,sSY);
                }
                symbSize=specSymbSize;
            }
            QColor symbColor=getLocalColor(i);
            QColor symbFillColor=symbColor.lighter();
            //qDebug()<<i<<symbolSize<<symbColor;
            if (first && drawLine) {
                double xl1=xold;
                double yl1=yold;
                double xl2=x;
                double yl2=y;

                if (drawSelectionLine) {
                    if (colorColumn>=0) linecolss<<symbColor.lighter();
                    else linecolss<<selectionLineColor;
                }
                linecols<<symbColor;
                lines<<QLineF(xl1, yl1, xl2, yl2);
                linesP<<QPointF(x,y);
                linewidths<<lineW;
            }

            if (drawSelectionLine && symbol!=JKQTPNoSymbol && symbolColumn<0) {
                JKQTPPlotSymbol(painter, x, y, JKQTPFilledCircle,symbSize, parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), penSelection.color(), penSelection.color());
            } else {
                JKQTPPlotSymbol(painter, x, y, getLocalSymbolType(i), symbSize, parent->pt2px(painter, symbolWidth*parent->get_lineWidthMultiplier()), symbColor, symbFillColor);
            }


            xold=x;
            yold=y;
            first=true;
        }
    }
    painter.restore();

    if (lines.size()>0) {
        painter.save();
        if (drawSelectionLine) {
            QPen pp=penSelection;
            if (colorColumn>=0) {
                for (int i=0; i<lines.size(); i++) {
                    pp.setColor(linecolss.value(i, selectionLineColor));
                    painter.setPen(pp);
                    painter.drawLine(lines[i]);
                }
            } else {
                pp.setColor(selectionLineColor);
                painter.setPen(pp);
                painter.drawPolyline(linesP);
            }
        }
        QPen pp=p;
        if (colorColumn>=0 || linewidthColumn>=0) {
            for (int i=0; i<lines.size(); i++) {
                pp.setColor(linecols.value(i, color));
                pp.setWidthF(linewidths.value(i, lineWidth));
                painter.setPen(pp);
                painter.drawLine(lines[i]);
            }
        } else {
            pp.setColor(selectionLineColor);
            painter.setPen(pp);
            painter.drawPolyline(linesP);
        }
        painter.restore();
    }
    painter.restore();
    drawErrorsAfter(painter);
}

void JKQTPXYParametrizedScatterGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    const double minSize=qMin(rect.width(), rect.height());
    const double maxSize=qMax(rect.width(), rect.height());
    double symbolSize1=parent->pt2px(painter, this->symbolSize);
    if (symbolSize1>minSize*0.9) symbolSize1=minSize*0.9;
    if (symbolSize1<minSize*0.6) symbolSize1=minSize*0.6;
    double symbolSize2=parent->pt2px(painter, this->symbolSize*0.75);
    if (symbolSize2>minSize*0.6) symbolSize2=minSize*0.5;
    if (symbolSize2<minSize*0.5) symbolSize2=minSize*0.5;

    if (sizeColumn<0) {
        symbolSize2=symbolSize1;
    }

    QColor color1=getKeyLabelColor();
    QColor color2=getKeyLabelColor();

    JKQTPGraphSymbols symbol1=symbol;
    JKQTPGraphSymbols symbol2=symbol;

    if (colorColumn>=0) {
        if (colorColumnContainsRGB) {
            color1=QColor("red");
            color2=QColor("blue");
        } else {
            QImage img;
            double colorval[]={0,1};
            JKQTPImagePlot_array2image<double>(colorval, 2, 1, img, get_palette(), double(0.0), double(1.0));
            color1=img.pixel(0,0);
            color2=img.pixel(1,0);
        }
    }

    if (symbolColumn>=0) {
        symbol1=JKQTPFilledCircle;
        symbol2=JKQTPFilledRect;
    }

    double symbolWidth=parent->pt2px(painter, this->symbolWidth*0.7*parent->get_lineWidthMultiplier());
    if (symbolWidth>0.2*symbolSize) symbolWidth=0.3*symbolSize;
    double lineWidth=parent->pt2px(painter, this->lineWidth*0.7*parent->get_lineWidthMultiplier());
    if (lineWidth>0.1*maxSize) lineWidth=0.1*maxSize;

    painter.save();
    QPen p=painter.pen();
    p.setColor(color1);
    p.setStyle(style);
    p.setWidthF(lineWidth);
    painter.setPen(p);
    double x1=rect.left()+symbolSize1/2.0;
    double y1=rect.top()+symbolSize1/2.0;
    double x2=rect.right()-symbolSize2/2.0;
    double y2=rect.bottom()-symbolSize2/2.0;
    JKQTPPlotSymbol(painter, x1, y1, symbol1, symbolSize1, symbolWidth, color1, color1.lighter());
    JKQTPPlotSymbol(painter, x2, y2, symbol2, symbolSize2, symbolWidth, color2, color2.lighter());
    if (drawLine) painter.drawLine(x1,y1, x2,y2);
    painter.restore();
}

QColor JKQTPXYParametrizedScatterGraph::getKeyLabelColor()
{
    return getLocalColor(-1);
}

void JKQTPXYParametrizedScatterGraph::setParent(JKQTBasePlotter *parent)
{
    JKQTPXYLineGraph::setParent(parent);
    cbSetParent(parent);
}

void JKQTPXYParametrizedScatterGraph::getOutsideSize(JKQTPEnhancedPainter &painter, int &leftSpace, int &rightSpace, int &topSpace, int &bottomSpace)
{
    JKQTPXYLineGraph::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar&& colorColumn>=0 && !colorColumnContainsRGB) cbGetOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPXYParametrizedScatterGraph::drawOutside(JKQTPEnhancedPainter &painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace)
{
    JKQTPXYLineGraph::drawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar&& colorColumn>=0 && !colorColumnContainsRGB) cbDrawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPXYParametrizedScatterGraph::cbGetDataMinMax(double &dmin, double &dmax)
{
    if (autoImageRange) {
        dmin=dmax=0;
        if (parent==nullptr) return;
        JKQTPDatastore* datastore=parent->getDatastore();
        if (datastore==nullptr) return;
        if (colorColumn<0) return;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

        bool first=true;
        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(colorColumn,i);
            if (first) {
                dmin=dmax=xv;
                first=false;
            } else {
                dmin=qMin(xv, dmin);
                dmax=qMax(xv, dmax);
            }
        }

    } else {
        dmin=imageMin;
        dmax=imageMax;
    }

}

bool JKQTPXYParametrizedScatterGraph::usesColumn(int c) const
{
    return (c==colorColumn) || (c==sizeColumn) || (c==symbolColumn) || (c==linewidthColumn) || JKQTPXYLineGraph::usesColumn(c);
}

double JKQTPXYParametrizedScatterGraph::getInternalDataMin() const
{
    return 0;
}

double JKQTPXYParametrizedScatterGraph::getInternalDataMax() const
{
    return 0;
}

double JKQTPXYParametrizedScatterGraph::getLocalSymbolSize(int i)
{
    if (parent==nullptr) return symbolSize;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return symbolSize;
    if (sizeColumn<0) return symbolSize;
    if (i>=(int64_t)datastore->getColumn(sizeColumn).getRows()) return symbolSize;
    return datastore->get(sizeColumn,i);
}

double JKQTPXYParametrizedScatterGraph::getLocalLineWidth(int i)
{
    if (parent==nullptr) return lineWidth;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return lineWidth;
    if (linewidthColumn<0) return lineWidth;
    if (i>=(int64_t)datastore->getColumn(linewidthColumn).getRows()) return lineWidth;
    return datastore->get(linewidthColumn,i);
}

QColor JKQTPXYParametrizedScatterGraph::getLocalColor(int i)
{
    if (parent==nullptr) return color;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return color;
    if (colorColumn<0) return color;
    if (colorColumnContainsRGB) {
        if (i<0 || i>=(int64_t)datastore->getColumn(colorColumn).getRows()) return color;
        //QRgb rgb=
        return QRgb(round(datastore->get(colorColumn,i)));
    } else {
        QImage img;
        double colorval=0;
        if (i>=0 && i<(int64_t)datastore->getColumn(colorColumn).getRows()) colorval=datastore->get(colorColumn,i);
        double colMin=0;
        double colMax=0;
        if (intColMin==intColMax) {
            colMin=0;
            colMax=datastore->getColumn(colorColumn).getRows()-1;
        } else {
            colMin=intColMin;
            colMax=intColMax;
        }
        JKQTPImagePlot_array2image(&colorval, 1, 1, img, palette, colMin, colMax);
        return img.pixel(0,0);
    }

}

JKQTPGraphSymbols JKQTPXYParametrizedScatterGraph::getLocalSymbolType(int i)
{
    if (parent==nullptr) return symbol;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return symbol;
    if (symbolColumn<0) return symbol;
    if (i>=static_cast<int64_t>(datastore->getColumn(symbolColumn).getRows())) return symbol;
    int id=static_cast<int>(floor(datastore->get(symbolColumn,i)));
    if (id<0) return symbol;
    return JKQTPGraphSymbols(id%(JKQTPMaxSymbolID+1));

}


JKQTPXYParametrizedErrorScatterGraph::JKQTPXYParametrizedErrorScatterGraph(JKQTBasePlotter *parent):
    JKQTPXYParametrizedScatterGraph(parent), JKQTPXYGraphErrors()
{  setErrorColorFromGraphColor(color); }

JKQTPXYParametrizedErrorScatterGraph::JKQTPXYParametrizedErrorScatterGraph(JKQTPlotter *parent):
    JKQTPXYParametrizedScatterGraph(parent), JKQTPXYGraphErrors()
{ setErrorColorFromGraphColor(color);  }

bool JKQTPXYParametrizedErrorScatterGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    if (xErrorColumn<0 || xErrorStyle==JKQTPNoError) {
        return JKQTPXYLineGraph::getXMinMax(minx, maxx, smallestGreaterZero);
    } else {
        bool start=true;
        minx=0;
        maxx=0;
        smallestGreaterZero=0;

        if (parent==nullptr) return false;

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

bool JKQTPXYParametrizedErrorScatterGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (yErrorColumn<0 || yErrorStyle==JKQTPNoError) {
        return JKQTPXYLineGraph::getYMinMax(miny, maxy, smallestGreaterZero);
    } else {
        bool start=true;
        miny=0;
        maxy=0;
        smallestGreaterZero=0;

        if (parent==nullptr) return false;

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=qMin(datastore->getColumn(static_cast<size_t>(xColumn)).getRows(), datastore->getColumn(static_cast<size_t>(yColumn)).getRows());
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

        for (int i=imin; i<imax; i++) {
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))+getYErrorU(i, datastore);
            double yvv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))-getYErrorL(i, datastore);
            if (JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(yvv) ) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
                if (start || yvv>maxy) maxy=yvv;
                if (start || yvv<miny) miny=yvv;
                xvsgz=yvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
}

bool JKQTPXYParametrizedErrorScatterGraph::usesColumn(int c) const
{
    return JKQTPXYParametrizedScatterGraph::usesColumn(c)||JKQTPXYGraphErrors::errorUsesColumn(c);

}

void JKQTPXYParametrizedErrorScatterGraph::drawErrorsBefore(JKQTPEnhancedPainter &painter)
{
    intSortData();
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);
}

bool JKQTPXYParametrizedErrorScatterGraph::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, JKQTBasePlotter * /*parent*/, JKQTPGraph* /*parentGraph*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPErrorPlotstyle /*xErrorStyle*/, JKQTPErrorPlotstyle /*yErrorStyle*/, int index, QColor &/*errorColor*/, QColor &/*errorFillColor*/)
{
    QColor c=getLocalColor(index);
    errorColor=c.darker();
    errorFillColor=c.lighter();
    //qDebug()<<"intPlotXYErrorIndicatorsGetColor("<<index<<"): "<<errorColor;
    return true;
}



