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


#ifndef JKQTPGRAPHSIMAGERGB_H
#define JKQTPGRAPHSIMAGERGB_H


#include <QString>
#include <QPainter>
#include <QImage>
#include <QIcon>
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"



/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots_elements

    \image html rgbimageplots.png
    \image html rgbimageplot_opencv.png
*/
class JKQTP_LIB_EXPORT JKQTPRGBMathImage: public JKQTPMathImageBase {
        Q_OBJECT
    public:
        using JKQTPMathImageBase::setData;


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
        JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter* parent=nullptr);

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
        JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPlotter* parent);

        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPRGBMathImage(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPRGBMathImage(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc JKQTPImageBase::setParent() */
        virtual void setParent(JKQTBasePlotter* parent) override;



        /** \brief set the plot-data to a given array \a data (red), \a dataG (green), \a dataB (blue) with size \a Nx * \a Ny in row-major ordering and with the given \a datatype */
        virtual void setData(void* dataR, void* dataG, void* dataB, int Nx, int Ny, DataType datatype);
        /** \brief set the plot-data to a given array \a data (red), \a dataG (green), \a dataB (blue) with size \a Nx * \a Ny in row-major ordering  */
        virtual void setData(void* dataR, void* dataG, void* dataB,  int Nx, int Ny);

        /** \brief set the plot-data to a given array \a data (red) with size \a Nx * \a Ny in row-major ordering and with the given \a datatype  */
        virtual void setData(void* dataR, int Nx, int Ny, DataType datatype) override;
        /** \brief set the plot-data to a given array \a data (red) with size \a Nx * \a Ny in row-major ordering  */
        virtual void setData(void* dataR,  int Nx, int Ny) override ;


        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax) override;


        /*! \copydoc data */
        virtual void setDataR(void* __value);
        /*! \copydoc data */
        void* getDataR() const;
        /*! \copydoc datatype */
        void setDatatypeR(DataType __value);
        /*! \copydoc datatype */
        DataType getDatatypeR() const;

        /*! \copydoc dataG */
        virtual void setDataG(void* __value);
        /*! \copydoc dataG */
        void* getDataG() const;
        /*! \copydoc datatypeG */
        void setDatatypeG(DataType __value);
        /*! \copydoc datatypeG */
        DataType getDatatypeG() const;
        /*! \copydoc dataB */ 
        virtual void setDataB(void* __value);
        /*! \copydoc dataB */ 
        void* getDataB() const;
        /*! \copydoc datatypeB */ 
        void setDatatypeB(DataType __value);
        /*! \copydoc datatypeB */ 
        DataType getDatatypeB() const;
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
        /*! \copydoc imageMinR */
        void setImageMin(double __value);
        /*! \copydoc imageMinR */
        double getImageMin() const;
        /*! \copydoc imageMinR */
        void setImageMinR(double m);
        /*! \copydoc imageMaxR */
        void setImageMax(double __value);
        /*! \copydoc imageMaxR */
        double getImageMax() const;
        /*! \copydoc imageMaxR */
        void setImageMaxR(double m);
        /*! \copydoc imageMinG */ 
        void setImageMinG(double __value);
        /*! \copydoc imageMinG */ 
        double getImageMinG() const;
        /*! \copydoc imageMaxG */ 
        void setImageMaxG(double __value);
        /*! \copydoc imageMaxG */ 
        double getImageMaxG() const;
        /*! \copydoc imageMinB */ 
        void setImageMinB(double __value);
        /*! \copydoc imageMinB */ 
        double getImageMinB() const;
        /*! \copydoc imageMaxB */ 
        void setImageMaxB(double __value);
        /*! \copydoc imageMaxB */
        double getImageMaxB() const;
        /*! \copydoc autoImageRange */ 
        void setAutoImageRange(bool __value);
        /*! \copydoc autoImageRange */ 
        bool getAutoImageRange() const;
        /*! \copydoc imageNameR */
        void setImageName(const QString & __value);
        /*! \copydoc imageNameR */
        QString getImageName() const;
        /*! \copydoc imageNameR */
        QString getImageNameR() const;
        /*! \copydoc imageNameR */
        void setImageNameR(const QString& m);
        /*! \copydoc imageNameG */ 
        void setImageNameG(const QString & __value);
        /*! \copydoc imageNameG */ 
        QString getImageNameG() const;
        /*! \copydoc imageNameB */ 
        void setImageNameB(const QString & __value);
        /*! \copydoc imageNameB */ 
        QString getImageNameB() const;
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
        JKQTPVerticalIndependentAxis* getColorBarRightAxisR();
        /*! \copydoc colorBarTopAxis */
        JKQTPHorizontalIndependentAxis *getColorBarTopAxisR();
        /*! \copydoc colorBarRightAxisG */
        JKQTPVerticalIndependentAxis* getColorBarRightAxisG();
        /*! \copydoc colorBarTopAxisG */
        JKQTPHorizontalIndependentAxis* getColorBarTopAxisG();
        /*! \copydoc colorBarRightAxisB */
        JKQTPVerticalIndependentAxis* getColorBarRightAxisB();
        /*! \copydoc colorBarTopAxisB */
        JKQTPHorizontalIndependentAxis* getColorBarTopAxisB();
        /*! \copydoc colorBarRightAxis */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxis() const;
        /*! \copydoc colorBarTopAxis */
        const JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const;
        /*! \copydoc colorBarRightAxis */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxisR() const;
        /*! \copydoc colorBarTopAxis */
        const JKQTPHorizontalIndependentAxis* getColorBarTopAxisR() const;
        /*! \copydoc colorBarRightAxisG */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxisG() const;
        /*! \copydoc colorBarTopAxisG */
        const JKQTPHorizontalIndependentAxis *getColorBarTopAxisG() const;
        /*! \copydoc colorBarRightAxisB */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxisB() const;
        /*! \copydoc colorBarTopAxisB */
        const JKQTPHorizontalIndependentAxis *getColorBarTopAxisB() const;
        /*! \copydoc colorBarTopVisible */ 
        void setColorBarTopVisible(bool __value);
        /*! \copydoc colorBarTopVisible */ 
        bool getColorBarTopVisible() const;
        /*! \copydoc colorBarRightVisible */ 
        void setColorBarRightVisible(bool __value);
        /*! \copydoc colorBarRightVisible */ 
        bool getColorBarRightVisible() const;
        /*! \copydoc colorbarsSideBySide */ 
        void setColorbarsSideBySide(bool __value);
        /*! \copydoc colorbarsSideBySide */ 
        bool getColorbarsSideBySide() const;
        /*! \copydoc rgbMode */ 
        void setRgbMode(JKQTPRGBMathImageRGBMode __value);
        /*! \copydoc rgbMode */ 
        JKQTPRGBMathImageRGBMode getRgbMode() const;

        /** \brief return the data of the green channel used for plotting as a QVector<double> in row-major data-ordering */
        QVector<double> getDataGAsDoubleVector() const;
        /** \brief return the data of the blue channel used for plotting as a QVector<double> in row-major data-ordering */
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

        /** \brief how to interpret the three data channels red, green and blue */
        JKQTPRGBMathImageRGBMode rgbMode;

        /** \brief top color bar visible */
        bool colorBarTopVisible;
        /** \brief right color bar visible */
        bool colorBarRightVisible;
        /** \brief name of the red image channel displayed above color bar (may contain LaTeX markup!) */
        QString imageNameR;
        /** \brief name of the green image channel displayed above color bar (may contain LaTeX markup!) */
        QString imageNameG;
        /** \brief name of the blue image channel displayed above color bar (may contain LaTeX markup!) */
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
        /** \brief red image value range minimum */
        double imageMinR;
        /** \brief red image value range maximum */
        double imageMaxR;
        /** \brief green image value range minimum */
        double imageMinG;
        /** \brief green image value range maximum */
        double imageMaxG;
        /** \brief blue image value range minimum */
        double imageMinB;
        /** \brief blue image value range maximum */
        double imageMaxB;
        /** \brief offset between outside space border and color bar */
        int colorBarOffset;
        /** \brief if \c true the different color bars share the space of a single color bar, if set \c false each colorbar uses the space of a single colorbar */
        bool colorbarsSideBySide;


        /** \brief object used for color bar axes (right border, red image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPVerticalIndependentAxis* colorBarRightAxis;
        /** \brief object used for color bar axes (top border, red image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPHorizontalIndependentAxis* colorBarTopAxis;
        /** \brief object used for color bar axes (right border, green image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPVerticalIndependentAxis* colorBarRightAxisG;
        /** \brief object used for color bar axes (top border, green image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPHorizontalIndependentAxis* colorBarTopAxisG;
        /** \brief object used for color bar axes (right border, blue image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPVerticalIndependentAxis* colorBarRightAxisB;
        /** \brief object used for color bar axes (top border, blue image data)
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPHorizontalIndependentAxis* colorBarTopAxisB;


        /** \brief internal storage for minimum of the green image value range
         *
         * This is set e.g. when calling drawImage() or draw()
         */
        double internalDataMinG;
        /** \brief internal storage for maximum of the green image value range
         *
         * This is set e.g. when calling drawImage() or draw()
         */
        double internalDataMaxG;
        /** \brief internal storage for minimum of the blue image value range
         *
         * This is set e.g. when calling drawImage() or draw()
         */
        double internalDataMinB;
        /** \brief internal storage for maximum of the blue image value range
         *
         * This is set e.g. when calling drawImage() or draw()
         */
        double internalDataMaxB;

    protected:
        /** \brief action that calls saveImagePlotAsImage() */
        QAction* actSaveImage;
        /** \brief action that calls copyImagePlotAsImage() */
        QAction* actCopyImage;
    public:
        /** \copydoc JKQTPImageBase::setTitle() */
        virtual void setTitle(const QString& title) override;
    public slots:
        /** \brief save the plotted image as a file with \a filename and format \a outputFormat */
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        /** \brief copy the plotted image as an image into the clipboard */
        void copyImagePlotAsImage();
};



