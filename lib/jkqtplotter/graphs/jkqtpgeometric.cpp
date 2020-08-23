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



#include "jkqtplotter/graphs/jkqtpgeometric.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <QApplication>
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPGeoBaseLine::JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQTBasePlotter* parent):
    JKQTPPlotObject(parent)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
}

JKQTPGeoBaseLine::JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, style, parent->getPlotter())
{
}

JKQTPGeoBaseLine::JKQTPGeoBaseLine(JKQTBasePlotter *parent):
    JKQTPPlotObject(parent)
{

}

JKQTPGeoBaseLine::JKQTPGeoBaseLine(JKQTPlotter *parent):
    JKQTPGeoBaseLine(parent->getPlotter())
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


JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQTBasePlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    setFillColor(fillColor);
    setFillStyle(fillStyle);
}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    setFillColor(fillColor);
    setFillStyle(fillStyle);
}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    setFillColor(fillColor);
    setFillStyle(Qt::SolidPattern);
}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, Qt::SolidLine, parent)
{
    setFillColor(fillColor);
    setFillStyle(Qt::SolidPattern);
}
JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, 2.0, Qt::SolidLine, parent)
{
    setFillColor(fillColor);
    setFillStyle(Qt::SolidPattern);
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





JKQTPGeoText::JKQTPGeoText(JKQTBasePlotter* parent, double x, double y, const QString& text, double fontSize, QColor color):
    JKQTPPlotObject(parent), JKQTPGraphTextStyleMixin(parent)
{
    this->x=x;
    this->y=y;
    this->text=text;
    setTextFontSize(fontSize);
    setTextColor(color);
}

JKQTPGeoText::JKQTPGeoText(JKQTPlotter* parent, double x, double y, const QString& text, double fontSize, QColor color):
    JKQTPGeoText(parent->getPlotter(),x,y,text,fontSize,color)
{
}

JKQTPGeoText::JKQTPGeoText(JKQTBasePlotter *parent, double x, double y, const QString &text):
    JKQTPPlotObject(parent), JKQTPGraphTextStyleMixin(parent)
{
    this->x=x;
    this->y=y;
    this->text=text;
}

JKQTPGeoText::JKQTPGeoText(JKQTPlotter *parent, double x, double y, const QString &text):
    JKQTPGeoText(parent->getPlotter(),x,y,text)
{
}

void JKQTPGeoText::setText(const QString &__value)
{
    this->text = __value;
}

QString JKQTPGeoText::getText() const
{
    return this->text;
}

void JKQTPGeoText::setX(double __value)
{
    this->x = __value;
}

double JKQTPGeoText::getX() const
{
    return this->x;
}

void JKQTPGeoText::setY(double __value)
{
    this->y = __value;
}

double JKQTPGeoText::getY() const
{
    return this->y;
}

void JKQTPGeoText::setColor(QColor c)
{
    setTextColor(c);
}

bool JKQTPGeoText::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=maxx=x;
    smallestGreaterZero=0;
    if (x>10.0*DBL_MIN) smallestGreaterZero=x;
    return true;
}

bool JKQTPGeoText::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=maxy=y;
    smallestGreaterZero=0;
    if (y>10.0*DBL_MIN) smallestGreaterZero=y;
    return true;
}

void JKQTPGeoText::draw(JKQTPEnhancedPainter& painter) {
    clearHitTestData();
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    parent->getMathText()->setFontRomanOrSpecial(getTextFontName());
    parent->getMathText()->setFontSize(getTextFontSize()*parent->getFontSizeMultiplier());
    parent->getMathText()->setFontColor(getTextColor());
    parent->getMathText()->parse(text);
    parent->getMathText()->draw(painter, transformX(x), transformY(y));

}

void JKQTPGeoText::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(QPen(getTextColor()));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));

}

QColor JKQTPGeoText::getKeyLabelColor() const {
    return getTextColor();
}






