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
#include "jkqtptools.h"
#include "jkqtp_imexport.h"
#include "jkqtpimagetools.h"
#include "jkqtpbasegraphs.h"

#ifndef JKQTPBARCHARTELEMENTS_H
#define JKQTPBARCHARTELEMENTS_H



/*! \brief This implements a bar graph with bars starting at \f$ y=0 \f$ to \f$ y=f(x) \f$
    \ingroup jkqtplotter_plots

    This class plots a bargraph. This image explains the parameters:

    \image html bargraph_basics.png

    By default the sift parameter is, so the bar is centered at the x-value. The width is 0.9,
    so adjacent bars are plotted with a small distance between them. It is possible to use these two parameters
    to plot multiple bars for every x-value, by having on JKQTPstepHorizontalGraph object per
    set of bars that belong together. For example for three bars per x-value one would set:
    \verbatim
          width=0.3
          shift=-0.3 / 0 / +0.3
    \endverbatim
    This results in a bargraph, as shown here:

    \image html plot_bargraphverplot.png


    You can use JKQTPlotter::addHorizontalBargraph() to add a series of bargraphs, where the width and shift are determined
    automatically. The y-columns are given as a QVector<int> to this function.


 */
class LIB_EXPORT JKQTPbarVerticalGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPbarVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPbarVerticalGraph(JKQtPlotter* parent);

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

        /** \brief finds all bar charts of the same orientation and determines width and shift, so they stand side by side
         *
         *  \param maxWidth the maximum (relative) width, that all bars will span of the (doubled) inter-bar distance
         *  \param shrinkFactor factor, by which the bar are shrinked compared to the available space
         *
         *  \note This function will scale ALL graphs of the parent plot, which were derived from JKQTPbarHorizontalGraph, that match in orientation (as returned by isHorizontal() ).
         */
        virtual void autoscaleBarWidthAndShift(double maxWidth=0.9, double shrinkFactor=0.8);
        void autoscaleBarWidthAndShiftSeparatedGroups(double groupWidth=0.75);

        virtual bool isHorizontal() const;

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, shift)
        JKQTPGET_SET_MACRO(double, width)
        JKQTPGET_SET_MACRO(double, baseline)
        /** \brief sets the fill color and the color together, where fillColor is set to \a fill and the line-color is set to \c fill.darker(colorDarker) */
        void set_fillColor_and_darkenedColor(QColor fill, int colorDarker=200);
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
        /** \brief the width of the bargraphs, relative to the distance between the current and the next x-value
         *
         * See the following graphic to understand this concept:
         *     \image html bargraph_basics.png
         */
        double width;
        /** \brief the shift of the bargraphs, relative to the distance between the current and the next x-value
         *
         * See the following graphic to understand this concept:
         *     \image html bargraph_basics.png
         */
        double shift;

        /** \brief baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!)
         */
        double baseline;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;


        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;

        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint
         *
         *  \note This function returns \a baseline in this implementation. It is implemented in the derived classes JKQTPbarVerticalStackableGraph
         *        and JKQTPbarHorizontalStackableGraph. The function is placed here, so the plotting does not have to be reimplemented in the
         *        derived classes that allow for stacking, but can be implemented once centrally.
         */
        virtual double getStackedMax(int index) const;

        /** \brief calls getStackedMax() on the stack parent (if available)
         *
         *  \note This function returns \c 0.0 in this implementation. It is implemented in the derived classes JKQTPbarVerticalStackableGraph
         *        and JKQTPbarHorizontalStackableGraph. The function is placed here, so the plotting does not have to be reimplemented in the
         *        derived classes that allow for stacking, but can be implemented once centrally.
         */
        virtual double getParentStackedMax(int index) const;

        /** \brief returns \c true, if a stack parent is set (if available)
         *
         *  \note This function returns \c false in this implementation. It is implemented in the derived classes JKQTPbarVerticalStackableGraph
         *        and JKQTPbarHorizontalStackableGraph. The function is placed here, so the plotting does not have to be reimplemented in the
         *        derived classes that allow for stacking, but can be implemented once centrally.
         */
        virtual bool hasStackParent() const;

};



/*! \brief This implements a bar graph with bars starting at \f$ y=0 \f$ to \f$ y=f(x) \f$
 *         Optionally several graphs of this type may be stacked on top of each other
 *  \ingroup jkqtplotter_plots
 *
 *  Draw stacked barcharts by connecting several plots by calling \c set_stackedParent(belowPlot) for each plot
 *  \image html JKQTPbarVerticalGraphStacked.png
 *
 */
