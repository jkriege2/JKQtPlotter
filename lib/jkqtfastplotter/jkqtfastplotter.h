/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>) (DKFZ) & IWR, University of Heidelberg

    

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


/*
  Name: jkqtpfastplotter.h
  Copyright: (c) 2010-2019
  Author: Jan krieger <jan@jkrieger.de>, http://www.jkrieger.de/
*/


#ifndef JKQTFASTPLOTTER_H
#define JKQTFASTPLOTTER_H

#include "jkqtcommon/jkqtp_imexport.h"
#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QColor>
#include <QVector>
#include <QPair>
#include <QStringList>
#include <QIcon>
#include <cmath>
#include <iostream>
#include <QMutex>
#include <QGLWidget>

#include "jkqtcommon/jkqtptools.h"
#ifdef DEBUG_TIMING
#  include "jkqtplottertools/jkqtphighrestimer.h"
#endif


// forward declaration
class JKQTFPPlot;




/*! \brief a speed optimized plotter class
    \ingroup jkqtfastplotter

    This plotter class is (in contrast to JKQTPlotter) optimized for speed. It allows to plot function graphs/data in
    a simple manner. The focus of design was not on a nice output (athough this is not unimportant), but on the speed
    of the output. So the class may be used to di quick plotting of data, i.e. in fast image sequence applications.

    These measures were taken to increase speed:
      - The coordinate system is drawn onto an internal image which is only updated, when the size of the widget
        or a property of the coordinate system changes. When only the data changes, just the data plot has to be
        updated.
      - The class uses an internal buffer with the complete image, so a repaint does not always trigger a complete
        repaint of all elements, but just measns painting the internal image to the screen. This also allows to
        plot selections ... over the plot, without having to replot the plots.
      - The definition of the coordinate system as well as the definition the system ticks ... is reduced to the absolute
        minimum.
      - The method setPlotUpdateEnabled() allows to prevent replotting of the class contents, e.g. when you want to set
        a multitude of parameters without triggering a replot every time.
    .

    The class supports these features:
      - coordinate axes may be linear or logarithmic.
      - ticks and tick labels are drawn by a simple algorithm: The user has to supply two values:
        \c zeroTick is a tick value that exists in any case (i.e. 0) and \c tickDistance which contains the distance
        between two ticks (only applicable to linear axes). Then the class starts to draw at zeroTick and move on to
        \c zeroTick+tickDistance and \c zeroTick-tickDistance until \c min / \c max of the axis is reached. For logarithmic axes
        the class uses \c zeroTick*10 and \c zeroTick/10
      - plot may contain a grid (x and y grid may be switched on/off separately).
      - plots are represented by descendents of JKQTFPPlot and the plotting code is completely
        contained therein (and should be speed optimized).
      - it is possible to keep a specified aspect ratio
      - it is possible to couple different plots in a way, that they are plottetd with the same width/height
        both in pixel and world coordinates by using plotterSizesChanged() and synchronizeX() / synchronizeY().
    .

*/
class JKQTP_LIB_EXPORT JKQTFastPlotter : public QGLWidget {
        Q_OBJECT
    public:

        /*! \brief smallest allowed line width for JKQTFastPlotter
        */
        static const double ABS_MIN_LINEWIDTH;

        /*! \brief size of the lookup tables used by JKQTFPimagePlot_array2image()
        */
        static const int LUTSIZE;

    protected:
        /** \brief indicates whether to do full repaint (system and data) at the next repaint (any of the repaint meothods) */
        bool doFullRepaint;
        /** \brief this stores the currently displayed coordinate system */
        QImage systemImage;
        /** \brief this stores the currently displayed plot */
        QImage image;

        /** \brief this can be used when drawing a zoom rectangle to store an unchanged
         *         copy of the currently displayed image.
         */
        QImage oldImage;

        /** \brief if \c false the plotting won't be performed (may be used to set more than one parameter without replot */
        bool doDrawing;

        /** \brief list of plots in this graph */
        QVector<JKQTFPPlot*> plots;



        /** \brief free space between widget left border and plot left border  */
        int plotBorderLeft; 
        /*! \brief default value for property plotBorderLeft. 
            \see plotBorderLeft for more information */ 
        int default_plotBorderLeft;
        /** \brief free space between widget bottom border and plot bottom border  */
        int plotBorderBottom; 
        /*! \brief default value for property plotBorderBottom. 
            \see plotBorderBottom for more information */ 
        int default_plotBorderBottom;
        /** \brief free space between widget right border and plot left border */
        int plotBorderRight; 
        /*! \brief default value for property plotBorderRight. 
            \see plotBorderRight for more information */ 
        int default_plotBorderRight;
        /** \brief free space between widget top border and plot bottom border */
        int plotBorderTop; 
        /*! \brief default value for property plotBorderTop. 
            \see plotBorderTop for more information */ 
        int default_plotBorderTop;

        /** \brief internally calculated: free space between widget left border and plot left border
         *
         * \note This is the actual border used for plotting, as it may be different from what the user supplied.
         *       Note also that this is at least as huge as the suer supplied border value!
         */
        int internalPlotBorderLeft;
        /** \brief internally calculated: free space between widget bottom border and plot bottom border
         *
         * \note This is the actual border used for plotting, as it may be different from what the user supplied.
         *       Note also that this is at least as huge as the suer supplied border value!
         */
        int internalPlotBorderBottom;
        /** \brief internally calculated: free space between widget right border and plot left border
         *
         * \note This is the actual border used for plotting, as it may be different from what the user supplied.
         *       Note also that this is at least as huge as the suer supplied border value!
         */
        int internalPlotBorderRight;
        /** \brief internally calculated: free space between widget top border and plot bottom border
         *
         * \note This is the actual border used for plotting, as it may be different from what the user supplied.
         *       Note also that this is at least as huge as the suer supplied border value!
         */
        int internalPlotBorderTop;

        /** \brief plot width in pt inside the widget (calculated by calcPlotScaling() from plotBorderLeft, plotBorderRight and widgetWidth) */
        int plotWidth;
        /** \brief plot height in pt inside the widget (calculated by calcPlotScaling() from plotBorderTop, plotBorderBottom and widgetHeight) */
        int plotHeight;

        /** \brief color of the coordinate system */
        QColor systemColor; 
        /*! \brief default value for property systemColor. 
            \see systemColor for more information */ 
        QColor default_systemColor;
        /** \brief width of the coordinate (in pixel) */
        double systemWidth; 
        /*! \brief default value for property systemWidth. 
            \see systemWidth for more information */ 
        double default_systemWidth;
        /** \brief color of the background*/
        QColor backgroundColor; 
        /*! \brief default value for property backgroundColor. 
            \see backgroundColor for more information */ 
        QColor default_backgroundColor;
        /** \brief color of the plot's background
         *
         *  \note the background is not drawn if this color is set to \c QColor(Qt::transparent) !
         */
        QColor plotBackgroundColor; 
        /*! \brief default value for property plotBackgroundColor. 
            \see plotBackgroundColor for more information */ 
        QColor default_plotBackgroundColor;
        /** \brief indicates whether to draw a system box */
        bool drawSystemBox; 
        /*! \brief default value for property drawSystemBox. 
            \see drawSystemBox for more information */ 
        bool default_drawSystemBox;
        /** \brief indicates whether to draw axes at x=0/y=0 */
        bool drawZeroAxes; 
        /*! \brief default value for property drawZeroAxes. 
            \see drawZeroAxes for more information */ 
        bool default_drawZeroAxes;

        /** \brief indicates whether to draw a grid */
        bool drawGrid; 
        /*! \brief default value for property drawGrid. 
            \see drawGrid for more information */ 
        bool default_drawGrid;
        /** \brief color of the coordinate grid */
        QColor gridColor; 
        /*! \brief default value for property gridColor. 
            \see gridColor for more information */ 
        QColor default_gridColor;
        /** \brief style of the coordinate grid */
        Qt::PenStyle gridStyle; 
        /*! \brief default value for property gridStyle. 
            \see gridStyle for more information */ 
        Qt::PenStyle default_gridStyle;
        /** \brief width of the coordinate grid (in pixel) */
        double gridWidth; 
        /*! \brief default value for property gridWidth. 
            \see gridWidth for more information */ 
        double default_gridWidth;
        /** \brief font size (in points) of the axis labels */
        double labelFontSize; 
        /*! \brief default value for property labelFontSize. 
            \see labelFontSize for more information */ 
        double default_labelFontSize;
        /** \brief font name of the axis labels */
        QString labelFontName; 
        /*! \brief default value for property labelFontName. 
            \see labelFontName for more information */ 
        QString default_labelFontName;
        /** \brief font size (in points) of the tick labels */
        double tickFontSize; 
        /*! \brief default value for property tickFontSize. 
            \see tickFontSize for more information */ 
        double default_tickFontSize;
        /** \brief font name of the tick labels */
        QString tickFontName; 
        /*! \brief default value for property tickFontName. 
            \see tickFontName for more information */ 
        QString default_tickFontName;

        /** \brief length of an axis tick (pixels) */
        double tickLength; 
        /*! \brief default value for property tickLength. 
            \see tickLength for more information */ 
        double default_tickLength;


        /** \brief minimum value of x axis */
        double xMin;
        /** \brief maximum value of x axis */
        double xMax;
        /** \brief minimum value of y axis */
        double yMin;
        /** \brief maximum value of y axis */
        double yMax;
        /** \brief is x axis logarithmic? */
        bool xAxisLog;
        /** \brief is x axis logarithmic? */
        bool yAxisLog;

        /** \brief width of plot on x axis (calculated internally)  */
        double xWidth;
        /** \brief x axis scaling factor (calculated internally)  */
        double xScale;
        /** \brief x axis offset (calculated internally)  */
        double xOffset;
        /** \brief width of plot on y axis (calculated internally)  */
        double yWidth;
        /** \brief y axis scaling factor (calculated internally)  */
        double yScale;
        /** \brief y axis offset (calculated internally)  */
        double yOffset;
        /** \brief a label position from which axis label plotting is started for the x axis */
        double xZeroTick;
        /** \brief a label position from which axis label plotting is started for the y axis */
        double yZeroTick;
        /** \brief distance between two labels on x axis */
        double xTickDistance;
        /** \brief distance between two labels on y axis */
        double yTickDistance;
        /** \brief label of x axis */
        QString xAxisLabel;
        /** \brief label of y axis */
        QString yAxisLabel;

        /** \brief plot y axis label */
        bool yAxisLabelVisible;
        /** \brief plot x axis label */
        bool xAxisLabelVisible;

        /** \brief synchronize x-axis settings to this plotter */
        JKQTFastPlotter* synchronizeX;
        /** \brief synchronize y-axis settings to this plotter */
        JKQTFastPlotter* synchronizeY;

        /** \brief aspect ratio of the plot, only used when maintainAspectRation is \c true
         *
         * The aspect ratio is defined as \c width/height of the plot in pt.
         * So if you want to have a plot spanning \c x=0..20 and \c y=0..10 where each 1x1 square should be
         * as wide as high  you will have to set aspectRatio to \c 2 .
         *
         * \note Note that aspect ratios only make sense for linear-linear plots. This will be ignored
         *       if any one axis is logarithmic.
         */
        double aspectRatio;
        /** \brief indicates whether to maintain a specific aspect ratio */
        bool maintainAspectRatio;

        QPoint mouseDragStart;
        QPoint mouseDragEnd;
        bool dragging;

        QPen dragShapePen;
        bool dragLine;

        QAction* actCopyImage;

        /** \brief this simply paints the stored image to the widget's surface */
        virtual void paintEvent(QPaintEvent *event) override;

        /** \brief resizes the internal representation (image) of the graphs */
        virtual void resizeEvent(QResizeEvent *event) override;


        /** \brief event handler for a double click */
        virtual void mouseDoubleClickEvent ( QMouseEvent * event ) override;

        /** \brief event handler for a mouse move */
        virtual void mouseMoveEvent ( QMouseEvent * event ) override;

        /** \brief event handler for a mouse button press */
        virtual void mousePressEvent ( QMouseEvent * event ) override;

        /** \brief event handler for a mouse button is released */
        virtual void mouseReleaseEvent(QMouseEvent* event) override;

        /** \brief paint the coordinate system */
        void plotSystem(QPainter& painter);

        /** \brief paint the graphs */
        void plotGraphs(QPainter& painter);

        /** \brief recalculate the scaling of the plot */
        void calcPlotScaling();

        QSize minimumSizeHint() const override;

        /**  \brief mutex to lock global widget repaint */
        QMutex mutexRepaint;

        /**  \brief mutex to lock data repaint */
        QMutex mutexRepaintData;

        /**  \brief mutex to lock system repaint */
        QMutex mutexRepaintSystem;

    public:
        /** \brief class constructor */
        JKQTFastPlotter(QWidget *parent = nullptr);

        /** \brief set the properties of the x axis */
        void setXRange(double min, double max, bool logarithmic=false);

        /** \brief set the properties of the y axis */
        void setYRange(double min, double max, bool logarithmic=false);

        /** \brief set the properties of both axes */
        void setXYRange(double xmin, double xmax, double ymin, double ymax, bool xlogarithmic=false, bool ylogarithmic=false);

        /** \brief clear all plots in the graph
         *
         * \param remove if \c true the object will also be deleted from memory, not just from the list
         */
        void clearPlots(bool remove=false);
        /** \brief add a new graph */
        void addPlot(JKQTFPPlot* g);
        /** \brief return the internal number (0..N-1) of the given plot, or -1*/
        int getPlotNum(JKQTFPPlot* g);
        /** \brief remove given graph
         *
         * \param id specifies the graph to delete
         * \param remove if \c true the object will also be deleted from memory, not just from the list
         */
        void deletePlot(int id, bool remove=false);
        /** \brief remove given graph
         *
         * \param g specifies the graph to delete
         * \param remove if \c true the object will also be deleted from memory, not just from the list
         */
        void deletePlot(JKQTFPPlot* g, bool remove=false);


        /** \brief return x-pixel coordinate from x coordinate */
        inline double x2p(double x) {
            if (xAxisLog) {
                if (x<0) return xOffset+log(xMin/10.0)/log(10.0)*xScale;
                return xOffset+log(x)/log(10.0)*xScale;
            } else {
                return xOffset+x*xScale;
            }
        }

        /** \brief return x coordinate coordinate from x-pixel */
        inline double p2x(long x) {
            if (xAxisLog) {
                return exp(log(10.0)*(static_cast<double>(x)-xOffset)/(xScale));
            } else {
                return (static_cast<double>(x)-xOffset)/(xScale);
            }
        }

