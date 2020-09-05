/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpgeometrytools.h"
#include <functional>

#ifndef jkqtpevaluatedfunctionbase_H
#define jkqtpevaluatedfunctionbase_H




/*! \brief Base class for graph classes that evaluate a mathematical function (e.g. defined as a C-fucntion), 
           using an adaptive plotting algorithm from JKQTPAdaptiveFunctionGraphEvaluator
    \ingroup jkqtplotter_functiongraphs

    This class uses the intelligent plotting algorithm for functions, implemented in JKQTPAdaptiveFunctionGraphEvaluator.
    It starts by sampling the function at minSamples positions. Then each function interval is bisected recursively if
    necessary. To do so the function is evaluated at the mid point and the slopes \f$ \alpha_{\mbox{left}} \f$
    and \f$ \alpha_{\mbox{right}} \f$ of the two linear segments are compared. the midpoint is added
    to the graph if \f[ \left|\alpha_{\mbox{right}}-\alpha_{\mbox{left}}\right|>\mbox{slopeTolerance} \f]
    In addition all sampling points except minimum and maximum are beeing shifted by a random fraction their
    distance to the other points. This helps to prevent beats when sampling periodic functions.

    Finally the obtained data is cleaned up to reduce the amount of points, by deleting a point, when it leads to an
    angle between consecutive line-segments of less than dataCleanupMaxAllowedAngleDegree.


    \see JKQTPAdaptiveFunctionGraphEvaluator, JKQTPXFunctionLineGraph, JKQTPYFunctionLineGraph, JKQTPXYFunctionLineGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPEvaluatedFunctionGraphBase: public JKQTPGraph {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPEvaluatedFunctionGraphBase(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPEvaluatedFunctionGraphBase(JKQTPlotter* parent);
        
        /** \brief class destructor */
        virtual ~JKQTPEvaluatedFunctionGraphBase() ;


        /*! \copydoc minSamples */ 
        unsigned int getMinSamples() const;
        /*! \copydoc maxRefinementDegree */ 
        unsigned int getMaxRefinementDegree() const;
        /*! \copydoc slopeTolerance */ 
        double getSlopeTolerance() const;
        /*! \copydoc minPixelPerSample */ 
        double getMinPixelPerSample() const;
        /*! \copydoc dataCleanupMaxAllowedAngleDegree */
        double getDataCleanupMaxAllowedAngleDegree() const;
        /*! \copydoc displaySamplePoints */ 
        bool getDisplaySamplePoints() const;
	public slots:
        /*! \copydoc minSamples */
        void setMinSamples(const unsigned int & __value);
        /*! \copydoc maxRefinementDegree */
        void setMaxRefinementDegree(const unsigned int & __value);
        /*! \copydoc slopeTolerance */
        void setSlopeTolerance(double __value);
        /*! \copydoc minPixelPerSample */
        void setMinPixelPerSample(double __value);
        /*! \copydoc dataCleanupMaxAllowedAngleDegree */
        void setDataCleanupMaxAllowedAngleDegree(double __value);
        /*! \copydoc displaySamplePoints */
        void setDisplaySamplePoints(bool __value);

    protected:


        /** \brief plot data calculated by createPlotData(), i.e. the datapoints \f$ \mbox{transform}\left(x, y=f(x, \vec{p})\right) \f$ to be plotted */
        QVector<QPointF> data;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true) =0;
        
        /** \brief the minimum number of points to evaluate the function at */
        unsigned int minSamples;
        /** \brief the maximum number of recursive refinement steps
         *
         * each step bisects the interval \f$ [a, b] \f$ into two halfes. So the maximum number
         * of points plotted at all are thus:
         *  \f[ \mbox{minSamples} \cdot 2^{\mbox{maxRefinementDegree}} \f]
         */
        unsigned int maxRefinementDegree;
        /** \brief the tolerance for the difference of two subsequent slopes */
        double slopeTolerance;
        /** \brief create one sample at least every \a minPixelPerSample pixels */
        double minPixelPerSample;
        /** \brief in the clean-up step of plot-data creation, a point is removed from the data, if
         *         it caused its neighboring line-segments to form an angle less than this value, given in degrees. */
        double dataCleanupMaxAllowedAngleDegree;
        /** \brief if true [default: off] display the points where the function has been sampled */
        bool displaySamplePoints;
        
        /** \brief draw all the sample points in data as small symbols */
        void drawSamplePoints(JKQTPEnhancedPainter &painter, QColor graphColor);
};


#endif // jkqtpevaluatedfunctionbase_H
