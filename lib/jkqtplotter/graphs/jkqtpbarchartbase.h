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

#ifndef jkqtpgraphsbarchartbase_H
#define jkqtpgraphsbarchartbase_H

#include <QString>
#include <QPainter>
#include <QPair>
#include <functional>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"


/** \brief This is a base-class for all bar graphs with vertical or horizontal orientation (the orientation is implemented in dervied classes!)
 *  \ingroup jkqtplotter_barcharts
 *
 *  This class provides basic properties and functionality for plotting a bargraph.
 *  The latter is defined by a series of datapoints \c (x,y=f(x)) .
 *  Bars are then drawn fromm a baseline (often \c =0 ) to the value \c y=f(x) at each position \c x.
 *  Thus the class is derived from JKQTPXYBaselineGraph, which provides a baseline as well as columns for \c x and \c y values.
 *
 *  The width of each bar is determined from its distance to its direct neighbors. It occupies a fraction "width" (JKQTPBarGraphBase::setWidth(), JKQTPBarGraphBase::getWidth() )
 *  of the available space. Typically \c width=0.9 so the bars occupy most space, but do not touch:
 *
 *  \image html bargraph_basics_width.png
 *
 *  \see JKQTPBarHorizontalGraph, JKQTPBarVerticalGraph for implementations of this virtual base class.
 *
 *
 *  \section JKQTPBarGraphBase_SideBySide several JKQTPBarGraphBase side-by-side
 *
 *  If you draw several JKQTPBarGraphBase of the same orientation (vertical or honrizontal) into the same plot, these will by default
 *  overlap, if all of the use the same x-values and have the same initial width parameter.
 *  Typically in such a case, you want to group the bars by x-values and ensure they are drawn side-by-side and do not overlap (or overlap slightly in a controlled way).
 *
 *  To achieve this, an addition shift parameter (JKQTPBarGraphBase::setShift(), JKQTPBarGraphBase::getShift() ) was introduced, which moves each bar inside the available space:
 *
 *  \image html bargraph_basics.png
 *
 *  By default the shift parameter (see JKQTPBarGraphBase::setShift() ) is, so the bar is centered at the x-value (i.e. \c shift=0 ).
 *
 *  You can now use shift to separate three JKQTPBarGraphBase in a graph, by setting e.g.:
 *  \verbatim
 *        width=0.3
 *        shift=-0.3 / 0 / +0.3
 *  \endverbatim
 *
 *  For convenience, two functions are provided (JKQTPBarGraphBase::autoscaleBarWidthAndShift(), JKQTPBarGraphBase::autoscaleBarWidthAndShiftSeparatedGroups() )
 *  that can be applied to any JKQTPBarGraphBase in the plot and will calculate all bargraphs' shift and width parameter
 *  in such a way that the graphs appear tidied up into groups as above. With these functions you don't have to calculate
 *  the shift and width values by hand! A call always only affects the JKQTPBarGraphBase in the plot with the same orientation
 *  (vertical or horizontal).
 *
 *  This results in a bargraph, as shown here:
 *
 *  \image html JKQTPBarVerticalGraph.png
 *
 *  \section JKQTPBarGraphBaseStyling Basic Bargraph Styling
 *
 *  There are several options that allow to style the barchraph. The most direct way is to use the setters from
 *    - JKQTPGraphLineStyleMixin
 *    - JKQTPGraphFillStyleMixin
 *  .
 *  that are available in the class and that allow directly chaning the appearance.
 *  It is also possibly to draw the baseline itself (\c JKQTPBarGraphBase::setDrawBaseline(true) ).
 *  Its style is defined in JKQTPBarGraphBase::baselineStyle() .
 *
 *  Several convenience functions allow to set these properties in a simplified way:
 *    - JKQTPBarGraphBase::setFillColor_and_darkenedColor()
 *  .
 *
 *  By default this class will draw boxes for each bar. They can be rounded at the corners by setting:
 *    - JKQTPBarGraphBase::setRectRadiusAtValue()
 *    - JKQTPBarGraphBase::setRectRadiusAtBaseline()
 *    - JKQTPBarGraphBase::setRectRadius()
 *  .
 *
 *
 *
 *  \section JKQTPBarGraphBaseDataDependentFilling Data-Dependent Styling of Bargraphs
 *
 *  You can also use <code>JKQTPBarGraphBase::setFillMode(JKQTPBarGraphBase::FillMode::TwoColorFilling)</code>,
 *  which uses different fill styles for bars above and below the baseline of the graph:
 *
 *  \image html JKQTPBarVerticalGraphTwoColorFilling.png
 *
 *  The styles for above the baseline is set by the inherited JKQTPGraphLineStyleMixin and JKQTPGraphFillStyleMixin.
 *  The alternate "below" style, can be modified using
 *    - JKQTPBarGraphBase::fillStyleBelow()
 *    - the line-style is the same as from the inherited JKQTPGraphLineStyleMixin
 *  .
 *
 *  If you use <code>JKQTPBarGraphBase::setFillMode(JKQTPBarGraphBase::FillMode::FunctorFilling)</code> you can specify
 *  the fill style (in the form of a  QBruch) for each bar by a custom functor, e.g.
 *  \code
 *    graph->setFillMode(JKQTPBarGraphBase::FillMode::FunctorFilling);
 *    graph->setFillBrushFunctor(
 *      [](double key, double value) -> QBrush {
 *        return QBrush(QColor::fromHsvF(key/12.0, 1.0, 1.0));
 *      }
 *    );
 *  \endcode
 *
 *  The result may look like this:
 *
 *  \image html JKQTPBarVerticalGraphFunctorFilling.png
 *
 *
 *
 *  \section JKQTPBarGraphBaseCustomStyling Custom Bargraph Drawing
 *
 *  You can also completely customize the drawing by defining a custom draw functor. This allows to draw other shapes than the typical bars:
 *  \code
 *    graph->setCustomDrawingFunctor(
 *      [](JKQTPEnhancedPainter& painter, const QRectF& bar_px, const QPointF& datapoint, Qt::Orientation orientation, JKQTPBarGraphBase* graph) {
 *        // draw the bar (if required), pen and brush are already set properly
 *        painter.drawRect(bar_px);
 *        // now we can add some decoration or replace the instruction above:
 *        // ........
 *      }
 *    );
 *    // enable usage of cutom draw functor
 *    graph->setUseCustomDrawFunctor(true);
 *  \endcode
 *
 *  See \ref JKQTPlotterBarchartsCustomDrawFunctor for a detailed example.
 *
 *  The result may look like this:
 *
 *  \image html JKQTPBarVerticalGraphCustomDrawFunctor.png
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarGraphBase: public JKQTPXYBaselineGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        /** \brief a type of functor for FillMode::FunctorFilling
         *
         *  \see setFillBrushFunctor(), getFillBrushFunctor()
         */
        typedef std::function<QBrush(double key, double value,JKQTPEnhancedPainter &painter, JKQTPBarGraphBase* graph)> FillBrushFunctor;
        /** \brief a simplified type of functor for FillMode::FunctorFilling
         *
         *  \see setFillBrushFunctor(), getFillBrushFunctor()
         */
        typedef std::function<QBrush(double key, double value)> SimpleFillBrushFunctor;

        /** \brief functor for custom drawing of bars */
        typedef std::function<void(JKQTPEnhancedPainter& painter, const QRectF& bar_px, const QPointF& datapoint, Qt::Orientation orientation, JKQTPBarGraphBase* graph)> CustomDrawingFunctor;
        /** \brief specifies how the area below the graph is filled
         *
         *  \see setFillMode(), getFillMode(), fillStyleBelow(), \ref JKQTPlotterWigglePlots
         */
        enum FillMode {
            SingleFilling=0, /*!< \brief the whole area is filled with the same color/pattern \image html JKQTPBarVerticalGraph.png */
            TwoColorFilling=1, /*!< \brief the area above and below baseline with the two different colors/pattern \image html JKQTPBarVerticalGraphTwoColorFilling.png */
            FunctorFilling=2, /*!< \brief a functor (use setFillBrushFunctor() to define one) is used to determine the fill color\image html JKQTPBarVerticalGraphFunctorFilling.png */
        };
        Q_ENUM(FillMode)

        /** \brief class constructor */
        JKQTPBarGraphBase(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBarGraphBase(JKQTPlotter* parent);

        /** \copydoc JKQTPXYBaselineGraph::drawKeyMarker() */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \copydoc JKQTPXYBaselineGraph::getKeyLabelColor() */
        virtual QColor getKeyLabelColor() const override;



        /** \copydoc shift */
        double getShift() const;
        /** \copydoc width */
        double getWidth() const;

        /** \brief sets the fill color and the color together, where fillColor is set to \a fill and the line-color is set to \c fill.darker(colorDarker)
         *  \see setColor()
         */
        void setFillColor_and_darkenedColor(QColor fill, int colorDarker=200);
		
		/** \brief returns xColumn or yColumn, whichever is used for the position of the bars (depending on whether the barchart is vertical or horizontal \see getBarHeightColumn(), xColumn, yColumn */
        int getBarPositionColumn() const;
		
		/** \brief returns xColumn or yColumn, whichever is used for the height of the bars (depending on whether the barchart is vertical or horizontal \see getBarPositionColumn(), xColumn, yColumn */
        int getBarHeightColumn() const;
        /** \copydoc m_fillStyleBelow */
        JKQTPGraphFillStyleMixin &fillStyleBelow();
        /** \copydoc m_fillStyleBelow */
        const JKQTPGraphFillStyleMixin& fillStyleBelow() const;
        /** \copydoc m_fillMode */
        FillMode getFillMode() const;
        /** \copydoc rectRadiusAtValue */
        double getRectRadiusAtValue() const;
        /** \copydoc rectRadiusAtBaseline */
        double getRectRadiusAtBaseline() const;
        /** \copydoc m_fillBrushFunctor */
        FillBrushFunctor& getFillBrushFunctor();
        /** \copydoc m_fillBrushFunctor */
        const FillBrushFunctor& getFillBrushFunctor() const;
        /** \copydoc m_lineColorDerivationModeForSpecialFill */
        JKQTPColorDerivationMode getLineColorDerivationModeForSpecialFill() const;
        /** \copydoc m_useCustomDrawFunctor */
        bool usesCustomDrawFunctor() const;
        /** \copydoc m_drawBaseline */
        bool getDrawBaseline() const;
        /** \copydoc m_stackSeparation */
        double getStackSeparation() const;
        /** \copydoc m_baselineStyle */
        JKQTPGraphLineStyleMixin &baselineStyle();
        /** \copydoc m_baselineStyle */
        const JKQTPGraphLineStyleMixin& baselineStyle() const;

    public Q_SLOTS:
        /** \copydoc m_fillMode */
        void setFillMode(JKQTPBarGraphBase::FillMode mode);

        /** \copydoc m_fillBrushFunctor */
        void setFillBrushFunctor(const JKQTPBarGraphBase::FillBrushFunctor& f);
        /** \copydoc m_fillBrushFunctor */
        void setFillBrushFunctor(JKQTPBarGraphBase::FillBrushFunctor&& f);

        /** \copydoc m_fillBrushFunctor */
        void setFillBrushFunctor(const JKQTPBarGraphBase::SimpleFillBrushFunctor& f);
        /** \copydoc m_fillBrushFunctor */
        void setFillBrushFunctor(JKQTPBarGraphBase::SimpleFillBrushFunctor&& f);

        /** \copydoc m_customDrawFunctor */
        void setCustomDrawingFunctor(JKQTPBarGraphBase::CustomDrawingFunctor&& f);
        /** \copydoc m_customDrawFunctor */
        void setCustomDrawingFunctor(const JKQTPBarGraphBase::CustomDrawingFunctor& f);
        /** \copydoc m_useCustomDrawFunctor */
        void setUseCustomDrawFunctor(bool enabled);

        /** \brief finds all bar charts of the same orientation and determines width and shift, so they stand side by side
         *
         *  \param maxWidth the maximum (relative) width, that all bars will span of the (doubled) inter-bar distance
         *  \param shrinkFactor factor, by which the bar are shrinked compared to the available space
         *
         *  \note This function will scale ALL graphs of the parent plot, which were derived from JKQTPBarHorizontalGraph, that match in orientation (as returned by isHorizontal() ).
         *
         *  Using \c autoscaleBarWidthAndSHift(0.75,1.0) You can separate the different groups by soe distance, but the bars touch each other:
         *
         *  \image html JKQTPBarVerticalAutoscaleMaxWidthOnly.png
         *
         *  On the other hand, using the other extreme \c autoscaleBarWidthAndSHift(1.0,0.9) there is no grouping of the bars, but they have a slight distance between each other.
         *
         *  \image html JKQTPBarVerticalAutoscaleShrinkOnly.png
         *
         *  Finally the default parameters \c autoscaleBarWidthAndSHift(0.75,0.9) will lead to a separation of the bars AND a grouping:
         *
         *  \image html JKQTPBarVerticalGraph.png
         *
         */
        virtual void autoscaleBarWidthAndShift(double maxWidth=0.75, double shrinkFactor=0.9);

        /** \brief equivalent to \c autoscaleBarWidthAndShift(groupWidth,0.9);
         *
         *  The default parameters \c autoscaleBarWidthAndShiftSeparatedGroups(0.75) will lead to a separation of the bars AND a grouping:
         *
         *  \image html JKQTPBarVerticalGraph.png
         *
         *  On the other hand, using \c autoscaleBarWidthAndShiftSeparatedGroups(1.0) there is no grouping of the bars, but they have a slight distance between each other.
         *
         *  \image html JKQTPBarVerticalAutoscaleShrinkOnly.png
         */
        void autoscaleBarWidthAndShiftSeparatedGroups(double groupWidth=0.75);
        /** \copydoc shift */
        void setShift(double __value);
        /** \copydoc width */
        void setWidth(double __value);
        /** \copydoc m_drawBaseline */
        void setDrawBaseline(bool __value);

        /** \copydoc m_stackSeparation */
        void setStackSeparation(double __value);
        /** \copydoc rectRadiusAtValue */
        void setRectRadiusAtValue(double __value);
        /** \copydoc rectRadiusAtBaseline */
        void setRectRadiusAtBaseline(double __value);
        /** \brief sets the corner radius of the bars for both ends */
        void setRectRadius(double all);
        /** \brief sets the corner radius of the bars for both ends */
        void setRectRadius(double atValue, double atBaseline);

        /** \copydoc m_lineColorDerivationModeForSpecialFill */
        void setLineColorDerivationModeForSpecialFill(const JKQTPColorDerivationMode& m);

        /** \brief set outline and fill color at the same time
         *  \see setFillColor_and_darkenedColor()
         */
        virtual void setColor(QColor c);


		/** \brief returns xColumn or yColumn, whichever is used for the position of the bars (depending on whether the barchart is vertical or horizontal \see getBarHeightColumn(), xColumn, yColumn */
        void setBarPositionColumn(int column)  ;
		
		/** \brief returns xColumn or yColumn, whichever is used for the position of the bars (depending on whether the barchart is vertical or horizontal \see getBarHeightColumn(), xColumn, yColumn */
        void setBarPositionColumn(size_t column)  ;
		
		/** \brief returns xColumn or yColumn, whichever is used for the height of the bars (depending on whether the barchart is vertical or horizontal \see getBarPositionColumn(), xColumn, yColumn */
        void setBarHeightColumn(int column)  ;
		
		/** \brief returns xColumn or yColumn, whichever is used for the height of the bars (depending on whether the barchart is vertical or horizontal \see getBarPositionColumn(), xColumn, yColumn */
        void setBarHeightColumn(size_t column)  ;
    protected:

        /** \brief the width of the bargraphs, relative to the distance between the current and the next x-value
         *
         * See the following graphic to understand this concept:
         *     \image html bargraph_basics_width.png
         *
         * \note this parameter can be combined with shift !
         *
         * \see setWidth(), getWidth(), setShift(), getShift(), shift, autoscaleBarWidthAndShift(), autoscaleBarWidthAndShiftSeparatedGroups()
         */
        double width;
        /** \brief the shift of the bargraphs, relative to the distance between the current and the next x-value
         *
         * See the following graphic to understand this concept:
         *     \image html bargraph_basics.png
         *
         * \see setShift(), getShift(), setWidth(), getWidth(), autoscaleBarWidthAndShift(), autoscaleBarWidthAndShiftSeparatedGroups()
         */
        double shift;
        /** \brief corner radius (in pt) for bars at the "value" end */
        double rectRadiusAtValue;
        /** \brief corner radius (in pt) for bars at the "baseline" end */
        double rectRadiusAtBaseline;
        /** \brief if m_drawBaseline \c ==true then this style is used to draw the baseline
         *
         *  \see baselineStyle() , setDrawBaseline() , m_drawBaseline
         */
        JKQTPGraphLineStyleMixin m_baselineStyle;
        /** \brief indicates whether to draw a line with style m_baselineStyle at the baseline-value
         *
         *  \image html JKQTPBarVerticalGraphBaseline.png "setDrawBaseline(true);"
         *
         *  \image html JKQTPBarVerticalGraphNoBaseline.png "setDrawBaseline(false);"
         *
         *  \see baselineStyle() , setDrawBaseline() , m_baselineStyle
         */
        bool m_drawBaseline;
        /** \brief separation (in pt) between two bars in a stack of bars */
        double m_stackSeparation;
        /** \brief specifies how the area of the graph is filles
         *
         *  \note If any fill style other than FillStyle::SingleFill is used, the peroperty m_lineColorDerivationModeForSpecialFill
         *        is used to derive the color of the bars' outlines from the fill color.
         *
         *  \see setFillMode(), getFillMode(), m_fillStyleBelow, m_fillBrushFunctor, m_lineColorDerivationModeForSpecialFill
         */
        FillMode m_fillMode;
        /** \brief if m_fillMode \c ==FillAboveAndBelowDifferently then this fill style is used below the baseline and
         *         the default fill style is used above
         *
         *  \see fillStyleBelow() , setFillMode()
         */
        JKQTPGraphFillStyleMixin m_fillStyleBelow;
        /** \brief defines how to derive the line color in m_fillMode!=SingleFilling
         *
         *  \see setLineColorDerivationModeForSpecialFill(), setFillMode(), getLineColorDerivationModeForSpecialFill(), FillMode
         */
        JKQTPColorDerivationMode m_lineColorDerivationModeForSpecialFill;

        /** \brief adapter that converts a SimpleFillBrushFunctor to a FillBrushFunctor */
        struct SimpleFillBrushFunctorAdaptor {
            inline SimpleFillBrushFunctorAdaptor(const SimpleFillBrushFunctor& f): m_f(f) {}
            inline SimpleFillBrushFunctorAdaptor(SimpleFillBrushFunctor&& f): m_f(std::forward<SimpleFillBrushFunctor>(f)) {}
            SimpleFillBrushFunctor m_f;
            inline QBrush operator()(double key, double value,JKQTPEnhancedPainter &, JKQTPBarGraphBase*) { return m_f(key,value);}
        };

        /** \brief functor, used to determine the color in m_fillMode==FunctorFilling
         *
         *  If you use FillMode::FunctorFilling you can specify the fill style by a functor, e.g.
         *  \code
         *    graph->setFillMode(JKQTPBarGraphBase::FillMode::FunctorFilling);
         *    graph->setFillBrushFunctor(
         *      [](double key, double value) {
         *        return QBrush(QColor::fromHsvF(key/12.0, 1.0, 1.0));
         *      }
         *    );
         *  \endcode
         *
         *  The result may look like this:
         *
         *  \image html JKQTPBarVerticalGraphFunctorFilling.png
         *
         *  \see setFillBrushFunctor(), getFillBrushFunctor(), m_fillMode
         */
        FillBrushFunctor m_fillBrushFunctor;
        /** \brief this allows to provide custom drawing code for the bars.
         *         It is called for every visible bar if activated by \c setUseCustomDrawFunctor(true) .
         *
         *  Here is an example for a custom draw functor:
         *  \code
         *    graph->setCustomDrawingFunctor(
         *      [](JKQTPEnhancedPainter& painter, const QRectF& bar_px, const QPointF& datapoint, Qt::Orientation orientation, JKQTPBarGraphBase* graph) {
         *        // draw the bar (if required), pen and brush are already set properly
         *        painter.drawRect(bar_px);
         *        // now we can add some decoration or replace the instruction above:
         *        // ........
         *      }
         *    );
         *    // enable usage of cutom draw functor
         *    graph->setUseCustomDrawFunctor(true);
         *  \endcode
         *
         *
         *  The result may look like this:
         *
         *  \image html JKQTPBarVerticalGraphCustomDrawFunctor.png
         *
         *  \see CustomDrawingFunctor, m_useCustomDrawFunctor, setUseCustomDrawFunctor(), \ref JKQTPlotterBarchartsCustomDrawFunctor for a detailed example
         */
        CustomDrawingFunctor m_customDrawFunctor;
        /** \brief enabled custom drawing by m_customDrawFunctor
         *
         *  \see m_customDrawFunctor
         */
        bool m_useCustomDrawFunctor;

        /** \brief returns a FillBrushFunctor that is appropriate for the currently selected m_fillMode */
        virtual FillBrushFunctor constructFillBrushFunctor() const;

        /** \brief this function is used by autoscaleBarWidthAndShift() to determine whether a given graph shall be taken into account when autoscaling. 
         *         Typically this returns \c true for all JKQTPBarGraphBase-derived objects with the same orientation (horizontal or vertical). For stacked
         *         graphs it excludes everything except the bottom of the stack
         */
        virtual bool considerForAutoscaling( JKQTPBarGraphBase* other) const=0;

        /** \brief used to generate stacked plots: returns the upper boundary of the parent plot in a stack, for the index-th datapoint */
        virtual double getParentStackedMax(int index) const ;

        /** \brief returns \c true, if a stack parent is set (if available) */
        virtual bool hasStackParent() const ;

        /** \brief get the maximum and minimum value in the box-elongation (i.e. value) direction of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getValuesMinMax(double& mmin, double& mmax, double& smallestGreaterZero) ;
        /** \brief get the maximum and minimum value of the box positions of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getPositionsMinMax(double& mmin, double& mmax, double& smallestGreaterZero) ;
};



/** \brief This is an interface-class for all stackable bargraphs ... it is used internally for autoscaling only
 *  \ingroup jkqtplotter_barcharts
 *
 *  \c dynamic_cast 'ing to this indicates that a barchart is stackable.
 *
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBarGraphStackInternalInterface {
public:
    inline virtual ~JKQTPBarGraphStackInternalInterface() {};
protected:

    /** \brief returns the barchart at the bottom of this stack (i.e. traverses the stack until there are no more parents */
    virtual JKQTPBarGraphBase* getBottomOfStack()=0;

    friend class JKQTPBarGraphBase;
};



#endif // jkqtpgraphsbarchartbase_H
