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

JKQTPGeoBaseLine::JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQTBasePlotter* parent, DrawMode drawMode):
    JKQTPPlotObject(drawMode, parent)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
}

JKQTPGeoBaseLine::JKQTPGeoBaseLine(JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPPlotObject(drawMode, parent)
{

}

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


JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQTBasePlotter* parent, DrawMode drawMode):
    JKQTPGeoBaseLine(color, lineWidth, style, parent, drawMode)
{
    setFillColor(fillColor);
    setFillStyle(fillStyle);
}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, JKQTBasePlotter* parent, DrawMode drawMode):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, Qt::SolidPattern, parent, drawMode)
{

}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, JKQTBasePlotter* parent, DrawMode drawMode):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, Qt::SolidLine, Qt::SolidPattern, parent, drawMode)
{

}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, JKQTBasePlotter* parent, DrawMode drawMode):
    JKQTPGeoBaseFilled(color, fillColor, 2.0, Qt::SolidLine, Qt::SolidPattern, parent, drawMode)
{
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
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
}


void JKQTPGeoBaseFilled::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    painter.drawRect(rect);

}


JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, Qt::PenStyle style, JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPPlotObject(drawMode, parent)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
    setHeadDecoratorStyle(headStyle);
}

JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPPlotObject(drawMode, parent)
{

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


JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, Qt::PenStyle style, JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPPlotObject(drawMode, parent)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
    setTailDecoratorStyle(tailStyle);
    setHeadDecoratorStyle(headStyle);
}

JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(JKQTBasePlotter *parent, DrawMode drawMode):
    JKQTPPlotObject(drawMode, parent)
{

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
