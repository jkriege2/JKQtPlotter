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



#include "jkqtplotter/overlays/jkqtpbasicoverlays.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include <stdlib.h>
#include <QDebug>


JKQTPOverlayVerticalLine::JKQTPOverlayVerticalLine(double pos, JKQTBasePlotter *parent):
    JKQTPOverlayOneCoordOverlay(pos, parent), JKQTPGraphTextStyleMixin(parent)
{

}

JKQTPOverlayVerticalLine::JKQTPOverlayVerticalLine(double pos, const QString& text, JKQTBasePlotter *parent):
    JKQTPOverlayOneCoordOverlay(pos, parent), JKQTPGraphTextStyleMixin(parent)
{
    setText(text);
}


void JKQTPOverlayVerticalLine::draw(JKQTPEnhancedPainter &painter) {
    if (!parent) return;
    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    QPointF p1=transform(position, ymin);
    QPointF p2=transform(position, ymax);
    QPointF p3=p2-QPointF(0, (p2.y()-p1.y())*0.1);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.drawLine(p1, p2);

    if (!text.isEmpty()) {

        //JKQTMathText mt(this);
        JKQTMathText* mt=parent->getMathText();
        mt->setFontSize(getTextFontSize());
        mt->setFontColor(getTextColor());
        mt->setFontRomanOrSpecial(getTextFontName());
        mt->parse(text);
        mt->draw(painter, p3.x(), p3.y());
    }

    
}

void JKQTPOverlayVerticalLine::setColor(QColor c)
{
    setLineColor(c);
    setTextColor(c);
}

void JKQTPOverlayVerticalLine::setText(const QString &__value)
{
    this->text = __value;
}

QString JKQTPOverlayVerticalLine::getText() const
{
    return this->text;
}


JKQTPOverlayVerticalRange::JKQTPOverlayVerticalRange(double pos, double pos2, JKQTBasePlotter *parent):
    JKQTPOverlayTwoCoordOverlay(pos, pos2, parent), JKQTPGraphTextStyleMixin(parent)
{
    inverted=false;
}

JKQTPOverlayVerticalRange::JKQTPOverlayVerticalRange(double pos, double pos2, const QString& text, JKQTBasePlotter *parent):
    JKQTPOverlayTwoCoordOverlay(pos, pos2, parent), JKQTPGraphTextStyleMixin(parent)
{
    setText(text);
    inverted=false;
}

void JKQTPOverlayVerticalRange::draw(JKQTPEnhancedPainter &painter) {
    if (!parent) return;
    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    double xmin=parent->getXMin();
    double xmax=parent->getXMax();
    QPointF p1=transform(position, ymin);
    QPointF p2=transform(position, ymax);
    QPointF p3=p2-QPointF(0, (p2.y()-p1.y())*0.1);

    QPointF p21=transform(position2, ymin);
    QPointF p22=transform(position2, ymax);
    //QPointF p23=p2-QPointF(0, (p2.y()-p1.y())*0.1);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    if (getFillColor()!=QColor(Qt::transparent)) {
        if (inverted) {
            painter.fillRect(QRectF(transform(xmin, ymin), p2), getFillBrush(painter, parent));
            painter.fillRect(QRectF(p21, transform(xmax, ymax)), getFillBrush(painter, parent));
        } else {
            painter.fillRect(QRectF(p2, p21), getFillBrush(painter, parent));
        }
    }
    painter.setPen(getLinePen(painter, parent));
    painter.drawLine(p1, p2);
    painter.drawLine(p21, p22);

    if (!text.isEmpty()) {
        //JKQTMathText mt(this);
        JKQTMathText* mt=parent->getMathText();
        mt->setFontSize(getTextFontSize());
        mt->setFontColor(getTextColor());
        mt->setFontRomanOrSpecial(getTextFontName());
        mt->parse(text);
        mt->draw(painter, p3.x(), p3.y());
    }

    
}

void JKQTPOverlayVerticalRange::setColor(QColor c)
{
    setLineColor(c);
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
    setTextColor(c);
}

void JKQTPOverlayVerticalRange::setInverted(bool __value)
{
    this->inverted = __value;
}

bool JKQTPOverlayVerticalRange::getInverted() const
{
    return this->inverted;
}

void JKQTPOverlayVerticalRange::setText(const QString &__value)
{
    text=__value;
}

QString JKQTPOverlayVerticalRange::getText() const
{
    return text;
}








JKQTPOverlayLine::JKQTPOverlayLine(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent):
    JKQTPOverlayTwoPositionOverlay(x1,y1,x2,y2,parent)
{
    infinite=false;
}


void JKQTPOverlayLine::draw(JKQTPEnhancedPainter &painter) {
    if (!parent) return;
    double xmin=parent->getXMin();
    double xmax=parent->getXMax();
    QPointF p1=transform(x1, y1);
    QPointF p2=transform(x2, y2);



    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    if (infinite) {
        double alpha=(p2.y()-p1.y())/(p2.x()-p1.x());
        double offset=p1.y()-alpha*p1.x();

        double pxmin=transfromX(xmin);
        double pxmax=transfromX(xmax);

        QPointF pmin=QPointF(pxmin, pxmin*alpha+offset);
        QPointF pmax=QPointF(pxmax, pxmax*alpha+offset);
        painter.drawLine(pmin, pmax);
    } else {
        painter.drawLine(p1,p2);
    }



    
}

void JKQTPOverlayLine::setInfinite(bool __value)
{
    this->infinite = __value;
}

bool JKQTPOverlayLine::getInfinite() const
{
    return this->infinite;
}

void JKQTPOverlayLine::setColor(QColor c)
{
    setLineColor(c);
}


JKQTPOverlayRectangle::JKQTPOverlayRectangle(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent):
    JKQTPOverlayTwoPositionOverlay(x1,y1,x2,y2,parent)
{

}

void JKQTPOverlayRectangle::draw(JKQTPEnhancedPainter &painter)
{
    if (!parent) return;
    QPointF p1=transform(x1, y1);
    QPointF p2=transform(x2, y2);
    QRectF rect(qMin(p1.x(), p2.x()), qMin(p1.y(), p2.y()), fabs(p1.x()-p2.x()), fabs(p1.y()-p2.y()));

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    //painter.fillRect(rect);
    painter.drawRect(rect);
    
}

void JKQTPOverlayRectangle::setColor(QColor c)
{
    setLineColor(c);
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
}
