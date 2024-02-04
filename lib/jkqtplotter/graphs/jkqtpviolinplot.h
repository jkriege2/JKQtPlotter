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

#ifndef jkqtpgraphsviolinplot_H
#define jkqtpgraphsviolinplot_H


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/graphs/jkqtpviolinplotstylingmixins.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"



/** \brief This implements a single vertical <a href="https://en.wikipedia.org/wiki/Violin_plot">Violin Plot</a> as a "geometric element"
 *  \ingroup jkqtplotter_statgraphs
 *  \ingroup jkqtplotter_statgraphs
 *
 *  Represents a violin plot in one of the following styles:
 *
 *  \image html JKQTPGraphViolinplot_SmoothViolin.png
 *
 *  \image html JKQTPGraphViolinplot_StepViolin.png
 *
 *  \image html JKQTPGraphViolinplot_BoxViolin.png
 *
 *  \see JKQTPViolinplotVerticalElement, JKQTPViolinplotHorizontalElement
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPViolinplotElementBase: public JKQTPPlotElement, public JKQTPGraphViolinplotStyleMixin {
    Q_OBJECT
public:
    /** \brief class constructor */
    JKQTPViolinplotElementBase(JKQTBasePlotter* parent=nullptr);

    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;


    /** \copydoc pos */
    void setPos(double __value);
    /** \copydoc pos */
    double getPos() const;
    /** \copydoc median */
    double getMedian() const;
    /** \copydoc mean */
    double getMean() const;
    /** \copydoc min */
    double getMin() const;
    /** \copydoc max */
    double getMax() const;


    /** \copydoc drawMean */
    bool getDrawMean() const;
    /** \copydoc drawMedian */
    bool getDrawMedian() const;
    /** \copydoc drawMinMax */
    bool getDrawMinMax() const;


    /** \copydoc violinPositionColumn */
    int getViolinPositionColumn() const;


    /** \copydoc violinFrequencyColumn */
    int getViolinFrequencyColumn() const;
public Q_SLOTS:
    /** \brief set the color of the graph (colors all elements, based on the given color \a c ) */
    virtual void setColor(QColor c);
    /** \copydoc violinFrequencyColumn */
    void setViolinFrequencyColumn(int __value);
    /** \copydoc violinFrequencyColumn */
    void setViolinFrequencyColumn(size_t __value);
    /** \copydoc violinPositionColumn */
    void setViolinPositionColumn(int __value);
    /** \copydoc violinPositionColumn */
    void setViolinPositionColumn(size_t __value);
    /** \copydoc drawMinMax */
    void setDrawMinMax(bool __value);
    /** \copydoc drawMedian */
    void setDrawMedian(bool __value);
    /** \copydoc drawMean */
    void setDrawMean(bool __value);
    /** \copydoc max */
    void setMax(double __value);
    /** \copydoc min */
    void setMin(double __value);
    /** \copydoc mean */
    void setMean(double __value);
    /** \copydoc median */
    void setMedian(double __value);
protected:

    /** \brief the position of the boxplot on the "other" axis */
    double pos;
    /** \brief the median value to be used for the boxplot */
    double median;
    /** \brief the mean value to be used for the boxplot */
    double mean;
    /** \brief indicates whether to draw the mean */
    bool drawMean;
    /** \brief indicates whether to draw the median */
    bool drawMedian;
    /** \brief indicates whether to draw the percentiles */
    bool drawMinMax;
    /** \brief the minimum value to be used for the boxplot */
    double min;
    /** \brief the maximum value to be used for the boxplot */
    double max;
    /** \brief column with data for the violin outline: category values (along min-max-axis) */
    int violinPositionColumn;
    /** \brief column with data for the violin outline: frequency values (perpendicular to min-max-axis) */
    int violinFrequencyColumn;
};


/*! \brief This implements a single vertical <a href="https://en.wikipedia.org/wiki/Violin_plot">Violin Plot</a> as a "geometric element"
    \ingroup jkqtplotter_statgraphs
    \ingroup jkqtplotter_statgraphs

    Draws a vertical violin plot in one of the following styles:

    \image html JKQTPGraphViolinplot_SmoothViolin.png

    \image html JKQTPGraphViolinplot_StepViolin.png

    \image html JKQTPGraphViolinplot_BoxViolin.png


    This graph/plot element represents a single such plot. Its center is positioned at an x-coordinate provided by setPos().
    The width of the full plot (left+right!) is given by JKQTPGraphViolinplotStyleMixin::setViolinWidthAbsolute().

    The data is represented partly by single values (e.g. setMedian(), setMin(), ...) and partly by the two columns set with
    setViolinPositionColumn() and setViolinFrequencyColumn(). The latter two represent a density estimate (or histogram) of the
    distribution represented by this plot, which is evaluated at the y-positions from setViolinPositionColumn(). For each such
    y-position a (not necessarily normalized) frequency is given by the values in setViolinFrequencyColumn().


    \image html JKQTPViolinplotHorizontalElement_elements.png

    \see \ref JKQTPlotterViolinplotGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPViolinplotVerticalElement: public JKQTPViolinplotElementBase {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPViolinplotVerticalElement(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPViolinplotVerticalElement(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;



        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


};


/*! \brief This implements a single horizontal <a href="https://en.wikipedia.org/wiki/Violin_plot">Violin Plot</a> as a "geometric element"
    \ingroup jkqtplotter_statgraphs
    \ingroup jkqtplotter_statgraphs

    Draws a horizontal violin plot in one of the following styles:

    \image html JKQTPGraphViolinplot_HSmoothViolin.png

    \image html JKQTPGraphViolinplot_HStepViolin.png

    \image html JKQTPGraphViolinplot_HBoxViolin.png

    This graph/plot element represents a single such plot. Its center is positioned at an y-coordinate provided by setPos().
    The width of the full plot (top+bottom!) is given by JKQTPGraphViolinplotStyleMixin::setViolinWidthAbsolute().

    The data is represented partly by single values (e.g. setMedian(), setMin(), ...) and partly by the two columns set with
    setViolinPositionColumn() and setViolinFrequencyColumn(). The latter two represent a density estimate (or histogram) of the
    distribution represented by this plot, which is evaluated at the x-positions from setViolinPositionColumn(). For each such
    x-position a (not necessarily normalized) frequency is given by the values in setViolinFrequencyColumn().

    \image html JKQTPViolinplotHorizontalElement_elements.png


    \see \ref JKQTPlotterViolinplotGraphs

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPViolinplotHorizontalElement: public JKQTPViolinplotElementBase {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPViolinplotHorizontalElement(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPViolinplotHorizontalElement(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;


        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;
};



#endif // jkqtpgraphsviolinplot_H
