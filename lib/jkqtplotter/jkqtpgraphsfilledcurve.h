/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

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
    \ingroup jkqtplotter_plots

    \image html plot_filledcurvexplots.png
 */
class LIB_EXPORT JKQTPfilledCurveXGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPfilledCurveXGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPfilledCurveXGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, baseline)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)

        JKQTPGET_SET_MACRO(bool, drawSelectionLine)
        JKQTPGET_SET_MACRO(QColor, selectionLineColor)

    protected:

        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
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
    \ingroup jkqtplotter_plots

    \image html plot_filledcurvexerrorplots.png
 */
class LIB_EXPORT JKQTPfilledCurveXErrorGraph: public JKQTPfilledCurveXGraph, public JKQTPyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPfilledCurveXErrorGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPfilledCurveXErrorGraph(JKQtPlotter* parent);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override;

};






/*! \brief This implements filled curve plots where the area is filled between the plot line and y-Axis
    \ingroup jkqtplotter_plots

    \image html plot_filledcurveyplots.png
 */
class LIB_EXPORT JKQTPfilledCurveYGraph: public JKQTPfilledCurveXGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPfilledCurveYGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPfilledCurveYGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
};



/*! \brief This implements filled curve plots with y errors where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_plots

    \image html plot_filledcurveyerrorplots.png
 */
class LIB_EXPORT JKQTPfilledCurveYErrorGraph: public JKQTPfilledCurveYGraph, public JKQTPxGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPfilledCurveYErrorGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPfilledCurveYErrorGraph(JKQtPlotter* parent);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override;

};







/*! \brief This implements filled curve plots where the area is filled between two data columns for each x-value
    \ingroup jkqtplotter_plots

    With set_drawlines(false):
    \image html JKQTPfilledVerticalRangeGraph.png

    With set_drawlines(true):
    \image html JKQTPfilledVerticalRangeGraph_WithLines.png
 */
class LIB_EXPORT JKQTPfilledVerticalRangeGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPfilledVerticalRangeGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPfilledVerticalRangeGraph(JKQtPlotter* parent);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int column) const override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        JKQTPGET_SET_MACRO(int, yColumn2)
        JKQTPSET_CAST_MACRO(size_t, int, yColumn2)

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(bool, drawLine)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)

        JKQTPGET_SET_MACRO(bool, drawSelectionLine)
        JKQTPGET_SET_MACRO(QColor, selectionLineColor)

    protected:

        /** \brief the column that contains the second y-component of the range */
        int yColumn2;
        /** \brief if \c true, draws a thick shadow around the line*/
        bool drawSelectionLine;
        /** \brief color for the shadow, or a lighter version of the line color, if transparent (default) */
        QColor selectionLineColor;

        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
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
