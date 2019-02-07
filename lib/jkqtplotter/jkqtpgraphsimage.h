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
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplottertools/jkqtp_imexport.h"
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
        virtual QColor getKeyLabelColor() override;

        /*! \copydoc x
            \see see x for details */ 
        inline virtual void setX(double __value)
        {
            this->x = __value;
        } 
        /*! \copydoc x
            \see see x for details */ 
        inline virtual double getX() const  
        {
            return this->x; 
        }
        /*! \copydoc y
            \see see y for details */ 
        inline virtual void setY(double __value)
        {
            this->y = __value;
        } 
        /*! \copydoc y
            \see see y for details */ 
        inline virtual double getY() const  
        {
            return this->y; 
        }
        /*! \copydoc width
            \see see width for details */ 
        inline virtual void setWidth(double __value)
        {
            this->width = __value;
        } 
        /*! \copydoc width
            \see see width for details */ 
        inline virtual double getWidth() const  
        {
            return this->width; 
        }
        /*! \copydoc height
            \see see height for details */ 
        inline virtual void setHeight(double __value)
        {
            this->height = __value;
        } 
        /*! \copydoc height
            \see see height for details */ 
        inline virtual double getHeight() const  
        {
            return this->height; 
        }

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
        inline virtual void setNx(int __value)
        {
            this->Nx = __value;
        } 
        /*! \copydoc Nx
            \see see Nx for details */ 
        inline virtual int getNx() const
        {
            return this->Nx; 
        }
        /*! \copydoc Ny
            \see see Ny for details */ 
        inline virtual void setNy(int __value)
        {
            this->Ny = __value;
        } 
        /*! \copydoc Ny
            \see see Ny for details */ 
        inline virtual int getNy() const
        {
            return this->Ny; 
        }
        /*! \copydoc data
            \see see data for details */ 
        inline virtual void setData(void* __value)  
        {
            this->data = __value;
        } 
        /*! \copydoc data
            \see see data for details */ 
        inline virtual void* getData() const  
        {
            return this->data; 
        }
        /*! \copydoc datatype
            \see see datatype for details */ 
        inline virtual void setDatatype(const DataType & __value)  
        {
            this->datatype = __value;
        } 
        /*! \copydoc datatype
            \see see datatype for details */ 
        inline virtual DataType getDatatype() const  
        {
            return this->datatype; 
        }
        /*! \copydoc dataModifier
            \see see dataModifier for details */ 
        inline virtual void setDataModifier(void* __value)  
        {
            this->dataModifier = __value;
        } 
        /*! \copydoc dataModifier
            \see see dataModifier for details */ 
        inline virtual void* getDataModifier() const  
        {
            return this->dataModifier; 
        }
        /*! \copydoc datatypeModifier
            \see see datatypeModifier for details */ 
        inline virtual void setDatatypeModifier(const DataType & __value)  
        {
            this->datatypeModifier = __value;
        } 
        /*! \copydoc datatypeModifier
            \see see datatypeModifier for details */ 
        inline virtual DataType getDatatypeModifier() const  
        {
            return this->datatypeModifier; 
        }
        /*! \copydoc modifierMode
            \see see modifierMode for details */ 
        inline virtual void setModifierMode(const ModifierMode & __value)  
        {
            this->modifierMode = __value;
        } 
        /*! \copydoc modifierMode
            \see see modifierMode for details */ 
        inline virtual ModifierMode getModifierMode() const  
        {
            return this->modifierMode; 
        }

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
        inline virtual void setPalette(const JKQTPMathImageColorPalette & __value)  
        {
            this->palette = __value;
        } 
        /*! \copydoc palette
            \see see palette for details */ 
        inline virtual JKQTPMathImageColorPalette getPalette() const  
        {
            return this->palette; 
        }
        /*! \copydoc rangeMinFailAction
            \see see rangeMinFailAction for details */ 
        inline virtual void setRangeMinFailAction(const JKQTPMathImageColorRangeFailAction & __value)  
        {
            this->rangeMinFailAction = __value;
        } 
        /*! \copydoc rangeMinFailAction
            \see see rangeMinFailAction for details */ 
        inline virtual JKQTPMathImageColorRangeFailAction getActionRangeMinFail() const  
        {
            return this->rangeMinFailAction; 
        }
        /*! \copydoc rangeMaxFailAction
            \see see rangeMaxFailAction for details */ 
        inline virtual void setRangeMaxFailAction(const JKQTPMathImageColorRangeFailAction & __value)  
        {
            this->rangeMaxFailAction = __value;
        } 
        /*! \copydoc rangeMaxFailAction
            \see see rangeMaxFailAction for details */ 
        inline virtual JKQTPMathImageColorRangeFailAction getActionRangeMaxFail() const  
        {
            return this->rangeMaxFailAction; 
        }
        /*! \copydoc rangeMinFailColor
            \see see rangeMinFailColor for details */ 
        inline virtual void setRangeMinFailColor(const QColor & __value)  
        {
            this->rangeMinFailColor = __value;
        } 
        /*! \copydoc rangeMinFailColor
            \see see rangeMinFailColor for details */ 
        inline virtual QColor getRangeMinFailColor() const  
        {
            return this->rangeMinFailColor; 
        }
        /*! \copydoc rangeMaxFailColor
            \see see rangeMaxFailColor for details */ 
        inline virtual void setRangeMaxFailColor(const QColor & __value)  
        {
            this->rangeMaxFailColor = __value;
        } 
        /*! \copydoc rangeMaxFailColor
            \see see rangeMaxFailColor for details */ 
        inline virtual QColor getRangeMaxFailColor() const  
        {
            return this->rangeMaxFailColor; 
        }
        /*! \copydoc nanColor
            \see see nanColor for details */ 
        inline virtual void setNanColor(const QColor & __value)  
        {
            this->nanColor = __value;
        } 
        /*! \copydoc nanColor
            \see see nanColor for details */ 
        inline virtual QColor getNanColor() const  
        {
            return this->nanColor; 
        }
        /*! \copydoc infColor
            \see see infColor for details */ 
        inline virtual void setInfColor(const QColor & __value)  
        {
            this->infColor = __value;
        } 
        /*! \copydoc infColor
            \see see infColor for details */ 
        inline virtual QColor getInfColor() const  
        {
            return this->infColor; 
        }
        /*! \copydoc showColorBar
            \see see showColorBar for details */ 
        inline virtual void setShowColorBar(bool __value)
        {
            this->showColorBar = __value;
        } 
        /*! \copydoc showColorBar
            \see see showColorBar for details */ 
        inline virtual bool getShowColorBar() const  
        {
            return this->showColorBar; 
        }
        /*! \copydoc colorBarWidth
            \see see colorBarWidth for details */ 
        inline virtual void setColorBarWidth(double __value)
        {
            this->colorBarWidth = __value;
        } 
        /*! \copydoc colorBarWidth
            \see see colorBarWidth for details */ 
        inline virtual double getColorBarWidth() const
        {
            return this->colorBarWidth; 
        }
        /*! \copydoc colorBarModifiedWidth
            \see see colorBarModifiedWidth for details */ 
        inline virtual void setColorBarModifiedWidth(double __value)
        {
            this->colorBarModifiedWidth = __value;
        } 
        /*! \copydoc colorBarModifiedWidth
            \see see colorBarModifiedWidth for details */ 
        inline virtual double getColorBarModifiedWidth() const
        {
            return this->colorBarModifiedWidth; 
        }
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        inline virtual void setColorBarOffset(double __value)
        {
            this->colorBarOffset = __value;
        } 
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        inline virtual double getColorBarOffset() const
        {
            return this->colorBarOffset; 
        }
        /*! \copydoc colorBarRelativeHeight
            \see see colorBarRelativeHeight for details */ 
        inline virtual void setColorBarRelativeHeight(double __value)
        {
            this->colorBarRelativeHeight = __value;
        } 
        /*! \copydoc colorBarRelativeHeight
            \see see colorBarRelativeHeight for details */ 
        inline virtual double getColorBarRelativeHeight() const  
        {
            return this->colorBarRelativeHeight; 
        }
        /*! \copydoc imageMin
            \see see imageMin for details */ 
        inline virtual void setImageMin(double __value)
        {
            this->imageMin = __value;
        } 
        /*! \copydoc imageMin
            \see see imageMin for details */ 
        inline virtual double getImageMin() const  
        {
            return this->imageMin; 
        }
        /*! \copydoc imageMax
            \see see imageMax for details */ 
        inline virtual void setImageMax(double __value)
        {
            this->imageMax = __value;
        } 
        /*! \copydoc imageMax
            \see see imageMax for details */ 
        inline virtual double getImageMax() const  
        {
            return this->imageMax; 
        }
        /*! \copydoc autoImageRange
            \see see autoImageRange for details */ 
        inline virtual void setAutoImageRange(bool __value)
        {
            this->autoImageRange = __value;
        } 
        /*! \copydoc autoImageRange
            \see see autoImageRange for details */ 
        inline virtual bool getAutoImageRange() const  
        {
            return this->autoImageRange; 
        }
        /*! \copydoc imageName
            \see see imageName for details */ 
        inline virtual void setImageName(const QString & __value)  
        {
            this->imageName = __value;
        } 
        /*! \copydoc imageName
            \see see imageName for details */ 
        inline virtual QString getImageName() const  
        {
            return this->imageName; 
        }
        /*! \copydoc imageNameFontSize
            \see see imageNameFontSize for details */ 
        inline virtual void setImageNameFontSize(double __value)
        {
            this->imageNameFontSize = __value;
        } 
        /*! \copydoc imageNameFontSize
            \see see imageNameFontSize for details */ 
        inline virtual double getImageNameFontSize() const  
        {
            return this->imageNameFontSize; 
        }
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */ 
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxis()  { return this->colorBarRightAxis; }
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */ 
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxis()  { return this->colorBarTopAxis; }
        /*! \brief returns the property modifierColorBarTopAxis ( \copybrief modifierColorBarTopAxis ). \details Description of the parameter modifierColorBarTopAxis is:  <BLOCKQUOTE>\copydoc modifierColorBarTopAxis </BLOCKQUOTE>. \see modifierColorBarTopAxis for more information */ 
        inline JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis()  { return this->modifierColorBarTopAxis; }
        /*! \brief returns the property modifierColorBarRightAxis ( \copybrief modifierColorBarRightAxis ). \details Description of the parameter modifierColorBarRightAxis  is:  <BLOCKQUOTE>\copydoc modifierColorBarRightAxis  </BLOCKQUOTE>. \see modifierColorBarRightAxis  for more information */ 
        inline JKQTPHorizontalIndependentAxis* getModifierColorBarRightAxis()   { return this->modifierColorBarRightAxis ; }
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */
        inline const JKQTPVerticalIndependentAxis* getColorBarRightAxis() const { return this->colorBarRightAxis; }
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */
        inline const JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const { return this->colorBarTopAxis; }
        /*! \brief returns the property modifierColorBarTopAxis ( \copybrief modifierColorBarTopAxis ). \details Description of the parameter modifierColorBarTopAxis is:  <BLOCKQUOTE>\copydoc modifierColorBarTopAxis </BLOCKQUOTE>. \see modifierColorBarTopAxis for more information */
        inline const JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis() const { return this->modifierColorBarTopAxis; }
        /*! \brief returns the property modifierColorBarRightAxis ( \copybrief modifierColorBarRightAxis ). \details Description of the parameter modifierColorBarRightAxis  is:  <BLOCKQUOTE>\copydoc modifierColorBarRightAxis  </BLOCKQUOTE>. \see modifierColorBarRightAxis  for more information */
        inline const JKQTPHorizontalIndependentAxis* getModifierColorBarRightAxis()  const { return this->modifierColorBarRightAxis ; }
        /*! \copydoc colorBarTopVisible
            \see see colorBarTopVisible for details */ 
        inline virtual void setColorBarTopVisible(bool __value)
        {
            this->colorBarTopVisible = __value;
        } 
        /*! \copydoc colorBarTopVisible
            \see see colorBarTopVisible for details */ 
        inline virtual bool getColorBarTopVisible() const  
        {
            return this->colorBarTopVisible; 
        }
        /*! \copydoc colorBarRightVisible
            \see see colorBarRightVisible for details */ 
        inline virtual void setColorBarRightVisible(bool __value)
        {
            this->colorBarRightVisible = __value;
        } 
        /*! \copydoc colorBarRightVisible
            \see see colorBarRightVisible for details */ 
        inline virtual bool getColorBarRightVisible() const  
        {
            return this->colorBarRightVisible; 
        }
        /*! \copydoc autoModifierRange
            \see see autoModifierRange for details */ 
        inline virtual void setAutoModifierRange(bool __value)
        {
            this->autoModifierRange = __value;
        } 
        /*! \copydoc autoModifierRange
            \see see autoModifierRange for details */ 
        inline virtual bool getAutoModifierRange() const  
        {
            return this->autoModifierRange; 
        }
        /*! \copydoc modifierMin
            \see see modifierMin for details */ 
        inline virtual void setModifierMin(double __value)
        {
            this->modifierMin = __value;
        } 
        /*! \copydoc modifierMin
            \see see modifierMin for details */ 
        inline virtual double getModifierMin() const  
        {
            return this->modifierMin; 
        }
        /*! \copydoc modifierMax
            \see see modifierMax for details */ 
        inline virtual void setModifierMax(double __value)
        {
            this->modifierMax = __value;
        } 
        /*! \copydoc modifierMax
            \see see modifierMax for details */ 
        inline virtual double getModifierMax() const  
        {
            return this->modifierMax; 
        }

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
        inline virtual void setImageColumn(int __value)
        {
            this->imageColumn = __value;
        } 
        /*! \copydoc imageColumn
            \see see imageColumn for details */ 
        inline virtual int getImageColumn() const  
        {
            return this->imageColumn; 
        }
        /*! \copydoc modifierColumn
            \see see modifierColumn for details */ 
        inline virtual void setModifierColumn(int __value)
        {
            this->modifierColumn = __value;
        } 
        /*! \copydoc modifierColumn
            \see see modifierColumn for details */ 
        inline virtual int getModifierColumn() const  
        {
            return this->modifierColumn; 
        }

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

