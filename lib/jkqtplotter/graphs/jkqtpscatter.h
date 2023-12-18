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

#ifndef jkqtpscatter_H
#define jkqtpscatter_H


#include <QString>
#include <QPainter>
#include <QPair>
#include <functional>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/graphs/jkqtprange.h"
#include "jkqtplotter/graphs/jkqtplines.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;
//class JKQTPColorPaletteStyleAndToolsMixin;




/*! \brief This implements xy scatter plots. This also alows to draw symbols at the data points.
    \ingroup jkqtplotter_linesymbolgraphs_scatter

    \image html JKQTPXYScatterGraph.png

    \note This classes can (and does by default) apply a line-compression strategy that improves plotting speed
          but reduces accuracy a bit. See JKQTPGraphLinesCompressionMixin for details.

    \see JKQTPXYScatterErrorGraph for a version with error indicators and JKQTPXYParametrizedScatterGraph for a more feature-rich version
         that allows to also change the color/size of the symbols from data
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYScatterGraph: public JKQTPXYGraph, public JKQTPGraphSymbolStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPXYScatterGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYScatterGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
        /** \brief set color of line and symbol */
        void setColor(QColor c);

    protected:





};






/*! \brief This implements xy scatter plots with x and y error indicators.
    \ingroup jkqtplotter_linesymbolgraphs_scatter

    \image html JKQTPXYScatterErrorGraph.png

    \see JKQTPXYScatterGraph, JKQTPXYGraphErrors
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYScatterErrorGraph: public JKQTPXYScatterGraph, public JKQTPXYGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPXYScatterErrorGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYScatterErrorGraph(JKQTPlotter* parent);

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








/*! \brief This implements xy scatter plots (like JKQTPXYScatterGraph), but the color and size of the symbols may be taken from a column.
    \ingroup jkqtplotter_linesymbolgraphs_param

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.

    \image html paramscatterplot.png "Different Styles of Parametrized Scatter/Line Graphs"

    \note This classes is meant for cases where you want to change the color/size/... of single symbols, in dependence
          of data. If you are looking for a simple scatter-plot without data-dependent properties, use JKQTPXYScatterGraph
          instead, which is faster.

    \note For the size, line width and symbol type columns, you can also set a functor, which converts the column value (optionally based
          also on the x- and y-location of the data point) into the local symbol size, symbol type or line width. Use the functions
          setLinewidthColumnFunctor(), setSymbolColumnFunctor() and setSizeColumnFunctor() to set these functors. This feature gives you
          even more control over the modification of the plot elements. Initially all functors are set to default values, that do not add
          any additional transformations.
          \image html JKQTPXYParametrizedScatterGraph_LinewidthFunctor.png
          \image html JKQTPXYParametrizedScatterGraph_SymbolFunctor.png

    \image html paramscatterplot_image_star.png "JKQTPXYParametrizedScatterGraph with symbols organized in a grid"


    \see JKQTPXYParametrizedErrorScatterGraph, \ref JKQTPlotterParamScatter , \ref JKQTPlotterParamScatterImage, \ref JKQTPlotterParametricCurves
*/
class JKQTPLOTTER_LIB_EXPORT JKQTPXYParametrizedScatterGraph: public JKQTPXYGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphSymbolStyleMixin, public JKQTPColorPaletteStyleAndToolsMixin {
        Q_OBJECT
    public:
        /** \brief functor, which converts the value of the symbol column (at a location x,y) into a JKQTPGraphSymbols */
        typedef std::function<JKQTPGraphSymbols(double x, double y, double symcolumn)> FunctorToSymbol;
        /** \brief functor, which converts the value of the line width column (at a location x,y) into a line width (in pt) */
        typedef std::function<double(double x, double y, double widcolumn)> FunctorToWidth;
        /** \brief functor, which converts the value of the line size column (at a location x,y) into a symbol size (in pt) */
        typedef std::function<double(double x, double y, double sizecolumn)> FunctorToSize;

        /** \brief class constructor */
        JKQTPXYParametrizedScatterGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYParametrizedScatterGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /** \copydoc sizeColumn */
        void setSizeColumn(int __value);
        /** \copydoc sizeColumn */
        void setSizeColumn (size_t __value);
        /** \copydoc sizeColumn */
        int getSizeColumn() const;
        /** \brief defines a functor, which converts a value from the sizeColumn into an actual symbol size in pt
         *
         *  \code
         *      graph6->setSizeColumnFunctor([](double x, double y, double w) {
         *          return fabs(sin(w/3.0)*25.0);
         *      });
         *  \endcode
         *
         *  \see setSizeColumnFunctor(), clearSizeColumnFunctor() and getSizeColumnFunctor()
         */
        void setSizeColumnFunctor(FunctorToSize ff);
        /** \brief defines a standard (1:1) functor, which converts a value from the sizeColumn into an actual symbol size in pt
         *  \see setSizeColumnFunctor(), clearSizeColumnFunctor() and getSizeColumnFunctor()
         */
        void clearSizeColumnFunctor();
        /** \brief returns the current functor, which converts a value from the sizeColumn into an actual symbol size in pt
         *  \see setSizeColumnFunctor(), clearSizeColumnFunctor() and getSizeColumnFunctor()
         */
        FunctorToSize getSizeColumnFunctor();


