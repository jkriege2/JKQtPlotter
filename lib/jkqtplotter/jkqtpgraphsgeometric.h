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



/** \file jkqtpgraphsgeometric.h
  * \ingroup jkqtplotter_geoplots
  */

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"

#ifndef jkqtpgraphsgeometric_H_INCLUDED
#define jkqtpgraphsgeometric_H_INCLUDED


/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

 */
class LIB_EXPORT JKQTPGeoBaseLine: public JKQTPPlotObject {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param color color of drawing
            \param style line style of drawing
            \param lineWidth lineWidth of drawing
         */
        explicit JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style=Qt::SolidLine, JKQTBasePlotter* parent=nullptr);
        /*! \brief class contructor

            \param color color of drawing
            \param style line style of drawing
            \param lineWidth lineWidth of drawing
         */
        explicit JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent);


        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth ( \copybrief lineWidth ) to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void setLineWidth(double __value)  
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth ( \copybrief lineWidth ). 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }

        /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
        virtual void setAlpha(float alpha);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

    protected:
        /** \brief color of the graph */
        QColor color;

        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief return a pen, that may be used for drawing */
        QPen getPen(JKQTPEnhancedPainter &painter);
};


/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

 */
class LIB_EXPORT JKQTPGeoBaseFilled: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param fillStyle filling style of the graph
            \param lineWidth lineWidth of drawing
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style=Qt::SolidLine, Qt::BrushStyle fillStyle=Qt::SolidPattern, JKQTBasePlotter* parent=nullptr);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param fillStyle filling style of the graph
            \param lineWidth lineWidth of drawing
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, Qt::BrushStyle fillStyle, JKQTPlotter* parent);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param lineWidth lineWidth of drawing
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, JKQTPlotter* parent);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param lineWidth lineWidth of drawing
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, JKQTPlotter* parent);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, JKQTPlotter* parent);

        /*! \brief sets the property fillColor ( \copybrief fillColor ) to the specified \a __value. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual void setFillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor ( \copybrief fillColor ). 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual QColor getFillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle ( \copybrief fillStyle ) to the specified \a __value. 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual void setFillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle ( \copybrief fillStyle ). 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle getFillStyle() const  
        {
            return this->fillStyle; 
        }

        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) to the same value */
        virtual void setAlpha(float alpha) override;
        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) */
        virtual void setAlpha(float alphaLine, float alphaFill);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    protected:

        /** \brief filling color of the graph */
        QColor fillColor;
        /** \brief fill style for the curve */
        Qt::BrushStyle fillStyle;
        /** \brief return a brush that may be used for drawing */
        QBrush getBrush(JKQTPEnhancedPainter& painter);

};



/*! \brief This virtual JKQTPGraph descendent may be used to display a single symbol (marker).
    \ingroup jkqtplotter_geoplots

 */
class LIB_EXPORT JKQTPGeoSymbol: public JKQTPPlotObject {
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

        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor ( \copybrief fillColor ) to the specified \a __value. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual void setFillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor ( \copybrief fillColor ). 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual QColor getFillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property symbol ( \copybrief symbol ) to the specified \a __value. 
            \details Description of the parameter symbol is: <BLOCKQUOTE>\copydoc symbol </BLOCKQUOTE> 
            \see symbol for more information */ 
        inline virtual void setSymbol(const JKQTPGraphSymbols & __value)  
        {
            this->symbol = __value;
        } 
        /*! \brief returns the property symbol ( \copybrief symbol ). 
            \details Description of the parameter symbol is: <BLOCKQUOTE>\copydoc symbol </BLOCKQUOTE> 
            \see symbol for more information */ 
        inline virtual JKQTPGraphSymbols getSymbol() const  
        {
            return this->symbol; 
        }
        /*! \brief sets the property symbolSize ( \copybrief symbolSize ) to the specified \a __value. 
            \details Description of the parameter symbolSize is: <BLOCKQUOTE>\copydoc symbolSize </BLOCKQUOTE> 
            \see symbolSize for more information */ 
        inline virtual void setSymbolSize(double __value)  
        {
            this->symbolSize = __value;
        } 
        /*! \brief returns the property symbolSize ( \copybrief symbolSize ). 
            \details Description of the parameter symbolSize is: <BLOCKQUOTE>\copydoc symbolSize </BLOCKQUOTE> 
            \see symbolSize for more information */ 
        inline virtual double getSymbolSize() const  
        {
            return this->symbolSize; 
        }
        /*! \brief sets the property symbolWidth ( \copybrief symbolWidth ) to the specified \a __value. 
            \details Description of the parameter symbolWidth is: <BLOCKQUOTE>\copydoc symbolWidth </BLOCKQUOTE> 
            \see symbolWidth for more information */ 
        inline virtual void setSymbolWidth(double __value)  
        {
            this->symbolWidth = __value;
        } 
        /*! \brief returns the property symbolWidth ( \copybrief symbolWidth ). 
            \details Description of the parameter symbolWidth is: <BLOCKQUOTE>\copydoc symbolWidth </BLOCKQUOTE> 
            \see symbolWidth for more information */ 
        inline virtual double getSymbolWidth() const  
        {
            return this->symbolWidth; 
        }
        /*! \brief sets the property x ( \copybrief x ) to the specified \a __value. 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual void setX(double __value)  
        {
            this->x = __value;
        } 
        /*! \brief returns the property x ( \copybrief x ). 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual double getX() const  
        {
            return this->x; 
        }
        /*! \brief sets the property y ( \copybrief y ) to the specified \a __value. 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual void setY(double __value)  
        {
            this->y = __value;
        } 
        /*! \brief returns the property y ( \copybrief y ). 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual double getY() const  
        {
            return this->y; 
        }

        /** \copydoc JKQTPGraph::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \copydoc JKQTPGraph::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

    protected:
        double x,y;

        /** \brief color of the graph */
        QColor color;
        /** \brief fill-color of the graph */
        QColor fillColor;

