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


/** \file jkqtpgraphsbase.h
  * \ingroup jkqtplotter_basegraphs
  */

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"

#ifndef jkqtpgraphsbase_H
#define jkqtpgraphsbase_H

// forward declarations
class JKQTPlotter;
class JKQTPDatastore;

/** \brief this virtual base class of every element, which is part of a JKQTPlotter plot and may appear in its key
 *         (basically any type of graph, except overlay elements!)
 * \ingroup jkqtplotter_basegraphs
 *
 * Each possible graph is represented by a child of this class. So additional plots may be created by
 * deriving new JKQTPGraph classes. To do so implement/overwrite these functions:
 *   - void draw(JKQTPEnhancedPainter& painter);
 *   - void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
 *   - bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
 *   - void getGraphsYMinMax(double& miny, double& maxy);
 *   - QColor getKeyLabelColor()=0;
 *   - bool usesColumn(int column) const;
 * .
 *
 * Optionally you may also overwrite these functions to draw elements outside the actual plot area (like e.g. colorbars):
 *   - void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);
 *   - void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);
 * .
 *
 */
class LIB_EXPORT JKQTPPlotElement: public QObject {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPPlotElement(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPPlotElement(JKQTPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPPlotElement() ;

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

        /*! \brief sets the property title to the specified \a __value.
            \details Description of the parameter title is: <BLOCKQUOTE>\copydoc title </BLOCKQUOTE>
            \see title for more information */
        inline virtual void set_title(const QString & __value)
        {
            this->title = __value;
        }
        /*! \brief returns the property title.
            \details Description of the parameter title is: <BLOCKQUOTE>\copydoc title </BLOCKQUOTE>
            \see title for more information */
        inline virtual QString get_title() const
        {
            return this->title;
        }
        /*! \brief sets the property visible to the specified \a __value.
            \details Description of the parameter visible is: <BLOCKQUOTE>\copydoc visible </BLOCKQUOTE>
            \see visible for more information */
        inline virtual void set_visible(bool __value)
        {
            this->visible = __value;
        }
        /*! \brief returns the property visible.
            \details Description of the parameter visible is: <BLOCKQUOTE>\copydoc visible </BLOCKQUOTE> 
            \see visible for more information */ 
        inline virtual bool get_visible() const  
        {
            return this->visible; 
        }

        /** \brief returns the parent painter class */
        inline JKQTBasePlotter* getParent() { return parent; }
        /** \brief sets the parent painter class */
        virtual void setParent(JKQTBasePlotter* parent);
        /** \brief sets the parent painter class */
        virtual void setParent(JKQTPlotter* parent);

        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);

    protected:
        /** \brief the plotter object this object belongs to */
        JKQTBasePlotter* parent;

        /** \brief title of the plot (for display in key!). If no title is supplied, no key entry is drawn. */
        QString title;

        /** \brief indicates whether the graph is visible in the plot */
        bool visible;



        /** \brief tool routine that transforms an x-coordinate for this plot element */
        virtual double transformX(double x) const;

        /** \brief tool routine that transforms a y-coordinate for this plot element */
        virtual double transformY(double y) const;

        /** \brief tool routine that backtransforms an x-coordinate for this plot element */
        virtual double backtransformX(double x) const;

        /** \brief tool routine that backtransforms a y-coordinate for this plot element */
        virtual double backtransformY(double y) const;


        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        inline QPointF transform(const QPointF& x) {
            return QPointF(transformX(x.x()), transformY(x.y()));
        }

        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        inline QPointF backTransform(const QPointF& x) {
            return QPointF(backtransformX(x.x()), backtransformY(x.y()));
        }

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
};

/** \brief this virtual base class of the (data-column based) graphs,
 *         which are part of a JKQTPlotter plot and which use the coordinate system
 *         of the JKQTPlotter (i.e. the two coordinate axes getXAxis() and getYAxis())
 *         as basis for the graphs
 * \ingroup jkqtplotter_basegraphs
 *
 * This class adds several features to work with data columns. In addition this class adds protected
 * functions that do coordinate transforms based on the current coordinate system, of the paren
 * JKQTPlotter (i.e. using the axes JKQTPLott:xAxis and JKQTPlotter::yAxis as basis for the plotting).
 *
 * There are two properties datarange_start and datarange_end. By default they are -1 and therefore ignored.
 * if they are != -1 the plotter only displays the datapoints with the indexes [datarange_start .. datarange_end]
 * although there might be more data points available (range [0 .. maxDataPoints]). The datarange is cut at the
 * full range, i.e. if datarange_end>maxDataPoints the plotter displays [datarange_start .. maxDataPoints].
 */
class LIB_EXPORT JKQTPGraph: public JKQTPPlotElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPGraph(JKQTPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPGraph() ;


        /*! \brief sets the property datarange_start to the specified \a __value. 
            \details Description of the parameter datarange_start is: <BLOCKQUOTE>\copydoc datarange_start </BLOCKQUOTE> 
            \see datarange_start for more information */ 
        inline virtual void set_datarange_start(int __value)
        {
            this->datarange_start = __value;
        } 
        /*! \brief returns the property datarange_start. 
            \details Description of the parameter datarange_start is: <BLOCKQUOTE>\copydoc datarange_start </BLOCKQUOTE> 
            \see datarange_start for more information */ 
        inline virtual int get_datarange_start() const  
        {
            return this->datarange_start; 
        }
        /*! \brief sets the property datarange_end to the specified \a __value. 
            \details Description of the parameter datarange_end is: <BLOCKQUOTE>\copydoc datarange_end </BLOCKQUOTE> 
            \see datarange_end for more information */ 
        inline virtual void set_datarange_end(int __value)
        {
            this->datarange_end = __value;
        } 
        /*! \brief returns the property datarange_end. 
            \details Description of the parameter datarange_end is: <BLOCKQUOTE>\copydoc datarange_end </BLOCKQUOTE> 
            \see datarange_end for more information */ 
        inline virtual int get_datarange_end() const  
        {
            return this->datarange_end; 
        }

        /** \brief returns \c true if the given column is used by the graph */
        virtual bool usesColumn(int column) const;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs.
         *
         * By default this function does nothing. But children of this class may overwrite it to implement
         * drawing error indicators.
         */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& /*painter*/);
        /** \brief this function is used to plot error inidcators after plotting the graphs.
         *
         * By default this function does nothing. But children of this class may overwrite it to implement
         * drawing error indicators.
         */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& /*painter*/);





        /** \brief start of the range of plot data. -1 switches the lower data range border off. */
        int datarange_start;
        /** \brief end of the range of plot data. -1 switches the upper data range border off. */
        int datarange_end;

        /** \brief get the maximum and minimum value of the given column
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getDataMinMax(int column, double& minx, double& maxx, double& smallestGreaterZero);

        friend class JKQTPGraphErrors;

};


/** \brief this is the virtual base class of all JKQTPPlotElement's in a JKQTPlotter plot that
 *         represent geometric forms or annotations. They have extended coordinate transform capabilities, because
 *         in addition to using the plot coordinates, you can also choose to use different other
 *         coordinate systems
 * \ingroup jkqtplotter_basegraphs
 *
 */
