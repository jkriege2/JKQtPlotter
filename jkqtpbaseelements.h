/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate$  (revision $Rev$)

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
 * \defgroup jkqtpbaseplotter_elements Basic Plot Elements
 * \ingroup jkqtplotter_elements
 *
 * This group contains some tool classes that implement basic elements of the plot (coordinate axes, key, ...).
 * These classes are used by JKQtPlotterBase to output the plot
 */

/** \file jkqtpelements.h
  * \ingroup jkqtpbaseplotter_elements
  */

#ifndef JKQTPBASEELEMENTS_H
#define JKQTPBASEELEMENTS_H


#include <QString>
#include <QPainter>
#include <QPair>
#include <QSettings>
#include "jkqtptools.h"
#include "jkqtmathtext.h"
#include "jkqtp_imexport.h"

// forward declarations
class JKQtBasePlotter;

/*! \brief this virtual class is the base for any type of coordinate axis, to be drawn by JKQTplotterBase.
    \ingroup jkqtpbaseplotter_elements

    This class implements all the functionality needed for a coordinate axis:
      - transform world to screen coordinates and vice versa
      - draw the axis (implemented by child classes!) with these elements: axis lines, ticks, tick labels, axis label, x/y=0 axis
      - measure the axes in screen coordinates
      - load and save the settings to an ini file
    .

    Most of the actual draw and measure functions have to be implemented in descendents of this class (namely
    JKQTPhorizontalAxis and JKQTPverticalAxis, as they are specific to whether the axis is drawn horizontally or
    vertically.

    Each axis is split up into several parts, as depicted in this image:
    \image html jkqtpcoordinateaxis1.png

    Which parts to actually draw is set by the diverse properties.


    \section jkqtplotter_base_systems Coordinate Systems and Transformations
    The plot itself is positioned inside the widget. You simply supply the widget dimensions and the border
    widths between the plot and the widget. The Object then calculates the size of the plot:

     \image html plot_widget_orientation.png

    The plot shows the parameter range xmin ... xmax and ymin ... ymax. Note that if you plot logarithmic plots
    you may only plot positive (>0) values. Any other value may lead to an error or unpredictable behaviour.

    From these parameters the object calculates the scaling laws for plotting pints to the screen. The next paragraphs
    show all calculations for x- and y-coordinates, ahough this class only implements a generic form. The actual calculation
    is also influenced by the parameters set in the child classes!

       -# <tt>width = xmax - xmin</tt>
       -# <tt>height = ymax - ymin</tt>
       -# <tt>plotWidth = widgetWidth - plotBorderLeft - plotBorderRight</tt>
       -# <tt>plotHeight = widgetHeight - plotBorderTop - plotBorderBottom</tt>
       -# <tt>xPlotOffset = plotBorderLeft</tt>
       -# <tt>yPlotOffset = plotBorderTop</tt>
    .
    These parameters are common to all scaling laws. The next image explains these formulas:
      \image html plot_coordinates.png
    The actual scaling laws \f$ (x_p(x), y_p(y)) \f$ may be derived from these equations:
     \f[ x_p(\mbox{xmin})=\mbox{xPlotOffset},\ \ \ \ \ x_p(\mbox{xmax})=\mbox{xPlotOffset}+\mbox{plotWidth} \f]
     \f[ y_p(\mbox{ymax})=\mbox{yPlotOffset},\ \ \ \ \ y_p(\mbox{ymin})=\mbox{yPlotOffset}+\mbox{plotHeight} \f]
    Here \f$ (x_p, y_p) \f$ denotes a point in pixel coordinates (green coordinate system) and \f$ (x,y) \f$ denotes a point in the
    plotting coordinate system.

    If you assume a <b>linear scaling law</b>. \f[ x_p(x)=\mbox{xoffset}+x\cdot\mbox{xscale} \ \ \ \ \ \ \ \ \ y_p(y)=\mbox{yoffset}-y\cdot\mbox{yscale} \f]
    you can derive:
     \f[ \mbox{xscale}=\frac{\mbox{plotWidth}}{\mbox{xwidth}},\ \ \ \ \  \ \mbox{xoffset}=\mbox{xPlotOffset}-\mbox{xmin}\cdot\mbox{xscale} \f]
     \f[ \mbox{yscale}=\frac{\mbox{plotHeight}}{\mbox{ywidth}},\ \ \ \ \ \ \mbox{yoffset}=\mbox{yPlotOffset}+\mbox{ymax}\cdot\mbox{yscale} \f]

    If you have a <b>logarithmic axis</b> (i.e. we plot \f$ \log_b(x) \f$ insetad of \f$ x \f$ for a given base \f$ b \f$ ) we get the same
    formulas, but with \f$ x \f$ exchanged by \f$ \log_b(x) \f$. If we use the equation \f$ \log_b(x)=\log(x)/\log(b) \f$ we finally get
    the scaling laws:
    \f[ x_p(x)=\mbox{xoffset}+\frac{\log(x)}{\log(\mbox{logXAxisBase})}\cdot\mbox{xscale} \ \ \ \ \ \ \ \ \ y_p(y)=\mbox{yoffset}-\frac{\log(y)}{\log(\mbox{logYAxisBase})}\cdot\mbox{yscale} \f]
    From these we can calculate their parameters with the same defining equations as above:
     \f[ \mbox{xscale}=\frac{\mbox{plotWidth}\cdot\log(\mbox{logXAxisBase})}{\log(\mbox{xmax})-\log(\mbox{xmin})},\ \ \ \ \  \ \mbox{xoffset}=\mbox{xPlotOffset}-\frac{\log(\mbox{xmin})}{\log(\mbox{logXAxisBase})}\cdot\mbox{xscale} \f]
     \f[ \mbox{yscale}=\frac{\mbox{plotHeight}\cdot\log(\mbox{logYAxisBase})}{\log(\mbox{ymax})-\log(\mbox{ymin})},\ \ \ \ \  \ \mbox{yoffset}=\mbox{yPlotOffset}+\frac{\log(\mbox{ymax})}{\log(\mbox{logYAxisBase})}\cdot\mbox{yscale} \f]

    The object implements the above coordinate transformations in the (inline) method x2p(). The inverse transformations
    are implemented in p2x(). They can be used to show the system coordinates of the current mouse position.


    \section jkqtplotter_base_grids Axis Ticks and Grids

    This section explains how this component draws the ticks on coordinate axes and the grids that may be drawn below
    the plots. In principle both - grids and axes - are drawn the same way, i.e. with the same step widths. There are
    two types of ticks and grids: The major and the minor ticks/grids. The major ticks also show a label that denotes the
    value they represent. Between two major ticks the axis shows \a minorTicks  small ticks that are not
    accompanied by a label. The next image shows an example of an axis:
      \image html plot_axis_ticksandlabels.png

    For the labels this class also uses an algorithm that extimates the number of valid digits (after the comma) that are
    needed so that two adjacent labels do not show the same text, so if you plot the range 1.10 .. 1.15 The algorithm will
    show at least two valid digits, as with one digit the labels would be 1.1, 1.1, 1.1, 1.1, 1.1, 1.1. With two digits they
    are 1.10, 1.11, 1.12, 1.13, 1.14, 1.15. The class may also use a method that can write \c 1m instead of \c 0.001 and \c 1k
    instead of \c 1000 (the algorithm supports the "exponent letters" f, p, n, u, m, k, M, G, T, P. The latter option may
    (de)activated by using showXExponentCharacter and showYExponentCharacter.

    For grids applies the same. There are two grids that are drawn in different styles (often the major grid is drawn
    thicker and darker than the minor grid).

    The minor ticks and grid lines are generated automatically, depending in the setting of \a minorTicks.
    These properties give the number of minor ticks between two major ticks, so if the major ticks are at 1,2,3,... and you
    want minor ticks at 1.1, 1.2, 1.3,... then you will have to set \c minorTicks=9 as there are nine ticks between two major
    ticks. So the minor tick spacing is calculated as: \f[ \Delta\mbox{MinorTicks}=\frac{\Delta\mbox{ticks}}{\mbox{minorTicks}+1} \f]

    The same applies for logarithmic axes. If the major ticks are at 1,10,100,... and you set \c minorTicks=9 the program will
    generate 9 equally spaced minor ticks in between, so you have minor ticks at 2,3,4,...,11,12,13,... This results in a standard
    logarithmic axis. If you set \c minorTicks=1 then you will get minor ticks at 5,15,150,...
      \image html plot_logaxis_ticksandlabels.png

    The major tick-tick distances of linear axes may be calculated automatically in a way that the axis shows at least a given
    number of ticks \c minTicks. The algorithm takes that tick spacing that will give a number of ticks per axis
    nearest but \c ">=" to the given \c minTicks. The Algorithm is described in detail with the function
    calcLinearTickSpacing(). To activate this automatic tick spacing you have to set <code>autoAxisSpacing=true</code>.
 */
