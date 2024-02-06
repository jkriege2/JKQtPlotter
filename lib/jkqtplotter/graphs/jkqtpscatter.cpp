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



#include "jkqtplotter/graphs/jkqtpscatter.h"
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
















JKQTPXYScatterGraph::JKQTPXYScatterGraph(JKQTPlotter* parent):
    JKQTPXYScatterGraph(parent->getPlotter())
{
}

JKQTPXYScatterGraph::JKQTPXYScatterGraph(JKQTBasePlotter* parent):
    JKQTPXYGraph(parent)
{
    sortData=JKQTPXYGraph::Unsorted;

    initSymbolStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
}

void JKQTPXYScatterGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPXYScatterGraph::draw");
#endif
    if (parent==nullptr) return;
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //qDebug()<<"JKQTPXYScatterGraph::draw();";

    drawErrorsBefore(painter);
    {
        //qDebug()<<"JKQTPXYScatterGraph::draw(): "<<1;
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        //qDebug()<<"JKQTPXYScatterGraph::draw(): "<<2;

        const auto symType=getSymbolType();
        const double xmin=transformX(getXAxis()->getMin());
        const double xmax=transformX(getXAxis()->getMax());
        const double ymin=transformY(getYAxis()->getMin());
        const double ymax=transformY(getYAxis()->getMax());
        const double symbolSize=parent->pt2px(painter, getSymbolSize());
        const QMarginsF clipMargins=(symType==JKQTPNoSymbol)?QMarginsF(0,0,0,0):QMarginsF(symbolSize,symbolSize,symbolSize,symbolSize);
        const QRectF cliprect=QRectF(qMin(xmin,xmax),qMin(ymin,ymax),fabs(xmax-xmin),fabs(ymax-ymin))+clipMargins;


        int imax=0;
        int imin=0;
        if (getIndexRange(imin, imax)) {
            for (int iii=imin; iii<imax; iii++) {
                const int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                const double x=transformX(xv);
                const double y=transformY(yv);
                //qDebug()<<"JKQTPXYScatterGraph::draw(): (xv, yv) =    ( "<<xv<<", "<<yv<<" )";
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {

                    //if (isHighlighted() && getSymbolType()!=JKQTPNoSymbol) {
                        //JKQTPPlotSymbol(painter, x, y, JKQTPFilledCircle, parent->pt2px(painter, symbolSize*1.5), parent->pt2px(painter, symbolWidth*parent->getLineWidthMultiplier()), penSelection.color(), penSelection.color());
                    //}
                    if ((!getXAxis()->isLogAxis() || xv>0.0) && (!getYAxis()->isLogAxis() || yv>0.0) ) {
                        if (symType!=JKQTPNoSymbol && cliprect.contains(x,y)) plotStyledSymbol(parent, painter, x, y);
                    }
                }
            }

        }
    }
    //qDebug()<<"JKQTPXYScatterGraph::draw(): "<<7;
    drawErrorsAfter(painter);
    //qDebug()<<"JKQTPXYScatterGraph::draw() ... done";
}

void JKQTPXYScatterGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    JKQTPPlotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, getSymbolType(), getKeySymbolSizePx(painter, rect, parent), getKeySymbolLineWidthPx(painter, rect, parent), getKeyLabelColor(), getSymbolFillColor(),getSymbolFont());
}

QColor JKQTPXYScatterGraph::getKeyLabelColor() const {
    return getSymbolColor();
}

void JKQTPXYScatterGraph::setColor(QColor c)
{
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.fillColorDerivationMode, c));
    c.setAlphaF(0.5);
}



JKQTPXYScatterErrorGraph::JKQTPXYScatterErrorGraph(JKQTBasePlotter *parent):
    JKQTPXYScatterGraph(parent)
{
    setErrorColorFromGraphColor(getSymbolColor());
    initErrorStyle(parent, parentPlotStyle);
}

JKQTPXYScatterErrorGraph::JKQTPXYScatterErrorGraph(JKQTPlotter *parent):
    JKQTPXYScatterErrorGraph(parent->getPlotter())
{

}

bool JKQTPXYScatterErrorGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero) {
    if (xErrorColumn<0 || xErrorStyle==JKQTPNoError) {
        return JKQTPXYScatterGraph::getXMinMax(minx, maxx, smallestGreaterZero);
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

bool JKQTPXYScatterErrorGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero) {
    if (yErrorColumn<0 || yErrorStyle==JKQTPNoError) {
        return JKQTPXYScatterGraph::getYMinMax(miny, maxy, smallestGreaterZero);
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

bool JKQTPXYScatterErrorGraph::usesColumn(int c) const
{
    return JKQTPXYScatterGraph::usesColumn(c)||JKQTPXYGraphErrors::errorUsesColumn(c);
}

void JKQTPXYScatterErrorGraph::drawErrorsBefore(JKQTPEnhancedPainter &painter)
{
    intSortData();
    if (sortData==JKQTPXYGraph::Unsorted) plotErrorIndicators(painter, parent, this, xColumn, yColumn);
    else plotErrorIndicators(painter, parent, this, xColumn, yColumn, 0, 0, &sortedIndices);

}
















JKQTPXYParametrizedScatterGraph::JKQTPXYParametrizedScatterGraph(JKQTBasePlotter *parent):
    JKQTPXYGraph(parent),
    JKQTPColorPaletteStyleAndToolsMixin(parent),
    drawLine(false),
    drawLineInForeground(true)
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
    symbolFillDerivationMode=JKQTPColorDerivationMode::JKQTPFFCMLighterColor;
    if (parent) {
        symbolFillDerivationMode=parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.fillColorDerivationMode;
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

    const QPen p=getLinePen(painter, parent);
    const QPen penSelection=getHighlightingLinePen(painter, parent);


    int imax=0;
    int imin=0;
    if (getIndexRange(imin, imax)) {


        QVector<QLineF> lines;
        QPolygonF linesP;
        QVector<QColor> linecols;
        QVector<QColor> linecolss;
        QVector<double> linewidths;
        QVector<SymbolDescription> symbols;
        //qDebug()<<"JKQTPXYLineGraph::draw(): "<<3<<" imin="<<imin<<" imax="<<imax;
        {
            painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
            double xold=-1;
            double yold=-1;
            bool first=false;

            intSortData();
            double specSymbSize=0;
            bool hasSpecSymbSize=false;
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

                    if ((!getXAxis()->isLogAxis() || xv>0.0) && (!getYAxis()->isLogAxis() || yv>0.0) ) {
                        if (isHighlighted() && getSymbolType()!=JKQTPNoSymbol && symbolColumn<0) {
                            JKQTPPlotSymbol(painter, x, y, JKQTPFilledCircle,symbSize*1.25, parent->pt2px(painter, getSymbolLineWidth()*parent->getLineWidthMultiplier()), penSelection.color(), penSelection.color(),getSymbolFont());
                        } else {
                            if (drawLineInForeground) JKQTPPlotSymbol(painter, x, y, getLocalSymbolType(i), symbSize, parent->pt2px(painter, getSymbolLineWidth()*parent->getLineWidthMultiplier()), symbColor, symbFillColor,getSymbolFont());
                            else symbols.push_back({x, y, getLocalSymbolType(i), symbSize, symbColor, symbFillColor});
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
                    painter.drawPolylineFast(linesP);
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
                painter.drawPolylineFast(linesP);
            }
        }
        for (auto& s: symbols) {
            JKQTPPlotSymbol(painter, s.x, s.y, s.type, s.size, parent->pt2px(painter, getSymbolLineWidth()*parent->getLineWidthMultiplier()), s.color, s.fillColor, getSymbolFont());
        }
    }

    drawErrorsAfter(painter);
}

void JKQTPXYParametrizedScatterGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    double symbolSize1=getKeySymbolSizePx(painter, rect, parent, 0.75);
    double symbolSize2=symbolSize1*0.8;

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
            JKQTPImageTools::array2image<double>(colorval, 2, 1, img, getColorPalette(), double(0.0), double(1.0));
            color1=img.pixel(0,0);
            color2=img.pixel(1,0);
        }
    }

    if (symbolColumn>=0) {
        symbol1=JKQTPFilledCircle;
        symbol2=JKQTPFilledRect;
        JKQTPDatastore* datastore=parent->getDatastore();
        if (datastore && datastore->getRows(symbolColumn)>0) {
            symbol1=getLocalSymbolType(0);
            symbol2=getLocalSymbolType(datastore->getRows(symbolColumn)-1);
        }

    }

    const double lineWidth=getKeyLineWidthPx(painter, rect, parent)*0.75;

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(color1);
    p.setStyle(getLineStyle());
    p.setWidthF(lineWidth);
    painter.setPen(p);
    const double x1=rect.left()+symbolSize1/2.0;
    const double y1=rect.top()+symbolSize1/2.0;
    const double x2=rect.right()-symbolSize2/2.0;
    const double y2=rect.bottom()-symbolSize2/2.0;
    if (drawLine && !drawLineInForeground) painter.drawLine(QLineF(x1,y1, x2,y2));
    JKQTPPlotSymbol(painter, x1, y1, symbol1, symbolSize1, getKeySymbolLineWidthPx(painter, rect, parent,0.5), color1, JKQTPGetDerivedColor(symbolFillDerivationMode, color1),getSymbolFont());
    JKQTPPlotSymbol(painter, x2, y2, symbol2, symbolSize2, getKeySymbolLineWidthPx(painter, rect, parent,0.5), color2, JKQTPGetDerivedColor(symbolFillDerivationMode, color2),getSymbolFont());
    if (drawLine && drawLineInForeground) painter.drawLine(QLineF(x1,y1, x2,y2));

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
    JKQTPXYGraph::setParent(parent);
    cbSetParent(parent);
}

void JKQTPXYParametrizedScatterGraph::getOutsideSize(JKQTPEnhancedPainter &painter, int &leftSpace, int &rightSpace, int &topSpace, int &bottomSpace)
{
    JKQTPXYGraph::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar&& colorColumn>=0 && !colorColumnContainsRGB) cbGetOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPXYParametrizedScatterGraph::drawOutside(JKQTPEnhancedPainter &painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace)
{
    JKQTPXYGraph::drawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
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
    return (c==colorColumn) || (c==sizeColumn) || (c==symbolColumn) || (c==linewidthColumn) || JKQTPXYGraph::usesColumn(c);
}

void JKQTPXYParametrizedScatterGraph::setDrawLine(bool __value)
{
    drawLine=__value;
}

bool JKQTPXYParametrizedScatterGraph::getDrawLine() const
{
    return drawLine;
}

void JKQTPXYParametrizedScatterGraph::setDrawLineInForeground(bool __value)
{
    drawLineInForeground=__value;
}

bool JKQTPXYParametrizedScatterGraph::getDrawLineInForeground() const
{
    return drawLineInForeground;
}

void JKQTPXYParametrizedScatterGraph::setColor(QColor c)
{
    setLineColor(c);
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.fillColorDerivationMode, c));
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
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
        return JKQTPXYGraph::getXMinMax(minx, maxx, smallestGreaterZero);
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
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))+getXErrorU(i, datastore);
                if (JKQTPIsOKFloat(xv)  ) {
                    if (start || xv>maxx) maxx=xv;
                    if (start || xv<minx) minx=xv;
                    const double xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                }
                const double xvv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i))-getXErrorL(i, datastore);
                if (JKQTPIsOKFloat(xvv)) {
                    start=false;
                    if (start || xvv>maxx) maxx=xvv;
                    if (start || xvv<minx) minx=xvv;
                    const double xvsgz=xvv; SmallestGreaterZeroCompare_xvsgz();
                    start=false;
                }
            }
            return !start;
        }
    }
    return false;
}

bool JKQTPXYParametrizedErrorScatterGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    if (yErrorColumn<0 || yErrorStyle==JKQTPNoError) {
        return JKQTPXYGraph::getYMinMax(miny, maxy, smallestGreaterZero);
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
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))+getYErrorU(i, datastore);
                if (JKQTPIsOKFloat(yv)) {
                    if (start || yv>maxy) maxy=yv;
                    if (start || yv<miny) miny=yv;
                    const double xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                    start=false;
                }
                const double yvv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i))-getYErrorL(i, datastore);
                if (JKQTPIsOKFloat(yvv) ) {
                    if (start || yvv>maxy) maxy=yvv;
                    if (start || yvv<miny) miny=yvv;
                    const double xvsgz=yvv; SmallestGreaterZeroCompare_xvsgz();
                    start=false;
                }
            }
            return !start;
        }
    }
    return false;
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

bool JKQTPXYParametrizedErrorScatterGraph::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, const JKQTBasePlotter * /*parent*/, const JKQTPGraph * /*parentGraph*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPErrorPlotstyle /*xErrorStyle*/, JKQTPErrorPlotstyle /*yErrorStyle*/, int index, QColor &errorLineColor, QColor &errorFillColor) const
{
    QColor c=getLocalColor(index);
    errorLineColor=c.darker();
    errorFillColor=c.lighter();
    //qDebug()<<"intPlotXYErrorIndicatorsGetColor("<<index<<"): "<<errorColor;
    return true;
}



