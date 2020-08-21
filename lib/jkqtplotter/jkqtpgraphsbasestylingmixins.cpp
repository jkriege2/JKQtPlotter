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

#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <QApplication>
#include <QDebug>


JKQTPGraphLineStyleMixin::JKQTPGraphLineStyleMixin()
{
    m_linePen=QPen(QColor("red"), Qt::SolidLine);
    m_linePen.setJoinStyle(Qt::RoundJoin);
    m_linePen.setCapStyle(Qt::RoundCap);
    m_lineWidth=2;
    m_highlightingLineColor=QColor("yellow");
    m_highlightingLineColor.setAlphaF(0.5);
}

void JKQTPGraphLineStyleMixin::initLineStyle(JKQTBasePlotter* parent, int &parentPlotStyle)
{
    if (parent) { // get style settings from parent object
        if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
        m_linePen.setColor(parent->getPlotStyle(parentPlotStyle).color());
        m_linePen.setStyle(parent->getPlotStyle(parentPlotStyle).style());
        m_lineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
        m_highlightingLineColor=getLineColor();
        m_highlightingLineColor.setAlphaF(0.5);
    }
}

JKQTPGraphLineStyleMixin::~JKQTPGraphLineStyleMixin()
{

}

void JKQTPGraphLineStyleMixin::setLineColor(const QColor &__value)
{
    m_linePen.setColor(__value);
}

QColor JKQTPGraphLineStyleMixin::getLineColor() const
{
    return m_linePen.color();
}

void JKQTPGraphLineStyleMixin::setLineStyle(Qt::PenStyle __value)
{
    m_linePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphLineStyleMixin::getLineStyle() const
{
    return m_linePen.style();
}

void JKQTPGraphLineStyleMixin::setLineWidth(double __value)
{
    this->m_lineWidth = __value;
}

double JKQTPGraphLineStyleMixin::getLineWidth() const
{
    return this->m_lineWidth;
}

void JKQTPGraphLineStyleMixin::setLineDashOffset(qreal offset)
{
    m_linePen.setDashOffset(offset);
}

qreal JKQTPGraphLineStyleMixin::getLineDashOffset() const
{
    return m_linePen.dashOffset();
}

void JKQTPGraphLineStyleMixin::setLineDashPattern(const QVector<qreal> &pattern)
{
    m_linePen.setDashPattern(pattern);
    m_linePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphLineStyleMixin::getLineDashPattern() const
{
    return m_linePen.dashPattern();
}

void JKQTPGraphLineStyleMixin::setLineJoinStyle(Qt::PenJoinStyle style)
{
    m_linePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphLineStyleMixin::getLineJoinStyle() const
{
    return m_linePen.joinStyle();
}

void JKQTPGraphLineStyleMixin::setLineCapStyle(Qt::PenCapStyle style)
{
    m_linePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphLineStyleMixin::getLineCapStyle() const
{
    return m_linePen.capStyle();
}

void JKQTPGraphLineStyleMixin::setLineBrush(const QBrush &style)
{
    m_linePen.setBrush(style);
}

QBrush JKQTPGraphLineStyleMixin::getLineBrush() const
{
    return m_linePen.brush();
}


QPen JKQTPGraphLineStyleMixin::getLinePen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const {
    QPen p=m_linePen;
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*m_lineWidth)));
    return p;
}

QPen JKQTPGraphLineStyleMixin::getLinePenForRects(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen p=getLinePen(painter, parent);
    p.setJoinStyle(Qt::MiterJoin);
    return p;
}















JKQTPGraphSymbolStyleMixin::JKQTPGraphSymbolStyleMixin()
{
    m_symbolColor=QColor("red");
    m_symbolType=JKQTPGraphSymbols::JKQTPDefaultSymbol;
    m_symbolSize=12;
    m_symbolFillColor=m_symbolColor.lighter();
    m_symbolLineWidth=1;
}

void JKQTPGraphSymbolStyleMixin::initSymbolStyle(JKQTBasePlotter *parent, int& parentPlotStyle)
{
    if (parent) { // get style settings from parent object
        if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
        m_symbolColor=parent->getPlotStyle(parentPlotStyle).color();
        m_symbolSize=parent->getPlotStyle(parentPlotStyle).symbolSize();
        m_symbolLineWidth=parent->getPlotStyle(parentPlotStyle).symbolLineWidthF();
        m_symbolType=parent->getPlotStyle(parentPlotStyle).symbol();
        m_symbolFillColor=parent->getPlotStyle(parentPlotStyle).symbolFillColor();
    }
}

JKQTPGraphSymbolStyleMixin::~JKQTPGraphSymbolStyleMixin()
{

}

void JKQTPGraphSymbolStyleMixin::setSymbolType(JKQTPGraphSymbols __value)
{
    m_symbolType=__value;
}

JKQTPGraphSymbols JKQTPGraphSymbolStyleMixin::getSymbolType() const
{
    return m_symbolType;
}

void JKQTPGraphSymbolStyleMixin::setSymbolSize(double __value)
{
    m_symbolSize=__value;
}

double JKQTPGraphSymbolStyleMixin::getSymbolSize() const
{
    return m_symbolSize;
}

void JKQTPGraphSymbolStyleMixin::setSymbolColor(const QColor &__value)
{
    m_symbolColor=__value;
}

QColor JKQTPGraphSymbolStyleMixin::getSymbolColor() const
{
    return m_symbolColor;
}

void JKQTPGraphSymbolStyleMixin::setSymbolFillColor(const QColor &__value)
{
    m_symbolFillColor=__value;
}

QColor JKQTPGraphSymbolStyleMixin::getSymbolFillColor() const
{
    return m_symbolFillColor;
}

void JKQTPGraphSymbolStyleMixin::setSymbolLineWidth(double __value)
{
    m_symbolLineWidth=__value;
}

double JKQTPGraphSymbolStyleMixin::getSymbolLineWidth() const
{
    return m_symbolLineWidth;
}

QPen JKQTPGraphSymbolStyleMixin::getSymbolPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const {
    QPen p;
    p.setColor(m_symbolColor);
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*m_symbolLineWidth)));
    p.setStyle(Qt::SolidLine);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

