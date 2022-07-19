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



#include "jkqtplotter/graphs/jkqtpboxplotstylingmixins.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"



JKQTPGraphBoxplotStyleMixin::JKQTPGraphBoxplotStyleMixin()
{

    m_whiskerLinePen=QPen(getLineColor(), getLineWidth());
    whiskerLineWidth=getLineWidth();
    m_whiskerCapLinePen=QPen(getLineColor(), getLineWidth());
    whiskerCapLineWidth=getLineWidth();
    m_medianLinePen=QPen(getLineColor(), getLineWidth());
    medianLineWidth=getLineWidth();

    m_meanSymbolLinePen=QPen(getLineColor(), getLineWidth());
    m_meanSymbolLineWidth=1;
    meanMode=MeanAsSymbol;
    m_meanSymbolType=JKQTPGraphSymbols::JKQTPDefaultSymbol;
    m_meanSymbolSize=12;
    m_meanSymbolFillColor=m_meanSymbolLinePen.color().lighter();


    boxWidthAbsolute=m_meanSymbolSize*3.0;
    relativeWhiskerWidth=0.5;
    relativeNotchIndent=0.25;
    drawBox=true;

}


void JKQTPGraphBoxplotStyleMixin::initBoxplotStyle(JKQTBasePlotter *parent, int &parentPlotStyle)
{
    setFillStyle(Qt::SolidPattern);
    setFillColor(parent->getCurrentPlotterStyle().plotBackgroundBrush.color());
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Boxplot);
    initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Boxplot);
    if (parent) { // get style settings from parent object
        if (parentPlotStyle<0) parentPlotStyle=parent->getNextStyle();
        const JKQTBasePlotter::JKQTPPen pen=parent->getPlotStyle(parentPlotStyle, JKQTPPlotStyleType::Boxplot);
        m_whiskerLinePen.setColor(pen.color());
        m_whiskerLinePen.setStyle(pen.style());
        whiskerLineWidth=pen.widthF();
        m_whiskerCapLinePen.setColor(pen.color());
        m_whiskerCapLinePen.setStyle(pen.style());
        whiskerCapLineWidth=pen.widthF();
        m_medianLinePen.setColor(pen.color());
        m_medianLinePen.setStyle(pen.style());
        medianLineWidth=pen.widthF();
        m_meanSymbolLinePen=QPen(pen.color(), pen.style());
        m_meanSymbolSize=pen.symbolSize();
        m_meanSymbolLineWidth=pen.symbolLineWidthF();
        m_meanSymbolType=pen.symbol();
        m_meanSymbolFillColor=pen.symbolFillColor();
    }

    setWhiskerLineColor(getLineColor());
    setWhiskerCapLineColor(getLineColor());
    setMedianLineColor(getLineColor());
    setMeanSymbolType(JKQTPPlus);

    if (m_meanSymbolSize>0) {
        boxWidthAbsolute=m_meanSymbolSize*3.0;
    }
}

void JKQTPGraphBoxplotStyleMixin::setBoxWidthAbsolute(double __value)
{
    boxWidthAbsolute=__value;
}

double JKQTPGraphBoxplotStyleMixin::getBoxWidthAbsolute() const
{
    return boxWidthAbsolute;
}

void JKQTPGraphBoxplotStyleMixin::setBoxplotColor(QColor c, JKQTBasePlotter *parent)
{
    setLineColor(c);
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.barchartStyle.fillColorDerivationMode, c));
    setMeanColor(c);
    setMeanFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.barchartStyle.symbolFillColorDerivationMode, c));
    setWhiskerLineColor(getLineColor());
    setWhiskerCapLineColor(getLineColor());
    setMedianLineColor(getLineColor());
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
}

void JKQTPGraphBoxplotStyleMixin::setBoxplotColor(QColor c, QColor bc, JKQTBasePlotter *parent)
{
    setBoxplotColor(c, parent);
    setFillColor(bc);
    setMeanFillColor(bc);
}

void JKQTPGraphBoxplotStyleMixin::setDrawBox(bool __value)
{
    drawBox=__value;
}

