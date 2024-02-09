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

    \note This type of plot is sometimes also refered to as quiver plot (e.g. in Matlab or matplotlib)

    \image html JKQTPVectorFieldGraph.png

    To achieve this, use code like this:
    \code
        // 1. setup a plotter window and get a pointer to the internal datastore (for convenience)
        JKQTPlotter plot;
        JKQTPDatastore* ds=plot.getDatastore();

        // 2. make up some arbitrary data to be used for plotting
        //    this generates a 2D grid of x/y-coordinates and then calculates dx=cos(y) and dy=sin(x)
        const auto columnXY=ds->addLinearGridColumns(NX, 0, 6, NY, -3, 3,"x","y");
        const auto columnDX=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return sin(y); });
        const auto columnDY=ds->addCalculatedColumnFromColumn(columnXY.first, columnXY.second, [](double x,double y) { return cos(x); });

        // 3. create JKQTPVectorFieldGraph to display the data:
        JKQTPVectorFieldGraph* graph1=new JKQTPVectorFieldGraph(&plot);
        graph1->setXYColumns(columnXY);
        graph1->setDxColumn(columnDX);
        graph1->setDyColumn(columnDY);
        graph1->setTitle(QObject::tr("$\\vec{f}(x,y)=\\bigl[\\sin(y), \\cos(x)\\bigr]^\\mathrm{T}$"));

        // 4. add the graphs to the plot, so it is actually displayed
        plot.addGraph(graph1);
    \endcode

    \see \ref JKQTPlotterVectorFieldExample , JKQTPGraphDecoratedLineStyleMixin , JKQTPXYAndVectorGraph

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPVectorFieldGraph: public JKQTPXYAndVectorGraph, public JKQTPGraphDecoratedLineStyleMixin {
        Q_OBJECT
    public:
        enum VectorAnchorPoint {
            AnchorBottom,  //!< \brief this is the default: the vector starts at (x,y)  \image html JKQTPVectorFieldGraphAnchorBottom.png
            AnchorMid,     //!< \brief the vector's mid is at (x,y)  \image html JKQTPVectorFieldGraphAnchorMid.png
            AnchorTip      //!< \brief the vector ends at (x,y) \image html JKQTPVectorFieldGraphAnchorTip.png
        };
        Q_ENUM(VectorAnchorPoint)

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

        /** \copydoc m_autoscaleLength */
        bool getAutoscaleLength() const;
        /** \copydoc m_autoscaleLength */
        void setAutoscaleLength(bool newAutoscaleLength);

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

        Q_PROPERTY(bool autoscaleLength READ getAutoscaleLength WRITE setAutoscaleLength )
        Q_PROPERTY(bool autoscaleLengthFactor READ getAutoscaleLengthFactor WRITE setAutoscaleLengthFactor )
        Q_PROPERTY(double lengthScaleFactor READ getLengthScaleFactor WRITE setLengthScaleFactor )
        Q_PROPERTY(VectorAnchorPoint anchorPoint READ getAnchorPoint WRITE setAnchorPoint )
    protected:
    private:
        /** \brief enables or disables the autoscaling of vector lengths
         *
         *  If disabled (\c false ) the vector is drawn from \c (x,y) to \c (x+dx,y+dy)*m_lengthScaleFactor ,
         *  otherweise to \c (x+dx,y+dy)*autoscale*m_autoscaleLengthFactor .
         *  The autoscaled length is calculated by a siple algorithm that uses the average vector length in the data:
         *      \c autoscale=plotwidth/VectorPerWidth/avgVectorLength .
         *
         *  \see m_autoscaleFactor, m_autoscaleLengthFactor, setAutoscaleLength(), getAutoscaleLength()
         */
        bool m_autoscaleLength;
        /** \brief a scaling factor that can be used to modify the result of the autoscaling algorithm (m_autoscaleLength \c ==true)
         *
         *  The vector length is further scaled by this value.
         *  \see m_autoscaleLength, setAutoscaleFactor(), getAutoscaleFactor()
         */
        double m_autoscaleLengthFactor;
        /** \brief if m_autoscaleLength \c ==false, this is the scale-factor used to calculate the vector length
         *
         *  \see setLengthScaleFactor(), getLengthScaleFactor(), m_autoscaleLength
         */
        double m_lengthScaleFactor;
        /** \brief defines where the vector is anchored
         *
         *  \see VectorAnchorPoint
         */
        VectorAnchorPoint m_anchorPoint;

};




#endif // jkqtpvectorfield_H
