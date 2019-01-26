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


#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"

#ifndef jkqtpgraphsfilledcurve_H
#define jkqtpgraphsfilledcurve_H





/*! \brief This implements filled curve plots where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_linesymbolgraphs

    \image html plot_filledcurvexerrorplots.png
 */
class LIB_EXPORT JKQTPFilledCurveXGraph: public JKQTPXYGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPFilledCurveXGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFilledCurveXGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

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
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
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
        /*! \brief sets the property baseline ( \copybrief baseline ) to the specified \a __value. 
            \details Description of the parameter baseline is: <BLOCKQUOTE>\copydoc baseline </BLOCKQUOTE> 
            \see baseline for more information */ 
        inline virtual void setBaseline(double __value)
        {
            this->baseline = __value;
        } 
        /*! \brief returns the property baseline ( \copybrief baseline ). 
            \details Description of the parameter baseline is: <BLOCKQUOTE>\copydoc baseline </BLOCKQUOTE> 
            \see baseline for more information */ 
        inline virtual double getBaseline() const  
        {
            return this->baseline; 
        }
        /*! \brief sets the property drawLine ( \copybrief drawLine ) to the specified \a __value. 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE> 
            \see drawLine for more information */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine ( \copybrief drawLine ). 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE> 
            \see drawLine for more information */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
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

        /*! \brief sets the property drawSelectionLine ( \copybrief drawSelectionLine ) to the specified \a __value. 
            \details Description of the parameter drawSelectionLine is: <BLOCKQUOTE>\copydoc drawSelectionLine </BLOCKQUOTE> 
            \see drawSelectionLine for more information */ 
        inline virtual void setDrawSelectionLine(bool __value)  
        {
            this->drawSelectionLine = __value;
        } 
        /*! \brief returns the property drawSelectionLine ( \copybrief drawSelectionLine ). 
            \details Description of the parameter drawSelectionLine is: <BLOCKQUOTE>\copydoc drawSelectionLine </BLOCKQUOTE> 
            \see drawSelectionLine for more information */ 
        inline virtual bool getDrawSelectionLine() const  
        {
            return this->drawSelectionLine; 
        }
        /*! \brief sets the property selectionLineColor ( \copybrief selectionLineColor ) to the specified \a __value. 
            \details Description of the parameter selectionLineColor is: <BLOCKQUOTE>\copydoc selectionLineColor </BLOCKQUOTE> 
            \see selectionLineColor for more information */ 
        inline virtual void setSelectionLineColor(const QColor & __value)  
        {
            this->selectionLineColor = __value;
        } 
        /*! \brief returns the property selectionLineColor ( \copybrief selectionLineColor ). 
            \details Description of the parameter selectionLineColor is: <BLOCKQUOTE>\copydoc selectionLineColor </BLOCKQUOTE> 
            \see selectionLineColor for more information */ 
        inline virtual QColor getSelectionLineColor() const  
        {
            return this->selectionLineColor; 
        }

    protected:

        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief fill styl for the curve */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!)
         */
        double baseline;
        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;
};



/*! \brief This implements filled curve plots with y errors where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_linesymbolgraphs

    \image html plot_filledcurvexerrorplots.png
 */
class LIB_EXPORT JKQTPFilledCurveXErrorGraph: public JKQTPFilledCurveXGraph, public JKQTPYGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPFilledCurveXErrorGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFilledCurveXErrorGraph(JKQTPlotter* parent);
        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override;

};






/*! \brief This implements filled curve plots where the area is filled between the plot line and y-Axis
    \ingroup jkqtplotter_linesymbolgraphs

    \image html plot_filledcurvexplots.png
 */
class LIB_EXPORT JKQTPFilledCurveYGraph: public JKQTPFilledCurveXGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPFilledCurveYGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFilledCurveYGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
};



/*! \brief This implements filled curve plots with y errors where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_linesymbolgraphs

    \image html plot_filledcurveyerrorplots.png
 */
