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

#ifndef jkqtpgeolines_H_INCLUDED
#define jkqtpgeolines_H_INCLUDED


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpgeobase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"



/** \brief This JKQTPGeometricPlotElement is used to draw a line, optionally line-end decorations (aka arrows) are pssible, but switched off by default.
 *  \ingroup jkqtplotter_geoplots
 *
 *  \image html JKQTPGeoLine_HeadTail.png
 *
 *
 *  \image html plot_geoline.png
 *
 *  \note This class support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
 *        and non-linear axes are chosen, the line is drawn as a curves, instead of straight
 *        a straight line. In the mode
 *        JKQTPGeometricPlotElement::DrawMode::DrawAsGraphicElement the line is drawn as a straight line.
 *
 *  You can also activate line-end decorators (aka arrows) for this poly-line, by using code like this:
 *  \code
 *      line->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
 *      line->setTailDecoratorStyle(JKQTPCircleDecorator);
 *  \endcode
 *
 *  This results in arrows drawn at the start (aka x1/y1, =tail) and end (aka x2/y2, =head) of the line.
 *
 *
 *  \note The convenience class JKQTPGeoArrow activates line-end decorations (aka arows) by default and allows to select them in the constructor.
 *
 *  \see JKQTPGeoArrow, \ref JKQTPlotterGeometricGraphs and \ref JKQTPlotterGeometricArrows
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoLine: public JKQTPGeoBaseDecoratedLine {
        Q_OBJECT
    public:
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x1 x-coordinate of first point of line
         *  \param y1 y-coordinate of first point of line
         *  \param x2 x-coordinate of second point of line
         *  \param y2 y-coordinate of second point of line
         *
         */
        JKQTPGeoLine(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x1 x-coordinate of first point of line
         *  \param y1 y-coordinate of first point of line
         *  \param x2 x-coordinate of second point of line
         *  \param y2 y-coordinate of second point of line
         */
        JKQTPGeoLine(JKQTPlotter* parent, double x1, double y1, double x2, double y2);




        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param p1 first point of line (tail)
         *  \param p2 second point of line (head)
         *
         */
        JKQTPGeoLine(JKQTBasePlotter* parent, const QPointF& p1, const QPointF& p2);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param p1 first point of line (tail)
         *  \param p2 second point of line (head)
         */
        JKQTPGeoLine(JKQTPlotter* parent, const QPointF& p1, const QPointF& p2);




        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param line the line
         *
         */
        JKQTPGeoLine(JKQTBasePlotter* parent, const QLineF& line);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param line the line
         */
        JKQTPGeoLine(JKQTPlotter* parent, const QLineF& line);



        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
         *       and non-linear axes are chosen, the line will possibly be represented as a curve,
         *       instead of a straight line.
         */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc x1 */
        void setX1(double __value);
        /** \copydoc x1 */
        double getX1() const;
        /** \copydoc y1 */
        void setY1(double __value);
        /** \copydoc y1 */
        double getY1() const;
        /** \copydoc x2 */
        void setX2(double __value);
        /** \copydoc x2 */
        double getX2() const;
        /** \copydoc y2 */
        void setY2(double __value);
        /** \copydoc y2 */
        double getY2() const;

        /** \brief return line-start/tail (x1,y1) as QPointF */
        QPointF getP1() const;
        /** \brief set line-start/tail (x1,y1) as QPointF */
        void setP1(const QPointF& p);
        /** \brief return line-end/head (x2,y2) as QPointF */
        QPointF getP2() const;
        /** \brief set line-end/head (x2,y2) as QPointF */
        void setP2(const QPointF& p);
        /** \brief return the line as a QLineF object */
        QLineF getLine() const;
        /** \brief define the line as a QLineF object */
        void setLine(const QLineF& line);
    protected:
        /** \brief x-coordinate of first point of line */
        double x1;
        /** \brief y-coordinate of first point of line */
        double y1;
        /** \brief x-coordinate of second point of line */
        double x2;
        /** \brief y-coordinate of second point of line */
        double y2;

};


