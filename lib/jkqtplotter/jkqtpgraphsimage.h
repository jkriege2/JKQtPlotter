/*
    Copyright (c) 2008-2019 Jan W. Krieger & Sebastian Isbaner (contour plot)

    

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


#ifndef jkqtpgraphsimage_H
#define jkqtpgraphsimage_H



/** \file jkqtpgraphsimage.h
  * \ingroup  jkqtplotter_imagelots_elements
  */

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
class LIB_EXPORT JKQTPImageBase: public JKQTPGraph {
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

        /*! \brief sets the property x ( \copybrief x ) to the specified \a __value. 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual void setX(double __value)
        {
            this->x = __value;
        } 
        /*! \brief returns the property x ( \copybrief x ). 
            \details Description of the parameter x is: <BLOCKQUOTE>\copydoc x </BLOCKQUOTE> 
            \see x for more information */ 
        inline virtual double getX() const  
        {
            return this->x; 
        }
        /*! \brief sets the property y ( \copybrief y ) to the specified \a __value. 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual void setY(double __value)
        {
            this->y = __value;
        } 
        /*! \brief returns the property y ( \copybrief y ). 
            \details Description of the parameter y is: <BLOCKQUOTE>\copydoc y </BLOCKQUOTE> 
            \see y for more information */ 
        inline virtual double getY() const  
        {
            return this->y; 
        }
        /*! \brief sets the property width ( \copybrief width ) to the specified \a __value. 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc width </BLOCKQUOTE> 
            \see width for more information */ 
        inline virtual void setWidth(double __value)
        {
            this->width = __value;
        } 
        /*! \brief returns the property width ( \copybrief width ). 
            \details Description of the parameter width is: <BLOCKQUOTE>\copydoc width </BLOCKQUOTE> 
            \see width for more information */ 
        inline virtual double getWidth() const  
        {
            return this->width; 
        }
        /*! \brief sets the property height ( \copybrief height ) to the specified \a __value. 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc height </BLOCKQUOTE> 
            \see height for more information */ 
        inline virtual void setHeight(double __value)
        {
            this->height = __value;
        } 
        /*! \brief returns the property height ( \copybrief height ). 
            \details Description of the parameter height is: <BLOCKQUOTE>\copydoc height </BLOCKQUOTE> 
            \see height for more information */ 
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
class LIB_EXPORT JKQTPMathImageBase: public JKQTPImageBase {
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

        /*! \brief sets the property Nx ( \copybrief Nx ) to the specified \a __value. 
            \details Description of the parameter Nx is: <BLOCKQUOTE>\copydoc Nx </BLOCKQUOTE> 
            \see Nx for more information */ 
        inline virtual void setNx(int __value)
        {
            this->Nx = __value;
        } 
        /*! \brief returns the property Nx ( \copybrief Nx ). 
            \details Description of the parameter Nx is: <BLOCKQUOTE>\copydoc Nx </BLOCKQUOTE> 
            \see Nx for more information */ 
        inline virtual int getNx() const
        {
            return this->Nx; 
        }
        /*! \brief sets the property Ny ( \copybrief Ny ) to the specified \a __value. 
            \details Description of the parameter Ny is: <BLOCKQUOTE>\copydoc Ny </BLOCKQUOTE> 
            \see Ny for more information */ 
        inline virtual void setNy(int __value)
        {
            this->Ny = __value;
        } 
        /*! \brief returns the property Ny ( \copybrief Ny ). 
            \details Description of the parameter Ny is: <BLOCKQUOTE>\copydoc Ny </BLOCKQUOTE> 
            \see Ny for more information */ 
        inline virtual int getNy() const
        {
            return this->Ny; 
        }
        /*! \brief sets the property data ( \copybrief data ) to the specified \a __value. 
            \details Description of the parameter data is: <BLOCKQUOTE>\copydoc data </BLOCKQUOTE> 
            \see data for more information */ 
        inline virtual void setData(void* __value)  
        {
            this->data = __value;
        } 
        /*! \brief returns the property data ( \copybrief data ). 
            \details Description of the parameter data is: <BLOCKQUOTE>\copydoc data </BLOCKQUOTE> 
            \see data for more information */ 
        inline virtual void* getData() const  
        {
            return this->data; 
        }
        /*! \brief sets the property datatype ( \copybrief datatype ) to the specified \a __value. 
            \details Description of the parameter datatype is: <BLOCKQUOTE>\copydoc datatype </BLOCKQUOTE> 
            \see datatype for more information */ 
        inline virtual void setDatatype(const DataType & __value)  
        {
            this->datatype = __value;
        } 
        /*! \brief returns the property datatype ( \copybrief datatype ). 
            \details Description of the parameter datatype is: <BLOCKQUOTE>\copydoc datatype </BLOCKQUOTE> 
            \see datatype for more information */ 
        inline virtual DataType getDatatype() const  
        {
            return this->datatype; 
        }
        /*! \brief sets the property dataModifier ( \copybrief dataModifier ) to the specified \a __value. 
            \details Description of the parameter dataModifier is: <BLOCKQUOTE>\copydoc dataModifier </BLOCKQUOTE> 
            \see dataModifier for more information */ 
        inline virtual void setDataModifier(void* __value)  
        {
            this->dataModifier = __value;
        } 
        /*! \brief returns the property dataModifier ( \copybrief dataModifier ). 
            \details Description of the parameter dataModifier is: <BLOCKQUOTE>\copydoc dataModifier </BLOCKQUOTE> 
            \see dataModifier for more information */ 
        inline virtual void* getDataModifier() const  
        {
            return this->dataModifier; 
        }
        /*! \brief sets the property datatypeModifier ( \copybrief datatypeModifier ) to the specified \a __value. 
            \details Description of the parameter datatypeModifier is: <BLOCKQUOTE>\copydoc datatypeModifier </BLOCKQUOTE> 
            \see datatypeModifier for more information */ 
        inline virtual void setDatatypeModifier(const DataType & __value)  
        {
            this->datatypeModifier = __value;
        } 
        /*! \brief returns the property datatypeModifier ( \copybrief datatypeModifier ). 
            \details Description of the parameter datatypeModifier is: <BLOCKQUOTE>\copydoc datatypeModifier </BLOCKQUOTE> 
            \see datatypeModifier for more information */ 
        inline virtual DataType getDatatypeModifier() const  
        {
            return this->datatypeModifier; 
        }
        /*! \brief sets the property modifierMode ( \copybrief modifierMode ) to the specified \a __value. 
            \details Description of the parameter modifierMode is: <BLOCKQUOTE>\copydoc modifierMode </BLOCKQUOTE> 
            \see modifierMode for more information */ 
        inline virtual void setModifierMode(const ModifierMode & __value)  
        {
            this->modifierMode = __value;
        } 
        /*! \brief returns the property modifierMode ( \copybrief modifierMode ). 
            \details Description of the parameter modifierMode is: <BLOCKQUOTE>\copydoc modifierMode </BLOCKQUOTE> 
            \see modifierMode for more information */ 
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
class LIB_EXPORT JKQTPImage: public JKQTPImageBase {
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
class LIB_EXPORT JKQTPMathImage: public JKQTPMathImageBase {
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


