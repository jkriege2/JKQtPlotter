/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

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

/** \file jkqtpgraphs.h
  * \ingroup jkqtplotter
  */

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"

#ifndef jkqtpgraphs_H
#define jkqtpgraphs_H

// forward declarations
class JKQtBasePlotter;
class JKQtPlotter;
class JKQTPcoordinateAxis;
class JKQTPdatastore;
//class JKQTPColorPaletteTools;






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
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor to the specified \a __value. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual void set_fillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual QColor get_fillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property style to the specified \a __value. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual void set_style(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual Qt::PenStyle get_style() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual void set_lineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual double get_lineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property symbol to the specified \a __value. 
            \details Description of the parameter symbol is: <CENTER>\copybrief symbol.</CENTER> 
            \see symbol for more information */ 
        inline virtual void set_symbol(const JKQTPgraphSymbols & __value)  
        {
            this->symbol = __value;
        } 
        /*! \brief returns the property symbol. 
            \details Description of the parameter symbol is: <CENTER>\copybrief symbol.</CENTER> 
            \see symbol for more information */ 
        inline virtual JKQTPgraphSymbols get_symbol() const  
        {
            return this->symbol; 
        }
        /*! \brief sets the property symbolWidth to the specified \a __value. 
            \details Description of the parameter symbolWidth is: <CENTER>\copybrief symbolWidth.</CENTER> 
            \see symbolWidth for more information */ 
        inline virtual void set_symbolWidth(double __value)
        {
            this->symbolWidth = __value;
        } 
        /*! \brief returns the property symbolWidth. 
            \details Description of the parameter symbolWidth is: <CENTER>\copybrief symbolWidth.</CENTER> 
            \see symbolWidth for more information */ 
        inline virtual double get_symbolWidth() const  
        {
            return this->symbolWidth; 
        }
        /*! \brief sets the property symbolSize to the specified \a __value. 
            \details Description of the parameter symbolSize is: <CENTER>\copybrief symbolSize.</CENTER> 
            \see symbolSize for more information */ 
        inline virtual void set_symbolSize(double __value)
        {
            this->symbolSize = __value;
        } 
        /*! \brief returns the property symbolSize. 
            \details Description of the parameter symbolSize is: <CENTER>\copybrief symbolSize.</CENTER> 
            \see symbolSize for more information */ 
        inline virtual double get_symbolSize() const  
        {
            return this->symbolSize; 
        }
        /*! \brief sets the property drawLine to the specified \a __value. 
            \details Description of the parameter drawLine is: <CENTER>\copybrief drawLine.</CENTER> 
            \see drawLine for more information */ 
        inline virtual void set_drawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine. 
            \details Description of the parameter drawLine is: <CENTER>\copybrief drawLine.</CENTER> 
            \see drawLine for more information */ 
        inline virtual bool get_drawLine() const  
        {
            return this->drawLine; 
        }
        /*! \brief sets the property drawSelectionLine to the specified \a __value. 
            \details Description of the parameter drawSelectionLine is: <CENTER>\copybrief drawSelectionLine.</CENTER> 
            \see drawSelectionLine for more information */ 
        inline virtual void set_drawSelectionLine(bool __value)  
        {
            this->drawSelectionLine = __value;
        } 
        /*! \brief returns the property drawSelectionLine. 
            \details Description of the parameter drawSelectionLine is: <CENTER>\copybrief drawSelectionLine.</CENTER> 
            \see drawSelectionLine for more information */ 
        inline virtual bool get_drawSelectionLine() const  
        {
            return this->drawSelectionLine; 
        }
        /*! \brief sets the property selectionLineColor to the specified \a __value. 
            \details Description of the parameter selectionLineColor is: <CENTER>\copybrief selectionLineColor.</CENTER> 
            \see selectionLineColor for more information */ 
        inline virtual void set_selectionLineColor(const QColor & __value)  
        {
            this->selectionLineColor = __value;
        } 
        /*! \brief returns the property selectionLineColor. 
            \details Description of the parameter selectionLineColor is: <CENTER>\copybrief selectionLineColor.</CENTER> 
            \see selectionLineColor for more information */ 
        inline virtual QColor get_selectionLineColor() const  
        {
            return this->selectionLineColor; 
        }

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
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /*! \brief sets the property sizeColumn to the specified \a __value. 
            \details Description of the parameter sizeColumn is: <CENTER>\copybrief sizeColumn.</CENTER> 
            \see sizeColumn for more information */ 
        inline virtual void set_sizeColumn(int __value)  
        {
            this->sizeColumn = __value;
        } 
        /*! \brief returns the property sizeColumn. 
            \details Description of the parameter sizeColumn is: <CENTER>\copybrief sizeColumn.</CENTER> 
            \see sizeColumn for more information */ 
        inline virtual int get_sizeColumn() const  
        {
            return this->sizeColumn; 
        }
        /*! \brief sets the property sizeColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter sizeColumn is:  <CENTER>\copybrief sizeColumn.</CENTER> 
        	\see sizeColumn for more information */ 
        inline virtual void set_sizeColumn (size_t __value) { this->sizeColumn = static_cast<int>(__value); }
        /*! \brief sets the property colorColumn to the specified \a __value. 
            \details Description of the parameter colorColumn is: <CENTER>\copybrief colorColumn.</CENTER> 
            \see colorColumn for more information */ 
        inline virtual void set_colorColumn(int __value)  
        {
            this->colorColumn = __value;
        } 
        /*! \brief returns the property colorColumn. 
            \details Description of the parameter colorColumn is: <CENTER>\copybrief colorColumn.</CENTER> 
            \see colorColumn for more information */ 
        inline virtual int get_colorColumn() const  
        {
            return this->colorColumn; 
        }
        /*! \brief sets the property colorColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter colorColumn is:  <CENTER>\copybrief colorColumn.</CENTER> 
        	\see colorColumn for more information */ 
        inline virtual void set_colorColumn (size_t __value) { this->colorColumn = static_cast<int>(__value); }
        /*! \brief sets the property symbolColumn to the specified \a __value. 
            \details Description of the parameter symbolColumn is: <CENTER>\copybrief symbolColumn.</CENTER> 
            \see symbolColumn for more information */ 
        inline virtual void set_symbolColumn(int __value)  
        {
            this->symbolColumn = __value;
        } 
        /*! \brief returns the property symbolColumn. 
            \details Description of the parameter symbolColumn is: <CENTER>\copybrief symbolColumn.</CENTER> 
            \see symbolColumn for more information */ 
        inline virtual int get_symbolColumn() const  
        {
            return this->symbolColumn; 
        }
        /*! \brief sets the property symbolColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter symbolColumn is:  <CENTER>\copybrief symbolColumn.</CENTER> 
        	\see symbolColumn for more information */ 
        inline virtual void set_symbolColumn (size_t __value) { this->symbolColumn = static_cast<int>(__value); }
        /*! \brief sets the property linewidthColumn to the specified \a __value. 
            \details Description of the parameter linewidthColumn is: <CENTER>\copybrief linewidthColumn.</CENTER> 
            \see linewidthColumn for more information */ 
        inline virtual void set_linewidthColumn(int __value)  
        {
            this->linewidthColumn = __value;
        } 
        /*! \brief returns the property linewidthColumn. 
            \details Description of the parameter linewidthColumn is: <CENTER>\copybrief linewidthColumn.</CENTER> 
            \see linewidthColumn for more information */ 
        inline virtual int get_linewidthColumn() const  
        {
            return this->linewidthColumn; 
        }
        /*! \brief sets the property linewidthColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter linewidthColumn is:  <CENTER>\copybrief linewidthColumn.</CENTER> 
        	\see linewidthColumn for more information */ 
        inline virtual void set_linewidthColumn( size_t __value) { this->linewidthColumn = static_cast<int>(__value); }
        /*! \brief sets the property colorColumnContainsRGB to the specified \a __value. 
            \details Description of the parameter colorColumnContainsRGB is: <CENTER>\copybrief colorColumnContainsRGB.</CENTER> 
            \see colorColumnContainsRGB for more information */ 
        inline virtual void set_colorColumnContainsRGB(bool __value)  
        {
            this->colorColumnContainsRGB = __value;
        } 
        /*! \brief returns the property colorColumnContainsRGB. 
            \details Description of the parameter colorColumnContainsRGB is: <CENTER>\copybrief colorColumnContainsRGB.</CENTER> 
            \see colorColumnContainsRGB for more information */ 
        inline virtual bool get_colorColumnContainsRGB() const  
        {
            return this->colorColumnContainsRGB; 
        }
        /*! \brief sets the property gridModeForSymbolSize to the specified \a __value. 
            \details Description of the parameter gridModeForSymbolSize is: <CENTER>\copybrief gridModeForSymbolSize.</CENTER> 
            \see gridModeForSymbolSize for more information */ 
        inline virtual void set_gridModeForSymbolSize(bool __value)  
        {
            this->gridModeForSymbolSize = __value;
        } 
        /*! \brief returns the property gridModeForSymbolSize. 
            \details Description of the parameter gridModeForSymbolSize is: <CENTER>\copybrief gridModeForSymbolSize.</CENTER> 
            \see gridModeForSymbolSize for more information */ 
        inline virtual bool get_gridModeForSymbolSize() const  
        {
            return this->gridModeForSymbolSize; 
        }
        /*! \brief sets the property gridDeltaX to the specified \a __value. 
            \details Description of the parameter gridDeltaX is: <CENTER>\copybrief gridDeltaX.</CENTER> 
            \see gridDeltaX for more information */ 
        inline virtual void set_gridDeltaX(double __value)
        {
            this->gridDeltaX = __value;
        } 
        /*! \brief returns the property gridDeltaX. 
            \details Description of the parameter gridDeltaX is: <CENTER>\copybrief gridDeltaX.</CENTER> 
            \see gridDeltaX for more information */ 
        inline virtual double get_gridDeltaX() const  
        {
            return this->gridDeltaX; 
        }
        /*! \brief sets the property gridDeltaY to the specified \a __value. 
            \details Description of the parameter gridDeltaY is: <CENTER>\copybrief gridDeltaY.</CENTER> 
            \see gridDeltaY for more information */ 
        inline virtual void set_gridDeltaY(double __value)
        {
            this->gridDeltaY = __value;
        } 
        /*! \brief returns the property gridDeltaY. 
            \details Description of the parameter gridDeltaY is: <CENTER>\copybrief gridDeltaY.</CENTER> 
            \see gridDeltaY for more information */ 
        inline virtual double get_gridDeltaY() const  
        {
            return this->gridDeltaY; 
        }
        /*! \brief sets the property gridSymbolFractionSize to the specified \a __value. 
            \details Description of the parameter gridSymbolFractionSize is: <CENTER>\copybrief gridSymbolFractionSize.</CENTER> 
            \see gridSymbolFractionSize for more information */ 
        inline virtual void set_gridSymbolFractionSize(double __value)
        {
            this->gridSymbolFractionSize = __value;
        } 
        /*! \brief returns the property gridSymbolFractionSize. 
            \details Description of the parameter gridSymbolFractionSize is: <CENTER>\copybrief gridSymbolFractionSize.</CENTER> 
            \see gridSymbolFractionSize for more information */ 
        inline virtual double get_gridSymbolFractionSize() const  
        {
            return this->gridSymbolFractionSize; 
        }

        virtual void setParent(JKQtBasePlotter* parent) override;


        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) override;

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) override;


        /** \brief determine min/max data value of the image */
        virtual void cbGetDataMinMax(double& imin, double& imax) override;
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


        virtual double getInternalDataMin() const  override;
        virtual double getInternalDataMax() const  override;
    protected:
        /** \brief this column contains the symbol size in pt */
        int sizeColumn;
        /** \brief this column contains the symbol color */
        int colorColumn;
        /** \brief this column contains the symbol type */
        int symbolColumn;
        /** \brief this column contains the line width */
        int linewidthColumn;

        /** \brief if the gridModeForSymbolSize mode is actiavted (true), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        bool gridModeForSymbolSize;
        /** \brief if the gridModeForSymbolSize mode is actiavted (true), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridDeltaX;
        /** \brief if the gridModeForSymbolSize mode is actiavted (true), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridDeltaY;
        /** \brief if the gridModeForSymbolSize mode is actiavted (true), the plot assumes that the scatter symbols are ordered in a grid. It the uses the given griDeltaX and gridDeltaY to calculate the symbol size, so they fill the available space to a fraction gridSymbolFractionSize. */
        double gridSymbolFractionSize;

        /** \brief if this is true, the value in the colorColumn is converted to an integer, representing a color in ARGB format (as in QRgb) */
        bool colorColumnContainsRGB;

        double getLocalLineWidth(int i);
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
        JKQTPxyLineErrorGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxyLineErrorGraph(JKQtPlotter* parent);

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
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter)  override;
};