JKQTPGeoLine::JKQTPGeoLine(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseDecoratedLine(color, lineWidth, JKQTPNoDecorator, JKQTPNoDecorator, style, parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
}

JKQTPGeoLine::JKQTPGeoLine(JKQTPlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoLine(parent->getPlotter(), x1,y1,x2,y2,color, lineWidth, style)
{
}

JKQTPGeoLine::JKQTPGeoLine(JKQTBasePlotter *parent, double x1, double y1, double x2, double y2):
    JKQTPGeoBaseDecoratedLine(parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
    setHeadDecoratorStyle(JKQTPNoDecorator);
    setTailDecoratorStyle(JKQTPNoDecorator);
}

JKQTPGeoLine::JKQTPGeoLine(JKQTPlotter *parent, double x1, double y1, double x2, double y2):
    JKQTPGeoLine(parent->getPlotter(), x1,y1,x2,y2)
{
}

bool JKQTPGeoLine::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=qMin(x1, x2);
    maxx=qMax(x1, x2);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=x1; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=x2; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPGeoLine::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=qMin(y1, y2);
    maxy=qMax(y1, y2);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=y1; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=y2; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

void JKQTPGeoLine::draw(JKQTPEnhancedPainter& painter) {
    clearHitTestData();
    reserveHitTestData(2);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.setBrush(getLineColor());
    QPointF xx1(transformX(x1),transformY(y1));
    QPointF xx2(transformX(x2), transformY(y2));
    const double angle1=atan2(xx2.y()-xx1.y(), xx2.x()-xx1.x());
    const double angle2=atan2(xx1.y()-xx2.y(), xx1.x()-xx2.x());
    if ( QLineF(xx1, xx2).length()>0) {
        QPointF lx1=xx1, lx2=xx2;
        JKQTPPlotLineDecorator(painter, xx1.x(), xx1.y(), angle1, getTailDecoratorStyle(), calcTailDecoratorSize(getLinePen(painter, getParent()).widthF()), &lx1);
        JKQTPPlotLineDecorator(painter, xx2.x(), xx2.y(), angle2, getHeadDecoratorStyle(), calcHeadDecoratorSize(getLinePen(painter, getParent()).widthF()), &lx2);
        // draw corrected line
        painter.drawLine(QLineF(lx1, lx2));
        addHitTestData(x1, y1);
        addHitTestData(x2, y2);
    }
}

void JKQTPGeoLine::setX1(double __value)
{
    this->x1 = __value;
}

double JKQTPGeoLine::getX1() const
{
    return this->x1;
}

void JKQTPGeoLine::setY1(double __value)
{
    this->y1 = __value;
}

double JKQTPGeoLine::getY1() const
{
    return this->y1;
}

void JKQTPGeoLine::setX2(double __value)
{
    this->x2 = __value;
}

double JKQTPGeoLine::getX2() const
{
    return this->x2;
}

void JKQTPGeoLine::setY2(double __value)
{
    this->y2 = __value;
}

double JKQTPGeoLine::getY2() const
{
    return this->y2;
}








JKQTPGeoArrow::JKQTPGeoArrow(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2, QColor color, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, double lineWidth, Qt::PenStyle style):
    JKQTPGeoLine(parent, x1,y1,x2,y2,color, lineWidth, style)
{
    setHeadDecoratorStyle(headStyle);
    setTailDecoratorStyle(tailStyle);
}

JKQTPGeoArrow::JKQTPGeoArrow(JKQTPlotter* parent, double x1, double y1, double x2, double y2, QColor color, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, double lineWidth, Qt::PenStyle style):
    JKQTPGeoArrow(parent->getPlotter(), x1,y1,x2,y2,color, headStyle, tailStyle, lineWidth, style)
{
}




JKQTPGeoInfiniteLine::JKQTPGeoInfiniteLine(JKQTBasePlotter* parent, double x, double y, double dx, double dy, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseDecoratedHeadLine(color, lineWidth, JKQTPNoDecorator, style, parent)
{
    this->x=x;
    this->y=y;
    this->dx=dx;
    this->dy=dy;
    this->two_sided=false;
}

JKQTPGeoInfiniteLine::JKQTPGeoInfiniteLine(JKQTPlotter* parent, double x, double y, double dx, double dy, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoInfiniteLine(parent->getPlotter(), x, y, dx, dy, color, lineWidth, style)
{
}

bool JKQTPGeoInfiniteLine::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=x;
    maxx=x;
    smallestGreaterZero=0;
    if (x>10.0*DBL_MIN) smallestGreaterZero=x;
    return true;
}

bool JKQTPGeoInfiniteLine::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=y;
    maxy=y;
    smallestGreaterZero=0;
    if (y>10.0*DBL_MIN) smallestGreaterZero=y;
    return true;
}

void JKQTPGeoInfiniteLine::draw(JKQTPEnhancedPainter& painter) {
    clearHitTestData();
    reserveHitTestData(2);

    double xmin=parent->getXAxis()->getMin();
    double xmax=parent->getXAxis()->getMax();
    double ymin=parent->getYAxis()->getMin();
    double ymax=parent->getYAxis()->getMax();
    QRectF bbox(QPointF(xmin, ymin), QPointF(xmax, ymax));
    bool doDraw=false;
    double x2=x, y2=y;
    double x1=x, y1=y;

    //  normalize lengh of direction
    double dl=sqrt(dx*dx+dy*dy);
    dx=dx/dl;
    dy=dy/dl;

    // first catch cases where we are parallel to one coordinate axis
    if (dy==0) {
        doDraw=((y>=ymin)&&(y<=ymax));
        x1=xmin;
        x2=xmax;
        if (!two_sided) {
            if ((dx>0)&&(x>xmin)) {
                x1=x;
            } else if ((dx<0)&&(x<xmax)) {
                x2=x;
            }
        }
    } else if (dx==0) {
        doDraw=((x>=xmin)&&(x<=xmax));
        y1=ymin;
        y2=ymax;
        if (!two_sided) {
            if ((dy>0)&&(y>ymin)) {
                y1=y;
            } else if ((dy<0)&&(y<ymax)) {
                y2=y;
            }
        }
    } else {
        // first we find intersection points of line and y=ymin, as well as y=ymax
        double tymin=(ymin-y)/dy;
        double xymin=x+tymin*dx;
        double tymax=(ymax-y)/dy;
        double xymax=x+tymax*dx;
        double t1=0, t2=0;
        if ((xymin>xmin)&&(xymin<xmax)) {
            // (xymin,ymin) is on the rectangle circumference => use it
            t1=tymin;
            if (two_sided) {
                doDraw=true;
            } else if (t1>0) {
                doDraw=true;
            } else {
                t1=0;
            }
        } else if (xymin<xmin) {
            //(xymin,ymin) is on the left, next to the rectangle, so we have to intersect with x=xmin
            t1=(xmin-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t1>0) {
                doDraw=true;
            } else {
                t1=0;
            }
        } else if (xymin>xmax) {
            //(xymin,ymin) is on the right, next to the rectangle, so we have to intersect with x=xmax
            t1=(xmax-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t1>0) {
                doDraw=true;
            } else {
                t1=0;
            }
        }

        if ((xymax>xmin)&&(xymax<xmax)) {
            // (xymax,ymin) is on the rectangle circumference => use it
            t2=tymax;
            if (two_sided) {
                doDraw=true;
            } else if (t2>0) {
                doDraw=true;
            } else {
                t2=0;
            }
        } else if (xymax<xmin) {
            //(xymax,ymax) is on the left, next to the rectangle, so we have to intersect with x=xmin
            t2=(xmin-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t2>0) {
                doDraw=true;
            } else {
                t2=0;
            }
        } else if (xymax>xmax) {
            //(xymax,ymax) is on the right, next to the rectangle, so we have to intersect with x=xmax
            t2=(xmax-x)/dx;
            if (two_sided) {
                doDraw=true;
            } else if (t2>0) {
                doDraw=true;
            } else {
                t2=0;
            }
        }

        x1=x+t1*dx;
        y1=y+t1*dy;
        x2=x+t2*dx;
        y2=y+t2*dy;
    }

    if (doDraw) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(getLinePen(painter, parent));
        QLineF l(QPointF(transformX(x1), transformY(y1)), QPointF(transformX(x2), transformY(y2)));
        if (l.length()>0) {
            painter.drawLine(l);
            addHitTestData(x, y, formatHitTestDefaultLabel(x,y)+
                                 QString(", \\ensuremath{\\mathrm{\\mathbf{d}}y/\\mathrm{\\mathbf{d}}x\\;=\\;%1/%2\\;=\\;%3\\;=\\;%4\\degree}").arg(jkqtp_floattolatexqstr(dy, 3)).arg(jkqtp_floattolatexqstr(dx, 3)).arg(jkqtp_floattolatexqstr(dy/dx, 3)).arg(jkqtp_floattolatexqstr(atan2(dy,dx), 1)));
            addHitTestData(x1, y1);
            addHitTestData(x2, y2);

            if (two_sided==false && x>=xmin && x<=xmax && y>=ymin && y<=ymax) {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(getLinePen(painter, parent));
                painter.setBrush(getLineColor());
                QPointF xx1(transformX(x),transformY(y));
                const double angle1=atan2(l.dy(), l.dx());
                JKQTPPlotLineDecorator(painter, xx1.x(), xx1.y(), angle1, getHeadDecoratorStyle(), calcHeadDecoratorSize(getLinePen(painter, getParent()).widthF()));
            }
        }

    }

}

void JKQTPGeoInfiniteLine::setX(double __value)
{
    this->x = __value;
}

double JKQTPGeoInfiniteLine::getX() const
{
    return this->x;
}

void JKQTPGeoInfiniteLine::setY(double __value)
{
    this->y = __value;
}

double JKQTPGeoInfiniteLine::getY() const
{
    return this->y;
}

void JKQTPGeoInfiniteLine::setDx(double __value)
{
    this->dx = __value;
}

double JKQTPGeoInfiniteLine::getDx() const
{
    return this->dx;
}

void JKQTPGeoInfiniteLine::setDy(double __value)
{
    this->dy = __value;
}

double JKQTPGeoInfiniteLine::getDy() const
{
    return this->dy;
}

void JKQTPGeoInfiniteLine::setTwoSided(bool __value)
{
    this->two_sided = __value;
}

bool JKQTPGeoInfiniteLine::getTwoSided() const
{
    return this->two_sided;
}




JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTBasePlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseDecoratedLine(color, lineWidth, JKQTPNoDecorator, JKQTPNoDecorator, style, parent)
{
    this->points=points;
}

JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTPlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoPolyLines(parent->getPlotter(), points, color, lineWidth, style)
{
}

JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTBasePlotter *parent, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseDecoratedLine(color, lineWidth, JKQTPNoDecorator, JKQTPNoDecorator, style, parent)
{

}

JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTPlotter *parent, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoPolyLines(parent->getPlotter(), color, lineWidth, style)
{

}

bool JKQTPGeoPolyLines::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
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
    //qDebug()<<"getXMinMax"<<minx<<maxx;
}

