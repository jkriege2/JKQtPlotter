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

#ifndef JKQTPELEMENTS_H
#define JKQTPELEMENTS_H

// forward declarations
class JKQtBasePlotter;
class JKQtPlotter;
class JKQTPcoordinateAxis;
class JKQTPdatastore;
//class JKQTPColorPaletteTools;

/** \brief this virtual base class describes one of the graphs, which are part of a JKQtPlotter object
 * \ingroup jkqtplotter_plots
 *
 * Each possible graph is represented by a subgraph of this. So additional plots may be created by
 * deriving new JKQTPgraph classes. To do so implement/overwrite these functions:
 *   - void draw(JKQTPEnhancedPainter& painter);
 *   - void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
 *   - bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
 *   - void getGraphsYMinMax(double& miny, double& maxy);
 *   - QColor getKeyLabelColor()=0;
 * .
 *
 * There are two properties datarange_start and datarange_end. By default they are -1 and therefore ignored.
 * if they are != -1 the plotter only displays the datapoints with the indexes [datarange_start .. datarange_end]
 * although there might be more data points available (range [0 .. maxDataPoints]). The datarange is cut at the
 * full range, i.e. if datarange_end>maxDataPoints the plotter displays [datarange_start .. maxDataPoints].
 */
class LIB_EXPORT JKQTPgraph: public QObject {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPgraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPgraph(JKQtPlotter* parent);

        /** \brief default wirtual destructor */
        inline virtual ~JKQTPgraph() {}

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter)=0;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect)=0;
        /** \brief returns an image with a key marker inside  */
        QImage generateKeyMarker(QSize size=QSize(16,16));

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero)=0;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero)=0;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor()=0;

        JKQTPGET_SET_VMACRO(QString, title)
        JKQTPGET_SET_MACRO(long long, datarange_start)
        JKQTPGET_SET_MACRO(long long, datarange_end)
        JKQTPGET_SET_MACRO(bool, visible)

        /** \brief returns the parent painter class */
        inline JKQtBasePlotter* getParent() { return parent; }
        /** \brief sets the parent painter class */
        virtual void setParent(JKQtBasePlotter* parent);
        /** \brief sets the parent painter class */
        virtual void setParent(JKQtPlotter* parent);

        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);
        /** \brief returns \c true if the given column is used by the graph */
        virtual bool usesColumn(int column);

    protected:
        /** \brief the plotter object this object belongs to */
        JKQtBasePlotter* parent;
        /** \brief this function is used to plot error inidcators before plotting the graphs.
         *
         * By default this function does nothing. But children of this class may overwrite it to implement
         * drawing error indicators.
         */
        inline virtual void drawErrorsBefore(JKQTPEnhancedPainter& /*painter*/) {}
        /** \brief this function is used to plot error inidcators after plotting the graphs.
         *
         * By default this function does nothing. But children of this class may overwrite it to implement
         * drawing error indicators.
         */
        inline virtual void drawErrorsAfter(JKQTPEnhancedPainter& /*painter*/) {}



        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        QPointF transform(const QPointF& x);

        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        QPointF backTransform(const QPointF& x);

        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        inline QPointF transform(double x, double y) {
            return transform(QPointF(x,y));
        }
        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        inline QPointF backTransform(double x, double y) {
            return backTransform(QPointF(x,y));
        }
        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules */
        QVector<QPointF> transform(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a (non-closed) path consisting of lines */
        QPainterPath transformToLinePath(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a polygon */
        inline QPolygonF transformToPolygon(const QVector<QPointF>& x) {
            return QPolygonF(transform(x));
        }



        /** \brief title of the plot (for display in key!). If no title is supplied, no key entry is drawn. */
        QString title;

        /** \brief start of the range of plot data. -1 switches the lower data range border off. */
        long long datarange_start;
        /** \brief end of the range of plot data. -1 switches the upper data range border off. */
        long long datarange_end;

        /** \brief indicates whether the graph is visible in the plot */
        bool visible;

        /** \brief get the maximum and minimum value of the given column
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getDataMinMax(int column, double& minx, double& maxx, double& smallestGreaterZero);

        JKQTPcoordinateAxis* xAxis;
        JKQTPcoordinateAxis* yAxis;

};


/*! \brief This virtual JKQTPgraph descendent may be used as base for all graphs that use at least two columns
           that specify x and y coordinates for the single plot points.
    \ingroup jkqtplotter_plots

    This class implements basic management facilities for the data columns and implements the function
      - bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
      - bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
    .

 */
class LIB_EXPORT JKQTPxyGraph: public JKQTPgraph {
        Q_OBJECT
    public:
        enum DataSortOrder {
            Unsorted=0,
            SortedX=1,
            SortedY=2
        };


        /** \brief class constructor */
        JKQTPxyGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxyGraph(JKQtPlotter* parent);

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

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int column);

        JKQTPGET_SET_MACRO(int, xColumn)
        JKQTPGET_SET_MACRO(int, yColumn)
        JKQTPGET_SET_MACRO(DataSortOrder, sortData)
        /** \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <CENTER>\copybrief sortData.</CENTER> \see sortData for more information */
        inline void set_sortData(int __value) {
            sortData=(DataSortOrder)__value;
        }

    protected:

        /** \brief the column that contains the x-component of the datapoints */
        int xColumn;
        /** \brief the column that contains the y-component of the datapoints */
        int yColumn;

        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;
        virtual void intSortData();
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }
};


/*! \brief This virtual JKQTPgraph descendent may be used as base for all graphs that use at least one column
           of data
    \ingroup jkqtplotter_plots


 */
