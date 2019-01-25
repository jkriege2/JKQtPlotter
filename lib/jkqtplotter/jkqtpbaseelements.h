/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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



/** \file jkqtpbaseelements.h
  * \ingroup jkqtpbaseplotter_elements
  */

#ifndef JKQTPBASEELEMENTS_H
#define JKQTPBASEELEMENTS_H


#include <QString>
#include <QPainter>
#include <QPair>
#include <QSettings>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtplottertools/jkqtp_imexport.h"

// forward declarations
class JKQTBasePlotter;

/*! \brief this virtual class is the base for any type of coordinate axis, to be drawn by JKQTBasePlotter.
    \ingroup jkqtpbaseplotter_elements

    This class implements all the functionality needed for a coordinate axis:
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


    \section jkqtplotter_base_grids_baseelemenets Axis Ticks and Grids

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
class LIB_EXPORT JKQTPCoordinateAxis: public QObject {
        Q_OBJECT
    protected:
    public:

        /** \brief class constructor */
        explicit JKQTPCoordinateAxis(JKQTBasePlotter* parent);
        /** \brief class destructor */
        virtual ~JKQTPCoordinateAxis();
        virtual void set_parent(JKQTBasePlotter* parent);
        /** \brief loads the plot properties from a QSettings object */
        virtual void loadSettings(QSettings& settings, QString group=QString("plots"));

        /** \brief saves the plot properties into a QSettings object.
         *
         * This method only saves those properties that differ from their default value.
         */
        virtual void saveSettings(QSettings& settings, QString group=QString("plots"));

        /** \brief load settings from other axis */
        virtual void loadSettings(JKQTPCoordinateAxis* settings);

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
        }

        /** \brief return time coordinate coordinate from x-pixel */
        inline double p2x(double x) const {
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
        }


        /** \brief clear axis tick labels. This switches back to automatic labels mode for the axis. */
        void clearAxisTickLabels();

        /** \brief add a new tick label to the axis */
        void addAxisTickLabel(double x, QString label);


        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const QVector<double>& x, const QStringList& label);
        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const double* x, const QStringList& label);
        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const QVector<double>& x, const QString* label);
        /** \brief add a new tick label to the axis */
        void addAxisTickLabels(const double* x, const QString* label, int items);

        /** \brief returns the size of the left/bottom axis in pt */
        virtual QSize getSize1(JKQTPEnhancedPainter& painter)=0;

        /** \brief returns the size of the right/top axis in pt */
        virtual QSize getSize2(JKQTPEnhancedPainter& painter)=0;

        /** \brief draw axes  */
        virtual void drawAxes(JKQTPEnhancedPainter& painter)=0;

        /** \brief draw grids  */
        virtual void drawGrids(JKQTPEnhancedPainter& painter)=0;


        /*! \brief returns the property tickSpacing. \details Description of the parameter tickSpacing is:  <BLOCKQUOTE>\copydoc tickSpacing </BLOCKQUOTE>. \see tickSpacing for more information */ 
        inline double get_tickSpacing() const { return this->tickSpacing; }
        /*! \brief returns the property labelDigits. \details Description of the parameter labelDigits is:  <BLOCKQUOTE>\copydoc labelDigits </BLOCKQUOTE>. \see labelDigits for more information */ 
        inline int get_labelDigits() const { return this->labelDigits; }
        /*! \brief returns the property autoAxisSpacing. \details Description of the parameter autoAxisSpacing is:  <BLOCKQUOTE>\copydoc autoAxisSpacing </BLOCKQUOTE>. \see autoAxisSpacing for more information */ 
        inline bool get_autoAxisSpacing() const { return this->autoAxisSpacing; }
        /*! \brief returns the property minorTickLabelsEnabled. \details Description of the parameter minorTickLabelsEnabled is:  <BLOCKQUOTE>\copydoc minorTickLabelsEnabled </BLOCKQUOTE>. \see minorTickLabelsEnabled for more information */ 
        inline bool get_minorTickLabelsEnabled() const { return this->minorTickLabelsEnabled; }
        /*! \brief returns the property logAxis. \details Description of the parameter logAxis is:  <BLOCKQUOTE>\copydoc logAxis </BLOCKQUOTE>. \see logAxis for more information */ 
        inline bool get_logAxis() const { return this->logAxis; }
        /*! \brief returns the property inverted. \details Description of the parameter inverted is:  <BLOCKQUOTE>\copydoc inverted </BLOCKQUOTE>. \see inverted for more information */ 
        inline bool get_inverted() const { return this->inverted; }
        /*! \brief returns the property logAxisBase. \details Description of the parameter logAxisBase is:  <BLOCKQUOTE>\copydoc logAxisBase </BLOCKQUOTE>. \see logAxisBase for more information */ 
        inline double get_logAxisBase() const { return this->logAxisBase; }
        /*! \brief returns the property userTickSpacing. \details Description of the parameter userTickSpacing is:  <BLOCKQUOTE>\copydoc userTickSpacing </BLOCKQUOTE>. \see userTickSpacing for more information */ 
        inline double getUserTickSpacing() const { return this->userTickSpacing; }
        /*! \brief returns the property userLogTickSpacing. \details Description of the parameter userLogTickSpacing is:  <BLOCKQUOTE>\copydoc userLogTickSpacing </BLOCKQUOTE>. \see userLogTickSpacing for more information */ 
        inline double getUserLogTickSpacing() const { return this->userLogTickSpacing; }
        /*! \brief returns the property labelType. \details Description of the parameter labelType is:  <BLOCKQUOTE>\copydoc labelType </BLOCKQUOTE>. \see labelType for more information */ 
        inline JKQTPCALabelType get_labelType() const { return this->labelType; }
        /*! \brief returns the property axisLabel. \details Description of the parameter axisLabel is:  <BLOCKQUOTE>\copydoc axisLabel </BLOCKQUOTE>. \see axisLabel for more information */ 
        inline QString getAxisLabel() const { return this->axisLabel; }
        /*! \brief returns the property labelPosition. \details Description of the parameter labelPosition is:  <BLOCKQUOTE>\copydoc labelPosition </BLOCKQUOTE>. \see labelPosition for more information */ 
        inline JKQTPLabelPosition get_labelPosition() const { return this->labelPosition; }
        /*! \brief returns the property labelFont. \details Description of the parameter labelFont is:  <BLOCKQUOTE>\copydoc labelFont </BLOCKQUOTE>. \see labelFont for more information */ 
        inline QString get_labelFont() const { return this->labelFont; }
        /*! \brief returns the property labelFontSize. \details Description of the parameter labelFontSize is:  <BLOCKQUOTE>\copydoc labelFontSize </BLOCKQUOTE>. \see labelFontSize for more information */ 
        inline double get_labelFontSize() const { return this->labelFontSize; }
        /*! \brief returns the property tickLabelFont. \details Description of the parameter tickLabelFont is:  <BLOCKQUOTE>\copydoc tickLabelFont </BLOCKQUOTE>. \see tickLabelFont for more information */ 
        inline QString get_tickLabelFont() const { return this->tickLabelFont; }
        /*! \brief returns the property tickLabelFontSize. \details Description of the parameter tickLabelFontSize is:  <BLOCKQUOTE>\copydoc tickLabelFontSize </BLOCKQUOTE>. \see tickLabelFontSize for more information */ 
        inline double get_tickLabelFontSize() const { return this->tickLabelFontSize; }
        /*! \brief returns the property minorTickLabelFontSize. \details Description of the parameter minorTickLabelFontSize is:  <BLOCKQUOTE>\copydoc minorTickLabelFontSize </BLOCKQUOTE>. \see minorTickLabelFontSize for more information */ 
        inline double get_minorTickLabelFontSize() const { return this->minorTickLabelFontSize; }
        /*! \brief returns the property minorTickLabelFullNumber. \details Description of the parameter minorTickLabelFullNumber is:  <BLOCKQUOTE>\copydoc minorTickLabelFullNumber </BLOCKQUOTE>. \see minorTickLabelFullNumber for more information */ 
        inline bool get_minorTickLabelFullNumber() const { return this->minorTickLabelFullNumber; }
        /*! \brief returns the property tickLabelAngle. \details Description of the parameter tickLabelAngle is:  <BLOCKQUOTE>\copydoc tickLabelAngle </BLOCKQUOTE>. \see tickLabelAngle for more information */ 
        inline double get_tickLabelAngle() const { return this->tickLabelAngle; }
        /*! \brief returns the property minTicks. \details Description of the parameter minTicks is:  <BLOCKQUOTE>\copydoc minTicks </BLOCKQUOTE>. \see minTicks for more information */ 
        inline unsigned int get_minTicks() const { return this->minTicks; }
        /*! \brief returns the property minorTicks. \details Description of the parameter minorTicks is:  <BLOCKQUOTE>\copydoc minorTicks </BLOCKQUOTE>. \see minorTicks for more information */ 
        inline unsigned int get_minorTicks() const { return this->minorTicks; }
        /*! \brief returns the property tickOutsideLength. \details Description of the parameter tickOutsideLength is:  <BLOCKQUOTE>\copydoc tickOutsideLength </BLOCKQUOTE>. \see tickOutsideLength for more information */ 
        inline double get_tickOutsideLength() const { return this->tickOutsideLength; }
        /*! \brief returns the property minorTickOutsideLength. \details Description of the parameter minorTickOutsideLength is:  <BLOCKQUOTE>\copydoc minorTickOutsideLength </BLOCKQUOTE>. \see minorTickOutsideLength for more information */ 
        inline double get_minorTickOutsideLength() const { return this->minorTickOutsideLength; }
        /*! \brief returns the property axisColor. \details Description of the parameter axisColor is:  <BLOCKQUOTE>\copydoc axisColor </BLOCKQUOTE>. \see axisColor for more information */ 
        inline QColor getAxisColor() const { return this->axisColor; }
        /*! \brief returns the property showZeroAxis. \details Description of the parameter showZeroAxis is:  <BLOCKQUOTE>\copydoc showZeroAxis </BLOCKQUOTE>. \see showZeroAxis for more information */ 
        inline bool getShowZeroAxis() const { return this->showZeroAxis; }
        /*! \brief returns the property gridColor. \details Description of the parameter gridColor is:  <BLOCKQUOTE>\copydoc gridColor </BLOCKQUOTE>. \see gridColor for more information */ 
        inline QColor getGridColor() const { return this->gridColor; }
        /*! \brief returns the property minorGridColor. \details Description of the parameter minorGridColor is:  <BLOCKQUOTE>\copydoc minorGridColor </BLOCKQUOTE>. \see minorGridColor for more information */ 
        inline QColor get_minorGridColor() const { return this->minorGridColor; }
        /*! \brief returns the property gridWidth. \details Description of the parameter gridWidth is:  <BLOCKQUOTE>\copydoc gridWidth </BLOCKQUOTE>. \see gridWidth for more information */ 
        inline double getGridWidth() const { return this->gridWidth; }
        /*! \brief returns the property gridStyle. \details Description of the parameter gridStyle is:  <BLOCKQUOTE>\copydoc gridStyle </BLOCKQUOTE>. \see gridStyle for more information */ 
        inline Qt::PenStyle getGridStyle() const { return this->gridStyle; }
        /*! \brief returns the property minorGridWidth. \details Description of the parameter minorGridWidth is:  <BLOCKQUOTE>\copydoc minorGridWidth </BLOCKQUOTE>. \see minorGridWidth for more information */ 
        inline double get_minorGridWidth() const { return this->minorGridWidth; }
        /*! \brief returns the property minorGridStyle. \details Description of the parameter minorGridStyle is:  <BLOCKQUOTE>\copydoc minorGridStyle </BLOCKQUOTE>. \see minorGridStyle for more information */ 
        inline Qt::PenStyle get_minorGridStyle() const { return this->minorGridStyle; }
        /*! \brief returns the property tickTimeFormat. \details Description of the parameter tickTimeFormat is:  <BLOCKQUOTE>\copydoc tickTimeFormat </BLOCKQUOTE>. \see tickTimeFormat for more information */ 
        inline QString get_tickTimeFormat() const { return this->tickTimeFormat; }
        /*! \brief returns the property tickDateFormat. \details Description of the parameter tickDateFormat is:  <BLOCKQUOTE>\copydoc tickDateFormat </BLOCKQUOTE>. \see tickDateFormat for more information */ 
        inline QString get_tickDateFormat() const { return this->tickDateFormat; }
        /*! \brief returns the property tickDateTimeFormat. \details Description of the parameter tickDateTimeFormat is:  <BLOCKQUOTE>\copydoc tickDateTimeFormat </BLOCKQUOTE>. \see tickDateTimeFormat for more information */ 
        inline QString get_tickDateTimeFormat() const { return this->tickDateTimeFormat; }
        /*! \brief returns the property tickMode. \details Description of the parameter tickMode is:  <BLOCKQUOTE>\copydoc tickMode </BLOCKQUOTE>. \see tickMode for more information */ 
        inline JKQTPLabelTickMode get_tickMode() const { return this->tickMode; }

        /*! \brief returns the property drawMode1. \details Description of the parameter drawMode1 is:  <BLOCKQUOTE>\copydoc drawMode1 </BLOCKQUOTE>. \see drawMode1 for more information */ 
        inline JKQTPCADrawMode get_drawMode1() const { return this->drawMode1; }
        /*! \brief returns the property drawMode2. \details Description of the parameter drawMode2 is:  <BLOCKQUOTE>\copydoc drawMode2 </BLOCKQUOTE>. \see drawMode2 for more information */ 
        inline JKQTPCADrawMode get_drawMode2() const { return this->drawMode2; }
        /*! \brief returns the property minorTickWidth. \details Description of the parameter minorTickWidth is:  <BLOCKQUOTE>\copydoc minorTickWidth </BLOCKQUOTE>. \see minorTickWidth for more information */ 
        inline double get_minorTickWidth() const { return this->minorTickWidth; }
        /*! \brief returns the property tickWidth. \details Description of the parameter tickWidth is:  <BLOCKQUOTE>\copydoc tickWidth </BLOCKQUOTE>. \see tickWidth for more information */ 
        inline double get_tickWidth() const { return this->tickWidth; }
        /*! \brief returns the property lineWidth. \details Description of the parameter lineWidth is:  <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE>. \see lineWidth for more information */ 
        inline double getLineWidth() const { return this->lineWidth; }
        /*! \brief returns the property lineWidthZeroAxis. \details Description of the parameter lineWidthZeroAxis is:  <BLOCKQUOTE>\copydoc lineWidthZeroAxis </BLOCKQUOTE>. \see lineWidthZeroAxis for more information */ 
        inline double getLineWidthZeroAxis() const { return this->lineWidthZeroAxis; }
        /*! \brief returns the property tickLabelDistance. \details Description of the parameter tickLabelDistance is:  <BLOCKQUOTE>\copydoc tickLabelDistance </BLOCKQUOTE>. \see tickLabelDistance for more information */ 
        inline double get_tickLabelDistance() const { return this->tickLabelDistance; }
        /*! \brief returns the property labelDistance. \details Description of the parameter labelDistance is:  <BLOCKQUOTE>\copydoc labelDistance </BLOCKQUOTE>. \see labelDistance for more information */ 
        inline double get_labelDistance() const { return this->labelDistance; }
        /*! \brief returns the property drawGrid. \details Description of the parameter drawGrid is:  <BLOCKQUOTE>\copydoc drawGrid </BLOCKQUOTE>. \see drawGrid for more information */ 
        inline bool get_drawGrid() const { return this->drawGrid; }
        /*! \brief returns the property drawMinorGrid. \details Description of the parameter drawMinorGrid is:  <BLOCKQUOTE>\copydoc drawMinorGrid </BLOCKQUOTE>. \see drawMinorGrid for more information */ 
        inline bool get_drawMinorGrid() const { return this->drawMinorGrid; }
        /*! \brief sets the property autoLabelDigits to the specified \a __value. 
            \details Description of the parameter autoLabelDigits is: <BLOCKQUOTE>\copydoc autoLabelDigits </BLOCKQUOTE> 
            \see autoLabelDigits for more information */ 
        inline virtual void set_autoLabelDigits(bool __value)
        {
            this->autoLabelDigits = __value;
        } 
        /*! \brief returns the property autoLabelDigits. 
            \details Description of the parameter autoLabelDigits is: <BLOCKQUOTE>\copydoc autoLabelDigits </BLOCKQUOTE> 
            \see autoLabelDigits for more information */ 
        inline virtual bool get_autoLabelDigits() const  
        {
            return this->autoLabelDigits; 
        }
        /*! \brief returns the property parent. \details Description of the parameter parent is:  <BLOCKQUOTE>\copydoc parent </BLOCKQUOTE>. \see parent for more information */ 
        inline JKQTBasePlotter* get_parent() const { return this->parent; }
        /*! \brief sets the property doUpdateScaling to the specified \a __value. 
            \details Description of the parameter doUpdateScaling is: <BLOCKQUOTE>\copydoc doUpdateScaling </BLOCKQUOTE> 
            \see doUpdateScaling for more information */ 
        inline virtual void set_doUpdateScaling(bool __value)
        {
            this->doUpdateScaling = __value;
        } 
        /*! \brief returns the property doUpdateScaling. 
            \details Description of the parameter doUpdateScaling is: <BLOCKQUOTE>\copydoc doUpdateScaling </BLOCKQUOTE> 
            \see doUpdateScaling for more information */ 
        inline virtual bool get_doUpdateScaling() const  
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


        bool isLogAxis() const;

    public slots:
        /** \brief set range of plot axis */
        void setRange(double amin, double amax);
        /** \brief set absolute range of plot axis */
        void setAbsoluteRange(double aamin, double aamax);
        /** \brief do not use an absolute range of plot axis */
        void setNoAbsoluteRange();

        /** \brief sets the property tickSpacing to the specified \a __value.
         *  \details Description of the parameter tickSpacing is:  <BLOCKQUOTE>\copydoc tickSpacing </BLOCKQUOTE>
         *  \see tickSpacing for more information */
        virtual void set_tickSpacing(double __value);

        /** \brief sets the property axisMinWidth to the specified \a __value.
         *  \details Description of the parameter tickSpacing is:  <BLOCKQUOTE>\copydoc axisMinWidth </BLOCKQUOTE>
         *  \see axisMinWidth for more information */
        virtual void setAxisMinWidth(double __value);

        /** \brief sets the property autoAxisSpacing to the specified \a __value.
         *  \details Description of the parameter autoAxisSpacing is:  <BLOCKQUOTE>\copydoc autoAxisSpacing </BLOCKQUOTE>
         *  \see autoAxisSpacing for more information */
        virtual void set_autoAxisSpacing(bool __value);

        /** \brief sets the property minorTickLabelsEnabled to the specified \a __value.
         *  \details Description of the parameter minorTickLabelsEnabled is:  <BLOCKQUOTE>\copydoc minorTickLabelsEnabled </BLOCKQUOTE>
         *  \see autoAxisSpacing for more information */
        virtual void set_minorTickLabelsEnabled(bool __value);

        /** \brief sets the property logAxis to the specified \a __value.
         *  \details Description of the parameter logAxis is:  <BLOCKQUOTE>\copydoc logAxis </BLOCKQUOTE>
         *  \see logAxis for more information */
        virtual void set_logAxis(bool __value) ;

        /** \brief sets the property logAxisBase to the specified \a __value.
         *  \details Description of the parameter logAxisBase is:  <BLOCKQUOTE>\copydoc logAxisBase </BLOCKQUOTE>
         *  \see logAxisBase for more information */
        virtual void set_logAxisBase (double __value);

        /** \brief sets the property userTickSpacing to the specified \a __value.
         *  \details Description of the parameter userTickSpacing is:  <BLOCKQUOTE>\copydoc userTickSpacing </BLOCKQUOTE>
         *  \see userTickSpacing for more information */
        virtual void setUserTickSpacing (double __value);

        /** \brief sets the property userLogTickSpacing to the specified \a __value.
         *  \details Description of the parameter userLogTickSpacing is:  <BLOCKQUOTE>\copydoc userLogTickSpacing </BLOCKQUOTE>
         *  \see userLogTickSpacing for more information */
        virtual void setUserLogTickSpacing (double __value);

        /** \brief sets the property labelType to the specified \a __value.
         *  \details Description of the parameter labelType is:  <BLOCKQUOTE>\copydoc labelType </BLOCKQUOTE>
         *  \see labelType for more information */
        virtual void set_labelType (JKQTPCALabelType __value);

        /** \brief sets the property tickMode to the specified \a __value.
         *  \details Description of the parameter tickMode is:  <BLOCKQUOTE>\copydoc tickMode </BLOCKQUOTE>
         *  \see tickMode for more information */
        virtual void set_tickMode (JKQTPLabelTickMode __value);
        /** \brief sets the property tickMode to the specified \a __value.
         *  \details Description of the parameter tickMode is:  <BLOCKQUOTE>\copydoc tickMode </BLOCKQUOTE>
         *  \see tickMode for more information */
        virtual void set_tickMode (int __value);

        /** \brief sets the property axisLabel to the specified \a __value.
         *  \details Description of the parameter axisLabel is:  <BLOCKQUOTE>\copydoc axisLabel </BLOCKQUOTE>
         *  \see axisLabel for more information */
        virtual void setAxisLabel (QString __value);

        /** \brief sets the property labelPosition to the specified \a __value.
         *  \details Description of the parameter labelPosition is:  <BLOCKQUOTE>\copydoc labelPosition </BLOCKQUOTE> */
        virtual void set_labelPosition (JKQTPLabelPosition __value);

        /** \brief sets the property labelFont to the specified \a __value.
         *  \details Description of the parameter labelFont is:  <BLOCKQUOTE>\copydoc labelFont </BLOCKQUOTE> */
        virtual void set_labelFont (QString __value);

        /** \brief sets the property labelFontSize to the specified \a __value.
         *  \details Description of the parameter labelFontSize is:  <BLOCKQUOTE>\copydoc labelFontSize </BLOCKQUOTE> */
        virtual void set_labelFontSize (double __value);

        /** \brief sets the property tickLabelFont to the specified \a __value.
         *  \details Description of the parameter tickLabelFont is:  <BLOCKQUOTE>\copydoc tickLabelFont </BLOCKQUOTE> */
        virtual void set_tickLabelFont (QString __value);


        /** \brief sets the property tickTimeFormat to the specified \a __value.
         *  \details Description of the parameter tickTimeFormat is:  <BLOCKQUOTE>\copydoc tickTimeFormat </BLOCKQUOTE> */
        virtual void set_tickTimeFormat (QString __value);


        /** \brief sets the property tickDateFormat to the specified \a __value.
         *  \details Description of the parameter tickDateFormat is:  <BLOCKQUOTE>\copydoc tickDateFormat </BLOCKQUOTE> */
        virtual void set_tickDateFormat (QString __value);


        /** \brief sets the property tickDateTimeFormat to the specified \a __value.
         *  \details Description of the parameter tickDateTimeFormat is:  <BLOCKQUOTE>\copydoc tickDateTimeFormat </BLOCKQUOTE> */
        virtual void set_tickDateTimeFormat (QString __value);


        /** \brief sets the property tickLabelFontSize to the specified \a __value.
         *  \details Description of the parameter tickLabelFontSize is:  <BLOCKQUOTE>\copydoc tickLabelFontSize </BLOCKQUOTE> */
        virtual void set_tickLabelFontSize (double __value);

        /** \brief sets the property minorTickLabelFontSize to the specified \a __value.
         *  \details Description of the parameter minorTickLabelFontSize is:  <BLOCKQUOTE>\copydoc minorTickLabelFontSize </BLOCKQUOTE> */
        virtual void set_minorTickLabelFontSize (double __value);

        /** \brief sets the property minorTickLabelFullNumber to the specified \a __value.
         *  \details Description of the parameter minorTickLabelFullNumber is:  <BLOCKQUOTE>\copydoc minorTickLabelFullNumber </BLOCKQUOTE> */
        virtual void set_minorTickLabelFullNumber (bool __value);

        /** \brief sets the property minTicks to the specified \a __value.
         *  \details Description of the parameter minTicks is:  <BLOCKQUOTE>\copydoc minTicks </BLOCKQUOTE> */
        virtual void set_minTicks(unsigned int __value);

        /** \brief sets the property minorTicks to the specified \a __value.
         *  \details Description of the parameter minorTicks is:  <BLOCKQUOTE>\copydoc minorTicks </BLOCKQUOTE> */
        virtual void set_minorTicks (unsigned int __value);

        /** \brief sets the property minorTicks to the specified \a __value.
         *  \details Description of the parameter minorTicks is:  <BLOCKQUOTE>\copydoc minorTicks </BLOCKQUOTE> */
        virtual void set_minorTicks (int __value);

        /** \brief sets the property tickOutsideLength to the specified \a __value.
         *  \details Description of the parameter tickOutsideLength is:  <BLOCKQUOTE>\copydoc tickOutsideLength </BLOCKQUOTE> */
        virtual void set_tickOutsideLength(double __value);

        /** \brief sets the property minorTickOutsideLength to the specified \a __value.
         *  \details Description of the parameter minorTickOutsideLength is:  <BLOCKQUOTE>\copydoc minorTickOutsideLength </BLOCKQUOTE> */
        virtual void set_minorTickOutsideLength (double __value);

        /** \brief sets the property tickInsideLength to the specified \a __value.
         *  \details Description of the parameter tickInsideLength is:  <BLOCKQUOTE>\copydoc tickInsideLength </BLOCKQUOTE> */
        virtual void set_tickInsideLength(double __value);

        /** \brief sets the property minorTickInsideLength to the specified \a __value.
         *  \details Description of the parameter minorTickInsideLength is:  <BLOCKQUOTE>\copydoc minorTickInsideLength </BLOCKQUOTE> */
        virtual void set_minorTickInsideLength (double __value);

        /** \brief sets the property axisColor to the specified \a __value.
         *  \details Description of the parameter axisColor is:  <BLOCKQUOTE>\copydoc axisColor </BLOCKQUOTE> */
        virtual void setAxisColor (QColor __value);

        /** \brief sets the property showZeroAxis to the specified \a __value.
         *  \details Description of the parameter showZeroAxis is:  <BLOCKQUOTE>\copydoc showZeroAxis </BLOCKQUOTE> */
        virtual void setShowZeroAxis(bool __value);

        /** \brief sets the property inverted to the specified \a __value.
         *  \details Description of the parameter inverted is:  <BLOCKQUOTE>\copydoc inverted </BLOCKQUOTE> */
        virtual void set_inverted(bool __value);

        /** \brief sets the property gridColor to the specified \a __value.
         *  \details Description of the parameter gridColor is:  <BLOCKQUOTE>\copydoc gridColor </BLOCKQUOTE> */
        virtual void setGridColor(QColor __value);

        /** \brief sets the property minorGridColor to the specified \a __value.
         *  \details Description of the parameter minorGridColor is:  <BLOCKQUOTE>\copydoc minorGridColor </BLOCKQUOTE> */
        virtual void set_minorGridColor(QColor __value);

        /** \brief sets the property gridWidth to the specified \a __value.
         *  \details Description of the parameter gridWidth is:  <BLOCKQUOTE>\copydoc gridWidth </BLOCKQUOTE> */
        virtual void setGridWidth (double __value);

        /** \brief sets the property gridStyle to the specified \a __value.
         *  \details Description of the parameter gridStyle is:  <BLOCKQUOTE>\copydoc gridStyle </BLOCKQUOTE> */
        virtual void setGridStyle(Qt::PenStyle __value);

        /** \brief sets the property minorGridWidth to the specified \a __value.
         *  \details Description of the parameter minorGridWidth is:  <BLOCKQUOTE>\copydoc minorGridWidth </BLOCKQUOTE> */
        virtual void set_minorGridWidth(double __value);

        /** \brief sets the property minorGridStyle to the specified \a __value.
         *  \details Description of the parameter minorGridStyle is:  <BLOCKQUOTE>\copydoc minorGridStyle </BLOCKQUOTE> */
        virtual void set_minorGridStyle (Qt::PenStyle __value);


        /** \brief sets the property drawMode1 to the specified \a __value.
         *  \details Description of the parameter drawMode1 is:  <BLOCKQUOTE>\copydoc drawMode1 </BLOCKQUOTE> */
        virtual void set_drawMode1 (JKQTPCADrawMode __value);

        /** \brief sets the property drawMode2 to the specified \a __value.
         *  \details Description of the parameter drawMode2 is:  <BLOCKQUOTE>\copydoc drawMode2 </BLOCKQUOTE> */
        virtual void set_drawMode2(JKQTPCADrawMode __value);

        /** \brief sets the property minorTickWidth to the specified \a __value.
         *  \details Description of the parameter minorTickWidth is:  <BLOCKQUOTE>\copydoc minorTickWidth </BLOCKQUOTE> */
        virtual void set_minorTickWidth(double __value);

        /** \brief sets the property tickWidth to the specified \a __value.
         *  \details Description of the parameter tickWidth is:  <BLOCKQUOTE>\copydoc tickWidth </BLOCKQUOTE> */
        virtual void set_tickWidth (double __value);

        /** \brief sets the property lineWidth to the specified \a __value.
         *  \details Description of the parameter lineWidth is:  <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> */
        virtual void setLineWidth (double __value);

        /** \brief sets the property lineWidthZeroAxis to the specified \a __value.
         *  \details Description of the parameter lineWidthZeroAxis is:  <BLOCKQUOTE>\copydoc lineWidthZeroAxis </BLOCKQUOTE> */
        virtual void setLineWidthZeroAxis (double __value);

        /** \brief sets the property tickLabelDistance to the specified \a __value.
         *  \details Description of the parameter tickLabelDistance is:  <BLOCKQUOTE>\copydoc tickLabelDistance </BLOCKQUOTE> */
        virtual void set_tickLabelDistance(double __value);

        /** \brief sets the property labelDistance to the specified \a __value.
         *  \details Description of the parameter labelDistance is:  <BLOCKQUOTE>\copydoc labelDistance </BLOCKQUOTE> */
        virtual void set_labelDistance(double __value);
        /** \brief sets the property labelDigits to the specified \a __value.
         *  \details Description of the parameter labelDistance is:  <BLOCKQUOTE>\copydoc labelDigits </BLOCKQUOTE> */
        virtual void set_labelDigits(int __value);

        /** \brief sets the property drawGrid to the specified \a __value.
         *  \details Description of the parameter drawGrid is:  <BLOCKQUOTE>\copydoc drawGrid </BLOCKQUOTE> */
        virtual void set_drawGrid(bool __value);

        /** \brief sets the property drawMinorGrid to the specified \a __value.
         *  \details Description of the parameter drawMinorGrid is:  <BLOCKQUOTE>\copydoc drawMinorGrid </BLOCKQUOTE> */
        virtual void set_drawMinorGrid(bool __value);


        /** \brief sets the property tickLabelAngle to the specified \a __value.
         *  \details Description of the parameter tickLabelAngle is:  <BLOCKQUOTE>\copydoc tickLabelAngle </BLOCKQUOTE> */
        virtual void set_tickLabelAngle(double __value);

    protected:
        /** \brief indicates whether one of the parameters has changed sinse the last recalculation of tickspacing ... */
        bool paramsChanged;
        bool doUpdateScaling;

        /** \brief simply calls the replotPlot method of the parent plotter class */
        void replotPlot();
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
        /** \brief retun parents JKQTMathText* object */
        virtual JKQTMathText* get_parent_mathText();


        /** \brief convert a float to a tick label string */
        QString floattolabel(double data);

        /** \brief convert a float to a tick label string with a given precision */
        QString floattolabel(double data, int past_comma);
        /** \brief parent plotter class */
        JKQTBasePlotter* parent;
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
        bool autoAxisSpacing;
        /*! \brief default value for property property varname. \see autoAxisSpacing for more information */ 
        bool def_autoAxisSpacing;
        /** \brief if \c true, the plotter displays minor axis labels as number between 1 and 10 in some cases */
        bool minorTickLabelsEnabled;
        /*! \brief default value for property property varname. \see minorTickLabelsEnabled for more information */ 
        bool def_minorTickLabelsEnabled;
        /** \brief indicates whether the y axis has a logarithmic scale */
        bool logAxis;
        /*! \brief default value for property property varname. \see logAxis for more information */ 
        bool def_logAxis;
        /** \brief the base for a logarithmic x axis */
        double logAxisBase;
        /*! \brief default value for property property varname. \see logAxisBase for more information */ 
        double def_logAxisBase;
        /** \brief if autoXAxisSpacing is \c false then this value is used for xTickSpacing. So this is the property which
         *         is editable by use of public access methods.
         */
        double userTickSpacing;
        /*! \brief default value for property property varname. \see userTickSpacing for more information */ 
        double def_userTickSpacing;
        /** \brief if autoXAxisSpacing is \c false then this value is used for xTickSpacing. So this is the property which
         *         is editable by use of public access methods.
         */
        double userLogTickSpacing;
        /*! \brief default value for property property varname. \see userLogTickSpacing for more information */ 
        double def_userLogTickSpacing;

        /** \brief indicates how to draw the labels */
        JKQTPCALabelType labelType;
        /*! \brief default value for property property varname. \see labelType for more information */ 
        JKQTPCALabelType def_labelType;

        /** \brief mode of the major ticks */
        JKQTPLabelTickMode tickMode;
        /*! \brief default value for property property varname. \see tickMode for more information */ 
        JKQTPLabelTickMode def_tickMode;

        /** \brief axis label of the axis */
        QString axisLabel;

        /** \brief position of the axis label */
        JKQTPLabelPosition labelPosition;
        /*! \brief default value for property property varname. \see labelPosition for more information */ 
        JKQTPLabelPosition def_labelPosition;
        /** \brief font of the axis labels */
        QString labelFont;
        /*! \brief default value for property property varname. \see labelFont for more information */ 
        QString def_labelFont;
        /** \brief fontsize of the axis labels */
        double labelFontSize;
        /*! \brief default value for property property varname. \see labelFontSize for more information */ 
        double def_labelFontSize;
        /** \brief font of the axis tick labels */
        QString tickLabelFont;
        /*! \brief default value for property property varname. \see tickLabelFont for more information */ 
        QString def_tickLabelFont;
        /** \brief fontsize of the axis tick labels */
        double tickLabelFontSize;
        /*! \brief default value for property property varname. \see tickLabelFontSize for more information */ 
        double def_tickLabelFontSize;
        /** \brief fontsize of the minor axis tick labels */
        double minorTickLabelFontSize;
        /*! \brief default value for property property varname. \see minorTickLabelFontSize for more information */ 
        double def_minorTickLabelFontSize;
        /** \brief indicates whether to draw a thick axis line at x=0 (zero axis) */
        bool showZeroAxis;
        /*! \brief default value for property property varname. \see showZeroAxis for more information */ 
        bool def_showZeroAxis;
        /** \brief indicates whether the minor tick labels should be full numbers, or just a number between 0..10 */
        bool minorTickLabelFullNumber;
        /*! \brief default value for property property varname. \see minorTickLabelFullNumber for more information */ 
        bool def_minorTickLabelFullNumber;


        /** \brief draw mode of the main (left/bottom) axis */
        JKQTPCADrawMode drawMode1;
        /*! \brief default value for property property varname. \see drawMode1 for more information */ 
        JKQTPCADrawMode def_drawMode1;
        /** \brief draw mode of the secondary (right/top) axis */
        JKQTPCADrawMode drawMode2;
        /*! \brief default value for property property varname. \see drawMode2 for more information */ 
        JKQTPCADrawMode def_drawMode2;
        /** \brief line width of minor ticks in pt */
        double minorTickWidth;
        /*! \brief default value for property property varname. \see minorTickWidth for more information */ 
        double def_minorTickWidth;
        /** \brief line width of ticks in pt */
        double tickWidth;
        /*! \brief default value for property property varname. \see tickWidth for more information */ 
        double def_tickWidth;
        /** \brief line width of axis in pt */
        double lineWidth;
        /*! \brief default value for property property varname. \see lineWidth for more information */ 
        double def_lineWidth;
        /** \brief line width of 0-line in pt */
        double lineWidthZeroAxis;
        /*! \brief default value for property property varname. \see lineWidthZeroAxis for more information */ 
        double def_lineWidthZeroAxis;


        /** \brief format string for time tick labels, see see QDateTime::toString() documentation for details on format strings */
        QString tickTimeFormat;
        /*! \brief default value for property property varname. \see tickTimeFormat for more information */ 
        QString def_tickTimeFormat;
        /** \brief format string for date tick labels, see see QDateTime::toString() documentation for details on format strings */
        QString tickDateFormat;
        /*! \brief default value for property property varname. \see tickDateFormat for more information */ 
        QString def_tickDateFormat;
        /** \brief format string for datetime tick labels, see see QDateTime::toString() documentation for details on format strings */
        QString tickDateTimeFormat;
        /*! \brief default value for property property varname. \see tickDateTimeFormat for more information */ 
        QString def_tickDateTimeFormat;

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
         *   \f[ \Delta\mbox{tick}=f\cdot 10^\rho,\ \ \ \ \ f\in\{1, 2, 2.5, 5\} \f]
         *
         *   -# The algorithm starts by finding a start exponent \f$ \rho_s=\left\lfloor\log_{10}(\mbox{awidth})\right\rfloor+3 \f$. This
         *      is a good (over)estimate for the biggest possible power \f$ \rho \f$.
         *   -# now we set \f$ \rho=\rho_0 \f$ and \f$ f=10 \f$.
         *   -# now the algorithm cycles through all possible factors \f$ f\in\{1, 2, 2.5, 5, 10\} \f$ (starting from 10) and calculates
         *      \f$ \Delta\mbox{tick}=f\cdot 10^\rho \f$. Then it checks whether \f$ \mbox{tickcount}=\mbox{round}\left(\frac{\mbox{awidth}}{\Delta\mbox{tick}}\right) \f$
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
        unsigned int minTicks;
        /*! \brief default value for property property varname. \see minTicks for more information */ 
        unsigned int def_minTicks;
        /** \brief number of minor grid lines per axis tick interval */
        unsigned int minorTicks;
        /*! \brief default value for property property varname. \see minorTicks for more information */ 
        unsigned int def_minorTicks;
        /** \brief length of an axis tick outside the plot border  in pt */
        double tickOutsideLength;
        /*! \brief default value for property property varname. \see tickOutsideLength for more information */ 
        double def_tickOutsideLength;
        /** \brief length of a minor axis tick outside the plot border in pt */
        double minorTickOutsideLength;
        /*! \brief default value for property property varname. \see minorTickOutsideLength for more information */ 
        double def_minorTickOutsideLength;
        /** \brief length of an axis tick inside the plot border  in pt */
        double tickInsideLength;
        /*! \brief default value for property property varname. \see tickInsideLength for more information */ 
        double def_tickInsideLength;
        /** \brief length of a minor axis tick inside the plot border in pt */
        double minorTickInsideLength;
        /*! \brief default value for property property varname. \see minorTickInsideLength for more information */ 
        double def_minorTickInsideLength;
        /** \brief color of the axis (labels, ticks, axis itself ...) */
        QColor axisColor;
        /*! \brief default value for property property varname. \see axisColor for more information */ 
        QColor def_axisColor;
        /** \brief distance between tick end and label start in pt */
        double tickLabelDistance;
        /*! \brief default value for property property varname. \see tickLabelDistance for more information */ 
        double def_tickLabelDistance;
        /** \brief distance between tick label and axis label in pt */
        double labelDistance;
        /*! \brief default value for property property varname. \see labelDistance for more information */ 
        double def_labelDistance;
        /** \brief rotation angle of tick labels [-180..180], i.e. given in degrees, default is 0 (horizontal) */
        double tickLabelAngle;
        /*! \brief default value for property property varname. \see tickLabelAngle for more information */ 
        double def_tickLabelAngle;

        /** \brief indicates whether to draw the major grid lines */
        bool drawGrid;
        /*! \brief default value for property property varname. \see drawGrid for more information */ 
        bool def_drawGrid;
        /** \brief indicates whether to draw the minor grid lines */
        bool drawMinorGrid;
        /*! \brief default value for property property varname. \see drawMinorGrid for more information */ 
        bool def_drawMinorGrid;
        /** \brief color of the grid*/
        QColor gridColor;
        /*! \brief default value for property property varname. \see gridColor for more information */ 
        QColor def_gridColor;
        /** \brief color of the minor grid lines */
        QColor minorGridColor;
        /*! \brief default value for property property varname. \see minorGridColor for more information */ 
        QColor def_minorGridColor;
        /** \brief width of the grid lines (in pixel) */
        double gridWidth;
        /*! \brief default value for property property varname. \see gridWidth for more information */ 
        double def_gridWidth;
        /** \brief line stye of the grid lines */
        Qt::PenStyle gridStyle;
        /*! \brief default value for property property varname. \see gridStyle for more information */ 
        Qt::PenStyle def_gridStyle;
        /** \brief width of the minor grid lines (in pixel) */
        double minorGridWidth;
        /*! \brief default value for property property varname. \see minorGridWidth for more information */ 
        double def_minorGridWidth;
        /** \brief line stye of the minor grid lines */
        Qt::PenStyle minorGridStyle;
        /*! \brief default value for property property varname. \see minorGridStyle for more information */ 
        Qt::PenStyle def_minorGridStyle;

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
        QSizeF getMaxTickLabelSize(JKQTPEnhancedPainter& painter, double* ascent=nullptr, double* descent=nullptr);
};





