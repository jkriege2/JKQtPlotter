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



#include "jkqtplotter/graphs/jkqtpviolinplotstylingmixins.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"



JKQTPGraphViolinplotStyleMixin::JKQTPGraphViolinplotStyleMixin()
{

    m_whiskerLinePen=QPen(getLineColor(), getLineWidth());
    whiskerLineWidth=getLineWidth();
    m_whiskerCapLinePen=QPen(getLineColor(), getLineWidth());
    whiskerCapLineWidth=getLineWidth();
    m_medianLinePen=QPen(getLineColor(), getLineWidth());
    medianLineWidth=getLineWidth();

    m_meanSymbolLinePen=QPen(getLineColor(), getLineWidth());
    m_meanSymbolLineWidth=1;
    m_meanSymbolType=JKQTPGraphSymbols::JKQTPDefaultSymbol;
    m_meanSymbolSize=12;
    m_meanSymbolFillColor=m_meanSymbolLinePen.color().lighter();
    m_meanSymbolFontName=QGuiApplication::font().family();


    violinWidthAbsolute=m_meanSymbolSize*6.0;
    relativeWhiskerWidth=0.5;

    m_violinStyle=ViolinStyle::SmoothViolin;
    m_violinPositionMode=ViolinPositionMode::ViolinBoth;
}


void JKQTPGraphViolinplotStyleMixin::initViolinplotStyle(JKQTBasePlotter *parent, int &parentPlotStyle)
{
    setFillStyle(Qt::SolidPattern);
    setFillColor(parent->getCurrentPlotterStyle().plotBackgroundBrush.color());
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Boxplot);
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
        m_meanSymbolFontName=parent->getDefaultTextFontName();
    }

    setWhiskerLineColor(getLineColor());
    setWhiskerCapLineColor(getLineColor());
    setMedianLineColor(getLineColor());
    setMeanSymbolType(JKQTPPlus);

    if (m_meanSymbolSize>0) {
        violinWidthAbsolute=m_meanSymbolSize*6.0;
    }
}

void JKQTPGraphViolinplotStyleMixin::setViolinWidthAbsolute(double __value)
{
    violinWidthAbsolute=__value;
}

double JKQTPGraphViolinplotStyleMixin::getViolinWidthAbsolute() const
{
    return violinWidthAbsolute;
}

void JKQTPGraphViolinplotStyleMixin::setViolinplotColor(QColor c, JKQTBasePlotter *parent)
{
    setLineColor(c);
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.filledStyle.fillColorDerivationMode, c));
    setMeanColor(c);
    setMeanFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.filledStyle.symbolFillColorDerivationMode, c));
    setWhiskerLineColor(getLineColor());
    setWhiskerCapLineColor(getLineColor());
    setMedianLineColor(getLineColor());
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
}

void JKQTPGraphViolinplotStyleMixin::setViolinplotColor(QColor c, QColor bc, JKQTBasePlotter *parent)
{
    setViolinplotColor(c, parent);
    setFillColor(bc);
    setMeanFillColor(bc);
}


void JKQTPGraphViolinplotStyleMixin::setRelativeWhiskerWidth(double __value)
{
    relativeWhiskerWidth=__value;
}

double JKQTPGraphViolinplotStyleMixin::getRelativeWhiskerWidth() const
{
    return relativeWhiskerWidth;
}







void JKQTPGraphViolinplotStyleMixin::setMeanSymbolType(JKQTPGraphSymbols __value)
{
    m_meanSymbolType=__value;
}

JKQTPGraphSymbols JKQTPGraphViolinplotStyleMixin::getMeanSymbolType() const
{
    return m_meanSymbolType;
}

void JKQTPGraphViolinplotStyleMixin::setMeanSymbolFontName(const QString &__value)
{
    m_meanSymbolFontName=__value;
}

QString JKQTPGraphViolinplotStyleMixin::getMeanSymbolFontName() const
{
    return m_meanSymbolFontName;
}

void JKQTPGraphViolinplotStyleMixin::setMeanSize(double __value)
{
    m_meanSymbolSize=__value;
}

