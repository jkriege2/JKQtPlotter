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

/** \file jkqtpgraphs.h
  * \ingroup jkqtplotter
  */

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"

#ifndef jkqtpgraphsboxplot_H
#define jkqtpgraphsboxplot_H




/*! \brief This implements vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplotvertical.png
 */
class LIB_EXPORT JKQTPboxplotVerticalGraph: public JKQTPgraph {
        Q_OBJECT
    public:

        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };



        /** \brief class constructor */
        JKQTPboxplotVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPboxplotVerticalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();


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

        JKQTPGET_SET_MACRO(DataSortOrder, sortData)
        /** \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <CENTER>\copybrief sortData.</CENTER> \see sortData for more information */
        inline void set_sortData(int __value) {
            sortData=(DataSortOrder)__value;
            if (__value>0) sortData=Sorted;
        }
        JKQTPGET_SET_MACRO(int, posColumn)
        JKQTPSET_CAST_MACRO(size_t, int, posColumn)
        JKQTPGET_SET_MACRO(int, medianColumn)
        JKQTPSET_CAST_MACRO(size_t, int, medianColumn)
        JKQTPGET_SET_MACRO(int, meanColumn)
        JKQTPSET_CAST_MACRO(size_t, int, meanColumn)
        JKQTPGET_SET_MACRO(int, minColumn)
        JKQTPSET_CAST_MACRO(size_t, int, minColumn)
        JKQTPGET_SET_MACRO(int, maxColumn)
        JKQTPSET_CAST_MACRO(size_t, int, maxColumn)
        JKQTPGET_SET_MACRO(int, percentile25Column)
        JKQTPSET_CAST_MACRO(size_t, int, percentile25Column)
        JKQTPGET_SET_MACRO(int, percentile75Column)
        JKQTPSET_CAST_MACRO(size_t, int, percentile75Column)
        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, whiskerStyle)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, boxWidth)

        JKQTPGET_SET_MACRO(JKQTPgraphSymbols, meanSymbol)
        JKQTPGET_SET_MACRO(double, meanSymbolWidth)
        JKQTPGET_SET_MACRO(double, meanSymbolSize)

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief the column that contains the x-component of the datapoints */
        int posColumn;
        /** \brief the column that contains the median-component of the datapoints */
        int medianColumn;
        /** \brief the column that contains the median-component of the datapoints. \note This column is strictly optional. */
        int meanColumn;
        /** \brief the column that contains the minimum-component of the datapoints */
        int minColumn;
        /** \brief the column that contains the maximum-component of the datapoints */
        int maxColumn;
        /** \brief the column that contains the 25% percentile-component of the datapoints */
        int percentile25Column;
        /** \brief the column that contains the 75% percentile-component of the datapoints */
        int percentile75Column;
        /** \brief color of the lines */
        QColor color;
        /** \brief color of the box fill */
        QColor fillColor;
        /** \brief fill style for the box */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the whisker lines */
        Qt::PenStyle whiskerStyle;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width of box in percent of distance between the current two posColumn values
         *         if we only plot one box&whiskers then this is the width in plot coordinates */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPgraphSymbols meanSymbol;
        /** \brief size (diameter in pixels) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the mean */
        double meanSymbolWidth;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;
        virtual void intSortData();
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }

};


/*! \brief This implements horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplothorizontal.png
 */
class LIB_EXPORT JKQTPboxplotHorizontalGraph: public JKQTPboxplotVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPboxplotHorizontalGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPboxplotVerticalGraph(parent)
        {
        }
        inline JKQTPboxplotHorizontalGraph(JKQtPlotter* parent):
            JKQTPboxplotVerticalGraph(parent)
        {
        }

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








/*! \brief This implements a vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPboxplotVerticalGraph
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplotvertical.png
 */
class LIB_EXPORT JKQTPboxplotVerticalElement: public JKQTPgraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPboxplotVerticalElement(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPboxplotVerticalElement(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();


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


        JKQTPGET_SET_MACRO(double, pos)
        JKQTPGET_SET_MACRO_I(double, median, drawMedian=true)
        JKQTPGET_SET_MACRO_I(double, mean, drawMean=true)
        JKQTPGET_SET_MACRO_I(double, min, drawMinMax=true)
        JKQTPGET_SET_MACRO_I(double, max, drawMinMax=true)
        JKQTPGET_SET_MACRO(double, percentile25)
        JKQTPGET_SET_MACRO(double, percentile75)
        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, whiskerStyle)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, boxWidth)

        JKQTPGET_SET_MACRO(JKQTPgraphSymbols, meanSymbol)
        JKQTPGET_SET_MACRO(double, meanSymbolWidth)
        JKQTPGET_SET_MACRO(double, meanSymbolSize)
        JKQTPGET_SET_MACRO(bool, drawMean)
        JKQTPGET_SET_MACRO(bool, drawMedian)
        JKQTPGET_SET_MACRO(bool, drawMinMax)

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief the column that contains the x-component of the datapoints */
        double pos;
        /** \brief the column that contains the median-component of the datapoints */
        double median;
        /** \brief the column that contains the median-component of the datapoints. \note This column is strictly optional. */
        double mean;
        /** \brief indicates whether to draw the mean */
        bool drawMean;
        /** \brief indicates whether to draw the median */
        bool drawMedian;
        /** \brief indicates whether to draw the percentiles */
        bool drawMinMax;
        /** \brief the column that contains the minimum-component of the datapoints */
        double min;
        /** \brief the column that contains the maximum-component of the datapoints */
        double max;
        /** \brief the column that contains the 25% percentile-component of the datapoints */
        double percentile25;
        /** \brief the column that contains the 75% percentile-component of the datapoints */
        double percentile75;
        /** \brief color of the lines */
        QColor color;
        /** \brief color of the box fill */
        QColor fillColor;
        /** \brief fill style for the box */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the whisker lines */
        Qt::PenStyle whiskerStyle;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width of box in plot coordinates */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPgraphSymbols meanSymbol;
        /** \brief size (diameter in pixels) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the mean */
        double meanSymbolWidth;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;
};


/*! \brief This implements a horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPboxplotVerticalGraph
    \ingroup jkqtplotter_plots

    the x position is given in pos. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplothorizontal.png
 */
class LIB_EXPORT JKQTPboxplotHorizontalElement: public JKQTPboxplotVerticalElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPboxplotHorizontalElement(JKQtBasePlotter* parent=nullptr):
            JKQTPboxplotVerticalElement(parent)
        {
        }
        /** \brief class constructor */
        inline JKQTPboxplotHorizontalElement(JKQtPlotter* parent):
            JKQTPboxplotVerticalElement(parent)
        {
        }

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



#endif // jkqtpgraphsboxplot_H