/*! \brief like JKQTPRGBMathImage but reads images from columns of the datastore
    \ingroup jkqtplotter_imagelots_elements


    \image html rgbimageplots.png

    \image html rgbimageplot_opencv.png

    \image html rgbimageplot.png

    \see Examples: \ref JKQTPlotterRGBImagePlot and  \ref JKQTPlotterImagePlotRGBOpenCV
 */
class JKQTP_LIB_EXPORT JKQTPColumnRGBMathImage: public JKQTPRGBMathImage {
        Q_OBJECT
    public:

        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPColumnRGBMathImage(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageRColumn column for red channel to be plotted
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageRColumn column for red channel to be plotted
         * \param imageGColumn column for green channel to be plotted
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageRColumn column for red channel to be plotted
         * \param imageGColumn column for green channel to be plotted
         * \param imageBColumn column for blue channel to be plotted
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor
         *
         * \param parent parent plotter object
         */
        JKQTPColumnRGBMathImage(JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageRColumn column for red channel to be plotted
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageRColumn column for red channel to be plotted
         * \param imageGColumn column for green channel to be plotted
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, JKQTPlotter* parent);
        /** \brief class constructor
         *
         * \param x origin of the image (x-direction) in system coordinates
         * \param y origin of the image (y-direction) in system coordinates
         * \param width width of the image in system coordinates
         * \param height height of the image in system coordinates
         * \param imageRColumn column for red channel to be plotted
         * \param imageGColumn column for green channel to be plotted
         * \param imageBColumn column for blue channel to be plotted
         * \param parent parent plotter object
         *
         */
        JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, JKQTPlotter* parent);

