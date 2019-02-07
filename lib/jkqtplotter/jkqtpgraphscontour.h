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


#ifndef JKQTPGRAPHSCONTOUR_H
#define JKQTPGRAPHSCONTOUR_H


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
class JKQTP_LIB_EXPORT JKQTPContour: public JKQTPMathImage {
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

        /*! \copydoc lineColor
            \see see lineColor for details */ 
        inline virtual void setLineColor(const QColor & __value)  
        {
            this->lineColor = __value;
        } 
        /*! \copydoc lineColor
            \see see lineColor for details */ 
        inline virtual QColor getLineColor() const  
        {
            return this->lineColor; 
        }
        /*! \copydoc style
            \see see style for details */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \copydoc style
            \see see style for details */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \copydoc lineWidth
            \see see lineWidth for details */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \copydoc ignoreOnPlane
            \see see ignoreOnPlane for details */ 
        inline virtual void setIgnoreOnPlane(bool __value)
        {
            this->ignoreOnPlane = __value;
        } 
        /*! \copydoc ignoreOnPlane
            \see see ignoreOnPlane for details */ 
        inline virtual bool getIgnoreOnPlane() const  
        {
            return this->ignoreOnPlane; 
        }
        /*! \copydoc numberOfLevels
            \see see numberOfLevels for details */ 
        inline virtual void setNumberOfLevels(int __value)
        {
            this->numberOfLevels = __value;
        } 
        /*! \copydoc numberOfLevels
            \see see numberOfLevels for details */ 
        inline virtual int getNumberOfLevels() const  
        {
            return this->numberOfLevels; 
        }
        /*! \copydoc colorFromPalette
            \see see colorFromPalette for details */ 
        inline virtual void setColorFromPalette(bool __value)
        {
            this->colorFromPalette = __value;
        } 
        /*! \copydoc colorFromPalette
            \see see colorFromPalette for details */ 
        inline virtual bool getColorFromPalette() const  
        {
            return this->colorFromPalette; 
        }
        /*! \copydoc contourLevels
            \see see contourLevels for details */ 
        inline virtual void setContourLevels(const QList<double> & __value)  
        {
            this->contourLevels = __value;
        } 
        /*! \copydoc contourLevels
            \see see contourLevels for details */ 
        inline virtual QList<double> getContourLevels() const  
        {
            return this->contourLevels; 
        }
        /*! \copydoc relativeLevels
            \see see relativeLevels for details */ 
        inline virtual void setRelativeLevels(bool __value)
        {
            this->relativeLevels = __value;
        } 
        /*! \copydoc relativeLevels
            \see see relativeLevels for details */ 
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





#endif // JKQTPGRAPHSCONTOUR_H