        /*! \brief sets the property palette ( \copybrief palette ) to the specified \a __value. 
            \details Description of the parameter palette is: <BLOCKQUOTE>\copydoc palette </BLOCKQUOTE> 
            \see palette for more information */ 
        inline virtual void setPalette(const JKQTPMathImageColorPalette & __value)  
        {
            this->palette = __value;
        } 
        /*! \brief returns the property palette ( \copybrief palette ). 
            \details Description of the parameter palette is: <BLOCKQUOTE>\copydoc palette </BLOCKQUOTE> 
            \see palette for more information */ 
        inline virtual JKQTPMathImageColorPalette getPalette() const  
        {
            return this->palette; 
        }
        /*! \brief sets the property rangeMinFailAction ( \copybrief rangeMinFailAction ) to the specified \a __value. 
            \details Description of the parameter rangeMinFailAction is: <BLOCKQUOTE>\copydoc rangeMinFailAction </BLOCKQUOTE> 
            \see rangeMinFailAction for more information */ 
        inline virtual void setRangeMinFailAction(const JKQTPMathImageColorRangeFailAction & __value)  
        {
            this->rangeMinFailAction = __value;
        } 
        /*! \brief returns the property rangeMinFailAction ( \copybrief rangeMinFailAction ). 
            \details Description of the parameter rangeMinFailAction is: <BLOCKQUOTE>\copydoc rangeMinFailAction </BLOCKQUOTE> 
            \see rangeMinFailAction for more information */ 
        inline virtual JKQTPMathImageColorRangeFailAction getActionRangeMinFail() const  
        {
            return this->rangeMinFailAction; 
        }
        /*! \brief sets the property rangeMaxFailAction ( \copybrief rangeMaxFailAction ) to the specified \a __value. 
            \details Description of the parameter rangeMaxFailAction is: <BLOCKQUOTE>\copydoc rangeMaxFailAction </BLOCKQUOTE> 
            \see rangeMaxFailAction for more information */ 
        inline virtual void setRangeMaxFailAction(const JKQTPMathImageColorRangeFailAction & __value)  
        {
            this->rangeMaxFailAction = __value;
        } 
        /*! \brief returns the property rangeMaxFailAction ( \copybrief rangeMaxFailAction ). 
            \details Description of the parameter rangeMaxFailAction is: <BLOCKQUOTE>\copydoc rangeMaxFailAction </BLOCKQUOTE> 
            \see rangeMaxFailAction for more information */ 
        inline virtual JKQTPMathImageColorRangeFailAction getActionRangeMaxFail() const  
        {
            return this->rangeMaxFailAction; 
        }
        /*! \brief sets the property rangeMinFailColor ( \copybrief rangeMinFailColor ) to the specified \a __value. 
            \details Description of the parameter rangeMinFailColor is: <BLOCKQUOTE>\copydoc rangeMinFailColor </BLOCKQUOTE> 
            \see rangeMinFailColor for more information */ 
        inline virtual void setRangeMinFailColor(const QColor & __value)  
        {
            this->rangeMinFailColor = __value;
        } 
        /*! \brief returns the property rangeMinFailColor ( \copybrief rangeMinFailColor ). 
            \details Description of the parameter rangeMinFailColor is: <BLOCKQUOTE>\copydoc rangeMinFailColor </BLOCKQUOTE> 
            \see rangeMinFailColor for more information */ 
        inline virtual QColor getRangeMinFailColor() const  
        {
            return this->rangeMinFailColor; 
        }
        /*! \brief sets the property rangeMaxFailColor ( \copybrief rangeMaxFailColor ) to the specified \a __value. 
            \details Description of the parameter rangeMaxFailColor is: <BLOCKQUOTE>\copydoc rangeMaxFailColor </BLOCKQUOTE> 
            \see rangeMaxFailColor for more information */ 
        inline virtual void setRangeMaxFailColor(const QColor & __value)  
        {
            this->rangeMaxFailColor = __value;
        } 
        /*! \brief returns the property rangeMaxFailColor ( \copybrief rangeMaxFailColor ). 
            \details Description of the parameter rangeMaxFailColor is: <BLOCKQUOTE>\copydoc rangeMaxFailColor </BLOCKQUOTE> 
            \see rangeMaxFailColor for more information */ 
        inline virtual QColor getRangeMaxFailColor() const  
        {
            return this->rangeMaxFailColor; 
        }
        /*! \brief sets the property nanColor ( \copybrief nanColor ) to the specified \a __value. 
            \details Description of the parameter nanColor is: <BLOCKQUOTE>\copydoc nanColor </BLOCKQUOTE> 
            \see nanColor for more information */ 
        inline virtual void setNanColor(const QColor & __value)  
        {
            this->nanColor = __value;
        } 
        /*! \brief returns the property nanColor ( \copybrief nanColor ). 
            \details Description of the parameter nanColor is: <BLOCKQUOTE>\copydoc nanColor </BLOCKQUOTE> 
            \see nanColor for more information */ 
        inline virtual QColor getNanColor() const  
        {
            return this->nanColor; 
        }
        /*! \brief sets the property infColor ( \copybrief infColor ) to the specified \a __value. 
            \details Description of the parameter infColor is: <BLOCKQUOTE>\copydoc infColor </BLOCKQUOTE> 
            \see infColor for more information */ 
        inline virtual void setInfColor(const QColor & __value)  
        {
            this->infColor = __value;
        } 
        /*! \brief returns the property infColor ( \copybrief infColor ). 
            \details Description of the parameter infColor is: <BLOCKQUOTE>\copydoc infColor </BLOCKQUOTE> 
            \see infColor for more information */ 
        inline virtual QColor getInfColor() const  
        {
            return this->infColor; 
        }
        /*! \brief sets the property showColorBar ( \copybrief showColorBar ) to the specified \a __value. 
            \details Description of the parameter showColorBar is: <BLOCKQUOTE>\copydoc showColorBar </BLOCKQUOTE> 
            \see showColorBar for more information */ 
        inline virtual void setShowColorBar(bool __value)
        {
            this->showColorBar = __value;
        } 
        /*! \brief returns the property showColorBar ( \copybrief showColorBar ). 
            \details Description of the parameter showColorBar is: <BLOCKQUOTE>\copydoc showColorBar </BLOCKQUOTE> 
            \see showColorBar for more information */ 
        inline virtual bool getShowColorBar() const  
        {
            return this->showColorBar; 
        }
        /*! \brief sets the property colorBarWidth ( \copybrief colorBarWidth ) to the specified \a __value. 
            \details Description of the parameter colorBarWidth is: <BLOCKQUOTE>\copydoc colorBarWidth </BLOCKQUOTE> 
            \see colorBarWidth for more information */ 
        inline virtual void setColorBarWidth(int __value)
        {
            this->colorBarWidth = __value;
        } 
        /*! \brief returns the property colorBarWidth ( \copybrief colorBarWidth ). 
            \details Description of the parameter colorBarWidth is: <BLOCKQUOTE>\copydoc colorBarWidth </BLOCKQUOTE> 
            \see colorBarWidth for more information */ 
        inline virtual int getColorBarWidth() const  
        {
            return this->colorBarWidth; 
        }
        /*! \brief sets the property colorBarModifiedWidth ( \copybrief colorBarModifiedWidth ) to the specified \a __value. 
            \details Description of the parameter colorBarModifiedWidth is: <BLOCKQUOTE>\copydoc colorBarModifiedWidth </BLOCKQUOTE> 
            \see colorBarModifiedWidth for more information */ 
        inline virtual void setColorBarModifiedWidth(int __value)
        {
            this->colorBarModifiedWidth = __value;
        } 
        /*! \brief returns the property colorBarModifiedWidth ( \copybrief colorBarModifiedWidth ). 
            \details Description of the parameter colorBarModifiedWidth is: <BLOCKQUOTE>\copydoc colorBarModifiedWidth </BLOCKQUOTE> 
            \see colorBarModifiedWidth for more information */ 
        inline virtual int getColorBarModifiedWidth() const  
        {
            return this->colorBarModifiedWidth; 
        }
        /*! \brief sets the property colorBarOffset ( \copybrief colorBarOffset ) to the specified \a __value. 
            \details Description of the parameter colorBarOffset is: <BLOCKQUOTE>\copydoc colorBarOffset </BLOCKQUOTE> 
            \see colorBarOffset for more information */ 
        inline virtual void setColorBarOffset(int __value)
        {
            this->colorBarOffset = __value;
        } 
        /*! \brief returns the property colorBarOffset ( \copybrief colorBarOffset ). 
            \details Description of the parameter colorBarOffset is: <BLOCKQUOTE>\copydoc colorBarOffset </BLOCKQUOTE> 
            \see colorBarOffset for more information */ 
        inline virtual int getColorBarOffset() const  
        {
            return this->colorBarOffset; 
        }
        /*! \brief sets the property colorBarRelativeHeight ( \copybrief colorBarRelativeHeight ) to the specified \a __value. 
            \details Description of the parameter colorBarRelativeHeight is: <BLOCKQUOTE>\copydoc colorBarRelativeHeight </BLOCKQUOTE> 
            \see colorBarRelativeHeight for more information */ 
        inline virtual void setColorBarRelativeHeight(double __value)
        {
            this->colorBarRelativeHeight = __value;
        } 
        /*! \brief returns the property colorBarRelativeHeight ( \copybrief colorBarRelativeHeight ). 
            \details Description of the parameter colorBarRelativeHeight is: <BLOCKQUOTE>\copydoc colorBarRelativeHeight </BLOCKQUOTE> 
            \see colorBarRelativeHeight for more information */ 
        inline virtual double getColorBarRelativeHeight() const  
        {
            return this->colorBarRelativeHeight; 
        }
        /*! \brief sets the property imageMin ( \copybrief imageMin ) to the specified \a __value. 
            \details Description of the parameter imageMin is: <BLOCKQUOTE>\copydoc imageMin </BLOCKQUOTE> 
            \see imageMin for more information */ 
        inline virtual void setImageMin(double __value)
        {
            this->imageMin = __value;
        } 
        /*! \brief returns the property imageMin ( \copybrief imageMin ). 
            \details Description of the parameter imageMin is: <BLOCKQUOTE>\copydoc imageMin </BLOCKQUOTE> 
            \see imageMin for more information */ 
        inline virtual double getImageMin() const  
        {
            return this->imageMin; 
        }
        /*! \brief sets the property imageMax ( \copybrief imageMax ) to the specified \a __value. 
            \details Description of the parameter imageMax is: <BLOCKQUOTE>\copydoc imageMax </BLOCKQUOTE> 
            \see imageMax for more information */ 
        inline virtual void setImageMax(double __value)
        {
            this->imageMax = __value;
        } 
        /*! \brief returns the property imageMax ( \copybrief imageMax ). 
            \details Description of the parameter imageMax is: <BLOCKQUOTE>\copydoc imageMax </BLOCKQUOTE> 
            \see imageMax for more information */ 
        inline virtual double getImageMax() const  
        {
            return this->imageMax; 
        }
        /*! \brief sets the property autoImageRange ( \copybrief autoImageRange ) to the specified \a __value. 
            \details Description of the parameter autoImageRange is: <BLOCKQUOTE>\copydoc autoImageRange </BLOCKQUOTE> 
            \see autoImageRange for more information */ 
        inline virtual void setAutoImageRange(bool __value)
        {
            this->autoImageRange = __value;
        } 
        /*! \brief returns the property autoImageRange ( \copybrief autoImageRange ). 
            \details Description of the parameter autoImageRange is: <BLOCKQUOTE>\copydoc autoImageRange </BLOCKQUOTE> 
            \see autoImageRange for more information */ 
        inline virtual bool getAutoImageRange() const  
        {
            return this->autoImageRange; 
        }
        /*! \brief sets the property imageName ( \copybrief imageName ) to the specified \a __value. 
            \details Description of the parameter imageName is: <BLOCKQUOTE>\copydoc imageName </BLOCKQUOTE> 
            \see imageName for more information */ 
        inline virtual void setImageName(const QString & __value)  
        {
            this->imageName = __value;
        } 
        /*! \brief returns the property imageName ( \copybrief imageName ). 
            \details Description of the parameter imageName is: <BLOCKQUOTE>\copydoc imageName </BLOCKQUOTE> 
            \see imageName for more information */ 
        inline virtual QString getImageName() const  
        {
            return this->imageName; 
        }
        /*! \brief sets the property imageNameFontName ( \copybrief imageNameFontName ) to the specified \a __value. 
            \details Description of the parameter imageNameFontName is: <BLOCKQUOTE>\copydoc imageNameFontName </BLOCKQUOTE> 
            \see imageNameFontName for more information */ 
        inline virtual void setImageNameFontName(const QString & __value)  
        {
            this->imageNameFontName = __value;
        } 
        /*! \brief returns the property imageNameFontName ( \copybrief imageNameFontName ). 
            \details Description of the parameter imageNameFontName is: <BLOCKQUOTE>\copydoc imageNameFontName </BLOCKQUOTE> 
            \see imageNameFontName for more information */ 
        inline virtual QString getImageNameFontName() const  
        {
            return this->imageNameFontName; 
        }
        /*! \brief sets the property imageNameFontSize ( \copybrief imageNameFontSize ) to the specified \a __value. 
            \details Description of the parameter imageNameFontSize is: <BLOCKQUOTE>\copydoc imageNameFontSize </BLOCKQUOTE> 
            \see imageNameFontSize for more information */ 
        inline virtual void setImageNameFontSize(double __value)
        {
            this->imageNameFontSize = __value;
        } 
        /*! \brief returns the property imageNameFontSize ( \copybrief imageNameFontSize ). 
            \details Description of the parameter imageNameFontSize is: <BLOCKQUOTE>\copydoc imageNameFontSize </BLOCKQUOTE> 
            \see imageNameFontSize for more information */ 
        inline virtual double getImageNameFontSize() const  
        {
            return this->imageNameFontSize; 
        }
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */ 
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxis() const { return this->colorBarRightAxis; }
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */ 
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const { return this->colorBarTopAxis; }
        /*! \brief returns the property modifierColorBarTopAxis ( \copybrief modifierColorBarTopAxis ). \details Description of the parameter modifierColorBarTopAxis is:  <BLOCKQUOTE>\copydoc modifierColorBarTopAxis </BLOCKQUOTE>. \see modifierColorBarTopAxis for more information */ 
        inline JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis() const { return this->modifierColorBarTopAxis; }
        /*! \brief returns the property modifierColorBarRightAxis ( \copybrief modifierColorBarRightAxis ). \details Description of the parameter modifierColorBarRightAxis  is:  <BLOCKQUOTE>\copydoc modifierColorBarRightAxis  </BLOCKQUOTE>. \see modifierColorBarRightAxis  for more information */ 
        inline JKQTPHorizontalIndependentAxis* getModifierColorBarRightAxis()  const { return this->modifierColorBarRightAxis ; }
        /*! \brief sets the property colorBarTopVisible ( \copybrief colorBarTopVisible ) to the specified \a __value. 
            \details Description of the parameter colorBarTopVisible is: <BLOCKQUOTE>\copydoc colorBarTopVisible </BLOCKQUOTE> 
            \see colorBarTopVisible for more information */ 
        inline virtual void setColorBarTopVisible(bool __value)
        {
            this->colorBarTopVisible = __value;
        } 
        /*! \brief returns the property colorBarTopVisible ( \copybrief colorBarTopVisible ). 
            \details Description of the parameter colorBarTopVisible is: <BLOCKQUOTE>\copydoc colorBarTopVisible </BLOCKQUOTE> 
            \see colorBarTopVisible for more information */ 
        inline virtual bool getColorBarTopVisible() const  
        {
            return this->colorBarTopVisible; 
        }
        /*! \brief sets the property colorBarRightVisible ( \copybrief colorBarRightVisible ) to the specified \a __value. 
            \details Description of the parameter colorBarRightVisible is: <BLOCKQUOTE>\copydoc colorBarRightVisible </BLOCKQUOTE> 
            \see colorBarRightVisible for more information */ 
        inline virtual void setColorBarRightVisible(bool __value)
        {
            this->colorBarRightVisible = __value;
        } 
        /*! \brief returns the property colorBarRightVisible ( \copybrief colorBarRightVisible ). 
            \details Description of the parameter colorBarRightVisible is: <BLOCKQUOTE>\copydoc colorBarRightVisible </BLOCKQUOTE> 
            \see colorBarRightVisible for more information */ 
        inline virtual bool getColorBarRightVisible() const  
        {
            return this->colorBarRightVisible; 
        }
        /*! \brief sets the property autoModifierRange ( \copybrief autoModifierRange ) to the specified \a __value. 
            \details Description of the parameter autoModifierRange is: <BLOCKQUOTE>\copydoc autoModifierRange </BLOCKQUOTE> 
            \see autoModifierRange for more information */ 
        inline virtual void setAutoModifierRange(bool __value)
        {
            this->autoModifierRange = __value;
        } 
        /*! \brief returns the property autoModifierRange ( \copybrief autoModifierRange ). 
            \details Description of the parameter autoModifierRange is: <BLOCKQUOTE>\copydoc autoModifierRange </BLOCKQUOTE> 
            \see autoModifierRange for more information */ 
        inline virtual bool getAutoModifierRange() const  
        {
            return this->autoModifierRange; 
        }
        /*! \brief sets the property modifierMin ( \copybrief modifierMin ) to the specified \a __value. 
            \details Description of the parameter modifierMin is: <BLOCKQUOTE>\copydoc modifierMin </BLOCKQUOTE> 
            \see modifierMin for more information */ 
        inline virtual void setModifierMin(double __value)
        {
            this->modifierMin = __value;
        } 
        /*! \brief returns the property modifierMin ( \copybrief modifierMin ). 
            \details Description of the parameter modifierMin is: <BLOCKQUOTE>\copydoc modifierMin </BLOCKQUOTE> 
            \see modifierMin for more information */ 
        inline virtual double getModifierMin() const  
        {
            return this->modifierMin; 
        }
        /*! \brief sets the property modifierMax ( \copybrief modifierMax ) to the specified \a __value. 
            \details Description of the parameter modifierMax is: <BLOCKQUOTE>\copydoc modifierMax </BLOCKQUOTE> 
            \see modifierMax for more information */ 
        inline virtual void setModifierMax(double __value)
        {
            this->modifierMax = __value;
        } 
        /*! \brief returns the property modifierMax ( \copybrief modifierMax ). 
            \details Description of the parameter modifierMax is: <BLOCKQUOTE>\copydoc modifierMax </BLOCKQUOTE> 
            \see modifierMax for more information */ 
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







/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_rgbimageplot_opencv.png
 */
class LIB_EXPORT JKQTPRGBMathImage: public JKQTPMathImageBase {
        Q_OBJECT
    public:
        using JKQTPMathImageBase::setData;


