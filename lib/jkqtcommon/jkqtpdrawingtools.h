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
#include <QPaintDevice>
#include <QPainter>
#include <QPolygonF>
#include <QPolygon>
#include <QRectF>
#include <QRect>
#include <QLineF>
#include <QLine>
#include <QPainterPath>
#include <QColor>
#include <QVector>
#include <vector>
#include <cmath>
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtcommon/jkqtpcodestructuring.h"

class JKQTPEnhancedPainter; // forward



/*! \brief tool class with static values used by JKQTPlotter/JKQTBasePlotter
   \ingroup jkqtptools_drawing
   */
JKQTP_LIB_EXPORT struct JKQTPlotterDrawingTools {
    /** \brief smallest linewidth any line in JKQTPlotter/JKQTBasePlotter may have
     */
    static const double ABS_MIN_LINEWIDTH;
};




/** \brief symbols that can be used to plot a datapoint for a graph
 * \ingroup jkqtptools_drawing
 */
enum JKQTPGraphSymbols {
    JKQTPNoSymbol=0,             /*!< \brief plots no symbol at all (usefull together with error bars) */
    JKQTPDot,                 /*!< \brief a small dot \image html symbols/symbol_dot.png */
    JKQTPCross,               /*!< \brief a X cross \image html symbols/symbol_cross.png */
    JKQTPPlus,                /*!< \brief a + cross \image html symbols/symbol_plus.png */
    JKQTPCircle,              /*!< \brief an unfilled circle \image html symbols/symbol_circle.png */
    JKQTPFilledCircle,        /*!< \brief a filled circle \image html symbols/symbol_filled_circle.png */
    JKQTPRect,                /*!< \brief an unfilled rectangle \image html symbols/symbol_rect.png */
    JKQTPFilledRect,          /*!< \brief a filled rectangle \image html symbols/symbol_filled_rect.png */
    JKQTPTriangle,            /*!< \brief an unfilled triangle (tip at top) \image html symbols/symbol_triangle.png */
    JKQTPFilledTriangle,      /*!< \brief a filled triangle (tip at top) \image html symbols/symbol_filled_triangle.png */
    JKQTPDiamond,                /*!< \brief an unfilled diamond \image html symbols/symbol_diamond.png */
    JKQTPFilledDiamond,          /*!< \brief a filled diamond \image html symbols/symbol_filled_diamond.png */
    JKQTPstar,                /*!< \brief an unfilled diamond \image html symbols/symbol_star.png */
    JKQTPFilledStar,          /*!< \brief a filled diamond \image html symbols/symbol_filled_star.png */
    JKQTPPentagon,                /*!< \brief an unfilled pentagon \image html symbols/symbol_pentagon.png */
    JKQTPFilledPentagon,          /*!< \brief a filled pentagon \image html symbols/symbol_filled_pentagon.png */
    JKQTPAsterisc,          /*!< \brief an asterisc star with 5 arms \image html symbols/symbol_asterisc.png */
    JKQTPHourglass,              /*!< \brief an hour glass symbol \image html symbols/symbol_hourglass.png */
    JKQTPFilledHourglass,              /*!< \brief a filled hour glass symbol \image html symbols/symbol_filled_hourglass.png */
    JKQTPCurvedTriangle,  /*!< \brief a  curved triangle\image html symbols/symbol_curved_triangle.png */
    JKQTPFilledCurvedTriangle,  /*!< \brief a filled curved triangle\image html symbols/symbol_filled_curved_triangle.png */
    JKQTPHexagon,                /*!< \brief an unfilled hexagon \image html symbols/symbol_hexagon.png */
    JKQTPFilledHexagon,          /*!< \brief a filled hexagon \image html symbols/symbol_filled_hexagon.png */

    JKQTPRectCross,              /*!< \brief a square symbol with a cross inside \image html symbols/symbol_rect_cross.png */
    JKQTPRectPlus,              /*!< \brief a square symbol with a plus inside \image html symbols/symbol_rect_plus.png */
    JKQTPRectTriangle,              /*!< \brief a square symbol with a triangle inside \image html symbols/symbol_rect_triangle.png */
    JKQTPRectDownTriangle,              /*!< \brief a square symbol with a triangle (tip to the bottom) inside \image html symbols/symbol_rect_downtriangle.png */
    JKQTPRectLeftTriangle,              /*!< \brief a square symbol with a triangle (tip to the left) inside \image html symbols/symbol_rect_lefttriangle.png */
    JKQTPRectRightTriangle,              /*!< \brief a square symbol with a triangle (tip to the right) inside \image html symbols/symbol_rect_righttriangle.png */

