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
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"

#ifndef jkqtpgeobase_H_INCLUDED
#define jkqtpgeobase_H_INCLUDED


/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseLine: public JKQTPPlotObject, public JKQTPGraphLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param color color of drawing
        \param style line style of drawing
        \param lineWidth lineWidth of drawing
        \param parent the parent plotter object
        */
    explicit JKQTPGeoBaseLine(QColor color, double lineWidth, Qt::PenStyle style=Qt::SolidLine, JKQTBasePlotter* parent=nullptr, DrawMode drawMode=DrawAsGraphicElement);

    /*! \brief class contructor

    */
    explicit JKQTPGeoBaseLine(JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);




    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};




/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that consist of lines with one decorated end (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseDecoratedHeadLine: public JKQTPPlotObject, public JKQTPGraphDecoratedHeadLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param color color of drawing
        \param lineWidth lineWidth of drawing
        \param headStyle style of the head decoration
        \param style line style of drawing
        \param parent the parent plotter object
    */
    explicit JKQTPGeoBaseDecoratedHeadLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, Qt::PenStyle style=Qt::SolidLine, JKQTBasePlotter* parent=nullptr, DrawMode drawMode=DrawAsGraphicElement);

    /*! \brief class contructor

    */
    explicit JKQTPGeoBaseDecoratedHeadLine(JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);




    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};




/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that consist of lines with decorated ends (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseDecoratedLine: public JKQTPPlotObject, public JKQTPGraphDecoratedLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param color color of drawing
        \param lineWidth lineWidth of drawing
        \param headStyle style of the head decoration
        \param tailStyle style of the tail decoration
        \param style line style of drawing
        \param parent the parent plotter object
    */
    explicit JKQTPGeoBaseDecoratedLine(QColor color, double lineWidth, JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, Qt::PenStyle style=Qt::SolidLine, JKQTBasePlotter* parent=nullptr, DrawMode drawMode=DrawAsGraphicElement);
    /*! \brief class contructor */
    explicit JKQTPGeoBaseDecoratedLine(JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);



    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};


/*! \brief This JKQTPPlotObject is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseFilled: public JKQTPGeoBaseLine, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param fillStyle filling style of the graph
            \param lineWidth lineWidth of drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style=Qt::SolidLine, Qt::BrushStyle fillStyle=Qt::SolidPattern, JKQTBasePlotter* parent=nullptr, DrawMode drawMode=DrawAsGraphicElement);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param style line style of drawing
            \param lineWidth lineWidth of drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, Qt::PenStyle style, JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param lineWidth lineWidth of drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, double lineWidth, JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);
        /*! \brief class contructor

            \param color color of drawing
            \param fillColor color of the filling in the drawing
            \param parent the parent plotter object
         */
        JKQTPGeoBaseFilled(QColor color, QColor fillColor, JKQTBasePlotter *parent, DrawMode drawMode=DrawAsGraphicElement);

        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) to the same value */
        virtual void setAlpha(float alpha) override;
        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) */
        virtual void setAlpha(float alphaLine, float alphaFill);
        /** \brief set line and fill color */
        virtual void setColor(QColor c) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    protected:


};

#endif // jkqtpgeobase_H_INCLUDED