/*! \brief implements a vertical axis, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

 */
class LIB_EXPORT JKQTPVerticalAxis: public JKQTPCoordinateAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPVerticalAxis(JKQTBasePlotter* parent);

        /** \brief returns the size of the left/bottom axis in pt */
        virtual QSize getSize1(JKQTPEnhancedPainter& painter) override;

        /** \brief returns the size of the right/top axis in pt */
        virtual QSize getSize2(JKQTPEnhancedPainter& painter) override;

        /** \brief draw axes */
        virtual void drawAxes(JKQTPEnhancedPainter& painter) override;

        /** \brief draw grids  */
        virtual void drawGrids(JKQTPEnhancedPainter& painter) override;


    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const override;
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const override;
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const override;
        virtual bool get_parent_otheraxis_inverted() const override;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const override;

        virtual void drawTickLabel1(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize) ;
        virtual void drawTickLabel2(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize) ;
};


/*! \brief implements a position-indipendent vertical axis, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    This axis may be draw at a user-supplied position (used e.g. for color bar axes)
 */
class LIB_EXPORT JKQTPVerticalIndependentAxis: public JKQTPVerticalAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPVerticalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQTBasePlotter* parent);
        /** \brief set the axis offset */
        virtual void setAxisOffset(double __value) ;
        /** \brief set the axis width */
        virtual void setAxisWidth(double __value) ;
        /** \brief set the other axis offset */
        virtual void set_otherAxisOffset(double __value) ;
        /** \brief set the other axis width */
        virtual void set_otherAxisWidth(double __value) ;
        /** \brief set the other axis width */
        virtual void set_otherAxisInverted(bool __value) ;
    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const override;
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const override;
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const override;
        virtual bool get_parent_otheraxis_inverted() const override;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const override;

        double axisOffset;
        double axisWidth;
        double otherAxisWidth;
        double otherAxisOffset;
        bool otherAxisInverted;
};



