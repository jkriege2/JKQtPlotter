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


#include <QObject>
#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtcommon/jkqtptools.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

#ifndef jkqtpelementsoverlay_H
#define jkqtpelementsoverlay_H

// forward declarations
class JKQTBasePlotter;

/*! \brief this virtual base class describes an interface for graph overlay elements, which are simple geometric
           forms drawn ONTO the graphe, so a redraw of the overlays does NOT require a redraw of the graph.
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class JKQTP_LIB_EXPORT JKQTPOverlayElement : public QObject {
        Q_OBJECT
    public:
        explicit JKQTPOverlayElement(JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter)=0;

        /** \brief returns the parent painter class */
        inline JKQTBasePlotter* getParent() { return parent; }

        /** \brief sets the parent painter class */
        virtual void setParent(JKQTBasePlotter* parent);



        /*! \copydoc visible
            \see see visible for details */ 
        void setVisible(bool __value);
        /*! \copydoc visible
            \see see visible for details */ 
        bool isVisible() const;

    protected:
        /** \brief the plotter object this object belongs to */
        JKQTBasePlotter* parent;

        /** \brief indictaes whether the overlay is visible (at all) */
        bool visible;


        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        QPointF transform(const QPointF& x);

        double transfromX(double x);
        double transfromY(double y);

        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        QPointF backTransform(const QPointF& x);

        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        inline QPointF transform(double x, double y) {
            return transform(QPointF(x,y));
        }
        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        inline QPointF backTransform(double x, double y) {
            return backTransform(QPointF(x,y));
        }
        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules */
        QVector<QPointF> transform(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a (non-closed) path consisting of lines */
        QPainterPath transformToLinePath(const QVector<QPointF>& x);

        /** \brief tool routine that transforms a QVector<QPointF> according to the parent's transformation rules
         *         and returns a polygon */
        inline QPolygonF transformToPolygon(const QVector<QPointF>& x) {
            return QPolygonF(transform(x));
        }



};



/*! \brief baseclass for a two-position overlay
   \ingroup jkqtplotter_overlays
 */
class JKQTP_LIB_EXPORT JKQTPOverlayTwoPositionOverlay : public JKQTPOverlayElement {
        Q_OBJECT
    public:
        explicit JKQTPOverlayTwoPositionOverlay(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = nullptr);

        /*! \copydoc x1
            \see see x1 for details */ 
        void setX1(double __value);
        /*! \copydoc x1
            \see see x1 for details */ 
        double getX1() const;
        /*! \copydoc x2
            \see see x2 for details */ 
        void setX2(double __value);
        /*! \copydoc x2
            \see see x2 for details */ 
        double getX2() const;
        /*! \copydoc y1
            \see see y1 for details */ 
        void setY1(double __value);
        /*! \copydoc y1
            \see see y1 for details */ 
        double getY1() const;
        /*! \copydoc y2
            \see see y2 for details */ 
        void setY2(double __value);
        /*! \copydoc y2
            \see see y2 for details */ 
        double getY2() const;
    protected:
        /** \brief x-coordinate of first point of overlay */
        double x1;
        /** \brief y-coordinate of first point of overlay */
        double y1;
        /** \brief x-coordinate of first second of overlay */
        double x2;
        /** \brief y-coordinate of first second of overlay */
        double y2;
};



/*! \brief baseclass for one-coordinate indicator overlays (horizontal/vertical lines
   \ingroup jkqtplotter_overlays
 */
class JKQTP_LIB_EXPORT JKQTPOverlayOneCoordOverlay : public JKQTPOverlayElement {
        Q_OBJECT
    public:
        explicit JKQTPOverlayOneCoordOverlay(double pos, JKQTBasePlotter *parent = nullptr);

        /*! \copydoc position
            \see see position for details */ 
        void setPosition(double __value);
        /*! \copydoc position
            \see see position for details */ 
        double getPosition() const;
    protected:
        /** \brief second position for the element */
        double position;
};



/*! \brief baseclass for two-coordinate indicator overlays (horizontal/vertical lines
   \ingroup jkqtplotter_overlays
 */
class JKQTP_LIB_EXPORT JKQTPOverlayTwoCoordOverlay : public JKQTPOverlayOneCoordOverlay {
        Q_OBJECT
    public:
        explicit JKQTPOverlayTwoCoordOverlay(double pos, double pos2, JKQTBasePlotter *parent = nullptr);

        /*! \copydoc position2
            \see see position2 for details */ 
        void setPosition2(double __value);
        /*! \copydoc position2
            \see see position2 for details */ 
        double getPosition2() const;
    protected:
        /** \brief second position for the element */
        double position2;
};




/*! \brief draws a vertical line as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class JKQTP_LIB_EXPORT JKQTPOverlayVerticalLine : public JKQTPOverlayOneCoordOverlay, public JKQTPGraphLineStyleMixin, public JKQTPGraphTextStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayVerticalLine(double pos, JKQTBasePlotter *parent = nullptr);
        explicit JKQTPOverlayVerticalLine(double pos, const QString& text, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief set element color */
        virtual void setColor(QColor c);

        /*! \copydoc text
            \see see text for details */
        void setText(const QString & __value);
        /*! \copydoc text
            \see see text for details */
        QString getText() const;
    protected:
        /** \brief a text to display next to the vertical line */
        QString text;
};



/*! \brief draws a vertical range as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class JKQTP_LIB_EXPORT JKQTPOverlayVerticalRange : public JKQTPOverlayTwoCoordOverlay, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin, public JKQTPGraphTextStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayVerticalRange(double pos, double pos2, JKQTBasePlotter *parent = nullptr);
        explicit JKQTPOverlayVerticalRange(double pos, double pos2, const QString& text, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief set element color */
        virtual void setColor(QColor c);

        /*! \copydoc inverted
            \see see inverted for details */ 
        void setInverted(bool __value);
        /*! \copydoc inverted
            \see see inverted for details */ 
        bool getInverted() const;

        /*! \copydoc text
            \see see text for details */
        void setText(const QString & __value);
        /*! \copydoc text
            \see see text for details */
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
class JKQTP_LIB_EXPORT JKQTPOverlayLine : public JKQTPOverlayTwoPositionOverlay, public JKQTPGraphLineStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayLine(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /*! \copydoc infinite
            \see see infinite for details */ 
        void setInfinite(bool __value);
        /*! \copydoc infinite
            \see see infinite for details */ 
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
class JKQTP_LIB_EXPORT JKQTPOverlayRectangle : public JKQTPOverlayTwoPositionOverlay, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        explicit JKQTPOverlayRectangle(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = nullptr);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief set element color */
        virtual void setColor(QColor c);

    protected:
};

#endif // jkqtpelementsoverlay_H