        /** \brief class constructor */
        JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPlotter* parent);

        /** \brief class constructor */
        JKQTPRGBMathImage(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPRGBMathImage(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        virtual void setParent(JKQTBasePlotter* parent) override;



        virtual void setData(void* data, void* dataG, void* dataB, int Nx, int Ny, DataType datatype);
        virtual void setData(void* data, void* dataG, void* dataB,  int Nx, int Ny);

        virtual void setData(void* data, int Nx, int Ny, DataType datatype) override;
        virtual void setData(void* data,  int Nx, int Ny) override ;


        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax) override;


        /*! \brief sets the property dataG ( \copybrief dataG ) to the specified \a __value. 
            \details Description of the parameter dataG is: <BLOCKQUOTE>\copydoc dataG </BLOCKQUOTE> 
            \see dataG for more information */ 
        inline virtual void setDataG(void* __value)  
        {
            this->dataG = __value;
        } 
        /*! \brief returns the property dataG ( \copybrief dataG ). 
            \details Description of the parameter dataG is: <BLOCKQUOTE>\copydoc dataG </BLOCKQUOTE> 
            \see dataG for more information */ 
        inline virtual void* getDataG() const  
        {
            return this->dataG; 
        }
        /*! \brief sets the property datatypeG ( \copybrief datatypeG ) to the specified \a __value. 
            \details Description of the parameter datatypeG is: <BLOCKQUOTE>\copydoc datatypeG </BLOCKQUOTE> 
            \see datatypeG for more information */ 
        inline virtual void setDatatypeG(const DataType & __value)  
        {
            this->datatypeG = __value;
        } 
        /*! \brief returns the property datatypeG ( \copybrief datatypeG ). 
            \details Description of the parameter datatypeG is: <BLOCKQUOTE>\copydoc datatypeG </BLOCKQUOTE> 
            \see datatypeG for more information */ 
        inline virtual DataType getDatatypeG() const  
        {
            return this->datatypeG; 
        }
        /*! \brief sets the property dataB ( \copybrief dataB ) to the specified \a __value. 
            \details Description of the parameter dataB is: <BLOCKQUOTE>\copydoc dataB </BLOCKQUOTE> 
            \see dataB for more information */ 
        inline virtual void setDataB(void* __value)  
        {
            this->dataB = __value;
        } 
        /*! \brief returns the property dataB ( \copybrief dataB ). 
            \details Description of the parameter dataB is: <BLOCKQUOTE>\copydoc dataB </BLOCKQUOTE> 
            \see dataB for more information */ 
        inline virtual void* getDataB() const  
        {
            return this->dataB; 
        }
        /*! \brief sets the property datatypeB ( \copybrief datatypeB ) to the specified \a __value. 
            \details Description of the parameter datatypeB is: <BLOCKQUOTE>\copydoc datatypeB </BLOCKQUOTE> 
            \see datatypeB for more information */ 
        inline virtual void setDatatypeB(const DataType & __value)  
        {
            this->datatypeB = __value;
        } 
        /*! \brief returns the property datatypeB ( \copybrief datatypeB ). 
            \details Description of the parameter datatypeB is: <BLOCKQUOTE>\copydoc datatypeB </BLOCKQUOTE> 
            \see datatypeB for more information */ 
        inline virtual DataType getDatatypeB() const  
        {
            return this->datatypeB; 
        }
        /*! \brief sets the property showColorBar ( \copybrief showColorBar ) to the specified \a __value. 
            \details Description of the parameter showColorBar is: <BLOCKQUOTE>\copydoc showColorBar </BLOCKQUOTE> 
            \see showColorBar for more information */ 
        inline virtual void setShowColorBar(bool __value)
        {
            this->showColorBar = __value;
        } 
        /*! \brief returns the property showColorBar ( \copybrief showColorBar ). 
            \details Description of the parameter showColorBar is: <BLOCKQUOTE>\copydoc showColorBar </BLOCKQUOTE> 
            \see showColorBar for more information */ 
        inline virtual bool getShowColorBar() const  
        {
            return this->showColorBar; 
        }
        /*! \brief sets the property colorBarWidth ( \copybrief colorBarWidth ) to the specified \a __value. 
            \details Description of the parameter colorBarWidth is: <BLOCKQUOTE>\copydoc colorBarWidth </BLOCKQUOTE> 
            \see colorBarWidth for more information */ 
        inline virtual void setColorBarWidth(int __value)
        {
            this->colorBarWidth = __value;
        } 
        /*! \brief returns the property colorBarWidth ( \copybrief colorBarWidth ). 
            \details Description of the parameter colorBarWidth is: <BLOCKQUOTE>\copydoc colorBarWidth </BLOCKQUOTE> 
            \see colorBarWidth for more information */ 
        inline virtual int getColorBarWidth() const  
        {
            return this->colorBarWidth; 
        }
        /*! \brief sets the property colorBarOffset ( \copybrief colorBarOffset ) to the specified \a __value. 
            \details Description of the parameter colorBarOffset is: <BLOCKQUOTE>\copydoc colorBarOffset </BLOCKQUOTE> 
            \see colorBarOffset for more information */ 
        inline virtual void setColorBarOffset(int __value)
        {
            this->colorBarOffset = __value;
        } 
        /*! \brief returns the property colorBarOffset ( \copybrief colorBarOffset ). 
            \details Description of the parameter colorBarOffset is: <BLOCKQUOTE>\copydoc colorBarOffset </BLOCKQUOTE> 
            \see colorBarOffset for more information */ 
        inline virtual int getColorBarOffset() const  
        {
            return this->colorBarOffset; 
        }
        /*! \brief sets the property colorBarRelativeHeight ( \copybrief colorBarRelativeHeight ) to the specified \a __value. 
            \details Description of the parameter colorBarRelativeHeight is: <BLOCKQUOTE>\copydoc colorBarRelativeHeight </BLOCKQUOTE> 
            \see colorBarRelativeHeight for more information */ 
        inline virtual void setColorBarRelativeHeight(double __value)
        {
            this->colorBarRelativeHeight = __value;
        } 
        /*! \brief returns the property colorBarRelativeHeight ( \copybrief colorBarRelativeHeight ). 
            \details Description of the parameter colorBarRelativeHeight is: <BLOCKQUOTE>\copydoc colorBarRelativeHeight </BLOCKQUOTE> 
            \see colorBarRelativeHeight for more information */ 
        inline virtual double getColorBarRelativeHeight() const  
        {
            return this->colorBarRelativeHeight; 
        }
        /*! \brief sets the property imageMin ( \copybrief imageMin ) to the specified \a __value. 
            \details Description of the parameter imageMin is: <BLOCKQUOTE>\copydoc imageMin </BLOCKQUOTE> 
            \see imageMin for more information */ 
        inline virtual void setImageMin(double __value)
        {
            this->imageMin = __value;
        } 
        /*! \brief returns the property imageMin ( \copybrief imageMin ). 
            \details Description of the parameter imageMin is: <BLOCKQUOTE>\copydoc imageMin </BLOCKQUOTE> 
            \see imageMin for more information */ 
        inline virtual double getImageMin() const  
        {
            return this->imageMin; 
        }
        inline void setImageMinR(double m) {
            setImageMin(m);
        }
        /*! \brief sets the property imageMax ( \copybrief imageMax ) to the specified \a __value. 
            \details Description of the parameter imageMax is: <BLOCKQUOTE>\copydoc imageMax </BLOCKQUOTE> 
            \see imageMax for more information */ 
        inline virtual void setImageMax(double __value)
        {
            this->imageMax = __value;
        } 
        /*! \brief returns the property imageMax ( \copybrief imageMax ). 
            \details Description of the parameter imageMax is: <BLOCKQUOTE>\copydoc imageMax </BLOCKQUOTE> 
            \see imageMax for more information */ 
        inline virtual double getImageMax() const  
        {
            return this->imageMax; 
        }
        inline void setImageMaxR(double m) {
            setImageMax(m);
        }
        /*! \brief sets the property imageMinG ( \copybrief imageMinG ) to the specified \a __value. 
            \details Description of the parameter imageMinG is: <BLOCKQUOTE>\copydoc imageMinG </BLOCKQUOTE> 
            \see imageMinG for more information */ 
        inline virtual void setImageMinG(double __value)
        {
            this->imageMinG = __value;
        } 
        /*! \brief returns the property imageMinG ( \copybrief imageMinG ). 
            \details Description of the parameter imageMinG is: <BLOCKQUOTE>\copydoc imageMinG </BLOCKQUOTE> 
            \see imageMinG for more information */ 
        inline virtual double getImageMinG() const  
        {
            return this->imageMinG; 
        }
        /*! \brief sets the property imageMaxG ( \copybrief imageMaxG ) to the specified \a __value. 
            \details Description of the parameter imageMaxG is: <BLOCKQUOTE>\copydoc imageMaxG </BLOCKQUOTE> 
            \see imageMaxG for more information */ 
        inline virtual void setImageMaxG(double __value)
        {
            this->imageMaxG = __value;
        } 
        /*! \brief returns the property imageMaxG ( \copybrief imageMaxG ). 
            \details Description of the parameter imageMaxG is: <BLOCKQUOTE>\copydoc imageMaxG </BLOCKQUOTE> 
            \see imageMaxG for more information */ 
        inline virtual double getImageMaxG() const  
        {
            return this->imageMaxG; 
        }
        /*! \brief sets the property imageMinB ( \copybrief imageMinB ) to the specified \a __value. 
            \details Description of the parameter imageMinB is: <BLOCKQUOTE>\copydoc imageMinB </BLOCKQUOTE> 
            \see imageMinB for more information */ 
        inline virtual void setImageMinB(double __value)
        {
            this->imageMinB = __value;
        } 
        /*! \brief returns the property imageMinB ( \copybrief imageMinB ). 
            \details Description of the parameter imageMinB is: <BLOCKQUOTE>\copydoc imageMinB </BLOCKQUOTE> 
            \see imageMinB for more information */ 
        inline virtual double getImageMinB() const  
        {
            return this->imageMinB; 
        }
        /*! \brief sets the property imageMaxB ( \copybrief imageMaxB ) to the specified \a __value. 
            \details Description of the parameter imageMaxB is: <BLOCKQUOTE>\copydoc imageMaxB </BLOCKQUOTE> 
            \see imageMaxB for more information */ 
        inline virtual void setImageMaxB(double __value)
        {
            this->imageMaxB = __value;
        } 
        /*! \brief returns the property imageMaxB ( \copybrief imageMaxB ). 
            \details Description of the parameter imageMaxB is: <BLOCKQUOTE>\copydoc imageMaxB </BLOCKQUOTE> 
            \see imageMaxB for more information */ 
        inline virtual double getImageMaxB() const  
        {
            return this->imageMaxB; 
        }
        /*! \brief sets the property autoImageRange ( \copybrief autoImageRange ) to the specified \a __value. 
            \details Description of the parameter autoImageRange is: <BLOCKQUOTE>\copydoc autoImageRange </BLOCKQUOTE> 
            \see autoImageRange for more information */ 
        inline virtual void setAutoImageRange(bool __value)
        {
            this->autoImageRange = __value;
        } 
        /*! \brief returns the property autoImageRange ( \copybrief autoImageRange ). 
            \details Description of the parameter autoImageRange is: <BLOCKQUOTE>\copydoc autoImageRange </BLOCKQUOTE> 
            \see autoImageRange for more information */ 
        inline virtual bool getAutoImageRange() const  
        {
            return this->autoImageRange; 
        }
        /*! \brief sets the property imageName ( \copybrief imageName ) to the specified \a __value. 
            \details Description of the parameter imageName is: <BLOCKQUOTE>\copydoc imageName </BLOCKQUOTE> 
            \see imageName for more information */ 
        inline virtual void setImageName(const QString & __value)  
        {
            this->imageName = __value;
        } 
        /*! \brief returns the property imageName ( \copybrief imageName ). 
            \details Description of the parameter imageName is: <BLOCKQUOTE>\copydoc imageName </BLOCKQUOTE> 
            \see imageName for more information */ 
        inline virtual QString getImageName() const  
        {
            return this->imageName; 
        }
        inline void setImageNameR(const QString& m) {
            setImageName(m);
        }
        /*! \brief sets the property imageNameG ( \copybrief imageNameG ) to the specified \a __value. 
            \details Description of the parameter imageNameG is: <BLOCKQUOTE>\copydoc imageNameG </BLOCKQUOTE> 
            \see imageNameG for more information */ 
        inline virtual void setImageNameG(const QString & __value)  
        {
            this->imageNameG = __value;
        } 
        /*! \brief returns the property imageNameG ( \copybrief imageNameG ). 
            \details Description of the parameter imageNameG is: <BLOCKQUOTE>\copydoc imageNameG </BLOCKQUOTE> 
            \see imageNameG for more information */ 
        inline virtual QString getImageNameG() const  
        {
            return this->imageNameG; 
        }
        /*! \brief sets the property imageNameB ( \copybrief imageNameB ) to the specified \a __value. 
            \details Description of the parameter imageNameB is: <BLOCKQUOTE>\copydoc imageNameB </BLOCKQUOTE> 
            \see imageNameB for more information */ 
        inline virtual void setImageNameB(const QString & __value)  
        {
            this->imageNameB = __value;
        } 
        /*! \brief returns the property imageNameB ( \copybrief imageNameB ). 
            \details Description of the parameter imageNameB is: <BLOCKQUOTE>\copydoc imageNameB </BLOCKQUOTE> 
            \see imageNameB for more information */ 
        inline virtual QString getImageNameB() const  
        {
            return this->imageNameB; 
        }
        /*! \brief sets the property imageNameFontName ( \copybrief imageNameFontName ) to the specified \a __value. 
            \details Description of the parameter imageNameFontName is: <BLOCKQUOTE>\copydoc imageNameFontName </BLOCKQUOTE> 
            \see imageNameFontName for more information */ 
        inline virtual void setImageNameFontName(const QString & __value)  
        {
            this->imageNameFontName = __value;
        } 
        /*! \brief returns the property imageNameFontName ( \copybrief imageNameFontName ). 
            \details Description of the parameter imageNameFontName is: <BLOCKQUOTE>\copydoc imageNameFontName </BLOCKQUOTE> 
            \see imageNameFontName for more information */ 
        inline virtual QString getImageNameFontName() const  
        {
            return this->imageNameFontName; 
        }
        /*! \brief sets the property imageNameFontSize ( \copybrief imageNameFontSize ) to the specified \a __value. 
            \details Description of the parameter imageNameFontSize is: <BLOCKQUOTE>\copydoc imageNameFontSize </BLOCKQUOTE> 
            \see imageNameFontSize for more information */ 
        inline virtual void setImageNameFontSize(double __value)
        {
            this->imageNameFontSize = __value;
        } 
        /*! \brief returns the property imageNameFontSize ( \copybrief imageNameFontSize ). 
            \details Description of the parameter imageNameFontSize is: <BLOCKQUOTE>\copydoc imageNameFontSize </BLOCKQUOTE> 
            \see imageNameFontSize for more information */ 
        inline virtual double getImageNameFontSize() const  
        {
            return this->imageNameFontSize; 
        }
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */ 
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxis() const { return this->colorBarRightAxis; }
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */ 
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const { return this->colorBarTopAxis; }
        /*! \brief returns the property colorBarRightAxisG ( \copybrief colorBarRightAxisG ). \details Description of the parameter colorBarRightAxisG is:  <BLOCKQUOTE>\copydoc colorBarRightAxisG </BLOCKQUOTE>. \see colorBarRightAxisG for more information */ 
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxisG() const { return this->colorBarRightAxisG; }
        /*! \brief returns the property colorBarTopAxisG ( \copybrief colorBarTopAxisG ). \details Description of the parameter colorBarTopAxisG is:  <BLOCKQUOTE>\copydoc colorBarTopAxisG </BLOCKQUOTE>. \see colorBarTopAxisG for more information */ 
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxisG() const { return this->colorBarTopAxisG; }
        /*! \brief returns the property colorBarRightAxisB ( \copybrief colorBarRightAxisB ). \details Description of the parameter colorBarRightAxisB is:  <BLOCKQUOTE>\copydoc colorBarRightAxisB </BLOCKQUOTE>. \see colorBarRightAxisB for more information */ 
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxisB() const { return this->colorBarRightAxisB; }
        /*! \brief returns the property colorBarTopAxisB ( \copybrief colorBarTopAxisB ). \details Description of the parameter colorBarTopAxisB is:  <BLOCKQUOTE>\copydoc colorBarTopAxisB </BLOCKQUOTE>. \see colorBarTopAxisB for more information */ 
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxisB() const { return this->colorBarTopAxisB; }
        /*! \brief sets the property colorBarTopVisible ( \copybrief colorBarTopVisible ) to the specified \a __value. 
            \details Description of the parameter colorBarTopVisible is: <BLOCKQUOTE>\copydoc colorBarTopVisible </BLOCKQUOTE> 
            \see colorBarTopVisible for more information */ 
        inline virtual void setColorBarTopVisible(bool __value)
        {
            this->colorBarTopVisible = __value;
        } 
        /*! \brief returns the property colorBarTopVisible ( \copybrief colorBarTopVisible ). 
            \details Description of the parameter colorBarTopVisible is: <BLOCKQUOTE>\copydoc colorBarTopVisible </BLOCKQUOTE> 
            \see colorBarTopVisible for more information */ 
        inline virtual bool getColorBarTopVisible() const  
        {
            return this->colorBarTopVisible; 
        }
        /*! \brief sets the property colorBarRightVisible ( \copybrief colorBarRightVisible ) to the specified \a __value. 
            \details Description of the parameter colorBarRightVisible is: <BLOCKQUOTE>\copydoc colorBarRightVisible </BLOCKQUOTE> 
            \see colorBarRightVisible for more information */ 
        inline virtual void setColorBarRightVisible(bool __value)
        {
            this->colorBarRightVisible = __value;
        } 
        /*! \brief returns the property colorBarRightVisible ( \copybrief colorBarRightVisible ). 
            \details Description of the parameter colorBarRightVisible is: <BLOCKQUOTE>\copydoc colorBarRightVisible </BLOCKQUOTE> 
            \see colorBarRightVisible for more information */ 
        inline virtual bool getColorBarRightVisible() const  
        {
            return this->colorBarRightVisible; 
        }
        /*! \brief sets the property colorbarsSideBySide ( \copybrief colorbarsSideBySide ) to the specified \a __value. 
            \details Description of the parameter colorbarsSideBySide is: <BLOCKQUOTE>\copydoc colorbarsSideBySide </BLOCKQUOTE> 
            \see colorbarsSideBySide for more information */ 
        inline virtual void setColorbarsSideBySide(bool __value)
        {
            this->colorbarsSideBySide = __value;
        } 
        /*! \brief returns the property colorbarsSideBySide ( \copybrief colorbarsSideBySide ). 
            \details Description of the parameter colorbarsSideBySide is: <BLOCKQUOTE>\copydoc colorbarsSideBySide </BLOCKQUOTE> 
            \see colorbarsSideBySide for more information */ 
        inline virtual bool getColorbarsSideBySide() const  
        {
            return this->colorbarsSideBySide; 
        }
        /*! \brief sets the property rgbMode ( \copybrief rgbMode ) to the specified \a __value. 
            \details Description of the parameter rgbMode is: <BLOCKQUOTE>\copydoc rgbMode </BLOCKQUOTE> 
            \see rgbMode for more information */ 
        inline virtual void setRgbMode(const JKQTPRGBMathImageRGBMode & __value)  
        {
            this->rgbMode = __value;
        } 
        /*! \brief returns the property rgbMode ( \copybrief rgbMode ). 
            \details Description of the parameter rgbMode is: <BLOCKQUOTE>\copydoc rgbMode </BLOCKQUOTE> 
            \see rgbMode for more information */ 
        inline virtual JKQTPRGBMathImageRGBMode getRgbMode() const  
        {
            return this->rgbMode; 
        }

        QVector<double> getDataGAsDoubleVector() const;
        QVector<double> getDataBAsDoubleVector() const;


        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) override;

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) override;