bool JKQTPGeoPolyLines::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
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
    //qDebug()<<"getYMinMax"<<miny<<maxy;
}

void JKQTPGeoPolyLines::draw(JKQTPEnhancedPainter& painter) {
    clearHitTestData();
    if (points.size()>=2) {
        reserveHitTestData(points.size());

        QVector<QPointF> path=transform(points);
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(getLinePen(painter, parent));
        painter.setBrush(getLineColor());

        // potentially draw line-end decorators/arrows
        const double angle1=atan2(path[1].y()-path[0].y(), path[1].x()-path[0].x());
        const double angle2=atan2(path[path.size()-2].y()-path[path.size()-1].y(), path[path.size()-2].x()-path[path.size()-1].x());
        QPointF xx1=path[0], xx2=path[path.size()-1];
        QPointF lx1=xx1, lx2=xx2;
        JKQTPPlotLineDecorator(painter, xx1.x(), xx1.y(), angle1, getTailDecoratorStyle(), calcTailDecoratorSize(getLinePen(painter, getParent()).widthF()), &lx1);
        JKQTPPlotLineDecorator(painter, xx2.x(), xx2.y(), angle2, getHeadDecoratorStyle(), calcHeadDecoratorSize(getLinePen(painter, getParent()).widthF()), &lx2);
        path[0]=lx1;
        path[path.size()-1]=lx2;

        // draw corrected line
        painter.drawPolyline(path.data(), path.size());
        for (const auto& p:points) {
            addHitTestData(p.x(), p.y());
        }

    }
}

