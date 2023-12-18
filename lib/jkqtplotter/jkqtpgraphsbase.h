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


#ifndef JKQTPGRAPHSBASE_H
#define JKQTPGRAPHSBASE_H

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"

// forward declarations
class JKQTPlotter;
class JKQTPDatastore;
class JKQTPGraphErrorStyleMixin;

/** \brief this virtual base class of every element, which is part of a JKQTPlotter plot and may appear in its key
 *         (basically any type of graph, except overlay elements!)
 * \ingroup jkqtplotter_basegraphs
 *
 * Each possible graph is represented by a child of this class. So additional plots may be created by
 * deriving new JKQTPGraph classes. To do so implement/overwrite these functions:
 *   - void draw(JKQTPEnhancedPainter& painter);
 *   - void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect);
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
class JKQTPLOTTER_LIB_EXPORT JKQTPPlotElement: public QObject {
        Q_OBJECT
    public:

        /** \brief class constructor */
        explicit JKQTPPlotElement(JKQTBasePlotter* parent=nullptr);

        /** \brief default wirtual destructor */
        virtual ~JKQTPPlotElement() = default;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter)=0;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect)=0;

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

        /** \brief returns the the title of the plot  */
        QString getTitle() const;
        /** \brief returns whether the graph is visible in the plot  */
        bool isVisible() const;
        /** \brief returns whether the graph is shown in a highlighted style in the plot  */
        bool isHighlighted() const;

        /** \brief returns the parent painter class */
        inline const JKQTBasePlotter* getParent() const { return parent; }
        /** \brief returns the parent painter class */
        inline JKQTBasePlotter* getParent() { return parent; }
        /** \brief sets the parent painter class */
        virtual void setParent(JKQTBasePlotter* parent);
        /** \brief sets the parent painter class */
        virtual void setParent(JKQTPlotter* parent);

        Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
        Q_PROPERTY(QString title READ getTitle WRITE setTitle)
        Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)
    public Q_SLOTS:
        /** \brief sets whether the graph is visible in the plot  */
        void setVisible(bool __value);
        /** \brief sets whether the graph is drawn in a highlighted style in the plot  */
        void setHighlighted(bool __value);
        /** \brief sets the title of the plot (for display in key!).
         *
         * \note If no title is supplied, no key entry is drawn. */
        virtual void setTitle(const QString & __value);

    public:


        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);

        /** \brief modes of operation for the function hitTest() */
        enum HitTestMode {
            HitTestXY,      /*!< \brief find closest point in x- and y-direction simulatneously (i.e. measure direct distance) */
            HitTestXOnly,   /*!< \brief find closest point in x-direction only */
            HitTestYOnly,   /*!< \brief find closest point in y-direction only */
        };
        Q_ENUM(HitTestMode)

        /*! \brief returns the closest distance of the plot element to the (screen pixel) position \a pos, or \c NAN

            This function is used to implement hit tests, i.e. to test whether a graph is close to a given position \a posSystem.
            The function will then return the distance of the closes graph-point and a label for this point. An example of what
            can be done with this function is the tooltip tool that JKQTPlotter provides via its context-menu/toolbar. This tool
            uses just the information of the closest point and its label to display a tooltip for that datapoint:

            \see jkqtpmdaToolTipForClosestDataPoint for details.


            \param posSystem position to test in system coordinates
            \param[out] closestSpotSystem optional output of the closest point found on the plot element in system coordinates
            \param[out] label optional output of a label for the closest point (that might e.g. be used in a tooltip)
            \param mode search mode, i.e. use sqrt(dx*dx+dy*dy) as distance, or just the absoulte values along one of the
                        two coordinate axes. Note that the returned distance depends on this parameter!
            \return NAN if not implemented, or if \a pos is very far from the plot element, or the closest distance (in screen pixels)
                    of \a pos from the plot element. Note that the returned distance depends on the choosen \a mode !!!
                    You can use JKQTPIsOKFloat() to check whether a valid distance was returned!


            Since the graph base class does not have any knowledge about how to perform a hit test on you specific graph, there is only a
            very general implementation in this class, which does not actually search through the graph itself, but searches through
            extra data that hs to be written during draw() and is stored in m_hitTestData. The implentation this base-class only searches this
            list of points+metadata to implement a basic hit-test. If the list is empty, of no close-by points are found (default), then
            hitTest() will simply return \a NAN.

            When writing a new graph, you can therefore implement hitTest() in one of these ways:
            <ol>
               <li> You simply fill m_hitTestData with appropriate data and rely on the implementation in JKQTPPlotElement to do the work for you:
                 You then need to call clearHitTestData() at the start of your draw() function and whenever you draw a datapoint, you add
                 its location and metadata to the internal storage with addHitTestData()
               <li> You derive from a graph class that already has an implementation. JKQTPXYGraph is an example of this. That class searches
                 through all x-/y-coordinates in the internally known columns and even takes into account possible graph errors in the label,
                 when the graph is also derived from JKQTPXGraphErrorData or JKQTPYGraphErrorData. This implementation therefore covers
                 most graph types pre-packaged with JKQTPlotter
               <li> You implement the function from scratch
            </ol>

            \see addHitTestData(), clearHitTestData(), m_hitTestData, HitTestLocation

         */
        virtual double hitTest(const QPointF & posSystem, QPointF* closestSpotSystem=nullptr, QString* label=nullptr, HitTestMode mode=HitTestXY) const;

        /** \brief Dataset for a single point on the graph, associated with its data-column index and a label that can be used by a basic implementation of hitTest()
         *
         * \see hitTest()
         */
        struct HitTestLocation {
            inline HitTestLocation(): pos(JKQTP_NAN, JKQTP_NAN), index(-1), label("") {}
            inline HitTestLocation(double x_, double y_, const QString& label_): pos(x_,y_), index(-1), label(label_) {}
            inline HitTestLocation(const QPointF& pos_, const QString& label_): pos(pos_), index(-1), label(label_) {}
            inline HitTestLocation(double x_, double y_, int index_, const QString& label_): pos(x_,y_), index(index_), label(label_) {}
            inline HitTestLocation(const QPointF& pos_, int index_, const QString& label_): pos(pos_), index(index_), label(label_) {}
            /** \brief position of the hit-test point */
            QPointF pos;
            /** \brief index of the hit-test point in the linked data-columns (or -1) */
            int index;
            /** \brief label for that specific hit-test point */
            QString label;
        };



        /** \brief tool routine that transforms an x-coordinate (plot coordinate --> pixels) for this plot element, uses the axis referenced in xAxisRef */
        double transformX(double x) const;

        /** \brief tool routine that transforms a y-coordinate (plot coordinate --> pixels) for this plot element, uses the axis referenced in yAxisRef */
        double transformY(double y) const;

        /** \brief tool routine that backtransforms an x-coordinate (pixels --> plot coordinate) for this plot element, uses the axis referenced in xAxisRef */
        double backtransformX(double x) const;

        /** \brief tool routine that backtransforms a y-coordinate (pixels --> plot coordinate) for this plot element, uses the axis referenced in yAxisRef */
        double backtransformY(double y) const;


        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules (plot coordinate --> pixels) */
        inline QPointF transform(const QPointF& x) const {
            return QPointF(transformX(x.x()), transformY(x.y()));
        }


        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules (pixels --> plot coordinate) */
        inline QPointF backTransform(const QPointF& x) const {
            return QPointF(backtransformX(x.x()), backtransformY(x.y()));
        }

        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules (plot coordinate --> pixels) */
        inline QPointF transform(double x, double y) const {
            return transform(QPointF(x,y));
        }

        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules (pixels --> plot coordinate) */
        inline QPointF backTransform(double x, double y) const {
            return backTransform(QPointF(x,y));
        }
        /** \brief tool routine that transforms a QPolygonF according to the parent's transformation rules (plot coordinate --> pixels) */
        QPolygonF transform(const QPolygonF& x) const;

        /** \brief tool routine that transforms a QPolygonF according to the parent's transformation rules
         *         and returns a (non-closed) path consisting of lines (plot coordinate --> pixels) */
        QPainterPath transformToLinePath(const QPolygonF& x) const;


        /** \brief transform all x-coordinates in a vector \a x */
        QVector<double> transformX(const QVector<double>& x) const;
        /** \brief transform all y-coordinates in a vector \a x */
        QVector<double> transformY(const QVector<double>& x) const;

        /** \copydoc xAxisRef */
        void setXAxis(JKQTPCoordinateAxisRef ref);
        /** \copydoc xAxisRef */
        JKQTPCoordinateAxisRef getXAxisRef() const;
        /** \copydoc yAxisRef */
        void setYAxis(JKQTPCoordinateAxisRef ref);
        /** \copydoc yAxisRef */
        JKQTPCoordinateAxisRef getYAxisRef() const;
        /** \brief returns the actual x-Axis-object from the parent plotter, referenced in xAxisRef */
        /** \brief returns the actual x-Axis-object from the parent plotter, referenced in xAxisRef */
        inline const JKQTPCoordinateAxis* getXAxis() const {
            return parent->getXAxis(xAxisRef);
        }
        /** \brief returns the actual y-Axis-object from the parent plotter, referenced in yAxisRef */
        inline const JKQTPCoordinateAxis* getYAxis() const {
            return parent->getYAxis(yAxisRef);
        }
        /** \brief set the coordinate axes to use for this plot element
         *
         *  \see xAxisRef, yAxisRef, transformX(), transformY()
         */
        void setAxes(JKQTPCoordinateAxisRef ref);

        Q_PROPERTY(JKQTPCoordinateAxisRef xAxisRef READ getXAxisRef WRITE setXAxis)
        Q_PROPERTY(JKQTPCoordinateAxisRef yAxisRef READ getYAxisRef WRITE setYAxis)

    protected:

        /** \brief clear the internal datastore for hitTest()
         *
         *  \note This function has to be called at the start of draw()
         *  \see hitTest(), addHitTestData(), m_hitTestData, HitTestLocation
         */
        inline void clearHitTestData() { m_hitTestData.clear(); }
        /** \brief reserve list entries for up to \a points graph points in the internal datastore for hitTest()
         *
         *  \note Call this after clearHitTestData() for improved speed of subsequent addHitTestData() calls!
         *  \see hitTest(), addHitTestData(), m_hitTestData, HitTestLocation
         */
        inline void reserveHitTestData(int points) { m_hitTestData.reserve(qMax(10, abs(points))); }
        /** \brief clear the internal datastore for hitTest()
         *
         *  \note This function has to be called at the start of draw()
         *  \see hitTest(), clearHitTestData(), m_hitTestData, HitTestLocation, reserveHitTestData()
         */
        inline void addHitTestData(const HitTestLocation& loc) { m_hitTestData<<loc; }
        /** \brief add a new point on the graph to the internal datastore for hitTest()
         *
         *  \param x_ x-position of the graph point in system coordinates
         *  \param y_ y-position of the graph point in system coordinates
         *  \param label_ a label for this datapoint, that can e.g. be displayed in a tooltip for this point
         *
         *  \see hitTest(), clearHitTestData(), m_hitTestData, HitTestLocation, reserveHitTestData()
         */
        inline void addHitTestData(double x_, double y_, const QString& label_) { addHitTestData(HitTestLocation(x_,y_,label_)); }
        /** \brief clear the internal datastore for hitTest()
         *
         *  \param pos_ position of the graph point in system coordinates
         *  \param label_ a label for this datapoint, that can e.g. be displayed in a tooltip for this point
         *
         *  \see hitTest(), clearHitTestData(), m_hitTestData, HitTestLocation, reserveHitTestData()
         */
        inline void addHitTestData(const QPointF& pos_, const QString& label_) { addHitTestData(HitTestLocation(pos_,label_)); }
        /** \brief add a new point on the graph to the internal datastore for hitTest(),
         *         this variant uses formatHitTestDefaultLabel() to auto-generate the label
         *
         *  \param x_ x-position of the graph point in system coordinates
         *  \param y_ y-position of the graph point in system coordinates
         *  \param index_ index of the graph point in the internal data columns, or -1
         *  \param datastore datastore for formatHitTestDefaultLabel()
         *
         *  \see hitTest(), clearHitTestData(), m_hitTestData, HitTestLocation, reserveHitTestData()
         */
        inline void addHitTestData(double x_, double y_, int index_=-1, JKQTPDatastore* datastore=nullptr) { addHitTestData(HitTestLocation(x_,y_,formatHitTestDefaultLabel(x_,y_, index_, datastore))); }
        /** \brief clear the internal datastore for hitTest(),
         *         this variant uses formatHitTestDefaultLabel() to auto-generate the label
         *
         *  \param pos_ position of the graph point in system coordinates
         *  \param index_ index of the graph point in the internal data columns, or -1
         *  \param datastore datastore for formatHitTestDefaultLabel()
         *
         *  \see hitTest(), clearHitTestData(), m_hitTestData, HitTestLocation, reserveHitTestData()
         */
        inline void addHitTestData(const QPointF& pos_, int index_=-1, JKQTPDatastore* datastore=nullptr) { addHitTestData(HitTestLocation(pos_,formatHitTestDefaultLabel(pos_.x(), pos_.y(), index_, datastore))); }
        /** \brief clear the internal datastore for hitTest()
         *
         *  \param x_ x-position of the graph point in system coordinates
         *  \param y_ y-position of the graph point in system coordinates
         *  \param index_ index of the graph point in the internal data columns
         *  \param label_ a label for this datapoint, that can e.g. be displayed in a tooltip for this point
         *
         *  \see hitTest(), clearHitTestData(), m_hitTestData, HitTestLocation, reserveHitTestData()
         */
        inline void addHitTestData(double x_, double y_, int index_, const QString& label_) { addHitTestData(HitTestLocation(x_,y_,index_,label_)); }
        /** \brief clear the internal datastore for hitTest()
         *
         *  \param pos_ position of the graph point in system coordinates
         *  \param index_ index of the graph point in the internal data columns
         *  \param label_ a label for this datapoint, that can e.g. be displayed in a tooltip for this point
         *
         *  \see hitTest(), clearHitTestData(), m_hitTestData, HitTestLocation, reserveHitTestData()
         */
        inline void addHitTestData(const QPointF& pos_, int index_, const QString& label_) { addHitTestData(HitTestLocation(pos_,index_,label_)); }

        /** \brief tool-function for hitTest(), which formats a default label, taking into account the x- and y-position (both provided)
         *         and optionally the errors of these positions.
         *
         * \param x x-position of the datapoint in system coordinates
         * \param y y-position of the datapoint in system coordinates
         * \param index the index of the data point in the associated data column(s), or -1 (optional!)
         * \param datastore The datastore to read error data from (optional!)
         * \returns a LaTeX formatted label
         */
        virtual QString formatHitTestDefaultLabel(double x, double y, int index=-1, JKQTPDatastore *datastore=nullptr) const;

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
        /** \brief indicates which coordinate axis to use for coordinate transforms in x-direction */
        JKQTPCoordinateAxisRef xAxisRef;
        /** \brief indicates which coordinate axis to use for coordinate transforms in y-direction */
        JKQTPCoordinateAxisRef yAxisRef;



        /** \brief dataset with graph-points and associated data fro the function hitTest()
         * \see hitTest(), HitTestLocation
         */
        QVector<HitTestLocation> m_hitTestData;

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
class JKQTPLOTTER_LIB_EXPORT JKQTPGraph: public JKQTPPlotElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPGraph(JKQTBasePlotter* parent=nullptr);

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


        friend class JKQTPGraphErrorStyleMixin;

};


