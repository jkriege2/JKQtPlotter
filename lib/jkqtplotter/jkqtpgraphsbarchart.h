/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include "jkqtcommon/jkqtptools.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#ifndef jkqtpgraphsbarchart_H
#define jkqtpgraphsbarchart_H



/*! \brief This implements a bar graph with bars starting at \f$ y=0 \f$ to \f$ y=f(x) \f$
    \ingroup jkqtplotter_barssticks

    This class plots a bargraph. This image explains the parameters:

    \image html bargraph_basics.png

    By default the sift parameter is, so the bar is centered at the x-value. The width is 0.9,
    so adjacent bars are plotted with a small distance between them. It is possible to use these two parameters
    to plot multiple bars for every x-value, by having on JKQTPSpecialLineHorizontalGraph object per
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
class JKQTP_LIB_EXPORT JKQTPBarVerticalGraph: public JKQTPXYGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarVerticalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

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
         *  \note This function will scale ALL graphs of the parent plot, which were derived from JKQTPBarHorizontalGraph, that match in orientation (as returned by isHorizontal() ).
         */
        virtual void autoscaleBarWidthAndShift(double maxWidth=0.9, double shrinkFactor=0.8);

        /** \brief equivalent to \c autoscaleBarWidthAndShift(groupWidth,1);
         */
        void autoscaleBarWidthAndShiftSeparatedGroups(double groupWidth=0.75);

        /** \brief retruns \c true, if the bars are horizontal (JKQTPBarHorizontalGraph) and \c false if they are vertical (JKQTPBarVerticalGraph) */
        virtual bool isHorizontal() const;

        /** \brief set outline and fill color at the same time
         *  \see setFillColor_and_darkenedColor()
         */
        virtual void setColor(QColor c);

        /*! \copydoc shift */ 
        void setShift(double __value);
        /*! \copydoc shift */ 
        double getShift() const;
        /*! \copydoc width */ 
        void setWidth(double __value);
        /*! \copydoc width */ 
        double getWidth() const;
        /*! \copydoc baseline */ 
        void setBaseline(double __value);
        /*! \copydoc baseline */ 
        double getBaseline() const;
        /** \brief sets the fill color and the color together, where fillColor is set to \a fill and the line-color is set to \c fill.darker(colorDarker)
         *  \see setColor()
         */
        void setFillColor_and_darkenedColor(QColor fill, int colorDarker=200);
    protected:
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


        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint
         *
         *  \note This function returns \a baseline in this implementation. It is implemented in the derived classes JKQTPBarVerticalStackableGraph
         *        and JKQTPBarHorizontalStackableGraph. The function is placed here, so the plotting does not have to be reimplemented in the
         *        derived classes that allow for stacking, but can be implemented once centrally.
         */
        virtual double getStackedMax(int index) const;

        /** \brief calls getStackedMax() on the stack parent (if available)
         *
         *  \note This function returns \c 0.0 in this implementation. It is implemented in the derived classes JKQTPBarVerticalStackableGraph
         *        and JKQTPBarHorizontalStackableGraph. The function is placed here, so the plotting does not have to be reimplemented in the
         *        derived classes that allow for stacking, but can be implemented once centrally.
         */
        virtual double getParentStackedMax(int index) const;

        /** \brief returns \c true, if a stack parent is set (if available)
         *
         *  \note This function returns \c false in this implementation. It is implemented in the derived classes JKQTPBarVerticalStackableGraph
         *        and JKQTPBarHorizontalStackableGraph. The function is placed here, so the plotting does not have to be reimplemented in the
         *        derived classes that allow for stacking, but can be implemented once centrally.
         */
        virtual bool hasStackParent() const;

};



/*! \brief This implements a bar graph with bars starting at \f$ y=0 \f$ to \f$ y=f(x) \f$
 *         Optionally several graphs of this type may be stacked on top of each other
 *  \ingroup jkqtplotter_barssticks
 *
 *  Draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
 *  \image html JKQTPBarVerticalGraphStacked.png
 *
 */