bool JKQTPGraphBoxplotStyleMixin::getDrawBox() const
{
    return drawBox;
}

void JKQTPGraphBoxplotStyleMixin::setRelativeWhiskerWidth(double __value)
{
    relativeWhiskerWidth=__value;
}

double JKQTPGraphBoxplotStyleMixin::getRelativeWhiskerWidth() const
{
    return relativeWhiskerWidth;
}

void JKQTPGraphBoxplotStyleMixin::setRelativeNotchIndent(double __value)
{
    relativeNotchIndent=__value;
}

double JKQTPGraphBoxplotStyleMixin::getRelativeNotchIndent() const
{
    return relativeNotchIndent;
}

void JKQTPGraphBoxplotStyleMixin::setMeanMode(JKQTPGraphBoxplotStyleMixin::MeanMode __value)
{
    meanMode=__value;
}

JKQTPGraphBoxplotStyleMixin::MeanMode JKQTPGraphBoxplotStyleMixin::getMeanMode() const
{
    return meanMode;
}







void JKQTPGraphBoxplotStyleMixin::setMeanSymbolType(JKQTPGraphSymbols __value)
{
    m_meanSymbolType=__value;
}

JKQTPGraphSymbols JKQTPGraphBoxplotStyleMixin::getMeanSymbolType() const
{
    return m_meanSymbolType;
}

void JKQTPGraphBoxplotStyleMixin::setMeanSize(double __value)
{
    m_meanSymbolSize=__value;
}

double JKQTPGraphBoxplotStyleMixin::getMeanSize() const
{
    return m_meanSymbolSize;
}

void JKQTPGraphBoxplotStyleMixin::setMeanColor(const QColor &__value)
{
    m_meanSymbolLinePen.setColor(__value);
}

QColor JKQTPGraphBoxplotStyleMixin::getMeanColor() const
{
    return m_meanSymbolLinePen.color();
}

void JKQTPGraphBoxplotStyleMixin::setMeanFillColor(const QColor &__value)
{
    m_meanSymbolFillColor=__value;
}

QColor JKQTPGraphBoxplotStyleMixin::getMeanFillColor() const
{
    return m_meanSymbolFillColor;
}

void JKQTPGraphBoxplotStyleMixin::setMeanLineWidth(double __value)
{
    m_meanSymbolLineWidth=__value;
}

double JKQTPGraphBoxplotStyleMixin::getMeanLineWidth() const
{
    return m_meanSymbolLineWidth;
}


