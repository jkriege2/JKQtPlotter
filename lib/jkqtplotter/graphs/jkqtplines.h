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

#ifndef JKQTPLINES_H
#define JKQTPLINES_H


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
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;
//class JKQTPColorPaletteStyleAndToolsMixin;






/*! \brief This implements xy line plots. This also alows to draw symbols at the data points.
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html JKQTPXYLineGraph.png

    \note This classes can (and does by default) apply a line-compression strategy that improves plotting speed
          but reduces accuracy a bit. See JKQTPGraphLinesCompressionMixin for details.

    \see \ref JKQTPlotterAdvancedLineAndFillStyling, \ref JKQTPlotterSimpleTest, \ref JKQTPlotterSymbolsAndStyles,
         jkqtpstatAddVKDE1D(), jkqtpstatAddVKDE1DAutoranged(), jkqtpstatAddHKDE1D(), jkqtpstatAddHKDE1DAutoranged(),
         JKQTPGraphLinesCompressionMixin
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYLineGraph: public JKQTPXYGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphSymbolStyleMixin, public JKQTPGraphLinesCompressionMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        explicit JKQTPXYLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYLineGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

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


};






/*! \brief This implements xy line plots with x and y error indicators.
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html plot_errorbarlineplots.png
    \image html plot_errorlinelineplots.png
    \image html plot_errorpolygonlineplots.png

    \see jkqtpstatAddXYErrorLineGraph(), jkqtpstatAddXErrorLineGraph(), jkqtpstatAddYErrorLineGraph(), \ref JKQTPlotterErrorBarStyles, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYLineErrorGraph: public JKQTPXYLineGraph, public JKQTPXYGraphErrors {
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






#endif // JKQTPLINES_H
