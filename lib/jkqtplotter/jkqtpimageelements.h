/*
    Copyright (c) 2008-2018 Jan W. Krieger & Sebastian Isbaner (contour plot), German Cancer Research Center

    

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


#ifndef JKQTPIMAGEELEMENTS_H
#define JKQTPIMAGEELEMENTS_H




/**
 * \defgroup jkqtplotter_imagelots Image Plotting Elements
 * \ingroup jkqtplotter_elements

 */

/** \file jkqtpimageelements.h
  * \ingroup  jkqtplotter_imagelots
  */

#include <QString>
#include <QPainter>
#include <QImage>
#include <QIcon>
#include "jkqtplotter/jkqtpelements.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"



/*! \brief base class for plotting an image
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPImageBase: public JKQTPgraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPImageBase(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImageBase(double x, double y, double width, double height, JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImageBase(JKQtPlotter* parent);
        /** \brief class constructor */
        JKQTPImageBase(double x, double y, double width, double height, JKQtPlotter* parent);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(double, x)
        JKQTPGET_SET_MACRO(double, y)
        JKQTPGET_SET_MACRO(double, width)
        JKQTPGET_SET_MACRO(double, height)

    protected:

        /** \brief x coordinate of lower left corner */
        double x;
        /** \brief y coordinate of lower left corner */
        double y;
        /** \brief width of image */
        double width;
        /** \brief height of image */
        double height;

        /*! \brief plot the given QImage onto the widget where the QImage fills the area defined by x, y (lower left corner) and width, height

            in the simplest case your implementation of draw() will call
            <code>plotImage(painter, image, this->x, this->y, this->width, this->height);</code>
         */
        virtual void plotImage(JKQTPEnhancedPainter& painter, QImage& image, double x, double y, double width, double height);

};


/*! \brief base class to hold an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPMathImageBase: public JKQTPImageBase {
        Q_OBJECT
    public:
        /** \brief possible datatypes of the data array, plotted by this class. */
        enum DataType {
            FloatArray,
            DoubleArray,
            UInt8Array,
            UInt16Array,
            UInt32Array,
            UInt64Array,
            Int8Array,
            Int16Array,
            Int32Array,
            Int64Array
        };

        enum ModifierMode {
            ModifyNone=0,
            ModifyValue=1,
            ModifySaturation=2,
            ModifyAlpha=3
        };

        static ModifierMode StringToModifierMode(const QString& mode) {
            QString m=mode.toLower();
            if (m=="value" ) return ModifyValue;
            if (m=="saturation" ) return ModifySaturation;
            if (m=="alpha" ) return ModifyAlpha;

            return ModifyNone;
        }

        static QString ModifierModeToString(const ModifierMode& mode) {

            if (mode==ModifyValue) return "value";
            if (mode==ModifySaturation) return "saturation";
            if (mode==ModifyAlpha) return "alpha";
            return "none";
        }



        /** \brief class constructor */
        JKQTPMathImageBase(JKQtBasePlotter* parent=nullptr);
        JKQTPMathImageBase(double x, double y, double width, double height, JKQtBasePlotter* parent=nullptr);
        JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, uint32_t Nx, uint32_t Ny, JKQtBasePlotter* parent=nullptr);


        /** \brief class constructor */
        JKQTPMathImageBase(JKQtPlotter* parent);
        JKQTPMathImageBase(double x, double y, double width, double height, JKQtPlotter* parent=nullptr);
        JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        JKQTPGET_SET_MACRO(uint32_t, Nx)
        JKQTPGET_SET_MACRO(uint32_t, Ny)
        JKQTPGET_SET_MACRO(void*, data)
        JKQTPGET_SET_MACRO(DataType, datatype)
        JKQTPGET_SET_MACRO(void*, dataModifier)
        JKQTPGET_SET_MACRO(DataType, datatypeModifier)
        JKQTPGET_SET_MACRO(ModifierMode, modifierMode)

        virtual void set_dataModifier(void* data, DataType datatype);
        virtual void set_data(void* data, uint32_t Nx, uint32_t Ny, DataType datatype);
        virtual void set_data(void* data, uint32_t Nx, uint32_t Ny);
        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax);
        /** \brief determine min/max data value of the image */
        virtual void getModifierMinMax(double& imin, double& imax);

        QVector<double> getDataAsDoubleVector() const;
        QVector<double> getDataModifierAsDoubleVector() const;
    protected:
        /** \brief points to the data array, holding the image */
        void* data;
        /** \brief datatype of the data array data */
        DataType datatype;
        /** \brief width of the data array data in pixels */
        uint32_t Nx;
        /** \brief height of the data array data in pixels */
        uint32_t Ny;

        /** \brief points to the data array, holding the modifier image */
        void* dataModifier;
        /** \brief datatype of the data array data */
        DataType datatypeModifier;

        ModifierMode modifierMode;

        double internalDataMin;
        double internalDataMax;
        double internalModifierMin;
        double internalModifierMax;
        /** \brief overwrite this to fill the data poiters before they are accessed (e.g. to load data from a column in the datastore */
        virtual void ensureImageData();

        void modifyImage(QImage& img);
        void modifyImage(QImage& img, void* dataModifier, DataType datatypeModifier, uint32_t Nx, uint32_t Ny, double internalModifierMin, double internalModifierMax);


};



