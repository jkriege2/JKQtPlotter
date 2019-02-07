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
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"

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
        explicit JKQTPOverlayElement(JKQTBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter)=0;

        /** \brief returns the parent painter class */
        inline JKQTBasePlotter* getParent() { return parent; }

        /** \brief sets the parent painter class */
        virtual void setParent(JKQTBasePlotter* parent);

        /*! \copydoc color
            \see see color for details */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \copydoc color
            \see see color for details */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual void setFillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \copydoc fillColor
            \see see fillColor for details */ 
        inline virtual QColor getFillColor() const  
        {
            return this->fillColor; 
        }
        /*! \copydoc lineStyle
            \see see lineStyle for details */ 
        inline virtual void setLineStyle(const Qt::PenStyle & __value)  
        {
            this->lineStyle = __value;
        } 
        /*! \copydoc lineStyle
            \see see lineStyle for details */ 
        inline virtual Qt::PenStyle getLineStyle() const  
        {
            return this->lineStyle; 
        }
        /*! \copydoc fillStyle
            \see see fillStyle for details */ 
        inline virtual void setFillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \copydoc fillStyle
            \see see fillStyle for details */ 
        inline virtual Qt::BrushStyle getFillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual void setLineWidth(double __value)  
        {
            this->lineWidth = __value;
        } 
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \copydoc text
            \see see text for details */ 
        inline virtual void setText(const QString & __value)  
        {
            this->text = __value;
        } 
        /*! \copydoc text
            \see see text for details */ 
        inline virtual QString getText() const  
        {
            return this->text; 
        }
        /*! \copydoc fontName
            \see see fontName for details */ 
        inline virtual void setFontName(const QString & __value)  
        {
            this->fontName = __value;
        } 
        /*! \copydoc fontName
            \see see fontName for details */ 
        inline virtual QString getFontName() const  
        {
            return this->fontName; 
        }
        /*! \copydoc fontSize
            \see see fontSize for details */ 
        inline virtual void setFontSize(double __value)  
        {
            this->fontSize = __value;
        } 
        /*! \copydoc fontSize
            \see see fontSize for details */ 
        inline virtual double getFontSize() const  
        {
            return this->fontSize; 
        }
        /*! \copydoc visible
            \see see visible for details */ 
        inline virtual void setVisible(bool __value)  
        {
            this->visible = __value;
        } 
        /*! \copydoc visible
            \see see visible for details */ 
        inline virtual bool isVisible() const  
        {
            return this->visible; 
        }

    protected:
        /** \brief the plotter object this object belongs to */
        JKQTBasePlotter* parent;

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
class JKQTP_LIB_EXPORT JKQTPOverlayTwoPositionOverlay : public JKQTPOverlayElement {
        Q_OBJECT
    public:
        explicit JKQTPOverlayTwoPositionOverlay(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = 0);

        /*! \copydoc x1
            \see see x1 for details */ 
        inline virtual void setX1(double __value)  
        {
            this->x1 = __value;
        } 
        /*! \copydoc x1
            \see see x1 for details */ 
        inline virtual double getX1() const  
        {
            return this->x1; 
        }
        /*! \copydoc x2
            \see see x2 for details */ 
        inline virtual void setX2(double __value)  
        {
            this->x2 = __value;
        } 
        /*! \copydoc x2
            \see see x2 for details */ 
        inline virtual double getX2() const  
        {
            return this->x2; 
        }
        /*! \copydoc y1
            \see see y1 for details */ 
        inline virtual void setY1(double __value)  
        {
            this->y1 = __value;
        } 
        /*! \copydoc y1
            \see see y1 for details */ 
        inline virtual double getY1() const  
        {
            return this->y1; 
        }
        /*! \copydoc y2
            \see see y2 for details */ 
        inline virtual void setY2(double __value)  
        {
            this->y2 = __value;
        } 
        /*! \copydoc y2
            \see see y2 for details */ 
        inline virtual double getY2() const  
        {
            return this->y2; 
        }
    protected:
        double x1;
        double y1;
        double x2;
        double y2;
};



