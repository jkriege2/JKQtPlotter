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
#include <QVector3D>
#include <QIcon>
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"



/*! \brief class for a contour plot
 *  \ingroup jkqtplotter_imagelots_contour
 *
 *  This class calculates the contour plot for a given image \f$ f(x,y) \f$ , i.e. it
 *  finds the iso lines/polygons, connecting all neighboring points \f$ \{(x,y)|f(x,y)\equiv L_i\} \f$
 *  that equal a given set of contour levels \f$ \{L_i\} \f$ .
 *
 *  For its calculations, this class implements the CONREC algorithm
 *  (see <a href="http://paulbourke.net/papers/conrec/">http://paulbourke.net/papers/conrec/</a>).
 *
 *  In order to use this class, you first have to set an image column with the data for this plot and then
 *  define several contour levels for which to plot the contour lines. See \ref JKQTPlotterContourPlot for a
 *  detailed example!
 *
 *  \note Although this class is derived from JKQTPColumnMathImage, which supports a modifier to the data,
 *        (see JKQTPColumnMathImage::modifierColumn), these features do not make sense for a contour plot
 *        and are not used here. The access functions are hidden in this class, but may be accessible
 *        via a down-cast! Still they do not have any effect on the plotted graph!
 *
 *  \see See \ref JKQTPlotterContourPlot for a detailed example.
 *       See https://en.wikipedia.org/wiki/Contour_line for details on Contour Lines and Plots.
 *       The implementation for Qt is adapted from Qwt: <a href="http://qwt.sourceforge.net/">http://qwt.sourceforge.net/</a>
 *
 * \author Sebastian Isbaner (2013-2014) & Jan Krieger (2019)
 *
 * \image html JKQTPContour.png
 *
 * \image html contourplot.png
 *
 * \note This class uses a caching strategy to determine whether contour lines need to be recalculated (which is time-consuming):
 *       Whenever contours have been calculated, these contours are stored in the member contourLinesCache and can then be used for
 *       redrawing. In addition the checksum of the data column is stored in contourLinesCachedForChecksum. On every redraw,
 *       the checksum is recalculated and compared to the value in contourLinesCachedForChecksum. If it matches, no recalculation
 *       is necessary. If it doesn't match, the contours are recalculated. In addition several methods that change certain
 *       properties of the plot will force a recalculation. Finally you can call clearCachedContours() to force a recalculation!
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPContourPlot: public JKQTPMathImage, public JKQTPGraphLineStyleMixin  {
        Q_OBJECT
    public:
        /** \brief options of how the colors for the contours are chosen (note that all images in the documentation show the same graph, just with different coloring modes!)*/
        enum ContourColoringMode {
            SingleColorContours, /*!< \brief each contour line has the same color \image html contourplot_SingleColorContours.png */
            ColorContoursFromPaletteByValue, /*!< \brief the color for each contour line is determined by the current color palette, image range and the value associated with each contour line \image html contourplot_ColorContoursFromPaletteByValue.png */
            ColorContoursFromPalette, /*!< \brief the color for each contour line is chosen from the current color palette, distributing the contour lines evenly over the palette, wirhout any connection to the value associated with each contour line \image html contourplot_ColorContoursFromPalette.png */
        };

        /** \brief class constructor */
        JKQTPContourPlot(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPContourPlot(JKQTPlotter* parent);
        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

        /** \brief creates at nLevels contour levels linearly spaced between the data's minimum and maximum values */
        void createContourLevels(int nLevels=3);
        /** \brief creates at least nLevels contour levels with logarithmic spacing. FIXME: Has not been tested yet */
        void createContourLevelsLog(int nLevels=3,int m=2);

        /*! \copydoc ignoreOnPlane */ 
        void setIgnoreOnPlane(bool __value);
        /*! \copydoc ignoreOnPlane */ 
        bool getIgnoreOnPlane() const;
        /*! \copydoc contourLevels */
        int getNumberOfLevels() const;
        /*! \copydoc contourColoringMode */
        void setContourColoringMode(ContourColoringMode __value);
        /*! \copydoc contourColoringMode */
        ContourColoringMode getContourColoringMode() const;
        /** \brief sets new contour levels from a container \a levels
         *
         * \tparam TConatiner any standard container with c++ standard iterators. The contents needs to be convertible to double via jkqtp_todouble()
         * \param levels the container to copy from
         */
        template <class TContainer>
        void setContourLevels(const TContainer &levels);
        /** \brief sets new contour levels from a container \a levels
         *
         * \tparam TConatiner any standard container with c++ standard iterators. The contents needs to be convertible to double via jkqtp_todouble()
         * \tparam TColorContainer any standard container with c++ standard iterators. The contents needs to be convertible to QColor
         * \param levels the container to copy from
         * \param colors a container with QColors inside, which define the line colors
         */
        template <class TContainer, class TColorContainer>
        void setContourLevels(const TContainer &levels, const TColorContainer& colors);
        /*! \copydoc contourLevels */
        QVector<double> getContourLevels() const;
        /*! \copydoc relativeLevels */ 
        void setRelativeLevels(bool __value);
        /*! \copydoc relativeLevels */ 
        bool getRelativeLevels() const;
        /** \brief add another level for which to draw a contour
         *  \see setOverrideColor(), addContourLevel(), hasOverrideColor(), removeOverrideColor(), getOverrideColor()
         */
        void addContourLevel(double level);
        /** \brief add another level for which to draw a contour and define an override color for this line
         *
         *  \image html contourplot_overridecolors.png
         *
         *  \see setOverrideColor(), addContourLevel(), hasOverrideColor(), removeOverrideColor(), getOverrideColor()
         */
        void addContourLevel(double level, QColor overrideColor);
        /** \brief define an override color for the contour line at the given level
         *
         *  \image html contourplot_overridecolors.png
         *
         *  \see setOverrideColor(), addContourLevel(), hasOverrideColor(), removeOverrideColor(), getOverrideColor()
         */
        void setOverrideColor(double level, QColor overrideColor);
        /** \brief returns the override color for contour level \a level
         *  \see setOverrideColor(), addContourLevel(), hasOverrideColor(), removeOverrideColor(), getOverrideColor()
         */
        QColor getOverrideColor(int level) const;
        /** \brief returns the override color for contour level \a level
         *  \see setOverrideColor(), addContourLevel(), hasOverrideColor(), removeOverrideColor(), getOverrideColor()
         */
        bool hasOverrideColor(int level) const;
        /** \brief removes the override color for contour level \a level
         *  \see setOverrideColor(), addContourLevel(), hasOverrideColor(), removeOverrideColor(), getOverrideColor()
         */
        void removeOverrideColor(int level) ;
        /** \brief clear all currently defined contour levels */
        void clearContourLevel();

        /** \brief overrides the caching strategy and forces a recalculation of the contours */
        void clearCachedContours();

    protected:
        /** \brief if true, vertices that all lie on the contour plane will be ignored*/
        bool ignoreOnPlane;
        /** \brief defines how the colors for the contours are chosen */
        ContourColoringMode contourColoringMode;

        /** \brief the list of contour levels */
        QVector<double> contourLevels;
        /** \brief override contour level colors for specific levels, when \c contourColoringMode==SingleColorContours */
        QMap<double,QColor> contourOverrideColor;
        /** \brief indicates wether the contour levels are absolute values or relative to the maximum (max-min) */
        bool relativeLevels;



        // hide unusable members (this class does not provide data modifier functionality
        using JKQTPMathImage::setDataModifier;
        using JKQTPMathImage::getDataModifier;
        using JKQTPMathImage::setDatatypeModifier;
        using JKQTPMathImage::getDatatypeModifier;
        using JKQTPMathImage::setModifierMode;
        using JKQTPMathImage::getModifierMode;
        using JKQTPMathImage::getModifierMinMax;
        using JKQTPMathImage::getDataModifierAsDoubleVector;
        using JKQTPMathImage::setColorBarModifiedWidth;
        using JKQTPMathImage::getColorBarModifiedWidth;
        using JKQTPMathImage::getModifierColorBarTopAxis;
        using JKQTPMathImage::getModifierColorBarRightAxis;
        using JKQTPMathImage::setModifierMin;
        using JKQTPMathImage::getModifierMin;
        using JKQTPMathImage::setAutoModifierRange;
        using JKQTPMathImage::getAutoModifierRange;
        using JKQTPMathImage::setModifierMax;
        using JKQTPMathImage::getModifierMax;
        using JKQTPMathImage::getModifierValueAt;
        using JKQTPMathImage::getModifierSampleSize;
        using JKQTPMathImage::getModifierPixelValue;
    private:
        /** \brief  gives the intersection line of a plane defined by three vertices with a contour level in the x-y plane of heigth level */
        inline bool intersect(QLineF &line, const QVector3D &vertex1,const QVector3D &vertex2,const QVector3D &vertex3,double level) const;
        /**
         * @brief interpolatePoint linear interpolation of a line to the x-y plane using the z-value relative to level
         * @param point1 start point of the line
         * @param point2 end point of the line
         * @param level the heigth for the interpolation (this z-value is projected onto the xy-plane)
         * @return the interpolated point in the x-y plane
         */
        inline QPointF interpolatePoint(const QVector3D &point1, const QVector3D &point2,double level) const ;
        /**
         * @brief compare2level checks if the vertex lies above, under or on the contour plane level
         * @param vertex
         * @param level contour plane
         * @return above (1), on the plane (0), below (-1);
         */
        inline int compare2level(const QVector3D &vertex, float level) const ;
        /** \brief calcContourLines calculates the contour lines using the CONREC algorithm */
        void calcContourLines(QList<QVector<QLineF > > &ContourLines);
        /** \brief For Caching: the contour lines as vector of single lines (one for each triangle). the list index refers to the contour level.
         *  \internal
         *
         *  This property is used to store the last result of the contour line calculation. This is done, because
         *  calculating contour lines is rather expensive. In order to determine the need for a recalculation,
         *  the data in the column is converted to a checksum, which can be compared to the sum value stored in
         *  contourLinesCachedForChecksum.
         *
         *  \see contourLinesCachedForChecksum
         */
        QList<QVector<QPolygonF > > contourLinesCache; // contour levels, squares on grid, line
        /** \brief if (>=0) checksum of the column for which contours were cached in contourLinesCache
         *  \internal
         *  \see contourLinesCache
         */
        int64_t contourLinesCachedForChecksum;

};