/*! \brief This implements xy scatter plots (like JKQTPxyLineGraph), but the color and size of the symbols may be taken from a column. with errorbars
    \ingroup jkqtplotter_plots

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.
 */
class LIB_EXPORT JKQTPxyParametrizedErrorScatterGraph: public JKQTPxyParametrizedScatterGraph, public JKQTPxyGraphErrors {
        Q_OBJECT
    public:
        JKQTPxyParametrizedErrorScatterGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPxyParametrizedErrorScatterGraph(JKQtPlotter* parent);


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
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter)  override;

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQtBasePlotter* parent, JKQTPgraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPerrorPlotstyle xErrorStyle, JKQTPerrorPlotstyle yErrorStyle, int index, QColor& errorColor, QColor& errorFillColor) override;

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
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor to the specified \a __value. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual void set_fillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual QColor get_fillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle to the specified \a __value. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual void set_fillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle get_fillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property style to the specified \a __value. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual void set_style(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual Qt::PenStyle get_style() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual void set_lineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual double get_lineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property drawLine to the specified \a __value. 
            \details Description of the parameter drawLine is: <CENTER>\copybrief drawLine.</CENTER> 
            \see drawLine for more information */ 
        inline virtual void set_drawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine. 
            \details Description of the parameter drawLine is: <CENTER>\copybrief drawLine.</CENTER> 
            \see drawLine for more information */ 
        inline virtual bool get_drawLine() const  
        {
            return this->drawLine; 
        }
        /*! \brief sets the property fillCurve to the specified \a __value. 
            \details Description of the parameter fillCurve is: <CENTER>\copybrief fillCurve.</CENTER> 
            \see fillCurve for more information */ 
        inline virtual void set_fillCurve(bool __value)  
        {
            this->fillCurve = __value;
        } 
        /*! \brief returns the property fillCurve. 
            \details Description of the parameter fillCurve is: <CENTER>\copybrief fillCurve.</CENTER> 
            \see fillCurve for more information */ 
        inline virtual bool get_fillCurve() const  
        {
            return this->fillCurve; 
        }
        /*! \brief sets the property stepType to the specified \a __value. 
            \details Description of the parameter stepType is: <CENTER>\copybrief stepType.</CENTER> 
            \see stepType for more information */ 
        inline virtual void set_stepType(const JKQTPstepType & __value)  
        {
            this->stepType = __value;
        } 
        /*! \brief returns the property stepType. 
            \details Description of the parameter stepType is: <CENTER>\copybrief stepType.</CENTER> 
            \see stepType for more information */ 
        inline virtual JKQTPstepType get_stepType() const  
        {
            return this->stepType; 
        }
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
        /** \brief specifies whether the steps elongate to the left, the right, or are centered around the \f$ x_i \f$ values. */
        JKQTPstepType stepType;

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
        JKQTPstepVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPstepVerticalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
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
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

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
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        void setDrawCenterLineOnly();

        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor to the specified \a __value. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual void set_fillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual QColor get_fillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle to the specified \a __value. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual void set_fillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle get_fillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property style to the specified \a __value. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual void set_style(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual Qt::PenStyle get_style() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual void set_lineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual double get_lineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property centerColor to the specified \a __value. 
            \details Description of the parameter centerColor is: <CENTER>\copybrief centerColor.</CENTER> 
            \see centerColor for more information */ 
        inline virtual void set_centerColor(const QColor & __value)  
        {
            this->centerColor = __value;
        } 
        /*! \brief returns the property centerColor. 
            \details Description of the parameter centerColor is: <CENTER>\copybrief centerColor.</CENTER> 
            \see centerColor for more information */ 
        inline virtual QColor get_centerColor() const  
        {
            return this->centerColor; 
        }
        /*! \brief sets the property centerStyle to the specified \a __value. 
            \details Description of the parameter centerStyle is: <CENTER>\copybrief centerStyle.</CENTER> 
            \see centerStyle for more information */ 
        inline virtual void set_centerStyle(const Qt::PenStyle & __value)  
        {
            this->centerStyle = __value;
        } 
        /*! \brief returns the property centerStyle. 
            \details Description of the parameter centerStyle is: <CENTER>\copybrief centerStyle.</CENTER> 
            \see centerStyle for more information */ 
        inline virtual Qt::PenStyle get_centerStyle() const  
        {
            return this->centerStyle; 
        }
        /*! \brief sets the property centerLineWidth to the specified \a __value. 
            \details Description of the parameter centerLineWidth is: <CENTER>\copybrief centerLineWidth.</CENTER> 
            \see centerLineWidth for more information */ 
        inline virtual void set_centerLineWidth(double __value)
        {
            this->centerLineWidth = __value;
        } 
        /*! \brief returns the property centerLineWidth. 
            \details Description of the parameter centerLineWidth is: <CENTER>\copybrief centerLineWidth.</CENTER> 
            \see centerLineWidth for more information */ 
        inline virtual double get_centerLineWidth() const  
        {
            return this->centerLineWidth; 
        }
        /*! \brief sets the property rangeMin to the specified \a __value. 
            \details Description of the parameter rangeMin is: <CENTER>\copybrief rangeMin.</CENTER> 
            \see rangeMin for more information */ 
        inline virtual void set_rangeMin(double __value)
        {
            this->rangeMin = __value;
        } 
        /*! \brief returns the property rangeMin. 
            \details Description of the parameter rangeMin is: <CENTER>\copybrief rangeMin.</CENTER> 
            \see rangeMin for more information */ 
        inline virtual double get_rangeMin() const  
        {
            return this->rangeMin; 
        }
        /*! \brief sets the property rangeMax to the specified \a __value. 
            \details Description of the parameter rangeMax is: <CENTER>\copybrief rangeMax.</CENTER> 
            \see rangeMax for more information */ 
        inline virtual void set_rangeMax(double __value)
        {
            this->rangeMax = __value;
        } 
        /*! \brief returns the property rangeMax. 
            \details Description of the parameter rangeMax is: <CENTER>\copybrief rangeMax.</CENTER> 
            \see rangeMax for more information */ 
        inline virtual double get_rangeMax() const  
        {
            return this->rangeMax; 
        }
        /*! \brief sets the property sizeMin to the specified \a __value. 
            \details Description of the parameter sizeMin is: <CENTER>\copybrief sizeMin.</CENTER> 
            \see sizeMin for more information */ 
        inline virtual void set_sizeMin(double __value)
        {
            this->sizeMin = __value;
        } 
        /*! \brief returns the property sizeMin. 
            \details Description of the parameter sizeMin is: <CENTER>\copybrief sizeMin.</CENTER> 
            \see sizeMin for more information */ 
        inline virtual double get_sizeMin() const  
        {
            return this->sizeMin; 
        }
        /*! \brief sets the property sizeMax to the specified \a __value. 
            \details Description of the parameter sizeMax is: <CENTER>\copybrief sizeMax.</CENTER> 
            \see sizeMax for more information */ 
        inline virtual void set_sizeMax(double __value)
        {
            this->sizeMax = __value;
        } 
        /*! \brief returns the property sizeMax. 
            \details Description of the parameter sizeMax is: <CENTER>\copybrief sizeMax.</CENTER> 
            \see sizeMax for more information */ 
        inline virtual double get_sizeMax() const  
        {
            return this->sizeMax; 
        }
        /*! \brief sets the property unlimitedSizeMin to the specified \a __value. 
            \details Description of the parameter unlimitedSizeMin is: <CENTER>\copybrief unlimitedSizeMin.</CENTER> 
            \see unlimitedSizeMin for more information */ 
        inline virtual void set_unlimitedSizeMin(bool __value)  
        {
            this->unlimitedSizeMin = __value;
        } 
        /*! \brief returns the property unlimitedSizeMin. 
            \details Description of the parameter unlimitedSizeMin is: <CENTER>\copybrief unlimitedSizeMin.</CENTER> 
            \see unlimitedSizeMin for more information */ 
        inline virtual bool get_unlimitedSizeMin() const  
        {
            return this->unlimitedSizeMin; 
        }
        /*! \brief sets the property unlimitedSizeMax to the specified \a __value. 
            \details Description of the parameter unlimitedSizeMax is: <CENTER>\copybrief unlimitedSizeMax.</CENTER> 
            \see unlimitedSizeMax for more information */ 
        inline virtual void set_unlimitedSizeMax(bool __value)  
        {
            this->unlimitedSizeMax = __value;
        } 
        /*! \brief returns the property unlimitedSizeMax. 
            \details Description of the parameter unlimitedSizeMax is: <CENTER>\copybrief unlimitedSizeMax.</CENTER> 
            \see unlimitedSizeMax for more information */ 
        inline virtual bool get_unlimitedSizeMax() const  
        {
            return this->unlimitedSizeMax; 
        }

        /*! \brief sets the property rangeCenter to the specified \a __value. 
            \details Description of the parameter rangeCenter is: <CENTER>\copybrief rangeCenter.</CENTER> 
            \see rangeCenter for more information */ 
        inline virtual void set_rangeCenter(double __value)
        {
            this->rangeCenter = __value;
        } 
        /*! \brief returns the property rangeCenter. 
            \details Description of the parameter rangeCenter is: <CENTER>\copybrief rangeCenter.</CENTER> 
            \see rangeCenter for more information */ 
        inline virtual double get_rangeCenter() const  
        {
            return this->rangeCenter; 
        }
        /*! \brief sets the property plotCenterLine to the specified \a __value. 
            \details Description of the parameter plotCenterLine is: <CENTER>\copybrief plotCenterLine.</CENTER> 
            \see plotCenterLine for more information */ 
        inline virtual void set_plotCenterLine(bool __value)  
        {
            this->plotCenterLine = __value;
        } 
        /*! \brief returns the property plotCenterLine. 
            \details Description of the parameter plotCenterLine is: <CENTER>\copybrief plotCenterLine.</CENTER> 
            \see plotCenterLine for more information */ 
        inline virtual bool get_plotCenterLine() const  
        {
            return this->plotCenterLine; 
        }
        /*! \brief sets the property invertedRange to the specified \a __value. 
            \details Description of the parameter invertedRange is: <CENTER>\copybrief invertedRange.</CENTER> 
            \see invertedRange for more information */ 
        inline virtual void set_invertedRange(bool __value)  
        {
            this->invertedRange = __value;
        } 
        /*! \brief returns the property invertedRange. 
            \details Description of the parameter invertedRange is: <CENTER>\copybrief invertedRange.</CENTER> 
            \see invertedRange for more information */ 
        inline virtual bool get_invertedRange() const  
        {
            return this->invertedRange; 
        }
        /*! \brief sets the property plotRange to the specified \a __value. 
            \details Description of the parameter plotRange is: <CENTER>\copybrief plotRange.</CENTER> 
            \see plotRange for more information */ 
        inline virtual void set_plotRange(bool __value)  
        {
            this->plotRange = __value;
        } 
        /*! \brief returns the property plotRange. 
            \details Description of the parameter plotRange is: <CENTER>\copybrief plotRange.</CENTER> 
            \see plotRange for more information */ 
        inline virtual bool get_plotRange() const  
        {
            return this->plotRange; 
        }
        /*! \brief sets the property fillRange to the specified \a __value. 
            \details Description of the parameter fillRange is: <CENTER>\copybrief fillRange.</CENTER> 
            \see fillRange for more information */ 
        inline virtual void set_fillRange(bool __value)  
        {
            this->fillRange = __value;
        } 
        /*! \brief returns the property fillRange. 
            \details Description of the parameter fillRange is: <CENTER>\copybrief fillRange.</CENTER> 
            \see fillRange for more information */ 
        inline virtual bool get_fillRange() const  
        {
            return this->fillRange; 
        }
        /*! \brief sets the property plotRangeLines to the specified \a __value. 
            \details Description of the parameter plotRangeLines is: <CENTER>\copybrief plotRangeLines.</CENTER> 
            \see plotRangeLines for more information */ 
        inline virtual void set_plotRangeLines(bool __value)  
        {
            this->plotRangeLines = __value;
        } 
        /*! \brief returns the property plotRangeLines. 
            \details Description of the parameter plotRangeLines is: <CENTER>\copybrief plotRangeLines.</CENTER> 
            \see plotRangeLines for more information */ 
        inline virtual bool get_plotRangeLines() const  
        {
            return this->plotRangeLines; 
        }
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
        /** \brief if \c true, this fills the range with \a fillColor */
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
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

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
};

#endif // jkqtpgraphs_H