double JKQTPGraphViolinplotStyleMixin::getMeanSize() const
{
    return m_meanSymbolSize;
}

void JKQTPGraphViolinplotStyleMixin::setMeanColor(const QColor &__value)
{
    m_meanSymbolLinePen.setColor(__value);
}

QColor JKQTPGraphViolinplotStyleMixin::getMeanColor() const
{
    return m_meanSymbolLinePen.color();
}

void JKQTPGraphViolinplotStyleMixin::setMeanFillColor(const QColor &__value)
{
    m_meanSymbolFillColor=__value;
}

QColor JKQTPGraphViolinplotStyleMixin::getMeanFillColor() const
{
    return m_meanSymbolFillColor;
}

void JKQTPGraphViolinplotStyleMixin::setMeanLineWidth(double __value)
{
    m_meanSymbolLineWidth=__value;
}

double JKQTPGraphViolinplotStyleMixin::getMeanLineWidth() const
{
    return m_meanSymbolLineWidth;
}

QFont JKQTPGraphViolinplotStyleMixin::getMeanSymbolFont() const
{
    QFont f(m_meanSymbolFontName, m_meanSymbolSize);
    f.setStyleStrategy(QFont::PreferDefault);
    return f;
}

JKQTPGraphViolinplotStyleMixin::ViolinStyle JKQTPGraphViolinplotStyleMixin::getViolinStyle() const
{
    return m_violinStyle;
}

void JKQTPGraphViolinplotStyleMixin::setViolinStyle(JKQTPGraphViolinplotStyleMixin::ViolinStyle style)
{
    m_violinStyle=style;
}

JKQTPGraphViolinplotStyleMixin::ViolinPositionMode JKQTPGraphViolinplotStyleMixin::getViolinPositionMode() const
{
    return m_violinPositionMode;
}

void JKQTPGraphViolinplotStyleMixin::setViolinPositionMode(JKQTPGraphViolinplotStyleMixin::ViolinPositionMode positionMode)
{
    m_violinPositionMode=positionMode;
}