class LIB_EXPORT JKQTPcoordinateAxis: public QObject {
        Q_OBJECT
    protected:
    public:

        /** \brief class constructor */
        explicit JKQTPcoordinateAxis(JKQtBasePlotter* parent);
        /** \brief class destructor */
        virtual ~JKQTPcoordinateAxis();
        virtual void set_parent(JKQtBasePlotter* parent);
        /** \brief loads the plot properties from a QSettings object */
        virtual void loadSettings(QSettings& settings, QString group=QString("plots"));

        /** \brief saves the plot properties into a QSettings object.
         *
         * This method only saves those properties that differ from their default value.
         */
        virtual void saveSettings(QSettings& settings, QString group=QString("plots"));

        /** \brief load settings from other axis */
        virtual void loadSettings(JKQTPcoordinateAxis* settings);

        /** \brief return x-pixel coordinate from time coordinate */
        inline double x2p(double x) const {
            double r=0;
            if (logAxis) {
                if (x<=0) r= offset+scaleSign*log(axismin)/log(logAxisBase)*scale;
                else r= offset+scaleSign*log(x)/log(logAxisBase)*scale;
            } else {
                r= offset+scaleSign*x*scale;
            }
            if (inverted) {
                return 2.0*get_parent_plotoffset()+get_parent_plotwidth()-r;//get_parent_plotoffset()+get_parent_plotwidth()-(r-get_parent_plotoffset());
            } else {
                return r;
            }
        };

        /** \brief return time coordinate coordinate from x-pixel */
        inline double p2x(long x) const {
            double xx=x;
            if (inverted) {
                xx=2.0*get_parent_plotoffset()+get_parent_plotwidth()-x;
            }
            xx=xx-offset;
            if (logAxis) {
                return exp(log(logAxisBase)*(xx)/(scaleSign*scale));
            } else {
                return xx/(scaleSign*scale);
            }
        };


        /** \brief clear axis tick labels. This switches back to automatic labels mode for the axis. */
        inline void clearAxisTickLabels() {
            tickLabels.clear();
            update_plot();
        };

        /** \brief add a new tick label to the axis */
        inline void addAxisTickLabel(double x, QString label) {
            tickLabels.append(qMakePair(x, label));
            update_plot();
        };

        /** \brief returns the size of the left/bottom axis in pixels */
        virtual QSize getSize1(JKQTPEnhancedPainter& painter)=0;

        /** \brief returns the size of the right/top axis in pixels */
        virtual QSize getSize2(JKQTPEnhancedPainter& painter)=0;

        /** \brief draw axes  */
        virtual void drawAxes(JKQTPEnhancedPainter& painter)=0;

        /** \brief draw grids  */
        virtual void drawGrids(JKQTPEnhancedPainter& painter)=0;


