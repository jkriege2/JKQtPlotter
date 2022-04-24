/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#ifndef jkqtpgeoshapes_H_INCLUDED
#define jkqtpgeoshapes_H_INCLUDED


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpgeobase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"




/** \brief This JKQTPGeometricPlotElement is used to draw a rectangle
 *  \ingroup jkqtplotter_geoplots
 *
 *  \image html plot_georectangle.png
 *
 *  \note This class support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
 *        and non-linear axes are chosen, the corner points of the rectangle will be possibly
 *        connected by curves, instead of straight lines. In the mode
 *        JKQTPGeometricPlotElement::DrawMode::DrawAsGraphicElement the corners are connected by straight
 *        lines, independent of the linearity or non-linearity of the coordinate axes.
 *
 *  \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoRectangle: public JKQTPGeoBaseFilled {
        Q_OBJECT
    public:



        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of rectangle
         *  \param y y-coordinate of center of rectangle
         *  \param width width of rectangle
         *  \param height of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoRectangle(JKQTBasePlotter* parent, double x, double y, double width, double height, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of rectangle
         *  \param y y-coordinate of center of rectangle
         *  \param width width of rectangle
         *  \param height of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoRectangle(JKQTPlotter* parent, double x, double y, double width, double height, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param bottomleft bottom left corner of rectangle
         *  \param topright top right corner of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoRectangle(JKQTBasePlotter* parent, QPointF bottomleft, QPointF topright, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param bottomleft bottom left corner of rectangle
         *  \param topright top right corner of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoRectangle(JKQTPlotter* parent, QPointF bottomleft, QPointF topright, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param center center of rectangle
         *  \param size width and heigt of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoRectangle(JKQTBasePlotter* parent, const QPointF& center, const QSizeF& size, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param center center of rectangle
         *  \param size width and heigt of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoRectangle(JKQTPlotter* parent, const QPointF& center, const QSizeF& size, DrawMode drawMode=DrawAsGraphicElement);





        /** \copydoc JKQTPGeometricPlotElement::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPGeometricPlotElement::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
         *       and non-linear axes are chosen, the corner points of the rectangle will be possibly
         *       connected by curves, instead of straight lines.
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
        /** \brief returns the center point of the rectangle */
        QPointF getCenter() const;
        /** \brief sets the center point of the rectangle */
        void setCenter(const QPointF& center);
        /** \copydoc width */ 
        void setWidth(double __value);
        /** \copydoc width */ 
        double getWidth() const;
        /** \copydoc height */ 
        void setHeight(double __value);
        /** \copydoc height */ 
        double getHeight() const;
        /** \brief returns the size (width and height) of the rectangle */
        QSizeF getSize() const;
        /** \brief sets the size (width and height) of the rectangle */
        void setSize(const QSizeF& size);
        /** \copydoc angle */
        void setAngle(double __value);
        /** \copydoc angle */ 
        double getAngle() const;
        /** \brief set the rectangle using the bottom-left corner, as well as its width and height */
        void setBottomLeftRectangle(double x, double y, double width, double height);
        /** \brief set the rectangle using the bottom-left corner, as well as its width and height */
        void setBottomLeftRectangle(const QRectF& rect);
        /** \brief returns the rectangle using the bottom-left corner, as well as its width and height, ignoring the rotation angle!!!
         *
         * This is equivalent to
         * \code
         *   QRectF result(x-width/2.0, y-height/2.0, width, height);
         * \endcode
         */
        QRectF getBottomLeftRectangle() const;

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
        /** \brief returns the transformation matrix used for this rectangle */
        QTransform getTransform();
        /** \brief returns a QPolygonF which represents the rectangle after rotation, but still in the world coordinate system, not in the screen/widget system */
        QPolygonF getPolygon();
};


