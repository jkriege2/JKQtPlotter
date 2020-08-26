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



#include "jkqtplotter/graphs/jkqtpgeoshapes.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <QApplication>
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;





JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTBasePlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    angle=0;
    this->width=width;
    this->height=height;
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTPlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    angle=0;
    this->width=width;
    this->height=height;
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTBasePlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    this->angle=angle;
    this->width=width;
    this->height=height;
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTPlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->x=x;
    this->y=y;
    this->angle=angle;
    this->width=width;
    this->height=height;
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTBasePlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->angle=0;
    this->width=fabs(topright.x()-bottomleft.x());
    this->height=fabs(topright.y()-bottomleft.y());
    this->x=bottomleft.x()+this->width/2.0;
    this->y=bottomleft.y()+this->height/2.0;
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTPlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->angle=0;
    this->width=fabs(topright.x()-bottomleft.x());
    this->height=fabs(topright.y()-bottomleft.y());
    this->x=bottomleft.x()+this->width/2.0;
    this->y=bottomleft.y()+this->height/2.0;
}

QTransform JKQTPGeoRectangle::getTransform() {
    QTransform trans;
    trans.rotate(angle);
    return trans;
}

bool JKQTPGeoRectangle::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QRectF bound=getPolygon().boundingRect();
    //std::cout<<"JKQTPGeoRectangle::getXMinMax: b.left="<<bound.left()<<" b.right="<<bound.right()<<" b.top="<<bound.top()<<" b.bottom="<<bound.bottom()<<std::endl;
    minx=bound.left();
    maxx=bound.right();
    if (minx>maxx) std::swap(minx, maxx);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPGeoRectangle::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QRectF bound=getPolygon().boundingRect();
    miny=bound.bottom();
    maxy=bound.top();
    if (miny>maxy) std::swap(miny, maxy);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

QPolygonF JKQTPGeoRectangle::getPolygon() {
    QTransform m=getTransform();
    QPolygonF rect;
    rect.append(m.map(QPointF(0-width/2.0, 0-height/2.0)));
    rect.append(m.map(QPointF(0-width/2.0, 0+height/2.0)));
    rect.append(m.map(QPointF(0+width/2.0, 0+height/2.0)));
    rect.append(m.map(QPointF(0+width/2.0, 0-height/2.0)));
    rect.translate(x,y);
    return rect;
}

void JKQTPGeoRectangle::draw(JKQTPEnhancedPainter& painter) {
    clearHitTestData();
    QPolygonF poly=getPolygon();
    reserveHitTestData(poly.size());
    QPolygonF rect;
    for (int i=0; i<poly.size(); i++) {
        rect.append(QPointF(transformX(poly[i].x()), transformY(poly[i].y())));
    }
    for (const auto& p:poly) {
        addHitTestData(p.x(), p.y());
    }

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePenForRects(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    painter.drawPolygon(rect);

}

void JKQTPGeoRectangle::setX(double __value)
{
    this->x = __value;
}

double JKQTPGeoRectangle::getX() const
{
    return this->x;
}

void JKQTPGeoRectangle::setY(double __value)
{
    this->y = __value;
}

double JKQTPGeoRectangle::getY() const
{
    return this->y;
}

void JKQTPGeoRectangle::setWidth(double __value)
{
    this->width = __value;
}

double JKQTPGeoRectangle::getWidth() const
{
    return this->width;
}

void JKQTPGeoRectangle::setHeight(double __value)
{
    this->height = __value;
}

double JKQTPGeoRectangle::getHeight() const
{
    return this->height;
}

void JKQTPGeoRectangle::setAngle(double __value)
{
    this->angle = __value;
}

double JKQTPGeoRectangle::getAngle() const
{
    return this->angle;
}


void JKQTPGeoRectangle::setBottomleftrectangle(double x, double y, double width, double height) {
    this->x=x+width/2.0;
    this->y=y+height/2.0;
    this->width=width;
    this->height=height;
}



JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTBasePlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->points=points;
}
JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTPlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
    this->points=points;
}

JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTBasePlotter* parent, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
}
JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTPlotter* parent, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoBaseFilled(color, fillColor, lineWidth, style, fillStyle, parent)
{
}

bool JKQTPGeoPolygon::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;
    if (points.size()>0) {
        minx=points[0].x();
        maxx=points[0].x();
        for (int i=1; i<points.size(); i++) {
            double x=points[i].x();
            if (x>maxx) maxx=x;
            if (x<minx) minx=x;
            double xvsgz;
            xvsgz=x; SmallestGreaterZeroCompare_xvsgz();
        }
        return true;
    }
    return false;
}