QBrush JKQTPGraphSymbolStyleMixin::getSymbolBrush(JKQTPEnhancedPainter& /*painter*/, JKQTBasePlotter* /*parent*/) const {
    QBrush b;
    b.setColor(m_symbolFillColor);
    return b;
}

void JKQTPGraphSymbolStyleMixin::plotStyledSymbol(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, double x, double y) const
{
    JKQTPPlotSymbol(painter, x, y,m_symbolType, parent->pt2px(painter, m_symbolSize), parent->pt2px(painter, m_symbolLineWidth*parent->getLineWidthMultiplier()), m_symbolColor, m_symbolFillColor);
}

void JKQTPGraphSymbolStyleMixin::plotStyledSymbol(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, double x, double y, JKQTPGraphSymbols type) const
{
    JKQTPPlotSymbol(painter, x, y,type, parent->pt2px(painter, m_symbolSize), parent->pt2px(painter, m_symbolLineWidth*parent->getLineWidthMultiplier()), m_symbolColor, m_symbolFillColor);
}

void JKQTPGraphSymbolStyleMixin::plotStyledSymbol(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, double x, double y, double symbolSize) const
{
    JKQTPPlotSymbol(painter, x, y,m_symbolType, symbolSize, parent->pt2px(painter, m_symbolLineWidth*parent->getLineWidthMultiplier()), m_symbolColor, m_symbolFillColor);
}

