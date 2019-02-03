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
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;
//class JKQTPColorPaletteTools;






/*! \brief This implements xy line plots. This also alows to draw symbols at the data points.
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html plot_lineplots.png
 */
class JKQTP_LIB_EXPORT JKQTPXYLineGraph: public JKQTPXYGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPXYLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYLineGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

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
        /*! \brief sets the property drawLine ( \copybrief drawLine ) to the specified \a __value. 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE>
            \see drawLine for more information */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine ( \copybrief drawLine ). 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE>
            \see drawLine for more information */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
        }
        /*! \brief sets the property drawSelectionLine ( \copybrief drawSelectionLine ) to the specified \a __value. 
            \details Description of the parameter drawSelectionLine is: <BLOCKQUOTE>\copydoc drawSelectionLine </BLOCKQUOTE>
            \see drawSelectionLine for more information */ 
        inline virtual void setDrawSelectionLine(bool __value)  
        {
            this->drawSelectionLine = __value;
        } 
        /*! \brief returns the property drawSelectionLine ( \copybrief drawSelectionLine ). 
            \details Description of the parameter drawSelectionLine is: <BLOCKQUOTE>\copydoc drawSelectionLine </BLOCKQUOTE>
            \see drawSelectionLine for more information */ 
        inline virtual bool getDrawSelectionLine() const  
        {
            return this->drawSelectionLine; 
        }
        /*! \brief sets the property selectionLineColor ( \copybrief selectionLineColor ) to the specified \a __value. 
            \details Description of the parameter selectionLineColor is: <BLOCKQUOTE>\copydoc selectionLineColor </BLOCKQUOTE>
            \see selectionLineColor for more information */ 
        inline virtual void setSelectionLineColor(const QColor & __value)  
        {
            this->selectionLineColor = __value;
        } 
        /*! \brief returns the property selectionLineColor ( \copybrief selectionLineColor ). 
            \details Description of the parameter selectionLineColor is: <BLOCKQUOTE>\copydoc selectionLineColor </BLOCKQUOTE>
            \see selectionLineColor for more information */ 
        inline virtual QColor getSelectionLineColor() const  
        {
            return this->selectionLineColor; 
        }

    protected:

        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;
        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph, given in pt */
        double lineWidth;
        /** \brief which symbol to use for the datapoints */
        JKQTPGraphSymbols symbol;
        /** \brief size (diameter in pt) of the symbol for the data points, given in pt */
        double symbolSize;
        /** \brief width (in pt) of the lines used to plot the symbol for the data points, given in pt */
        double symbolWidth;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;


        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
        QPen getSymbolPen(JKQTPEnhancedPainter& painter) const;
        QBrush getBrush(JKQTPEnhancedPainter &painter) const;



};