void JKQTPGeoPolyLines::setPoints(const QVector<QPointF> &__value)
{
    this->points = __value;
}

QVector<QPointF> JKQTPGeoPolyLines::getPoints() const
{
    return this->points;
}

void JKQTPGeoPolyLines::appendPoint(const QPointF &p) {
    points.append(p);
}

void JKQTPGeoPolyLines::appendPoint(const double x, const double y) {
    points.append(QPointF(x, y));
}


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



JKQTPGeoArc::JKQTPGeoArc(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->angleStart=angleStart;
    this->angleStop=angleStop;
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->angle=0;
    this->controlPoints=180;
}

JKQTPGeoArc::JKQTPGeoArc(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoArc(parent->getPlotter(), x, y, width, height, angleStart, angleStop, color, lineWidth, style)
{
}


void JKQTPGeoArc::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getLinePen(painter, parent));
    painter.drawPath(rect);

}

void JKQTPGeoArc::setControlPoints(const unsigned int &__value)
{
    this->controlPoints = __value;
}

unsigned int JKQTPGeoArc::getControlPoints() const
{
    return this->controlPoints;
}

void JKQTPGeoArc::setAngleStart(double __value)
{
    this->angleStart = __value;
}

double JKQTPGeoArc::getAngleStart() const
{
    return this->angleStart;
}

