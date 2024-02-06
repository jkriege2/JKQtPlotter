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

#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
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

void JKQTPGraphLineStyleMixin::initLineStyle(JKQTBasePlotter* parent, int &parentPlotStyle, JKQTPPlotStyleType styletype)
{
    if (parent) { // get style settings from parent object
        if (styletype==JKQTPPlotStyleType::Annotation) {
            m_linePen=QPen(parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultLineStyle);
            m_linePen.setColor(parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultColor);
            m_linePen.setWidthF(parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultLineWidth);
            m_lineWidth=m_linePen.widthF();
            m_highlightingLineColor=getLineColor();
            m_highlightingLineColor.setAlphaF(0.5);
        } else {
            if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
            const JKQTBasePlotter::JKQTPPen pen=parent->getPlotStyle(parentPlotStyle, styletype);
            m_linePen.setColor(pen.color());
            m_linePen.setStyle(pen.style());
            m_lineWidth=pen.widthF();
            m_highlightingLineColor=getLineColor();
            m_highlightingLineColor.setAlphaF(0.5);
        }
    }
}

JKQTPGraphLineStyleMixin::~JKQTPGraphLineStyleMixin()
{

}

void JKQTPGraphLineStyleMixin::setLineColor(const QColor &__value)
{
    m_linePen.setColor(__value);
}