/** \brief this is the virtual base class of all JKQTPPlotElement's in a JKQTPlotter plot that
 *         represent geometric forms.
 *  \ingroup jkqtplotter_basegraphs
 *
 *  \see \ref jkqtplotter_graphsgroup_classstructure, \ref JKQTPlotterGeometricGraphs
 *
 *  \section JKQTPGeometricPlotElement_coordinates Coordinate Systems
 *  JKQTPGeometricPlotElement's have extended coordinate transform capabilities, because in addition to using
 *  the plot coordinates, you can also choose to use different other coordinate systems.
 *
 *  \section JKQTPGeometricPlotElement_DrawMode Draw Modes
 *
 *  \copydetails m_drawMode
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeometricPlotElement: public JKQTPPlotElement {
    Q_OBJECT
public:

    /** \brief indicates how to draw the geometric object */
    enum DrawMode {
        DrawAsGraphicElement,     /*!< \brief draw lines as lines (i.e. graphic elements)  \image html JKQTPGeometricPlotElement_DrawAsGraphicElement.png */
        DrawAsMathematicalCurve   /*!< \brief draw lines as the mathematically correct curve \image html JKQTPGeometricPlotElement_DrawAsMathematicalCurve.png */
    };
    Q_ENUM(DrawMode)

    /** \brief class constructor */
    explicit JKQTPGeometricPlotElement(DrawMode drawMode=DrawAsGraphicElement, JKQTBasePlotter* parent=nullptr);

    /** \brief default wirtual destructor */
    virtual ~JKQTPGeometricPlotElement() ;
    /** \copybrief m_drawMode
         *
         *  \return the currently set DrawMode
         *  \see m_drawMode, DrawMode
         */
    DrawMode getDrawMode() const;

    Q_PROPERTY(DrawMode drawMode READ getDrawMode WRITE setDrawMode)