        /** \brief size of the symbol in pt */
        double symbolSize;
        /** \brief width of the symbol lines in pt */
        double symbolWidth;
        /** \brief type of the symbol */
        JKQTPGraphSymbols symbol;
};


/*! \brief This JKQTPPlotObject is used to display text. It uses the JKQTMathText
           class in order to display LaTeX formulas.
    \ingroup jkqtplotter_geoplots

 */
class LIB_EXPORT JKQTPGeoText: public JKQTPPlotObject {
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
        JKQTPGeoText(JKQTBasePlotter* parent, double x, double y, const QString& text, double fontSize=10, QColor color=QColor("black"));
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of text
            \param y y-coordinate of text
            \param text the text to display
            \param color color of drawing
            \param fontSize base font size of text
         */
        JKQTPGeoText(JKQTPlotter* parent, double x, double y, const QString& text, double fontSize=10, QColor color=QColor("black"));

        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \brief sets the property text ( \copybrief text ) to the specified \a __value. 
            \details Description of the parameter text is: <BLOCKQUOTE>\copydoc text </BLOCKQUOTE> 
            \see text for more information */ 
        inline virtual void setText(const QString & __value)  
        {
            this->text = __value;
        } 
        /*! \brief returns the property text ( \copybrief text ). 
            \details Description of the parameter text is: <BLOCKQUOTE>\copydoc text </BLOCKQUOTE> 
            \see text for more information */ 
        inline virtual QString getText() const  
        {
            return this->text; 
        }
        /*! \brief sets the property fontSize ( \copybrief fontSize ) to the specified \a __value. 
            \details Description of the parameter fontSize is: <BLOCKQUOTE>\copydoc fontSize </BLOCKQUOTE> 
            \see fontSize for more information */ 
        inline virtual void setFontSize(double __value)  
        {
            this->fontSize = __value;
        } 
        /*! \brief returns the property fontSize ( \copybrief fontSize ). 
            \details Description of the parameter fontSize is: <BLOCKQUOTE>\copydoc fontSize </BLOCKQUOTE> 
            \see fontSize for more information */ 
        inline virtual double getFontSize() const  
        {
            return this->fontSize; 
        }
        /*! \brief sets the property x ( \copybrief x ) to the specified \a __value. 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual void setX(double __value)  
        {
            this->x = __value;
        } 
        /*! \brief returns the property x ( \copybrief x ). 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual double getX() const  
        {
            return this->x; 
        }
        /*! \brief sets the property y ( \copybrief y ) to the specified \a __value. 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual void setY(double __value)  
        {
            this->y = __value;
        } 
        /*! \brief returns the property y ( \copybrief y ). 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual double getY() const  
        {
            return this->y; 
        }

        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

    protected:
        double x,y;

        /** \brief color of the graph */
        QColor color;