class LIB_EXPORT JKQTPsingleColumnGraph: public JKQTPgraph {
        Q_OBJECT
    public:
        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };
        /** \brief class constructor */
        JKQTPsingleColumnGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPsingleColumnGraph(int dataColumn, JKQtBasePlotter* parent=nullptr);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style=Qt::SolidLine, double lineWidth=2.0, JKQtBasePlotter* parent=nullptr);
        JKQTPsingleColumnGraph(JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, JKQtPlotter* parent);
        JKQTPsingleColumnGraph(int dataColumn, QColor color, JKQtPlotter* parent);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(int, dataColumn)
        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(DataSortOrder, sortData)
        /** \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <CENTER>\copybrief sortData.</CENTER> \see sortData for more information */
        inline void set_sortData(int __value) {
            sortData=(DataSortOrder)__value;
            if (__value>0) sortData=Sorted;
        }

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief the column that contains the datapoints */
        int dataColumn;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief color of the graph */
        QColor color;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;

        QPen getLinePen(JKQTPEnhancedPainter &painter) const;

        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;
        virtual void intSortData();
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }

};


/*! \brief interprets data as a stream of x- or y-positions \f$ p_i \f$ (depending in \a yPeaks ) ans plots a line on the height
           \a baseline and upright lines from baseline to baseline+peakHeight at each position \f$ p_i \f$. This can be used to
           display a stream of photons with given arrivaltimes \f$ p_i \f$.
    \ingroup jkqtplotter_plots

    \image html JKQTPPeakStreamGraphY.png "yPeaks=true"
    \image html JKQTPPeakStreamGraphX.png "yPeaks=false"
 */
class LIB_EXPORT JKQTPPeakStreamGraph: public JKQTPsingleColumnGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPPeakStreamGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQtBasePlotter* parent=nullptr);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQtBasePlotter* parent=nullptr);
         JKQTPPeakStreamGraph(JKQtPlotter* parent);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, QColor color, JKQtPlotter* parent);
        JKQTPPeakStreamGraph(int dataColumn, double baseline, double peakHeight, JKQtPlotter* parent);

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


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        JKQTPGET_SET_MACRO(double, baseline)
        JKQTPGET_SET_MACRO(double, peakHeight)
        JKQTPGET_SET_MACRO(bool, yPeaks)
        JKQTPGET_SET_MACRO(bool, drawBaseline)

    protected:

        /** \brief position of the baseline */
        double baseline;
        /** \brief height of each peak */
        double peakHeight;
        /** \brief if set \c true the peaks are in Y direction (default: true)*/
        bool yPeaks;
        /** \brief indicates whether to draw the basleine (default: \c true ) */
        bool drawBaseline;

};






/*! \brief Descendents of this class add data fields for error indicators and methods to plot them to a class.
    \ingroup jkqtplotter_plots

    This class is meant to be used with multiple inheritance. I.e. if you implemented some kind of plot
    you may derive an error plot class in which you will have to overwrite the JKQTPgraph::drawErrorsBefor()
    or drawErrorsAfter() so it calls the plotErrorIndicators() method from this class. In addition this class
    will add some public datamemebers and methods to your class that allow to specify the properties of the
    error indicators (plot properties: color, width, ... and columns for the data).

 */
class LIB_EXPORT JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPgraphErrors(QColor graphColor=QColor("black"));

        JKQTPGET_SET_MACRO(QColor, errorColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, errorStyle)
        JKQTPGET_SET_MACRO(double, errorWidth)
        JKQTPGET_SET_MACRO(QColor, errorFillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, errorFillStyle)
        JKQTPGET_SET_MACRO(double, errorbarSize)
        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c);

        void setErrorColorFromGraphColor(QColor graphColor);

    protected:
        /** \brief color of the error lines/bars */
        QColor errorColor;
        /** \brief linestyle of the error lines/bars */
        Qt::PenStyle errorStyle;
        /** \brief width (pixels) of the error lines/bars */
        double errorWidth;
        /** \brief fill color for error polygons */
        QColor errorFillColor;
        /** \brief fill style for error polygons */
        Qt::BrushStyle errorFillStyle;
        /** \brief size of the error bar end markers [pixels] */
        double errorbarSize;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. This method is called by
         *         the JKQTPgraph descendents */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis,  int xColumn, int yColumn, long long datarange_start=-1, long long datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const  QVector<int>* dataorder=nullptr)=0;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        void intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, long long datarange_start=-1, long long datarange_end=-1, int xErrorColumnLower=-1, int yErrorColumnLower=-1, bool xErrorSymmetric=true, bool yErrorSymmetric=true, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr);

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, long long index, QColor& errorColor, QColor& errorFillColor);


        virtual double getXErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getXErrorL(int i, JKQTPdatastore* ds) const;
        virtual double getYErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getYErrorL(int i, JKQTPdatastore* ds) const;

        //** \brief plot a single error indicator */
        //void intPlotXYErrorIndicator(JKQTPEnhancedPainter& painter, double x, double xperror, double xmerror, double y, double yperror, double ymerror, JKQTPerrorPlotstyle xErrorStyle=JKQTPnoError, JKQTPerrorPlotstyle yErrorStyle=JKQTPnoError, double lastx=0, double lastxperror=0, double lastxmerror=0, double lasty=0, double lastyperror=0, double lastymerror=0);
};


/*! \brief This class adds data fields for error indicators in x direction to a JKQTPgraph descendent.
    \ingroup jkqtplotter_plots
    \see JKQTPgraphErrors
 */
class LIB_EXPORT JKQTPxGraphErrors: public JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPxGraphErrors(QColor graphColor=QColor("black"));

        JKQTPGET_SET_MACRO(bool, xErrorSymmetric)
        JKQTPGET_MACRO(int, xErrorColumnLower)
        JKQTPGET_MACRO(int, xErrorColumn)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, xErrorStyle)

        inline  void set_xErrorColumn(int __value) {
            if (this->xErrorColumn != __value) { \
                this->xErrorColumn = __value; \
                if (xErrorColumn>=0 && xErrorStyle==JKQTPnoError) xErrorStyle=JKQTPerrorBars; \
            } \
        }
        inline void set_xErrorColumnLower(int __value) {
            if (this->xErrorColumnLower != __value) { \
                this->xErrorColumnLower = __value; \
                if (xErrorColumnLower>=0 && xErrorStyle==JKQTPnoError) xErrorStyle=JKQTPerrorBars; \
            } \
        }

        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c);

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle xErrorStyle;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, long long datarange_start=-1, long long datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr);

        virtual double getXErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getXErrorL(int i, JKQTPdatastore* ds) const;

};