class LIB_EXPORT JKQTPPlotObject: public JKQTPPlotElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPPlotObject(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPPlotObject(JKQTPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPPlotObject() ;


    protected:


};



/*! \brief This virtual JKQTPGraph descendent may be used as base for all graphs that use at least two columns
           that specify x and y coordinates for the single plot points.
    \ingroup jkqtplotter_basegraphs

    This class implements basic management facilities for the data columns and implements the function
      - bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
      - bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
    .

 */
class LIB_EXPORT JKQTPXYGraph: public JKQTPGraph {
        Q_OBJECT
    public:
        enum DataSortOrder {
            Unsorted=0,
            SortedX=1,
            SortedY=2
        };


        /** \brief class constructor */
        JKQTPXYGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYGraph(JKQTPlotter* parent);

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

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int column) const override;

        /*! \brief sets the property xColumn to the specified \a __value. 
            \details Description of the parameter xColumn is: <BLOCKQUOTE>\copydoc xColumn </BLOCKQUOTE> 
            \see xColumn for more information */ 
        inline virtual void set_xColumn(int __value)
        {
            this->xColumn = __value;
        } 
        /*! \brief returns the property xColumn. 
            \details Description of the parameter xColumn is: <BLOCKQUOTE>\copydoc xColumn </BLOCKQUOTE> 
            \see xColumn for more information */ 
        inline virtual int get_xColumn() const  
        {
            return this->xColumn; 
        }
        /*! \brief sets the property xColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter xColumn is:  <BLOCKQUOTE>\copydoc xColumn </BLOCKQUOTE> 
        	\see xColumn for more information */ 
        inline virtual void set_xColumn (size_t __value) { this->xColumn = static_cast<int>(__value); }
        /*! \brief sets the property yColumn to the specified \a __value. 
            \details Description of the parameter yColumn is: <BLOCKQUOTE>\copydoc yColumn </BLOCKQUOTE> 
            \see yColumn for more information */ 
        inline virtual void set_yColumn(int __value)
        {
            this->yColumn = __value;
        } 
        /*! \brief returns the property yColumn. 
            \details Description of the parameter yColumn is: <BLOCKQUOTE>\copydoc yColumn </BLOCKQUOTE> 
            \see yColumn for more information */ 
        inline virtual int get_yColumn() const  
        {
            return this->yColumn; 
        }
        /*! \brief sets the property yColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter yColumn is:  <BLOCKQUOTE>\copydoc yColumn </BLOCKQUOTE> 
        	\see yColumn for more information */ 
        inline virtual void set_yColumn (size_t __value) { this->yColumn = static_cast<int>(__value); }
        /*! \brief sets the property sortData to the specified \a __value. 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual void set_sortData(const DataSortOrder & __value)  
        {
            this->sortData = __value;
        } 
        /*! \brief returns the property sortData. 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual DataSortOrder get_sortData() const  
        {
            return this->sortData; 
        }
        /*! \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> \see sortData for more information */
        void set_sortData(int __value);

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
        /** \brief returns the index of the i-th datapoint (where i is an index into the SORTED datapoints)
         *
         * This function can beu used to get the correct datapoint after sorting the datapoints,
         * As sorting is done by sorting an index and not reordering the data in the columns themselves.
         * */
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }
};