        /** \copydoc colorColumn */
        void setColorColumn(int __value);
        /** \copydoc colorColumn */
        int getColorColumn() const;
        /** \copydoc colorColumn */
        void setColorColumn (size_t __value);


        /** \copydoc symbolColumn */
        void setSymbolColumn(int __value);
        /** \copydoc symbolColumn */
        int getSymbolColumn() const;
        /** \copydoc symbolColumn */
        void setSymbolColumn (size_t __value);
        /** \brief defines a functor, which converts a value from the symbolColumn into an actual symbol type
         *
         *  \code
         *      graph1->setSymbolColumnFunctor([](double x, double y, double sym) -> JKQTPGraphSymbols {
         *          if (sym<Ndata/2) {
         *              return JKQTPGraphSymbols::JKQTPCircle;
         *          } else if (sym>Ndata/2) {
         *              return JKQTPGraphSymbols::JKQTPFilledCircle;
         *          } else {
         *              return JKQTPGraphSymbols::JKQTPPlus;
         *          }
         *      });
         *  \endcode
         *
         *  \image html JKQTPXYParametrizedScatterGraph_SymbolFunctor.png
         *  \see setSymbolColumnFunctor(), clearSymbolColumnFunctor() and getSymbolColumnFunctor(), setMappedSymbolColumnFunctor(), \ref JKQTPlotterParamScatter
         */
        void setSymbolColumnFunctor(FunctorToSymbol ff);
        /** \brief defines a standard (1:1) functor, which converts a value from the symbolColumn into an actual symbol type
         *  \image html JKQTPXYParametrizedScatterGraph_SymbolFunctor.png
         *  \see setSymbolColumnFunctor(), clearSymbolColumnFunctor() and getSymbolColumnFunctor(), setMappedSymbolColumnFunctor(), \ref JKQTPlotterParamScatter
         */
        void clearSymbolColumnFunctor();
        /** \brief defines a special functor, which converts a value from the symbolColumn into an actual symbol type,
         *         where the relation between value and symbol is defined by a map double->symbol
         *
         *  This generates a special functor, which will convert values to symbols by using the values from the given map
         *  and these rules:
         *    - everything below the smallest value is mapped to the smallest value's symbol
         *    - everything abive the largest value is mapped to the largest value's symbol
         *    - everything in between is mapped to the symbol of the value closest to the actual value
         *  .
         *
         * \code
         *     QMap<double, JKQTPGraphSymbols> mapped;
         *     mapped[0]=JKQTPGraphSymbols::JKQTPCircle;
         *     mapped[5]=JKQTPGraphSymbols::JKQTPFilledCircle;
         *     mapped[10]=JKQTPGraphSymbols::JKQTPPlus;
         *     graph1->setMappedSymbolColumnFunctor(mapped);
         * \endcode
         *
         *  \see setSymbolColumnFunctor(), clearSymbolColumnFunctor() and getSymbolColumnFunctor(), setMappedSymbolColumnFunctor(), \ref JKQTPlotterParamScatter
         */
        void setMappedSymbolColumnFunctor(const QMap<double, JKQTPGraphSymbols>& symmap);
        /** \brief returns the current functor, which converts a value from the symbolColumn into an actual symbol type
         *  \see setSymbolColumnFunctor(), clearSymbolColumnFunctor() and getSymbolColumnFunctor(), setMappedSymbolColumnFunctor(), \ref JKQTPlotterParamScatter
         */
        FunctorToSymbol getSymbolColumnFunctor();


        /** \copydoc linewidthColumn */
        void setLinewidthColumn(int __value);
        /** \copydoc linewidthColumn */
        int getLinewidthColumn() const;
        /** \copydoc linewidthColumn */
        void setLinewidthColumn( size_t __value);
        /** \brief defines a functor, which converts a value from the symbolColumn into an actual line width in pt
         *
         *  \code
         *      graph6->setLinewidthColumnFunctor([](double x, double y, double w) {
         *          return fabs(sin(w/3.0)*25.0);
         *      });
         *  \endcode
         *
         *  \image html JKQTPXYParametrizedScatterGraph_LinewidthFunctor.png
         *
         *  \see setLinewidthColumnFunctor(), clearLinewidthColumnFunctor() and getLinewidthColumnFunctor(), \ref JKQTPlotterParamScatter
         */
        void setLinewidthColumnFunctor(FunctorToWidth ff);
        /** \brief defines a standard (1:1) functor, which converts a value from the symbolColumn into an actual line width in pt
         *
         *  \image html JKQTPXYParametrizedScatterGraph_LinewidthFunctor.png
         *
         *  \see setLinewidthColumnFunctor(), clearLinewidthColumnFunctor() and getLinewidthColumnFunctor(), \ref JKQTPlotterParamScatter
         */
        void clearLinewidthColumnFunctor();
        /** \brief returns the current functor, which converts a value from the symbolColumn into an actual line width in pt
         *  \see setLinewidthColumnFunctor(), clearLinewidthColumnFunctor() and getLinewidthColumnFunctor(), \ref JKQTPlotterParamScatter
         */
        FunctorToWidth getLinewidthColumnFunctor();




