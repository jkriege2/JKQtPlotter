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



#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
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
    JKQTPXYGraph(parent)
{
    sortData=JKQTPXYGraph::Unsorted;
    drawLine=true;

    initLineStyle(parent, parentPlotStyle);
    initSymbolStyle(parent, parentPlotStyle);
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
    {
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<1;
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<2;

        QPen p=getLinePen(painter, parent);


        QPen penSelection=getHighlightingLinePen(painter, parent);


        int imax= static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));
        int imin=0;

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
        std::vector<QPolygonF> vec_linesP;
        vec_linesP.push_back(QPolygonF());
        intSortData();
        for (int iii=imin; iii<imax; iii++) {
            int i=qBound(imin, getDataIndex(iii), imax);
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            double x=transformX(xv);
            double y=transformY(yv);
            //qDebug()<<"JKQTPXYLineGraph::draw(): (xv, yv) =    ( "<<xv<<", "<<yv<<" )";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {

                if (isHighlighted() && getSymbolType()!=JKQTPNoSymbol) {
                    //JKQTPPlotSymbol(painter, x, y, JKQTPFilledCircle, parent->pt2px(painter, symbolSize*1.5), parent->pt2px(painter, symbolWidth*parent->getLineWidthMultiplier()), penSelection.color(), penSelection.color());
                }
                if ((!parent->getXAxis()->isLogAxis() || xv>0.0) && (!parent->getYAxis()->isLogAxis() || yv>0.0) ) {
                    plotStyledSymbol(parent, painter, x, y);
                    if (drawLine) {
                        vec_linesP[vec_linesP.size()-1] << QPointF(x,y);
                    }
                } else {
                    vec_linesP.push_back(QPolygonF());
                }
            }
        }
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<4<<" lines="<<lines.size();
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<5<<"  p="<<painter.pen();
        for (auto &linesP : vec_linesP) {
            if (linesP.size()>0) {
                if (isHighlighted()) {
                    painter.setPen(penSelection);
                    //painter.drawLines(lines);
                    painter.drawPolyline(linesP);
                }
                painter.setPen(p);
                //painter.drawLines(lines);
                painter.drawPolyline(linesP);
            }
        }
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<6;
    }
    //qDebug()<<"JKQTPXYLineGraph::draw(): "<<7;
    drawErrorsAfter(painter);
    //qDebug()<<"JKQTPXYLineGraph::draw() ... done";
}

void JKQTPXYLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    const double minSize=qMin(rect.width(), rect.height());
    const double maxSize=qMax(rect.width(), rect.height());
    double symbolSize=parent->pt2px(painter, this->getSymbolSize());
    if (symbolSize>minSize*0.9) symbolSize=minSize*0.9;
    double symbolWidth=parent->pt2px(painter, this->getSymbolLineWidth()*parent->getLineWidthMultiplier());
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;
    double lineWidth=parent->pt2px(painter, this->getLineWidth()*parent->getLineWidthMultiplier());
    if (lineWidth>0.5*maxSize) lineWidth=0.5*maxSize;

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setColor(getKeyLabelColor());
    p.setStyle(getLineStyle());
    p.setWidthF(lineWidth);
    painter.setPen(p);
    double y=rect.top()+rect.height()/2.0;
    if (drawLine) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    JKQTPPlotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, getSymbolType(), symbolSize, symbolWidth, getKeyLabelColor(), getSymbolFillColor());

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

void JKQTPXYLineGraph::setColor(QColor c)
{
    setLineColor(c);
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
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

        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax= static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));

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
        int imax= static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));

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
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);

}


















JKQTPXYParametrizedScatterGraph::JKQTPXYParametrizedScatterGraph(JKQTBasePlotter *parent):
    JKQTPXYLineGraph(parent),
    JKQTPColorPaletteStyleAndToolsMixin(parent)
{
    sizeColumn=-1;
    colorColumn=-1;
    symbolColumn=-1;
    linewidthColumn=-1;
    palette=JKQTPMathImageMATLAB;
    colorColumnContainsRGB=false;
    setSymbolType(JKQTPFilledCircle);
    drawLine=false;

    gridModeForSymbolSize=false;
    gridDeltaX=1;
    gridDeltaY=1;
    gridSymbolFractionSize=0.9;
    symbolFillDerivationMode=JKQTPFFCMLighterColor;
    if (parent) {
        symbolFillDerivationMode=parent->getCurrentPlotterStyle().graphFillColorDerivationMode;
    }

    clearSizeColumnFunctor();
    clearSymbolColumnFunctor();
    clearLinewidthColumnFunctor();

}