public Q_SLOTS:
    /** \copybrief m_drawMode
         *
         *  \param mode the DrawMode to use from now on
         *  \see m_drawMode, DrawMode
         */
    void setDrawMode(DrawMode mode);
protected:
    /** \brief indicated whether to draw lines as graphic elements (even on non-linear coordinate systems),
         *         or as mathematically correct curves
         *
         * It is possible to define in which ways the forms shall be treated/drawn into non-linear
         * coordinate systems (e.g. semi-log or log-log plots). Imagine drawing a line from (x1,y1) to (x2,y2)
         * In a linear coordinate system, this is always a line, but in a non-linear system, the line might
         * have to be represented by a curve instead. Depending on how you want to use the JKQTPGeometricPlotElement you
         * can choose to still draw it as a line connecting the points (x1,y1) and (x2,y2), or as the -
         * mathematically correct - curve connecting these two points:
         *
         * \image html JKQTPGeometricPlotElement_DrawAsMathematicalCurve.png
         *
         * \image html JKQTPGeometricPlotElement_DrawAsGraphicElement.png
         *
         * \see DrawMode, setDrawMode(), getDrawMode()
         */
    DrawMode m_drawMode;

};


/** \brief this is the virtual base class of all JKQTPPlotElement's in a JKQTPlotter plot that
 *         represent annotations of a plot. It differs from JKQTPGeometricPlotElement by not providing a
 *         selection of drawModes, as annotations are always drawn as graphic elements.
 *  \ingroup jkqtplotter_basegraphs
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPPlotAnnotationElement: public JKQTPPlotElement {
    Q_OBJECT
public:

    /** \brief class constructor */
    explicit JKQTPPlotAnnotationElement(JKQTBasePlotter* parent=nullptr);

    /** \brief default wirtual destructor */
    virtual ~JKQTPPlotAnnotationElement() ;