/** \brief class for a contour plots (same as JKQTPContourPlot) of images stored in a JKQTPDatastore column
 *  \ingroup jkqtplotter_imagelots_contour
 *
 *  \copydetails JKQTPContourPlot
 *
 *  \see jkqtpstatAddKDE2DContour(), jkqtpstatAddHistogram2DContour(), \ref JKQTPlotterContourPlot, \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPColumnContourPlot: public JKQTPContourPlot  {
        Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPColumnContourPlot(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPColumnContourPlot(JKQTPlotter* parent);

        /*! \copydoc imageColumn */
        void setImageColumn(int __value);

        /*! \copydoc imageColumn */
        void setImageColumn(size_t __value);
        /*! \copydoc imageColumn */
        int getImageColumn() const;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;



    protected:
        /** \brief  column containing the displayed image */
        int imageColumn;

        virtual void ensureImageData() override;

};











////////////////////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TColorContainer>
void JKQTPContourPlot::setContourLevels(const TContainer &levels, const TColorContainer &colors)
{
    contourLevels.clear();
    contourOverrideColor.clear();
    auto itc=colors.begin();
    for (auto it=levels.begin(); it!=levels.end(); ++it) {
        const double v=jkqtp_todouble(*it);
        contourLevels<<v;
        contourOverrideColor[v]=*itc;
        ++itc;
    }
    std::sort(contourLevels.begin(), contourLevels.end());
    clearCachedContours();

}

