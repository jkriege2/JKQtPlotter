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
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"

#ifndef jkqtpgraphsgeometric_H_INCLUDED
#define jkqtpgraphsgeometric_H_INCLUDED


/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
    class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseLine: public JKQTPPlotObject, public JKQTPGraphLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param color color of drawing
        \param style line style of drawing
        \param lineWidth lineWidth of drawing
        \param parent the parent plotter object
        */
    explicit JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style=Qt::SolidLine, JKQTBasePlotter* parent=nullptr);
    /*! \brief class contructor

        \param color color of drawing
        \param style line style of drawing
        \param lineWidth lineWidth of drawing
        \param parent the parent plotter object
        */
    explicit JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent);
    /*! \brief class contructor

    */
    explicit JKQTPGeoBaseLine(JKQTBasePlotter* parent);
    /*! \brief class contructor

    */
    explicit JKQTPGeoBaseLine(JKQTPlotter* parent);



    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};
/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that consist of lines with decorated ends (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseDecoratedLine: public JKQTPPlotObject, public JKQTPGraphDecoratedLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param color color of drawing
        \param lineWidth lineWidth of drawing
        \param headStyle style of the head decoration
        \param tailStyle style of the tail decoration
        \param style line style of drawing
        \param parent the parent plotter object
        */
    explicit JKQTPGeoBaseDecoratedLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, Qt::PenStyle style=Qt::SolidLine, JKQTBasePlotter* parent=nullptr);
    /*! \brief class contructor

        \param color color of drawing
        \param lineWidth lineWidth of drawing
        \param headStyle style of the head decoration
        \param tailStyle style of the tail decoration
        \param style line style of drawing
        \param parent the parent plotter object
        */
    explicit JKQTPGeoBaseDecoratedLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, Qt::PenStyle style, JKQTPlotter* parent);
    /*! \brief class contructor

    */
    explicit JKQTPGeoBaseDecoratedLine(JKQTBasePlotter* parent);
    /*! \brief class contructor

    */
    explicit JKQTPGeoBaseDecoratedLine(JKQTPlotter* parent);



    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};


/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseFilled: public JKQTPGeoBaseLine, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param fillStyle filling style of the graph
            \param lineWidth lineWidth of drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style=Qt::SolidLine, Qt::BrushStyle fillStyle=Qt::SolidPattern, JKQTBasePlotter* parent=nullptr);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param fillStyle filling style of the graph
            \param lineWidth lineWidth of drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQTPlotter* parent);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param lineWidth lineWidth of drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param lineWidth lineWidth of drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, JKQTPlotter* parent);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, JKQTPlotter* parent);

        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) to the same value */
        virtual void setAlpha(float alpha) override;
        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) */
        virtual void setAlpha(float alphaLine, float alphaFill);
        /** \brief set line and fill color */
        virtual void setColor(QColor c) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    protected:


};



/*! \brief This virtual JKQTPGraph descendent may be used to display a single symbol (marker).
    \ingroup jkqtplotter_geoplots

    \see \ref JKQTPlotterGeometricGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoSymbol: public JKQTPPlotObject, public JKQTPGraphSymbolStyleMixin {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
            \param color color of drawing
            \param fillColor fill color of the symbol (if filled)
         */
        JKQTPGeoSymbol(JKQTBasePlotter* parent, double x, double y, JKQTPGraphSymbols symbol=JKQTPCross, double symbolSize=10, QColor color=QColor("black"), QColor fillColor=QColor("grey"));
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
            \param color color of drawing
            \param fillColor fill color of the symbol (if filled)
         */
        JKQTPGeoSymbol(JKQTPlotter* parent, double x, double y, JKQTPGraphSymbols symbol=JKQTPCross, double symbolSize=10, QColor color=QColor("black"), QColor fillColor=QColor("grey"));

        /*! set the symbol color and symbol fill color */
        virtual void setColor(QColor c);

        /*! \copydoc x */ 
        void setX(double __value);
        /*! \copydoc x */ 
        double getX() const;
        /*! \copydoc y */ 
        void setY(double __value);
        /*! \copydoc y */ 
        double getY() const;

        /** \copydoc JKQTPGraph::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPGraph::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

    protected:
        /** \brief x-position (in plot coordinates) of the symbol (symbol center) */
        double x;
        /** \brief y-position (in plot coordinates) of the symbol (symbol center) */
        double y;

};


