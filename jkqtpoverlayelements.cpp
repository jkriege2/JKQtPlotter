/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate$  (revision $Rev$)

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtpoverlayelements.h"
#include "jkqtpbaseplotter.h"
#include "jkqtptools.h"
#include <stdlib.h>
#include <QDebug>


JKQTPoverlayElement::JKQTPoverlayElement(JKQtBasePlotter *parent) :
    QObject(parent)
{
    color=QColor("red");
    fillColor=color.lighter();
    lineStyle=Qt::SolidLine;
    fillStyle=Qt::SolidPattern;
    lineWidth=1.0;
    text="";
    fontName=QFont().family();
    fontSize=QFont().pointSizeF();
    visible=true;
}

void JKQTPoverlayElement::setParent(JKQtBasePlotter *parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

QPointF JKQTPoverlayElement::transform(const QPointF &x) {
    return QPointF(parent->x2p(x.x()), parent->y2p(x.y()));
}

double JKQTPoverlayElement::transfromX(double x)
{
    return parent->x2p(x);
}

double JKQTPoverlayElement::transfromY(double y)
{
    return parent->y2p(y);
}

QPointF JKQTPoverlayElement::backTransform(const QPointF &x) {
    return QPointF(parent->p2x(x.x()), parent->p2y(x.y()));
}

QVector<QPointF> JKQTPoverlayElement::transform(const QVector<QPointF> &x) {
    QVector<QPointF> res;
    for (int i=0; i<x.size(); i++) {
        res.append(transform(x[i]));
    }
    return res;
}

QPainterPath JKQTPoverlayElement::transformToLinePath(const QVector<QPointF> &x) {
    QPainterPath res;
    if (x.size()>0) {
        res.moveTo(transform(x[0]));
        for (int i=1; i<x.size(); i++) {
            res.lineTo(transform(x[i]));
        }
    }
    return res;
}

QBrush JKQTPoverlayElement::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPoverlayElement::getPen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth)));
    p.setStyle(lineStyle);
    return p;
}

QFont JKQTPoverlayElement::getFont() const {
    QFont f;
    f.setFamily(fontName);
    f.setPointSizeF(fontSize);
    return f;
}



JKQTPoverlayOneCoordOverlay::JKQTPoverlayOneCoordOverlay(double pos, JKQtBasePlotter *parent):
    JKQTPoverlayElement(parent)
 {
    this->position=pos;
}





JKQTPoverlayVerticalLine::JKQTPoverlayVerticalLine(double pos, JKQtBasePlotter *parent):
    JKQTPoverlayOneCoordOverlay(pos, parent)
{

}

JKQTPoverlayVerticalLine::JKQTPoverlayVerticalLine(double pos, QString text, JKQtBasePlotter *parent):
    JKQTPoverlayOneCoordOverlay(pos, parent)
{
    set_text(text);
}


void JKQTPoverlayVerticalLine::draw(JKQTPEnhancedPainter &painter) {
    if (!parent) return;
    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    QPointF p1=transform(position, ymin);
    QPointF p2=transform(position, ymax);
    QPointF p3=p2-QPointF(0, (p2.y()-p1.y())*0.1);
    painter.save();
    painter.setPen(getPen(painter));
    painter.drawLine(p1, p2);

    if (!text.isEmpty()) {

        //JKQTmathText mt(this);
        JKQTmathText* mt=parent->get_mathText();
        mt->set_fontSize(fontSize);
        mt->set_fontColor(color);
#ifdef USE_XITS_FONTS
        mt->useXITS();
#endif
        mt->parse(text);
        mt->draw(painter, p3.x(), p3.y());
    }

    painter.restore();
}

JKQTPoverlayTwoCoordOverlay::JKQTPoverlayTwoCoordOverlay(double pos, double pos2, JKQtBasePlotter *parent):
    JKQTPoverlayOneCoordOverlay(pos, parent)
{
    position2=pos2;
}

JKQTPoverlayVerticalRange::JKQTPoverlayVerticalRange(double pos, double pos2, JKQtBasePlotter *parent):
    JKQTPoverlayTwoCoordOverlay(pos, pos2, parent)
{
    fillColor=QColor(Qt::transparent);
    inverted=false;
}

JKQTPoverlayVerticalRange::JKQTPoverlayVerticalRange(double pos, double pos2, QString text, JKQtBasePlotter *parent):
    JKQTPoverlayTwoCoordOverlay(pos, pos2, parent)
{
    set_text(text);
    fillColor=QColor(Qt::transparent);
    inverted=false;
}

void JKQTPoverlayVerticalRange::draw(JKQTPEnhancedPainter &painter) {
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
    QPointF p23=p2-QPointF(0, (p2.y()-p1.y())*0.1);
    painter.save();
    if (fillColor!=QColor(Qt::transparent)) {
        if (inverted) {
            painter.fillRect(QRectF(transform(xmin, ymin), p2), getBrush(painter));
            painter.fillRect(QRectF(p21, transform(xmax, ymax)), getBrush(painter));
        } else {
            painter.fillRect(QRectF(p2, p21), getBrush(painter));
        }
    }
    painter.setPen(getPen(painter));
    painter.drawLine(p1, p2);
    painter.drawLine(p21, p22);

    if (!text.isEmpty()) {
        //JKQTmathText mt(this);
        JKQTmathText* mt=parent->get_mathText();
        mt->set_fontSize(fontSize);
        mt->set_fontColor(color);
#ifdef USE_XITS_FONTS
        mt->useXITS();
#endif
        mt->parse(text);
        mt->draw(painter, p3.x(), p3.y());
    }

    painter.restore();
}

JKQTPoverlayTwoPositionOverlay::JKQTPoverlayTwoPositionOverlay(double x1, double y1, double x2, double y2, JKQtBasePlotter *parent):
    JKQTPoverlayElement(parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
}










JKQTPoverlayLine::JKQTPoverlayLine(double x1, double y1, double x2, double y2, JKQtBasePlotter *parent):
    JKQTPoverlayTwoPositionOverlay(x1,y1,x2,y2,parent)
{
    infinite=false;
}


void JKQTPoverlayLine::draw(JKQTPEnhancedPainter &painter) {
    if (!parent) return;
    double xmin=parent->getXMin();
    double xmax=parent->getXMax();
    QPointF p1=transform(x1, y1);
    QPointF p2=transform(x2, y2);



    painter.save();
    painter.setPen(getPen(painter));
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



    painter.restore();
}


JKQTPoverlayRectangle::JKQTPoverlayRectangle(double x1, double y1, double x2, double y2, JKQtBasePlotter *parent):
    JKQTPoverlayTwoPositionOverlay(x1,y1,x2,y2,parent)
{

}

void JKQTPoverlayRectangle::draw(JKQTPEnhancedPainter &painter)
{
    if (!parent) return;
    QPointF p1=transform(x1, y1);
    QPointF p2=transform(x2, y2);
    QRectF rect(qMin(p1.x(), p2.x()), qMin(p1.y(), p2.y()), fabs(p1.x()-p2.x()), fabs(p1.y()-p2.y()));

    painter.save();
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    //painter.fillRect(rect);
    painter.drawRect(rect);
    painter.restore();
}
