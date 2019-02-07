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



#include "jkqtplotter/jkqtpgraphsgeometric.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <QApplication>
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPGeoBaseLine::JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQTBasePlotter* parent):
    JKQTPPlotObject(parent)
{
    this->color=color;
    this->lineWidth=lineWidth;
    this->style=style;
    title="";
}

JKQTPGeoBaseLine::JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent):
    JKQTPPlotObject(parent)
{
    this->color=color;
    this->lineWidth=lineWidth;
    this->style=style;
    title="";
}

void JKQTPGeoBaseLine::setAlpha(float alpha)
{
    color.setAlphaF(alpha);
}

QPen JKQTPGeoBaseLine::getPen(JKQTPEnhancedPainter& painter) {
    QPen p;
    p.setColor(color);
    p.setStyle(style);
    p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    return p;
}

void JKQTPGeoBaseLine::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));

}

QColor JKQTPGeoBaseLine::getKeyLabelColor() {
    return color;
}


JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQTBasePlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=fillStyle;
}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=fillStyle;
}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=Qt::SolidPattern;
}

JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, lineWidth, Qt::SolidLine, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=Qt::SolidPattern;
}
JKQTPGeoBaseFilled::JKQTPGeoBaseFilled(QColor color, QColor fillColor, JKQTPlotter* parent):
    JKQTPGeoBaseLine(color, 2.0, Qt::SolidLine, parent)
{
    this->fillColor=fillColor;
    this->fillStyle=Qt::SolidPattern;
}

void JKQTPGeoBaseFilled::setAlpha(float alpha)
{
    JKQTPGeoBaseLine::setAlpha(alpha);
    fillColor.setAlphaF(alpha);
}

void JKQTPGeoBaseFilled::setAlpha(float alphaLine, float alphaFill)
{
    JKQTPGeoBaseLine::setAlpha(alphaLine);
    fillColor.setAlphaF(alphaFill);
}
QBrush JKQTPGeoBaseFilled::getBrush(JKQTPEnhancedPainter &/*painter*/) {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

void JKQTPGeoBaseFilled::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawRect(rect);

}





JKQTPGeoText::JKQTPGeoText(JKQTBasePlotter* parent, double x, double y, const QString& text, double fontSize, QColor color):
    JKQTPPlotObject(parent)
{
    this->x=x;
    this->y=y;
    this->text=text;
    this->fontSize=fontSize;
    this->fontName=QApplication::font().family()+"+XITS";
    this->color=color;
    if (parent) {
        this->fontSize=parent->getCurrentPlotterStyle().defaultFontSize;
        this->fontName=parent->getCurrentPlotterStyle().defaultFontName;
    }
}

JKQTPGeoText::JKQTPGeoText(JKQTPlotter* parent, double x, double y, const QString& text, double fontSize, QColor color):
    JKQTPGeoText(parent->getPlotter(),x,y,text,fontSize,color)
{
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
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    parent->getMathText()->setFontRomanOrSpecial(fontName);
    parent->getMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    parent->getMathText()->setFontColor(color);
    parent->getMathText()->parse(text);
    parent->getMathText()->draw(painter, transformX(x), transformY(y));

}

void JKQTPGeoText::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    double y=rect.top()+rect.height()/2.0;
    if (rect.width()>0) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));

}

QColor JKQTPGeoText::getKeyLabelColor() {
    return color;
}

QPen JKQTPGeoText::getPen(JKQTPEnhancedPainter &/*painter*/) {
    QPen p;
    p.setColor(color);
    return p;
}






JKQTPGeoLine::JKQTPGeoLine(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
}

JKQTPGeoLine::JKQTPGeoLine(JKQTPlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
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
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    QLineF l(QPointF(transformX(x1), transformY(y1)), QPointF(transformX(x2), transformY(y2)));
    if (l.length()>0) painter.drawLine(l);

}






