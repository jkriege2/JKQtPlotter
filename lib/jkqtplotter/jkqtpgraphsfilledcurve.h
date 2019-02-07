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


#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"

#ifndef jkqtpgraphsfilledcurve_H
#define jkqtpgraphsfilledcurve_H





/*! \brief This implements filled curve plots where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_filledgraphs

    \image html jkqtplotter_simpletest_filledgraphs.png

    \see \ref JKQTPlotterFilledGraphs
 */
class JKQTP_LIB_EXPORT JKQTPFilledCurveXGraph: public JKQTPXYGraph {
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
        /*! \copydoc style
            \see see style for details */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \copydoc style
            \see see style for details */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
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
        /*! \copydoc baseline
            \see see baseline for details */ 
        inline virtual void setBaseline(double __value)
        {
            this->baseline = __value;
        } 
        /*! \copydoc baseline
            \see see baseline for details */ 
        inline virtual double getBaseline() const  
        {
            return this->baseline; 
        }
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
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

        /*! \copydoc drawSelectionLine
            \see see drawSelectionLine for details */ 
        inline virtual void setDrawSelectionLine(bool __value)  
        {
            this->drawSelectionLine = __value;
        } 
        /*! \copydoc drawSelectionLine
            \see see drawSelectionLine for details */ 
        inline virtual bool getDrawSelectionLine() const  
        {
            return this->drawSelectionLine; 
        }
        /*! \copydoc selectionLineColor
            \see see selectionLineColor for details */ 
        inline virtual void setSelectionLineColor(const QColor & __value)  
        {
            this->selectionLineColor = __value;
        } 
        /*! \copydoc selectionLineColor
            \see see selectionLineColor for details */ 
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
    \ingroup jkqtplotter_filledgraphs

    \image html plot_filledcurvexerrorplots.png


    \see \ref JKQTPlotterFilledGraphs
 */
class JKQTP_LIB_EXPORT JKQTPFilledCurveXErrorGraph: public JKQTPFilledCurveXGraph, public JKQTPYGraphErrors {
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
    \ingroup jkqtplotter_filledgraphs

    \image html jkqtplotter_simpletest_filledgraphs_yaxis.png

    \see \ref JKQTPlotterFilledGraphs

 */
class JKQTP_LIB_EXPORT JKQTPFilledCurveYGraph: public JKQTPFilledCurveXGraph {
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
    \ingroup jkqtplotter_filledgraphs

    \image html plot_filledcurveyerrorplots.png

    \see \ref JKQTPlotterFilledGraphs
 */
class JKQTP_LIB_EXPORT JKQTPFilledCurveYErrorGraph: public JKQTPFilledCurveYGraph, public JKQTPXGraphErrors {
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
    \ingroup jkqtplotter_filledgraphs

    With setDrawlines(false):
    \image html JKQTPFilledVerticalRangeGraph.png

    With setDrawlines(true):
    \image html JKQTPFilledVerticalRangeGraph_WithLines.png



    \see \ref JKQTPlotterDateTimeAxes
 */
class JKQTP_LIB_EXPORT JKQTPFilledVerticalRangeGraph: public JKQTPXYGraph {
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

        /*! \copydoc yColumn2
            \see see yColumn2 for details */ 
        inline virtual void setYColumn2(int __value)  
        {
            this->yColumn2 = __value;
        } 
        /*! \copydoc yColumn2
            \see see yColumn2 for details */ 
        inline virtual int getYColumn2() const  
        {
            return this->yColumn2; 
        }
        /*! \brief sets the property yColumn2 ( \copybrief yColumn2 ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter yColumn2 is:  <BLOCKQUOTE>\copydoc yColumn2 </BLOCKQUOTE> 
            \see yColumn2 for more information */ 
        inline virtual void setYColumn2 (size_t __value) { this->yColumn2 = static_cast<int>(__value); }

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
        /*! \copydoc style
            \see see style for details */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \copydoc style
            \see see style for details */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual void setDrawLine(bool __value)  
        {
            this->drawLine = __value;
        } 
        /*! \copydoc drawLine
            \see see drawLine for details */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
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

        /*! \copydoc drawSelectionLine
            \see see drawSelectionLine for details */ 
        inline virtual void setDrawSelectionLine(bool __value)  
        {
            this->drawSelectionLine = __value;
        } 
        /*! \copydoc drawSelectionLine
            \see see drawSelectionLine for details */ 
        inline virtual bool getDrawSelectionLine() const  
        {
            return this->drawSelectionLine; 
        }
        /*! \copydoc selectionLineColor
            \see see selectionLineColor for details */ 
        inline virtual void setSelectionLineColor(const QColor & __value)  
        {
            this->selectionLineColor = __value;
        } 
        /*! \copydoc selectionLineColor
            \see see selectionLineColor for details */ 
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