////////////////////////////////////////////////////////////////////////////////////////////////
template<class TContainer>
void JKQTPContourPlot::setContourLevels(const TContainer &levels)
{
    contourLevels.clear();
    contourOverrideColor.clear();
    for (auto it=levels.begin(); it!=levels.end(); ++it) {
        contourLevels<<jkqtp_todouble(*it);
    }
    std::sort(contourLevels.begin(), contourLevels.end());
    clearCachedContours();

}


////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPContourPlot::intersect(QLineF &line, const QVector3D &vertex1, const QVector3D &vertex2, const QVector3D &vertex3, double level) const
{
    bool found = true;

    // Are the vertices below (-1), on (0) or above (1) the plane ?
    const int eq1 = compare2level(vertex1,level);
    const int eq2 = compare2level(vertex2,level);
    const int eq3 = compare2level(vertex3,level);

    /*
        (a) All the vertices lie below the contour level.
        (b) Two vertices lie below and one on the contour level.
        (c) Two vertices lie below and one above the contour level.
        (d) One vertex lies below and two on the contour level.
        (e) One vertex lies below, one on and one above the contour level.
        (f) One vertex lies below and two above the contour level.
        (g) Three vertices lie on the contour level.
        (h) Two vertices lie on and one above the contour level.
        (i) One vertex lies on and two above the contour level.
        (j) All the vertices lie above the contour level.
     */

    static const int caseLUT[3][3][3] =
    {
        // jump table to avoid nested case statements
        { { 0, 0, 8 }, { 0, 2, 5 }, { 7, 6, 9 } },
        { { 0, 3, 4 }, { 1, 10, 1 }, { 4, 3, 0 } },
        { { 9, 6, 7 }, { 5, 2, 0 }, { 8, 0, 0 } }
    };

    const int caseType = caseLUT[eq1+1][eq2+1][eq3+1];
    switch (caseType)
    {
        case 1:
            // d(0,0,-1), h(0,0,1)
            line.setP1(vertex1.toPointF());
            line.setP2(vertex2.toPointF());
            break;
        case 2:
            // d(-1,0,0), h(1,0,0)
            line.setP1(vertex2.toPointF());
            line.setP2(vertex3.toPointF());
            break;
        case 3:
            // d(0,-1,0), h(0,1,0)
            line.setP1(vertex3.toPointF());
            line.setP2(vertex1.toPointF());
            break;
        case 4:
            // e(0,-1,1), e(0,1,-1)
            line.setP1(vertex1.toPointF());
            line.setP2(interpolatePoint(vertex2, vertex3, level));
            break;
        case 5:
            // e(-1,0,1), e(1,0,-1)
            line.setP1(vertex2.toPointF());
            line.setP2(interpolatePoint(vertex3, vertex1, level));
            break;
        case 6:
            // e(-1,1,0), e(1,0,-1)
            line.setP1(vertex3.toPointF());
            line.setP2(interpolatePoint(vertex1, vertex2, level));
            break;
        case 7:
            // c(-1,1,-1), f(1,1,-1)
            line.setP1(interpolatePoint(vertex1, vertex2, level));
            line.setP2(interpolatePoint(vertex2, vertex3, level));
            break;
        case 8:
            // c(-1,-1,1), f(1,1,-1)
            line.setP1(interpolatePoint(vertex2, vertex3, level));
            line.setP2(interpolatePoint(vertex3, vertex1, level));
            break;
        case 9:
            // f(-1,1,1), c(1,-1,-1)
            line.setP1(interpolatePoint(vertex3, vertex1, level));
            line.setP2(interpolatePoint(vertex1, vertex2, level));
            break;
        case 10:
            // g(0,0,0)
            // The CONREC algorithm has no satisfying solution for
            // what to do, when all vertices are on the plane.

            if ( ignoreOnPlane )
                found = false;
            else
            {
                line.setP1(vertex3.toPointF());
                line.setP2(vertex1.toPointF());
            }
            break;
        default:
            found = false;
    }
    //    qDebug()<<caseType;
    //    qDebug()<<line;
    return found;
}

