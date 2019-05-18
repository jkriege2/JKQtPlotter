/*
    Copyright (c) 2008-2019 Jan W. Krieger & Sebastian Isbaner (contour plot)

    

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


#ifndef JKQTPGRAPHSIMAGEOVERLAYS_H
#define JKQTPGRAPHSIMAGEOVERLAYS_H


#include <QString>
#include <QPainter>
#include <QImage>
#include <QIcon>
#include "jkqtplotter/jkqtpgraphsscatter.h"
#include "jkqtcommon/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"



/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots_overlays

    \image html overlayimage.png

 */
class JKQTP_LIB_EXPORT JKQTPOverlayImage: public JKQTPImageBase {
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
        virtual QColor getKeyLabelColor() const override;

        /*! \copydoc trueColor */ 
        void setTrueColor(const QColor & __value);
        /*! \copydoc trueColor */ 
        QColor getTrueColor() const;
        /*! \copydoc falseColor */ 
        void setFalseColor(const QColor & __value);
        /*! \copydoc falseColor */ 
        QColor getFalseColor() const;
        /*! \copydoc Nx */ 
        void setNx(int __value);
        /*! \copydoc Nx */ 
        int getNx() const;
        /*! \copydoc Ny */ 
        void setNy(int __value);
        /*! \copydoc Ny */ 
        int getNy() const;
        /*! \copydoc data */ 
        virtual void setData(bool*  __value);
        /*! \copydoc data */ 
        bool *getData() const;

        /** \brief set the plot-data to a given array \a data with size \a Nx * \a Ny in row-major ordering */
        virtual void setData(bool* data, int Nx, int Ny);

        /** \brief return the data used for plotting as a QVector<double> in row-major data-ordering */
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
        /** \brief action that calls saveImagePlotAsImage() */
        QAction* actSaveImage;
        /** \brief action that calls copyImagePlotAsImage() */
        QAction* actCopyImage;
    public:
        /** \copydoc JKQTPImageBase::setTitle() */
        virtual void setTitle(const QString& title) override;
        /** \copydoc JKQTPImageBase::setParent() */
        virtual void setParent(JKQTBasePlotter* parent) override;
    public slots:
        /** \brief save the plotted image as a file with \a filename and format \a outputFormat */
        void saveImagePlotAsImage(const QString &filename=QString(""), const QByteArray &outputFormat=QByteArray());
        /** \brief copy the plotted image as an image into the clipboard */
        void copyImagePlotAsImage();

};



/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots_overlays

    In contrast to JKQTPOverlayImage this class draws ist contents as rectangles, not as semi-transparent image. This may lead to nicer results,but could be slower.
    Also it is possible to draw other types of markers (cross, circles, ...)

    \image html overlayimageenhanced.png
 */
class JKQTP_LIB_EXPORT JKQTPOverlayImageEnhanced: public JKQTPOverlayImage {
        Q_OBJECT
    public:
        enum OverlayImageEnhancedDrawMode {
            DrawAsRectangles,
            DrawAsImage,
            DrawAsSymbols
        };

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

        /*! \copydoc symbol */ 
        void setSymbolType(JKQTPGraphSymbols __value);
        /*! \copydoc symbol */ 
        JKQTPGraphSymbols getSymbol() const;
        /*! \copydoc symbolLineWidth */
        void setSymbolLineWidth(double __value);
        /*! \copydoc symbolLineWidth */
        double getSymbolLineWidth() const;
        /*! \copydoc drawMode */
        void setDrawMode(OverlayImageEnhancedDrawMode __value);
        /*! \copydoc drawMode */
        OverlayImageEnhancedDrawMode getDrawMode() const;
        /*! \copydoc symbolSizeFactor */ 
        void setSymbolSizeFactor(double __value);
        /*! \copydoc symbolSizeFactor */ 
        double getSymbolSizeFactor() const;

    protected:
        /** \brief which symbol to use for the datapoints */
        JKQTPGraphSymbols symbol;
        /** \brief width (in pt) of the lines used to plot the symbol for the data points */
        double symbolLineWidth;

        /** \brief indicates whether to draw filled rectangles, or symbols */
        OverlayImageEnhancedDrawMode drawMode;

        /** \brief a rescaling factor  for the symbols */
        double symbolSizeFactor;


};




/*! \brief class to plot an image from an 2-dimensional array of boolean values: alle \c true values are plotted in a given color, while the \c false pixels are drawn in another (default: transparent)
    \ingroup jkqtplotter_imagelots_overlays

    In contrast to JKQTPOverlayImage this class draws ist contents as rectangles, not as semi-transparent image. This may lead to nicer results,but could be slower.
    Also it is possible to draw other types of markers (cross, circles, ...)

    \image html overlayimageenhanced.png

 */
class JKQTP_LIB_EXPORT JKQTPColumnOverlayImageEnhanced: public JKQTPOverlayImageEnhanced {
        Q_OBJECT
    public:
        JKQTPColumnOverlayImageEnhanced(JKQTBasePlotter* parent=nullptr);
        JKQTPColumnOverlayImageEnhanced(JKQTPlotter* parent);

        /*! \copydoc imageColumn */ 
        virtual void setImageColumn(int __value);
        /*! \copydoc imageColumn */ 
        int getImageColumn() const;
        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


    protected:
        /** \brief column to read overlay image from  */
        int imageColumn;
};


#endif // JKQTPGRAPHSIMAGEOVERLAYS_H

