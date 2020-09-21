/*
    Copyright (c) 2008-2020 Jan W. Krieger



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
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"



/*! \brief base class for plotting an image
    \ingroup jkqtplotter_imagelots_elements
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImageBase: public JKQTPGraph {
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

        /** \copydoc x */
        void setX(double __value);
        /** \copydoc x */
        double getX() const;
        /** \copydoc y */
        void setY(double __value);
        /** \copydoc y */
        double getY() const;
        /** \copydoc width */
        void setWidth(double __value);
        /** \copydoc width */
        double getWidth() const;
        /** \copydoc height */
        void setHeight(double __value);
        /** \copydoc height */
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

    \image html imageplot.png

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPMathImageBase: public JKQTPImageBase {
        Q_OBJECT
    public:

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
        JKQTPMathImageBase(double x, double y, double width, double height, JKQTPMathImageDataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);


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
        JKQTPMathImageBase(double x, double y, double width, double height, JKQTPMathImageDataType datatype, void* data, int Nx, int Ny, JKQTPlotter* parent);

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;

		/** \copydoc Nx */
		void setNx(int __value);
		/** \copydoc Nx */
		void setNx(size_t __value);
		/** \copydoc Nx */
        int getNx() const;
        /** \copydoc Ny */
        void setNy(int __value);
         /** \copydoc Ny */
        void setNy(size_t __value);
       /** \copydoc Ny */
        int getNy() const;
        /** \copydoc data */
        virtual void setData(void* __value);
        /** \copydoc data */
        virtual void *getData() const;
        /** \copydoc datatype */
        virtual void setDatatype(JKQTPMathImageDataType __value);
        /** \copydoc datatype */
        virtual JKQTPMathImageDataType getDatatype() const;
        /** \copydoc dataModifier */
        virtual void setDataModifier(void* __value);
        /** \copydoc dataModifier */
        virtual void *getDataModifier() const;
        /** \copydoc datatypeModifier */
        virtual void setDatatypeModifier(JKQTPMathImageDataType __value);
        /** \copydoc datatypeModifier */
        virtual JKQTPMathImageDataType getDatatypeModifier() const;
        /** \copydoc modifierMode */

        /** \brief sets dataModifier (\copybrief dataModifier ) and datatypeModifier (\copybrief datatypeModifier ) */
        virtual void setDataModifier(void* data, JKQTPMathImageDataType datatype);
        /** \brief sets data (\copybrief data ) and datatype (\copybrief datatype ), as well as the size of data (Nx: \copybrief Nx and Ny: \copybrief Ny ) */
        virtual void setData(void* data, int Nx, int Ny, JKQTPMathImageDataType datatype);
        /** \brief sets data (\copybrief data ), as well as the size of data (Nx: \copybrief Nx and Ny: \copybrief Ny ) */
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
        JKQTPMathImageDataType datatype;
        /** \brief width of the data array data in pt */
        int Nx;
        /** \brief height of the data array data in pt */
        int Ny;

        /** \brief points to the data array, holding the modifier image */
        void* dataModifier;
        /** \brief datatype of the data array data */
        JKQTPMathImageDataType datatypeModifier;


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


};



