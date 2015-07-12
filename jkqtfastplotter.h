/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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


/*
  Name: jkqtpfastplotter.h
  Copyright: (c) 2010
  Author: Jan krieger <jan@jkrieger.de>, http://www.jkrieger.de/
*/

/**
 * \defgroup jkqtfastplotter Speed-Optimized Plotter class
 * \ingroup tools_qt
 */

/** \file jkqtpfastplotter.h
  * \ingroup jkqtfastplotter
  */
#ifndef JKQTFASTPLOTTER_H
#define JKQTFASTPLOTTER_H

#include "jkqtp_imexport.h"
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

#include "jkqtptools.h"
#ifdef DEBUG_TIMING
#  include "jkqtphighrestimer.h"
#endif

#define JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH 0.05
// forward declaration
class JKQTFPPlot;

#ifndef JKQTFASTPLOTTER_NOOPENGL
#  define JKQTFASTPLOTTER_BASEWIDGET QGLWidget
#  define JKQTFASTPLOTTER_BASEWIDGET_CONSTRUCTOR(parent) QGLWidget(QGLFormat(QGL::StencilBuffer | QGL::AlphaChannel | QGL::Rgba), parent)
#else
#  define JKQTFASTPLOTTER_BASEWIDGET QWidget
#  define JKQTFASTPLOTTER_BASEWIDGET_CONSTRUCTOR(parent) QWidget(parent)
#endif

/**
 * \brief create a property variable and a default variable for it. Also creates a doxygen comment for the default variable
 * \ingroup jkqtfastplotter
 */
#define JKQTFPPROPERTY(type,varname) \
  type varname; \
  /** \brief default value for property property varname. \see varname for more information */ \
  type def_##varname;

/**
 * \brief saves the given property (for which also a def_property exists) into the given settings object
 * \ingroup jkqtfastplotter
 */