/*! \brief This virtual JKQTPGraph descendent may be used as base for all graphs that use at least one column
           of data
    \ingroup jkqtplotter_basegraphs


 */
class LIB_EXPORT JKQTPSingleColumnGraph: public JKQTPGraph {
        Q_OBJECT
    public:
        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };

        enum class DataDirection {
            X,
            Y
        };

        /** \brief class constructor */
        JKQTPSingleColumnGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPSingleColumnGraph(int dataColumn, JKQTBasePlotter* parent=nullptr);
        JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style=Qt::SolidLine, double lineWidth=2.0, JKQTBasePlotter* parent=nullptr);
        JKQTPSingleColumnGraph(JKQTPlotter* parent);
        JKQTPSingleColumnGraph(int dataColumn, JKQTPlotter* parent);
        JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, double lineWidth, JKQTPlotter* parent);
        JKQTPSingleColumnGraph(int dataColumn, QColor color, Qt::PenStyle style, JKQTPlotter* parent);
        JKQTPSingleColumnGraph(int dataColumn, QColor color, JKQTPlotter* parent);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        /*! \brief sets the property dataColumn to the specified \a __value. 
            \details Description of the parameter dataColumn is: <BLOCKQUOTE>\copydoc dataColumn </BLOCKQUOTE> 
            \see dataColumn for more information */ 
        inline virtual void set_dataColumn(int __value)
        {
            this->dataColumn = __value;
        } 
        /*! \brief returns the property dataColumn. 
            \details Description of the parameter dataColumn is: <BLOCKQUOTE>\copydoc dataColumn </BLOCKQUOTE> 
            \see dataColumn for more information */ 
        inline virtual int get_dataColumn() const  
        {
            return this->dataColumn; 
        }
        /*! \brief sets the property dataColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter dataColumn is:  <BLOCKQUOTE>\copydoc dataColumn </BLOCKQUOTE> 
        	\see dataColumn for more information */ 
        inline virtual void set_dataColumn (size_t __value) { this->dataColumn = static_cast<int>(__value); }
        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property style to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual void set_style(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual Qt::PenStyle get_style() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property sortData to the specified \a __value. 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual void set_sortData(const DataSortOrder & __value)  
        {
            this->sortData = __value;
        } 
        /*! \brief returns the property sortData. 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual DataSortOrder get_sortData() const  
        {
            return this->sortData; 
        }
        /*! \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> \see sortData for more information */
        void set_sortData(int __value);

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief the column that contains the datapoints */
        int dataColumn;

        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief color of the graph */
        QColor color;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pt) of the graph, given in pt */
        double lineWidth;

        QPen getLinePen(JKQTPEnhancedPainter &painter) const;

        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;
        virtual void intSortData();
        /** \brief returns the index of the i-th datapoint (where i is an index into the SORTED datapoints)
         *
         * This function can beu used to get the correct datapoint after sorting the datapoints,
         * As sorting is done by sorting an index and not reordering the data in the columns themselves.
         * */
        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }


};






