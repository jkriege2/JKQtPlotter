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

#ifndef jkqtpgraphsboxplot_H
#define jkqtpgraphsboxplot_H


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/graphs/jkqtpboxplotbase.h"
#include "jkqtplotter/graphs/jkqtpboxplotstylingmixins.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"





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

    \see \ref JKQTPlotterBoxplotsGraphs, jkqtpstatVAddBoxplots(),\ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat, \ref JKQTPlotterBasicJKQTPDatastoreStatistics, \ref JKQTPlotterBoxplotStyling

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBoxplotVerticalGraph: public JKQTPBoxplotGraphBase {
        Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPBoxplotVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBoxplotVerticalGraph(JKQTPlotter* parent);

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

    protected:

};


/*! \brief This implements horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>, optionally also a notched boxplot
    \ingroup jkqtplotter_statgraphs

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplothoricontal.png

    \note See the documentation of JKQTPBoxplotVerticalGraph for details on the properties of this class!

    \see JKQTPBoxplotVerticalGraph \ref JKQTPlotterBoxplotsGraphs, jkqtpstatHAddBoxplots(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat, \ref JKQTPlotterBasicJKQTPDatastoreStatistics, \ref JKQTPlotterBoxplotStyling

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBoxplotHorizontalGraph: public JKQTPBoxplotGraphBase {
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
    \ingroup jkqtplotter_diverse



    the x position is given in posColumn. All other data are given in the median, min, max,
    percentile25 and percentile75.

    The different features of a boxplot are:

    \image html plot_boxplotverticalelement.png


    The example \ref JKQTPlotterBoxplotStyling discusses different options to style boxplots:

    \image html test_styledboxplot.png

    \see jkqtpstatVAddBoxplot(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics, \ref JKQTPlotterBoxplotsGraphs, \ref JKQTPlotterBoxplotStyling, jkqtpstatAddVBoxplotAndOutliers()

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBoxplotVerticalElement: public JKQTPBoxplotElementBase {
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


    protected:


};


/*! \brief This implements a horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">(notched) boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPBoxplotVerticalGraph
    \ingroup jkqtplotter_statgraphs
    \ingroup jkqtplotter_diverse

    the x position is given in pos. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplothorizontalelement.png

    \note See JKQTPBoxplotVerticalElement for a detailed documentation

    \see JKQTPBoxplotVerticalElement, jkqtpstatHAddBoxplot(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics, \ref JKQTPlotterBoxplotsGraphs, \ref JKQTPlotterBoxplotStyling, jkqtpstatAddHBoxplotAndOutliers()

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBoxplotHorizontalElement: public JKQTPBoxplotElementBase {
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
