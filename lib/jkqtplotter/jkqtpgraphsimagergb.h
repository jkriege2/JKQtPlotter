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
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtcommon/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"



/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots_elements

    \image html rgbimageplots.png
    \image html jkqtplotter_simpletest_rgbimageplot_opencv.png
*/
class JKQTP_LIB_EXPORT JKQTPRGBMathImage: public JKQTPMathImageBase {
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


        /*! \copydoc data
            \see see data for details */
        virtual void setDataR(void* __value);
        /*! \copydoc data
            \see see data for details */
        void* getDataR() const;
        /*! \copydoc datatype
            \see see datatype for details */
        void setDatatypeR(DataType __value);
        /*! \copydoc datatype
            \see see datatype for details */
        DataType getDatatypeR() const;

        /*! \copydoc dataG
            \see see dataG for details */
        virtual void setDataG(void* __value);
        /*! \copydoc dataG
            \see see dataG for details */
        void* getDataG() const;
        /*! \copydoc datatypeG
            \see see datatypeG for details */
        void setDatatypeG(DataType __value);
        /*! \copydoc datatypeG
            \see see datatypeG for details */
        DataType getDatatypeG() const;
        /*! \copydoc dataB
            \see see dataB for details */ 
        virtual void setDataB(void* __value);
        /*! \copydoc dataB
            \see see dataB for details */ 
        void* getDataB() const;
        /*! \copydoc datatypeB
            \see see datatypeB for details */ 
        void setDatatypeB(DataType __value);
        /*! \copydoc datatypeB
            \see see datatypeB for details */ 
        DataType getDatatypeB() const;
        /*! \copydoc showColorBar
            \see see showColorBar for details */ 
        void setShowColorBar(bool __value);
        /*! \copydoc showColorBar
            \see see showColorBar for details */ 
        bool getShowColorBar() const;
        /*! \copydoc colorBarWidth
            \see see colorBarWidth for details */ 
        void setColorBarWidth(int __value);
        /*! \copydoc colorBarWidth
            \see see colorBarWidth for details */ 
        int getColorBarWidth() const;
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        void setColorBarOffset(int __value);
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        int getColorBarOffset() const;
        /*! \copydoc colorBarRelativeHeight
            \see see colorBarRelativeHeight for details */ 
        void setColorBarRelativeHeight(double __value);
        /*! \copydoc colorBarRelativeHeight
            \see see colorBarRelativeHeight for details */ 
        double getColorBarRelativeHeight() const;
        /*! \copydoc imageMinR
            \see see imageMinR for details */
        void setImageMin(double __value);
        /*! \copydoc imageMinR
            \see see imageMinR for details */
        double getImageMin() const;
        /*! \copydoc imageMinR
            \see see imageMinR for details */
        void setImageMinR(double m);
        /*! \copydoc imageMaxR
            \see see imageMaxR for details */
        void setImageMax(double __value);
        /*! \copydoc imageMaxR
            \see see imageMaxR for details */
        double getImageMax() const;
        /*! \copydoc imageMaxR
            \see see imageMaxR for details */
        void setImageMaxR(double m);
        /*! \copydoc imageMinG
            \see see imageMinG for details */ 
        void setImageMinG(double __value);
        /*! \copydoc imageMinG
            \see see imageMinG for details */ 
        double getImageMinG() const;
        /*! \copydoc imageMaxG
            \see see imageMaxG for details */ 
        void setImageMaxG(double __value);
        /*! \copydoc imageMaxG
            \see see imageMaxG for details */ 
        double getImageMaxG() const;
        /*! \copydoc imageMinB
            \see see imageMinB for details */ 
        void setImageMinB(double __value);
        /*! \copydoc imageMinB
            \see see imageMinB for details */ 
        double getImageMinB() const;
        /*! \copydoc imageMaxB
            \see see imageMaxB for details */ 
        void setImageMaxB(double __value);
        /*! \copydoc imageMaxB
            \see see imageMaxB for details */
        double getImageMaxB() const;
        /*! \copydoc autoImageRange
            \see see autoImageRange for details */ 
        void setAutoImageRange(bool __value);
        /*! \copydoc autoImageRange
            \see see autoImageRange for details */ 
        bool getAutoImageRange() const;
        /*! \copydoc imageNameR
            \see see imageNameR for details */
        void setImageName(const QString & __value);
        /*! \copydoc imageNameR
            \see see imageNameR for details */
        QString getImageName() const;
        /*! \copydoc imageNameR
            \see see imageNameR for details */
        QString getImageNameR() const;
        /*! \copydoc imageNameR
            \see see imageNameR for details */
        void setImageNameR(const QString& m);
        /*! \copydoc imageNameG
            \see see imageNameG for details */ 
        void setImageNameG(const QString & __value);
        /*! \copydoc imageNameG
            \see see imageNameG for details */ 
        QString getImageNameG() const;
        /*! \copydoc imageNameB
            \see see imageNameB for details */ 
        void setImageNameB(const QString & __value);
        /*! \copydoc imageNameB
            \see see imageNameB for details */ 
        QString getImageNameB() const;
        /*! \copydoc imageNameFontName
            \see see imageNameFontName for details */ 
        void setImageNameFontName(const QString & __value);
        /*! \copydoc imageNameFontName
            \see see imageNameFontName for details */ 
        QString getImageNameFontName() const;
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
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */
        JKQTPVerticalIndependentAxis* getColorBarRightAxisR();
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */
        JKQTPHorizontalIndependentAxis *getColorBarTopAxisR();
        /*! \brief returns the property colorBarRightAxisG ( \copybrief colorBarRightAxisG ). \details Description of the parameter colorBarRightAxisG is:  <BLOCKQUOTE>\copydoc colorBarRightAxisG </BLOCKQUOTE>. \see colorBarRightAxisG for more information */
        JKQTPVerticalIndependentAxis* getColorBarRightAxisG();
        /*! \brief returns the property colorBarTopAxisG ( \copybrief colorBarTopAxisG ). \details Description of the parameter colorBarTopAxisG is:  <BLOCKQUOTE>\copydoc colorBarTopAxisG </BLOCKQUOTE>. \see colorBarTopAxisG for more information */
        JKQTPHorizontalIndependentAxis* getColorBarTopAxisG();
        /*! \brief returns the property colorBarRightAxisB ( \copybrief colorBarRightAxisB ). \details Description of the parameter colorBarRightAxisB is:  <BLOCKQUOTE>\copydoc colorBarRightAxisB </BLOCKQUOTE>. \see colorBarRightAxisB for more information */
        JKQTPVerticalIndependentAxis* getColorBarRightAxisB();
        /*! \brief returns the property colorBarTopAxisB ( \copybrief colorBarTopAxisB ). \details Description of the parameter colorBarTopAxisB is:  <BLOCKQUOTE>\copydoc colorBarTopAxisB </BLOCKQUOTE>. \see colorBarTopAxisB for more information */
        JKQTPHorizontalIndependentAxis* getColorBarTopAxisB();
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxis() const;
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */
        const JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const;
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxisR() const;
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */
        const JKQTPHorizontalIndependentAxis* getColorBarTopAxisR() const;
        /*! \brief returns the property colorBarRightAxisG ( \copybrief colorBarRightAxisG ). \details Description of the parameter colorBarRightAxisG is:  <BLOCKQUOTE>\copydoc colorBarRightAxisG </BLOCKQUOTE>. \see colorBarRightAxisG for more information */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxisG() const;
        /*! \brief returns the property colorBarTopAxisG ( \copybrief colorBarTopAxisG ). \details Description of the parameter colorBarTopAxisG is:  <BLOCKQUOTE>\copydoc colorBarTopAxisG </BLOCKQUOTE>. \see colorBarTopAxisG for more information */
        const JKQTPHorizontalIndependentAxis *getColorBarTopAxisG() const;
        /*! \brief returns the property colorBarRightAxisB ( \copybrief colorBarRightAxisB ). \details Description of the parameter colorBarRightAxisB is:  <BLOCKQUOTE>\copydoc colorBarRightAxisB </BLOCKQUOTE>. \see colorBarRightAxisB for more information */
        const JKQTPVerticalIndependentAxis* getColorBarRightAxisB() const;
        /*! \brief returns the property colorBarTopAxisB ( \copybrief colorBarTopAxisB ). \details Description of the parameter colorBarTopAxisB is:  <BLOCKQUOTE>\copydoc colorBarTopAxisB </BLOCKQUOTE>. \see colorBarTopAxisB for more information */
        const JKQTPHorizontalIndependentAxis *getColorBarTopAxisB() const;
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
        /*! \copydoc colorbarsSideBySide
            \see see colorbarsSideBySide for details */ 
        void setColorbarsSideBySide(bool __value);
        /*! \copydoc colorbarsSideBySide
            \see see colorbarsSideBySide for details */ 
        bool getColorbarsSideBySide() const;
        /*! \copydoc rgbMode
            \see see rgbMode for details */ 
        void setRgbMode(JKQTPRGBMathImageRGBMode __value);
        /*! \copydoc rgbMode
            \see see rgbMode for details */ 
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