        /** \brief return y-pixel coordinate from y coordinate */
        inline double y2p(double y) {
            if (yAxisLog) {
                if (y<0) return yOffset-log(yMin/10.0)/log(10.0)*yScale;
                return yOffset-log(y)/log(10.0)*yScale;
            } else {
                return yOffset-y*yScale;
            }
        }

        /** \brief return y coordinate coordinate from y-pixel */
        inline double p2y(long y) {
            if (yAxisLog) {
                return exp(log(10.0)*(static_cast<double>(y)-yOffset)/(-1.0*yScale));
            } else {
                return (static_cast<double>(y)-yOffset)/(-1.0*yScale);
            }
        }

        /*! \brief sets the property dragShapePen ( \copybrief dragShapePen ) to the specified \a __value. 
            \details Description of the parameter dragShapePen is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::dragShapePen </BLOCKQUOTE>
            \see dragShapePen for more information */ 
        inline void setDragShapePen(const QPen & __value)
        {
            if (this->dragShapePen != __value) {
                this->dragShapePen = __value; 
                update(); 
            } 
        } 
        /*! \brief returns the property dragShapePen ( \copybrief dragShapePen ). 
            \details Description of the parameter dragShapePen is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::dragShapePen </BLOCKQUOTE>
            \see dragShapePen for more information */ 
        inline QPen getDragShapePen() const
        {
            return this->dragShapePen; 
        }
        /*! \brief sets the property dragLine ( \copybrief dragLine ) to the specified \a __value. 
            \details Description of the parameter dragLine is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::dragLine </BLOCKQUOTE>
            \see dragLine for more information */ 
        inline void setDragLine(bool __value)
        {
            if (this->dragLine != __value) {
                this->dragLine = __value; 
                update(); 
            } 
        } 
        /*! \brief returns the property dragLine ( \copybrief dragLine ). 
            \details Description of the parameter dragLine is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::dragLine </BLOCKQUOTE>
            \see dragLine for more information */ 
        inline bool getDragLine() const
        {
            return this->dragLine; 
        }