#define JKQTFPPROPERTYsave(settings, group, var, varname) \
    if (var!=def_##var) settings.setValue(group+varname, var);
/**
 * \brief loads the given property from the given settings object
 * \ingroup jkqtfastplotter
 */
#define JKQTFPPROPERTYload(settings, group, var, varname, varconvert) \
    var=settings.value(group+varname, var).varconvert;





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
      - The method set_doDrawing() allows to prevent replotting of the class contents, e.g. when you want to set
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
      - it is possible to keep a specified aspect ration
      - it is possible to couple different plots in a way, that they are plottetd with the same width/height
        both in pixel and world coordinates by using plotterSizesChanged() and synchronizeX() / synchronizeY().
    .

*/
class LIB_EXPORT JKQTFastPlotter : public JKQTFASTPLOTTER_BASEWIDGET {
        Q_OBJECT
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
        JKQTFPPROPERTY(int, plotBorderLeft);
        /** \brief free space between widget bottom border and plot bottom border  */
        JKQTFPPROPERTY(int, plotBorderBottom);
        /** \brief free space between widget right border and plot left border */
        JKQTFPPROPERTY(int, plotBorderRight);
        /** \brief free space between widget top border and plot bottom border */
        JKQTFPPROPERTY(int, plotBorderTop);

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

        /** \brief plot width in pixels inside the widget (calculated by calcPlotScaling() from plotBorderLeft, plotBorderRight and widgetWidth) */
        int plotWidth;
        /** \brief plot height in pixels inside the widget (calculated by calcPlotScaling() from plotBorderTop, plotBorderBottom and widgetHeight) */
        int plotHeight;

        /** \brief color of the coordinate system */
        JKQTFPPROPERTY(QColor, systemColor);
        /** \brief width of the coordinate (in pixel) */
        JKQTFPPROPERTY(double, systemWidth);
        /** \brief color of the background*/
        JKQTFPPROPERTY(QColor, backgroundColor);
        /** \brief color of the plot's background
         *
         *  \note the background is not drawn if this color is set to \c QColor(Qt::transparent) !
         */
        JKQTFPPROPERTY(QColor, plotBackgroundColor);
        /** \brief indicates whether to draw a system box */
        JKQTFPPROPERTY(bool, drawSystemBox);
        /** \brief indicates whether to draw axes at x=0/y=0 */
        JKQTFPPROPERTY(bool, drawZeroAxes);

        /** \brief indicates whether to draw a grid */
        JKQTFPPROPERTY(bool, drawGrid);
        /** \brief color of the coordinate grid */
        JKQTFPPROPERTY(QColor, gridColor);
        /** \brief style of the coordinate grid */
        JKQTFPPROPERTY(Qt::PenStyle, gridStyle);
        /** \brief width of the coordinate grid (in pixel) */
        JKQTFPPROPERTY(double, gridWidth);
        /** \brief font size (in points) of the axis labels */
        JKQTFPPROPERTY(double, labelFontSize);
        /** \brief font name of the axis labels */
        JKQTFPPROPERTY(QString, labelFontName);
        /** \brief font size (in points) of the tick labels */
        JKQTFPPROPERTY(double, tickFontSize);
        /** \brief font name of the tick labels */
        JKQTFPPROPERTY(QString, tickFontName);

        /** \brief length of an axis tick (pixels) */
        JKQTFPPROPERTY(double, tickLength);


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

        /** \brief aspect ration of the plot, only used when maintainAspectRation is \c true
         *
         * The aspect ratio is defined as \c width/height of the plot in pixels.
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
        virtual void paintEvent(QPaintEvent *event);

        /** \brief resizes the internal representation (image) of the graphs */
        virtual void resizeEvent(QResizeEvent *event);


        /** \brief event handler for a double click */
        virtual void mouseDoubleClickEvent ( QMouseEvent * event );

        /** \brief event handler for a mouse move */
        virtual void mouseMoveEvent ( QMouseEvent * event );

        /** \brief event handler for a mouse button press */
        virtual void mousePressEvent ( QMouseEvent * event );

        /** \brief event handler for a mouse button is released */
        virtual void mouseReleaseEvent(QMouseEvent* event);

        /** \brief paint the coordinate system */
        void plotSystem(QPainter& painter);

        /** \brief paint the graphs */
        void plotGraphs(QPainter& painter);

        /** \brief recalculate the scaling of the plot */
        void calcPlotScaling();

        QSize minimumSizeHint() const;

        /**  \brief mutex to lock global widget repaint */
        QMutex mutexRepaint;

        /**  \brief mutex to lock data repaint */
        QMutex mutexRepaintData;

        /**  \brief mutex to lock system repaint */
        QMutex mutexRepaintSystem;

    public:
        /** \brief class constructor */
        JKQTFastPlotter(QWidget *parent = 0);

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
        };

        /** \brief return x coordinate coordinate from x-pixel */
        inline double p2x(long x) {
            if (xAxisLog) {
                return exp(log(10.0)*((double)x-xOffset)/(xScale));
            } else {
                return ((double)x-xOffset)/(xScale);
            }
        };

        /** \brief return y-pixel coordinate from y coordinate */
        inline double y2p(double y) {
            if (yAxisLog) {
                if (y<0) return yOffset-log(yMin/10.0)/log(10.0)*yScale;
                return yOffset-log(y)/log(10.0)*yScale;
            } else {
                return yOffset-y*yScale;
            }
        };

        /** \brief return y coordinate coordinate from y-pixel */
        inline double p2y(long y) {
            if (yAxisLog) {
                return exp(log(10.0)*((double)y-yOffset)/(-1.0*yScale));
            } else {
                return ((double)y-yOffset)/(-1.0*yScale);
            }
        };

        JKQTPGET_SET_MACRO_I(QPen, dragShapePen, update())
        JKQTPGET_SET_MACRO_I(bool, dragLine, update())

        JKQTPGET_SET_MACRO_I(int, plotBorderLeft, update_plot())
        JKQTPGET_SET_MACRO_I(int, plotBorderBottom, update_plot())
        JKQTPGET_SET_MACRO_I(int, plotBorderRight, update_plot())
        JKQTPGET_SET_MACRO_I(int, plotBorderTop, update_plot())
        JKQTPGET_MACRO(int, plotWidth)
        JKQTPGET_MACRO(int, plotHeight)
        JKQTPGET_SET_MACRO(bool, doDrawing)
        JKQTPGET_MACRO(int, internalPlotBorderLeft)
        JKQTPGET_MACRO(int, internalPlotBorderBottom)
        JKQTPGET_MACRO(int, internalPlotBorderRight)
        JKQTPGET_MACRO(int, internalPlotBorderTop)
        JKQTPGET_MACRO(double, xMin)
        JKQTPGET_MACRO(double, xMax)
        JKQTPGET_MACRO(double, yMin)
        JKQTPGET_MACRO(double, yMax)
        JKQTPGET_MACRO(bool, xAxisLog)
        JKQTPGET_MACRO(bool, yAxisLog)

        JKQTPGET_SET_MACRO_I(QColor, backgroundColor, update_plot())
        JKQTPGET_SET_MACRO_I(QColor, plotBackgroundColor, update_plot())
        JKQTPGET_SET_MACRO_I(bool, drawGrid, update_plot())
        JKQTPGET_SET_MACRO_I(QColor, gridColor, update_plot())
        JKQTPGET_SET_MACRO_I(Qt::PenStyle, gridStyle, update_plot())
        JKQTPGET_SET_MACRO_I(double, gridWidth, update_plot())
        JKQTPGET_SET_MACRO_I(double, labelFontSize, update_plot())
        JKQTPGET_SET_MACRO_I(QString, labelFontName, update_plot())
        JKQTPGET_SET_MACRO_I(double, tickFontSize, update_plot())
        JKQTPGET_SET_MACRO_I(QString, tickFontName, update_plot())
        JKQTPGET_SET_MACRO_I(double, tickLength, update_plot())
        JKQTPGET_SET_MACRO_I(bool, drawSystemBox, update_plot())
        JKQTPGET_SET_MACRO_I(bool, drawZeroAxes, update_plot())
        JKQTPGET_SET_MACRO_I(QColor, systemColor, update_plot())
        JKQTPGET_SET_MACRO_I(double, systemWidth, update_plot())
        JKQTPGET_SET_MACRO_I(double, xZeroTick, update_plot())
        JKQTPGET_SET_MACRO_I(double, yZeroTick, update_plot())
        JKQTPGET_SET_MACRO_I(double, xTickDistance, update_plot())
        JKQTPGET_SET_MACRO_I(double, yTickDistance, update_plot())
        JKQTPGET_SET_MACRO_I(QString, xAxisLabel, update_plot())
        JKQTPGET_SET_MACRO_I(QString, yAxisLabel, update_plot())
        JKQTPGET_SET_MACRO_I(double, aspectRatio, update_plot())
        JKQTPGET_SET_MACRO_I(bool, maintainAspectRatio, update_plot())
        JKQTPGET_SET_MACRO_I(bool, xAxisLabelVisible, update_plot())
        JKQTPGET_SET_MACRO_I(bool, yAxisLabelVisible, update_plot())
        JKQTPGET_SET_MACRO_I(JKQTFastPlotter*, synchronizeX, update_plot())
        JKQTPGET_SET_MACRO_I(JKQTFastPlotter*, synchronizeY, update_plot())

        /** \brief draw the contents onto any QPainter, starting at (0,0), returns the size of the whole plot in \a size, if supplied with the default background color */
        void draw(QPainter* painter, QSize* size=NULL);

        /** \brief draw the contents onto any QPainter, starting at (0,0), returns the size of the whole plot in \a size, if supplied with the supplied\a background color */
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
        void update_plot();
        /** \brief replot everything (slowest possible plotting) and forces a repaint calling QWidget::repaint() */
        void update_plot_immediate();
        /** \brief replot only the data
         *
         *  This internally calls QWidget::update(), so no immediate repaint() is forced! If you want an immediate update,
         * call update_data_immediate() instead!
         */
        void update_data();
        /** \brief replot only the data, forces a redraw by calling QWidget::repaint()  */
        void update_data_immediate();

        /** \brief set xMin*/
        void set_xMin(double value){
            set_doDrawing(false);
            setXRange(value,xMax,xAxisLog);
            set_doDrawing(true);
        }
        /** \brief set xMax*/
        void set_xMax(double value){
            set_doDrawing(false);
            setXRange(xMin,value,xAxisLog);
            set_doDrawing(true);
        }
        /** \brief set yMin*/
        void set_yMin(double value){
            set_doDrawing(false);
            setYRange(value,yMax,yAxisLog);
            set_doDrawing(true);
        }
        /** \brief set yMax*/
        void set_yMax(double value){
            set_doDrawing(false);
            setYRange(yMin,value,yAxisLog);
            set_doDrawing(true);
        }


};





