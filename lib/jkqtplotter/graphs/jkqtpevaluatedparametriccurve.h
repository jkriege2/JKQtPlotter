/*
    Copyright (c) 2020-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include "jkqtplotter/graphs/jkqtpevaluatedfunctionbase.h"
#include <functional>

#ifndef jkqtpevaluatedparametriccurve_H
#define jkqtpevaluatedparametriccurve_H




/*! \brief type of functions that may be plotted by JKQTPXYFunctionLineGraph
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ [x,y]=f(t, \vec{p}) \f$ that may be plottet by JKQTPXYFunctionLineGraph.
    It is possible to supply parameters \f$ \vec{p} \f$ to the function that
    influence its result. Parameters are given as a pointer to some memory location. The function has to
    know on its own how to interpret these.
*/
typedef std::function<QPointF(double, void*)> jkqtpParametricCurveFunctionType;

/*! \brief simplified type of functions (without parameters) that may be plotted by JKQTPXYFunctionLineGraph
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ [x,y]=f(t) \f$ that may be plottet by JKQTPXYFunctionLineGraph
    and JKQTPYFunctionLineGraph.
*/
typedef std::function<QPointF(double)> jkqtpSimpleParametricCurveFunctionType;


/*! \brief This implements line plots where the data is taken from a user supplied function \f$ [x,y]=f(t) \f$
           The function is evaluated on a user-specified range \f$ t \in \left[t_\text{min}, t_\text{max}\right] \f$
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

    the following image shows a Lissajou's fugure drawn with this function

    \image html plot_evalcurve.png

    The source code for this example is:
    \code
        JKQTPXYFunctionLineGraph* func1=new JKQTPXYFunctionLineGraph(plot);
        func1->setPlotFunctionFunctor([](double t) -> QPointF {
            const double a=5;
            const double b=4;
            const double delta=JKQTPSTATISTICS_PI/4.0;
            return QPointF(sin(a*t+delta), sin(b*t));
        });
        func1->setTRange(0, 2.0*JKQTPSTATISTICS_PI);
        func1->setTitle("C++-inline function $[ sin(5{\\cdot}t+\\pi/4), sin(4{\\cdot}t) ]$");
    \endcode

    \see \ref JKQTPlotterEvalCurves , JKQTPAdaptiveFunctionGraphEvaluator, JKQTPXFunctionLineGraph, JKQTPYFunctionLineGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYFunctionLineGraph: public JKQTPEvaluatedFunctionGraphBase, public JKQTPGraphLineStyleMixin {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(const jkqtpSimpleParametricCurveFunctionType & f, const QString& title, double tmin_=0, double tmax_=1, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(const jkqtpSimpleParametricCurveFunctionType & f, const QString& title, double tmin_, double tmax_, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(jkqtpSimpleParametricCurveFunctionType && f, const QString& title, double tmin_=0, double tmax_=1, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(jkqtpSimpleParametricCurveFunctionType && f, const QString& title, double tmin_, double tmax_, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(const jkqtpParametricCurveFunctionType & f, const QString& title, double tmin_=0, double tmax_=1, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(const jkqtpParametricCurveFunctionType & f, const QString& title, double tmin_, double tmax_, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(jkqtpParametricCurveFunctionType && f, const QString& title, double tmin_=0, double tmax_=1, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXYFunctionLineGraph(jkqtpParametricCurveFunctionType && f, const QString& title, double tmin_, double tmax_, JKQTPlotter* parent);

        /** \brief class destructor */
        virtual ~JKQTPXYFunctionLineGraph() override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;

        /** \brief get the maximum and minimum x-value of the graph
         *
         * This functions returns 0 for both parameters, so that the plotter uses the predefined
         * min and max values.
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief sets a functor to be plotted
         *
         * \see plotFunction
         */
        virtual void setPlotFunctionFunctor (jkqtpParametricCurveFunctionType && __value);
        /** \brief sets a functor to be plotted
         *
         * \see plotFunction
         */
        virtual void setPlotFunctionFunctor (const jkqtpParametricCurveFunctionType & __value);
        /** \brief sets a functor to be plotted
         *
         * \see simplePlotFunction
         */
        virtual void setPlotFunctionFunctor (jkqtpSimpleParametricCurveFunctionType && __value);
        /** \brief sets a functor to be plotted
         *
         * \see simplePlotFunction
         */
        virtual void setPlotFunctionFunctor (const jkqtpSimpleParametricCurveFunctionType & __value);
        /*! \copydoc plotFunction */ \
        virtual jkqtpParametricCurveFunctionType getPlotFunctionFunctor () const;
        /*! \copydoc simplePlotFunction */ \
        virtual jkqtpSimpleParametricCurveFunctionType getSimplePlotFunction () const;

        /*! \copydoc params */ 
        virtual void setParams(void* __value);
        /*! \copydoc params */ 
        void* getParams() const;
        /** \brief sets the params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        virtual void setParams(const QVector<double>& params);
        /** \brief sets the params from a copy of the given array of length \a N */
        void setCopiedParams(const double* params, int N);
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

        /** \brief returns the currently set internal parameter vector */
        QVector<double> getInternalParams() const;



        /*! \copydoc parameterColumn */ 
        void setParameterColumn(int __value);
        /*! \copydoc parameterColumn */ 
        int getParameterColumn() const;
        /*! \copydoc parameterColumn */ 
        void setParameterColumn (size_t __value);


        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


        /*! \copydoc tmin */
        double getTMin() const;
        /*! \copydoc tmax */
        double getTMax() const;
        /*! \copydoc tmin */
        void setTMin(double val);
        /*! \copydoc tmax */
        void setTMax(double val);
        /*! \brief returns the t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ */
        QPair<double,double> getTRange() const;
        /*! \brief set the t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ */
        void setTRange(double tmin_, double tmax_);
        /*! \brief set the t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ */
        void setTRange(const QPair<double,double>& range);
    protected:
        /** \brief lower bound of t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ , i.e. \f$ t_\text{min} \f$ , default is 0 */
        double tmin;
        /** \brief upper bound of t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ , i.e. \f$ t_\text{min} \f$ , default is 1 */
        double tmax;
        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true) override;
        /** \brief ensure that current function parameters for plotFunction (which may stem from different sources, as direct data, a datastore column ...) are stored in iparams */
        virtual void collectParameters() ;

        /** \brief if set, the values from this datatsore column are used for the parameters \c p1 , \c p2 , \c p3 , ...  of the plot function */
        int parameterColumn;

        /** \brief the function to be plotted */
        jkqtpParametricCurveFunctionType plotFunction;
        /** \brief a simple function to be plotted, simplified form without parameters */
        jkqtpSimpleParametricCurveFunctionType simplePlotFunction;
        /** \brief pointer to the parameters supplied to the plotting funtion */
        void* params;

        /** \brief internal storage for the current function parameters for plotFunction (which may stem from different sources, as direct data, a datastore column ...) */
        QVector<double> iparams;
};

#endif // jkqtpevaluatedparametriccurve_H