/** \brief This JKQTPGeometricPlotElement is used to draw a polygon
 *  \ingroup jkqtplotter_geoplots
 *
 *  \image html plot_geopolygon.png
 *
 *  \note This function support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
 *        and non-linear axes are chosen, the points of the polygon will be possibly
 *        connected by curves, instead of straight lines.
 *
 *  \see \ref JKQTPlotterGeometricGraphs
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoPolygon: public JKQTPGeoBaseFilled {
        Q_OBJECT
    public:
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param points points on the polygon
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoPolygon(JKQTBasePlotter* parent, const QVector<QPointF>& points, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param points points on the polygon
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoPolygon(JKQTPlotter* parent, const QVector<QPointF>& points, DrawMode drawMode=DrawAsGraphicElement);

        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoPolygon(JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoPolygon(JKQTPlotter* parent, DrawMode drawMode=DrawAsGraphicElement);


        /** \copydoc JKQTPGeometricPlotElement::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPGeometricPlotElement::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve. If set,
         *       and non-linear axes are chosen, the points of the polygon will be possibly
         *       connected by curves, instead of straight lines.
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
        /** \brief returns a constant iterator after the last point  \see points*/
        QVector<QPointF>::const_iterator pointsCEnd() const;

    protected:
        /** \brief list with all points on the polygon */
        QVector<QPointF> points;
};

/** \brief This JKQTPGeometricPlotElement is used to draw an ellipse
 *  \ingroup jkqtplotter_geoplots
 *
 *  \image html plot_geoellipse.png
 *
 *  \image html plot_geoellipse_rotated.png
 *
 *  \note This function support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
 *        This means that ellipses are always treated as mathematical curves, as no meaningful
 *        parametrization for ellipses on non-linear axes could be found! In the mode
 *        JKQTPGeometricPlotElement::DrawMode::DrawAsGraphicElement the points are connected by straight
 *        lines, independent of the linearity or non-linearity of the coordinate axes.
 *
 *  \see <a href="http://www.codeguru.com/cpp/g-m/gdi/article.php/c131">http://www.codeguru.com/cpp/g-m/gdi/article.php/c131</a> and
 *       <a href="http://en.wikipedia.org/wiki/Ellipse#General_parametric_form">http://en.wikipedia.org/wiki/Ellipse#General_parametric_form</a>
 *
 *  \see \ref JKQTPlotterGeometricGraphs, JKQTPGeoPie, JKQTPGeoChord, JKQTPGeoArc
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoEllipse: public JKQTPGeoRectangle {
        Q_OBJECT
    public:



        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of rectangle
         *  \param y y-coordinate of center of rectangle
         *  \param width width of rectangle
         *  \param height of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoEllipse(JKQTBasePlotter* parent, double x, double y, double width, double height, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of rectangle
         *  \param y y-coordinate of center of rectangle
         *  \param width width of rectangle
         *  \param height of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoEllipse(JKQTPlotter* parent, double x, double y, double width, double height, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param bottomleft bottom left corner of rectangle
         *  \param topright top right corner of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoEllipse(JKQTBasePlotter* parent, QPointF bottomleft, QPointF topright, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param bottomleft bottom left corner of rectangle
         *  \param topright top right corner of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoEllipse(JKQTPlotter* parent, QPointF bottomleft, QPointF topright, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param center center of rectangle
         *  \param size width and heigt of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoEllipse(JKQTBasePlotter* parent, const QPointF& center, const QSizeF& size, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param center center of rectangle
         *  \param size width and heigt of rectangle
         *
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoEllipse(JKQTPlotter* parent, const QPointF& center, const QSizeF& size, DrawMode drawMode=DrawAsGraphicElement);




        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
         *       This means that ellipses are always treated as mathematical curves, as no meaningful
         *       parametrization for ellipses on non-linear axes could be found!*/
        virtual void draw(JKQTPEnhancedPainter& painter) override;

    protected:

        /** \brief drawing modes for drawInternal() */
        enum class InternalDrawMode {
            Ellipse,
            Pie,
            Chord
        };

        /** \brief inertnal draw function, can draw an ellipse, a pie or a chord! */
        void drawInternal(JKQTPEnhancedPainter& painter, double angleStart=0, double angleStop=0, InternalDrawMode mode=InternalDrawMode::Ellipse);
};