void JKQTPGraphSymbolStyleMixin::plotStyledSymbol(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, double x, double y, QColor color, QColor fillColor) const
{
    JKQTPPlotSymbol(painter, x, y,m_symbolType, parent->pt2px(painter, m_symbolSize), parent->pt2px(painter, m_symbolLineWidth*parent->getLineWidthMultiplier()), color, fillColor);
}







JKQTPGraphFillStyleMixin::JKQTPGraphFillStyleMixin()
{
    m_fillColor=QColor("red").lighter();
    m_fillBrush.setColor(m_fillColor);
    m_fillBrush.setStyle(Qt::SolidPattern);
}


void JKQTPGraphFillStyleMixin::initFillStyle(JKQTBasePlotter *parent, int &parentPlotStyle)
{
    if (parent) { // get style settings from parent object
        if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
        m_fillColor=parent->getPlotStyle(parentPlotStyle).fillColor();
        m_fillBrush.setColor(m_fillColor);
        m_fillBrush.setStyle(parent->getPlotStyle(parentPlotStyle).fillStyle());
    }
}

JKQTPGraphFillStyleMixin::~JKQTPGraphFillStyleMixin()
{

}

void JKQTPGraphFillStyleMixin::setFillStyle(Qt::BrushStyle __value)
{
    if (m_fillBrush.style()==Qt::LinearGradientPattern || m_fillBrush.style()==Qt::RadialGradientPattern || m_fillBrush.style()==Qt::ConicalGradientPattern || m_fillBrush.style()==Qt::TexturePattern) {
        m_fillBrush=QBrush(m_fillColor, __value);
    } else {
        m_fillBrush.setStyle(__value);
    }
}

Qt::BrushStyle JKQTPGraphFillStyleMixin::getFillStyle() const
{
    return m_fillBrush.style();
}

void JKQTPGraphFillStyleMixin::setFillColor(const QColor &__value)
{
    m_fillColor=__value;
    m_fillBrush.setColor(m_fillColor);
}

QColor JKQTPGraphFillStyleMixin::getFillColor() const
{
    return m_fillBrush.color();
}

void JKQTPGraphFillStyleMixin::setFillTexture(const QPixmap &__value)
{
    m_fillBrush.setStyle(Qt::TexturePattern);
    m_fillBrush.setTexture(__value);
}

void JKQTPGraphFillStyleMixin::setFillTexture(const QImage &__value)
{
    m_fillBrush.setStyle(Qt::TexturePattern);
    m_fillBrush.setTextureImage(__value);
}

QPixmap JKQTPGraphFillStyleMixin::getFillTexture() const
{
    return m_fillBrush.texture();
}

QImage JKQTPGraphFillStyleMixin::getFillTextureImage() const
{
    return m_fillBrush.textureImage();
}

void JKQTPGraphFillStyleMixin::setFillGradient(const QGradient &__value)
{
    m_fillBrush=QBrush(__value);
}

const QGradient *JKQTPGraphFillStyleMixin::getFillGradient() const
{
    return m_fillBrush.gradient();
}

void JKQTPGraphFillStyleMixin::setFillBrush(const QBrush &b)
{
    m_fillBrush=b;
    m_fillColor=b.color();
}


void JKQTPGraphFillStyleMixin::setFillTransform(const QTransform &b)
{
    m_fillBrush.setTransform(b);
}

QBrush JKQTPGraphFillStyleMixin::getFillBrush(JKQTPEnhancedPainter &/*painter*/, JKQTBasePlotter * /*parent*/) const
{
    return m_fillBrush;
}


void JKQTPGraphLineStyleMixin::setHighlightingLineColor(const QColor &__value)
{
    m_highlightingLineColor=__value;
}

QColor JKQTPGraphLineStyleMixin::getHighlightingLineColor() const
{
    return m_highlightingLineColor;
}

QPen JKQTPGraphLineStyleMixin::getHighlightingLinePen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen p=getLinePen(painter, parent);
    p.setColor(m_highlightingLineColor);
    p.setWidthF(p.widthF()*3.0);
    return p;
}

