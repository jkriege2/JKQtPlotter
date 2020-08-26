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


#include <QObject>
#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpoverlaysbase.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

#ifndef jkqtpbasicoverlays_H
#define jkqtpbasicoverlays_H

// forward declarations
class JKQTBasePlotter;



/*! \brief draws a vertical line as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPOverlayVerticalLine : public JKQTPOverlayOneCoordOverlay, public JKQTPGraphLineStyleMixin, public JKQTPGraphTextStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayVerticalLine(double pos, JKQTBasePlotter *parent = nullptr);
        explicit JKQTPOverlayVerticalLine(double pos, const QString& text, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief set element color */
        virtual void setColor(QColor c);

        /*! \copydoc text */
        void setText(const QString & __value);
        /*! \copydoc text */
        QString getText() const;
    protected:
        /** \brief a text to display next to the vertical line */
        QString text;
};



/*! \brief draws a vertical range as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPOverlayVerticalRange : public JKQTPOverlayTwoCoordOverlay, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin, public JKQTPGraphTextStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayVerticalRange(double pos, double pos2, JKQTBasePlotter *parent = nullptr);
        explicit JKQTPOverlayVerticalRange(double pos, double pos2, const QString& text, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief set element color */
        virtual void setColor(QColor c);

        /*! \copydoc inverted */ 
        void setInverted(bool __value);
        /*! \copydoc inverted */ 
        bool getInverted() const;

        /*! \copydoc text */
        void setText(const QString & __value);
        /*! \copydoc text */
        QString getText() const;
    protected:
        /** \brief a text to display next to the range */
        QString text;
        /** \brief if set \c false, the range is filled, otherwise everything outside the range is filled */
        bool inverted;
};

/*! \brief draws a line throught two points
   \ingroup jkqtplotter_overlays

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPOverlayLine : public JKQTPOverlayTwoPositionOverlay, public JKQTPGraphLineStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayLine(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /*! \copydoc infinite */ 
        void setInfinite(bool __value);
        /*! \copydoc infinite */ 
        bool getInfinite() const;
        /** \brief set element color */
        virtual void setColor(QColor c);
    protected:
        /** \brief the line goes on infinitely */
        bool infinite;
};

/*! \brief draws a rectangle, spanned by two points
   \ingroup jkqtplotter_overlays

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPOverlayRectangle : public JKQTPOverlayTwoPositionOverlay, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayRectangle(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief set element color */
        virtual void setColor(QColor c);

    protected:
};

#endif // jkqtpbasicoverlays_H
