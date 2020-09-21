/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/graphs/jkqtpboxplotstylingmixins.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

#ifndef jkqtpgraphsboxplotbase_H
#define jkqtpgraphsboxplotbase_H





/** \brief Base class for graphs representing a series of <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a>, elements
 *  \ingroup jkqtplotter_statgraphs
 *
 *  \image html plot_boxplotvertical.png
 *
 *
 *  The different features of a boxplot are:
 *
 *  \image html plot_boxplotverticalelement.png
 *
 *  \see JKQTPBoxplotVerticalGraph, JKQTPBoxplotHorizontalGraph, JKQTPBoxplotElementBase
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBoxplotGraphBase: public JKQTPGraph, public JKQTPGraphBoxplotStyleMixin {
        Q_OBJECT
    public:

        /** \brief Sort order in a  JKQTPBoxplotVerticalGraph (or one of its child classes) */
        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };



        /** \brief class constructor */
        JKQTPBoxplotGraphBase(JKQTBasePlotter* parent=nullptr);

        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

        /** \copydoc sortData */
        DataSortOrder getDataSortOrder() const;
        /** \copydoc posColumn */
        int getPositionColumn() const;
        /** \copydoc medianColumn */
        int getMedianColumn() const;
        /** \copydoc meanColumn */
        int getMeanColumn() const;
        /** \copydoc minColumn */
        int getMinColumn() const;
        /** \copydoc maxColumn */
        int getMaxColumn() const;
        /** \copydoc percentile25Column */
        int getPercentile25Column() const;
        /** \copydoc percentile75Column */
        int getPercentile75Column() const;
        /** \copydoc medianConfidenceColumn */
        int getMedianConfidenceColumn() const;


        /** \copydoc boxWidthRelative */
        double getBoxWidthRelative() const;


        /** \copydoc useRelativeBoxWidth */
        bool getUseRelativeBoxWidth() const;
    public slots:
        /** \brief set the color of the graph (colors all elements, based on the given color \a c ) */
        virtual void setColor(QColor c);
        /** \copydoc sortData */
        void setDataSortOrder(DataSortOrder  __value);
        /** \copydoc sortData */
        void setDataSortOrder(int __value);
        /** \copydoc posColumn */
        void setPositionColumn(int __value);
        /** \copydoc posColumn */
        void setPositionColumn (size_t __value);
        /** \copydoc medianColumn */
        void setMedianColumn(int __value);
        /** \copydoc medianColumn */
        void setMedianColumn (size_t __value);
        /** \copydoc meanColumn */
        void setMeanColumn(int __value);
        /** \copydoc meanColumn */
        void setMeanColumn (size_t __value);
        /** \copydoc minColumn */
        void setMinColumn(int __value);
        /** \copydoc minColumn */
        void setMinColumn( size_t __value);
        /** \copydoc maxColumn */
        void setMaxColumn(int __value);
        /** \copydoc maxColumn */
        void setMaxColumn (size_t __value);
        /** \copydoc percentile25Column */
        void setPercentile25Column(int __value);
        /** \copydoc percentile25Column */
        void setPercentile25Column (size_t __value);
        /** \copydoc percentile75Column */
        void setPercentile75Column(int __value);
        /** \copydoc percentile75Column */
        void setPercentile75Column (size_t __value);
        /** \copydoc medianConfidenceColumn */
        void setMedianConfidenceColumn (size_t __value);
        /** \copydoc boxWidthRelative */
        void setBoxWidthRelative(double __value);
        /** \copydoc useRelativeBoxWidth */
        void setUseRelativeBoxWidth(bool __value);
    protected:
        /** \brief width of box in percent of distance between the current two posColumn values
         *         if we only plot one box&whiskers then JKQTPGraphBoxplotStyleMixin::boxWidthAbsolute in pt is used */
        double boxWidthRelative;
        /** \brief if set \c true, boxplot widths are calculated automatically, based on boxWidthRelative,
         *         otherwise JKQTPGraphBoxplotStyleMixin::boxWidthAbsolute is used. */
        bool useRelativeBoxWidth;

        /** \brief the column that contains the x-component of the datapoints */
        int posColumn;
        /** \brief the column that contains the median-component of the datapoints */
        int medianColumn;
        /** \brief the column that contains the confidence interval width of the median (e.g. 1.57*IQR/sqrt(n) ). This is used to draw a notch in the plot (if set) */
        int medianConfidenceColumn;
        /** \brief the column that contains the median-component of the datapoints. \note This column is strictly optional. */
        int meanColumn;
        /** \brief the column that contains the minimum-component of the datapoints */
        int minColumn;
        /** \brief the column that contains the maximum-component of the datapoints */
        int maxColumn;
        /** \brief the column that contains the 25% percentile-component of the datapoints */
        int percentile25Column;
        /** \brief the column that contains the 75% percentile-component of the datapoints */
        int percentile75Column;
        /** \brief if \c !=Unsorted, the data is sorted before plotting */
        DataSortOrder sortData;
        /** \brief this array contains the order of indices, in which to access the data in the data columns */
        QVector<int> sortedIndices;

        /** \brief generates a map from unsorted to sorted data \see getDataIndex() */
        virtual void intSortData() ;
        /** \brief retrieves the index of the i-th element when data is sorted (after calling intSortData(), which generates a map from unsorted to sorted data */
        inline int getDataIndex(int i) const {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }

};




