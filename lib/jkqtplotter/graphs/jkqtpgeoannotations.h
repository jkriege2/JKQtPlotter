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

#ifndef jkqtpgeoannotations_H_INCLUDED
#define jkqtpgeoannotations_H_INCLUDED


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpgeobase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"




/*! \brief This virtual JKQTPPlotAnnotationElement descendent may be used to display a single symbol (marker).
    \ingroup jkqtplotter_annotations

    \see \ref JKQTPlotterGeometricGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoSymbol: public JKQTPPlotAnnotationElement, public JKQTPGraphSymbolStyleMixin {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
         */
        JKQTPGeoSymbol(JKQTBasePlotter* parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
         */
        JKQTPGeoSymbol(JKQTPlotter* parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
            \param color color of drawing
            \param fillColor fill color of the symbol (if filled)
        */
        JKQTPGeoSymbol(JKQTBasePlotter* parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, QColor color, QColor fillColor);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
            \param color color of drawing
            \param fillColor fill color of the symbol (if filled)
        */
        JKQTPGeoSymbol(JKQTPlotter* parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, QColor color, QColor fillColor);

        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
            \param color color of drawing
        */
        JKQTPGeoSymbol(JKQTBasePlotter* parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, QColor color);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
            \param symbolSize size of the symbol in pt
            \param color color of drawing
        */
        JKQTPGeoSymbol(JKQTPlotter* parent, double x, double y, JKQTPGraphSymbols symbol, double symbolSize, QColor color);


        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
        */
        JKQTPGeoSymbol(JKQTBasePlotter* parent, double x, double y, JKQTPGraphSymbols symbol);

        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
            \param symbol symbol type
          */
        JKQTPGeoSymbol(JKQTPlotter* parent, double x, double y, JKQTPGraphSymbols symbol);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
        */
        JKQTPGeoSymbol(JKQTBasePlotter* parent, double x, double y);

        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of symbol center
            \param y y-coordinate of symbol center
        */
        JKQTPGeoSymbol(JKQTPlotter* parent, double x, double y);
        /** \copydoc x */
        double getX() const;
        /** \copydoc y */ 
        double getY() const;
        /** \brief returns x and y at the same time \see x, y */
        QPointF getP() const;

        /** \copydoc JKQTPPlotAnnotationElement::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotAnnotationElement::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
    public slots:
        /** set the symbol color and symbol fill color  */
        virtual void setColor(QColor c);

        /** \copydoc x */ 
        void setX(double __value);
        /** \copydoc y */ 
        void setY(double __value);
        /** \brief set x and y at the same time \see x, y */
        void setP(const QPointF& p);

    protected:
        /** \brief x-position (in plot coordinates) of the symbol (symbol center) */
        double x;
        /** \brief y-position (in plot coordinates) of the symbol (symbol center) */
        double y;

};


/*! \brief This JKQTPPlotAnnotationElement is used to display text. It uses the JKQTMathText
           class in order to display LaTeX formulas.
    \ingroup jkqtplotter_annotations

    \see \ref JKQTPlotterGeometricGraphs
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoText: public JKQTPPlotAnnotationElement, public JKQTPGraphTextStyleMixin {
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
            \param fontSize base font size of text
        */
        JKQTPGeoText(JKQTBasePlotter* parent, double x, double y, const QString& text, double fontSize);
        /*! \brief class contructor

            \param parent parent plotter widget
            \param x x-coordinate of text
            \param y y-coordinate of text
            \param text the text to display
            \param fontSize base font size of text
        */
        JKQTPGeoText(JKQTPlotter* parent, double x, double y, const QString& text, double fontSize);


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

        /** \copydoc text */ 
        QString getText() const;
        /** \copydoc x */ 
        double getX() const;
        /** \copydoc y */ 
        double getY() const;
        /** \brief returns x and y at the same time \see x, y */
        QPointF getP() const;


        /** \copydoc JKQTPPlotAnnotationElement::getXMinMax()        */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotAnnotationElement::getYMinMax()        */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
	public slots:
        /** \brief set line and fill color */
        virtual void setColor(QColor c) ;
		
        /** \copydoc text */ 
        void setText(const QString & __value);
        /** \copydoc x */ 
        void setX(double __value);
        /** \copydoc y */ 
        void setY(double __value);
        /** \brief set x and y at the same time \see x, y */
        void setP(const QPointF& p);

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



#endif // jkqtpgeoannotations_H_INCLUDED