/*! \brief This class adds data fields for error indicators in y direction to a class.
    \ingroup jkqtplotter_plots
    \see JKQTPgraphErrors

 */
class LIB_EXPORT JKQTPyGraphErrors: public JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPyGraphErrors(QColor graphColor=QColor("black"));

        JKQTPGET_SET_MACRO(bool, yErrorSymmetric)
        JKQTPGET_MACRO(int, yErrorColumnLower)
        JKQTPGET_MACRO(int, yErrorColumn)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, yErrorStyle)
        /** \copydoc JKQTPgraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c);

        inline void set_yErrorColumn(int __value) {
            if (this->yErrorColumn != __value) { \
                this->yErrorColumn = __value; \
                if (yErrorColumn>=0 && yErrorStyle==JKQTPnoError) yErrorStyle=JKQTPerrorBars; \
            } \
        }
        inline void set_yErrorColumnLower(int __value) {
            if (this->yErrorColumnLower != __value) { \
                this->yErrorColumnLower = __value; \
                if (yErrorColumnLower>=0 && yErrorStyle==JKQTPnoError) yErrorStyle=JKQTPerrorBars; \
            } \
        }
    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle yErrorStyle;
        /** \brief indicates whether the y-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief the column that contains the error of the y-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, long long datarange_start=-1, long long datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr);

        virtual double getYErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getYErrorL(int i, JKQTPdatastore* ds) const;
};


/*! \brief This class adds data fields for error indicators in x and y direction to a class.
    \ingroup jkqtplotter_plots
    \see JKQTPgraphErrors

 */
class LIB_EXPORT JKQTPxyGraphErrors: public JKQTPgraphErrors {
    public:
        /** \brief class contructor */
        JKQTPxyGraphErrors(QColor graphColor=QColor("black"));
        JKQTPGET_SET_MACRO(bool, xErrorSymmetric)
        JKQTPGET_SET_MACRO(bool, yErrorSymmetric)
        JKQTPGET_MACRO(int, xErrorColumnLower)
        JKQTPGET_MACRO(int, xErrorColumn)
        JKQTPGET_MACRO(int, yErrorColumnLower)
        JKQTPGET_MACRO(int, yErrorColumn)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, yErrorStyle)
        JKQTPGET_SET_MACRO(JKQTPerrorPlotstyle, xErrorStyle)
        /** \copydoc JKQTPgraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c);

        inline void set_xErrorColumn(int __value) {
            if (this->xErrorColumn != __value) { \
                this->xErrorColumn = __value; \
                if (xErrorColumn>=0 && xErrorStyle==JKQTPnoError) xErrorStyle=JKQTPerrorBars; \
            } \
        }
        inline void set_xErrorColumnLower(int __value) {
            if (this->xErrorColumnLower != __value) { \
                this->xErrorColumnLower = __value; \
                if (xErrorColumnLower>=0 && xErrorStyle==JKQTPnoError) xErrorStyle=JKQTPerrorBars; \
            } \
        }
        inline void set_yErrorColumn(int __value) {
            if (this->yErrorColumn != __value) { \
                this->yErrorColumn = __value; \
                if (yErrorColumn>=0 && yErrorStyle==JKQTPnoError) yErrorStyle=JKQTPerrorBars; \
            } \
        }
        inline void set_yErrorColumnLower(int __value) {
            if (this->yErrorColumnLower != __value) { \
                this->yErrorColumnLower = __value; \
                if (yErrorColumnLower>=0 && yErrorStyle==JKQTPnoError) yErrorStyle=JKQTPerrorBars; \
            } \
        }

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle yErrorStyle;
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPerrorPlotstyle xErrorStyle;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief indicates whether the y-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief the column that contains the error of the y-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, long long datarange_start=-1, long long datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr);

        virtual double getXErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getXErrorL(int i, JKQTPdatastore* ds) const;
        virtual double getYErrorU(int i, JKQTPdatastore* ds) const;
        virtual double getYErrorL(int i, JKQTPdatastore* ds) const;
};





/*! \brief This implements xy line plots. This also alows to draw symbols at the data points.
    \ingroup jkqtplotter_plots

    \image html plot_lineplots.png
 */
class LIB_EXPORT JKQTPxyLineGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPxyLineGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxyLineGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(JKQTPgraphSymbols, symbol)
        JKQTPGET_SET_MACRO(double, symbolWidth)
        JKQTPGET_SET_MACRO(double, symbolSize)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO(bool, drawSelectionLine)
        JKQTPGET_SET_MACRO(QColor, selectionLineColor)

    protected:

        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;
        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief which symbol to use for the datapoints */
        JKQTPgraphSymbols symbol;
        /** \brief size (diameter in pixels) of the symbol for the data points */
        double symbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the data points */
        double symbolWidth;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;


        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
        QPen getSymbolPen(JKQTPEnhancedPainter& painter) const;
        QBrush getBrush(JKQTPEnhancedPainter &painter) const;



};









/*! \brief This implements xy scatter plots (like JKQTPxyLineGraph), but the color and size of the symbols may be taken from a column.
    \ingroup jkqtplotter_plots

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.
 */
class LIB_EXPORT JKQTPxyParametrizedScatterGraph: public JKQTPxyLineGraph, public JKQTPColorPaletteTools {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPxyParametrizedScatterGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxyParametrizedScatterGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(int, sizeColumn)
        JKQTPGET_SET_MACRO(int, colorColumn)
        JKQTPGET_SET_MACRO(int, symbolColumn)
        JKQTPGET_SET_MACRO(bool, colorColumnContainsRGB)
        JKQTPGET_SET_MACRO(bool, gridModeForSymbolSize)
        JKQTPGET_SET_MACRO(double, gridDeltaX)
        JKQTPGET_SET_MACRO(double, gridDeltaY)
        JKQTPGET_SET_MACRO(double, gridSymbolFractionSize)

