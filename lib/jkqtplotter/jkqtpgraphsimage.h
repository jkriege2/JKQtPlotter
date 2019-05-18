/*
    Copyright (c) 2008-2019 Jan W. Krieger 

    

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


#ifndef jkqtpgraphsimage_H
#define jkqtpgraphsimage_H


#include <QString>
#include <QPainter>
#include <QImage>
#include <QIcon>
#include "jkqtplotter/jkqtpgraphsscatter.h"
#include "jkqtcommon/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"



/*! \brief base class for plotting an image
    \ingroup jkqtplotter_imagelots_elements
 */
class JKQTP_LIB_EXPORT JKQTPImageBase: public JKQTPGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPImageBase(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImageBase(double x, double y, double width, double height, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImageBase(JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPImageBase(double x, double y, double width, double height, JKQTPlotter* parent);
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /*! \copydoc x
            \see see x for details */ 
        void setX(double __value);
        /*! \copydoc x
            \see see x for details */ 
        double getX() const;
        /*! \copydoc y
            \see see y for details */ 
        void setY(double __value);
        /*! \copydoc y
            \see see y for details */ 
        double getY() const;
        /*! \copydoc width
            \see see width for details */ 
        void setWidth(double __value);
        /*! \copydoc width
            \see see width for details */ 
        double getWidth() const;
        /*! \copydoc height
            \see see height for details */ 
        void setHeight(double __value);
        /*! \copydoc height
            \see see height for details */ 
        double getHeight() const;

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
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_imageplot.png

 */
class JKQTP_LIB_EXPORT JKQTPMathImageBase: public JKQTPImageBase {
        Q_OBJECT
    public:
        /** \brief possible datatypes of the data array, plotted by this class. */
        enum DataType {
            FloatArray, /*!< Data is of type \c float */
            DoubleArray, /*!< Data is of type \c double */
            UInt8Array, /*!< Data is of type \c uint8_t */
            UInt16Array, /*!< Data is of type \c uint16_t */
            UInt32Array, /*!< Data is of type \c uint32_t */
            UInt64Array, /*!< Data is of type \c uint8_t */
            Int8Array, /*!< Data is of type \c int8_t */
            Int16Array, /*!< Data is of type \c int16_t */
            Int32Array, /*!< Data is of type \c int32_t */
            Int64Array /*!< Data is of type \c int64_t */
        };

        /** \brief describes how to modify a rendered image with a second data array \see ModifierModeToString(), StringToModifierMode(), JKQTPImageModifierModeComboBox */
        enum ModifierMode {
            ModifyNone=0,  /*!< no modification \image html JKQTPMathImageBaseModifyNone.png */
            ModifyValue=1,  /*!< modify the VALUE-channel from the HSV color space \image html JKQTPMathImageBaseModifyValue.png */
            ModifySaturation=2,/*!< modify the SATURATION-channel from the HSV color space \image html JKQTPMathImageBaseModifySaturation.png */
            ModifyAlpha=3,/*!< modify the ALPHA/TRANSPARENCY-channel from the RGBA color space \image html JKQTPMathImageBaseModifyAlpha.png */
            ModifyTransparency=ModifyAlpha,/*!< \see ModifyAlpha */
            ModifyLuminance=4,/*!< modify the LUMINANCE-channel from the HSL color space \image html JKQTPMathImageBaseModifyLuminance.png */
            ModifyHue=5,/*!< modify the VALUE-channel from the HSV color space \image html JKQTPMathImageBaseModifyHue.png */
        };

        /** \brief convert a ModifierMode to a string \see ModifierModeToString(), ModifierMode */
        static ModifierMode StringToModifierMode(const QString& mode);

        /** \brief convert a string to a ModifierMode \see StringToModifierMode(), ModifierMode */
        static QString ModifierModeToString(const ModifierMode& mode);



        /** \brief class constructor */
        JKQTPMathImageBase(JKQTBasePlotter* parent=nullptr);
        JKQTPMathImageBase(double x, double y, double width, double height, JKQTBasePlotter* parent=nullptr);
        JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);


        /** \brief class constructor */
        JKQTPMathImageBase(JKQTPlotter* parent);
        JKQTPMathImageBase(double x, double y, double width, double height, JKQTPlotter* parent=nullptr);
        JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPlotter* parent);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

        /*! \copydoc Nx
            \see see Nx for details */ 
        void setNx(int __value);
        /*! \copydoc Nx
            \see see Nx for details */ 
        int getNx() const;
        /*! \copydoc Ny
            \see see Ny for details */ 
        void setNy(int __value);
        /*! \copydoc Ny
            \see see Ny for details */ 
        int getNy() const;
        /*! \copydoc data
            \see see data for details */ 
        virtual void setData(void* __value);
        /*! \copydoc data
            \see see data for details */ 
        virtual void *getData() const;
        /*! \copydoc datatype
            \see see datatype for details */ 
        virtual void setDatatype(DataType __value);
        /*! \copydoc datatype
            \see see datatype for details */ 
        virtual DataType getDatatype() const;
        /*! \copydoc dataModifier
            \see see dataModifier for details */ 
        virtual void setDataModifier(void* __value);
        /*! \copydoc dataModifier
            \see see dataModifier for details */ 
        virtual void *getDataModifier() const;
        /*! \copydoc datatypeModifier
            \see see datatypeModifier for details */ 
        virtual void setDatatypeModifier(DataType __value);
        /*! \copydoc datatypeModifier
            \see see datatypeModifier for details */ 
        virtual DataType getDatatypeModifier() const;
        /*! \copydoc modifierMode
            \see see modifierMode for details */ 
        void setModifierMode(const ModifierMode & __value);
        /*! \copydoc modifierMode
            \see see modifierMode for details */ 
        ModifierMode getModifierMode() const;

        virtual void setDataModifier(void* data, DataType datatype);
        virtual void setData(void* data, int Nx, int Ny, DataType datatype);
        virtual void setData(void* data, int Nx, int Ny);
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
        /** \brief width of the data array data in pt */
        int Nx;
        /** \brief height of the data array data in pt */
        int Ny;

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
        void modifyImage(QImage& img, void* dataModifier, DataType datatypeModifier, int Nx, int Ny, double internalModifierMin, double internalModifierMax);


};



/*! \brief class to plot an image from a QImage object
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_rgbimageplot_qt.png
 */
class JKQTP_LIB_EXPORT JKQTPImage: public JKQTPImageBase {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPImage(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPImage(JKQTPlotter* parent);
        /** \brief class constructor, which points to an external image (not owned by this object!!!) */
        JKQTPImage(double x, double y, double width, double height, QImage* image, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor, which points to an external image (not owned by this object!!!) */
        JKQTPImage(double x, double y, double width, double height, QImage* image, JKQTPlotter* parent);
        /** \brief class constructor, which generates an internal image object, by copying \a image */
        JKQTPImage(double x, double y, double width, double height, const QImage& image, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor, which generates an internal image object, by copying \a image */
        JKQTPImage(double x, double y, double width, double height, const QImage& image, JKQTPlotter* parent);

        virtual ~JKQTPImage() override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

        /** \brief copy an external image into an internally owned copy */
        virtual void setImage(const QImage& image);

        /** \brief set an external image to be plotted, the image will NOT BE OWNED by the graph-object */
        virtual void setImage(QImage* image);

        /** \brief deletes the internal image */
        void clear_image();

        /*! \brief returns the property image ( \copybrief image ). \details Description of the parameter image is:  <BLOCKQUOTE>\copydoc image </BLOCKQUOTE>. \see image for more information */ 
        inline QImage* getImage() const { return this->image; }
    protected:
        /** \brief the image to be plotted. This is freed by the destructor, iff \a image_owned is set to \c true (.e.g by QImage-copy-constructors) */
        QImage* image;
        /** \brief indicates that the image \a image is owned by this object (i.e. freed, when the object is destroyed) */
        bool image_owned;

        void createImageActions();

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
    public:
        virtual void setParent(JKQTBasePlotter* parent) override;
        virtual void setTitle(const QString& title) override;
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();
};





/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_imageplot.png
    \image html jkqtplotter_simpletest_imageplot_modifier.png
    \image html jkqtplotter_simpletest_imageplot__smallscaletransparent.png
 */
class JKQTP_LIB_EXPORT JKQTPMathImage: public JKQTPMathImageBase {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, JKQTBasePlotter* parent=nullptr);
        JKQTPMathImage(JKQTBasePlotter* parent=nullptr);
        JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTPlotter* parent);
        JKQTPMathImage(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        virtual void setParent(JKQTBasePlotter* parent) override;

        /*! \brief get list with all available palettes */
        static QStringList getPalettes() ;
        /*! \brief get list with all available palettes */
        static int getPalettesCount() ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QImage representing the given palette */
        static QImage getPaletteImage(int i, int width, int height=1) ;
        /*! \brief get QImage representing the given palette */
        static QImage getPaletteImage(JKQTPMathImageColorPalette palette, int width, int height=1) ;

        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QImage representing the given palette */
        static QImage getPaletteKeyImage(int i, int width, int height) ;
        /*! \brief get QImage representing the given palette */
        static QImage getPaletteKeyImage(JKQTPMathImageColorPalette palette, int width, int height) ;


        /*! \copydoc palette
            \see see palette for details */ 
        void setPalette(const JKQTPMathImageColorPalette & __value);
        /*! \copydoc palette
            \see see palette for details */ 
        JKQTPMathImageColorPalette getPalette() const;
        /*! \copydoc rangeMinFailAction
            \see see rangeMinFailAction for details */ 
        void setRangeMinFailAction(const JKQTPMathImageColorRangeFailAction & __value);
        /*! \copydoc rangeMinFailAction
            \see see rangeMinFailAction for details */ 
        JKQTPMathImageColorRangeFailAction getActionRangeMinFail() const;
        /*! \copydoc rangeMaxFailAction
            \see see rangeMaxFailAction for details */ 
        void setRangeMaxFailAction(const JKQTPMathImageColorRangeFailAction & __value);
        /*! \copydoc rangeMaxFailAction
            \see see rangeMaxFailAction for details */ 
        JKQTPMathImageColorRangeFailAction getActionRangeMaxFail() const;
        /*! \copydoc rangeMinFailColor
            \see see rangeMinFailColor for details */ 
        void setRangeMinFailColor(const QColor & __value);
        /*! \copydoc rangeMinFailColor
            \see see rangeMinFailColor for details */ 
        QColor getRangeMinFailColor() const;
        /*! \copydoc rangeMaxFailColor
            \see see rangeMaxFailColor for details */ 
        void setRangeMaxFailColor(const QColor & __value);
        /*! \copydoc rangeMaxFailColor
            \see see rangeMaxFailColor for details */ 
        QColor getRangeMaxFailColor() const;
        /*! \copydoc nanColor
            \see see nanColor for details */ 
        void setNanColor(const QColor & __value);
        /*! \copydoc nanColor
            \see see nanColor for details */ 
        QColor getNanColor() const;
        /*! \copydoc infColor
            \see see infColor for details */ 
        void setInfColor(const QColor & __value);
        /*! \copydoc infColor
            \see see infColor for details */ 
        QColor getInfColor() const;
        /*! \copydoc showColorBar
            \see see showColorBar for details */ 
        void setShowColorBar(bool __value);
        /*! \copydoc showColorBar
            \see see showColorBar for details */ 
        bool getShowColorBar() const;
        /*! \copydoc colorBarWidth
            \see see colorBarWidth for details */ 
        void setColorBarWidth(double __value);
        /*! \copydoc colorBarWidth
            \see see colorBarWidth for details */ 
        double getColorBarWidth() const;
        /*! \copydoc colorBarModifiedWidth
            \see see colorBarModifiedWidth for details */ 
        void setColorBarModifiedWidth(double __value);
        /*! \copydoc colorBarModifiedWidth
            \see see colorBarModifiedWidth for details */ 
        double getColorBarModifiedWidth() const;
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        void setColorBarOffset(double __value);
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        double getColorBarOffset() const;
        /*! \copydoc colorBarRelativeHeight
            \see see colorBarRelativeHeight for details */ 
        void setColorBarRelativeHeight(double __value);
        /*! \copydoc colorBarRelativeHeight
            \see see colorBarRelativeHeight for details */ 
        double getColorBarRelativeHeight() const;
        /*! \copydoc imageMin
            \see see imageMin for details */ 
        void setImageMin(double __value);
        /*! \copydoc imageMin
            \see see imageMin for details */ 
        double getImageMin() const;
        /*! \copydoc imageMax
            \see see imageMax for details */ 
        void setImageMax(double __value);
        /*! \copydoc imageMax
            \see see imageMax for details */ 
        double getImageMax() const;
        /*! \copydoc autoImageRange
            \see see autoImageRange for details */ 
        void setAutoImageRange(bool __value);
        /*! \copydoc autoImageRange
            \see see autoImageRange for details */ 
        bool getAutoImageRange() const;
        /*! \copydoc imageName
            \see see imageName for details */ 
        void setImageName(const QString & __value);
        /*! \copydoc imageName
            \see see imageName for details */ 
        QString getImageName() const;
        /*! \copydoc imageNameFontSize
            \see see imageNameFontSize for details */ 
        void setImageNameFontSize(double __value);
        /*! \copydoc imageNameFontSize
            \see see imageNameFontSize for details */ 
        double getImageNameFontSize() const;
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */ 
        JKQTPVerticalIndependentAxis* getColorBarRightAxis();
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */ 
        JKQTPHorizontalIndependentAxis* getColorBarTopAxis();
        /*! \brief returns the property modifierColorBarTopAxis ( \copybrief modifierColorBarTopAxis ). \details Description of the parameter modifierColorBarTopAxis is:  <BLOCKQUOTE>\copydoc modifierColorBarTopAxis </BLOCKQUOTE>. \see modifierColorBarTopAxis for more information */ 
        JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis();
        /*! \brief returns the property modifierColorBarRightAxis ( \copybrief modifierColorBarRightAxis ). \details Description of the parameter modifierColorBarRightAxis  is:  <BLOCKQUOTE>\copydoc modifierColorBarRightAxis  </BLOCKQUOTE>. \see modifierColorBarRightAxis  for more information */ 
        JKQTPHorizontalIndependentAxis* getModifierColorBarRightAxis();
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxis() const;
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */
        const JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const;
        /*! \brief returns the property modifierColorBarTopAxis ( \copybrief modifierColorBarTopAxis ). \details Description of the parameter modifierColorBarTopAxis is:  <BLOCKQUOTE>\copydoc modifierColorBarTopAxis </BLOCKQUOTE>. \see modifierColorBarTopAxis for more information */
        const JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis() const;
        /*! \brief returns the property modifierColorBarRightAxis ( \copybrief modifierColorBarRightAxis ). \details Description of the parameter modifierColorBarRightAxis  is:  <BLOCKQUOTE>\copydoc modifierColorBarRightAxis  </BLOCKQUOTE>. \see modifierColorBarRightAxis  for more information */
        const JKQTPHorizontalIndependentAxis *getModifierColorBarRightAxis()  const;
        /*! \copydoc colorBarTopVisible
            \see see colorBarTopVisible for details */ 
        void setColorBarTopVisible(bool __value);
        /*! \copydoc colorBarTopVisible
            \see see colorBarTopVisible for details */ 
        bool getColorBarTopVisible() const;
        /*! \copydoc colorBarRightVisible
            \see see colorBarRightVisible for details */ 
        void setColorBarRightVisible(bool __value);
        /*! \copydoc colorBarRightVisible
            \see see colorBarRightVisible for details */ 
        bool getColorBarRightVisible() const;
        /*! \copydoc autoModifierRange
            \see see autoModifierRange for details */ 
        void setAutoModifierRange(bool __value);
        /*! \copydoc autoModifierRange
            \see see autoModifierRange for details */ 
        bool getAutoModifierRange() const;
        /*! \copydoc modifierMin
            \see see modifierMin for details */ 
        void setModifierMin(double __value);
        /*! \copydoc modifierMin
            \see see modifierMin for details */ 
        double getModifierMin() const;
        /*! \copydoc modifierMax
            \see see modifierMax for details */ 
        void setModifierMax(double __value);
        /*! \copydoc modifierMax
            \see see modifierMax for details */ 
        double getModifierMax() const;

        void setPalette(int pal);

        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) override;

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) override;

        /*! \brief returns a QImage, which contains the plaette drawn outside the plot. \a steps is the number of data-setps (and the size of the output image) used for the palette image. */
        virtual QImage drawOutsidePalette(uint8_t steps=200);

        /*! \brief return the plotted image only as a QImage */
        virtual QImage drawImage();

        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax) override;

        /** \brief determine min/max data value of the image */
        virtual void getModifierMinMax(double& imin, double& imax) override;

        /** \brief return the value (raw data!) of the contained image at the given coordinate */
        double getValueAt(double x, double y);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

    protected:
        void initJKQTPMathImage();
        /** \brief top color bar visible */
        bool colorBarTopVisible;
        /** \brief right color bar visible */
        bool colorBarRightVisible;
        /** \brief name of the image displayed above color bar (may contain LaTeX markup!) */
        QString imageName;
        /** \brief font size in points when displaying imageName */
        double imageNameFontSize;
        /** \brief palette for plotting an image */
        JKQTPMathImageColorPalette palette;
        /** \brief indicate whether to display a color bar */
        bool showColorBar;
        /** \brief width of the color bar [pt] */
        double colorBarWidth;
        /** \brief width of the color bar when modifier is on */
        double colorBarModifiedWidth;
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
        /** \brief offset between outside space border and color bar [pt] */
        double colorBarOffset;
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
        JKQTPVerticalIndependentAxis* modifierColorBarTopAxis;
        JKQTPHorizontalIndependentAxis* modifierColorBarRightAxis;

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
        QAction* actSavePalette;
        QAction* actCopyPalette;
    public:
        virtual void setTitle(const QString& title) override;
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();
        void saveColorbarPlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyColorbarPlotAsImage();

};








