/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * \defgroup jkqtptools Tool Functions and Definitions
 * \ingroup jkqtplotter
 */

/** \file jkqtptools.h
  * \ingroup jkqtptools
  */


#ifndef JKQTPENHANCED_PAINTERS_H
#define JKQTPENHANCED_PAINTERS_H
#include "jkqtplottertools/jkqtp_imexport.h"
#include <QPainter>



/*! \brief this class extends the QPainter
    \ingroup jkqtptools


*/
class LIB_EXPORT JKQTPEnhancedPainter : public QPainter {
    public:
        JKQTPEnhancedPainter(QPaintDevice* device);
        JKQTPEnhancedPainter();
        virtual ~JKQTPEnhancedPainter();


        //void drawPath(const QPainterPath &path);
        //void drawPoints(const QPoint *points, int pointCount);
        //void drawPoints(const QPointF *points, int pointCount);
//        void drawLines(const QLineF *lines, int lineCount);
//        void drawLines(const QPointF *pointPairs, int lineCount);
//        void drawLines(const QLine *lines, int lineCount);
//        void drawLines(const QPoint *pointPairs, int lineCount);


//        inline void drawPoint(const QPointF &p) {
//            drawPoints(&p, 1);
//        }

//        inline void drawPoint(const QPoint &p) {
//             drawPoints(&p, 1);
//        }

//        inline void drawPoint(int x, int y) {
//            drawPoint(QPointF(x,y));
//        }

//        inline void drawPoint(double x, double y) {
//            drawPoint(QPointF(x,y));
//        }

//        inline void drawPoints(const QPolygonF &points){
//             drawPoints(points.constData(), points.size());
//        }

//        inline void drawPoints(const QPolygon &points){
//            drawPoints(points.constData(), points.size());
//        }

//        inline void drawLine(const QLineF &line) {
//            drawLines(&l, 1);
//        }

//        inline void drawLine(const QLine &line) {
//            drawLines(&l, 1);
//        }

//        inline void drawLine(int x1, int y1, int x2, int y2) {
//            drawLines(QLineF(x1,y1,x2,y2), 1);
//        }

//        inline void drawLine(double x1, double y1, double x2, double y2) {
//            drawLines(QLineF(x1,y1,x2,y2), 1);
//        }

//        inline void drawLine(const QPoint &p1, const QPoint &p2) {
//            drawLine(QLineF(QLine(p1, p2)));
//        }

//        inline void drawLine(const QPointF &p1, const QPointF &p2) {
//            drawLine(QLineF(p1, p2));
//        }

//        inline void drawLines(const QVector<QLineF> &lines) {
//            drawLines(lines.constData(), lines.size());
//        }

//        inline void drawLines(const QVector<QPointF> &pointPairs) {
//            drawLines(pointPairs.constData(), pointPairs.size() / 2);
//        }

//        inline void drawLines(const QVector<QLine> &lines) {
//            drawLines(lines.constData(), lines.size());
//        }

//        inline void drawLines(const QVector<QPoint> &pointPairs) {
//            drawLines(pointPairs.constData(), pointPairs.size() / 2);
//        }

//        inline void drawRect(const QRectF &rect) {
//            drawRects(&rect, 1);
//        }

//        inline void drawRect(int x1, int y1, int w, int h) {
//            QRectF r(x, y, w, h);
//            drawRects(&r, 1);
//        }

//        inline void drawRect(double x1, double y1, double w, double h) {
//            QRectF r(x, y, w, h);
//            drawRects(&r, 1);
//        }

//        inline void drawRect(const QRect &rect) {
//           drawRects(&r, 1);
//        }

    protected:
        virtual void initQEnhacedPainter();
    private:

};





#endif // JKQTPENHANCED_PAINTERS_H