/*! \brief implements a horizontal axis, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

 */
class LIB_EXPORT JKQTPHorizontalAxis: public JKQTPCoordinateAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPHorizontalAxis(JKQTBasePlotter* parent);

        /** \brief returns the size of the left/bottom axis in pt */
        virtual QSize getSize1(JKQTPEnhancedPainter& painter) override;

        /** \brief returns the size of the right/top axis in pt */
        virtual QSize getSize2(JKQTPEnhancedPainter& painter) override;

        /** \brief draw axes */
        virtual void drawAxes(JKQTPEnhancedPainter& painter) override;

        /** \brief draw grids  */
        virtual void drawGrids(JKQTPEnhancedPainter& painter) override;

    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const override;
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const override;
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const override;
        virtual bool get_parent_otheraxis_inverted() const override;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const override;


        virtual void drawTickLabel1(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize, double ascentMax, double descentMax) ;
        virtual void drawTickLabel2(JKQTPEnhancedPainter& painter, double xx, double yy, const QString &label, double fontSize, double ascentMax, double descentMax) ;

};


/*! \brief implements a position-indipendent horizontal axis, based on JKQTPCoordinateAxis (for most of documentation: see JKQTPCoordinateAxis).
    \ingroup jkqtpbaseplotter_elements

    This axis may be draw at a user-supplied position (used e.g. for color bar axes)
 */
