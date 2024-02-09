/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#ifndef jkqtpvectorfield_H
#define     jkqtpvectorfield_H


#include <QString>
#include <QPainter>
#include <QPair>
#include <functional>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;



/*! \brief This graph plots a vector field, i.e. a set of vectors (dx,dy) or (angle,length) at positions (x,y).
           This class immplements the most basic form of vector plot, i.e. the vector are drawn with a length
           corresponding to their magnitude.
    \ingroup jkqtplotter_vectorfieldgraphs

    \note This type of plot is sometimes also refered to as <b>quiver plot</b> (e.g. in Matlab or matplotlib)

    \image html JKQTPVectorFieldGraph.png

    To achieve this, use code like this:
    \code
        // 1. setup a plotter window and get a pointer to the internal datastore (for convenience)
        JKQTPlotter plot;
        JKQTPDatastore* ds=plot.getDatastore();

        // 2. make up some arbitrary data to be used for plotting
        //    this generates a 2D grid of x/y-coordinates and then calculates dx=cos(y)*sqrt(x/3.0) and dy=sin(x)*sqrt(x/3.0)
        const auto columnXY=ds->addLinearGridColumns(NX, 0, 6, NY, -3, 3,"x","y");
        const auto columnDX=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return sin(y)*sqrt(x/3.0); });
        const auto columnDY=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return cos(x)*sqrt(x/3.0); });

        // 3. create JKQTPVectorFieldGraph to display the data:
        JKQTPVectorFieldGraph* graph1=new JKQTPVectorFieldGraph(&plot);
        graph1->setXYColumns(columnXY);
        graph1->setDxColumn(columnDX);
        graph1->setDyColumn(columnDY);
        graph1->setTitle(QObject::tr("$\\vec{f}(x,y)=\\bigl[\\sin(y)\\cdot\\sqrt{x/3}, \\cos(x)\\cdot\\sqrt{x/3}\\bigr]^\\mathrm{T}$"));

        // 4. add the graphs to the plot, so it is actually displayed
        plot.addGraph(graph1);
    \endcode

    You have several options to influence the way the vectors are drawn:
      1. You can change the tip shape (and actually also the tail) of the vector by using the methods from
         JKQTPGraphDecoratedLineStyleMixin, e.g. use JKQTPGraphDecoratedLineStyleMixin::setHeadDecoratorStyle()
         to set another shape for the vector's tip, or modify JKQTPGraphDecoratedLineStyleMixin::setHeadDecoratorSizeFactor()
         to modify the size of the vector's head. the vector line width, color etz. can be modified by the
         methods from JKQTPGraphLineStyleMixin, like JKQTPGraphLineStyleMixin::setLineColor() or
         JKQTPGraphLineStyleMixin::setLineWidth().
      2. By default the length of the drawn vectors corresponds to the actual length of the vector data,
         but is modified by an autoscaling algorithm that should prevent them from overlapping.
         This behaviour can be changed by setVectorLengthMode() with the different options described in
         VectorLengthMode.
      3. By default, vector start at \c (x,y). But you can also make them be centered around  or
         point to \c (x,y) . This can be set by setAnchorPoint().
      4. Using setVectorLineWidthMode(JKQTPVectorFieldGraph::AutoscaleLineWidthFromLength) you can also
         scale the line width of each vector by the vector's length. This sometimes gives an easier to read
         representation of the vector properties, especially if combined with
         setVectorLengthMode(JKQTPVectorFieldGraph::IgnoreLength) :
           \image html JKQTPVectorFieldGraphIgnoreLengthAutoscaleLineWidthFromLength.png
    .

    \see \ref JKQTPlotterVectorFieldExample , JKQTPGraphDecoratedLineStyleMixin , JKQTPXYAndVectorGraph

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPVectorFieldGraph: public JKQTPXYAndVectorGraph, public JKQTPGraphDecoratedLineStyleMixin {
        Q_OBJECT
    public:
        /** \brief indicates the position of the point \c (x,y) relative to the vector */
        enum VectorAnchorPoint {
            AnchorBottom,  //!< \brief this is the default: the vector starts at \c (x,y)  \image html JKQTPVectorFieldGraphAnchorBottom.png
            AnchorMid,     //!< \brief the vector's mid is at \c (x,y)  \image html JKQTPVectorFieldGraphAnchorMid.png
            AnchorTip      //!< \brief the vector ends at \c (x,y) \image html JKQTPVectorFieldGraphAnchorTip.png
        };
        Q_ENUM(VectorAnchorPoint)

        /** \brief indicates how the drawn vector's length is calculated from the data
         *
         *  \see documentation of m_vectorLengthMode for details
         */
        enum VectorLengthMode {
            AutoscaleLength,  //!< \brief this is the default: vector lengths are autoscaled, so they don't overlap (in first approximation)  \image html JKQTPVectorFieldGraphAnchorBottom.png
            LengthFromData,   //!< \brief the vector's length is determined by the data directly  \image html JKQTPVectorFieldGraphAnchorMid.png
            IgnoreLength      //!< \brief all vectors have the same length \image html JKQTPVectorFieldGraphAnchorTip.png
        };
        Q_ENUM(VectorLengthMode)

        /** \brief describes how the line width scales with the vector properties (or not) */
        enum VectorLineWidthMode {
            DefaultVectorLineWidth, //!< \brief line width is equal to JKQTPGraphLineStyleMixin::getLineWidth() for all vectors \image html JKQTPVectorFieldGraphIgnoreLength.png
            AutoscaleLineWidthFromLength,     //!< \brief line width is determined from the vector length. The maximum line width is given by JKQTPGraphLineStyleMixin::getLineWidth() and the minim line width by getMinLineWidth() \image html JKQTPVectorFieldGraphAutoscaleLengthAutoscaleLineWidthFromLength.png


        };
        Q_ENUM(VectorLineWidthMode)

        /** \brief class constructor */
        explicit JKQTPVectorFieldGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPVectorFieldGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /** \copydoc m_vectorLengthMode */
        VectorLengthMode getVectorLengthMode() const;
        /** \copydoc m_vectorLengthMode */
        void setVectorLengthMode(VectorLengthMode newMode);

        /** \copydoc m_autoscaleLengthFactor */
        double getAutoscaleLengthFactor() const;
        /** \copydoc m_autoscaleLengthFactor */
        void setAutoscaleLengthFactor(double newAutoscaleLengthFactor);

        /** \copydoc m_lengthScaleFactor */
        double getLengthScaleFactor() const;
        /** \copydoc m_lengthScaleFactor */
        void setLengthScaleFactor(double newLengthScaleFactor);


        /** \copydoc m_anchorPoint */
        VectorAnchorPoint getAnchorPoint() const;
        /** \copydoc m_anchorPoint */
        void setAnchorPoint(VectorAnchorPoint newAnchorPoint);


        /** \copydoc m_vectorLineWidthMode */
        void setVectorLineWidthMode(VectorLineWidthMode m);
        /** \copydoc m_vectorLineWidthMode */
        VectorLineWidthMode getVectorLineWidthMode() const;
        /** \copydoc m_minLineWidth */
        void setMinLineWidth(double lw);
        /** \copydoc m_minLineWidth */
        double getMinLineWIdth() const;

        Q_PROPERTY(VectorLengthMode vectorLengthMode READ getVectorLengthMode WRITE setVectorLengthMode )
        Q_PROPERTY(bool autoscaleLengthFactor READ getAutoscaleLengthFactor WRITE setAutoscaleLengthFactor )
        Q_PROPERTY(double lengthScaleFactor READ getLengthScaleFactor WRITE setLengthScaleFactor )
        Q_PROPERTY(VectorAnchorPoint anchorPoint READ getAnchorPoint WRITE setAnchorPoint )
        Q_PROPERTY(double minLineWidth READ getMinLineWIdth WRITE setMinLineWidth )
        Q_PROPERTY(VectorLineWidthMode vectorLineWidthMode READ getVectorLineWidthMode WRITE setVectorLineWidthMode )
    protected:
    private:
        /** \brief indicates how the length of the drawn vectors are determined from the data
         *
         *  Several modes are possible:
         *    - If \c == LengthFromData the vector is drawn from \c (x,y) to \c (x+dx,y+dy)*m_lengthScaleFactor.
         *      \image html JKQTPVectorFieldGraphAutoscaleLength.png
         *    - If \c == AutoscaleLength the vector is drawn to \c (x+dx,y+dy)*autoscale*m_autoscaleLengthFactor .
         *      The autoscaled length is calculated by a simple algorithm that uses the 90% quantile of vector length in the data \c q90VectorLength :
         *        \c autoscale=plotwidth/VectorPerWidth/q90VectorLength .
         *      \image html JKQTPVectorFieldGraphLengthFromData.png
         *    - If \c == IgnoreLength all vectors are drawn with the same length, which is determined from \c autoscale*m_autoscaleLengthFactor
         *      where \c autoscale is defined as above.
         *      \image html JKQTPVectorFieldGraphIgnoreLength.png
         *  .
         *
         *  \see VectorLengthMode, setVectorLengthMode(), getVectorLengthMode(), m_lengthScaleFactor, m_autoscaleLengthFactor
         */
        VectorLengthMode m_vectorLengthMode;
        /** \brief a scaling factor that can be used to modify the result of the autoscaling algorithm (m_vectorLengthMode \c ==AutoscaleLength)
         *
         *  The vector length is further scaled by this value.
         *  \see m_vectorLengthMode, setAutoscaleFactor(), getAutoscaleFactor()
         */
        double m_autoscaleLengthFactor;
        /** \brief if m_vectorLengthMode \c ==false, this is the scale-factor used to calculate the vector length
         *
         *  \see setLengthScaleFactor(), getLengthScaleFactor(), m_vectorLengthMode
         */
        double m_lengthScaleFactor;
        /** \brief defines where the vector is anchored
         *
         *  \see VectorAnchorPoint, setAnchorPoint(), getAnchorPoint()
         */
        VectorAnchorPoint m_anchorPoint;
        /** \brief determines how the line width of the vectors is derived.
         *
         *  \note the available options are described with VectorLineWidthMode
         *
         *  \see setVectorLineWidthMode(), getVectorLineWidthMode(), VectorLineWidthMode, m_minLineWidth
         */
        VectorLineWidthMode m_vectorLineWidthMode;
        /** \brief minimum line-width in pt, used for some modes of m_vectorLineWidthMode
         *
         *  \see setMinLineWidth(), getMinLineWidth(), m_vectorLineWidthMode
         */
        double m_minLineWidth;

};




#endif // jkqtpvectorfield_H
