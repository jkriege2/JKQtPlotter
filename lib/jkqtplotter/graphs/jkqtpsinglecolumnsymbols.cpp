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



#include "jkqtplotter/graphs/jkqtpsinglecolumnsymbols.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include <random>

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;




JKQTPSingleColumnSymbolsGraph::JKQTPSingleColumnSymbolsGraph(JKQTBasePlotter *parent):
    JKQTPSingleColumnGraph(parent), seedValue(123456)
{
    parentPlotStyle=-1;
    dataDirection=DataDirection::Y;
    position=0;
    width=1;

    initSymbolStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
}



JKQTPSingleColumnSymbolsGraph::JKQTPSingleColumnSymbolsGraph(JKQTPlotter *parent):
    JKQTPSingleColumnSymbolsGraph(parent->getPlotter())
{

}


bool JKQTPSingleColumnSymbolsGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    //std::cout<<"JKQTPSingleColumnSymbolsGraph::getXMinMax(minx="<<minx<<", maxx="<<maxx<<", smallestGreaterZero="<<smallestGreaterZero<<")\n";
    if (dataDirection==DataDirection::X) {
        bool res= getDataMinMax(dataColumn, minx, maxx, smallestGreaterZero);
        //std::cout<<"JKQTPSingleColumnSymbolsGraph::getXMinMax(minx="<<minx<<", maxx="<<maxx<<", smallestGreaterZero="<<smallestGreaterZero<<"): dataDirection==DataDirection::X -->"<<std::boolalpha<<res<<"\n";
        return res;
    } else {
        minx=position;
        maxx=position;
        if (positionScatterStyle!=NoScatter) {
            minx=position-width/2;
            maxx=position+width/2;
        }
        ///std::cout<<"JKQTPSingleColumnSymbolsGraph::getXMinMax(minx="<<minx<<", maxx="<<maxx<<", smallestGreaterZero="<<smallestGreaterZero<<"): dataDirection!=DataDirection::X -->"<<std::boolalpha<<true<<"\n";
        return true;
        //smallestGreaterZero=qMax(double(0.0), qMin(baseline, baseline+peakHeight));
    }
}

bool JKQTPSingleColumnSymbolsGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    //std::cout<<"JKQTPSingleColumnSymbolsGraph::getYMinMax(miny="<<miny<<", maxy="<<maxy<<", smallestGreaterZero="<<smallestGreaterZero<<")\n";
    if (dataDirection==DataDirection::Y) {
        bool res= getDataMinMax(dataColumn, miny, maxy, smallestGreaterZero);
        //std::cout<<"JKQTPSingleColumnSymbolsGraph::getYMinMax(miny="<<miny<<", maxy="<<maxy<<", smallestGreaterZero="<<smallestGreaterZero<<"): dataDirection!=DataDirection::Y -->"<<std::boolalpha<<res<<"\n";
        return res;
    } else {
        miny=position;
        maxy=position;
        if (positionScatterStyle!=NoScatter) {
            miny=position-width/2;
            maxy=position+width/2;
        }
        //std::cout<<"JKQTPSingleColumnSymbolsGraph::getYMinMax(miny="<<miny<<", maxy="<<maxy<<", smallestGreaterZero="<<smallestGreaterZero<<"): dataDirection!=DataDirection::Y -->"<<std::boolalpha<<true<<" position="<<position<<", width="<<width<<"\n";
        return true;
        //smallestGreaterZero=qMax(double(0.0), qMin(baseline, baseline+peakHeight));
    }
}

void JKQTPSingleColumnSymbolsGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPSingleColumnSymbolsGraph::draw");
#endif
    clearHitTestData();

    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;
    if (dataColumn<0) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        int imax=static_cast<int>(datastore->getRows(static_cast<size_t>(dataColumn)));
        int imin=0;
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;
        reserveHitTestData(imax-imin);

        std::random_device rd; // random number generators:
        std::minstd_rand gen{rd()};
        gen.seed(seedValue);
        std::uniform_real_distribution<> dRandomScatter{position-width/2.0, position+width/2.0};

        const double symSize=parent->pt2px(painter, getSymbolSize());
        QPen p=painter.pen();
        p.setColor(getSymbolColor());
        p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,  parent->pt2px(painter, getSymbolLineWidth())));
        p.setStyle(Qt::SolidLine);
        p.setCapStyle(Qt::FlatCap);


        QVector<QPointF> plotSymbols; // collects symbol locations e.g. for BeeSwarmScatter-plots
        plotSymbols.reserve(qMax(100,imax-imin));
        auto doesCollide=[&plotSymbols,&symSize](double x, double y)->bool {
            for (auto& p: plotSymbols) {
                if (fabs(p.x()-x)<symSize && fabs(p.y()-y)<symSize) {
                    return true;
                }
            }
            return false;
        };

        if (dataDirection==DataDirection::X) {
            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                int i=qBound<int>(imin, getDataIndex(static_cast<int>(iii)), imax);
                const double xv=datastore->get(dataColumn,i);
                double yv=position;
                if (positionScatterStyle==RandomScatter) {
                    yv=dRandomScatter(gen);
                }
                const double x=transformX(xv);
                double y=transformY(yv);
                if (positionScatterStyle==BeeSwarmScatter) {
                    while (doesCollide(x,y)) {
                        if (i%2==0) {
                            y=y-symSize;
                        } else {
                            y=y+symSize;
                        }
                    }
                }
                plotSymbols.append(QPointF(x,y));
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                    if (positionScatterStyle!=RugPlot) {
                        plotStyledSymbol(parent, painter, x, y);
                    } else {
                        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
                        painter.setPen(p);
                        painter.drawLine(QLineF(x, y-symSize,x,y+symSize));
                    }
                    addHitTestData(xv, yv,iii, datastore);
                }
            }
        } else {
            intSortData();
            for (int iii=imin; iii<imax; iii++) {
                int i=qBound<int>(imin, getDataIndex(static_cast<int>(iii)), imax);
                double xv=position;
                if (positionScatterStyle==RandomScatter) {
                    xv=dRandomScatter(gen);
                }
                const double yv=datastore->get(dataColumn,i);
                double x=transformX(xv);
                const double y=transformY(yv);
                if (positionScatterStyle==BeeSwarmScatter) {
                    while (doesCollide(x,y)) {
                        if (i%2==0) {
                            x=x-symSize;
                        } else {
                            x=x+symSize;
                        }
                    }
                }
                plotSymbols.append(QPointF(x,y));
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                    if (positionScatterStyle!=RugPlot) {
                        plotStyledSymbol(parent, painter, x, y);
                    } else {
                        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
                        painter.setPen(p);
                        painter.drawLine(QLineF(x-symSize, y,x+symSize,y));
                    }
                    addHitTestData(xv, yv,iii, datastore);
                }
            }
        }



    }

    drawErrorsAfter(painter);
}

void JKQTPSingleColumnSymbolsGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    const double minSize=qMin(rect.width(), rect.height());
    //const double maxSize=qMax(rect.width(), rect.height());
    double symbolSize=parent->pt2px(painter, this->getSymbolSize());
    if (symbolSize>minSize*0.9) symbolSize=minSize*0.9;
    double symbolWidth=parent->pt2px(painter, this->getSymbolLineWidth()*parent->getLineWidthMultiplier());
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getSymbolPen(painter, parent);
    painter.setPen(p);
    if (positionScatterStyle!=RugPlot) {
        JKQTPPlotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, getSymbolType(), symbolSize, symbolWidth, getKeyLabelColor(), getSymbolFillColor(),getSymbolFont());
    } else {
        painter.translate(rect.center());
        if (dataDirection==DataDirection::X) {
            painter.rotate(90);
        }
        painter.drawLine(QLineF(-rect.width()/4.0, rect.height()*(-0.4), rect.width()/4.0, rect.height()*(-0.4)));
        painter.drawLine(QLineF(-rect.width()/4.0, rect.height()*(-0.2), rect.width()/4.0, rect.height()*(-0.2)));
        painter.drawLine(QLineF(-rect.width()/4.0, rect.height()*0.0, rect.width()/4.0, rect.height()*0.0));
        painter.drawLine(QLineF(-rect.width()/4.0, rect.height()*0.05, rect.width()/4.0, rect.height()*0.05));
        painter.drawLine(QLineF(-rect.width()/4.0, rect.height()*0.15, rect.width()/4.0, rect.height()*0.15));
        painter.drawLine(QLineF(-rect.width()/4.0, rect.height()*0.3, rect.width()/4.0, rect.height()*0.3));
        painter.drawLine(QLineF(-rect.width()/4.0, rect.height()*0.45, rect.width()/4.0, rect.height()*0.45));
    }

}

QColor JKQTPSingleColumnSymbolsGraph::getKeyLabelColor() const
{
    return getSymbolColor();
}

void JKQTPSingleColumnSymbolsGraph::setColor(QColor col)
{
    setSymbolColor(col);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.fillColorDerivationMode, col));
}

void JKQTPSingleColumnSymbolsGraph::setPosition(double __value)
{
    this->position = __value;
}

double JKQTPSingleColumnSymbolsGraph::getPosition() const
{
    return this->position;
}

void JKQTPSingleColumnSymbolsGraph::setWidth(double __value)
{
    width=__value;
}

double JKQTPSingleColumnSymbolsGraph::getWidth() const
{
    return width;
}

void JKQTPSingleColumnSymbolsGraph::setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::ScatterStyle __value)
{
    this->positionScatterStyle = __value;
}

JKQTPSingleColumnSymbolsGraph::ScatterStyle JKQTPSingleColumnSymbolsGraph::getPositionScatterStyle() const
{
    return this->positionScatterStyle;
}

void JKQTPSingleColumnSymbolsGraph::setSeedValue(unsigned int val) {
    seedValue=val;
}

unsigned int JKQTPSingleColumnSymbolsGraph::getSeedValue() const {
    return seedValue;
}