/*! \brief base class for all plots that may be plotted by JKQTFastPlotter
    \ingroup jkqtfastplotter
*/
class LIB_EXPORT JKQTFPPlot: public QObject {
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
        virtual ~JKQTFPPlot() {};

        /** \brief set the parent of the plot */
        void setParent(JKQTFastPlotter* parent) { this->parent=parent; QObject::setParent(parent); };

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
class LIB_EXPORT JKQTFPLinePlot: public JKQTFPPlot {
    public:
        /** \brief used to store which datatype is used for the plot data */
        enum DataType {
            JKQTFPLPPointer,
            JKQTFPLPVector
        };
    protected:
        /** \brief number of datapoints in the plot */
        unsigned int N;
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
        /** \brief width of the graph (in pixels) */
        double width;
        /** \brief color of the graph */
        QColor errorColor;
        /** \brief style of the graph */
        Qt::PenStyle errorStyle;
        /** \brief width of the graph (in pixels) */
        double errorWidth;
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pixels)
         */
        JKQTFPLinePlot(JKQTFastPlotter* parent, unsigned int N, double* x, double* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;
        /*! \brief class constructor

            \param parent parent widget
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pixels)
         */
        JKQTFPLinePlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);

        inline void set_data(double* x, double* y, unsigned int N) {
            this->x=x;
            this->y=y;
            this->yerr=NULL;
            this->N=N;
            datatype=JKQTFPLPPointer;
            replot();
        };

        inline void set_data(double* x, double* y, double* yerr, unsigned int N) {
            this->x=x;
            this->y=y;
            this->yerr=yerr;
            this->N=N;
            datatype=JKQTFPLPPointer;
            replot();
        };

        inline void set_data(QVector<double>* x, QVector<double>* y) {
            this->x=NULL;
            this->y=NULL;
            this->yerr=NULL;
            this->xv=x;
            this->yv=y;
            this->yerrv=NULL;
            this->N=x->size();
            datatype=JKQTFPLPVector;
            replot();
        };

        inline void set_data(QVector<double>* x, QVector<double>* y, QVector<double>* yerr) {
            this->x=NULL;
            this->y=NULL;
            this->yerr=NULL;
            this->xv=x;
            this->yv=y;
            this->yerrv=yerr;
            this->N=x->size();
            datatype=JKQTFPLPVector;
            replot();
        };

        inline unsigned int get_N() {
            if (datatype==JKQTFPLPPointer)
                return N;
            if (datatype==JKQTFPLPVector)
                return xv->size();
            return N;
        }

        JKQTPGET_MACRO(double*, x)
        JKQTPGET_MACRO(double*, y)
        JKQTPGET_MACRO(double*, yerr)
        JKQTPGET_MACRO(QVector<double>*, xv)
        JKQTPGET_MACRO(QVector<double>*, yv)
        JKQTPGET_MACRO(QVector<double>*, yerrv)
        JKQTPGET_MACRO(DataType, datatype)
        JKQTPGET_SET_MACRO_I(QColor, color, replot())
        JKQTPGET_SET_MACRO_I(Qt::PenStyle, style, replot())
        JKQTPGET_SET_MACRO_I(double, width, replot())
        JKQTPGET_SET_MACRO_I(QColor, errorColor, replot())
        JKQTPGET_SET_MACRO_I(Qt::PenStyle, errorStyle, replot())
        JKQTPGET_SET_MACRO_I(double, errorWidth, replot())
};




/*! \brief a simple plot that draws a cross for every datapoint
    \ingroup jkqtfastplotter

    This class does not support y errors!
*/
class LIB_EXPORT JKQTFPVCrossPlot: public JKQTFPLinePlot {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pixels)
         */
        JKQTFPVCrossPlot(JKQTFastPlotter* parent, unsigned int N, double* x, double* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /*! \brief class constructor

            \param parent parent widget
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pixels)
         */
        JKQTFPVCrossPlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);

        JKQTPGET_SET_MACRO_I(double, crossWidth, replot())
    protected:
        /** \brief width of the crosses */
        double crossWidth;

};



