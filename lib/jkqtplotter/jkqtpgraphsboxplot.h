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
#include "jkqtplottertools/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"

#ifndef jkqtpgraphsboxplot_H
#define jkqtpgraphsboxplot_H




/*! \brief This implements vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>
    \ingroup jkqtplotter_statgraphs

    The x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplotvertical.png

    The different features of a boxplot are:

    \image html boxplots.png

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
        graph->setPosColumn(columnPOS);
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
class JKQTP_LIB_EXPORT JKQTPBoxplotVerticalGraph: public JKQTPGraph {
        Q_OBJECT
    public:

        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };



        /** \brief class constructor */
        JKQTPBoxplotVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPBoxplotVerticalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;


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

        /*! \copydoc sortData
            \see see sortData for details */ 
        inline virtual void setDataSortOrder(const DataSortOrder & __value)  
        {
            this->sortData = __value;
        } 
        /*! \copydoc sortData
            \see see sortData for details */ 
        inline virtual DataSortOrder getDataSortOrder() const  
        {
            return this->sortData; 
        }
        /*! \brief sets the property sortData ( \copybrief sortData ) to the specified \a __value. \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> \see sortData for more information */
        void setDataSortOrder(int __value);
        /*! \copydoc posColumn
            \see see posColumn for details */ 
        inline virtual void setPosColumn(int __value)
        {
            this->posColumn = __value;
        } 
        /*! \copydoc posColumn
            \see see posColumn for details */ 
        inline virtual int getPosColumn() const  
        {
            return this->posColumn; 
        }
        /*! \brief sets the property posColumn ( \copybrief posColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter posColumn is:  <BLOCKQUOTE>\copydoc posColumn </BLOCKQUOTE> 
            \see posColumn for more information */ 
        inline virtual void setPosColumn (size_t __value) { this->posColumn = static_cast<int>(__value); }
        /*! \copydoc medianColumn
            \see see medianColumn for details */ 
        inline virtual void setMedianColumn(int __value)
        {
            this->medianColumn = __value;
        } 
        /*! \copydoc medianColumn
            \see see medianColumn for details */ 
        inline virtual int getMedianColumn() const  
        {
            return this->medianColumn; 
        }
        /*! \brief sets the property medianColumn ( \copybrief medianColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter medianColumn is:  <BLOCKQUOTE>\copydoc medianColumn </BLOCKQUOTE> 
            \see medianColumn for more information */ 
        inline virtual void setMedianColumn (size_t __value) { this->medianColumn = static_cast<int>(__value); }
        /*! \copydoc meanColumn
            \see see meanColumn for details */ 
        inline virtual void setMeanColumn(int __value)
        {
            this->meanColumn = __value;
        } 
        /*! \copydoc meanColumn
            \see see meanColumn for details */ 
        inline virtual int getMeanColumn() const  
        {
            return this->meanColumn; 
        }
        /*! \brief sets the property meanColumn ( \copybrief meanColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter meanColumn is:  <BLOCKQUOTE>\copydoc meanColumn </BLOCKQUOTE> 
            \see meanColumn for more information */ 
        inline virtual void setMeanColumn (size_t __value) { this->meanColumn = static_cast<int>(__value); }
        /*! \copydoc minColumn
            \see see minColumn for details */ 
        inline virtual void setMinColumn(int __value)
        {
            this->minColumn = __value;
        } 
        /*! \copydoc minColumn
            \see see minColumn for details */ 
        inline virtual int getMinColumn() const  
        {
            return this->minColumn; 
        }
        /*! \brief sets the property minColumn ( \copybrief minColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter minColumn is:  <BLOCKQUOTE>\copydoc minColumn </BLOCKQUOTE> 
            \see minColumn for more information */ 
        inline virtual void setMinColumn( size_t __value) { this->minColumn = static_cast<int>(__value); }
        /*! \copydoc maxColumn
            \see see maxColumn for details */ 
        inline virtual void setMaxColumn(int __value)
        {
            this->maxColumn = __value;
        } 
        /*! \copydoc maxColumn
            \see see maxColumn for details */ 
        inline virtual int getMaxColumn() const  
        {
            return this->maxColumn; 
        }
        /*! \brief sets the property maxColumn ( \copybrief maxColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter maxColumn is:  <BLOCKQUOTE>\copydoc maxColumn </BLOCKQUOTE> 
            \see maxColumn for more information */ 
        inline virtual void setMaxColumn (size_t __value) { this->maxColumn = static_cast<int>(__value); }
        /*! \copydoc percentile25Column
            \see see percentile25Column for details */ 
        inline virtual void setPercentile25Column(int __value)
        {
            this->percentile25Column = __value;
        } 
        /*! \copydoc percentile25Column
            \see see percentile25Column for details */ 
        inline virtual int getPercentile25Column() const  
        {
            return this->percentile25Column; 
        }
        /*! \brief sets the property percentile25Column ( \copybrief percentile25Column ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter percentile25Column is:  <BLOCKQUOTE>\copydoc percentile25Column </BLOCKQUOTE> 
            \see percentile25Column for more information */ 
        inline virtual void setPercentile25Column (size_t __value) { this->percentile25Column = static_cast<int>(__value); }
        /*! \copydoc percentile75Column
            \see see percentile75Column for details */ 
        inline virtual void setPercentile75Column(int __value)
        {
            this->percentile75Column = __value;
        } 
        /*! \copydoc percentile75Column
            \see see percentile75Column for details */ 
        inline virtual int getPercentile75Column() const  
        {
            return this->percentile75Column; 
        }
        /*! \brief sets the property percentile75Column ( \copybrief percentile75Column ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter percentile75Column is:  <BLOCKQUOTE>\copydoc percentile75Column </BLOCKQUOTE> 
            \see percentile75Column for more information */ 
        inline virtual void setPercentile75Column (size_t __value) { this->percentile75Column = static_cast<int>(__value); }
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
        /*! \copydoc whiskerStyle
            \see see whiskerStyle for details */ 
        inline virtual void setWhiskerStyle(const Qt::PenStyle & __value)  
        {
            this->whiskerStyle = __value;
        } 
        /*! \copydoc whiskerStyle
            \see see whiskerStyle for details */ 
        inline virtual Qt::PenStyle getWhiskerStyle() const  
        {
            return this->whiskerStyle; 
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
        /*! \copydoc boxWidth
            \see see boxWidth for details */ 
        inline virtual void setBoxWidth(double __value)
        {
            this->boxWidth = __value;
        } 
        /*! \copydoc boxWidth
            \see see boxWidth for details */ 
        inline virtual double getBoxWidth() const  
        {
            return this->boxWidth; 
        }

        /*! \copydoc meanSymbol
            \see see meanSymbol for details */ 
        inline virtual void setMeanSymbol(JKQTPGraphSymbols __value)  
        {
            this->meanSymbol = __value;
        } 
        /*! \copydoc meanSymbol
            \see see meanSymbol for details */ 
        inline virtual JKQTPGraphSymbols getMeanSymbol() const  
        {
            return this->meanSymbol; 
        }
        /*! \copydoc meanSymbolWidth
            \see see meanSymbolWidth for details */ 
        inline virtual void setMeanSymbolWidth(double __value)
        {
            this->meanSymbolWidth = __value;
        } 
        /*! \copydoc meanSymbolWidth
            \see see meanSymbolWidth for details */ 
        inline virtual double getMeanSymbolWidth() const  
        {
            return this->meanSymbolWidth; 
        }
        /*! \copydoc meanSymbolSize
            \see see meanSymbolSize for details */ 
        inline virtual void setMeanSymbolSize(double __value)
        {
            this->meanSymbolSize = __value;
        } 
        /*! \copydoc meanSymbolSize
            \see see meanSymbolSize for details */ 
        inline virtual double getMeanSymbolSize() const  
        {
            return this->meanSymbolSize; 
        }

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief the column that contains the x-component of the datapoints */
        int posColumn;
        /** \brief the column that contains the median-component of the datapoints */
        int medianColumn;
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
        /** \brief color of the lines */
        QColor color;
        /** \brief color of the box fill */
        QColor fillColor;
        /** \brief fill style for the box */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the whisker lines */
        Qt::PenStyle whiskerStyle;
        /** \brief width (pt) of the graph, given in pt */
        double lineWidth;
        /** \brief width of box in percent of distance between the current two posColumn values
         *         if we only plot one box&whiskers then this is the width in plot coordinates */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPGraphSymbols meanSymbol;
        /** \brief size (diameter in pt) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pt) of the lines used to plot the symbol for the mean */
        double meanSymbolWidth;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter &painter) const;
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