/** \brief Tbaseclass for a single <a href="http://en.wikipedia.org/wiki/Box_plot">(notched) boxplot</a> as a "geometric element",
 *         where the data is directly given to the object and not stored in a column, as in JKQTPBoxplotGraphBase
 *  \ingroup jkqtplotter_statgraphs
 *  \ingroup jkqtplotter_diverse
 *
 *  The different features of a boxplot are:
 *
 *  \image html plot_boxplotverticalelement.png
 *
 *  \see JKQTPBoxplotElementBase, JKQTPBoxplotHorizontalElement, JKQTPBoxplotGraphBase
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBoxplotElementBase: public JKQTPPlotElement, public JKQTPGraphBoxplotStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBoxplotElementBase(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBoxplotElementBase(JKQTPlotter* parent);

        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /** \copydoc pos */
        double getPos() const;
        /** \copydoc median */
        double getMedian() const;
        /** \copydoc mean */
        double getMean() const;
        /** \copydoc min */
        double getMin() const;
        /** \copydoc max */
        double getMax() const;
        /** \copydoc percentile25 */
        double getPercentile25() const;
        /** \copydoc percentile75 */
        double getPercentile75() const;

        /** \copydoc drawMean */
        void setDrawMean(bool __value);
        /** \copydoc drawMean */
        bool getDrawMean() const;
        /** \copydoc drawMedian */
        bool getDrawMedian() const;
        /** \copydoc drawMinMax */
        bool getDrawMinMax() const;
        /** \copydoc drawNotch */
        bool getDrawNotch() const;

        /** \copydoc medianConfidenceIntervalWidth */
        double getMedianConfidenceIntervalWidth() const;
    public slots:
        /** \brief set the color of the graph (colors all elements, based on the given color \a c ) */
        virtual void setColor(QColor c);

        /** \copydoc pos */
        void setPos(double __value);
        /** \copydoc median */
        void setMedian(double __value);
        /** \copydoc min */
        void setMin(double __value);
        /** \copydoc mean */
        void setMean(double __value);
        /** \copydoc max */
        void setMax(double __value);
        /** \copydoc percentile25 */
        void setPercentile25(double __value);
        /** \copydoc percentile75 */
        void setPercentile75(double __value);
        /** \copydoc drawMedian */
        void setDrawMedian(bool __value);
        /** \copydoc drawMinMax */
        void setDrawMinMax(bool __value);
        /** \copydoc drawNotch */
        void setDrawNotch(bool __value);
        /** \copydoc medianConfidenceIntervalWidth */
        void setMedianConfidenceIntervalWidth(double __value);
    protected:

        /** \brief the position of the boxplot on the "other" axis */
        double pos;
        /** \brief the median value to be used for the boxplot */
        double median;
        /** \brief the width of the confidence interval around the median */
        double medianConfidenceIntervalWidth;
        /** \brief indicates whether to draw a notch with width medianConfidenceIntervalWidth */
        bool drawNotch;
        /** \brief the mean value to be used for the boxplot */
        double mean;
        /** \brief indicates whether to draw the mean */
        bool drawMean;
        /** \brief indicates whether to draw the median */
        bool drawMedian;
        /** \brief indicates whether to draw the percentiles */
        bool drawMinMax;
        /** \brief the minimum value to be used for the boxplot */
        double min;
        /** \brief the maximum value to be used for the boxplot */
        double max;
        /** \brief the 25% percentile value to be used for the boxplot */
        double percentile25;
        /** \brief the 75% percentile value to be used for the boxplot */
        double percentile75;
};




#endif // jkqtpgraphsboxplotbase_H