public Q_SLOTS:

protected:

};



/** \brief This virtual JKQTPGraph descendent may be used as base for all graphs that use at least two columns
 *         that specify x and y coordinates for the single plot points.
 *  \ingroup jkqtplotter_basegraphs
 *
 *  This class implements basic management facilities for the data columns:
 *    - setXColumn(), setYColumn() to set the columns to be used for the graph data
 *    - setDataSortOrder() to specify whether and how the data should be sorted before drawing
 *      \image html jkqtplotter_unsorted.png "Unsorted Data"
 *      \image html jkqtplotter_sortedx.png "Data sorted along x-axis (DataSortOrder::SortedX)"
 *  .
 *
 *  ... and overrides/implements the functions:
 *    - getXMinMax()
 *    - getYMinMax()
 *    - usesColumn()
 *  .
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYGraph: public JKQTPGraph {
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
    Q_ENUM(DataSortOrder)


    /** \brief class constructor */
    JKQTPXYGraph(JKQTBasePlotter* parent=nullptr);

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

    /** \copydoc xColumn */
    int getXColumn() const;
    /** \copydoc yColumn */
    int getYColumn() const;
    /** \copydoc sortData */
    DataSortOrder getDataSortOrder() const;
    /** \brief returns the column used as "key" for the current graph (typically this call getXColumn(), but for horizontal graphs like filled curves or barcharts it may call getYColumn() ) */
    virtual int getKeyColumn() const;
    /** \brief returns the column used as "value" for the current graph (typically this call getXColumn(), but for horizontal graphs like filled curves or barcharts it may call getYColumn() ) */
    virtual int getValueColumn() const;

    Q_PROPERTY(DataSortOrder sortData READ getDataSortOrder WRITE setDataSortOrder)
    Q_PROPERTY(int xColumn READ getXColumn WRITE setXColumn)
    Q_PROPERTY(int yColumn READ getYColumn WRITE setYColumn)


    /** \brief Implmentation of JKQTPPlotElement::hitTest(), which searches through all graph points defined by xColumn and yColumn
     *         and returns a general x/y-label, also taking into account possibly known errors to the graphs (if it is derived
     *         from JKQTPXGraphErrorData and/or JKQTPYGraphErrorData
     *
     * \note This function first checks whether JKQTPPlotElement::hitTest() returns any result, so you can use the basic implementation
     *       in JKQTPPlotElement to override the behaviour here, by simply calling addHitTestData() during your draw() implementation
     *
     * \see See JKQTPPlotElement::hitTest() for details on the function definition!
     */
    virtual double hitTest(const QPointF &posSystem, QPointF* closestSpotSystem=nullptr, QString* label=nullptr, HitTestMode mode=HitTestXY) const override;