JKQTPXYParametrizedScatterGraph::JKQTPXYParametrizedScatterGraph(JKQTPlotter *parent):
    JKQTPXYParametrizedScatterGraph(parent->getPlotter())
{

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

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

    QPen p=getLinePen(painter, parent);


    QPen penSelection=getHighlightingLinePen(painter, parent);


    int imax= static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));
    int imin=0;
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
    {
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
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
                QColor symbFillColor=JKQTPGetDerivedColor(symbolFillDerivationMode, symbColor);
                //qDebug()<<i<<symbolSize<<symbColor;
                if (drawLine) {
                    linesP<<QPointF(x,y);
                }
                if (first && drawLine) {
                    double xl1=xold;
                    double yl1=yold;
                    double xl2=x;
                    double yl2=y;

                    if (isHighlighted()) {
                        if (colorColumn>=0) linecolss<<symbColor.lighter();
                        else linecolss<<getHighlightingLineColor();
                    }
                    linecols<<symbColor;
                    lines<<QLineF(xl1, yl1, xl2, yl2);
                    linewidths<<lineW;
                }

                if ((!parent->getXAxis()->isLogAxis() || xv>0.0) && (!parent->getYAxis()->isLogAxis() || yv>0.0) ) {
                    if (isHighlighted() && getSymbolType()!=JKQTPNoSymbol && symbolColumn<0) {
                        JKQTPPlotSymbol(painter, x, y, JKQTPFilledCircle,symbSize, parent->pt2px(painter, getSymbolLineWidth()*parent->getLineWidthMultiplier()), penSelection.color(), penSelection.color());
                    } else {
                        JKQTPPlotSymbol(painter, x, y, getLocalSymbolType(i), symbSize, parent->pt2px(painter, getSymbolLineWidth()*parent->getLineWidthMultiplier()), symbColor, symbFillColor);
                    }
                }


                xold=x;
                yold=y;
                first=true;
            }
        }
    }



    if (lines.size()>0) {
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
        if (isHighlighted()) {
            QPen pp=penSelection;
            if (colorColumn>=0) {
                for (int i=0; i<lines.size(); i++) {
                    pp.setColor(linecolss.value(i, getHighlightingLineColor()));
                    painter.setPen(pp);
                    painter.drawLine(lines[i]);
                }
            } else {
                pp.setColor(getHighlightingLineColor());
                painter.setPen(pp);
                painter.drawPolyline(linesP);
            }
        }
        QPen pp=p;
        if (colorColumn>=0 || linewidthColumn>=0) {
            for (int i=0; i<lines.size(); i++) {
                pp.setColor(linecols.value(i, getLineColor()));
                pp.setWidthF(linewidths.value(i, parent->pt2px(painter, getLineWidth()*parent->getLineWidthMultiplier())));
                painter.setPen(pp);
                painter.drawLine(lines[i]);
            }
        } else {
            pp.setColor(getHighlightingLineColor());
            painter.setPen(pp);
            painter.drawPolyline(linesP);
        }

    }

    drawErrorsAfter(painter);
}

void JKQTPXYParametrizedScatterGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    const double minSize=qMin(rect.width(), rect.height());
    const double maxSize=qMax(rect.width(), rect.height());
    double symbolSize1=parent->pt2px(painter, this->getSymbolSize());
    if (symbolSize1>minSize*0.9) symbolSize1=minSize*0.9;
    if (symbolSize1<minSize*0.6) symbolSize1=minSize*0.6;
    double symbolSize2=parent->pt2px(painter, this->getSymbolSize()*0.75);
    if (symbolSize2>minSize*0.6) symbolSize2=minSize*0.5;
    if (symbolSize2<minSize*0.5) symbolSize2=minSize*0.5;

    if (sizeColumn<0) {
        symbolSize2=symbolSize1;
    }

    QColor color1=getKeyLabelColor();
    QColor color2=getKeyLabelColor();

    JKQTPGraphSymbols symbol1=getSymbolType();
    JKQTPGraphSymbols symbol2=getSymbolType();

    if (colorColumn>=0) {
        if (colorColumnContainsRGB) {
            color1=QColor("red");
            color2=QColor("blue");
        } else {
            QImage img;
            double colorval[]={0,1};
            JKQTPImageTools::array2image<double>(colorval, 2, 1, img, getPalette(), double(0.0), double(1.0));
            color1=img.pixel(0,0);
            color2=img.pixel(1,0);
        }
    }

    if (symbolColumn>=0) {
        symbol1=JKQTPFilledCircle;
        symbol2=JKQTPFilledRect;
    }

    double symbolWidth=parent->pt2px(painter, this->getSymbolLineWidth()*0.7*parent->getLineWidthMultiplier());
    if (symbolWidth>0.2*getSymbolLineWidth()) symbolWidth=0.3*getSymbolLineWidth();
    double lineWidth=parent->pt2px(painter, this->getSymbolLineWidth()*0.7*parent->getLineWidthMultiplier());
    if (lineWidth>0.1*maxSize) lineWidth=0.1*maxSize;

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(color1);
    p.setStyle(getLineStyle());
    p.setWidthF(lineWidth);
    painter.setPen(p);
    double x1=rect.left()+symbolSize1/2.0;
    double y1=rect.top()+symbolSize1/2.0;
    double x2=rect.right()-symbolSize2/2.0;
    double y2=rect.bottom()-symbolSize2/2.0;
    JKQTPPlotSymbol(painter, x1, y1, symbol1, symbolSize1, symbolWidth, color1, JKQTPGetDerivedColor(symbolFillDerivationMode, color1));
    JKQTPPlotSymbol(painter, x2, y2, symbol2, symbolSize2, symbolWidth, color2, JKQTPGetDerivedColor(symbolFillDerivationMode, color2));
    if (drawLine) painter.drawLine(QLineF(x1,y1, x2,y2));

}

QColor JKQTPXYParametrizedScatterGraph::getKeyLabelColor() const
{
    return getLocalColor(-1);
}

void JKQTPXYParametrizedScatterGraph::setSizeColumn(int __value)
{
    this->sizeColumn = __value;
}

int JKQTPXYParametrizedScatterGraph::getSizeColumn() const
{
    return this->sizeColumn;
}

void JKQTPXYParametrizedScatterGraph::setSizeColumnFunctor(JKQTPXYParametrizedScatterGraph::FunctorToSize ff)
{
    m_toSizePtFunctor=ff;
}

void JKQTPXYParametrizedScatterGraph::clearSizeColumnFunctor()
{
    m_toSizePtFunctor=[](double /*x*/, double /*y*/, double sizecolumn)->double {return sizecolumn; };
}

JKQTPXYParametrizedScatterGraph::FunctorToSize JKQTPXYParametrizedScatterGraph::getSizeColumnFunctor()
{
    return m_toSizePtFunctor;
}

void JKQTPXYParametrizedScatterGraph::setSizeColumn(size_t __value) {
    this->sizeColumn = static_cast<int>(__value);
}

void JKQTPXYParametrizedScatterGraph::setColorColumn(int __value)
{
    this->colorColumn = __value;
}

int JKQTPXYParametrizedScatterGraph::getColorColumn() const
{
    return this->colorColumn;
}

void JKQTPXYParametrizedScatterGraph::setColorColumn(size_t __value) {
    this->colorColumn = static_cast<int>(__value);
}

void JKQTPXYParametrizedScatterGraph::setSymbolColumn(int __value)
{
    this->symbolColumn = __value;
}

int JKQTPXYParametrizedScatterGraph::getSymbolColumn() const
{
    return this->symbolColumn;
}

void JKQTPXYParametrizedScatterGraph::setSymbolColumn(size_t __value) {
    this->symbolColumn = static_cast<int>(__value);
}

void JKQTPXYParametrizedScatterGraph::setSymbolColumnFunctor(JKQTPXYParametrizedScatterGraph::FunctorToSymbol ff)
{
    m_toSymbolFunctor=ff;
}

