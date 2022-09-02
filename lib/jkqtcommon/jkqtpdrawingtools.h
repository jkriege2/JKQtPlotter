/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QPaintDevice>
#include <QPainter>
#include <QPolygonF>
#include <QPolygon>
#include <QRectF>
#include <QRect>
#include <QLineF>
#include <QLine>
#include <array>
#include <QPainterPath>
#include <QColor>
#include <QVector>
#include <vector>
#include <forward_list>
#include <cmath>
#include <utility>
#include <QDebug>
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtcommon/jkqtpcodestructuring.h"

class JKQTPEnhancedPainter; // forward



/*! \brief tool class with static values used by JKQTPlotter/JKQTBasePlotter
   \ingroup jkqtptools_drawing
   */
struct JKQTPlotterDrawingTools {
    /** \brief smallest linewidth any line in JKQTPlotter/JKQTBasePlotter may have
     */
    static JKQTCOMMON_LIB_EXPORT const double ABS_MIN_LINEWIDTH;
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
JKQTCOMMON_LIB_EXPORT QString JKQTPGraphSymbols2String(JKQTPGraphSymbols pos);
/** \brief converts a JKQTPGraphSymbols variable into a human-readable string
 * \ingroup jkqtptools_drawing
 */
JKQTCOMMON_LIB_EXPORT QString JKQTPGraphSymbols2NameString(JKQTPGraphSymbols pos);

/** \brief converts a String into a JKQTPGraphSymbols
 * \ingroup jkqtptools_drawing
 */
JKQTCOMMON_LIB_EXPORT JKQTPGraphSymbols String2JKQTPGraphSymbols(const QString& pos);





/** \brief symbols that can be used to plot a datapoint for a graph
 * \ingroup jkqtptools_drawing
 *
 * \image html geo_arrow_tips.png
 *
 * Note that all arrows end at the designated line-end (here indicated by dashed grey lines), even circles and rectangle:
 *
 * \image html geo_arrow_tipsatlineend.png
 *
 * \see \ref JKQTPlotterGeometricArrows and \ref JKQTPlotterGeometricGraphs
 */
enum JKQTPLineDecoratorStyle {
    JKQTPNoDecorator=0,                /*!< \brief no decorator, i.e. a simple line-end */
    JKQTPArrow,                        /*!< \brief a simple arrow tip, unfilled \image html JKQTPArrow.png*/
    JKQTPFilledArrow,                  /*!< \brief a nice filled arrow tip \image html JKQTPFilledArrow.png */
    JKQTPTriangleDecorator,                /*!< \brief a triangular arrow tip \image html JKQTPTriangleDecorator.png */
    JKQTPFilledTriangleDecorator,          /*!< \brief a triangular filled arrow tip \image html JKQTPFilledTriangleDecorator.png */
    JKQTPTriangleDecoratorAndBar,         /*!< \brief a triangular arrow tip, with vertical bar \image html JKQTPTriangleDecoratorAndBar.png */
    JKQTPFilledTriangleDecoratorAndBar,   /*!< \brief a triangular filled arrow tip, with vertical bar \image html JKQTPFilledTriangleDecoratorAndBar.png */
    JKQTPDoubleArrow,                  /*!< \brief a nice double-arrow tip  \image html JKQTPDoubleArrow.png*/
    JKQTPFilledDoubleArrow,            /*!< \brief a nice filled double-arrow tip \image html JKQTPFilledDoubleArrow.png */
    JKQTPCircleDecorator,              /*!< \brief an open circle tip \image html JKQTPCircleDecorator.png */
    JKQTPFilledCircleDecorator,        /*!< \brief a filled circle tip \image html JKQTPFilledCircleDecorator.png */
    JKQTPRectangleDecorator,           /*!< \brief an open rectangle tip \image html JKQTPRectangleDecorator.png */
    JKQTPFilledRectangleDecorator,     /*!< \brief a filled rectangle tip \image html JKQTPFilledRectangleDecorator.png */
    JKQTPArrowAndBar,                 /*!< \brief a simple arrow tip, unfilled, with vertical bar \image html JKQTPArrowAndBar.png */
    JKQTPDoubleArrowAndBar,           /*!< \brief a simple double-arrow tip, unfilled, with vertical bar \image html JKQTPDoubleArrowAndBar.png */
    JKQTPBarDecorator,            /*!< \brief a full vertical bar \image html JKQTPBarDecorator.png */
    JKQTPBracketDecorator,             /*!< \brief a vertical bracket decorator \image html JKQTPBracketDecorator.png */
    JKQTPDiamondDecorator,           /*!< \brief an open diamond tip \image html JKQTPDiamondDecorator.png */
    JKQTPDiamondDecoratorAndBar,           /*!< \brief an open diamond tip \image html JKQTPDiamondDecoratorAndBar.png */
    JKQTPFilledDiamondDecorator,     /*!< \brief a filled diamond tip \image html JKQTPFilledDiamondDecorator.png */
    JKQTPFilledDiamondDecoratorAndBar,     /*!< \brief a filled diamond tip \image html JKQTPFilledDiamondDecoratorAndBar.png */
    JKQTPHalfBarDecorator,            /*!< \brief a half vertical bar \image html JKQTPHalfBarDecorator.png */
    JKQTPHarpoonDecorator,            /*!< \brief an harpoon arrow \image html JKQTPHarpoonDecorator.png */
    JKQTPHarpoonDecoratorAndBar,            /*!< \brief an harpoon arrow, with vertical bar \image html JKQTPHarpoonDecoratorAndBar.png */
    JKQTPSkewedBarDecorator,            /*!< \brief a skewed vertical bar \image html JKQTPSkewedBarDecorator.png */