        /*! \brief return the plotted image only as a QImage */
        virtual QImage drawImage();

        /** \brief determine min/max data value of the image */
        virtual void getDataMinMaxG(double& imin, double& imax);
        /** \brief determine min/max data value of the image */
        virtual void getDataMinMaxB(double& imin, double& imax);

        /** \brief return the value (raw data!) of the contained image at the given coordinate */
        double getValueAt(double x, double y, int channel=0) ;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

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
        JKQTPVerticalIndependentAxis* colorBarRightAxis;
        JKQTPHorizontalIndependentAxis* colorBarTopAxis;
        JKQTPVerticalIndependentAxis* colorBarRightAxisG;
        JKQTPHorizontalIndependentAxis* colorBarTopAxisG;
        JKQTPVerticalIndependentAxis* colorBarRightAxisB;
        JKQTPHorizontalIndependentAxis* colorBarTopAxisB;


        double internalDataMinG;
        double internalDataMaxG;
        double internalDataMinB;
        double internalDataMaxB;

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
    public:
        virtual void setTitle(const QString& title) override;
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();
};




/*! \brief class to plot an image from an 2-dimensional array of values stored in a column of the datastore
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_imageplot.png
    \image html jkqtplotter_simpletest_imageplot_modifier.png
    \image html jkqtplotter_simpletest_imageplot__smallscaletransparent.png

 */
class LIB_EXPORT JKQTPColumnMathImage: public JKQTPMathImage {
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