/*! \brief a QComboBox which shows  JKQTPMathImageBase::ModifierMode
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPImageModifierModeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPImageModifierModeComboBox(QWidget* parent=nullptr);

        JKQTPMathImageBase::ModifierMode getModifierMode() const;
        void setModifierMode(JKQTPMathImageBase::ModifierMode mode);
    protected:
        void addMode(JKQTPMathImageBase::ModifierMode mode, const QString& name, const QIcon& icon=QIcon());
};

/*! \brief class to plot an image from a QImage object
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPImage: public JKQTPImageBase {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPImage(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImage(JKQtPlotter* parent);
        /** \brief class constructor */
        JKQTPImage(double x, double y, double width, double height, QImage* image, JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImage(double x, double y, double width, double height, QImage* image, JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);



        JKQTPGET_SET_MACRO(QImage*, image)
    protected:
        /** \brief the image to be plotted */
        QImage* image;

        void createImageActions();

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
    public:
        virtual void setParent(JKQtBasePlotter* parent);
        virtual void set_title(const typedef_set_title& title);
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();
};





/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPMathImage: public JKQTPMathImageBase {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, uint32_t Nx, uint32_t Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, JKQtBasePlotter* parent=nullptr);
        JKQTPMathImage(JKQtBasePlotter* parent=nullptr);
        JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, uint32_t Nx, uint32_t Ny, JKQTPMathImageColorPalette palette, JKQtPlotter* parent);
        JKQTPMathImage(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        virtual void setParent(JKQtBasePlotter* parent);

        /*! \brief get list with all available palettes */
        static QStringList getPalettes() ;
        /*! \brief get list with all available palettes */
        static int getPalettesCount() ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteImage(int i, int width, int height=1) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteImage(JKQTPMathImageColorPalette palette, int width, int height=1) ;

        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(int i, int width, int height) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(JKQTPMathImageColorPalette palette, int width, int height) ;


        JKQTPGET_SET_MACRO(JKQTPMathImageColorPalette, palette)
        JKQTPGET_SET_MACRO(JKQTPMathImageColorRangeFailAction, rangeMinFailAction)
        JKQTPGET_SET_MACRO(JKQTPMathImageColorRangeFailAction, rangeMaxFailAction)
        JKQTPGET_SET_MACRO(QColor, rangeMinFailColor)
        JKQTPGET_SET_MACRO(QColor, rangeMaxFailColor)
        JKQTPGET_SET_MACRO(QColor, nanColor)
        JKQTPGET_SET_MACRO(QColor, infColor)
        JKQTPGET_SET_MACRO(bool, showColorBar)
        JKQTPGET_SET_MACRO(int, colorBarWidth)
        JKQTPGET_SET_MACRO(int, colorBarModifiedWidth)
        JKQTPGET_SET_MACRO(int, colorBarOffset)
        JKQTPGET_SET_MACRO(double, colorBarRelativeHeight)
        JKQTPGET_SET_MACRO(double, imageMin)
        JKQTPGET_SET_MACRO(double, imageMax)
        JKQTPGET_SET_MACRO(bool, autoImageRange)
        JKQTPGET_SET_MACRO(QString, imageName)
        JKQTPGET_SET_MACRO(QString, imageNameFontName)
        JKQTPGET_SET_MACRO(double, imageNameFontSize)
        JKQTPGET_MACRO(JKQTPverticalIndependentAxis*, colorBarRightAxis)
        JKQTPGET_MACRO(JKQTPhorizontalIndependentAxis*, colorBarTopAxis)
        JKQTPGET_MACRO(JKQTPverticalIndependentAxis*, modifierColorBarTopAxis)
        JKQTPGET_MACRO(JKQTPhorizontalIndependentAxis*, modifierColorBarRightAxis )
        JKQTPGET_SET_MACRO(bool, colorBarTopVisible)
        JKQTPGET_SET_MACRO(bool, colorBarRightVisible)
        JKQTPGET_SET_MACRO(bool, autoModifierRange)
        JKQTPGET_SET_MACRO(double, modifierMin)
        JKQTPGET_SET_MACRO(double, modifierMax)

        void set_palette(int pal);

        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);

        /*! \brief returns a QImage, which contains the plaette drawn outside the plot. \a steps is the number of data-setps (and the size of the output image) used for the palette image. */
        virtual QImage drawOutsidePalette(int steps=200);

        /*! \brief return the plotted image only as a QImage */
        virtual QImage drawImage();

        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax);

        /** \brief determine min/max data value of the image */
        virtual void getModifierMinMax(double& imin, double& imax);

        /** \brief return the value (raw data!) of the contained image at the given coordinate */
        double getValueAt(double x, double y);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

    protected:
        void initJKQTPMathImage();
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
        /** \brief width of the color bar when modifier is on */
        int colorBarModifiedWidth;
        /** \brief height of the color bar, as multiple of plotHeight */
        double colorBarRelativeHeight;
        /** \brief indicates whether to estimate min/max of the image automatically */
        bool autoImageRange;
        /** \brief image value range minimum */
        double imageMin;
        /** \brief image value range maximum */
        double imageMax;
        /** \brief indicates whether to estimate min/max of the modifier automatically */
        bool autoModifierRange;
        /** \brief modifier value range minimum */
        double modifierMin;
        /** \brief modifier value range maximum */
        double modifierMax;
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
        JKQTPverticalIndependentAxis* colorBarRightAxis;
        JKQTPhorizontalIndependentAxis* colorBarTopAxis;
        JKQTPverticalIndependentAxis* modifierColorBarTopAxis;
        JKQTPhorizontalIndependentAxis* modifierColorBarRightAxis;

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
        QAction* actSavePalette;
        QAction* actCopyPalette;
    public:
        virtual void set_title(const typedef_set_title& title);
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();
        void saveColorbarPlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyColorbarPlotAsImage();

};







