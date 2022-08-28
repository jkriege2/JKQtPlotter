/*
    Copyright (c) 2020-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

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


#ifndef jkqtpevaluatedparametriccurve_H
#define jkqtpevaluatedparametriccurve_H


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpgeometrytools.h"
#include "jkqtplotter/graphs/jkqtpevaluatedfunctionbase.h"
#include <functional>


/** \brief Base class for line plots where the data is taken from a user supplied function \f$ [x,y]=f(t) \f$
  *        The function is evaluated on a user-specified range \f$ t \in \left[t_\text{min}, t_\text{max}\right] \f$
  * \ingroup jkqtplotter_functiongraphs
  *
  * This class uses the intelligent plotting algorithm for functions, implemented in JKQTPAdaptiveFunctionGraphEvaluator.
  * It starts by sampling the function at minSamples positions. Then each function interval is bisected recursively if
  * necessary. To do so the function is evaluated at the mid point and the slopes \f$ \alpha_{\mbox{left}} \f$
  * and \f$ \alpha_{\mbox{right}} \f$ of the two linear segments are compared. the midpoint is added
  * to the graph if \f[ \left|\alpha_{\mbox{right}}-\alpha_{\mbox{left}}\right|>\mbox{slopeTolerance} \f]
  * In addition all sampling points except minimum and maximum are beeing shifted by a random fraction their
  * distance to the other points. This helps to prevent beats when sampling periodic functions.
  *
  * Finally the obtained data is cleaned up to reduce the amount of points, by deleting a point, when it leads to an
  * angle between consecutive line-segments of less than dataCleanupMaxAllowedAngleDegree.
  *
  *
  * \see JKQTPXYFunctionLineGraphBase for a concrete implementation with C++-functors as functions
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYFunctionLineGraphBase: public JKQTPEvaluatedFunctionGraphBase, public JKQTPGraphLineStyleMixin {
    Q_OBJECT
public:

    /** \brief class constructor */
    JKQTPXYFunctionLineGraphBase(JKQTBasePlotter* parent=nullptr);

    /** \brief class constructor */
    JKQTPXYFunctionLineGraphBase(JKQTPlotter* parent);


    /** \brief class constructor */
    JKQTPXYFunctionLineGraphBase(double tmin_, double tmax_, JKQTBasePlotter* parent);

    /** \brief class constructor */
    JKQTPXYFunctionLineGraphBase(double tmin_, double tmax_, JKQTPlotter* parent);

    /** \brief class destructor */
    virtual ~JKQTPXYFunctionLineGraphBase() override;

    /** \brief plots the graph to the plotter object specified as parent */
    virtual void draw(JKQTPEnhancedPainter& painter) override;
    /** \brief plots a key marker inside the specified rectangle \a rect */
    virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
    /** \brief returns the color to be used for the key label */
    virtual QColor getKeyLabelColor() const override;


    /** \brief returns the t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ */
    QPair<double,double> getTRange() const;
    /** \copydoc tmin */
    double getTMin() const;
    /** \copydoc tmax */
    double getTMax() const;

public slots:
    /** \copydoc tmin */
    void setTMin(double val);
    /** \copydoc tmax */
    void setTMax(double val);
    /** \brief set the t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ */
    void setTRange(double tmin_, double tmax_);
    /** \brief set the t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ */
    void setTRange(const QPair<double,double>& range);
protected:

    /** \brief lower bound of t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ , i.e. \f$ t_\text{min} \f$ , default is 0
         *
         *  \see getTMin(), getTMax(), setTMin(), setTMax(), setTRange(), getTRange() */
    double tmin;
    /** \brief upper bound of t-value range for \f$ [x,y]=f(t), t \in \left[t_\text{min}, t_\text{max}\right] \f$ , i.e. \f$ t_\text{min} \f$ , default is 1
         *
         *  \see getTMin(), getTMax(), setTMin(), setTMax(), setTRange(), getTRange() */
    double tmax;

};



/** \brief type of functions that may be plotted by JKQTPXYFunctionLineGraph
 *  \ingroup jkqtplotter_functiongraphs
 *
 *  This is the type of functions \f$ [x,y]=f(t, \vec{p}) \f$ that may be plottet by JKQTPXYFunctionLineGraph.
 *  It is possible to supply parameters \f$ \vec{p} \f$ to the function that
 *  influence its result. Parameters are given as a pointer to some memory location. The function has to
 *  know on its own how to interpret these.
*/
typedef std::function<QPointF(double, const QVector<double>)> jkqtpParametricCurveFunctionType;