class LIB_EXPORT JKQTPHorizontalIndependentAxis: public JKQTPHorizontalAxis {
        Q_OBJECT
    protected:
    public:
        /** \brief class constructor */
        JKQTPHorizontalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQTBasePlotter* parent);
        /** \brief se the axis offset */
        virtual void setAxisOffset(double __value);
        /** \brief se the axis width */
        virtual void setAxisWidth(double __value);
        /** \brief set the other axis offset */
        virtual void set_otherAxisOffset(double __value);
        /** \brief set the other axis width */
        virtual void set_otherAxisWidth(double __value);
        virtual void set_otherAxisInverted(bool __value);
    protected:
        /** \brief width of the plot in the direction of the axis */
        virtual double get_parent_plotwidth() const override;
        /** \brief offset of the plot in the direction of the axis */
        virtual double get_parent_plotoffset() const override;
        /** \brief pixel of other (perpendicular) axis (needed for grids) */
        virtual double get_parent_otheraxis_width() const override;
        virtual bool get_parent_otheraxis_inverted() const override;
        /** \brief pixel offset of (perpendicular) other axis (needed for grids) */
        virtual double get_parent_otheraxis_offset() const override;

        double axisOffset;
        double axisWidth;
        double otherAxisWidth;
        double otherAxisOffset;
        bool otherAxisInverted;
};

#endif // JKQTPBASEELEMENTS_H
