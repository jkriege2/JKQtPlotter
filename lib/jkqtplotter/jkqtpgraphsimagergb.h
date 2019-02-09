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
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplottertools/jkqtp_imexport.h"
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

        virtual void setParent(JKQTBasePlotter* parent) override;



        virtual void setData(void* data, void* dataG, void* dataB, int Nx, int Ny, DataType datatype);
        virtual void setData(void* data, void* dataG, void* dataB,  int Nx, int Ny);

        virtual void setData(void* data, int Nx, int Ny, DataType datatype) override;
        virtual void setData(void* data,  int Nx, int Ny) override ;


        /** \brief determine min/max data value of the image */
        virtual void getDataMinMax(double& imin, double& imax) override;


        /*! \copydoc dataG
            \see see dataG for details */ 
        inline virtual void setDataG(void* __value)  
        {
            this->dataG = __value;
        } 
        /*! \copydoc dataG
            \see see dataG for details */ 
        inline virtual void* getDataG() const  
        {
            return this->dataG; 
        }
        /*! \copydoc datatypeG
            \see see datatypeG for details */ 
        inline virtual void setDatatypeG(const DataType & __value)  
        {
            this->datatypeG = __value;
        } 
        /*! \copydoc datatypeG
            \see see datatypeG for details */ 
        inline virtual DataType getDatatypeG() const  
        {
            return this->datatypeG; 
        }
        /*! \copydoc dataB
            \see see dataB for details */ 
        inline virtual void setDataB(void* __value)  
        {
            this->dataB = __value;
        } 
        /*! \copydoc dataB
            \see see dataB for details */ 
        inline virtual void* getDataB() const  
        {
            return this->dataB; 
        }
        /*! \copydoc datatypeB
            \see see datatypeB for details */ 
        inline virtual void setDatatypeB(const DataType & __value)  
        {
            this->datatypeB = __value;
        } 
        /*! \copydoc datatypeB
            \see see datatypeB for details */ 
        inline virtual DataType getDatatypeB() const  
        {
            return this->datatypeB; 
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
        inline virtual void setColorBarWidth(int __value)
        {
            this->colorBarWidth = __value;
        } 
        /*! \copydoc colorBarWidth
            \see see colorBarWidth for details */ 
        inline virtual int getColorBarWidth() const  
        {
            return this->colorBarWidth; 
        }
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        inline virtual void setColorBarOffset(int __value)
        {
            this->colorBarOffset = __value;
        } 
        /*! \copydoc colorBarOffset
            \see see colorBarOffset for details */ 
        inline virtual int getColorBarOffset() const  
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
        inline void setImageMinR(double m) {
            setImageMin(m);
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
        inline void setImageMaxR(double m) {
            setImageMax(m);
        }
        /*! \copydoc imageMinG
            \see see imageMinG for details */ 
        inline virtual void setImageMinG(double __value)
        {
            this->imageMinG = __value;
        } 
        /*! \copydoc imageMinG
            \see see imageMinG for details */ 
        inline virtual double getImageMinG() const  
        {
            return this->imageMinG; 
        }
        /*! \copydoc imageMaxG
            \see see imageMaxG for details */ 
        inline virtual void setImageMaxG(double __value)
        {
            this->imageMaxG = __value;
        } 
        /*! \copydoc imageMaxG
            \see see imageMaxG for details */ 
        inline virtual double getImageMaxG() const  
        {
            return this->imageMaxG; 
        }
        /*! \copydoc imageMinB
            \see see imageMinB for details */ 
        inline virtual void setImageMinB(double __value)
        {
            this->imageMinB = __value;
        } 
        /*! \copydoc imageMinB
            \see see imageMinB for details */ 
        inline virtual double getImageMinB() const  
        {
            return this->imageMinB; 
        }
        /*! \copydoc imageMaxB
            \see see imageMaxB for details */ 
        inline virtual void setImageMaxB(double __value)
        {
            this->imageMaxB = __value;
        } 
        /*! \copydoc imageMaxB
            \see see imageMaxB for details */ 
        inline virtual double getImageMaxB() const  
        {
            return this->imageMaxB; 
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
        inline void setImageNameR(const QString& m) {
            setImageName(m);
        }
        /*! \copydoc imageNameG
            \see see imageNameG for details */ 
        inline virtual void setImageNameG(const QString & __value)  
        {
            this->imageNameG = __value;
        } 
        /*! \copydoc imageNameG
            \see see imageNameG for details */ 
        inline virtual QString getImageNameG() const  
        {
            return this->imageNameG; 
        }
        /*! \copydoc imageNameB
            \see see imageNameB for details */ 
        inline virtual void setImageNameB(const QString & __value)  
        {
            this->imageNameB = __value;
        } 
        /*! \copydoc imageNameB
            \see see imageNameB for details */ 
        inline virtual QString getImageNameB() const  
        {
            return this->imageNameB; 
        }
        /*! \copydoc imageNameFontName
            \see see imageNameFontName for details */ 
        inline virtual void setImageNameFontName(const QString & __value)  
        {
            this->imageNameFontName = __value;
        } 
        /*! \copydoc imageNameFontName
            \see see imageNameFontName for details */ 
        inline virtual QString getImageNameFontName() const  
        {
            return this->imageNameFontName; 
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
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxis() { return this->colorBarRightAxis; }
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxis()  { return this->colorBarTopAxis; }
        /*! \brief returns the property colorBarRightAxisG ( \copybrief colorBarRightAxisG ). \details Description of the parameter colorBarRightAxisG is:  <BLOCKQUOTE>\copydoc colorBarRightAxisG </BLOCKQUOTE>. \see colorBarRightAxisG for more information */
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxisG()  { return this->colorBarRightAxisG; }
        /*! \brief returns the property colorBarTopAxisG ( \copybrief colorBarTopAxisG ). \details Description of the parameter colorBarTopAxisG is:  <BLOCKQUOTE>\copydoc colorBarTopAxisG </BLOCKQUOTE>. \see colorBarTopAxisG for more information */
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxisG()  { return this->colorBarTopAxisG; }
        /*! \brief returns the property colorBarRightAxisB ( \copybrief colorBarRightAxisB ). \details Description of the parameter colorBarRightAxisB is:  <BLOCKQUOTE>\copydoc colorBarRightAxisB </BLOCKQUOTE>. \see colorBarRightAxisB for more information */
        inline JKQTPVerticalIndependentAxis* getColorBarRightAxisB()  { return this->colorBarRightAxisB; }
        /*! \brief returns the property colorBarTopAxisB ( \copybrief colorBarTopAxisB ). \details Description of the parameter colorBarTopAxisB is:  <BLOCKQUOTE>\copydoc colorBarTopAxisB </BLOCKQUOTE>. \see colorBarTopAxisB for more information */
        inline JKQTPHorizontalIndependentAxis* getColorBarTopAxisB()  { return this->colorBarTopAxisB; }
        /*! \brief returns the property colorBarRightAxis ( \copybrief colorBarRightAxis ). \details Description of the parameter colorBarRightAxis is:  <BLOCKQUOTE>\copydoc colorBarRightAxis </BLOCKQUOTE>. \see colorBarRightAxis for more information */
        inline const JKQTPVerticalIndependentAxis* getColorBarRightAxis() const { return this->colorBarRightAxis; }
        /*! \brief returns the property colorBarTopAxis ( \copybrief colorBarTopAxis ). \details Description of the parameter colorBarTopAxis is:  <BLOCKQUOTE>\copydoc colorBarTopAxis </BLOCKQUOTE>. \see colorBarTopAxis for more information */
        inline const JKQTPHorizontalIndependentAxis* getColorBarTopAxis() const { return this->colorBarTopAxis; }
        /*! \brief returns the property colorBarRightAxisG ( \copybrief colorBarRightAxisG ). \details Description of the parameter colorBarRightAxisG is:  <BLOCKQUOTE>\copydoc colorBarRightAxisG </BLOCKQUOTE>. \see colorBarRightAxisG for more information */
        inline const JKQTPVerticalIndependentAxis* getColorBarRightAxisG() const { return this->colorBarRightAxisG; }
        /*! \brief returns the property colorBarTopAxisG ( \copybrief colorBarTopAxisG ). \details Description of the parameter colorBarTopAxisG is:  <BLOCKQUOTE>\copydoc colorBarTopAxisG </BLOCKQUOTE>. \see colorBarTopAxisG for more information */
        inline const JKQTPHorizontalIndependentAxis* getColorBarTopAxisG() const { return this->colorBarTopAxisG; }
        /*! \brief returns the property colorBarRightAxisB ( \copybrief colorBarRightAxisB ). \details Description of the parameter colorBarRightAxisB is:  <BLOCKQUOTE>\copydoc colorBarRightAxisB </BLOCKQUOTE>. \see colorBarRightAxisB for more information */
        inline const JKQTPVerticalIndependentAxis* getColorBarRightAxisB() const { return this->colorBarRightAxisB; }
        /*! \brief returns the property colorBarTopAxisB ( \copybrief colorBarTopAxisB ). \details Description of the parameter colorBarTopAxisB is:  <BLOCKQUOTE>\copydoc colorBarTopAxisB </BLOCKQUOTE>. \see colorBarTopAxisB for more information */
        inline const JKQTPHorizontalIndependentAxis* getColorBarTopAxisB() const { return this->colorBarTopAxisB; }
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
        /*! \copydoc colorbarsSideBySide
            \see see colorbarsSideBySide for details */ 
        inline virtual void setColorbarsSideBySide(bool __value)
        {
            this->colorbarsSideBySide = __value;
        } 
        /*! \copydoc colorbarsSideBySide
            \see see colorbarsSideBySide for details */ 
        inline virtual bool getColorbarsSideBySide() const  
        {
            return this->colorbarsSideBySide; 
        }
        /*! \copydoc rgbMode
            \see see rgbMode for details */ 
        inline virtual void setRgbMode(const JKQTPRGBMathImageRGBMode & __value)  
        {
            this->rgbMode = __value;
        } 
        /*! \copydoc rgbMode
            \see see rgbMode for details */ 
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
        inline virtual void setImageRColumn(int __value)
        {
            this->imageRColumn = __value;
        } 
        /*! \copydoc imageRColumn
            \see see imageRColumn for details */ 
        inline virtual int getImageRColumn() const  
        {
            return this->imageRColumn; 
        }
        /*! \copydoc imageGColumn
            \see see imageGColumn for details */ 
        inline virtual void setImageGColumn(int __value)
        {
            this->imageGColumn = __value;
        } 
        /*! \copydoc imageGColumn
            \see see imageGColumn for details */ 
        inline virtual int getImageGColumn() const  
        {
            return this->imageGColumn; 
        }
        /*! \copydoc imageBColumn
            \see see imageBColumn for details */ 
        inline virtual void setImageBColumn(int __value)
        {
            this->imageBColumn = __value;
        } 
        /*! \copydoc imageBColumn
            \see see imageBColumn for details */ 
        inline virtual int getImageBColumn() const  
        {
            return this->imageBColumn; 
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