    \image html jkqtplotter_simpletest_rgbimageplot_opencv.png

    \image html jkqtplotter_simpletest_rgbimageplot.png

    \see Examples: \ref JKQTPlotterRGBImagePlot and  \ref JKQTPlotterImagePlotRGBOpenCV
 */
class JKQTP_LIB_EXPORT JKQTPColumnRGBMathImage: public JKQTPRGBMathImage {
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

        /*! \copydoc imageRColumn
            \see see imageRColumn for details */ 
        virtual void setImageRColumn(int __value);
        /*! \copydoc imageRColumn
            \see see imageRColumn for details */ 
        int getImageRColumn() const;
        /*! \copydoc imageGColumn
            \see see imageGColumn for details */ 
        virtual void setImageGColumn(int __value);
        /*! \copydoc imageGColumn
            \see see imageGColumn for details */ 
        int getImageGColumn() const;
        /*! \copydoc imageBColumn
            \see see imageBColumn for details */ 
        virtual void setImageBColumn(int __value);
        /*! \copydoc imageBColumn
            \see see imageBColumn for details */ 
        int getImageBColumn() const;
        /*! \copydoc modifierColumn
            \see see modifierColumn for details */ 
        virtual void setModifierColumn(int __value);
        /*! \copydoc modifierColumn
            \see see modifierColumn for details */ 
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

        void ensureImageData() override;
};



#endif // JKQTPGRAPHSIMAGERGB_H