/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPRGBMathImage: public JKQTPMathImageBase {
        Q_OBJECT
    public:



        /** \brief class constructor */
        JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, uint32_t Nx, uint32_t Ny, JKQtBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);

        /** \brief class constructor */
        JKQTPRGBMathImage(JKQtBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPRGBMathImage(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        virtual void setParent(JKQtBasePlotter* parent);



        virtual void set_data(void* data, void* dataG, void* dataB, uint32_t Nx, uint32_t Ny, DataType datatype);
        virtual void set_data(void* data, void* dataG, void* dataB,  uint32_t Nx, uint32_t Ny);

        virtual void set_data(void* data, uint32_t Nx, uint32_t Ny, DataType datatype);
        virtual void set_data(void* data,  uint32_t Nx, uint32_t Ny);


        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax);


        JKQTPGET_SET_MACRO(void*, dataG)
        JKQTPGET_SET_MACRO(DataType, datatypeG)
        JKQTPGET_SET_MACRO(void*, dataB)
        JKQTPGET_SET_MACRO(DataType, datatypeB)
        JKQTPGET_SET_MACRO(bool, showColorBar)
        JKQTPGET_SET_MACRO(int, colorBarWidth)
        JKQTPGET_SET_MACRO(int, colorBarOffset)
        JKQTPGET_SET_MACRO(double, colorBarRelativeHeight)
        JKQTPGET_SET_MACRO(double, imageMin)
        inline void set_imageMinR(double m) {
            set_imageMin(m);
        }
        JKQTPGET_SET_MACRO(double, imageMax)
        inline void set_imageMaxR(double m) {
            set_imageMax(m);
        }
        JKQTPGET_SET_MACRO(double, imageMinG)
        JKQTPGET_SET_MACRO(double, imageMaxG)
        JKQTPGET_SET_MACRO(double, imageMinB)
        JKQTPGET_SET_MACRO(double, imageMaxB)
        JKQTPGET_SET_MACRO(bool, autoImageRange)
        JKQTPGET_SET_MACRO(QString, imageName)
        inline void set_imageNameR(const QString& m) {
            set_imageName(m);
        }
        JKQTPGET_SET_MACRO(QString, imageNameG)
        JKQTPGET_SET_MACRO(QString, imageNameB)
        JKQTPGET_SET_MACRO(QString, imageNameFontName)
        JKQTPGET_SET_MACRO(double, imageNameFontSize)
        JKQTPGET_MACRO(JKQTPverticalIndependentAxis*, colorBarRightAxis)
        JKQTPGET_MACRO(JKQTPhorizontalIndependentAxis*, colorBarTopAxis)
        JKQTPGET_MACRO(JKQTPverticalIndependentAxis*, colorBarRightAxisG)
        JKQTPGET_MACRO(JKQTPhorizontalIndependentAxis*, colorBarTopAxisG)
        JKQTPGET_MACRO(JKQTPverticalIndependentAxis*, colorBarRightAxisB)
        JKQTPGET_MACRO(JKQTPhorizontalIndependentAxis*, colorBarTopAxisB)
        JKQTPGET_SET_MACRO(bool, colorBarTopVisible)
        JKQTPGET_SET_MACRO(bool, colorBarRightVisible)
        JKQTPGET_SET_MACRO(bool, colorbarsSideBySide)
        JKQTPGET_SET_MACRO(JKQTPRGBMathImageRGBMode, rgbMode)

        QVector<double> getDataGAsDoubleVector() const;
        QVector<double> getDataBAsDoubleVector() const;


        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);

        /*! \brief return the plotted image only as a QImage */
        virtual QImage drawImage();

        /** \brief determine min/max data value of the image */
        virtual void getDataMinMaxG(double& imin, double& imax);
        /** \brief determine min/max data value of the image */
        virtual void getDataMinMaxB(double& imin, double& imax);

        /** \brief return the value (raw data!) of the contained image at the given coordinate */
        double getValueAt(double x, double y, int channel=0) ;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

    protected:
        void initObject();
        /** \brief points to the data array, holding the image */
        void* dataG;
        /** \brief datatype of the data array data */
        DataType datatypeG;
        /** \brief points to the data array, holding the image */
        void* dataB;
        /** \brief datatype of the data array data */
        DataType datatypeB;

        JKQTPRGBMathImageRGBMode rgbMode;

        /** \brief top color bar visible */
        bool colorBarTopVisible;
        /** \brief right color bar visible */
        bool colorBarRightVisible;
        /** \brief name of the image displayed above color bar (may contain LaTeX markup!) */
        QString imageName;
        /** \brief name of the image displayed above color bar (may contain LaTeX markup!) */
        QString imageNameG;
        /** \brief name of the image displayed above color bar (may contain LaTeX markup!) */
        QString imageNameB;
        /** \brief font name when displaying imageName */
        QString imageNameFontName;
        /** \brief font size in points when displaying imageName */
        double imageNameFontSize;

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
        /** \brief image value range minimum */
        double imageMinG;
        /** \brief image value range maximum */
        double imageMaxG;
        /** \brief image value range minimum */
        double imageMinB;
        /** \brief image value range maximum */
        double imageMaxB;
        /** \brief offset between outside space border and color bar */
        int colorBarOffset;
        /** \brief if \c true the different color bars share the space of a single color bar, if set \c false each colorbar uses the space of a single colorbar */
        bool colorbarsSideBySide;


        /** \brief object used for color bar axes
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPverticalIndependentAxis* colorBarRightAxis;
        JKQTPhorizontalIndependentAxis* colorBarTopAxis;
        JKQTPverticalIndependentAxis* colorBarRightAxisG;
        JKQTPhorizontalIndependentAxis* colorBarTopAxisG;
        JKQTPverticalIndependentAxis* colorBarRightAxisB;
        JKQTPhorizontalIndependentAxis* colorBarTopAxisB;


        double internalDataMinG;
        double internalDataMaxG;
        double internalDataMinB;
        double internalDataMaxB;

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
    public:
        virtual void set_title(const typedef_set_title& title);
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();
};




/*! \brief class to plot an image from an 2-dimensional array of values stored in a column of the datastore
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPColumnMathImage: public JKQTPMathImage {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPColumnMathImage(JKQtBasePlotter* parent=nullptr);
        JKQTPColumnMathImage(double x, double y, double width, double height, uint32_t Nx, uint32_t Ny, JKQtBasePlotter* parent=nullptr);
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, uint32_t Nx, uint32_t Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPColumnMathImage(JKQtPlotter* parent);
        JKQTPColumnMathImage(double x, double y, double width, double height, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, uint32_t Nx, uint32_t Ny, JKQTPMathImageColorPalette palette, JKQtPlotter* parent);
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);

        JKQTPGET_SET_MACRO(int, imageColumn)
        JKQTPGET_SET_MACRO(int, modifierColumn)

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);



    protected:
        /** \brief  column containing the displayed image */
        int imageColumn;
        /** \brief  column containing the modifier image */
        int modifierColumn;

        virtual void ensureImageData();
};


