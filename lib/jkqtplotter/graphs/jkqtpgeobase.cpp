/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/graphs/jkqtpgeobase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <QApplication>
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPGeoBaseLine::JKQTPGeoBaseLine(JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPGeometricPlotElement(drawMode, parent)
{
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Geometric);
}

void JKQTPGeoBaseLine::setStyle(QColor color, double lineWidth)
{
    setLineColor(color);
    setLineWidth(lineWidth);
}

void JKQTPGeoBaseLine::setStyle(QColor color, double lineWidth, Qt::PenStyle style)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
}

void JKQTPGeoBaseLine::setStyle(QColor color, Qt::PenStyle style)
{
    setLineColor(color);
    setLineStyle(style);}

void JKQTPGeoBaseLine::setAlpha(float alpha)
{
    auto color=getLineColor();
    color.setAlphaF(alpha);
    setLineColor(color);
}

void JKQTPGeoBaseLine::setColor(QColor c)
{
    setLineColor(c);
}


void JKQTPGeoBaseLine::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));

}

QColor JKQTPGeoBaseLine::getKeyLabelColor() const {
    return getLineColor();
}



JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(JKQTBasePlotter *parent, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoBaseLine(parent, drawMode)
{
    initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Geometric);
}

void JKQTPGeoBaseFilled::setAlpha(float alpha)
{
    JKQTPGeoBaseLine::setAlpha(alpha);
    auto fillColor=getFillColor();
    fillColor.setAlphaF(alpha);
    setFillColor(fillColor);
}

void JKQTPGeoBaseFilled::setAlpha(float alphaLine, float alphaFill)
{
    JKQTPGeoBaseLine::setAlpha(alphaLine);
    auto fillColor=getFillColor();
    fillColor.setAlphaF(alphaFill);
    setFillColor(fillColor);
}

void JKQTPGeoBaseFilled::setColor(QColor c)
{
    JKQTPGeoBaseLine::setColor(c);
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.geometricStyle.fillColorDerivationMode, c));
}

void JKQTPGeoBaseFilled::setStyle(QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
    setFillColor(fillColor);
    setFillStyle(fillStyle);
}

void JKQTPGeoBaseFilled::setStyle(QColor color, double lineWidth, Qt::PenStyle style)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
}

void JKQTPGeoBaseFilled::setStyle(QColor color, Qt::PenStyle style)
{
    setLineStyle(style);
    setLineColor(color);
}

void JKQTPGeoBaseFilled::setStyle(QColor color, double lineWidth)
{
    setLineColor(color);
    setLineWidth(lineWidth);
}

void JKQTPGeoBaseFilled::setStyle(QColor color, QColor fillColor)
{
    setLineColor(color);
    setFillColor(fillColor);
}

void JKQTPGeoBaseFilled::setStyleTransparentFill(QColor color, double lineWidth, Qt::PenStyle style)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
    setFillColor(Qt::transparent);
    setFillStyle(Qt::SolidPattern);
}

void JKQTPGeoBaseFilled::setStyleTransparentFill(QColor color, Qt::PenStyle style)
{
    setLineColor(color);
    setLineStyle(style);
    setFillColor(Qt::transparent);
    setFillStyle(Qt::SolidPattern);
}

void JKQTPGeoBaseFilled::setStyleTransparentFill(QColor color, double lineWidth)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setFillColor(Qt::transparent);
    setFillStyle(Qt::SolidPattern);
}

void JKQTPGeoBaseFilled::setStyleTransparentFill(QColor color)
{
    setLineColor(color);
    setFillColor(Qt::transparent);
    setFillStyle(Qt::SolidPattern);
}


void JKQTPGeoBaseFilled::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    painter.drawRect(rect);

}


JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(JKQTPLineDecoratorStyle headStyle, JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPGeometricPlotElement(drawMode, parent)
{
    int parStyleDummy=0;
    initDecoratedHeadLineStyle(parent, parStyleDummy, JKQTPPlotStyleType::Geometric);
    setHeadDecoratorStyle(headStyle);
}

JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPGeometricPlotElement(drawMode, parent)
{
    initDecoratedHeadLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Geometric);
}

void JKQTPGeoBaseDecoratedHeadLine::setStyle(QColor color, double lineWidth)
{
    setLineColor(color);
    setLineWidth(lineWidth);
}

void JKQTPGeoBaseDecoratedHeadLine::setStyle(QColor color, double lineWidth, Qt::PenStyle style)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
}

void JKQTPGeoBaseDecoratedHeadLine::setStyle(QColor color, Qt::PenStyle style)
{
    setLineColor(color);
    setLineStyle(style);
}

void JKQTPGeoBaseDecoratedHeadLine::setAlpha(float alpha)
{
    auto color=getLineColor();
    color.setAlphaF(alpha);
    setLineColor(color);
}

void JKQTPGeoBaseDecoratedHeadLine::setColor(QColor c)
{
    setLineColor(c);
}

void JKQTPGeoBaseDecoratedHeadLine::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
}

QColor JKQTPGeoBaseDecoratedHeadLine::getKeyLabelColor() const
{
    return getLineColor();
}


JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPGeometricPlotElement(drawMode, parent)
{
    int parStyleDummy=0;
    initDecoratedLineStyle(parent, parStyleDummy, JKQTPPlotStyleType::Geometric);
    setTailDecoratorStyle(tailStyle);
    setHeadDecoratorStyle(headStyle);
}

JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPGeometricPlotElement(drawMode, parent)
{
    initDecoratedLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Geometric);
}


void JKQTPGeoBaseDecoratedLine::setStyle(QColor color, double lineWidth)
{
    setLineColor(color);
    setLineWidth(lineWidth);
}

void JKQTPGeoBaseDecoratedLine::setStyle(QColor color, double lineWidth, Qt::PenStyle style)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
}

void JKQTPGeoBaseDecoratedLine::setStyle(QColor color, Qt::PenStyle style)
{
    setLineColor(color);
    setLineStyle(style);
}

void JKQTPGeoBaseDecoratedLine::setAlpha(float alpha)
{
    auto color=getLineColor();
    color.setAlphaF(alpha);
    setLineColor(color);
}

void JKQTPGeoBaseDecoratedLine::setColor(QColor c)
{
    setLineColor(c);
}

void JKQTPGeoBaseDecoratedLine::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
}

QColor JKQTPGeoBaseDecoratedLine::getKeyLabelColor() const
{
    return getLineColor();
}