public Q_SLOTS:
    /** \brief sets xColumn and yColumn at the same time */
    void setXYColumns(size_t xCol, size_t yCol);
    /** \brief sets xColumn and yColumn at the same time */
    void setXYColumns(int xCol, int yCol);
#if QT_VERSION<QT_VERSION_CHECK(6,0,0)
    /** \brief sets xColumn and yColumn at the same time */
    void setXYColumns(std::pair<int,int> xyColPair);
    /** \brief sets xColumn and yColumn at the same time */
    void setXYColumns(std::pair<size_t,size_t> xyColPair);
#endif
    /** \brief sets xColumn and yColumn at the same time */
    void setXYColumns(QPair<int,int> xyColPair);
    /** \brief sets xColumn and yColumn at the same time */
    void setXYColumns(QPair<size_t,size_t> xyColPair);
    /** \copydoc sortData */
    void setDataSortOrder(int __value);
    /** \copydoc sortData */
    void setDataSortOrder(DataSortOrder  __value);
    /** \copydoc xColumn */
    void setXColumn(int __value);
    /** \copydoc xColumn */
    void setXColumn (size_t __value);
    /** \copydoc yColumn */
    void setYColumn(int __value);
    /** \copydoc yColumn */
    void setYColumn (size_t __value);
    /** \brief sets the column used as "key" for the current graph (typically this call setXColumn(), but for horizontal graphs like filled curves or barcharts it may call setYColumn() ) */
    virtual void setKeyColumn(int __value);
    /** \brief sets the column used as "value" for the current graph (typically this call setXColumn(), but for horizontal graphs like filled curves or barcharts it may call setYColumn() ) */
    virtual void setValueColumn(int __value);