/*! \brief Descendents of this class add data fields for error indicators and methods to plot them to a class.
    \ingroup jkqtplotter_basegraphserrors

    This class is meant to be used with multiple inheritance. I.e. if you implemented some kind of plot
    you may derive an error plot class in which you will have to overwrite the JKQTPGraph::drawErrorsBefor()
    or drawErrorsAfter() so it calls the plotErrorIndicators() method from this class. In addition this class
    will add some public datamemebers and methods to your class that allow to specify the properties of the
    error indicators (plot properties: color, width, ... and columns for the data).

 */
class LIB_EXPORT JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPGraphErrors(QColor graphColor=QColor("black"));
        virtual ~JKQTPGraphErrors();

        /*! \brief sets the property errorColor to the specified \a __value. 
            \details Description of the parameter errorColor is: <BLOCKQUOTE>\copydoc errorColor </BLOCKQUOTE> 
            \see errorColor for more information */ 
        inline virtual void set_errorColor(const QColor & __value)  
        {
            this->errorColor = __value;
        } 
        /*! \brief returns the property errorColor. 
            \details Description of the parameter errorColor is: <BLOCKQUOTE>\copydoc errorColor </BLOCKQUOTE> 
            \see errorColor for more information */ 
        inline virtual QColor get_errorColor() const  
        {
            return this->errorColor; 
        }
        /*! \brief sets the property errorStyle to the specified \a __value. 
            \details Description of the parameter errorStyle is: <BLOCKQUOTE>\copydoc errorStyle </BLOCKQUOTE> 
            \see errorStyle for more information */ 
        inline virtual void set_errorStyle(const Qt::PenStyle & __value)  
        {
            this->errorStyle = __value;
        } 
        /*! \brief returns the property errorStyle. 
            \details Description of the parameter errorStyle is: <BLOCKQUOTE>\copydoc errorStyle </BLOCKQUOTE> 
            \see errorStyle for more information */ 
        inline virtual Qt::PenStyle get_errorStyle() const  
        {
            return this->errorStyle; 
        }
        /*! \brief sets the property errorWidth to the specified \a __value. 
            \details Description of the parameter errorWidth is: <BLOCKQUOTE>\copydoc errorWidth </BLOCKQUOTE> 
            \see errorWidth for more information */ 
        inline virtual void set_errorWidth(double __value)
        {
            this->errorWidth = __value;
        } 
        /*! \brief returns the property errorWidth. 
            \details Description of the parameter errorWidth is: <BLOCKQUOTE>\copydoc errorWidth </BLOCKQUOTE> 
            \see errorWidth for more information */ 
        inline virtual double get_errorWidth() const  
        {
            return this->errorWidth; 
        }
        /*! \brief sets the property errorFillColor to the specified \a __value. 
            \details Description of the parameter errorFillColor is: <BLOCKQUOTE>\copydoc errorFillColor </BLOCKQUOTE> 
            \see errorFillColor for more information */ 
        inline virtual void set_errorFillColor(const QColor & __value)  
        {
            this->errorFillColor = __value;
        } 
        /*! \brief returns the property errorFillColor. 
            \details Description of the parameter errorFillColor is: <BLOCKQUOTE>\copydoc errorFillColor </BLOCKQUOTE> 
            \see errorFillColor for more information */ 
        inline virtual QColor get_errorFillColor() const  
        {
            return this->errorFillColor; 
        }
        /*! \brief sets the property errorFillStyle to the specified \a __value. 
            \details Description of the parameter errorFillStyle is: <BLOCKQUOTE>\copydoc errorFillStyle </BLOCKQUOTE> 
            \see errorFillStyle for more information */ 
        inline virtual void set_errorFillStyle(const Qt::BrushStyle & __value)  
        {
            this->errorFillStyle = __value;
        } 
        /*! \brief returns the property errorFillStyle. 
            \details Description of the parameter errorFillStyle is: <BLOCKQUOTE>\copydoc errorFillStyle </BLOCKQUOTE> 
            \see errorFillStyle for more information */ 
        inline virtual Qt::BrushStyle get_errorFillStyle() const  
        {
            return this->errorFillStyle; 
        }
        /*! \brief sets the property errorbarSize to the specified \a __value. 
            \details Description of the parameter errorbarSize is: <BLOCKQUOTE>\copydoc errorbarSize </BLOCKQUOTE> 
            \see errorbarSize for more information */ 
        inline virtual void set_errorbarSize(double __value)
        {
            this->errorbarSize = __value;
        } 
        /*! \brief returns the property errorbarSize. 
            \details Description of the parameter errorbarSize is: <BLOCKQUOTE>\copydoc errorbarSize </BLOCKQUOTE> 
            \see errorbarSize for more information */ 
        inline virtual double get_errorbarSize() const  
        {
            return this->errorbarSize; 
        }
        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const;

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
         *         the JKQTPGraph descendents */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph,  int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const  QVector<int>* dataorder=nullptr)=0;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        void intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int datarange_start=-1, int datarange_end=-1, int xErrorColumnLower=-1, int yErrorColumnLower=-1, bool xErrorSymmetric=true, bool yErrorSymmetric=true, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr);

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int index, QColor& errorColor, QColor& errorFillColor);


        virtual double getXErrorU(int i, JKQTPDatastore* ds) const;
        virtual double getXErrorL(int i, JKQTPDatastore* ds) const;
        virtual double getYErrorU(int i, JKQTPDatastore* ds) const;
        virtual double getYErrorL(int i, JKQTPDatastore* ds) const;

        //** \brief plot a single error indicator */
        //void intPlotXYErrorIndicator(JKQTPEnhancedPainter& painter, double x, double xperror, double xmerror, double y, double yperror, double ymerror, JKQTPErrorPlotstyle xErrorStyle=JKQTPNoError, JKQTPErrorPlotstyle yErrorStyle=JKQTPNoError, double lastx=0, double lastxperror=0, double lastxmerror=0, double lasty=0, double lastyperror=0, double lastymerror=0);
};


