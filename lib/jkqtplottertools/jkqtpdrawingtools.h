/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef JKQTPDRAWINGTOOLS_H_INCLUDED
#define JKQTPDRAWINGTOOLS_H_INCLUDED
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtcommon/jkqtptools.h"
#include <QPaintDevice>

class JKQTPEnhancedPainter; // forward


/*! \brief tool class with static values used by JKQTPlotter/JKQTBasePlotter
   \ingroup jkqtptools_drawing
   */
JKQTP_LIB_EXPORT struct JKQTPlotterDrawingTools {
    /** \brief smallest linewidth any line in JKQTPlotter/JKQTBasePlotter may have
     */
    static const double ABS_MIN_LINEWIDTH;
};

/*! \brief plot the specified symbol at pixel position x,y
   \ingroup jkqtptools_drawing

    \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to draw to
    \param x x-coordinate of the symbol center
    \param y y-coordinate of the symbol center
    \param symbol type of the symbol to plot, see JKQTPGraphSymbols
    \param size size (width/height) of the symbol around (\a x , \a y)
    \param symbolLineWidth width of the lines used to draw the symbol
    \param color color of the symbol lines
    \param fillColor color of the symbol filling
 */
JKQTP_LIB_EXPORT void JKQTPPlotSymbol(JKQTPEnhancedPainter& painter, double x, double y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);

/*! \brief plot the specified symbol at pixel position x,y
   \ingroup jkqtptools_drawing

    \param paintDevice the paint device to draw on
    \param x x-coordinate of the symbol center
    \param y y-coordinate of the symbol center
    \param symbol type of the symbol to plot, see JKQTPGraphSymbols
    \param size size (width/height) of the symbol around (\a x , \a y)
    \param symbolLineWidth width of the lines used to draw the symbol
    \param color color of the symbol lines
    \param fillColor color of the symbol filling
 */
JKQTP_LIB_EXPORT void JKQTPPlotSymbol(QPaintDevice& paintDevice, double x, double y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);


/*! \brief plot an arrow between positions (x1,y1) and (x2,y2)
   \ingroup jkqtptools_drawing

    \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to draw to
    \param x1 first x-coordinate of the arrow
    \param y1 first y-coordinate of the arrow
    \param x2 second x-coordinate of the arrow
    \param y2 second y-coordinate of the arrow
    \param symbol type of the symbol to plot, see JKQTPGraphSymbols
    \param size size (width/height) of the symbol around (\a x , \a y)
    \param symbolLineWidth width of the lines used to draw the symbol
    \param color color of the symbol lines
    \param fillColor color of the symbol filling
 */
//JKQTP_LIB_EXPORT void JKQTPPlotArrow(JKQTPEnhancedPainter& painter, int x, int y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);


/*! \brief draw an ellipse without setting pen or brush, or saving the painter!
    \ingroup jkqtptools_drawing

    \return a QVector<QPointF> with points that may be used for drawing
    \param x center of ellipse (x-coordinate)
    \param y center of ellipse (y-coordinate)
    \param a half axis in x-direction
    \param b half axis in y-direction
    \param angle_start starting angle of ellipse section
    \param angle_end ending angle of ellipse section
    \param alpha rotation angle of ellipse
    \param controlPoints the number of points to use for drawing
    \param[out] x_start first point of ellipse
    \param[out] x_end last point of ellipse

    \note all angles are given in degrees [0..360]
*/
JKQTP_LIB_EXPORT QVector<QPointF> JKQTPDrawEllipse(double x, double y, double a, double b, double angle_start=0, double angle_end=360, double alpha=0, int controlPoints=180, QPointF* x_start=nullptr, QPointF* x_end=nullptr);

/*! \brief draw a tooltip, using the current brush and pen of the provided painter
    \ingroup jkqtptools_drawing

    \param painter JKQTPEnhancedPainter to use for painting
    \param x x-coordinate of position the tooltip points to
    \param y y-coordinate of position the tooltip points to
    \param rect rectangle of the main tooltip area
*/
JKQTP_LIB_EXPORT void JKQTPDrawTooltip(JKQTPEnhancedPainter& painter, double x, double y, const QRectF& rect);


/** \brief cleans a polygon by uniting all consecutive points that were closer than distanceThreshold are united
 *  \ingroup jkqtptools_drawing
 *
 *  \param poly polygon to clean
 *  \param distanceThreshold if two end-points are closer together as this value, they are united to a single point
 *  \return a cleaned polygon, where all consecutive points that were closer than distanceThreshold are united
 */
JKQTP_LIB_EXPORT QPolygonF JKQTPCleanPolygon(const QPolygonF& poly, double distanceThreshold=0.3);

/** \brief takes a list of QLineF objesct \a lines and tries to combine as many of them as possible to QPolygonF objects.
 *         <b>Note: This method implements an incomplete algorithm with \a searchMaxSurroundingElements>0, as solving
 *         the complete problem is very time-consuming (cubic runtime)
 *  \ingroup jkqtptools_drawing
 *
 *  \param lines line segments to unify
 *  \param distanceThreshold if two end-points are closer together as this value, they are united to a single point
 *  \param searchMaxSurroundingElements limits the search for a connected polygon to at most this number of neighbors
 *  \return a vector of QPolygonF objects, which contain longer line-segments formed from \a lines
 */
JKQTP_LIB_EXPORT QVector<QPolygonF> JKQTPUnifyLinesToPolygons(const QVector<QLineF>& lines, double distanceThreshold=0.3, int searchMaxSurroundingElements=10);

#endif // JKQTPDRAWINGTOOLS_H_INCLUDED
