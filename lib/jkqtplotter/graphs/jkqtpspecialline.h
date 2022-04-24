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

#ifndef jkqtpgraphsspecialline_H
#define jkqtpgraphsspecialline_H


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/graphs/jkqtprange.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;
//class JKQTPColorPaletteStyleAndToolsMixin;




/** \brief a Base class for special line graphs (steps ...) like e.g. JKQTPSpecialLineHorizontalGraph
 *  \ingroup jkqtplotter_linesymbolgraphs_simple
 *
 *  \image html stepplots.png
 *
 *  \see JKQTPSpecialLineHorizontalGraph, JKQTPSpecialLineVerticalGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSpecialLineGraphBase: public JKQTPXYBaselineGraph, public JKQTPGraphLineAndFillStyleMixin, public JKQTPGraphSymbolStyleMixin {
    Q_OBJECT
public:
    /** \brief class constructor */
    explicit JKQTPSpecialLineGraphBase(JKQTBasePlotter* parent=nullptr);

    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;
    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;


    /** \copydoc m_drawSymbols */
    bool getDrawSymbols() const;
    /** \copydoc m_specialLineType */
    JKQTPSpecialLineType getSpecialLineType() const;

    Q_PROPERTY(bool drawSymbols READ getDrawSymbols WRITE setDrawSymbols)
    Q_PROPERTY(JKQTPSpecialLineType specialLineType READ getSpecialLineType WRITE setSpecialLineType)
public slots:
    /** \brief set line-color, fill color and symbol color */
    void setColor(QColor c);
    /** \copydoc m_drawSymbols */
    void setDrawSymbols(bool __value);
    /** \copydoc m_specialLineType */
    void setSpecialLineType(const JKQTPSpecialLineType & __value);
protected:

    /** \brief indicates whether to draw a symbols at the datapoints, or not */
    bool m_drawSymbols;
    /** \brief type of connecting (step)lines */
    JKQTPSpecialLineType m_specialLineType;


};


/** \brief This implements a step plot with values \f$ \left(x, f(x) \right) \f$
 *  \ingroup jkqtplotter_linesymbolgraphs_simple
 *
 *  \image html stepplots.png
 *
 *  \see JKQTPSpecialLineVerticalGraph, \ref JKQTPlotterSpecialStepLinePlot
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSpecialLineHorizontalGraph: public JKQTPSpecialLineGraphBase {
    Q_OBJECT
public:
    /** \brief class constructor */
    JKQTPSpecialLineHorizontalGraph(JKQTBasePlotter* parent=nullptr);
    /** \brief class constructor */
    JKQTPSpecialLineHorizontalGraph(JKQTPlotter* parent);

    /** \brief plots the graph to the plotter object specified as parent */
    virtual void draw(JKQTPEnhancedPainter& painter) override;

protected:

};



/*! \brief This implements a step plot with values \f$ \left(f(y), y \right) \f$
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html stepplots_vertical.png

    \see JKQTPSpecialLineHorizontalGraph, \ref JKQTPlotterSpecialStepLinePlot
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSpecialLineVerticalGraph: public JKQTPSpecialLineGraphBase {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPSpecialLineVerticalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPSpecialLineVerticalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
};




#endif // jkqtpgraphsspecialline_H