        virtual void setParent(JKQtBasePlotter* parent);


        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);


        /** \brief determine min/max data value of the image */
        virtual void cbGetDataMinMax(double& imin, double& imax);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);


        virtual double getInternalDataMin() const ;
        virtual double getInternalDataMax() const ;
    protected:
        /** \brief this column contains the symbol size in pt */
        int sizeColumn;
        /** \brief this column contains the symbol color */
        int colorColumn;
        /** \brief this column contains the symbol type */
        int symbolColumn;

        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        bool gridModeForSymbolSize;
        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridDeltaX;
        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridDeltaY;
        /** \brief if the gridModeForSymbolSize mode is actiavted (false), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridSymbolFractionSize;

        /** \brief if this is true, the value in the colorColumn is converted to an integer, representing a color in ARGB format (as in QRgb) */
        bool colorColumnContainsRGB;

        double getLocalSymbolSize(int i);
        QColor getLocalColor(int i);
        JKQTPgraphSymbols getLocalSymbolType(int i);


        /** \brief internally used to store the range of the color column */
        double intColMin;
        /** \brief internally used to store the range of the color column */
        double intColMax;

};




/*! \brief This implements xy line plots with x and y error indicators.
    \ingroup jkqtplotter_plots

    \image html plot_errorbarlineplots.png
    \image html plot_errorlinelineplots.png
    \image html plot_errorpolygonlineplots.png
 */
class LIB_EXPORT JKQTPxyLineErrorGraph: public JKQTPxyLineGraph, public JKQTPxyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPxyLineErrorGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPxyLineGraph(parent), JKQTPxyGraphErrors()
        { setErrorColorFromGraphColor(color); }
        /** \brief class constructor */
        inline JKQTPxyLineErrorGraph(JKQtPlotter* parent):
            JKQTPxyLineGraph(parent), JKQTPxyGraphErrors()
        { setErrorColorFromGraphColor(color);  }

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
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        inline virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter) {
            intSortData();
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);
        };
};



/*! \brief This implements xy scatter plots (like JKQTPxyLineGraph), but the color and size of the symbols may be taken from a column. with errorbars
    \ingroup jkqtplotter_plots

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.
 */
class LIB_EXPORT JKQTPxyParametrizedErrorScatterGraph: public JKQTPxyParametrizedScatterGraph, public JKQTPxyGraphErrors {
        Q_OBJECT
    public:
        inline JKQTPxyParametrizedErrorScatterGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPxyParametrizedScatterGraph(parent), JKQTPxyGraphErrors()
        {  setErrorColorFromGraphColor(color); }
        inline JKQTPxyParametrizedErrorScatterGraph(JKQtPlotter* parent):
            JKQTPxyParametrizedScatterGraph(parent), JKQTPxyGraphErrors()
        { setErrorColorFromGraphColor(color);  }


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
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        inline virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter) {
            intSortData();
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);
        }

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPcoordinateAxis* xAxis, JKQTPcoordinateAxis* yAxis, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, long long index, QColor& errorColor, QColor& errorFillColor);

};


/*! \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from x=0 to x=f(y) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesxplots.png
 */
class LIB_EXPORT JKQTPimpulsesHorizontalGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPimpulsesHorizontalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPimpulsesHorizontalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, baseline)

        /** \brief color of the graph */
        QColor color;
        /** \brief width (pixels) of the lines */
        double lineWidth;

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!)
         */
        double baseline;
        QPen getPen(JKQTPEnhancedPainter &painter) const;
};



/*! \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from x=0 to x=f(y) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesxerrorsplots.png
 */
class LIB_EXPORT JKQTPimpulsesHorizontalErrorGraph: public JKQTPimpulsesHorizontalGraph, public JKQTPxGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPimpulsesHorizontalErrorGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPimpulsesHorizontalGraph(parent), JKQTPxGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        inline JKQTPimpulsesHorizontalErrorGraph(JKQtPlotter* parent):
            JKQTPimpulsesHorizontalGraph(parent), JKQTPxGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        inline virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) {
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

            //plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
        }

};




/*! \brief This implements an impulse plot with impulses in direction of the Y axis (i.e. from y=0 to y=f(x) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesyplots.png
 */
class LIB_EXPORT JKQTPimpulsesVerticalGraph: public JKQTPimpulsesHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPimpulsesVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPimpulsesVerticalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
};



/*! \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from x=0 to x=f(y) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesyerrorsplots.png
 */
class LIB_EXPORT JKQTPimpulsesVerticalErrorGraph: public JKQTPimpulsesVerticalGraph, public JKQTPyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPimpulsesVerticalErrorGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPimpulsesVerticalGraph(parent), JKQTPyGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        /** \brief class constructor */
        inline JKQTPimpulsesVerticalErrorGraph(JKQtPlotter* parent):
            JKQTPimpulsesVerticalGraph(parent), JKQTPyGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        inline virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) {
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

            //plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
        }

};







/*! \brief This implements filled curve plots where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_plots

    \image html plot_filledcurvexplots.png
 */
class LIB_EXPORT JKQTPfilledCurveXGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPfilledCurveXGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPfilledCurveXGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, baseline)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)

        JKQTPGET_SET_MACRO(bool, drawSelectionLine)
        JKQTPGET_SET_MACRO(QColor, selectionLineColor)

    protected:

        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief fill styl for the curve */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!)
         */
        double baseline;
        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;
};



/*! \brief This implements filled curve plots with y errors where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_plots

    \image html plot_filledcurvexerrorplots.png
 */