        /** \brief base font size of text */
        double fontSize;
        /** \brief the text to display */
        QString text;
        /** \brief return a pen, that may be used for drawing */
        QPen getPen(JKQTPEnhancedPainter& painter);
};

/*! \brief This JKQTPPlotObject is used to draw a line
    \ingroup jkqtplotter_geoplots

    \image html plot_geoline.png

 */
class LIB_EXPORT JKQTPGeoLine: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
            \param x1 x-coordinate of first point of line
            \param y1 y-coordinate of first point of line
            \param x2 x-coordinate of second point of line
            \param y2 y-coordinate of second point of line
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoLine(JKQTBasePlotter* parent, double x1, double y1, double x2, double y2, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter class
            \param x1 x-coordinate of first point of line
            \param y1 y-coordinate of first point of line
            \param x2 x-coordinate of second point of line
            \param y2 y-coordinate of second point of line
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoLine(JKQTPlotter* parent, double x1, double y1, double x2, double y2, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);


        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \brief sets the property x1 ( \copybrief x1 ) to the specified \a __value. 
            \details Description of the parameter x1 is: <BLOCKQUOTE>\copydoc x1 </BLOCKQUOTE> 
            \see x1 for more information */ 
        inline virtual void setX1(double __value)  
        {
            this->x1 = __value;
        } 
        /*! \brief returns the property x1 ( \copybrief x1 ). 
            \details Description of the parameter x1 is: <BLOCKQUOTE>\copydoc x1 </BLOCKQUOTE> 
            \see x1 for more information */ 
        inline virtual double getX1() const  
        {
            return this->x1; 
        }
        /*! \brief sets the property y1 ( \copybrief y1 ) to the specified \a __value. 
            \details Description of the parameter y1 is: <BLOCKQUOTE>\copydoc y1 </BLOCKQUOTE> 
            \see y1 for more information */ 
        inline virtual void setY1(double __value)  
        {
            this->y1 = __value;
        } 
        /*! \brief returns the property y1 ( \copybrief y1 ). 
            \details Description of the parameter y1 is: <BLOCKQUOTE>\copydoc y1 </BLOCKQUOTE> 
            \see y1 for more information */ 
        inline virtual double getY1() const  
        {
            return this->y1; 
        }
        /*! \brief sets the property x2 ( \copybrief x2 ) to the specified \a __value. 
            \details Description of the parameter x2 is: <BLOCKQUOTE>\copydoc x2 </BLOCKQUOTE> 
            \see x2 for more information */ 
        inline virtual void setX2(double __value)  
        {
            this->x2 = __value;
        } 
        /*! \brief returns the property x2 ( \copybrief x2 ). 
            \details Description of the parameter x2 is: <BLOCKQUOTE>\copydoc x2 </BLOCKQUOTE> 
            \see x2 for more information */ 
        inline virtual double getX2() const  
        {
            return this->x2; 
        }
        /*! \brief sets the property y2 ( \copybrief y2 ) to the specified \a __value. 
            \details Description of the parameter y2 is: <BLOCKQUOTE>\copydoc y2 </BLOCKQUOTE> 
            \see y2 for more information */ 
        inline virtual void setY2(double __value)  
        {
            this->y2 = __value;
        } 
        /*! \brief returns the property y2 ( \copybrief y2 ). 
            \details Description of the parameter y2 is: <BLOCKQUOTE>\copydoc y2 </BLOCKQUOTE> 
            \see y2 for more information */ 
        inline virtual double getY2() const  
        {
            return this->y2; 
        }
    protected:
        double x1, y1, x2, y2;
};



/*! \brief This JKQTPPlotObject is used to draw an infinite line
    \ingroup jkqtplotter_geoplots

    an infinite line has a starting point and then goes on in a given direction
    until the end of the plotting range.

    \image html plot_geoinfiniteline.png

 */