/*! \brief This implements xy scatter plots (like JKQTPXYLineGraph), but the color and size of the symbols may be taken from a column.
    \ingroup jkqtplotter_linesymbolgraphs_param

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.

    \image html jkqtplotter_simpletest_paramscatterplot.png "Different Styles of Parametrized Scatter/Line Graphs"

    \image html jkqtplotter_simpletest_paramscatterplot_image_star.png "JKQTPXYParametrizedScatterGraph with symbols organized in a grid"


    \see JKQTPXYParametrizedErrorScatterGraph, \ref JKQTPlotterParamScatter , \ref JKQTPlotterParamScatterImage
*/
class JKQTP_LIB_EXPORT JKQTPXYParametrizedScatterGraph: public JKQTPXYLineGraph, public JKQTPColorPaletteTools {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPXYParametrizedScatterGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYParametrizedScatterGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /*! \brief sets the property sizeColumn ( \copybrief sizeColumn ) to the specified \a __value. 
            \details Description of the parameter sizeColumn is: <BLOCKQUOTE>\copydoc sizeColumn </BLOCKQUOTE>
            \see sizeColumn for more information */ 
        inline virtual void setSizeColumn(int __value)  
        {
            this->sizeColumn = __value;
        } 
        /*! \brief returns the property sizeColumn ( \copybrief sizeColumn ). 
            \details Description of the parameter sizeColumn is: <BLOCKQUOTE>\copydoc sizeColumn </BLOCKQUOTE>
            \see sizeColumn for more information */ 
        inline virtual int getSizeColumn() const  
        {
            return this->sizeColumn; 
        }
        /*! \brief sets the property sizeColumn ( \copybrief sizeColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter sizeColumn is:  <BLOCKQUOTE>\copydoc sizeColumn </BLOCKQUOTE>
        	\see sizeColumn for more information */ 
        inline virtual void setSizeColumn (size_t __value) { this->sizeColumn = static_cast<int>(__value); }
        /*! \brief sets the property colorColumn ( \copybrief colorColumn ) to the specified \a __value. 
            \details Description of the parameter colorColumn is: <BLOCKQUOTE>\copydoc colorColumn </BLOCKQUOTE>
            \see colorColumn for more information */ 
        inline virtual void setColorColumn(int __value)  
        {
            this->colorColumn = __value;
        } 
        /*! \brief returns the property colorColumn ( \copybrief colorColumn ). 
            \details Description of the parameter colorColumn is: <BLOCKQUOTE>\copydoc colorColumn </BLOCKQUOTE>
            \see colorColumn for more information */ 
        inline virtual int getColorColumn() const  
        {
            return this->colorColumn; 
        }
        /*! \brief sets the property colorColumn ( \copybrief colorColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter colorColumn is:  <BLOCKQUOTE>\copydoc colorColumn </BLOCKQUOTE>
        	\see colorColumn for more information */ 
        inline virtual void setColorColumn (size_t __value) { this->colorColumn = static_cast<int>(__value); }
        /*! \brief sets the property symbolColumn ( \copybrief symbolColumn ) to the specified \a __value. 
            \details Description of the parameter symbolColumn is: <BLOCKQUOTE>\copydoc symbolColumn </BLOCKQUOTE>
            \see symbolColumn for more information */ 
        inline virtual void setSymbolColumn(int __value)  
        {
            this->symbolColumn = __value;
        } 
        /*! \brief returns the property symbolColumn ( \copybrief symbolColumn ). 
            \details Description of the parameter symbolColumn is: <BLOCKQUOTE>\copydoc symbolColumn </BLOCKQUOTE>
            \see symbolColumn for more information */ 
        inline virtual int getSymbolColumn() const  
        {
            return this->symbolColumn; 
        }
        /*! \brief sets the property symbolColumn ( \copybrief symbolColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter symbolColumn is:  <BLOCKQUOTE>\copydoc symbolColumn </BLOCKQUOTE>
        	\see symbolColumn for more information */ 
        inline virtual void setSymbolColumn (size_t __value) { this->symbolColumn = static_cast<int>(__value); }
        /*! \brief sets the property linewidthColumn ( \copybrief linewidthColumn ) to the specified \a __value. 
            \details Description of the parameter linewidthColumn is: <BLOCKQUOTE>\copydoc linewidthColumn </BLOCKQUOTE>
            \see linewidthColumn for more information */ 
        inline virtual void setLinewidthColumn(int __value)  
        {
            this->linewidthColumn = __value;
        } 
        /*! \brief returns the property linewidthColumn ( \copybrief linewidthColumn ). 
            \details Description of the parameter linewidthColumn is: <BLOCKQUOTE>\copydoc linewidthColumn </BLOCKQUOTE>
            \see linewidthColumn for more information */ 
        inline virtual int getLinewidthColumn() const  
        {
            return this->linewidthColumn; 
        }
        /*! \brief sets the property linewidthColumn ( \copybrief linewidthColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter linewidthColumn is:  <BLOCKQUOTE>\copydoc linewidthColumn </BLOCKQUOTE>
        	\see linewidthColumn for more information */ 
        inline virtual void setLinewidthColumn( size_t __value) { this->linewidthColumn = static_cast<int>(__value); }
        /*! \brief sets the property colorColumnContainsRGB ( \copybrief colorColumnContainsRGB ) to the specified \a __value. 
            \details Description of the parameter colorColumnContainsRGB is: <BLOCKQUOTE>\copydoc colorColumnContainsRGB </BLOCKQUOTE>
            \see colorColumnContainsRGB for more information */ 
        inline virtual void setColorColumnContainsRGB(bool __value)  
        {
            this->colorColumnContainsRGB = __value;
        } 
        /*! \brief returns the property colorColumnContainsRGB ( \copybrief colorColumnContainsRGB ). 
            \details Description of the parameter colorColumnContainsRGB is: <BLOCKQUOTE>\copydoc colorColumnContainsRGB </BLOCKQUOTE>
            \see colorColumnContainsRGB for more information */ 
        inline virtual bool getColorColumnContainsRGB() const  
        {
            return this->colorColumnContainsRGB; 
        }
        /*! \brief sets the property gridModeForSymbolSize ( \copybrief gridModeForSymbolSize ) to the specified \a __value. 
            \details Description of the parameter gridModeForSymbolSize is: <BLOCKQUOTE>\copydoc gridModeForSymbolSize </BLOCKQUOTE>
            \see gridModeForSymbolSize for more information */ 
        inline virtual void setGridModeForSymbolSize(bool __value)  
        {
            this->gridModeForSymbolSize = __value;
        } 
        /*! \brief returns the property gridModeForSymbolSize ( \copybrief gridModeForSymbolSize ). 
            \details Description of the parameter gridModeForSymbolSize is: <BLOCKQUOTE>\copydoc gridModeForSymbolSize </BLOCKQUOTE>
            \see gridModeForSymbolSize for more information */ 
        inline virtual bool getGridModeForSymbolSize() const  
        {
            return this->gridModeForSymbolSize; 
        }
        /*! \brief sets the property gridDeltaX ( \copybrief gridDeltaX ) to the specified \a __value. 
            \details Description of the parameter gridDeltaX is: <BLOCKQUOTE>\copydoc gridDeltaX </BLOCKQUOTE>
            \see gridDeltaX for more information */ 
        inline virtual void setGridDeltaX(double __value)
        {
            this->gridDeltaX = __value;
        } 
        /*! \brief returns the property gridDeltaX ( \copybrief gridDeltaX ). 
            \details Description of the parameter gridDeltaX is: <BLOCKQUOTE>\copydoc gridDeltaX </BLOCKQUOTE>
            \see gridDeltaX for more information */ 
        inline virtual double getGridDeltaX() const  
        {
            return this->gridDeltaX; 
        }
        /*! \brief sets the property gridDeltaY ( \copybrief gridDeltaY ) to the specified \a __value. 
            \details Description of the parameter gridDeltaY is: <BLOCKQUOTE>\copydoc gridDeltaY </BLOCKQUOTE>
            \see gridDeltaY for more information */ 
        inline virtual void setGridDeltaY(double __value)
        {
            this->gridDeltaY = __value;
        } 
        /*! \brief returns the property gridDeltaY ( \copybrief gridDeltaY ). 
            \details Description of the parameter gridDeltaY is: <BLOCKQUOTE>\copydoc gridDeltaY </BLOCKQUOTE>
            \see gridDeltaY for more information */ 
        inline virtual double getGridDeltaY() const  
        {
            return this->gridDeltaY; 
        }
        /*! \brief sets the property gridSymbolFractionSize ( \copybrief gridSymbolFractionSize ) to the specified \a __value. 
            \details Description of the parameter gridSymbolFractionSize is: <BLOCKQUOTE>\copydoc gridSymbolFractionSize </BLOCKQUOTE>
            \see gridSymbolFractionSize for more information */ 
        inline virtual void setGridSymbolFractionSize(double __value)
        {
            this->gridSymbolFractionSize = __value;
        } 
        /*! \brief returns the property gridSymbolFractionSize ( \copybrief gridSymbolFractionSize ). 
            \details Description of the parameter gridSymbolFractionSize is: <BLOCKQUOTE>\copydoc gridSymbolFractionSize </BLOCKQUOTE>
            \see gridSymbolFractionSize for more information */ 
        inline virtual double getGridSymbolFractionSize() const  
        {
            return this->gridSymbolFractionSize; 
        }

