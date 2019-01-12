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



/**
 * \defgroup jkqtplotter_elements Plot Elements
 * \ingroup jkqtplotter
 * \defgroup jkqtplotter_plots Graphs
 * \ingroup jkqtplotter_elements
 */

/** \file jkqtpgraphsboxplot.h
  * \ingroup jkqtplotter
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
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplotvertical.png
 */
class LIB_EXPORT JKQTPboxplotVerticalGraph: public JKQTPgraph {
        Q_OBJECT
    public:

        enum DataSortOrder {
            Unsorted=0,
            Sorted=1
        };



        /** \brief class constructor */
        JKQTPboxplotVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPboxplotVerticalGraph(JKQtPlotter* parent);

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

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

        /*! \brief sets the property sortData to the specified \a __value. 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copybrief sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual void set_sortData(const DataSortOrder & __value)  
        {
            this->sortData = __value;
        } 
        /*! \brief returns the property sortData. 
            \details Description of the parameter sortData is: <BLOCKQUOTE>\copybrief sortData </BLOCKQUOTE> 
            \see sortData for more information */ 
        inline virtual DataSortOrder get_sortData() const  
        {
            return this->sortData; 
        }
        /*! \brief sets the property sortData to the specified \a __value. \details Description of the parameter sortData is: <BLOCKQUOTE>\copybrief sortData </BLOCKQUOTE> \see sortData for more information */
        void set_sortData(int __value);
        /*! \brief sets the property posColumn to the specified \a __value. 
            \details Description of the parameter posColumn is: <BLOCKQUOTE>\copybrief posColumn </BLOCKQUOTE> 
            \see posColumn for more information */ 
        inline virtual void set_posColumn(int __value)
        {
            this->posColumn = __value;
        } 
        /*! \brief returns the property posColumn. 
            \details Description of the parameter posColumn is: <BLOCKQUOTE>\copybrief posColumn </BLOCKQUOTE> 
            \see posColumn for more information */ 
        inline virtual int get_posColumn() const  
        {
            return this->posColumn; 
        }
        /*! \brief sets the property posColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter posColumn is:  <BLOCKQUOTE>\copybrief posColumn </BLOCKQUOTE> 
        	\see posColumn for more information */ 
        inline virtual void set_posColumn (size_t __value) { this->posColumn = static_cast<int>(__value); }
        /*! \brief sets the property medianColumn to the specified \a __value. 
            \details Description of the parameter medianColumn is: <BLOCKQUOTE>\copybrief medianColumn </BLOCKQUOTE> 
            \see medianColumn for more information */ 
        inline virtual void set_medianColumn(int __value)
        {
            this->medianColumn = __value;
        } 
        /*! \brief returns the property medianColumn. 
            \details Description of the parameter medianColumn is: <BLOCKQUOTE>\copybrief medianColumn </BLOCKQUOTE> 
            \see medianColumn for more information */ 
        inline virtual int get_medianColumn() const  
        {
            return this->medianColumn; 
        }
        /*! \brief sets the property medianColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter medianColumn is:  <BLOCKQUOTE>\copybrief medianColumn </BLOCKQUOTE> 
        	\see medianColumn for more information */ 
        inline virtual void set_medianColumn (size_t __value) { this->medianColumn = static_cast<int>(__value); }
        /*! \brief sets the property meanColumn to the specified \a __value. 
            \details Description of the parameter meanColumn is: <BLOCKQUOTE>\copybrief meanColumn </BLOCKQUOTE> 
            \see meanColumn for more information */ 
        inline virtual void set_meanColumn(int __value)
        {
            this->meanColumn = __value;
        } 
        /*! \brief returns the property meanColumn. 
            \details Description of the parameter meanColumn is: <BLOCKQUOTE>\copybrief meanColumn </BLOCKQUOTE> 
            \see meanColumn for more information */ 
        inline virtual int get_meanColumn() const  
        {
            return this->meanColumn; 
        }
        /*! \brief sets the property meanColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter meanColumn is:  <BLOCKQUOTE>\copybrief meanColumn </BLOCKQUOTE> 
        	\see meanColumn for more information */ 
        inline virtual void set_meanColumn (size_t __value) { this->meanColumn = static_cast<int>(__value); }
        /*! \brief sets the property minColumn to the specified \a __value. 
            \details Description of the parameter minColumn is: <BLOCKQUOTE>\copybrief minColumn </BLOCKQUOTE> 
            \see minColumn for more information */ 
        inline virtual void set_minColumn(int __value)
        {
            this->minColumn = __value;
        } 
        /*! \brief returns the property minColumn. 
            \details Description of the parameter minColumn is: <BLOCKQUOTE>\copybrief minColumn </BLOCKQUOTE> 
            \see minColumn for more information */ 
        inline virtual int get_minColumn() const  
        {
            return this->minColumn; 
        }
        /*! \brief sets the property minColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter minColumn is:  <BLOCKQUOTE>\copybrief minColumn </BLOCKQUOTE> 
        	\see minColumn for more information */ 
        inline virtual void set_minColumn( size_t __value) { this->minColumn = static_cast<int>(__value); }
        /*! \brief sets the property maxColumn to the specified \a __value. 
            \details Description of the parameter maxColumn is: <BLOCKQUOTE>\copybrief maxColumn </BLOCKQUOTE> 
            \see maxColumn for more information */ 
        inline virtual void set_maxColumn(int __value)
        {
            this->maxColumn = __value;
        } 
        /*! \brief returns the property maxColumn. 
            \details Description of the parameter maxColumn is: <BLOCKQUOTE>\copybrief maxColumn </BLOCKQUOTE> 
            \see maxColumn for more information */ 
        inline virtual int get_maxColumn() const  
        {
            return this->maxColumn; 
        }
        /*! \brief sets the property maxColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter maxColumn is:  <BLOCKQUOTE>\copybrief maxColumn </BLOCKQUOTE> 
        	\see maxColumn for more information */ 
        inline virtual void set_maxColumn (size_t __value) { this->maxColumn = static_cast<int>(__value); }
        /*! \brief sets the property percentile25Column to the specified \a __value. 
            \details Description of the parameter percentile25Column is: <BLOCKQUOTE>\copybrief percentile25Column </BLOCKQUOTE> 
            \see percentile25Column for more information */ 
        inline virtual void set_percentile25Column(int __value)
        {
            this->percentile25Column = __value;
        } 
        /*! \brief returns the property percentile25Column. 
            \details Description of the parameter percentile25Column is: <BLOCKQUOTE>\copybrief percentile25Column </BLOCKQUOTE> 
            \see percentile25Column for more information */ 
        inline virtual int get_percentile25Column() const  
        {
            return this->percentile25Column; 
        }
        /*! \brief sets the property percentile25Column to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter percentile25Column is:  <BLOCKQUOTE>\copybrief percentile25Column </BLOCKQUOTE> 
        	\see percentile25Column for more information */ 
        inline virtual void set_percentile25Column (size_t __value) { this->percentile25Column = static_cast<int>(__value); }
        /*! \brief sets the property percentile75Column to the specified \a __value. 
            \details Description of the parameter percentile75Column is: <BLOCKQUOTE>\copybrief percentile75Column </BLOCKQUOTE> 
            \see percentile75Column for more information */ 
        inline virtual void set_percentile75Column(int __value)
        {
            this->percentile75Column = __value;
        } 
        /*! \brief returns the property percentile75Column. 
            \details Description of the parameter percentile75Column is: <BLOCKQUOTE>\copybrief percentile75Column </BLOCKQUOTE> 
            \see percentile75Column for more information */ 
        inline virtual int get_percentile75Column() const  
        {
            return this->percentile75Column; 
        }
        /*! \brief sets the property percentile75Column to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter percentile75Column is:  <BLOCKQUOTE>\copybrief percentile75Column </BLOCKQUOTE> 
        	\see percentile75Column for more information */ 
        inline virtual void set_percentile75Column (size_t __value) { this->percentile75Column = static_cast<int>(__value); }
        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copybrief color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copybrief color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor to the specified \a __value. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copybrief fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual void set_fillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copybrief fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual QColor get_fillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property whiskerStyle to the specified \a __value. 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copybrief whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual void set_whiskerStyle(const Qt::PenStyle & __value)  
        {
            this->whiskerStyle = __value;
        } 
        /*! \brief returns the property whiskerStyle. 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copybrief whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual Qt::PenStyle get_whiskerStyle() const  
        {
            return this->whiskerStyle; 
        }
        /*! \brief sets the property fillStyle to the specified \a __value. 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copybrief fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual void set_fillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle. 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copybrief fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle get_fillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copybrief lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void set_lineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copybrief lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double get_lineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property boxWidth to the specified \a __value. 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copybrief boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual void set_boxWidth(double __value)
        {
            this->boxWidth = __value;
        } 
        /*! \brief returns the property boxWidth. 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copybrief boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual double get_boxWidth() const  
        {
            return this->boxWidth; 
        }

        /*! \brief sets the property meanSymbol to the specified \a __value. 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copybrief meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual void set_meanSymbol(const JKQTPgraphSymbols & __value)  
        {
            this->meanSymbol = __value;
        } 
        /*! \brief returns the property meanSymbol. 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copybrief meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual JKQTPgraphSymbols get_meanSymbol() const  
        {
            return this->meanSymbol; 
        }
        /*! \brief sets the property meanSymbolWidth to the specified \a __value. 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copybrief meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual void set_meanSymbolWidth(double __value)
        {
            this->meanSymbolWidth = __value;
        } 
        /*! \brief returns the property meanSymbolWidth. 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copybrief meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual double get_meanSymbolWidth() const  
        {
            return this->meanSymbolWidth; 
        }
        /*! \brief sets the property meanSymbolSize to the specified \a __value. 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copybrief meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
        inline virtual void set_meanSymbolSize(double __value)
        {
            this->meanSymbolSize = __value;
        } 
        /*! \brief returns the property meanSymbolSize. 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copybrief meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
        inline virtual double get_meanSymbolSize() const  
        {
            return this->meanSymbolSize; 
        }

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
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
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief width of box in percent of distance between the current two posColumn values
         *         if we only plot one box&whiskers then this is the width in plot coordinates */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPgraphSymbols meanSymbol;
        /** \brief size (diameter in pixels) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the mean */
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
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the medianColumn, minColumn, maxColumn,
    percentile25Column and percentile75Column.