        JKQTPGET_MACRO(double, tickSpacing)
        JKQTPGET_MACRO(int, labelDigits)
        JKQTPGET_MACRO(bool, autoAxisSpacing)
        JKQTPGET_MACRO(bool, minorTickLabelsEnabled)
        JKQTPGET_MACRO(bool, logAxis)
        JKQTPGET_MACRO(bool, inverted)
        JKQTPGET_MACRO(double, logAxisBase)
        JKQTPGET_MACRO(double, userTickSpacing)
        JKQTPGET_MACRO(double, userLogTickSpacing)
        JKQTPGET_MACRO(JKQTPCAlabelType, labelType)
        JKQTPGET_MACRO(QString, axisLabel)
        JKQTPGET_MACRO(JKQTPlabelPosition, labelPosition)
        JKQTPGET_MACRO(QString, labelFont)
        JKQTPGET_MACRO(double, labelFontSize)
        JKQTPGET_MACRO(QString, tickLabelFont)
        JKQTPGET_MACRO(double, tickLabelFontSize)
        JKQTPGET_MACRO(double, minorTickLabelFontSize)
        JKQTPGET_MACRO(bool, minorTickLabelFullNumber)
        JKQTPGET_MACRO(double, tickLabelAngle)
        JKQTPGET_MACRO(unsigned int, minTicks)
        JKQTPGET_MACRO(unsigned int, minorTicks)
        JKQTPGET_MACRO(double, tickOutsideLength)
        JKQTPGET_MACRO(double, minorTickOutsideLength)
        JKQTPGET_MACRO(QColor, axisColor)
        JKQTPGET_MACRO(bool, showZeroAxis)
        JKQTPGET_MACRO(QColor, gridColor)
        JKQTPGET_MACRO(QColor, minorGridColor)
        JKQTPGET_MACRO(double, gridWidth)
        JKQTPGET_MACRO(Qt::PenStyle, gridStyle)
        JKQTPGET_MACRO(double, minorGridWidth)
        JKQTPGET_MACRO(Qt::PenStyle, minorGridStyle)
        JKQTPGET_MACRO(QString, tickTimeFormat)
        JKQTPGET_MACRO(QString, tickDateFormat)
        JKQTPGET_MACRO(QString, tickDateTimeFormat)
        JKQTPGET_MACRO(JKQTPLabelTickMode, tickMode)

        JKQTPGET_MACRO(JKQTPCAdrawMode, drawMode1)
        JKQTPGET_MACRO(JKQTPCAdrawMode, drawMode2)
        JKQTPGET_MACRO(double, minorTickWidth)
        JKQTPGET_MACRO(double, tickWidth)
        JKQTPGET_MACRO(double, lineWidth)
        JKQTPGET_MACRO(double, lineWidthZeroAxis)
        JKQTPGET_MACRO(double, tickLabelDistance)
        JKQTPGET_MACRO(double, labelDistance)
        JKQTPGET_MACRO(bool, drawGrid)
        JKQTPGET_MACRO(bool, drawMinorGrid)
        JKQTPGET_SET_MACRO(bool, autoLabelDigits)
        JKQTPGET_MACRO(JKQtBasePlotter*, parent)
        JKQTPGET_SET_MACRO(bool, doUpdateScaling)


        /** \brief returns the current min */
        inline double getMin() const {return axismin; };

        /** \brief returns the current max */
        inline double getMax() const {return axismax; };

        /** \brief returns the current absolute min */
        inline double getAbsoluteMin() const {return axisabsoultemin; };

        /** \brief returns the current absolute max */
        inline double getAbsoluteMax() const {return axisabsoultemax; };

        /** \brief calculate the scaling and offset values from axis min/max values
         *
         *  This is only executed when \c paramChanged==true, as otherwise the data has to be still
         *  correct. recalculating may be forced by caling calcPlotScaing(true);
         */
        void calcPlotScaling(bool force=false);


        inline bool isLogAxis() const {
            return logAxis || (tickMode==JKQTPLTMPower);
        }

    public slots:
        /** \brief set range of plot axis */
        void setRange(double amin, double amax);
        /** \brief set absolute range of plot axis */
        void setAbsoluteRange(double aamin, double aamax);
        /** \brief do not use an absolute range of plot axis */
        void setNoAbsoluteRange();