void JKQTPGraphLineStyleMixin::setLineColor(const QColor &__value, double alpha)
{
    QColor c=__value;
    c.setAlphaF(alpha);
    setLineColor(c);
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


QPen JKQTPGraphLineStyleMixin::getKeyLinePen(JKQTPEnhancedPainter& painter, const QRectF& rect, JKQTBasePlotter* parent) const {
    QPen p=m_linePen;
    p.setWidthF(getKeyLineWidthPx(painter, rect, parent));
    return p;
}

QPen JKQTPGraphLineStyleMixin::getLinePenForRects(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen p=getLinePen(painter, parent);
    p.setJoinStyle(Qt::MiterJoin);
    return p;
}









JKQTPGraphLinesCompressionMixin::JKQTPGraphLinesCompressionMixin():
    m_useNonvisibleLineCompression(false),
    m_nonvisibleLineCompressionAgressiveness(1)
{

}

JKQTPGraphLinesCompressionMixin::~JKQTPGraphLinesCompressionMixin()
{

}

void JKQTPGraphLinesCompressionMixin::setUseNonvisibleLineCompression(bool _useNonvisibleLineCompression)
{
    m_useNonvisibleLineCompression=_useNonvisibleLineCompression;
}

bool JKQTPGraphLinesCompressionMixin::getUseNonvisibleLineCompression() const
{
    return m_useNonvisibleLineCompression;
}

void JKQTPGraphLinesCompressionMixin::setNonvisibleLineCompressionAgressiveness(double Agressiveness)
{
    m_nonvisibleLineCompressionAgressiveness=Agressiveness;
}

double JKQTPGraphLinesCompressionMixin::getNonvisibleLineCompressionAgressiveness() const
{
    return m_nonvisibleLineCompressionAgressiveness;
}







JKQTPGraphSymbolStyleMixin::JKQTPGraphSymbolStyleMixin()
{
    m_symbolColor=QColor("red");
    m_symbolType=JKQTPGraphSymbols::JKQTPDefaultSymbol;
    m_symbolSize=12;
    m_symbolFillColor=m_symbolColor.lighter();
    m_symbolLineWidth=1;
    m_symbolFontName=QGuiApplication::font().family();
}

void JKQTPGraphSymbolStyleMixin::initSymbolStyle(JKQTBasePlotter *parent, int& parentPlotStyle, JKQTPPlotStyleType styletype)
{
    if (parent) { // get style settings from parent object
        if (styletype==JKQTPPlotStyleType::Annotation) {
            m_symbolType=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultSymbol;
            m_symbolColor=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultColor;
            m_symbolLineWidth=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultSymbolLineWidth;
            m_symbolSize=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultSymbolSize;
            m_symbolFillColor=JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.symbolFillColorDerivationMode, m_symbolColor);
        } else {
            if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
            const JKQTBasePlotter::JKQTPPen pen=parent->getPlotStyle(parentPlotStyle, styletype);
            m_symbolColor=pen.color();
            m_symbolSize=pen.symbolSize();
            m_symbolLineWidth=pen.symbolLineWidthF();
            m_symbolType=pen.symbol();
            m_symbolFillColor=pen.symbolFillColor();
            m_symbolFontName=parent->getDefaultTextFontName();
        }
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

void JKQTPGraphSymbolStyleMixin::setSymbolColor(const QColor &__value, double alpha)
{
    m_symbolColor=__value;
    m_symbolColor.setAlphaF(alpha);
}

QColor JKQTPGraphSymbolStyleMixin::getSymbolColor() const
{
    return m_symbolColor;
}

void JKQTPGraphSymbolStyleMixin::setSymbolFillColor(const QColor &__value)
{
    m_symbolFillColor=__value;
}

void JKQTPGraphSymbolStyleMixin::setSymbolFillColor(const QColor &__value, double alpha)
{
    m_symbolFillColor=__value;
    m_symbolFillColor.setAlphaF(alpha);
}

QColor JKQTPGraphSymbolStyleMixin::getSymbolFillColor() const
{
    return m_symbolFillColor;
}

void JKQTPGraphSymbolStyleMixin::setSymbolAlpha(double alpha)
{
    m_symbolColor.setAlphaF(alpha);
    m_symbolFillColor.setAlphaF(alpha);
}

void JKQTPGraphSymbolStyleMixin::setSymbolLineWidth(double __value)
{
    m_symbolLineWidth=__value;
}

double JKQTPGraphSymbolStyleMixin::getSymbolLineWidth() const
{
    return m_symbolLineWidth;
}

void JKQTPGraphSymbolStyleMixin::setSymbolFontName(const QString &__value)
{
    m_symbolFontName=__value;
}

QString JKQTPGraphSymbolStyleMixin::getSymbolFontName() const
{
    return m_symbolFontName;
}

double JKQTPGraphSymbolStyleMixin::getKeySymbolLineWidthPx(JKQTPEnhancedPainter& painter, const QRectF& keyRect, const JKQTBasePlotter *parent, double maxSymbolSizeFracton) const
{
    //const double minSize=qMin(keyRect.width(), keyRect.height());
    double symbolWidth=parent->pt2px(painter, this->getSymbolLineWidth()*parent->getLineWidthMultiplier());
    double symbolSize=getKeySymbolSizePx(painter, keyRect, parent, maxSymbolSizeFracton);
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;
    return symbolWidth;
}

double JKQTPGraphSymbolStyleMixin::getKeySymbolSizePx(JKQTPEnhancedPainter& painter, const QRectF& keyRect, const JKQTBasePlotter *parent, double maxSymbolSizeFracton) const
{
    const double minSize=qMin(keyRect.width(), keyRect.height());
    double symbolSize=parent->pt2px(painter, this->getSymbolSize());
    if (symbolSize>minSize*maxSymbolSizeFracton) symbolSize=minSize*maxSymbolSizeFracton;
    return symbolSize;
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

QFont JKQTPGraphSymbolStyleMixin::getSymbolFont() const
{
    QFont f(m_symbolFontName, m_symbolSize);
    f.setStyleStrategy(QFont::PreferDefault);
    return f;
}















JKQTPGraphFillStyleMixin::JKQTPGraphFillStyleMixin()
{
    m_fillColor=QColor("red").lighter();
    m_fillBrush.setColor(m_fillColor);
    m_fillBrush.setStyle(Qt::SolidPattern);
}


void JKQTPGraphFillStyleMixin::initFillStyleInvertedColor(JKQTPGraphFillStyleMixin *other)
{
    if (other) { // get style settings from parent object
        m_fillColor=other->getFillColor();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        float h=0,s=0,v=0,a=0;
#else
        qreal h=0,s=0,v=0,a=0;
#endif
        m_fillColor.getHsvF(&h, &s, &v, &a);
        h=std::fmod(h+120.0/360.0, 1.0);
        m_fillColor.setHsvF(h,s,v,a);
        m_fillBrush.setColor(m_fillColor);
        m_fillBrush.setStyle(other->getFillStyle());
    }
}

void JKQTPGraphFillStyleMixin::initFillStyle(JKQTBasePlotter *parent, int &parentPlotStyle, JKQTPPlotStyleType styletype)
{
    if (parent) { // get style settings from parent object
        if (styletype==JKQTPPlotStyleType::Annotation) {
            m_fillColor=JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.fillColorDerivationMode, parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultColor);
            m_fillBrush=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultFillStyle.brush(m_fillColor);
        } else {
            if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
            const JKQTBasePlotter::JKQTPPen pen=parent->getPlotStyle(parentPlotStyle, styletype);
            m_fillColor=pen.fillColor();
            m_fillBrush=pen.fillStyle().brush(m_fillColor);
        }
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

void JKQTPGraphFillStyleMixin::setFillColor(const QColor &__value, double alpha)
{
    m_fillColor=__value;
    m_fillColor.setAlphaF(alpha);
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

void JKQTPGraphLineStyleMixin::setHighlightingLineColor(const QColor &__value, double alpha)
{
    m_highlightingLineColor=__value;
    m_highlightingLineColor.setAlphaF(alpha);
}

QColor JKQTPGraphLineStyleMixin::getHighlightingLineColor() const
{
    return m_highlightingLineColor;
}

double JKQTPGraphLineStyleMixin::getKeyLineWidthPx(JKQTPEnhancedPainter &painter, const QRectF &keyRect, const JKQTBasePlotter *parent) const
{
    const double minSize=qMin(keyRect.width(), keyRect.height());
    double lineWidth=parent->pt2px(painter, this->getLineWidth()*parent->getLineWidthMultiplier());
    if (lineWidth>0.4*minSize) lineWidth=0.4*minSize;
    return lineWidth;
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
    m_textFontSize=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultFontSize;
    m_textColor=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultTextColor;
    m_textFontName=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultFontName;
}

void JKQTPGraphTextStyleMixin::initTextStyle(JKQTBasePlotter *parent, int &/*parentPlotStyle*/, JKQTPPlotStyleType styletype)
{
    if (parent) { // get style settings from parent object
        m_textColor=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultTextColor;
        m_textFontName=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultFontName;
        if (styletype==JKQTPPlotStyleType::Annotation) {
            m_textFontSize=parent->getCurrentPlotterStyle().graphsStyle.annotationStyle.defaultFontSize;
        } else {
            m_textFontSize=QApplication::font().pointSizeF();
        }
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

void JKQTPGraphTextStyleMixin::setTextColor(const QColor &__value, double alpha)
{
    m_textColor=__value;
    m_textColor.setAlphaF(alpha);
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
    m_tailDecoratorSizeFactor=m_headDecoratorSizeFactor=8.0;
}

void JKQTPGraphDecoratedLineStyleMixin::initDecoratedLineStyle(JKQTBasePlotter *parent, int &parentPlotStyle, JKQTPPlotStyleType styletype)
{
    initLineStyle(parent, parentPlotStyle, styletype);
    if (parent) { // get style settings from parent object
        m_headDecoratorStyle=parent->getCurrentPlotterStyle().graphsStyle.getGraphStyleByType(styletype).defaultHeadDecoratorStyle;
        m_headDecoratorSizeFactor=m_tailDecoratorSizeFactor=parent->getCurrentPlotterStyle().graphsStyle.getGraphStyleByType(styletype).defaultHeadDecoratorSizeFactor;
    }
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

double JKQTPGraphDecoratedLineStyleMixin::calcTailDecoratorSize(double line_width) const
{
    return JKQTPLineDecoratorStyleCalcDecoratorSize(line_width, m_tailDecoratorSizeFactor);
}

double JKQTPGraphDecoratedLineStyleMixin::calcHeadDecoratorSize(double line_width) const
{
    return JKQTPLineDecoratorStyleCalcDecoratorSize(line_width, m_headDecoratorSizeFactor);
}



JKQTPGraphDecoratedHeadLineStyleMixin::JKQTPGraphDecoratedHeadLineStyleMixin():
    JKQTPGraphLineStyleMixin()
{
    m_headDecoratorStyle=JKQTPLineDecoratorStyle::JKQTPDefaultLineDecorator;
    m_headDecoratorSizeFactor=8.0;
}

void JKQTPGraphDecoratedHeadLineStyleMixin::initDecoratedHeadLineStyle(JKQTBasePlotter *parent, int &parentPlotStyle, JKQTPPlotStyleType styletype)
{
    initLineStyle(parent, parentPlotStyle, styletype);
    if (parent) { // get style settings from parent object
        m_headDecoratorStyle=parent->getCurrentPlotterStyle().graphsStyle.getGraphStyleByType(styletype).defaultHeadDecoratorStyle;
        m_headDecoratorSizeFactor=parent->getCurrentPlotterStyle().graphsStyle.getGraphStyleByType(styletype).defaultHeadDecoratorSizeFactor;
    }
}

JKQTPGraphDecoratedHeadLineStyleMixin::~JKQTPGraphDecoratedHeadLineStyleMixin()
{

}

void JKQTPGraphDecoratedHeadLineStyleMixin::setHeadDecoratorStyle(const JKQTPLineDecoratorStyle &__value)
{
    m_headDecoratorStyle=__value;
}

JKQTPLineDecoratorStyle JKQTPGraphDecoratedHeadLineStyleMixin::getHeadDecoratorStyle() const
{
    return m_headDecoratorStyle;
}


void JKQTPGraphDecoratedHeadLineStyleMixin::setHeadDecoratorSizeFactor(const double &__value)
{
    m_headDecoratorSizeFactor=__value;
}

double JKQTPGraphDecoratedHeadLineStyleMixin::getHeadDecoratorSizeFactor() const
{
    return m_headDecoratorSizeFactor;
}

double JKQTPGraphDecoratedHeadLineStyleMixin::calcHeadDecoratorSize(double line_width) const
{
    return JKQTPLineDecoratorStyleCalcDecoratorSize(line_width, m_headDecoratorSizeFactor);
}



JKQTPGraphLineAndFillStyleMixin::JKQTPGraphLineAndFillStyleMixin():
    m_drawLine(true), m_fillCurve(true)
{

}

void JKQTPGraphLineAndFillStyleMixin::setDrawLine(bool __value)
{
    m_drawLine=__value;
}

bool JKQTPGraphLineAndFillStyleMixin::getDrawLine() const
{
    return m_drawLine;
}

bool JKQTPGraphLineAndFillStyleMixin::doDrawLine() const
{
    return m_drawLine;
}

void JKQTPGraphLineAndFillStyleMixin::setFillCurve(bool __value)
{
    m_fillCurve=__value;
}

bool JKQTPGraphLineAndFillStyleMixin::getFillCurve() const
{
    return m_fillCurve;
}

bool JKQTPGraphLineAndFillStyleMixin::doFillCurve() const
{
    return m_fillCurve;
}
