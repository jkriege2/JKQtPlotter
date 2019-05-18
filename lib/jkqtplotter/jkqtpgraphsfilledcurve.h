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


#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/jkqtpgraphsspecialline.h"

#ifndef jkqtpgraphsfilledcurve_H
#define jkqtpgraphsfilledcurve_H





/*! \brief This implements filled curve plots where the area is filled between the plot line and the x-Axis.
    \ingroup jkqtplotter_filledgraphs

    \image html jkqtplotter_simpletest_filledgraphs.png

    \see \ref JKQTPlotterFilledGraphs
 */
class JKQTP_LIB_EXPORT JKQTPFilledCurveXGraph: public JKQTPSpecialLineHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPFilledCurveXGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFilledCurveXGraph(JKQTPlotter* parent);
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
class JKQTP_LIB_EXPORT JKQTPFilledCurveYGraph: public JKQTPSpecialLineVerticalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPFilledCurveYGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFilledCurveYGraph(JKQTPlotter* parent);


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
class JKQTP_LIB_EXPORT JKQTPFilledVerticalRangeGraph: public JKQTPXYGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
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
        virtual QColor getKeyLabelColor() const override;

        /*! \copydoc yColumn2
            \see see yColumn2 for details */ 
        void setYColumn2(int __value);
        /*! \copydoc yColumn2
            \see see yColumn2 for details */ 
        int getYColumn2() const;
        /*! \brief sets the property yColumn2 ( \copybrief yColumn2 ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter yColumn2 is:  <BLOCKQUOTE>\copydoc yColumn2 </BLOCKQUOTE> 
            \see yColumn2 for more information */ 
        void setYColumn2 (size_t __value);

        /*! \copydoc drawLine
            \see see drawLine for details */
        void setDrawLine(bool __value);
        /*! \copydoc drawLine
            \see see drawLine for details */
        bool getDrawLine() const;


    protected:

        /** \brief the column that contains the second y-component of the range */
        int yColumn2;

        /** \brief indicates whether to draw a line or not */
        bool drawLine;

};




#endif // jkqtpgraphsfilledcurve_H
