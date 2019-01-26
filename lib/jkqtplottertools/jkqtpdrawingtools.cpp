/*
Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    

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





/** \file jkqtpdrawingtools.cpp
  * \ingroup jkqtptools
  */

#include "jkqtplottertools/jkqtpdrawingtools.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"



void JKQTPPlotSymbol(QPaintDevice& paintDevice, double x, double y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor) {
    JKQTPEnhancedPainter p(&paintDevice);
    JKQTPPlotSymbol(p, x, y, symbol, size, symbolLineWidth, color, fillColor);
}

void JKQTPPlotSymbol(JKQTPEnhancedPainter& painter, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, double symbolLineWidth, QColor color, QColor fillColor) {
    painter.save();
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,  symbolLineWidth));
    p.setStyle(Qt::SolidLine);
    painter.setPen(p);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(Qt::SolidPattern);
    const double w=symbolSize;
    const double w2=w/2.0;
    const double w3=w/3.0;

    // calculate star cordinates as static values
    static int star_items=0;
    static double starcordsx[10];
    static double starcordsy[10];
    if (star_items==0) {
        star_items=5;
        double angle=360.0/double(star_items)/180.0*M_PI;
        for (int i=0; i<star_items; i++) {
            double a=(static_cast<double>(i)+0.5)*angle;
            starcordsx[i*2]=sin(a);
            starcordsx[i*2+1]=0.5*sin(a+angle/2.0);
            starcordsy[i*2]=cos(a);
            starcordsy[i*2+1]=0.5*cos(a+angle/2.0);
        }
    }

    switch(symbol) {
        case JKQTPDot:
            painter.drawPoint(QPointF(x,y));
            break;
        case JKQTPCross:{
                /*QVector<QLineF> lines;
                lines<<QLineF(x-w2,y-w2,x+w2,y+w2);
                lines<<QLineF(x-w2,y+w2,x+w2,y-w2);
                painter.drawLines(lines);*/
                QPainterPath path;
                path.moveTo(x-w2,y-w2);
                path.lineTo(x+w2,y+w2);
                path.moveTo(x-w2,y+w2);
                path.lineTo(x+w2,y-w2);
                painter.drawPath(path);
            }
            break;
        case JKQTPPlus:{
                QVector<QLineF> lines;
                lines<<QLineF(x,y-w2,x,y+w2);
                lines<<QLineF(x-w2,y,x+w2,y);
                painter.drawLines(lines);
            }
            break;
        case JKQTPCircle:{
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawEllipse(rectangle);
            }
            break;
        case JKQTPTarget:{
                QPainterPath path;
                QRectF rectangle3(x-w3, y-w3, 2.0*w3, 2.0*w3);
                path.addEllipse(rectangle3);
                path.moveTo(QPointF(x,y-w2));
                path.lineTo(QPointF(x,y+w2));
                path.moveTo(QPointF(x-w2,y));
                path.lineTo(QPointF(x+w2,y));
                painter.drawPath(path);
            }
            break;
        case JKQTPFilledCircle:{
                painter.setBrush(b);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawEllipse(rectangle);
            }
            break;
        case JKQTPRect:{
                painter.setBrush(QColor(Qt::transparent));
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPRectCross:{
                painter.setBrush(QColor(Qt::transparent));
                QPainterPath path;
                path.moveTo(x-w2,y-w2);
                path.lineTo(x+w2,y+w2);
                path.moveTo(x-w2,y+w2);
                path.lineTo(x+w2,y-w2);
                painter.drawPath(path);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPRectPlus:{
                painter.setBrush(QColor(Qt::transparent));
                QVector<QLineF> lines;
                lines<<QLineF(x,y-w2,x,y+w2);
                lines<<QLineF(x-w2,y,x+w2,y);
                painter.drawLines(lines);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPFilledRect:{
                painter.setBrush(b);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPTriangle: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledTriangle: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPDownTriangle: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2)<<QPointF(x, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledDownTriangle: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2)<<QPointF(x, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPDiamond: {
            QPolygonF poly;
            painter.setBrush(QColor(Qt::transparent));
            poly<<QPointF(x, y-w2)<<QPointF(x+w2, y)<<QPointF(x, y+w2)<<QPointF(x-w2, y);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledDiamond: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x, y-w2)<<QPointF(x+w2, y)<<QPointF(x, y+w2)<<QPointF(x-w2, y);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPstar: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            for (int i=0; i<star_items*2; i++) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledStar: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star_items*2; i++) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPPentagon: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            for (int i=0; i<star_items*2; i+=2) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPAsterisc: {
            //QVector<QLineF> lines;
            QPainterPath path;
            for (int i=0; i<star_items*2; i+=2) {
                //lines<<QLineF(x+starcordsx[i]*w2, y+starcordsy[i]*w2, x, y);
                path.moveTo(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
                path.lineTo(x,y);
            }
            //painter.drawLines(lines);
            painter.drawPath(path);
            } break;
        case JKQTPFilledPentagon: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star_items*2; i+=2) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        default: break;
    }
    painter.restore();
};



QVector<QPointF> JKQTPDrawEllipse(double x, double y, double a, double b, double angle_start, double angle_end, double alpha, int controlPoints, QPointF* x_start, QPointF* x_end) {
    QVector<QPointF> result;
    double start=angle_start*M_PI/180.0;
    double stop=angle_end*M_PI/180.0;
    double step=(stop-start)/(double)controlPoints;
    while (fabs(stop-start)/step<10) step=step/2.0;
    double sina=sin(1.0*alpha/180.0*M_PI);
    double cosa=cos(1.0*alpha/180.0*M_PI);
    QPointF xp(x+a*cos(start)*cosa-b*sin(start)*sina, y+a*cos(start)*sina+b*sin(start)*cosa);
    result.append(xp);
    if (x_start) *x_start = xp;
    double t=start+step;
    for (int i=1; i<controlPoints; i++) {
        double cost=cos(t);
        double sint=sin(t);
        xp=QPointF( x+a*cost*cosa-b*sint*sina,  y+a*cost*sina+b*sint*cosa);
        result.append(xp);
        //std::cout<<"t="<<t/M_PI*180.0<<":  sin(al)="<<sina<<" cos(al)="<<cosa<<" sin(t)="<<sint<<" cos(t)="<<cost<<" a="<<a<<" b="<<b<<": ("<<x+a*cost*cosa-b*sint*sina<<", "<<y+a*cost*sina+b*sint*cosa<<") = ("<<xp.x()<<", "<<xp.y()<<") \n";
        t=t+step;
    }
    if (x_end) *x_end=xp;
    return result;
}