/*! \brief class to plot an image from an 2-dimensional array of values stored in a column of the datastore
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_imageplot.png
    \image html jkqtplotter_simpletest_imageplot_modifier.png
    \image html jkqtplotter_simpletest_imageplot__smallscaletransparent.png

 */
class JKQTP_LIB_EXPORT JKQTPColumnMathImage: public JKQTPMathImage {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPColumnMathImage(JKQTBasePlotter* parent=nullptr);
        JKQTPColumnMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, int Nx, int Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPColumnMathImage(JKQTPlotter* parent);
        JKQTPColumnMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTPlotter* parent);
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTPlotter* parent);
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, int Nx, int Ny, JKQTPlotter* parent);

        /*! \copydoc imageColumn
            \see see imageColumn for details */ 
        virtual void setImageColumn(int __value);
        /*! \copydoc imageColumn
            \see see imageColumn for details */ 
        int getImageColumn() const;
        /*! \copydoc modifierColumn
            \see see modifierColumn for details */ 
        virtual void setModifierColumn(int __value);
        /*! \copydoc modifierColumn
            \see see modifierColumn for details */ 
        int getModifierColumn() const;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;



    protected:
        /** \brief  column containing the displayed image */
        int imageColumn;
        /** \brief  column containing the modifier image */
        int modifierColumn;

        virtual void ensureImageData() override;
};



#endif // jkqtpgraphsimage_H

