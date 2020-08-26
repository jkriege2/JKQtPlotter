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


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpgeobase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"

#ifndef jkqtpgeoshapes_H_INCLUDED
#define jkqtpgeoshapes_H_INCLUDED




/*! \brief This JKQTPPlotObject is used to draw a rectangle
    \ingroup jkqtplotter_geoplots

    \image html plot_georectangle.png

    \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoRectangle: public JKQTPGeoBaseFilled {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of rectangle
            \param y y-coordinate of center of rectangle
            \param width width of rectangle
            \param height of rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoRectangle(JKQTBasePlotter* parent, double x, double y, double width, double height, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of rectangle
            \param y y-coordinate of center of rectangle
            \param width width of rectangle
            \param height of rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoRectangle(JKQTPlotter* parent, double x, double y, double width, double height, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of rectangle
            \param y y-coordinate of center of rectangle
            \param width width of rectangle
            \param height of rectangle
            \param angle rotation angle of the rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoRectangle(JKQTBasePlotter* parent, double x, double y, double width, double height, double angle, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of rectangle
            \param y y-coordinate of center of rectangle
            \param width width of rectangle
            \param height of rectangle
            \param angle rotation angle of the rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoRectangle(JKQTPlotter* parent, double x, double y, double width, double height, double angle, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param bottomleft bottom left corner of rectangle
            \param topright top right corner of rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoRectangle(JKQTBasePlotter* parent, QPointF bottomleft, QPointF topright, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param bottomleft bottom left corner of rectangle
            \param topright top right corner of rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoRectangle(JKQTPlotter* parent, QPointF bottomleft, QPointF topright, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);



        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \copydoc x */ 
        void setX(double __value);
        /*! \copydoc x */ 
        double getX() const;
        /*! \copydoc y */ 
        void setY(double __value);
        /*! \copydoc y */ 
        double getY() const;
        /*! \copydoc width */ 
        void setWidth(double __value);
        /*! \copydoc width */ 
        double getWidth() const;
        /*! \copydoc height */ 
        void setHeight(double __value);
        /*! \copydoc height */ 
        double getHeight() const;
        /*! \copydoc angle */ 
        void setAngle(double __value);
        /*! \copydoc angle */ 
        double getAngle() const;
        /** \brief set the rectangle using the bottom-left corner, as well as its width and height */
        void setBottomleftrectangle(double x, double y, double width, double height);
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


/*! \brief This JKQTPPlotObject is used to draw a polygon
    \ingroup jkqtplotter_geoplots

    \image html plot_geopolygon.png

    \see \ref JKQTPlotterGeometricGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoPolygon: public JKQTPGeoBaseFilled {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor color of the filling
            \param fillStyle style of the filling

         */
        JKQTPGeoPolygon(JKQTBasePlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor color of the filling
            \param fillStyle style of the filling
         */
        JKQTPGeoPolygon(JKQTPlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);

        /*! \brief class constructor

            \param parent the parent plotter object
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor color of the filling
            \param fillStyle style of the filling

         */
        JKQTPGeoPolygon(JKQTBasePlotter* parent, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor color of the filling
            \param fillStyle style of the filling
         */
        JKQTPGeoPolygon(JKQTPlotter* parent, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);


        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \copydoc points */ 
        void setPoints(const QVector<QPointF> & __value);
        /*! \copydoc points */ 
        QVector<QPointF> getPoints() const;

        /** \brief append a point to the polygon */
        void appendPoint(const QPointF& p);

        /** \brief append a point to the polygon */
        void appendPoint(const double x, const double y);

    protected:
        /** \brief list with all points on the polygon */
        QVector<QPointF> points;
};

/*! \brief This JKQTPPlotObject is used to draw an ellipse
    \ingroup jkqtplotter_geoplots

    \image html plot_geoellipse.png

    \see <a href="http://www.codeguru.com/cpp/g-m/gdi/article.php/c131">http://www.codeguru.com/cpp/g-m/gdi/article.php/c131</a> and
         <a href="http://en.wikipedia.org/wiki/Ellipse#General_parametric_form">http://en.wikipedia.org/wiki/Ellipse#General_parametric_form</a>

    \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoEllipse: public JKQTPGeoRectangle {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of ellipse
            \param fillStyle filling style of ellipse
         */
        JKQTPGeoEllipse(JKQTBasePlotter* parent, double x, double y, double width, double height, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);

        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of ellipse
            \param fillStyle filling style of ellipse
         */
        JKQTPGeoEllipse(JKQTPlotter* parent, double x, double y, double width, double height, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of rectangle
            \param y y-coordinate of center of rectangle
            \param width width of rectangle
            \param height of rectangle
            \param angle rotation angle of the rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoEllipse(JKQTBasePlotter* parent, double x, double y, double width, double height, double angle, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of rectangle
            \param y y-coordinate of center of rectangle
            \param width width of rectangle
            \param height of rectangle
            \param angle rotation angle of the rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoEllipse(JKQTPlotter* parent, double x, double y, double width, double height, double angle, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param bottomleft bottom left corner of rectangle
            \param topright top right corner of rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoEllipse(JKQTBasePlotter* parent, QPointF bottomleft, QPointF topright, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param bottomleft bottom left corner of rectangle
            \param topright top right corner of rectangle
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of rectangle
            \param fillStyle filling style of rectangle
         */
        JKQTPGeoEllipse(JKQTPlotter* parent, QPointF bottomleft, QPointF topright, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \copydoc controlPoints */ 
        void setControlPoints(const unsigned int & __value);
        /*! \copydoc controlPoints */ 
        unsigned int getControlPoints() const;
    protected:
        /** \brief number of steps/control points to draw the ellipse */
        unsigned int controlPoints;
};




/*! \brief This JKQTPPlotObject is used to draw a pie
    \ingroup jkqtplotter_geoplots

    \image html plot_geopie.png

    \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoPie: public JKQTPGeoEllipse {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
            \param angleStart if we only draw an arc, this is the starting angle in degrees
            \param angleStop if we only draw an arc, this is the ending angle in degrees
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of ellipse
            \param fillStyle filling style of ellipse
         */
        JKQTPGeoPie(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
            \param angleStart if we only draw an arc, this is the starting angle in degrees
            \param angleStop if we only draw an arc, this is the ending angle in degrees
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of ellipse
            \param fillStyle filling style of ellipse
         */
        JKQTPGeoPie(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);


        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \copydoc angleStart */ 
        void setAngleStart(double __value);
        /*! \copydoc angleStart */ 
        double getAngleStart() const;
        /*! \copydoc angleStop */ 
        void setAngleStop(double __value);
        /*! \copydoc angleStop */ 
        double getAngleStop() const;
    protected:
         /** \brief if we only draw an arc, this is the starting angle */
        double angleStart;
         /** \brief if we only draw an arc, this is the ending angle */
        double angleStop;
};




/*! \brief This JKQTPPlotObject is used to draw a chord
    \ingroup jkqtplotter_geoplots

    \image html plot_geochord.png

    \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoChord: public JKQTPGeoPie {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
            \param angleStart if we only draw an arc, this is the starting angle in degrees
            \param angleStop if we only draw an arc, this is the ending angle in degrees
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of ellipse
            \param fillStyle filling style of ellipse
         */
        JKQTPGeoChord(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
            \param angleStart if we only draw an arc, this is the starting angle in degrees
            \param angleStop if we only draw an arc, this is the ending angle in degrees
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor filling color of ellipse
            \param fillStyle filling style of ellipse
         */
        JKQTPGeoChord(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);

        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
};


#endif // jkqtpgeoshapes_H_INCLUDED
