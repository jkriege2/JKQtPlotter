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
#ifndef jkqtpgraphsrange_H
#define jkqtpgraphsrange_H


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;
//class JKQTPColorPaletteStyleAndToolsMixin;







/** \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
 *         or a range of interest, or the range of good values, ...
 *  \ingroup jkqtplotter_annotations
 *
 *  \image html JKQTPRangeBase.png
 *
 *  You can also invert the range, i.e. everything outside the range will be filled (\c setInvertedRange() ):
 *
 *  \image html JKQTPRangeBase_Inverted.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPRangeBase: public JKQTPPlotElement, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPRangeBase(JKQTBasePlotter* parent=nullptr);


        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;


        /** \copydoc centerColor */
        QColor getCenterColor() const;
        /** \copydoc centerStyle */
        Qt::PenStyle getCenterStyle() const;
        /** \copydoc centerLineWidth */
        void setCenterLineWidth(double __value);
        /** \copydoc centerLineWidth */
        double getCenterLineWidth() const;
        /** \copydoc rangeMin */
        double getRangeMin() const;
        /** \copydoc rangeMax */
        double getRangeMax() const;
        /** \copydoc sizeMin */
        double getSizeMin() const;
        /** \copydoc sizeMax */
        double getSizeMax() const;
        /** \copydoc unlimitedSizeMin */
        bool getUnlimitedSizeMin() const;
        /** \copydoc unlimitedSizeMax */
        bool getUnlimitedSizeMax() const;

        /** \copydoc rangeCenter */
        double getRangeCenter() const;
        /** \copydoc plotCenterLine */
        bool getPlotCenterLine() const;
        /** \copydoc invertedRange */
        bool getInvertedRange() const;
        /** \copydoc plotRange */
        bool getPlotRange() const;
        /** \copydoc fillRange */
        bool getFillRange() const;
        /** \copydoc plotRangeLines */
        bool getPlotRangeLines() const;
    public Q_SLOTS:
        /** \brief set the color of the graph (all lines and filling)  */
        virtual void setColor(QColor c);

        /** \brief disables all drawing features except the centerline */
        void setDrawCenterLineOnly();
        /** \copydoc centerColor */
        void setCenterColor(const QColor & __value);
        /** \copydoc centerStyle */
        void setCenterStyle(Qt::PenStyle  __value);
        /** \copydoc plotRangeLines */
        void setPlotRangeLines(bool __value);
        /** \copydoc fillRange */
        void setFillRange(bool __value);
        /** \copydoc plotRange */
        void setPlotRange(bool __value);
        /** \copydoc invertedRange */
        void setInvertedRange(bool __value);
        /** \copydoc rangeCenter */
        void setRangeCenter(double __value);
        /** \copydoc plotCenterLine */
        void setPlotCenterLine(bool __value);
        /** \copydoc unlimitedSizeMax */
        void setUnlimitedSizeMax(bool __value);
        /** \copydoc unlimitedSizeMin */
        void setUnlimitedSizeMin(bool __value);
        /** \copydoc sizeMax
         *
         *  \note this also sets unlimitedSizeMax to \c true
         *  \see unlimitedSizeMax
         */
        void setSizeMax(double __value);
        /** \copydoc sizeMin
         *
         *  \note this also sets unlimitedSizeMin to \c true
         *  \see unlimitedSizeMin
         */
        void setSizeMin(double __value);
        /** \copydoc rangeMax*/
        void setRangeMax(double __value);
        /** \copydoc rangeMin*/
        void setRangeMin(double __value);
    protected:
        /** \brief min-value of range
         *  \image html JKQTPRangeBase.png
         */
        double rangeMin;
        /** \brief max-value of range
         *  \image html JKQTPRangeBase.png
         */
        double rangeMax;
        /** \brief where to plot the center line
         *  \image html JKQTPRangeBase.png
         */
        double rangeCenter;
        /** \brief if \c unlimitedSizeMin==false, then the range rectangle only extends to this value on the axis perpendicular to the range axis (towards smaller numbers)
         *
         *  \image html JKQTPRangeBase.png
         *  \see unlimitedSizeMin
         */
        double sizeMin;
        /** \brief if \c unlimitedSizeMax==false, then the range rectangle only extends to this value on the axis perpendicular to the range axis (towards larger numbers)
         *
         *  \image html JKQTPRangeBase.png
         *  \see unlimitedSizeMax
         */
        double sizeMax;
        /** \brief if \c true, the range elongates infinitely in the direction perpendicular to the range axis and towards smaller numbers, otherwise the range rectangle stops at rangeMin
         *
         *  \image html JKQTPRangeBase.png
         *  \see sizeMin
         */
        bool unlimitedSizeMin;
        /** \brief if \c true, the range elongates infinitely in the direction perpendicular to the range axis and towards larger numbers, otherwise the range rectangle stops at rangeMin
         *
         *  \image html JKQTPRangeBase.png
         *  \see sizeMax
         */
        bool unlimitedSizeMax;
        /** \brief indicates whether to plot a center line */
        bool plotCenterLine;
        /** \brief if \c true, fills the parts outside the given range
         *
         *  \image html JKQTPRangeBase_Inverted.png
         */
        bool invertedRange;
        /** \brief if \c true, the range is plotted, if \c false, only the center line is plotted (if \a plotCenterLine is \c true ) */
        bool plotRange;
        /** \brief if \c true, draws lines at the range borders \a color */
        bool plotRangeLines;
        /** \brief if \c true, this fills the range with \a fillColor */
        bool fillRange;
        /** \brief color of the center line graph */
        QColor centerColor;
        /** \brief linestyle of the center line graph lines */
        Qt::PenStyle centerStyle;
        /** \brief width (pt) of the center line graph, given in pt */
        double centerLineWidth;

};





/** \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
 *         or a range of interest, or the range of good values, ...
 *  \ingroup jkqtplotter_annotations
 *
 *  \image html JKQTPHorizontalRange.png
 *
 *  \see JKQTPVerticalRange
 *
 *  <b>Explanation of basic features:</b>
 *
 *  \copydetails JKQTPRangeBase
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPHorizontalRange: public JKQTPRangeBase {
    Q_OBJECT
public:
    /** \brief class constructor */
    JKQTPHorizontalRange(JKQTBasePlotter* parent=nullptr);
    /** \brief class constructor */
    JKQTPHorizontalRange(JKQTPlotter* parent);

    /** \brief plots the graph to the plotter object specified as parent */
    virtual void draw(JKQTPEnhancedPainter& painter) override;
    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;

    /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
    bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
    /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
    bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;
};




/** \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
 *         or a range of interest, or the range of good values, ...
 *  \ingroup jkqtplotter_annotations
 *
 *  \image html JKQTPVerticalRange.png
 *
 *  \see JKQTPHorizontalRange
 *
 *  <b>Explanation of basic features (using the example of JKQTPHorizontalRange):</b>
 *
 *  \copydetails JKQTPRangeBase
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPVerticalRange: public JKQTPRangeBase {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPVerticalRange(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPVerticalRange(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;
};

#endif // jkqtpgraphsrange_H