bool JKQTPGeoPolygon::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;
    if (points.size()>0) {
        miny=points[0].y();
        maxy=points[0].y();
        for (int i=1; i<points.size(); i++) {
            double y=points[i].y();
            if (y>maxy) maxy=y;
            if (y<miny) miny=y;
            double xvsgz;
            xvsgz=y; SmallestGreaterZeroCompare_xvsgz();
        }
        return true;
    }
    return false;
}

void JKQTPGeoPolygon::draw(JKQTPEnhancedPainter& painter) {
    clearHitTestData();
    reserveHitTestData(points.size());
    QPolygonF path=transformToPolygon(points);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    painter.drawPolygon(path);
    for (const auto& p:points) {
        addHitTestData(p.x(), p.y());
    }

}

void JKQTPGeoPolygon::setPoints(const QVector<QPointF> &__value)
{
    this->points = __value;
}

QVector<QPointF> JKQTPGeoPolygon::getPoints() const
{
    return this->points;
}

void JKQTPGeoPolygon::appendPoint(const QPointF &p) {
    points.append(p);
}

void JKQTPGeoPolygon::appendPoint(const double x, const double y) {
    points.append(QPointF(x, y));
}



JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTBasePlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoRectangle(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTPlotter* parent, double x, double y, double width, double height, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoRectangle(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTBasePlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoRectangle(parent, x, y, width, height, angle, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTPlotter *parent, double x, double y, double width, double height, double angle, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoRectangle(parent, x, y, width, height, angle, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTBasePlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoRectangle(parent, bottomleft, topright, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTPlotter *parent, QPointF bottomleft, QPointF topright, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoRectangle(parent, bottomleft, topright, color, lineWidth, style, fillColor, fillStyle)
{
    controlPoints=180;
}


void JKQTPGeoEllipse::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,0,360,angle, controlPoints));
    rect.closeSubpath();

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    painter.drawPath(rect);

}

void JKQTPGeoEllipse::setControlPoints(const unsigned int &__value)
{
    this->controlPoints = __value;
}

unsigned int JKQTPGeoEllipse::getControlPoints() const
{
    return this->controlPoints;
}







JKQTPGeoPie::JKQTPGeoPie(JKQTBasePlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoEllipse(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
}

JKQTPGeoPie::JKQTPGeoPie(JKQTPlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoEllipse(parent, x, y, width, height, color, lineWidth, style, fillColor, fillStyle)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
}


void JKQTPGeoPie::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.lineTo(transform(x,y));
    rect.closeSubpath();


    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePenForRects(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    painter.drawPath(rect);

}

void JKQTPGeoPie::setAngleStart(double __value)
{
    this->angleStart = __value;
}

double JKQTPGeoPie::getAngleStart() const
{
    return this->angleStart;
}

void JKQTPGeoPie::setAngleStop(double __value)
{
    this->angleStop = __value;
}

double JKQTPGeoPie::getAngleStop() const
{
    return this->angleStop;
}

bool JKQTPGeoPie::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.append(QPointF(x,y));
    minx=rect.boundingRect().left();
    maxx=rect.boundingRect().right();
    if (minx>maxx) std::swap(minx, maxx);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPGeoPie::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.append(QPointF(x,y));
    miny=rect.boundingRect().bottom();
    maxy=rect.boundingRect().top();
    if (miny>maxy) std::swap(miny, maxy);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}




JKQTPGeoChord::JKQTPGeoChord(JKQTBasePlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoPie(parent, x, y, width, height, angleStart, angleStop, color, lineWidth, style, fillColor, fillStyle)
{

}

JKQTPGeoChord::JKQTPGeoChord(JKQTPlotter* parent, double x, double y, double width, double height,double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle):
    JKQTPGeoPie(parent, x, y, width, height, angleStart, angleStop, color, lineWidth, style, fillColor, fillStyle)
{

}


void JKQTPGeoChord::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    rect.closeSubpath();


    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));
    painter.drawPath(rect);

}

bool JKQTPGeoChord::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    minx=rect.boundingRect().left();
    maxx=rect.boundingRect().right();
    if (minx>maxx) std::swap(minx, maxx);
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPGeoChord::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    miny=rect.boundingRect().bottom();
    maxy=rect.boundingRect().top();
    if (miny>maxy) std::swap(miny, maxy);
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