/*! \brief This class adds data fields for error indicators in x direction to a JKQTPGraph descendent.
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPGraphErrors
 */
class LIB_EXPORT JKQTPXGraphErrors: public JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPXGraphErrors(QColor graphColor=QColor("black"));

        /*! \brief sets the property xErrorSymmetric to the specified \a __value. 
            \details Description of the parameter xErrorSymmetric is: <BLOCKQUOTE>\copydoc xErrorSymmetric </BLOCKQUOTE> 
            \see xErrorSymmetric for more information */ 
        inline virtual void set_xErrorSymmetric(bool __value)
        {
            this->xErrorSymmetric = __value;
        } 
        /*! \brief returns the property xErrorSymmetric. 
            \details Description of the parameter xErrorSymmetric is: <BLOCKQUOTE>\copydoc xErrorSymmetric </BLOCKQUOTE> 
            \see xErrorSymmetric for more information */ 
        inline virtual bool get_xErrorSymmetric() const  
        {
            return this->xErrorSymmetric; 
        }
        /*! \brief returns the property xErrorColumnLower. \details Description of the parameter xErrorColumnLower is:  <BLOCKQUOTE>\copydoc xErrorColumnLower </BLOCKQUOTE>. \see xErrorColumnLower for more information */ 
        inline int get_xErrorColumnLower() const { return this->xErrorColumnLower; }
        /*! \brief returns the property xErrorColumn. \details Description of the parameter xErrorColumn is:  <BLOCKQUOTE>\copydoc xErrorColumn </BLOCKQUOTE>. \see xErrorColumn for more information */ 
        inline int get_xErrorColumn() const { return this->xErrorColumn; }
        /*! \brief sets the property xErrorStyle to the specified \a __value. 
            \details Description of the parameter xErrorStyle is: <BLOCKQUOTE>\copydoc xErrorStyle </BLOCKQUOTE> 
            \see xErrorStyle for more information */ 
        inline virtual void set_xErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->xErrorStyle = __value;
        } 
        /*! \brief returns the property xErrorStyle. 
            \details Description of the parameter xErrorStyle is: <BLOCKQUOTE>\copydoc xErrorStyle </BLOCKQUOTE> 
            \see xErrorStyle for more information */ 
        inline virtual JKQTPErrorPlotstyle get_xErrorStyle() const  
        {
            return this->xErrorStyle; 
        }
        /** \brief set the column from which to read the error values for x-error indicators */
        void set_xErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_xErrorColumnLower(int __value);

        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const override;

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle xErrorStyle;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getXErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getXErrorL(int i, JKQTPDatastore* ds) const override;

};