class LIB_EXPORT JKQTPbarVerticalStackableGraph: public JKQTPbarVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPbarVerticalStackableGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPbarVerticalStackableGraph(JKQtPlotter* parent);
        /** \brief stacks this barchart upon the given \a parentGraph */
        void stackUpon(JKQTPbarVerticalStackableGraph* parentGraph);
        /** \brief unstacks this graph (i.e. deletes the parent graph in the stack) */
        void dontStackUpon();
        /** \brief returns the stack parent graph, or \c nullptr */
        const JKQTPbarVerticalStackableGraph* getStackParent() const;

    protected:

        /** \brief if set (!=nullptr), the current plot is drawn stacked onto this plot
         *
         *  draw stacked barcharts by connecting several plots by calling \c set_stackedParent(belowPlot) for each plot
         */
        JKQTPbarVerticalStackableGraph* stackParent;

        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint */
        virtual double getStackedMax(int index) const;
        /** \brief calls getStackedMax() on the stack parent (if available), or \c 0.0 */
        virtual double getParentStackedMax(int index) const;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const;
};

/*! \brief This implements a bar graph with bars starting at \f$ y=0 \f$ to \f$ y=f(x) \f$
 *         and error indicator
 *  \ingroup jkqtplotter_plots
 *
 *  This works much the same as JKQTPbarHorizontalGraph. Here is an example output:
 *  \image html plot_bargraphverploterr.png
 *
 */
class LIB_EXPORT JKQTPbarVerticalErrorGraph: public JKQTPbarVerticalGraph, public JKQTPyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPbarVerticalErrorGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPbarVerticalErrorGraph(JKQtPlotter* parent);

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) ;
};




/*! \brief This implements a bar graph with bars starting at \f$ x=0 \f$ to \f$ x=f(y) \f$
    \ingroup jkqtplotter_plots

    This works much the same as JKQTPbarHorizontalGraph. Here is an example output:
    \image html plot_bargraphhorplot.png

 */
class LIB_EXPORT JKQTPbarHorizontalGraph: public JKQTPbarVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPbarHorizontalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPbarHorizontalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

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

        virtual bool isHorizontal() const;

 };



/*! \brief This implements a bar graph with bars starting at \f$ y=0 \f$ to \f$ y=f(x) \f$
 *         Optionally several graphs of this type may be stacked on top of each other
 *  \ingroup jkqtplotter_plots
 *
 *  Draw stacked barcharts by connecting several plots by calling \c set_stackedParent(belowPlot) for each plot
 *  \image html JKQTPbarHorizontalGraphStacked.png
 *
 */
class LIB_EXPORT JKQTPbarHorizontalStackableGraph: public JKQTPbarHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPbarHorizontalStackableGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPbarHorizontalStackableGraph(JKQtPlotter* parent);
        /** \brief stacks this barchart upon the given \a parentGraph */
        void stackUpon(JKQTPbarHorizontalStackableGraph* parentGraph);
        /** \brief unstacks this graph (i.e. deletes the parent graph in the stack) */
        void dontStackUpon();
        /** \brief returns the stack parent graph, or \c nullptr */
        const JKQTPbarHorizontalStackableGraph* getStackParent() const;

    protected:

        /** \brief if set (!=nullptr), the current plot is drawn stacked onto this plot
         *
         *  draw stacked barcharts by connecting several plots by calling \c set_stackedParent(belowPlot) for each plot
         */
        JKQTPbarHorizontalStackableGraph* stackParent;

        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint */
        virtual double getStackedMax(int index) const;
        /** \brief calls getStackedMax() on the stack parent (if available), or \c 0.0 */
        virtual double getParentStackedMax(int index) const;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const;
};

/*! \brief This implements a bar graph with bars starting at \f$ x=0 \f$ to \f$ x=f(y) \f$
 *         and error indicator
 *  \ingroup jkqtplotter_plots
 *
 *  This works much the same as JKQTPbarHorizontalGraph. Here is an example output:
 *  \image html plot_bargraphhorploterr.png
 *
 */
class LIB_EXPORT JKQTPbarHorizontalErrorGraph: public JKQTPbarHorizontalGraph, public JKQTPxGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPbarHorizontalErrorGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPbarHorizontalErrorGraph(JKQtPlotter* parent);

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);
        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) ;
};







#endif // JKQTPBARCHARTELEMENTS_H
