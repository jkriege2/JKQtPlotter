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
#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsboxplotstylingmixins.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

#ifndef jkqtpgraphsboxplot_H
#define jkqtpgraphsboxplot_H





/*! \brief This implements vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>, optionally also a notched boxplot
    \ingroup jkqtplotter_statgraphs

    The x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplotvertical.png


    The different features of a boxplot are:

    \image html plot_boxplotverticalelement.png


    The example \ref JKQTPlotterBoxplotStyling discusses different options to style boxplots:

    \image html test_styledboxplot.png

    This class also implements hitTest() in a way that displays all data of the boxplot in the tooltips:

    \image html tooltip_boxplot.png

    \note There are additional classes to draw a single boxplot element: JKQTPBoxplotHorizontalElement and JKQTPBoxplotVerticalElement.
          In these you can set the data values, as they are NOT drawn from a data column. This can be useful, if you e.g. want to
          draw the statistical properties of a distribution.

    \section BoxplotOutliers Outliers

    If you also want to display outliers, as shown here with circles:

    \image html boxplot_outliers.png

    You need to add them as (x,y)-pairs to the datastore and add a separate JKQTPXYLineGraph that shows these. See \ref JKQTPlotterBoxplotsGraphs for details. Here is an example code-snippet:

    \code
        // 4. create a graph of vertical boxplots:
        JKQTPBoxplotVerticalGraph* graph=new JKQTPBoxplotVerticalGraph(&plot);
        graph->setPositionColumn(columnPOS);
        graph->setMinColumn(columnMIN);
        graph->setPercentile25Column(columnQ25);
        graph->setMedianColumn(columnMEDIAN);
        graph->setMeanColumn(columnMEAN);
        graph->setPercentile75Column(columnQ75);
        graph->setMaxColumn(columnMAX);
        graph->setTitle("vertical Boxplots");

        // 5. outliers need to be drawn separately
        JKQTPXYLineGraph* graphOutliers=new JKQTPXYLineGraph(&plot);
        graphOutliers->setXColumn(columnOUTLIERSX);
        graphOutliers->setYColumn(columnOUTLIERSY);
        graphOutliers->setTitle("outliers");
        // make the color a darker shade of the color of graph
        graphOutliers->setColor(graph->getColor().darker());
        graphOutliers->setFillColor(QColor("white"));
        // draw outliers as small circles, without lines
        graphOutliers->setSymbolType(JKQTPCircle);
        graphOutliers->setDrawLine(false);
        graphOutliers->setSymbolSize(7);
    \endcode

    \see \ref JKQTPlotterBoxplotsGraphs

 */
class JKQTP_LIB_EXPORT JKQTPBoxplotVerticalGraph: public JKQTPGraph, public JKQTPGraphBoxplotStyleMixin {
        Q_OBJECT
    public:

        /** \brief Sort order in a  JKQTPBoxplotVerticalGraph (or one of its child classes) */
        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };



        /** \brief class constructor */
        JKQTPBoxplotVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBoxplotVerticalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
        /*! \brief set the color of the graph (colors all elements, based on the given color \a c )*/
        virtual void setColor(QColor c);


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

        /*! \copydoc sortData */ 
        void setDataSortOrder(DataSortOrder  __value);
        /*! \copydoc sortData */ 
        DataSortOrder getDataSortOrder() const;
        /*! \copydoc sortData */
        void setDataSortOrder(int __value);
        /*! \copydoc posColumn */ 
        void setPositionColumn(int __value);
        /*! \copydoc posColumn */ 
        int getPositionColumn() const;
        /*! \copydoc posColumn */ 
        void setPositionColumn (size_t __value);
        /*! \copydoc medianColumn */ 
        void setMedianColumn(int __value);
        /*! \copydoc medianColumn */ 
        int getMedianColumn() const;
        /*! \copydoc medianColumn */ 
        void setMedianColumn (size_t __value);
        /*! \copydoc meanColumn */ 
        void setMeanColumn(int __value);
        /*! \copydoc meanColumn */ 
        int getMeanColumn() const;
        /*! \copydoc meanColumn */ 
        void setMeanColumn (size_t __value);
        /*! \copydoc minColumn */ 
        void setMinColumn(int __value);
        /*! \copydoc minColumn */ 
        int getMinColumn() const;
        /*! \copydoc minColumn */ 
        void setMinColumn( size_t __value);
        /*! \copydoc maxColumn */ 
        void setMaxColumn(int __value);
        /*! \copydoc maxColumn */ 
        int getMaxColumn() const;
        /*! \copydoc maxColumn */ 
        void setMaxColumn (size_t __value);
        /*! \copydoc percentile25Column */ 
        void setPercentile25Column(int __value);
        /*! \copydoc percentile25Column */ 
        int getPercentile25Column() const;
        /*! \copydoc percentile25Column */ 
        void setPercentile25Column (size_t __value);
        /*! \copydoc percentile75Column */ 
        void setPercentile75Column(int __value);
        /*! \copydoc percentile75Column */ 
        int getPercentile75Column() const;
        /*! \copydoc percentile75Column */ 
        void setPercentile75Column (size_t __value);
        /*! \copydoc medianConfidenceColumn */
        int getMedianConfidenceColumn() const;
        /*! \copydoc medianConfidenceColumn */
        void setMedianConfidenceColumn (size_t __value);


        /*! \copydoc boxWidthRelative */
        void setBoxWidthRelative(double __value);
        /*! \copydoc boxWidthRelative */
        double getBoxWidthRelative() const;


        /*! \copydoc useRelativeBoxWidth */
        void setUseRelativeBoxWidth(bool __value);
        /*! \copydoc useRelativeBoxWidth */
        bool getUseRelativeBoxWidth() const;
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

        virtual void intSortData() ;

        inline  int getDataIndex(int i) {
            if (sortData==Unsorted) return i;
            return sortedIndices.value(i,i);
        }

};