        /*! \brief sets the property plotBorderLeft ( \copybrief plotBorderLeft ) to the specified \a __value. 
            \details Description of the parameter plotBorderLeft is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderLeft </BLOCKQUOTE>
            \see plotBorderLeft for more information */ 
        inline void setPlotBorderLeft(int __value)
        {
            if (this->plotBorderLeft != __value) {
                this->plotBorderLeft = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotBorderLeft ( \copybrief plotBorderLeft ). 
            \details Description of the parameter plotBorderLeft is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderLeft </BLOCKQUOTE>
            \see plotBorderLeft for more information */ 
        inline int getPlotBorderLeft() const
        {
            return this->plotBorderLeft; 
        }
        /*! \brief sets the property plotBorderBottom ( \copybrief plotBorderBottom ) to the specified \a __value. 
            \details Description of the parameter plotBorderBottom is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderBottom </BLOCKQUOTE>
            \see plotBorderBottom for more information */ 
        inline void setPlotBorderBottom(int __value)
        {
            if (this->plotBorderBottom != __value) {
                this->plotBorderBottom = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotBorderBottom ( \copybrief plotBorderBottom ). 
            \details Description of the parameter plotBorderBottom is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderBottom </BLOCKQUOTE>
            \see plotBorderBottom for more information */ 
        inline int getPlotBorderBottom() const
        {
            return this->plotBorderBottom; 
        }
        /*! \brief sets the property plotBorderRight ( \copybrief plotBorderRight ) to the specified \a __value. 
            \details Description of the parameter plotBorderRight is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderRight </BLOCKQUOTE>
            \see plotBorderRight for more information */ 
        inline void setPlotBorderRight(int __value)
        {
            if (this->plotBorderRight != __value) {
                this->plotBorderRight = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotBorderRight ( \copybrief plotBorderRight ). 
            \details Description of the parameter plotBorderRight is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderRight </BLOCKQUOTE>
            \see plotBorderRight for more information */ 
        inline int getPlotBorderRight() const
        {
            return this->plotBorderRight; 
        }
        /*! \brief sets the property plotBorderTop ( \copybrief plotBorderTop ) to the specified \a __value. 
            \details Description of the parameter plotBorderTop is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderTop </BLOCKQUOTE>
            \see plotBorderTop for more information */ 
        inline void setPlotBorderTop(int __value)
        {
            if (this->plotBorderTop != __value) {
                this->plotBorderTop = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotBorderTop ( \copybrief plotBorderTop ). 
            \details Description of the parameter plotBorderTop is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBorderTop </BLOCKQUOTE>
            \see plotBorderTop for more information */ 
        inline int getPlotBorderTop() const
        {
            return this->plotBorderTop; 
        }
        /*! \brief returns the property plotWidth ( \copybrief plotWidth ). 
            \details Description of the parameter plotWidth is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotWidth </BLOCKQUOTE>. 
            \see plotWidth for more information */ 
        inline int getPlotWidth() const { 
            return this->plotWidth; 
        }
        /*! \brief returns the property plotHeight ( \copybrief plotHeight ). 
            \details Description of the parameter plotHeight is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotHeight </BLOCKQUOTE>. 
            \see plotHeight for more information */ 
        inline int getPlotHeight() const { 
            return this->plotHeight; 
        }
        /*! \brief sets the property doDrawing ( \copybrief doDrawing ) to the specified \a __value. 
            \details Description of the parameter doDrawing is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::doDrawing </BLOCKQUOTE>
            \see doDrawing for more information */ 
        inline void setPlotUpdateEnabled(bool __value)
        {
            this->doDrawing = __value;
        } 
        /*! \brief returns the property doDrawing ( \copybrief doDrawing ). 
            \details Description of the parameter doDrawing is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::doDrawing </BLOCKQUOTE>
            \see doDrawing for more information */ 
        inline bool isPlotUpdateEnabled() const
        {
            return this->doDrawing; 
        }
        /*! \brief returns the property internalPlotBorderLeft ( \copybrief internalPlotBorderLeft ). 
            \details Description of the parameter internalPlotBorderLeft is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::internalPlotBorderLeft </BLOCKQUOTE>. 
            \see internalPlotBorderLeft for more information */ 
        inline int getInternalPlotBorderLeft() const { 
            return this->internalPlotBorderLeft; 
        }
        /*! \brief returns the property internalPlotBorderBottom ( \copybrief internalPlotBorderBottom ). 
            \details Description of the parameter internalPlotBorderBottom is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::internalPlotBorderBottom </BLOCKQUOTE>. 
            \see internalPlotBorderBottom for more information */ 
        inline int getInternalPlotBorderBottom() const { 
            return this->internalPlotBorderBottom; 
        }
        /*! \brief returns the property internalPlotBorderRight ( \copybrief internalPlotBorderRight ). 
            \details Description of the parameter internalPlotBorderRight is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::internalPlotBorderRight </BLOCKQUOTE>. 
            \see internalPlotBorderRight for more information */ 
        inline int getInternalPlotBorderRight() const { 
            return this->internalPlotBorderRight; 
        }
        /*! \brief returns the property internalPlotBorderTop ( \copybrief internalPlotBorderTop ). 
            \details Description of the parameter internalPlotBorderTop is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::internalPlotBorderTop </BLOCKQUOTE>. 
            \see internalPlotBorderTop for more information */ 
        inline int getInternalPlotBorderTop() const { 
            return this->internalPlotBorderTop; 
        }
        /*! \brief returns the property xMin ( \copybrief xMin ). 
            \details Description of the parameter xMin is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::xMin </BLOCKQUOTE>. 
            \see xMin for more information */ 
        inline double getXMin() const { 
            return this->xMin; 
        }
        /*! \brief returns the property xMax ( \copybrief xMax ). 
            \details Description of the parameter xMax is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::xMax </BLOCKQUOTE>. 
            \see xMax for more information */ 
        inline double getXMax() const { 
            return this->xMax; 
        }
        /*! \brief returns the property yMin ( \copybrief yMin ). 
            \details Description of the parameter yMin is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::yMin </BLOCKQUOTE>. 
            \see yMin for more information */ 
        inline double getYMin() const { 
            return this->yMin; 
        }
        /*! \brief returns the property yMax ( \copybrief yMax ). 
            \details Description of the parameter yMax is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::yMax </BLOCKQUOTE>. 
            \see yMax for more information */ 
        inline double getYMax() const { 
            return this->yMax; 
        }
        /*! \brief returns the property xAxisLog ( \copybrief xAxisLog ). 
            \details Description of the parameter xAxisLog is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::xAxisLog </BLOCKQUOTE>. 
            \see xAxisLog for more information */ 
        inline bool getXAxisLog() const { 
            return this->xAxisLog; 
        }
        /*! \brief returns the property yAxisLog ( \copybrief yAxisLog ). 
            \details Description of the parameter yAxisLog is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::yAxisLog </BLOCKQUOTE>. 
            \see yAxisLog for more information */ 
        inline bool getYAxisLog() const { 
            return this->yAxisLog; 
        }

        /*! \brief sets the property backgroundColor ( \copybrief backgroundColor ) to the specified \a __value. 
            \details Description of the parameter backgroundColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::backgroundColor </BLOCKQUOTE>
            \see backgroundColor for more information */ 
        inline void setBackgroundColor(const QColor & __value)
        {
            if (this->backgroundColor != __value) {
                this->backgroundColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property backgroundColor ( \copybrief backgroundColor ). 
            \details Description of the parameter backgroundColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::backgroundColor </BLOCKQUOTE>
            \see backgroundColor for more information */ 
        inline QColor getBackgroundColor() const
        {
            return this->backgroundColor; 
        }
        /*! \brief sets the property plotBackgroundColor ( \copybrief plotBackgroundColor ) to the specified \a __value. 
            \details Description of the parameter plotBackgroundColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBackgroundColor </BLOCKQUOTE>
            \see plotBackgroundColor for more information */ 
        inline void setPlotBackgroundColor(const QColor & __value)
        {
            if (this->plotBackgroundColor != __value) {
                this->plotBackgroundColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotBackgroundColor ( \copybrief plotBackgroundColor ). 
            \details Description of the parameter plotBackgroundColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::plotBackgroundColor </BLOCKQUOTE>
            \see plotBackgroundColor for more information */ 
        inline QColor getPlotBackgroundColor() const
        {
            return this->plotBackgroundColor; 
        }
        /*! \brief sets the property drawGrid ( \copybrief drawGrid ) to the specified \a __value. 
            \details Description of the parameter drawGrid is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::drawGrid </BLOCKQUOTE>
            \see drawGrid for more information */ 
        inline void setDrawGrid(bool __value)
        {
            if (this->drawGrid != __value) {
                this->drawGrid = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property drawGrid ( \copybrief drawGrid ). 
            \details Description of the parameter drawGrid is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::drawGrid </BLOCKQUOTE>
            \see drawGrid for more information */ 
        inline bool getDrawGrid() const
        {
            return this->drawGrid; 
        }
        /*! \brief sets the property gridColor ( \copybrief gridColor ) to the specified \a __value. 
            \details Description of the parameter gridColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::gridColor </BLOCKQUOTE>
            \see gridColor for more information */ 
        inline void setGridColor(const QColor & __value)
        {
            if (this->gridColor != __value) {
                this->gridColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property gridColor ( \copybrief gridColor ). 
            \details Description of the parameter gridColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::gridColor </BLOCKQUOTE>
            \see gridColor for more information */ 
        inline QColor getGridColor() const
        {
            return this->gridColor; 
        }
        /*! \brief sets the property gridStyle ( \copybrief gridStyle ) to the specified \a __value. 
            \details Description of the parameter gridStyle is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::gridStyle </BLOCKQUOTE>
            \see gridStyle for more information */ 
        inline void setGridStyle(Qt::PenStyle  __value)
        {
            if (this->gridStyle != __value) {
                this->gridStyle = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property gridStyle ( \copybrief gridStyle ). 
            \details Description of the parameter gridStyle is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::gridStyle </BLOCKQUOTE>
            \see gridStyle for more information */ 
        inline Qt::PenStyle getGridStyle() const
        {
            return this->gridStyle; 
        }
        /*! \brief sets the property gridWidth ( \copybrief gridWidth ) to the specified \a __value. 
            \details Description of the parameter gridWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::gridWidth </BLOCKQUOTE>
            \see gridWidth for more information */ 
        inline void setGridWidth(double __value)
        {
            if (this->gridWidth != __value) {
                this->gridWidth = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property gridWidth ( \copybrief gridWidth ). 
            \details Description of the parameter gridWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::gridWidth </BLOCKQUOTE>
            \see gridWidth for more information */ 
        inline double getGridWidth() const
        {
            return this->gridWidth; 
        }
        /*! \brief sets the property labelFontSize ( \copybrief labelFontSize ) to the specified \a __value. 
            \details Description of the parameter labelFontSize is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::labelFontSize </BLOCKQUOTE>
            \see labelFontSize for more information */ 
        inline void setLabelFontSize(double __value)
        {
            if (this->labelFontSize != __value) {
                this->labelFontSize = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property labelFontSize ( \copybrief labelFontSize ). 
            \details Description of the parameter labelFontSize is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::labelFontSize </BLOCKQUOTE>
            \see labelFontSize for more information */ 
        inline double getLabelFontSize() const
        {
            return this->labelFontSize; 
        }
        /*! \brief sets the property labelFontName ( \copybrief labelFontName ) to the specified \a __value. 
            \details Description of the parameter labelFontName is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::labelFontName </BLOCKQUOTE>
            \see labelFontName for more information */ 
        inline void setLabelFontName(const QString & __value)
        {
            if (this->labelFontName != __value) {
                this->labelFontName = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property labelFontName ( \copybrief labelFontName ). 
            \details Description of the parameter labelFontName is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::labelFontName </BLOCKQUOTE>
            \see labelFontName for more information */ 
        inline QString getLabelFontName() const
        {
            return this->labelFontName; 
        }
        /*! \brief sets the property tickFontSize ( \copybrief tickFontSize ) to the specified \a __value. 
            \details Description of the parameter tickFontSize is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::tickFontSize </BLOCKQUOTE>
            \see tickFontSize for more information */ 
        inline void setTickFontSize(double __value)
        {
            if (this->tickFontSize != __value) {
                this->tickFontSize = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property tickFontSize ( \copybrief tickFontSize ). 
            \details Description of the parameter tickFontSize is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::tickFontSize </BLOCKQUOTE>
            \see tickFontSize for more information */ 
        inline double getTickFontSize() const
        {
            return this->tickFontSize; 
        }
        /*! \brief sets the property tickFontName ( \copybrief tickFontName ) to the specified \a __value. 
            \details Description of the parameter tickFontName is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::tickFontName </BLOCKQUOTE>
            \see tickFontName for more information */ 
        inline void setTickFontName(const QString & __value)
        {
            if (this->tickFontName != __value) {
                this->tickFontName = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property tickFontName ( \copybrief tickFontName ). 
            \details Description of the parameter tickFontName is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::tickFontName </BLOCKQUOTE>
            \see tickFontName for more information */ 
        inline QString getTickFontName() const
        {
            return this->tickFontName; 
        }
        /*! \brief sets the property tickLength ( \copybrief tickLength ) to the specified \a __value. 
            \details Description of the parameter tickLength is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::tickLength </BLOCKQUOTE>
            \see tickLength for more information */ 
        inline void setTickLength(double __value)
        {
            if (this->tickLength != __value) {
                this->tickLength = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property tickLength ( \copybrief tickLength ). 
            \details Description of the parameter tickLength is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::tickLength </BLOCKQUOTE>
            \see tickLength for more information */ 
        inline double getTickLength() const
        {
            return this->tickLength; 
        }
        /*! \brief sets the property drawSystemBox ( \copybrief drawSystemBox ) to the specified \a __value. 
            \details Description of the parameter drawSystemBox is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::drawSystemBox </BLOCKQUOTE>
            \see drawSystemBox for more information */ 
        inline void setDrawSystemBox(bool __value)
        {
            if (this->drawSystemBox != __value) {
                this->drawSystemBox = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property drawSystemBox ( \copybrief drawSystemBox ). 
            \details Description of the parameter drawSystemBox is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::drawSystemBox </BLOCKQUOTE>
            \see drawSystemBox for more information */ 
        inline bool getDrawSystemBox() const
        {
            return this->drawSystemBox; 
        }
        /*! \brief sets the property drawZeroAxes ( \copybrief drawZeroAxes ) to the specified \a __value. 
            \details Description of the parameter drawZeroAxes is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::drawZeroAxes </BLOCKQUOTE>
            \see drawZeroAxes for more information */ 
        inline void setDrawZeroAxes(bool __value)
        {
            if (this->drawZeroAxes != __value) {
                this->drawZeroAxes = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property drawZeroAxes ( \copybrief drawZeroAxes ). 
            \details Description of the parameter drawZeroAxes is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::drawZeroAxes </BLOCKQUOTE>
            \see drawZeroAxes for more information */ 
        inline bool getDrawZeroAxes() const
        {
            return this->drawZeroAxes; 
        }
        /*! \brief sets the property systemColor ( \copybrief systemColor ) to the specified \a __value. 
            \details Description of the parameter systemColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::systemColor </BLOCKQUOTE>
            \see systemColor for more information */ 
        inline void setSystemColor(const QColor & __value)
        {
            if (this->systemColor != __value) {
                this->systemColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property systemColor ( \copybrief systemColor ). 
            \details Description of the parameter systemColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::systemColor </BLOCKQUOTE>
            \see systemColor for more information */ 
        inline QColor getSystemColor() const
        {
            return this->systemColor; 
        }
        /*! \brief sets the property systemWidth ( \copybrief systemWidth ) to the specified \a __value. 
            \details Description of the parameter systemWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::systemWidth </BLOCKQUOTE>
            \see systemWidth for more information */ 
        inline void setSystemWidth(double __value)
        {
            if (this->systemWidth != __value) {
                this->systemWidth = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property systemWidth ( \copybrief systemWidth ). 
            \details Description of the parameter systemWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::systemWidth </BLOCKQUOTE>
            \see systemWidth for more information */ 
        inline double getSystemWidth() const
        {
            return this->systemWidth; 
        }
        /*! \brief sets the property xZeroTick ( \copybrief xZeroTick ) to the specified \a __value. 
            \details Description of the parameter xZeroTick is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xZeroTick </BLOCKQUOTE>
            \see xZeroTick for more information */ 
        inline void setXZeroTick(double __value)
        {
            if (this->xZeroTick != __value) {
                this->xZeroTick = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property xZeroTick ( \copybrief xZeroTick ). 
            \details Description of the parameter xZeroTick is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xZeroTick </BLOCKQUOTE>
            \see xZeroTick for more information */ 
        inline double getXZeroTick() const
        {
            return this->xZeroTick; 
        }
        /*! \brief sets the property yZeroTick ( \copybrief yZeroTick ) to the specified \a __value. 
            \details Description of the parameter yZeroTick is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yZeroTick </BLOCKQUOTE>
            \see yZeroTick for more information */ 
        inline void setYZeroTick(double __value)
        {
            if (this->yZeroTick != __value) {
                this->yZeroTick = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property yZeroTick ( \copybrief yZeroTick ). 
            \details Description of the parameter yZeroTick is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yZeroTick </BLOCKQUOTE>
            \see yZeroTick for more information */ 
        inline double getYZeroTick() const
        {
            return this->yZeroTick; 
        }
        /*! \brief sets the property xTickDistance ( \copybrief xTickDistance ) to the specified \a __value. 
            \details Description of the parameter xTickDistance is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xTickDistance </BLOCKQUOTE>
            \see xTickDistance for more information */ 
        inline void setXTickDistance(double __value)
        {
            if (this->xTickDistance != __value) {
                this->xTickDistance = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property xTickDistance ( \copybrief xTickDistance ). 
            \details Description of the parameter xTickDistance is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xTickDistance </BLOCKQUOTE>
            \see xTickDistance for more information */ 
        inline double getXTickDistance() const
        {
            return this->xTickDistance; 
        }
        /*! \brief sets the property yTickDistance ( \copybrief yTickDistance ) to the specified \a __value. 
            \details Description of the parameter yTickDistance is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yTickDistance </BLOCKQUOTE>
            \see yTickDistance for more information */ 
        inline void setYTickDistance(double __value)
        {
            if (this->yTickDistance != __value) {
                this->yTickDistance = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property yTickDistance ( \copybrief yTickDistance ). 
            \details Description of the parameter yTickDistance is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yTickDistance </BLOCKQUOTE>
            \see yTickDistance for more information */ 
        inline double getYTickDistance() const
        {
            return this->yTickDistance; 
        }
        /*! \brief sets the property xAxisLabel ( \copybrief xAxisLabel ) to the specified \a __value. 
            \details Description of the parameter xAxisLabel is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xAxisLabel </BLOCKQUOTE>
            \see xAxisLabel for more information */ 
        inline void setXAxisLabel(const QString & __value)
        {
            if (this->xAxisLabel != __value) {
                this->xAxisLabel = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property xAxisLabel ( \copybrief xAxisLabel ). 
            \details Description of the parameter xAxisLabel is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xAxisLabel </BLOCKQUOTE>
            \see xAxisLabel for more information */ 
        inline QString getXAxisLabel() const
        {
            return this->xAxisLabel; 
        }
        /*! \brief sets the property yAxisLabel ( \copybrief yAxisLabel ) to the specified \a __value. 
            \details Description of the parameter yAxisLabel is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yAxisLabel </BLOCKQUOTE>
            \see yAxisLabel for more information */ 
        inline void setYAxisLabel(const QString & __value)
        {
            if (this->yAxisLabel != __value) {
                this->yAxisLabel = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property yAxisLabel ( \copybrief yAxisLabel ). 
            \details Description of the parameter yAxisLabel is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yAxisLabel </BLOCKQUOTE>
            \see yAxisLabel for more information */ 
        inline QString getYAxisLabel() const
        {
            return this->yAxisLabel; 
        }
        /*! \brief sets the property aspectRatio ( \copybrief aspectRatio ) to the specified \a __value. 
            \details Description of the parameter aspectRatio is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::aspectRatio </BLOCKQUOTE>
            \see aspectRatio for more information */ 
        inline void setAspectRatio(double __value)
        {
            if (this->aspectRatio != __value) {
                this->aspectRatio = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property aspectRatio ( \copybrief aspectRatio ). 
            \details Description of the parameter aspectRatio is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::aspectRatio </BLOCKQUOTE>
            \see aspectRatio for more information */ 
        inline double getAspectRatio() const
        {
            return this->aspectRatio; 
        }
        /*! \brief sets the property maintainAspectRatio ( \copybrief maintainAspectRatio ) to the specified \a __value. 
            \details Description of the parameter maintainAspectRatio is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::maintainAspectRatio </BLOCKQUOTE>
            \see maintainAspectRatio for more information */ 
        inline void setMaintainAspectRatio(bool __value)
        {
            if (this->maintainAspectRatio != __value) {
                this->maintainAspectRatio = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property maintainAspectRatio ( \copybrief maintainAspectRatio ). 
            \details Description of the parameter maintainAspectRatio is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::maintainAspectRatio </BLOCKQUOTE>
            \see maintainAspectRatio for more information */ 
        inline bool doesMaintainAspectRatio() const
        {
            return this->maintainAspectRatio; 
        }
        /*! \brief sets the property xAxisLabelVisible ( \copybrief xAxisLabelVisible ) to the specified \a __value. 
            \details Description of the parameter xAxisLabelVisible is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xAxisLabelVisible </BLOCKQUOTE>
            \see xAxisLabelVisible for more information */ 
        inline void setXAxisLabelVisible(bool __value)
        {
            if (this->xAxisLabelVisible != __value) {
                this->xAxisLabelVisible = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property xAxisLabelVisible ( \copybrief xAxisLabelVisible ). 
            \details Description of the parameter xAxisLabelVisible is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::xAxisLabelVisible </BLOCKQUOTE>
            \see xAxisLabelVisible for more information */ 
        inline bool getXAxisLabelVisible() const
        {
            return this->xAxisLabelVisible; 
        }
        /*! \brief sets the property yAxisLabelVisible ( \copybrief yAxisLabelVisible ) to the specified \a __value. 
            \details Description of the parameter yAxisLabelVisible is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yAxisLabelVisible </BLOCKQUOTE>
            \see yAxisLabelVisible for more information */ 
        inline void setYAxisLabelVisible(bool __value)
        {
            if (this->yAxisLabelVisible != __value) {
                this->yAxisLabelVisible = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property yAxisLabelVisible ( \copybrief yAxisLabelVisible ). 
            \details Description of the parameter yAxisLabelVisible is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::yAxisLabelVisible </BLOCKQUOTE>
            \see yAxisLabelVisible for more information */ 
        inline bool getYAxisLabelVisible() const
        {
            return this->yAxisLabelVisible; 
        }
        /*! \brief sets the property synchronizeX ( \copybrief synchronizeX ) to the specified \a __value. 
            \details Description of the parameter synchronizeX is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::synchronizeX </BLOCKQUOTE>
            \see synchronizeX for more information */ 
        inline void setSynchronizeX(JKQTFastPlotter* & __value)
        {
            if (this->synchronizeX != __value) {
                this->synchronizeX = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property synchronizeX ( \copybrief synchronizeX ). 
            \details Description of the parameter synchronizeX is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::synchronizeX </BLOCKQUOTE>
            \see synchronizeX for more information */ 
        inline JKQTFastPlotter* getSynchronizeX() const
        {
            return this->synchronizeX; 
        }
        /*! \brief sets the property synchronizeY ( \copybrief synchronizeY ) to the specified \a __value. 
            \details Description of the parameter synchronizeY is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::synchronizeY </BLOCKQUOTE>
            \see synchronizeY for more information */ 
        inline void setSynchronizeY(JKQTFastPlotter* & __value)
        {
            if (this->synchronizeY != __value) {
                this->synchronizeY = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property synchronizeY ( \copybrief synchronizeY ). 
            \details Description of the parameter synchronizeY is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::synchronizeY </BLOCKQUOTE>
            \see synchronizeY for more information */ 
        inline JKQTFastPlotter* getSynchronizeY() const
        {
            return this->synchronizeY; 
        }

        /** \brief draw the contents onto any <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>, starting at (0,0), returns the size of the whole plot in \a size, if supplied with the default background color */
        void draw(QPainter* painter, QSize* size=nullptr);

        /** \brief draw the contents onto any <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>, starting at (0,0), returns the size of the whole plot in \a size, if supplied with the supplied\a background color */
        void draw(QPainter* painter, QColor background, QSize* size);

    signals:
        /** \brief emitted whenever the graph sizes (borders, plotWidth, plotHeight) change*/
        void plotterSizesChanged();
        /** \brief emitted whenever the graph is replotted */
        void replotting();
        /** \brief emitted whenever the mouse is clicked inside the plot */
        void clicked(double x, double y);
        /** \brief emitted whenever the mouse is clicked inside the plot */
        void clicked(double x, double y, Qt::KeyboardModifiers modifiers);
        /** \brief emitted whenever the mouse is double-clicked inside the plot */
        void doubleClicked(double x, double y);
        /** \brief emitted whenever the mouse is double-clicked inside the plot */
        void doubleClicked(double x, double y, Qt::KeyboardModifiers modifiers);
        /** \brief emitted whenever the mouse is clicked inside the plot */
        void mouseMoved(double x, double y);
        /** \brief emitted when the mouse has been dragged with the left button clicked */
        void mouseDragged(double x_start, double y_start, double x_end, double y_end, Qt::KeyboardModifiers modifiers);
        /** \brief emitted after the mouse has been dragged with the left button clicked */
        void mouseDragFinished(double x_start, double y_start, double x_end, double y_end, Qt::KeyboardModifiers modifiers);
    public slots:
        /** \brief copy the current plot image to the clipboard */
        void copyImage();
        /** \brief replot everything (slowest possible plotting) */
        void redrawPlot();
        /** \brief replot everything (slowest possible plotting) and forces a repaint calling QWidget::repaint() */
        void redrawPlotImmediate();
        /** \brief replot only the data
         *
         *  This internally calls QWidget::update(), so no immediate repaint() is forced! If you want an immediate update,
         * call updateDataImmediate() instead!
         */
        void updateData();
        /** \brief replot only the data, forces a redraw by calling QWidget::repaint()  */
        void updateDataImmediate();

        /** \brief set xMin*/
        void setXMin(double value){
            setPlotUpdateEnabled(false);
            setXRange(value,xMax,xAxisLog);
            setPlotUpdateEnabled(true);
        }
        /** \brief set xMax*/
        void setXMax(double value){
            setPlotUpdateEnabled(false);
            setXRange(xMin,value,xAxisLog);
            setPlotUpdateEnabled(true);
        }
        /** \brief set yMin*/
        void setYMin(double value){
            setPlotUpdateEnabled(false);
            setYRange(value,yMax,yAxisLog);
            setPlotUpdateEnabled(true);
        }
        /** \brief set yMax*/
        void setYMax(double value){
            setPlotUpdateEnabled(false);
            setYRange(yMin,value,yAxisLog);
            setPlotUpdateEnabled(true);
        }


};





/*! \brief base class for all plots that may be plotted by JKQTFastPlotter
    \ingroup jkqtfastplotter
*/
class JKQTP_LIB_EXPORT JKQTFPPlot: public QObject {
        Q_OBJECT
    protected:
        /** \brief parent class, i.e. the plotter to plot on */
        JKQTFastPlotter* parent;

        /** \brief indicates whether the plot is visible or not */
        bool visible;
    public:
        /** \brief class constructor */
        JKQTFPPlot(JKQTFastPlotter* parent);

        /** \brief virtual class destructor */
        virtual ~JKQTFPPlot();

        /** \brief set the parent of the plot */
        void setParent(JKQTFastPlotter* parent) { this->parent=parent; QObject::setParent(parent); }

        /** \brief returns whether this graph is visible inside the plot */
        bool isVisible() const { return this->visible; }

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter)=0;

        /** \brief draw the graph */
        void paint(QPainter& painter);

        /** \brief start a replot of the parent widget */
        void replot();
    public slots:
        void setVisible(bool visible) { this->visible=visible; replot(); }

};



/*! \brief a simple line plot for JKQTFastPlotter
    \ingroup jkqtfastplotter

    This class plots data as (x,y) points connected by straight lines. If errors for the y values are
    provided, also y+/-yerr errorlines are drawn.
*/
class JKQTP_LIB_EXPORT JKQTFPLinePlot: public JKQTFPPlot {
    public:
        /** \brief used to store which datatype is used for the plot data */
        enum DataType {
            JKQTFPLPPointer,
            JKQTFPLPVector
        };
    protected:
        /** \brief number of datapoints in the plot */
        int N;
        /** \brief pointer to x-coordinate data to display */
        double* x;
        /** \brief pointer to y-coordinate data to display */
        double* y;
        /** \brief pointer to yerr-coordinate data to display */
        double* yerr;
        /** \brief pointer to x-coordinate data to display */
        QVector<double>* xv;
        /** \brief pointer to y-coordinate data to display */
        QVector<double>* yv;
        /** \brief pointer to yerr-coordinate data to display */
        QVector<double>* yerrv;
        /** \brief which type of data is used for plotting ? */
        DataType datatype;
        /** \brief color of the graph */
        QColor color;
        /** \brief style of the graph */
        Qt::PenStyle style;
        /** \brief width of the graph (in pt) */
        double width;
        /** \brief color of the graph */
        QColor errorColor;
        /** \brief style of the graph */
        Qt::PenStyle errorStyle;
        /** \brief width of the graph (in pt) */
        double errorWidth;
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPLinePlot(JKQTFastPlotter* parent, int N, double* x, double* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;
        /*! \brief class constructor

            \param parent parent widget
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPLinePlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        inline void setData(double* x, double* y, int N) {
            this->x=x;
            this->y=y;
            this->yerr=nullptr;
            this->N=N;
            datatype=JKQTFPLPPointer;
            replot();
        }

        inline void setData(double* x, double* y, double* yerr, int N) {
            this->x=x;
            this->y=y;
            this->yerr=yerr;
            this->N=N;
            datatype=JKQTFPLPPointer;
            replot();
        }

        inline void setData(QVector<double>* x, QVector<double>* y) {
            this->x=nullptr;
            this->y=nullptr;
            this->yerr=nullptr;
            this->xv=x;
            this->yv=y;
            this->yerrv=nullptr;
            this->N=x->size();
            datatype=JKQTFPLPVector;
            replot();
        }

        inline void setData(QVector<double>* x, QVector<double>* y, QVector<double>* yerr) {
            this->x=nullptr;
            this->y=nullptr;
            this->yerr=nullptr;
            this->xv=x;
            this->yv=y;
            this->yerrv=yerr;
            this->N=x->size();
            datatype=JKQTFPLPVector;
            replot();
        }

        inline int getN() {
            if (datatype==JKQTFPLPPointer)
                return N;
            if (datatype==JKQTFPLPVector)
                return xv->size();
            return N;
        }

        /*! \brief returns the property x ( \copybrief x ). 
            \details Description of the parameter x is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::x </BLOCKQUOTE>. 
            \see x for more information */ 
        inline double* getX() const { 
            return this->x; 
        }
        /*! \brief returns the property y ( \copybrief y ). 
            \details Description of the parameter y is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::y </BLOCKQUOTE>. 
            \see y for more information */ 
        inline double* getY() const { 
            return this->y; 
        }
        /*! \brief returns the property yerr ( \copybrief yerr ). 
            \details Description of the parameter yerr is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::yerr </BLOCKQUOTE>. 
            \see yerr for more information */ 
        inline double* getYerr() const { 
            return this->yerr; 
        }
        /*! \brief returns the property xv ( \copybrief xv ). 
            \details Description of the parameter xv is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::xv </BLOCKQUOTE>. 
            \see xv for more information */ 
        inline QVector<double>* getXv() const { 
            return this->xv; 
        }
        /*! \brief returns the property yv ( \copybrief yv ). 
            \details Description of the parameter yv is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::yv </BLOCKQUOTE>. 
            \see yv for more information */ 
        inline QVector<double>* getYv() const { 
            return this->yv; 
        }
        /*! \brief returns the property yerrv ( \copybrief yerrv ). 
            \details Description of the parameter yerrv is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::yerrv </BLOCKQUOTE>. 
            \see yerrv for more information */ 
        inline QVector<double>* getYerrv() const { 
            return this->yerrv; 
        }
        /*! \brief returns the property datatype ( \copybrief datatype ). 
            \details Description of the parameter datatype is:  <BLOCKQUOTE>\copydoc JKQTFastPlotter::datatype </BLOCKQUOTE>. 
            \see datatype for more information */ 
        inline DataType getDatatype() const { 
            return this->datatype; 
        }
        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::color </BLOCKQUOTE>
            \see color for more information */ 
        inline void setColor(const QColor & __value)
        {
            if (this->color != __value) {
                this->color = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::color </BLOCKQUOTE>
            \see color for more information */ 
        inline QColor getColor() const
        {
            return this->color; 
        }
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::style </BLOCKQUOTE>
            \see style for more information */ 
        inline void setStyle(Qt::PenStyle  __value)
        {
            if (this->style != __value) {
                this->style = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::style </BLOCKQUOTE>
            \see style for more information */ 
        inline Qt::PenStyle getStyle() const
        {
            return this->style; 
        }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::width </BLOCKQUOTE>
            \see width for more information */ 
        inline void setWidth(double __value)
        {
            if (this->width != __value) {
                this->width = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::width </BLOCKQUOTE>
            \see width for more information */ 
        inline double getWidth() const
        {
            return this->width; 
        }
        /*! \brief sets the property errorColor ( \copybrief errorColor ) to the specified \a __value. 
            \details Description of the parameter errorColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::errorColor </BLOCKQUOTE>
            \see errorColor for more information */ 
        inline void setErrorLineColor(const QColor & __value)
        {
            if (this->errorColor != __value) {
                this->errorColor = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property errorColor ( \copybrief errorColor ). 
            \details Description of the parameter errorColor is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::errorColor </BLOCKQUOTE>
            \see errorColor for more information */ 
        inline QColor getErrorLineColor() const
        {
            return this->errorColor; 
        }
        /*! \brief sets the property errorStyle ( \copybrief errorStyle ) to the specified \a __value. 
            \details Description of the parameter errorStyle is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::errorStyle </BLOCKQUOTE>
            \see errorStyle for more information */ 
        inline void setErrorLineStyle(Qt::PenStyle  __value)
        {
            if (this->errorStyle != __value) {
                this->errorStyle = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property errorStyle ( \copybrief errorStyle ). 
            \details Description of the parameter errorStyle is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::errorStyle </BLOCKQUOTE>
            \see errorStyle for more information */ 
        inline Qt::PenStyle getErrorLineStyle() const
        {
            return this->errorStyle; 
        }
        /*! \brief sets the property errorWidth ( \copybrief errorWidth ) to the specified \a __value. 
            \details Description of the parameter errorWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::errorWidth </BLOCKQUOTE>
            \see errorWidth for more information */ 
        inline void setErrorLineWidth(double __value)
        {
            if (this->errorWidth != __value) {
                this->errorWidth = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property errorWidth ( \copybrief errorWidth ). 
            \details Description of the parameter errorWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::errorWidth </BLOCKQUOTE>
            \see errorWidth for more information */ 
        inline double getErrorLineWidth() const
        {
            return this->errorWidth; 
        }
};




/*! \brief a simple plot that draws a cross for every datapoint
    \ingroup jkqtfastplotter

    This class does not support y errors!
*/
class JKQTP_LIB_EXPORT JKQTFPVCrossPlot: public JKQTFPLinePlot {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPVCrossPlot(JKQTFastPlotter* parent, int N, double* x, double* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /*! \brief class constructor

            \param parent parent widget
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPVCrossPlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        /*! \brief sets the property crossWidth ( \copybrief crossWidth ) to the specified \a __value. 
            \details Description of the parameter crossWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::crossWidth </BLOCKQUOTE>
            \see crossWidth for more information */ 
        inline void setCrossWidth(double __value)
        {
            if (this->crossWidth != __value) {
                this->crossWidth = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property crossWidth ( \copybrief crossWidth ). 
            \details Description of the parameter crossWidth is: <BLOCKQUOTE>\copydoc JKQTFastPlotter::crossWidth </BLOCKQUOTE>
            \see crossWidth for more information */ 
        inline double getCrossWidth() const
        {
            return this->crossWidth; 
        }
    protected:
        /** \brief width of the crosses */
        double crossWidth;

};



/*! \brief a simple vertical bar plot for JKQTFastPlotter
    \ingroup jkqtfastplotter

    This class does not support y errors!
*/
class JKQTP_LIB_EXPORT JKQTFPVBarPlot: public JKQTFPLinePlot {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPVBarPlot(JKQTFastPlotter* parent, int N, double* x, double* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /*! \brief class constructor

            \param parent parent widget
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPVBarPlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;
};


/*! \brief plot a range of x values
    \ingroup jkqtfastplotter

*/
class JKQTP_LIB_EXPORT JKQTFPXRangePlot: public JKQTFPPlot {
        Q_OBJECT
    protected:
        /** \brief start of x range */
        double xmin;
        /** \brief end of x range */
        double xmax;
        double centerline;
        bool showCenterline;
        /** \brief color of the graph */
        QColor color;
        /** \brief style of the graph */
        Qt::PenStyle style;
        /** \brief width of the graph (in pt) */
        double width;
        /** \brief fill color of the graph */
        QColor fillColor;
        /** \brief fill style of the graph */
        Qt::BrushStyle fillStyle;
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPXRangePlot(JKQTFastPlotter* parent, double xmin, double xmax, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1, Qt::BrushStyle fillStyle=Qt::NoBrush) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline void setColor(const QColor & __value)
        {
            if (this->color != __value) {
                this->color = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline QColor getColor() const
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor ( \copybrief fillColor ) to the specified \a __value. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::fillColor </BLOCKQUOTE>
            \see fillColor for more information */ 
        inline void setFillColor(const QColor & __value)
        {
            if (this->fillColor != __value) {
                this->fillColor = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property fillColor ( \copybrief fillColor ). 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::fillColor </BLOCKQUOTE>
            \see fillColor for more information */ 
        inline QColor getFillColor() const
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle ( \copybrief fillStyle ) to the specified \a __value. 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::fillStyle </BLOCKQUOTE>
            \see fillStyle for more information */ 
        inline void setFillStyle(Qt::BrushStyle  __value)
        {
            if (this->fillStyle != __value) {
                this->fillStyle = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property fillStyle ( \copybrief fillStyle ). 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::fillStyle </BLOCKQUOTE>
            \see fillStyle for more information */ 
        inline Qt::BrushStyle getFillStyle() const
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::style </BLOCKQUOTE>
            \see style for more information */ 
        inline void setStyle(Qt::PenStyle  __value)
        {
            if (this->style != __value) {
                this->style = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::style </BLOCKQUOTE>
            \see style for more information */ 
        inline Qt::PenStyle getStyle() const
        {
            return this->style; 
        }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline void setWidth(double __value)
        {
            if (this->width != __value) {
                this->width = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline double getWidth() const
        {
            return this->width; 
        }
        /*! \brief returns the property xmin ( \copybrief xmin ). 
            \details Description of the parameter xmin is:  <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::xmin </BLOCKQUOTE>. 
            \see xmin for more information */ 
        inline double getXmin() const { 
            return this->xmin; 
        }
        /*! \brief returns the property xmax ( \copybrief xmax ). 
            \details Description of the parameter xmax is:  <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::xmax </BLOCKQUOTE>. 
            \see xmax for more information */ 
        inline double getXmax() const { 
            return this->xmax; 
        }
        /*! \brief returns the property centerline ( \copybrief centerline ). 
            \details Description of the parameter centerline is:  <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::centerline </BLOCKQUOTE>. 
            \see centerline for more information */ 
        inline double getCenterline() const { 
            return this->centerline; 
        }
        /*! \brief sets the property showCenterline ( \copybrief showCenterline ) to the specified \a __value. 
            \details Description of the parameter showCenterline is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::showCenterline </BLOCKQUOTE>
            \see showCenterline for more information */ 
        inline void setShowCenterline(bool __value)
        {
            if (this->showCenterline != __value) {
                this->showCenterline = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property showCenterline ( \copybrief showCenterline ). 
            \details Description of the parameter showCenterline is: <BLOCKQUOTE>\copydoc JKQTFPXRangePlot::showCenterline </BLOCKQUOTE>
            \see showCenterline for more information */ 
        inline bool getShowCenterline() const
        {
            return this->showCenterline; 
        }
    public slots:
        void setCenterline(int centerline) {
            if (this->centerline!=centerline) {
                this->centerline=centerline;
                replot();
            }
        }
        void setXmin(double xmin) {
            if (this->xmin!=xmin) {
                this->xmin=xmin;
                replot();
            }
        }
        void setXmax(double xmax) {
            if (this->xmax!=xmax) {
                this->xmax=xmax;
                replot();
            }
        }
        void setXmin(int xmin) {
            if (this->xmin!=xmin) {
                this->xmin=xmin;
                replot();
            }
        }
        void setXmax(int xmax) {
            if (this->xmax!=xmax) {
                this->xmax=xmax;
                replot();
            }
        }
};



/*! \brief plot a range of x values
    \ingroup jkqtfastplotter

*/
class JKQTP_LIB_EXPORT JKQTFPYRangePlot: public JKQTFPPlot {
        Q_OBJECT
    protected:
        /** \brief start of x range */
        double ymin;
        /** \brief end of x range */
        double ymax;
        double centerline;
        bool showCenterline;
        /** \brief color of the graph */
        QColor color;
        /** \brief style of the graph */
        Qt::PenStyle style;
        /** \brief width of the graph (in pt) */
        double width;
        /** \brief fill color of the graph */
        QColor fillColor;
        /** \brief fill style of the graph */
        Qt::BrushStyle fillStyle;
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pt)
         */
        JKQTFPYRangePlot(JKQTFastPlotter* parent, double ymin, double ymax, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1, Qt::BrushStyle fillStyle=Qt::NoBrush) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline void setColor(const QColor & __value)
        {
            if (this->color != __value) {
                this->color = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline QColor getColor() const
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor ( \copybrief fillColor ) to the specified \a __value. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::fillColor </BLOCKQUOTE>
            \see fillColor for more information */ 
        inline void setFillColor(const QColor & __value)
        {
            if (this->fillColor != __value) {
                this->fillColor = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property fillColor ( \copybrief fillColor ). 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::fillColor </BLOCKQUOTE>
            \see fillColor for more information */ 
        inline QColor getFillColor() const
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle ( \copybrief fillStyle ) to the specified \a __value. 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::fillStyle </BLOCKQUOTE>
            \see fillStyle for more information */ 
        inline void setFillStyle(Qt::BrushStyle  __value)
        {
            if (this->fillStyle != __value) {
                this->fillStyle = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property fillStyle ( \copybrief fillStyle ). 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::fillStyle </BLOCKQUOTE>
            \see fillStyle for more information */ 
        inline Qt::BrushStyle getFillStyle() const
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::style </BLOCKQUOTE>
            \see style for more information */ 
        inline void setStyle(Qt::PenStyle  __value)
        {
            if (this->style != __value) {
                this->style = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::style </BLOCKQUOTE>
            \see style for more information */ 
        inline Qt::PenStyle getStyle() const
        {
            return this->style; 
        }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline void setWidth(double __value)
        {
            if (this->width != __value) {
                this->width = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline double getWidth() const
        {
            return this->width; 
        }
        /*! \brief returns the property ymin ( \copybrief ymin ). 
            \details Description of the parameter ymin is:  <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::ymin </BLOCKQUOTE>. 
            \see ymin for more information */ 
        inline double getYmin() const { 
            return this->ymin; 
        }
        /*! \brief returns the property ymax ( \copybrief ymax ). 
            \details Description of the parameter ymax is:  <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::ymax </BLOCKQUOTE>. 
            \see ymax for more information */ 
        inline double getYmax() const { 
            return this->ymax; 
        }
        /*! \brief returns the property centerline ( \copybrief centerline ). 
            \details Description of the parameter centerline is:  <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::centerline </BLOCKQUOTE>. 
            \see centerline for more information */ 
        inline double getCenterline() const { 
            return this->centerline; 
        }
        /*! \brief sets the property showCenterline ( \copybrief showCenterline ) to the specified \a __value. 
            \details Description of the parameter showCenterline is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::showCenterline </BLOCKQUOTE>
            \see showCenterline for more information */ 
        inline void setShowCenterline(bool __value)
        {
            if (this->showCenterline != __value) {
                this->showCenterline = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property showCenterline ( \copybrief showCenterline ). 
            \details Description of the parameter showCenterline is: <BLOCKQUOTE>\copydoc JKQTFPYRangePlot::showCenterline </BLOCKQUOTE>
            \see showCenterline for more information */ 
        inline bool getShowCenterline() const
        {
            return this->showCenterline; 
        }
    public slots:
        void setCenterline(int centerline) {
            if (this->centerline!=centerline) {
                this->centerline=centerline;
                replot();
            }
        }
        void setYmin(double xmin) {
            if (this->ymin!=xmin) {
                this->ymin=xmin;
                replot();
            }
        }
        void setYmax(double xmax) {
            if (this->ymax!=xmax) {
                this->ymax=xmax;
                replot();
            }
        }
        void setYmin(int xmin) {
            if (this->ymin!=xmin) {
                this->ymin=xmin;
                replot();
            }
        }
        void setYmax(int xmax) {
            if (this->ymax!=xmax) {
                this->ymax=xmax;
                replot();
            }
        }
};



/*! \brief a plot of a QImage
    \ingroup jkqtfastplotter
*/
class JKQTP_LIB_EXPORT JKQTFPQImagePlot: public JKQTFPPlot {
        Q_OBJECT
    protected:
        /** \brief image to plot */
        QImage* image;
        /** \brief minimum x value of the image */
        double xmin;
        /** \brief maximum x value of the image */
        double xmax;
        /** \brief minimum x value of the image */
        double ymin;
        /** \brief maximum x value of the image */
        double ymax;
    public:
        /*! \brief class constructor

         */
        JKQTFPQImagePlot(JKQTFastPlotter* parent, QImage* image, double xmin, double xmax, double ymin, double ymax) ;

        JKQTFPQImagePlot(JKQTFastPlotter* parent, QImage* image);

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        /*! \brief sets the property image ( \copybrief image ) to the specified \a __value. 
            \details Description of the parameter image is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::image </BLOCKQUOTE>
            \see image for more information */ 
        inline void setImage(QImage* __value)
        {
            if (this->image != __value) {
                this->image = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property image ( \copybrief image ). 
            \details Description of the parameter image is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::image </BLOCKQUOTE>
            \see image for more information */ 
        inline QImage* getImage() const
        {
            return this->image; 
        }
        /*! \brief sets the property xmin ( \copybrief xmin ) to the specified \a __value. 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline void setXmin(double __value)
        {
            if (this->xmin != __value) {
                this->xmin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmin ( \copybrief xmin ). 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline double getXmin() const
        {
            return this->xmin; 
        }
        /*! \brief sets the property xmax ( \copybrief xmax ) to the specified \a __value. 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline void setXmax(double __value)
        {
            if (this->xmax != __value) {
                this->xmax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmax ( \copybrief xmax ). 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline double getXmax() const
        {
            return this->xmax; 
        }
        /*! \brief sets the property ymin ( \copybrief ymin ) to the specified \a __value. 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline void setYmin(double __value)
        {
            if (this->ymin != __value) {
                this->ymin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymin ( \copybrief ymin ). 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline double getYmin() const
        {
            return this->ymin; 
        }
        /*! \brief sets the property ymax ( \copybrief ymax ) to the specified \a __value. 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline void setYmax(double __value)
        {
            if (this->ymax != __value) {
                this->ymax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymax ( \copybrief ymax ). 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPQImagePlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline double getYmax() const
        {
            return this->ymax; 
        }
};



/*!
   \brief An enum for selecting the palette for coloring
    \ingroup jkqtfastplotter
   \details Here, the color palettes are illustrated (left is the color for the minimum and right for the maximum).
 */
enum JKQTFPColorPalette {
    JKQTFP_RED=0, /*!< \image html palettes/palette_red.png */
    JKQTFP_INVERTEDRED=1, /*!< \image html palettes/palette_invred.png */
    JKQTFP_GREEN=2, /*!< \image html palettes/palette_green.png */
    JKQTFP_INVERTEDGREEN=3, /*!< \image html palettes/palette_invgreen.png */
    JKQTFP_BLUE=4, /*!< \image html palettes/palette_blue.png */
    JKQTFP_INVERTEDBLUE=5, /*!< \image html palettes/palette_invblue.png */
    JKQTFP_GRAY=6, /*!< \image html palettes/palette_gray.png */
    JKQTFP_INVERTEDGRAY=7, /*!< \image html palettes/palette_invgray.png */
    JKQTFP_MATLAB=8, /*!< \image html palettes/palette_Matlab.png */
    JKQTFP_RYGB=9, /*!< \image html palettes/palette_RYGB.png */
    JKQTFP_HSV=10, /*!< \image html palettes/palette_HSV.png */
    JKQTFP_INVERTED_HSV=11, /*!< \image html palettes/palette_invHSV.png */
    JKQTFP_RAINBOW=12, /*!< \image html palettes/palette_rainbow.png */
    JKQTFP_HOT=13, /*!< \image html palettes/palette_AFMhot.png */
    JKQTFP_OCEAN=14, /*!< \image html palettes/palette_ocean.png */
    JKQTFP_TRAFFICLIGHT=15, /*!< \image html palettes/palette_trafficlight.png */
    JKQTFP_BLUEMAGENTAYELLOW=16, /*!< \image html palettes/palette_BlMaYe.png */
    JKQTFP_BLUEYELLOW=17, /*!< \image html palettes/palette_BlYe.png */
    JKQTFP_CYAN=18 /*!< \image html palettes/palette_cyan.png */
};

/*!datatype for an image plotpalette for coloring
    \ingroup jkqtfastplotter
   \details Here, the color palettes are illustrated (left is the color for the minimum and right for the maximum).
 */
enum JKQTFPImageFormat {
    JKQTFP_uint8=0,  /*!< \image 8 bit int */
    JKQTFP_uint16=1, /*!< \image 16 bit int */
    JKQTFP_uint32=2, /*!< \image 32 bit int */
    JKQTFP_float=3,  /*!< \image float */
    JKQTFP_double=4, /*!< \image double */
    JKQTFP_int64=5   /*!< \image 64 bit signed int */
};



/*! \brief convert a 2D image (as 1D array) into a QImage with given palette (see JKQTFPColorPalette)
    \ingroup jkqtfastplotter

    This method uses lookup tables which are saved as static variables to convert the 2D array into
    an image. The luts are only created once, and stored then, so mor CPU time is saved. The precompiler define
    JKQTFastPlotter::LUTSIZE sets the size of the LUTs. Note that if you don't use a specific color palette,
    the according LUT won't be calculated and stored!
*/
template <class T>
inline void JKQTFPimagePlot_array2image(T* dbl, int width, int height, QImage &img, JKQTFPColorPalette palette, double minColor, double maxColor)
{
    if (!dbl)
        return;

    #ifdef DEBUG_TIMING
    JKQTPHighResTimer timer;
    double time_sum=0;
    double time_gt=0;
    std::cout<<"  timing JKQTFPimagePlot_array2image:\n";
    timer.start();
    #endif
    //std::cout<<"color range: "<<minColor<<"..."<<maxColor<<std::endl;
    double min = *dbl;
    double max = *dbl;
    if (minColor == maxColor) {
        for (int i=1; i<width*height; ++i)
        {
            T v=dbl[i];
            if (v < min)
                min = v;
            else if (v > max)
                max = v;
        }
    } else {
        min = minColor;
        max = maxColor;
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.getTime();
    time_sum+=time_gt;
    std::cout<<"      finding min&max: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif
    //std::cout<<"minColor="<<minColor<<"   maxColor="<<maxColor<<"       min="<<min<<"  max="<<max<<"\n";

    double delta=max-min;

    QRgb* lut_used=nullptr;
    static QRgb* lut_red=nullptr;
    static QRgb* lut_green=nullptr;
    static QRgb* lut_blue=nullptr;
    static QRgb* lut_gray=nullptr;
    static QRgb* lut_invred=nullptr;
    static QRgb* lut_invgreen=nullptr;
    static QRgb* lut_invblue=nullptr;
    static QRgb* lut_invgray=nullptr;
    static QRgb* lut_matlab=nullptr;
    static QRgb* lut_rygb=nullptr;
    static QRgb* lut_hsv=nullptr;
    static QRgb* lut_invhsv=nullptr;
    static QRgb* lut_rainbow=nullptr;
    static QRgb* lut_hot=nullptr;
    static QRgb* lut_ocean=nullptr;
    static QRgb* lut_trafficlight=nullptr;
    static QRgb* lut_bluemagentayellow=nullptr;
    static QRgb* lut_blueyellow=nullptr;
    static QRgb* lut_cyan=nullptr;


    img = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    if (min == max)
        img.fill(0);
    else
    {
        if (palette == JKQTFP_RED)
        {
            QRgb** plut=&lut_red;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        (*plut)[l]=qRgb(static_cast<int>(255.0*v), 0, 0);
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_GREEN)
        {
            QRgb** plut=&lut_green;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        (*plut)[l]=qRgb(0, static_cast<int>(255.0*v), 0);
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_BLUE)
        {
            QRgb** plut=&lut_blue;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        (*plut)[l]=qRgb(0, 0, static_cast<int>(255.0*v));
                    }
                }
            }
            lut_used=(*plut);

        }


        else if (palette == JKQTFP_GRAY)
        {
            QRgb** plut=&lut_gray;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        (*plut)[l]=qRgb(static_cast<int>(255.0*v),
                                            static_cast<int>(255.0*v),
                                            static_cast<int>(255.0*v));
                    }
                }
            }
            lut_used=(*plut);
        }

        else if (palette == JKQTFP_INVERTEDRED)
        {
            QRgb** plut=&lut_invred;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        (*plut)[l]=qRgb(static_cast<int>(255.0*(1.0-v)), 0, 0);
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_INVERTEDGREEN)
        {
            QRgb** plut=&lut_invgreen;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        (*plut)[l]=qRgb(0, static_cast<int>(255.0*(1.0-v)), 0);
                    }
                }
            }
            lut_used=(*plut);
        }
        else if (palette == JKQTFP_INVERTEDBLUE)
        {
            QRgb** plut=&lut_invblue;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        (*plut)[l]=qRgb(0, 0, static_cast<int>(255.0*(1.0-v)));
                    }
                }
            }
            lut_used=(*plut);
        }
        else if (palette == JKQTFP_INVERTEDGRAY)
        {
            QRgb** plut=&lut_invgray;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=1.0-(l/static_cast<double>(JKQTFastPlotter::LUTSIZE));
                        (*plut)[l]=qRgb(static_cast<int>(255.0*v),
                                            static_cast<int>(255.0*v),
                                            static_cast<int>(255.0*v));
                    }
                }
            }
            lut_used=(*plut);
        }

        else if (palette == JKQTFP_MATLAB)
        {
            QRgb** plut=&lut_matlab;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = 382.5 - 1020.0 * std::abs(v - 0.75);
                        if (r > 255.0)
                            r = 255.0;
                        else if (r < 0.0)
                            r = 0.0;

                        double g = 382.5 - 1020.0 * std::abs(v - 0.5);
                        if (g > 255.0)
                            g = 255.0;
                        else if (g < 0.0)
                            g = 0.0;

                        double b = 382.5 - 1020.0 * std::abs(v - 0.25);
                        if (b > 255.0)
                            b = 255.0;
                        else if (b < 0.0)
                            b = 0.0;

                        (*plut)[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_RYGB) //gnuplot: 30,-13,-23
        {
            QRgb** plut=&lut_rygb;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = 796.875*v - 199.21875;
                        if (r > 255.0)
                            r = 255.0;
                        else if (r < 0.0)
                            r = 0.0;

                        double g = 255.0 * std::sin(M_PI*v);

                        double b = 255.0 - 765.0 * v;
                        if (b < 0.0)
                            b = 0.0;
                        (*plut)[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_HSV)
        {
            QRgb** plut=&lut_hsv;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        int h = static_cast<int>(floor(6*v));
                        double f = 6*v-double(h);

                        switch (h)
                        {
                            case 0: (*plut)[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                            case 1: (*plut)[l]=qRgb(static_cast<int>(255.0*(1-f)), 255, 0); break;
                            case 2: (*plut)[l]=qRgb(0, 255, static_cast<int>(255.0*f)); break;
                            case 3: (*plut)[l]=qRgb(0, static_cast<int>(255.0*(1-f)), 255); break;
                            case 4: (*plut)[l]=qRgb(static_cast<int>(255.0*f), 0, 255); break;
                            case 5: (*plut)[l]=qRgb(255, 0, static_cast<int>(255.0*(1-f))); break;
                            case 6: (*plut)[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                        }
                    }
                }
            }
            lut_used=(*plut);
        }
        else if (palette == JKQTFP_INVERTED_HSV)
        {
            QRgb** plut=&lut_invhsv;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        int h = static_cast<int>(floor(6.0-6.0*v));
                        double f = 6.0-6.0*v-double(h);

                        switch (h)
                        {
                            case 0: (*plut)[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                            case 1: (*plut)[l]=qRgb(static_cast<int>(255.0*(1-f)), 255, 0); break;
                            case 2: (*plut)[l]=qRgb(0, 255, static_cast<int>(255.0*f)); break;
                            case 3: (*plut)[l]=qRgb(0, static_cast<int>(255.0*(1-f)), 255); break;
                            case 4: (*plut)[l]=qRgb(static_cast<int>(255.0*f), 0, 255); break;
                            case 5: (*plut)[l]=qRgb(255, 0, static_cast<int>(255.0*(1-f))); break;
                            case 6: (*plut)[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                        }
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_RAINBOW) //gnuplot: 33,13,10
        {
            if (lut_rainbow==nullptr) {
                lut_rainbow=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                //std::cout<<"!!! creating rainbow lut\n";
                if (lut_rainbow!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = 255.0*std::abs(2.0*v-0.5);
                        if (r > 255.0)
                            r = 255.0;

                        double g = 255.0*sin(M_PI*v);

                        double b = 255.0*cos(0.5*M_PI*v);

                        lut_rainbow[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=lut_rainbow;
        }
        else if (palette == JKQTFP_HOT) //gnuplot: 21,22,23
        {
            QRgb** plut=&lut_hot;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                    double r = 765.0*v;
                    if (r > 255.0)
                        r = 255.0;

                    double g = 765.0*v-255.0;
                    if (g > 255.0)
                        g = 255.0;
                    else if (g < 0.0)
                        g = 0.0;

                    double b = 765.0*v-510.0;
                    if (b < 0.0)
                        b = 0.0;
                    (*plut)[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=(*plut);
        }
        else if (palette == JKQTFP_OCEAN) //gnuplot: 23,28,3
        {
            QRgb** plut=&lut_ocean;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = 765.0*v-510.0;
                        if (r < 0.0)
                            r = 0.0;

                        double g = std::abs(382.5*v-127.5);

                        double b = 255.0*v;
                        (*plut)[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=(*plut);
        }
        else if (palette == JKQTFP_BLUEMAGENTAYELLOW) //gnuplot: 30,31,32
        {
            QRgb** plut=&lut_bluemagentayellow;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = (v/0.32-0.78125);
                        if (r < 0.0) r = 0.0;
                        if (r > 1.0) r = 1.0;

                        double g = 2.0*v-0.84;
                        if (g < 0.0) g = 0.0;
                        if (g > 1.0) g = 1.0;

                        double b = 4.0*v;
                        if (b>1 || b<0) b = -2.0*v+1.84;
                        if (b>1 || b<0) b = v/0.08-11.5;
                        if (b>1 || b<0) b=1;

                        if (b < 0.0) b = 0.0;
                        if (b > 1.0) b = 1.0;
                        (*plut)[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
                    }
                }
            }
            lut_used=(*plut);
        }
        else if (palette == JKQTFP_BLUEYELLOW) //gnuplot: 8,9,10
        {
            QRgb** plut=&lut_blueyellow;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = sqrt(sqrt(v));
                        if (r < 0.0) r = 0.0;
                        if (r > 1.0) r = 1.0;

                        double g = sin(M_PI/2.0*v);
                        if (g < 0.0) g = 0.0;
                        if (g > 1.0) g = 1.0;

                        double b = cos(M_PI/2.0*v);
                        if (b < 0.0) b = 0.0;
                        if (b > 1.0) b = 1.0;

                        (*plut)[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_CYAN)
        {
            QRgb** plut=&lut_cyan;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = v*0.5;
                        if (r < 0.0) r = 0.0;
                        if (r > 1.0) r = 1.0;

                        double g = v;
                        if (g < 0.0) g = 0.0;
                        if (g > 1.0) g = 1.0;

                        double b = v;
                        if (b < 0.0) b = 0.0;
                        if (b > 1.0) b = 1.0;
                        (*plut)[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_TRAFFICLIGHT)
        {
            QRgb** plut=&lut_trafficlight;
            if ((*plut)==nullptr) {
                (*plut)=static_cast<QRgb*>(malloc((JKQTFastPlotter::LUTSIZE+2)*sizeof(QRgb)));
                if ((*plut)!=nullptr) {
                    for (int l=0; l<=JKQTFastPlotter::LUTSIZE; l++) {
                        double v=l/static_cast<double>(JKQTFastPlotter::LUTSIZE);
                        double r = (v < 0.5) ? 128.0*sin(M_PI*(2.0*v-0.5))+128.0 : 255.0;
                        if (r > 255.0)
                            r = 255.0;

                        double g = (v < 0.5) ? 512.0*v+128.0 : 512.0-512.0*v;
                        if (g > 255.0)
                            g = 255.0;
                        (*plut)[l]=qRgb(static_cast<int>(r), static_cast<int>(g), 0);
                    }
                }
            }
            lut_used=(*plut);

        }

        #ifdef DEBUG_TIMING
        time_gt=timer.getTime();
        time_sum+=time_gt;
        std::cout<<"      calculated/found LUT: "<<time_gt/1000.0<<" ms\n";
        timer.start();
        #endif
        if (lut_used!=nullptr) {
            // LUT found: collor the image accordingly
            for (int j=0; j<height; ++j) {
                QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                for (int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)/delta*JKQTFastPlotter::LUTSIZE;
                    v = (v < 0) ? 0 : ( (v > JKQTFastPlotter::LUTSIZE) ? JKQTFastPlotter::LUTSIZE : v);
                    line[i]=lut_used[v];
                }
            }
        } else {
            // no LUT found: paint a black image!
            img.fill(0);
        }
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.getTime();
    time_sum+=time_gt;
    std::cout<<"      creating colored image: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif
    #ifdef DEBUG_TIMING
    time_gt=timer.getTime();
    time_sum+=time_gt;
    std::cout<<"    sum: "<<time_sum/1000.0<<" ms\n";
    timer.start();
    #endif
};

/*! \brief get list with all available palettes
    \ingroup jkqtfastplotter
    \see JKQTFPimagePlot_array2image()
 */
JKQTP_LIB_EXPORT QStringList JKQTFPimagePlot_getPalettes();
/*! \brief get QIcon representing the given palette
    \ingroup jkqtfastplotter
    \see JKQTFPimagePlot_array2image()
*/
JKQTP_LIB_EXPORT QIcon JKQTFPimagePlot_getPaletteIcon(int i);
/*! \brief get QIcon representing the given palette
    \ingroup jkqtfastplotter
    \see JKQTFPimagePlot_array2image()
*/
JKQTP_LIB_EXPORT QIcon JKQTFPimagePlot_getPaletteIcon(JKQTFPColorPalette palette);


/*! \brief plots a given grayscale image with a given color palette
    \ingroup jkqtfastplotter

      - This plot may plot any 2D array of data (8-, 16-, 32-bit integer images, float and double).
      - This class also draws a color bar in the right border of the plot
    .

*/
class JKQTP_LIB_EXPORT JKQTFPimagePlot: public JKQTFPPlot {
        Q_OBJECT
    protected:
        /** \brief image to plot */
        void* image;
        /** \brief format of the image */
        JKQTFPImageFormat imageFormat;
        /** \brief width of image in pt */
        int width;
        /** \brief height of image in pt */
        int height;
        /** \brief minimum x value of the image */
        double xmin;
        /** \brief maximum x value of the image */
        double xmax;
        /** \brief minimum x value of the image */
        double ymin;
        /** \brief maximum x value of the image */
        double ymax;
        /** \brief defines the palette to use */
        JKQTFPColorPalette palette;
        /** \brief defines minimum color value */
        double colorMin;
        /** \brief defines maximum color value */
        double colorMax;
        /** \brief draw color bar */
        bool drawColorBar;
        /** \brief color bar width in pt */
        int colorBarWidth;
        /** \brief rotation of the image when plotting in units of 90 degrees (counter clock wise) */
        int rotation;
    public:


        /*! \brief class constructor

         */
        JKQTFPimagePlot(JKQTFastPlotter* parent, void* image, JKQTFPImageFormat imageFormat, int width, int height, double xmin, double xmax, double ymin, double ymax, JKQTFPColorPalette palette=JKQTFP_MATLAB) ;

        JKQTFPimagePlot(JKQTFastPlotter* parent, void* image, JKQTFPImageFormat imageFormat, int width, int height, JKQTFPColorPalette palette=JKQTFP_MATLAB);

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;



        /*! \brief sets the property image ( \copybrief image ) to the specified \a __value. 
            \details Description of the parameter image is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::image </BLOCKQUOTE>
            \see image for more information */ 
        inline void setImage(void* __value)
        {
            if (this->image != __value) {
                this->image = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property image ( \copybrief image ). 
            \details Description of the parameter image is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::image </BLOCKQUOTE>
            \see image for more information */ 
        inline void* getImage() const
        {
            return this->image; 
        }
        /*! \brief sets the property imageFormat ( \copybrief imageFormat ) to the specified \a __value. 
            \details Description of the parameter imageFormat is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::imageFormat </BLOCKQUOTE>
            \see imageFormat for more information */ 
        inline void setImageFormat(const JKQTFPImageFormat & __value)
        {
            if (this->imageFormat != __value) {
                this->imageFormat = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property imageFormat ( \copybrief imageFormat ). 
            \details Description of the parameter imageFormat is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::imageFormat </BLOCKQUOTE>
            \see imageFormat for more information */ 
        inline JKQTFPImageFormat getImageFormat() const
        {
            return this->imageFormat; 
        }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline void setWidth(int __value)
        {
            if (this->width != __value) {
                this->width = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline int getWidth() const
        {
            return this->width; 
        }
        /*! \brief sets the property height ( \copybrief height ) to the specified \a __value. 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::height </BLOCKQUOTE>
            \see height for more information */ 
        inline void setHeight(int __value)
        {
            if (this->height != __value) {
                this->height = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property height ( \copybrief height ). 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::height </BLOCKQUOTE>
            \see height for more information */ 
        inline int getHeight() const
        {
            return this->height; 
        }
        /*! \brief sets the property xmin ( \copybrief xmin ) to the specified \a __value. 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline void setXmin(double __value)
        {
            if (this->xmin != __value) {
                this->xmin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmin ( \copybrief xmin ). 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline double getXmin() const
        {
            return this->xmin; 
        }
        /*! \brief sets the property xmax ( \copybrief xmax ) to the specified \a __value. 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline void setXmax(double __value)
        {
            if (this->xmax != __value) {
                this->xmax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmax ( \copybrief xmax ). 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline double getXmax() const
        {
            return this->xmax; 
        }
        /*! \brief sets the property ymin ( \copybrief ymin ) to the specified \a __value. 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline void setYmin(double __value)
        {
            if (this->ymin != __value) {
                this->ymin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymin ( \copybrief ymin ). 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline double getYmin() const
        {
            return this->ymin; 
        }
        /*! \brief sets the property ymax ( \copybrief ymax ) to the specified \a __value. 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline void setYmax(double __value)
        {
            if (this->ymax != __value) {
                this->ymax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymax ( \copybrief ymax ). 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline double getYmax() const
        {
            return this->ymax; 
        }
        /*! \brief sets the property drawColorBar ( \copybrief drawColorBar ) to the specified \a __value. 
            \details Description of the parameter drawColorBar is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::drawColorBar </BLOCKQUOTE>
            \see drawColorBar for more information */ 
        inline void setDrawColorBar(bool __value)
        {
            if (this->drawColorBar != __value) {
                this->drawColorBar = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property drawColorBar ( \copybrief drawColorBar ). 
            \details Description of the parameter drawColorBar is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::drawColorBar </BLOCKQUOTE>
            \see drawColorBar for more information */ 
        inline bool getDrawColorBar() const
        {
            return this->drawColorBar; 
        }
        /*! \brief sets the property colorBarWidth ( \copybrief colorBarWidth ) to the specified \a __value. 
            \details Description of the parameter colorBarWidth is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::colorBarWidth </BLOCKQUOTE>
            \see colorBarWidth for more information */ 
        inline void setColorBarWidth(int __value)
        {
            if (this->colorBarWidth != __value) {
                this->colorBarWidth = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property colorBarWidth ( \copybrief colorBarWidth ). 
            \details Description of the parameter colorBarWidth is: <BLOCKQUOTE>\copydoc JKQTFPimagePlot::colorBarWidth </BLOCKQUOTE>
            \see colorBarWidth for more information */ 
        inline int getColorBarWidth() const
        {
            return this->colorBarWidth; 
        }
        /*! \brief returns the property rotation ( \copybrief rotation ). 
            \details Description of the parameter rotation is:  <BLOCKQUOTE>\copydoc JKQTFPimagePlot::rotation </BLOCKQUOTE>. 
            \see rotation for more information */ 
        inline int getRotation() const { 
            return this->rotation; 
        }
        /*! \brief returns the property palette ( \copybrief palette ). 
            \details Description of the parameter palette is:  <BLOCKQUOTE>\copydoc JKQTFPimagePlot::palette </BLOCKQUOTE>. 
            \see palette for more information */ 
        inline JKQTFPColorPalette getPalette() const { 
            return this->palette; 
        }
        /*! \brief returns the property colorMin ( \copybrief colorMin ). 
            \details Description of the parameter colorMin is:  <BLOCKQUOTE>\copydoc JKQTFPimagePlot::colorMin </BLOCKQUOTE>. 
            \see colorMin for more information */ 
        inline double getColorMin() const { 
            return this->colorMin; 
        }
        /*! \brief returns the property colorMax ( \copybrief colorMax ). 
            \details Description of the parameter colorMax is:  <BLOCKQUOTE>\copydoc JKQTFPimagePlot::colorMax </BLOCKQUOTE>. 
            \see colorMax for more information */ 
        inline double getColorMax() const { 
            return this->colorMax; 
        }
        void setImage(void* image, JKQTFPImageFormat imageFormat, int width, int height) {
            this->image=image;
            this->imageFormat=imageFormat;
            this->width=width;
            this->height=height;
            replot();
        }
    public slots:
        void setRotation(int rotation) {
            if (this->rotation!=rotation) {
                this->rotation=(JKQTFPColorPalette)rotation;
                replot();
            }
        }
        void setPalette(int palette) {
            if (this->palette!=palette) {
                this->palette=(JKQTFPColorPalette)palette;
                replot();
            }
        };
        void setPalette(JKQTFPColorPalette palette) {
            if (this->palette!=palette) {
                this->palette=palette;
                replot();
            }
        };
        void setColorMin(uint32_t colorMin) {
            if (this->colorMin!=colorMin) {
                this-> colorMin= colorMin;
                replot();
            }
        }
        void setColorMax(uint32_t colorMax) {
            if (this->colorMax!=colorMax) {
                this->colorMax=colorMax;
                replot();
            }
        }
        void setColorMin(int colorMin) {
            if (this->colorMin!=colorMin) {
                this-> colorMin= colorMin;
                replot();
            }
        }
        void setColorMax(int colorMax) {
            if (this->colorMax!=colorMax) {
                this->colorMax=colorMax;
                replot();
            }
        }
        void setColorMin(double colorMin) {
            if (this->colorMin!=colorMin) {
                this-> colorMin= colorMin;
                replot();
            }
        }
        void setColorMax(double colorMax) {
            if (this->colorMax!=colorMax) {
                this->colorMax=colorMax;
                replot();
            }
        }
};







/*! \brief convert a 2D image (as 1D array) into a QImage and puts the image values into one color channel (set by \a channel).
    \ingroup jkqtfastplotter

*/
template <class T>
inline void JKQTFPRGBImageOverlayPlot_array2image(T* dbl, int width, int height, QImage &img, int channel, T minColor, T maxColor)
{
    if (!dbl)
        return;

    #ifdef DEBUG_TIMING
    JKQTPHighResTimer timer;
    double time_sum=0;
    double time_gt=0;
    std::cout<<"  timing JKQTFPimagePlot_array2image:\n";
    timer.start();
    #endif
    //qDebug()<<"channel = "<<channel;
    //qDebug()<<"color range: "<<minColor<<"..."<<maxColor;
    double min = *dbl;
    double max = *dbl;
    if (minColor == maxColor) {
        for (int i=1; i<width*height; ++i)
        {
            T v=dbl[i];
            if (v < min)
                min = v;
            else if (v > max)
                max = v;
        }
    } else {
        min = minColor;
        max = maxColor;
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.getTime();
    time_sum+=time_gt;
    std::cout<<"      finding min&max: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif
    //qDebug()<<"minColor="<<minColor<<"   maxColor="<<maxColor<<"       min="<<min<<"  max="<<max<<"\n";

    double delta=max-min;


    if (min != max) {
        // LUT found: collor the image accordingly
        if (channel==0) {
            for (int j=0; j<height; ++j) {
                QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                for (int i=0; i<width; ++i) {
                    int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    const QRgb l=line[i];
                    line[i]=qRgb(v,qGreen(l),qBlue(l));
                    //if (i==j) qDebug()<<dbl[j*width+i]<<v<<hex<<l<<line[i]<<dec;
                }
            }
        } else if (channel==1) {
            for (int j=0; j<height; ++j) {
                QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                for (int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    const QRgb l=line[i];
                    line[i]=qRgb(qRed(l),v,qBlue(l));
                    //if (i==j) qDebug()<<dbl[j*width+i]<<v<<hex<<l<<line[i]<<dec;
                }
            }
        } else if (channel==2) {
            for (int j=0; j<height; ++j) {
                QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                for (int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    const QRgb l=line[i];
                    line[i]=qRgb(qRed(l),qGreen(l),v);
                    //if (i==j) qDebug()<<dbl[j*width+i]<<v<<hex<<l<<line[i]<<dec;
                }
            }
        }
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.getTime();
    time_sum+=time_gt;
    std::cout<<"      creating colored image: "<<time_gt/1000.0<<" ms\n";
    timer.start();

    time_gt=timer.getTime();
    time_sum+=time_gt;
    std::cout<<"    sum: "<<time_sum/1000.0<<" ms\n";
    timer.start();
    #endif
};





/*! \brief plots 1,2 or 3 given grayscale images as an overlay plot, where each channel is drawn as one color channel (e.g. red, green or blue).
    \ingroup jkqtfastplotter

      - This plot may plot any 2D array of data (8-, 16-, 32-bit integer images, float and double).
      - The images all have to have the same size
    .

*/
class JKQTP_LIB_EXPORT JKQTFPRGBImageOverlayPlot: public JKQTFPPlot {
        Q_OBJECT
    protected:
        /** \brief image to plot */
        void* imageRed;
        /** \brief format of the image */
        JKQTFPImageFormat imageFormatRed;
        /** \brief image to plot */
        void* imageGreen;
        /** \brief format of the image */
        JKQTFPImageFormat imageFormatGreen;
        /** \brief image to plot */
        void* imageBlue;
        /** \brief format of the image */
        JKQTFPImageFormat imageFormatBlue;
        /** \brief width of image in pt */
        int width;
        /** \brief height of image in pt */
        int height;
        /** \brief minimum x value of the image */
        double xmin;
        /** \brief maximum x value of the image */
        double xmax;
        /** \brief minimum x value of the image */
        double ymin;
        /** \brief maximum x value of the image */
        double ymax;
        /** \brief defines minimum color value */
        double colorMinRed;
        /** \brief defines maximum color value */
        double colorMaxRed;
        /** \brief defines minimum color value */
        double colorMinGreen;
        /** \brief defines maximum color value */
        double colorMaxGreen;
        /** \brief defines minimum color value */
        double colorMinBlue;
        /** \brief defines maximum color value */
        double colorMaxBlue;
        /** \brief rotation of the image when plotting in units of 90 degrees (counter clock wise) */
        int rotation;
    public:
        /*! \brief class constructor

         */
        JKQTFPRGBImageOverlayPlot(JKQTFastPlotter* parent, void* imageRed, JKQTFPImageFormat imageFormat, int width, int height, double xmin, double xmax, double ymin, double ymax) ;
        JKQTFPRGBImageOverlayPlot(JKQTFastPlotter* parent, void* imageRed, JKQTFPImageFormat imageFormat, int width, int height);
        JKQTFPRGBImageOverlayPlot(JKQTFastPlotter* parent);

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;



        /*! \brief sets the properties imageRed and imageFormatRed to the specified \a __value and \a __value2. 
            \details Description of the parameter imageRed is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageRed </BLOCKQUOTE> \details Description of the parameter imageFormatRed is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatRed </BLOCKQUOTE>
            \see imageRed and imageFormatRed for more information */ 
        inline void setImageRed (void* __value, JKQTFPImageFormat __value2)
        {
            bool set=false; 
            if (this->imageRed != __value) { 
                this->imageRed = __value; 
                set=true; 
            } 
            if (this->imageFormatRed != __value2) { 
                this->imageFormatRed = __value2; 
                set=true; 
            } 
            if (set) { 
                replot(); 
            } 
        } 
        /*! \brief returns the property imageRed ( \copybrief imageRed ). 
            \details Description of the parameter imageRed is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageRed </BLOCKQUOTE> \details Description of the parameter imageFormatRed is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatRed </BLOCKQUOTE>
            \see imageRed for more information */ 
        inline void* getImageRed () const { return this->imageRed; }
        /*! \brief returns the property imageFormatRed ( \copybrief imageFormatRed ). 
            \details Description of the parameter imageRed is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageRed </BLOCKQUOTE> \details Description of the parameter imageFormatRed is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatRed </BLOCKQUOTE>
            \see imageFormatRed for more information */
        inline JKQTFPImageFormat getImageFormatRed () const { return this->imageFormatRed; }
        /*! \brief sets the properties imageGreen and imageFormatGreen to the specified \a __value and \a __value2. 
            \details Description of the parameter imageGreen is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageGreen </BLOCKQUOTE> \details Description of the parameter imageFormatGreen is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatGreen </BLOCKQUOTE>
            \see imageGreen and imageFormatGreen for more information */ 
        inline void setImageGreen (void* __value, JKQTFPImageFormat __value2)
        {
            bool set=false; 
            if (this->imageGreen != __value) { 
                this->imageGreen = __value; 
                set=true; 
            } 
            if (this->imageFormatGreen != __value2) { 
                this->imageFormatGreen = __value2; 
                set=true; 
            } 
            if (set) { 
                replot(); 
            } 
        } 
        /*! \brief returns the property imageGreen ( \copybrief imageGreen ). 
            \details Description of the parameter imageGreen is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageGreen </BLOCKQUOTE> \details Description of the parameter imageFormatGreen is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatGreen </BLOCKQUOTE>
            \see imageGreen for more information */ 
        inline void* getImageGreen () const { return this->imageGreen; }
        /*! \brief returns the property imageFormatGreen ( \copybrief imageFormatGreen ). 
            \details Description of the parameter imageGreen is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageGreen </BLOCKQUOTE> \details Description of the parameter imageFormatGreen is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatGreen </BLOCKQUOTE>
            \see imageFormatGreen for more information */
        inline JKQTFPImageFormat getImageFormatGreen () const { return this->imageFormatGreen; }
        /*! \brief sets the properties imageBlue and imageFormatBlue to the specified \a __value and \a __value2. 
            \details Description of the parameter imageBlue is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageBlue </BLOCKQUOTE> \details Description of the parameter imageFormatBlue is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatBlue </BLOCKQUOTE>
            \see imageBlue and imageFormatBlue for more information */ 
        inline void setImageBlue (void* __value, JKQTFPImageFormat __value2)
        {
            bool set=false; 
            if (this->imageBlue != __value) { 
                this->imageBlue = __value; 
                set=true; 
            } 
            if (this->imageFormatBlue != __value2) { 
                this->imageFormatBlue = __value2; 
                set=true; 
            } 
            if (set) { 
                replot(); 
            } 
        } 
        /*! \brief returns the property imageBlue ( \copybrief imageBlue ). 
            \details Description of the parameter imageBlue is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageBlue </BLOCKQUOTE> \details Description of the parameter imageFormatBlue is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatBlue </BLOCKQUOTE>
            \see imageBlue for more information */ 
        inline void* getImageBlue () const { return this->imageBlue; }
        /*! \brief returns the property imageFormatBlue ( \copybrief imageFormatBlue ). 
            \details Description of the parameter imageBlue is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageBlue </BLOCKQUOTE> \details Description of the parameter imageFormatBlue is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::imageFormatBlue </BLOCKQUOTE>
            \see imageFormatBlue for more information */
        inline JKQTFPImageFormat getImageFormatBlue () const { return this->imageFormatBlue; }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline void setWidth(int __value)
        {
            if (this->width != __value) {
                this->width = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline int getWidth() const
        {
            return this->width; 
        }
        /*! \brief sets the property height ( \copybrief height ) to the specified \a __value. 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::height </BLOCKQUOTE>
            \see height for more information */ 
        inline void setHeight(int __value)
        {
            if (this->height != __value) {
                this->height = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property height ( \copybrief height ). 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::height </BLOCKQUOTE>
            \see height for more information */ 
        inline int getHeight() const
        {
            return this->height; 
        }
        /*! \brief sets the property xmin ( \copybrief xmin ) to the specified \a __value. 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline void setXmin(double __value)
        {
            if (this->xmin != __value) {
                this->xmin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmin ( \copybrief xmin ). 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline double getXmin() const
        {
            return this->xmin; 
        }
        /*! \brief sets the property xmax ( \copybrief xmax ) to the specified \a __value. 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline void setXmax(double __value)
        {
            if (this->xmax != __value) {
                this->xmax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmax ( \copybrief xmax ). 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline double getXmax() const
        {
            return this->xmax; 
        }
        /*! \brief sets the property ymin ( \copybrief ymin ) to the specified \a __value. 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline void setYmin(double __value)
        {
            if (this->ymin != __value) {
                this->ymin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymin ( \copybrief ymin ). 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline double getYmin() const
        {
            return this->ymin; 
        }
        /*! \brief sets the property ymax ( \copybrief ymax ) to the specified \a __value. 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline void setYmax(double __value)
        {
            if (this->ymax != __value) {
                this->ymax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymax ( \copybrief ymax ). 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline double getYmax() const
        {
            return this->ymax; 
        }
        /*! \brief returns the property rotation ( \copybrief rotation ). 
            \details Description of the parameter rotation is:  <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::rotation </BLOCKQUOTE>. 
            \see rotation for more information */ 
        inline int getRotation() const { 
            return this->rotation; 
        }
        /*! \brief returns the property colorMinRed ( \copybrief colorMinRed ). 
            \details Description of the parameter colorMinRed is:  <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::colorMinRed </BLOCKQUOTE>. 
            \see colorMinRed for more information */ 
        inline double getColorMinRed() const { 
            return this->colorMinRed; 
        }
        /*! \brief returns the property colorMaxRed ( \copybrief colorMaxRed ). 
            \details Description of the parameter colorMaxRed is:  <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::colorMaxRed </BLOCKQUOTE>. 
            \see colorMaxRed for more information */ 
        inline double getColorMaxRed() const { 
            return this->colorMaxRed; 
        }
        /*! \brief returns the property colorMinGreen ( \copybrief colorMinGreen ). 
            \details Description of the parameter colorMinGreen is:  <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::colorMinGreen </BLOCKQUOTE>. 
            \see colorMinGreen for more information */ 
        inline double getColorMinGreen() const { 
            return this->colorMinGreen; 
        }
        /*! \brief returns the property colorMaxGreen ( \copybrief colorMaxGreen ). 
            \details Description of the parameter colorMaxGreen is:  <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::colorMaxGreen </BLOCKQUOTE>. 
            \see colorMaxGreen for more information */ 
        inline double getColorMaxGreen() const { 
            return this->colorMaxGreen; 
        }
        /*! \brief returns the property colorMinBlue ( \copybrief colorMinBlue ). 
            \details Description of the parameter colorMinBlue is:  <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::colorMinBlue </BLOCKQUOTE>. 
            \see colorMinBlue for more information */ 
        inline double getColorMinBlue() const { 
            return this->colorMinBlue; 
        }
        /*! \brief returns the property colorMaxBlue ( \copybrief colorMaxBlue ). 
            \details Description of the parameter colorMaxBlue is:  <BLOCKQUOTE>\copydoc JKQTFPRGBImageOverlayPlot::colorMaxBlue </BLOCKQUOTE>. 
            \see colorMaxBlue for more information */ 
        inline double getColorMaxBlue() const { 
            return this->colorMaxBlue; 
        }
        void setImage(void* imageRed, JKQTFPImageFormat imageFormatRed, int width, int height);
        void setImage(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, int width, int height);
        void setImage(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, void* imageBlue, JKQTFPImageFormat imageFormatBlue, int width, int height);
        void setImage(void* imageRed, JKQTFPImageFormat imageFormatRed, int width, int height, double xmin, double xmax, double ymin, double ymax);
        void setImage(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, int width, int height, double xmin, double xmax, double ymin, double ymax);
        void setImage(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, void* imageBlue, JKQTFPImageFormat imageFormatBlue, int width, int height, double xmin, double xmax, double ymin, double ymax);
    public slots:
        inline void setRotation(int rotation) {
            if (this->rotation!=rotation) {
                this->rotation=(JKQTFPColorPalette)rotation;
                replot();
            }
        }
        inline void setColorMinRed(uint32_t colorMin) {
            if (this->colorMinRed!=colorMin) {
                this-> colorMinRed= colorMin;
                replot();
            }
        }
        inline void setColorMaxRed(uint32_t colorMax) {
            if (this->colorMaxRed!=colorMax) {
                this->colorMaxRed=colorMax;
                replot();
            }
        }
        inline void setColorMinRed(int colorMin) {
            if (this->colorMinRed!=colorMin) {
                this-> colorMinRed= colorMin;
                replot();
            }
        }
        inline void setColorMaxRed(int colorMax) {
            if (this->colorMaxRed!=colorMax) {
                this->colorMaxRed=colorMax;
                replot();
            }
        }
        inline void setColorMinRed(double colorMin) {
            if (this->colorMinRed!=colorMin) {
                this-> colorMinRed= colorMin;
                replot();
            }
        }
        inline void setColorMaxRed(double colorMax) {
            if (this->colorMaxRed!=colorMax) {
                this->colorMaxRed=colorMax;
                replot();
            }
        }


        inline void setColorMinGreen(uint32_t colorMin) {
            if (this->colorMinGreen!=colorMin) {
                this-> colorMinGreen= colorMin;
                replot();
            }
        }
        inline void setColorMaxGreen(uint32_t colorMax) {
            if (this->colorMaxGreen!=colorMax) {
                this->colorMaxGreen=colorMax;
                replot();
            }
        }
        inline void setColorMinGreen(int colorMin) {
            if (this->colorMinGreen!=colorMin) {
                this-> colorMinGreen= colorMin;
                replot();
            }
        }
        inline void setColorMaxGreen(int colorMax) {
            if (this->colorMaxGreen!=colorMax) {
                this->colorMaxGreen=colorMax;
                replot();
            }
        }
        inline void setColorMinGreen(double colorMin) {
            if (this->colorMinGreen!=colorMin) {
                this-> colorMinGreen= colorMin;
                replot();
            }
        }
        inline void setColorMaxGreen(double colorMax) {
            if (this->colorMaxGreen!=colorMax) {
                this->colorMaxGreen=colorMax;
                replot();
            }
        }

        inline void setColorMinBlue(uint32_t colorMin) {
            if (this->colorMinBlue!=colorMin) {
                this-> colorMinBlue= colorMin;
                replot();
            }
        }
        inline void setColorMaxBlue(uint32_t colorMax) {
            if (this->colorMaxBlue!=colorMax) {
                this->colorMaxBlue=colorMax;
                replot();
            }
        }
        inline void setColorMinBlue(int colorMin) {
            if (this->colorMinBlue!=colorMin) {
                this-> colorMinBlue= colorMin;
                replot();
            }
        }
        inline void setColorMaxBlue(int colorMax) {
            if (this->colorMaxBlue!=colorMax) {
                this->colorMaxBlue=colorMax;
                replot();
            }
        }
        inline void setColorMinBlue(double colorMin) {
            if (this->colorMinBlue!=colorMin) {
                this-> colorMinBlue= colorMin;
                replot();
            }
        }
        inline void setColorMaxBlue(double colorMax) {
            if (this->colorMaxBlue!=colorMax) {
                this->colorMaxBlue=colorMax;
                replot();
            }
        }
};



/*! \brief plots an image overlay, i.e. a boolean image where each \c true pixel is drawn with a given color
           and the \c false pixels are transparent
    \ingroup jkqtfastplotter

*/
class JKQTP_LIB_EXPORT JKQTFPimageOverlayPlot: public JKQTFPPlot {
        Q_OBJECT
    public:
        enum SymbolType {
            stCircle,
            stRectangle
        };

    protected:
        /** \brief image to plot */
        bool* image;
        /** \brief width of image in pt */
        int width;
        /** \brief height of image in pt */
        int height;
        /** \brief minimum x value of the image */
        double xmin;
        /** \brief maximum x value of the image */
        double xmax;
        /** \brief minimum x value of the image */
        double ymin;
        /** \brief maximum x value of the image */
        double ymax;
        /** \brief color of the overlay pixels */
        QColor color;
        /** \brief linewidth of symbols */
        double linewidth;
        /** \brief rotation of the image when plotting in units of 90 degrees */
        int rotation;
        SymbolType symboltype;
        bool showAsSymbols;
    public:
        /*! \brief class constructor

         */
        JKQTFPimageOverlayPlot(JKQTFastPlotter* parent, bool* image, int width, int height, double xmin, double xmax, double ymin, double ymax, QColor color=QColor("red"));

        JKQTFPimageOverlayPlot(JKQTFastPlotter* parent, bool* image, int width, int height, QColor color=QColor("red"));

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        void setData(bool* image, int width, int height) {
            this->image=image;
            this->width=width;
            this->height=height;
            replot();
        }

        /*! \brief returns the property image ( \copybrief image ). 
            \details Description of the parameter image is:  <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::image </BLOCKQUOTE>. 
            \see image for more information */ 
        inline bool* getImage() const { 
            return this->image; 
        }
        /*! \brief returns the property rotation ( \copybrief rotation ). 
            \details Description of the parameter rotation is:  <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::rotation </BLOCKQUOTE>. 
            \see rotation for more information */ 
        inline int getRotation() const { 
            return this->rotation; 
        }
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is:  <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::width </BLOCKQUOTE>. 
            \see width for more information */ 
        inline int getWidth() const {
            return this->width; 
        }
        /*! \brief returns the property height ( \copybrief height ). 
            \details Description of the parameter height is:  <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::height </BLOCKQUOTE>. 
            \see height for more information */ 
        inline int getHeight() const {
            return this->height; 
        }
        /*! \brief sets the property xmin ( \copybrief xmin ) to the specified \a __value. 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline void setXmin(double __value)
        {
            if (this->xmin != __value) {
                this->xmin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmin ( \copybrief xmin ). 
            \details Description of the parameter xmin is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::xmin </BLOCKQUOTE>
            \see xmin for more information */ 
        inline double getXmin() const
        {
            return this->xmin; 
        }
        /*! \brief sets the property xmax ( \copybrief xmax ) to the specified \a __value. 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline void setXmax(double __value)
        {
            if (this->xmax != __value) {
                this->xmax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property xmax ( \copybrief xmax ). 
            \details Description of the parameter xmax is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::xmax </BLOCKQUOTE>
            \see xmax for more information */ 
        inline double getXmax() const
        {
            return this->xmax; 
        }
        /*! \brief sets the property ymin ( \copybrief ymin ) to the specified \a __value. 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline void setYmin(double __value)
        {
            if (this->ymin != __value) {
                this->ymin = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymin ( \copybrief ymin ). 
            \details Description of the parameter ymin is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::ymin </BLOCKQUOTE>
            \see ymin for more information */ 
        inline double getYmin() const
        {
            return this->ymin; 
        }
        /*! \brief sets the property ymax ( \copybrief ymax ) to the specified \a __value. 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline void setYmax(double __value)
        {
            if (this->ymax != __value) {
                this->ymax = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property ymax ( \copybrief ymax ). 
            \details Description of the parameter ymax is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::ymax </BLOCKQUOTE>
            \see ymax for more information */ 
        inline double getYmax() const
        {
            return this->ymax; 
        }
        /*! \brief sets the property showAsSymbols ( \copybrief showAsSymbols ) to the specified \a __value. 
            \details Description of the parameter showAsSymbols is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::showAsSymbols </BLOCKQUOTE>
            \see showAsSymbols for more information */ 
        inline void setShowAsSymbols(bool __value)
        {
            if (this->showAsSymbols != __value) {
                this->showAsSymbols = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property showAsSymbols ( \copybrief showAsSymbols ). 
            \details Description of the parameter showAsSymbols is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::showAsSymbols </BLOCKQUOTE>
            \see showAsSymbols for more information */ 
        inline bool getShowAsSymbols() const
        {
            return this->showAsSymbols; 
        }
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is:  <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::color </BLOCKQUOTE>. 
            \see color for more information */ 
        inline QColor getColor() const { 
            return this->color; 
        }
    public slots:
        inline void setRotation(int rotation) {
            if (this->rotation!=rotation) {
                this->rotation=(JKQTFPColorPalette)rotation;
                replot();
            }
        }
        inline void setColor(QColor color) {
            if (this->color!=color) {
                this->color=color;
                replot();
            }
        };
};



/*! \brief plot a horizontal scale bar
    \ingroup jkqtfastplotter

*/
class JKQTP_LIB_EXPORT JKQTFPQScaleBarXPlot: public JKQTFPPlot {
        Q_OBJECT
    public:
        enum Position {
            TopLeft=0,
            TopRight=1,
            BottomLeft=2,
            BottomRight=3
        };
    protected:
        /** \brief width of scale bar */
        double width;
        /** \brief line width of scale bar, given in pt */
        double lineWidth;
        /** \brief color of scale bar */
        QColor color;
        /** \brief scale bar label (use %1 to include value of width) */
        QString label;
        /** \brief font of scale bar */
        QFont font;
        /** \brief scale bar position */
        Position position;

    public:

        /*! \brief class constructor

         */
        JKQTFPQScaleBarXPlot(JKQTFastPlotter* parent, double width=1, const QString& label=QString("%1"), QColor color=QColor("grey"), double lineWidth=2) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline void setWidth(double __value)
        {
            if (this->width != __value) {
                this->width = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline double getWidth() const
        {
            return this->width; 
        }
        /*! \brief sets the property lineWidth ( \copybrief lineWidth ) to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::lineWidth </BLOCKQUOTE>
            \see lineWidth for more information */ 
        inline void setLineWidth(double __value)
        {
            if (this->lineWidth != __value) {
                this->lineWidth = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property lineWidth ( \copybrief lineWidth ). 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::lineWidth </BLOCKQUOTE>
            \see lineWidth for more information */ 
        inline double getLineWidth() const
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline void setColor(const QColor & __value)
        {
            if (this->color != __value) {
                this->color = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline QColor getColor() const
        {
            return this->color; 
        }
        /*! \brief sets the property label ( \copybrief label ) to the specified \a __value. 
            \details Description of the parameter label is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::label </BLOCKQUOTE>
            \see label for more information */ 
        inline void setLabel(const QString & __value)
        {
            if (this->label != __value) {
                this->label = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property label ( \copybrief label ). 
            \details Description of the parameter label is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::label </BLOCKQUOTE>
            \see label for more information */ 
        inline QString getLabel() const
        {
            return this->label; 
        }
        /*! \brief sets the property font ( \copybrief font ) to the specified \a __value. 
            \details Description of the parameter font is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::font </BLOCKQUOTE>
            \see font for more information */ 
        inline void setFont(const QFont & __value)
        {
            if (this->font != __value) {
                this->font = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property font ( \copybrief font ). 
            \details Description of the parameter font is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::font </BLOCKQUOTE>
            \see font for more information */ 
        inline QFont getFont() const
        {
            return this->font; 
        }
        /*! \brief sets the property position ( \copybrief position ) to the specified \a __value. 
            \details Description of the parameter position is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::position </BLOCKQUOTE>
            \see position for more information */ 
        inline void setPosition(const Position & __value)
        {
            if (this->position != __value) {
                this->position = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property position ( \copybrief position ). 
            \details Description of the parameter position is: <BLOCKQUOTE>\copydoc JKQTFPimageOverlayPlot::position </BLOCKQUOTE>
            \see position for more information */ 
        inline Position getPosition() const
        {
            return this->position; 
        }
    public slots:
        void setPosition(int pos) {
            setPosition((Position)pos);
        }
};




/*! \brief plot a horizontal scale bar
    \ingroup jkqtfastplotter

*/
class JKQTP_LIB_EXPORT JKQTFPQOverlayLinearGridPlot: public JKQTFPPlot {
        Q_OBJECT

    protected:
        /** \brief distance between grid lines */
        double width;
        /** \brief line width of grid lines, given in pt */
        double lineWidth;
        /** \brief color of the grid */
        QColor color;
        /** \brief grid line style */
        Qt::PenStyle style;
    public:

        /*! \brief class constructor

         */
        JKQTFPQOverlayLinearGridPlot(JKQTFastPlotter* parent, double width=1, QColor color=QColor("grey"), double lineWidth=1, Qt::PenStyle style=Qt::DashLine) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter) override;

        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline void setWidth(double __value)
        {
            if (this->width != __value) {
                this->width = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::width </BLOCKQUOTE>
            \see width for more information */ 
        inline double getWidth() const
        {
            return this->width; 
        }
        /*! \brief sets the property lineWidth ( \copybrief lineWidth ) to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::lineWidth </BLOCKQUOTE>
            \see lineWidth for more information */ 
        inline void setLineWidth(double __value)
        {
            if (this->lineWidth != __value) {
                this->lineWidth = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property lineWidth ( \copybrief lineWidth ). 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::lineWidth </BLOCKQUOTE>
            \see lineWidth for more information */ 
        inline double getLineWidth() const
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline void setColor(const QColor & __value)
        {
            if (this->color != __value) {
                this->color = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::color </BLOCKQUOTE>
            \see color for more information */ 
        inline QColor getColor() const
        {
            return this->color; 
        }
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::style </BLOCKQUOTE>
            \see style for more information */ 
        inline void setStyle(Qt::PenStyle  __value)
        {
            if (this->style != __value) {
                this->style = __value; 
                replot(); 
            } 
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc JKQTFPQOverlayLinearGridPlot::style </BLOCKQUOTE>
            \see style for more information */ 
        inline Qt::PenStyle getStyle() const
        {
            return this->style; 
        }

};
#endif // JKQTFASTPLOTTER_H
