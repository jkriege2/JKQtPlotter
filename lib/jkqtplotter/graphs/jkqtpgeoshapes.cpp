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



#include "jkqtplotter/graphs/jkqtpgeoshapes.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtcommon/jkqtpgeometrytools.h"
#include <stdlib.h>
#include <QDebug>
#include <QApplication>
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;







JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTBasePlotter *parent, double x, double y, double width, double height, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoBaseFilled(parent, drawMode)
{
    this->x=x;
    this->y=y;
    this->angle=0;
    this->width=width;
    this->height=height;
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTPlotter *parent, double x, double y, double width, double height, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoRectangle(parent->getPlotter(), x, y, width, height, drawMode)
{

}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTBasePlotter *parent, QPointF bottomleft, QPointF topright, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoBaseFilled(parent, drawMode)
{
    this->angle=0;
    this->width=fabs(topright.x()-bottomleft.x());
    this->height=fabs(topright.y()-bottomleft.y());
    this->x=bottomleft.x()+this->width/2.0;
    this->y=bottomleft.y()+this->height/2.0;
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTPlotter *parent, QPointF bottomleft, QPointF topright, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoRectangle(parent->getPlotter(), bottomleft, topright, drawMode)
{

}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTBasePlotter *parent, const QPointF &center, const QSizeF &size, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoBaseFilled(parent, drawMode)
{
    this->x=center.x();
    this->y=center.y();
    this->angle=0;
    this->width=size.width();
    this->height=size.height();
}

JKQTPGeoRectangle::JKQTPGeoRectangle(JKQTPlotter *parent, const QPointF &center, const QSizeF &size, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoRectangle(parent->getPlotter(), center, size, drawMode)
{

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
    rect.append(m.map(QPointF(0-width/2.0, 0-height/2.0)));
    rect.translate(x,y);
    return rect;
}

void JKQTPGeoRectangle::draw(JKQTPEnhancedPainter& painter) {
    clearHitTestData();
    QPolygonF poly=getPolygon();
    reserveHitTestData(poly.size());
    QPolygonF rect;
    if ((getDrawMode()==DrawAsGraphicElement) || (getParent()->getXAxis()->isLinearAxis() && getParent()->getYAxis()->isLinearAxis())) {
        rect=transform(poly);
    } else {
        auto fTransform=std::bind([](const JKQTPGeometricPlotElement* plot, const QPointF& p) { return plot->transform(p); }, this, std::placeholders::_1);
        rect=JKQTPSplitPolylineIntoPoints(poly, fTransform);
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

QPointF JKQTPGeoRectangle::getCenter() const
{
    return QPointF(x, y);
}

void JKQTPGeoRectangle::setCenter(const QPointF &center)
{
    x=center.x();
    y=center.y();
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

QSizeF JKQTPGeoRectangle::getSize() const
{
    return QSizeF(width,height);
}

void JKQTPGeoRectangle::setSize(const QSizeF &size)
{
    width=size.width();
    height=size.height();
}

void JKQTPGeoRectangle::setAngle(double __value)
{
    this->angle = __value;
}

double JKQTPGeoRectangle::getAngle() const
{
    return this->angle;
}

void JKQTPGeoRectangle::setBottomLeftRectangle(double x, double y, double width, double height)
{
    this->x=x+width/2.0;
    this->y=y+height/2.0;
    this->width=width;
    this->height=height;

}


void JKQTPGeoRectangle::setBottomLeftRectangle(const QRectF &rect) {
    setBottomLeftRectangle(rect.x(), rect.y(), rect.width(), rect.height());
}

QRectF JKQTPGeoRectangle::getBottomLeftRectangle() const
{
    return QRectF(x-width/2.0, y-height/2.0, width, height);
}




JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTBasePlotter *parent, const QVector<QPointF> &points, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoBaseFilled(parent, drawMode)
{
    this->points=points;
}

JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTPlotter *parent, const QVector<QPointF> &points, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoPolygon(parent->getPlotter(), points, drawMode)
{

}

JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTBasePlotter *parent, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoBaseFilled(parent, drawMode)
{

}

JKQTPGeoPolygon::JKQTPGeoPolygon(JKQTPlotter *parent, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoPolygon(parent->getPlotter(), drawMode)
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
    if (points.size()>=2) {
        reserveHitTestData(points.size());

        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(getLinePen(painter, parent));
        painter.setBrush(getFillBrush(painter, parent));
        if ((getDrawMode()==DrawAsGraphicElement) || (getParent()->getXAxis()->isLinearAxis() && getParent()->getYAxis()->isLinearAxis())) {
            const QPolygonF path=transformToPolygon(points);
            painter.drawPolygon(path);
        } else {
            // for non-linear axes, a line might not be drawn as a line, so we need to segment the line (i.e. linear function in coordinate space)
            // and transform each node to draw the corresponding non-linear curve in pixel-space!
            auto fTransform=std::bind([](const JKQTPGeometricPlotElement* plot, const QPointF& p) { return plot->transform(p); }, this, std::placeholders::_1);
            QVector<QPointF> polyp=points;
            if (polyp.first()!=polyp.last()) polyp.push_back(polyp.first());
            QPolygonF path=JKQTPSplitPolylineIntoPoints(polyp, fTransform);
            path=JKQTPSimplyfyLineSegemnts(path);
            painter.drawPolygon(path);
        }


        for (const auto& p:points) {
            addHitTestData(p.x(), p.y());
        }

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

int JKQTPGeoPolygon::getPointCount() const
{
    return points.size();
}

const QPointF &JKQTPGeoPolygon::getPoint(int i) const
{
    return points[i];
}

QPointF &JKQTPGeoPolygon::getPoint(int i)
{
    return points[i];
}

void JKQTPGeoPolygon::setPoint(int i, const QPointF &point)
{
    points[i]=point;
}

void JKQTPGeoPolygon::removePoint(int i)
{
    points.remove(i);
}

QVector<QPointF>::iterator JKQTPGeoPolygon::pointsBegin()
{
    return points.begin();
}

QVector<QPointF>::iterator JKQTPGeoPolygon::pointsEnd()
{
    return points.end();
}

QVector<QPointF>::const_iterator JKQTPGeoPolygon::pointsCBegin() const
{
    return points.cbegin();
}

QVector<QPointF>::const_iterator JKQTPGeoPolygon::pointsCEnd() const
{
    return points.cend();
}







JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTBasePlotter *parent, double x, double y, double width, double height, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoRectangle(parent, x, y, width, height, drawMode)
{
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTPlotter *parent, double x, double y, double width, double height, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoEllipse(parent->getPlotter(), x, y, width, height, drawMode)
{

}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTBasePlotter *parent, QPointF bottomleft, QPointF topright, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoRectangle(parent, bottomleft, topright, drawMode)
{
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTPlotter *parent, QPointF bottomleft, QPointF topright, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoEllipse(parent->getPlotter(), bottomleft, topright, drawMode)
{

}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTBasePlotter *parent, const QPointF &center, const QSizeF &size, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoRectangle(parent, center, size, drawMode)
{
}

JKQTPGeoEllipse::JKQTPGeoEllipse(JKQTPlotter *parent, const QPointF &center, const QSizeF &size, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoEllipse(parent->getPlotter(), center, size, drawMode)
{

}




void JKQTPGeoEllipse::draw(JKQTPEnhancedPainter& painter) {
    drawInternal(painter, 0, 0, InternalDrawMode::Ellipse);
}


void JKQTPGeoEllipse::drawInternal(JKQTPEnhancedPainter& painter, double angleStart, double angleStop, JKQTPGeoEllipse::InternalDrawMode mode)
{

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getFillBrush(painter, parent));


    auto fTransform=std::bind([](const JKQTPGeometricPlotElement* plot, const QPointF& p) { return plot->transform(p); }, this, std::placeholders::_1);
    QPolygonF rect;
    if(mode==InternalDrawMode::Ellipse) {
        rect=JKQTPSplitEllipseIntoPoints(fTransform, x,y,width/2.0, height/2.0,0,360);
    } else if (mode==InternalDrawMode::Pie) {
        QPointF first, last;
        rect=JKQTPSplitEllipseIntoPoints(fTransform, x,y,width/2.0, height/2.0, angleStart, angleStop, 0, nullptr, nullptr, &first, &last);
        QVector<QPointF> pie;
        pie<<last<<QPointF(x,y)<<first;
        rect.append(JKQTPSimplyfyLineSegemnts(JKQTPSplitPolylineIntoPoints(pie, fTransform)));
    } else if (mode==InternalDrawMode::Chord) {
        QPointF first, last;
        rect=JKQTPSplitEllipseIntoPoints(fTransform, x,y,width/2.0, height/2.0, angleStart, angleStop, 0, nullptr, nullptr, &first, &last);
        rect.append(JKQTPSimplyfyLineSegemnts(JKQTPSplitLineIntoPoints(QLineF(last, first), fTransform)));
    }
    painter.drawPolygon(rect);


}






JKQTPGeoPie::JKQTPGeoPie(JKQTBasePlotter *parent, double x, double y, double width, double height, double angleStart, double angleStop, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoEllipse(parent, x, y, width, height, drawMode)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
}

JKQTPGeoPie::JKQTPGeoPie(JKQTPlotter *parent, double x, double y, double width, double height, double angleStart, double angleStop, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoPie(parent->getPlotter(), x, y, width, height, angleStart, angleStop, drawMode)
{

}

void JKQTPGeoPie::draw(JKQTPEnhancedPainter& painter) {
    drawInternal(painter, angleStart, angleStop, InternalDrawMode::Pie);
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
    QPolygonF rect=QPolygonF(JKQTPSplitEllipseIntoPoints(x,y,width/2.0, height/2.0, angleStart, angleStop, 180));
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
    QPolygonF rect=QPolygonF(JKQTPSplitEllipseIntoPoints(x,y,width/2.0, height/2.0, angleStart, angleStop, 180));
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





JKQTPGeoChord::JKQTPGeoChord(JKQTBasePlotter *parent, double x, double y, double width, double height, double angleStart, double angleStop, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoPie(parent, x, y, width, height, angleStart, angleStop, drawMode)
{

}

JKQTPGeoChord::JKQTPGeoChord(JKQTPlotter *parent, double x, double y, double width, double height, double angleStart, double angleStop, JKQTPGeometricPlotElement::DrawMode drawMode):
    JKQTPGeoChord(parent->getPlotter(), x, y, width, height, angleStart, angleStop, drawMode)
{

}

void JKQTPGeoChord::draw(JKQTPEnhancedPainter& painter) {
    drawInternal(painter, angleStart, angleStop, InternalDrawMode::Chord);
}


bool JKQTPGeoChord::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    const QPolygonF rect=QPolygonF(JKQTPSplitEllipseIntoPoints(x,y,width/2.0, height/2.0, angleStart, angleStop, 180));
    minx=rect.boundingRect().left();
    maxx=rect.boundingRect().right();
    if (minx>maxx) std::swap(minx, maxx);
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPGeoChord::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    const QPolygonF rect=QPolygonF(JKQTPSplitEllipseIntoPoints(x,y,width/2.0, height/2.0, angleStart, angleStop, 180));
    miny=rect.boundingRect().bottom();
    maxy=rect.boundingRect().top();
    if (miny>maxy) std::swap(miny, maxy);
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