/*! \brief This implements horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplots</a>
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








/*! \brief This implements a single vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> as a "geometric element",
           where the data is directly given to the object and not stored in a column, as in JKQTPBoxplotVerticalGraph
    \ingroup jkqtplotter_statgraphs
    \ingroup jkqtplotter_geoplots


    \image html plot_boxplotverticalelement.png

    the x position is given in posColumn. All other data are given in the median, min, max,
    percentile25 and percentile75.

    The different features of a boxplot are:

    \image html boxplots.png
 */
class JKQTP_LIB_EXPORT JKQTPBoxplotVerticalElement: public JKQTPPlotObject {
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


        /*! \copydoc pos
            \see see pos for details */ 
        inline virtual void setPos(double __value)
        {
            this->pos = __value;
        } 
        /*! \copydoc pos
            \see see pos for details */ 
        inline virtual double getPos() const  
        {
            return this->pos; 
        }
        /*! \copydoc median
            \see see median for details */ 
        inline virtual void setMedian(double __value)
        {
            if (this->median != __value) { 
                this->median = __value; 
                drawMedian=true; 
            } 
        } 
        /*! \copydoc median
            \see see median for details */ 
        inline virtual double getMedian() const  
        {
            return this->median; 
        }
        /*! \copydoc mean
            \see see mean for details */ 
        inline virtual void setMean(double __value)
        {
            if (this->mean != __value) { 
                this->mean = __value; 
                drawMean=true; 
            } 
        } 
        /*! \copydoc mean
            \see see mean for details */ 
        inline virtual double getMean() const  
        {
            return this->mean; 
        }
        /*! \copydoc min
            \see see min for details */ 
        inline virtual void setMin(double __value)
        {
            if (this->min != __value) { 
                this->min = __value; 
                drawMinMax=true; 
            } 
        } 
        /*! \copydoc min
            \see see min for details */ 
        inline virtual double getMin() const  
        {
            return this->min; 
        }
        /*! \copydoc max
            \see see max for details */ 
        inline virtual void setMax(double __value)
        {
            if (this->max != __value) { 
                this->max = __value; 
                drawMinMax=true; 
            } 
        } 
        /*! \copydoc max
            \see see max for details */ 
        inline virtual double getMax() const  
        {
            return this->max; 
        }
        /*! \copydoc percentile25
            \see see percentile25 for details */ 
        inline virtual void setPercentile25(double __value)
        {
            this->percentile25 = __value;
        } 
        /*! \copydoc percentile25
            \see see percentile25 for details */ 
        inline virtual double getPercentile25() const  
        {
            return this->percentile25; 
        }
        /*! \copydoc percentile75
            \see see percentile75 for details */ 
        inline virtual void setPercentile75(double __value)
        {
            this->percentile75 = __value;
        } 
        /*! \copydoc percentile75
            \see see percentile75 for details */ 
        inline virtual double getPercentile75() const  
        {
            return this->percentile75; 
        }
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
        /*! \copydoc whiskerStyle
            \see see whiskerStyle for details */ 
        inline virtual void setWhiskerStyle(const Qt::PenStyle & __value)  
        {
            this->whiskerStyle = __value;
        } 
        /*! \copydoc whiskerStyle
            \see see whiskerStyle for details */ 
        inline virtual Qt::PenStyle getWhiskerStyle() const  
        {
            return this->whiskerStyle; 
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
        /*! \copydoc boxWidth
            \see see boxWidth for details */ 
        inline virtual void setBoxWidth(double __value)
        {
            this->boxWidth = __value;
        } 
        /*! \copydoc boxWidth
            \see see boxWidth for details */ 
        inline virtual double getBoxWidth() const  
        {
            return this->boxWidth; 
        }

        /*! \copydoc meanSymbol
            \see see meanSymbol for details */ 
        inline virtual void setMeanSymbol(JKQTPGraphSymbols __value)  
        {
            this->meanSymbol = __value;
        } 
        /*! \copydoc meanSymbol
            \see see meanSymbol for details */ 
        inline virtual JKQTPGraphSymbols getMeanSymbol() const  
        {
            return this->meanSymbol; 
        }
        /*! \copydoc meanSymbolWidth
            \see see meanSymbolWidth for details */ 
        inline virtual void setMeanSymbolWidth(double __value)
        {
            this->meanSymbolWidth = __value;
        } 
        /*! \copydoc meanSymbolWidth
            \see see meanSymbolWidth for details */ 
        inline virtual double getMeanSymbolWidth() const  
        {
            return this->meanSymbolWidth; 
        }
        /*! \copydoc meanSymbolSize
            \see see meanSymbolSize for details */ 
        inline virtual void setMeanSymbolSize(double __value)
        {
            this->meanSymbolSize = __value;
        } 
        /*! \copydoc meanSymbolSize
            \see see meanSymbolSize for details */ 
        inline virtual double getMeanSymbolSize() const  
        {
            return this->meanSymbolSize; 
        }
        /*! \copydoc drawMean
            \see see drawMean for details */ 
        inline virtual void setDrawMean(bool __value)
        {
            this->drawMean = __value;
        } 
        /*! \copydoc drawMean
            \see see drawMean for details */ 
        inline virtual bool getDrawMean() const  
        {
            return this->drawMean; 
        }
        /*! \copydoc drawMedian
            \see see drawMedian for details */ 
        inline virtual void setDrawMedian(bool __value)
        {
            this->drawMedian = __value;
        } 
        /*! \copydoc drawMedian
            \see see drawMedian for details */ 
        inline virtual bool getDrawMedian() const  
        {
            return this->drawMedian; 
        }
        /*! \copydoc drawMinMax
            \see see drawMinMax for details */ 
        inline virtual void setDrawMinMax(bool __value)
        {
            this->drawMinMax = __value;
        } 
        /*! \copydoc drawMinMax
            \see see drawMinMax for details */ 
        inline virtual bool getDrawMinMax() const  
        {
            return this->drawMinMax; 
        }

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief the column that contains the x-component of the datapoints */
        double pos;
        /** \brief the column that contains the median-component of the datapoints */
        double median;
        /** \brief the column that contains the median-component of the datapoints. \note This column is strictly optional. */
        double mean;
        /** \brief indicates whether to draw the mean */
        bool drawMean;
        /** \brief indicates whether to draw the median */
        bool drawMedian;
        /** \brief indicates whether to draw the percentiles */
        bool drawMinMax;
        /** \brief the column that contains the minimum-component of the datapoints */
        double min;
        /** \brief the column that contains the maximum-component of the datapoints */
        double max;
        /** \brief the column that contains the 25% percentile-component of the datapoints */
        double percentile25;
        /** \brief the column that contains the 75% percentile-component of the datapoints */
        double percentile75;
        /** \brief color of the lines */
        QColor color;
        /** \brief color of the box fill */
        QColor fillColor;
        /** \brief fill style for the box */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the whisker lines */
        Qt::PenStyle whiskerStyle;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width of box, given in pt */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPGraphSymbols meanSymbol;
        /** \brief size (diameter in pt) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pt) of the lines used to plot the symbol for the mean */
        double meanSymbolWidth;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;
};


/*! \brief This implements a horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> where the data is directly given to the
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