////////////////////////////////////////////////////////////////////////////////////////////////
QPointF JKQTPContourPlot::interpolatePoint(const QVector3D &point1, const QVector3D &point2, double level) const
{
    const double h1 = point1.z() - level; // height above contour level
    const double h2 = point2.z() - level;

    //    // check if h1 or h2 is zero
    //    Division by zero is not possible (the intersect function is not called if h2-h1 is zero, !)
    //    if(h2-h1==0||h1==0||h2==0) {
    //        qDebug()<<h1<<h2;
    //        qDebug()<<"interpolate p1="<<point1<<", p2="<<point2<<" level="<<level;
    //    }

    const double x = (h2 * point1.x() - h1 * point2.x()) / (h2 - h1); // linear interpolation in x.direction (independent of y)
    const double y = (h2 * point1.y() - h1 * point2.y()) / (h2 - h1);

    //    const double alpha=(level-point1.z())/(point2.z()-point1.z());
    //    const double x=point1.x()+alpha*(point2.x()-point1.x());
    //    const double y=point1.y()+alpha*(point2.y()-point1.y());

    return QPointF(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPContourPlot::compare2level(const QVector3D &vertex, float level) const {
    if (vertex.z() > level)  {
        return 1;
    }

    if (vertex.z() < level) {
        return -1;
    }

    return 0;
}





#endif // JKQTPGRAPHSCONTOUR_H