class LIB_EXPORT JKQTPGeoInfiniteLine: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

        /*! \brief sets the property x ( \copybrief x ) to the specified \a __value. 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual void setX(double __value)  
        {
            this->x = __value;
        } 
        /*! \brief returns the property x ( \copybrief x ). 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual double getX() const  
        {
            return this->x; 
        }
        /*! \brief sets the property y ( \copybrief y ) to the specified \a __value. 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual void setY(double __value)  
        {
            this->y = __value;
        } 
        /*! \brief returns the property y ( \copybrief y ). 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual double getY() const  
        {
            return this->y; 
        }
        /*! \brief sets the property dx ( \copybrief dx ) to the specified \a __value. 
            \details Description of the parameter dx is: <BLOCKQUOTE>\copydoc dx </BLOCKQUOTE> 
            \see dx for more information */ 
        inline virtual void setDx(double __value)  
        {
            this->dx = __value;
        } 
        /*! \brief returns the property dx ( \copybrief dx ). 
            \details Description of the parameter dx is: <BLOCKQUOTE>\copydoc dx </BLOCKQUOTE> 
            \see dx for more information */ 
        inline virtual double getDx() const  
        {
            return this->dx; 
        }
        /*! \brief sets the property dy ( \copybrief dy ) to the specified \a __value. 
            \details Description of the parameter dy is: <BLOCKQUOTE>\copydoc dy </BLOCKQUOTE> 
            \see dy for more information */ 
        inline virtual void setDy(double __value)  
        {
            this->dy = __value;
        } 
        /*! \brief returns the property dy ( \copybrief dy ). 
            \details Description of the parameter dy is: <BLOCKQUOTE>\copydoc dy </BLOCKQUOTE> 
            \see dy for more information */ 
        inline virtual double getDy() const  
        {
            return this->dy; 
        }
        /*! \brief sets the property two_sided ( \copybrief two_sided ) to the specified \a __value. 
            \details Description of the parameter two_sided is: <BLOCKQUOTE>\copydoc two_sided </BLOCKQUOTE> 
            \see two_sided for more information */ 
        inline virtual void setTwoSided(bool __value)  
        {
            this->two_sided = __value;
        } 
        /*! \brief returns the property two_sided ( \copybrief two_sided ). 
            \details Description of the parameter two_sided is: <BLOCKQUOTE>\copydoc two_sided </BLOCKQUOTE> 
            \see two_sided for more information */ 
        inline virtual bool getTwoSided() const  
        {
            return this->two_sided; 
        }
    protected:
        double x, y, dx, dy;
        /** \brief indicates whether the line ends at the given point \f$ (x,y) \f$ (false, default),
         *         or is infinite in both directions (true) */
        bool two_sided;
};


/*! \brief This JKQTPPlotObject is used to draw a poly line
    \ingroup jkqtplotter_geoplots

    \image html plot_geolines.png

 */