    JKQTPCircleCross,              /*!< \brief a circle symbol with a cross inside \image html symbols/symbol_circle_cross.png */
    JKQTPCirclePlus,              /*!< \brief a circle symbol with a plus inside \image html symbols/symbol_circle_plus.png */
    JKQTPCirclePeace,          /*!< \brief a circled peace symbol \image html symbols/symbol_circle_peace.png */

    JKQTPDiamondPlus,              /*!< \brief a diamond symbol with a plus inside \image html symbols/symbol_diamond_plus.png */
    JKQTPDiamondCross,              /*!< \brief a diamond symbol with a cross inside \image html symbols/symbol_diamond_cross.png */

    JKQTPTripod,  /*!< \brief a tripod symbol \image html symbols/symbol_tripod.png */
    JKQTPDownTripod,  /*!< \brief a tripod symbol, pointing down \image html symbols/symbol_down_tripod.png */
    JKQTPLeftTripod,  /*!< \brief a tripod symbol, pointing to the left \image html symbols/symbol_left_tripod.png */
    JKQTPRightTripod,  /*!< \brief a tripod symbol, pointing to the right \image html symbols/symbol_right_tripod.png */
    JKQTPAsterisc6,          /*!< \brief an asterisc star with 6 arms \image html symbols/symbol_asterisc6.png */
    JKQTPAsterisc8,          /*!< \brief an asterisc star with 8 arms \image html symbols/symbol_asterisc8.png */
    JKQTPPeace,          /*!< \brief a peace symbol \image html symbols/symbol_peace.png */
    JKQTPTarget,              /*!< \brief a target symbol (circle with cross) \image html symbols/symbol_target.png */

    JKQTPDownTriangle,        /*!< \brief an unfilled triangle (tip at bottom) \image html symbols/symbol_down_triangle.png */
    JKQTPFilledDownTriangle,  /*!< \brief a filled triangle (tip at bottom) \image html symbols/symbol_filled_down_triangle.png */
    JKQTPLeftTriangle,        /*!< \brief an unfilled triangle (tip to the left) \image html symbols/symbol_left_triangle.png */
    JKQTPFilledLeftTriangle,  /*!< \brief a filled triangle (tip to the left) \image html symbols/symbol_filled_left_triangle.png */
    JKQTPRightTriangle,        /*!< \brief an unfilled triangle (tip to the right) \image html symbols/symbol_right_triangle.png */
    JKQTPFilledRightTriangle,  /*!< \brief a filled triangle (tip to the right) \image html symbols/symbol_filled_right_triangle.png */
    JKQTPDownCurvedTriangle,  /*!< \brief a  curved triangle, pointing down \image html symbols/symbol_down_curved_triangle.png */
    JKQTPFilledDownCurvedTriangle,  /*!< \brief a filled curved triangle, pointing down \image html symbols/symbol_filled_down_curved_triangle.png */
    JKQTPLeftCurvedTriangle,  /*!< \brief a  curved triangle, pointing to the left \image html symbols/symbol_left_curved_triangle.png */
    JKQTPFilledLeftCurvedTriangle,  /*!< \brief a filled curved triangle, pointing to the left \image html symbols/symbol_filled_left_curved_triangle.png */
    JKQTPRightCurvedTriangle,  /*!< \brief a  curved triangle, pointing to the right \image html symbols/symbol_right_curved_triangle.png */
    JKQTPFilledRightCurvedTriangle,  /*!< \brief a filled curved triangle, pointing to the right \image html symbols/symbol_filled_right_curved_triangle.png */
    JKQTPOctagon,                /*!< \brief an unfilled octagon \image html symbols/symbol_octagon.png */
    JKQTPFilledOctagon,          /*!< \brief a filled octagon \image html symbols/symbol_filled_octagon.png */
    JKQTPUpDownTriangle,              /*!< \brief a overlay of an up and a down triangle symbol \image html symbols/symbol_updowntriangle.png */
    JKQTPFilledUpDownTriangle,              /*!< \brief a filled version of the overlay of an up and a down triangle \image html symbols/symbol_filled_updowntriangle.png */
    JKQTPHorizontalHourglass,              /*!< \brief a horizontal hour glass symbol \image html symbols/symbol_horizontal_hourglass.png */
    JKQTPFilledHorizontalHourglass,              /*!< \brief a filled horizontal hour glass symbol \image html symbols/symbol_filled_horizontal_hourglass.png */

