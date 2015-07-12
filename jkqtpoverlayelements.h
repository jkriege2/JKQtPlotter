/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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



/**
 * \defgroup jkqtplotter_overlays Overlay Elements
 * \ingroup jkqtplotter
 */

/** \file jkqtpoverlayelements.h
  * \ingroup jkqtplotter_overlays
  */

#include <QObject>
#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtptools.h"
#include "jkqtp_imexport.h"

#ifndef JKQTPOVERLAYELEMENTS_H
#define JKQTPOVERLAYELEMENTS_H

// forward declarations
class JKQtBasePlotter;

/*! \brief this virtual base class describes an interface for graph overlay elements, which are simple geometric
           forms drawn ONTO the graphe, so a redraw of the overlays does NOT require a redraw of the graph.
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class LIB_EXPORT JKQTPoverlayElement : public QObject {
        Q_OBJECT
    public:
        explicit JKQTPoverlayElement(JKQtBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter)=0;

        /** \brief returns the parent painter class */
        inline JKQtBasePlotter* getParent() { return parent; }

        /** \brief sets the parent painter class */
        virtual void setParent(JKQtBasePlotter* parent);

        JKQTPGET_SET_MACRO(QColor, color)
        JKQTPGET_SET_MACRO(QColor, fillColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, lineStyle)
        JKQTPGET_SET_MACRO(Qt::BrushStyle, fillStyle)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(QString, text)
        JKQTPGET_SET_MACRO(QString, fontName)
        JKQTPGET_SET_MACRO(double, fontSize)
        JKQTPGET_SET_MACRO(bool, visible)

    protected:
        /** \brief the plotter object this object belongs to */
        JKQtBasePlotter* parent;

        QColor color;
        QColor fillColor;
        Qt::PenStyle lineStyle;
        Qt::BrushStyle fillStyle;
        double lineWidth;
        QString text;
        QString fontName;
        double fontSize;
        bool visible;
        QFont getFont() const;


        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        QPointF transform(const QPointF& x);

        double transfromX(double x);
        double transfromY(double y);

        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        QPointF backTransform(const QPointF& x);

        /** \brief tool routine that transforms a QPointF according to the parent's transformation rules */
        inline QPointF transform(double x, double y) {
            return transform(QPointF(x,y));
        };
        /** \brief tool routine that back-transforms a QPointF according to the parent's transformation rules */
        inline QPointF backTransform(double x, double y) {
            return backTransform(QPointF(x,y));
        };
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

        virtual QBrush getBrush(JKQTPEnhancedPainter &painter) const;
        virtual QPen getPen(JKQTPEnhancedPainter& painter) const;

};



/*! \brief baseclass for a two-position overlay
   \ingroup jkqtplotter_overlays
 */
class LIB_EXPORT JKQTPoverlayTwoPositionOverlay : public JKQTPoverlayElement {
        Q_OBJECT
    public:
        explicit JKQTPoverlayTwoPositionOverlay(double x1, double y1, double x2, double y2, JKQtBasePlotter *parent = 0);

        JKQTPGET_SET_MACRO(double, x1)
        JKQTPGET_SET_MACRO(double, x2)
        JKQTPGET_SET_MACRO(double, y1)
        JKQTPGET_SET_MACRO(double, y2)
    protected:
        double x1;
        double y1;
        double x2;
        double y2;
};



/*! \brief baseclass for one-coordinate indicator overlays (horizontal/vertical lines
   \ingroup jkqtplotter_overlays
 */
class LIB_EXPORT JKQTPoverlayOneCoordOverlay : public JKQTPoverlayElement {
        Q_OBJECT
    public:
        explicit JKQTPoverlayOneCoordOverlay(double pos, JKQtBasePlotter *parent = 0);

        JKQTPGET_SET_MACRO(double, position)
    protected:
        double position;
};



/*! \brief baseclass for two-coordinate indicator overlays (horizontal/vertical lines
   \ingroup jkqtplotter_overlays
 */
class LIB_EXPORT JKQTPoverlayTwoCoordOverlay : public JKQTPoverlayOneCoordOverlay {
        Q_OBJECT
    public:
        explicit JKQTPoverlayTwoCoordOverlay(double pos, double pos2, JKQtBasePlotter *parent = 0);

        JKQTPGET_SET_MACRO(double, position2)
    protected:
        double position2;
};




/*! \brief draws a vertical line as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class LIB_EXPORT JKQTPoverlayVerticalLine : public JKQTPoverlayOneCoordOverlay {
        Q_OBJECT
    public:
        explicit JKQTPoverlayVerticalLine(double pos, JKQtBasePlotter *parent = 0);
        explicit JKQTPoverlayVerticalLine(double pos, QString text, JKQtBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
};



/*! \brief draws a vertical range as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class LIB_EXPORT JKQTPoverlayVerticalRange : public JKQTPoverlayTwoCoordOverlay {
        Q_OBJECT
    public:
        explicit JKQTPoverlayVerticalRange(double pos, double pos2, JKQtBasePlotter *parent = 0);
        explicit JKQTPoverlayVerticalRange(double pos, double pos2, QString text, JKQtBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        JKQTPGET_SET_MACRO(bool, inverted)
    protected:
        /** \brief if set \c false, the range is filled, otherwise everything outside the range is filled */
        bool inverted;
};

/*! \brief draws a line throught two points
   \ingroup jkqtplotter_overlays

 */
class LIB_EXPORT JKQTPoverlayLine : public JKQTPoverlayTwoPositionOverlay {
        Q_OBJECT
    public:
        explicit JKQTPoverlayLine(double x1, double y1, double x2, double y2, JKQtBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        JKQTPGET_SET_MACRO(bool, infinite)
    protected:
        /** \brief the line goes on infinitely */
        bool infinite;
};

/*! \brief draws a rectangle, spanned by two points
   \ingroup jkqtplotter_overlays

 */
class LIB_EXPORT JKQTPoverlayRectangle : public JKQTPoverlayTwoPositionOverlay {
        Q_OBJECT
    public:
        explicit JKQTPoverlayRectangle(double x1, double y1, double x2, double y2, JKQtBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

    protected:
};

#endif // JKQTPOVERLAYELEMENTS_H