class LIB_EXPORT JKQTPfilledCurveXErrorGraph: public JKQTPfilledCurveXGraph, public JKQTPyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPfilledCurveXErrorGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPfilledCurveXGraph(parent), JKQTPyGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        /** \brief class constructor */
        inline JKQTPfilledCurveXErrorGraph(JKQtPlotter* parent):
            JKQTPfilledCurveXGraph(parent), JKQTPyGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) {
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

            //plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
        };

};






/*! \brief This implements filled curve plots where the area is filled between the plot line and y-Axis
    \ingroup jkqtplotter_plots

    \image html plot_filledcurveyplots.png
 */
class LIB_EXPORT JKQTPfilledCurveYGraph: public JKQTPfilledCurveXGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPfilledCurveYGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPfilledCurveYGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
};



/*! \brief This implements filled curve plots with y errors where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_plots

    \image html plot_filledcurveyerrorplots.png
 */
class LIB_EXPORT JKQTPfilledCurveYErrorGraph: public JKQTPfilledCurveYGraph, public JKQTPxGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPfilledCurveYErrorGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPfilledCurveYGraph(parent), JKQTPxGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        inline JKQTPfilledCurveYErrorGraph(JKQtPlotter* parent):
            JKQTPfilledCurveYGraph(parent), JKQTPxGraphErrors()
        {
            setErrorColorFromGraphColor(color);
        }
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        inline virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) {
            //plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, 0, &sortedIndices);

        }

};




/*! \brief This implements vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplotvertical.png
 */
class LIB_EXPORT JKQTPboxplotVerticalGraph: public JKQTPgraph {
        Q_OBJECT
    public:

        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };



        /** \brief class constructor */
        JKQTPboxplotVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPboxplotVerticalGraph(JKQtPlotter* parent);

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

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

        JKQTPGET_SET_MACRO(DataSortOrder, sortData)
        /** \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <CENTER>\copybrief sortData.</CENTER> \see sortData for more information */
        inline void set_sortData(int __value) {
            sortData=(DataSortOrder)__value;
            if (__value>0) sortData=Sorted;
        }
        JKQTPGET_SET_MACRO(int, posColumn)
        JKQTPGET_SET_MACRO(int, medianColumn)
        JKQTPGET_SET_MACRO(int, meanColumn)
        JKQTPGET_SET_MACRO(int, minColumn)
        JKQTPGET_SET_MACRO(int, maxColumn)
        JKQTPGET_SET_MACRO(int, percentile25Column)
        JKQTPGET_SET_MACRO(int, percentile75Column)
        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, whiskerStyle)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, boxWidth)

        JKQTPGET_SET_MACRO(JKQTPgraphSymbols, meanSymbol)
        JKQTPGET_SET_MACRO(double, meanSymbolWidth)
        JKQTPGET_SET_MACRO(double, meanSymbolSize)

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief the column that contains the x-component of the datapoints */
        int posColumn;
        /** \brief the column that contains the median-component of the datapoints */
        int medianColumn;
        /** \brief the column that contains the median-component of the datapoints. \note This column is strictly optional. */
        int meanColumn;
        /** \brief the column that contains the minimum-component of the datapoints */
        int minColumn;
        /** \brief the column that contains the maximum-component of the datapoints */
        int maxColumn;
        /** \brief the column that contains the 25% percentile-component of the datapoints */
        int percentile25Column;
        /** \brief the column that contains the 75% percentile-component of the datapoints */
        int percentile75Column;
        /** \brief color of the lines */
        QColor color;
        /** \brief color of the box fill */
        QColor fillColor;
        /** \brief fill style for the box */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the whisker lines */
        Qt::PenStyle whiskerStyle;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width of box in percent of distance between the current two posColumn values
         *         if we only plot one box&whiskers then this is the width in plot coordinates */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPgraphSymbols meanSymbol;
        /** \brief size (diameter in pixels) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the mean */
        double meanSymbolWidth;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;
        virtual void intSortData();
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }

};


/*! \brief This implements horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplothorizontal.png
 */
class LIB_EXPORT JKQTPboxplotHorizontalGraph: public JKQTPboxplotVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPboxplotHorizontalGraph(JKQtBasePlotter* parent=nullptr):
            JKQTPboxplotVerticalGraph(parent)
        {
        }
        inline JKQTPboxplotHorizontalGraph(JKQtPlotter* parent):
            JKQTPboxplotVerticalGraph(parent)
        {
        }

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);


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
};








/*! \brief This implements a vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPboxplotVerticalGraph
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplotvertical.png
 */
class LIB_EXPORT JKQTPboxplotVerticalElement: public JKQTPgraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPboxplotVerticalElement(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPboxplotVerticalElement(JKQtPlotter* parent);

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


        JKQTPGET_SET_MACRO(double, pos)
        JKQTPGET_SET_MACRO_I(double, median, drawMedian=true)
        JKQTPGET_SET_MACRO_I(double, mean, drawMean=true)
        JKQTPGET_SET_MACRO_I(double, min, drawMinMax=true)
        JKQTPGET_SET_MACRO_I(double, max, drawMinMax=true)
        JKQTPGET_SET_MACRO(double, percentile25)
        JKQTPGET_SET_MACRO(double, percentile75)
        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, whiskerStyle)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, boxWidth)

        JKQTPGET_SET_MACRO(JKQTPgraphSymbols, meanSymbol)
        JKQTPGET_SET_MACRO(double, meanSymbolWidth)
        JKQTPGET_SET_MACRO(double, meanSymbolSize)
        JKQTPGET_SET_MACRO(bool, drawMean)
        JKQTPGET_SET_MACRO(bool, drawMedian)
        JKQTPGET_SET_MACRO(bool, drawMinMax)

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief the column that contains the x-component of the datapoints */
        double pos;
        /** \brief the column that contains the median-component of the datapoints */
        double median;
        /** \brief the column that contains the median-component of the datapoints. \note This column is strictly optional. */
        double mean;
        /** \brief indicates whether to draw the mean */
        bool drawMean;
        /** \brief indicates whether to draw the median */
        bool drawMedian;
        /** \brief indicates whether to draw the percentiles */
        bool drawMinMax;
        /** \brief the column that contains the minimum-component of the datapoints */
        double min;
        /** \brief the column that contains the maximum-component of the datapoints */
        double max;
        /** \brief the column that contains the 25% percentile-component of the datapoints */
        double percentile25;
        /** \brief the column that contains the 75% percentile-component of the datapoints */
        double percentile75;
        /** \brief color of the lines */
        QColor color;
        /** \brief color of the box fill */
        QColor fillColor;
        /** \brief fill style for the box */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the whisker lines */
        Qt::PenStyle whiskerStyle;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width of box in plot coordinates */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPgraphSymbols meanSymbol;
        /** \brief size (diameter in pixels) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the mean */
        double meanSymbolWidth;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;
};


