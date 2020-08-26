/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#ifndef jkqtpgraphsrange_H
#define jkqtpgraphsrange_H

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;
//class JKQTPColorPaletteStyleAndToolsMixin;







/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_diverse

    \image html JKQTPHorizontalRange.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPHorizontalRange: public JKQTPGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPHorizontalRange(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPHorizontalRange(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

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
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /** \brief disables all drawing features except the centerline */
        void setDrawCenterLineOnly();

        /*! \brief set the color of the graph (all lines and filling) */
        virtual void setColor(QColor c);

        /*! \copydoc centerColor */ 
        void setCenterColor(const QColor & __value);
        /*! \copydoc centerColor */ 
        QColor getCenterColor() const;
        /*! \copydoc centerStyle */ 
        void setCenterStyle(Qt::PenStyle  __value);
        /*! \copydoc centerStyle */ 
        Qt::PenStyle getCenterStyle() const;
        /*! \copydoc centerLineWidth */ 
        void setCenterLineWidth(double __value);
        /*! \copydoc centerLineWidth */ 
        double getCenterLineWidth() const;
        /*! \copydoc rangeMin */ 
        void setRangeMin(double __value);
        /*! \copydoc rangeMin */ 
        double getRangeMin() const;
        /*! \copydoc rangeMax */ 
        void setRangeMax(double __value);
        /*! \copydoc rangeMax */ 
        double getRangeMax() const;
        /*! \copydoc sizeMin */ 
        void setSizeMin(double __value);
        /*! \copydoc sizeMin */ 
        double getSizeMin() const;
        /*! \copydoc sizeMax */ 
        void setSizeMax(double __value);
        /*! \copydoc sizeMax */ 
        double getSizeMax() const;
        /*! \copydoc unlimitedSizeMin */ 
        void setUnlimitedSizeMin(bool __value);
        /*! \copydoc unlimitedSizeMin */ 
        bool getUnlimitedSizeMin() const;
        /*! \copydoc unlimitedSizeMax */ 
        void setUnlimitedSizeMax(bool __value);
        /*! \copydoc unlimitedSizeMax */ 
        bool getUnlimitedSizeMax() const;

        /*! \copydoc rangeCenter */ 
        void setRangeCenter(double __value);
        /*! \copydoc rangeCenter */ 
        double getRangeCenter() const;
        /*! \copydoc plotCenterLine */ 
        void setPlotCenterLine(bool __value);
        /*! \copydoc plotCenterLine */ 
        bool getPlotCenterLine() const;
        /*! \copydoc invertedRange */ 
        void setInvertedRange(bool __value);
        /*! \copydoc invertedRange */ 
        bool getInvertedRange() const;
        /*! \copydoc plotRange */ 
        void setPlotRange(bool __value);
        /*! \copydoc plotRange */ 
        bool getPlotRange() const;
        /*! \copydoc fillRange */ 
        void setFillRange(bool __value);
        /*! \copydoc fillRange */ 
        bool getFillRange() const;
        /*! \copydoc plotRangeLines */ 
        void setPlotRangeLines(bool __value);
        /*! \copydoc plotRangeLines */ 
        bool getPlotRangeLines() const;
    protected:
        /** \brief min-value of range */
        double rangeMin;
        /** \brief max-value of range */
        double rangeMax;
        /** \brief where to plot the center line */
        double rangeCenter;
        double sizeMin;
        double sizeMax;
        bool unlimitedSizeMin;
        bool unlimitedSizeMax;
        /** \brief indicates whether to plot a center line */
        bool plotCenterLine;
        /** \brief if \c true, fills the parts outside the given range */
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






/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_diverse

    \image html JKQTPVerticalRange.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPVerticalRange: public JKQTPHorizontalRange {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPVerticalRange(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPVerticalRange(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

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