        virtual void setParent(JKQTBasePlotter* parent) override;


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
        /** \copydoc JKQTPGraph::usesColumn() */
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
        JKQTPGraphSymbols getLocalSymbolType(int i);


        /** \brief internally used to store the range of the color column */
        double intColMin;
        /** \brief internally used to store the range of the color column */
        double intColMax;

};




/*! \brief This implements xy line plots with x and y error indicators.
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html plot_errorbarlineplots.png
    \image html plot_errorlinelineplots.png
    \image html plot_errorpolygonlineplots.png
 */
class JKQTP_LIB_EXPORT JKQTPXYLineErrorGraph: public JKQTPXYLineGraph, public JKQTPXYGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPXYLineErrorGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYLineErrorGraph(JKQTPlotter* parent);

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
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter)  override;
};



/*! \brief This implements xy scatter plots (like JKQTPXYLineGraph), but the color and size of the symbols may be taken from a column. with errorbars
    \ingroup jkqtplotter_linesymbolgraphs_param

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.

    \image html screen_parmetrizedplots_datatable.png

    \see JKQTPXYParametrizedScatterGraph, \ref JKQTPlotterParamScatter
 */
class JKQTP_LIB_EXPORT JKQTPXYParametrizedErrorScatterGraph: public JKQTPXYParametrizedScatterGraph, public JKQTPXYGraphErrors {
        Q_OBJECT
    public:
        JKQTPXYParametrizedErrorScatterGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPXYParametrizedErrorScatterGraph(JKQTPlotter* parent);


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
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsBefore(JKQTPEnhancedPainter& painter)  override;

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int index, QColor& errorColor, QColor& errorFillColor) override;

};



