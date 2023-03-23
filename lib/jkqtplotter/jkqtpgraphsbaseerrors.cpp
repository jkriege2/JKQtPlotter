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



#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtpgraphsbase.h"


#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;






JKQTPGraphErrorStyleMixin::JKQTPGraphErrorStyleMixin() {
    m_errorLinePen=QPen(QColor("red"), Qt::SolidLine);
    m_errorLinePen.setJoinStyle(Qt::RoundJoin);
    m_errorLinePen.setCapStyle(Qt::RoundCap);
    m_errorLineWidth=1;
    m_errorBarCapSize=7;
}

void JKQTPGraphErrorStyleMixin::initErrorStyle(JKQTBasePlotter *parent, int &parentPlotStyle, JKQTPPlotStyleType styletype)
{
    if (parent) { // get style settings from parent object
        if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
        const JKQTBasePlotter::JKQTPPen pen=parent->getPlotStyle(parentPlotStyle, styletype);
        m_errorFillColor=pen.errorFillColor();
        m_errorFillBrush.setColor(m_errorFillColor);
        m_errorFillBrush.setStyle(pen.errorFillStyle());
        m_errorLinePen.setColor(pen.errorColor());
        m_errorLinePen.setStyle(pen.errorStyle());
        m_errorLineWidth=pen.errorWidthF();
    }

}

void JKQTPGraphErrorStyleMixin::setErrorStyleFromPen(const JKQTBasePlotter::JKQTPPen &pen)
{
    m_errorLineWidth=pen.errorWidthF();
    setErrorLineColor(pen.errorColor());
    setErrorFillColor(pen.errorFillColor());
    setErrorFillStyle(pen.errorFillStyle());
    setErrorLineStyle(pen.errorStyle());
    m_errorBarCapSize=pen.symbolSize()*0.75;
}



void JKQTPGraphErrorStyleMixin::setErrorColorFromGraphColor(QColor graphColor)
{
    setErrorLineColor(graphColor.darker());
    setErrorFillColor(graphColor.lighter());
    //errorColor.setAlphaF(0.5);
}

QPen JKQTPGraphErrorStyleMixin::getErrorLinePen(JKQTPEnhancedPainter &painter, const JKQTBasePlotter *parent) const
{
    QPen p=m_errorLinePen;
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*m_errorLineWidth)));
    return p;
}

QPen JKQTPGraphErrorStyleMixin::getErrorLinePenForRects(JKQTPEnhancedPainter &painter, const JKQTBasePlotter *parent) const
{
    QPen p=getErrorLinePen(painter, parent);
    p.setJoinStyle(Qt::MiterJoin);
    return p;
}

QBrush JKQTPGraphErrorStyleMixin::getErrorFillBrush(JKQTPEnhancedPainter &/*painter*/, const JKQTBasePlotter * /*parent*/) const
{
    return m_errorFillBrush;
}

void JKQTPGraphErrorStyleMixin::setErrorLineColor(const QColor &__value)
{
    m_errorLinePen.setColor(__value);
}

QColor JKQTPGraphErrorStyleMixin::getErrorLineColor() const
{
    return this->m_errorLinePen.color();
}

void JKQTPGraphErrorStyleMixin::setErrorLineStyle(Qt::PenStyle __value)
{
    this->m_errorLinePen.setStyle( __value);
}

Qt::PenStyle JKQTPGraphErrorStyleMixin::getErrorLineStyle() const
{
    return this->m_errorLinePen.style();
}

void JKQTPGraphErrorStyleMixin::setErrorLineWidth(double __value)
{
    this->m_errorLineWidth = __value;
}

double JKQTPGraphErrorStyleMixin::getErrorLineWidth() const
{
    return this->m_errorLineWidth;
}


void JKQTPGraphErrorStyleMixin::setErrorLineDashOffset(qreal offset)
{
    m_errorLinePen.setDashOffset(offset);
}

qreal JKQTPGraphErrorStyleMixin::getErrorLineDashOffset() const
{
    return m_errorLinePen.dashOffset();
}