/*! \brief This implements a horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPboxplotVerticalGraph
    \ingroup jkqtplotter_plots

    the x position is given in pos. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplothorizontal.png
 */
class LIB_EXPORT JKQTPboxplotHorizontalElement: public JKQTPboxplotVerticalElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPboxplotHorizontalElement(JKQtBasePlotter* parent=nullptr):
            JKQTPboxplotVerticalElement(parent)
        {
        }
        /** \brief class constructor */
        inline JKQTPboxplotHorizontalElement(JKQtPlotter* parent):
            JKQTPboxplotVerticalElement(parent)
        {
        }

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);


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
};






/*! \brief type of functions that may be plottet
    \ingroup jkqtplotter_plots

    This is the type of functions \f$ y=f(x, \vec{p}) \f$ that may be plottet by JKQTPxFunctionLineGraph
    and JKQTPyFunctionLineGraph. It is possible to supply parameters \f$ \vec{p} \f$ to the function that
    influence its result. Parameters are given as a pointer to some memory location. The function has to
    know on its own how to interpret these.
*/
typedef double(*jkqtpPlotFunctionType)(double, void*);


/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$
    \ingroup jkqtplotter_plots

    This class implements an intelligent plotting algorithm for functions. It starts by sampling
    the function at minSamples positions. Then each function interval is bisected recursively if
    necessary. To do so the function is evaluated at the mid point and the slopes \f$ \alpha_{\mbox{left}} \f$
    and \f$ \alpha_{\mbox{right}} \f$ of the two linear segments are compared. the midpoint is added
    to the graph if \f[ \left|\alpha_{\mbox{right}}-\alpha_{\mbox{left}}\right|>\mbox{slopeTolerance} \f]
    In addition all sampling points except minimum and maximum are beeing shifted by a random fraction their
    distance to the other points. This helps to prevent beats when sampling periodic functions.

    the following image
    \image html plot_functionplots.png
 */
class LIB_EXPORT JKQTPxFunctionLineGraph: public JKQTPgraph {
        Q_OBJECT
    public:

        enum SpecialFunction {
            Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x+p_2x^2+p_3x^3+... \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Line=Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Exponential,  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot\exp(x/p_2) \f$ or \f$ f(x)=p_0\cdot\exp(x/p_1) \f$ (depending on the number of parameters). The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            PowerLaw  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot x^{p_3} \f$ or \f$ f(x)=p_0\cdot x^{p_1} \f$ or \f$ f(x)= x^{p_0} \f$ (depending on the number of parameters) The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
        };