/*! \brief This implements a step plot with values \f$ \left(x, f(x) \right) \f$
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html jkqtplotter_simpletest_stepplots.png

    \see JKQTPStepVerticalGraph, \ref JKQTPlotterStepPlot
 */
class JKQTP_LIB_EXPORT JKQTPStepHorizontalGraph: public JKQTPXYGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPStepHorizontalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPStepHorizontalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

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
        /*! \brief sets the property drawLine ( \copybrief drawLine ) to the specified \a __value. 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE>
            \see drawLine for more information */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine ( \copybrief drawLine ). 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE>
            \see drawLine for more information */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
        }
        /*! \brief sets the property fillCurve ( \copybrief fillCurve ) to the specified \a __value. 
            \details Description of the parameter fillCurve is: <BLOCKQUOTE>\copydoc fillCurve </BLOCKQUOTE>
            \see fillCurve for more information */ 
        inline virtual void setFillCurve(bool __value)  
        {
            this->fillCurve = __value;
        } 
        /*! \brief returns the property fillCurve ( \copybrief fillCurve ). 
            \details Description of the parameter fillCurve is: <BLOCKQUOTE>\copydoc fillCurve </BLOCKQUOTE>
            \see fillCurve for more information */ 
        inline virtual bool getFillCurve() const  
        {
            return this->fillCurve; 
        }
        /*! \brief sets the property stepType ( \copybrief stepType ) to the specified \a __value. 
            \details Description of the parameter stepType is: <BLOCKQUOTE>\copydoc stepType </BLOCKQUOTE>
            \see stepType for more information */ 
        inline virtual void setStepType(const JKQTPStepType & __value)  
        {
            this->stepType = __value;
        } 
        /*! \brief returns the property stepType ( \copybrief stepType ). 
            \details Description of the parameter stepType is: <BLOCKQUOTE>\copydoc stepType </BLOCKQUOTE>
            \see stepType for more information */ 
        inline virtual JKQTPStepType getStepType() const  
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
        /** \brief width (pixels) of the graph, given in pt */
        double lineWidth;
        /** \brief fill style, if the curve should be filled */
        Qt::BrushStyle fillStyle;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief indicates whether to fill the space between the curve and the x-axis */
        bool fillCurve;
        /** \brief specifies whether the steps elongate to the left, the right, or are centered around the \f$ x_i \f$ values. */
        JKQTPStepType stepType;

        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
};



