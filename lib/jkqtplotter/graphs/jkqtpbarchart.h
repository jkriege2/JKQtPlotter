/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#ifndef jkqtpgraphsbarchart_H
#define jkqtpgraphsbarchart_H


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/graphs/jkqtpbarchartbase.h"


/*! \brief This implements a vertical bar graph with bars between \f$ y=\mbox{baseline} \f$ and \f$ y=f(x) \f$
    \ingroup jkqtplotter_barcharts

    This class plots a bargraph. This image explains the parameters:

    \image html bargraph_basics.png

    By default the shift parameter is, so the bar is centered at the x-value. The width is 0.9,
    so adjacent bars are plotted with a small distance between them. It is possible to use these two parameters
    to plot multiple bars for every x-value, by having on JKQTPSpecialLineHorizontalGraph object per
    set of bars that belong together. For example for three bars per x-value one would set:
    \verbatim
          width=0.3
          shift=-0.3 / 0 / +0.3
    \endverbatim
    This results in a bargraph, as shown here:

    \image html JKQTPBarVerticalGraph.png


    You can also set JKQTPBarGraphBase::FillMode::TwoColorFilling, which uses different fill styles for bars above and below
    the baseline of the graph:

    \image html JKQTPBarVerticalGraphTwoColorFilling.png

   If you use JKQTPBarGraphBase::FillMode::FunctorFilling you can specify the fill style by a functor, e.g.
   \code
     graph->setFillMode(JKQTPBarGraphBase::FillMode::FunctorFilling);
     graph->setFillBrushFunctor(
       [](double key, double value) {
         return QBrush(QColor::fromHsvF(key/12.0, 1.0, 1.0));
       }
     );
   \endcode

   The result may look like this:

   \image html JKQTPBarVerticalGraphFunctorFilling.png


   You can also completely customize the drawing by defining a custom draw functor:
   \code
     graph->setCustomDrawingFunctor(
       [](JKQTPEnhancedPainter& painter, const QRectF& bar_px, const QPointF& datapoint, Qt::Orientation orientation, JKQTPBarGraphBase* graph) {
         // draw the bar (if required), pen and brush are already set properly
         painter.drawRect(bar_px);
         // now we can add some decoration or replace the instruction above:
         // ........
       }
     );
     // enable usage of cutom draw functor
     graph->setUseCustomDrawFunctor(true);
   \endcode

   See \ref JKQTPlotterBarchartsCustomDrawFunctor for a detailed example.
   The result may look like this:

   \image html JKQTPBarVerticalGraphCustomDrawFunctor.png

   \see JKQTPBarHorizontalGraph, \ref JKQTPlotterBarcharts, jkqtpstatAddHHistogram1D(), jkqtpstatAddHHistogram1DAutoranged()
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarVerticalGraph: public JKQTPBarGraphBase {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarVerticalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

    public Q_SLOTS:

    protected:
	
        /** \brief this function is used by autoscaleBarWidthAndShift() to determine whether a given graph shall be taken into account when autoscaling. 
		 *         Typically this returns \c true for all JKQTPBarGraphBase-derved objects with the same orientation (horizontal or vertical) */
        virtual bool considerForAutoscaling( JKQTPBarGraphBase* other) const override;

};

/*! \brief This implements a vertical bar graph with bars between \f$ y=\mbox{baseline} \f$ and \f$ y=f(x) \f$
 *         and error indicator
 *  \ingroup jkqtplotter_barcharts
 *
 *  This works much the same as JKQTPBarHorizontalGraph. Here is an example output:
 *  \image html JKQTPBarVerticalErrorGraph.png
 *
 * \see jkqtpstatAddYErrorBarGraph(), JKQTPBarVerticalGraph, \ref JKQTPlotterBarcharts
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarVerticalErrorGraph: public JKQTPBarVerticalGraph, public JKQTPYGraphErrors {
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

        /** \brief returns the column that contains the bar height errors */
        int getBarErrorColumn() const;
        /** \brief returns the column that contains the lower bar height errors */
        int getBarLowerErrorColumn() const;
        /** \brief returns the error style of the bar */
        JKQTPErrorPlotstyle getBarErrorStyle() const;
        /** \brief returns whether the errors of the bars are symmetric */
        bool getBarErrorSymmetric() const;

    public Q_SLOTS:
        /** \brief sets whether the errors of the bars are symmetric */
        void setBarErrorSymmetric(bool __value);
        /** \brief sets the error style of the bar */
        void setBarErrorStyle(JKQTPErrorPlotstyle  __value);
        /** \brief sets the column that contains the bar height errors */
        void setBarErrorColumn(int column) ;

        /** \brief sets the column that contains the bar height errors */
        void setBarErrorColumn(size_t column) ;
        /** \brief sets the column that contains the bar height errors */
        void setBarLowerErrorColumn(int column) ;

        /** \brief sets the column that contains the bar height errors */
        void setBarLowerErrorColumn(size_t column) ;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter)  override;
};