    JKQTPSantaClauseHouse,              /*!< \brief a small house symbol ("Das is das haus vom Nicolaus") \image html symbols/symbol_santaclause.png */
    JKQTPFilledSantaClauseHouse,              /*!< \brief a filled small house symbol ("Das is das haus vom Nicolaus") \image html symbols/symbol_filled_santaclause.png */

    JKQTPMale,                /*!< \brief a male symbol \image html symbols/symbol_male.png */
    JKQTPFemale,                /*!< \brief a female symbol \image html symbols/symbol_female.png */

    JKQTPSymbolCount, /*!< \brief can be used to iterate over all symbols using: <code>for (int i=0; i<static_cast<int>(JKQTPSymbolCount); i++) { JKQTPGraphSymbols s=static_cast<JKQTPGraphSymbols>(i); ... }</code> */
    JKQTPMaxSymbolID=JKQTPSymbolCount-1, /*!< \brief points to the last available symbol, can be used to iterate over all symbols: <code>for (int i=0; i<=static_cast<int>(JKQTPMaxSymbolID); i++) { JKQTPGraphSymbols s=static_cast<JKQTPGraphSymbols>(i); ... }</code> */
    JKQTPDefaultSymbol=JKQTPCross, /*!< \brief a default symbol used for plotting */
};

/** \brief converts a JKQTPGraphSymbols variable into a identifier string
 * \ingroup jkqtptools_drawing
 */
JKQTP_LIB_EXPORT QString JKQTPGraphSymbols2String(JKQTPGraphSymbols pos);
/** \brief converts a JKQTPGraphSymbols variable into a human-readable string
 * \ingroup jkqtptools_drawing
 */
JKQTP_LIB_EXPORT QString JKQTPGraphSymbols2NameString(JKQTPGraphSymbols pos);

/** \brief converts a String into a JKQTPGraphSymbols
 * \ingroup jkqtptools_drawing
 */
JKQTP_LIB_EXPORT JKQTPGraphSymbols String2JKQTPGraphSymbols(const QString& pos);




/** \brief rotate a rectangle by  given angle (rotates all points around the center of the rectangle and returns it as a QPolygonF)
 * \ingroup jkqtptools_drawing
 */
JKQTP_LIB_EXPORT QPolygonF jkqtpRotateRect(QRectF r, double angle);

/*! \brief plot the specified symbol at pixel position x,y
   \ingroup jkqtptools_drawing

    \tparam TPainter Type of \a painter: A class like JKQTPEnhancedPainter or <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>
    \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to draw to
    \param x x-coordinate of the symbol center
    \param y y-coordinate of the symbol center
    \param symbol type of the symbol to plot, see JKQTPGraphSymbols
    \param size size (width/height) of the symbol around (\a x , \a y)
    \param symbolLineWidth width of the lines used to draw the symbol
    \param color color of the symbol lines
    \param fillColor color of the symbol filling
 */
template <class TPainter>
inline void JKQTPPlotSymbol(TPainter& painter, double x, double y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);

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

    \tparam TPainter Type of \a painter: A class like JKQTPEnhancedPainter or <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>
    \param painter QPainter-like object to use for painting
    \param x x-coordinate of position the tooltip points to
    \param y y-coordinate of position the tooltip points to
    \param rect rectangle of the main tooltip area
*/
template <class TPainter>
inline void JKQTPDrawTooltip(TPainter& painter, double x, double y, const QRectF& rect);


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











template <class TPainter>
inline void JKQTPPlotSymbol(TPainter& painter, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, double symbolLineWidth, QColor color, QColor fillColor) {
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

}

template <class TPainter>
inline void JKQTPDrawTooltip(TPainter& painter, double x, double y, const QRectF& rect)
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
#endif // JKQTPDRAWINGTOOLS_H_INCLUDED
