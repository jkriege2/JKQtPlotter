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




#ifndef JKQTPCOORDINATEAXES_H
#define JKQTPCOORDINATEAXES_H


#include <QString>
#include <QPainter>
#include <QPair>
#include <QSettings>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpcoordinateaxesstyle.h"

// forward declarations
class JKQTBasePlotter;

/*! \brief named references for different oordinate axes in the plot
    \ingroup jkqtpplottersupprt
*/
enum JKQTPCoordinateAxes: uint8_t {
    JKQTPPrimaryAxis=0,
    JKQTPSecondaryAxis=1
};

/*! \brief type for indexing coordinate axes in a plot
    \ingroup jkqtpplottersupprt
*/
typedef JKQTPCoordinateAxes JKQTPCoordinateAxisRef;

/*! \brief this virtual class is the base for any type of coordinate axis, to be drawn by JKQTBasePlotter.
    \ingroup jkqtpbaseplotter_elements

    Classes derived from JKQTPCoordinateAxis implements all the functionality needed for a coordinate axis:
      - transform world to screen coordinates and vice versa
      - draw the axis (implemented by child classes!) with these elements: axis lines, ticks, tick labels, axis label, x/y=0 axis
      - measure the axes in screen coordinates
      - load and save the settings to an ini file
    .

    Most of the actual draw and measure functions have to be implemented in descendents of this class (namely
    JKQTPHorizontalAxis and JKQTPVerticalAxis, as they are specific to whether the axis is drawn horizontally or
    vertically.

    Each axis is split up into several parts, as depicted in this image:

    \image html jkqtpcoordinateaxis1.png

    Which parts to actually draw is set by the diverse properties.


    \section jkqtplotter_base_systems_baseelements Coordinate Systems and Transformations
    The plot itself is positioned inside the widget (see \ref jkqtplotter_base_plotsize for details).
    You simply supply the widget dimensions and the border
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


    \section jkqtplotter_coordinateaxes_inverted Inverted Coordinate Axes

    In some cases it may be necessary to invert the direction of increasing coordinates on an axis. One such case is image analysis, as computer images usually
    have coordinates starting with (0,0) at the top left and increasing to the right (x) and down (y). You can invert any axis by setting \c setInverted(true).

    \image html jkqtplotter_inverted_yaxis.png

    \see You can find example here: \ref JKQTPlotterImagePlotQImageRGB and \ref JKQTPlotterImagePlotRGBOpenCV


    \section jkqtplotter_base_grids_baseelemenets Axis Ticks and Grids
    This section explains how this component draws the ticks on coordinate axes and the grids that may be drawn below
    the plots. In principle both - grids and axes - are drawn the same way, i.e. with the same step widths. There are
    two types of ticks and grids: The major and the minor ticks/grids. The major ticks also show a label that denotes the
    value they represent. Between two major ticks the axis shows \a JKQTPCoordinateAxisStyle::minorTicks  small ticks that are not
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

    The minor ticks and grid lines are generated automatically, depending in the setting of \a JKQTPCoordinateAxisStyle::minorTicks.
    These properties give the number of minor ticks between two major ticks, so if the major ticks are at 1,2,3,... and you
    want minor ticks at 1.1, 1.2, 1.3,... then you will have to set \c JKQTPCoordinateAxisStyle::minorTicks=9 as there are nine ticks between two major
    ticks. So the minor tick spacing is calculated as: \f[ \Delta\mbox{MinorTicks}=\frac{\Delta\mbox{ticks}}{\mbox{minorTicks}+1} \f]

    The same applies for logarithmic axes. If the major ticks are at 1,10,100,... and you set \c JKQTPCoordinateAxisStyle::minorTicks=9 the program will
    generate 9 equally spaced minor ticks in between, so you have minor ticks at 2,3,4,...,11,12,13,... This results in a standard
    logarithmic axis. If you set \c JKQTPCoordinateAxisStyle::minorTicks=1 then you will get minor ticks at 5,15,150,...

      \image html plot_logaxis_ticksandlabels.png

    The major tick-tick distances of linear axes may be calculated automatically in a way that the axis shows at least a given
    number of ticks \c JKQTPCoordinateAxisStyle::minTicks. The algorithm takes that tick spacing that will give a number of ticks per axis
    nearest but \c ">=" to the given \c JKQTPCoordinateAxisStyle::minTicks. The Algorithm is described in detail with the function
    calcLinearTickSpacing(). To activate this automatic tick spacing you have to set <code>autoAxisSpacing=true</code>.

    \section jkqtplotter_coordinateaxes_tickscaling Axis Tick Units/Scaling
    In some cases it is desired to put the axis ticks not at 1,2,3,... but rather at \f$ 1\pi \f$ , \f$ 2\pi \f$ , \f$ 3\pi \f$  or any other
    unit than \f$ pi \f$ ,i.e.:

      \image html axisstyle/axis_unit_scaling_none.png "no axis scaling (default case)"
      \image html axisstyle/axis_unit_scaling_pi.png "pi-axis scaling"

    You can use these methods to set such a factor:
      - JKQTPCoordinateAxis::setTickUnitFactor() for the actual factor and setTickUnitName() for a name, added to the tick label
      - JKQTPCoordinateAxis::setTickUnit() sets factor and name in one call
      - JKQTPCoordinateAxis::setTickUnitPi() shortcut to set pi-scaling
      - JKQTPCoordinateAxis::resetTickUnit() resets to no-scaling (default case)
    .

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPCoordinateAxis: public QObject {
        Q_OBJECT
    protected:
    public:

        /** \brief class constructor */
        explicit JKQTPCoordinateAxis(JKQTBasePlotter* parent);
        /** \brief class destructor */
        virtual ~JKQTPCoordinateAxis();
        virtual void setParent(JKQTBasePlotter* parent);
        /** \brief loads the plot properties from a QSettings object */
        virtual void loadSettings(const QSettings &settings, const QString& group=QString("plots/axes/"));

        /** \brief saves the plot properties into a QSettings object.
         *
         * This method only saves those properties that differ from their default value.
         */
        virtual void saveSettings(QSettings& settings, const QString& group=QString("plots/axes/")) const;


        /** \brief return x-pixel coordinate from x coordinate */
        inline double x2p(double x) const {
            double r=0;
            if (logAxis) {
                if (x<=0) r= offset+scaleSign*log(axismin)/log(logAxisBase)*scale;
                else r= offset+scaleSign*log(x)/log(logAxisBase)*scale;
            } else {
                r= offset+scaleSign*x*scale;
            }
            if (inverted) {
                return 2.0*getParentPlotOffset()+getParentPlotWidth()-r;//getParentPlotOffset()+getParentPlotWidth()-(r-getParentPlotOffset());
            } else {
                return r;
            }
        }

        /** \brief return x coordinate from x-pixel */
        inline double p2x(double x) const {
            double xx=x;
            if (inverted) {
                xx=2.0*getParentPlotOffset()+getParentPlotWidth()-x;
            }
            xx=xx-offset;
            if (logAxis) {
                return exp(log(logAxisBase)*(xx)/(scaleSign*scale));
            } else {
                return xx/(scaleSign*scale);
            }
        }


        /** \brief clear axis tick labels. This switches back to automatic labels mode for the axis. */
        void clearAxisTickLabels();

        /** \brief add a new tick label to the axis */
        void addAxisTickLabel(double x, const QString& label);


        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const QVector<double>& x, const QStringList& label);
        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const double* x, const QStringList& label);
        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const QVector<double>& x, const QString* label);
        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const double* x, const QString* label, int items);


        /** \brief return value type for getSize1() and getSize2() */
        struct JKQTPLOTTER_LIB_EXPORT AxisElementsSizeDescription {
            inline AxisElementsSizeDescription(double _requiredSize=0.0, double _elongateMin=0.0, double _elongateMax=0.0): requiredSize(_requiredSize), elongateMin(_elongateMin), elongateMax(_elongateMax) {}

            /** \brief required space, starting at the axis line, in outward direction [pixels] */
            double requiredSize;
            /** \brief extra space on the min (horizontal: left, vertical: bottom) side BESIDE the axis, e.g. when labels elongate to the left/right or top/bottom of the plot rectangle [pixels] */
            double elongateMin;
            /** \brief extra space on the max (horizontal: right, vertical: top) side BESIDE the axis, e.g. when labels elongate to the left/right or top/bottom of the plot rectangle [pixels] */
            double elongateMax;
        };

        /** \brief returns the sizerequirement of the left/bottom axis in pixels
         *
         *  \param painter a JKQTPEnhancedPainter to use for determining sizes (the same that would be used for drawing!)
         *
         *  \returns size of the additional space outside the plot rectangle, required for the axis.
         */
        virtual AxisElementsSizeDescription getSize1(JKQTPEnhancedPainter& painter)=0;

        /** \brief returns the size of the right/top axis in pixels
         *
         *  \param painter a JKQTPEnhancedPainter to use for determining sizes (the same that would be used for drawing!)
         *
         *  \returns size of the additional space outside the plot rectangle, required for the axis.
         */
        virtual AxisElementsSizeDescription getSize2(JKQTPEnhancedPainter& painter)=0;

        /** \brief draw the axes
         *
         *  \param painter the painter to use for drawing
         *  \param move1 offset in pixels on the major side (horizontal: bottom, vertical: left)
         *  \param move2 offset in pixels on the secondary side (horizontal: top, vertical: right)
         */
        virtual void drawAxes(JKQTPEnhancedPainter& painter, int move1=0, int move2=0)=0;

        /** \brief draw grids  */
        virtual void drawGrids(JKQTPEnhancedPainter& painter)=0;


        /** \copydoc tickSpacing */
        inline double getTickSpacing() const { return this->tickSpacing; }
        /** \copydoc JKQTPCoordinateAxisStyle::labelDigits */
        inline int getLabelDigits() const { return this->axisStyle.labelDigits; }
        /** \copydoc autoAxisSpacing */
        inline bool getAutoAxisSpacing() const { return this->autoAxisSpacing; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelsEnabled */
        inline bool getMinorTickLabelsEnabled() const { return this->axisStyle.minorTickLabelsEnabled; }
        /** \copydoc logAxis */
        inline bool getLogAxis() const { return this->logAxis; }
        /** \copydoc inverted */
        inline bool getInverted() const { return this->inverted; }
        /** \copydoc logAxisBase */
        inline double getLogAxisBase() const { return this->logAxisBase; }
        /** \copydoc userTickSpacing */
        inline double getUserTickSpacing() const { return this->userTickSpacing; }
        /** \copydoc userLogTickSpacing */
        inline double getUserLogTickSpacing() const { return this->userLogTickSpacing; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelType */
        inline JKQTPCALabelType getTickLabelType() const { return this->axisStyle.tickLabelType; }
        /** \copydoc axisLabel */
        inline QString getAxisLabel() const { return this->axisLabel; }
        /** \copydoc tickUnitFactor */
        inline double getTickUnitFactor() const { return this->tickUnitFactor; }
        /** \copydoc tickUnitName */
        inline QString getTickUnitName() const { return this->tickUnitName; }
        /** \copydoc JKQTPCoordinateAxisStyle::labelPosition */
        inline JKQTPLabelPosition getLabelPosition() const { return this->axisStyle.labelPosition; }
        /** \copydoc JKQTPCoordinateAxisStyle::labelFontSize */
        inline double getLabelFontSize() const { return this->axisStyle.labelFontSize; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelFontSize */
        inline double getTickLabelFontSize() const { return this->axisStyle.tickLabelFontSize; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelFontSize */
        inline double getMinorTickLabelFontSize() const { return this->axisStyle.minorTickLabelFontSize; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelFullNumber */
        inline bool getMinorTickLabelFullNumber() const { return this->axisStyle.minorTickLabelFullNumber; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelAngle */
        inline double getTickLabelAngle() const { return this->axisStyle.tickLabelAngle; }
        /** \copydoc JKQTPCoordinateAxisStyle::minTicks */
        inline unsigned int getMinTicks() const { return this->axisStyle.minTicks; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTicks */
        inline unsigned int getMinorTicks() const { return this->axisStyle.minorTicks; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickOutsideLength */
        inline double getTickOutsideLength() const { return this->axisStyle.tickOutsideLength; }
        /** \copydoc JKQTPCoordinateAxisStyle::arrowSizeFactor */
        inline double getArrowSizeFactor() const { return this->axisStyle.arrowSizeFactor; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickOutsideLength */
        inline double getMinorTickOutsideLength() const { return this->axisStyle.minorTickOutsideLength; }
        /** \copydoc JKQTPCoordinateAxisStyle::axisColor */
        inline QColor getAxisColor() const { return this->axisStyle.axisColor; }
        /** \copydoc JKQTPCoordinateAxisStyle::labelColor */
        inline QColor getLabelColor() const { return this->axisStyle.labelColor; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickColor */
        inline QColor getMinorTickColor() const { return this->axisStyle.minorTickColor; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelColor */
        inline QColor getMinorTickLabelColor() const { return this->axisStyle.minorTickLabelColor; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickColor */
        inline QColor getTickColor() const { return this->axisStyle.tickColor; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelColor */
        inline QColor getTickLabelColor() const { return this->axisStyle.tickLabelColor; }
        /** \copydoc JKQTPCoordinateAxisStyle::showZeroAxis */
        inline bool getShowZeroAxis() const { return this->axisStyle.showZeroAxis; }
        /** \copydoc JKQTPGridStyle::lineColor */
        inline QColor getGridColor() const { return this->axisStyle.majorGridStyle.lineColor; }
        /** \copydoc JKQTPGridStyle::lineColor */
        inline QColor getMinorGridColor() const { return this->axisStyle.minorGridStyle.lineColor; }
        /** \copydoc JKQTPGridStyle::lineWidth */
        inline double getGridWidth() const { return this->axisStyle.majorGridStyle.lineWidth; }
        /** \copydoc JKQTPGridStyle::lineStyle */
        inline Qt::PenStyle getGridStyle() const { return this->axisStyle.majorGridStyle.lineStyle; }
        /** \copydoc JKQTPGridStyle::lineWidth */
        inline double getMinorGridWidth() const { return this->axisStyle.minorGridStyle.lineWidth; }
        /** \copydoc JKQTPGridStyle::lineStyle */
        inline Qt::PenStyle getMinorGridStyle() const { return this->axisStyle.minorGridStyle.lineStyle; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickTimeFormat */
        inline QString getTickTimeFormat() const { return this->axisStyle.tickTimeFormat; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickDateFormat */
        inline QString getTickDateFormat() const { return this->axisStyle.tickDateFormat; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickDateTimeFormat */
        inline QString getTickDateTimeFormat() const { return this->axisStyle.tickDateTimeFormat; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickPrintfFormat */
        inline QString getTickPrintfFormat() const { return this->axisStyle.tickPrintfFormat; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickMode */
        inline JKQTPLabelTickMode getTickMode() const { return this->axisStyle.tickMode; }
#if __cplusplus >= 202002L
# ifdef __cpp_lib_format
        /** \copydoc JKQTPCoordinateAxisStyle::tickFormatFormat */
        inline QString getTickFormatfFormat() const { return this->axisStyle.tickFormatFormat; }
# endif
#endif

        /** \copydoc JKQTPCoordinateAxisStyle::drawMode0 */
        inline JKQTPCADrawMode getDrawMode0() const { return this->axisStyle.drawMode0; }
        /** \copydoc JKQTPCoordinateAxisStyle::drawMode1 */
        inline JKQTPCADrawMode getDrawMode1() const { return this->axisStyle.drawMode1; }
        /** \copydoc JKQTPCoordinateAxisStyle::drawMode2 */
        inline JKQTPCADrawMode getDrawMode2() const { return this->axisStyle.drawMode2; }
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickWidth */
        inline double getMinorTickWidth() const { return this->axisStyle.minorTickWidth; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickWidth */
        inline double getTickWidth() const { return this->axisStyle.tickWidth; }
        /** \copydoc JKQTPCoordinateAxisStyle::lineWidth */
        inline double getLineWidth() const { return this->axisStyle.lineWidth; }
        /** \copydoc JKQTPCoordinateAxisStyle::lineWidthZeroAxis */
        inline double getLineWidthZeroAxis() const { return this->axisStyle.lineWidthZeroAxis; }
        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelDistance */
        inline double getTickLabelDistance() const { return this->axisStyle.tickLabelDistance; }
        /** \copydoc JKQTPCoordinateAxisStyle::labelDistance */
        inline double getLabelDistance() const { return this->axisStyle.labelDistance; }
        /** \copydoc JKQTPGridStyle::enabled */
        inline bool getDrawGrid() const { return this->axisStyle.majorGridStyle.enabled; }
        /** \copydoc JKQTPGridStyle::enabled */
        inline bool getDrawMinorGrid() const { return this->axisStyle.minorGridStyle.enabled; }
        /** \copydoc JKQTPCoordinateAxisStyle::autoLabelDigits */
        inline void setAutoLabelDigits(bool __value)
        {
            this->axisStyle.autoLabelDigits = __value;
        } 
        /** \copydoc JKQTPCoordinateAxisStyle::autoLabelDigits */
       inline bool getAutoLabelDigits() const
        {
            return this->axisStyle.autoLabelDigits;
        }
        /** \copydoc parent */
        inline const JKQTBasePlotter* getParent() const { return this->parent; }
        /** \copydoc parent */
        inline JKQTBasePlotter* getParent()  { return this->parent; }
        /** \copydoc doUpdateScaling */ 
        inline void setDoUpdateScaling(bool __value)
        {
            this->doUpdateScaling = __value;
        } 
        /** \copydoc doUpdateScaling */ 
        inline bool getDoUpdateScaling() const
        {
            return this->doUpdateScaling; 
        }


        /** \brief returns the current min */
        inline double getMin() const {return axismin; }

        /** \brief returns the current max */
        inline double getMax() const {return axismax; }

        /** \brief returns the current absolute min */
        inline double getAbsoluteMin() const {return axisabsoultemin; }

        /** \brief returns the current absolute max */
        inline double getAbsoluteMax() const {return axisabsoultemax; }
        /** \brief calculate the scaling and offset values from axis min/max values
         *
         *  This is only executed when \c paramChanged==true, as otherwise the data has to be still
         *  correct. recalculating may be forced by caling calcPlotScaing(true);
         */
        void calcPlotScaling(bool force=false);

        /** \brief returns whether this axis uses logarithmic scaling */
        bool isLogAxis() const;

        /** \brief returns whether this axis uses linear scaling (is false e.g. for isLogAxis()==true) */
        bool isLinearAxis() const;


        /** \brief current style properties for this JKQTBasePlotter
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), setCurrentAxisStyle(), getCurrentAxisStyle(), \ref jkqtpplotter_styling
         */
        const JKQTPCoordinateAxisStyle &getCurrentAxisStyle() const;

        /** \brief replace the current style properties for this JKQTBasePlotter
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), setCurrentAxisStyle(), getCurrentAxisStyle(), \ref jkqtpplotter_styling
         */
        void setCurrentAxisStyle(const JKQTPCoordinateAxisStyle& style);
        /** \brief replace the current style properties for this JKQTBasePlotter with properties loaded from \a settings
         *
         * \param settings the QSettings object to read from
         * \param group group in \a settings to read from
         *
         * \see JKQTPCoordinateAxisStyle getCurrentAxisStyle(), \ref jkqtpplotter_styling
         */
        void loadCurrentAxisStyle(const QSettings& settings, const QString& group="plot/axes/");
        /** \brief store the current style properties for this JKQTBasePlotter with properties loaded from \a settings
         *
         * \param settings the QSettings object to write to
         * \param group group in \a settings to write to
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), setCurrentAxisStyle(), getCurrentAxisStyle(), \ref jkqtpplotter_styling
         */
        void saveCurrentAxisStyle(QSettings& settings, const QString& group="plot/axes/") const;

        /** \brief width of the plot in the direction of the axis */
        virtual double getParentPlotWidth() const=0;
        /** \brief offset of the plot in the direction of the axis */
        virtual double getParentPlotOffset() const=0;
        /** \brief width of other (perpendicular) axis (needed for grids) */
        virtual double getParentOtheraxisWidth() const=0;
        /** \brief is other (perpendicular) axis inverted (needed for grids) */
        virtual bool getParentOtheraxisInverted() const=0;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double getParentOtheraxisOffset() const=0;
        /** \brief calls x2p() of the other axis (or returns \c NAN if the other axis does not exist */
        virtual double parentOtherAxisX2P(double x) const =0;

    public Q_SLOTS:
        /** \brief set range of plot axis */
        void setRange(double amin, double amax);
        /** \brief sets absolutely limiting range of the plot
         *
         * The user (or programmer) cannot zoom to a viewport that is larger than the range given to this function.
         *
         *  \param aamin absolute minimum of the axis
         *  \param aamax absolute maximum of the axis
         *
         * \note if the aspect ratio (set in the JKQTBasePlotter or JKQTPlotter) of this does not fit into the widget, it is possible that you don't see the complete contents!
         *
         * \see setAbsoluteX(), setAbsoluteY(), zoomToFit(), JKQTPCoordinateAxis::setAbsoluteRange()
         */
        void setAbsoluteRange(double aamin, double aamax);
        /** \brief do not use an absolute range of plot axis */
        void setNoAbsoluteRange();

        /** \copydoc tickSpacing */
        void setTickSpacing(double __value);

        /** \copydoc axisMinWidth */
        void setAxisMinWidth(double __value);

        /** \copydoc autoAxisSpacing */
        void setAutoAxisSpacing(bool __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelsEnabled */
        void setMinorTickLabelsEnabled(bool __value);

        /** \copydoc logAxis */
        void setLogAxis(bool __value) ;

        /** \copydoc logAxisBase */
        void setLogAxisBase (double __value);

        /** \copydoc userTickSpacing */
        void setUserTickSpacing (double __value);

        /** \copydoc userLogTickSpacing */
        void setUserLogTickSpacing (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelType */
        void setTickLabelType (JKQTPCALabelType __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickMode */
        void setTickMode (JKQTPLabelTickMode __value);
        /** \copydoc JKQTPCoordinateAxisStyle::tickMode */
        void setTickMode (int __value);

        /** \copydoc axisLabel */
        void setAxisLabel (const QString& __value);

        /** \copydoc tickUnitName */
        void setTickUnitName(const QString& __value);

        /** \copydoc tickUnitFactor */
        void setTickUnitFactor(double __value);
        /** \brief sets tickUnitFactor and tickUnitName in one call
         *
         *  \see calls setTickUnitName() and setTickUnitFactor()
         */
        void setTickUnit(double factor, const QString& name);
        /** \brief sets pi-scaling for tickUnitFactor and tickUnitName in one call
         *
         *  \image html axisstyle/axis_unit_scaling_pi.png
         *  \see calls setTickUnitName() and setTickUnitFactor()
         */
        void setTickUnitPi();
        /** \brief resets tickUnitFactor and tickUnitName in one call
         *
         *  \see calls setTickUnit(), setTickUnitName() and setTickUnitFactor()
         */
        void resetTickUnit();

        /** \copydoc JKQTPCoordinateAxisStyle::labelPosition */
        void setLabelPosition (JKQTPLabelPosition __value);

        /** \copydoc JKQTPCoordinateAxisStyle::labelFontSize */
        void setLabelFontSize (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickTimeFormat */
        void setTickTimeFormat (const QString& __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickDateFormat */
        void setTickDateFormat (const QString& __value);


        /** \copydoc JKQTPCoordinateAxisStyle::tickDateTimeFormat */
        void setTickDateTimeFormat (const QString& __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickPrintfFormat */
        void setTickPrintfFormat(const QString& __value);
#if __cplusplus >= 202002L
# ifdef __cpp_lib_format
        /** \copydoc JKQTPCoordinateAxisStyle::tickFormatFormat */
        void setTickFormatFormat(const QString& __value);
# endif
#endif
        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelFontSize */
        void setTickLabelFontSize (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelFontSize */
        void setMinorTickLabelFontSize (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelFullNumber */
        void setMinorTickLabelFullNumber (bool __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minTicks */
        void setMinTicks(unsigned int __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTicks */
        void setMinorTicks (unsigned int __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTicks */
        void setMinorTicks (int __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickOutsideLength */
        void setTickOutsideLength(double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTickOutsideLength */
        void setMinorTickOutsideLength (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickInsideLength */
        void setTickInsideLength(double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTickInsideLength */
        void setMinorTickInsideLength (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::axisColor */
        void setAxisColor (const QColor& __value);

        /** \copydoc JKQTPCoordinateAxisStyle::showZeroAxis */
        void setShowZeroAxis(bool __value);

        /** \copydoc inverted */
        void setInverted(bool __value);

        /** \copydoc JKQTPGridStyle::lineColor */
        void setGridColor(const QColor& __value);
        /** \copydoc JKQTPGridStyle::lineColor */
        void setGridColor(const QColor& __value, double alpha);

        /** \copydoc JKQTPGridStyle::lineColor */
        void setMinorGridColor(const QColor& __value);
        /** \copydoc JKQTPGridStyle::lineColor */
        void setMinorGridColor(const QColor& __value, double alpha);

        /** \copydoc JKQTPGridStyle::lineWidth */
        void setGridWidth (double __value);

        /** \copydoc JKQTPGridStyle::lineStyle */
        void setGridStyle(Qt::PenStyle __value);

        /** \copydoc JKQTPGridStyle::lineWidth */
        void setMinorGridWidth(double __value);

        /** \copydoc JKQTPGridStyle::lineStyle */
        void setMinorGridStyle (Qt::PenStyle __value);


        /** \copydoc JKQTPCoordinateAxisStyle::drawMode1 */
        void setDrawMode1 (JKQTPCADrawMode __value);
        /** \copydoc JKQTPCoordinateAxisStyle::drawMode0 */
        void setDrawMode0 (JKQTPCADrawMode __value);

        /** \copydoc JKQTPCoordinateAxisStyle::drawMode2 */
        void setDrawMode2(JKQTPCADrawMode __value);

        /** \copydoc JKQTPCoordinateAxisStyle::minorTickWidth */
        void setMinorTickWidth(double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickWidth */
        void setTickWidth (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::lineWidth */
        void setLineWidth (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::lineWidthZeroAxis */
        void setLineWidthZeroAxis (double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelDistance */
        void setTickLabelDistance(double __value);

        /** \copydoc JKQTPCoordinateAxisStyle::labelDistance */
        void setLabelDistance(double __value);
        /** \copydoc JKQTPCoordinateAxisStyle::labelDigits */
        void setLabelDigits(int __value);

        /** \copydoc JKQTPGridStyle::enabled */
        void setDrawGrid(bool __value);

        /** \copydoc JKQTPGridStyle::enabled */
        void setDrawMinorGrid(bool __value);


        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelAngle */
        void setTickLabelAngle(double __value);
        /** \copydoc JKQTPCoordinateAxisStyle::arrowSizeFactor */
        void setArrowSizeFactor(double f) ;
        /** \copydoc JKQTPCoordinateAxisStyle::labelColor */
        void setLabelColor(QColor c) ;
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickColor */
        void setMinorTickColor(QColor c) ;
        /** \copydoc JKQTPCoordinateAxisStyle::minorTickLabelColor */
        void setMinorTickLabelColor(QColor c) ;
        /** \copydoc JKQTPCoordinateAxisStyle::tickColor */
        void setTickColor(QColor c);
        /** \copydoc JKQTPCoordinateAxisStyle::tickLabelColor */
        void setTickLabelColor(QColor c) ;
        /** \brief sets all colors (line, ticks, label, ...) of the axis */
        void setColor(QColor c) ;

    protected:
        struct JKQTPLOTTER_LIB_EXPORT Axis0ElementsSizeDescription: public AxisElementsSizeDescription {
            inline Axis0ElementsSizeDescription(double _requiredSize=0.0, double _requiredSizeOpposite=0.0, double _elongateMin=0.0, double _elongateMax=0.0): AxisElementsSizeDescription(_requiredSize, _elongateMin, _elongateMax), requiredSizeOpposite(_requiredSizeOpposite) {}
            /** \brief additional size, required for the axis, in the opposite direction as AxisElementsSizeDescription::requiredSize */
            double requiredSizeOpposite;
            inline double maxRequiredSize() const {
                return qMax(requiredSize,requiredSizeOpposite);
            }
        };

        /** \brief returns the size of the zero axis in pixels, the first part of the return-value is the lhs size and the second part the rhs size */
        virtual Axis0ElementsSizeDescription getSize0(JKQTPEnhancedPainter& painter) ;
        /** \brief indicates whether one of the parameters has changed sinse the last recalculation of tickSpacing ... */
        bool paramsChanged;
        /** \brief can be used to switch off calcPlotScaling() temporarily, while modifying some properties
         *
         *  use setDoUpdateScaling() to set this property
         *
         *  \see setDoUpdateScaling() and getDoUpdateScaling()
         */
        bool doUpdateScaling;

        /** \brief simply calls the redrawPlot method of the parent plotter class */
        void redrawPlot();
        /** \brief a list of tick labels.
         *
         *  If this list contains items, this class will NOT plot a standard x-axis,
         * but only mark the positions in this list. Every item is a x-position together with
         * the label to be plotted there. The label may contain LaTeX markup.
         */
        QVector<QPair<double, QString> > tickLabels;
        /** \brief retun parents JKQTMathText* object */
        JKQTMathText* getParentMathText();
        /** \brief retun parents JKQTMathText* object */
        const JKQTMathText* getParentMathText() const;


        /** \brief convert a float to a string using \a format (\c f|e|E|g|G ) with given number of decimals after comma \a past_comma and optional removal of trailing zeros behind decimal separator \a remove_trail0. Uses current local, disables not use Group-Separator */
        QString floattostringWithFormat(const QLocale & loc, double data, char format, int past_comma, bool remove_trail0=true) const;
        /** \brief convert a float to a string using \a format (\c f|e|E|g|G ) with given number of decimals after comma \a past_comma and optional removal of trailing zeros behind decimal separator \a remove_trail0. Uses current local, disables not use Group-Separator */
        QString floattostringWithFormat(double data, char format, int past_comma, bool remove_trail0=true) const;
        /** \brief convert a float to a tick label string */
        QString floattolabel(double data) const;

        /** \brief convert a float to a tick label string with a given precision */
        QString floattolabel(double data, int past_comma) const;
        /** \brief parent plotter class */
        JKQTBasePlotter* parent;
        /** \brief current view: minimum of axis */
        double axismin;
        /** \brief current view: maximum of axis */
        double axismax;

        /** \brief absoulte minimum of axis (axismin/axismax xan not be set below this) */
        double axisabsoultemin;
        /** \brief absoulte maximum of axis (axismin/axismax xan not be set above this) */
        double axisabsoultemax;


        /** \brief current style properties for this coordinate axis instance
         *
         * \see JKQTBasePlotterStyle, JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), setCurrentAxisStyle(), getCurrentAxisStyle(), \ref jkqtpplotter_styling
         */
        JKQTPCoordinateAxisStyle axisStyle;

        /** \brief absolute minimum range width, feature switched off when <0 */
        double axisMinWidth;

        /** \brief <b>calculated property:</b> width of plot on axis (calculated by calcPlotScaling() )
         *
         * \see calcPlotScaling(), calcTickSpacing()
         */
        double width;

        /** \brief <b>calculated property:</b> axis scaling factor (calculated by calcPlotScaling() )
         *
         * \see calcPlotScaling(), calcTickSpacing()
         */
        double scale;
        /** \brief <b>calculated property:</b> axis offset (calculated by calcPlotScaling() )
         *
         * \see calcPlotScaling(), calcTickSpacing()
         */
        double offset;
        /** \brief indicates whether the axis is to be inverted or not
         *
         * \image html jkqtplotter_inverted_yaxis.png
         */
        bool inverted;

        /** \brief <b>calculated property:</b> x position of the first tick (calculated by calcPlotScaling() ). Given in system coordinates, not pixel coordinates.
         *
         * \see calcPlotScaling(), calcTickSpacing()
         */
        double tickStart;

        /** \brief <b>calculated property:</b> indicates whether the object should use automatic tick spacing for the x axis (calculated by calcPlotScaling() )
         *
         * \see calcPlotScaling(), calcTickSpacing()
         */
        bool autoAxisSpacing;

        /** \brief indicates whether the y axis has a logarithmic scale */
        bool logAxis;
        /** \brief the base for a logarithmic x axis */
        double logAxisBase;
        /** \brief if autoXAxisSpacing is \c false then this value is used for xTickSpacing. So this is the property which
         *         is editable by use of public access methods.
         */
        double userTickSpacing;
        /** \brief if autoXAxisSpacing is \c false then this value is used for xTickSpacing. So this is the property which
         *         is editable by use of public access methods.
         */
        double userLogTickSpacing;

        /** \brief <b>calculated property:</b> axis tick spacing (calculated by calcPlotScaling() and calcTickSpacing() ) for logarithmic/linear axes.
         *
         * \see calcPlotScaling(), calcTickSpacing()
         */
        double tickSpacing;
        /** \brief <b>calculated property:</b> axis tick spacing for logarithmic JKQTPCoordinateAxisStyle::ticks (calculated by calcPlotScaling() and calcTickSpacing() ) axes.
         *
         * \see calcPlotScaling()
         */
        double tickSpacingLog;


        /** \brief axis label of the axis */
        QString axisLabel;

        /** \brief tick values are the actual x/y-coordiniate, divided by this value (e.g. pu \f$ \pi \f$ to have an axis with values <tt>0, pi, 2pi, 3pi ...</tt>) */
        double tickUnitFactor;

        /** \brief name of the factor tickUnitFactor. This string is used in tick-labels to write e.g. \c "2pi" instead of 6.28... */
        QString tickUnitName;


        /** \brief calculates the tick spacing for a linear axis that spans \a awidth and that should
         *         show at least \a JKQTPCoordinateAxisStyle::minTicks JKQTPCoordinateAxisStyle::ticks.
         *
         * The <b>algorithm</b> used in here works as follows:
         *
         * There is only a limited subset of allowed tick distances. These distances are 1 (resulting in 10 JKQTPCoordinateAxisStyle::ticks per decade, i.e. 0,1,2,3,4,...),
         * 2 (5 JKQTPCoordinateAxisStyle::ticks/decade, i.e. 0,2,4,...), 2.5 (4 JKQTPCoordinateAxisStyle::ticks/decade, i.e. 0,2.5,5,...) and 5 (2 JKQTPCoordinateAxisStyle::ticks/decade, i.e. 0,5,10,15,...). So the
         * axis is divided into decades that each is divided into JKQTPCoordinateAxisStyle::ticks, as defined above. With these preconditions the algorithm tries
         * to find a tick increment \f$ \Delta\mbox{tick} \f$ which is one of the above distances multiplied by a power \f$ \rho \f$ of 10.
         * This increment is determined in a way, that the axis contains at least \a JKQTPCoordinateAxisStyle::minTicks JKQTPCoordinateAxisStyle::ticks:
         *   \f[ \Delta\mbox{tick}=f\cdot 10^\rho,\ \ \ \ \ f\in\{1, 2, 2.5, 5\} \f]
         *
         *   -# The algorithm starts by finding a start exponent \f$ \rho_s=\left\lfloor\log_{10}(\mbox{awidth})\right\rfloor+3 \f$. This
         *      is a good (over)estimate for the biggest possible power \f$ \rho \f$.
         *   -# now we set \f$ \rho=\rho_0 \f$ and \f$ f=10 \f$.
         *   -# now the algorithm cycles through all possible factors \f$ f\in\{1, 2, 2.5, 5, 10\} \f$ (starting from 10) and calculates
         *      \f$ \Delta\mbox{tick}=f\cdot 10^\rho \f$. Then it checks whether \f$ \mbox{tickcount}=\mbox{round}\left(\frac{\mbox{awidth}}{\Delta\mbox{tick}}\right) \f$
         *      is smaller than \a JKQTPCoordinateAxisStyle::minTicks (if yes the algorithm reached its end and \f$ \Delta\mbox{tick} \f$ can be used.
         *   -# if \f$ f=1 \f$ is reached and checked \c false, then the algorithm decreases the exponent \f$ \rho \leftarrow \rho-1 \f$ and returns to step 3.
         *
         */
        double calcLinearTickSpacing();
        /** \brief same as calcLinearTickSpacing(), but for logarithmic scaling
         *
         * \see calcLinearTickSpacing()
         * */
        double calcLogTickSpacing();

        /** \brief Calculate the number of digits needed for the labels of an axis that starts at
         *         \a minval and where the tick spacing is \a tickSpacing.
         *
         * This method determines how many digits to output for the labels on a coordinate axis.
         * This is done by testing different digit numbers and comparing subsequent labels. If two labels
         * are equal, then we need more valid digits to distinguish them.
         */
        int calcLinearUnitDigits();



        /** \brief axis prefix for storage of parameters */
        QString axisPrefix;
        /** \brief this is used by x2p() and p2x() to determine the sign */
        double scaleSign;

        /** \brief calculates the next label from the given parameters.
         *
         *  \return \c true on success and \c false if there is no more label
         *  \param x \b before \b call: the position of the label for which the text should be returned (or JKQTPCoordinateAxisStyle::tickStart for first call),
         *           \b after \b call: position of the next label (given in world coordinates!)
         *  \param label \b after \b call: text of the (input) tick label This is an empty string, if the label is not visible!
         *  \param init call this function with \c init=true to obtain the first label (at JKQTPCoordinateAxisStyle::tickStart)
         */
        bool getNextLabel(double& x, QString& label, bool init=false);

        /** \brief returns the distance from the current tick position (x before call) to the next tick position */
        double getNextLabelDistance(double x);

        /** \brief calculates the maximum width and height (returned as QSize) of all tick labels.
         *         Ascent and descent may also be returned in the two additional pointer arguments- */
        QSizeF getMaxTickLabelSize(JKQTPEnhancedPainter& painter, double* ascent=nullptr, double* descent=nullptr);
        /** \brief draw the axis line \a l (pointing from axismin to axismax) optionally decorated as specified by \a drawMode using JKQTPEnhancedPainter \a painter */
        void drawAxisLine(JKQTPEnhancedPainter& painter, const QLineF& l, JKQTPCADrawMode drawMode) const;

        /** \brief calculate the position of the zero-axis (and whether to draw  it or not) */
        double getZeroAxisPos(bool*  drawZeroAxis=nullptr);

        /** \brief figures out (possibly by probing the parent JKQTBasePloter), whether this axis is a secondary axis */
        virtual bool isSecondaryAxis() const;


};





/*! \brief base class for vertical axes, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    The positioning of the different axis elements depends on the psition of the "other" axis (i.e. a horizontal/x-axis),
    i.e. the corresponding vertical axis:

    \image html JKQTPVerticalAxisPositioning.png

    But this "other>" axis is not defined here, but has to be defined in derived classes by immplementing the corresponding
    pure virtual functions:
      - getParentPlotWidth()
      - getParentPlotOffset()
      - getParentOtheraxisWidth()
      - getParentOtheraxisInverted()
      - getParentOtheraxisOffset()
      - parentOtherAxisX2P()
    .

    \see JKQTPHorizontalAxisBase
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPVerticalAxisBase: public JKQTPCoordinateAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPVerticalAxisBase(JKQTBasePlotter* parent);

        /** \brief returns the size of the left axis in pixels */
        virtual AxisElementsSizeDescription getSize1(JKQTPEnhancedPainter& painter) override;
        QSizeF getQSize1(JKQTPEnhancedPainter& painter);

        /** \brief returns the size of the right axis in pixels */
        virtual AxisElementsSizeDescription getSize2(JKQTPEnhancedPainter& painter) override;
        QSizeF getQSize2(JKQTPEnhancedPainter& painter);

        /** copydoc JKQTPCoordinateAxis::drawAxes() */
        virtual void drawAxes(JKQTPEnhancedPainter& painter, int move1=0, int move2=0) override;

        /** copydoc JKQTPCoordinateAxis::drawGrids() */
        virtual void drawGrids(JKQTPEnhancedPainter& painter) override;


    protected:
        /** copydoc JKQTPCoordinateAxis::getSize0() */
        virtual Axis0ElementsSizeDescription getSize0(JKQTPEnhancedPainter& painter) override;
        /** \brief draw a tick label on the left axis 1 with text \a label (with optional rotation) at ( \a xx , \a yy ) (in pixel)
         *
         *  \param painter the JKQTPEnhancedPainter used for drawing
         *  \param xx the exact position of the tick in pixels
         *  \param yy the exact position of the tick in pixels
         *  \param labelOffset offset of the label from ( \a xx , \a yy ) in pt, this is typically equal to \c tickOuterLength+tickLabelDistance
         *  \param label text to display
         *  \param fontSize the fontSize of the label (in pt)
         *  \param isMinor indicates whether  the axis tick is a minor tick
         */
        void drawTickLabel1(JKQTPEnhancedPainter& painter, double xx, double yy, double labelOffset, const QString &label, double fontSize, bool isMinor=false) ;
        /** \brief draw a tick label on the right axis 2 with text \a label (with optional rotation) at ( \a xx , \a yy ) (in pixel)
         *
         *  \param painter the JKQTPEnhancedPainter used for drawing
         *  \param xx the exact position of the tick in pixels
         *  \param yy the exact position of the tick in pixels
         *  \param labelOffset offset of the label from ( \a xx , \a yy ) in pt, this is typically equal to \c tickOuterLength+tickLabelDistance
         *  \param label text to display
         *  \param fontSize the fontSize of the label (in pt)
         *  \param isMinor indicates whether  the axis tick is a minor tick
         */
        void drawTickLabel2(JKQTPEnhancedPainter& painter, double xx, double yy, double labelOffset, const QString &label, double fontSize, bool isMinor=false) ;
        /** \brief draw the axis label using \a painter for axis 1 at \c x= \a left and \c y= \a bottom. \a labelMax is the maximum Size of all tick labels */
        void drawAxisLabel1(JKQTPEnhancedPainter &painter, double left, double bottom, QSizeF labelMax, JKQTPCADrawMode drawMode);
        /** \brief draw the axis label using \a painter for axis 2 at \c x= \a right and \c y= \a bottom. \a labelMax is the maximum Size of all tick labels */
        void drawAxisLabel2(JKQTPEnhancedPainter &painter, double right, double bottom, QSizeF labelMax, JKQTPCADrawMode drawMode);

};



/*! \brief implements a vertical axis for use as primary, secondary, ... axis of a JKQTPBasePlotter,
 *         based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    The positioning of the different axis elements depends on the psition of the "other" axis (here x-axis),
    i.e. the corresponding vertical axis:

    \image html JKQTPVerticalAxisPositioning.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPVerticalAxis: public JKQTPVerticalAxisBase {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPVerticalAxis(JKQTBasePlotter* parent, JKQTPCoordinateAxisRef otherAxisRef=JKQTPPrimaryAxis);


        /** copydoc JKQTPCoordinateAxis::getParentPlotWidth() */
        virtual double getParentPlotWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentPlotOffset() */
        virtual double getParentPlotOffset() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisWidth() */
        virtual double getParentOtheraxisWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisInverted() */
        virtual bool getParentOtheraxisInverted() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisOffset() */
        virtual double getParentOtheraxisOffset() const override;
        /** copydoc JKQTPCoordinateAxis::parentOtherAxisX2P() */
        virtual double parentOtherAxisX2P(double x) const override;
        /** \copydoc otherAxisRef */
        JKQTPCoordinateAxisRef getOtherAxisRef() const;
        /** \brief returns the "other" axis, refernced by otherAxisRef */
        const JKQTPCoordinateAxis* getOtherAxis() const;

    protected:

        /** \brief references the other axis fromm the JKQTPBasePLotter to use as "other" axis */
        JKQTPCoordinateAxisRef otherAxisRef;
};


/*! \brief implements a position-indipendent vertical axis, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements


    This axis may be draw at a user-supplied position (used e.g. for color bar axes).
    The axis positioning parameters are supplied via the constructor,or setter methods.

    \image html JKQTPVerticalIndependentAxisPositioning.png

    Basically this class overwrites several virtual methods as follows:
      - getParentPlotWidth() returns axisWidth, provided in the connstructor or via setParentPlotWidth()
      - getParentPlotOffset() returns axisOffset, provided in the connstructor or via setParentPlotOffset()
      - getParentOtheraxisWidth() returns otherAxisWidth, provided in the connstructor or via setParentPlotOtheraxisWidth()
      - getParentOtheraxisOffset() returns otherAxisOffset, provided in the connstructor or via setParentPlotOtheraxisOffset()
    .

    If it is paired with another axis, the parameters of that axis have to be given explizitly in the constructor or with  setters.

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPVerticalIndependentAxis: public JKQTPVerticalAxisBase {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPVerticalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQTBasePlotter* parent);

        /** copydoc JKQTPCoordinateAxis::getParentPlotWidth() */
        virtual double getParentPlotWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentPlotOffset() */
        virtual double getParentPlotOffset() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisWidth() */
        virtual double getParentOtheraxisWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisInverted() */
        virtual bool getParentOtheraxisInverted() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisOffset() */
        virtual double getParentOtheraxisOffset() const override;
        /** copydoc JKQTPCoordinateAxis::parentOtherAxisX2P() */
        virtual double parentOtherAxisX2P(double x) const override;
    public Q_SLOTS:
        /** \brief set the axis offset */
        virtual void setAxisOffset(double __value) ;
        /** \brief set the axis width */
        virtual void setAxisWidth(double __value) ;
        /** \brief set the other axis offset */
        virtual void setOtherAxisOffset(double __value) ;
        /** \brief set the other axis width */
        virtual void setOtherAxisWidth(double __value) ;
        /** \brief set whether the other axis is inverted */
        virtual void setOtherAxisInverted(bool __value) ;

    protected:
        /** \brief the offset of the axis */
        double axisOffset;
        /** \brief the width of the axis */
        double axisWidth;
        /** \brief the width of the other axis */
        double otherAxisWidth;
        /** \brief the offset of the other axis */
        double otherAxisOffset;
        /** \brief indicates whether the other axis is inverted */
        bool otherAxisInverted;
};



/*! \brief base class for horizontal axes, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    The positioning of the different axis elements depends on the psition of the "other"  axis (i.e. a vertical/y-axis),
    i.e. the corresponding vertical axis:

    \image html JKQTPHorizontalAxisPositioning.png


    But this "other>" axis is not defined here, but has to be defined in derived classes by immplementing the corresponding
    pure virtual functions:
      - getParentPlotWidth()
      - getParentPlotOffset()
      - getParentOtheraxisWidth()
      - getParentOtheraxisInverted()
      - getParentOtheraxisOffset()
      - parentOtherAxisX2P()
    .

    \see JKQTPVerticalAxisBase
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPHorizontalAxisBase: public JKQTPCoordinateAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPHorizontalAxisBase(JKQTBasePlotter* parent);

        /** \brief returns the size of the bottom axis in pixels */
        virtual AxisElementsSizeDescription getSize1(JKQTPEnhancedPainter& painter) override;
        QSizeF getQSize1(JKQTPEnhancedPainter& painter);

        /** \brief returns the size of the top axis in pixels */
        virtual AxisElementsSizeDescription getSize2(JKQTPEnhancedPainter& painter) override;
        QSizeF getQSize2(JKQTPEnhancedPainter& painter);

        /** copydoc JKQTPCoordinateAxis::drawAxes() */
        virtual void drawAxes(JKQTPEnhancedPainter& painter, int move1=0, int move2=0) override;

        /** copydoc JKQTPCoordinateAxis::drawGrids() */
        virtual void drawGrids(JKQTPEnhancedPainter& painter) override;


    protected:
        /** copydoc JKQTPCoordinateAxis::getSize0() */
        virtual Axis0ElementsSizeDescription getSize0(JKQTPEnhancedPainter& painter) override;
        /** \brief draw a tick label on the lower axis 1 with text \a label (with optional rotation) at ( \a xx , \a yy ) (in pixel)
         *
         *  \param painter the JKQTPEnhancedPainter used for drawing
         *  \param xx the exact position of the tick in pixels
         *  \param yy the exact position of the tick in pixels
         *  \param labelOffset offset of the label from ( \a xx , \a yy ) in pt, this is typically equal to \c tickOuterLength+tickLabelDistance
         *  \param label text to display
         *  \param fontSize the fontSize of the label (in pt)
         *  \param ascentMax maximum ascent of all tick labels
         *  \param descentMax maximum descent of all tick labels
         *  \param isMinor indicates whether  the axis tick is a minor tick
         */
        void drawTickLabel1(JKQTPEnhancedPainter& painter, double xx, double yy, double labelOffset, const QString &label, double fontSize, double ascentMax, double descentMax, bool isMinor=false) ;
        /** \brief draw a tick label on the upper axis 2 with text \a label (with optional rotation) at ( \a xx , \a yy ) (in pixel)
         *
         *  \param painter the JKQTPEnhancedPainter used for drawing
         *  \param xx the exact position of the tick in pixels
         *  \param yy the exact position of the tick in pixels
         *  \param labelOffset offset of the label from ( \a xx , \a yy ) in pt, this is typically equal to \c tickOuterLength+tickLabelDistance
         *  \param label text to display
         *  \param fontSize the fontSize of the label (in pt)
         *  \param ascentMax maximum ascent of all tick labels
         *  \param descentMax maximum descent of all tick labels
         *  \param isMinor indicates whether  the axis tick is a minor tick
         */
        void drawTickLabel2(JKQTPEnhancedPainter& painter, double xx, double yy, double labelOffset, const QString &label, double fontSize, double ascentMax, double descentMax, bool isMinor=false) ;
        /** \brief draw the axis label using \a painter for axis 1 at \c x= \a left and \c y= \a bottom. \a labelMax is the maximum Size of all tick labels */
        void drawAxisLabel1(JKQTPEnhancedPainter &painter, double left, double bottom, QSizeF labelMax, JKQTPCADrawMode drawMode);
        /** \brief draw the axis label using \a painter for axis 2 at \c x= \a left and \c y= \a top. \a labelMax is the maximum Size of all tick labels */
        void drawAxisLabel2(JKQTPEnhancedPainter &painter, double left, double top, QSizeF labelMax, JKQTPCADrawMode drawMode);

};


/*! \brief implements a horizontal axis, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    The positioning of the different axis elements depends on the psition of the "other"  axis (here y-axis),
    i.e. the corresponding vertical axis:

    \image html JKQTPHorizontalAxisPositioning.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPHorizontalAxis: public JKQTPHorizontalAxisBase {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPHorizontalAxis(JKQTBasePlotter* parent, JKQTPCoordinateAxisRef otherAxisRef_=JKQTPPrimaryAxis);

        /** copydoc JKQTPCoordinateAxis::getParentPlotWidth() */
        virtual double getParentPlotWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentPlotOffset() */
        virtual double getParentPlotOffset() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisWidth() */
        virtual double getParentOtheraxisWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisInverted() */
        virtual bool getParentOtheraxisInverted() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisOffset() */
        virtual double getParentOtheraxisOffset() const override;
        /** copydoc JKQTPCoordinateAxis::parentOtherAxisX2P() */
        virtual double parentOtherAxisX2P(double x) const override;
        /** \copydoc otherAxisRef */
        JKQTPCoordinateAxisRef getOtherAxisRef() const;
        /** \brief returns the "other" axis, refernced by otherAxisRef */
        const JKQTPCoordinateAxis* getOtherAxis() const;

    protected:

        /** \brief references the other axis fromm the JKQTPBasePLotter to use as "other" axis */
        JKQTPCoordinateAxisRef otherAxisRef;

};

/*! \brief implements a position-indipendent horizontal axis, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    This axis may be draw at a user-supplied position (used e.g. for color bar axes).
    The axis positioning parameters are supplied via the constructor,or setter methods.

    \image html JKQTPHorizontalIndependentAxisPositioning.png

    Basically this class overwrites several virtual methods as follows:
      - getParentPlotWidth() returns axisWidth, provided in the connstructor or via setParentPlotWidth()
      - getParentPlotOffset() returns axisOffset, provided in the connstructor or via setParentPlotOffset()
      - getParentOtheraxisWidth() returns otherAxisWidth, provided in the connstructor or via setParentPlotOtheraxisWidth()
      - getParentOtheraxisOffset() returns otherAxisOffset, provided in the connstructor or via setParentPlotOtheraxisOffset()
    .

    If it is paired with another axis, the parameters of that axis have to be given explizitly in the constructor or with  setters.

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPHorizontalIndependentAxis: public JKQTPHorizontalAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPHorizontalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQTBasePlotter* parent);

        /** copydoc JKQTPCoordinateAxis::getParentPlotWidth() */
        virtual double getParentPlotWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentPlotOffset() */
        virtual double getParentPlotOffset() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisWidth() */
        virtual double getParentOtheraxisWidth() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisInverted() */
        virtual bool getParentOtheraxisInverted() const override;
        /** copydoc JKQTPCoordinateAxis::getParentOtheraxisOffset() */
        virtual double getParentOtheraxisOffset() const override;
        /** copydoc JKQTPCoordinateAxis::parentOtherAxisX2P() */
        virtual double parentOtherAxisX2P(double x) const override;
    public Q_SLOTS:
        /** \brief set the axis offset */
        virtual void setAxisOffset(double __value);
        /** \brief set the axis width */
        virtual void setAxisWidth(double __value);
        /** \brief set the other axis offset */
        virtual void setOtherAxisOffset(double __value);
        /** \brief set the other axis width */
        virtual void setOtherAxisWidth(double __value);
        /** \brief set whether the other axis is inverted */
        virtual void setOtherAxisInverted(bool __value);

    protected:


        /** \brief the offset of the axis */
        double axisOffset;
        /** \brief the width of the axis */
        double axisWidth;
        /** \brief the width of the other axis */
        double otherAxisWidth;
        /** \brief the offset of the other axis */
        double otherAxisOffset;
        /** \brief indicates whether the other axis is inverted */
        bool otherAxisInverted;
};

#endif // JKQTPCOORDINATEAXES_H