/** \brief simplified type of functions (without parameters) that may be plotted by JKQTPXYFunctionLineGraph
 *  \ingroup jkqtplotter_functiongraphs
 *
 *  This is the type of functions \f$ [x,y]=f(t) \f$ that may be plottet by JKQTPXYFunctionLineGraph
 *  and JKQTPYFunctionLineGraph.
*/
typedef std::function<QPointF(double)> jkqtpSimpleParametricCurveFunctionType;


/** \brief This implements line plots where the data is taken from a user supplied function \f$ [x,y]=f(t) \f$
  *        The function is evaluated on a user-specified range \f$ t \in \left[t_\text{min}, t_\text{max}\right] \f$
  * \ingroup jkqtplotter_functiongraphs
  *
  * \see JKQTPXYFunctionLineGraphBase for details on the used plotting algorithm
  *
  * The following image shows a Lissajou's fugure drawn with this function
  *
  * \image html JKQTPXYFunctionLineGraph.png
  *
  * The source code for this example is:
  * \code
  *     JKQTPXYFunctionLineGraph* func1=new JKQTPXYFunctionLineGraph(plot);
  *     // here we define the C++-functor for [x,y]=f(t)
  *     func1->setPlotFunctionFunctor([](double t) -> QPointF {
  *         const double a=5;
  *         const double b=4;
  *         const double delta=JKQTPSTATISTICS_PI/4.0;
  *         return QPointF(sin(a*t+delta), sin(b*t));
  *     });
  *     // and define the range over which to evaluate
  *     func1->setTRange(0, 2.0*JKQTPSTATISTICS_PI);
  * \endcode
  *
  * The source code for the same example, but using functions that also get a parameter vector:
  * \code
  *     JKQTPXYFunctionLineGraph* func1=new JKQTPXYFunctionLineGraph(plot);
  *     // here we define the C++-functor for [x,y]=f(t)
  *     func1->setPlotFunctionFunctor([](double t, const QVector<double>& params) -> QPointF {
  *         return QPointF(3.0*sin(params[0]*t+params[2])+8.0, 3.0*sin(params[1]*t));
  *     });
  *     // now we define the 3 parameters of the function
  *     // parameters are a, b, delta, as in the example above (in that order)
  *     func1->setParamsV(5, 4, JKQTPSTATISTICS_PI/4.0);
  *     // and define the range over which to evaluate
  *     func1->setTRange(0, 2.0*JKQTPSTATISTICS_PI);
  * \endcode
  *
  * \see \ref JKQTPlotterEvalCurves , JKQTPAdaptiveFunctionGraphEvaluator, JKQTPXFunctionLineGraph, JKQTPYFunctionLineGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYFunctionLineGraph: public JKQTPXYFunctionLineGraphBase {
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


        /** \brief sets a functor to be plotted
         *
         * \see plotFunction
         */
        void setPlotFunctionFunctor (jkqtpParametricCurveFunctionType && __value);
        /** \brief sets a functor to be plotted
         *
         * \see plotFunction
         */
        void setPlotFunctionFunctor (const jkqtpParametricCurveFunctionType & __value);
        /** \brief sets a functor to be plotted
         *
         * \see simplePlotFunction
         */
        void setPlotFunctionFunctor (jkqtpSimpleParametricCurveFunctionType && __value);
        /** \brief sets a functor to be plotted
         *
         * \see simplePlotFunction
         */
        void setPlotFunctionFunctor (const jkqtpSimpleParametricCurveFunctionType & __value);
        /** \copydoc plotFunction
         *
         * \see isSimplePlotFunction() */
        jkqtpParametricCurveFunctionType getPlotFunctionFunctor () const;
        /** \copydoc simplePlotFunction
         *
         * \see isSimplePlotFunction() */
        jkqtpSimpleParametricCurveFunctionType getSimplePlotFunction () const;

        /** \brief returns whether the plot function was defined as a jkqtpSimpleParametricCurveFunctionType (\c true ) or
         *         a jkqtpParametricCurveFunctionType (\c false ) */
        bool isSimplePlotFunction() const;


    protected:
        /** \copydoc JKQTPEvaluatedFunctionGraphBase::buildPlotFunctorSpec() */
        virtual PlotFunctorSpec buildPlotFunctorSpec() override;


        /** \brief the function to be plotted */
        jkqtpParametricCurveFunctionType plotFunction;
        /** \brief a simple function to be plotted, simplified form without parameters */
        jkqtpSimpleParametricCurveFunctionType simplePlotFunction;

};

#endif // jkqtpevaluatedparametriccurve_H
