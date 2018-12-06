/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

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



/**
 * \defgroup jkqtplotter_elements Plot Elements
 * \ingroup jkqtplotter
 * \defgroup jkqtplotter_plots Graphs
 * \ingroup jkqtplotter_elements
 */

/** \file jkqtpelements.h
  * \ingroup jkqtplotter
  */

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpbasegraphs.h"

#ifndef JKQTPELEMENTS_H
#define JKQTPELEMENTS_H

// forward declarations
class JKQtBasePlotter;
class JKQtPlotter;
class JKQTPcoordinateAxis;
class JKQTPdatastore;
//class JKQTPColorPaletteTools;


/*! \brief interprets data as a stream of x- or y-positions \f$ p_i \f$ (depending in \a yPeaks ) ans plots a line on the height
           \a baseline and upright lines from baseline to baseline+peakHeight at each position \f$ p_i \f$. This can be used to
           display a stream of photons with given arrivaltimes \f$ p_i \f$.
    \ingroup jkqtplotter_plots

    \image html JKQTPPeakStreamGraphY.png "yPeaks=true"
    \image html JKQTPPeakStreamGraphX.png "yPeaks=false"
 */
class LIB_EXPORT JKQTPPeakStreamGraph: public JKQTPsingleColumnGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPPeakStreamGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQtBasePlotter* parent=nullptr);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQtBasePlotter* parent=nullptr);
         JKQTPPeakStreamGraph(JKQtPlotter* parent);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQtPlotter* parent);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQtPlotter* parent);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        JKQTPGET_SET_MACRO(double, baseline)
        JKQTPGET_SET_MACRO(double, peakHeight)
        JKQTPGET_SET_MACRO(bool, yPeaks)
        JKQTPGET_SET_MACRO(bool, drawBaseline)

    protected:

        /** \brief position of the baseline */
        double baseline;
        /** \brief height of each peak */
        double peakHeight;
        /** \brief if set \c true the peaks are in Y direction (default: true)*/
        bool yPeaks;
        /** \brief indicates whether to draw the basleine (default: \c true ) */
        bool drawBaseline;

};







/*! \brief This implements xy line plots. This also alows to draw symbols at the data points.
    \ingroup jkqtplotter_plots

    \image html plot_lineplots.png
 */
class LIB_EXPORT JKQTPxyLineGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPxyLineGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxyLineGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(JKQTPgraphSymbols, symbol)
        JKQTPGET_SET_MACRO(double, symbolWidth)
        JKQTPGET_SET_MACRO(double, symbolSize)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO(bool, drawSelectionLine)
        JKQTPGET_SET_MACRO(QColor, selectionLineColor)

    protected:

        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;
        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief which symbol to use for the datapoints */
        JKQTPgraphSymbols symbol;
        /** \brief size (diameter in pixels) of the symbol for the data points */
        double symbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the data points */
        double symbolWidth;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;


        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
        QPen getSymbolPen(JKQTPEnhancedPainter& painter) const;
        QBrush getBrush(JKQTPEnhancedPainter &painter) const;



};









/*! \brief This implements xy scatter plots (like JKQTPxyLineGraph), but the color and size of the symbols may be taken from a column.
    \ingroup jkqtplotter_plots

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.
 */
class LIB_EXPORT JKQTPxyParametrizedScatterGraph: public JKQTPxyLineGraph, public JKQTPColorPaletteTools {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPxyParametrizedScatterGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxyParametrizedScatterGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(int, sizeColumn)
        JKQTPSET_CAST_MACRO(size_t, int, sizeColumn)
        JKQTPGET_SET_MACRO(int, colorColumn)
        JKQTPSET_CAST_MACRO(size_t, int, colorColumn)
        JKQTPGET_SET_MACRO(int, symbolColumn)
        JKQTPSET_CAST_MACRO(size_t, int, symbolColumn)
        JKQTPGET_SET_MACRO(bool, colorColumnContainsRGB)
        JKQTPGET_SET_MACRO(bool, gridModeForSymbolSize)
        JKQTPGET_SET_MACRO(double, gridDeltaX)
        JKQTPGET_SET_MACRO(double, gridDeltaY)
        JKQTPGET_SET_MACRO(double, gridSymbolFractionSize)