void JKQTPGraphBoxplotStyleMixin::setMeanLineStyle(Qt::PenStyle __value)
{
    this->m_meanSymbolLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphBoxplotStyleMixin::getMeanLineStyle() const
{
    return this->m_meanSymbolLinePen.style();
}


void JKQTPGraphBoxplotStyleMixin::setMeanLineDashOffset(qreal offset)
{
    m_meanSymbolLinePen.setDashOffset(offset);
}

qreal JKQTPGraphBoxplotStyleMixin::getMeanLineDashOffset() const
{
    return m_meanSymbolLinePen.dashOffset();
}

void JKQTPGraphBoxplotStyleMixin::setMeanLineDashPattern(const QVector<qreal> &pattern)
{
    m_meanSymbolLinePen.setDashPattern(pattern);
    m_meanSymbolLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphBoxplotStyleMixin::getMeanLineDashPattern() const
{
    return m_meanSymbolLinePen.dashPattern();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineWidth(double __value)
{
    medianLineWidth=__value;
}

double JKQTPGraphBoxplotStyleMixin::getMedianLineWidth() const
{
    return medianLineWidth;
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineColor(QColor __value)
{
    m_medianLinePen.setColor(__value);
}

QColor JKQTPGraphBoxplotStyleMixin::getMedianLineColor() const
{
    return m_medianLinePen.color();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineStyle(Qt::PenStyle __value)
{
    this->m_medianLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphBoxplotStyleMixin::getMedianLineStyle() const
{
    return this->m_medianLinePen.style();
}


void JKQTPGraphBoxplotStyleMixin::setMedianLineDashOffset(qreal offset)
{
    m_medianLinePen.setDashOffset(offset);
}

qreal JKQTPGraphBoxplotStyleMixin::getMedianLineDashOffset() const
{
    return m_medianLinePen.dashOffset();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineDashPattern(const QVector<qreal> &pattern)
{
    m_medianLinePen.setDashPattern(pattern);
    m_medianLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphBoxplotStyleMixin::getMedianLineDashPattern() const
{
    return m_medianLinePen.dashPattern();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineJoinStyle(Qt::PenJoinStyle style)
{
    m_medianLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphBoxplotStyleMixin::getMedianLineJoinStyle() const
{
    return m_medianLinePen.joinStyle();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineCapStyle(Qt::PenCapStyle style)
{
    m_medianLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphBoxplotStyleMixin::getMedianLineCapStyle() const
{
    return m_medianLinePen.capStyle();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineBrush(const QBrush &style)
{
    m_medianLinePen.setBrush(style);
}

QBrush JKQTPGraphBoxplotStyleMixin::getMedianLineBrush() const
{
    return m_medianLinePen.brush();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineStyle(Qt::PenStyle __value)
{
    this->m_whiskerLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphBoxplotStyleMixin::getWhiskerLineStyle() const
{
    return this->m_whiskerLinePen.style();
}


void JKQTPGraphBoxplotStyleMixin::setWhiskerLineWidth(double __value)
{
    whiskerLineWidth=__value;
}

double JKQTPGraphBoxplotStyleMixin::getWhiskerLineWidth() const
{
    return whiskerLineWidth;
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineColor(QColor __value)
{
    m_whiskerLinePen.setColor(__value);
}

QColor JKQTPGraphBoxplotStyleMixin::getWhiskerLineColor() const
{
    return m_whiskerLinePen.color();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineDashOffset(qreal offset)
{
    m_whiskerLinePen.setDashOffset(offset);
}

qreal JKQTPGraphBoxplotStyleMixin::getWhiskerLineDashOffset() const
{
    return m_whiskerLinePen.dashOffset();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineDashPattern(const QVector<qreal> &pattern)
{
    m_whiskerLinePen.setDashPattern(pattern);
    m_whiskerLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphBoxplotStyleMixin::getWhiskerLineDashPattern() const
{
    return m_whiskerLinePen.dashPattern();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineJoinStyle(Qt::PenJoinStyle style)
{
    m_whiskerLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphBoxplotStyleMixin::getWhiskerLineJoinStyle() const
{
    return m_whiskerLinePen.joinStyle();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineCapStyle(Qt::PenCapStyle style)
{
    m_whiskerLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphBoxplotStyleMixin::getWhiskerLineCapStyle() const
{
    return m_whiskerLinePen.capStyle();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineBrush(const QBrush &style)
{
    m_whiskerLinePen.setBrush(style);
}

QBrush JKQTPGraphBoxplotStyleMixin::getWhiskerLineBrush() const
{
    return m_whiskerLinePen.brush();
}

QPen JKQTPGraphBoxplotStyleMixin::getWhiskerPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_whiskerLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*whiskerLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    pw.setCapStyle(Qt::FlatCap);
    return pw;
}








void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineStyle(Qt::PenStyle __value)
{
    this->m_whiskerCapLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineStyle() const
{
    return this->m_whiskerCapLinePen.style();
}


void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineWidth(double __value)
{
    whiskerCapLineWidth=__value;
}

double JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineWidth() const
{
    return whiskerCapLineWidth;
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineColor(QColor __value)
{
    m_whiskerCapLinePen.setColor(__value);
}

QColor JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineColor() const
{
    return m_whiskerCapLinePen.color();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineDashOffset(qreal offset)
{
    m_whiskerCapLinePen.setDashOffset(offset);
}

qreal JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineDashOffset() const
{
    return m_whiskerCapLinePen.dashOffset();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineDashPattern(const QVector<qreal> &pattern)
{
    m_whiskerCapLinePen.setDashPattern(pattern);
    m_whiskerCapLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineDashPattern() const
{
    return m_whiskerCapLinePen.dashPattern();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineJoinStyle(Qt::PenJoinStyle style)
{
    m_whiskerCapLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineJoinStyle() const
{
    return m_whiskerCapLinePen.joinStyle();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineCapStyle(Qt::PenCapStyle style)
{
    m_whiskerCapLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineCapStyle() const
{
    return m_whiskerCapLinePen.capStyle();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerCapLineBrush(const QBrush &style)
{
    m_whiskerCapLinePen.setBrush(style);
}

QBrush JKQTPGraphBoxplotStyleMixin::getWhiskerCapLineBrush() const
{
    return m_whiskerCapLinePen.brush();
}

QPen JKQTPGraphBoxplotStyleMixin::getWhiskerCapPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_whiskerCapLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*whiskerCapLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    return pw;
}









QPen JKQTPGraphBoxplotStyleMixin::getMedianPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_medianLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*medianLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    pw.setCapStyle(Qt::FlatCap);
    return pw;
}

QPen JKQTPGraphBoxplotStyleMixin::getMeanSymbolPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const {
    QPen p=m_meanSymbolLinePen;
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*m_meanSymbolLineWidth)));
    p.setStyle(Qt::SolidLine);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

QPen JKQTPGraphBoxplotStyleMixin::getMeanLinePen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const {
    QPen p=m_meanSymbolLinePen;
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*m_meanSymbolLineWidth)));
    p.setJoinStyle(Qt::MiterJoin);
    p.setCapStyle(Qt::FlatCap);
    return p;
}

QBrush JKQTPGraphBoxplotStyleMixin::getMeanSymbolBrush(JKQTPEnhancedPainter& /*painter*/, JKQTBasePlotter* /*parent*/) const {
    QBrush b;
    b.setColor(m_meanSymbolFillColor);
    return b;
}

void JKQTPGraphBoxplotStyleMixin::plotStyledMeanSymbol(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, double x, double y) const
{
    JKQTPPlotSymbol(painter, x, y,m_meanSymbolType, parent->pt2px(painter, m_meanSymbolSize), parent->pt2px(painter, m_meanSymbolLineWidth*parent->getLineWidthMultiplier()), m_meanSymbolLinePen.color(), m_meanSymbolFillColor);
}



void JKQTPGraphBoxplotStyleMixin::plotVerticalBoxplot(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, double xp, double xpleft, double xpright, double minp, double q25p, double medianp, double q75p, double maxp, double meanp, double notchLowerp, double notchUpperp) const
{
    if (JKQTPIsOKFloat(xp) ) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(Qt::NoBrush);


        double minstop=q25p;
        double maxstop=q75p;
        if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(medianp)) minstop=medianp;
        else if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(meanp)) minstop=meanp;
        else if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(maxp)) minstop=maxp;
        if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(medianp)) maxstop=medianp;
        else if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(meanp)) maxstop=meanp;
        else if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(minp)) maxstop=minp;
        const double wl=fabs(xpright-xp);
        const double wr=fabs(xpleft-xp);
        const double xprightWhisker=xp+wr*getRelativeWhiskerWidth();
        const double xpleftWhisker=xp-wl*getRelativeWhiskerWidth();

        // whisker lines
        painter.setPen(getWhiskerPen(painter, parent));
        if (JKQTPIsOKFloat(minp) && JKQTPIsOKFloat(minstop)) {
            painter.drawLine(QLineF(xp, minp, xp, minstop));
        }
        if (JKQTPIsOKFloat(maxp) && JKQTPIsOKFloat(maxstop)) {
            painter.drawLine(QLineF(xp, maxp, xp, maxstop));
        }

        // whisker caps
        if (JKQTPIsOKFloat(xpleftWhisker) && JKQTPIsOKFloat(xprightWhisker)){
            painter.setPen(getWhiskerCapPen(painter, parent));
            if (JKQTPIsOKFloat(minp)) {
                painter.drawLine(QLineF(xpleftWhisker, minp, xprightWhisker, minp));
            }
            if (JKQTPIsOKFloat(maxp)) {
                painter.drawLine(QLineF(xpleftWhisker, maxp, xprightWhisker, maxp));
            }
        }

        // draw main box
        double medianMin=xpleft;
        double medianMax=xpright;
        if (JKQTPIsOKFloat(q25p) && JKQTPIsOKFloat(q75p) && getDrawBox()) {
            painter.setPen(getLinePenForRects(painter, parent));
            painter.setBrush(getFillBrush(painter, parent));
            if (getDrawBox()) {
                QPolygonF poly;
                if (JKQTPIsOKFloat(notchLowerp) && JKQTPIsOKFloat(notchUpperp)) {
                    // notched boxplot
                    poly<<QPointF(xpleft, q75p)<<QPointF(xpleft, notchUpperp)<<QPointF(xpleft+wl*getRelativeNotchIndent(), medianp)<<QPointF(xpleft, notchLowerp)<<QPointF(xpleft, q25p)
                       <<QPointF(xpright, q25p)<<QPointF(xpright, notchLowerp)<<QPointF(xpright-wr*getRelativeNotchIndent(), medianp)<<QPointF(xpright, notchUpperp)<<QPointF(xpright, q75p)
                      <<QPointF(xpleft, q75p);
                    medianMin=xpleft+wl*getRelativeNotchIndent();
                    medianMax=xpright-wr*getRelativeNotchIndent();
                } else {
                    // simple rectangle
                    poly<<QPointF(xpleft, q75p)<<QPointF(xpright, q75p)<<QPointF(xpright, q25p)<<QPointF(xpleft, q25p)<<QPointF(xpleft, q75p);
                }
                painter.drawPolygon(poly);
            }
        }

        // draw median
        if (JKQTPIsOKFloat(medianp)) {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(getMedianPen(painter, parent));
            painter.drawLine(QLineF(medianMin, medianp, medianMax, medianp));
        }

        // draw mean
        if (JKQTPIsOKFloat(meanp)) {
            painter.setBrush(Qt::NoBrush);
            if (getMeanMode()==MeanAsSymbol) {
                plotStyledMeanSymbol(parent, painter,xp,meanp);
            } else {
                painter.setPen(getMeanLinePen(painter, parent));
                painter.drawLine(QLineF(xpleft, meanp, xpright, meanp));
            }
        }
    }
}

void JKQTPGraphBoxplotStyleMixin::plotHorizontalBoxplot(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, double yp, double ypbottom, double yptop, double minp, double q25p, double medianp, double q75p, double maxp, double meanp, double notchLowerp, double notchUpperp) const
{
    if (JKQTPIsOKFloat(yp) ) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(Qt::NoBrush);


        double minstop=q25p;
        double maxstop=q75p;
        if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(medianp)) minstop=medianp;
        else if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(meanp)) minstop=meanp;
        else if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(maxp)) minstop=maxp;
        if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(medianp)) maxstop=medianp;
        else if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(meanp)) maxstop=meanp;
        else if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(minp)) maxstop=minp;
        const double wbot=fabs(ypbottom-yp);
        const double wtop=fabs(yptop-yp);
        const double xbottomWhisker=yp+wbot*getRelativeWhiskerWidth();
        const double xptopWhisker=yp-wtop*getRelativeWhiskerWidth();

        // whisker lines
        painter.setPen(getWhiskerPen(painter, parent));
        if (JKQTPIsOKFloat(minp) && JKQTPIsOKFloat(minstop)) {
            painter.drawLine(QLineF(minp, yp, minstop, yp));
        }
        if (JKQTPIsOKFloat(maxp) && JKQTPIsOKFloat(maxstop)) {
            painter.drawLine(QLineF(maxp, yp, maxstop, yp));
        }

        // whisker caps
        if (JKQTPIsOKFloat(xptopWhisker) && JKQTPIsOKFloat(xbottomWhisker)){
            painter.setPen(getWhiskerCapPen(painter, parent));
            if (JKQTPIsOKFloat(minp)) {
                painter.drawLine(QLineF(minp, xptopWhisker, minp, xbottomWhisker));
            }
            if (JKQTPIsOKFloat(maxp)) {
                painter.drawLine(QLineF(maxp, xptopWhisker, maxp, xbottomWhisker));
            }
        }

        // draw main box
        double medianMin=yptop;
        double medianMax=ypbottom;
        if (JKQTPIsOKFloat(q25p) && JKQTPIsOKFloat(q75p) && getDrawBox()) {
            painter.setPen(getLinePenForRects(painter, parent));
            painter.setBrush(getFillBrush(painter, parent));
            if (getDrawBox()) {
                QPolygonF poly;
                if (JKQTPIsOKFloat(notchLowerp) && JKQTPIsOKFloat(notchUpperp)) {
                    // notched boxplot
                    poly<<QPointF(q75p, yptop)<<QPointF(notchUpperp, yptop)<<QPointF(medianp, yptop-wtop*getRelativeNotchIndent())<<QPointF(notchLowerp, yptop)<<QPointF(q25p, yptop)
                       <<QPointF(q25p, ypbottom)<<QPointF(notchLowerp, ypbottom)<<QPointF(medianp, ypbottom+wbot*getRelativeNotchIndent())<<QPointF(notchUpperp, ypbottom)<<QPointF(q75p, ypbottom)
                      <<QPointF(q75p, yptop);
                    medianMin=yptop-wtop*getRelativeNotchIndent();
                    medianMax=ypbottom+wbot*getRelativeNotchIndent();
                } else {
                    // simple rectangle
                    poly<<QPointF(q75p, yptop)<<QPointF(q75p, ypbottom)<<QPointF(q25p, ypbottom)<<QPointF(q25p, yptop)<<QPointF(q75p, yptop);
                }
                painter.drawPolygon(poly);
            }
        }

        // draw median
        if (JKQTPIsOKFloat(medianp)) {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(getMedianPen(painter, parent));
            painter.drawLine(QLineF(medianp, medianMin, medianp, medianMax));
        }

        // draw mean
        if (JKQTPIsOKFloat(meanp)) {
            painter.setBrush(Qt::NoBrush);
            if (getMeanMode()==MeanAsSymbol) {
                plotStyledMeanSymbol(parent, painter,meanp,yp);
            } else {
                painter.setPen(getMeanLinePen(painter, parent));
                painter.drawLine(QLineF(meanp, yptop, meanp, ypbottom));
            }
        }
    }
}

void JKQTPGraphBoxplotStyleMixin::plotVerticalKeyMarker(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePenForRects(painter, parent);
    QPen pw=getWhiskerPen(painter, parent);
    QPen pm=getMedianPen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);

    p.setWidthF(qMin(0.5, p.widthF()));
    p.setStyle(Qt::SolidLine);
    pw.setWidthF(qMin(0.5, pw.widthF()));
    pw.setStyle(Qt::SolidLine);
    pm.setWidthF(qMin(0.5, pm.widthF()));
    pm.setStyle(Qt::SolidLine);

    const double x=rect.left()+rect.width()/2.0;
    const double xma=x+rect.width()/2.5;
    const double xmi=x-rect.width()/2.5;
    const double min=rect.bottom();
    const double max=rect.top();
    const double median=max+rect.height()/2.0;
    const double w=fabs(xma-xmi);
    const double p25=max+0.75*rect.height();
    const double p75=max+0.25*rect.height();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaintrect=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        if (getDrawBox()) painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
    }

    painter.setPen(pm);
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.setPen(pw);
    painter.drawLine(QLineF(x-w*0.4, max, x+w*0.4, max));
    painter.drawLine(QLineF(x-w*0.4, min, x+w*0.4, min));
    painter.drawLine(QLineF(x, max, x, p75));
    painter.drawLine(QLineF(x, min, x, p25));
}


void JKQTPGraphBoxplotStyleMixin::plotHorizontalKeyMarker(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

    painter.translate(rect.center());
    painter.rotate(90);
    painter.translate(-1.0*rect.height()/2.0, -1.0*rect.width()/2.0);
    plotVerticalKeyMarker(parent, painter, QRectF(0,0,rect.height(),rect.width()));
}