protected:

    /** \brief the column that contains the x-component of the datapoints */
    int xColumn;
    /** \brief the column that contains the y-component of the datapoints */
    int yColumn;

    /** \brief if \c !=Unsorted, the data is sorted before plotting */
    DataSortOrder sortData;
    /** \brief this array contains the order of indices, in which to access the data in the data columns */
    QVector<int> sortedIndices;
    /** \brief sorts data according to the specified criterion in \a sortData ... The result is stored as a index-map in sorted Indices */
    virtual void intSortData();
    /** \brief returns the index of the i-th datapoint (where i is an index into the SORTED datapoints)
     *
     * This function can beu used to get the correct datapoint after sorting the datapoints,
     * As sorting is done by sorting an index and not reordering the data in the columns themselves.
     *
     * \see setDataSortOrder(), getDataSortOrder()
     * */
    inline int getDataIndex(int i) const {
        if (sortData==Unsorted) return i;
        return sortedIndices.value(i,i);
    }

    /** \brief determines the range of row indexes available in the data columns of this graph
     *
     * \param[out] imin first usable row-index
     * \param[out] imax last usable row-index
     *  \return \c true on success and \c false if the information is not available
     */
    virtual bool getIndexRange(int &imin, int &imax) const;
};



/** \brief This virtual JKQTPGraph descendent extends JKQTPXYGraph adds a baseline-property, which is necessary, e.g. for barcharts, filled graphs to indicate until where to draw the bar or fill the curve (default is 0).
 *  \ingroup jkqtplotter_basegraphs
 *
 *  \see JKQTPSpecialLineHorizontalGraph, JKQTPBarVerticalGraph, JKQTPImpulsesHorizontalGraph, ...
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYBaselineGraph: public JKQTPXYGraph {
    Q_OBJECT
public:

    /** \brief class constructor */
    JKQTPXYBaselineGraph(JKQTBasePlotter* parent=nullptr);

    /** \copydoc m_baseline */
    double getBaseline() const;

    Q_PROPERTY(double baseline READ getBaseline WRITE setBaseline)