void JKQTPGraphViolinplotStyleMixin::setMeanLineStyle(Qt::PenStyle __value)
{
    this->m_meanSymbolLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphViolinplotStyleMixin::getMeanLineStyle() const
{
    return this->m_meanSymbolLinePen.style();
}


void JKQTPGraphViolinplotStyleMixin::setMeanLineDashOffset(qreal offset)
{
    m_meanSymbolLinePen.setDashOffset(offset);
}

qreal JKQTPGraphViolinplotStyleMixin::getMeanLineDashOffset() const
{
    return m_meanSymbolLinePen.dashOffset();
}

void JKQTPGraphViolinplotStyleMixin::setMeanLineDashPattern(const QVector<qreal> &pattern)
{
    m_meanSymbolLinePen.setDashPattern(pattern);
    m_meanSymbolLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphViolinplotStyleMixin::getMeanLineDashPattern() const
{
    return m_meanSymbolLinePen.dashPattern();
}

void JKQTPGraphViolinplotStyleMixin::setMedianLineWidth(double __value)
{
    medianLineWidth=__value;
}

double JKQTPGraphViolinplotStyleMixin::getMedianLineWidth() const
{
    return medianLineWidth;
}

void JKQTPGraphViolinplotStyleMixin::setMedianLineColor(QColor __value)
{
    m_medianLinePen.setColor(__value);
}

QColor JKQTPGraphViolinplotStyleMixin::getMedianLineColor() const
{
    return m_medianLinePen.color();
}

void JKQTPGraphViolinplotStyleMixin::setMedianLineStyle(Qt::PenStyle __value)
{
    this->m_medianLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphViolinplotStyleMixin::getMedianLineStyle() const
{
    return this->m_medianLinePen.style();
}


void JKQTPGraphViolinplotStyleMixin::setMedianLineDashOffset(qreal offset)
{
    m_medianLinePen.setDashOffset(offset);
}

qreal JKQTPGraphViolinplotStyleMixin::getMedianLineDashOffset() const
{
    return m_medianLinePen.dashOffset();
}

void JKQTPGraphViolinplotStyleMixin::setMedianLineDashPattern(const QVector<qreal> &pattern)
{
    m_medianLinePen.setDashPattern(pattern);
    m_medianLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphViolinplotStyleMixin::getMedianLineDashPattern() const
{
    return m_medianLinePen.dashPattern();
}

void JKQTPGraphViolinplotStyleMixin::setMedianLineJoinStyle(Qt::PenJoinStyle style)
{
    m_medianLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphViolinplotStyleMixin::getMedianLineJoinStyle() const
{
    return m_medianLinePen.joinStyle();
}

void JKQTPGraphViolinplotStyleMixin::setMedianLineCapStyle(Qt::PenCapStyle style)
{
    m_medianLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphViolinplotStyleMixin::getMedianLineCapStyle() const
{
    return m_medianLinePen.capStyle();
}

void JKQTPGraphViolinplotStyleMixin::setMedianLineBrush(const QBrush &style)
{
    m_medianLinePen.setBrush(style);
}

QBrush JKQTPGraphViolinplotStyleMixin::getMedianLineBrush() const
{
    return m_medianLinePen.brush();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerLineStyle(Qt::PenStyle __value)
{
    this->m_whiskerLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphViolinplotStyleMixin::getWhiskerLineStyle() const
{
    return this->m_whiskerLinePen.style();
}


void JKQTPGraphViolinplotStyleMixin::setWhiskerLineWidth(double __value)
{
    whiskerLineWidth=__value;
}

double JKQTPGraphViolinplotStyleMixin::getWhiskerLineWidth() const
{
    return whiskerLineWidth;
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerLineColor(QColor __value)
{
    m_whiskerLinePen.setColor(__value);
}

QColor JKQTPGraphViolinplotStyleMixin::getWhiskerLineColor() const
{
    return m_whiskerLinePen.color();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerLineDashOffset(qreal offset)
{
    m_whiskerLinePen.setDashOffset(offset);
}

qreal JKQTPGraphViolinplotStyleMixin::getWhiskerLineDashOffset() const
{
    return m_whiskerLinePen.dashOffset();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerLineDashPattern(const QVector<qreal> &pattern)
{
    m_whiskerLinePen.setDashPattern(pattern);
    m_whiskerLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphViolinplotStyleMixin::getWhiskerLineDashPattern() const
{
    return m_whiskerLinePen.dashPattern();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerLineJoinStyle(Qt::PenJoinStyle style)
{
    m_whiskerLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphViolinplotStyleMixin::getWhiskerLineJoinStyle() const
{
    return m_whiskerLinePen.joinStyle();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerLineCapStyle(Qt::PenCapStyle style)
{
    m_whiskerLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphViolinplotStyleMixin::getWhiskerLineCapStyle() const
{
    return m_whiskerLinePen.capStyle();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerLineBrush(const QBrush &style)
{
    m_whiskerLinePen.setBrush(style);
}

QBrush JKQTPGraphViolinplotStyleMixin::getWhiskerLineBrush() const
{
    return m_whiskerLinePen.brush();
}

QPen JKQTPGraphViolinplotStyleMixin::getWhiskerPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_whiskerLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*whiskerLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    pw.setCapStyle(Qt::FlatCap);
    return pw;
}








void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineStyle(Qt::PenStyle __value)
{
    this->m_whiskerCapLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineStyle() const
{
    return this->m_whiskerCapLinePen.style();
}


void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineWidth(double __value)
{
    whiskerCapLineWidth=__value;
}

double JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineWidth() const
{
    return whiskerCapLineWidth;
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineColor(QColor __value)
{
    m_whiskerCapLinePen.setColor(__value);
}

QColor JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineColor() const
{
    return m_whiskerCapLinePen.color();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineDashOffset(qreal offset)
{
    m_whiskerCapLinePen.setDashOffset(offset);
}

qreal JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineDashOffset() const
{
    return m_whiskerCapLinePen.dashOffset();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineDashPattern(const QVector<qreal> &pattern)
{
    m_whiskerCapLinePen.setDashPattern(pattern);
    m_whiskerCapLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineDashPattern() const
{
    return m_whiskerCapLinePen.dashPattern();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineJoinStyle(Qt::PenJoinStyle style)
{
    m_whiskerCapLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineJoinStyle() const
{
    return m_whiskerCapLinePen.joinStyle();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineCapStyle(Qt::PenCapStyle style)
{
    m_whiskerCapLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineCapStyle() const
{
    return m_whiskerCapLinePen.capStyle();
}

void JKQTPGraphViolinplotStyleMixin::setWhiskerCapLineBrush(const QBrush &style)
{
    m_whiskerCapLinePen.setBrush(style);
}

QBrush JKQTPGraphViolinplotStyleMixin::getWhiskerCapLineBrush() const
{
    return m_whiskerCapLinePen.brush();
}

QPen JKQTPGraphViolinplotStyleMixin::getWhiskerCapPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_whiskerCapLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*whiskerCapLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    return pw;
}









QPen JKQTPGraphViolinplotStyleMixin::getMedianPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_medianLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*medianLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    pw.setCapStyle(Qt::FlatCap);
    return pw;
}

QPen JKQTPGraphViolinplotStyleMixin::getMeanSymbolPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const {
    QPen p=m_meanSymbolLinePen;
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*m_meanSymbolLineWidth)));
    p.setStyle(Qt::SolidLine);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    return p;
}

QPen JKQTPGraphViolinplotStyleMixin::getMeanLinePen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const {
    QPen p=m_meanSymbolLinePen;
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*m_meanSymbolLineWidth)));
    p.setJoinStyle(Qt::MiterJoin);
    p.setCapStyle(Qt::FlatCap);
    return p;
}

QBrush JKQTPGraphViolinplotStyleMixin::getMeanSymbolBrush(JKQTPEnhancedPainter& /*painter*/, JKQTBasePlotter* /*parent*/) const {
    QBrush b;
    b.setColor(m_meanSymbolFillColor);
    return b;
}

void JKQTPGraphViolinplotStyleMixin::plotStyledMeanSymbol(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, double x, double y) const
{
    JKQTPPlotSymbol(painter, x, y,m_meanSymbolType, parent->pt2px(painter, m_meanSymbolSize), parent->pt2px(painter, m_meanSymbolLineWidth*parent->getLineWidthMultiplier()), m_meanSymbolLinePen.color(), m_meanSymbolFillColor, getMeanSymbolFont());
}



void JKQTPGraphViolinplotStyleMixin::plotVerticalViolinplot(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, double xp, double xpleft, double xpright, const QVector<double> &violin_cat, const QVector<double> &violin_freq, double minp, double medianp, double maxp, double meanp) const
{
    if (JKQTPIsOKFloat(xp) ) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(Qt::NoBrush);


        double minstop=medianp;
        double maxstop=medianp;
        if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(meanp)) minstop=meanp;
        else if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(maxp)) minstop=maxp;
        if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(meanp)) maxstop=meanp;
        else if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(minp)) maxstop=minp;
        const double wl=fabs(xpright-xp);
        const double wr=fabs(xpleft-xp);
        const double xprightWhisker=xp+wr*getRelativeWhiskerWidth();
        const double xpleftWhisker=xp-wl*getRelativeWhiskerWidth();


        // draw violin
        const int NViol=std::min(violin_cat.size(), violin_freq.size());
        double medianMin=xpleft;
        double medianMax=xpright;
        if (NViol>0) {
            double fmin=violin_freq[0], fmax=violin_freq[0];
            for (int i=1; i<NViol; i++) {
                fmin=std::min(fmin, violin_freq[i]);
                fmax=std::max(fmax, violin_freq[i]);
            }

            const double freqFactorr=1.0/fmax*fabs(xpright-xp);
            const double freqFactorl=1.0/fmax*fabs(xpleft-xp);

            if (m_violinStyle==ViolinStyle::BoxViolin && NViol>1) {
                painter.save(); auto __finalpaintboxvio=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(getLinePen(painter, parent));
                painter.setBrush(getFillBrush(painter, parent));

                for (int i=0; i<NViol; i++) {
                    if (i==0) {
                        const double hl=(violin_cat[1]-violin_cat[0]);
                        if (m_violinPositionMode==ViolinBoth) {
                            painter.drawRect(QRectF(QPointF(xp-violin_freq[i]*freqFactorl, violin_cat.first()), QPointF(xp+violin_freq[i]*freqFactorr, violin_cat.first()+hl/2.0)));
                        } else if (m_violinPositionMode==ViolinLeft) {
                            painter.drawRect(QRectF(QPointF(xp-violin_freq[i]*freqFactorl, violin_cat.first()), QPointF(xp, violin_cat.first()+hl/2.0)));
                        } else if (m_violinPositionMode==ViolinRight) {
                            painter.drawRect(QRectF(QPointF(xp, violin_cat.first()), QPointF(xp+violin_freq[i]*freqFactorr, violin_cat.first()+hl/2.0)));
                        }

                    } else if (i>0 && i<NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        if (m_violinPositionMode==ViolinBoth) {
                            painter.drawRect(QRectF(QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]-hl/2.0), QPointF(xp+violin_freq[i]*freqFactorr, violin_cat[i]+hl/2.0)));
                        } else if (m_violinPositionMode==ViolinLeft) {
                            painter.drawRect(QRectF(QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]-hl/2.0), QPointF(xp, violin_cat[i]+hl/2.0)));
                        } else if (m_violinPositionMode==ViolinRight) {
                            painter.drawRect(QRectF(QPointF(xp, violin_cat[i]-hl/2.0), QPointF(xp+violin_freq[i]*freqFactorr, violin_cat[i]+hl/2.0)));
                        }
                    } else if (i==NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        if (m_violinPositionMode==ViolinBoth) {
                            painter.drawRect(QRectF(QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]-hl/2.0), QPointF(xp+violin_freq[i]*freqFactorr, violin_cat[i])));
                        } else if (m_violinPositionMode==ViolinLeft) {
                            painter.drawRect(QRectF(QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]-hl/2.0), QPointF(xp, violin_cat[i])));
                        } else if (m_violinPositionMode==ViolinRight) {
                            painter.drawRect(QRectF(QPointF(xp, violin_cat[i]-hl/2.0), QPointF(xp+violin_freq[i]*freqFactorr, violin_cat[i])));
                        }

                    }

                }
            } else if (m_violinStyle==ViolinStyle::SmoothViolin && NViol>1) {
                QPolygonF pleft, pright;
                pleft<<QPointF(xp, violin_cat.first());
                pright<<QPointF(xp, violin_cat.last());
                for (int i=0; i<NViol; i++) {
                    pleft<<QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]);
                    pright<<QPointF(xp+violin_freq[NViol-1-i]*freqFactorr, violin_cat[NViol-1-i]);
                }
                pleft<<QPointF(xp, violin_cat.last());
                pright<<QPointF(xp, violin_cat.first());


                painter.save(); auto __finalpaintsmoothvio=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(getLinePen(painter, parent));
                painter.setBrush(getFillBrush(painter, parent));
                if (m_violinPositionMode==ViolinBoth) {
                    pleft.append(pright);
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinLeft) {
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinRight) {
                    painter.drawPolygon(pright);
                }
            } else if (m_violinStyle==ViolinStyle::StepViolin && NViol>1) {
                QPolygonF pleft, pright;
                for (int i=0; i<NViol; i++) {
                    if (i==0) {
                        const double hl=(violin_cat[1]-violin_cat[0]);
                        pleft<<QPointF(xp, violin_cat.first())
                             <<QPointF(xp-violin_freq[i]*freqFactorl, violin_cat.first())
                             <<QPointF(xp-violin_freq[i]*freqFactorl, violin_cat.first()+hl/2.0);
                    } else if (i>0 && i<NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        pleft<<QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]-hl/2.0)
                             <<QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]+hl/2.0);
                    } else if (i==NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        pleft<<QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i]-hl/2.0)
                             <<QPointF(xp-violin_freq[i]*freqFactorl, violin_cat[i])
                             <<QPointF(xp, violin_cat[i]);

                    }
                    if (i==0) {
                        const double hr=fabs(violin_cat[NViol-1]-violin_cat[NViol-2]);
                        pright<<QPointF(xp, violin_cat.last())
                              <<QPointF(xp+violin_freq.last()*freqFactorr, violin_cat.last())
                              <<QPointF(xp+violin_freq.last()*freqFactorr, violin_cat.last()+hr/2.0);
                    } else if (i>0 && i<NViol-1) {
                        const double hr=fabs(violin_cat[NViol-1-i]-violin_cat[NViol-1-i-1]);
                        pright<<QPointF(xp+violin_freq[NViol-1-i]*freqFactorr, violin_cat[NViol-1-i]-hr/2.0)
                              <<QPointF(xp+violin_freq[NViol-1-i]*freqFactorr, violin_cat[NViol-1-i]+hr/2.0);
                    } else if (i==NViol-1) {
                        const double hr=fabs(violin_cat[1]-violin_cat[0]);
                        pright<<QPointF(xp+violin_freq[0]*freqFactorr, violin_cat[0]-hr/2.0)
                              <<QPointF(xp+violin_freq[0]*freqFactorr, violin_cat[0])
                              <<QPointF(xp, violin_cat[0]);
                    }
                }

                painter.save(); auto __finalpaintstepvio=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(getLinePen(painter, parent));
                painter.setBrush(getFillBrush(painter, parent));
                if (m_violinPositionMode==ViolinBoth) {
                    pleft.append(pright);
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinLeft) {
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinRight) {
                    painter.drawPolygon(pright);
                }

            }
        }

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



        // draw median
        if (JKQTPIsOKFloat(medianp)) {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(getMedianPen(painter, parent));
            painter.drawLine(QLineF(medianMin, medianp, medianMax, medianp));
        }

        // draw mean
        if (JKQTPIsOKFloat(meanp)) {
            painter.setBrush(Qt::NoBrush);
            plotStyledMeanSymbol(parent, painter,xp,meanp);
        }
    }
}