/*! \brief This implements a horizontal bar graph with bars between \f$ x=\mbox{baseline} \f$ and \f$ x=f(y) \f$
    \ingroup jkqtplotter_barcharts

    This works much the same as JKQTPBarHorizontalGraph. Here is an example output:

    \image html JKQTPBarHorizontalGraph.png

    You can also set JKQTPBarGraphBase::FillMode::TwoColorFilling, which uses different fill styles for bars above and below
    the baseline of the graph:

    \image html JKQTPBarHorizontalGraphTwoColorFilling.png

   If you use JKQTPBarGraphBase::FillMode::FunctorFilling you can specify the fill style by a functor, e.g.
   \code
     graph->setFillMode(JKQTPBarGraphBase::FillMode::FunctorFilling);
     graph->setFillBrushFunctor(
       [](double key, double value) {
         return QBrush(QColor::fromHsvF(key/12.0, 1.0, 1.0));
       }
     );
   \endcode

   The result may look like this:

   \image html JKQTPBarHorizontalGraphFunctorFilling.png

   You can also completely customize the drawing by defining a custom draw functor:
   \code
     graph->setCustomDrawingFunctor(
       [](JKQTPEnhancedPainter& painter, const QRectF& bar_px, const QPointF& datapoint, Qt::Orientation orientation, JKQTPBarGraphBase* graph) {
         // draw the bar (if required), pen and brush are already set properly
         painter.drawRect(bar_px);
         // now we can add some decoration or replace the instruction above:
         // ........
       }
     );
     // enable usage of cutom draw functor
     graph->setUseCustomDrawFunctor(true);
   \endcode

   See \ref JKQTPlotterBarchartsCustomDrawFunctor for a detailed example.
   The result may look like this:

   \image html JKQTPBarHorizontalGraphCustomDrawFunctor.png

    \see \ref JKQTPlotterBarcharts, jkqtpstatAddVHistogram1D(), jkqtpstatAddVHistogram1DAutoranged()
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarHorizontalGraph: public JKQTPBarGraphBase {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarHorizontalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarHorizontalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;
        /** \brief returns the column used as "key" for the current graph (typically this call getXColumn(), but for horizontal graphs like filled curves or barcharts it may call getYColumn() ) */
        virtual int getKeyColumn() const override;
        /** \brief returns the column used as "value" for the current graph (typically this call getXColumn(), but for horizontal graphs like filled curves or barcharts it may call getYColumn() ) */
        virtual int getValueColumn() const override;
    public Q_SLOTS:
        /** \brief sets the column used as "key" for the current graph (typically this call setXColumn(), but for horizontal graphs like filled curves or barcharts it may call setYColumn() ) */
        virtual void setKeyColumn(int __value) override;
        /** \brief sets the column used as "value" for the current graph (typically this call setXColumn(), but for horizontal graphs like filled curves or barcharts it may call setYColumn() ) */
        virtual void setValueColumn(int __value) override;
    protected:

        /** \brief this function is used by autoscaleBarWidthAndShift() to determine whether a given graph shall be taken into account when autoscaling. 
		 *         Typically this returns \c true for all JKQTPBarGraphBase-derved objects with the same orientation (horizontal or vertical) */
        virtual bool considerForAutoscaling( JKQTPBarGraphBase* other) const override;
 };