        /** \copydoc colorColumnContainsRGB */
        void setColorColumnContainsRGB(bool __value);
        /** \copydoc colorColumnContainsRGB */
        bool getColorColumnContainsRGB() const;

        /** \copydoc gridModeForSymbolSize */
        void setGridModeForSymbolSize(bool __value);
        /** \copydoc gridModeForSymbolSize */
        bool getGridModeForSymbolSize() const;
        /** \copydoc gridDeltaX */
        void setGridDeltaX(double __value);
        /** \copydoc gridDeltaX */
        double getGridDeltaX() const;
        /** \copydoc gridDeltaY */
        void setGridDeltaY(double __value);
        /** \copydoc gridDeltaY */
        double getGridDeltaY() const;
        /** \copydoc gridSymbolFractionSize */
        void setGridSymbolFractionSize(double __value);
        /** \copydoc gridSymbolFractionSize */
        double getGridSymbolFractionSize() const;

        /** \copydoc symbolFillDerivationMode */
        JKQTPColorDerivationMode getSymbolFillDerivationMode()  const;
        /** \copydoc symbolFillDerivationMode */
        void setSymbolFillDerivationMode(JKQTPColorDerivationMode m);

        /** \copydoc  JKQTPGraph::setParent() */
        virtual void setParent(JKQTBasePlotter* parent) override;


        /** \copydoc  JKQTPGraph::getOutsideSize() */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) override;

        /** \copydoc  JKQTPGraph::drawOutside() */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) override;


        /** \brief determine min/max data value of the image */
        virtual void cbGetDataMinMax(double& imin, double& imax) override;
        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;
        /** \copydoc drawLine */
        void setDrawLine(bool __value);
        /** \copydoc drawLine */
        bool getDrawLine() const;
        /** \copydoc drawLineInForeground */
        void setDrawLineInForeground(bool __value);
        /** \copydoc drawLineInForeground */
        bool getDrawLineInForeground() const;

        /** \brief set color of line and symbol */
        void setColor(QColor c);

    protected:

        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief indicates whether to draw the line behind or above the symbols */
        bool drawLineInForeground;
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

        /** \brief retrns the local line width for the i-th datapoint */
        double getLocalLineWidth(int i);
        /** \brief retrns the local symbol size for the i-th datapoint */
        double getLocalSymbolSize(int i);
        /** \brief retrns the local color for the i-th datapoint */
        QColor getLocalColor(int i) const;
        /** \brief retrns the local symbol type for the i-th datapoint */
        JKQTPGraphSymbols getLocalSymbolType(int i);


        /** \brief internally used to store the range of the color column */
        double intColMin;
        /** \brief internally used to store the range of the color column */
        double intColMax;

        /** \brief specifies how to derive the symbol fill color from the symbol/line color */
        JKQTPColorDerivationMode symbolFillDerivationMode;

        /** \brief functor, which converts the value of the symbol column (at a location x,y) into a JKQTPGraphSymbols */
        FunctorToSymbol m_toSymbolFunctor;
        /** \brief functor, which converts the value of the line width column (at a location x,y) into a line width (in pt) */
        FunctorToWidth m_toWidthPtFunctor;
        /** \brief functor, which converts the value of the line size column (at a location x,y) into a symbol size (in pt) */
        FunctorToSize m_toSizePtFunctor;

        /** \brief helper class for setMappedSymbolColumnFunctor() */
        struct MappedSymbolFunctor {
          QMap<double, JKQTPGraphSymbols> mapping;
          JKQTPXYParametrizedScatterGraph* parent;
          MappedSymbolFunctor(const QMap<double, JKQTPGraphSymbols>& mapping_, JKQTPXYParametrizedScatterGraph *graph);
          JKQTPGraphSymbols operator()(double x, double y, double symcolumn) const;
        };

        /** \brief helper struct, which describes a single symbol */
        struct SymbolDescription {
            double x;
            double y;
            JKQTPGraphSymbols type;
            double size;
            QColor color;
            QColor fillColor;
        };
};




/*! \brief This implements xy scatter plots (like JKQTPXYScatterGraph), but the color and size of the symbols may be taken from a column. with errorbars
    \ingroup jkqtplotter_linesymbolgraphs_param

    set the properties sizeColumn and/or colorColumn to change the size and/or color of the symbols according to the values in the column.

    \image html screen_parmetrizedplots_datatable.png

    \see JKQTPXYParametrizedScatterGraph, \ref JKQTPlotterParamScatter, jkqtpstatAddXErrorParametrizedScatterGraph(), jkqtpstatAddYErrorParametrizedScatterGraph()
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYParametrizedErrorScatterGraph: public JKQTPXYParametrizedScatterGraph, public JKQTPXYGraphErrors {
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
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, const JKQTBasePlotter* parent, const JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int index, QColor& errorLineColor, QColor& errorFillColor) const override;

};





#endif // jkqtpscatter_H