        /** \brief class constructor */
        JKQTPxFunctionLineGraph(JKQtBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPxFunctionLineGraph(JKQtPlotter* parent);

        /** \brief class destructor */
        virtual ~JKQTPxFunctionLineGraph();

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        /** \brief get the maximum and minimum x-value of the graph
         *
         * This functions returns 0 for both parameters, so that the plotter uses the predefined
         * min and max values.
         */
        inline virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) { smallestGreaterZero=minx=maxx=0; return false; }
        /** \brief get the maximum and minimum y-value of the graph
         */
        inline virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) { smallestGreaterZero=miny=maxy=0; return false; }

        /** \brief clear the data sampled from the function. */
        void clearData();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO_I(jkqtpPlotFunctionType, plotFunction, clearData())
        JKQTPGET_SET_MACRO_I(void*, params, clearData())
        /** \brief sets the params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        void set_params(const QVector<double>& params);
        /** \brief sets the params from a copy of the given array of length \a N */
        void set_copiedParams(const double* params, int N);
        inline void set_paramsV(double p1) {
            QVector<double> p;
            p<<p1;
            set_params(p);
        }
        inline void set_paramsV(double p1, double p2) {
            QVector<double> p;
            p<<p1<<p2;
            set_params(p);
        }
        inline void set_paramsV(double p1, double p2, double p3) {
            QVector<double> p;
            p<<p1<<p2<<p3;
            set_params(p);
        }
        inline void set_paramsV(double p1, double p2, double p3, double p4) {
            QVector<double> p;
            p<<p1<<p2<<p3<<p4;
            set_params(p);
        }
        inline void set_paramsV(double p1, double p2, double p3, double p4, double p5) {
            QVector<double> p;
            p<<p1<<p2<<p3<<p4<<p5;
            set_params(p);
        }

        QVector<double> get_internalParams() const;
        QVector<double> get_internalErrorParams() const;
        JKQTPGET_SET_MACRO(unsigned int, minSamples)
        JKQTPGET_SET_MACRO(unsigned int, maxRefinementDegree)
        JKQTPGET_SET_MACRO(double, slopeTolerance)
        JKQTPGET_SET_MACRO(double, minPixelPerSample)
        JKQTPGET_SET_MACRO(bool, plotRefinement)
        JKQTPGET_SET_MACRO(bool, displaySamplePoints)
        JKQTPGET_SET_MACRO(bool, drawErrorPolygons)
        JKQTPGET_SET_MACRO(bool, drawErrorLines)
        JKQTPGET_SET_MACRO(jkqtpPlotFunctionType, errorPlotFunction)
        JKQTPGET_SET_MACRO(void*, errorParams)
        /** \brief sets the error params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        void set_errorParams(const QVector<double>& errorParams);

        JKQTPGET_SET_MACRO(int, parameterColumn)
        JKQTPGET_SET_MACRO(int, errorParameterColumn)

        JKQTPGET_SET_MACRO(QColor, errorColor)
        JKQTPGET_SET_MACRO(QColor, errorFillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, errorFillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, errorStyle)
        JKQTPGET_SET_MACRO(double, errorLineWidth)

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);


        /** \brief sets function to the given special function */
        void setSpecialFunction(SpecialFunction function);
    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        struct doublePair {
            double x;
            double f;
            doublePair* next;
        };
        /** \brief a linked list holding the datapoints \f$ \left(x, y=f(x, \vec{p})\right) \f$ to be plotted */
        doublePair* data;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true);

        virtual void collectParameters();

        void refine(doublePair* a, doublePair* b, unsigned int degree=0);

        int parameterColumn;
        int errorParameterColumn;

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
        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief indicates whether to fill the space between the curve and the x-axis */
        bool fillCurve;
        /** \brief the function to be plotted */
        jkqtpPlotFunctionType plotFunction;
        /** \brief pointer to the parameters supplied to the plotting funtion */
        void* params;
        /** \brief the minimum number of points to evaluate the function at */
        unsigned int minSamples;
        /** \brief the maximum number of recursive refinement steps
         *
         * each step bisects the interval \f$ [a, b] \f$ into two halfes. So the maximum number
         * of points plotted at all are thus:
         *  \f[ \mbox{minSamples} \cdot 2^{\mbox{maxRefinementDegree}} \f]
         */
        unsigned int maxRefinementDegree;
        /** \brief the tolerance for the difference of two subsequent slopes */
        double slopeTolerance;
        /** \brief create one sample at least every \a minPixelPerSample pixels */
        double minPixelPerSample;
        /** \brief switch on or off [default: on] the plot refinement algorithm */
        bool plotRefinement;
        /** \brief if true [default: off] display the points where the function has been sampled */
        bool displaySamplePoints;
        /** \brief indicates whether an error polygon should be drawn */
        bool drawErrorPolygons;
        /** \brief indicates whether error lines should be drawn */
        bool drawErrorLines;
        /** \brief this function calculates the error at a given position */
        jkqtpPlotFunctionType errorPlotFunction;
        /** \brief parameters for errorFunction */
        void* errorParams;
        /** \brief color of the error graph */
        QColor errorColor;
        /** \brief color of the error graph fill */
        QColor errorFillColor;
        /** \brief linestyle of the error graph lines */
        Qt::PenStyle errorStyle;
        /** \brief width (pixels) of the error graph */
        double errorLineWidth;
        /** \brief fill style, if the error curve should be filled */
        Qt::BrushStyle errorFillStyle;


        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;

        QBrush getErrorBrush(JKQTPEnhancedPainter& painter) const;
        QPen getErrorLinePen(JKQTPEnhancedPainter &painter) const;

        QVector<double> iparams, ierrorparams;
};

/*! \brief This implements line plots where the data is taken from a user supplied function \f$ x=f(y) \f$
    \ingroup jkqtplotter_plots

 */
class LIB_EXPORT JKQTPyFunctionLineGraph: public JKQTPxFunctionLineGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPyFunctionLineGraph(JKQtBasePlotter* parent=nullptr):JKQTPxFunctionLineGraph(parent) {}
        /** \brief class constructor */
        inline JKQTPyFunctionLineGraph(JKQtPlotter* parent):JKQTPxFunctionLineGraph(parent) {}

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
    protected:

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true);

};







/*! \brief This implements a step plot with values \f$ \left(x, f(x) \right) \f$
    \ingroup jkqtplotter_plots

    A step plot starts at \f$ \left(x_{i-1}, f(x_{i-1})\right) \f$ and then goes on to
    \f$ \left(x_{i}, f(x_{i-1})\right) \f$. There it raises immediately to
    \f$ \left(x_i, f(x_i)\right) \f$.

    If you want the \f$ x_i \f$ values in the center of the steps, use
    \code JKQTPstepHorizontalGraph::set_xCentered(true) \endcode . In that case the steps
    go from \f$ \left(x_{i-1}, f(x_{i-1})\right) \f$ to \f$ \left(x_{i}-\delta/2, f(x_{i-1})\right) \f$ ,
    then to \f$ \left(x_{i}-\delta/2, f(x_{i})\right) \f$ and finally
    to \f$ \left(x_{i}, f(x_{i})\right) \f$ where \f$ \delta=(x_{i}-x_{i-1}) \f$ .

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_valuesCentered(false) \endcode :
    \image html plot_stephorplot1.png

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_valuesCentered(true) \endcode :
    \image html plot_stephorplot2.png
 */
class LIB_EXPORT JKQTPstepHorizontalGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPstepHorizontalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPstepHorizontalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO(bool, fillCurve)
        JKQTPGET_SET_MACRO(bool, valuesCentered)
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
        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief indicates whether to fill the space between the curve and the x-axis */
        bool fillCurve;
        /** \brief The steps are centered around the \f$ x_i \f$ values, if this is true. */
        bool valuesCentered;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
};



/*! \brief This implements a step plot with values \f$ \left(f(y), y \right) \f$
    \ingroup jkqtplotter_plots

    A step plot starts at \f$ \left(f(y_{i-1}), x_{i-1}\right) \f$ and then goes on to
    \f$ \left(f(y_{i-1}), y_{i}\right) \f$. There it raises immediately to
    \f$ \left(f(y_i), y_i\right) \f$.

    If you want the \f$ y_i \f$ values in the center of the steps, use
    \code JKQTPstepHorizontalGraph::set_valuesCentered(true) \endcode . In that case the steps
    go from \f$ \left(f(y_{i-1}), y_{i-1})\right) \f$ to \f$ \left(f(y_{i-1}), y_{i}-\delta/2\right) \f$ ,
    then to \f$ \left(f(y_{i}, y_{i}-\delta/2\right) \f$ and finally
    to \f$ \left(f(y_{i}), y_{i}\right) \f$ where \f$ \delta=(y_{i}-y_{i-1}) \f$ .

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_xCentered(false) \endcode :
    \image html plot_stepverplot1.png

    This diagram shows a plot with \code JKQTPstepHorizontalGraph::set_xCentered(true) \endcode :
    \image html plot_stepverplot2.png
 */