/** \brief This convenience specialisation of JKQTPGeoLine is used to draw a line with decorations (e.g. arrows)
 *  \ingroup jkqtplotter_geoplots
 *
 *  This class does not add any functionality on top of JKQTPGeoLine, just activates line-end markers by default!
 *
 *  \image html JKQTPGeoLine_HeadTail.png
 *
 *  \image html plot_geoarrows.png
 *
 *  \see JKQTPLineDecoratorStyle, JKQTPGeoLine, \ref JKQTPlotterGeometricArrows and \ref JKQTPlotterGeometricGraphs
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoArrow: public JKQTPGeoLine {
    Q_OBJECT
public:
    /** \brief class constructor
      *
      * \param parent the parent plotter object
      * \param x1 x-coordinate of first point of line
      * \param y1 y-coordinate of first point of line
      * \param x2 x-coordinate of second point of line
      * \param y2 y-coordinate of second point of line
      * \param headStyle decorator/arrow style of the head
      * \param tailStyle decorator/arrow style of the tail
    */
    JKQTPGeoArrow(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator);
    /** \brief class constructor
      *
      * \param parent the parent plotter object
      * \param x1 x-coordinate of first point of line
      * \param y1 y-coordinate of first point of line
      * \param x2 x-coordinate of second point of line
      * \param y2 y-coordinate of second point of line
      * \param headStyle decorator/arrow style of the head
      * \param tailStyle decorator/arrow style of the tail
      */
    JKQTPGeoArrow(JKQTPlotter* parent, double x1, double y1, double x2, double y2, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator);
    /** \brief class constructor
      *
      *  \param parent the parent plotter object
      *  \param p1 first point of line (tail)
      *  \param p2 second point of line (head)
      *  \param headStyle decorator/arrow style of the head
      *  \param tailStyle decorator/arrow style of the tail
      */
    JKQTPGeoArrow(JKQTBasePlotter* parent, const QPointF& p1, const QPointF& p2, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator);
    /** \brief class constructor
      *
      *  \param parent the parent plotter object
      *  \param p1 first point of line (tail)
      *  \param p2 second point of line (head)
      *  \param headStyle decorator/arrow style of the head
      *  \param tailStyle decorator/arrow style of the tail
      */
    JKQTPGeoArrow(JKQTPlotter* parent, const QPointF& p1, const QPointF& p2, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator);




    /** \brief class constructor
      *
      *  \param parent the parent plotter object
      *  \param line the line
      *  \param headStyle decorator/arrow style of the head
      *  \param tailStyle decorator/arrow style of the tail
      *
      */
    JKQTPGeoArrow(JKQTBasePlotter* parent, const QLineF& line, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator);
    /** \brief class constructor
      *
      *  \param parent the parent plotter object
      *  \param line the line
      *  \param headStyle decorator/arrow style of the head
      *  \param tailStyle decorator/arrow style of the tail
      */
    JKQTPGeoArrow(JKQTPlotter* parent, const QLineF& line, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator);
};