        /*! \brief sets the property imageColumn ( \copybrief imageColumn ) to the specified \a __value. 
            \details Description of the parameter imageColumn is: <BLOCKQUOTE>\copydoc imageColumn </BLOCKQUOTE> 
            \see imageColumn for more information */ 
        inline virtual void setImageColumn(int __value)
        {
            this->imageColumn = __value;
        } 
        /*! \brief returns the property imageColumn ( \copybrief imageColumn ). 
            \details Description of the parameter imageColumn is: <BLOCKQUOTE>\copydoc imageColumn </BLOCKQUOTE> 
            \see imageColumn for more information */ 
        inline virtual int getImageColumn() const  
        {
            return this->imageColumn; 
        }
        /*! \brief sets the property modifierColumn ( \copybrief modifierColumn ) to the specified \a __value. 
            \details Description of the parameter modifierColumn is: <BLOCKQUOTE>\copydoc modifierColumn </BLOCKQUOTE> 
            \see modifierColumn for more information */ 
        inline virtual void setModifierColumn(int __value)
        {
            this->modifierColumn = __value;
        } 
        /*! \brief returns the property modifierColumn ( \copybrief modifierColumn ). 
            \details Description of the parameter modifierColumn is: <BLOCKQUOTE>\copydoc modifierColumn </BLOCKQUOTE> 
            \see modifierColumn for more information */ 
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


/*! \brief like JKQTPRGBMathImage but reads images from columns of the datastore
    \ingroup jkqtplotter_imagelots_elements


    \image html jkqtplotter_simpletest_rgbimageplot_opencv.png
 */
class LIB_EXPORT JKQTPColumnRGBMathImage: public JKQTPRGBMathImage {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPColumnRGBMathImage(JKQTBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPColumnRGBMathImage(JKQTPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int Nx, int Ny, JKQTPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int Nx, int Ny, JKQTPlotter* parent);
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, int Nx, int Ny, JKQTPlotter* parent);