public Q_SLOTS:
    /** \copydoc m_baseline */
    void setBaseline(double __value);

protected:
    /** \brief can be called by JKQTPGraph::getXMinMax() or JKQTPGraph::getYMinMax() calculates min/max/... for data from the given column, including the baseline */
    bool getMinMaxWithBaseline(int dataColumn, double &minv, double &maxv, double &smallestGreaterZero);
    /** \brief can be called by JKQTPGraph::getXMinMax() or JKQTPGraph::getYMinMax() calculates min/max/... for data from the given column, including the baseline and an optional error */
    bool getMinMaxWithErrorsAndBaseline(int dataColumn, int errorColumn, int errorColumnLower, bool errorSymmetric, double &minv, double &maxv, double &smallestGreaterZero);


    /** \brief baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!)
     *
     * \image html impulsesplot_baseline.png
     */
    double m_baseline;

private:
    /** \brief returns the upper error for the i-th datapoint, read from datastore \a ds */
    double getErrorU(int i, const JKQTPDatastore* ds, int xErrorColumn) const;
    /** \brief returns the lower error for the i-th datapoint, read from datastore \a ds */
    double getErrorL(int i, const JKQTPDatastore *ds, int xErrorColumn, int xErrorColumnLower, bool xErrorSymmetric) const;

};


/** \brief This virtual JKQTPGraph descendent extends JKQTPXYGraph to two columns for y-values (e.g. for filled range plots in JKQTPFilledVerticalRangeGraph).
 *  \ingroup jkqtplotter_basegraphs
 *
 *  \see JKQTPXXYGraph and e.g. JKQTPFilledVerticalRangeGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYYGraph: public JKQTPXYGraph {
    Q_OBJECT
public:

    /** \brief class constructor */
    JKQTPXYYGraph(JKQTBasePlotter* parent=nullptr);


    /** \brief get the maximum and minimum y-value of the graph
     *
     * The result is given in the two parameters which are call-by-reference parameters!
     */
    virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

    /** \copydoc JKQTPGraph::usesColumn() */
    virtual bool usesColumn(int column) const override;

    /** \copydoc yColumn2 */
    int getYColumn2() const;
    /** \brief returns the column used as secondary "value" for the current graph (typically this call getXColumn(), but for horizontal graphs like filled curves or barcharts it may call getYColumn() ) */
    virtual int getValue2Column() const;

    /** \copydoc JKQTPXYGraph::hitTest() */
    virtual double hitTest(const QPointF &posSystem, QPointF* closestSpotSystem=nullptr, QString* label=nullptr, HitTestMode mode=HitTestXY) const override;

    Q_PROPERTY(int yColumn2 READ getYColumn2 WRITE setYColumn2)
public Q_SLOTS:
    /** \brief sets xColumn, yColumn and yColumn2 at the same time */
    void setXYYColumns(size_t xCol, size_t yCol, size_t y2Col);
    /** \brief sets xColumn, yColumn and yColumn2 at the same time */
    void setXYYColumns(int xCol, int yCol, int y2Col);


    /** \copydoc yColumn2 */
    void setYColumn2(int __value);
    /** \copydoc yColumn2 */
    void setYColumn2(size_t __value);
    /** \brief sets the column used as secondary "value" for the current graph (typically this call setXColumn(), but for horizontal graphs like filled curves or barcharts it may call setYColumn() ) */
    virtual void setValue2Column(int __value);
protected:

    /** \brief the column that contains the second y-component of the datapoints */
    int yColumn2;


    /** \brief determines the range of row indexes available in the data columns of this graph
     *
     * \param[out] imin first usable row-index
     * \param[out] imax last usable row-index
     *  \return \c true on success and \c false if the information is not available
     */
    virtual bool getIndexRange(int &imin, int &imax) const override;
};



