/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

#ifndef jkqtpgraphsimpulses_H
#define jkqtpgraphsimpulses_H



/** \brief This is a base class for all impulse graphs
 *  \ingroup jkqtplotter_barssticks
 *
 *  \see JKQTPImpulsesHorizontalGraph, JKQTPImpulsesVerticalGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImpulsesGraphBase: public JKQTPXYBaselineGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphSymbolStyleMixin{
    Q_OBJECT
public:
    /** \brief class constructor */
    JKQTPImpulsesGraphBase(JKQTBasePlotter* parent=nullptr);


    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;
    /*! \copydoc drawSymbols */
    bool getDrawSymbols() const;

public slots:
    /*! \brief color of symbols and impulses in one call */
    virtual void setColor(QColor c);

    /*! \copydoc drawSymbols */
    void setDrawSymbols(bool __value);

protected:

    /** \brief indicates whether to draw symbols at the top of the impulse
         *
         * \image html impulsesplot_symbols.png
         */
    bool drawSymbols;
};


/** \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from y=0 to y=f(x) )
 *  \ingroup jkqtplotter_barssticks
 *
 *  \image html plot_impulsesxplots.png
 *
 *  \see JKQTPImpulsesVerticalGraph, \ref JKQTPlotterImpulsePlots
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImpulsesHorizontalGraph: public JKQTPImpulsesGraphBase {
    Q_OBJECT
public:
    /** \brief class constructor */
    JKQTPImpulsesHorizontalGraph(JKQTBasePlotter* parent=nullptr);
    /** \brief class constructor */
    JKQTPImpulsesHorizontalGraph(JKQTPlotter* parent);

    /** \brief plots the graph to the plotter object specified as parent */
    virtual void draw(JKQTPEnhancedPainter& painter) override;
    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

protected:
};



/** \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from x=0 to x=f(y) )
 *  \ingroup jkqtplotter_barssticks
 *
 *  \image html plot_impulsesxerrorsplots.png
 *
 *  \see jkqtpstatAddXErrorImpulsesGraph(), JKQTPImpulsesHorizontalGraph, \ref JKQTPlotterImpulsePlots
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImpulsesHorizontalErrorGraph: public JKQTPImpulsesHorizontalGraph, public JKQTPXGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPImpulsesHorizontalErrorGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPImpulsesHorizontalErrorGraph(JKQTPlotter* parent);
        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override ;

};




/*! \brief This implements an impulse plot with impulses in direction of the Y axis (i.e. from y=0 to y=f(x) )
    \ingroup jkqtplotter_barssticks

    \image html plot_impulsesyplots.png

    \see \ref JKQTPlotterImpulsePlots
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImpulsesVerticalGraph: public JKQTPImpulsesGraphBase {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPImpulsesVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImpulsesVerticalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
};



/*! \brief This implements an impulse plot with impulses in direction of the X axis (i.e. from x=0 to x=f(y) )
    \ingroup jkqtplotter_barssticks

    \image html plot_impulsesyerrorsplots.png

    \see JKQTPImpulsesVerticalGraph, jkqtpstatAddYErrorImpulsesGraph(), \ref JKQTPlotterImpulsePlots
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImpulsesVerticalErrorGraph: public JKQTPImpulsesVerticalGraph, public JKQTPYGraphErrors {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPImpulsesVerticalErrorGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImpulsesVerticalErrorGraph(JKQTPlotter* parent);
        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief this function is used to plot error inidcators before plotting the graphs. */
        virtual void drawErrorsAfter(JKQTPEnhancedPainter& painter) override ;

};




#endif // jkqtpgraphsimpulses_H