    \image html plot_boxplothorizontal.png
 */
class LIB_EXPORT JKQTPboxplotHorizontalGraph: public JKQTPboxplotVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPboxplotHorizontalGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPboxplotHorizontalGraph(JKQtPlotter* parent);

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








/*! \brief This implements a vertical <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPboxplotVerticalGraph
    \ingroup jkqtplotter_plots

    the x position is given in posColumn. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplotvertical.png
 */
class LIB_EXPORT JKQTPboxplotVerticalElement: public JKQTPgraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPboxplotVerticalElement(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPboxplotVerticalElement(JKQtPlotter* parent);

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


        /*! \brief sets the property pos to the specified \a __value. 
            \details Description of the parameter pos is: <BLOCKQUOTE>\copybrief pos </BLOCKQUOTE> 
            \see pos for more information */ 
        inline virtual void set_pos(double __value)
        {
            this->pos = __value;
        } 
        /*! \brief returns the property pos. 
            \details Description of the parameter pos is: <BLOCKQUOTE>\copybrief pos </BLOCKQUOTE> 
            \see pos for more information */ 
        inline virtual double get_pos() const  
        {
            return this->pos; 
        }
        /*! \brief sets the property median to the specified \a __value. 
            \details Description of the parameter median is: <BLOCKQUOTE>\copybrief median </BLOCKQUOTE> 
            \see median for more information */ 
        inline virtual void set_median(double __value)
        {
            if (this->median != __value) { 
                this->median = __value; 
                drawMedian=true; 
            } 
        } 
        /*! \brief returns the property median. 
            \details Description of the parameter median is: <BLOCKQUOTE>\copybrief median </BLOCKQUOTE> 
            \see median for more information */ 
        inline virtual double get_median() const  
        {
            return this->median; 
        }
        /*! \brief sets the property mean to the specified \a __value. 
            \details Description of the parameter mean is: <BLOCKQUOTE>\copybrief mean </BLOCKQUOTE> 
            \see mean for more information */ 
        inline virtual void set_mean(double __value)
        {
            if (this->mean != __value) { 
                this->mean = __value; 
                drawMean=true; 
            } 
        } 
        /*! \brief returns the property mean. 
            \details Description of the parameter mean is: <BLOCKQUOTE>\copybrief mean </BLOCKQUOTE> 
            \see mean for more information */ 
        inline virtual double get_mean() const  
        {
            return this->mean; 
        }
        /*! \brief sets the property min to the specified \a __value. 
            \details Description of the parameter min is: <BLOCKQUOTE>\copybrief min </BLOCKQUOTE> 
            \see min for more information */ 
        inline virtual void set_min(double __value)
        {
            if (this->min != __value) { 
                this->min = __value; 
                drawMinMax=true; 
            } 
        } 
        /*! \brief returns the property min. 
            \details Description of the parameter min is: <BLOCKQUOTE>\copybrief min </BLOCKQUOTE> 
            \see min for more information */ 
        inline virtual double get_min() const  
        {
            return this->min; 
        }
        /*! \brief sets the property max to the specified \a __value. 
            \details Description of the parameter max is: <BLOCKQUOTE>\copybrief max </BLOCKQUOTE> 
            \see max for more information */ 
        inline virtual void set_max(double __value)
        {
            if (this->max != __value) { 
                this->max = __value; 
                drawMinMax=true; 
            } 
        } 
        /*! \brief returns the property max. 
            \details Description of the parameter max is: <BLOCKQUOTE>\copybrief max </BLOCKQUOTE> 
            \see max for more information */ 
        inline virtual double get_max() const  
        {
            return this->max; 
        }
        /*! \brief sets the property percentile25 to the specified \a __value. 
            \details Description of the parameter percentile25 is: <BLOCKQUOTE>\copybrief percentile25 </BLOCKQUOTE> 
            \see percentile25 for more information */ 
        inline virtual void set_percentile25(double __value)
        {
            this->percentile25 = __value;
        } 
        /*! \brief returns the property percentile25. 
            \details Description of the parameter percentile25 is: <BLOCKQUOTE>\copybrief percentile25 </BLOCKQUOTE> 
            \see percentile25 for more information */ 
        inline virtual double get_percentile25() const  
        {
            return this->percentile25; 
        }
        /*! \brief sets the property percentile75 to the specified \a __value. 
            \details Description of the parameter percentile75 is: <BLOCKQUOTE>\copybrief percentile75 </BLOCKQUOTE> 
            \see percentile75 for more information */ 
        inline virtual void set_percentile75(double __value)
        {
            this->percentile75 = __value;
        } 
        /*! \brief returns the property percentile75. 
            \details Description of the parameter percentile75 is: <BLOCKQUOTE>\copybrief percentile75 </BLOCKQUOTE> 
            \see percentile75 for more information */ 
        inline virtual double get_percentile75() const  
        {
            return this->percentile75; 
        }
        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copybrief color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copybrief color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor to the specified \a __value. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copybrief fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual void set_fillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copybrief fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual QColor get_fillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property whiskerStyle to the specified \a __value. 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copybrief whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual void set_whiskerStyle(const Qt::PenStyle & __value)  
        {
            this->whiskerStyle = __value;
        } 
        /*! \brief returns the property whiskerStyle. 
            \details Description of the parameter whiskerStyle is: <BLOCKQUOTE>\copybrief whiskerStyle </BLOCKQUOTE> 
            \see whiskerStyle for more information */ 
        inline virtual Qt::PenStyle get_whiskerStyle() const  
        {
            return this->whiskerStyle; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copybrief lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void set_lineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copybrief lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double get_lineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property boxWidth to the specified \a __value. 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copybrief boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual void set_boxWidth(double __value)
        {
            this->boxWidth = __value;
        } 
        /*! \brief returns the property boxWidth. 
            \details Description of the parameter boxWidth is: <BLOCKQUOTE>\copybrief boxWidth </BLOCKQUOTE> 
            \see boxWidth for more information */ 
        inline virtual double get_boxWidth() const  
        {
            return this->boxWidth; 
        }

        /*! \brief sets the property meanSymbol to the specified \a __value. 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copybrief meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual void set_meanSymbol(const JKQTPgraphSymbols & __value)  
        {
            this->meanSymbol = __value;
        } 
        /*! \brief returns the property meanSymbol. 
            \details Description of the parameter meanSymbol is: <BLOCKQUOTE>\copybrief meanSymbol </BLOCKQUOTE> 
            \see meanSymbol for more information */ 
        inline virtual JKQTPgraphSymbols get_meanSymbol() const  
        {
            return this->meanSymbol; 
        }
        /*! \brief sets the property meanSymbolWidth to the specified \a __value. 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copybrief meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual void set_meanSymbolWidth(double __value)
        {
            this->meanSymbolWidth = __value;
        } 
        /*! \brief returns the property meanSymbolWidth. 
            \details Description of the parameter meanSymbolWidth is: <BLOCKQUOTE>\copybrief meanSymbolWidth </BLOCKQUOTE> 
            \see meanSymbolWidth for more information */ 
        inline virtual double get_meanSymbolWidth() const  
        {
            return this->meanSymbolWidth; 
        }
        /*! \brief sets the property meanSymbolSize to the specified \a __value. 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copybrief meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
        inline virtual void set_meanSymbolSize(double __value)
        {
            this->meanSymbolSize = __value;
        } 
        /*! \brief returns the property meanSymbolSize. 
            \details Description of the parameter meanSymbolSize is: <BLOCKQUOTE>\copybrief meanSymbolSize </BLOCKQUOTE> 
            \see meanSymbolSize for more information */ 
        inline virtual double get_meanSymbolSize() const  
        {
            return this->meanSymbolSize; 
        }
        /*! \brief sets the property drawMean to the specified \a __value. 
            \details Description of the parameter drawMean is: <BLOCKQUOTE>\copybrief drawMean </BLOCKQUOTE> 
            \see drawMean for more information */ 
        inline virtual void set_drawMean(bool __value)
        {
            this->drawMean = __value;
        } 
        /*! \brief returns the property drawMean. 
            \details Description of the parameter drawMean is: <BLOCKQUOTE>\copybrief drawMean </BLOCKQUOTE> 
            \see drawMean for more information */ 
        inline virtual bool get_drawMean() const  
        {
            return this->drawMean; 
        }
        /*! \brief sets the property drawMedian to the specified \a __value. 
            \details Description of the parameter drawMedian is: <BLOCKQUOTE>\copybrief drawMedian </BLOCKQUOTE> 
            \see drawMedian for more information */ 
        inline virtual void set_drawMedian(bool __value)
        {
            this->drawMedian = __value;
        } 
        /*! \brief returns the property drawMedian. 
            \details Description of the parameter drawMedian is: <BLOCKQUOTE>\copybrief drawMedian </BLOCKQUOTE> 
            \see drawMedian for more information */ 
        inline virtual bool get_drawMedian() const  
        {
            return this->drawMedian; 
        }
        /*! \brief sets the property drawMinMax to the specified \a __value. 
            \details Description of the parameter drawMinMax is: <BLOCKQUOTE>\copybrief drawMinMax </BLOCKQUOTE> 
            \see drawMinMax for more information */ 
        inline virtual void set_drawMinMax(bool __value)
        {
            this->drawMinMax = __value;
        } 
        /*! \brief returns the property drawMinMax. 
            \details Description of the parameter drawMinMax is: <BLOCKQUOTE>\copybrief drawMinMax </BLOCKQUOTE> 
            \see drawMinMax for more information */ 
        inline virtual bool get_drawMinMax() const  
        {
            return this->drawMinMax; 
        }

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
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
        /** \brief width of box in plot coordinates */
        double boxWidth;
        /** \brief which symbol to use for the mean  */
        JKQTPgraphSymbols meanSymbol;
        /** \brief size (diameter in pixels) of the symbol for the mean */
        double meanSymbolSize;
        /** \brief width (in pixels) of the lines used to plot the symbol for the mean */
        double meanSymbolWidth;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;
};


/*! \brief This implements a horizontal <a href="http://en.wikipedia.org/wiki/Box_plot">boxplot</a> where the data is directly given to the
           object and not stored in a column, as in JKQTPboxplotVerticalGraph
    \ingroup jkqtplotter_plots

    the x position is given in pos. All other data are given in the median, min, max,
    percentile25 and percentile75.

    \image html plot_boxplothorizontal.png
 */
class LIB_EXPORT JKQTPboxplotHorizontalElement: public JKQTPboxplotVerticalElement {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPboxplotHorizontalElement(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPboxplotHorizontalElement(JKQtPlotter* parent);

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
