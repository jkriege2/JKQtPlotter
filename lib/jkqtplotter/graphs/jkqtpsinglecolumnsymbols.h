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
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

#ifndef jkqtpgraphssinglecolumnsymbols_H_INCLUDED
#define jkqtpgraphssinglecolumnsymbols_H_INCLUDED



/*! \brief plots a 1-column set of data-values with symbols onto a JKQtPlotter/JKQtBasePlotter. the data from the single column
           is either interpreted as x- or y-values and the missing coordinate is taken from a fixed value position.
    \ingroup jkqtplotter_linesymbolgraphs_simple

    The first coordinate of the data is taken from dataColumn i.e.
      - the x-values if dataDirection==DataDirection::X
      - the y-values if dataDirection==DataDirection::Y
    .

    The second coordinate is:
      - if positionScatterStyle==NoScatter it is simply the value of position, e.g. <br>\image html JKQTPSingleColumnSymbolsGraph_NoScatter.png
      - if positionScatterStyle==RugPlot is like a NoScatter-plot, but with lines instead of symbols (see <a href="https://en.wikipedia.org/wiki/Rug_plot">https://en.wikipedia.org/wiki/Rug_plot</a>) <br>\image html JKQTPSingleColumnSymbolsGraph_RugPlot.png
      - if positionScatterStyle==RandomScatter it scatters around position in the range [position-width/2 ... position+width/2], e.g. <br>\image html JKQTPSingleColumnSymbolsGraph_RandomScatter.png
      - if positionScatterStyle==BeeSwarmScatter it scatters around position with an algorithm that tries to avoid any overlay of the plot symbols (SLOW!), e.g. <br>\image html JKQTPSingleColumnSymbolsGraph_BeeSwarmScatter.png
    .
    All example images above have dataDirection==DataDirection::X !

    \see \ref JKQTPlotterDistributionPlot, \ref JKQTPlotterViolinplotGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSingleColumnSymbolsGraph: public JKQTPSingleColumnGraph, public JKQTPGraphSymbolStyleMixin {
        Q_OBJECT
    public:
        /** \brief describes how data from dataColumn of a JKQTPSingleColumnSymbolsGraph positioned at position */
        enum ScatterStyle {
            NoScatter, /*!< \brief missing coordinate is exactly position for every datapoint in dataColumn \image html JKQTPSingleColumnSymbolsGraph_NoScatter.png */
            RugPlot, /*!< \brief like NoScatter but draws each data-point as a horzintal/vertical line, centered around position, not as a symbol ("rug plot", see e.g. <a href="https://en.wikipedia.org/wiki/Rug_plot">https://en.wikipedia.org/wiki/Rug_plot</a>).  \image html JKQTPSingleColumnSymbolsGraph_RugPlot.png */
            RandomScatter, /*!< \brief missing coordinate scatters around position (with distribution-width width ) \image html JKQTPSingleColumnSymbolsGraph_RandomScatter.png */
            BeeSwarmScatter, /*!< \brief missing coordinate scatters around position and the algorithm tries to prevent overlay of two symbols ("bee swarm plot", see e.g. <a href="https://www.r-statistics.com/2011/03/beeswarm-boxplot-and-plotting-it-with-r/">https://www.r-statistics.com/2011/03/beeswarm-boxplot-and-plotting-it-with-r/</a>). Note that this algorithm can be rather slow!!! \image html JKQTPSingleColumnSymbolsGraph_BeeSwarmScatter.png */
        };

        /** \brief class constructor */
        JKQTPSingleColumnSymbolsGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPSingleColumnSymbolsGraph(JKQTPlotter* parent);

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


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
        /** \brief set symbol color and fill color at the same time */
        void setColor(QColor col);

        /*! \copydoc position */
        void setPosition(double __value);
        /*! \copydoc position */
        double getPosition() const;

        /*! \copydoc width */
        void setWidth(double __value);
        /*! \copydoc width */
        double getWidth() const;



        /*! \copydoc positionScatterStyle */
        void setPositionScatterStyle(ScatterStyle __value);
        /*! \copydoc positionScatterStyle */
        ScatterStyle getPositionScatterStyle() const;



        /** \copydoc seedValue */
        void setSeedValue(unsigned int val);
        /** \copydoc seedValue */
        unsigned int getSeedValue() const;
    protected:
        /** \brief missing coordinate, i.e. if the data from \a dataColumn is interpreted as x-values, this is the y-position and vice versa (see DataDirection ) */
        double position;
        /** \brief width of data distribution around position for the missing coordinate (see positionScatterStyle and position) */
        double width;
        /** \brief how to distribute the datapoints from dataColumn at the location position */
        ScatterStyle positionScatterStyle;
        /** \brief seed value for the pseudo-random number generator used for drawing.
         *
         * This seed is used instead of e.g. the system time to ensure that replotting results in the
         * same plot every time. ou can set the seed using setSeedvalue() ) */
        unsigned int seedValue;

};




#endif // jkqtpgraphssinglecolumnsymbols_H_INCLUDED