/** \brief This virtual JKQTPGraph descendent extends JKQTPXYGraph to two columns for x-values (e.g. for filled range plots).
 *  \ingroup jkqtplotter_basegraphs
 *
 *  \see JKQTPXYYGraph and e.g. JKQTPFilledVerticalRangeGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXXYGraph: public JKQTPXYGraph {
    Q_OBJECT
public:

    /** \brief class constructor */
    JKQTPXXYGraph(JKQTBasePlotter* parent=nullptr);


    /** \brief get the maximum and minimum x-value of the graph
     *
     * The result is given in the two parameters which are call-by-reference parameters!
     */
    virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;

    /** \copydoc JKQTPGraph::usesColumn() */
    virtual bool usesColumn(int column) const override;

    /** \copydoc xColumn2 */
    int getXColumn2() const;
    /** \brief returns the column used as "key" for the current graph (typically this call getXColumn(), but for horizontal graphs like filled curves or barcharts it may call getYColumn() ) */
    virtual int getKey2Column() const;

    /** \copydoc JKQTPXYGraph::hitTest() */
    virtual double hitTest(const QPointF &posSystem, QPointF* closestSpotSystem=nullptr, QString* label=nullptr, HitTestMode mode=HitTestXY) const override;

    Q_PROPERTY(int xColumn2 READ getXColumn2 WRITE setXColumn2)
public Q_SLOTS:
    /** \brief sets xColumn, yColumn and xColumn2 at the same time */
    void setXXYColumns(size_t xCol, size_t x2Col, size_t yCol);
    /** \brief sets xColumn, yColumn and xColumn2 at the same time */
    void setXXYColumns(int xCol, int x2Col, int yCol);


    /** \copydoc xColumn2 */
    void setXColumn2(int __value);
    /** \copydoc xColumn2 */
    void setXColumn2(size_t __value);
    /** \brief sets the column used as "key" for the current graph (typically this call setXColumn(), but for horizontal graphs like filled curves or barcharts it may call setYColumn() ) */
    virtual void setKey2Column(int __value);
protected:

    /** \brief the column that contains the second y-component of the datapoints */
    int xColumn2;


    /** \brief determines the range of row indexes available in the data columns of this graph
     *
     * \param[out] imin first usable row-index
     * \param[out] imax last usable row-index
     *  \return \c true on success and \c false if the information is not available
     */
    virtual bool getIndexRange(int &imin, int &imax) const override;
};





/** \brief This virtual JKQTPGraph descendent may be used as base for all graphs that use at least one column
 *         of data
 *  \ingroup jkqtplotter_basegraphs
 *
 *
 *  \see \ref jkqtplotter_graphsgroup_classstructure
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSingleColumnGraph: public JKQTPGraph {
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
        Q_ENUM(DataSortOrder)

        /** \brief specifies whether the data for a JKQTPSingleColumnGraph represent x-axis or y-axis values */
        enum class DataDirection {
            X, /*!< \brief the data for a JKQTPSingleColumnGraph is data belonging to the x-axis of the plot */
            Y /*!< \brief the data for a JKQTPSingleColumnGraph is data belonging to the y-axis of the plot */
        };
        Q_ENUM(DataDirection)

        /** \brief class constructor */
        JKQTPSingleColumnGraph(JKQTBasePlotter* parent=nullptr);

        /** \copydoc dataColumn */
        int getDataColumn() const;

        /** \copydoc sortData */
        DataSortOrder getDataSortOrder() const;


        /** \copydoc dataDirection */
        DataDirection getDataDirection() const;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

        Q_PROPERTY(DataSortOrder sortData READ getDataSortOrder WRITE setDataSortOrder)
        Q_PROPERTY(int dataColumn READ getDataColumn WRITE setDataColumn)
        Q_PROPERTY(DataDirection dataDirection READ getDataDirection WRITE setDataDirection)

    public Q_SLOTS:
        /** \copydoc dataColumn */
        void setDataColumn(int __value);
        /** \copydoc dataColumn */
        void setDataColumn (size_t __value);
        /** \copydoc dataDirection */
        void setDataDirection(DataDirection __value);
        /** \copydoc sortData */
        void setDataSortOrder(int __value);
        /** \copydoc sortData */
        void setDataSortOrder(DataSortOrder  __value);

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
        inline int getDataIndex(int i) const {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }

        /** \brief determines the range of row indexes available in the data columns of this graph
         *
         * \param[out] imin first usable row-index
         * \param[out] imax last usable row-index
         *  \return \c true on success and \c false if the information is not available
         */
        virtual bool getIndexRange(int &imin, int &imax) const;

};




#endif // JKQTPGRAPHSBASE_H