class JKQTP_LIB_EXPORT JKQTPBarVerticalStackableGraph: public JKQTPBarVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarVerticalStackableGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarVerticalStackableGraph(JKQTPlotter* parent);
        /** \brief stacks this barchart upon the given \a parentGraph */
        void stackUpon(JKQTPBarVerticalStackableGraph* parentGraph);
        /** \brief unstacks this graph (i.e. deletes the parent graph in the stack) */
        void dontStackUpon();
        /** \brief returns the stack parent graph, or \c nullptr */
        const JKQTPBarVerticalStackableGraph* getStackParent() const;

    protected:

        /** \brief if set (!=nullptr), the current plot is drawn stacked onto this plot
         *
         *  draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
         */
        JKQTPBarVerticalStackableGraph* stackParent;

        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint */
        virtual double getStackedMax(int index) const override;
        /** \brief calls getStackedMax() on the stack parent (if available), or \c 0.0 */
        virtual double getParentStackedMax(int index) const override;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const override;
};

/*! \brief This implements a bar graph with bars starting at \f$ y=0 \f$ to \f$ y=f(x) \f$
 *         and error indicator
 *  \ingroup jkqtplotter_barssticks
 *
 *  This works much the same as JKQTPBarHorizontalGraph. Here is an example output:
 *  \image html plot_bargraphverploterr.png
 *
 */
class JKQTP_LIB_EXPORT JKQTPBarVerticalErrorGraph: public JKQTPBarVerticalGraph, public JKQTPYGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarVerticalErrorGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarVerticalErrorGraph(JKQTPlotter* parent);

        /** \copydoc JKQTPGraph::usesColumn() */
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
    \ingroup jkqtplotter_barssticks

    This works much the same as JKQTPBarHorizontalGraph. Here is an example output:
    \image html plot_bargraphhorplot.png

 */
class JKQTP_LIB_EXPORT JKQTPBarHorizontalGraph: public JKQTPBarVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarHorizontalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarHorizontalGraph(JKQTPlotter* parent);

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
 *  \ingroup jkqtplotter_barssticks
 *
 *  Draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
 *  \image html JKQTPBarHorizontalGraphStacked.png
 *
 */
class JKQTP_LIB_EXPORT JKQTPBarHorizontalStackableGraph: public JKQTPBarHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarHorizontalStackableGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarHorizontalStackableGraph(JKQTPlotter* parent);
        /** \brief stacks this barchart upon the given \a parentGraph */
        void stackUpon(JKQTPBarHorizontalStackableGraph* parentGraph);
        /** \brief unstacks this graph (i.e. deletes the parent graph in the stack) */
        void dontStackUpon();
        /** \brief returns the stack parent graph, or \c nullptr */
        const JKQTPBarHorizontalStackableGraph* getStackParent() const;

    protected:

        /** \brief if set (!=nullptr), the current plot is drawn stacked onto this plot
         *
         *  draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
         */
        JKQTPBarHorizontalStackableGraph* stackParent;

        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint */
        virtual double getStackedMax(int index) const override;
        /** \brief calls getStackedMax() on the stack parent (if available), or \c 0.0 */
        virtual double getParentStackedMax(int index) const override;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const override;
};

/*! \brief This implements a bar graph with bars starting at \f$ x=0 \f$ to \f$ x=f(y) \f$
 *         and error indicator
 *  \ingroup jkqtplotter_barssticks
 *
 *  This works much the same as JKQTPBarHorizontalGraph. Here is an example output:
 *  \image html plot_bargraphhorploterr.png
 *
 */
class JKQTP_LIB_EXPORT JKQTPBarHorizontalErrorGraph: public JKQTPBarHorizontalGraph, public JKQTPXGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarHorizontalErrorGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarHorizontalErrorGraph(JKQTPlotter* parent);

        /** \copydoc JKQTPGraph::usesColumn() */
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