/** \brief This JKQTPGeometricPlotElement is used to draw an infinite line
 *  \ingroup jkqtplotter_geoplots
 *
 *  an infinite line has a starting point and then goes on in a given direction
 *  until the end of the plotting range.
 *
 *  \image html plot_geoinfiniteline.png
 *
 *
 *  \note This class support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
 *        and non-linear axes are chosen, the line is drawn as a curves, instead of straight
 *        a straight line. In the mode
 *        JKQTPGeometricPlotElement::DrawMode::DrawAsGraphicElement the line is drawn as a straight line.
 *
 *  You can add a decorator to the head of the line (i.e. the given start point (x,y) ) iff this line is one-sided, i.e. two_sided \c ==false .
 *
 *  \see \ref JKQTPlotterGeometricGraphs and \ref JKQTPlotterGeometricArrows
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoInfiniteLine: public JKQTPGeoBaseDecoratedHeadLine {
        Q_OBJECT
    public:
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of start point of line
         *  \param y y-coordinate of start point of line
         *  \param dx x-direction of the line
         *  \param dy y-direction of the line
         */
        JKQTPGeoInfiniteLine(JKQTBasePlotter* parent, double x, double y, double dx, double dy);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of start point of line
         *  \param y y-coordinate of start point of line
         *  \param dx x-direction of the line
         *  \param dy y-direction of the line
         */
        JKQTPGeoInfiniteLine(JKQTPlotter* parent, double x, double y, double dx, double dy);


        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
         *       and non-linear axes are chosen, the line will possibly be represented as a curve,
         *       instead of a straight line.
         */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc x */
        void setX(double __value);
        /** \copydoc x */
        double getX() const;
        /** \copydoc y */
        void setY(double __value);
        /** \copydoc y */
        double getY() const;
        /** \copydoc dx */
        void setDx(double __value);
        /** \copydoc dx */
        double getDx() const;
        /** \copydoc dy */
        void setDy(double __value);
        /** \copydoc dy */
        double getDy() const;
        /** \copydoc two_sided */
        void setTwoSided(bool __value);
        /** \copydoc two_sided */
        bool getTwoSided() const;
        /** \brief return point (x,y) on the line as QPointF */
        QPointF getP() const;
        /** \brief set point (x,y) on the line as QPointF */
        void setP(const QPointF& p);
    protected:
        /** \brief x-coordinate of a point on the line */
        double x;
        /** \brief y-coordinate of a point on the line */
        double y;
        /** \brief x-component of the slope of the line */
        double dx;
        /** \brief y-component of the slope of the line */
        double dy;
        /** \brief indicates whether the line ends at the given point \f$ (x,y) \f$ (false, default),
         *         or is infinite in both directions (true) */
        bool two_sided;
};


/** \brief This JKQTPGeometricPlotElement is used to draw a poly line
 *  \ingroup jkqtplotter_geoplots
 *
 *  \image html plot_geolines.png
 *
 *  \note This class support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
 *        and non-linear axes are chosen, the points of the poly-line will be possibly
 *        connected by curves, instead of straight lines. In the mode
 *        JKQTPGeometricPlotElement::DrawMode::DrawAsGraphicElement the points are connected by straight
 *        lines, independent of the linearity or non-linearity of the coordinate axes.
 *
 *  You can also activate line-end decorators (aka arrows) for this poly-line, by using code like this:
 *  \code
 *      polyLine->setHeadDecoratorStyle(JKQTPFilledDoubleArrow);
 *      polyLine->setTailDecoratorStyle(JKQTPCircleDecorator);
 *  \endcode
 *
 *  This results in arrows drawn at the start (=tail) and end (=head) of the poly-line:
 *
 *  \image html geo_arrow_polylines.png
 *
 *  \see \ref JKQTPlotterGeometricGraphs and \ref JKQTPlotterGeometricArrows
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoPolyLines: public JKQTPGeoBaseDecoratedLine {
        Q_OBJECT
    public:
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param points points on the polygon
         */
        JKQTPGeoPolyLines(JKQTBasePlotter* parent, const QVector<QPointF>& points);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param points points on the polygon
         */
        JKQTPGeoPolyLines(JKQTPlotter* parent, const QVector<QPointF>& points);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         */
        JKQTPGeoPolyLines(JKQTBasePlotter* parent);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         */
        JKQTPGeoPolyLines(JKQTPlotter* parent);


        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
         *       and non-linear axes are chosen, the points of the poly-line will be possibly
         *       connected by curves, instead of straight lines. In the mode
         *       JKQTPGeometricPlotElement::DrawMode::DrawAsGraphicElement the points are connected by straight
         *       lines, independent of the linearity or non-linearity of the coordinate axes.
         */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc points */
        void setPoints(const QVector<QPointF> & __value);
        /** \copydoc points */
        QVector<QPointF> getPoints() const;

        /** \brief append a point to the polygon \see points */
        void appendPoint(const QPointF& p);

        /** \brief append a point to the polygon \see points */
        void appendPoint(const double x, const double y);

        /** \brief returns the number of points \see points */
        int getPointCount() const;
        /** \brief returns the i-th of point \see points */
        const QPointF& getPoint(int i) const;
        /** \brief returns the i-th of point \see points */
        QPointF& getPoint(int i);
        /** \brief overwrites the i-th point \see points */
        void setPoint(int i, const QPointF& point);
        /** \brief deletes the i-the point \see points */
        void removePoint(int i);
        /** \brief returns an iterator to the first point \see points */
        QVector<QPointF>::iterator pointsBegin();
        /** \brief returns an iterator after the last point \see points */
        QVector<QPointF>::iterator pointsEnd();
        /** \brief returns a constant iterator to the first point \see points */
        QVector<QPointF>::const_iterator pointsCBegin() const;
        /** \brief returns a constant iterator after the last point \see points */
        QVector<QPointF>::const_iterator pointsCEnd() const;
    protected:
        /** \brief list with all points on the poly-line */
        QVector<QPointF> points;
};


