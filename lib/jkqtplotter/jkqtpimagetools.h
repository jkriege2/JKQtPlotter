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


#ifndef JKQTPIMAGETOOLS_H
#define JKQTPIMAGETOOLS_H
#include <QIcon>
#include <QDebug>
#include <QImage>
#include <QStringList>
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpbasicimagetools.h"
#include <cmath>
#include <cfloat>
#include <stdint.h>
#include <QColor>
#include "jkqtplotter/jkqtptools.h"

class JKQTPVerticalIndependentAxis; // forward
class JKQTPHorizontalIndependentAxis; // forward
class JKQTBasePlotter; // forward

/*! \brief if a class is derived from this class, it may use color bars
    \ingroup jkqtplotter_imagelots_tools
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPColorPaletteStyleAndToolsMixin {
        Q_GADGET
    public:

        JKQTPColorPaletteStyleAndToolsMixin(JKQTBasePlotter *parent);
        virtual ~JKQTPColorPaletteStyleAndToolsMixin();

        /*! \brief get list with all available palettes */
        static QStringList getPalettes() ;
        /*! \brief get list with all available palettes */
        static int getPalettesCount() ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteImage(int i, int width) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteImage(JKQTPMathImageColorPalette palette, int width) ;

        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(int i, int width, int height) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(JKQTPMathImageColorPalette palette, int width, int height) ;




        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        void cbGetOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        void cbDrawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);


        /** \brief set the parent class for internal objects (e.g. color bars) */
        void cbSetParent(JKQTBasePlotter* parent);


        /** \brief determine min/max data value of the image */
        virtual void cbGetDataMinMax(double& imin, double& imax)=0;




        /*! \copydoc palette */
        void setPalette(int pal);
        /*! \copydoc palette */
        void setPalette(const JKQTPMathImageColorPalette & __value);
        /*! \copydoc palette */
        JKQTPMathImageColorPalette getPalette() const;
        /*! \copydoc rangeMinFailAction */
        void setRangeMinFailAction(const JKQTPMathImageColorRangeFailAction & __value);
        /*! \copydoc rangeMinFailAction */
        JKQTPMathImageColorRangeFailAction getActionRangeMinFail() const;
        /*! \copydoc rangeMaxFailAction */
        void setRangeMaxFailAction(const JKQTPMathImageColorRangeFailAction & __value);
        /*! \copydoc rangeMaxFailAction */
        JKQTPMathImageColorRangeFailAction getActionRangeMaxFail() const;
        /*! \copydoc rangeMinFailColor */
        void setRangeMinFailColor(const QColor & __value);
        /*! \copydoc rangeMinFailColor */
        QColor getRangeMinFailColor() const;
        /*! \copydoc rangeMaxFailColor */
        void setRangeMaxFailColor(const QColor & __value);
        /*! \copydoc rangeMaxFailColor */
        QColor getRangeMaxFailColor() const;
        /*! \copydoc nanColor */
        void setNanColor(const QColor & __value);
        /*! \copydoc nanColor */
        QColor getNanColor() const;
        /*! \copydoc infColor */
        void setInfColor(const QColor & __value);
        /*! \copydoc infColor */
        QColor getInfColor() const;
        /*! \copydoc showColorBar */
        void setShowColorBar(bool __value);
        /*! \copydoc showColorBar */
        bool getShowColorBar() const;
        /*! \copydoc colorBarWidth */
        void setColorBarWidth(int __value);
        /*! \copydoc colorBarWidth */
        int getColorBarWidth() const;
        /*! \copydoc colorBarOffset */
        void setColorBarOffset(int __value);
        /*! \copydoc colorBarOffset */
        int getColorBarOffset() const;
        /*! \copydoc colorBarRelativeHeight */
        void setColorBarRelativeHeight(double __value);
        /*! \copydoc colorBarRelativeHeight */
        double getColorBarRelativeHeight() const;
        /*! \copydoc imageMin */
        void setImageMin(double __value);
        /*! \copydoc imageMin */
        double getImageMin() const;
        /*! \copydoc imageMax */
        void setImageMax(double __value);
        /*! \copydoc imageMax */
        double getImageMax() const;
        /*! \copydoc autoImageRange */
        void setAutoImageRange(bool __value);
        /*! \copydoc autoImageRange */
        bool getAutoImageRange() const;
        /*! \copydoc imageName */
        void setImageName(const QString & __value);
        /*! \copydoc imageName */
        QString getImageName() const;
        /*! \copydoc imageNameFontName */
        void setImageNameFontName(const QString & __value);
        /*! \copydoc imageNameFontName */
        QString getImageNameFontName() const;
        /*! \copydoc imageNameFontSize */
        void setImageNameFontSize(double __value);
        /*! \copydoc imageNameFontSize */
        double getImageNameFontSize() const;
        /*! \copydoc colorBarRightAxis */
        JKQTPVerticalIndependentAxis* getColorBarRightAxis();
        /*! \copydoc colorBarTopAxis */
        JKQTPHorizontalIndependentAxis* getColorBarTopAxis();
        /*! \copydoc colorBarRightAxis */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxis() const;
        /*! \copydoc colorBarTopAxis */
        const JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const;
        /*! \copydoc colorBarTopVisible */
        void setColorBarTopVisible(bool __value);
        /*! \copydoc colorBarTopVisible */
        bool getColorBarTopVisible() const;
        /*! \copydoc colorBarRightVisible */
        void setColorBarRightVisible(bool __value);
        /*! \copydoc colorBarRightVisible */
        bool getColorBarRightVisible() const;


    protected:
        JKQTBasePlotter* cbParent;


        /** \brief top color bar visible */
        bool colorBarTopVisible;
        /** \brief right color bar visible */
        bool colorBarRightVisible;
        /** \brief name of the image displayed above color bar (may contain LaTeX markup!) */
        QString imageName;
        /** \brief font name when displaying imageName */
        QString imageNameFontName;
        /** \brief font size in points when displaying imageName */
        double imageNameFontSize;
        /** \brief palette for plotting an image */
        JKQTPMathImageColorPalette palette;
        /** \brief indicate whether to display a color bar */
        bool showColorBar;
        /** \brief width of the color bar */
        int colorBarWidth;
        /** \brief height of the color bar, as multiple of plotHeight */
        double colorBarRelativeHeight;
        /** \brief indicates whether to estimate min/max of the image automatically */
        bool autoImageRange;
        /** \brief image value range minimum */
        double imageMin;
        /** \brief image value range maximum */
        double imageMax;
        /** \brief offset between outside space border and color bar */
        int colorBarOffset;
        /** \brief which action to take if a color is below \a imageMin and \a autoImageRange \c ==false */
        JKQTPMathImageColorRangeFailAction rangeMinFailAction;
        /** \brief which action to take if a color is above \a imageMax and \a autoImageRange \c ==false */
        JKQTPMathImageColorRangeFailAction rangeMaxFailAction;
        /** \brief color to use for some settings of \a rangeMinFailAction */
        QColor rangeMinFailColor;
        /** \brief color to use for some settings of \a rangeMaxFailAction */
        QColor rangeMaxFailColor;
        /** \brief color to use for a not-a-number value */
        QColor nanColor;
        /** \brief color to use for an infinity value */
        QColor infColor;


        /** \brief object used for color bar axes
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPVerticalIndependentAxis* colorBarRightAxis;
        JKQTPHorizontalIndependentAxis* colorBarTopAxis;



};


#endif // JKQTPIMAGETOOLS_H