    JKQTPLineDecoratorCount, /*!< \brief can be used to iterate over all symbols using: <code>for (int i=0; i<static_cast<int>(JKQTPLineDecoratorCount); i++) { JKQTPLineDecoratorStyle s=static_cast<JKQTPLineDecoratorStyle>(i); ... }</code> */
    JKQTPMaxLineDecoratorID=JKQTPLineDecoratorCount-1, /*!< \brief points to the last available symbol, can be used to iterate over all symbols: <code>for (int i=0; i<=static_cast<int>(JKQTPMaxLineDecoratorID); i++) { JKQTPLineDecoratorStyle s=static_cast<JKQTPLineDecoratorStyle>(i); ... }</code> */
    JKQTPDefaultLineDecorator=JKQTPFilledArrow /*!< \brief a default symbol used for plotting */
};

/** \brief converts a JKQTPLineDecoratorStyle variable into a identifier string
 * \ingroup jkqtptools_drawing
 */
JKQTCOMMON_LIB_EXPORT QString JKQTPLineDecoratorStyle2String(JKQTPLineDecoratorStyle pos);
/** \brief converts a JKQTPLineDecoratorStyle variable into a human-readable string
 * \ingroup jkqtptools_drawing
 */
JKQTCOMMON_LIB_EXPORT QString JKQTPLineDecoratorStyle2NameString(JKQTPLineDecoratorStyle pos);

/** \brief converts a String into a JKQTPLineDecoratorStyle
 * \ingroup jkqtptools_drawing
 */
JKQTCOMMON_LIB_EXPORT JKQTPLineDecoratorStyle String2JKQTPLineDecoratorStyle(const QString& pos);

/*! \brief plot the specified symbol at pixel position x,y. Note that this function only draws the decorator, NOT the line pointing to it!
   \ingroup jkqtptools_drawing

    \tparam TPainter Type of \a painter: A class like JKQTPEnhancedPainter or <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>
    \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to draw to
    \param x x-coordinate of the decorator tip
    \param y y-coordinate of the decorator tip
    \param angle_rad angle of the line pointing to (x,y), given in radians, 0rad points to the right, >0rad is a counter-clockwise rotation, as calculated by atan2() from dx, dy of a line!
    \param style type of the decorator to plot, see JKQTPLineDecoratorStyle
    \param size size of the decorator
    \param[out] line_start optional output parameter: when drawing the line let it end here, not necessarily at (x,y)
   */
template <class TPainter>
inline void JKQTPPlotLineDecorator(TPainter& painter, double x, double y, double angle_rad, JKQTPLineDecoratorStyle style, double size, QPointF* line_start=nullptr);

/** \brief calculates the tail decorator size from the line width \a line_width, using decoratorSizeFactor and a non-linear scaling function that levels off towards small \a line_width and increases sub-linearly for large ones, so the arrow heads to not grow too much */
JKQTCOMMON_LIB_EXPORT double JKQTPLineDecoratorStyleCalcDecoratorSize(double line_width, double decoratorSizeFactor);

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
JKQTCOMMON_LIB_EXPORT void JKQTPPlotSymbol(QPaintDevice& paintDevice, double x, double y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);




/*! \brief clips the given line (\a x1 , \a y1 ) -- (\a x2 , \a y2 ) to the given rectangle \a xmin .. \a xmax and \a ymin ... \a ymax
    \ingroup jkqtptools_drawing

    \return the clipped line in \a x1 , \a y1 , \a x2 , \a y2 and \c true if the line is still to be drawn or \c false else

    This function implements the algorithm descripbed in https://www.researchgate.net/publication/335018076_Another_Simple_but_Faster_Method_for_2D_Line_Clipping
    i.e. in Pseudocode
    \verbatim
    // x1 , y1 , x2 , y2 , xmin , ymax , xmax , ymin //
    if not ( x1<xmin and x2<xmin ) and not ( x1>xmax and x2>xmax ) then
        if not ( y1<ymin and y2<ymin ) and not ( y1>ymax and y2>ymax ) then
            x[1]= x1
            y[1]= y1
            x[2]= x2
            y[2]= y2
            i =1
            repeat
                if x[i] < xmin then
                    x[i] = xmin
                    y[i] = ( ( y2-y1 ) / ( x2-x1 ) ) * ( xmin-x1)+y1
                elseif x[i] > xmax then
                    x[i] = xmax
                    y[i] = ( ( y2-y1 ) / ( x2-x1 ) ) * ( xmax-x1)+y1
                endif
                if y[i] < ymin then
                    y[i] = ymin
                    x[i] = ( ( x2-x1 ) / ( y2-y1 ) ) * ( ymin-y1)+x1
                elseif y[i] > ymax then
                    y[i] = ymax
                    x[i] = ( ( x2-x1 ) / ( y2-y1 ) ) * ( ymax-y1)+x1
                endif
                i = i + 1
            until i >2
            if not ( x [1 ] < xmin and x [2 ] < xmin ) and not ( x [1 ] >xmax and x [2 ] >xmax ) then
                drawLine ( x[1] , y[1] , x[2] , y[2] )
            endif
        endif
    endif
    \endverbatim
*/
inline bool JKQTPClipLine(double& x1 , double& y1 , double& x2 , double& y2 , double xmin , double xmax , double ymin, double ymax) {
    if (! ( x1<xmin && x2<xmin ) && !( x1>xmax && x2>xmax )) {
        if ( !( y1<ymin && y2<ymin ) && !( y1>ymax && y2>ymax ) ) {
            double x[2]= {x1,x2};
            double y[2]= {y1,y2};
            for (int i=0; i<2; i++) {
            if (x[i] < xmin) {
                    x[i] = xmin;
                    y[i] = ( ( y2-y1 ) / ( x2-x1 ) ) * ( xmin-x1)+y1;
               } else if (x[i] > xmax) {
                    x[i] = xmax;
                    y[i] = ( ( y2-y1 ) / ( x2-x1 ) ) * ( xmax-x1)+y1;
               }
               if (y[i] < ymin) {
                    y[i] = ymin;
                    x[i] = ( ( x2-x1 ) / ( y2-y1 ) ) * ( ymin-y1)+x1;
               } else if (y[i] > ymax) {
                    y[i] = ymax;
                    x[i] = ( ( x2-x1 ) / ( y2-y1 ) ) * ( ymax-y1)+x1;
                }
            }
            if (! ( x[0] < xmin && x[1]< xmin ) && !( x[0] >xmax && x[1] >xmax )) {
                x1=x[0];
                y1=y[0];
                x2=x[1];
                y2=y[1];
                return true;
            }
        }
    }
    return false;
}

/*! \brief clips the given line \a line to the given rectangle rectangle \a xmin .. \a xmax and \a ymin ... \a ymax
    \ingroup jkqtptools_drawing

    \return the clipped line or a line with 0-length, i.e. QLineF()

    This function implements the algorithm descripbed in https://www.researchgate.net/publication/335018076_Another_Simple_but_Faster_Method_for_2D_Line_Clipping

*/
inline QLineF JKQTPClipLine(const QLineF& line, double xmin , double xmax , double ymin, double ymax) {
    double x1=line.x1();
    double y1=line.y1();
    double x2=line.x2();
    double y2=line.y2();
    if (JKQTPClipLine(x1,y1,x2,y2,xmin,xmax,ymin,ymax)) {
        return QLineF(x1,y1,x2,y2);
    } else {
        return QLineF();
    }
}

/*! \brief clips the given line \a line to the given rectangle \a clipRect
    \ingroup jkqtptools_drawing

    \param line line to be clipped
    \param clipRect rectangle to clip to
    \return the clipped line or a line with 0-length, i.e. QLineF()

    This function implements the algorithm descripbed in https://www.researchgate.net/publication/335018076_Another_Simple_but_Faster_Method_for_2D_Line_Clipping

*/
inline QLineF JKQTPClipLine(const QLineF& line, const QRectF& clipRect) {
    const double xmin=qMin(clipRect.left(), clipRect.right());
    const double xmax=qMax(clipRect.left(), clipRect.right());
    const double ymin=qMin(clipRect.top(), clipRect.bottom());
    const double ymax=qMax(clipRect.top(), clipRect.bottom());

    double x1=line.x1();
    double y1=line.y1();
    double x2=line.x2();
    double y2=line.y2();
    if (JKQTPClipLine(x1,y1,x2,y2,xmin,xmax,ymin,ymax)) {
        return QLineF(x1,y1,x2,y2);
    } else {
        return QLineF();
    }
}

/*! \brief clips the given list of poly-lines \a polylines_in to the given rectangle \a clipRect
    \ingroup jkqtptools_drawing

    \param polylines_in list of poly-lines to be clipped
    \param clipRect rectangle to clip to
    \return a list of poly-lines representing the clipped lines. Note that some lines may be split further so the number of poly-lines in the output may actually be larger than the number of polylines in the input!
*/
JKQTCOMMON_LIB_EXPORT QList<QPolygonF> JKQTPClipPolyLines(const QList<QPolygonF> & polylines_in, const QRectF& clipRect);
/*! \brief clips the given poly-line \a polyline_in to the given rectangle \a clipRect
    \ingroup jkqtptools_drawing

    \param polyline_in poly-line to be clipped
    \param clipRect rectangle to clip to
    \return a list of poly-lines representing the clipped line.
*/
JKQTCOMMON_LIB_EXPORT QList<QPolygonF> JKQTPClipPolyLine(const QPolygonF & polyline_in, const QRectF& clipRect);

/*! \brief tries to reduce the complexity of the given list of poly-lines \a lines_in, but keeping the appearance as if all lines were drawn
    \ingroup jkqtptools_drawing

    \param lines_in list of poly-lines to be simplified
    \param maxDeltaXY a group has to be either less wide or less high than this, typically equals the linewidth of the poly-line
    \return a simplified version of lines_in
*/
JKQTCOMMON_LIB_EXPORT QList<QPolygonF> JKQTPSimplifyPolyLines(const QList<QPolygonF>& lines_in, double maxDeltaXY=1.0);

/*! \brief tries to reduce the complexity of the given poly-line \a lines_in, but keeping the appearance as if all lines were drawn
    \ingroup jkqtptools_drawing

    \param lines_in poly-line to be simplified
    \param maxDeltaXY a group has to be either less wide or less high than this, typically equals the linewidth of the poly-line
    \return a simplified version of lines_in
*/
JKQTCOMMON_LIB_EXPORT QPolygonF JKQTPSimplifyPolyLines(const QPolygonF& lines_in, double maxDeltaXY=1.0);

/*! \brief draw a tooltip, using the current brush and pen of the provided painter
    \ingroup jkqtptools_drawing

    \tparam TPainter Type of \a painter: A class like JKQTPEnhancedPainter or <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>
    \param painter QPainter-like object to use for painting
    \param x x-coordinate of position the tooltip points to
    \param y y-coordinate of position the tooltip points to
    \param rect rectangle of the main tooltip area

    \image html tooltiptool_example.png
*/
template <class TPainter>
inline void JKQTPDrawTooltip(TPainter& painter, double x, double y, const QRectF& rect);










template <class TPainter>
inline void JKQTPPlotSymbol(TPainter& painter, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, double symbolLineWidth, QColor color, QColor fillColor) {
    if (symbol==JKQTPNoSymbol) return;
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,  symbolLineWidth));
    p.setStyle(Qt::SolidLine);
    p.setCapStyle(Qt::FlatCap);
    QPen pDescaled=p;
    pDescaled.setWidthF(pDescaled.widthF()/symbolSize);
    const QBrush b=QBrush(fillColor, Qt::SolidPattern);


    static bool pathsInitialized=false;
    static std::array<QPainterPath, JKQTPSymbolCount> paths;
    static std::array<QPainterPath, JKQTPSymbolCount> filledpaths;
    static std::array<QVector<QLineF>, JKQTPSymbolCount> lines;
    static std::array<QPolygonF, JKQTPSymbolCount> polygons;
    static std::array<QPolygonF, JKQTPSymbolCount> filledpolygons;
    static std::array<qreal, JKQTPSymbolCount> pathsrotation;
    if (!pathsInitialized) {
        // calculate star cordinates as static values
        static double s45=fabs(cos(45.0/180.0*JKQTPSTATISTICS_PI));
        static int star5_items=0;
        static double star5cordsx[10];
        static double star5cordsy[10];
        if (star5_items==0) {
            star5_items=5;
            double angle=360.0/double(star5_items)/180.0*JKQTPSTATISTICS_PI;
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
            double angle=360.0/double(star6_items)/180.0*JKQTPSTATISTICS_PI;
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
            double angle=360.0/double(star8_items)/180.0*JKQTPSTATISTICS_PI;
            for (int i=0; i<star8_items; i++) {
                double a=(static_cast<double>(i)+0.5)*angle;
                star8cordsx[i*2]=sin(a);
                star8cordsx[i*2+1]=0.5*sin(a+angle/2.0);
                star8cordsy[i*2]=cos(a);
                star8cordsy[i*2+1]=0.5*cos(a+angle/2.0);
            }
        }

        pathsrotation.fill(0);
        paths[JKQTPCross].moveTo(-0.5,-0.5);
        paths[JKQTPCross].lineTo(0.5,0.5);
        paths[JKQTPCross].moveTo(-0.5,+0.5);
        paths[JKQTPCross].lineTo(+0.5,-0.5);
        paths[JKQTPPlus].moveTo(-0.5,0);
        paths[JKQTPPlus].lineTo(0.5,0);
        paths[JKQTPPlus].moveTo(0,+0.5);
        paths[JKQTPPlus].lineTo(0,-0.5);
        paths[JKQTPCircle].addEllipse(QPointF(0,0), 0.5, 0.5);
        filledpaths[JKQTPFilledCircle]=paths[JKQTPCircle];
        paths[JKQTPCircleCross].addEllipse(QPointF(0,0), 0.5, 0.5);
        paths[JKQTPCircleCross].moveTo(-0.5*s45,-0.5*s45);
        paths[JKQTPCircleCross].lineTo(0.5*s45,0.5*s45);
        paths[JKQTPCircleCross].moveTo(-0.5*s45,+0.5*s45);
        paths[JKQTPCircleCross].lineTo(+0.5*s45,-0.5*s45);
        paths[JKQTPCirclePlus].addEllipse(QPointF(0,0), 0.5, 0.5);
        paths[JKQTPCirclePlus].moveTo(-0.5,0);
        paths[JKQTPCirclePlus].lineTo(0.5,0);
        paths[JKQTPCirclePlus].moveTo(0,+0.5);
        paths[JKQTPCirclePlus].lineTo(0,-0.5);
        paths[JKQTPCirclePeace].addEllipse(QPointF(0,0), 0.5, 0.5);
        paths[JKQTPCirclePeace].moveTo(0,-0.5);
        paths[JKQTPCirclePeace].lineTo(0, 0.5);
        paths[JKQTPCirclePeace].moveTo(0,0);
        paths[JKQTPCirclePeace].lineTo(0.5*s45,0.5*s45);
        paths[JKQTPCirclePeace].moveTo(0,0);
        paths[JKQTPCirclePeace].lineTo(-0.5*s45,0.5*s45);
        paths[JKQTPPeace].moveTo(0,-0.5);
        paths[JKQTPPeace].lineTo(0, 0.5);
        paths[JKQTPPeace].moveTo(0,0);
        paths[JKQTPPeace].lineTo(0.5*s45,0.5*s45);
        paths[JKQTPPeace].moveTo(0,0);
        paths[JKQTPPeace].lineTo(-0.5*s45,0.5*s45);
        paths[JKQTPTarget].addEllipse(QPointF(0,0), 0.33333, 0.33333);
        paths[JKQTPTarget].moveTo(QPointF(0,-0.5));
        paths[JKQTPTarget].lineTo(QPointF(0,0.5));
        paths[JKQTPTarget].moveTo(QPointF(-0.5,0));
        paths[JKQTPTarget].lineTo(QPointF(0.5,0));
        paths[JKQTPFemale].addEllipse(-0.25,-0.5,0.5,0.5);
        paths[JKQTPFemale].moveTo(0,0);
        paths[JKQTPFemale].lineTo(0,0.5);
        paths[JKQTPFemale].moveTo(-0.5/3.0,0.5/2.0);
        paths[JKQTPFemale].lineTo(0.5/3.0,0.5/2.0);
        paths[JKQTPMale].addEllipse(QRectF(-0.5/2.0, -0.5/2.0, 0.5, 0.5));
        paths[JKQTPMale].moveTo(QPointF(+0.5/2.0*cos(45.0/180.0*JKQTPSTATISTICS_PI),-0.5/2.0*cos(45.0/180.0*JKQTPSTATISTICS_PI)));
        paths[JKQTPMale].lineTo(QPointF(+0.5,-0.5));
        paths[JKQTPMale].moveTo(QPointF(+0.5-0.5/2.0,-0.5));
        paths[JKQTPMale].lineTo(QPointF(+0.5,-0.5));
        paths[JKQTPMale].lineTo(QPointF(+0.5,-0.5+0.5/2.0));
        paths[JKQTPRect].addRect(-0.5,-0.5, 1,1);
        filledpaths[JKQTPFilledRect]=paths[JKQTPRect];
        paths[JKQTPRectCross].addRect(-0.5,-0.5, 1,1);
        paths[JKQTPRectCross].moveTo(-0.5,-0.5);
        paths[JKQTPRectCross].lineTo(0.5,0.5);
        paths[JKQTPRectCross].moveTo(-0.5,+0.5);
        paths[JKQTPRectCross].lineTo(+0.5,-0.5);
        paths[JKQTPRectPlus].addRect(-0.5,-0.5, 1,1);
        paths[JKQTPRectPlus].moveTo(-0.5,0);
        paths[JKQTPRectPlus].lineTo(0.5,0);
        paths[JKQTPRectPlus].moveTo(0,+0.5);
        paths[JKQTPRectPlus].lineTo(0,-0.5);
        paths[JKQTPCurvedTriangle].moveTo(0,0-0.5);
        paths[JKQTPCurvedTriangle].quadTo(0-1.0/10.0,0+1.0/4.0, 0-0.5,0+0.5);
        paths[JKQTPCurvedTriangle].quadTo(0,0+1.0/4.0, 0+0.5,0+0.5);
        paths[JKQTPCurvedTriangle].quadTo(0+1.0/10.0,0+1.0/4.0, 0,0-0.5);
        filledpaths[JKQTPFilledCurvedTriangle]=paths[JKQTPCurvedTriangle];

        paths[JKQTPDownCurvedTriangle]=paths[JKQTPCurvedTriangle];
        pathsrotation[JKQTPDownCurvedTriangle]=180.0;
        filledpaths[JKQTPFilledDownCurvedTriangle]=paths[JKQTPDownCurvedTriangle];
        pathsrotation[JKQTPFilledDownCurvedTriangle]=180.0;

        paths[JKQTPLeftCurvedTriangle]=paths[JKQTPCurvedTriangle];
        pathsrotation[JKQTPLeftCurvedTriangle]=-90.0;
        filledpaths[JKQTPFilledLeftCurvedTriangle]=paths[JKQTPLeftCurvedTriangle];
        pathsrotation[JKQTPFilledLeftCurvedTriangle]=-90.0;

        paths[JKQTPRightCurvedTriangle]=paths[JKQTPCurvedTriangle];
        pathsrotation[JKQTPRightCurvedTriangle]=90.0;
        filledpaths[JKQTPFilledRightCurvedTriangle]=paths[JKQTPRightCurvedTriangle];
        pathsrotation[JKQTPFilledRightCurvedTriangle]=90.0;

        {
            QPolygonF poly;
            painter.setBrush(QColor(Qt::transparent));
            poly<<QPointF(0.0, 0.0-0.5)<<QPointF(0.0+0.5, 0.0)<<QPointF(0.0, 0.0+0.5)<<QPointF(0.0-0.5, 0.0);
            poly<<poly[0];
            paths[JKQTPDiamondPlus].addPolygon(poly);
            paths[JKQTPDiamondPlus].moveTo(poly[0]);
            paths[JKQTPDiamondPlus].lineTo(poly[2]);
            paths[JKQTPDiamondPlus].moveTo(poly[1]);
            paths[JKQTPDiamondPlus].lineTo(poly[3]);
        }
        {
            QPolygonF poly;
            painter.setBrush(QColor(Qt::transparent));
            poly<<QPointF(0.0, 0.0-0.5)<<QPointF(0.0+0.5, 0.0)<<QPointF(0.0, 0.0+0.5)<<QPointF(0.0-0.5, 0.0);
            poly<<poly[0];
            paths[JKQTPDiamondCross].addPolygon(poly);
            paths[JKQTPDiamondCross].moveTo((poly[0]+poly[1])/2.0);
            paths[JKQTPDiamondCross].lineTo((poly[2]+poly[3])/2.0);
            paths[JKQTPDiamondCross].moveTo((poly[1]+poly[2])/2.0);
            paths[JKQTPDiamondCross].lineTo((poly[3]+poly[0])/2.0);
        }


        for (int i=0; i<star8_items*2; i+=2) {
            paths[JKQTPAsterisc8].moveTo(star8cordsx[i]*0.5, star8cordsy[i]*0.5);
            paths[JKQTPAsterisc8].lineTo(0,0);
        }
        for (int i=0; i<star6_items*2; i+=2) {
            paths[JKQTPAsterisc6].moveTo(star6cordsx[i]*0.5, star6cordsy[i]*0.5);
            paths[JKQTPAsterisc6].lineTo(0,0);
        }
        for (int i=0; i<star5_items*2; i+=2) {
            paths[JKQTPAsterisc].moveTo(star5cordsx[i]*0.5, star5cordsy[i]*0.5);
            paths[JKQTPAsterisc].lineTo(0,0);
        }

        polygons[JKQTPRectTriangle]<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0, 0.0-0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0+0.5);
        polygons[JKQTPRectDownTriangle]<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0, 0.0+0.5)<<QPointF(0.0+0.5, 0.0-0.5)<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0-0.5);
        polygons[JKQTPRectLeftTriangle]<<QPointF(0.0+0.5, 0.0-0.5)<<QPointF(0.0-0.5, 0.0)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0+0.5);
        polygons[JKQTPRectRightTriangle]<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0)<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0-0.5)<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0-0.5, 0.0+0.5);
        polygons[JKQTPTriangle]<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0, 0.0-0.5);
        filledpolygons[JKQTPFilledTriangle]=polygons[JKQTPTriangle];
        polygons[JKQTPDownTriangle]<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0-0.5)<<QPointF(0.0, 0.0+0.5);
        filledpolygons[JKQTPFilledDownTriangle]=polygons[JKQTPDownTriangle];
        polygons[JKQTPLeftTriangle]<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0)<<QPointF(0.0+0.5, 0.0-0.5);
        filledpolygons[JKQTPFilledLeftTriangle]=polygons[JKQTPLeftTriangle];
        polygons[JKQTPRightTriangle]<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0)<<QPointF(0.0-0.5, 0.0-0.5);
        filledpolygons[JKQTPFilledRightTriangle]=polygons[JKQTPRightTriangle];
        for (int i=0; i<star5_items*2; i++) {
            polygons[JKQTPstar]<<QPointF(0.0+star5cordsx[i]*0.5, 0.0+star5cordsy[i]*0.5);
            filledpolygons[JKQTPFilledStar]<<QPointF(0.0+star5cordsx[i]*0.5, 0.0+star5cordsy[i]*0.5);
            if (i%2==0) {
                polygons[JKQTPPentagon]<<QPointF(0.0+star5cordsx[i]*0.5, 0.0+star5cordsy[i]*0.5);
                filledpolygons[JKQTPFilledPentagon]<<QPointF(0.0+star5cordsx[i]*0.5, 0.0+star5cordsy[i]*0.5);
            }
        }
        for (int i=0; i<star6_items*2; i+=2) {
            polygons[JKQTPHexagon]<<QPointF(0.0+star6cordsx[i]*0.5, 0.0+star6cordsy[i]*0.5);
            filledpolygons[JKQTPFilledHexagon]<<QPointF(0.0+star6cordsx[i]*0.5, 0.0+star6cordsy[i]*0.5);
        }
        for (int i=0; i<star8_items*2; i+=2) {
            polygons[JKQTPOctagon]<<QPointF(0.0+star8cordsx[i]*0.5, 0.0+star8cordsy[i]*0.5);
            filledpolygons[JKQTPFilledOctagon]<<QPointF(0.0+star8cordsx[i]*0.5, 0.0+star8cordsy[i]*0.5);
        }
        polygons[JKQTPDiamond]<<QPointF(0.0, 0.0-0.5)<<QPointF(0.0+0.5, 0.0)<<QPointF(0.0, 0.0+0.5)<<QPointF(0.0-0.5, 0.0);
        filledpolygons[JKQTPFilledDiamond]=polygons[JKQTPDiamond];
        polygons[JKQTPHourglass]<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0-0.5);
        filledpolygons[JKQTPFilledHourglass]=polygons[JKQTPHourglass];
        polygons[JKQTPHorizontalHourglass]<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0-0.5);
        filledpolygons[JKQTPFilledHorizontalHourglass]=polygons[JKQTPHorizontalHourglass];
        polygons[JKQTPSantaClauseHouse]<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0+0.5, 0.0-1.0/6.0)<<QPointF(0.0-0.5, 0.0-1.0/6.0)<<QPointF(0.0, 0.0-0.5)<<QPointF(0.0+0.5, 0.0-1.0/6.0)<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0-0.5, 0.0-1.0/6.0)<<QPointF(0.0+0.5, 0.0+0.5);
        filledpolygons[JKQTPFilledSantaClauseHouse]=polygons[JKQTPSantaClauseHouse];
        polygons[JKQTPUpDownTriangle]<<QPointF(0.0-0.5, 0.0+0.5)<<QPointF(0.0, 0.0+0.5)<<QPointF(0.0+0.5, 0.0-0.5)<<QPointF(0.0-0.5, 0.0-0.5)<<QPointF(0.0, 0.0+0.5)<<QPointF(0.0+0.5, 0.0+0.5)<<QPointF(0.0, 0.0-0.5)<<QPointF(0.0-0.5, 0.0+0.5);
        filledpolygons[JKQTPFilledUpDownTriangle]=polygons[JKQTPUpDownTriangle];


        lines[JKQTPTripod]<<QLineF(0.0, 0.0-0.5, 0.0, 0.0)
            <<QLineF(0.0, 0.0, 0.0-s45, 0.0+s45)
            <<QLineF(0.0, 0.0, 0.0+s45, 0.0+s45);

        lines[JKQTPDownTripod]<<QLineF(0.0, 0.0+0.5, 0.0, 0.0)
            <<QLineF(0.0, 0.0, 0.0-s45, 0.0-s45)
            <<QLineF(0.0, 0.0, 0.0+s45, 0.0-s45);

        lines[JKQTPLeftTripod]<<QLineF(0.0-0.5, 0.0, 0.0, 0.0)
            <<QLineF(0.0, 0.0, 0.0+s45, 0.0-s45)
            <<QLineF(0.0, 0.0, 0.0+s45, 0.0+s45);

        lines[JKQTPRightTripod]<<QLineF(0.0+0.5, 0.0, 0.0, 0.0)
            <<QLineF(0.0, 0.0, 0.0-s45, 0.0-s45)
            <<QLineF(0.0, 0.0, 0.0-s45, 0.0+s45);

        pathsInitialized=true;
    }

    painter.setBrush(QColor(Qt::transparent));
    painter.setPen(p);
    switch(symbol) {
        case JKQTPDot:
            painter.drawPoint(QPointF(x,y));
            break;

        case JKQTPCross:
        case JKQTPPlus:
        case JKQTPCircle:
        case JKQTPCircleCross:
        case JKQTPCirclePlus:
        case JKQTPCirclePeace:
        case JKQTPPeace:
        case JKQTPFemale:
        case JKQTPMale:
        case JKQTPTarget:
        case JKQTPRect:
        case JKQTPRectCross:
        case JKQTPRectPlus:
        case JKQTPDownCurvedTriangle:
        case JKQTPCurvedTriangle:
        case JKQTPLeftCurvedTriangle:
        case JKQTPRightCurvedTriangle:
        case JKQTPAsterisc:
        case JKQTPAsterisc6:
        case JKQTPAsterisc8:
        case JKQTPDiamondPlus:
        case JKQTPDiamondCross:
                painter.translate(QPointF(x,y));
                painter.scale(symbolSize,symbolSize);
                painter.setBrush(QColor(Qt::transparent));
                painter.setPen(pDescaled);
                if (pathsrotation[symbol]!=0.0) painter.rotate(pathsrotation[symbol]);
                painter.drawPath(paths[symbol]);
            break;



        case JKQTPFilledCircle:
        case JKQTPFilledRect:
        case JKQTPFilledDownCurvedTriangle:
        case JKQTPFilledCurvedTriangle:
        case JKQTPFilledLeftCurvedTriangle:
        case JKQTPFilledRightCurvedTriangle:
                painter.translate(QPointF(x,y));
                painter.scale(symbolSize,symbolSize);
                painter.setBrush(b);
                painter.setPen(pDescaled);
                if (pathsrotation[symbol]!=0.0) painter.rotate(pathsrotation[symbol]);
                painter.drawPath(filledpaths[symbol]);

            break;



        case JKQTPRectTriangle:
        case JKQTPRectDownTriangle:
        case JKQTPRectLeftTriangle:
        case JKQTPRectRightTriangle:
        case JKQTPTriangle:
        case JKQTPDownTriangle:
        case JKQTPLeftTriangle:
        case JKQTPRightTriangle:
        case JKQTPstar:
        case JKQTPPentagon:
        case JKQTPHexagon:
        case JKQTPOctagon:
        case JKQTPUpDownTriangle:
        case JKQTPSantaClauseHouse:
        case JKQTPHourglass:
        case JKQTPHorizontalHourglass:
        case JKQTPDiamond:
                painter.translate(QPointF(x,y));
                painter.scale(symbolSize,symbolSize);
                painter.setBrush(QColor(Qt::transparent));
                painter.setPen(pDescaled);
                if (pathsrotation[symbol]!=0.0) painter.rotate(pathsrotation[symbol]);
                painter.drawConvexPolygon(polygons[symbol]);
            break;



        case JKQTPFilledTriangle:
        case JKQTPFilledDownTriangle:
        case JKQTPFilledLeftTriangle:
        case JKQTPFilledRightTriangle:
        case JKQTPFilledStar:
        case JKQTPFilledPentagon:
        case JKQTPFilledHexagon:
        case JKQTPFilledOctagon:
        case JKQTPFilledSantaClauseHouse:
        case JKQTPFilledUpDownTriangle:
        case JKQTPFilledHourglass:
        case JKQTPFilledHorizontalHourglass:
        case JKQTPFilledDiamond:
                painter.translate(QPointF(x,y));
                painter.scale(symbolSize,symbolSize);
                painter.setBrush(b);
                painter.setPen(pDescaled);
                if (pathsrotation[symbol]!=0.0) painter.rotate(pathsrotation[symbol]);
                painter.drawConvexPolygon(filledpolygons[symbol]);
            break;



        case JKQTPTripod:
        case JKQTPDownTripod:
        case JKQTPLeftTripod:
        case JKQTPRightTripod:
            painter.translate(QPointF(x,y));
            painter.scale(symbolSize,symbolSize);
            painter.setBrush(QColor(Qt::transparent));
            painter.setPen(pDescaled);
            if (pathsrotation[symbol]!=0.0) painter.rotate(pathsrotation[symbol]);
            painter.drawLines(lines[symbol]);
        break;



        case JKQTPNoSymbol:
        case JKQTPSymbolCount:
            break;
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

template <class TPainter>
inline void JKQTPPlotLineDecorator(TPainter& painter, double x, double y, double angle_rad, JKQTPLineDecoratorStyle style, double size, QPointF* line_start) {
    if (line_start) *line_start=QPointF(x,y);
    static double default_theta_open_tip=60.0/2.0 /180.0*JKQTPSTATISTICS_PI;
    static double tan__default_theta_open_tip=tan(default_theta_open_tip);
    static double default_theta_closed_tip=50.0/2.0 /180.0*JKQTPSTATISTICS_PI;
    static double tan__default_theta_closed_tip=tan(default_theta_closed_tip);
    QPen pinit=painter.pen();
    pinit.setCapStyle(Qt::FlatCap);
    pinit.setJoinStyle(Qt::RoundJoin);
    QPen p0=pinit;
    p0.setWidthF(0);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(x,y);
        painter.rotate(angle_rad/JKQTPSTATISTICS_PI*180.0);
        painter.setPen(p0);

        switch(style) {
        case JKQTPArrow:
        case JKQTPArrowAndBar: {
            const QPointF poly[3] = {
                QPointF(size, -tan__default_theta_open_tip*size),
                QPointF(0,0),
                QPointF(size, tan__default_theta_open_tip*size)
            };
            painter.setPen(pinit);
            if (style==JKQTPArrowAndBar) painter.drawLine(QPointF(0,-tan__default_theta_open_tip*size), QPointF(0,tan__default_theta_open_tip*size));
            painter.drawPolyline(poly, 3);
        } break;
        case JKQTPHarpoonDecorator:
        case JKQTPHarpoonDecoratorAndBar: {
            painter.setPen(pinit);
            if (style==JKQTPHarpoonDecoratorAndBar) painter.drawLine(QPointF(0,-tan__default_theta_open_tip*size), QPointF(0,tan__default_theta_open_tip*size));
            painter.drawLine(QPointF(0,0), QPointF(size, tan__default_theta_open_tip*size));
        } break;
        case JKQTPDoubleArrow:
        case JKQTPDoubleArrowAndBar: {
            const QPointF poly[3] = {
                QPointF(size, -tan__default_theta_open_tip*size),
                QPointF(0,0),
                QPointF(size, tan__default_theta_open_tip*size)
            };
            painter.setPen(pinit);
            if (style==JKQTPDoubleArrowAndBar) painter.drawLine(QPointF(0,-tan__default_theta_open_tip*size), QPointF(0,tan__default_theta_open_tip*size));
            painter.drawPolyline(poly, 3);
            painter.translate(4.0*pinit.widthF(),0);
            painter.drawPolyline(poly, 3);
            if (line_start) *line_start=QPointF(x,y)+QPointF(4.0*pinit.widthF()*cos(angle_rad),4.0*pinit.widthF()*sin(angle_rad));
        } break;
        case JKQTPFilledArrow:
        case JKQTPFilledDoubleArrow: {
            const QPointF poly[4] = {
                QPointF(0,0),
                QPointF(size, tan__default_theta_closed_tip*size),
                QPointF(0.75*size,0),
                QPointF(size, -tan__default_theta_closed_tip*size)
            };
            painter.drawPolygon(poly, 4);
            if (style==JKQTPFilledDoubleArrow) {
                painter.translate(0.5*size, 0);
                painter.drawPolygon(poly, 4);
                if (line_start) *line_start=QPointF(x,y)+QPointF(1.25*size*cos(angle_rad),1.25*size*sin(angle_rad));
            } else {
                if (line_start) *line_start=QPointF(x,y)+QPointF(0.75*size*cos(angle_rad),0.75*size*sin(angle_rad));
            }
        } break;

        case JKQTPTriangleDecorator:
        case JKQTPTriangleDecoratorAndBar: {
            const QPointF poly[3] = {
                QPointF(size, -tan__default_theta_closed_tip*size),
                QPointF(0,0),
                QPointF(size, tan__default_theta_closed_tip*size)
            };
            painter.setBrush(Qt::NoBrush);
            painter.setPen(pinit);
            painter.drawConvexPolygon(poly, 3);
            if (style==JKQTPTriangleDecoratorAndBar) painter.drawLine(QPointF(0,-tan__default_theta_closed_tip*size), QPointF(0,tan__default_theta_closed_tip*size));
            if (line_start) *line_start=QPointF(x,y)+QPointF(size*cos(angle_rad),size*sin(angle_rad));
        } break;

        case JKQTPFilledTriangleDecorator:
        case JKQTPFilledTriangleDecoratorAndBar: {
            const QPointF poly[3] = {
                QPointF(size, -tan__default_theta_closed_tip*size),
                QPointF(0,0),
                QPointF(size, tan__default_theta_closed_tip*size)
            };
            painter.setPen(p0);
            painter.drawConvexPolygon(poly, 3);
            if (style==JKQTPFilledTriangleDecoratorAndBar) {
                painter.setPen(pinit);
                painter.drawLine(QPointF(0,-tan__default_theta_closed_tip*size), QPointF(0,tan__default_theta_closed_tip*size));
            }
            if (line_start) *line_start=QPointF(x,y)+QPointF(size*cos(angle_rad),size*sin(angle_rad));
        } break;


        case JKQTPDiamondDecorator:
        case JKQTPDiamondDecoratorAndBar: {
            const QPointF poly[4] = {
                QPointF(0,0),
                QPointF(size/2.0, -tan__default_theta_closed_tip*size),
                QPointF(size,0),
                QPointF(size/2.0, tan__default_theta_closed_tip*size)
            };
            painter.setBrush(Qt::NoBrush);
            painter.setPen(pinit);
            painter.drawConvexPolygon(poly, 4);
            if (style==JKQTPDiamondDecoratorAndBar) painter.drawLine(QPointF(0,-tan__default_theta_closed_tip*size), QPointF(0,tan__default_theta_closed_tip*size));
            if (line_start) *line_start=QPointF(x,y)+QPointF(size*cos(angle_rad),size*sin(angle_rad));
        } break;

        case JKQTPFilledDiamondDecorator:
        case JKQTPFilledDiamondDecoratorAndBar: {
            const QPointF poly[4] = {
                QPointF(0,0),
                QPointF(size/2.0, -tan__default_theta_closed_tip*size),
                QPointF(size,0),
                QPointF(size/2.0, tan__default_theta_closed_tip*size)
            };
            painter.setPen(p0);
            painter.drawConvexPolygon(poly, 4);
            if (style==JKQTPFilledDiamondDecoratorAndBar) {
                painter.setPen(pinit);
                painter.drawLine(QPointF(0,-tan__default_theta_closed_tip*size), QPointF(0,tan__default_theta_closed_tip*size));
            }
            if (line_start) *line_start=QPointF(x,y)+QPointF(size*cos(angle_rad),size*sin(angle_rad));
        } break;

        case JKQTPCircleDecorator:
        case JKQTPFilledCircleDecorator: {
            if (style==JKQTPCircleDecorator) {
                painter.setBrush(Qt::NoBrush);
                painter.setPen(pinit);
            } else {
                painter.setPen(p0);
            }
            painter.drawEllipse(QRectF(0,-size/2.0,size,size));
            if (line_start) *line_start=QPointF(x,y)+QPointF(size*cos(angle_rad),size*sin(angle_rad));
        } break;

        case JKQTPRectangleDecorator:
        case JKQTPFilledRectangleDecorator: {
            if (style==JKQTPRectangleDecorator) {
                painter.setBrush(Qt::NoBrush);
                painter.setPen(pinit);
            } else {
                painter.setPen(p0);
            }
            painter.drawRect(QRectF(0,-size/2.0,size,size));
            if (line_start) *line_start=QPointF(x,y)+QPointF(size*cos(angle_rad),size*sin(angle_rad));
        } break;

        case JKQTPBarDecorator: {
            painter.setPen(pinit);
            painter.drawLine(QPointF(0,-tan__default_theta_open_tip*size), QPointF(0,tan__default_theta_open_tip*size));
        } break;

        case JKQTPHalfBarDecorator: {
            painter.setPen(pinit);
            painter.drawLine(QPointF(0,0), QPointF(0,tan__default_theta_open_tip*size));
        } break;

        case JKQTPSkewedBarDecorator: {
            painter.setPen(pinit);
            painter.drawLine(QPointF(0.25*size,-tan__default_theta_open_tip*size), QPointF(-0.25*size,tan__default_theta_open_tip*size));
        } break;

        case JKQTPBracketDecorator: {
            const QPointF poly[4] = {
                QPointF(-size*0.2,-tan__default_theta_open_tip*size),
                QPointF(0,-tan__default_theta_open_tip*size),
                QPointF(0,tan__default_theta_open_tip*size),
                QPointF(-size*0.2, tan__default_theta_open_tip*size)
            };
            painter.setPen(pinit);
            painter.drawPolyline(poly, 4);
        } break;
        case JKQTPLineDecoratorCount:
        case JKQTPNoDecorator:
            break;
        }

    }
}


#endif // JKQTPDRAWINGTOOLS_H_INCLUDED
