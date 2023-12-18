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

#ifndef jkqtpgeobase_H_INCLUDED
#define jkqtpgeobase_H_INCLUDED


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"


/*! \brief This JKQTPGeometricPlotElement is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseLine: public JKQTPGeometricPlotElement, public JKQTPGraphLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param parent the parent plotter object
        \param drawMode how to draw te element (as geometric or graphic element)
    */
    explicit JKQTPGeoBaseLine(JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);

    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, double lineWidth);
    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, double lineWidth, Qt::PenStyle style);
    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, Qt::PenStyle style);




    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};




/*! \brief This JKQTPGeometricPlotElement is used as base class for geometric drawing
           elements that consist of lines with one decorated end (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseDecoratedHeadLine: public JKQTPGeometricPlotElement, public JKQTPGraphDecoratedHeadLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param headStyle style of the head decoration
        \param parent the parent plotter object
        \param drawMode how to draw te element (as geometric or graphic element)
    */
    explicit JKQTPGeoBaseDecoratedHeadLine(JKQTPLineDecoratorStyle headStyle, JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);

    /*! \brief class contructor

        \param parent the parent plotter object
        \param drawMode how to draw te element (as geometric or graphic element)
    */
    explicit JKQTPGeoBaseDecoratedHeadLine(JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);


    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, double lineWidth);
    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, double lineWidth, Qt::PenStyle style);
    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, Qt::PenStyle style);


    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};




/*! \brief This JKQTPGeometricPlotElement is used as base class for geometric drawing
           elements that consist of lines with decorated ends (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

*/
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseDecoratedLine: public JKQTPGeometricPlotElement, public JKQTPGraphDecoratedLineStyleMixin {
    Q_OBJECT
public:
    /*! \brief class contructor

        \param headStyle style of the head decoration
        \param tailStyle style of the tail decoration
        \param parent the parent plotter object
        \param drawMode how to draw te element (as geometric or graphic element)
    */
    explicit JKQTPGeoBaseDecoratedLine(JKQTPLineDecoratorStyle headStyle, JKQTPLineDecoratorStyle tailStyle, JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);
    /** \brief class contructor
     *
     *  \param parent the parent plotter object
     *  \param drawMode how to draw te element (as geometric or graphic element)
     */
    explicit JKQTPGeoBaseDecoratedLine(JKQTBasePlotter* parent, DrawMode drawMode=DrawAsGraphicElement);

    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, double lineWidth);
    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, double lineWidth, Qt::PenStyle style);
    /** \brief set several of the style properties with one call */
    virtual void setStyle(QColor color, Qt::PenStyle style);



    /** \brief sets the alpha-channel of the \a color (i.e. its transparency) */
    virtual void setAlpha(float alpha);
    /** \brief set line color */
    virtual void setColor(QColor c);

    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;

protected:

};


/*! \brief This JKQTPGeometricPlotElement is used as base class for geometric drawing
           elements that only consist of lines (i.e. no filling of any kind is done)
    \ingroup jkqtplotter_geoplots

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGeoBaseFilled: public JKQTPGeoBaseLine, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:
        /*! \brief class contructor

            \param parent the parent plotter object
            \param drawMode how to draw te element (as geometric or graphic element)
         */
        JKQTPGeoBaseFilled(JKQTBasePlotter *parent, DrawMode drawMode=DrawAsGraphicElement);

        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) to the same value */
        virtual void setAlpha(float alpha) override;
        /** \brief sets the alpha-channel of the \a color and \a fillColor (i.e. its transparency) */
        virtual void setAlpha(float alphaLine, float alphaFill);
        /** \brief set line and fill color */
        virtual void setColor(QColor c) override;
        /** \brief set several of the style properties with one call */
        virtual void setStyle(QColor color, double lineWidth, Qt::PenStyle style, QColor fillColor, Qt::BrushStyle fillStyle) ;
        /** \brief set several of the style properties with one call */
        virtual void setStyle(QColor color, double lineWidth, Qt::PenStyle style) override;
        /** \brief set several of the style properties with one call */
        virtual void setStyle(QColor color, Qt::PenStyle style) override;
        /** \brief set several of the style properties with one call */
        virtual void setStyle(QColor color, double lineWidth) override;
        /** \brief set several of the style properties with one call */
        void setStyle(QColor color, QColor fillColor);
        /** \brief set several of the style properties with one call, sets fill to transparent */
        void setStyleTransparentFill(QColor color, double lineWidth, Qt::PenStyle style);
        /** \brief set several of the style properties with one call, sets fill to transparent */
        void setStyleTransparentFill(QColor color, Qt::PenStyle style);
        /** \brief set several of the style properties with one call, sets fill to transparent */
        void setStyleTransparentFill(QColor color, double lineWidth);
        /** \brief set several of the style properties with one call, sets fill to transparent */
        void setStyleTransparentFill(QColor color);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
    protected:


};

#endif // jkqtpgeobase_H_INCLUDED