JKQTPGeoInfiniteLine::JKQTPGeoInfiniteLine(JKQTBasePlotter* parent, double x, double y, double dx, double dy, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->x=x;
    this->y=y;
    this->dx=dx;
    this->dy=dy;
    this->two_sided=false;
}

JKQTPGeoInfiniteLine::JKQTPGeoInfiniteLine(JKQTPlotter* parent, double x, double y, double dx, double dy, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->x=x;
    this->y=y;
    this->dx=dx;
    this->dy=dy;
    this->two_sided=false;
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
        painter.setPen(getPen(painter));
        QLineF l(QPointF(transformX(x1), transformY(y1)), QPointF(transformX(x2), transformY(y2)));
        if (l.length()>0) painter.drawLine(l);

    }

}




JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTBasePlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->points=points;
}
JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTPlotter* parent, const QVector<QPointF>& points, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{
    this->points=points;
}
JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTBasePlotter *parent, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
{

}

JKQTPGeoPolyLines::JKQTPGeoPolyLines(JKQTPlotter *parent, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTPGeoBaseLine(color, lineWidth, style, parent)
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
    QPainterPath path=transformToLinePath(points);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    painter.drawPath(path);

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

QMatrix JKQTPGeoRectangle::getMatrix() {
    QMatrix trans;
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
    QMatrix m=getMatrix();
    QPolygonF rect;
    rect.append(m.map(QPointF(0-width/2.0, 0-height/2.0)));
    rect.append(m.map(QPointF(0-width/2.0, 0+height/2.0)));
    rect.append(m.map(QPointF(0+width/2.0, 0+height/2.0)));
    rect.append(m.map(QPointF(0+width/2.0, 0-height/2.0)));
    rect.translate(x,y);
    return rect;
}

void JKQTPGeoRectangle::draw(JKQTPEnhancedPainter& painter) {
    QPolygonF poly=getPolygon();
    QPolygonF rect;
    for (int i=0; i<poly.size(); i++) {
        rect.append(QPointF(transformX(poly[i].x()), transformY(poly[i].y())));
    }

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPolygon(rect);

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
    QPolygonF path=transformToPolygon(points);
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPolygon(path);

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
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPath(rect);

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


void JKQTPGeoArc::draw(JKQTPEnhancedPainter& painter) {
    QPainterPath rect;
    rect=transformToLinePath(JKQTPDrawEllipse(x,y,width/2.0, height/2.0,angleStart,angleStop,angle, controlPoints));

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(getPen(painter));
    painter.drawPath(rect);

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
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
    painter.drawPath(rect);

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
    painter.setPen(getPen(painter));
    painter.setBrush(getBrush(painter));
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
    this->symbol=symbol;
    this->symbolSize=symbolSize;
    this->color=color;
    this->fillColor=fillColor;
    this->symbolWidth=1;
}

JKQTPGeoSymbol::JKQTPGeoSymbol(JKQTPlotter *parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, QColor color, QColor fillColor):
    JKQTPPlotObject(parent)
{
    this->x=x;
    this->y=y;
    this->symbol=symbol;
    this->symbolSize=symbolSize;
    this->color=color;
    this->fillColor=fillColor;
    this->symbolWidth=1;
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
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    JKQTPPlotSymbol(painter, transformX(x), transformY(y), symbol, symbolSize, symbolWidth, color, fillColor);

}

void JKQTPGeoSymbol::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    const double minSize=qMin(rect.width(), rect.height());
    double symbolSize=parent->pt2px(painter, this->symbolSize);
    if (symbolSize>minSize*0.9) symbolSize=minSize*0.9;
    double symbolWidth=parent->pt2px(painter, this->symbolWidth*parent->getLineWidthMultiplier());
    if (symbolWidth>0.3*symbolSize) symbolWidth=0.3*symbolSize;

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    JKQTPPlotSymbol(painter, rect.left()+rect.width()/2.0, rect.top()+rect.height()/2.0, symbol, symbolSize, symbolWidth, color, fillColor);

}

QColor JKQTPGeoSymbol::getKeyLabelColor()
{
    return color;
}
