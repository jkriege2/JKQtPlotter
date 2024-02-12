/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#ifndef jkqtpfinancial_H
#define jkqtpfinancial_H

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"


/** \brief This class draws an Open-High-Low-Close (OHLC) or candle-stick graph typically representing financial data
 *  \ingroup jkqtplotter_financialgraphs
 *
 *  This graph draws either <b>candlstick charts</b>:
 *    \image html financialgraphs_basics_candlestick.png
 *  or <b>Open-High-Low-Close (OHLC) charts</b> (depending on what you set in setGraphType() ).
 *    \image html financialgraphs_basics_ohlc.png
 *
 *  Here is an example graph showcasing both variants:
 *
 *  \image html JKQTPFinancialGraph.png
 *
 *  \see For details on the graph types see https://en.wikipedia.org/wiki/Candlestick_chart and https://en.wikipedia.org/wiki/Open-high-low-close_chart
 *  \see JKQTPXGraph
 *
 *
 *  \section JKQTPFinancialGraphStyling Styling the Financial Graphs
 *  You can control the type of graph and it's appearance with these functions (example images are in the documentation of the functions):
 *    - setGraphType()
 *    - setCandlestickTwoColor()
 *    - setCandlestickOneColor()
 *    - setOHLCTwoColor()
 *  .
 *
 *  For fine-tuning, you can access the line- and fill-styles for positiove (close>open) and negative (close<open)
 *  items separately via:
 *    - lineStylePositive() and fillStylePositive()
 *    - lineStyleNegative() and fillStyleNegative()
 *  .
 *
 *  Note that the width of the single elements for each x-value is automatically from the distance to the
 *  next neighbours. Here set parameter width (setWidth(), getWidth() ) is employed. It sets the relative
 *  amount of space that an element occupies in its part of the  graph. Typically the default width is 0.8 (or 80%).
 *
 *  \image html financialgraphs_width.png
 *
 *
 *  \section JKQTPFinancialGraphSideBySide Several Financial Graphs side-by-side
 *  In case you want to display several of these graphs in the same plot, you can make them appear side-by-side, even
 *  if they all cover the same x-values and would thus be draw atop each other. We apply the same principles that
 *  were used for barcharts in JKQTPBarGraphBase and add a shift-parameter (setShift() ) that shifts each element
 *  in the available space:
 *
 *  \image html financialgraphs_shiftwidth.png
 *
 *  For example for two financial graph elements per x-value one would set:
 *  \verbatim
 *        width=0.4
 *        shift=-0.5 / +0.5
 *  \endverbatim
 *
 *  Two functions are provided (JKQTPFinancialGraph::autoscaleBoxWidthAndShift(), JKQTPFinancialGraph::autoscaleBoxWidthAndShiftSeparatedGroups() )
 *  that can be applied to any JKQTPFinancialGraph in the plot and will calculate all JKQTPFinancialGraphs' shift and width parameter
 *  in such a way that the graphs appear tidied up into groups by x-value. With these functions you don't have to calculate
 *  the shift and width values by hand! A call always only affects the JKQTPFinancialGraph in the plot with the same orientation
 *  (vertical or horizontal).
 *
 *  \image html JKQTPFinancialGraphSidyBySide.png
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPFinancialGraph: public JKQTPXGraph {
        Q_OBJECT
    public:
        /** \brief types of financial graphs that can be drawn by this class */
        enum FinancialGraphType {
            CandleStick, //!< \brief candlestick graphs \image html JKQTPFinancialGraphCandleStick.png
            OHLC         //!< \brief OHLC (Open-High-Low-Close) graphs \image html JKQTPFinancialGraphOHLC.png
        };
        Q_ENUM(FinancialGraphType)

        /** \brief class constructor */
        JKQTPFinancialGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFinancialGraph(JKQTPlotter* parent);

        /** \copydoc JKQTPXGraph::drawKeyMarker() */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \copydoc JKQTPXGraph::getKeyLabelColor() */
        virtual QColor getKeyLabelColor() const override;
        /** \copydoc JKQTPXGraph::draw() */
        virtual void draw(JKQTPEnhancedPainter& painter) override;



        /** \copydoc shift */
        double getShift() const;
        /** \copydoc width */
        double getWidth() const;

        /** \copydoc m_fillStyleNegative */
        JKQTPGraphFillStyleMixin &fillStyleNegative();
        /** \copydoc m_fillStyleNegative */
        const JKQTPGraphFillStyleMixin& fillStyleNegative() const;
        /** \copydoc m_fillStylePositive */
        JKQTPGraphFillStyleMixin &fillStylePositive();
        /** \copydoc m_fillStylePositive */
        const JKQTPGraphFillStyleMixin& fillStylePositive() const;


        /** \copydoc m_lineStyleNegative */
        JKQTPGraphLineStyleMixin &lineStyleNegative();
        /** \copydoc m_lineStyleNegative */
        const JKQTPGraphLineStyleMixin& lineStyleNegative() const;
        /** \copydoc m_lineStylePositive */
        JKQTPGraphLineStyleMixin &lineStylePositive();
        /** \copydoc m_lineStylePositive */
        const JKQTPGraphLineStyleMixin& lineStylePositive() const;

        /** \copydoc JKQTPXGraph::getIndexRange() */
        virtual bool getIndexRange(int &imin, int &imax) const override;
        /** \copydoc JKQTPXGraph::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;
        /** \copydoc JKQTPXGraph::usesColumn() */
        virtual bool usesColumn(int column) const override;



        /** \copydoc openColumn */
        int getOpenColumn() const;

        /** \copydoc closeColumn */
        int getCloseColumn() const;

        /** \copydoc highColumn */
        int getHighColumn() const;

        /** \copydoc lowColumn */
        int getLowColumn() const;
        /** \copydoc graphType */
        FinancialGraphType getGraphType() const;

        Q_PROPERTY(int openColumn READ getOpenColumn WRITE setOpenColumn)
        Q_PROPERTY(int closeColumn READ getCloseColumn WRITE setCloseColumn)
        Q_PROPERTY(int highColumn READ getHighColumn WRITE setHighColumn)
        Q_PROPERTY(int lowColumn READ getLowColumn WRITE setLowColumn)
        Q_PROPERTY(FinancialGraphType graphType READ getGraphType WRITE setGraphType)

    public Q_SLOTS:

        /** \brief finds all financial charts of the same orientation and determines width and shift, so they stand side by side
         *
         *  \param maxWidth the maximum (relative) width, that all boxes will span of the (doubled) inter-box distance
         *  \param shrinkFactor factor, by which the boxes are shrinked compared to the available space
         *
         *  \note This function will scale ALL graphs of the parent plot, which were derived from JKQTPFinancialGraph, that match in orientation (currently all).
         */
        virtual void autoscaleBoxWidthAndShift(double maxWidth=0.8, double shrinkFactor=0.8);

        /** \brief equivalent to \c autoscaleBoxWidthAndShift(groupWidth,0.8);
         */
        void autoscaleBoxWidthAndShiftSeparatedGroups(double groupWidth=0.65);
        /** \copydoc shift */
        void setShift(double __value);
        /** \copydoc width */
        void setWidth(double __value);
        /** \copydoc graphType */
        void setGraphType(FinancialGraphType __value);

        /** \brief set outline and fill color at the same time, for both positive and negative boxes
         *
         *  \param cPositive color for positive items (close>open)
         *  \param cNegative color for negative items (close<open)
         */
        virtual void setColor(QColor cPositive, QColor cNegative);
        /** \brief set outline and fill color at the same time, for both positive and negative boxes
         *
         *  \param linePositive line-color for positive items (close>open)
         *  \param fillPositive fill-color for positive items (close>open)
         *  \param lineNegative line-color for negative items (close<open)
         *  \param fillNegative fill-color for negative items (close<open)
         */
        virtual void setColor(QColor linePositive, QColor fillPositive, QColor lineNegative, QColor fillNegative);
        /** \brief sets graphType = FinancialGraphType::CandleStick and sets the positive and negative color (for fill and line equal!)
         *
         *  \param cPositive color for positive items (close>open)
         *  \param cNegative color for negative items (close<open)
         *
         *  Here is an example output for <code>setCandlestickTwoColor(QColor("blue"), QColor("orange"))</code>:
         *
         *  \image html JKQTPFinancialGraphSetCandlestickTwoColor.png
         */
        void setCandlestickTwoColor(QColor cPositive, QColor cNegative);
        /** \brief sets graphType = FinancialGraphType::CandleStick and sets the positive and negative fill color and a single line-color
         *
         *  \param cPositive color for fill of positive items (close>open)
         *  \param cNegative color for fill of negative items (close<open)
         *
         *  Here is an example output for <code>setCandlestickTwoColor(QColor("green"), QColor("red"), QColor("black"))</code>:
         *
         *  \image html JKQTPFinancialGraphSetCandlestickTwoColor2.png
         */
        void setCandlestickTwoColor(QColor cPositive, QColor cNegative, QColor lineCOlor);
        /** \brief sets graphType = FinancialGraphType::CandleStick and sets one color \a cLine for positive and negative items, where positive items are unfilled and negative items are filled
         *
         *  \param cLine line-color for all items and also fill-color for positive items (close>open), negative items are unfilled
         *
         *  Here is an example output for <code>setCandlestickOneColor(QColor("black"))</code>:
         *
         *  \image html JKQTPFinancialGraphSetCandlestickOneColor.png
         */
        void setCandlestickOneColor(QColor cLine);
        /** \brief sets graphType = FinancialGraphType::OHLC and sets the positive and negative color (for fill and line equal!)
         *
         *  \param cPositive color for positive items (close>open)
         *  \param cNegative color for negative items (close<open)
         *
         *  Here is an example output for <code>setOHLCTwoColor(QColor("darkblue"), QColor("darkorange"))</code>:
         *
         *  \image html JKQTPFinancialGraphSetOHLCTwoColor.png
         */
        void setOHLCTwoColor(QColor cPositive, QColor cNegative);


        /** \copydoc openColumn */
        void setOpenColumn(int __value);
        /** \copydoc openColumn */
        void setOpenColumn (size_t __value);
        /** \copydoc closeColumn */
        void setCloseColumn(int __value);
        /** \copydoc closeColumn */
        void setCloseColumn (size_t __value);
        /** \copydoc highColumn */
        void setHighColumn(int __value);
        /** \copydoc highColumn */
        void setHighColumn (size_t __value);
        /** \copydoc lowColumn */
        void setLowColumn(int __value);
        /** \copydoc lowColumn */
        void setLowColumn (size_t __value);

    protected:
        /** \brief type of the financial graph (OHLC or candle-stick)
         *
         *  \see setGraphType(), getGraphType(), FinancialGraphType
         */
        FinancialGraphType graphType;
        /** \brief column for the "Open" value of the financial graph
         *
         *  \see setOpenColumn(), getOpenColumn()
         */
        int openColumn;
        /** \brief column for the "Close" value of the financial graph
         *
         *  \see setCloseColumn(), getCloseColumn()
         */
        int closeColumn;
        /** \brief column for the "High" value of the financial graph
         *
         *  \see setHighColumn(), getHighColumn()
         */
        int highColumn;
        /** \brief column for the "Low" value of the financial graph
         *
         *  \see setLowColumn(), getLowColumn()
         */
        int lowColumn;

        /** \brief the width of the boxes, relative to the distance between the current and the next x-value
         *
         * See the following graphic to understand this concept (same principle as described in the JKQTPBarGraphBase documentation):
         *     \image html financialgraphs_shiftwidth.png
         */
        double width;
        /** \brief the shift of the boxes, relative to the distance between the current and the next x-value
         *
         * See the following graphic to understand this concept (same principle as described in the JKQTPBarGraphBase documentation):
         *     \image html financialgraphs_shiftwidth.png
         */
        double shift;
        /** \brief fill style for positive items (close>open)
         *
         *  \see fillStylePositive()
         */
        JKQTPGraphFillStyleMixin m_fillStylePositive;
        /** \brief fill style for negative items (close<open)
         *
         *  \see fillStyleNegative()
         */
        JKQTPGraphFillStyleMixin m_fillStyleNegative;
        /** \brief fill style for positive items (close>open)
         *
         *  \see lineStylePositive()
         */
        JKQTPGraphLineStyleMixin m_lineStylePositive;
        /** \brief fill style for negative items (close<open)
         *
         *  \see lineStyleNegative()
         */
        JKQTPGraphLineStyleMixin m_lineStyleNegative;
        /** \brief this function is used by autoscaleBoxWidthAndShift() to determine whether a given graph shall be taken into account when autoscaling.
         *         Typically this returns \c true for all JKQTPFinancialGraph-derved objects with the same orientation (horizontal or vertical, i.e. currently all) */
        virtual bool considerForAutoscaling( JKQTPFinancialGraph* other) const;



};







#endif // jkqtpfinancial_H