/*! \brief class to plot an image from a QImage object
    \ingroup jkqtplotter_imagelots_elements

    \image html rgbimageplot_qt.png

    \see \ref JKQTPlotterImagePlotQImageRGB
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImage: public JKQTPImageBase {
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

        /** \copydoc image */
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

    \image html imageplot.png
    \image html imageplot_modifier.png
    \image html imageplot__smallscaletransparent.png

    \see \ref JKQTPlotterImagePlotNoDatastore
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPMathImage: public JKQTPMathImageBase, public JKQTPColorPaletteWithModifierStyleAndToolsMixin {
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
        JKQTPMathImage(double x, double y, double width, double height, JKQTPMathImageDataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette=JKQTPMathImageGRAY, JKQTBasePlotter* parent=nullptr);
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
        JKQTPMathImage(double x, double y, double width, double height, JKQTPMathImageDataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPMathImage(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        virtual void setParent(JKQTBasePlotter* parent) override;



        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        virtual void getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) override;

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        virtual void drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) override;

        /** \brief returns a QImage, which contains the plaette drawn outside the plot. \a steps is the number of data-setps (and the size of the output image) used for the palette image.  */
        virtual QImage drawOutsidePalette(uint8_t steps=200);

        /** \brief return the plotted image only as a QImage  */
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
        /** \brief determine min/max data value of the image */
        virtual void cbGetDataMinMax(double& imin, double& imax) override;
        /** \brief determine min/max data value of the modifier image */
        virtual void cbGetModifierDataMinMax(double& imin, double& imax) override;

    protected:
        void initJKQTPMathImage();

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
    int xx=jkqtp_truncTo<int>((x-this->x)/width*double(Nx));
    int yy=jkqtp_truncTo<int>((y-this->y)/height*double(Ny));
    if (xx>=0 && xx<static_cast<int64_t>(Nx) && yy>=0 && yy<static_cast<int64_t>(Ny)) {
        switch(datatype) {
            case JKQTPMathImageDataType::DoubleArray: return static_cast<double>((static_cast<double*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::FloatArray: return static_cast<double>((static_cast<float*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt8Array: return static_cast<double>((static_cast<uint8_t*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt16Array: return static_cast<double>((static_cast<uint16_t*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt32Array: return static_cast<double>((static_cast<uint32_t*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt64Array: return static_cast<double>((static_cast<uint64_t*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int8Array: return static_cast<double>((static_cast<int8_t*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int16Array: return static_cast<double>((static_cast<int16_t*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int32Array: return static_cast<double>((static_cast<int32_t*>(data))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int64Array: return static_cast<double>((static_cast<int64_t*>(data))[yy*Nx+xx]);
        }    }
    return 0.0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
double JKQTPMathImage::getPixelValue(int xIdx, int yIdx) const {
    // row-major in datastore
    //ensureImageData();
    if (!data) return 0;
    switch(datatype) {
        case JKQTPMathImageDataType::DoubleArray:
            return (static_cast<double*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageDataType::FloatArray:
            return static_cast<double>((static_cast<float*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt8Array:
            return static_cast<double>((static_cast<uint8_t*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt16Array:
            return static_cast<double>((static_cast<uint16_t*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt32Array:
            return static_cast<double>((static_cast<uint32_t*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt64Array:
            return static_cast<double>((static_cast<uint64_t*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int8Array:
            return static_cast<double>((static_cast<int8_t*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int16Array:
            return static_cast<double>((static_cast<int16_t*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int32Array:
            return static_cast<double>((static_cast<int32_t*>(data))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int64Array:
            return static_cast<double>((static_cast<int64_t*>(data))[yIdx*getNx()+xIdx]);
        default:
            return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPMathImage::getSampleSize() const {
    switch(datatype) {
        case JKQTPMathImageDataType::DoubleArray:
            return sizeof(double);
        case JKQTPMathImageDataType::FloatArray:
            return sizeof(float);
        case JKQTPMathImageDataType::UInt8Array:
            return sizeof(uint8_t);
        case JKQTPMathImageDataType::UInt16Array:
            return sizeof(uint16_t);
        case JKQTPMathImageDataType::UInt32Array:
            return sizeof(uint32_t);
        case JKQTPMathImageDataType::UInt64Array:
            return sizeof(uint64_t);
        case JKQTPMathImageDataType::Int8Array:
            return sizeof(int8_t);
        case JKQTPMathImageDataType::Int16Array:
            return sizeof(int16_t);
        case JKQTPMathImageDataType::Int32Array:
            return sizeof(int32_t);
        case JKQTPMathImageDataType::Int64Array:
            return sizeof(int64_t);
        default:
            return 0;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////
double JKQTPMathImage::getModifierValueAt(double x, double y)
{
    ensureImageData();
    int xx=jkqtp_truncTo<int>((x-this->x)/width*double(Nx));
    int yy=jkqtp_truncTo<int>((y-this->y)/height*double(Ny));
    if (xx>=0 && xx<(int64_t)Nx && yy>=0 && yy<(int64_t)Ny) {
        switch(datatypeModifier) {
            case JKQTPMathImageDataType::DoubleArray: return (static_cast<double*>(dataModifier))[yy*Nx+xx];
            case JKQTPMathImageDataType::FloatArray: return static_cast<double>((static_cast<float*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt8Array: return static_cast<double>((static_cast<uint8_t*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt16Array: return static_cast<double>((static_cast<uint16_t*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt32Array: return static_cast<double>((static_cast<uint32_t*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::UInt64Array: return static_cast<double>((static_cast<uint64_t*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int8Array: return static_cast<double>((static_cast<int8_t*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int16Array: return static_cast<double>((static_cast<int16_t*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int32Array: return static_cast<double>((static_cast<int32_t*>(dataModifier))[yy*Nx+xx]);
            case JKQTPMathImageDataType::Int64Array: return static_cast<double>((static_cast<int64_t*>(dataModifier))[yy*Nx+xx]);
        }    }
    return 0.0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
double JKQTPMathImage::getModifierPixelValue(int xIdx, int yIdx) const {
    // row-major in datastore
    //ensureImageData();
    if (!dataModifier) return 0;
    switch(datatypeModifier) {
        case JKQTPMathImageDataType::DoubleArray:
            return (static_cast<double*>(dataModifier))[yIdx*getNx()+xIdx];
        case JKQTPMathImageDataType::FloatArray:
            return static_cast<double>((static_cast<float*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt8Array:
            return static_cast<double>((static_cast<uint8_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt16Array:
            return static_cast<double>((static_cast<uint16_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt32Array:
            return static_cast<double>((static_cast<uint32_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::UInt64Array:
            return static_cast<double>((static_cast<uint64_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int8Array:
            return static_cast<double>((static_cast<int8_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int16Array:
            return static_cast<double>((static_cast<int16_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int32Array:
            return static_cast<double>((static_cast<int32_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        case JKQTPMathImageDataType::Int64Array:
            return static_cast<double>((static_cast<int64_t*>(dataModifier))[yIdx*getNx()+xIdx]);
        default:
            return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPMathImage::getModifierSampleSize() const {
    switch(datatypeModifier) {
        case JKQTPMathImageDataType::DoubleArray:
            return sizeof(double);
        case JKQTPMathImageDataType::FloatArray:
            return sizeof(float);
        case JKQTPMathImageDataType::UInt8Array:
            return sizeof(uint8_t);
        case JKQTPMathImageDataType::UInt16Array:
            return sizeof(uint16_t);
        case JKQTPMathImageDataType::UInt32Array:
            return sizeof(uint32_t);
        case JKQTPMathImageDataType::UInt64Array:
            return sizeof(uint64_t);
        case JKQTPMathImageDataType::Int8Array:
            return sizeof(int8_t);
        case JKQTPMathImageDataType::Int16Array:
            return sizeof(int16_t);
        case JKQTPMathImageDataType::Int32Array:
            return sizeof(int32_t);
        case JKQTPMathImageDataType::Int64Array:
            return sizeof(int64_t);
        default:
            return 0;
    }
}




/*! \brief class to plot an image from an 2-dimensional array of values stored in a column of the datastore
    \ingroup jkqtplotter_imagelots_elements

    \image html imageplot.png
    \image html imageplot_modifier.png
    \image html imageplot__smallscaletransparent.png

    \see jkqtpstatAddKDE2DImage(), jkqtpstatAddHistogram2DImage(), \ref JKQTPlotterImagePlot, \ref JKQTPlotterImagePlotModifier, \ref JKQTPlotterImagePlotOpenCV
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPColumnMathImage: public JKQTPMathImage {
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

        /** \copydoc imageColumn */
        virtual void setImageColumn(int __value);

        /** \copydoc imageColumn */
        virtual void setImageColumn(size_t __value);
        /** \copydoc imageColumn */
        int getImageColumn() const;
        /** \copydoc modifierColumn */
        virtual void setModifierColumn(int __value);
         /** \copydoc modifierColumn */
        virtual void setModifierColumn(size_t __value);
       /** \copydoc modifierColumn */
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