/*! \brief This implements a step plot with values \f$ \left(f(y), y \right) \f$
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html plot_stepverplot1.png

    \see JKQTPStepHorizontalGraph, \ref JKQTPlotterStepPlot
 */
class JKQTP_LIB_EXPORT JKQTPStepVerticalGraph: public JKQTPStepHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPStepVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPStepVerticalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
};









/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_diverse

 */
class JKQTP_LIB_EXPORT JKQTPHorizontalRange: public JKQTPGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPHorizontalRange(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPHorizontalRange(JKQTPlotter* parent);

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
        /*! \brief sets the property centerColor ( \copybrief centerColor ) to the specified \a __value. 
            \details Description of the parameter centerColor is: <BLOCKQUOTE>\copydoc centerColor </BLOCKQUOTE>
            \see centerColor for more information */ 
        inline virtual void setCenterColor(const QColor & __value)  
        {
            this->centerColor = __value;
        } 
        /*! \brief returns the property centerColor ( \copybrief centerColor ). 
            \details Description of the parameter centerColor is: <BLOCKQUOTE>\copydoc centerColor </BLOCKQUOTE>
            \see centerColor for more information */ 
        inline virtual QColor getCenterColor() const  
        {
            return this->centerColor; 
        }
        /*! \brief sets the property centerStyle ( \copybrief centerStyle ) to the specified \a __value. 
            \details Description of the parameter centerStyle is: <BLOCKQUOTE>\copydoc centerStyle </BLOCKQUOTE>
            \see centerStyle for more information */ 
        inline virtual void setCenterStyle(const Qt::PenStyle & __value)  
        {
            this->centerStyle = __value;
        } 
        /*! \brief returns the property centerStyle ( \copybrief centerStyle ). 
            \details Description of the parameter centerStyle is: <BLOCKQUOTE>\copydoc centerStyle </BLOCKQUOTE>
            \see centerStyle for more information */ 
        inline virtual Qt::PenStyle getCenterStyle() const  
        {
            return this->centerStyle; 
        }
        /*! \brief sets the property centerLineWidth ( \copybrief centerLineWidth ) to the specified \a __value. 
            \details Description of the parameter centerLineWidth is: <BLOCKQUOTE>\copydoc centerLineWidth </BLOCKQUOTE>
            \see centerLineWidth for more information */ 
        inline virtual void setCenterLineWidth(double __value)
        {
            this->centerLineWidth = __value;
        } 
        /*! \brief returns the property centerLineWidth ( \copybrief centerLineWidth ). 
            \details Description of the parameter centerLineWidth is: <BLOCKQUOTE>\copydoc centerLineWidth </BLOCKQUOTE>
            \see centerLineWidth for more information */ 
        inline virtual double getCenterLineWidth() const  
        {
            return this->centerLineWidth; 
        }
        /*! \brief sets the property rangeMin ( \copybrief rangeMin ) to the specified \a __value. 
            \details Description of the parameter rangeMin is: <BLOCKQUOTE>\copydoc rangeMin </BLOCKQUOTE>
            \see rangeMin for more information */ 
        inline virtual void setRangeMin(double __value)
        {
            this->rangeMin = __value;
        } 
        /*! \brief returns the property rangeMin ( \copybrief rangeMin ). 
            \details Description of the parameter rangeMin is: <BLOCKQUOTE>\copydoc rangeMin </BLOCKQUOTE>
            \see rangeMin for more information */ 
        inline virtual double getRangeMin() const  
        {
            return this->rangeMin; 
        }
        /*! \brief sets the property rangeMax ( \copybrief rangeMax ) to the specified \a __value. 
            \details Description of the parameter rangeMax is: <BLOCKQUOTE>\copydoc rangeMax </BLOCKQUOTE>
            \see rangeMax for more information */ 
        inline virtual void setRangeMax(double __value)
        {
            this->rangeMax = __value;
        } 
        /*! \brief returns the property rangeMax ( \copybrief rangeMax ). 
            \details Description of the parameter rangeMax is: <BLOCKQUOTE>\copydoc rangeMax </BLOCKQUOTE>
            \see rangeMax for more information */ 
        inline virtual double getRangeMax() const  
        {
            return this->rangeMax; 
        }
        /*! \brief sets the property sizeMin ( \copybrief sizeMin ) to the specified \a __value. 
            \details Description of the parameter sizeMin is: <BLOCKQUOTE>\copydoc sizeMin </BLOCKQUOTE>
            \see sizeMin for more information */ 
        inline virtual void setSizeMin(double __value)
        {
            this->sizeMin = __value;
        } 
        /*! \brief returns the property sizeMin ( \copybrief sizeMin ). 
            \details Description of the parameter sizeMin is: <BLOCKQUOTE>\copydoc sizeMin </BLOCKQUOTE>
            \see sizeMin for more information */ 
        inline virtual double getSizeMin() const  
        {
            return this->sizeMin; 
        }
        /*! \brief sets the property sizeMax ( \copybrief sizeMax ) to the specified \a __value. 
            \details Description of the parameter sizeMax is: <BLOCKQUOTE>\copydoc sizeMax </BLOCKQUOTE>
            \see sizeMax for more information */ 
        inline virtual void setSizeMax(double __value)
        {
            this->sizeMax = __value;
        } 
        /*! \brief returns the property sizeMax ( \copybrief sizeMax ). 
            \details Description of the parameter sizeMax is: <BLOCKQUOTE>\copydoc sizeMax </BLOCKQUOTE>
            \see sizeMax for more information */ 
        inline virtual double getSizeMax() const  
        {
            return this->sizeMax; 
        }
        /*! \brief sets the property unlimitedSizeMin ( \copybrief unlimitedSizeMin ) to the specified \a __value. 
            \details Description of the parameter unlimitedSizeMin is: <BLOCKQUOTE>\copydoc unlimitedSizeMin </BLOCKQUOTE>
            \see unlimitedSizeMin for more information */ 
        inline virtual void setUnlimitedSizeMin(bool __value)  
        {
            this->unlimitedSizeMin = __value;
        } 
        /*! \brief returns the property unlimitedSizeMin ( \copybrief unlimitedSizeMin ). 
            \details Description of the parameter unlimitedSizeMin is: <BLOCKQUOTE>\copydoc unlimitedSizeMin </BLOCKQUOTE>
            \see unlimitedSizeMin for more information */ 
        inline virtual bool getUnlimitedSizeMin() const  
        {
            return this->unlimitedSizeMin; 
        }
        /*! \brief sets the property unlimitedSizeMax ( \copybrief unlimitedSizeMax ) to the specified \a __value. 
            \details Description of the parameter unlimitedSizeMax is: <BLOCKQUOTE>\copydoc unlimitedSizeMax </BLOCKQUOTE>
            \see unlimitedSizeMax for more information */ 
        inline virtual void setUnlimitedSizeMax(bool __value)  
        {
            this->unlimitedSizeMax = __value;
        } 
        /*! \brief returns the property unlimitedSizeMax ( \copybrief unlimitedSizeMax ). 
            \details Description of the parameter unlimitedSizeMax is: <BLOCKQUOTE>\copydoc unlimitedSizeMax </BLOCKQUOTE>
            \see unlimitedSizeMax for more information */ 
        inline virtual bool getUnlimitedSizeMax() const  
        {
            return this->unlimitedSizeMax; 
        }

        /*! \brief sets the property rangeCenter ( \copybrief rangeCenter ) to the specified \a __value. 
            \details Description of the parameter rangeCenter is: <BLOCKQUOTE>\copydoc rangeCenter </BLOCKQUOTE>
            \see rangeCenter for more information */ 
        inline virtual void setRangeCenter(double __value)
        {
            this->rangeCenter = __value;
        } 
        /*! \brief returns the property rangeCenter ( \copybrief rangeCenter ). 
            \details Description of the parameter rangeCenter is: <BLOCKQUOTE>\copydoc rangeCenter </BLOCKQUOTE>
            \see rangeCenter for more information */ 
        inline virtual double getRangeCenter() const  
        {
            return this->rangeCenter; 
        }
        /*! \brief sets the property plotCenterLine ( \copybrief plotCenterLine ) to the specified \a __value. 
            \details Description of the parameter plotCenterLine is: <BLOCKQUOTE>\copydoc plotCenterLine </BLOCKQUOTE>
            \see plotCenterLine for more information */ 
        inline virtual void setPlotCenterLine(bool __value)  
        {
            this->plotCenterLine = __value;
        } 
        /*! \brief returns the property plotCenterLine ( \copybrief plotCenterLine ). 
            \details Description of the parameter plotCenterLine is: <BLOCKQUOTE>\copydoc plotCenterLine </BLOCKQUOTE>
            \see plotCenterLine for more information */ 
        inline virtual bool getPlotCenterLine() const  
        {
            return this->plotCenterLine; 
        }
        /*! \brief sets the property invertedRange ( \copybrief invertedRange ) to the specified \a __value. 
            \details Description of the parameter invertedRange is: <BLOCKQUOTE>\copydoc invertedRange </BLOCKQUOTE>
            \see invertedRange for more information */ 
        inline virtual void setInvertedRange(bool __value)  
        {
            this->invertedRange = __value;
        } 
        /*! \brief returns the property invertedRange ( \copybrief invertedRange ). 
            \details Description of the parameter invertedRange is: <BLOCKQUOTE>\copydoc invertedRange </BLOCKQUOTE>
            \see invertedRange for more information */ 
        inline virtual bool getInvertedRange() const  
        {
            return this->invertedRange; 
        }
        /*! \brief sets the property plotRange ( \copybrief plotRange ) to the specified \a __value. 
            \details Description of the parameter plotRange is: <BLOCKQUOTE>\copydoc plotRange </BLOCKQUOTE>
            \see plotRange for more information */ 
        inline virtual void setPlotRange(bool __value)  
        {
            this->plotRange = __value;
        } 
        /*! \brief returns the property plotRange ( \copybrief plotRange ). 
            \details Description of the parameter plotRange is: <BLOCKQUOTE>\copydoc plotRange </BLOCKQUOTE>
            \see plotRange for more information */ 
        inline virtual bool getPlotRange() const  
        {
            return this->plotRange; 
        }
        /*! \brief sets the property fillRange ( \copybrief fillRange ) to the specified \a __value. 
            \details Description of the parameter fillRange is: <BLOCKQUOTE>\copydoc fillRange </BLOCKQUOTE>
            \see fillRange for more information */ 
        inline virtual void setFillRange(bool __value)  
        {
            this->fillRange = __value;
        } 
        /*! \brief returns the property fillRange ( \copybrief fillRange ). 
            \details Description of the parameter fillRange is: <BLOCKQUOTE>\copydoc fillRange </BLOCKQUOTE>
            \see fillRange for more information */ 
        inline virtual bool getFillRange() const  
        {
            return this->fillRange; 
        }
        /*! \brief sets the property plotRangeLines ( \copybrief plotRangeLines ) to the specified \a __value. 
            \details Description of the parameter plotRangeLines is: <BLOCKQUOTE>\copydoc plotRangeLines </BLOCKQUOTE>
            \see plotRangeLines for more information */ 
        inline virtual void setPlotRangeLines(bool __value)  
        {
            this->plotRangeLines = __value;
        } 
        /*! \brief returns the property plotRangeLines ( \copybrief plotRangeLines ). 
            \details Description of the parameter plotRangeLines is: <BLOCKQUOTE>\copydoc plotRangeLines </BLOCKQUOTE>
            \see plotRangeLines for more information */ 
        inline virtual bool getPlotRangeLines() const  
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
        /** \brief width (pt) of the graph, given in pt */
        double lineWidth;
        /** \brief width (pt) of the center line graph, given in pt */
        double centerLineWidth;
        /** \brief fill style, if the curve should be filled */
        Qt::BrushStyle fillStyle;

        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;

};






/*! \brief simply marks a range (and possibly a centerline) in a plot. This may be used to display e.g. mean +/- stddev
           or a range of interest, or the range of good values, ...
    \ingroup jkqtplotter_diverse

 */
class JKQTP_LIB_EXPORT JKQTPVerticalRange: public JKQTPHorizontalRange {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPVerticalRange(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPVerticalRange(JKQTPlotter* parent);

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
