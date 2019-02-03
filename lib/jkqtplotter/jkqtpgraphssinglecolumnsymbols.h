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
#include "jkqtplotter/jkqtpgraphsbase.h"

#ifndef jkqtpgraphssinglecolumnsymbols_H_INCLUDED
#define jkqtpgraphssinglecolumnsymbols_H_INCLUDED



/*! \brief plots a 1-column set of data-values with symbols onto a JKQtPlotter/JKQtBasePlotter. the data from the single column
           is either interpreted as x- or y-values and the missing coordinate is taken from a fixed value position.
    \ingroup jkqtplotter_linesymbolgraphs

    The first coordinate of the data is taken from dataColumn i.e.
      - the x-values if dataDirection==DataDirection::X
      - the y-values if dataDirection==DataDirection::Y
    .

    The second coordinate is:
      - if positionScatterStyle==NoScatter it is simply the value of position, e.g. <br>\image html JKQTPSingleColumnSymbolsGraph_NoScatter.png
      - if positionScatterStyle==RandomScatter it scatters around position in the range [position-width/2 ... position+width/2], e.g. <br>\image html JKQTPSingleColumnSymbolsGraph_RandomScatter.png
      - if positionScatterStyle==BeeSwarmScatter it scatters around position with an algorithm that tries to avoid any overlay of the plot symbols (SLOW!), e.g. <br>\image html JKQTPSingleColumnSymbolsGraph_BeeSwarmScatter.png
    .
    All example images above have dataDirection==DataDirection::X !

    \see \ref JKQTPlotterDistributionPlot

 */
class JKQTP_LIB_EXPORT JKQTPSingleColumnSymbolsGraph: public JKQTPSingleColumnGraph {
        Q_OBJECT
    public:
        /** \brief describes how data from dataColumn of a JKQTPSingleColumnSymbolsGraph positioned at position */
        enum ScatterStyle {
            NoScatter, /*!< \brief missing coordinate is exactly position for every datapoint in dataColumn \image html JKQTPSingleColumnSymbolsGraph_NoScatter.png */
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

        /*! \brief sets the property position ( \copybrief position ) to the specified \a __value.
            \details Description of the parameter position is: <BLOCKQUOTE>\copydoc position </BLOCKQUOTE>
            \see position for more information */
        inline virtual void setPosition(double __value)
        {
            this->position = __value;
        }
        /*! \brief returns the property position ( \copybrief position ).
            \details Description of the parameter position is: <BLOCKQUOTE>\copydoc position </BLOCKQUOTE>
            \see position for more information */
        inline virtual double getPosition() const
        {
            return this->position;
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

        /*! \brief sets the property dataDirection ( \copybrief dataDirection ) to the specified \a __value.
            \details Description of the parameter dataDirection is: <BLOCKQUOTE>\copydoc dataDirection </BLOCKQUOTE>
            \see dataDirection for more information */
        inline virtual void setDataDirection(DataDirection __value)
        {
            this->dataDirection = __value;
        }
        /*! \brief returns the property dataDirection ( \copybrief dataDirection ).
            \details Description of the parameter dataDirection is: <BLOCKQUOTE>\copydoc dataDirection </BLOCKQUOTE>
            \see dataDirection for more information */
        inline virtual DataDirection getDataDirection() const
        {
            return this->dataDirection;
        }

        /*! \brief sets the property positionScatterStyle ( \copybrief positionScatterStyle ) to the specified \a __value.
            \details Description of the parameter positionScatterStyle is: <BLOCKQUOTE>\copydoc positionScatterStyle </BLOCKQUOTE>
            \see positionScatterStyle for more information */
        inline virtual void setPositionScatterStyle(ScatterStyle __value)
        {
            this->positionScatterStyle = __value;
        }
        /*! \brief returns the property positionScatterStyle ( \copybrief positionScatterStyle ).
            \details Description of the parameter positionScatterStyle is: <BLOCKQUOTE>\copydoc positionScatterStyle </BLOCKQUOTE>
            \see positionScatterStyle for more information */
        inline virtual ScatterStyle getPositionScatterStyle() const
        {
            return this->positionScatterStyle;
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
        /** \copydoc seedValue */
        inline void setSeedValue(unsigned int val) {
            seedValue=val;
        }
        /** \copydoc seedValue */
        inline unsigned int getSeedValue() const {
            return seedValue;
        }
    protected:
        /** \brief interpret the data from dataColumn either as X- or Y-data */
        DataDirection dataDirection;
        /** \brief missing coordinate, i.e. if the data from \a dataColumn is interpreted as x-values, this is the y-position and vice versa (see DataDirection ) */
        double position;
        /** \brief width of data distribution around position for the missing coordinate (see positionScatterStyle and position) */
        double width;
        /** \brief how to distribute the datapoints from dataColumn at the location position */
        ScatterStyle positionScatterStyle;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief which symbol to use for the datapoints */
        JKQTPGraphSymbols symbol;
        /** \brief size (diameter in pt) of the symbol for the data points */
        double symbolSize;
        /** \brief width (in pt) of the lines used to plot the symbol for the data points */
        double symbolWidth;
        /** \brief seed value for the pseudo-random number generator used for drawing.
         *
         * This seed is used instead of e.g. the system time to ensure that replotting results in the
         * same plot every time. ou can set the seed using setSeedvalue() ) */
        unsigned int seedValue;

        QPen getSymbolPen(JKQTPEnhancedPainter& painter) const;
        QBrush getBrush(JKQTPEnhancedPainter &painter) const;

};




#endif // jkqtpgraphssinglecolumnsymbols_H_INCLUDED