        /*! \brief sets the property imageRColumn ( \copybrief imageRColumn ) to the specified \a __value. 
            \details Description of the parameter imageRColumn is: <BLOCKQUOTE>\copydoc imageRColumn </BLOCKQUOTE> 
            \see imageRColumn for more information */ 
        inline virtual void setImageRColumn(int __value)
        {
            this->imageRColumn = __value;
        } 
        /*! \brief returns the property imageRColumn ( \copybrief imageRColumn ). 
            \details Description of the parameter imageRColumn is: <BLOCKQUOTE>\copydoc imageRColumn </BLOCKQUOTE> 
            \see imageRColumn for more information */ 
        inline virtual int getImageRColumn() const  
        {
            return this->imageRColumn; 
        }
        /*! \brief sets the property imageGColumn ( \copybrief imageGColumn ) to the specified \a __value. 
            \details Description of the parameter imageGColumn is: <BLOCKQUOTE>\copydoc imageGColumn </BLOCKQUOTE> 
            \see imageGColumn for more information */ 
        inline virtual void setImageGColumn(int __value)
        {
            this->imageGColumn = __value;
        } 
        /*! \brief returns the property imageGColumn ( \copybrief imageGColumn ). 
            \details Description of the parameter imageGColumn is: <BLOCKQUOTE>\copydoc imageGColumn </BLOCKQUOTE> 
            \see imageGColumn for more information */ 
        inline virtual int getImageGColumn() const  
        {
            return this->imageGColumn; 
        }
        /*! \brief sets the property imageBColumn ( \copybrief imageBColumn ) to the specified \a __value. 
            \details Description of the parameter imageBColumn is: <BLOCKQUOTE>\copydoc imageBColumn </BLOCKQUOTE> 
            \see imageBColumn for more information */ 
        inline virtual void setImageBColumn(int __value)
        {
            this->imageBColumn = __value;
        } 
        /*! \brief returns the property imageBColumn ( \copybrief imageBColumn ). 
            \details Description of the parameter imageBColumn is: <BLOCKQUOTE>\copydoc imageBColumn </BLOCKQUOTE> 
            \see imageBColumn for more information */ 
        inline virtual int getImageBColumn() const  
        {
            return this->imageBColumn; 
        }
        /*! \brief sets the property modifierColumn ( \copybrief modifierColumn ) to the specified \a __value. 
            \details Description of the parameter modifierColumn is: <BLOCKQUOTE>\copydoc modifierColumn </BLOCKQUOTE> 
            \see modifierColumn for more information */ 
        inline virtual void setModifierColumn(int __value)
        {
            this->modifierColumn = __value;
        } 
        /*! \brief returns the property modifierColumn ( \copybrief modifierColumn ). 
            \details Description of the parameter modifierColumn is: <BLOCKQUOTE>\copydoc modifierColumn </BLOCKQUOTE> 
            \see modifierColumn for more information */ 
        inline virtual int getModifierColumn() const  
        {
            return this->modifierColumn; 
        }
        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;

    protected:
        /** \brief image column for R channel */
        int imageRColumn;
        /** \brief image column for G channel */
        int imageGColumn;
        /** \brief image column for B channel */
        int imageBColumn;
        /** \brief  column containing the modifier image */
        int modifierColumn;

        void ensureImageData() override;
};


/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots_elements