class LIB_EXPORT JKQTPFilledCurveYErrorGraph: public JKQTPFilledCurveYGraph, public JKQTPXGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPFilledCurveYErrorGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPFilledCurveYErrorGraph(JKQTPlotter* parent);
        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override;

};







/*! \brief This implements filled curve plots where the area is filled between two data columns for each x-value
    \ingroup jkqtplotter_linesymbolgraphs

    With setDrawlines(false):
    \image html JKQTPFilledVerticalRangeGraph.png

    With setDrawlines(true):
    \image html JKQTPFilledVerticalRangeGraph_WithLines.png
 */
class LIB_EXPORT JKQTPFilledVerticalRangeGraph: public JKQTPXYGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPFilledVerticalRangeGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFilledVerticalRangeGraph(JKQTPlotter* parent);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int column) const override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /*! \brief sets the property yColumn2 ( \copybrief yColumn2 ) to the specified \a __value. 
            \details Description of the parameter yColumn2 is: <BLOCKQUOTE>\copydoc yColumn2 </BLOCKQUOTE> 
            \see yColumn2 for more information */ 
        inline virtual void setYColumn2(int __value)  
        {
            this->yColumn2 = __value;
        } 
        /*! \brief returns the property yColumn2 ( \copybrief yColumn2 ). 
            \details Description of the parameter yColumn2 is: <BLOCKQUOTE>\copydoc yColumn2 </BLOCKQUOTE> 
            \see yColumn2 for more information */ 
        inline virtual int getYColumn2() const  
        {
            return this->yColumn2; 
        }
        /*! \brief sets the property yColumn2 ( \copybrief yColumn2 ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter yColumn2 is:  <BLOCKQUOTE>\copydoc yColumn2 </BLOCKQUOTE> 
        	\see yColumn2 for more information */ 
        inline virtual void setYColumn2 (size_t __value) { this->yColumn2 = static_cast<int>(__value); }

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
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \brief sets the property drawLine ( \copybrief drawLine ) to the specified \a __value. 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE> 
            \see drawLine for more information */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine ( \copybrief drawLine ). 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE> 
            \see drawLine for more information */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
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

        /*! \brief sets the property drawSelectionLine ( \copybrief drawSelectionLine ) to the specified \a __value. 
            \details Description of the parameter drawSelectionLine is: <BLOCKQUOTE>\copydoc drawSelectionLine </BLOCKQUOTE> 
            \see drawSelectionLine for more information */ 
        inline virtual void setDrawSelectionLine(bool __value)  
        {
            this->drawSelectionLine = __value;
        } 
        /*! \brief returns the property drawSelectionLine ( \copybrief drawSelectionLine ). 
            \details Description of the parameter drawSelectionLine is: <BLOCKQUOTE>\copydoc drawSelectionLine </BLOCKQUOTE> 
            \see drawSelectionLine for more information */ 
        inline virtual bool getDrawSelectionLine() const  
        {
            return this->drawSelectionLine; 
        }
        /*! \brief sets the property selectionLineColor ( \copybrief selectionLineColor ) to the specified \a __value. 
            \details Description of the parameter selectionLineColor is: <BLOCKQUOTE>\copydoc selectionLineColor </BLOCKQUOTE> 
            \see selectionLineColor for more information */ 
        inline virtual void setSelectionLineColor(const QColor & __value)  
        {
            this->selectionLineColor = __value;
        } 
        /*! \brief returns the property selectionLineColor ( \copybrief selectionLineColor ). 
            \details Description of the parameter selectionLineColor is: <BLOCKQUOTE>\copydoc selectionLineColor </BLOCKQUOTE> 
            \see selectionLineColor for more information */ 
        inline virtual QColor getSelectionLineColor() const  
        {
            return this->selectionLineColor; 
        }

    protected:

        /** \brief the column that contains the second y-component of the range */
        int yColumn2;
        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief fill styl for the curve */
        Qt::BrushStyle fillStyle;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;

        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;
};




#endif // jkqtpgraphsfilledcurve_H