/*! \brief This JKQTPPlotObject is used to display text. It uses the JKQTMathText
           class in order to display LaTeX formulas.
    \ingroup jkqtplotter_geoplots

    \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoText: public JKQTPPlotObject, public JKQTPGraphTextStyleMixin {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of text
            \param y y-coordinate of text
            \param text the text to display
            \param color color of drawing
            \param fontSize base font size of text
         */
        JKQTPGeoText(JKQTBasePlotter* parent, double x, double y, const QString& text, double fontSize, QColor color);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of text
            \param y y-coordinate of text
            \param text the text to display
            \param color color of drawing
            \param fontSize base font size of text
         */
        JKQTPGeoText(JKQTPlotter* parent, double x, double y, const QString& text, double fontSize, QColor color);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of text
            \param y y-coordinate of text
            \param text the text to display
         */
        JKQTPGeoText(JKQTBasePlotter* parent, double x, double y, const QString& text);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of text
            \param y y-coordinate of text
            \param text the text to display
         */
        JKQTPGeoText(JKQTPlotter* parent, double x, double y, const QString& text);

        /*! \copydoc text */ 
        void setText(const QString & __value);
        /*! \copydoc text */ 
        QString getText() const;
        /*! \copydoc x */ 
        void setX(double __value);
        /*! \copydoc x */ 
        double getX() const;
        /*! \copydoc y */ 
        void setY(double __value);
        /*! \copydoc y */ 
        double getY() const;

        /** \brief set line and fill color */
        virtual void setColor(QColor c) ;

        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

    protected:
        /** \brief x-position (in plot coordinates) of the text (left/baseline) */
        double x;
        /** \brief y-position (in plot coordinates) of the text (left/baseline) */
        double y;

        /** \brief the text to display */
        QString text;
        /** \brief return a pen, that may be used for drawing */
        QPen getPen(JKQTPEnhancedPainter& painter);
};