/** \brief This JKQTPGeometricPlotElement is used to draw an arc
 *  \ingroup jkqtplotter_geoplots
 *
 *
 *  A pie on linear and log-log axes (start angle: 10deg and end-angle 110deg):
 *
 *  \image html plot_geopie.png
 *
 *  \note This class support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
 *        This means that arcs are always treated as mathematical curves, as no meaningful
 *        parametrization for arcs on non-linear axes could be found!
 *
 *  \see \ref JKQTPlotterGeometricGraphs, JKQTPGeoChord, JKQTPGeoPie, JKQTPGeoEllipse
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoArc: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /** \brief class constructor

         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of ellipse
         *  \param y y-coordinate of center of ellipse
         *  \param width width of ellipse (2 * half axis)
         *  \param height of ellipse (2 * half axis)
         *  \param angleStart if we only draw an arc, this is the starting angle in degrees
         *  \param angleStop if we only draw an arc, this is the ending angle in degrees

         */
        JKQTPGeoArc(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop);
        /** \brief class constructor

         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of ellipse
         *  \param y y-coordinate of center of ellipse
         *  \param width width of ellipse (2 * half axis)
         *  \param height of ellipse (2 * half axis)
         *  \param angleStart if we only draw an arc, this is the starting angle in degrees
         *  \param angleStop if we only draw an arc, this is the ending angle in degrees

         */
        JKQTPGeoArc(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop);

        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
         *       This means that arcs are always treated as mathematical curves, as no meaningful
         *       parametrization for arcs on non-linear axes could be found!
         */
        virtual void draw(JKQTPEnhancedPainter& painter) override;


        /** \copydoc angleStart */
        void setAngleStart(double __value);
        /** \copydoc angleStart */
        double getAngleStart() const;
        /** \copydoc angleStop */
        void setAngleStop(double __value);
        /** \copydoc angleStop */
        double getAngleStop() const;
        /** \copydoc x */
        void setX(double __value);
        /** \copydoc x */
        double getX() const;
        /** \copydoc y */
        void setY(double __value);
        /** \copydoc y */
        double getY() const;
        /** \brief returns the center point of the arc */
        QPointF getCenter() const;
        /** \brief sets the center point of the arc */
        void setCenter(const QPointF& center);
        /** \copydoc width */
        void setWidth(double __value);
        /** \copydoc width */
        double getWidth() const;
        /** \copydoc height */
        void setHeight(double __value);
        /** \copydoc height */
        double getHeight() const;
        /** \brief returns the size (width and height) of the arc */
        QSizeF getSize() const;
        /** \brief sets the size (width and height) of the arc */
        void setSize(const QSizeF& size);
        /** \copydoc angle */
        void setAngle(double __value);
        /** \copydoc angle */
        double getAngle() const;
    protected:
        /** \brief x-coordinate of a center of the rectangle */
        double x;
        /** \brief y-coordinate of a center of the rectangle */
        double y;
        /** \brief width of a center of the rectangle */
        double width;
        /** \brief height of a center of the rectangle */
        double height;
        /** \brief rotation angle of rectangle [degrees] around (x,y) */
        double angle;
        /** \brief if we only draw an arc, this is the starting angle in degrees */
        double angleStart;
        /** \brief if we only draw an arc, this is the ending angle in degrees */
        double angleStop;


        /** \brief closing mode for arcs: secand or pie */
        /** \brief returns the transformation matrix used for this rectangle */
        QTransform getTransform();
        /** \brief returns a QPolygonF which represents the rectangle after rotation, but still in the world coordinate system, not in the screen/widget system */
        QPolygonF getPolygon();

};




#endif // jkqtpgeolines_H_INCLUDED