class LIB_EXPORT JKQTPstepVerticalGraph: public JKQTPstepHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPstepVerticalGraph(JKQtBasePlotter* parent=nullptr): JKQTPstepHorizontalGraph(parent) {}
        /** \brief class constructor */
        inline JKQTPstepVerticalGraph(JKQtPlotter* parent): JKQTPstepHorizontalGraph(parent) {}

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
};









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

    \image html plot_bargraphhorplot.png


    You can use JKQTPlotter::addHorizontalBargraph() to add a series of bargraphs, where the width and shift are determined
    automatically. The y-columns are given as a QVector<int> to this function.

 */
class LIB_EXPORT JKQTPbarHorizontalGraph: public JKQTPxyGraph, public JKQTPxyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPbarHorizontalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPbarHorizontalGraph(JKQtPlotter* parent);

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
        inline void autoscaleBarWidthAndShiftSeparatedGroups(double groupWidth=0.75) {
            autoscaleBarWidthAndShift(groupWidth, 1);
        }

        virtual bool isHorizontal() const;

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, shift)
        JKQTPGET_SET_MACRO(double, width)
        JKQTPGET_SET_MACRO(double, baseline)
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
    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        inline virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) {
            //plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, shift, 0.0);
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, shift, 0.0);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, shift, 0, &sortedIndices);

        }


};






/*! \brief This implements a bar graph with bars starting at \f$ x=0 \f$ to \f$ x=f(y) \f$
    \ingroup jkqtplotter_plots

    This works much the same as JKQTPbarHorizontalGraph. Here is an example output:
    \image html plot_bargraphverplot.png

 */
class LIB_EXPORT JKQTPbarVerticalGraph: public JKQTPbarHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        inline JKQTPbarVerticalGraph(JKQtBasePlotter* parent=nullptr): JKQTPbarHorizontalGraph(parent) {}
        /** \brief class constructor */
        inline JKQTPbarVerticalGraph(JKQtPlotter* parent): JKQTPbarHorizontalGraph(parent) {}

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

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) {
            //plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0.0, shift);
            if (sortData==JKQTPxyLineGraph::Unsorted) plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0.0, shift);
            else plotErrorIndicators(painter, parent, xAxis, yAxis, xColumn, yColumn, datarange_start, datarange_end, 0, shift, &sortedIndices);

        };
};









/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_plots

 */
class LIB_EXPORT JKQTPhorizontalRange: public JKQTPgraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPhorizontalRange(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPhorizontalRange(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        inline virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero){ smallestGreaterZero=minx=maxx=0;  return false; }
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        inline virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
            miny=rangeMin;
            maxy=rangeMax;
            smallestGreaterZero=0;
            if (rangeMin>0) smallestGreaterZero=rangeMin;
             return true;
        }
        /** \brief returns the color to be used for the key label */
        inline virtual QColor getKeyLabelColor() { return color; }

        void setDrawCenterLineOnly();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(QColor, centerColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, centerStyle)
        JKQTPGET_SET_MACRO(double, centerLineWidth)
        JKQTPGET_SET_MACRO(double, rangeMin)
        JKQTPGET_SET_MACRO(double, rangeMax)
        JKQTPGET_SET_MACRO(double, sizeMin)
        JKQTPGET_SET_MACRO(double, sizeMax)
        JKQTPGET_SET_MACRO(bool, unlimitedSizeMin)
        JKQTPGET_SET_MACRO(bool, unlimitedSizeMax)

        JKQTPGET_SET_MACRO(double, rangeCenter)
        JKQTPGET_SET_MACRO(bool, plotCenterLine)
        JKQTPGET_SET_MACRO(bool, invertedRange)
        JKQTPGET_SET_MACRO(bool, plotRange)
        JKQTPGET_SET_MACRO(bool, fillRange)
        JKQTPGET_SET_MACRO(bool, plotRangeLines)
    protected:
        /** \brief min-value of range */
        double rangeMin;
        /** \brief max-value of range */
        double rangeMax;
        /** \brief where to plot the center line */
        double rangeCenter;
        double sizeMin;
        double sizeMax;
        bool unlimitedSizeMin;
        bool unlimitedSizeMax;
        /** \brief indicates whether to plot a center line */
        bool plotCenterLine;
        /** \brief if \c true, fills the parts outside the given range */
        bool invertedRange;
        /** \brief if \c true, the range is plotted, if \c false, only the center line is plotted (if \a plotCenterLine is \c true ) */
        bool plotRange;
        /** \brief if \c true, draws lines at the range borders \a color */
        bool plotRangeLines;
        /** \brief if \c true, this fills the range with \a fillCOlor */
        bool fillRange;
        /** \brief color of the graph */
        QColor color;
        /** \brief color of the center line graph */
        QColor centerColor;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief linestyle of the center line graph lines */
        Qt::PenStyle centerStyle;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width (pixels) of the center line graph */
        double centerLineWidth;
        /** \brief fill style, if the curve should be filled */
        Qt::BrushStyle fillStyle;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;

};






/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_plots

 */
class LIB_EXPORT JKQTPverticalRange: public JKQTPhorizontalRange {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPverticalRange(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPverticalRange(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        inline virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero){
            minx=rangeMin;
            maxx=rangeMax;
            smallestGreaterZero=0;
            if (rangeMin>0) smallestGreaterZero=rangeMin;
             return true;

        }
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        inline virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) { smallestGreaterZero=miny=maxy=0; return false; }
};

#endif // JKQTPELEMENTS_H