void JKQTPGraphViolinplotStyleMixin::plotHorizontalViolinplot(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, double yp, double ypbottom, double yptop, const QVector<double> &violin_cat, const QVector<double> &violin_freq, double minp, double medianp, double maxp, double meanp) const
{
    if (JKQTPIsOKFloat(yp) ) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(Qt::NoBrush);


        double minstop=medianp;
        double maxstop=medianp;
        if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(meanp)) minstop=meanp;
        else if (!JKQTPIsOKFloat(minstop) && JKQTPIsOKFloat(maxp)) minstop=maxp;
        if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(meanp)) maxstop=meanp;
        else if (!JKQTPIsOKFloat(maxstop) && JKQTPIsOKFloat(minp)) maxstop=minp;
        const double wbot=fabs(ypbottom-yp);
        const double wtop=fabs(yptop-yp);
        const double xbottomWhisker=yp+wbot*getRelativeWhiskerWidth();
        const double xptopWhisker=yp-wtop*getRelativeWhiskerWidth();



        // draw violin
        const int NViol=std::min(violin_cat.size(), violin_freq.size());
        double medianMin=ypbottom;
        double medianMax=yptop;
        if (NViol>0) {
            double fmin=violin_freq[0], fmax=violin_freq[0];
            for (int i=1; i<NViol; i++) {
                fmin=std::min(fmin, violin_freq[i]);
                fmax=std::max(fmax, violin_freq[i]);
            }

            const double freqFactorr=1.0/fmax*fabs(yptop-yp);
            const double freqFactorl=1.0/fmax*fabs(ypbottom-yp);

            if (m_violinStyle==ViolinStyle::BoxViolin && NViol>1) {
                painter.save(); auto __finalpaintboxvio=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(getLinePen(painter, parent));
                painter.setBrush(getFillBrush(painter, parent));

                for (int i=0; i<NViol; i++) {
                    if (i==0) {
                        const double hl=(violin_cat[1]-violin_cat[0]);
                        if (m_violinPositionMode==ViolinBoth) {
                            painter.drawRect(QRectF(QPointF(violin_cat.first(), yp+violin_freq[i]*freqFactorl), QPointF(violin_cat.first()+hl/2.0, yp-violin_freq[i]*freqFactorr)));
                        } else if (m_violinPositionMode==ViolinLeft) {
                            painter.drawRect(QRectF(QPointF(violin_cat.first(), yp+violin_freq[i]*freqFactorl), QPointF(violin_cat.first()+hl/2.0, yp)));
                        } else if (m_violinPositionMode==ViolinRight) {
                            painter.drawRect(QRectF(QPointF(violin_cat.first(), yp), QPointF(violin_cat.first()+hl/2.0, yp-violin_freq[i]*freqFactorr)));
                        }

                    } else if (i>0 && i<NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        if (m_violinPositionMode==ViolinBoth) {
                            painter.drawRect(QRectF(QPointF(violin_cat[i]-hl/2.0, yp+violin_freq[i]*freqFactorl), QPointF(violin_cat[i]+hl/2.0, yp-violin_freq[i]*freqFactorr)));
                        } else if (m_violinPositionMode==ViolinLeft) {
                            painter.drawRect(QRectF(QPointF(violin_cat[i]-hl/2.0, yp+violin_freq[i]*freqFactorl), QPointF(violin_cat[i]+hl/2.0, yp)));
                        } else if (m_violinPositionMode==ViolinRight) {
                            painter.drawRect(QRectF(QPointF(violin_cat[i]-hl/2.0, yp), QPointF(violin_cat[i]+hl/2.0, yp-violin_freq[i]*freqFactorr)));
                        }
                    } else if (i==NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        if (m_violinPositionMode==ViolinBoth) {
                            painter.drawRect(QRectF(QPointF(violin_cat[i]-hl/2.0, yp+violin_freq[i]*freqFactorl), QPointF(violin_cat[i], yp-violin_freq[i]*freqFactorr)));
                        } else if (m_violinPositionMode==ViolinLeft) {
                            painter.drawRect(QRectF(QPointF(violin_cat[i]-hl/2.0, yp+violin_freq[i]*freqFactorl), QPointF(violin_cat[i], yp)));
                        } else if (m_violinPositionMode==ViolinRight) {
                            painter.drawRect(QRectF(QPointF(violin_cat[i]-hl/2.0, yp), QPointF(violin_cat[i], yp-violin_freq[i]*freqFactorr)));
                        }

                    }

                }
            } else if (m_violinStyle==ViolinStyle::SmoothViolin && NViol>1) {
                QPolygonF pleft, pright;
                pleft<<QPointF(violin_cat.first(), yp);
                pright<<QPointF(violin_cat.last(), yp);
                for (int i=0; i<NViol; i++) {
                    pleft<<QPointF(violin_cat[i], yp+violin_freq[i]*freqFactorl);
                    pright<<QPointF(violin_cat[NViol-1-i], yp-violin_freq[NViol-1-i]*freqFactorr);
                }
                pleft<<QPointF(violin_cat.last(), yp);
                pright<<QPointF(violin_cat.first(), yp);


                painter.save(); auto __finalpaintsmoothvio=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(getLinePen(painter, parent));
                painter.setBrush(getFillBrush(painter, parent));
                if (m_violinPositionMode==ViolinBoth) {
                    pleft.append(pright);
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinLeft) {
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinRight) {
                    painter.drawPolygon(pright);
                }
            } else if (m_violinStyle==ViolinStyle::StepViolin && NViol>1) {
                QPolygonF pleft, pright;
                for (int i=0; i<NViol; i++) {
                    if (i==0) {
                        const double hl=(violin_cat[1]-violin_cat[0]);
                        pleft<<QPointF(violin_cat.first(), yp)
                             <<QPointF(violin_cat.first(), yp+violin_freq[i]*freqFactorl)
                             <<QPointF(violin_cat.first()+hl/2.0, yp+violin_freq[i]*freqFactorl);
                    } else if (i>0 && i<NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        pleft<<QPointF(violin_cat[i]-hl/2.0, yp+violin_freq[i]*freqFactorl)
                             <<QPointF(violin_cat[i]+hl/2.0, yp+violin_freq[i]*freqFactorl);
                    } else if (i==NViol-1) {
                        const double hl=(violin_cat[i]-violin_cat[i-1]);
                        pleft<<QPointF(violin_cat[i]-hl/2.0, yp+violin_freq[i]*freqFactorl)
                             <<QPointF(violin_cat[i], yp+violin_freq[i]*freqFactorl)
                             <<QPointF(violin_cat[i], yp);

                    }
                    if (i==0) {
                        const double hr=-fabs(violin_cat[NViol-1]-violin_cat[NViol-2]);
                        pright<<QPointF(violin_cat.last(), yp)
                              <<QPointF(violin_cat.last(), yp-violin_freq.last()*freqFactorr)
                              <<QPointF(violin_cat.last()+hr/2.0, yp-violin_freq.last()*freqFactorr);
                    } else if (i>0 && i<NViol-1) {
                        const double hr=-fabs(violin_cat[NViol-1-i]-violin_cat[NViol-1-i-1]);
                        pright<<QPointF(violin_cat[NViol-1-i]-hr/2.0, yp-violin_freq[NViol-1-i]*freqFactorr)
                              <<QPointF(violin_cat[NViol-1-i]+hr/2.0, yp-violin_freq[NViol-1-i]*freqFactorr);
                    } else if (i==NViol-1) {
                        const double hr=-fabs(violin_cat[1]-violin_cat[0]);
                        pright<<QPointF(violin_cat[0]-hr/2.0, yp-violin_freq[0]*freqFactorr)
                              <<QPointF(violin_cat[0], yp-violin_freq[0]*freqFactorr)
                              <<QPointF(violin_cat[0], yp);
                    }
                }

                painter.save(); auto __finalpaintstepvio=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(getLinePen(painter, parent));
                painter.setBrush(getFillBrush(painter, parent));
                if (m_violinPositionMode==ViolinBoth) {
                    pleft.append(pright);
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinLeft) {
                    painter.drawPolygon(pleft);
                } else if (m_violinPositionMode==ViolinRight) {
                    painter.drawPolygon(pright);
                }

            }
        }



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

        // draw median
        if (JKQTPIsOKFloat(medianp)) {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(getMedianPen(painter, parent));
            painter.drawLine(QLineF(medianp, medianMin, medianp, medianMax));
        }

        // draw mean
        if (JKQTPIsOKFloat(meanp)) {
            painter.setBrush(Qt::NoBrush);
            plotStyledMeanSymbol(parent, painter,meanp,yp);
        }
    }
}

void JKQTPGraphViolinplotStyleMixin::plotVerticalKeyMarker(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, const QRectF &rect) const
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

    QPainterPath path;

    path.moveTo(x,max);
    path.cubicTo(x,max+rect.height()/4.0,xma,median-rect.height()/4.0,xma,median);
    path.cubicTo(xma,median+rect.height()/4.0,x,min-rect.height()/4.0,x,min);
    path.cubicTo(x,min-rect.height()/4.0,xmi,median+rect.height()/4.0,xmi,median);
    path.cubicTo(xmi,median-rect.height()/4.0,x,max+rect.height()/4.0,x,max);


    painter.setPen(p);
    {
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawPath(path);
    }

    painter.setPen(pm);
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.setPen(pw);
    painter.drawLine(QLineF(x, max, x, min));
}


void JKQTPGraphViolinplotStyleMixin::plotHorizontalKeyMarker(JKQTBasePlotter *parent, JKQTPEnhancedPainter &painter, const QRectF &rect) const
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

    painter.translate(rect.center());
    painter.rotate(90);
    painter.translate(-1.0*rect.height()/2.0, -1.0*rect.width()/2.0);
    plotVerticalKeyMarker(parent, painter, QRectF(0,0,rect.height(),rect.width()));
}
