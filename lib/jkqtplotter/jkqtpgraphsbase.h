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
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"

#ifndef JKQTPGRAPHSBASE_H
#define JKQTPGRAPHSBASE_H

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
 *   - bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
 *   - QColor getKeyLabelColor() const=0;
  * .
 *
 * Optionally you may also overwrite these functions to draw elements outside the actual plot area (like e.g. colorbars):
 *   - void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);
 *   - void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);
 * .
 *
 * In addition this class provudes protected
 * functions that do coordinate transforms based on the current coordinate system, of the paren
 * JKQTPlotter (i.e. using the axes JKQTPLott:xAxis and JKQTPlotter::yAxis as basis for the plotting).
 *
 * \see \ref jkqtplotter_graphsgroup_classstructure
 */
class JKQTP_LIB_EXPORT JKQTPPlotElement: public QObject {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPPlotElement(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPPlotElement(JKQTPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPPlotElement() = default;

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
        virtual QColor getKeyLabelColor() const=0;

        /** \brief sets the title of the plot (for display in key!).
         *
         * \note If no title is supplied, no key entry is drawn. */
        virtual void setTitle(const QString & __value);
        /*! \brief returns the the title of the plot */
        virtual QString getTitle() const;
        /*! \brief sets whether the graph is visible in the plot */
        void virtual setVisible(bool __value);
        /*! \brief returns whether the graph is visible in the plot */
        bool virtual isVisible() const;
        /*! \brief sets whether the graph is drawn in a highlighted style in the plot */
        void virtual setHighlighted(bool __value);
        /*! \brief returns whether the graph is shown in a highlighted style in the plot */
        bool virtual isHighlighted() const;

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



        /** \brief tool routine that transforms an x-coordinate (plot coordinate --> pixels) for this plot element */
        virtual double transformX(double x) const;

        /** \brief tool routine that transforms a y-coordinate (plot coordinate --> pixels) for this plot element */
        virtual double transformY(double y) const;

        /** \brief tool routine that backtransforms an x-coordinate (pixels --> plot coordinate) for this plot element */
        virtual double backtransformX(double x) const;

        /** \brief tool routine that backtransforms a y-coordinate (pixels --> plot coordinate) for this plot element */
        virtual double backtransformY(double y) const;


        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules (plot coordinate --> pixels) */
        inline QPointF transform(const QPointF& x) {
            return QPointF(transformX(x.x()), transformY(x.y()));
        }

        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules (pixels --> plot coordinate) */
        inline QPointF backTransform(const QPointF& x) {
            return QPointF(backtransformX(x.x()), backtransformY(x.y()));
        }

        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules (plot coordinate --> pixels) */
        inline QPointF transform(double x, double y) {
            return transform(QPointF(x,y));
        }
        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules (pixels --> plot coordinate) */
        inline QPointF backTransform(double x, double y) {
            return backTransform(QPointF(x,y));
        }
        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules (plot coordinate --> pixels) */
        QVector<QPointF> transform(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a (non-closed) path consisting of lines (plot coordinate --> pixels) */
        QPainterPath transformToLinePath(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a polygon (plot coordinate --> pixels) */
        inline QPolygonF transformToPolygon(const QVector<QPointF>& x) {
            return QPolygonF(transform(x));
        }

        /** \brief the plotter object this object belongs to */
        JKQTBasePlotter* parent;

        /** \brief title of the plot (for display in key!). If no title is supplied, no key entry is drawn. */
        QString title;

        /** \brief indicates whether the graph is visible in the plot */
        bool visible;
        /** \brief indicates whether the graph is shown in a "highlghted" in the plot */
        bool highlighted;
        /** \brief internal storage for the used parent plot style */
        int parentPlotStyle;
};

/** \brief this virtual base class of the (data-column based) graphs,
 *         which are part of a JKQTPlotter plot and which use the coordinate system
 *         of the JKQTPlotter (i.e. the two coordinate axes getXAxis() and getYAxis())
 *         as basis for the graphs
 * \ingroup jkqtplotter_basegraphs
 *
 * This class adds features to work with data columns.
 *   - There are two properties datarange_start and datarange_end. By default they are -1 and therefore ignored.
 *     if they are != -1 the plotter only displays the datapoints with the indexes [datarange_start .. datarange_end]
 *     although there might be more data points available (range [0 .. maxDataPoints]). The datarange is cut at the
 *     full range, i.e. if datarange_end>maxDataPoints the plotter displays [datarange_start .. maxDataPoints].
 *   - Also there is a virtual function  usesColumn() which checks whether a given column is used by this graph.
 *     Override this function in your derived graphs to indicate to JKQTPlotter / JKQTBasePlotter , which columns
 *     from the internal JKQTPDatastore are actually used. This information can be used e.g. for graph-specific data-export.
 * .
 *
 * \see \ref jkqtplotter_graphsgroup_classstructure
 */
class JKQTP_LIB_EXPORT JKQTPGraph: public JKQTPPlotElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        explicit JKQTPGraph(JKQTPlotter* parent);

        /** \brief default wirtual destructor */
        virtual ~JKQTPGraph() = default ;


        /** \brief returns \c true if the given column is used by the graph
         *
         * This virtual function indicates whether a given column is used by this graph.
         * Override this function in your derived graphs to indicate to JKQTPlotter / JKQTBasePlotter , which columns
         * from the internal JKQTPDatastore are actually used. This information can be used e.g. for graph-specific data-export.
         */
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

        /** \brief get the maximum and minimum value of the given column
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        bool getDataMinMax(int column, double& minx, double& maxx, double& smallestGreaterZero);



    protected:


        friend class JKQTPGraphErrors;

};


/** \brief this is the virtual base class of all JKQTPPlotElement's in a JKQTPlotter plot that
 *         represent geometric forms or annotations. They have extended coordinate transform capabilities, because
 *         in addition to using the plot coordinates, you can also choose to use different other
 *         coordinate systems
 * \ingroup jkqtplotter_basegraphs
 *
 * \see \ref jkqtplotter_graphsgroup_classstructure
 *
 */
class JKQTP_LIB_EXPORT JKQTPPlotObject: public JKQTPPlotElement {
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

    This class implements basic management facilities for the data columns:
      - setXColumn(), setYColumn() to set the columns to be used for the graph data
      - setDataSortOrder() to specify whether and how the data should be sorted before drawing
        \image html jkqtplotter_unsorted.png "Unsorted Data"
        \image html jkqtplotter_sortedx.png "Data sorted along x-axis (DataSortOrder::SortedX)"
    .
    ... and overrides/implements the functions:
      - getXMinMax()
      - getYMinMax()
      - usesColumn()
    .

 */
class JKQTP_LIB_EXPORT JKQTPXYGraph: public JKQTPGraph {
        Q_OBJECT
    public:
        /** \brief specifies how to sort the data in a JKQTPXYGraph before drawing
         *
         * \image html jkqtplotter_unsorted.png "Unsorted Data"
         *
         * \image html jkqtplotter_sortedx.png "Data sorted along x-axis (DataSortOrder::SortedX)"
         */
        enum DataSortOrder {
            Unsorted=0, /*!< \brief the data for a JKQTPXYGraph is not sorted before drawing */
            SortedX=1, /*!< \brief the data for a JKQTPXYGraph is sorted so the x-values appear in ascending before drawing */
            SortedY=2 /*!< \brief the data for a JKQTPXYGraph is sorted so the y-values appear in ascending before drawing */
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

        /*! \copydoc xColumn
            \see see xColumn for details */ 
        inline virtual void setXColumn(int __value)
        {
            this->xColumn = __value;
        } 
        /*! \copydoc xColumn
            \see see xColumn for details */ 
        inline virtual int getXColumn() const  
        {
            return this->xColumn; 
        }
        /*! \brief sets the property xColumn ( \copybrief xColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter xColumn is:  <BLOCKQUOTE>\copydoc xColumn </BLOCKQUOTE> 
            \see xColumn for more information */ 
        inline virtual void setXColumn (size_t __value) { this->xColumn = static_cast<int>(__value); }
        /*! \copydoc yColumn
            \see see yColumn for details */ 
        inline virtual void setYColumn(int __value)
        {
            this->yColumn = __value;
        } 
        /*! \copydoc yColumn
            \see see yColumn for details */ 
        inline virtual int getYColumn() const  
        {
            return this->yColumn; 
        }
        /*! \brief sets the property yColumn ( \copybrief yColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter yColumn is:  <BLOCKQUOTE>\copydoc yColumn </BLOCKQUOTE> 
            \see yColumn for more information */ 
        inline virtual void setYColumn (size_t __value) { this->yColumn = static_cast<int>(__value); }
        /*! \copydoc sortData
            \see see sortData for details */ 
        inline virtual void setDataSortOrder(const DataSortOrder & __value)  
        {
            this->sortData = __value;
        } 
        /*! \copydoc sortData
            \see see sortData for details */ 
        inline virtual DataSortOrder getDataSortOrder() const  
        {
            return this->sortData; 
        }
        /*! \brief sets the property sortData ( \copybrief sortData ) to the specified \a __value. \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> \see sortData for more information */
        void setDataSortOrder(int __value);

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
         *
         * \see setDataSortOrder(), getDataSortOrder()
         * */
        inline int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }
};


/*! \brief This virtual JKQTPGraph descendent may be used as base for all graphs that use at least one column
           of data
    \ingroup jkqtplotter_basegraphs


    \see \ref jkqtplotter_graphsgroup_classstructure

 */
class JKQTP_LIB_EXPORT JKQTPSingleColumnGraph: public JKQTPGraph {
        Q_OBJECT
    public:
        /** \brief specifies how to sort the data for a JKQTPSingleColumnGraph before drawing
         *
         * \image html jkqtplotter_unsorted.png "Unsorted Data"
         *
         * \image html jkqtplotter_sortedx.png "Data sorted along x-axis (DataSortOrder::SortedX)"
         */
        enum DataSortOrder {
            Unsorted=0, /*!< \brief the data for a JKQTPSingleColumnGraph is not sorted before drawing */
            Sorted=1 /*!< \brief the data for a JKQTPSingleColumnGraph is sorted (in ascending order) before drawing */
        };

        /** \brief specifies whether the data for a JKQTPSingleColumnGraph represent x-axis or y-axis values */
        enum class DataDirection {
            X, /*!< \brief the data for a JKQTPSingleColumnGraph is data belonging to the x-axis of the plot */
            Y /*!< \brief the data for a JKQTPSingleColumnGraph is data belonging to the y-axis of the plot */
        };

        /** \brief class constructor */
        JKQTPSingleColumnGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPSingleColumnGraph(JKQTPlotter* parent);

        /*! \copydoc dataColumn
            \see see dataColumn for details */ 
        void setDataColumn(int __value);
        /*! \copydoc dataColumn
            \see see dataColumn for details */ 
        int getDataColumn() const;
        /*! \brief sets the property dataColumn ( \copybrief dataColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter dataColumn is:  <BLOCKQUOTE>\copydoc dataColumn </BLOCKQUOTE> 
            \see dataColumn for more information */ 
        void setDataColumn (size_t __value);

        /*! \copydoc sortData
            \see see sortData for details */ 
        void setDataSortOrder(const DataSortOrder & __value);
        /*! \copydoc sortData
            \see see sortData for details */ 
        DataSortOrder getDataSortOrder() const;
        /*! \brief sets the property sortData ( \copybrief sortData ) to the specified \a __value. \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> \see sortData for more information */
        void setDataSortOrder(int __value);


        /*! \copydoc dataDirection
            \see see dataDirection for details */
        void setDataDirection(DataDirection __value);
        /*! \copydoc dataDirection
            \see see dataDirection for details */
        DataDirection getDataDirection() const;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief the column that contains the datapoints */
        int dataColumn;
        /** \brief interpret the data from dataColumn either as X- or Y-data */
        DataDirection dataDirection;


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
        inline int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }


};




#endif // JKQTPGRAPHSBASE_H