/*! \brief like JKQTPRGBMathImage but reads images from columns of the datastore
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPColumnRGBMathImage: public JKQTPRGBMathImage {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPColumnRGBMathImage(JKQtBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, uint32_t Nx, uint32_t Ny, JKQtBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, uint32_t Nx, uint32_t Ny, JKQtBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, uint32_t Nx, uint32_t Ny, JKQtBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, uint32_t Nx, uint32_t Ny, JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPColumnRGBMathImage(JKQtPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, uint32_t Nx, uint32_t Ny, JKQtPlotter* parent);

        JKQTPGET_SET_MACRO(int, imageRColumn)
        JKQTPGET_SET_MACRO(int, imageGColumn)
        JKQTPGET_SET_MACRO(int, imageBColumn)
        JKQTPGET_SET_MACRO(int, modifierColumn)
        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);

    protected:
        /** \brief image column for R channel */
        int imageRColumn;
        /** \brief image column for G channel */
        int imageGColumn;
        /** \brief image column for B channel */
        int imageBColumn;
        /** \brief  column containing the modifier image */
        int modifierColumn;

        void ensureImageData();
};


/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPOverlayImage: public JKQTPImageBase {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPOverlayImage(double x, double y, double width, double height, bool* data, uint32_t Nx, uint32_t Ny, QColor colTrue, JKQtBasePlotter* parent=nullptr);
        JKQTPOverlayImage(JKQtBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPOverlayImage(double x, double y, double width, double height, bool* data, uint32_t Nx, uint32_t Ny, QColor colTrue, JKQtPlotter* parent);
        JKQTPOverlayImage(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /*! \brief return the plotted image only as a QImage */
        virtual QImage drawImage();

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor();

        JKQTPGET_SET_MACRO(QColor, trueColor)
        JKQTPGET_SET_MACRO(QColor, falseColor)
        JKQTPGET_SET_MACRO(uint32_t, Nx)
        JKQTPGET_SET_MACRO(uint32_t, Ny)
        JKQTPGET_SET_MACRO(bool*, data)

        void set_data(bool* data, uint32_t Nx, uint32_t Ny);

        QVector<double> getDataAsDoubleVector() const;

    protected:
        /** \brief points to the data array, holding the image */
        bool* data;
        /** \brief width of the data array data in pixels */
        uint32_t Nx;
        /** \brief height of the data array data in pixels */
        uint32_t Ny;

        /** \brief color for \c true pixels */
        QColor trueColor;
        /** \brief color for \c false pixels */
        QColor falseColor;

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
    public:
        virtual void set_title(const typedef_set_title& title);
        virtual void setParent(JKQtBasePlotter* parent);
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();

};



/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots

    In contrast to JKQTPOverlayImage this class draws ist contents as rectangles, not as semi-transparent image. This may lead to nicer results,but could be slower.
    Also it is possible to draw other types of markers (cross, circles, ...)

 */
class LIB_EXPORT JKQTPOverlayImageEnhanced: public JKQTPOverlayImage {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, uint32_t Nx, uint32_t Ny, QColor colTrue, JKQtBasePlotter* parent=nullptr);
        JKQTPOverlayImageEnhanced(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, uint32_t Nx, uint32_t Ny, QColor colTrue, JKQtPlotter* parent);
        JKQTPOverlayImageEnhanced(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect);

        JKQTPGET_SET_MACRO(JKQTPgraphSymbols, symbol)
        JKQTPGET_SET_MACRO(double, symbolWidth)
        JKQTPGET_SET_MACRO(bool, drawAsRectangles)
        JKQTPGET_SET_MACRO(bool, rectanglesAsImageOverlay)
        JKQTPGET_SET_MACRO(double, symbolSizeFactor)

    protected:
        /** \brief which symbol to use for the datapoints */
        JKQTPgraphSymbols symbol;
        /** \brief width (in pixels) of the lines used to plot the symbol for the data points */
        double symbolWidth;

        /** \brief indicates whether to draw filled rectangles (\c false, default) or symbols */
        bool drawAsRectangles;

        /** \brief a rescaling factor  for the symbols */
        double symbolSizeFactor;

        /** \brief when \c drawAsRectangles==true this reactivates the drawing from JKQTPOverlayImage, i.e. the overlay is not drawn as separated rectangles, but as an overlay image */
        bool rectanglesAsImageOverlay;

};




/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots

    In contrast to JKQTPOverlayImage this class draws ist contents as rectangles, not as semi-transparent image. This may lead to nicer results,but could be slower.
    Also it is possible to draw other types of markers (cross, circles, ...)

 */
class LIB_EXPORT JKQTPColumnOverlayImageEnhanced: public JKQTPOverlayImageEnhanced {
        Q_OBJECT
    public:
        JKQTPColumnOverlayImageEnhanced(JKQtBasePlotter* parent=nullptr);
        JKQTPColumnOverlayImageEnhanced(JKQtPlotter* parent);

        JKQTPGET_SET_MACRO(int, imageColumn)
        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c);


    protected:
        /** \brief top color bar visible */
        int imageColumn;
};