/*! \brief baseclass for one-coordinate indicator overlays (horizontal/vertical lines
   \ingroup jkqtplotter_overlays
 */
class JKQTP_LIB_EXPORT JKQTPOverlayOneCoordOverlay : public JKQTPOverlayElement {
        Q_OBJECT
    public:
        explicit JKQTPOverlayOneCoordOverlay(double pos, JKQTBasePlotter *parent = 0);

        /*! \copydoc position
            \see see position for details */ 
        inline virtual void setPosition(double __value)  
        {
            this->position = __value;
        } 
        /*! \copydoc position
            \see see position for details */ 
        inline virtual double getPosition() const  
        {
            return this->position; 
        }
    protected:
        double position;
};



/*! \brief baseclass for two-coordinate indicator overlays (horizontal/vertical lines
   \ingroup jkqtplotter_overlays
 */
class JKQTP_LIB_EXPORT JKQTPOverlayTwoCoordOverlay : public JKQTPOverlayOneCoordOverlay {
        Q_OBJECT
    public:
        explicit JKQTPOverlayTwoCoordOverlay(double pos, double pos2, JKQTBasePlotter *parent = 0);

        /*! \copydoc position2
            \see see position2 for details */ 
        inline virtual void setPosition2(double __value)  
        {
            this->position2 = __value;
        } 
        /*! \copydoc position2
            \see see position2 for details */ 
        inline virtual double getPosition2() const  
        {
            return this->position2; 
        }
    protected:
        double position2;
};




/*! \brief draws a vertical line as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class JKQTP_LIB_EXPORT JKQTPOverlayVerticalLine : public JKQTPOverlayOneCoordOverlay {
        Q_OBJECT
    public:
        explicit JKQTPOverlayVerticalLine(double pos, JKQTBasePlotter *parent = 0);
        explicit JKQTPOverlayVerticalLine(double pos, const QString& text, JKQTBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
};



/*! \brief draws a vertical range as an overlay
   \ingroup jkqtplotter_overlays

   These simple primitive elements can be used to e.g. display fast changing indicators on the graph ...
 */
class JKQTP_LIB_EXPORT JKQTPOverlayVerticalRange : public JKQTPOverlayTwoCoordOverlay {
        Q_OBJECT
    public:
        explicit JKQTPOverlayVerticalRange(double pos, double pos2, JKQTBasePlotter *parent = 0);
        explicit JKQTPOverlayVerticalRange(double pos, double pos2, const QString& text, JKQTBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /*! \copydoc inverted
            \see see inverted for details */ 
        inline virtual void setInverted(bool __value)  
        {
            this->inverted = __value;
        } 
        /*! \copydoc inverted
            \see see inverted for details */ 
        inline virtual bool getInverted() const  
        {
            return this->inverted; 
        }
    protected:
        /** \brief if set \c false, the range is filled, otherwise everything outside the range is filled */
        bool inverted;
};

/*! \brief draws a line throught two points
   \ingroup jkqtplotter_overlays

 */
class JKQTP_LIB_EXPORT JKQTPOverlayLine : public JKQTPOverlayTwoPositionOverlay {
        Q_OBJECT
    public:
        explicit JKQTPOverlayLine(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /*! \copydoc infinite
            \see see infinite for details */ 
        inline virtual void setInfinite(bool __value)  
        {
            this->infinite = __value;
        } 
        /*! \copydoc infinite
            \see see infinite for details */ 
        inline virtual bool getInfinite() const  
        {
            return this->infinite; 
        }
    protected:
        /** \brief the line goes on infinitely */
        bool infinite;
};

/*! \brief draws a rectangle, spanned by two points
   \ingroup jkqtplotter_overlays

 */
class JKQTP_LIB_EXPORT JKQTPOverlayRectangle : public JKQTPOverlayTwoPositionOverlay {
        Q_OBJECT
    public:
        explicit JKQTPOverlayRectangle(double x1, double y1, double x2, double y2, JKQTBasePlotter *parent = 0);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

    protected:
};

#endif // jkqtpelementsoverlay_H
