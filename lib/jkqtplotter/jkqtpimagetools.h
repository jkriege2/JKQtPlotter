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
        static QImage getPaletteImage(int i, size_t width) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteImage(JKQTPMathImageColorPalette palette, size_t width) ;

        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(int i, size_t width, size_t height) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(JKQTPMathImageColorPalette palette, size_t width, size_t height) ;




        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void cbGetOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void cbDrawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);


        /** \brief set the parent class for internal objects (e.g. color bars) */
        virtual void cbSetParent(JKQTBasePlotter* parent);


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


        /** \brief object used for color bar axes at the right
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPVerticalIndependentAxis* colorBarRightAxis;
        /** \brief object used for color bar axes at the top
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPHorizontalIndependentAxis* colorBarTopAxis;



};



/*! \brief if a class is derived from this class, it may use color bars that have 2 axes (one "data"/color axis and one "modifier" axis)
    \ingroup jkqtplotter_imagelots_tools
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPColorPaletteWithModifierStyleAndToolsMixin : public JKQTPColorPaletteStyleAndToolsMixin {
        Q_GADGET
    public:



        JKQTPColorPaletteWithModifierStyleAndToolsMixin(JKQTBasePlotter *parent);
        virtual ~JKQTPColorPaletteWithModifierStyleAndToolsMixin()=default;

        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void cbGetOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) override;

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void cbDrawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) override;


        /** \brief set the parent class for internal objects (e.g. color bars) */
        virtual void cbSetParent(JKQTBasePlotter* parent) override;


        /** \brief determine min/max data value of the modifier image */
        virtual void cbGetModifierDataMinMax(double& imin, double& imax)=0;


        /*! \copydoc modifierMode */
        void setModifierMode(const JKQTPMathImageModifierMode & __value);
        /*! \copydoc modifierMode */
        JKQTPMathImageModifierMode getModifierMode() const;

        /*! \copydoc colorBarModifiedWidth */
        void setColorBarModifiedWidth(double __value);
        /*! \copydoc colorBarModifiedWidth */
        double getColorBarModifiedWidth() const;
        /*! \copydoc modifierColorBarTopAxis */
        JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis();
        /*! \copydoc modifierColorBarRightAxis */
        JKQTPHorizontalIndependentAxis* getModifierColorBarRightAxis();
        /*! \copydoc modifierColorBarTopAxis */
        const JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis() const;
        /*! \copydoc modifierColorBarRightAxis */
        const JKQTPHorizontalIndependentAxis *getModifierColorBarRightAxis()  const;
        /*! \copydoc autoModifierRange */
        void setAutoModifierRange(bool __value);
        /*! \copydoc autoModifierRange */
        bool getAutoModifierRange() const;
        /*! \copydoc modifierMin */
        void setModifierMin(double __value);
        /*! \copydoc modifierMin */
        double getModifierMin() const;
        /*! \copydoc modifierMax */
        void setModifierMax(double __value);
        /*! \copydoc modifierMax */
        double getModifierMax() const;


        /** \brief modify the given image \a img, using  modifier image \a dataModifier (of type \a datatypeModifier and size \a Nx * \a Ny), using values in the range \a internalModifierMin ... \a internalModifierMax
         */
        void modifyImage(QImage& img, void* dataModifier, JKQTPMathImageDataType datatypeModifier, int Nx, int Ny, double internalModifierMin, double internalModifierMax);

    protected:

        /** \brief indicates whether to estimate min/max of the modifier automatically */
        bool autoModifierRange;
        /** \brief modifier value range minimum */
        double modifierMin;
        /** \brief modifier value range maximum */
        double modifierMax;
        /** \brief width of the color bar when modifier is on */
        double colorBarModifiedWidth;
        /** \brief how to apply the modifier column dataModifier
         *  \see ModifierMode
         */
        JKQTPMathImageModifierMode modifierMode;


        /** \brief object used for color bar axes (right border, modifier image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPVerticalIndependentAxis* modifierColorBarTopAxis;
        /** \brief object used for color bar axes (top border, modifier image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPHorizontalIndependentAxis* modifierColorBarRightAxis;
};




#endif // JKQTPIMAGETOOLS_H