class LIB_EXPORT JKQTPGeoPolyLines: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoPolyLines(JKQTBasePlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter class
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoPolyLines(JKQTPlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter class
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
         */
        JKQTPGeoPolyLines(JKQTBasePlotter* parent, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter class
            \param points points on the polygon
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

        /*! \brief sets the property points ( \copybrief points ) to the specified \a __value. 
            \details Description of the parameter points is: <BLOCKQUOTE>\copydoc points </BLOCKQUOTE> 
            \see points for more information */ 
        inline virtual void setPoints(const QVector<QPointF> & __value)  
        {
            this->points = __value;
        } 
        /*! \brief returns the property points ( \copybrief points ). 
            \details Description of the parameter points is: <BLOCKQUOTE>\copydoc points </BLOCKQUOTE> 
            \see points for more information */ 
        inline virtual QVector<QPointF> getPoints() const  
        {
            return this->points; 
        }

        /** \brief append a point to the polygon */
        inline void appendPoint(const QPointF& p) {
            points.append(p);
        }

        /** \brief append a point to the polygon */
        inline void appendPoint(const double x, const double y) {
            points.append(QPointF(x, y));
        }
    protected:
        QVector<QPointF> points;
};

/*! \brief This JKQTPPlotObject is used to draw a rectangle
    \ingroup jkqtplotter_geoplots

    \image html plot_georectangle.png
 */
class LIB_EXPORT JKQTPGeoRectangle: public JKQTPGeoBaseFilled {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

        /*! \brief sets the property x ( \copybrief x ) to the specified \a __value. 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual void setX(double __value)  
        {
            this->x = __value;
        } 
        /*! \brief returns the property x ( \copybrief x ). 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual double getX() const  
        {
            return this->x; 
        }
        /*! \brief sets the property y ( \copybrief y ) to the specified \a __value. 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual void setY(double __value)  
        {
            this->y = __value;
        } 
        /*! \brief returns the property y ( \copybrief y ). 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual double getY() const  
        {
            return this->y; 
        }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc width </BLOCKQUOTE> 
            \see width for more information */ 
        inline virtual void setWidth(double __value)  
        {
            this->width = __value;
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc width </BLOCKQUOTE> 
            \see width for more information */ 
        inline virtual double getWidth() const  
        {
            return this->width; 
        }
        /*! \brief sets the property height ( \copybrief height ) to the specified \a __value. 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc height </BLOCKQUOTE> 
            \see height for more information */ 
        inline virtual void setHeight(double __value)  
        {
            this->height = __value;
        } 
        /*! \brief returns the property height ( \copybrief height ). 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc height </BLOCKQUOTE> 
            \see height for more information */ 
        inline virtual double getHeight() const  
        {
            return this->height; 
        }
        /*! \brief sets the property angle ( \copybrief angle ) to the specified \a __value. 
            \details Description of the parameter angle is: <BLOCKQUOTE>\copydoc angle </BLOCKQUOTE> 
            \see angle for more information */ 
        inline virtual void setAngle(double __value)  
        {
            this->angle = __value;
        } 
        /*! \brief returns the property angle ( \copybrief angle ). 
            \details Description of the parameter angle is: <BLOCKQUOTE>\copydoc angle </BLOCKQUOTE> 
            \see angle for more information */ 
        inline virtual double getAngle() const  
        {
            return this->angle; 
        }

        void setBottomleftrectangle(double x, double y, double width, double height);
protected:
        double x,y,width,height;
        /** \brief rotation angle of rectangle */
        double angle;
        /** \brief returns the transformation matrix used for this rectangle */
        QMatrix getMatrix();
        /** \brief returns a QPolygonF which represents the rectangle after rotation, but still in the world coordinate system, not in the screen/widget system */
        QPolygonF getPolygon();
};


/*! \brief This JKQTPPlotObject is used to draw a polygon
    \ingroup jkqtplotter_geoplots

    \image html plot_geopolygon.png

 */
class LIB_EXPORT JKQTPGeoPolygon: public JKQTPGeoBaseFilled {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor color of the filling
            \param fillStyle style of the filling

         */
        JKQTPGeoPolygon(JKQTBasePlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter class
            \param points points on the polygon
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor color of the filling
            \param fillStyle style of the filling
         */
        JKQTPGeoPolygon(JKQTPlotter* parent, const QVector<QPointF>& points, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);

        /*! \brief class constructor

            \param parent the parent plotter class
            \param color color of line
            \param lineWidth width of line
            \param style line style
            \param fillColor color of the filling
            \param fillStyle style of the filling

         */
        JKQTPGeoPolygon(JKQTBasePlotter* parent, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter class
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

        /*! \brief sets the property points ( \copybrief points ) to the specified \a __value. 
            \details Description of the parameter points is: <BLOCKQUOTE>\copydoc points </BLOCKQUOTE> 
            \see points for more information */ 
        inline virtual void setPoints(const QVector<QPointF> & __value)  
        {
            this->points = __value;
        } 
        /*! \brief returns the property points ( \copybrief points ). 
            \details Description of the parameter points is: <BLOCKQUOTE>\copydoc points </BLOCKQUOTE> 
            \see points for more information */ 
        inline virtual QVector<QPointF> getPoints() const  
        {
            return this->points; 
        }

        /** \brief append a point to the polygon */
        inline void appendPoint(const QPointF& p) {
            points.append(p);
        }

        /** \brief append a point to the polygon */
        inline void appendPoint(const double x, const double y) {
            points.append(QPointF(x, y));
        }

    protected:
        QVector<QPointF> points;
};

/*! \brief This JKQTPPlotObject is used to draw an ellipse
    \ingroup jkqtplotter_geoplots

    \image html plot_geoellipse.png

    \see <a href="http://www.codeguru.com/cpp/g-m/gdi/article.php/c131">http://www.codeguru.com/cpp/g-m/gdi/article.php/c131</a> and
         <a href="http://en.wikipedia.org/wiki/Ellipse#General_parametric_form">http://en.wikipedia.org/wiki/Ellipse#General_parametric_form</a>
 */
class LIB_EXPORT JKQTPGeoEllipse: public JKQTPGeoRectangle {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

            \param parent the parent plotter class
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

        /*! \brief sets the property controlPoints ( \copybrief controlPoints ) to the specified \a __value. 
            \details Description of the parameter controlPoints is: <BLOCKQUOTE>\copydoc controlPoints </BLOCKQUOTE> 
            \see controlPoints for more information */ 
        inline virtual void setControlPoints(const unsigned int & __value)  
        {
            this->controlPoints = __value;
        } 
        /*! \brief returns the property controlPoints ( \copybrief controlPoints ). 
            \details Description of the parameter controlPoints is: <BLOCKQUOTE>\copydoc controlPoints </BLOCKQUOTE> 
            \see controlPoints for more information */ 
        inline virtual unsigned int getControlPoints() const  
        {
            return this->controlPoints; 
        }
    protected:
        /** \brief number of steps/control points to draw the ellipse */
        unsigned int controlPoints;
};


/*! \brief This JKQTPPlotObject is used to draw an arc
    \ingroup jkqtplotter_geoplots

    \image html plot_geoarc.png
 */
class LIB_EXPORT JKQTPGeoArc: public JKQTPGeoBaseLine {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
            \param color color of line
            \param lineWidth width of line
            \param style line style

         */
        JKQTPGeoArc(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine);
        /*! \brief class constructor

            \param parent the parent plotter class
            \param x x-coordinate of center of ellipse
            \param y y-coordinate of center of ellipse
            \param width width of ellipse (2 * half axis)
            \param height of ellipse (2 * half axis)
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

        /*! \brief sets the property controlPoints ( \copybrief controlPoints ) to the specified \a __value. 
            \details Description of the parameter controlPoints is: <BLOCKQUOTE>\copydoc controlPoints </BLOCKQUOTE> 
            \see controlPoints for more information */ 
        inline virtual void setControlPoints(const unsigned int & __value)  
        {
            this->controlPoints = __value;
        } 
        /*! \brief returns the property controlPoints ( \copybrief controlPoints ). 
            \details Description of the parameter controlPoints is: <BLOCKQUOTE>\copydoc controlPoints </BLOCKQUOTE> 
            \see controlPoints for more information */ 
        inline virtual unsigned int getControlPoints() const  
        {
            return this->controlPoints; 
        }
        /*! \brief sets the property angleStart ( \copybrief angleStart ) to the specified \a __value. 
            \details Description of the parameter angleStart is: <BLOCKQUOTE>\copydoc angleStart </BLOCKQUOTE> 
            \see angleStart for more information */ 
        inline virtual void setAngleStart(double __value)  
        {
            this->angleStart = __value;
        } 
        /*! \brief returns the property angleStart ( \copybrief angleStart ). 
            \details Description of the parameter angleStart is: <BLOCKQUOTE>\copydoc angleStart </BLOCKQUOTE> 
            \see angleStart for more information */ 
        inline virtual double getAngleStart() const  
        {
            return this->angleStart; 
        }
        /*! \brief sets the property angleStop ( \copybrief angleStop ) to the specified \a __value. 
            \details Description of the parameter angleStop is: <BLOCKQUOTE>\copydoc angleStop </BLOCKQUOTE> 
            \see angleStop for more information */ 
        inline virtual void setAngleStop(double __value)  
        {
            this->angleStop = __value;
        } 
        /*! \brief returns the property angleStop ( \copybrief angleStop ). 
            \details Description of the parameter angleStop is: <BLOCKQUOTE>\copydoc angleStop </BLOCKQUOTE> 
            \see angleStop for more information */ 
        inline virtual double getAngleStop() const  
        {
            return this->angleStop; 
        }
        /*! \brief sets the property x ( \copybrief x ) to the specified \a __value. 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual void setX(double __value)  
        {
            this->x = __value;
        } 
        /*! \brief returns the property x ( \copybrief x ). 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual double getX() const  
        {
            return this->x; 
        }
        /*! \brief sets the property y ( \copybrief y ) to the specified \a __value. 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual void setY(double __value)  
        {
            this->y = __value;
        } 
        /*! \brief returns the property y ( \copybrief y ). 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual double getY() const  
        {
            return this->y; 
        }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc width </BLOCKQUOTE> 
            \see width for more information */ 
        inline virtual void setWidth(double __value)  
        {
            this->width = __value;
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc width </BLOCKQUOTE> 
            \see width for more information */ 
        inline virtual double getWidth() const  
        {
            return this->width; 
        }
        /*! \brief sets the property height ( \copybrief height ) to the specified \a __value. 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc height </BLOCKQUOTE> 
            \see height for more information */ 
        inline virtual void setHeight(double __value)  
        {
            this->height = __value;
        } 
        /*! \brief returns the property height ( \copybrief height ). 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc height </BLOCKQUOTE> 
            \see height for more information */ 
        inline virtual double getHeight() const  
        {
            return this->height; 
        }
        /*! \brief sets the property angle ( \copybrief angle ) to the specified \a __value. 
            \details Description of the parameter angle is: <BLOCKQUOTE>\copydoc angle </BLOCKQUOTE> 
            \see angle for more information */ 
        inline virtual void setAngle(double __value)  
        {
            this->angle = __value;
        } 
        /*! \brief returns the property angle ( \copybrief angle ). 
            \details Description of the parameter angle is: <BLOCKQUOTE>\copydoc angle </BLOCKQUOTE> 
            \see angle for more information */ 
        inline virtual double getAngle() const  
        {
            return this->angle; 
        }
    protected:
        double x,y,width,height;
        /** \brief rotation angle of rectangle */
        double angle;
        /** \brief if we only draw an arc, this is the starting angle in degrees */
        double angleStart;
        /** \brief if we only draw an arc, this is the ending angle in degrees */
        double angleStop;
        /** \brief closing mode for arcs: secand or pie */
        double angle_start;
        /** \brief if we only draw an arc, this is the ending angle */
        double angle_end;
        /** \brief closing mode for arcs: secand or pie */
        /** \brief returns the transformation matrix used for this rectangle */
        QMatrix getMatrix();
        /** \brief returns a QPolygonF which represents the rectangle after rotation, but still in the world coordinate system, not in the screen/widget system */
        QPolygonF getPolygon();
        /** \brief number of steps/control points to draw the ellipse */
        unsigned int controlPoints;
};




/*! \brief This JKQTPPlotObject is used to draw a pie
    \ingroup jkqtplotter_geoplots

    \image html plot_geopie.png
 */
class LIB_EXPORT JKQTPGeoPie: public JKQTPGeoEllipse {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
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
        JKQTPGeoPie(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter class
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
        JKQTPGeoPie(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);


        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \brief sets the property angleStart ( \copybrief angleStart ) to the specified \a __value. 
            \details Description of the parameter angleStart is: <BLOCKQUOTE>\copydoc angleStart </BLOCKQUOTE> 
            \see angleStart for more information */ 
        inline virtual void setAngleStart(double __value)  
        {
            this->angleStart = __value;
        } 
        /*! \brief returns the property angleStart ( \copybrief angleStart ). 
            \details Description of the parameter angleStart is: <BLOCKQUOTE>\copydoc angleStart </BLOCKQUOTE> 
            \see angleStart for more information */ 
        inline virtual double getAngleStart() const  
        {
            return this->angleStart; 
        }
        /*! \brief sets the property angleStop ( \copybrief angleStop ) to the specified \a __value. 
            \details Description of the parameter angleStop is: <BLOCKQUOTE>\copydoc angleStop </BLOCKQUOTE> 
            \see angleStop for more information */ 
        inline virtual void setAngleStop(double __value)  
        {
            this->angleStop = __value;
        } 
        /*! \brief returns the property angleStop ( \copybrief angleStop ). 
            \details Description of the parameter angleStop is: <BLOCKQUOTE>\copydoc angleStop </BLOCKQUOTE> 
            \see angleStop for more information */ 
        inline virtual double getAngleStop() const  
        {
            return this->angleStop; 
        }
    protected:
         /** \brief if we only draw an arc, this is the starting angle */
        double angleStart;
         /** \brief if we only draw an arc, this is the ending angle */
        double angleStop;
};




/*! \brief This JKQTPPlotObject is used to draw a chord
    \ingroup jkqtplotter_geoplots

    \image html plot_geochord.png
 */
class LIB_EXPORT JKQTPGeoChord: public JKQTPGeoPie {
        Q_OBJECT
    public:
        /*! \brief class constructor

            \param parent the parent plotter class
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
        JKQTPGeoChord(JKQTBasePlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);
        /*! \brief class constructor

            \param parent the parent plotter class
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
        JKQTPGeoChord(JKQTPlotter* parent, double x, double y, double width, double height, double angleStart, double angleStop, QColor color=QColor("black"), double lineWidth=1, Qt::PenStyle style=Qt::SolidLine, QColor fillColor=QColor("transparent"), Qt::BrushStyle fillStyle=Qt::SolidPattern);

        /** \copydoc JKQTPPlotObject::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotObject::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
};


#endif // jkqtpgraphsgeometric_H_INCLUDED