/*! \brief a simple vertical bar plot for JKQTFastPlotter
    \ingroup jkqtfastplotter

    This class does not support y errors!
*/
class LIB_EXPORT JKQTFPVBarPlot: public JKQTFPLinePlot {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent parent widget
            \param N number of datapoints in the plot
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pixels)
         */
        JKQTFPVBarPlot(JKQTFastPlotter* parent, unsigned int N, double* x, double* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /*! \brief class constructor

            \param parent parent widget
            \param x points to the x values in the plot
            \param y points to the y values in the plot
            \param color color of the plot
            \param style style of the graph
            \param width width of the plot (in pixels)
         */
        JKQTFPVBarPlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);
};


/*! \brief plot a range of x values
    \ingroup jkqtfastplotter

*/
class LIB_EXPORT JKQTFPXRangePlot: public JKQTFPPlot {
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
        /** \brief width of the graph (in pixels) */
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
            \param width width of the plot (in pixels)
         */
        JKQTFPXRangePlot(JKQTFastPlotter* parent, double xmin, double xmax, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1, Qt::BrushStyle fillStyle=Qt::NoBrush) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);

        JKQTPGET_SET_MACRO_I(QColor, color, replot())
        JKQTPGET_SET_MACRO_I(QColor, fillColor, replot())
        JKQTPGET_SET_MACRO_I(Qt::BrushStyle, fillStyle, replot())
        JKQTPGET_SET_MACRO_I(Qt::PenStyle, style, replot())
        JKQTPGET_SET_MACRO_I(double, width, replot())
        JKQTPGET_MACRO(double, xmin)
        JKQTPGET_MACRO(double, xmax)
        JKQTPGET_MACRO(double, centerline)
        JKQTPGET_SET_MACRO_I(bool, showCenterline, replot())
    public slots:
        void set_centerline(int centerline) {
            if (this->centerline!=centerline) {
                this->centerline=centerline;
                replot();
            }
        }
        void set_xmin(double xmin) {
            if (this->xmin!=xmin) {
                this->xmin=xmin;
                replot();
            }
        }
        void set_xmax(double xmax) {
            if (this->xmax!=xmax) {
                this->xmax=xmax;
                replot();
            }
        }
        void set_xmin(int xmin) {
            if (this->xmin!=xmin) {
                this->xmin=xmin;
                replot();
            }
        }
        void set_xmax(int xmax) {
            if (this->xmax!=xmax) {
                this->xmax=xmax;
                replot();
            }
        }
};



/*! \brief plot a range of x values
    \ingroup jkqtfastplotter

*/
class LIB_EXPORT JKQTFPYRangePlot: public JKQTFPPlot {
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
        /** \brief width of the graph (in pixels) */
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
            \param width width of the plot (in pixels)
         */
        JKQTFPYRangePlot(JKQTFastPlotter* parent, double ymin, double ymax, QColor color=QColor("red"), Qt::PenStyle style=Qt::SolidLine, double width=1, Qt::BrushStyle fillStyle=Qt::NoBrush) ;

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);

        JKQTPGET_SET_MACRO_I(QColor, color, replot())
        JKQTPGET_SET_MACRO_I(QColor, fillColor, replot())
        JKQTPGET_SET_MACRO_I(Qt::BrushStyle, fillStyle, replot())
        JKQTPGET_SET_MACRO_I(Qt::PenStyle, style, replot())
        JKQTPGET_SET_MACRO_I(double, width, replot())
        JKQTPGET_MACRO(double, ymin)
        JKQTPGET_MACRO(double, ymax)
        JKQTPGET_MACRO(double, centerline)
        JKQTPGET_SET_MACRO_I(bool, showCenterline, replot())
    public slots:
        void set_centerline(int centerline) {
            if (this->centerline!=centerline) {
                this->centerline=centerline;
                replot();
            }
        }
        void set_ymin(double xmin) {
            if (this->ymin!=xmin) {
                this->ymin=xmin;
                replot();
            }
        }
        void set_ymax(double xmax) {
            if (this->ymax!=xmax) {
                this->ymax=xmax;
                replot();
            }
        }
        void set_ymin(int xmin) {
            if (this->ymin!=xmin) {
                this->ymin=xmin;
                replot();
            }
        }
        void set_ymax(int xmax) {
            if (this->ymax!=xmax) {
                this->ymax=xmax;
                replot();
            }
        }
};



/*! \brief a plot of a QImage
    \ingroup jkqtfastplotter
*/
class LIB_EXPORT JKQTFPQImagePlot: public JKQTFPPlot {
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
        virtual void drawGraph(QPainter& painter);

        JKQTPGET_SET_MACRO_I(QImage*, image, replot())
        JKQTPGET_SET_MACRO_I(double, xmin, replot())
        JKQTPGET_SET_MACRO_I(double, xmax, replot())
        JKQTPGET_SET_MACRO_I(double, ymin, replot())
        JKQTPGET_SET_MACRO_I(double, ymax, replot())
};



/*!
   \brief An enum for selecting the palette for coloring
    \ingroup jkqtfastplotter
   \details Here, the color palettes are illustrated (left is the color for the minimum and right for the maximum).
 */
