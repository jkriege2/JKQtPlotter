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

#include "jkqtplottertools/jkqtpdrawingtools.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"
#include <QDebug>

const double JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH= 0.02;



QPolygonF jkqtpRotateRect(QRectF r, double angle) {
    QPolygonF p;
    QMatrix m;
    m.rotate(angle);
    p.append(m.map(r.bottomLeft()));
    p.append(m.map(r.bottomRight()));
    p.append(m.map(r.topRight()));
    p.append(m.map(r.topLeft()));
    return p;
}

void JKQTPPlotSymbol(QPaintDevice& paintDevice, double x, double y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor) {
    JKQTPEnhancedPainter p(&paintDevice);
    JKQTPPlotSymbol(p, x, y, symbol, size, symbolLineWidth, color, fillColor);
}

void JKQTPPlotSymbol(JKQTPEnhancedPainter& painter, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, double symbolLineWidth, QColor color, QColor fillColor) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,  symbolLineWidth));
    p.setStyle(Qt::SolidLine);
    p.setCapStyle(Qt::FlatCap);
    painter.setPen(p);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(Qt::SolidPattern);
    const double w=symbolSize;
    const double w2=w/2.0;
    const double w45=fabs(w*cos(45.0/180.0*M_PI));
    const double w3=w/3.0;

    // calculate star cordinates as static values
    static int star5_items=0;
    static double star5cordsx[10];
    static double star5cordsy[10];
    if (star5_items==0) {
        star5_items=5;
        double angle=360.0/double(star5_items)/180.0*M_PI;
        for (int i=0; i<star5_items; i++) {
            double a=(static_cast<double>(i)+0.5)*angle;
            star5cordsx[i*2]=sin(a);
            star5cordsx[i*2+1]=0.5*sin(a+angle/2.0);
            star5cordsy[i*2]=cos(a);
            star5cordsy[i*2+1]=0.5*cos(a+angle/2.0);
        }
    }
    static int star6_items=0;
    static double star6cordsx[12];
    static double star6cordsy[12];
    if (star6_items==0) {
        star6_items=6;
        double angle=360.0/double(star6_items)/180.0*M_PI;
        for (int i=0; i<star6_items; i++) {
            double a=(static_cast<double>(i)+0.5)*angle;
            star6cordsx[i*2]=sin(a);
            star6cordsx[i*2+1]=0.5*sin(a+angle/2.0);
            star6cordsy[i*2]=cos(a);
            star6cordsy[i*2+1]=0.5*cos(a+angle/2.0);
        }
    }
    static int star8_items=0;
    static double star8cordsx[16];
    static double star8cordsy[16];
    if (star8_items==0) {
        star8_items=8;
        double angle=360.0/double(star8_items)/180.0*M_PI;
        for (int i=0; i<star8_items; i++) {
            double a=(static_cast<double>(i)+0.5)*angle;
            star8cordsx[i*2]=sin(a);
            star8cordsx[i*2+1]=0.5*sin(a+angle/2.0);
            star8cordsy[i*2]=cos(a);
            star8cordsy[i*2+1]=0.5*cos(a+angle/2.0);
        }
    }

    switch(symbol) {
        case JKQTPDot:
            painter.drawPoint(QPointF(x,y));
            break;
        case JKQTPCross:{
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
        case JKQTPCircleCross:{
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawEllipse(rectangle);
                QVector<QLineF> lines;
                lines<<QLineF(x-w45/2.0,y-w45/2.0,x+w45/2.0,y+w45/2.0);
                lines<<QLineF(x-w45/2.0,y+w45/2.0,x+w45/2.0,y-w45/2.0);
                painter.drawLines(lines);
            }
            break;
        case JKQTPCirclePlus:{
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawEllipse(rectangle);
                QVector<QLineF> lines;
                lines<<QLineF(x,y-w2,x,y+w2);
                lines<<QLineF(x-w2,y,x+w2,y);
                painter.drawLines(lines);
            }
            break;
        case JKQTPCirclePeace:{
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawEllipse(rectangle);
                QVector<QLineF> lines;
                lines<<QLineF(x,y-w2,x,y+w2);
                lines<<QLineF(x,y,x+w45/2.0,y+w45/2.0);
                lines<<QLineF(x,y,x-w45/2.0,y+w45/2.0);
                painter.drawLines(lines);
            }
            break;
        case JKQTPPeace:{
                QVector<QLineF> lines;
                lines<<QLineF(x,y-w2,x,y+w2);
                lines<<QLineF(x,y,x+w45/2.0,y+w45/2.0);
                lines<<QLineF(x,y,x-w45/2.0,y+w45/2.0);
                painter.drawLines(lines);
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
        case JKQTPFemale:{
                QPainterPath path;
                QRectF rectangle3(x-w2/2.0, y-w2, w2, w2);
                path.addEllipse(rectangle3);
                path.moveTo(QPointF(x,y));
                path.lineTo(QPointF(x,y+w2));
                path.moveTo(QPointF(x-w2/3.0,y+w2/2.0));
                path.lineTo(QPointF(x+w2/3.0,y+w2/2.0));
                painter.drawPath(path);
            }
            break;
        case JKQTPMale:{
                QPainterPath path;
                QRectF rectangle3(x-w2/2.0, y-w2/2.0, w2, w2);
                path.addEllipse(rectangle3);
                path.moveTo(QPointF(x+w2/2.0*cos(45.0/180.0*M_PI),y-w2/2.0*cos(45.0/180.0*M_PI)));
                path.lineTo(QPointF(x+w2,y-w2));
                path.moveTo(QPointF(x+w2-w2/2.0,y-w2));
                path.lineTo(QPointF(x+w2,y-w2));
                path.lineTo(QPointF(x+w2,y-w2+w2/2.0));
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
        case JKQTPFilledCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.setBrush(b);
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;
        case JKQTPCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.setBrush(QColor(Qt::transparent));
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;

        case JKQTPFilledDownCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.rotate(180);
                painter.setBrush(b);
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;
        case JKQTPDownCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.rotate(180);
                painter.setBrush(QColor(Qt::transparent));
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;
        case JKQTPFilledLeftCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.rotate(-90);
                painter.setBrush(b);
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;
        case JKQTPLeftCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.rotate(-90);
                painter.setBrush(QColor(Qt::transparent));
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;
        case JKQTPFilledRightCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.rotate(90);
                painter.setBrush(b);
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;
        case JKQTPRightCurvedTriangle:{
                painter.save();
                painter.translate(x,y);
                painter.rotate(90);
                painter.setBrush(QColor(Qt::transparent));
                QPainterPath path;
                path.moveTo(0,0-w2);
                path.quadTo(0-w/10.0,0+w/4.0, 0-w2,0+w2);
                path.quadTo(0,0+w/4.0, 0+w2,0+w2);
                path.quadTo(0+w/10.0,0+w/4.0, 0,0-w2);
                painter.drawPath(path);
                painter.restore();
            }
            break;

        case JKQTPRectTriangle:{
                painter.setBrush(QColor(Qt::transparent));
                QPolygonF poly;
                poly<<QPointF(x-w2, y+w2)<<QPointF(x, y-w2)<<QPointF(x+w2, y+w2)<<QPointF(x-w2, y+w2)<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2)<<QPointF(x+w2, y+w2);
                painter.drawConvexPolygon(poly);
            }
            break;
        case JKQTPRectDownTriangle:{
                painter.setBrush(QColor(Qt::transparent));
                QPolygonF poly;
                poly<<QPointF(x-w2, y-w2)<<QPointF(x, y+w2)<<QPointF(x+w2, y-w2)<<QPointF(x-w2, y-w2)<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x+w2, y-w2);
                painter.drawConvexPolygon(poly);
            }
            break;
        case JKQTPRectLeftTriangle:{
                painter.setBrush(QColor(Qt::transparent));
                QPolygonF poly;
                poly<<QPointF(x+w2, y-w2)<<QPointF(x-w2, y)<<QPointF(x+w2, y+w2)<<QPointF(x-w2, y+w2)<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2)<<QPointF(x+w2, y+w2);
                painter.drawConvexPolygon(poly);
            }
            break;
        case JKQTPRectRightTriangle:{
                painter.setBrush(QColor(Qt::transparent));
                QPolygonF poly;
                poly<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y)<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x+w2, y-w2)<<QPointF(x-w2, y-w2)<<QPointF(x-w2, y+w2);
                painter.drawConvexPolygon(poly);
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
        case JKQTPLeftTriangle: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x+w2, y+w2)<<QPointF(x-w2, y)<<QPointF(x+w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledLeftTriangle: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x+w2, y+w2)<<QPointF(x-w2, y)<<QPointF(x+w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPRightTriangle: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y)<<QPointF(x-w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledRightTriangle: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y)<<QPointF(x-w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPTripod: {
            painter.setBrush(QColor(Qt::transparent));
            QVector<QLineF> lines;
            lines<<QLineF(x, y-w2, x, y)
                 <<QLineF(x, y, x-w45, y+w45)
                 <<QLineF(x, y, x+w45, y+w45);
            painter.drawLines(lines);
            } break;
        case JKQTPDownTripod: {
            painter.setBrush(QColor(Qt::transparent));
            QVector<QLineF> lines;
            lines<<QLineF(x, y+w2, x, y)
                 <<QLineF(x, y, x-w45, y-w45)
                 <<QLineF(x, y, x+w45, y-w45);
            painter.drawLines(lines);
            } break;
        case JKQTPLeftTripod: {
            painter.setBrush(QColor(Qt::transparent));
            QVector<QLineF> lines;
            lines<<QLineF(x-w2, y, x, y)
                 <<QLineF(x, y, x+w45, y-w45)
                 <<QLineF(x, y, x+w45, y+w45);
            painter.drawLines(lines);
            } break;
        case JKQTPRightTripod: {
            painter.setBrush(QColor(Qt::transparent));
            QVector<QLineF> lines;
            lines<<QLineF(x+w2, y, x, y)
                 <<QLineF(x, y, x-w45, y-w45)
                 <<QLineF(x, y, x-w45, y+w45);
            painter.drawLines(lines);
            } break;
        case JKQTPUpDownTriangle: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x, y+w2)<<QPointF(x+w2, y-w2)<<QPointF(x-w2, y-w2)<<QPointF(x, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x, y-w2)<<QPointF(x-w2, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledUpDownTriangle: {
                painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x, y+w2)<<QPointF(x+w2, y-w2)<<QPointF(x-w2, y-w2)<<QPointF(x, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x, y-w2)<<QPointF(x-w2, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPSantaClauseHouse: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x+w2, y-w/6.0)<<QPointF(x-w2, y-w/6.0)<<QPointF(x, y-w2)<<QPointF(x+w2, y-w/6.0)<<QPointF(x-w2, y+w2)<<QPointF(x-w2, y-w/6.0)<<QPointF(x+w2, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledSantaClauseHouse: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x+w2, y-w/6.0)<<QPointF(x-w2, y-w/6.0)<<QPointF(x, y-w2)<<QPointF(x+w2, y-w/6.0)<<QPointF(x-w2, y+w2)<<QPointF(x-w2, y-w/6.0)<<QPointF(x+w2, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPHourglass: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPHorizontalHourglass: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y+w2)<<QPointF(x+w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledHourglass: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledHorizontalHourglass: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y+w2)<<QPointF(x+w2, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPDiamond: {
            QPolygonF poly;
            painter.setBrush(QColor(Qt::transparent));
            poly<<QPointF(x, y-w2)<<QPointF(x+w2, y)<<QPointF(x, y+w2)<<QPointF(x-w2, y);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPDiamondPlus: {
            QPolygonF poly;
            painter.setBrush(QColor(Qt::transparent));
            poly<<QPointF(x, y-w2)<<QPointF(x+w2, y)<<QPointF(x, y+w2)<<QPointF(x-w2, y);
            painter.drawConvexPolygon(poly);
            painter.drawLine(poly[0], poly[2]);
            painter.drawLine(poly[1], poly[3]);
            } break;
        case JKQTPDiamondCross: {
            QPolygonF poly;
            painter.setBrush(QColor(Qt::transparent));
            poly<<QPointF(x, y-w2)<<QPointF(x+w2, y)<<QPointF(x, y+w2)<<QPointF(x-w2, y);
            painter.drawConvexPolygon(poly);
            painter.drawLine((poly[0]+poly[1])/2.0, (poly[2]+poly[3])/2.0);
            painter.drawLine((poly[1]+poly[2])/2.0, (poly[3]+poly[0])/2.0);
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
            for (int i=0; i<star5_items*2; i++) {
                poly<<QPointF(x+star5cordsx[i]*w2, y+star5cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledStar: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star5_items*2; i++) {
                poly<<QPointF(x+star5cordsx[i]*w2, y+star5cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPPentagon: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            for (int i=0; i<star5_items*2; i+=2) {
                poly<<QPointF(x+star5cordsx[i]*w2, y+star5cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPHexagon: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            for (int i=0; i<star6_items*2; i+=2) {
                poly<<QPointF(x+star6cordsx[i]*w2, y+star6cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPOctagon: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            for (int i=0; i<star8_items*2; i+=2) {
                poly<<QPointF(x+star8cordsx[i]*w2, y+star8cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPAsterisc: {
            QPainterPath path;
            for (int i=0; i<star5_items*2; i+=2) {
                path.moveTo(x+star5cordsx[i]*w2, y+star5cordsy[i]*w2);
                path.lineTo(x,y);
            }
            painter.drawPath(path);
            } break;
        case JKQTPAsterisc6: {
            QPainterPath path;
            for (int i=0; i<star6_items*2; i+=2) {
                path.moveTo(x+star6cordsx[i]*w2, y+star6cordsy[i]*w2);
                path.lineTo(x,y);
            }
            painter.drawPath(path);
            } break;
        case JKQTPAsterisc8: {
            QPainterPath path;
            for (int i=0; i<star8_items*2; i+=2) {
                path.moveTo(x+star8cordsx[i]*w2, y+star8cordsy[i]*w2);
                path.lineTo(x,y);
            }
            painter.drawPath(path);
            } break;
        case JKQTPFilledPentagon: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star5_items*2; i+=2) {
                poly<<QPointF(x+star5cordsx[i]*w2, y+star5cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledHexagon: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star6_items*2; i+=2) {
                poly<<QPointF(x+star6cordsx[i]*w2, y+star6cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPFilledOctagon: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star8_items*2; i+=2) {
                poly<<QPointF(x+star8cordsx[i]*w2, y+star8cordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        default: break;
    }
    
};



QVector<QPointF> JKQTPDrawEllipse(double x, double y, double a, double b, double angle_start, double angle_end, double alpha, int controlPoints, QPointF* x_start, QPointF* x_end) {
    QVector<QPointF> result;
    double start=angle_start*M_PI/180.0;
    double stop=angle_end*M_PI/180.0;
    double step=(stop-start)/static_cast<double>(controlPoints);
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



void JKQTPDrawTooltip(JKQTPEnhancedPainter &painter, double x, double y, const QRectF &rect)
{
    QPolygonF poly;
    if (y<rect.top()) {
        poly<<rect.topLeft()<<QPointF(rect.left()+rect.width()/3, rect.top())<<QPointF(x,y)<<QPointF(rect.right()-rect.width()/3, rect.top())<< rect.topRight()<<rect.bottomRight()<<rect.bottomLeft()<<rect.topLeft();
        painter.drawPolygon(poly);
    } else if (y>rect.bottom()) {
        poly<<rect.topLeft()<<rect.topRight()<<rect.bottomRight()<<QPointF(rect.right()-rect.width()/3, rect.bottom())<<QPointF(x,y)<<QPointF(rect.left()+rect.width()/3, rect.bottom())<< rect.bottomLeft()<<rect.topLeft();
        painter.drawPolygon(poly);
    } else if (x<rect.left()) {
        poly<<QPointF(x,y)<<rect.topLeft()<<rect.topRight()<<rect.bottomRight()<<rect.bottomLeft()<<QPointF(rect.left(), rect.top()+rect.height()/2)<<QPointF(x,y);
        painter.drawPolygon(poly);
    } else if (x>rect.left()) {
        poly<<rect.topLeft()<<rect.topRight()<<QPointF(x,y)<<QPointF(rect.right(), rect.top()+rect.height()/2)<<rect.bottomRight()<<rect.bottomLeft()<<rect.topLeft();
        painter.drawPolygon(poly);
    } else {
        painter.drawRect(rect);
    }
}

QVector<QPolygonF> JKQTPUnifyLinesToPolygons(const QVector<QLineF> &lines, double distanceThreshold, int searchMaxSurroundingElements)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPUnifyLinesToPolygons(%1, %2, %3)").arg(lines.size()).arg(distanceThreshold).arg(searchMaxSurroundingElements));
#endif
    QList<QPolygonF> res;
    res.reserve(lines.size());

    // first simply convert all lines to polygons
    for (const QLineF& l: lines) {
        QPolygonF p;
        p<<l.p1()<<l.p2();
        res<<p;
    }
    //return res.toVector();
    // clean the resulting polygon
    for (QPolygonF& p: res) {
        p=JKQTPCleanPolygon(p, distanceThreshold);
    }

    int maxIterations=100;
    int iter=0;
    bool found=true;
    //qDebug()<<"   iter "<<-1<<" -> polygons start "<<res.size();
    while (found && iter<maxIterations) {
        found=false;
        int i=0;
        while (i<res.size()-1) {
            int j=i+1;
            while (j<res.size() && j<i+searchMaxSurroundingElements) {
                if (jkqtp_distance(res[i].first(),res[j].first())<=distanceThreshold) {
                    found=true;
                    for (int k=1; k<res[j].size(); k++) {
                        res[i].prepend(res[j].at(k));
                    }
                    res.removeAt(j);
                } else if (jkqtp_distance(res[i].first(),res[j].last())<=distanceThreshold) {
                    found=true;
                    for (int k=res[j].size()-2; k>=0; k--) {
                        res[i].prepend(res[j].at(k));
                    }
                    res.removeAt(j);
                } else if (jkqtp_distance(res[i].last(),res[j].first())<=distanceThreshold) {
                    found=true;
                    for (int k=1; k<res[j].size(); k++) {
                        res[i].append(res[j].at(k));
                    }
                    res.removeAt(j);
                } else if (jkqtp_distance(res[i].last(),res[j].last())<=distanceThreshold) {
                    found=true;
                    for (int k=res[j].size()-2; k>=0; k--) {
                        res[i].append(res[j].at(k));
                    }
                    res.removeAt(j);
                } else {
                    j++;
                }
            }
            res[i]=JKQTPCleanPolygon(res[i], distanceThreshold);
            i++;
        }
        //qDebug()<<"   iter "<<iter<<" -> polygons left "<<res.size();
        iter++;
    }

    return res.toVector();
}

QPolygonF JKQTPCleanPolygon(const QPolygonF &poly, double distanceThreshold)
{
    if (poly.size()<=2) return poly;
    QPolygonF p;
    QPointF p0=poly[0];
    p<<p0;
    QVector<QPointF> inbetween;
    int i=1;
    while (i<poly.size()) {
        if ((jkqtp_distance(poly[i], p0)<=distanceThreshold)) {
            inbetween<<poly[i];
        } else {
            QPointF pmean(0,0);
            if (inbetween.size()>0) {
                for (const QPointF& pi: inbetween) {
                    pmean=QPointF(pmean.x()+pi.x()/static_cast<double>(inbetween.size()), pmean.y()+pi.y()/static_cast<double>(inbetween.size()));
                }
            } else {
                pmean=poly[i];
            }
            p<<pmean;
            p0=pmean;
            inbetween.clear();
        }
        i++;
    }

    // maybe we have something left to add
    QPointF pmean(0,0);
    if (inbetween.size()>0) {
        for (const QPointF& pi: inbetween) {
            pmean=QPointF(pmean.x()+pi.x()/static_cast<double>(inbetween.size()), pmean.y()+pi.y()/static_cast<double>(inbetween.size()));
        }
    } else {
        pmean=p0;
    }

    if (jkqtp_distance(pmean, poly.last())>distanceThreshold) {
        p<<pmean<<poly.last();
    } else {
        if (p.last()!=poly.last()) p<<poly.last();
    }
    return p;
}