/*! \brief This implements horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>, optionally also a notched boxplot
    \ingroup jkqtplotter_statgraphs

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplothoricontal.png

    \note See the documentation of JKQTPBoxplotVerticalGraph for details on the properties of this class!

    \see JKQTPBoxplotVerticalGraph \ref JKQTPlotterBoxplotsGraphs

 */
class JKQTP_LIB_EXPORT JKQTPBoxplotHorizontalGraph: public JKQTPBoxplotVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBoxplotHorizontalGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPBoxplotHorizontalGraph(JKQTPlotter* parent);

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








/*! \brief This implements a single vertical <a href="http://en.wikipedia.org/wiki/Box_plot">(notched) boxplot</a> as a "geometric element",
           where the data is directly given to the object and not stored in a column, as in JKQTPBoxplotVerticalGraph
    \ingroup jkqtplotter_statgraphs
    \ingroup jkqtplotter_geoplots



    the x position is given in posColumn. All other data are given in the median, min, max,
    percentile25 and percentile75.

    The different features of a boxplot are:

    \image html plot_boxplotverticalelement.png


    The example \ref JKQTPlotterBoxplotStyling discusses different options to style boxplots:

    \image html test_styledboxplot.png

 */
class JKQTP_LIB_EXPORT JKQTPBoxplotVerticalElement: public JKQTPPlotObject, public JKQTPGraphBoxplotStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBoxplotVerticalElement(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBoxplotVerticalElement(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
        /*! \brief set the color of the graph (colors all elements, based on the given color \a c )*/
        virtual void setColor(QColor c);


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


        /*! \copydoc pos */ 
        void setPos(double __value);
        /*! \copydoc pos */ 
        double getPos() const;
        /*! \copydoc median */ 
        void setMedian(double __value);
        /*! \copydoc median */ 
        double getMedian() const;
        /*! \copydoc mean */ 
        void setMean(double __value);
        /*! \copydoc mean */ 
        double getMean() const;
        /*! \copydoc min */ 
        void setMin(double __value);
        /*! \copydoc min */ 
        double getMin() const;
        /*! \copydoc max */ 
        void setMax(double __value);
        /*! \copydoc max */ 
        double getMax() const;
        /*! \copydoc percentile25 */ 
        void setPercentile25(double __value);
        /*! \copydoc percentile25 */ 
        double getPercentile25() const;
        /*! \copydoc percentile75 */ 
        void setPercentile75(double __value);
        /*! \copydoc percentile75 */ 
        double getPercentile75() const;

        /*! \copydoc drawMean */ 
        void setDrawMean(bool __value);
        /*! \copydoc drawMean */ 
        bool getDrawMean() const;
        /*! \copydoc drawMedian */ 
        void setDrawMedian(bool __value);
        /*! \copydoc drawMedian */ 
        bool getDrawMedian() const;
        /*! \copydoc drawMinMax */ 
        void setDrawMinMax(bool __value);
        /*! \copydoc drawMinMax */ 
        bool getDrawMinMax() const;
        /*! \copydoc drawNotch */
        void setDrawNotch(bool __value);
        /*! \copydoc drawNotch */
        bool getDrawNotch() const;

        /*! \copydoc medianConfidenceIntervalWidth */
        double getMedianConfidenceIntervalWidth() const;
        /*! \copydoc medianConfidenceIntervalWidth */
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


/*! \brief This implements a horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">(notched) boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPBoxplotVerticalGraph
    \ingroup jkqtplotter_statgraphs
    \ingroup jkqtplotter_geoplots

    the x position is given in pos. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplothorizontalelement.png

    \note See JKQTPBoxplotVerticalElement for a detailed documentation

    \see JKQTPBoxplotVerticalElement
 */
class JKQTP_LIB_EXPORT JKQTPBoxplotHorizontalElement: public JKQTPBoxplotVerticalElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPBoxplotHorizontalElement(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBoxplotHorizontalElement(JKQTPlotter* parent);

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



#endif // jkqtpgraphsboxplot_H