        /** \brief sets the property tickSpacing to the specified \a __value.
         *  \details Description of the parameter tickSpacing is:  <CENTER>\copybrief tickSpacing.</CENTER>
         *  \see tickSpacing for more information */
        inline virtual void set_tickSpacing(double __value) {
            this->tickSpacing = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property axisMinWidth to the specified \a __value.
         *  \details Description of the parameter tickSpacing is:  <CENTER>\copybrief axisMinWidth.</CENTER>
         *  \see axisMinWidth for more information */
        inline virtual void set_axisMinWidth(double __value) {
            this->axisMinWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property autoAxisSpacing to the specified \a __value.
         *  \details Description of the parameter autoAxisSpacing is:  <CENTER>\copybrief autoAxisSpacing.</CENTER>
         *  \see autoAxisSpacing for more information */
        inline virtual void set_autoAxisSpacing(bool __value) {
            this->autoAxisSpacing = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorTickLabelsEnabled to the specified \a __value.
         *  \details Description of the parameter minorTickLabelsEnabled is:  <CENTER>\copybrief minorTickLabelsEnabled.</CENTER>
         *  \see autoAxisSpacing for more information */
        inline virtual void set_minorTickLabelsEnabled(bool __value) {
            this->minorTickLabelsEnabled = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property logAxis to the specified \a __value.
         *  \details Description of the parameter logAxis is:  <CENTER>\copybrief logAxis.</CENTER>
         *  \see logAxis for more information */
        virtual void set_logAxis(bool __value) ;

        /** \brief sets the property logAxisBase to the specified \a __value.
         *  \details Description of the parameter logAxisBase is:  <CENTER>\copybrief logAxisBase.</CENTER>
         *  \see logAxisBase for more information */
        inline virtual void set_logAxisBase (double __value) {
            this->logAxisBase = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property userTickSpacing to the specified \a __value.
         *  \details Description of the parameter userTickSpacing is:  <CENTER>\copybrief userTickSpacing.</CENTER>
         *  \see userTickSpacing for more information */
        inline virtual void set_userTickSpacing (double __value) {
            this->userTickSpacing = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property userLogTickSpacing to the specified \a __value.
         *  \details Description of the parameter userLogTickSpacing is:  <CENTER>\copybrief userLogTickSpacing.</CENTER>
         *  \see userLogTickSpacing for more information */
        inline virtual void set_userLogTickSpacing (double __value) {
            this->userLogTickSpacing = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property labelType to the specified \a __value.
         *  \details Description of the parameter labelType is:  <CENTER>\copybrief labelType.</CENTER>
         *  \see labelType for more information */
        inline virtual void set_labelType (JKQTPCAlabelType __value) {
            this->labelType = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property tickMode to the specified \a __value.
         *  \details Description of the parameter tickMode is:  <CENTER>\copybrief tickMode.</CENTER>
         *  \see tickMode for more information */
        virtual void set_tickMode (JKQTPLabelTickMode __value);
        /** \brief sets the property tickMode to the specified \a __value.
         *  \details Description of the parameter tickMode is:  <CENTER>\copybrief tickMode.</CENTER>
         *  \see tickMode for more information */
        inline virtual void set_tickMode (int __value) {
           set_tickMode(JKQTPLabelTickMode(__value));
        };

        /** \brief sets the property axisLabel to the specified \a __value.
         *  \details Description of the parameter axisLabel is:  <CENTER>\copybrief axisLabel.</CENTER>
         *  \see axisLabel for more information */
        inline virtual void set_axisLabel (QString __value) {
            this->axisLabel = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property labelPosition to the specified \a __value.
         *  \details Description of the parameter labelPosition is:  <CENTER>\copybrief labelPosition.</CENTER> */
        inline virtual void set_labelPosition (JKQTPlabelPosition __value) {
            this->labelPosition = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property labelFont to the specified \a __value.
         *  \details Description of the parameter labelFont is:  <CENTER>\copybrief labelFont.</CENTER> */
        inline virtual void set_labelFont (QString __value) {
            this->labelFont = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property labelFontSize to the specified \a __value.
         *  \details Description of the parameter labelFontSize is:  <CENTER>\copybrief labelFontSize.</CENTER> */
        inline virtual void set_labelFontSize (double __value) {
            this->labelFontSize = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property tickLabelFont to the specified \a __value.
         *  \details Description of the parameter tickLabelFont is:  <CENTER>\copybrief tickLabelFont.</CENTER> */
        inline virtual void set_tickLabelFont (QString __value) {
            this->tickLabelFont = __value;
            this->paramsChanged=true;
            update_plot();
        };


        /** \brief sets the property tickTimeFormat to the specified \a __value.
         *  \details Description of the parameter tickTimeFormat is:  <CENTER>\copybrief tickTimeFormat.</CENTER> */
        inline virtual void set_tickTimeFormat (QString __value) {
            this->tickTimeFormat = __value;
            this->paramsChanged=true;
            update_plot();
        };


        /** \brief sets the property tickDateFormat to the specified \a __value.
         *  \details Description of the parameter tickDateFormat is:  <CENTER>\copybrief tickDateFormat.</CENTER> */
        inline virtual void set_tickDateFormat (QString __value) {
            this->tickDateFormat = __value;
            this->paramsChanged=true;
            update_plot();
        };


        /** \brief sets the property tickDateTimeFormat to the specified \a __value.
         *  \details Description of the parameter tickDateTimeFormat is:  <CENTER>\copybrief tickDateTimeFormat.</CENTER> */
        inline virtual void set_tickDateTimeFormat (QString __value) {
            this->tickDateTimeFormat = __value;
            this->paramsChanged=true;
            update_plot();
        };




        /** \brief sets the property tickLabelFontSize to the specified \a __value.
         *  \details Description of the parameter tickLabelFontSize is:  <CENTER>\copybrief tickLabelFontSize.</CENTER> */
        inline virtual void set_tickLabelFontSize (double __value) {
            this->tickLabelFontSize = __value;
            this->paramsChanged=true;
            update_plot();
        };



        /** \brief sets the property minorTickLabelFontSize to the specified \a __value.
         *  \details Description of the parameter minorTickLabelFontSize is:  <CENTER>\copybrief minorTickLabelFontSize.</CENTER> */
        inline virtual void set_minorTickLabelFontSize (double __value) {
            this->minorTickLabelFontSize = __value;
            this->paramsChanged=true;
            update_plot();
        };


        /** \brief sets the property minorTickLabelFullNumber to the specified \a __value.
         *  \details Description of the parameter minorTickLabelFullNumber is:  <CENTER>\copybrief minorTickLabelFullNumber.</CENTER> */
        inline virtual void set_minorTickLabelFullNumber (bool __value) {
            this->minorTickLabelFullNumber = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minTicks to the specified \a __value.
         *  \details Description of the parameter minTicks is:  <CENTER>\copybrief minTicks.</CENTER> */
        inline virtual void set_minTicks(unsigned int __value) {
            this->minTicks = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorTicks to the specified \a __value.
         *  \details Description of the parameter minorTicks is:  <CENTER>\copybrief minorTicks.</CENTER> */
        inline virtual void set_minorTicks (unsigned int __value) {
            this->minorTicks = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorTicks to the specified \a __value.
         *  \details Description of the parameter minorTicks is:  <CENTER>\copybrief minorTicks.</CENTER> */
        inline virtual void set_minorTicks (int __value) {
            this->minorTicks = qMax(int(0), __value);
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property tickOutsideLength to the specified \a __value.
         *  \details Description of the parameter tickOutsideLength is:  <CENTER>\copybrief tickOutsideLength.</CENTER> */
        inline virtual void set_tickOutsideLength(double __value) {
            this->tickOutsideLength = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorTickOutsideLength to the specified \a __value.
         *  \details Description of the parameter minorTickOutsideLength is:  <CENTER>\copybrief minorTickOutsideLength.</CENTER> */
        inline virtual void set_minorTickOutsideLength (double __value) {
            this->minorTickOutsideLength = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property tickInsideLength to the specified \a __value.
         *  \details Description of the parameter tickInsideLength is:  <CENTER>\copybrief tickInsideLength.</CENTER> */
        inline virtual void set_tickInsideLength(double __value) {
            this->tickInsideLength = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorTickInsideLength to the specified \a __value.
         *  \details Description of the parameter minorTickInsideLength is:  <CENTER>\copybrief minorTickInsideLength.</CENTER> */
        inline virtual void set_minorTickInsideLength (double __value) {
            this->minorTickInsideLength = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property axisColor to the specified \a __value.
         *  \details Description of the parameter axisColor is:  <CENTER>\copybrief axisColor.</CENTER> */
        inline virtual void set_axisColor (QColor __value) {
            this->axisColor = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property showZeroAxis to the specified \a __value.
         *  \details Description of the parameter showZeroAxis is:  <CENTER>\copybrief showZeroAxis.</CENTER> */
        inline virtual void set_showZeroAxis(bool __value) {
            this->showZeroAxis = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property inverted to the specified \a __value.
         *  \details Description of the parameter inverted is:  <CENTER>\copybrief inverted.</CENTER> */
        inline virtual void set_inverted(bool __value) {
            this->inverted = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property gridColor to the specified \a __value.
         *  \details Description of the parameter gridColor is:  <CENTER>\copybrief gridColor.</CENTER> */
        inline virtual void set_gridColor(QColor __value) {
            this->gridColor = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorGridColor to the specified \a __value.
         *  \details Description of the parameter minorGridColor is:  <CENTER>\copybrief minorGridColor.</CENTER> */
        inline virtual void set_minorGridColor(QColor __value) {
            this->minorGridColor = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property gridWidth to the specified \a __value.
         *  \details Description of the parameter gridWidth is:  <CENTER>\copybrief gridWidth.</CENTER> */
        inline virtual void set_gridWidth (double __value) {
            this->gridWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property gridStyle to the specified \a __value.
         *  \details Description of the parameter gridStyle is:  <CENTER>\copybrief gridStyle.</CENTER> */
        inline virtual void set_gridStyle(Qt::PenStyle __value) {
            this->gridStyle = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorGridWidth to the specified \a __value.
         *  \details Description of the parameter minorGridWidth is:  <CENTER>\copybrief minorGridWidth.</CENTER> */
        inline virtual void set_minorGridWidth(double __value) {
            this->minorGridWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorGridStyle to the specified \a __value.
         *  \details Description of the parameter minorGridStyle is:  <CENTER>\copybrief minorGridStyle.</CENTER> */
        inline virtual void set_minorGridStyle (Qt::PenStyle __value) {
            this->minorGridStyle = __value;
            this->paramsChanged=true;
            update_plot();
        };


        /** \brief sets the property drawMode1 to the specified \a __value.
         *  \details Description of the parameter drawMode1 is:  <CENTER>\copybrief drawMode1.</CENTER> */
        inline virtual void set_drawMode1 (JKQTPCAdrawMode __value) {
            this->drawMode1 = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property drawMode2 to the specified \a __value.
         *  \details Description of the parameter drawMode2 is:  <CENTER>\copybrief drawMode2.</CENTER> */
        inline virtual void set_drawMode2(JKQTPCAdrawMode __value) {
            this->drawMode2 = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property minorTickWidth to the specified \a __value.
         *  \details Description of the parameter minorTickWidth is:  <CENTER>\copybrief minorTickWidth.</CENTER> */
        inline virtual void set_minorTickWidth(double __value) {
            this->minorTickWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property tickWidth to the specified \a __value.
         *  \details Description of the parameter tickWidth is:  <CENTER>\copybrief tickWidth.</CENTER> */
        inline virtual void set_tickWidth (double __value) {
            this->tickWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property lineWidth to the specified \a __value.
         *  \details Description of the parameter lineWidth is:  <CENTER>\copybrief lineWidth.</CENTER> */
        inline virtual void set_lineWidth (double __value) {
            this->lineWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property lineWidthZeroAxis to the specified \a __value.
         *  \details Description of the parameter lineWidthZeroAxis is:  <CENTER>\copybrief lineWidthZeroAxis.</CENTER> */
        inline virtual void set_lineWidthZeroAxis (double __value) {
            this->lineWidthZeroAxis = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property tickLabelDistance to the specified \a __value.
         *  \details Description of the parameter tickLabelDistance is:  <CENTER>\copybrief tickLabelDistance.</CENTER> */
        inline virtual void set_tickLabelDistance(double __value) {
            this->tickLabelDistance = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property labelDistance to the specified \a __value.
         *  \details Description of the parameter labelDistance is:  <CENTER>\copybrief labelDistance.</CENTER> */
        inline virtual void set_labelDistance(double __value) {
            this->labelDistance = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief sets the property labelDigits to the specified \a __value.
         *  \details Description of the parameter labelDistance is:  <CENTER>\copybrief labelDigits.</CENTER> */
        inline virtual void set_labelDigits(int __value) {
            this->labelDigits = __value;
            this->paramsChanged=true;
            this->autoLabelDigits=false;
            update_plot();
        };

        /** \brief sets the property drawGrid to the specified \a __value.
         *  \details Description of the parameter drawGrid is:  <CENTER>\copybrief drawGrid.</CENTER> */
        inline virtual void set_drawGrid(bool __value) {
            this->drawGrid = __value;
            this->paramsChanged=true;
            update_plot();
        };

        /** \brief sets the property drawMinorGrid to the specified \a __value.
         *  \details Description of the parameter drawMinorGrid is:  <CENTER>\copybrief drawMinorGrid.</CENTER> */
        inline virtual void set_drawMinorGrid(bool __value) {
            this->drawMinorGrid = __value;
            this->paramsChanged=true;
            update_plot();
        };


        /** \brief sets the property tickLabelAngle to the specified \a __value.
         *  \details Description of the parameter tickLabelAngle is:  <CENTER>\copybrief tickLabelAngle.</CENTER> */
        inline virtual void set_tickLabelAngle(double __value) {
            this->tickLabelAngle = __value;
            this->paramsChanged=true;
            update_plot();
        };

    protected:
        /** \brief indicates whether one of the parameters has changed sinse the last recalculation of tickspacing ... */
        bool paramsChanged;
        bool doUpdateScaling;

        /** \brief simply calls the update_plot method of the parent plotter class */
        void update_plot();
        /** \brief a list of tick labels.
         *
         *  If this list contains items, this class will NOT plot a standard x-axis,
         * but only mark the positions in this list. Every item is a x-position together with
         * the label to be plotted there. The label may contain LaTeX markup.
         */
        QVector<QPair<double, QString> > tickLabels;
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const=0;
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const=0;
        /** \brief width of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const=0;
        /** \brief is other (perpendicular) axis inverted (needed for grids) */
        virtual bool get_parent_otheraxis_inverted() const=0;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const=0;
        /** \brief retun parents JKQTmathText* object */
        virtual JKQTmathText* get_parent_mathText();


        /** \brief convert a float to a tick label string */
        QString floattolabel(double data);

        /** \brief convert a float to a tick label string with a given precision */
        QString floattolabel(double data, int past_comma);
        /** \brief parent plotter class */
        JKQtBasePlotter* parent;
        /** \brief current view: minimum of time axis */
        double axismin;
        /** \brief current view: maximum of time axis */
        double axismax;

        /** \brief absoulte minimum of axis (axismin/axismax xan not be set below this) */
        double axisabsoultemin;
        /** \brief absoulte maximum of axis (axismin/axismax xan not be set above this) */
        double axisabsoultemax;

        /** \brief absolute minimum range width, feature switched off when <0 */
        double axisMinWidth;

        /** \brief width of plot on time axis (calculated by calcPlotScaling() )  */
        double width;

        /** \brief time axis scaling factor (calculated by calcPlotScaling() )  */
        double scale;
        /** \brief time axis offset (calculated by calcPlotScaling() )  */
        double offset;
        /** \brief indicates whether the axis is to be inverted or not */
        bool inverted;
        /** \brief axis tick spacing (calculated by calcPlotScaling() and calcTickSpacing() ) for logarithmic/linear axes.
         */
        double tickSpacing;
        /** \brief axis tick spacing for logarithmic ticks (calculated by calcPlotScaling() and calcTickSpacing() ) axes.
         */
        double tickSpacingLog;
        /** \brief digits used for tick labels */
        int labelDigits;
        /** \brief x position of the first tick (calculated by calcPlotScaling() ). Given in system coordinates, not pixel coordinates. */
        double tickStart;
        /** \brief when \c true, the digits of the labels are calculated automatically */
        bool autoLabelDigits;
        /** \brief indicates whether the object should use automatic tick spacing for the x axis (calculated by calcPlotScaling() ) */
        JKQTPPROPERTY(bool, autoAxisSpacing);
        /** \brief if \c true, the plotter displays minor axis labels as number between 1 and 10 in some cases */
        JKQTPPROPERTY(bool, minorTickLabelsEnabled);
        /** \brief indicates whether the y axis has a logarithmic scale */
        JKQTPPROPERTY(bool, logAxis);
        /** \brief the base for a logarithmic x axis */
        JKQTPPROPERTY(double, logAxisBase);
        /** \brief if autoXAxisSpacing is \c false then this value is used for xTickSpacing. So this is the property which
         *         is editable by use of public access methods.
         */
        JKQTPPROPERTY(double, userTickSpacing);
        /** \brief if autoXAxisSpacing is \c false then this value is used for xTickSpacing. So this is the property which
         *         is editable by use of public access methods.
         */
        JKQTPPROPERTY(double, userLogTickSpacing);

        /** \brief indicates how to draw the labels */
        JKQTPPROPERTY(JKQTPCAlabelType, labelType);

        /** \brief mode of the major ticks */
        JKQTPPROPERTY(JKQTPLabelTickMode, tickMode);

        /** \brief axis label of the axis */
        QString axisLabel;

        /** \brief position of the axis label */
        JKQTPPROPERTY(JKQTPlabelPosition, labelPosition);
        /** \brief font of the axis labels */
        JKQTPPROPERTY(QString, labelFont);
        /** \brief fontsize of the axis labels */
        JKQTPPROPERTY(double, labelFontSize);
        /** \brief font of the axis tick labels */
        JKQTPPROPERTY(QString, tickLabelFont);
        /** \brief fontsize of the axis tick labels */
        JKQTPPROPERTY(double, tickLabelFontSize);
        /** \brief fontsize of the minor axis tick labels */
        JKQTPPROPERTY(double, minorTickLabelFontSize);
        /** \brief indicates whether to draw a thick axis line at x=0 (zero axis) */
        JKQTPPROPERTY(bool, showZeroAxis);
        /** \brief indicates whether the minor tick labels should be full numbers, or just a number between 0..10 */
        JKQTPPROPERTY(bool, minorTickLabelFullNumber);


        /** \brief draw mode of the main (left/bottom) axis */
        JKQTPPROPERTY(JKQTPCAdrawMode, drawMode1);
        /** \brief draw mode of the secondary (right/top) axis */
        JKQTPPROPERTY(JKQTPCAdrawMode, drawMode2);
        /** \brief line width of minor ticks in pixels */
        JKQTPPROPERTY(double, minorTickWidth);
        /** \brief line width of ticks in pixels */
        JKQTPPROPERTY(double, tickWidth);
        /** \brief line width of axis in pixels */
        JKQTPPROPERTY(double, lineWidth);
        /** \brief line width of 0-line in pixels */
        JKQTPPROPERTY(double, lineWidthZeroAxis);


        /** \brief format string for time tick labels */
        JKQTPPROPERTY(QString, tickTimeFormat);
        /** \brief format string for date tick labels */
        JKQTPPROPERTY(QString, tickDateFormat);
        /** \brief format string for datetime tick labels */
        JKQTPPROPERTY(QString, tickDateTimeFormat);

        /** \brief calculates the tick spacing for a linear axis that spans \a awidth and that should
         *         show at least \a minTicks ticks.
         *
         * The <b>algorithm</b> used in here works as follows:
         *
         * There is only a limited subset of allowed tick distances. These distances are 1 (resulting in 10 ticks per decade, i.e. 0,1,2,3,4,...),
         * 2 (5 ticks/decade, i.e. 0,2,4,...), 2.5 (4 ticks/decade, i.e. 0,2.5,5,...) and 5 (2 ticks/decade, i.e. 0,5,10,15,...). So the
         * axis is divided into decades that each is divided into ticks, as defined above. With these preconditions the algorithm tries
         * to find a tick increment \f$ \Delta\mbox{tick} \f$ which is one of the above distances multiplied by a power \f$ \rho \f$ of 10.
         * This increment is determined in a way, that the axis contains at least \a minTicks ticks:
         *   \f[ \Delta\mbox{tick}=f\cdot10^\rho,\ \ \ \ \ f\in\{1, 2, 2.5, 5\} \f]
         *
         *   -# The algorithm starts by finding a start exponent \f$ \rho_s=\left\lfloor\log_{10}(\mbox{awidth})\right\rfloor+3 \f$. This
         *      is a good (over)estimate for the biggest possible power \f$ \rho \f$.
         *   -# now we set \f$ \rho=\rho_0 \f$ and \f$ f=10 \f$.
         *   -# now the algorithm cycles through all possible factors \f$ f\in\{1, 2, 2.5, 5, 10\} \f$ (starting from 10) and calculates
         *      \f$ \Delta\mbox{tick}=f\cdot10^\rho \f$. Then it checks whether \f$ \mbox{tickcount}=\mbox{round}\left(\frac{\mbox{awidth}}{\Delta\mbox{tick}}\right) \f$
         *      is smaller than \a minTicks (if yes the algorithm reached its end and \f$ \Delta\mbox{tick} \f$ can be used.
         *   -# if \f$ f=1 \f$ is reached and checked \c false, then the algorithm decreases the exponent \f$ \rho \leftarrow \rho-1 \f$ and returns to step 3.
         *
         */
        double calcLinearTickSpacing();
        double calcLogTickSpacing();

        /** \brief Calculate the number of digits needed for the labels of an axis that starts at
         *         \a minval and where the tick spacing is \a tickSpacing.
         *
         * This method determines how many digits to output for the labels on a coordinate axis.
         * This is done by testing different digit numbers and comparing subsequent labels. If two labels
         * are equal, then we need more valid digits to distinguish them.
         */
        int calcLinearUnitDigits();

        /** \brief minimum number of axis ticks */
        JKQTPPROPERTY(unsigned int, minTicks);
        /** \brief number of minor grid lines per axis tick interval */
        JKQTPPROPERTY(unsigned int, minorTicks);
        /** \brief length of an axis tick outside the plot border  in pt */
        JKQTPPROPERTY(double, tickOutsideLength);
        /** \brief length of a minor axis tick outside the plot border in pt */
        JKQTPPROPERTY(double, minorTickOutsideLength);
        /** \brief length of an axis tick inside the plot border  in pt */
        JKQTPPROPERTY(double, tickInsideLength);
        /** \brief length of a minor axis tick inside the plot border in pt */
        JKQTPPROPERTY(double, minorTickInsideLength);
        /** \brief color of the axis (labels, ticks, axis itself ...) */
        JKQTPPROPERTY(QColor, axisColor);
        /** \brief distance between tick end and label start in pt */
        JKQTPPROPERTY(double, tickLabelDistance);
        /** \brief distance between tick label and axis label in pt */
        JKQTPPROPERTY(double, labelDistance);
        /** \brief rotation angle of tick labels [-180..180], i.e. given in degrees, default is 0 (horizontal) */
        JKQTPPROPERTY(double, tickLabelAngle);

        /** \brief indicates whether to draw the major grid lines */
        JKQTPPROPERTY(bool, drawGrid);
        /** \brief indicates whether to draw the minor grid lines */
        JKQTPPROPERTY(bool, drawMinorGrid);
        /** \brief color of the grid*/
        JKQTPPROPERTY(QColor, gridColor);
        /** \brief color of the minor grid lines */
        JKQTPPROPERTY(QColor, minorGridColor);
        /** \brief width of the grid lines (in pixel) */
        JKQTPPROPERTY(double, gridWidth);
        /** \brief line stye of the grid lines */
        JKQTPPROPERTY(Qt::PenStyle, gridStyle);
        /** \brief width of the minor grid lines (in pixel) */
        JKQTPPROPERTY(double, minorGridWidth);
        /** \brief line stye of the minor grid lines */
        JKQTPPROPERTY(Qt::PenStyle, minorGridStyle);

        /** \brief axis prefix for storage of parameters */
        QString axisPrefix;
        /** \brief this is used by x2p() and p2x() to determine the sign */
        double scaleSign;

        /** \brief calculates the next label from the given parameters.
         *
         *  \return \c true on success and \c false if there is no more label
         *  \param x \b before \b call: the position of the label for which the text should be returned (or tickStart for first call),
         *           \b after \b call: position of the next label (given in world coordinates!)
         *  \param label \b after \b call: text of the (input) tick label This is an empty string, if the label is not visible!
         *  \param init call this function with \c init=true to obtain the first label (at tickStart)
         */
        bool getNextLabel(double& x, QString& label, bool init=false);

        /** \brief returns the distance from the current tick position (x before call) to the next tick position */
        double getNextLabelDistance(double x);

        /** \brief calculates the maximum width and height (returned as QSize) of all tick labels.
         *         Ascent and descent may also be returned in the two additional pointer arguments- */
        QSizeF getMaxTickLabelSize(JKQTPEnhancedPainter& painter, double* ascent=NULL, double* descent=NULL);
};





/*! \brief implements a vertical axis, based on JKQTPcoordinateAxis (for most of documentation: see JKQTPcoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

 */
class LIB_EXPORT JKQTPverticalAxis: public JKQTPcoordinateAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPverticalAxis(JKQtBasePlotter* parent);

        /** \brief returns the size of the left/bottom axis in pixels */
        virtual QSize getSize1(JKQTPEnhancedPainter& painter);

        /** \brief returns the size of the right/top axis in pixels */
        virtual QSize getSize2(JKQTPEnhancedPainter& painter);

        /** \brief draw axes */
        virtual void drawAxes(JKQTPEnhancedPainter& painter);

        /** \brief draw grids  */
        virtual void drawGrids(JKQTPEnhancedPainter& painter);


    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const;
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const;
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const;
        virtual bool get_parent_otheraxis_inverted() const;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const;

        virtual void drawTickLabel1(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize);
        virtual void drawTickLabel2(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize);
};


/*! \brief implements a position-indipendent vertical axis, based on JKQTPcoordinateAxis (for most of documentation: see JKQTPcoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    This axis may be draw at a user-supplied position (used e.g. for color bar axes)
 */
class LIB_EXPORT JKQTPverticalIndependentAxis: public JKQTPverticalAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPverticalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQtBasePlotter* parent);
        /** \brief set the axis offset */
        inline virtual void set_axisOffset(double __value) {
            this->axisOffset = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief set the axis width */
        inline virtual void set_axisWidth(double __value) {
            this->axisWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief set the other axis offset */
        inline virtual void set_otherAxisOffset(double __value) {
            this->otherAxisOffset = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief set the other axis width */
        inline virtual void set_otherAxisWidth(double __value) {
            this->otherAxisWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief set the other axis width */
        inline virtual void set_otherAxisInverted(bool __value) {
            this->otherAxisInverted = __value;
            this->paramsChanged=true;
            update_plot();
        };
    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const { return axisWidth; }
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const { return axisOffset; }
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const { return otherAxisWidth; }
        virtual bool get_parent_otheraxis_inverted() const { return otherAxisInverted; }
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const { return otherAxisOffset; }

        double axisOffset;
        double axisWidth;
        double otherAxisWidth;
        double otherAxisOffset;
        bool otherAxisInverted;
};



/*! \brief implements a horizontal axis, based on JKQTPcoordinateAxis (for most of documentation: see JKQTPcoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

 */
class LIB_EXPORT JKQTPhorizontalAxis: public JKQTPcoordinateAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPhorizontalAxis(JKQtBasePlotter* parent);

        /** \brief returns the size of the left/bottom axis in pixels */
        virtual QSize getSize1(JKQTPEnhancedPainter& painter);

        /** \brief returns the size of the right/top axis in pixels */
        virtual QSize getSize2(JKQTPEnhancedPainter& painter);

        /** \brief draw axes */
        virtual void drawAxes(JKQTPEnhancedPainter& painter);

        /** \brief draw grids  */
        virtual void drawGrids(JKQTPEnhancedPainter& painter);

    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const;
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const;
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const;
        virtual bool get_parent_otheraxis_inverted() const;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const;


        virtual void drawTickLabel1(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize, double ascentMax, double descentMax);
        virtual void drawTickLabel2(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize, double ascentMax, double descentMax);

};


/*! \brief implements a position-indipendent horizontal axis, based on JKQTPcoordinateAxis (for most of documentation: see JKQTPcoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    This axis may be draw at a user-supplied position (used e.g. for color bar axes)
 */
class LIB_EXPORT JKQTPhorizontalIndependentAxis: public JKQTPhorizontalAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPhorizontalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQtBasePlotter* parent);
        /** \brief se the axis offset */
        inline virtual void set_axisOffset(double __value) {
            this->axisOffset = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief se the axis width */
        inline virtual void set_axisWidth(double __value) {
            this->axisWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief set the other axis offset */
        inline virtual void set_otherAxisOffset(double __value) {
            this->otherAxisOffset = __value;
            this->paramsChanged=true;
            update_plot();
        };
        /** \brief set the other axis width */
        inline virtual void set_otherAxisWidth(double __value) {
            this->otherAxisWidth = __value;
            this->paramsChanged=true;
            update_plot();
        };
        inline virtual void set_otherAxisInverted(bool __value) {
            this->otherAxisInverted = __value;
            this->paramsChanged=true;
            update_plot();
        };
    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const { return axisWidth; }
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const { return axisOffset; }
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const { return otherAxisWidth; }
        virtual bool get_parent_otheraxis_inverted() const { return otherAxisInverted; }
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const { return otherAxisOffset; }

        double axisOffset;
        double axisWidth;
        double otherAxisWidth;
        double otherAxisOffset;
        bool otherAxisInverted;
};

#endif // JKQTPBASEELEMENTS_H