        /*! \copydoc imageRColumn */ 
        virtual void setImageRColumn(int __value);
        /*! \copydoc imageRColumn */ 
        int getImageRColumn() const;
        /*! \copydoc imageGColumn */ 
        virtual void setImageGColumn(int __value);
        /*! \copydoc imageGColumn */ 
        int getImageGColumn() const;
        /*! \copydoc imageBColumn */ 
        virtual void setImageBColumn(int __value);
        /*! \copydoc imageBColumn */ 
        int getImageBColumn() const;
        /*! \copydoc modifierColumn */ 
        virtual void setModifierColumn(int __value);
        /*! \copydoc modifierColumn */ 
        int getModifierColumn() const;
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
        /** \copydoc JKQTPRGBMathImage::ensureImageData() */
        void ensureImageData() override;

    private:
        using JKQTPRGBMathImage::setData;
        using JKQTPRGBMathImage::setDatatype;
        using JKQTPRGBMathImage::setDataR;
        using JKQTPRGBMathImage::setDatatypeR;
        using JKQTPRGBMathImage::setDataG;
        using JKQTPRGBMathImage::setDatatypeG;
        using JKQTPRGBMathImage::setDataB;
        using JKQTPRGBMathImage::setDatatypeB;
        using JKQTPRGBMathImage::setDataModifier;
        using JKQTPRGBMathImage::setDatatypeModifier;
};



#endif // JKQTPGRAPHSIMAGERGB_H

