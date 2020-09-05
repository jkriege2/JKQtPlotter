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




/** \brief Base class for graph classes that evaluate a mathematical function (e.g. defined as a C-fucntion),
 *         using an adaptive plotting algorithm from JKQTPAdaptiveFunctionGraphEvaluator
 *  \ingroup jkqtplotter_functiongraphs
 *
 *  This class uses the intelligent plotting algorithm for functions, implemented in JKQTPAdaptiveFunctionGraphEvaluator.
 *  It starts by sampling the function at minSamples positions. Then each function interval is bisected recursively if
 *  necessary. To do so the function is evaluated at the mid point and the slopes \f$ \alpha_{\mbox{left}} \f$
 *  and \f$ \alpha_{\mbox{right}} \f$ of the two linear segments are compared. the midpoint is added
 *  to the graph if \f[ \left|\alpha_{\mbox{right}}-\alpha_{\mbox{left}}\right|>\mbox{slopeTolerance} \f]
 *  In addition all sampling points except minimum and maximum are beeing shifted by a random fraction their
 *  distance to the other points. This helps to prevent beats when sampling periodic functions.
 *
 *  Finally the obtained data is cleaned up to reduce the amount of points, by deleting a point, when it leads to an
 *  angle between consecutive line-segments of less than dataCleanupMaxAllowedAngleDegree.
 *
 *
 *  \see JKQTPAdaptiveFunctionGraphEvaluator, JKQTPXFunctionLineGraph, JKQTPYFunctionLineGraph, JKQTPXYFunctionLineGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPEvaluatedFunctionGraphBase: public JKQTPGraph {
        Q_OBJECT
    public:

        /** \brief class constructor */
        explicit JKQTPEvaluatedFunctionGraphBase(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        explicit JKQTPEvaluatedFunctionGraphBase(JKQTPlotter* parent);
        
        /** \brief class destructor */
        virtual ~JKQTPEvaluatedFunctionGraphBase() ;

        /** \brief get the maximum and minimum x-value of the graph
         *
         * This functions returns 0 for both parameters, so that the plotter uses the predefined
         * min and max values.
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \copydoc minSamples */
        unsigned int getMinSamples() const;
        /** \copydoc maxRefinementDegree */
        unsigned int getMaxRefinementDegree() const;
        /** \copydoc slopeTolerance */
        double getSlopeTolerance() const;
        /** \copydoc minPixelPerSample */
        double getMinPixelPerSample() const;
        /** \copydoc dataCleanupMaxAllowedAngleDegree */
        double getDataCleanupMaxAllowedAngleDegree() const;
        /** \copydoc displaySamplePoints */
        bool getDisplaySamplePoints() const;
	public slots:
        /** \copydoc minSamples */
        void setMinSamples(const unsigned int & __value);
        /** \copydoc maxRefinementDegree */
        void setMaxRefinementDegree(const unsigned int & __value);
        /** \copydoc slopeTolerance */
        void setSlopeTolerance(double __value);
        /** \copydoc minPixelPerSample */
        void setMinPixelPerSample(double __value);
        /** \copydoc dataCleanupMaxAllowedAngleDegree */
        void setDataCleanupMaxAllowedAngleDegree(double __value);
        /** \copydoc displaySamplePoints */
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


/** \brief extends JKQTPEvaluatedFunctionGraphBase with a set of functions that support function parameters
 *  \ingroup jkqtplotter_functiongraphs
 *
 *  \see JKQTPEvaluatedFunctionGraphBase
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPEvaluatedFunctionWithParamsGraphBase: public JKQTPEvaluatedFunctionGraphBase {
    Q_OBJECT
public:
    /** \brief class constructor */
    explicit JKQTPEvaluatedFunctionWithParamsGraphBase(JKQTBasePlotter* parent=nullptr);
    /** \brief class constructor */
    explicit JKQTPEvaluatedFunctionWithParamsGraphBase(JKQTPlotter* parent);
    /** \brief class destructor */
    virtual ~JKQTPEvaluatedFunctionWithParamsGraphBase();


    /** \brief sets the params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
    virtual void setParams(const QVector<double>& params);
    /** \brief sets the params from a copy of the given array of length \a N */
    void setCopiedParams(const double* params, int N);

    /** \brief returns the currently set internal parameter vector */
    QVector<double> getInternalParams() const;
    /** \copydoc parameterColumn */
    int getParameterColumn() const;

    /** \copydoc JKQTPGraph::usesColumn() */
    virtual bool usesColumn(int c) const override;

public slots:
    /** \brief set an internal parameter vector as function parameters, initialized with {p1} */
    void setParamsV(double p1);
    /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2} */
    void setParamsV(double p1, double p2);
    /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3} */
    void setParamsV(double p1, double p2, double p3);
    /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3,p4} */
    void setParamsV(double p1, double p2, double p3, double p4);
    /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3,p4,p5} */
    void setParamsV(double p1, double p2, double p3, double p4, double p5);

    /** \copydoc parameterColumn */
    void setParameterColumn(int __value);
    /** \copydoc parameterColumn */
    void setParameterColumn (size_t __value);
protected:
    /** \brief ensure that current function parameters for a plot function (which may stem from different sources, as direct data, a datastore column ...) are stored in iparams */
    virtual void collectParameters();

    /** \brief if set, the values from this datatsore column are used for the parameters \c p1 , \c p2 , \c p3 , ...  of the plot function */
    int parameterColumn;

    /** \brief internal storage for the current function parameters for plotFunction (which may stem from different sources, as direct data, a datastore column ...) */
    QVector<double> iparams;
};


#endif // jkqtpevaluatedfunctionbase_H
