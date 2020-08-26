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

#ifndef jkqtpgraphsspecialline_H
#define jkqtpgraphsspecialline_H

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;
//class JKQTPColorPaletteStyleAndToolsMixin;




/*! \brief This implements a step plot with values \f$ \left(x, f(x) \right) \f$
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html stepplots.png

    \see JKQTPSpecialLineVerticalGraph, JKQTPFilledCurveXGraph, \ref JKQTPlotterSpecialStepLinePlot
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSpecialLineHorizontalGraph: public JKQTPXYGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin, public JKQTPGraphSymbolStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPSpecialLineHorizontalGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPSpecialLineHorizontalGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /*! sets whether to draw a line or not */
        void setDrawLine(bool __value);
        /*! returns whether the graph line is drawn */
        bool getDrawLine() const;
        /*! sets whether to draw symbols */
        void setDrawSymbols(bool __value);
        /*! returns whether symbols are drawn */
        bool getDrawSymbols() const;
        /*! sets whether to fill the space between the curve and the baseline/x-axis */
        void setFillCurve(bool __value);
        /*! returns whether the space between the curve and the baseline/x-axis is filled */
        bool getFillCurve() const;
        /** \brief set the type of connecting (step-)lines */
        void setSpecialLineType(const JKQTPSpecialLineType & __value);
        /** \brief get the type of connecting (step-)lines */
        JKQTPSpecialLineType getSpecialLineType() const;
        /*! sets baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!) */
        void setBaseline(double __value);
        /*! returns the baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!) */
        double getBaseline() const;
        /** \brief set line-color, fill color and symbol color */
        void setColor(QColor c);
    protected:
        /** \brief indicates whether to draw a line or not */
        bool m_drawLine;
        /** \brief indicates whether to draw a symbols or not */
        bool m_drawSymbols;
        /** \brief indicates whether to fill the space between the curve and the baseline/x-axis */
        bool m_fillCurve;
        /** \brief type of connecting (step)lines */
        JKQTPSpecialLineType m_specialLineType;
        /** \brief baseline of the plot (NOTE: 0 is interpreted as until plot border in log-mode!!!) */
        double m_baseline;

};



/*! \brief This implements a step plot with values \f$ \left(f(y), y \right) \f$
    \ingroup jkqtplotter_linesymbolgraphs_simple

    \image html stepplots_vertical.png

    \see JKQTPSpecialLineHorizontalGraph, JKQTPFilledCurveYGraph, \ref JKQTPlotterSpecialStepLinePlot
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSpecialLineVerticalGraph: public JKQTPSpecialLineHorizontalGraph {
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