void JKQTPGraphErrorStyleMixin::setErrorLineDashPattern(const QVector<qreal> &pattern)
{
    m_errorLinePen.setDashPattern(pattern);
    m_errorLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphErrorStyleMixin::getErrorLineDashPattern() const
{
    return m_errorLinePen.dashPattern();
}

void JKQTPGraphErrorStyleMixin::setErrorLineJoinStyle(Qt::PenJoinStyle style)
{
    m_errorLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphErrorStyleMixin::getErrorLineJoinStyle() const
{
    return m_errorLinePen.joinStyle();
}

void JKQTPGraphErrorStyleMixin::setErrorLineCapStyle(Qt::PenCapStyle style)
{
    m_errorLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphErrorStyleMixin::getErrorLineCapStyle() const
{
    return m_errorLinePen.capStyle();
}

void JKQTPGraphErrorStyleMixin::setErrorLineBrush(const QBrush &style)
{
    m_errorLinePen.setBrush(style);
}

QBrush JKQTPGraphErrorStyleMixin::getErrorLineBrush() const
{
    return m_errorLinePen.brush();
}

void JKQTPGraphErrorStyleMixin::setErrorFillColor(const QColor &__value)
{
    m_errorFillColor=__value;
    m_errorFillBrush.setColor(__value);
}

QColor JKQTPGraphErrorStyleMixin::getErrorFillColor() const
{
    return this->m_errorFillBrush.color();
}

void JKQTPGraphErrorStyleMixin::setErrorFillStyle(Qt::BrushStyle __value)
{
    if (m_errorFillBrush.style()==Qt::LinearGradientPattern || m_errorFillBrush.style()==Qt::RadialGradientPattern || m_errorFillBrush.style()==Qt::ConicalGradientPattern || m_errorFillBrush.style()==Qt::TexturePattern) {
        m_errorFillBrush=QBrush(m_errorFillColor, __value);
    } else {
        m_errorFillBrush.setStyle(__value);
    }
}

Qt::BrushStyle JKQTPGraphErrorStyleMixin::getErrorFillStyle() const
{
    return this->m_errorFillBrush.style();
}

void JKQTPGraphErrorStyleMixin::setErrorBarCapSize(double __value)
{
    this->m_errorBarCapSize = __value;
}

double JKQTPGraphErrorStyleMixin::getErrorBarCapSize() const
{
    return this->m_errorBarCapSize;
}


void JKQTPGraphErrorStyleMixin::setErrorFillTexture(const QPixmap &__value)
{
    m_errorFillBrush.setStyle(Qt::TexturePattern);
    m_errorFillBrush.setTexture(__value);
}

void JKQTPGraphErrorStyleMixin::setErrorFillTexture(const QImage &__value)
{
    m_errorFillBrush.setStyle(Qt::TexturePattern);
    m_errorFillBrush.setTextureImage(__value);
}

QPixmap JKQTPGraphErrorStyleMixin::getErrorFillTexture() const
{
    return m_errorFillBrush.texture();
}

QImage JKQTPGraphErrorStyleMixin::getErrorFillTextureImage() const
{
    return m_errorFillBrush.textureImage();
}

void JKQTPGraphErrorStyleMixin::setErrorFillGradient(const QGradient &__value)
{
    m_errorFillBrush=QBrush(__value);
}

const QGradient *JKQTPGraphErrorStyleMixin::getErrorFillGradient() const
{
    return m_errorFillBrush.gradient();
}

void JKQTPGraphErrorStyleMixin::setErrorFillBrush(const QBrush &b)
{
    m_errorFillBrush=b;
    m_errorFillColor=b.color();
}

void JKQTPGraphErrorStyleMixin::setErrorFillTransform(const QTransform &b)
{
    m_errorFillBrush.setTransform(b);
}

void JKQTPGraphErrorStyleMixin::intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, const JKQTBasePlotter* parent, const JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int xErrorColumnLower, int yErrorColumnLower, bool xErrorSymmetric, bool yErrorSymmetric, double xrelshift, double yrelshift, const  QVector<int>* dataorder) const {
    //std::cout<<"JKQTPGraphErrors::intPlotXYErrorIndicators(p, "<<parent<<", "<<xColumn<<", "<<yColumn<<", "<<xErrorColumn<<", "<<yErrorColumn<<", "<<xErrorStyle<<", "<<yErrorStyle<<", ...)\n";
    if (parent==nullptr) return;
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    if ((yErrorStyle==JKQTPNoError) && (xErrorStyle==JKQTPNoError)) return;
    bool visX=(xErrorStyle!=JKQTPNoError)&&(xErrorColumn>=0||xErrorColumnLower>=0)&&(xColumn>=0)&&(yColumn>=0);
    bool visY=(yErrorStyle!=JKQTPNoError)&&(yErrorColumn>=0||yErrorColumnLower>=0)&&(xColumn>=0)&&(yColumn>=0);
    if (!visX&&!visY) return;
    //std::cout<<"   JKQTPGraphErrors::intPlotXYErrorIndicators(p, "<<parent<<", "<<xColumn<<", "<<yColumn<<", "<<xErrorColumn<<", "<<yErrorColumn<<", ...)\n";

    const double xmin=parentGraph->transformX(parentGraph->getXAxis()->getMin());
    const double xmax=parentGraph->transformX(parentGraph->getXAxis()->getMax());
    const double ymin=parentGraph->transformY(parentGraph->getYAxis()->getMin());
    const double ymax=parentGraph->transformY(parentGraph->getYAxis()->getMax());
    const QMarginsF clipMargins(50,50,50,50);
    const QRectF cliprect=QRectF(qMin(xmin,xmax),qMin(ymin,ymax),fabs(xmax-xmin),fabs(ymax-ymin))+clipMargins;

    QBrush b=getErrorFillBrush(painter, parent);
    QPen p=getErrorLinePen(painter, parent);
    QPen pr=getErrorLinePenForRects(painter, parent);
    painter.setPen(p);

    size_t imaxx=0, imaxy=0;
    if (xColumn>=0) imaxx=datastore->getRows(static_cast<size_t>(xColumn));
    if (yColumn>=0) imaxy=datastore->getRows(static_cast<size_t>(yColumn));
    size_t imax=qMin(imaxx, imaxy);
    size_t imin=0;
    if (imax<imin) {
        size_t h=imin;
        imin=imax;
        imax=h;
    }

    double xold=-1;
    double xpold=-1;
    double xmold=-1;
    double yold=-1;
    double ypold=-1;
    double ymold=-1;
    bool pastFirst=false;
    double ebs_px=parent->pt2px(painter, m_errorBarCapSize);
    QPolygonF polyX, polyY;
    QPolygonF polyXTopPoints, polyXBottomPoints, polyYTopPoints, polyYBottomPoints;
    QList<QColor> errFC, errC;
    bool defaultErrorColor=true;



    for (size_t iii=imin; iii<imax; iii++) {
        int i=static_cast<int>(iii);
        if (dataorder) i=dataorder->value(static_cast<int>(iii), static_cast<int>(iii));
        const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));

        double deltax=1;
        double deltapx=0;
        double deltamx=0;
        if (imin==imax) { // only one x-value
            deltamx=0.5;
            deltapx=0.5;
        } else if (static_cast<int>(i)==static_cast<int>(imax)-1&& static_cast<int>(i)-1>=0) { // the right-most x-value
            deltapx=deltamx=fabs(xv-datastore->get(xColumn,static_cast<size_t>(i-1)))/2.0;
        } else if (i==static_cast<int>(imin) && i+1<static_cast<int>(imax)) { // the left-most x-value
            deltamx=deltapx=fabs(datastore->get(xColumn,static_cast<size_t>(i+1))-xv)/2.0;
        } else {
            if (static_cast<int>(i)-1>=0) deltamx=fabs(xv-datastore->get(xColumn,static_cast<size_t>(i-1)))/2.0;
            if (i+1<static_cast<int>(imax)) deltapx=fabs(datastore->get(xColumn,static_cast<size_t>(i+1))-xv)/2.0;
        }
        deltax=deltapx+deltamx;

        double deltay=1;
        double deltapy=0;
        double deltamy=0;
        if (imin==imax) { // only one y-value
            deltamy=0.5;
            deltapy=0.5;
        } else if (i==static_cast<int>(imax)-1&& static_cast<int>(i)-1>=0) { // the right-most y-value
            deltapy=deltamy=fabs(yv-datastore->get(yColumn,static_cast<size_t>(i-1)))/2.0;
        } else if (i==static_cast<int>(imin) && i+1<static_cast<int>(imax)) { // the left-most y-value
            deltamy=deltapy=fabs(datastore->get(yColumn,static_cast<size_t>(i+1))-yv)/2.0;
        } else {
            if (static_cast<int>(i)-1>=0) deltamy=fabs(yv-datastore->get(yColumn,static_cast<size_t>(i-1)))/2.0;
            if (i+1<static_cast<int>(imax)) deltapy=fabs(datastore->get(yColumn,static_cast<size_t>(i+1))-yv)/2.0;
        }
        deltay=deltapy+deltamy;

        bool plotlowerbarx=false;
        bool plotupperbarx=false;
        bool plotlowerbary=false;
        bool plotupperbary=false;

        double xe=0;   if (xErrorStyle!=JKQTPNoError && xErrorColumn>=0) { xe=datastore->get(xErrorColumn,static_cast<size_t>(i)); plotupperbarx=true; }
        double ye=0;   if (yErrorStyle!=JKQTPNoError && yErrorColumn>=0) { ye=datastore->get(yErrorColumn,static_cast<size_t>(i)); plotupperbary=true; }
        double xl=0;   if (xErrorSymmetric) { xl=xe; plotlowerbarx=plotupperbarx||(xl>0); }
                       else if (xErrorStyle!=JKQTPNoError && xErrorColumnLower>=0) { xl=datastore->get(xErrorColumnLower,static_cast<size_t>(i)); plotlowerbarx=true; }
        double yl=0;   if (yErrorSymmetric) { yl=ye; plotlowerbary=plotupperbary||(yl>0); }
                       else if (yErrorStyle!=JKQTPNoError && yErrorColumnLower>=0) { yl=datastore->get(yErrorColumnLower,static_cast<size_t>(i)); plotlowerbary=true; }

        if (xErrorStyle.testFlag(JKQTPErrorDirectionOutwards)) {
            if (xv>=0.0) { xl=0; plotlowerbarx=false; }
            else { xe=0; plotupperbarx=false; }
        } else if (xErrorStyle.testFlag(JKQTPErrorDirectionInwards)) {
            if (xv<0.0) { xl=0; plotlowerbarx=false; }
            else { xe=0; plotupperbarx=false; }
        } else if (xErrorStyle.testFlag(JKQTPErrorDirectionAbove)) {
            xl=0; plotlowerbarx=false;
        } else if (xErrorStyle.testFlag(JKQTPErrorDirectionBelow)) {
            xe=0; plotupperbarx=false;
        }

        if (yErrorStyle.testFlag(JKQTPErrorDirectionOutwards)) {
            if (yv>=0.0) { yl=0; plotlowerbary=false; }
            else { ye=0; plotupperbary=false; }
        } else if (yErrorStyle.testFlag(JKQTPErrorDirectionInwards)) {
            if (yv<0.0) { yl=0; plotlowerbary=false; }
            else { ye=0; plotupperbary=false; }
        } else if (yErrorStyle.testFlag(JKQTPErrorDirectionAbove)) {
            yl=0; plotlowerbary=false;
        } else if (yErrorStyle.testFlag(JKQTPErrorDirectionBelow)) {
            ye=0; plotupperbary=false;
        }


        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(xe) && JKQTPIsOKFloat(ye) && JKQTPIsOKFloat(xl) && JKQTPIsOKFloat(yl)) {
            double x=parentGraph->transformX(xv+xrelshift*deltax); bool xok=JKQTPIsOKFloat(x);
            double y=parentGraph->transformY(yv+yrelshift*deltay); bool yok=JKQTPIsOKFloat(y);
            QColor terrCol=getErrorLineColor();
            QColor terrFillCol=getErrorFillColor();
            defaultErrorColor = defaultErrorColor && !this->intPlotXYErrorIndicatorsGetColor(painter, parent, parentGraph, xColumn, yColumn, xErrorColumn, yErrorColumn, xErrorStyle, yErrorStyle, i, terrCol, terrFillCol);

            // x-errorpolygons
            if (xErrorStyle.testFlag(JKQTPErrorPolygons)) {
                double xl2m=parentGraph->transformX(xv+xrelshift*deltax-xl);
                double xl2p=parentGraph->transformX(xv+xrelshift*deltax+xe);
                double yl2=y;

                polyXTopPoints<<QPointF(xl2p, yl2);
                polyXBottomPoints<<QPointF(xl2m, yl2);
                errFC<<terrFillCol;
                errC<<terrCol;
            }

            // y-errorpolygons
            if (yErrorStyle.testFlag(JKQTPErrorPolygons)) {

                double yl2m=parentGraph->transformY(yv+yrelshift*deltay-yl);
                double yl2p=parentGraph->transformY(yv+yrelshift*deltay+ye);
                double xl2=x;

                polyYTopPoints<<QPointF(xl2, yl2p);
                polyYBottomPoints<<QPointF(xl2, yl2m);
                errFC<<terrFillCol;
                errC<<terrCol;
            }


            //x-errorbars
            if ((xErrorColumn>=0 || xErrorColumnLower>=0) && (xErrorStyle.testFlag(JKQTPErrorSimpleBars)))
            {
                    double x0=parentGraph->transformX(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                    double x1=parentGraph->transformX(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                    QPen pp=p;
                    if (!defaultErrorColor) pp.setColor(terrCol);
                    painter.setPen(pp);
                    QList<QLineF> elines;
                    if (x0ok&&x1ok&&xok&&yok) {
                        elines<<QLineF(x0, y, x1, y);
                    } else if (x0ok&&!x1ok&&xok&&yok) {
                        elines<<QLineF(x0, y, x, y);
                        if (x0<x) elines<<QLineF(x,y,parentGraph->transformX(parent->getXMax()),y);
                        else elines<<QLineF(x,y,parentGraph->transformX(parent->getXMin()),y);
                    } else if (!x0ok&&x1ok&&xok&&yok) {
                        elines<<QLineF(x1, y, x, y);
                        if (x1<x) elines<<QLineF(x,y,parentGraph->transformX(parent->getXMin()),y);
                        else elines<<QLineF(x,y,parentGraph->transformX(parent->getXMax()),y);
                    }
                    for (QLineF& l: elines) {
                        l=JKQTPClipLine(l, cliprect);
                        if (l.length()>0) painter.drawLine(l);
                    }
            }
            // x-bar indicators
            if ((xErrorColumn>=0 || xErrorColumnLower>=0) && (xErrorStyle.testFlag(JKQTPErrorIndicatorBar)))
            {
                double x0=parentGraph->transformX(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                double x1=parentGraph->transformX(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                painter.setBrush(pp.color());
                if (x0ok&&xok && plotlowerbarx) painter.drawLine(QLineF(x0, y-ebs_px/2.0,x0, y+ebs_px/2.0));
                if (x1ok&&xok && plotupperbarx) painter.drawLine(QLineF(x1, y-ebs_px/2.0,x1, y+ebs_px/2.0));
            }

            // x-inwardArrow indicators
            if ((xErrorColumn>=0 || xErrorColumnLower>=0) && (xErrorStyle.testFlag(JKQTPErrorIndicatorInwardArrows)))
            {
                double x0=parentGraph->transformX(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                double x1=parentGraph->transformX(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                painter.setBrush(pp.color());
                if (x0ok&&xok && plotlowerbarx) painter.drawPolygon(QPolygonF()<<QPointF(x0,y)<<QPointF(x0-ebs_px/2.0,y+ebs_px/2.0)<<QPointF(x0-ebs_px/2.0,y-ebs_px/2.0));
                if (x1ok&&xok && plotupperbarx) painter.drawPolygon(QPolygonF()<<QPointF(x1,y)<<QPointF(x1+ebs_px/2.0,y+ebs_px/2.0)<<QPointF(x1+ebs_px/2.0,y-ebs_px/2.0));
            }

            // x-arrow indicators
            if ((xErrorColumn>=0 || xErrorColumnLower>=0) && (xErrorStyle.testFlag(JKQTPErrorIndicatorArrows)))
            {
                double x0=parentGraph->transformX(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                double x1=parentGraph->transformX(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                painter.setBrush(pp.color());
                if (x0ok&&xok && plotlowerbarx) painter.drawPolygon(QPolygonF()<<QPointF(x0,y)<<QPointF(x0+ebs_px/2.0,y+ebs_px/2.0)<<QPointF(x0+ebs_px/2.0,y-ebs_px/2.0));
                if (x1ok&&xok && plotupperbarx) painter.drawPolygon(QPolygonF()<<QPointF(x1,y)<<QPointF(x1-ebs_px/2.0,y+ebs_px/2.0)<<QPointF(x1-ebs_px/2.0,y-ebs_px/2.0));
            }

            // y-errorbars
            if ((yErrorColumn>=0 || yErrorColumnLower>=0) && (yErrorStyle.testFlag(JKQTPErrorSimpleBars)))
            {
                double y0=parentGraph->transformY(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                double y1=parentGraph->transformY(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                painter.setBrush(pp.color());
                QList<QLineF> elines;
                if (y0ok&&y1ok&&xok&&yok) {
                    elines<<QLineF(x, y0, x, y1);
                } else if (y0ok&&!y1ok&&xok&&yok) {   // upper errorbar OK, lower errorbar NAN
                    elines<<QLineF(x, y0, x, y);
                    if (y0<y) elines<<QLineF(x,y,x,parentGraph->transformY(parent->getYMin()));
                    else elines<<QLineF(x,y,x,parentGraph->transformY(parent->getYMax())); // inverted axis!
                } else if (!y0ok&&y1ok&&xok&&yok) {
                    elines<<QLineF(x, y1, x, y);
                    if (y1<y) elines<<QLineF(x,y,x,parentGraph->transformY(parent->getYMax()));
                    else elines<<QLineF(x,y,x,parentGraph->transformY(parent->getYMin()));
                }
                for (QLineF& l: elines) {
                    l=JKQTPClipLine(l, cliprect);
                    if (l.length()>0) painter.drawLine(l);
                }
            }
            // y-bar indicators
            if ((yErrorColumn>=0 || yErrorColumnLower>=0) && (yErrorStyle.testFlag(JKQTPErrorIndicatorBar)))
            {
                double y0=parentGraph->transformY(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                double y1=parentGraph->transformY(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                painter.setBrush(pp.color());
                if (y0ok&&xok && plotlowerbary) painter.drawLine(QLineF(x-ebs_px/2.0,y0,x+ebs_px/2.0,y0));
                if (y1ok&&xok && plotupperbary) painter.drawLine(QLineF(x-ebs_px/2.0,y1,x+ebs_px/2.0,y1));
            }

            // y-inwardArrow indicators
            if ((yErrorColumn>=0 || yErrorColumnLower>=0) && (yErrorStyle.testFlag(JKQTPErrorIndicatorInwardArrows)))
            {
                double y0=parentGraph->transformY(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                double y1=parentGraph->transformY(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                painter.setBrush(pp.color());
                if (y0ok&&xok && plotlowerbary) painter.drawPolygon(QPolygonF()<<QPointF(x,y0)<<QPointF(x-ebs_px/2.0,y0+ebs_px/2.0)<<QPointF(x+ebs_px/2.0,y0+ebs_px/2.0));
                if (y1ok&&xok && plotupperbary) painter.drawPolygon(QPolygonF()<<QPointF(x,y1)<<QPointF(x-ebs_px/2.0,y1-ebs_px/2.0)<<QPointF(x+ebs_px/2.0,y1-ebs_px/2.0));
            }

            // y-arrow indicators
            if ((yErrorColumn>=0 || yErrorColumnLower>=0) && (yErrorStyle.testFlag(JKQTPErrorIndicatorArrows)))
            {
                double y0=parentGraph->transformY(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                double y1=parentGraph->transformY(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                painter.setBrush(pp.color());
                if (y0ok&&xok && plotlowerbary) painter.drawPolygon(QPolygonF()<<QPointF(x,y0)<<QPointF(x-ebs_px/2.0,y0-ebs_px/2.0)<<QPointF(x+ebs_px/2.0,y0-ebs_px/2.0));
                if (y1ok&&xok && plotupperbary) painter.drawPolygon(QPolygonF()<<QPointF(x,y1)<<QPointF(x-ebs_px/2.0,y1+ebs_px/2.0)<<QPointF(x+ebs_px/2.0,y1+ebs_px/2.0));
            }

            // error boxes
            if (yErrorStyle.testFlag(JKQTPErrorBoxes) || xErrorStyle.testFlag(JKQTPErrorBoxes) || yErrorStyle.testFlag(JKQTPErrorEllipses) || xErrorStyle.testFlag(JKQTPErrorEllipses) ) {
                double y0=parentGraph->transformY(yv+yrelshift*deltay-yl); bool y0ok=JKQTPIsOKFloat(y0);
                double y1=parentGraph->transformY(yv+yrelshift*deltay+ye); bool y1ok=JKQTPIsOKFloat(y1);
                double x0=parentGraph->transformX(xv+xrelshift*deltax-xl); bool x0ok=JKQTPIsOKFloat(x0);
                double x1=parentGraph->transformX(xv+xrelshift*deltax+xe); bool x1ok=JKQTPIsOKFloat(x1);
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                QBrush bb=b;
                if (!defaultErrorColor) bb.setColor(terrFillCol);
                painter.setBrush(bb);

                const QRectF errRect=QRectF(QPointF(x0,y0), QPointF(x1,y1));
                if (((y0ok&&y1ok)||(x0ok&&x1ok))&&cliprect.intersects(errRect)) {
                    if (yErrorStyle.testFlag(JKQTPErrorEllipses) || xErrorStyle.testFlag(JKQTPErrorEllipses)) painter.drawEllipse(errRect);
                    else painter.drawRect(errRect);
                }


            }
            // x-errorlines
            if (pastFirst && (xErrorStyle.testFlag(JKQTPErrorLines))) {
                double xl1m=xmold;
                double xl1p=xpold;
                double yl1=yold;
                double xl2m=parentGraph->transformX(xv+xrelshift*deltax-xl);
                double xl2p=parentGraph->transformX(xv+xrelshift*deltax+xe);
                double yl2=y;
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                if (JKQTPIsOKFloat(xl1m)&&JKQTPIsOKFloat(yl1)&&JKQTPIsOKFloat(xl2m)&&JKQTPIsOKFloat(yl2)) {
                    const QLineF l=JKQTPClipLine(QLineF(xl1m, yl1, xl2m, yl2),cliprect);
                    if (l.length()>0) painter.drawLine(l);
                }
                if (JKQTPIsOKFloat(xl1p)&&JKQTPIsOKFloat(yl1)&&JKQTPIsOKFloat(xl2p)&&JKQTPIsOKFloat(yl2)) {
                    const QLineF l=JKQTPClipLine(QLineF(xl1p, yl1, xl2p, yl2),cliprect);
                    if (l.length()>0) painter.drawLine(l);
                }

            }


            // y-errorlines
            if (pastFirst && (yErrorStyle.testFlag(JKQTPErrorLines))) {
                double yl1m=ymold;
                double yl1p=ypold;
                double xl1=xold;
                double yl2m=parentGraph->transformY(yv+yrelshift*deltay-yl);
                double yl2p=parentGraph->transformY(yv+yrelshift*deltay+ye);
                double xl2=x;
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen pp=p;
                if (!defaultErrorColor) pp.setColor(terrCol);
                painter.setPen(pp);
                if (JKQTPIsOKFloat(xl1)&&JKQTPIsOKFloat(yl1m)&&JKQTPIsOKFloat(xl2)&&JKQTPIsOKFloat(yl2m)) {
                    const QLineF l=JKQTPClipLine(QLineF(xl1, yl1m, xl2, yl2m),cliprect);
                    if (l.length()>0) painter.drawLine(l);
                }
                if (JKQTPIsOKFloat(xl1)&&JKQTPIsOKFloat(yl1p)&&JKQTPIsOKFloat(xl2)&&JKQTPIsOKFloat(yl2p)) {
                    const QLineF l=JKQTPClipLine(QLineF(xl1, yl1p, xl2, yl2p),cliprect);
                    if (l.length()>0) painter.drawLine(l);
                }

            }



            pastFirst=true;
            xold=x;
            xmold=parentGraph->transformX(xv+xrelshift*deltax-xl);
            xpold=parentGraph->transformX(xv+xrelshift*deltax+xe);
            yold=y;
            ymold=parentGraph->transformY(yv+yrelshift*deltay-yl);
            ypold=parentGraph->transformY(yv+yrelshift*deltay+ye);
        }
    }
    // x-errorpolygons
    if ((polyXTopPoints.size()>0 || polyXBottomPoints.size()>0) && (xErrorStyle.testFlag(JKQTPErrorPolygons))) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.setPen(QPen(Qt::NoPen));
        QPolygonF poly;
        //poly << QPointF(xl1p, yl1) << QPointF(xl1m, yl1) << QPointF(xl2m, yl2) << QPointF(xl2p, yl2);
        for (int i=0; i<polyXTopPoints.size(); i++) {
            poly<<polyXTopPoints[i];
        }
        for (int i=polyXBottomPoints.size()-1; i>=0; i--) {
            poly<<polyXBottomPoints[i];
        }
        painter.drawConvexPolygon(poly.intersected(cliprect));

    }
    if ((polyYTopPoints.size()>0 || polyYBottomPoints.size()>0) && (yErrorStyle.testFlag(JKQTPErrorPolygons))) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.setPen(QPen(Qt::NoPen));
        QPolygonF poly;
        //poly << QPointF(xl1p, yl1) << QPointF(xl1m, yl1) << QPointF(xl2m, yl2) << QPointF(xl2p, yl2);
        for (int i=0; i<polyYTopPoints.size(); i++) {
            poly<<polyYTopPoints[i];
        }
        for (int i=polyYBottomPoints.size()-1; i>=0; i--) {
            poly<<polyYBottomPoints[i];
        }

        painter.drawConvexPolygon(poly.intersected(cliprect));

    }
    //std::cout<<"end\n";
}

bool JKQTPGraphErrorStyleMixin::intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter &/*painter*/, const JKQTBasePlotter * /*parent*/, const JKQTPGraph* /*parentGraph*/, int /*xColumn*/, int /*yColumn*/, int /*xErrorColumn*/, int /*yErrorColumn*/, JKQTPErrorPlotstyle /*xErrorStyle*/, JKQTPErrorPlotstyle /*yErrorStyle*/, int /*index*/, QColor &/*errorColor*/, QColor &/*errorFillColor*/) const
{
    return false;
}







JKQTPXGraphErrorData::JKQTPXGraphErrorData()
{
    xErrorSymmetric=true;
    xErrorColumn=-1;
    xErrorStyle=JKQTPNoError;
    xErrorColumnLower=-1;
}

void JKQTPXGraphErrorData::setXErrorSymmetric(bool __value)
{
    this->xErrorSymmetric = __value;
}

bool JKQTPXGraphErrorData::getXErrorSymmetric() const
{
    return this->xErrorSymmetric;
}

int JKQTPXGraphErrorData::getXErrorColumnLower() const {
    return this->xErrorColumnLower;
}

int JKQTPXGraphErrorData::getXErrorColumn() const {
    return this->xErrorColumn;
}

void JKQTPXGraphErrorData::setXErrorStyle(JKQTPErrorPlotstyle __value)
{
    this->xErrorStyle = __value;
}

JKQTPErrorPlotstyle JKQTPXGraphErrorData::getXErrorStyle() const
{
    return this->xErrorStyle;
}

void JKQTPXGraphErrorData::setXErrorColumn(int __value) {
    if (this->xErrorColumn != __value) {
        this->xErrorColumn = __value;
        if (xErrorColumn>=0 && xErrorStyle==JKQTPNoError) xErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPXGraphErrorData::setXErrorColumnLower(int __value) {
    if (this->xErrorColumnLower != __value) {
        this->xErrorColumnLower = __value;
        if (xErrorColumnLower>=0 && xErrorStyle==JKQTPNoError) xErrorStyle=JKQTPErrorBars;
    }
}


double JKQTPXGraphErrorData::getXErrorU(int i, const JKQTPDatastore *ds) const
{
    if (ds && xErrorColumn>=0 && i>=0 && i<static_cast<int>(ds->getRows(xErrorColumn))) {
        return ds->get(xErrorColumn, static_cast<size_t>(i));
    }
    return 0.0;
}

double JKQTPXGraphErrorData::getXErrorL(int i, const JKQTPDatastore *ds) const
{
    if (ds) {
        if (xErrorSymmetric) {
            if (xErrorColumn>=0 && i>=0 && i<static_cast<int>(ds->getRows(xErrorColumn))) return ds->get(xErrorColumn, static_cast<size_t>(i));
        } else {
            if (xErrorColumnLower>=0 && i>=0 && i<static_cast<int>(ds->getRows(xErrorColumnLower))) return ds->get(xErrorColumnLower, static_cast<size_t>(i));
        }
    }
    return 0.0;
}






JKQTPYGraphErrorData::JKQTPYGraphErrorData()
{
    yErrorSymmetric=true;
    yErrorColumn=-1;
    yErrorStyle=JKQTPNoError;
    yErrorColumnLower=-1;
}

void JKQTPYGraphErrorData::setYErrorSymmetric(bool __value)
{
    this->yErrorSymmetric = __value;
}

bool JKQTPYGraphErrorData::getYErrorSymmetric() const
{
    return this->yErrorSymmetric;
}

int JKQTPYGraphErrorData::getYErrorColumnLower() const {
    return this->yErrorColumnLower;
}

int JKQTPYGraphErrorData::getYErrorColumn() const {
    return this->yErrorColumn;
}

void JKQTPYGraphErrorData::setYErrorStyle(JKQTPErrorPlotstyle __value)
{
    this->yErrorStyle = __value;
}

JKQTPErrorPlotstyle JKQTPYGraphErrorData::getYErrorStyle() const
{
    return this->yErrorStyle;
}

void JKQTPYGraphErrorData::setYErrorColumn(int __value) {
    if (this->yErrorColumn != __value) {
        this->yErrorColumn = __value;
        if (yErrorColumn>=0 && yErrorStyle==JKQTPNoError) yErrorStyle=JKQTPErrorBars;
    }
}

void JKQTPYGraphErrorData::setYErrorColumnLower(int __value) {
    if (this->yErrorColumnLower != __value) {
        this->yErrorColumnLower = __value;
        if (yErrorColumnLower>=0 && yErrorStyle==JKQTPNoError) yErrorStyle=JKQTPErrorBars;
    }
}


double JKQTPYGraphErrorData::getYErrorU(int i, const JKQTPDatastore *ds) const
{
    if (ds && yErrorColumn>=0 && i>=0 && i<static_cast<int>(ds->getRows(yErrorColumn))) {
        return ds->get(yErrorColumn, static_cast<size_t>(i));
    }
    return 0.0;
}

double JKQTPYGraphErrorData::getYErrorL(int i, const JKQTPDatastore *ds) const
{
    if (ds) {
        if (yErrorSymmetric) {
            if (yErrorColumn>=0 && i>=0 && i<static_cast<int>(ds->getRows(yErrorColumn))) return ds->get(yErrorColumn, static_cast<size_t>(i));
        } else {
            if (yErrorColumnLower>=0 && i>=0 && i<static_cast<int>(ds->getRows(yErrorColumnLower))) return ds->get(yErrorColumnLower, static_cast<size_t>(i));
        }
    }
    return 0.0;
}







JKQTPXGraphErrors::JKQTPXGraphErrors()
{

}

bool JKQTPXGraphErrors::errorUsesColumn(int c) const
{
    return c==(xErrorColumn) || (c==xErrorColumnLower);
}

void JKQTPXGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, const JKQTBasePlotter* parent, const JKQTPGraph *parentGraph, int xColumn, int yColumn, double xrelshift, double yrelshift, const  QVector<int>* dataorder) const {
    intPlotXYErrorIndicators(painter, parent, parentGraph, xColumn, yColumn, xErrorColumn, -1, xErrorStyle, JKQTPNoError, xErrorColumnLower, -1, xErrorSymmetric, true, xrelshift, yrelshift, dataorder);
}





JKQTPYGraphErrors::JKQTPYGraphErrors()
{
}

void JKQTPYGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, const JKQTBasePlotter* parent, const JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift, double yrelshift, const  QVector<int>* dataorder) const {
    intPlotXYErrorIndicators(painter, parent, parentGraph, xColumn, yColumn, -1, yErrorColumn, JKQTPNoError, yErrorStyle, -1, yErrorColumnLower, true, yErrorSymmetric, xrelshift, yrelshift, dataorder);
}

bool JKQTPYGraphErrors::errorUsesColumn(int c) const
{
    return (c==yErrorColumn) || (c==yErrorColumnLower);
}







JKQTPXYGraphErrors::JKQTPXYGraphErrors()
{

}

void JKQTPXYGraphErrors::plotErrorIndicators(JKQTPEnhancedPainter& painter, const JKQTBasePlotter* parent, const JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift, double yrelshift, const  QVector<int>* dataorder) const {
    this->intPlotXYErrorIndicators(painter, parent, parentGraph, xColumn, yColumn, xErrorColumn, yErrorColumn, xErrorStyle, yErrorStyle, xErrorColumnLower, yErrorColumnLower, xErrorSymmetric, yErrorSymmetric, xrelshift, yrelshift, dataorder);
}

bool JKQTPXYGraphErrors::errorUsesColumn(int c) const
{
    return (c==xErrorColumn)||(c==yErrorColumn)||(c==xErrorColumnLower)||(c==yErrorColumnLower);
}


