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


/** \file jkqtpgraphsboxplot.h
  * \ingroup jkqtplotter_statgraphs
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
        graphOutliers->setSymbol(JKQTPCircle);
        graphOutliers->setDrawLine(false);
        graphOutliers->setSymbolSize(7);
    \endcode

    \see \ref JKQTPlotterBoxplotsGraphs

 */
class LIB_EXPORT JKQTPBoxplotVerticalGraph: public JKQTPGraph {
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
        virtual QColor getKeyLabelColor();


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

        /*! \brief sets the property sortData ( \copybrief sortData ) to the specified \a __value. 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual void setSortData(const DataSortOrder & __value)  
        {
            this->sortData = __value;
        } 
        /*! \brief returns the property sortData ( \copybrief sortData ). 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual DataSortOrder getSortData() const  
        {
            return this->sortData; 
        }
        /*! \brief sets the property sortData ( \copybrief sortData ) to the specified \a __value. \details Description of the parameter sortData is: <BLOCKQUOTE>\copydoc sortData </BLOCKQUOTE> \see sortData for more information */
        void setSortData(int __value);
        /*! \brief sets the property posColumn ( \copybrief posColumn ) to the specified \a __value. 
            \details Description of the parameter posColumn is: <BLOCKQUOTE>\copydoc posColumn </BLOCKQUOTE> 
            \see posColumn for more information */ 
        inline virtual void setPosColumn(int __value)
        {
            this->posColumn = __value;
        } 
        /*! \brief returns the property posColumn ( \copybrief posColumn ). 
            \details Description of the parameter posColumn is: <BLOCKQUOTE>\copydoc posColumn </BLOCKQUOTE> 
            \see posColumn for more information */ 
        inline virtual int getPosColumn() const  
        {
            return this->posColumn; 
        }
        /*! \brief sets the property posColumn ( \copybrief posColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter posColumn is:  <BLOCKQUOTE>\copydoc posColumn </BLOCKQUOTE> 
        	\see posColumn for more information */ 
        inline virtual void setPosColumn (size_t __value) { this->posColumn = static_cast<int>(__value); }
        /*! \brief sets the property medianColumn ( \copybrief medianColumn ) to the specified \a __value. 
            \details Description of the parameter medianColumn is: <BLOCKQUOTE>\copydoc medianColumn </BLOCKQUOTE> 
            \see medianColumn for more information */ 
        inline virtual void setMedianColumn(int __value)
        {
            this->medianColumn = __value;
        } 
        /*! \brief returns the property medianColumn ( \copybrief medianColumn ). 
            \details Description of the parameter medianColumn is: <BLOCKQUOTE>\copydoc medianColumn </BLOCKQUOTE> 
            \see medianColumn for more information */ 
        inline virtual int getMedianColumn() const  
        {
            return this->medianColumn; 
        }
        /*! \brief sets the property medianColumn ( \copybrief medianColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter medianColumn is:  <BLOCKQUOTE>\copydoc medianColumn </BLOCKQUOTE> 
        	\see medianColumn for more information */ 
        inline virtual void setMedianColumn (size_t __value) { this->medianColumn = static_cast<int>(__value); }
        /*! \brief sets the property meanColumn ( \copybrief meanColumn ) to the specified \a __value. 
            \details Description of the parameter meanColumn is: <BLOCKQUOTE>\copydoc meanColumn </BLOCKQUOTE> 
            \see meanColumn for more information */ 
        inline virtual void setMeanColumn(int __value)
        {
            this->meanColumn = __value;
        } 
        /*! \brief returns the property meanColumn ( \copybrief meanColumn ). 
            \details Description of the parameter meanColumn is: <BLOCKQUOTE>\copydoc meanColumn </BLOCKQUOTE> 
            \see meanColumn for more information */ 
        inline virtual int getMeanColumn() const  
        {
            return this->meanColumn; 
        }
        /*! \brief sets the property meanColumn ( \copybrief meanColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter meanColumn is:  <BLOCKQUOTE>\copydoc meanColumn </BLOCKQUOTE> 
        	\see meanColumn for more information */ 
        inline virtual void setMeanColumn (size_t __value) { this->meanColumn = static_cast<int>(__value); }
        /*! \brief sets the property minColumn ( \copybrief minColumn ) to the specified \a __value. 
            \details Description of the parameter minColumn is: <BLOCKQUOTE>\copydoc minColumn </BLOCKQUOTE> 
            \see minColumn for more information */ 
        inline virtual void setMinColumn(int __value)
        {
            this->minColumn = __value;
        } 
        /*! \brief returns the property minColumn ( \copybrief minColumn ). 
            \details Description of the parameter minColumn is: <BLOCKQUOTE>\copydoc minColumn </BLOCKQUOTE> 
            \see minColumn for more information */ 
        inline virtual int getMinColumn() const  
        {
            return this->minColumn; 
        }
        /*! \brief sets the property minColumn ( \copybrief minColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter minColumn is:  <BLOCKQUOTE>\copydoc minColumn </BLOCKQUOTE> 
        	\see minColumn for more information */ 
        inline virtual void setMinColumn( size_t __value) { this->minColumn = static_cast<int>(__value); }
        /*! \brief sets the property maxColumn ( \copybrief maxColumn ) to the specified \a __value. 
            \details Description of the parameter maxColumn is: <BLOCKQUOTE>\copydoc maxColumn </BLOCKQUOTE> 
            \see maxColumn for more information */ 
        inline virtual void setMaxColumn(int __value)
        {
            this->maxColumn = __value;
        } 
        /*! \brief returns the property maxColumn ( \copybrief maxColumn ). 
            \details Description of the parameter maxColumn is: <BLOCKQUOTE>\copydoc maxColumn </BLOCKQUOTE> 
            \see maxColumn for more information */ 
        inline virtual int getMaxColumn() const  
        {
            return this->maxColumn; 
        }
        /*! \brief sets the property maxColumn ( \copybrief maxColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter maxColumn is:  <BLOCKQUOTE>\copydoc maxColumn </BLOCKQUOTE> 
        	\see maxColumn for more information */ 
        inline virtual void setMaxColumn (size_t __value) { this->maxColumn = static_cast<int>(__value); }
        /*! \brief sets the property percentile25Column ( \copybrief percentile25Column ) to the specified \a __value. 
            \details Description of the parameter percentile25Column is: <BLOCKQUOTE>\copydoc percentile25Column </BLOCKQUOTE> 
            \see percentile25Column for more information */ 
        inline virtual void setPercentile25Column(int __value)
        {
            this->percentile25Column = __value;
        } 
        /*! \brief returns the property percentile25Column ( \copybrief percentile25Column ). 
            \details Description of the parameter percentile25Column is: <BLOCKQUOTE>\copydoc percentile25Column </BLOCKQUOTE> 
            \see percentile25Column for more information */ 
        inline virtual int getPercentile25Column() const  
        {
            return this->percentile25Column; 
        }
        /*! \brief sets the property percentile25Column ( \copybrief percentile25Column ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter percentile25Column is:  <BLOCKQUOTE>\copydoc percentile25Column </BLOCKQUOTE> 
        	\see percentile25Column for more information */ 
        inline virtual void setPercentile25Column (size_t __value) { this->percentile25Column = static_cast<int>(__value); }
        /*! \brief sets the property percentile75Column ( \copybrief percentile75Column ) to the specified \a __value. 
            \details Description of the parameter percentile75Column is: <BLOCKQUOTE>\copydoc percentile75Column </BLOCKQUOTE> 
            \see percentile75Column for more information */ 
        inline virtual void setPercentile75Column(int __value)
        {
            this->percentile75Column = __value;
        } 
        /*! \brief returns the property percentile75Column ( \copybrief percentile75Column ). 
            \details Description of the parameter percentile75Column is: <BLOCKQUOTE>\copydoc percentile75Column </BLOCKQUOTE> 
            \see percentile75Column for more information */ 
        inline virtual int getPercentile75Column() const  
        {
            return this->percentile75Column; 
        }
        /*! \brief sets the property percentile75Column ( \copybrief percentile75Column ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter percentile75Column is:  <BLOCKQUOTE>\copydoc percentile75Column </BLOCKQUOTE> 
        	\see percentile75Column for more information */ 
        inline virtual void setPercentile75Column (size_t __value) { this->percentile75Column = static_cast<int>(__value); }
        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
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
        /*! \brief sets the property whiskerStyle ( \copybrief whiskerStyle ) to the specified \a __value. 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copydoc whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual void setWhiskerStyle(const Qt::PenStyle & __value)  
        {
            this->whiskerStyle = __value;
        } 
        /*! \brief returns the property whiskerStyle ( \copybrief whiskerStyle ). 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copydoc whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual Qt::PenStyle getWhiskerStyle() const  
        {
            return this->whiskerStyle; 
        }
        /*! \brief sets the property fillStyle ( \copybrief fillStyle ) to the specified \a __value. 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual void setFillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle ( \copybrief fillStyle ). 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle getFillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property lineWidth ( \copybrief lineWidth ) to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth ( \copybrief lineWidth ). 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property boxWidth ( \copybrief boxWidth ) to the specified \a __value. 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copydoc boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual void setBoxWidth(double __value)
        {
            this->boxWidth = __value;
        } 
        /*! \brief returns the property boxWidth ( \copybrief boxWidth ). 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copydoc boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual double getBoxWidth() const  
        {
            return this->boxWidth; 
        }

        /*! \brief sets the property meanSymbol ( \copybrief meanSymbol ) to the specified \a __value. 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copydoc meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual void setMeanSymbol(const JKQTPGraphSymbols & __value)  
        {
            this->meanSymbol = __value;
        } 
        /*! \brief returns the property meanSymbol ( \copybrief meanSymbol ). 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copydoc meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual JKQTPGraphSymbols getMeanSymbol() const  
        {
            return this->meanSymbol; 
        }
        /*! \brief sets the property meanSymbolWidth ( \copybrief meanSymbolWidth ) to the specified \a __value. 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copydoc meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual void setMeanSymbolWidth(double __value)
        {
            this->meanSymbolWidth = __value;
        } 
        /*! \brief returns the property meanSymbolWidth ( \copybrief meanSymbolWidth ). 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copydoc meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual double getMeanSymbolWidth() const  
        {
            return this->meanSymbolWidth; 
        }
        /*! \brief sets the property meanSymbolSize ( \copybrief meanSymbolSize ) to the specified \a __value. 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copydoc meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
        inline virtual void setMeanSymbolSize(double __value)
        {
            this->meanSymbolSize = __value;
        } 
        /*! \brief returns the property meanSymbolSize ( \copybrief meanSymbolSize ). 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copydoc meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
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
class LIB_EXPORT JKQTPBoxplotHorizontalGraph: public JKQTPBoxplotVerticalGraph {
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
class LIB_EXPORT JKQTPBoxplotVerticalElement: public JKQTPPlotObject {
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
        virtual QColor getKeyLabelColor() override;


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


        /*! \brief sets the property pos ( \copybrief pos ) to the specified \a __value. 
            \details Description of the parameter pos is: <BLOCKQUOTE>\copydoc pos </BLOCKQUOTE> 
            \see pos for more information */ 
        inline virtual void setPos(double __value)
        {
            this->pos = __value;
        } 
        /*! \brief returns the property pos ( \copybrief pos ). 
            \details Description of the parameter pos is: <BLOCKQUOTE>\copydoc pos </BLOCKQUOTE> 
            \see pos for more information */ 
        inline virtual double getPos() const  
        {
            return this->pos; 
        }
        /*! \brief sets the property median ( \copybrief median ) to the specified \a __value. 
            \details Description of the parameter median is: <BLOCKQUOTE>\copydoc median </BLOCKQUOTE> 
            \see median for more information */ 
        inline virtual void setMedian(double __value)
        {
            if (this->median != __value) { 
                this->median = __value; 
                drawMedian=true; 
            } 
        } 
        /*! \brief returns the property median ( \copybrief median ). 
            \details Description of the parameter median is: <BLOCKQUOTE>\copydoc median </BLOCKQUOTE> 
            \see median for more information */ 
        inline virtual double getMedian() const  
        {
            return this->median; 
        }
        /*! \brief sets the property mean ( \copybrief mean ) to the specified \a __value. 
            \details Description of the parameter mean is: <BLOCKQUOTE>\copydoc mean </BLOCKQUOTE> 
            \see mean for more information */ 
        inline virtual void setMean(double __value)
        {
            if (this->mean != __value) { 
                this->mean = __value; 
                drawMean=true; 
            } 
        } 
        /*! \brief returns the property mean ( \copybrief mean ). 
            \details Description of the parameter mean is: <BLOCKQUOTE>\copydoc mean </BLOCKQUOTE> 
            \see mean for more information */ 
        inline virtual double getMean() const  
        {
            return this->mean; 
        }
        /*! \brief sets the property min ( \copybrief min ) to the specified \a __value. 
            \details Description of the parameter min is: <BLOCKQUOTE>\copydoc min </BLOCKQUOTE> 
            \see min for more information */ 
        inline virtual void setMin(double __value)
        {
            if (this->min != __value) { 
                this->min = __value; 
                drawMinMax=true; 
            } 
        } 
        /*! \brief returns the property min ( \copybrief min ). 
            \details Description of the parameter min is: <BLOCKQUOTE>\copydoc min </BLOCKQUOTE> 
            \see min for more information */ 
        inline virtual double getMin() const  
        {
            return this->min; 
        }
        /*! \brief sets the property max ( \copybrief max ) to the specified \a __value. 
            \details Description of the parameter max is: <BLOCKQUOTE>\copydoc max </BLOCKQUOTE> 
            \see max for more information */ 
        inline virtual void setMax(double __value)
        {
            if (this->max != __value) { 
                this->max = __value; 
                drawMinMax=true; 
            } 
        } 
        /*! \brief returns the property max ( \copybrief max ). 
            \details Description of the parameter max is: <BLOCKQUOTE>\copydoc max </BLOCKQUOTE> 
            \see max for more information */ 
        inline virtual double getMax() const  
        {
            return this->max; 
        }
        /*! \brief sets the property percentile25 ( \copybrief percentile25 ) to the specified \a __value. 
            \details Description of the parameter percentile25 is: <BLOCKQUOTE>\copydoc percentile25 </BLOCKQUOTE> 
            \see percentile25 for more information */ 
        inline virtual void setPercentile25(double __value)
        {
            this->percentile25 = __value;
        } 
        /*! \brief returns the property percentile25 ( \copybrief percentile25 ). 
            \details Description of the parameter percentile25 is: <BLOCKQUOTE>\copydoc percentile25 </BLOCKQUOTE> 
            \see percentile25 for more information */ 
        inline virtual double getPercentile25() const  
        {
            return this->percentile25; 
        }
        /*! \brief sets the property percentile75 ( \copybrief percentile75 ) to the specified \a __value. 
            \details Description of the parameter percentile75 is: <BLOCKQUOTE>\copydoc percentile75 </BLOCKQUOTE> 
            \see percentile75 for more information */ 
        inline virtual void setPercentile75(double __value)
        {
            this->percentile75 = __value;
        } 
        /*! \brief returns the property percentile75 ( \copybrief percentile75 ). 
            \details Description of the parameter percentile75 is: <BLOCKQUOTE>\copydoc percentile75 </BLOCKQUOTE> 
            \see percentile75 for more information */ 
        inline virtual double getPercentile75() const  
        {
            return this->percentile75; 
        }
        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
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
        /*! \brief sets the property whiskerStyle ( \copybrief whiskerStyle ) to the specified \a __value. 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copydoc whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual void setWhiskerStyle(const Qt::PenStyle & __value)  
        {
            this->whiskerStyle = __value;
        } 
        /*! \brief returns the property whiskerStyle ( \copybrief whiskerStyle ). 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copydoc whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual Qt::PenStyle getWhiskerStyle() const  
        {
            return this->whiskerStyle; 
        }
        /*! \brief sets the property lineWidth ( \copybrief lineWidth ) to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth ( \copybrief lineWidth ). 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property boxWidth ( \copybrief boxWidth ) to the specified \a __value. 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copydoc boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual void setBoxWidth(double __value)
        {
            this->boxWidth = __value;
        } 
        /*! \brief returns the property boxWidth ( \copybrief boxWidth ). 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copydoc boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual double getBoxWidth() const  
        {
            return this->boxWidth; 
        }

        /*! \brief sets the property meanSymbol ( \copybrief meanSymbol ) to the specified \a __value. 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copydoc meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual void setMeanSymbol(const JKQTPGraphSymbols & __value)  
        {
            this->meanSymbol = __value;
        } 
        /*! \brief returns the property meanSymbol ( \copybrief meanSymbol ). 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copydoc meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual JKQTPGraphSymbols getMeanSymbol() const  
        {
            return this->meanSymbol; 
        }
        /*! \brief sets the property meanSymbolWidth ( \copybrief meanSymbolWidth ) to the specified \a __value. 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copydoc meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual void setMeanSymbolWidth(double __value)
        {
            this->meanSymbolWidth = __value;
        } 
        /*! \brief returns the property meanSymbolWidth ( \copybrief meanSymbolWidth ). 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copydoc meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual double getMeanSymbolWidth() const  
        {
            return this->meanSymbolWidth; 
        }
        /*! \brief sets the property meanSymbolSize ( \copybrief meanSymbolSize ) to the specified \a __value. 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copydoc meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
        inline virtual void setMeanSymbolSize(double __value)
        {
            this->meanSymbolSize = __value;
        } 
        /*! \brief returns the property meanSymbolSize ( \copybrief meanSymbolSize ). 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copydoc meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
        inline virtual double getMeanSymbolSize() const  
        {
            return this->meanSymbolSize; 
        }
        /*! \brief sets the property drawMean ( \copybrief drawMean ) to the specified \a __value. 
            \details Description of the parameter drawMean is: <BLOCKQUOTE>\copydoc drawMean </BLOCKQUOTE> 
            \see drawMean for more information */ 
        inline virtual void setDrawMean(bool __value)
        {
            this->drawMean = __value;
        } 
        /*! \brief returns the property drawMean ( \copybrief drawMean ). 
            \details Description of the parameter drawMean is: <BLOCKQUOTE>\copydoc drawMean </BLOCKQUOTE> 
            \see drawMean for more information */ 
        inline virtual bool getDrawMean() const  
        {
            return this->drawMean; 
        }
        /*! \brief sets the property drawMedian ( \copybrief drawMedian ) to the specified \a __value. 
            \details Description of the parameter drawMedian is: <BLOCKQUOTE>\copydoc drawMedian </BLOCKQUOTE> 
            \see drawMedian for more information */ 
        inline virtual void setDrawMedian(bool __value)
        {
            this->drawMedian = __value;
        } 
        /*! \brief returns the property drawMedian ( \copybrief drawMedian ). 
            \details Description of the parameter drawMedian is: <BLOCKQUOTE>\copydoc drawMedian </BLOCKQUOTE> 
            \see drawMedian for more information */ 
        inline virtual bool getDrawMedian() const  
        {
            return this->drawMedian; 
        }
        /*! \brief sets the property drawMinMax ( \copybrief drawMinMax ) to the specified \a __value. 
            \details Description of the parameter drawMinMax is: <BLOCKQUOTE>\copydoc drawMinMax </BLOCKQUOTE> 
            \see drawMinMax for more information */ 
        inline virtual void setDrawMinMax(bool __value)
        {
            this->drawMinMax = __value;
        } 
        /*! \brief returns the property drawMinMax ( \copybrief drawMinMax ). 
            \details Description of the parameter drawMinMax is: <BLOCKQUOTE>\copydoc drawMinMax </BLOCKQUOTE> 
            \see drawMinMax for more information */ 
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
class LIB_EXPORT JKQTPBoxplotHorizontalElement: public JKQTPBoxplotVerticalElement {
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