void JKQTPGeoArc::setAngleStop(double __value)
{
    this->angleStop = __value;
}

double JKQTPGeoArc::getAngleStop() const
{
    return this->angleStop;
}

void JKQTPGeoArc::setX(double __value)
{
    this->x = __value;
}

double JKQTPGeoArc::getX() const
{
    return this->x;
}

void JKQTPGeoArc::setY(double __value)
{
    this->y = __value;
}

double JKQTPGeoArc::getY() const
{
    return this->y;
}

void JKQTPGeoArc::setWidth(double __value)
{
    this->width = __value;
}

double JKQTPGeoArc::getWidth() const
{
    return this->width;
}

void JKQTPGeoArc::setHeight(double __value)
{
    this->height = __value;
}

double JKQTPGeoArc::getHeight() const
{
    return this->height;
}

void JKQTPGeoArc::setAngle(double __value)
{
    this->angle = __value;
}

double JKQTPGeoArc::getAngle() const
{
    return this->angle;
}


bool JKQTPGeoArc::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    minx=rect.boundingRect().left();
    maxx=rect.boundingRect().right();
    if (minx>maxx) std::swap(minx, maxx);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPGeoArc::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    QPolygonF rect;
    rect=QPolygonF(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));
    miny=rect.boundingRect().bottom();
    maxy=rect.boundingRect().top();
    if (miny>maxy) std::swap(miny, maxy);
    smallestGreaterZero=0;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
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