void JKQTPXYParametrizedScatterGraph::clearSymbolColumnFunctor()
{

    m_toSymbolFunctor=std::bind([](double /*x*/, double /*y*/, double symbolcolumn, JKQTPXYParametrizedScatterGraph* th)->JKQTPGraphSymbols {
        int id=static_cast<int>(floor(symbolcolumn));
        if (id<0) return th->getSymbolType();
        return JKQTPGraphSymbols(id%(JKQTPMaxSymbolID+1));

    }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, this);
}


JKQTPXYParametrizedScatterGraph::MappedSymbolFunctor::MappedSymbolFunctor(const QMap<double, JKQTPGraphSymbols> &mapping_, JKQTPXYParametrizedScatterGraph* graph):
    mapping(mapping_), parent(graph)
{

}

JKQTPGraphSymbols JKQTPXYParametrizedScatterGraph::MappedSymbolFunctor::operator()(double , double , double symcolumn) const
{
    if (mapping.size()<=0) return parent->getSymbolType();
    if (mapping.size()==1) return mapping.first();
    if (symcolumn<=mapping.firstKey()) return mapping.first();
    if (symcolumn>=mapping.lastKey()) return mapping.last();
    auto it=mapping.begin();
    it++;
    auto itlast=it;
    for (; it!=mapping.end(); ++it) {
        itlast=it;
        itlast--;
        if (symcolumn>=itlast.key() && symcolumn<=it.key()) {
            if (fabs(symcolumn-itlast.key())<fabs(symcolumn-it.key())) {
                return itlast.value();
            } else {
                return it.value();
            }
        }
    }
    return itlast.value();
}

void JKQTPXYParametrizedScatterGraph::setMappedSymbolColumnFunctor(const QMap<double, JKQTPGraphSymbols> &symmap)
{
    m_toSymbolFunctor=MappedSymbolFunctor(symmap, this);
}

JKQTPXYParametrizedScatterGraph::FunctorToSymbol JKQTPXYParametrizedScatterGraph::getSymbolColumnFunctor()
{
    return m_toSymbolFunctor;
}

void JKQTPXYParametrizedScatterGraph::setLinewidthColumn(int __value)
{
    this->linewidthColumn = __value;
}

int JKQTPXYParametrizedScatterGraph::getLinewidthColumn() const
{
    return this->linewidthColumn;
}

void JKQTPXYParametrizedScatterGraph::setLinewidthColumn(size_t __value) {
    this->linewidthColumn = static_cast<int>(__value);
}

void JKQTPXYParametrizedScatterGraph::setLinewidthColumnFunctor(JKQTPXYParametrizedScatterGraph::FunctorToWidth ff)
{
    m_toWidthPtFunctor=ff;
}

void JKQTPXYParametrizedScatterGraph::clearLinewidthColumnFunctor()
{
    m_toWidthPtFunctor=[](double /*x*/, double /*y*/, double widthcolumn)->double {return widthcolumn; };
}

JKQTPXYParametrizedScatterGraph::FunctorToWidth JKQTPXYParametrizedScatterGraph::getLinewidthColumnFunctor()
{
    return m_toWidthPtFunctor;
}

void JKQTPXYParametrizedScatterGraph::setColorColumnContainsRGB(bool __value)
{
    this->colorColumnContainsRGB = __value;
}

bool JKQTPXYParametrizedScatterGraph::getColorColumnContainsRGB() const
{
    return this->colorColumnContainsRGB;
}

void JKQTPXYParametrizedScatterGraph::setGridModeForSymbolSize(bool __value)
{
    this->gridModeForSymbolSize = __value;
}

bool JKQTPXYParametrizedScatterGraph::getGridModeForSymbolSize() const
{
    return this->gridModeForSymbolSize;
}

void JKQTPXYParametrizedScatterGraph::setGridDeltaX(double __value)
{
    this->gridDeltaX = __value;
}

double JKQTPXYParametrizedScatterGraph::getGridDeltaX() const
{
    return this->gridDeltaX;
}

void JKQTPXYParametrizedScatterGraph::setGridDeltaY(double __value)
{
    this->gridDeltaY = __value;
}

double JKQTPXYParametrizedScatterGraph::getGridDeltaY() const
{
    return this->gridDeltaY;
}

void JKQTPXYParametrizedScatterGraph::setGridSymbolFractionSize(double __value)
{
    this->gridSymbolFractionSize = __value;
}

