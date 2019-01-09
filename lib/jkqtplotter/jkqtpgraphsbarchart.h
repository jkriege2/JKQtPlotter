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

#ifndef jkqtpgraphsbarchart_H
#define jkqtpgraphsbarchart_H



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
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

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

        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor to the specified \a __value. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual void set_fillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual QColor get_fillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle to the specified \a __value. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual void set_fillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle get_fillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property style to the specified \a __value. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual void set_style(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual Qt::PenStyle get_style() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual void set_lineWidth(double __value)  
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual double get_lineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property shift to the specified \a __value. 
            \details Description of the parameter shift is: <CENTER>\copybrief shift.</CENTER> 
            \see shift for more information */ 
        inline virtual void set_shift(double __value)  
        {
            this->shift = __value;
        } 
        /*! \brief returns the property shift. 
            \details Description of the parameter shift is: <CENTER>\copybrief shift.</CENTER> 
            \see shift for more information */ 
        inline virtual double get_shift() const  
        {
            return this->shift; 
        }
        /*! \brief sets the property width to the specified \a __value. 
            \details Description of the parameter width is: <CENTER>\copybrief width.</CENTER> 
            \see width for more information */ 
        inline virtual void set_width(double __value)  
        {
            this->width = __value;
        } 
        /*! \brief returns the property width. 
            \details Description of the parameter width is: <CENTER>\copybrief width.</CENTER> 
            \see width for more information */ 
        inline virtual double get_width() const  
        {
            return this->width; 
        }
        /*! \brief sets the property baseline to the specified \a __value. 
            \details Description of the parameter baseline is: <CENTER>\copybrief baseline.</CENTER> 
            \see baseline for more information */ 
        inline virtual void set_baseline(double __value)  
        {
            this->baseline = __value;
        } 
        /*! \brief returns the property baseline. 
            \details Description of the parameter baseline is: <CENTER>\copybrief baseline.</CENTER> 
            \see baseline for more information */ 
        inline virtual double get_baseline() const  
        {
            return this->baseline; 
        }
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
        virtual double getStackedMax(int index) const override;
        /** \brief calls getStackedMax() on the stack parent (if available), or \c 0.0 */
        virtual double getParentStackedMax(int index) const override;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const override;
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
        virtual bool usesColumn(int c) const override;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter)  override;
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
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        virtual bool isHorizontal() const override;

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
        virtual double getStackedMax(int index) const override;
        /** \brief calls getStackedMax() on the stack parent (if available), or \c 0.0 */
        virtual double getParentStackedMax(int index) const override;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const override;
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
        virtual bool usesColumn(int c) const override;
        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override;
};







#endif // jkqtpgraphsbarchart_H
