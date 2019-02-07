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
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplottertools/jkqtp_imexport.h"
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
        virtual QColor getKeyLabelColor() override;

        /*! \copydoc trueColor
            \see see trueColor for details */ 
        inline virtual void setTrueColor(const QColor & __value)  
        {
            this->trueColor = __value;
        } 
        /*! \copydoc trueColor
            \see see trueColor for details */ 
        inline virtual QColor getTrueColor() const  
        {
            return this->trueColor; 
        }
        /*! \copydoc falseColor
            \see see falseColor for details */ 
        inline virtual void setFalseColor(const QColor & __value)  
        {
            this->falseColor = __value;
        } 
        /*! \copydoc falseColor
            \see see falseColor for details */ 
        inline virtual QColor getFalseColor() const  
        {
            return this->falseColor; 
        }
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
        inline virtual void setData(bool*  __value)
        {
            this->data = __value;
        } 
        /*! \copydoc data
            \see see data for details */ 
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
    \ingroup jkqtplotter_imagelots_overlays

    In contrast to JKQTPOverlayImage this class draws ist contents as rectangles, not as semi-transparent image. This may lead to nicer results,but could be slower.
    Also it is possible to draw other types of markers (cross, circles, ...)

    \image html overlayimageenhanced.png
 */
class JKQTP_LIB_EXPORT JKQTPOverlayImageEnhanced: public JKQTPOverlayImage {
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

        /*! \copydoc symbol
            \see see symbol for details */ 
        inline virtual void setSymbol(const JKQTPGraphSymbols & __value)  
        {
            this->symbol = __value;
        } 
        /*! \copydoc symbol
            \see see symbol for details */ 
        inline virtual JKQTPGraphSymbols getSymbol() const  
        {
            return this->symbol; 
        }
        /*! \copydoc symbolWidth
            \see see symbolWidth for details */ 
        inline virtual void setSymbolWidth(double __value)
        {
            this->symbolWidth = __value;
        } 
        /*! \copydoc symbolWidth
            \see see symbolWidth for details */ 
        inline virtual double getSymbolWidth() const  
        {
            return this->symbolWidth; 
        }
        /*! \copydoc drawAsRectangles
            \see see drawAsRectangles for details */ 
        inline virtual void setDrawAsRectangles(bool __value)
        {
            this->drawAsRectangles = __value;
        } 
        /*! \copydoc drawAsRectangles
            \see see drawAsRectangles for details */ 
        inline virtual bool getDrawAsRectangles() const  
        {
            return this->drawAsRectangles; 
        }
        /*! \copydoc rectanglesAsImageOverlay
            \see see rectanglesAsImageOverlay for details */ 
        inline virtual void setRectanglesAsImageOverlay(bool __value)
        {
            this->rectanglesAsImageOverlay = __value;
        } 
        /*! \copydoc rectanglesAsImageOverlay
            \see see rectanglesAsImageOverlay for details */ 
        inline virtual bool getRectanglesAsImageOverlay() const  
        {
            return this->rectanglesAsImageOverlay; 
        }
        /*! \copydoc symbolSizeFactor
            \see see symbolSizeFactor for details */ 
        inline virtual void setSymbolSizeFactor(double __value)
        {
            this->symbolSizeFactor = __value;
        } 
        /*! \copydoc symbolSizeFactor
            \see see symbolSizeFactor for details */ 
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
        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


    protected:
        /** \brief top color bar visible */
        int imageColumn;
};


#endif // JKQTPGRAPHSIMAGEOVERLAYS_H

