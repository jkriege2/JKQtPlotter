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
        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPImageBase(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
        JKQTPImageBase(double x, double y, double width, double height, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPImageBase(JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
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

        /*! \copydoc x */
        void setX(double __value);
        /*! \copydoc x */
        double getX() const;
        /*! \copydoc y */
        void setY(double __value);
        /*! \copydoc y */
        double getY() const;
        /*! \copydoc width */
        void setWidth(double __value);
        /*! \copydoc width */
        double getWidth() const;
        /*! \copydoc height */
        void setHeight(double __value);
        /*! \copydoc height */
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



        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPMathImageBase(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
        JKQTPMathImageBase(double x, double y, double width, double height, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param datatype datatype of the image given in \a data
         * \param data points to an image to be plotted (of size \a Nx * \a Ny )
         * \param Nx width (in number of pixels) of \a data
         * \param Ny height (in number of pixels) of \a data
         * \param parent parent plotter object
         *
         */
        JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);


        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPMathImageBase(JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
        JKQTPMathImageBase(double x, double y, double width, double height, JKQTPlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param datatype datatype of the image given in \a data
         * \param data points to an image to be plotted (of size \a Nx * \a Ny )
         * \param Nx width (in number of pixels) of \a data
         * \param Ny height (in number of pixels) of \a data
         * \param parent parent plotter object
         *
         */
        JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPlotter* parent);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

        /*! \copydoc Nx */
        void setNx(int __value);
        /*! \copydoc Nx */
        int getNx() const;
        /*! \copydoc Ny */
        void setNy(int __value);
        /*! \copydoc Ny */
        int getNy() const;
        /*! \copydoc data */
        virtual void setData(void* __value);
        /*! \copydoc data */
        virtual void *getData() const;
        /*! \copydoc datatype */
        virtual void setDatatype(DataType __value);
        /*! \copydoc datatype */
        virtual DataType getDatatype() const;
        /*! \copydoc dataModifier */
        virtual void setDataModifier(void* __value);
        /*! \copydoc dataModifier */
        virtual void *getDataModifier() const;
        /*! \copydoc datatypeModifier */
        virtual void setDatatypeModifier(DataType __value);
        /*! \copydoc datatypeModifier */
        virtual DataType getDatatypeModifier() const;
        /*! \copydoc modifierMode */
        void setModifierMode(const ModifierMode & __value);
        /*! \copydoc modifierMode */
        ModifierMode getModifierMode() const;

        /** \brief sets dataModifier (\copybrief dataModifier) and datatypeModifier (\copybrief datatypeModifier) */
        virtual void setDataModifier(void* data, DataType datatype);
        /** \brief sets data (\copybrief data) and datatype (\copybrief datatype), as well as the size of data (Nx: \copybrief Nx and Ny: \copybrief Ny) */
        virtual void setData(void* data, int Nx, int Ny, DataType datatype);
        /** \brief sets data (\copybrief data), as well as the size of data (Nx: \copybrief Nx and Ny: \copybrief Ny) */
        virtual void setData(void* data, int Nx, int Ny);
        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax);
        /** \brief determine min/max data value of the image */
        virtual void getModifierMinMax(double& imin, double& imax);

        /** \brief returns the contents of the internal data image as a QVector<double> */
        QVector<double> getDataAsDoubleVector() const;
        /** \brief returns the contents of the internal modifier image as a QVector<double> */
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

        /** \brief how to apply the modifier column dataModifier
         *  \see ModifierMode
         */
        ModifierMode modifierMode;

        /** \brief internal storage for minimum of the image value range
         *
         * This is set e.g. when calling drawImage() or draw()
         */
        double internalDataMin;
        /** \brief internal storage for maximum of the image value range
         *
         * This is set e.g. when calling drawImage() or draw()
         */
        double internalDataMax;
        /** \brief internal storage for minimum of the modifier image value range
         *
         * This is set e.g. when calling modifyImage() or draw()
         */
        double internalModifierMin;
        /** \brief internal storage for maximum of the modifier image value range
         *
         * This is set e.g. when calling modifyImage() or draw()
         */
        double internalModifierMax;
        /** \brief overwrite this to fill the data poiters before they are accessed (e.g. to load data from a column in the datastore */
        virtual void ensureImageData();

        /** \brief modify the given image \a img, using the internally set modifier data
         *  \see dataModifier, datatypeModifier
         */
        void modifyImage(QImage& img);
        /** \brief modify the given image \a img, using  modifier image \a dataModifier (of type \a datatypeModifier and size \a Nx * \a Ny), using values in the range \a internalModifierMin ... \a internalModifierMax
         */
        void modifyImage(QImage& img, void* dataModifier, DataType datatypeModifier, int Nx, int Ny, double internalModifierMin, double internalModifierMax);


};



/*! \brief class to plot an image from a QImage object
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_rgbimageplot_qt.png
 */
class JKQTP_LIB_EXPORT JKQTPImage: public JKQTPImageBase {
        Q_OBJECT
    public:

        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPImage(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPImage(JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param image points to a QImage that shall be plotted (the constructed object does not take ownership!)
         * \param parent parent plotter object
         *
         */
        JKQTPImage(double x, double y, double width, double height, QImage* image, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param image points to a QImage that shall be plotted (the constructed object does not take ownership!)
         * \param parent parent plotter object
         *
         */
        JKQTPImage(double x, double y, double width, double height, QImage* image, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param image a QImage that shall be plotted (the constructed object copies the contents into an internally owned variable)
         * \param parent parent plotter object
         *
         */
        JKQTPImage(double x, double y, double width, double height, const QImage& image, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param image a QImage that shall be plotted (the constructed object copies the contents into an internally owned variable)
         * \param parent parent plotter object
         *
         */
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

        /*! \copydoc image */
        inline QImage* getImage() const { return this->image; }
    protected:
        /** \brief the image to be plotted. This is freed by the destructor, iff \a image_owned is set to \c true (.e.g by QImage-copy-constructors) */
        QImage* image;
        /** \brief indicates that the image \a image is owned by this object (i.e. freed, when the object is destroyed) */
        bool image_owned;

        /** \brief create QActions that are shown in the context menu of the JKQTPlotter
         *
         * \see actSaveImage, actCopyImage, saveImagePlotAsImage(), copyImagePlotAsImage()
         */
        void createImageActions();

    protected:
        /** \brief QActions that saves the image (asking the user for a filename)
         *
         * \see actSaveImage, saveImagePlotAsImage()
         */
        QAction* actSaveImage;
        /** \brief QActions that copies the image into the clipboard
         *
         * \see actCopyImage, copyImagePlotAsImage()
         */
        QAction* actCopyImage;
    public:
        /** \copydoc JKQTPImageBase::setParent() */
        virtual void setParent(JKQTBasePlotter* parent) override;
        /** \copydoc JKQTPImageBase::setTitle() */
        virtual void setTitle(const QString& title) override;
    public slots:
        /** \brief saves the image (asking the user for a filename, if \a filename is empty)
         *
         * \param filename name of the file that should be create (if empty, a file save dialog is shown)
         * \param outputFormat format of the output file, see <a href="https://doc.qt.io/qt-5/qimage.html#save">QImage::save()</a>
         *
         * \see actSaveImage, saveImagePlotAsImage(), <a href="https://doc.qt.io/qt-5/qimage.html#save">QImage::save()</a>
         */
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        /** \brief copies the image into the clipboard
         *
         * \see actCopyImage, copyImagePlotAsImage()
         */
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

        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPMathImage(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param datatype datatype of the image given in \a data
         * \param data points to an image to be plotted (of size \a Nx * \a Ny )
         * \param Nx width (in number of pixels) of \a data
         * \param Ny height (in number of pixels) of \a data
         * \param palette color palette to use for plotting
         * \param parent parent plotter object
         *
         */
        JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param datatype datatype of the image given in \a data
         * \param data points to an image to be plotted (of size \a Nx * \a Ny )
         * \param Nx width (in number of pixels) of \a data
         * \param Ny height (in number of pixels) of \a data
         * \param palette color palette to use for plotting
         * \param parent parent plotter object
         *
         */
        JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
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
        void setColorBarWidth(double __value);
        /*! \copydoc colorBarWidth */
        double getColorBarWidth() const;
        /*! \copydoc colorBarModifiedWidth */
        void setColorBarModifiedWidth(double __value);
        /*! \copydoc colorBarModifiedWidth */
        double getColorBarModifiedWidth() const;
        /*! \copydoc colorBarOffset */
        void setColorBarOffset(double __value);
        /*! \copydoc colorBarOffset */
        double getColorBarOffset() const;
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
        /*! \copydoc imageNameFontSize */
        void setImageNameFontSize(double __value);
        /*! \copydoc imageNameFontSize */
        double getImageNameFontSize() const;
        /*! \copydoc colorBarRightAxis */
        JKQTPVerticalIndependentAxis* getColorBarRightAxis();
        /*! \copydoc colorBarTopAxis */
        JKQTPHorizontalIndependentAxis* getColorBarTopAxis();
        /*! \copydoc modifierColorBarTopAxis */
        JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis();
        /*! \copydoc modifierColorBarRightAxis */
        JKQTPHorizontalIndependentAxis* getModifierColorBarRightAxis();
        /*! \copydoc colorBarRightAxis */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxis() const;
        /*! \copydoc colorBarTopAxis */
        const JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const;
        /*! \copydoc modifierColorBarTopAxis */
        const JKQTPVerticalIndependentAxis* getModifierColorBarTopAxis() const;
        /*! \copydoc modifierColorBarRightAxis */
        const JKQTPHorizontalIndependentAxis *getModifierColorBarRightAxis()  const;
        /*! \copydoc colorBarTopVisible */
        void setColorBarTopVisible(bool __value);
        /*! \copydoc colorBarTopVisible */
        bool getColorBarTopVisible() const;
        /*! \copydoc colorBarRightVisible */
        void setColorBarRightVisible(bool __value);
        /*! \copydoc colorBarRightVisible */
        bool getColorBarRightVisible() const;
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
        inline double getValueAt(double x, double y);

        /** \brief retrieve size of a single sample in the image data (i.e. size of the datatype) */
        inline int getSampleSize() const;
        /** \brief retrieve image data at a given position as double */
        inline double getPixelValue(int xIdx, int yIdx) const;

        /** \brief return the value (raw data!) of the contained modifier image at the given coordinate */
        inline double getModifierValueAt(double x, double y);

        /** \brief retrieve size of a single sample in the modifier image data (i.e. size of the datatype) */
        inline int getModifierSampleSize() const;
        /** \brief retrieve modifier image data at a given position as double */
        inline double getModifierPixelValue(int xIdx, int yIdx) const;

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


        /** \brief object used for color bar axes (right border, image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPVerticalIndependentAxis* colorBarRightAxis;
        /** \brief object used for color bar axes (top border, image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPHorizontalIndependentAxis* colorBarTopAxis;
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

    protected:
        /** \brief QActions that saves the image (asking the user for a filename)
         *
         * \see actSaveImage, saveImagePlotAsImage()
         */
        QAction* actSaveImage;
        /** \brief QActions that copies the image into the clipboard
         *
         * \see actCopyImage, copyImagePlotAsImage()
         */
        QAction* actCopyImage;
        /** \brief QActions that saves the current palette
         *
         * \see actSaveImage, saveColorbarPlotAsImage()
         */
        QAction* actSavePalette;
        /** \brief QActions that copies the current palette
         *
         * \see actCopyImage, copyColorbarPlotAsImage()
         */
        QAction* actCopyPalette;
    public:
        /** \copydoc JKQTPImageBase::setTitle() */
        virtual void setTitle(const QString& title) override;
    public slots:
        /** \brief saves the image (asking the user for a filename, if \a filename is empty)
         *
         * \param filename name of the file that should be create (if empty, a file save dialog is shown)
         * \param outputFormat format of the output file, see <a href="https://doc.qt.io/qt-5/qimage.html#save">QImage::save()</a>
         *
         * \see actSaveImage, saveImagePlotAsImage(), <a href="https://doc.qt.io/qt-5/qimage.html#save">QImage::save()</a>
         */
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        /** \brief copies the image into the clipboard
         *
         * \see actCopyImage, copyImagePlotAsImage()
         */
        void copyImagePlotAsImage();
        /** \brief saves an image of the current palette (asking the user for a filename, if \a filename is empty)
         *
         * \param filename name of the file that should be create (if empty, a file save dialog is shown)
         * \param outputFormat format of the output file, see <a href="https://doc.qt.io/qt-5/qimage.html#save">QImage::save()</a>
         *
         * \see actSavePalette, saveColorbarPlotAsImage(), <a href="https://doc.qt.io/qt-5/qimage.html#save">QImage::save()</a>
         */
        void saveColorbarPlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        /** \brief copies an image of the current palette into the clipboard
         *
         * \see actCopyPalette, copyColorbarPlotAsImage()
         */
        void copyColorbarPlotAsImage();

};

////////////////////////////////////////////////////////////////////////////////////////////////
double JKQTPMathImage::getValueAt(double x, double y)
{
    ensureImageData();
    int xx=trunc((x-this->x)/width*double(Nx));
    int yy=trunc((y-this->y)/height*double(Ny));
    if (xx>=0 && xx<(int64_t)Nx && yy>=0 && yy<(int64_t)Ny) {
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray: return (static_cast<double*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::FloatArray: return (static_cast<float*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt8Array: return (static_cast<uint8_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt16Array: return (static_cast<uint16_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt32Array: return (static_cast<uint32_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt64Array: return (static_cast<uint64_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int8Array: return (static_cast<int8_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int16Array: return (static_cast<int16_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int32Array: return (static_cast<int32_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int64Array: return (static_cast<int64_t*>(data))[yy*Nx+xx]; break;
        }    }
    return 0.0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
double JKQTPMathImage::getPixelValue(int xIdx, int yIdx) const {
    // row-major in datastore
    //ensureImageData();
    if (!data) return 0;
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return (static_cast<double*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::FloatArray:
            return (static_cast<float*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt8Array:
            return (static_cast<uint8_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt16Array:
            return (static_cast<uint16_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt32Array:
            return (static_cast<uint32_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt64Array:
            return (static_cast<uint64_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int8Array:
            return (static_cast<int8_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int16Array:
            return (static_cast<int16_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int32Array:
            return (static_cast<int32_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int64Array:
            return (static_cast<int64_t*>(data))[yIdx*getNx()+xIdx];
        default:
            return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPMathImage::getSampleSize() const {
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return sizeof(double);
        case JKQTPMathImageBase::FloatArray:
            return sizeof(float);
        case JKQTPMathImageBase::UInt8Array:
            return sizeof(uint8_t);
        case JKQTPMathImageBase::UInt16Array:
            return sizeof(uint16_t);
        case JKQTPMathImageBase::UInt32Array:
            return sizeof(uint32_t);
        case JKQTPMathImageBase::UInt64Array:
            return sizeof(uint64_t);
        case JKQTPMathImageBase::Int8Array:
            return sizeof(int8_t);
        case JKQTPMathImageBase::Int16Array:
            return sizeof(int16_t);
        case JKQTPMathImageBase::Int32Array:
            return sizeof(int32_t);
        case JKQTPMathImageBase::Int64Array:
            return sizeof(int64_t);
        default:
            return 0;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////
double JKQTPMathImage::getModifierValueAt(double x, double y)
{
    ensureImageData();
    int xx=trunc((x-this->x)/width*double(Nx));
    int yy=trunc((y-this->y)/height*double(Ny));
    if (xx>=0 && xx<(int64_t)Nx && yy>=0 && yy<(int64_t)Ny) {
        switch(datatypeModifier) {
            case JKQTPMathImageBase::DoubleArray: return (static_cast<double*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::FloatArray: return (static_cast<float*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt8Array: return (static_cast<uint8_t*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt16Array: return (static_cast<uint16_t*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt32Array: return (static_cast<uint32_t*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt64Array: return (static_cast<uint64_t*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int8Array: return (static_cast<int8_t*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int16Array: return (static_cast<int16_t*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int32Array: return (static_cast<int32_t*>(dataModifier))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int64Array: return (static_cast<int64_t*>(dataModifier))[yy*Nx+xx]; break;
        }    }
    return 0.0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
double JKQTPMathImage::getModifierPixelValue(int xIdx, int yIdx) const {
    // row-major in datastore
    //ensureImageData();
    if (!dataModifier) return 0;
    switch(datatypeModifier) {
        case JKQTPMathImageBase::DoubleArray:
            return (static_cast<double*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::FloatArray:
            return (static_cast<float*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt8Array:
            return (static_cast<uint8_t*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt16Array:
            return (static_cast<uint16_t*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt32Array:
            return (static_cast<uint32_t*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt64Array:
            return (static_cast<uint64_t*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int8Array:
            return (static_cast<int8_t*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int16Array:
            return (static_cast<int16_t*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int32Array:
            return (static_cast<int32_t*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int64Array:
            return (static_cast<int64_t*>(dataModifier))[yIdx*getNx()+xIdx];
        default:
            return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPMathImage::getModifierSampleSize() const {
    switch(datatypeModifier) {
        case JKQTPMathImageBase::DoubleArray:
            return sizeof(double);
        case JKQTPMathImageBase::FloatArray:
            return sizeof(float);
        case JKQTPMathImageBase::UInt8Array:
            return sizeof(uint8_t);
        case JKQTPMathImageBase::UInt16Array:
            return sizeof(uint16_t);
        case JKQTPMathImageBase::UInt32Array:
            return sizeof(uint32_t);
        case JKQTPMathImageBase::UInt64Array:
            return sizeof(uint64_t);
        case JKQTPMathImageBase::Int8Array:
            return sizeof(int8_t);
        case JKQTPMathImageBase::Int16Array:
            return sizeof(int16_t);
        case JKQTPMathImageBase::Int32Array:
            return sizeof(int32_t);
        case JKQTPMathImageBase::Int64Array:
            return sizeof(int64_t);
        default:
            return 0;
    }
}




/*! \brief class to plot an image from an 2-dimensional array of values stored in a column of the datastore
    \ingroup jkqtplotter_imagelots_elements

    \image html jkqtplotter_simpletest_imageplot.png
    \image html jkqtplotter_simpletest_imageplot_modifier.png
    \image html jkqtplotter_simpletest_imageplot__smallscaletransparent.png

 */
class JKQTP_LIB_EXPORT JKQTPColumnMathImage: public JKQTPMathImage {
        Q_OBJECT
    public:

        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPColumnMathImage(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
        JKQTPColumnMathImage(double x, double y, double width, double height, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageColumn column to be plotted
         * \param palette color palette to use for the plotting
         * \param parent parent plotter object
         *
         */
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPColumnMathImage(JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
        JKQTPColumnMathImage(double x, double y, double width, double height, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageColumn column to be plotted
         * \param palette color palette to use for the plotting
         * \param parent parent plotter object
         *
         */
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, JKQTPMathImageColorPalette palette, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageColumn column to be plotted
         * \param parent parent plotter object
         *
         */
        JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, JKQTPlotter* parent);

        /*! \copydoc imageColumn */
        virtual void setImageColumn(int __value);
        /*! \copydoc imageColumn */
        int getImageColumn() const;
        /*! \copydoc modifierColumn */
        virtual void setModifierColumn(int __value);
        /*! \copydoc modifierColumn */
        int getModifierColumn() const;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;



    protected:
        /** \brief  column containing the displayed image */
        int imageColumn;
        /** \brief  column containing the modifier image */
        int modifierColumn;

        /** \copydoc JKQTPMathImage::ensureImageData() */
        virtual void ensureImageData() override;

    private:
        using JKQTPMathImage::setData;
        using JKQTPMathImage::setDatatype;
        using JKQTPMathImage::setDataModifier;
        using JKQTPMathImage::setDatatypeModifier;
};



#endif // jkqtpgraphsimage_H