JKQTPGeoSymbol::JKQTPGeoSymbol(JKQTBasePlotter *parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, QColor color, QColor fillColor):
    JKQTPPlotObject(parent)
{
    this->x=x;
    this->y=y;
    setSymbolType(symbol);
    setSymbolSize(symbolSize);
    setSymbolColor(color);
    setSymbolFillColor(fillColor);
    setSymbolLineWidth(1);
}

JKQTPGeoSymbol::JKQTPGeoSymbol(JKQTPlotter *parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, QColor color, QColor fillColor):
    JKQTPPlotObject(parent)
{
    this->x=x;
    this->y=y;
    setSymbolType(symbol);
    setSymbolSize(symbolSize);
    setSymbolColor(color);
    setSymbolFillColor(fillColor);
    setSymbolLineWidth(1);
}

void JKQTPGeoSymbol::setColor(QColor c)
{
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
}

void JKQTPGeoSymbol::setX(double __value)
{
    this->x = __value;
}

double JKQTPGeoSymbol::getX() const
{
    return this->x;
}

void JKQTPGeoSymbol::setY(double __value)
{
    this->y = __value;
}

double JKQTPGeoSymbol::getY() const
{
    return this->y;
}

bool JKQTPGeoSymbol::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    minx=x;
    maxx=x;
    double xvsgz;
    xvsgz=minx; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxx; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPGeoSymbol::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    miny=y;
    maxy=y;
    double xvsgz;
    xvsgz=miny; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=maxy; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

void JKQTPGeoSymbol::draw(JKQTPEnhancedPainter &painter)
{
    clearHitTestData();
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    plotStyledSymbol(parent, painter, transformX(x), transformY(y));
    addHitTestData(x,y);
}

void JKQTPGeoSymbol::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    const double minSize=qMin(rect.width(), rect.height());
    double symbolSize=parent->pt2px(painter, this->getSymbolSize());
    if (symbolSize>minSize*0.9) symbolSize=minSize*0.9;
    double symbolWidth=parent->pt2px(painter, this->getSymbolLineWidth()*parent->getLineWidthMultiplier());
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    JKQTPPlotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, getSymbolType(), symbolSize, symbolWidth, getSymbolColor(), getSymbolFillColor());

}

QColor JKQTPGeoSymbol::getKeyLabelColor() const
{
    return getSymbolColor();
}

JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, Qt::PenStyle style, JKQTBasePlotter *parent):
    JKQTPPlotObject(parent)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
    setHeadDecoratorStyle(headStyle);
}

JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, Qt::PenStyle style, JKQTPlotter *parent):
    JKQTPGeoBaseDecoratedHeadLine(color, lineWidth, headStyle, style, parent->getPlotter())
{

}

JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(JKQTBasePlotter *parent):
    JKQTPPlotObject(parent)
{

}

JKQTPGeoBaseDecoratedHeadLine::JKQTPGeoBaseDecoratedHeadLine(JKQTPlotter *parent):
    JKQTPPlotObject(parent->getPlotter())
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



JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, Qt::PenStyle style, JKQTBasePlotter *parent):
    JKQTPPlotObject(parent)
{
    setLineColor(color);
    setLineWidth(lineWidth);
    setLineStyle(style);
    setTailDecoratorStyle(tailStyle);
    setHeadDecoratorStyle(headStyle);
}

JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, Qt::PenStyle style, JKQTPlotter *parent):
    JKQTPGeoBaseDecoratedLine(color, lineWidth, headStyle, tailStyle, style, parent->getPlotter())
{

}

JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(JKQTBasePlotter *parent):
    JKQTPPlotObject(parent)
{

}

JKQTPGeoBaseDecoratedLine::JKQTPGeoBaseDecoratedLine(JKQTPlotter *parent):
    JKQTPPlotObject(parent->getPlotter())
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