enum JKQTFPColorPalette {
    JKQTFP_RED=0, /*!< \image html RED.png */
    JKQTFP_INVERTEDRED=1, /*!< \image html RED.png */
    JKQTFP_GREEN=2, /*!< \image html GREEN.png */
    JKQTFP_INVERTEDGREEN=3, /*!< \image html GREEN.png */
    JKQTFP_BLUE=4, /*!< \image html BLUE.png */
    JKQTFP_INVERTEDBLUE=5, /*!< \image html BLUE.png */
    JKQTFP_GRAY=6, /*!< \image html GRAY.png */
    JKQTFP_INVERTEDGRAY=7, /*!< \image html GRAY.png */
    JKQTFP_MATLAB=8, /*!< \image html MATLAB.png */
    JKQTFP_RYGB=9, /*!< \image html RYGB.png */
    JKQTFP_HSV=10, /*!< \image html HSV.png */
    JKQTFP_INVERTED_HSV=11, /*!< \image html INVERTED_HSV.png */
    JKQTFP_RAINBOW=12, /*!< \image html RAINBOW.png */
    JKQTFP_HOT=13, /*!< \image html HOT.png */
    JKQTFP_OCEAN=14, /*!< \image html OCEAN.png */
    JKQTFP_TRAFFICLIGHT=15, /*!< \image html TRAFFICLIGHT.png */
    JKQTFP_BLUEMAGENTAYELLOW=16, /*!< \image html BLUEMAGENTAYELLOW.png */
    JKQTFP_BLUEYELLOW=17, /*!< \image html BLUEYELLOW.png */
    JKQTFP_CYAN=18 /*!< \image html CYAN.png */
};

/*!datatype for an image plotpalette for coloring
    \ingroup jkqtfastplotter
   \details Here, the color palettes are illustrated (left is the color for the minimum and right for the maximum).
 */
enum JKQTFPImageFormat {
    JKQTFP_uint8=0,  /*!< \image 8 bit unsigned int */
    JKQTFP_uint16=1, /*!< \image 16 bit unsigned int */
    JKQTFP_uint32=2, /*!< \image 32 bit unsigned int */
    JKQTFP_float=3,  /*!< \image float */
    JKQTFP_double=4, /*!< \image double */
    JKQTFP_int64=5   /*!< \image 64 bit signed int */
};


/*! \brief size of the lookup tables used by JKQTFPimagePlot_array2image()
    \ingroup jkqtfastplotter
*/
#define JKQTFPimagePlot_LUTSIZE 256


