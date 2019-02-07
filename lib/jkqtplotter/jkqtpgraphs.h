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
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"

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

        /*! \copydoc color
            \see see color for details */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \copydoc color
            \see see color for details */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual void setFillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual QColor getFillColor() const  
        {
            return this->fillColor; 
        }
        /*! \copydoc style
            \see see style for details */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \copydoc style
            \see see style for details */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \copydoc symbol
            \see see symbol for details */ 
        inline virtual void setSymbol(const JKQTPGraphSymbols & __value)  
        {
            this->symbol = __value;
        } 
        /*! \copydoc symbol
            \see see symbol for details */ 
        inline virtual JKQTPGraphSymbols getSymbol() const  
        {
            return this->symbol; 
        }
        /*! \copydoc symbolWidth
            \see see symbolWidth for details */ 
        inline virtual void setSymbolWidth(double __value)
        {
            this->symbolWidth = __value;
        } 
        /*! \copydoc symbolWidth
            \see see symbolWidth for details */ 
        inline virtual double getSymbolWidth() const  
        {
            return this->symbolWidth; 
        }
        /*! \copydoc symbolSize
            \see see symbolSize for details */ 
        inline virtual void setSymbolSize(double __value)
        {
            this->symbolSize = __value;
        } 
        /*! \copydoc symbolSize
            \see see symbolSize for details */ 
        inline virtual double getSymbolSize() const  
        {
            return this->symbolSize; 
        }
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
        }
        /*! \copydoc drawSelectionLine
            \see see drawSelectionLine for details */ 
        inline virtual void setDrawSelectionLine(bool __value)  
        {
            this->drawSelectionLine = __value;
        } 
        /*! \copydoc drawSelectionLine
            \see see drawSelectionLine for details */ 
        inline virtual bool getDrawSelectionLine() const  
        {
            return this->drawSelectionLine; 
        }
        /*! \copydoc selectionLineColor
            \see see selectionLineColor for details */ 
        inline virtual void setSelectionLineColor(const QColor & __value)  
        {
            this->selectionLineColor = __value;
        } 
        /*! \copydoc selectionLineColor
            \see see selectionLineColor for details */ 
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

        /*! \copydoc sizeColumn
            \see see sizeColumn for details */ 
        inline virtual void setSizeColumn(int __value)  
        {
            this->sizeColumn = __value;
        } 
        /*! \copydoc sizeColumn
            \see see sizeColumn for details */ 
        inline virtual int getSizeColumn() const  
        {
            return this->sizeColumn; 
        }
        /*! \brief sets the property sizeColumn ( \copybrief sizeColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter sizeColumn is:  <BLOCKQUOTE>\copydoc sizeColumn </BLOCKQUOTE>
            \see sizeColumn for more information */ 
        inline virtual void setSizeColumn (size_t __value) { this->sizeColumn = static_cast<int>(__value); }
        /*! \copydoc colorColumn
            \see see colorColumn for details */ 
        inline virtual void setColorColumn(int __value)  
        {
            this->colorColumn = __value;
        } 
        /*! \copydoc colorColumn
            \see see colorColumn for details */ 
        inline virtual int getColorColumn() const  
        {
            return this->colorColumn; 
        }
        /*! \brief sets the property colorColumn ( \copybrief colorColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter colorColumn is:  <BLOCKQUOTE>\copydoc colorColumn </BLOCKQUOTE>
            \see colorColumn for more information */ 
        inline virtual void setColorColumn (size_t __value) { this->colorColumn = static_cast<int>(__value); }
        /*! \copydoc symbolColumn
            \see see symbolColumn for details */ 
        inline virtual void setSymbolColumn(int __value)  
        {
            this->symbolColumn = __value;
        } 
        /*! \copydoc symbolColumn
            \see see symbolColumn for details */ 
        inline virtual int getSymbolColumn() const  
        {
            return this->symbolColumn; 
        }
        /*! \brief sets the property symbolColumn ( \copybrief symbolColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter symbolColumn is:  <BLOCKQUOTE>\copydoc symbolColumn </BLOCKQUOTE>
            \see symbolColumn for more information */ 
        inline virtual void setSymbolColumn (size_t __value) { this->symbolColumn = static_cast<int>(__value); }
        /*! \copydoc linewidthColumn
            \see see linewidthColumn for details */ 
        inline virtual void setLinewidthColumn(int __value)  
        {
            this->linewidthColumn = __value;
        } 
        /*! \copydoc linewidthColumn
            \see see linewidthColumn for details */ 
        inline virtual int getLinewidthColumn() const  
        {
            return this->linewidthColumn; 
        }
        /*! \brief sets the property linewidthColumn ( \copybrief linewidthColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter linewidthColumn is:  <BLOCKQUOTE>\copydoc linewidthColumn </BLOCKQUOTE>
            \see linewidthColumn for more information */ 
        inline virtual void setLinewidthColumn( size_t __value) { this->linewidthColumn = static_cast<int>(__value); }
        /*! \copydoc colorColumnContainsRGB
            \see see colorColumnContainsRGB for details */ 
        inline virtual void setColorColumnContainsRGB(bool __value)  
        {
            this->colorColumnContainsRGB = __value;
        } 
        /*! \copydoc colorColumnContainsRGB
            \see see colorColumnContainsRGB for details */ 
        inline virtual bool getColorColumnContainsRGB() const  
        {
            return this->colorColumnContainsRGB; 
        }
        /*! \copydoc gridModeForSymbolSize
            \see see gridModeForSymbolSize for details */ 
        inline virtual void setGridModeForSymbolSize(bool __value)  
        {
            this->gridModeForSymbolSize = __value;
        } 
        /*! \copydoc gridModeForSymbolSize
            \see see gridModeForSymbolSize for details */ 
        inline virtual bool getGridModeForSymbolSize() const  
        {
            return this->gridModeForSymbolSize; 
        }
        /*! \copydoc gridDeltaX
            \see see gridDeltaX for details */ 
        inline virtual void setGridDeltaX(double __value)
        {
            this->gridDeltaX = __value;
        } 
        /*! \copydoc gridDeltaX
            \see see gridDeltaX for details */ 
        inline virtual double getGridDeltaX() const  
        {
            return this->gridDeltaX; 
        }
        /*! \copydoc gridDeltaY
            \see see gridDeltaY for details */ 
        inline virtual void setGridDeltaY(double __value)
        {
            this->gridDeltaY = __value;
        } 
        /*! \copydoc gridDeltaY
            \see see gridDeltaY for details */ 
        inline virtual double getGridDeltaY() const  
        {
            return this->gridDeltaY; 
        }
        /*! \copydoc gridSymbolFractionSize
            \see see gridSymbolFractionSize for details */ 
        inline virtual void setGridSymbolFractionSize(double __value)
        {
            this->gridSymbolFractionSize = __value;
        } 
        /*! \copydoc gridSymbolFractionSize
            \see see gridSymbolFractionSize for details */ 
        inline virtual double getGridSymbolFractionSize() const  
        {
            return this->gridSymbolFractionSize; 
        }
        /*! \copydoc symbolFillDerivationMode
            \see see symbolFillDerivationMode for details */
        inline virtual JKQTPColorDerivationMode getSymbolFillDerivationMode()  const
        {
            return this->symbolFillDerivationMode;
        }
        /*! \copydoc symbolFillDerivationMode
            \see see symbolFillDerivationMode for details */
        inline virtual void setSymbolFillDerivationMode(JKQTPColorDerivationMode m)
        {
            this->symbolFillDerivationMode=m;
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

        /** \brief specifies how to derive the symbol fill color from the symbol/line color */
        JKQTPColorDerivationMode symbolFillDerivationMode;

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

        /*! \copydoc color
            \see see color for details */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \copydoc color
            \see see color for details */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual void setFillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual QColor getFillColor() const  
        {
            return this->fillColor; 
        }
        /*! \copydoc fillStyle
            \see see fillStyle for details */ 
        inline virtual void setFillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \copydoc fillStyle
            \see see fillStyle for details */ 
        inline virtual Qt::BrushStyle getFillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \copydoc style
            \see see style for details */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \copydoc style
            \see see style for details */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
        }
        /*! \copydoc fillCurve
            \see see fillCurve for details */ 
        inline virtual void setFillCurve(bool __value)  
        {
            this->fillCurve = __value;
        } 
        /*! \copydoc fillCurve
            \see see fillCurve for details */ 
        inline virtual bool getFillCurve() const  
        {
            return this->fillCurve; 
        }
        /*! \copydoc stepType
            \see see stepType for details */ 
        inline virtual void setStepType(const JKQTPStepType & __value)  
        {
            this->stepType = __value;
        } 
        /*! \copydoc stepType
            \see see stepType for details */ 
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

    \image html JKQTPHorizontalRange.png
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

        /*! \copydoc color
            \see see color for details */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \copydoc color
            \see see color for details */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual void setFillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual QColor getFillColor() const  
        {
            return this->fillColor; 
        }
        /*! \copydoc fillStyle
            \see see fillStyle for details */ 
        inline virtual void setFillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \copydoc fillStyle
            \see see fillStyle for details */ 
        inline virtual Qt::BrushStyle getFillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \copydoc style
            \see see style for details */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \copydoc style
            \see see style for details */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \copydoc centerColor
            \see see centerColor for details */ 
        inline virtual void setCenterColor(const QColor & __value)  
        {
            this->centerColor = __value;
        } 
        /*! \copydoc centerColor
            \see see centerColor for details */ 
        inline virtual QColor getCenterColor() const  
        {
            return this->centerColor; 
        }
        /*! \copydoc centerStyle
            \see see centerStyle for details */ 
        inline virtual void setCenterStyle(const Qt::PenStyle & __value)  
        {
            this->centerStyle = __value;
        } 
        /*! \copydoc centerStyle
            \see see centerStyle for details */ 
        inline virtual Qt::PenStyle getCenterStyle() const  
        {
            return this->centerStyle; 
        }
        /*! \copydoc centerLineWidth
            \see see centerLineWidth for details */ 
        inline virtual void setCenterLineWidth(double __value)
        {
            this->centerLineWidth = __value;
        } 
        /*! \copydoc centerLineWidth
            \see see centerLineWidth for details */ 
        inline virtual double getCenterLineWidth() const  
        {
            return this->centerLineWidth; 
        }
        /*! \copydoc rangeMin
            \see see rangeMin for details */ 
        inline virtual void setRangeMin(double __value)
        {
            this->rangeMin = __value;
        } 
        /*! \copydoc rangeMin
            \see see rangeMin for details */ 
        inline virtual double getRangeMin() const  
        {
            return this->rangeMin; 
        }
        /*! \copydoc rangeMax
            \see see rangeMax for details */ 
        inline virtual void setRangeMax(double __value)
        {
            this->rangeMax = __value;
        } 
        /*! \copydoc rangeMax
            \see see rangeMax for details */ 
        inline virtual double getRangeMax() const  
        {
            return this->rangeMax; 
        }
        /*! \copydoc sizeMin
            \see see sizeMin for details */ 
        inline virtual void setSizeMin(double __value)
        {
            this->sizeMin = __value;
        } 
        /*! \copydoc sizeMin
            \see see sizeMin for details */ 
        inline virtual double getSizeMin() const  
        {
            return this->sizeMin; 
        }
        /*! \copydoc sizeMax
            \see see sizeMax for details */ 
        inline virtual void setSizeMax(double __value)
        {
            this->sizeMax = __value;
        } 
        /*! \copydoc sizeMax
            \see see sizeMax for details */ 
        inline virtual double getSizeMax() const  
        {
            return this->sizeMax; 
        }
        /*! \copydoc unlimitedSizeMin
            \see see unlimitedSizeMin for details */ 
        inline virtual void setUnlimitedSizeMin(bool __value)  
        {
            this->unlimitedSizeMin = __value;
        } 
        /*! \copydoc unlimitedSizeMin
            \see see unlimitedSizeMin for details */ 
        inline virtual bool getUnlimitedSizeMin() const  
        {
            return this->unlimitedSizeMin; 
        }
        /*! \copydoc unlimitedSizeMax
            \see see unlimitedSizeMax for details */ 
        inline virtual void setUnlimitedSizeMax(bool __value)  
        {
            this->unlimitedSizeMax = __value;
        } 
        /*! \copydoc unlimitedSizeMax
            \see see unlimitedSizeMax for details */ 
        inline virtual bool getUnlimitedSizeMax() const  
        {
            return this->unlimitedSizeMax; 
        }

        /*! \copydoc rangeCenter
            \see see rangeCenter for details */ 
        inline virtual void setRangeCenter(double __value)
        {
            this->rangeCenter = __value;
        } 
        /*! \copydoc rangeCenter
            \see see rangeCenter for details */ 
        inline virtual double getRangeCenter() const  
        {
            return this->rangeCenter; 
        }
        /*! \copydoc plotCenterLine
            \see see plotCenterLine for details */ 
        inline virtual void setPlotCenterLine(bool __value)  
        {
            this->plotCenterLine = __value;
        } 
        /*! \copydoc plotCenterLine
            \see see plotCenterLine for details */ 
        inline virtual bool getPlotCenterLine() const  
        {
            return this->plotCenterLine; 
        }
        /*! \copydoc invertedRange
            \see see invertedRange for details */ 
        inline virtual void setInvertedRange(bool __value)  
        {
            this->invertedRange = __value;
        } 
        /*! \copydoc invertedRange
            \see see invertedRange for details */ 
        inline virtual bool getInvertedRange() const  
        {
            return this->invertedRange; 
        }
        /*! \copydoc plotRange
            \see see plotRange for details */ 
        inline virtual void setPlotRange(bool __value)  
        {
            this->plotRange = __value;
        } 
        /*! \copydoc plotRange
            \see see plotRange for details */ 
        inline virtual bool getPlotRange() const  
        {
            return this->plotRange; 
        }
        /*! \copydoc fillRange
            \see see fillRange for details */ 
        inline virtual void setFillRange(bool __value)  
        {
            this->fillRange = __value;
        } 
        /*! \copydoc fillRange
            \see see fillRange for details */ 
        inline virtual bool getFillRange() const  
        {
            return this->fillRange; 
        }
        /*! \copydoc plotRangeLines
            \see see plotRangeLines for details */ 
        inline virtual void setPlotRangeLines(bool __value)  
        {
            this->plotRangeLines = __value;
        } 
        /*! \copydoc plotRangeLines
            \see see plotRangeLines for details */ 
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

    \image html JKQTPVerticalRange.png
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