/*! \brief This class adds data fields for error indicators in y direction to a class.
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPGraphErrors

 */
class LIB_EXPORT JKQTPYGraphErrors: public JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPYGraphErrors(QColor graphColor=QColor("black"));

        /*! \brief sets the property yErrorSymmetric to the specified \a __value. 
            \details Description of the parameter yErrorSymmetric is: <BLOCKQUOTE>\copydoc yErrorSymmetric </BLOCKQUOTE> 
            \see yErrorSymmetric for more information */ 
        inline virtual void set_yErrorSymmetric(bool __value)
        {
            this->yErrorSymmetric = __value;
        } 
        /*! \brief returns the property yErrorSymmetric. 
            \details Description of the parameter yErrorSymmetric is: <BLOCKQUOTE>\copydoc yErrorSymmetric </BLOCKQUOTE> 
            \see yErrorSymmetric for more information */ 
        inline virtual bool get_yErrorSymmetric() const  
        {
            return this->yErrorSymmetric; 
        }
        /*! \brief returns the property yErrorColumnLower. \details Description of the parameter yErrorColumnLower is:  <BLOCKQUOTE>\copydoc yErrorColumnLower </BLOCKQUOTE>. \see yErrorColumnLower for more information */ 
        inline int get_yErrorColumnLower() const { return this->yErrorColumnLower; }
        /*! \brief returns the property yErrorColumn. \details Description of the parameter yErrorColumn is:  <BLOCKQUOTE>\copydoc yErrorColumn </BLOCKQUOTE>. \see yErrorColumn for more information */ 
        inline int get_yErrorColumn() const { return this->yErrorColumn; }
        /*! \brief sets the property yErrorStyle to the specified \a __value. 
            \details Description of the parameter yErrorStyle is: <BLOCKQUOTE>\copydoc yErrorStyle </BLOCKQUOTE> 
            \see yErrorStyle for more information */ 
        inline virtual void set_yErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->yErrorStyle = __value;
        } 
        /*! \brief returns the property yErrorStyle. 
            \details Description of the parameter yErrorStyle is: <BLOCKQUOTE>\copydoc yErrorStyle </BLOCKQUOTE> 
            \see yErrorStyle for more information */ 
        inline virtual JKQTPErrorPlotstyle get_yErrorStyle() const  
        {
            return this->yErrorStyle; 
        }
        /** \copydoc JKQTPGraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c) const override;

        /** \brief set the column from which to read the error values for y-error indicators */
        void set_yErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_yErrorColumnLower(int __value);
    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle yErrorStyle;
        /** \brief indicates whether the y-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief the column that contains the error of the y-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getYErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getYErrorL(int i, JKQTPDatastore* ds) const override;
};


/*! \brief This class adds data fields for error indicators in x and y direction to a class.
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPGraphErrors

 */