/** \brief This JKQTPGeometricPlotElement is used to draw a pie
 *  \ingroup jkqtplotter_geoplots
 *
 *
 *  A pie on linear and log-log axes (start angle: 10deg and end-angle 110deg):
 *  \image html plot_geopie.png
 *
 *  \note This function support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
 *        This means that pies are always treated as mathematical curves, as no meaningful
 *        parametrization for pies on non-linear axes could be found!
 *
 *  \see \ref JKQTPlotterGeometricGraphs, JKQTPGeoEllipse, JKQTPGeoChord, JKQTPGeoArc
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoPie: public JKQTPGeoEllipse {
        Q_OBJECT
    public:
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of ellipse
         *  \param y y-coordinate of center of ellipse
         *  \param width width of ellipse (2 * half axis)
         *  \param height of ellipse (2 * half axis)
         *  \param angleStart if we only draw an arc, this is the starting angle in degrees
         *  \param angleStop if we only draw an arc, this is the ending angle in degrees
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoPie(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of ellipse
         *  \param y y-coordinate of center of ellipse
         *  \param width width of ellipse (2 * half axis)
         *  \param height of ellipse (2 * half axis)
         *  \param angleStart if we only draw an arc, this is the starting angle in degrees
         *  \param angleStop if we only draw an arc, this is the ending angle in degrees
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoPie(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, DrawMode drawMode=DrawAsGraphicElement);


        /** \copydoc JKQTPGeometricPlotElement::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPGeometricPlotElement::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
         *       This means that pies are always treated as mathematical curves, as no meaningful
         *       parametrization for pies on non-linear axes could be found!*/
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc angleStart */ 
        void setAngleStart(double __value);
        /** \copydoc angleStart */ 
        double getAngleStart() const;
        /** \copydoc angleStop */ 
        void setAngleStop(double __value);
        /** \copydoc angleStop */ 
        double getAngleStop() const;
    protected:
         /** \brief if we only draw an arc, this is the starting angle */
        double angleStart;
         /** \brief if we only draw an arc, this is the ending angle */
        double angleStop;
};




/** \brief This JKQTPGeometricPlotElement is used to draw a chord
 *  \ingroup jkqtplotter_geoplots
 *
 *  A chord on linear and log-log axes (start angle: 10deg and end-angle 110deg):
 *
 *  \image html plot_geochord.png
 *
 *  \note This function support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
 *        This means that chords are always treated as mathematical curves, as no meaningful
 *        parametrization for chords on non-linear axes could be found!
 *
 *  \see \ref JKQTPlotterGeometricGraphs, JKQTPGeoPie, JKQTPGeoEllipse, JKQTPGeoArc
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoChord: public JKQTPGeoPie {
        Q_OBJECT
    public:
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of ellipse
         *  \param y y-coordinate of center of ellipse
         *  \param width width of ellipse (2 * half axis)
         *  \param height of ellipse (2 * half axis)
         *  \param angleStart if we only draw an arc, this is the starting angle in degrees
         *  \param angleStop if we only draw an arc, this is the ending angle in degrees
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoChord(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, DrawMode drawMode=DrawAsGraphicElement);
        /** \brief class constructor
         *
         *  \param parent the parent plotter object
         *  \param x x-coordinate of center of ellipse
         *  \param y y-coordinate of center of ellipse
         *  \param width width of ellipse (2 * half axis)
         *  \param height of ellipse (2 * half axis)
         *  \param angleStart if we only draw an arc, this is the starting angle in degrees
         *  \param angleStop if we only draw an arc, this is the ending angle in degrees
         *  \param drawMode draw the rectangle as a graphic or geometric element
         */
        JKQTPGeoChord(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, DrawMode drawMode=DrawAsGraphicElement);

        /** \copydoc JKQTPGeometricPlotElement::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPGeometricPlotElement::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent
         *
         * \note This function support the JKQTPGeometricPlotElement::DrawMode::DrawAsMathematicalCurve only.
         *       This means that chords are always treated as mathematical curves, as no meaningful
         *       parametrization for chords on non-linear axes could be found!
         */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
};


#endif // jkqtpgeoshapes_H_INCLUDED