        virtual void setParent(JKQtBasePlotter* parent);


        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);


        /** \brief determine min/max data value of the image */
        virtual void cbGetDataMinMax(double& imin, double& imax);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);


        virtual double getInternalDataMin() const ;
        virtual double getInternalDataMax() const ;
    protected:
        /** \brief this column contains the symbol size in pt */
        int sizeColumn;
        /** \brief this column contains the symbol color */
        int colorColumn;
        /** \brief this column contains the symbol type */
        int symbolColumn;

        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        bool gridModeForSymbolSize;
        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridDeltaX;
        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridDeltaY;
        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridSymbolFractionSize;

        /** \brief if this is true, the value in the colorColumn is converted to an integer, representing a color in ARGB format (as in QRgb) */
        bool colorColumnContainsRGB;

        double getLocalSymbolSize(int i);
        QColor getLocalColor(int i);
        JKQTPgraphSymbols getLocalSymbolType(int i);


        /** \brief internally used to store the range of the color column */
        double intColMin;
        /** \brief internally used to store the range of the color column */
        double intColMax;

};




/*! \brief This implements xy line plots with x and y error indicators.
    \ingroup jkqtplotter_plots

    \image html plot_errorbarlineplots.png
    \image html plot_errorlinelineplots.png
    \image html plot_errorpolygonlineplots.png
 */
class LIB_EXPORT JKQTPxyLineErrorGraph: public JKQTPxyLineGraph, public JKQTPxyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPxyLineErrorGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPxyLineGraph(parent), JKQTPxyGraphErrors()
        { setErrorColorFromGraphColor(color); }
        /** \brief class constructor */
        inline JKQTPxyLineErrorGraph(JKQtPlotter* parent):
            JKQTPxyLineGraph(parent), JKQTPxyGraphErrors()
        { setErrorColorFromGraphColor(color);  }

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter) ;
};



/*! \brief This implements xy scatter plots (like JKQTPxyLineGraph), but the color and size of the symbols may be taken from a column. with errorbars
    \ingroup jkqtplotter_plots

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.
 */
class LIB_EXPORT JKQTPxyParametrizedErrorScatterGraph: public JKQTPxyParametrizedScatterGraph, public JKQTPxyGraphErrors {
        Q_OBJECT
    public:
        inline JKQTPxyParametrizedErrorScatterGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPxyParametrizedScatterGraph(parent), JKQTPxyGraphErrors()
        {  setErrorColorFromGraphColor(color); }
        inline JKQTPxyParametrizedErrorScatterGraph(JKQtPlotter* parent):
            JKQTPxyParametrizedScatterGraph(parent), JKQTPxyGraphErrors()
        { setErrorColorFromGraphColor(color);  }


        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter) ;

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, long long index, QColor& errorColor, QColor& errorFillColor);

};



/*! \brief This implements a step plot with values \f$ \left(x, f(x) \right) \f$
    \ingroup jkqtplotter_plots

    A step plot starts at \f$ \left(x_{i-1}, f(x_{i-1})\right) \f$ and then goes on to
    \f$ \left(x_{i}, f(x_{i-1})\right) \f$. There it raises immediately to
    \f$ \left(x_i, f(x_i)\right) \f$.

    If you want the \f$ x_i \f$ values in the center of the steps, use
    \code JKQTPstepHorizontalGraph::set_xCentered(true) \endcode . In that case the steps
    go from \f$ \left(x_{i-1}, f(x_{i-1})\right) \f$ to \f$ \left(x_{i}-\delta/2, f(x_{i-1})\right) \f$ ,
    then to \f$ \left(x_{i}-\delta/2, f(x_{i})\right) \f$ and finally
    to \f$ \left(x_{i}, f(x_{i})\right) \f$ where \f$ \delta=(x_{i}-x_{i-1}) \f$ .

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_valuesCentered(false) \endcode :
    \image html plot_stephorplot1.png

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_valuesCentered(true) \endcode :
    \image html plot_stephorplot2.png
 */