double JKQTPXYParametrizedScatterGraph::getGridSymbolFractionSize() const
{
    return this->gridSymbolFractionSize;
}

JKQTPColorDerivationMode JKQTPXYParametrizedScatterGraph::getSymbolFillDerivationMode() const
{
    return this->symbolFillDerivationMode;
}

void JKQTPXYParametrizedScatterGraph::setSymbolFillDerivationMode(JKQTPColorDerivationMode m)
{
    this->symbolFillDerivationMode=m;
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
        int imax= static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));
        int imin=0;
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


double JKQTPXYParametrizedScatterGraph::getLocalSymbolSize(int i)
{
    if (parent==nullptr) return getSymbolSize();
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return getSymbolSize();
    if (sizeColumn<0) return getSymbolSize();
    if (i>=(int64_t)datastore->getRows(sizeColumn)) return getSymbolSize();
    return m_toSizePtFunctor(datastore->get(xColumn,i), datastore->get(yColumn,i), datastore->get(sizeColumn,i));
}

double JKQTPXYParametrizedScatterGraph::getLocalLineWidth(int i)
{
    if (parent==nullptr) return getLineWidth();
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return getLineWidth();
    if (linewidthColumn<0) return getLineWidth();
    if (i>=(int64_t)datastore->getRows(linewidthColumn)) return getLineWidth();
    return m_toWidthPtFunctor(datastore->get(xColumn,i), datastore->get(yColumn,i), datastore->get(linewidthColumn,i));
}

QColor JKQTPXYParametrizedScatterGraph::getLocalColor(int i) const
{
    if (parent==nullptr) return getLineColor();
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return getLineColor();
    if (colorColumn<0) return getLineColor();
    if (colorColumnContainsRGB) {
        if (i<0 || i>=(int64_t)datastore->getRows(colorColumn)) return getLineColor();
        //QRgb rgb=
        return QRgb(round(datastore->get(colorColumn,i)));
    } else {
        QImage img;
        double colorval=0;
        if (i>=0 && i<(int64_t)datastore->getRows(colorColumn)) colorval=datastore->get(colorColumn,i);
        double colMin=0;
        double colMax=0;
        if (intColMin==intColMax) {
            colMin=0;
            colMax=datastore->getRows(colorColumn)-1;
        } else {
            colMin=intColMin;
            colMax=intColMax;
        }
        JKQTPImageTools::array2image(&colorval, 1, 1, img, palette, colMin, colMax);
        return img.pixel(0,0);
    }

}

JKQTPGraphSymbols JKQTPXYParametrizedScatterGraph::getLocalSymbolType(int i)
{
    if (parent==nullptr) return getSymbolType();
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return getSymbolType();
    if (symbolColumn<0) return getSymbolType();
    if (i>=static_cast<int64_t>(datastore->getRows(symbolColumn))) return getSymbolType();
    return m_toSymbolFunctor(datastore->get(xColumn,i), datastore->get(yColumn,i), datastore->get(symbolColumn,i));

}


JKQTPXYParametrizedErrorScatterGraph::JKQTPXYParametrizedErrorScatterGraph(JKQTBasePlotter *parent):
    JKQTPXYParametrizedScatterGraph(parent)
{
    setErrorColorFromGraphColor(getSymbolColor());
    initErrorStyle(parent, parentPlotStyle);
}

JKQTPXYParametrizedErrorScatterGraph::JKQTPXYParametrizedErrorScatterGraph(JKQTPlotter *parent):
    JKQTPXYParametrizedErrorScatterGraph(parent->getPlotter())
{

}

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
        int imax= static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));

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
        int imax= static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));

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
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);
}

bool JKQTPXYParametrizedErrorScatterGraph::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, JKQTBasePlotter * /*parent*/, JKQTPGraph* /*parentGraph*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPErrorPlotstyle /*xErrorStyle*/, JKQTPErrorPlotstyle /*yErrorStyle*/, int index, QColor &errorLineColor, QColor &errorFillColor)
{
    QColor c=getLocalColor(index);
    errorLineColor=c.darker();
    errorFillColor=c.lighter();
    //qDebug()<<"intPlotXYErrorIndicatorsGetColor("<<index<<"): "<<errorColor;
    return true;
}