class LIB_EXPORT JKQTPXYGraphErrors: public JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPXYGraphErrors(QColor graphColor=QColor("black"));
        /*! \brief sets the property xErrorSymmetric to the specified \a __value. 
            \details Description of the parameter xErrorSymmetric is: <BLOCKQUOTE>\copydoc xErrorSymmetric </BLOCKQUOTE> 
            \see xErrorSymmetric for more information */ 
        inline virtual void set_xErrorSymmetric(bool __value)
        {
            this->xErrorSymmetric = __value;
        } 
        /*! \brief returns the property xErrorSymmetric. 
            \details Description of the parameter xErrorSymmetric is: <BLOCKQUOTE>\copydoc xErrorSymmetric </BLOCKQUOTE> 
            \see xErrorSymmetric for more information */ 
        inline virtual bool get_xErrorSymmetric() const  
        {
            return this->xErrorSymmetric; 
        }
        /*! \brief sets the property yErrorSymmetric to the specified \a __value. 
            \details Description of the parameter yErrorSymmetric is: <BLOCKQUOTE>\copydoc yErrorSymmetric </BLOCKQUOTE> 
            \see yErrorSymmetric for more information */ 
        inline virtual void set_yErrorSymmetric(bool __value)
        {
            this->yErrorSymmetric = __value;
        } 
        /*! \brief returns the property yErrorSymmetric. 
            \details Description of the parameter yErrorSymmetric is: <BLOCKQUOTE>\copydoc yErrorSymmetric </BLOCKQUOTE> 
            \see yErrorSymmetric for more information */ 
        inline virtual bool get_yErrorSymmetric() const  
        {
            return this->yErrorSymmetric; 
        }
        /*! \brief returns the property xErrorColumnLower. \details Description of the parameter xErrorColumnLower is:  <BLOCKQUOTE>\copydoc xErrorColumnLower </BLOCKQUOTE>. \see xErrorColumnLower for more information */ 
        inline int get_xErrorColumnLower() const { return this->xErrorColumnLower; }
        /*! \brief returns the property xErrorColumn. \details Description of the parameter xErrorColumn is:  <BLOCKQUOTE>\copydoc xErrorColumn </BLOCKQUOTE>. \see xErrorColumn for more information */ 
        inline int get_xErrorColumn() const { return this->xErrorColumn; }
        /*! \brief returns the property yErrorColumnLower. \details Description of the parameter yErrorColumnLower is:  <BLOCKQUOTE>\copydoc yErrorColumnLower </BLOCKQUOTE>. \see yErrorColumnLower for more information */ 
        inline int get_yErrorColumnLower() const { return this->yErrorColumnLower; }
        /*! \brief returns the property yErrorColumn. \details Description of the parameter yErrorColumn is:  <BLOCKQUOTE>\copydoc yErrorColumn </BLOCKQUOTE>. \see yErrorColumn for more information */ 
        inline int get_yErrorColumn() const { return this->yErrorColumn; }
        /*! \brief sets the property yErrorStyle to the specified \a __value. 
            \details Description of the parameter yErrorStyle is: <BLOCKQUOTE>\copydoc yErrorStyle </BLOCKQUOTE> 
            \see yErrorStyle for more information */ 
        inline virtual void set_yErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->yErrorStyle = __value;
        } 
        /*! \brief returns the property yErrorStyle. 
            \details Description of the parameter yErrorStyle is: <BLOCKQUOTE>\copydoc yErrorStyle </BLOCKQUOTE> 
            \see yErrorStyle for more information */ 
        inline virtual JKQTPErrorPlotstyle get_yErrorStyle() const  
        {
            return this->yErrorStyle; 
        }
        /*! \brief sets the property xErrorStyle to the specified \a __value. 
            \details Description of the parameter xErrorStyle is: <BLOCKQUOTE>\copydoc xErrorStyle </BLOCKQUOTE> 
            \see xErrorStyle for more information */ 
        inline virtual void set_xErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->xErrorStyle = __value;
        } 
        /*! \brief returns the property xErrorStyle. 
            \details Description of the parameter xErrorStyle is: <BLOCKQUOTE>\copydoc xErrorStyle </BLOCKQUOTE> 
            \see xErrorStyle for more information */ 
        inline virtual JKQTPErrorPlotstyle get_xErrorStyle() const  
        {
            return this->xErrorStyle; 
        }
        /** \copydoc JKQTPGraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c) const override;

        /** \brief set the column from which to read the error values for x-error indicators */
        void set_xErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_xErrorColumnLower(int __value);
        /** \brief set the column from which to read the error values for x-error indicators */
        void set_yErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_yErrorColumnLower(int __value);
        /** \brief set the column from which to read the error values for y-error indicators */
        void set_xErrorColumn(size_t __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_xErrorColumnLower(size_t __value);
        /** \brief set the column from which to read the error values for y-error indicators */
        void set_yErrorColumn(size_t __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void set_yErrorColumnLower(size_t __value);

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle yErrorStyle;
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle xErrorStyle;
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
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int datarange_start=-1, int datarange_end=-1, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getXErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getXErrorL(int i, JKQTPDatastore* ds) const override;
        virtual double getYErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getYErrorL(int i, JKQTPDatastore* ds) const override;
};







#endif // jkqtpgraphsbase_H