class LIB_EXPORT JKQTPstepHorizontalGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPstepHorizontalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPstepHorizontalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO(bool, fillCurve)
        JKQTPGET_SET_MACRO(JKQTPstepType, stepType)
    protected:
        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief fill style, if the curve should be filled */
        Qt::BrushStyle fillStyle;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief indicates whether to fill the space between the curve and the x-axis */
        bool fillCurve;
        /** \brief specifies whether the steps elongate to the left, the right, or are centered around the \f$ x_i \f$ values. */
        JKQTPstepType stepType;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
};



/*! \brief This implements a step plot with values \f$ \left(f(y), y \right) \f$
    \ingroup jkqtplotter_plots

    A step plot starts at \f$ \left(f(y_{i-1}), x_{i-1}\right) \f$ and then goes on to
    \f$ \left(f(y_{i-1}), y_{i}\right) \f$. There it raises immediately to
    \f$ \left(f(y_i), y_i\right) \f$.

    If you want the \f$ y_i \f$ values in the center of the steps, use
    \code JKQTPstepHorizontalGraph::set_valuesCentered(true) \endcode . In that case the steps
    go from \f$ \left(f(y_{i-1}), y_{i-1})\right) \f$ to \f$ \left(f(y_{i-1}), y_{i}-\delta/2\right) \f$ ,
    then to \f$ \left(f(y_{i}, y_{i}-\delta/2\right) \f$ and finally
    to \f$ \left(f(y_{i}), y_{i}\right) \f$ where \f$ \delta=(y_{i}-y_{i-1}) \f$ .

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_xCentered(false) \endcode :
    \image html plot_stepverplot1.png

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_xCentered(true) \endcode :
    \image html plot_stepverplot2.png
 */
class LIB_EXPORT JKQTPstepVerticalGraph: public JKQTPstepHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPstepVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPstepVerticalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
};









/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_plots

 */
class LIB_EXPORT JKQTPhorizontalRange: public JKQTPgraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPhorizontalRange(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPhorizontalRange(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        void setDrawCenterLineOnly();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(QColor, centerColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, centerStyle)
        JKQTPGET_SET_MACRO(double, centerLineWidth)
        JKQTPGET_SET_MACRO(double, rangeMin)
        JKQTPGET_SET_MACRO(double, rangeMax)
        JKQTPGET_SET_MACRO(double, sizeMin)
        JKQTPGET_SET_MACRO(double, sizeMax)
        JKQTPGET_SET_MACRO(bool, unlimitedSizeMin)
        JKQTPGET_SET_MACRO(bool, unlimitedSizeMax)

        JKQTPGET_SET_MACRO(double, rangeCenter)
        JKQTPGET_SET_MACRO(bool, plotCenterLine)
        JKQTPGET_SET_MACRO(bool, invertedRange)
        JKQTPGET_SET_MACRO(bool, plotRange)
        JKQTPGET_SET_MACRO(bool, fillRange)
        JKQTPGET_SET_MACRO(bool, plotRangeLines)
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
        /** \brief if \c true, this fills the range with \a fillCOlor */
        bool fillRange;
        /** \brief color of the graph */
        QColor color;
        /** \brief color of the center line graph */
        QColor centerColor;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief linestyle of the center line graph lines */
        Qt::PenStyle centerStyle;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width (pixels) of the center line graph */
        double centerLineWidth;
        /** \brief fill style, if the curve should be filled */
        Qt::BrushStyle fillStyle;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;

};






/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_plots

 */
class LIB_EXPORT JKQTPverticalRange: public JKQTPhorizontalRange {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPverticalRange(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPverticalRange(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
};

#endif // JKQTPELEMENTS_H