 */
class LIB_EXPORT JKQTPOverlayImage: public JKQTPImageBase {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPOverlayImage(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTBasePlotter* parent=nullptr);
        JKQTPOverlayImage(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPOverlayImage(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTPlotter* parent);
        JKQTPOverlayImage(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /*! \brief return the plotted image only as a QImage */
        virtual QImage drawImage();

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /*! \brief sets the property trueColor ( \copybrief trueColor ) to the specified \a __value. 
            \details Description of the parameter trueColor is: <BLOCKQUOTE>\copydoc trueColor </BLOCKQUOTE> 
            \see trueColor for more information */ 
        inline virtual void setTrueColor(const QColor & __value)  
        {
            this->trueColor = __value;
        } 
        /*! \brief returns the property trueColor ( \copybrief trueColor ). 
            \details Description of the parameter trueColor is: <BLOCKQUOTE>\copydoc trueColor </BLOCKQUOTE> 
            \see trueColor for more information */ 
        inline virtual QColor getTrueColor() const  
        {
            return this->trueColor; 
        }
        /*! \brief sets the property falseColor ( \copybrief falseColor ) to the specified \a __value. 
            \details Description of the parameter falseColor is: <BLOCKQUOTE>\copydoc falseColor </BLOCKQUOTE> 
            \see falseColor for more information */ 
        inline virtual void setFalseColor(const QColor & __value)  
        {
            this->falseColor = __value;
        } 
        /*! \brief returns the property falseColor ( \copybrief falseColor ). 
            \details Description of the parameter falseColor is: <BLOCKQUOTE>\copydoc falseColor </BLOCKQUOTE> 
            \see falseColor for more information */ 
        inline virtual QColor getFalseColor() const  
        {
            return this->falseColor; 
        }
        /*! \brief sets the property Nx ( \copybrief Nx ) to the specified \a __value. 
            \details Description of the parameter Nx is: <BLOCKQUOTE>\copydoc Nx </BLOCKQUOTE> 
            \see Nx for more information */ 
        inline virtual void setNx(int __value)
        {
            this->Nx = __value;
        } 
        /*! \brief returns the property Nx ( \copybrief Nx ). 
            \details Description of the parameter Nx is: <BLOCKQUOTE>\copydoc Nx </BLOCKQUOTE> 
            \see Nx for more information */ 
        inline virtual int getNx() const
        {
            return this->Nx; 
        }
        /*! \brief sets the property Ny ( \copybrief Ny ) to the specified \a __value. 
            \details Description of the parameter Ny is: <BLOCKQUOTE>\copydoc Ny </BLOCKQUOTE> 
            \see Ny for more information */ 
        inline virtual void setNy(int __value)
        {
            this->Ny = __value;
        } 
        /*! \brief returns the property Ny ( \copybrief Ny ). 
            \details Description of the parameter Ny is: <BLOCKQUOTE>\copydoc Ny </BLOCKQUOTE> 
            \see Ny for more information */ 
        inline virtual int getNy() const
        {
            return this->Ny; 
        }
        /*! \brief sets the property data ( \copybrief data ) to the specified \a __value. 
            \details Description of the parameter data is: <BLOCKQUOTE>\copydoc data </BLOCKQUOTE> 
            \see data for more information */ 
        inline virtual void setData(bool*  __value)
        {
            this->data = __value;
        } 
        /*! \brief returns the property data ( \copybrief data ). 
            \details Description of the parameter data is: <BLOCKQUOTE>\copydoc data </BLOCKQUOTE> 
            \see data for more information */ 
        inline virtual bool* getData() const  
        {
            return this->data; 
        }

        void setData(bool* data, int Nx, int Ny);

        QVector<double> getDataAsDoubleVector() const;

    protected:
        /** \brief points to the data array, holding the image */
        bool* data;
        /** \brief width of the data array data in pt */
        int Nx;
        /** \brief height of the data array data in pt */
        int Ny;

        /** \brief color for \c true pixels */
        QColor trueColor;
        /** \brief color for \c false pixels */
        QColor falseColor;

    protected:
        QAction* actSaveImage;
        QAction* actCopyImage;
    public:
        virtual void setTitle(const QString& title) override;
        virtual void setParent(JKQTBasePlotter* parent) override;
    public slots:
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        void copyImagePlotAsImage();

};



/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots_elements

    In contrast to JKQTPOverlayImage this class draws ist contents as rectangles, not as semi-transparent image. This may lead to nicer results,but could be slower.
    Also it is possible to draw other types of markers (cross, circles, ...)

 */
class LIB_EXPORT JKQTPOverlayImageEnhanced: public JKQTPOverlayImage {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTBasePlotter* parent=nullptr);
        JKQTPOverlayImageEnhanced(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTPlotter* parent);
        JKQTPOverlayImageEnhanced(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

        /*! \brief sets the property symbol ( \copybrief symbol ) to the specified \a __value. 
            \details Description of the parameter symbol is: <BLOCKQUOTE>\copydoc symbol </BLOCKQUOTE> 
            \see symbol for more information */ 
        inline virtual void setSymbol(const JKQTPGraphSymbols & __value)  
        {
            this->symbol = __value;
        } 
        /*! \brief returns the property symbol ( \copybrief symbol ). 
            \details Description of the parameter symbol is: <BLOCKQUOTE>\copydoc symbol </BLOCKQUOTE> 
            \see symbol for more information */ 
        inline virtual JKQTPGraphSymbols getSymbol() const  
        {
            return this->symbol; 
        }
        /*! \brief sets the property symbolWidth ( \copybrief symbolWidth ) to the specified \a __value. 
            \details Description of the parameter symbolWidth is: <BLOCKQUOTE>\copydoc symbolWidth </BLOCKQUOTE> 
            \see symbolWidth for more information */ 
        inline virtual void setSymbolWidth(double __value)
        {
            this->symbolWidth = __value;
        } 
        /*! \brief returns the property symbolWidth ( \copybrief symbolWidth ). 
            \details Description of the parameter symbolWidth is: <BLOCKQUOTE>\copydoc symbolWidth </BLOCKQUOTE> 
            \see symbolWidth for more information */ 
        inline virtual double getSymbolWidth() const  
        {
            return this->symbolWidth; 
        }
        /*! \brief sets the property drawAsRectangles ( \copybrief drawAsRectangles ) to the specified \a __value. 
            \details Description of the parameter drawAsRectangles is: <BLOCKQUOTE>\copydoc drawAsRectangles </BLOCKQUOTE> 
            \see drawAsRectangles for more information */ 
        inline virtual void setDrawAsRectangles(bool __value)
        {
            this->drawAsRectangles = __value;
        } 
        /*! \brief returns the property drawAsRectangles ( \copybrief drawAsRectangles ). 
            \details Description of the parameter drawAsRectangles is: <BLOCKQUOTE>\copydoc drawAsRectangles </BLOCKQUOTE> 
            \see drawAsRectangles for more information */ 
        inline virtual bool getDrawAsRectangles() const  
        {
            return this->drawAsRectangles; 
        }
        /*! \brief sets the property rectanglesAsImageOverlay ( \copybrief rectanglesAsImageOverlay ) to the specified \a __value. 
            \details Description of the parameter rectanglesAsImageOverlay is: <BLOCKQUOTE>\copydoc rectanglesAsImageOverlay </BLOCKQUOTE> 
            \see rectanglesAsImageOverlay for more information */ 
        inline virtual void setRectanglesAsImageOverlay(bool __value)
        {
            this->rectanglesAsImageOverlay = __value;
        } 
        /*! \brief returns the property rectanglesAsImageOverlay ( \copybrief rectanglesAsImageOverlay ). 
            \details Description of the parameter rectanglesAsImageOverlay is: <BLOCKQUOTE>\copydoc rectanglesAsImageOverlay </BLOCKQUOTE> 
            \see rectanglesAsImageOverlay for more information */ 
        inline virtual bool getRectanglesAsImageOverlay() const  
        {
            return this->rectanglesAsImageOverlay; 
        }
        /*! \brief sets the property symbolSizeFactor ( \copybrief symbolSizeFactor ) to the specified \a __value. 
            \details Description of the parameter symbolSizeFactor is: <BLOCKQUOTE>\copydoc symbolSizeFactor </BLOCKQUOTE> 
            \see symbolSizeFactor for more information */ 
        inline virtual void setSymbolSizeFactor(double __value)
        {
            this->symbolSizeFactor = __value;
        } 
        /*! \brief returns the property symbolSizeFactor ( \copybrief symbolSizeFactor ). 
            \details Description of the parameter symbolSizeFactor is: <BLOCKQUOTE>\copydoc symbolSizeFactor </BLOCKQUOTE> 
            \see symbolSizeFactor for more information */ 
        inline virtual double getSymbolSizeFactor() const  
        {
            return this->symbolSizeFactor; 
        }

    protected:
        /** \brief which symbol to use for the datapoints */
        JKQTPGraphSymbols symbol;
        /** \brief width (in pt) of the lines used to plot the symbol for the data points */
        double symbolWidth;

        /** \brief indicates whether to draw filled rectangles (\c false, default) or symbols */
        bool drawAsRectangles;

        /** \brief a rescaling factor  for the symbols */
        double symbolSizeFactor;

        /** \brief when \c drawAsRectangles==true this reactivates the drawing from JKQTPOverlayImage, i.e. the overlay is not drawn as separated rectangles, but as an overlay image */
        bool rectanglesAsImageOverlay;

};




/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots_elements

    In contrast to JKQTPOverlayImage this class draws ist contents as rectangles, not as semi-transparent image. This may lead to nicer results,but could be slower.
    Also it is possible to draw other types of markers (cross, circles, ...)

 */
class LIB_EXPORT JKQTPColumnOverlayImageEnhanced: public JKQTPOverlayImageEnhanced {
        Q_OBJECT
    public:
        JKQTPColumnOverlayImageEnhanced(JKQTBasePlotter* parent=nullptr);
        JKQTPColumnOverlayImageEnhanced(JKQTPlotter* parent);

        /*! \brief sets the property imageColumn ( \copybrief imageColumn ) to the specified \a __value. 
            \details Description of the parameter imageColumn is: <BLOCKQUOTE>\copydoc imageColumn </BLOCKQUOTE> 
            \see imageColumn for more information */ 
        inline virtual void setImageColumn(int __value)
        {
            this->imageColumn = __value;
        } 
        /*! \brief returns the property imageColumn ( \copybrief imageColumn ). 
            \details Description of the parameter imageColumn is: <BLOCKQUOTE>\copydoc imageColumn </BLOCKQUOTE> 
            \see imageColumn for more information */ 
        inline virtual int getImageColumn() const  
        {
            return this->imageColumn; 
        }
        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


    protected:
        /** \brief top color bar visible */
        int imageColumn;
};



/*! \brief class for a contour plot
 *  \ingroup jkqtplotter_imagelots_contour
 *  calculates the contour of a given image using the CONREC algorithm
 *  <a href="http://paulbourke.net/papers/conrec/">http://paulbourke.net/papers/conrec/</a>
 *  The implementation for Qt is adapted from Qwt
 *  <a href="http://qwt.sourceforge.net/">http://qwt.sourceforge.net/</a>
 *
 *  The contour lines are calculated only once and then redrawn to save plotting time.
 *  Make sure you add a new graph when your data changes. The contour levels can be defined in
 *  contourLevels, each elemt in the list represents a contour plane. You can also create contour
 *  levels with createContourLevels(). The levels are linearly spaced between the maximum and minimum
 *  value in your data. For logarithmic data, use createContourLevelsLog() to create contour levels
 *  with logarithmic spacing.
 *
 * \author Sebastian Isbaner, 2013-2014
 *
 * \image html JKQTPContour.png
 */
class LIB_EXPORT JKQTPContour: public JKQTPMathImage {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPContour(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPContour(double x, double y, double width, double height, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, DataType datatype = JKQTPMathImageBase::DoubleArray, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPContour(JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPContour(double x, double y, double width, double height, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, DataType datatype , JKQTPlotter* parent);
        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief creates at nLevels contour levels linearly spaced between the data's minimum and maximum values */
        void createContourLevels(int nLevels=3);
        /** \brief creates at least nLevels contour levels with logarithmic spacing. FIXME: Has not been tested yet */
        void createContourLevelsLog(int nLevels=3,int m=2);

        /*! \brief sets the property lineColor ( \copybrief lineColor ) to the specified \a __value. 
            \details Description of the parameter lineColor is: <BLOCKQUOTE>\copydoc lineColor </BLOCKQUOTE> 
            \see lineColor for more information */ 
        inline virtual void setLineColor(const QColor & __value)  
        {
            this->lineColor = __value;
        } 
        /*! \brief returns the property lineColor ( \copybrief lineColor ). 
            \details Description of the parameter lineColor is: <BLOCKQUOTE>\copydoc lineColor </BLOCKQUOTE> 
            \see lineColor for more information */ 
        inline virtual QColor getLineColor() const  
        {
            return this->lineColor; 
        }
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth ( \copybrief lineWidth ) to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth ( \copybrief lineWidth ). 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property ignoreOnPlane ( \copybrief ignoreOnPlane ) to the specified \a __value. 
            \details Description of the parameter ignoreOnPlane is: <BLOCKQUOTE>\copydoc ignoreOnPlane </BLOCKQUOTE> 
            \see ignoreOnPlane for more information */ 
        inline virtual void setIgnoreOnPlane(bool __value)
        {
            this->ignoreOnPlane = __value;
        } 
        /*! \brief returns the property ignoreOnPlane ( \copybrief ignoreOnPlane ). 
            \details Description of the parameter ignoreOnPlane is: <BLOCKQUOTE>\copydoc ignoreOnPlane </BLOCKQUOTE> 
            \see ignoreOnPlane for more information */ 
        inline virtual bool getIgnoreOnPlane() const  
        {
            return this->ignoreOnPlane; 
        }
        /*! \brief sets the property numberOfLevels ( \copybrief numberOfLevels ) to the specified \a __value. 
            \details Description of the parameter numberOfLevels is: <BLOCKQUOTE>\copydoc numberOfLevels </BLOCKQUOTE> 
            \see numberOfLevels for more information */ 
        inline virtual void setNumberOfLevels(int __value)
        {
            this->numberOfLevels = __value;
        } 
        /*! \brief returns the property numberOfLevels ( \copybrief numberOfLevels ). 
            \details Description of the parameter numberOfLevels is: <BLOCKQUOTE>\copydoc numberOfLevels </BLOCKQUOTE> 
            \see numberOfLevels for more information */ 
        inline virtual int getNumberOfLevels() const  
        {
            return this->numberOfLevels; 
        }
        /*! \brief sets the property colorFromPalette ( \copybrief colorFromPalette ) to the specified \a __value. 
            \details Description of the parameter colorFromPalette is: <BLOCKQUOTE>\copydoc colorFromPalette </BLOCKQUOTE> 
            \see colorFromPalette for more information */ 
        inline virtual void setColorFromPalette(bool __value)
        {
            this->colorFromPalette = __value;
        } 
        /*! \brief returns the property colorFromPalette ( \copybrief colorFromPalette ). 
            \details Description of the parameter colorFromPalette is: <BLOCKQUOTE>\copydoc colorFromPalette </BLOCKQUOTE> 
            \see colorFromPalette for more information */ 
        inline virtual bool getColorFromPalette() const  
        {
            return this->colorFromPalette; 
        }
        /*! \brief sets the property contourLevels ( \copybrief contourLevels ) to the specified \a __value. 
            \details Description of the parameter contourLevels is: <BLOCKQUOTE>\copydoc contourLevels </BLOCKQUOTE> 
            \see contourLevels for more information */ 
        inline virtual void setContourLevels(const QList<double> & __value)  
        {
            this->contourLevels = __value;
        } 
        /*! \brief returns the property contourLevels ( \copybrief contourLevels ). 
            \details Description of the parameter contourLevels is: <BLOCKQUOTE>\copydoc contourLevels </BLOCKQUOTE> 
            \see contourLevels for more information */ 
        inline virtual QList<double> getContourLevels() const  
        {
            return this->contourLevels; 
        }
        /*! \brief sets the property relativeLevels ( \copybrief relativeLevels ) to the specified \a __value. 
            \details Description of the parameter relativeLevels is: <BLOCKQUOTE>\copydoc relativeLevels </BLOCKQUOTE> 
            \see relativeLevels for more information */ 
        inline virtual void setRelativeLevels(bool __value)
        {
            this->relativeLevels = __value;
        } 
        /*! \brief returns the property relativeLevels ( \copybrief relativeLevels ). 
            \details Description of the parameter relativeLevels is: <BLOCKQUOTE>\copydoc relativeLevels </BLOCKQUOTE> 
            \see relativeLevels for more information */ 
        inline virtual bool getRelativeLevels() const  
        {
            return this->relativeLevels; 
        }
        /** convenience function to work with JKQTPDatastore */
        void setImageColumn(size_t columnID);

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
        virtual void ensureImageData() override;

private:
        double value(int xIdx, int yIdx);
        /// gives the intersection line of a plane defined by three vertices with a contour level in the x-y plane of heigth level
        bool intersect(QLineF &line, const QVector3D &vertex1,const QVector3D &vertex2,const QVector3D &vertex3,double level);
        /**
         * @brief interpolatePoint linear interpolation of a line to the x-y plane using the z-value relative to level
         * @param point1 start point of the line
         * @param point2 end point of the line
         * @param level the heigth for the interpolation (this z-value is projected onto the xy-plane)
         * @return the interpolated point in the x-y plane
         */
        QPointF interpolatePoint(const QVector3D &point1, const QVector3D &point2,double level);
        /**
         * @brief compare2level checks if the vertex lies above, under or on the contour plane level
         * @param vertex
         * @param level contour plane
         * @return above (1), on the plane (0), below (-1);
         */
        int compare2level(const QVector3D &vertex, double level);
        /**
         * @brief calcContourLines calculates the contour lines using the CONREC algorithm
         */
        void calcContourLines(QList<QVector<QLineF > > &ContourLines);
        /// the contour lines as vector of single lines (one for each triangle). the list index refers to the contour level.
        QList<QVector<QLineF > > contourLines; // contour levels, squares on grid, line

};





#endif // jkqtpgraphsimage_H

