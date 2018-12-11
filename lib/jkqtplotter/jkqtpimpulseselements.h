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
#include "jkqtplotter/jkqtpbasegraphs.h"


#ifndef JKQTPIMPULSESELEMENTS_H
#define JKQTPIMPULSESELEMENTS_H



/*! \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from y=0 to y=f(x) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesxplots.png
 */
class LIB_EXPORT JKQTPimpulsesHorizontalGraph: public JKQTPxyGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPimpulsesHorizontalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPimpulsesHorizontalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(double, baseline)

        /** \brief color of the graph */
        QColor color;
        /** \brief width (pixels) of the lines */
        double lineWidth;

    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        /** \brief baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!)
         */
        double baseline;
        QPen getPen(JKQTPEnhancedPainter &painter) const;
};



/*! \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from x=0 to x=f(y) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesxerrorsplots.png
 */
class LIB_EXPORT JKQTPimpulsesHorizontalErrorGraph: public JKQTPimpulsesHorizontalGraph, public JKQTPxGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPimpulsesHorizontalErrorGraph(JKQtBasePlotter* parent=nullptr);
        JKQTPimpulsesHorizontalErrorGraph(JKQtPlotter* parent);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) ;

};




/*! \brief This implements an impulse plot with impulses in direction of the Y axis (i.e. from y=0 to y=f(x) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesyplots.png
 */
class LIB_EXPORT JKQTPimpulsesVerticalGraph: public JKQTPimpulsesHorizontalGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPimpulsesVerticalGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPimpulsesVerticalGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
};



/*! \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from x=0 to x=f(y) )
    \ingroup jkqtplotter_plots

    \image html plot_impulsesyerrorsplots.png
 */
class LIB_EXPORT JKQTPimpulsesVerticalErrorGraph: public JKQTPimpulsesVerticalGraph, public JKQTPyGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPimpulsesVerticalErrorGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPimpulsesVerticalErrorGraph(JKQtPlotter* parent);
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) ;

};




#endif // JKQTPIMPULSESELEMENTS_H