/*! \brief This implements a horizontal bar graph with bars between \f$ x=\mbox{baseline} \f$ and \f$ x=f(y) \f$
 *         and error indicator
 *  \ingroup jkqtplotter_barcharts
 *
 *  This works much the same as JKQTPBarHorizontalGraph. Here is an example output:
 *  \image html JKQTPBarHorizontalErrorGraph.png
 *
 * \see jkqtpstatAddXErrorBarGraph(), JKQTPBarHorizontalGraph, \ref JKQTPlotterBarcharts
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarHorizontalErrorGraph: public JKQTPBarHorizontalGraph, public JKQTPXGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBarHorizontalErrorGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarHorizontalErrorGraph(JKQTPlotter* parent);

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;
        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;


        /** \brief returns the column that contains the bar height errors */
        int getBarErrorColumn() const;
        /** \brief returns the column that contains the lower bar height errors */
        int getBarLowerErrorColumn() const;
        /** \brief returns the error style of the bar */
        JKQTPErrorPlotstyle getBarErrorStyle() const;
        /** \brief returns whether the errors of the bars are symmetric */
        bool getBarErrorSymmetric() const;

    public Q_SLOTS:
        /** \brief sets whether the errors of the bars are symmetric */
        void setBarErrorSymmetric(bool __value);
        /** \brief sets the error style of the bar */
        void setBarErrorStyle(JKQTPErrorPlotstyle  __value);
        /** \brief sets the column that contains the bar height errors */
        void setBarErrorColumn(int column) ;

        /** \brief sets the column that contains the bar height errors */
        void setBarErrorColumn(size_t column) ;
        /** \brief sets the column that contains the bar height errors */
        void setBarLowerErrorColumn(int column) ;

        /** \brief sets the column that contains the bar height errors */
        void setBarLowerErrorColumn(size_t column) ;
    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override;
};














/*! \brief This implements a vertical bar graph with bars between \f$ y=\mbox{baseline} \f$ and \f$ y=f(x) \f$
 *         Optionally several graphs of this type may be stacked on top of each other
 *  \ingroup jkqtplotter_barcharts
 *
 *  Draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
 *
 *  \image html JKQTPBarVerticalGraphStacked.png
 *
 * \see JKQTPBarVerticalGraph, \ref JKQTPlotterStackedBarChart
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarVerticalStackableGraph: public JKQTPBarVerticalGraph {
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
        /** \brief returns the stack parent graph, or \c nullptr */
        JKQTPBarVerticalStackableGraph* getStackParent();

    protected:

        /** \brief if set (!=nullptr), the current plot is drawn stacked onto this plot
         *
         *  draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
         */
        JKQTPBarVerticalStackableGraph* stackParent;

        /** \brief used to generate stacked plots: returns the upper boundary of the parent plot in a stack, for the index-th datapoint */
        double getParentStackedMax(int index) const ;


        /** \brief returns \c true, if a stack parent is set (if available) */
        bool hasStackParent() const ;
        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint */
        double getStackedMax(int index) const;
};






/*! \brief This implements a horizontal bar graph with bars between \f$ x=\mbox{baseline} \f$ and \f$ x=f(y) \f$
 *         Optionally several graphs of this type may be stacked on top of each other
 *  \ingroup jkqtplotter_barcharts
 *
 *  Draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
 *
 *  \image html JKQTPBarHorizontalGraphStacked.png
 *
 *
 * \see JKQTPBarHorizontalGraph, \ref JKQTPlotterStackedBarChart
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarHorizontalStackableGraph: public JKQTPBarHorizontalGraph {
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
        /** \brief returns the stack parent graph, or \c nullptr */
        JKQTPBarHorizontalStackableGraph* getStackParent() ;

    protected:

        /** \brief if set (!=nullptr), the current plot is drawn stacked onto this plot
         *
         *  draw stacked barcharts by connecting several plots by calling \c setStackedParent(belowPlot) for each plot
         */
        JKQTPBarHorizontalStackableGraph* stackParent;

        /** \brief used to generate stacked plots: returns the upper boundary of the parent plot in a stack, for the index-th datapoint */
        virtual double getParentStackedMax(int index) const override;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const override;
        /** \brief used to generate stacked plots: returns the upper boundary of this plot in a stack, for the index-th datapoint */
        double getStackedMax(int index) const;
};








#endif // jkqtpgraphsbarchart_H