/*! \brief This JKQTPPlotObject is used to draw a line
    \ingroup jkqtplotter_geoplots

    \image html plot_geoline.png

    \see \ref JKQTPlotterGeometricGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoLine: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x1 x-coordinate of first point of line
            \param y1 y-coordinate of first point of line
            \param x2 x-coordinate of second point of line
            \param y2 y-coordinate of second point of line
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param parent the parent plotter object
         */
        JKQTPGeoLine(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x1 x-coordinate of first point of line
            \param y1 y-coordinate of first point of line
            \param x2 x-coordinate of second point of line
            \param y2 y-coordinate of second point of line
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param parent the parent plotter object
         */
        JKQTPGeoLine(JKQTPlotter* parent, double x1, double y1, double x2, double y2, QColor color, double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x1 x-coordinate of first point of line
            \param y1 y-coordinate of first point of line
            \param x2 x-coordinate of second point of line
            \param y2 y-coordinate of second point of line

         */
        JKQTPGeoLine(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x1 x-coordinate of first point of line
            \param y1 y-coordinate of first point of line
            \param x2 x-coordinate of second point of line
            \param y2 y-coordinate of second point of line

         */
        JKQTPGeoLine(JKQTPlotter* parent, double x1, double y1, double x2, double y2);


        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \copydoc x1 */ 
        void setX1(double __value);
        /*! \copydoc x1 */ 
        double getX1() const;
        /*! \copydoc y1 */ 
        void setY1(double __value);
        /*! \copydoc y1 */ 
        double getY1() const;
        /*! \copydoc x2 */ 
        void setX2(double __value);
        /*! \copydoc x2 */ 
        double getX2() const;
        /*! \copydoc y2 */ 
        void setY2(double __value);
        /*! \copydoc y2 */ 
        double getY2() const;
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


/*! \brief This JKQTPPlotObject is used to draw a line with decorations (e.g. arrows)
    \ingroup jkqtplotter_geoplots

    \image html plot_geoarrows.png

    \see \ref JKQTPlotterGeometricGraphs

    */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoArrow: public JKQTPGeoBaseDecoratedLine {
    Q_OBJECT
public:
    /*! \brief class constructor

        \param parent the parent plotter object
        \param x1 x-coordinate of first point of line
        \param y1 y-coordinate of first point of line
        \param x2 x-coordinate of second point of line
        \param y2 y-coordinate of second point of line
        \param color color of line
        \param lineWidth width of line
        \param style line style
        \param parent the parent plotter object
    */
    JKQTPGeoArrow(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2, QColor color, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator, double lineWidth=1.0, Qt::PenStyle style=Qt::SolidLine);
    /*! \brief class constructor

        \param parent the parent plotter object
        \param x1 x-coordinate of first point of line
        \param y1 y-coordinate of first point of line
        \param x2 x-coordinate of second point of line
        \param y2 y-coordinate of second point of line
        \param color color of line
        \param lineWidth width of line
        \param style line style
        \param parent the parent plotter object
      */
    JKQTPGeoArrow(JKQTPlotter* parent, double x1, double y1, double x2, double y2, QColor color, JKQTPLineDecoratorStyle headStyle=JKQTPDefaultLineDecorator, JKQTPLineDecoratorStyle tailStyle=JKQTPNoDecorator, double lineWidth=1.0, Qt::PenStyle style=Qt::SolidLine);


    /** \copydoc JKQTPPlotObject::getXMinMax()        */
    virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
    /** \copydoc JKQTPPlotObject::getYMinMax()        */
    virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

    /** \brief plots the graph to the plotter object specified as parent */
    virtual void draw(JKQTPEnhancedPainter& painter) override;

    /*! \copydoc x1 */
    void setX1(double __value);
    /*! \copydoc x1 */
    double getX1() const;
    /*! \copydoc y1 */
    void setY1(double __value);
    /*! \copydoc y1 */
    double getY1() const;
    /*! \copydoc x2 */
    void setX2(double __value);
    /*! \copydoc x2 */
    double getX2() const;
    /*! \copydoc y2 */
    void setY2(double __value);
    /*! \copydoc y2 */
    double getY2() const;
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

/*! \brief This JKQTPPlotObject is used to draw an infinite line
    \ingroup jkqtplotter_geoplots

    an infinite line has a starting point and then goes on in a given direction
    until the end of the plotting range.

    \image html plot_geoinfiniteline.png

    \see \ref JKQTPlotterGeometricGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoInfiniteLine: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of start point of line
            \param y y-coordinate of start point of line
            \param dx x-direction of the line
            \param dy y-direction of the line
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoInfiniteLine(JKQTBasePlotter* parent, double x, double y, double dx, double dy, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param x x-coordinate of start point of line
            \param y y-coordinate of start point of line
            \param dx x-direction of the line
            \param dy y-direction of the line
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoInfiniteLine(JKQTPlotter* parent, double x, double y, double dx, double dy, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);


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
        /*! \copydoc dx */ 
        void setDx(double __value);
        /*! \copydoc dx */ 
        double getDx() const;
        /*! \copydoc dy */ 
        void setDy(double __value);
        /*! \copydoc dy */ 
        double getDy() const;
        /*! \copydoc two_sided */ 
        void setTwoSided(bool __value);
        /*! \copydoc two_sided */ 
        bool getTwoSided() const;
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


/*! \brief This JKQTPPlotObject is used to draw a poly line
    \ingroup jkqtplotter_geoplots

    \image html plot_geolines.png

    \see \ref JKQTPlotterGeometricGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoPolyLines: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter object
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoPolyLines(JKQTBasePlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoPolyLines(JKQTPlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoPolyLines(JKQTBasePlotter* parent, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter object
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoPolyLines(JKQTPlotter* parent, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);


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
        /** \brief list with all points on the poly-line */
        QVector<QPointF> points;
};

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


/*! \brief This JKQTPPlotObject is used to draw an arc
    \ingroup jkqtplotter_geoplots

    \image html plot_geoarc.png

    \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoArc: public JKQTPGeoBaseLine {
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

         */
        JKQTPGeoArc(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
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

         */
        JKQTPGeoArc(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);

        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \copydoc controlPoints */
        void setControlPoints(const unsigned int & __value);
        /*! \copydoc controlPoints */
        unsigned int getControlPoints() const;
        /*! \copydoc angleStart */ 
        void setAngleStart(double __value);
        /*! \copydoc angleStart */ 
        double getAngleStart() const;
        /*! \copydoc angleStop */ 
        void setAngleStop(double __value);
        /*! \copydoc angleStop */ 
        double getAngleStop() const;
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
        /** \brief number of steps/control points to draw the ellipse */
        unsigned int controlPoints;

        /** \brief closing mode for arcs: secand or pie */
        /** \brief returns the transformation matrix used for this rectangle */
        QTransform getTransform();
        /** \brief returns a QPolygonF which represents the rectangle after rotation, but still in the world coordinate system, not in the screen/widget system */
        QPolygonF getPolygon();

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


#endif // jkqtpgraphsgeometric_H_INCLUDED
