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



#include "jkqtplotter/jkqtpoverlaysbase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include <stdlib.h>
#include <QDebug>



JKQTPOverlayElement::JKQTPOverlayElement(JKQTBasePlotter *parent) :
    QObject(parent)
{
    visible=true;
}

void JKQTPOverlayElement::setParent(JKQTBasePlotter *parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPOverlayElement::setVisible(bool __value)
{
    this->visible = __value;
}

bool JKQTPOverlayElement::isVisible() const
{
    return this->visible;
}

QPointF JKQTPOverlayElement::transform(const QPointF &x) {
    return QPointF(parent->x2p(x.x()), parent->y2p(x.y()));
}

double JKQTPOverlayElement::transfromX(double x)
{
    return parent->x2p(x);
}

double JKQTPOverlayElement::transfromY(double y)
{
    return parent->y2p(y);
}

QPointF JKQTPOverlayElement::backTransform(const QPointF &x) {
    return QPointF(parent->p2x(x.x()), parent->p2y(x.y()));
}

QVector<QPointF> JKQTPOverlayElement::transform(const QVector<QPointF> &x) {
    QVector<QPointF> res;
    for (int i=0; i<x.size(); i++) {
        res.append(transform(x[i]));
    }
    return res;
}

QPainterPath JKQTPOverlayElement::transformToLinePath(const QVector<QPointF> &x) {
    QPainterPath res;
    if (x.size()>0) {
        res.moveTo(transform(x[0]));
        for (int i=1; i<x.size(); i++) {
            res.lineTo(transform(x[i]));
        }
    }
    return res;
}




JKQTPOverlayOneCoordOverlay::JKQTPOverlayOneCoordOverlay(double pos, JKQTBasePlotter *parent):
    JKQTPOverlayElement(parent)
 {
    this->position=pos;
}

void JKQTPOverlayOneCoordOverlay::setPosition(double __value)
{
    this->position = __value;
}

double JKQTPOverlayOneCoordOverlay::getPosition() const
{
    return this->position;
}






JKQTPOverlayTwoCoordOverlay::JKQTPOverlayTwoCoordOverlay(double pos, double pos2, JKQTBasePlotter *parent):
    JKQTPOverlayOneCoordOverlay(pos, parent)
{
    position2=pos2;
}

void JKQTPOverlayTwoCoordOverlay::setPosition2(double __value)
{
    this->position2 = __value;
}

double JKQTPOverlayTwoCoordOverlay::getPosition2() const
{
    return this->position2;
}

JKQTPOverlayTwoPositionOverlay::JKQTPOverlayTwoPositionOverlay(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent):
    JKQTPOverlayElement(parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
}

void JKQTPOverlayTwoPositionOverlay::setX1(double __value)
{
    this->x1 = __value;
}

double JKQTPOverlayTwoPositionOverlay::getX1() const
{
    return this->x1;
}

void JKQTPOverlayTwoPositionOverlay::setX2(double __value)
{
    this->x2 = __value;
}

double JKQTPOverlayTwoPositionOverlay::getX2() const
{
    return this->x2;
}

void JKQTPOverlayTwoPositionOverlay::setY1(double __value)
{
    this->y1 = __value;
}

double JKQTPOverlayTwoPositionOverlay::getY1() const
{
    return this->y1;
}

void JKQTPOverlayTwoPositionOverlay::setY2(double __value)
{
    this->y2 = __value;
}

double JKQTPOverlayTwoPositionOverlay::getY2() const
{
    return this->y2;
}