/*! \brief class for a contour plot
 *  \ingroup jkqtplotter_imagelots
 *  calculates the contour of a given image using the CONREC algorithm
 *  \link http://paulbourke.net/papers/conrec/
 *  The implementation for Qt is adapted from Qwt
 *  \link http://qwt.sourceforge.net/
 *
 *  The contour lines are calculated only once and then redrawn to save plotting time.
 *  Make sure you add a new graph when your data changes. The contour levels can be defined in
 *  contourLevels, each elemt in the list represents a contour plane. You can also create contour
 *  levels with createContourLevels(). The levels are linearly spaced between the maximum and minimum
 *  value in your data. For logarithmic data, use createContourLevelsLog() to create contour levels
 *  with logarithmic spacing.
 *
 * \author Sebastian Isbaner, 2013-2014
 */
class LIB_EXPORT JKQTPContour: public JKQTPMathImage {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPContour(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPContour(double x, double y, double width, double height, void* data, uint32_t Nx, uint32_t Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, DataType datatype = JKQTPMathImageBase::DoubleArray, JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPContour(JKQtPlotter* parent);
        /** \brief class constructor */
        JKQTPContour(double x, double y, double width, double height, void* data, uint32_t Nx, uint32_t Ny, JKQTPMathImageColorPalette palette, DataType datatype , JKQtPlotter* parent);
        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter);

        /** \brief creates at nLevels contour levels linearly spaced between the data's minimum and maximum values */
        void createContourLevels(int nLevels=3);
        /** \brief creates at least nLevels contour levels with logarithmic spacing. FIXME: Has not been tested yet */
        void createContourLevelsLog(int nLevels=3,int m=2);

        JKQTPGET_SET_MACRO(QColor, lineColor)
        JKQTPGET_SET_MACRO(Qt::PenStyle, style)
        JKQTPGET_SET_MACRO(double, lineWidth)
        JKQTPGET_SET_MACRO(bool, ignoreOnPlane)
        JKQTPGET_SET_MACRO(int, numberOfLevels)
        JKQTPGET_SET_MACRO(bool, colorFromPalette)
        JKQTPGET_SET_MACRO(QList<double>, contourLevels)
        JKQTPGET_SET_MACRO(bool, relativeLevels)
        /** convenience function to work with JKQTPdatastore */
        void set_imageColumn(size_t columnID);

        void addContourLevel(double &level);



    protected:
        /** \brief color of the contour lines */
        QColor lineColor;
        /** \brief linestyle of the contour lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief if true, vertices that all lie on the contour plane will be ignored*/
        bool ignoreOnPlane;
        /** \brief if true, the colors of the \a palette are used for the contour lines */
        bool colorFromPalette;
        /** \brief the number of contour levels. Is only used if contourLevels is empty*/
        int numberOfLevels;

        /** \brief the list of contour levels */
        QList<double> contourLevels;
//        /** \brief indicates wether the contour levels are absolute values or relative to the maximum (max-min) */
        bool relativeLevels;
        virtual void ensureImageData();

private:
        double value(int xIdx, int yIdx);
        /// gives the intersection line of a plane defined by three vertices with a contour level in the x-y plane of heigth level
        bool intersect(QLineF &line, const QVector3D &vertex1,const QVector3D &vertex2,const QVector3D &vertex3,const double &level);
        /**
         * @brief interpolatePoint linear interpolation of a line to the x-y plane using the z-value relative to level
         * @param point1 start point of the line
         * @param point2 end point of the line
         * @param level the heigth for the interpolation (this z-value is projected onto the xy-plane)
         * @return the interpolated point in the x-y plane
         */
        QPointF interpolatePoint(const QVector3D &point1, const QVector3D &point2,const double &level);
        /**
         * @brief compare2level checks if the vertex lies above, under or on the contour plane level
         * @param vertex
         * @param level contour plane
         * @return above (1), on the plane (0), below (-1);
         */
        int compare2level(const QVector3D &vertex, const double &level);
        /**
         * @brief calcContourLines calculates the contour lines using the CONREC algorithm
         */
        void calcContourLines(QList<QVector<QLineF > > &ContourLines);
        /// the contour lines as vector of single lines (one for each triangle). the list index refers to the contour level.
        QList<QVector<QLineF > > contourLines; // contour levels, squares on grid, line

};





#endif // JKQTPIMAGEELEMENTS_H