/*! \brief convert a 2D image (as 1D array) into a QImage with given palette (see JKQTFPColorPalette)
    \ingroup jkqtfastplotter

    This method uses lookup tables which are saved as static variables to convert the 2D array into
    an image. The luts are only created once, and stored then, so mor CPU time is saved. The precompiler define
    JKQTFPimagePlot_LUTSIZE sets the size of the LUTs. Note that if you don't use a specific color palette,
    the according LUT won't be calculated and stored!
*/
template <class T>
inline void JKQTFPimagePlot_array2image(T* dbl, int width, int height, QImage &img, JKQTFPColorPalette palette, T minColor, T maxColor)
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
		    register T v=dbl[i];
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
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"      finding min&max: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif
    //std::cout<<"minColor="<<minColor<<"   maxColor="<<maxColor<<"       min="<<min<<"  max="<<max<<"\n";

	double delta=max-min;

    unsigned int* lut_used=NULL;
    static unsigned int* lut_red=NULL;
    static unsigned int* lut_green=NULL;
    static unsigned int* lut_blue=NULL;
    static unsigned int* lut_gray=NULL;
    static unsigned int* lut_invred=NULL;
    static unsigned int* lut_invgreen=NULL;
    static unsigned int* lut_invblue=NULL;
    static unsigned int* lut_invgray=NULL;
    static unsigned int* lut_matlab=NULL;
    static unsigned int* lut_rygb=NULL;
    static unsigned int* lut_hsv=NULL;
    static unsigned int* lut_invhsv=NULL;
    static unsigned int* lut_rainbow=NULL;
    static unsigned int* lut_hot=NULL;
    static unsigned int* lut_ocean=NULL;
    static unsigned int* lut_trafficlight=NULL;
    static unsigned int* lut_bluemagentayellow=NULL;
    static unsigned int* lut_blueyellow=NULL;
    static unsigned int* lut_cyan=NULL;


    img = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
	if (min == max)
		img.fill(0);
	else
	{
        if (palette == JKQTFP_RED)
        {
            unsigned int** plut=&lut_red;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
                        (*plut)[l]=qRgb(static_cast<int>(255.0*v), 0, 0);
                    }
                }
            }
            lut_used=(*plut);

        }
        else if (palette == JKQTFP_GREEN)
		{
            unsigned int** plut=&lut_green;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
                        (*plut)[l]=qRgb(0, static_cast<int>(255.0*v), 0);
                    }
                }
            }
            lut_used=(*plut);

		}
		else if (palette == JKQTFP_BLUE)
		{
            unsigned int** plut=&lut_blue;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
                        (*plut)[l]=qRgb(0, 0, static_cast<int>(255.0*v));
                    }
                }
            }
            lut_used=(*plut);

		}


        else if (palette == JKQTFP_GRAY)
		{
            unsigned int** plut=&lut_gray;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_invred;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
                        (*plut)[l]=qRgb(static_cast<int>(255.0*(1.0-v)), 0, 0);
                    }
                }
            }
            lut_used=(*plut);

		}
		else if (palette == JKQTFP_INVERTEDGREEN)
		{
            unsigned int** plut=&lut_invgreen;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
                        (*plut)[l]=qRgb(0, static_cast<int>(255.0*(1.0-v)), 0);
                    }
                }
            }
            lut_used=(*plut);
		}
		else if (palette == JKQTFP_INVERTEDBLUE)
		{
            unsigned int** plut=&lut_invblue;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
                        (*plut)[l]=qRgb(0, 0, static_cast<int>(255.0*(1.0-v)));
                    }
                }
            }
            lut_used=(*plut);
		}
		else if (palette == JKQTFP_INVERTEDGRAY)
		{
            unsigned int** plut=&lut_invgray;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=1.0-(l/(double)(JKQTFPimagePlot_LUTSIZE));
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
            unsigned int** plut=&lut_matlab;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_rygb;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_hsv;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_invhsv;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            if (lut_rainbow==NULL) {
                lut_rainbow=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(int));
                //std::cout<<"!!! creating rainbow lut\n";
                if (lut_rainbow!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_hot;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_ocean;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_bluemagentayellow;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_blueyellow;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_cyan;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
            unsigned int** plut=&lut_trafficlight;
            if ((*plut)==NULL) {
                (*plut)=(unsigned int*)malloc((JKQTFPimagePlot_LUTSIZE+2)*sizeof(unsigned int));
                if ((*plut)!=NULL) {
                    for (int l=0; l<=JKQTFPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTFPimagePlot_LUTSIZE);
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
        time_gt=timer.get_time();
        time_sum+=time_gt;
        std::cout<<"      calculated/found LUT: "<<time_gt/1000.0<<" ms\n";
        timer.start();
        #endif
		if (lut_used!=NULL) {
		    // LUT found: collor the image accordingly
			for (register int j=0; j<height; ++j) {
			    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
				for (register int i=0; i<width; ++i) {
					int v = (dbl[j*width+i]-min)/delta*JKQTFPimagePlot_LUTSIZE;
					v = (v < 0) ? 0 : ( (v > JKQTFPimagePlot_LUTSIZE) ? JKQTFPimagePlot_LUTSIZE : v);
					line[i]=lut_used[v];
				}
			}
        } else {
            // no LUT found: paint a black image!
			img.fill(0);
		}
	}
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"      creating colored image: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    sum: "<<time_sum/1000.0<<" ms\n";
    timer.start();
    #endif
};

/*! \brief get list with all available palettes
    \ingroup jkqtfastplotter
    \see JKQTFPimagePlot_array2image()
 */
LIB_EXPORT QStringList JKQTFPimagePlot_getPalettes();
/*! \brief get QIcon representing the given palette
    \ingroup jkqtfastplotter
    \see JKQTFPimagePlot_array2image()
*/
LIB_EXPORT QIcon JKQTFPimagePlot_getPaletteIcon(int i);
/*! \brief get QIcon representing the given palette
    \ingroup jkqtfastplotter
    \see JKQTFPimagePlot_array2image()
*/
LIB_EXPORT QIcon JKQTFPimagePlot_getPaletteIcon(JKQTFPColorPalette palette);


/*! \brief plots a given grayscale image with a given color palette
    \ingroup jkqtfastplotter

      - This plot may plot any 2D array of data (8-, 16-, 32-bit integer images, float and double).
      - This class also draws a color bar in the right border of the plot
    .

*/
class LIB_EXPORT JKQTFPimagePlot: public JKQTFPPlot {
        Q_OBJECT
    protected:
        /** \brief image to plot */
        void* image;
        /** \brief format of the image */
        JKQTFPImageFormat imageFormat;
        /** \brief width of image in pixels */
        unsigned int width;
        /** \brief height of image in pixels */
        unsigned int height;
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
        /** \brief color bar width in pixels */
        int colorBarWidth;
        /** \brief rotation of the image when plotting in units of 90 degrees (counter clock wise) */
        int rotation;
    public:
        /*! \brief class constructor

         */
        JKQTFPimagePlot(JKQTFastPlotter* parent, void* image, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax, JKQTFPColorPalette palette=JKQTFP_MATLAB) ;

        JKQTFPimagePlot(JKQTFastPlotter* parent, void* image, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height, JKQTFPColorPalette palette=JKQTFP_MATLAB);

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);



        JKQTPGET_SET_MACRO_I(void*, image, replot())
        JKQTPGET_SET_MACRO_I(JKQTFPImageFormat, imageFormat, replot())
        JKQTPGET_SET_MACRO_I(unsigned int, width, replot())
        JKQTPGET_SET_MACRO_I(unsigned int, height, replot())
        JKQTPGET_SET_MACRO_I(double, xmin, replot())
        JKQTPGET_SET_MACRO_I(double, xmax, replot())
        JKQTPGET_SET_MACRO_I(double, ymin, replot())
        JKQTPGET_SET_MACRO_I(double, ymax, replot())
        JKQTPGET_SET_MACRO_I(bool, drawColorBar, replot())
        JKQTPGET_SET_MACRO_I(int, colorBarWidth, replot())
        JKQTPGET_MACRO(int, rotation)
        JKQTPGET_MACRO(JKQTFPColorPalette, palette)
        JKQTPGET_MACRO(double, colorMin)
        JKQTPGET_MACRO(double, colorMax)
        void set_image(void* image, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height) {
            this->image=image;
            this->imageFormat=imageFormat;
            this->width=width;
            this->height=height;
            replot();
        }
    public slots:
        void set_rotation(int rotation) {
            if (this->rotation!=rotation) {
                this->rotation=(JKQTFPColorPalette)rotation;
                replot();
            }
        }
        void set_palette(int palette) {
            if (this->palette!=palette) {
                this->palette=(JKQTFPColorPalette)palette;
                replot();
            }
        };
        void set_palette(JKQTFPColorPalette palette) {
            if (this->palette!=palette) {
                this->palette=palette;
                replot();
            }
        };
        void set_colorMin(uint32_t colorMin) {
            if (this->colorMin!=colorMin) {
                this-> colorMin= colorMin;
                replot();
            }
        }
        void set_colorMax(uint32_t colorMax) {
            if (this->colorMax!=colorMax) {
                this->colorMax=colorMax;
                replot();
            }
        }
        void set_colorMin(int colorMin) {
            if (this->colorMin!=colorMin) {
                this-> colorMin= colorMin;
                replot();
            }
        }
        void set_colorMax(int colorMax) {
            if (this->colorMax!=colorMax) {
                this->colorMax=colorMax;
                replot();
            }
        }
        void set_colorMin(double colorMin) {
            if (this->colorMin!=colorMin) {
                this-> colorMin= colorMin;
                replot();
            }
        }
        void set_colorMax(double colorMax) {
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
            register T v=dbl[i];
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
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"      finding min&max: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif
    //qDebug()<<"minColor="<<minColor<<"   maxColor="<<maxColor<<"       min="<<min<<"  max="<<max<<"\n";

    double delta=max-min;


    if (min != max) {
        // LUT found: collor the image accordingly
        if (channel==0) {
            for (register int j=0; j<height; ++j) {
                QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                for (register int i=0; i<width; ++i) {
                    int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    const register QRgb l=line[i];
                    line[i]=qRgb(v,qGreen(l),qBlue(l));
                    //if (i==j) qDebug()<<dbl[j*width+i]<<v<<hex<<l<<line[i]<<dec;
                }
            }
        } else if (channel==1) {
            for (register int j=0; j<height; ++j) {
                QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                for (register int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    const register QRgb l=line[i];
                    line[i]=qRgb(qRed(l),v,qBlue(l));
                    //if (i==j) qDebug()<<dbl[j*width+i]<<v<<hex<<l<<line[i]<<dec;
                }
            }
        } else if (channel==2) {
            for (register int j=0; j<height; ++j) {
                QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                for (register int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    const register QRgb l=line[i];
                    line[i]=qRgb(qRed(l),qGreen(l),v);
                    //if (i==j) qDebug()<<dbl[j*width+i]<<v<<hex<<l<<line[i]<<dec;
                }
            }
        }
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"      creating colored image: "<<time_gt/1000.0<<" ms\n";
    timer.start();

    time_gt=timer.get_time();
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
class LIB_EXPORT JKQTFPRGBImageOverlayPlot: public JKQTFPPlot {
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
        /** \brief width of image in pixels */
        unsigned int width;
        /** \brief height of image in pixels */
        unsigned int height;
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
        JKQTFPRGBImageOverlayPlot(JKQTFastPlotter* parent, void* imageRed, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax) ;
        JKQTFPRGBImageOverlayPlot(JKQTFastPlotter* parent, void* imageRed, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height);
        JKQTFPRGBImageOverlayPlot(JKQTFastPlotter* parent);

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);



        JKQTPGET_SET_MACRO_TWO_I(imageRed, void*, imageRed, JKQTFPImageFormat, imageFormatRed, replot())
        JKQTPGET_SET_MACRO_TWO_I(imageGreen, void*, imageGreen, JKQTFPImageFormat, imageFormatGreen, replot())
        JKQTPGET_SET_MACRO_TWO_I(imageBlue, void*, imageBlue, JKQTFPImageFormat, imageFormatBlue, replot())
        JKQTPGET_SET_MACRO_I(unsigned int, width, replot())
        JKQTPGET_SET_MACRO_I(unsigned int, height, replot())
        JKQTPGET_SET_MACRO_I(double, xmin, replot())
        JKQTPGET_SET_MACRO_I(double, xmax, replot())
        JKQTPGET_SET_MACRO_I(double, ymin, replot())
        JKQTPGET_SET_MACRO_I(double, ymax, replot())
        JKQTPGET_MACRO(int, rotation)
        JKQTPGET_MACRO(double, colorMinRed)
        JKQTPGET_MACRO(double, colorMaxRed)
        JKQTPGET_MACRO(double, colorMinGreen)
        JKQTPGET_MACRO(double, colorMaxGreen)
        JKQTPGET_MACRO(double, colorMinBlue)
        JKQTPGET_MACRO(double, colorMaxBlue)
        void set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, unsigned int width, unsigned int height);
        void set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, unsigned int width, unsigned int height);
        void set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, void* imageBlue, JKQTFPImageFormat imageFormatBlue, unsigned int width, unsigned int height);
        void set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax);
        void set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax);
        void set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, void* imageBlue, JKQTFPImageFormat imageFormatBlue, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax);
    public slots:
        inline void set_rotation(int rotation) {
            if (this->rotation!=rotation) {
                this->rotation=(JKQTFPColorPalette)rotation;
                replot();
            }
        }
        inline void set_colorMinRed(uint32_t colorMin) {
            if (this->colorMinRed!=colorMin) {
                this-> colorMinRed= colorMin;
                replot();
            }
        }
        inline void set_colorMaxRed(uint32_t colorMax) {
            if (this->colorMaxRed!=colorMax) {
                this->colorMaxRed=colorMax;
                replot();
            }
        }
        inline void set_colorMinRed(int colorMin) {
            if (this->colorMinRed!=colorMin) {
                this-> colorMinRed= colorMin;
                replot();
            }
        }
        inline void set_colorMaxRed(int colorMax) {
            if (this->colorMaxRed!=colorMax) {
                this->colorMaxRed=colorMax;
                replot();
            }
        }
        inline void set_colorMinRed(double colorMin) {
            if (this->colorMinRed!=colorMin) {
                this-> colorMinRed= colorMin;
                replot();
            }
        }
        inline void set_colorMaxRed(double colorMax) {
            if (this->colorMaxRed!=colorMax) {
                this->colorMaxRed=colorMax;
                replot();
            }
        }


        inline void set_colorMinGreen(uint32_t colorMin) {
            if (this->colorMinGreen!=colorMin) {
                this-> colorMinGreen= colorMin;
                replot();
            }
        }
        inline void set_colorMaxGreen(uint32_t colorMax) {
            if (this->colorMaxGreen!=colorMax) {
                this->colorMaxGreen=colorMax;
                replot();
            }
        }
        inline void set_colorMinGreen(int colorMin) {
            if (this->colorMinGreen!=colorMin) {
                this-> colorMinGreen= colorMin;
                replot();
            }
        }
        inline void set_colorMaxGreen(int colorMax) {
            if (this->colorMaxGreen!=colorMax) {
                this->colorMaxGreen=colorMax;
                replot();
            }
        }
        inline void set_colorMinGreen(double colorMin) {
            if (this->colorMinGreen!=colorMin) {
                this-> colorMinGreen= colorMin;
                replot();
            }
        }
        inline void set_colorMaxGreen(double colorMax) {
            if (this->colorMaxGreen!=colorMax) {
                this->colorMaxGreen=colorMax;
                replot();
            }
        }

        inline void set_colorMinBlue(uint32_t colorMin) {
            if (this->colorMinBlue!=colorMin) {
                this-> colorMinBlue= colorMin;
                replot();
            }
        }
        inline void set_colorMaxBlue(uint32_t colorMax) {
            if (this->colorMaxBlue!=colorMax) {
                this->colorMaxBlue=colorMax;
                replot();
            }
        }
        inline void set_colorMinBlue(int colorMin) {
            if (this->colorMinBlue!=colorMin) {
                this-> colorMinBlue= colorMin;
                replot();
            }
        }
        inline void set_colorMaxBlue(int colorMax) {
            if (this->colorMaxBlue!=colorMax) {
                this->colorMaxBlue=colorMax;
                replot();
            }
        }
        inline void set_colorMinBlue(double colorMin) {
            if (this->colorMinBlue!=colorMin) {
                this-> colorMinBlue= colorMin;
                replot();
            }
        }
        inline void set_colorMaxBlue(double colorMax) {
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
class LIB_EXPORT JKQTFPimageOverlayPlot: public JKQTFPPlot {
        Q_OBJECT
    public:
        enum SymbolType {
            stCircle,
            stRectangle
        };

    protected:
        /** \brief image to plot */
        bool* image;
        /** \brief width of image in pixels */
        unsigned int width;
        /** \brief height of image in pixels */
        unsigned int height;
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
        JKQTFPimageOverlayPlot(JKQTFastPlotter* parent, bool* image, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax, QColor color=QColor("red"));

        JKQTFPimageOverlayPlot(JKQTFastPlotter* parent, bool* image, unsigned int width, unsigned int height, QColor color=QColor("red"));

        /** \brief draw the graph */
        virtual void drawGraph(QPainter& painter);

        void set_data(bool* image, unsigned int width, unsigned int height) {
            this->image=image;
            this->width=width;
            this->height=height;
            replot();
        }

        JKQTPGET_MACRO(bool*, image)
        JKQTPGET_MACRO(int, rotation)
        JKQTPGET_MACRO(unsigned int, width)
        JKQTPGET_MACRO(unsigned int, height)
        JKQTPGET_SET_MACRO_I(double, xmin, replot())
        JKQTPGET_SET_MACRO_I(double, xmax, replot())
        JKQTPGET_SET_MACRO_I(double, ymin, replot())
        JKQTPGET_SET_MACRO_I(double, ymax, replot())
        JKQTPGET_SET_MACRO_I(bool, showAsSymbols, replot())
        JKQTPGET_MACRO(QColor, color)
    public slots:
        inline void set_rotation(int rotation) {
            if (this->rotation!=rotation) {
                this->rotation=(JKQTFPColorPalette)rotation;
                replot();
            }
        }
        inline void set_color(QColor color) {
            if (this->color!=color) {
                this->color=color;
                replot();
            }
        };
};



/*! \brief plot a horizontal scale bar
    \ingroup jkqtfastplotter

*/
class LIB_EXPORT JKQTFPQScaleBarXPlot: public JKQTFPPlot {
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
        /** \brief line width of scale bar */
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
        virtual void drawGraph(QPainter& painter);

        JKQTPGET_SET_MACRO_I(double, width, replot())
        JKQTPGET_SET_MACRO_I(double, lineWidth, replot())
        JKQTPGET_SET_MACRO_I(QColor, color, replot())
        JKQTPGET_SET_MACRO_I(QString, label, replot())
        JKQTPGET_SET_MACRO_I(QFont, font, replot())
        JKQTPGET_SET_MACRO_I(Position, position, replot())
    public slots:
        void set_position(int pos) {
            set_position((Position)pos);
        }
};




/*! \brief plot a horizontal scale bar
    \ingroup jkqtfastplotter

*/
class LIB_EXPORT JKQTFPQOverlayLinearGridPlot: public JKQTFPPlot {
        Q_OBJECT

    protected:
        /** \brief distance between grid lines */
        double width;
        /** \brief line width of grid lines */
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
        virtual void drawGraph(QPainter& painter);

        JKQTPGET_SET_MACRO_I(double, width, replot())
        JKQTPGET_SET_MACRO_I(double, lineWidth, replot())
        JKQTPGET_SET_MACRO_I(QColor, color, replot())
        JKQTPGET_SET_MACRO_I(Qt::PenStyle, style, replot())

};
#endif // JKQTFASTPLOTTER_H