QPen JKQTPGraphLineStyleMixin::getHighlightingLinePenForRects(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen p=getHighlightingLinePen(painter, parent);
    p.setJoinStyle(Qt::MiterJoin);
    return p;
}

JKQTPGraphTextStyleMixin::JKQTPGraphTextStyleMixin(JKQTBasePlotter *parent)
{
    m_textFontName=parent->getDefaultTextFontName();
    m_textFontSize=parent->getDefaultTextSize();
    m_textColor=parent->getDefaultTextColor();
}

void JKQTPGraphTextStyleMixin::initTextStyle(JKQTBasePlotter *parent, int &parentPlotStyle)
{
    if (parent) { // get style settings from parent object
        if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
        m_textColor=parent->getPlotStyle(parentPlotStyle).color();
    }
}

JKQTPGraphTextStyleMixin::~JKQTPGraphTextStyleMixin()
{

}

void JKQTPGraphTextStyleMixin::setTextFontSize(double __value)
{
    m_textFontSize=__value;
}

double JKQTPGraphTextStyleMixin::getTextFontSize() const
{
    return m_textFontSize;
}

void JKQTPGraphTextStyleMixin::setTextFontName(const QString &__value)
{
    m_textFontName=__value;
}

QString JKQTPGraphTextStyleMixin::getTextFontName() const
{
    return m_textFontName;
}

void JKQTPGraphTextStyleMixin::setTextColor(const QColor &__value)
{
    m_textColor=__value;
}

QColor JKQTPGraphTextStyleMixin::getTextColor() const
{
    return m_textColor;
}

JKQTPGraphDecoratedLineStyleMixin::JKQTPGraphDecoratedLineStyleMixin():
    JKQTPGraphLineStyleMixin()
{
    m_headDecoratorStyle=JKQTPLineDecoratorStyle::JKQTPDefaultLineDecorator;
    m_tailDecoratorStyle=JKQTPLineDecoratorStyle::JKQTPNoDecorator;
    m_tailDecoratorSizeFactor=m_headDecoratorSizeFactor=9.0;
}

void JKQTPGraphDecoratedLineStyleMixin::initDecoratedLineStyle(JKQTBasePlotter *parent, int &parentPlotStyle)
{
    initLineStyle(parent, parentPlotStyle);
}

JKQTPGraphDecoratedLineStyleMixin::~JKQTPGraphDecoratedLineStyleMixin()
{

}

void JKQTPGraphDecoratedLineStyleMixin::setHeadDecoratorStyle(const JKQTPLineDecoratorStyle &__value)
{
    m_headDecoratorStyle=__value;
}

JKQTPLineDecoratorStyle JKQTPGraphDecoratedLineStyleMixin::getHeadDecoratorStyle() const
{
    return m_headDecoratorStyle;
}

void JKQTPGraphDecoratedLineStyleMixin::setTailDecoratorStyle(const JKQTPLineDecoratorStyle &__value)
{
    m_tailDecoratorStyle=__value;
}

JKQTPLineDecoratorStyle JKQTPGraphDecoratedLineStyleMixin::getTailDecoratorStyle() const
{
    return m_tailDecoratorStyle;
}

void JKQTPGraphDecoratedLineStyleMixin::setHeadDecoratorSizeFactor(const double &__value)
{
    m_headDecoratorSizeFactor=__value;
}

double JKQTPGraphDecoratedLineStyleMixin::getHeadDecoratorSizeFactor() const
{
    return m_headDecoratorSizeFactor;
}

void JKQTPGraphDecoratedLineStyleMixin::setTailDecoratorSizeFactor(const double &__value)
{
    m_tailDecoratorSizeFactor=__value;
}

double JKQTPGraphDecoratedLineStyleMixin::getTailDecoratorSizeFactor() const
{
    return m_tailDecoratorSizeFactor;
}
