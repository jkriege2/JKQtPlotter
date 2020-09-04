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
#include <functional>

#ifndef jkqtpgraphsevaluatedfunction_H
#define jkqtpgraphsevaluatedfunction_H




/*! \brief type of functions that may be plotted by JKQTPXFunctionLineGraph and JKQTPYFunctionLineGraph
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ y=f(x, \vec{p}) \f$ that may be plottet by JKQTPXFunctionLineGraph
    and JKQTPYFunctionLineGraph. It is possible to supply parameters \f$ \vec{p} \f$ to the function that
    influence its result. Parameters are given as a pointer to some memory location. The function has to
    know on its own how to interpret these.
*/
typedef std::function<double(double, void*)> jkqtpPlotFunctionType;

/*! \brief simplified type of functions (without parameters) that may be plotted by JKQTPXFunctionLineGraph and JKQTPYFunctionLineGraph
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ y=f(x) \f$ that may be plottet by JKQTPXFunctionLineGraph
    and JKQTPYFunctionLineGraph.
*/
typedef std::function<double(double)> jkqtpSimplePlotFunctionType;


/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$
    \ingroup jkqtplotter_functiongraphs

    This class implements an intelligent plotting algorithm for functions. It starts by sampling
    the function at minSamples positions. Then each function interval is bisected recursively if
    necessary. To do so the function is evaluated at the mid point and the slopes \f$ \alpha_{\mbox{left}} \f$
    and \f$ \alpha_{\mbox{right}} \f$ of the two linear segments are compared. the midpoint is added
    to the graph if \f[ \left|\alpha_{\mbox{right}}-\alpha_{\mbox{left}}\right|>\mbox{slopeTolerance} \f]
    In addition all sampling points except minimum and maximum are beeing shifted by a random fraction their
    distance to the other points. This helps to prevent beats when sampling periodic functions.

    The following image shows some example graphs:

    \image html plot_functionplots.png

    \see \ref JKQTPlotterFunctionPlots, JKQTPYFunctionLineGraph, JKQTPXYFunctionLineGraph, jkqtpstatAddPolyFit(), jkqtpstatAddWeightedRegression(), jkqtpstatAddRobustIRLSRegression(), jkqtpstatAddRegression(), jkqtpstatAddLinearWeightedRegression(), jkqtpstatAddRobustIRLSLinearRegression(), jkqtpstatAddLinearRegression()
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph: public JKQTPGraph, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
        Q_OBJECT
    public:

        enum SpecialFunction {
            Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x+p_2x^2+p_3x^3+... \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Line=Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Exponential,  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot\exp(x/p_2) \f$ or \f$ f(x)=p_0\cdot\exp(x/p_1) \f$ (depending on the number of parameters). The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            PowerLaw,  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot x^{p_3} \f$ or \f$ f(x)=p_0\cdot x^{p_1} \f$ or \f$ f(x)= x^{p_0} \f$ (depending on the number of parameters) The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */

            UserFunction,  /*!< \brief no special function but the function is provided by the user */
        };

        /** \brief class constructor */
        JKQTPXFunctionLineGraph(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPXFunctionLineGraph(JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPXFunctionLineGraph(const jkqtpSimplePlotFunctionType & f, const QString& title, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXFunctionLineGraph(const jkqtpSimplePlotFunctionType & f, const QString& title, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType && f, const QString& title, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType && f, const QString& title, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPXFunctionLineGraph(SpecialFunction type, const QVector<double>& params, const QString& title, JKQTBasePlotter* parent);
        /** \brief class constructor */
        JKQTPXFunctionLineGraph(SpecialFunction type, const QVector<double>& params, const QString& title, JKQTPlotter* parent);

        /** \brief class destructor */
        virtual ~JKQTPXFunctionLineGraph() override;

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

        /** \brief clear the data sampled from the function. */
        void clearData();

        /*! \brief set color, fill color and error color at the same time */
        void setColor(QColor c);

        /*! \copydoc drawLine */ 
        void setDrawLine(bool __value);
        /*! \copydoc drawLine */ 
        bool getDrawLine() const;

        /** \brief sets a functor to be plotted
         *
         * \see plotFunction
         */
        virtual void setPlotFunctionFunctor (jkqtpPlotFunctionType && __value);
        /** \brief sets a functor to be plotted
         *
         * \see plotFunction
         */
        virtual void setPlotFunctionFunctor (const jkqtpPlotFunctionType & __value);
        /** \brief sets a functor to be plotted
         *
         * \see simplePlotFunction
         */
        virtual void setPlotFunctionFunctor (jkqtpSimplePlotFunctionType && __value);
        /** \brief sets a functor to be plotted
         *
         * \see simplePlotFunction
         */
        virtual void setPlotFunctionFunctor (const jkqtpSimplePlotFunctionType & __value);
        /*! \copydoc plotFunction */ \
        virtual jkqtpPlotFunctionType getPlotFunctionFunctor () const;
        /*! \copydoc simplePlotFunction */ \
        virtual jkqtpSimplePlotFunctionType getSimplePlotFunction () const;

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
        /** \brief returns the currently set internal parameter vector */
        QVector<double> getInternalErrorParams() const;
        /*! \copydoc minSamples */ 
        void setMinSamples(const unsigned int & __value);
        /*! \copydoc minSamples */ 
        unsigned int getMinSamples() const;
        /*! \copydoc maxRefinementDegree */ 
        void setMaxRefinementDegree(const unsigned int & __value);
        /*! \copydoc maxRefinementDegree */ 
        unsigned int getMaxRefinementDegree() const;
        /*! \copydoc slopeTolerance */ 
        void setSlopeTolerance(double __value);
        /*! \copydoc slopeTolerance */ 
        double getSlopeTolerance() const;
        /*! \copydoc minPixelPerSample */ 
        void setMinPixelPerSample(double __value);
        /*! \copydoc minPixelPerSample */ 
        double getMinPixelPerSample() const;
        /*! \copydoc plotRefinement */ 
        void setPlotRefinement(bool __value);
        /*! \copydoc plotRefinement */ 
        bool getPlotRefinement() const;
        /*! \copydoc displaySamplePoints */ 
        void setDisplaySamplePoints(bool __value);
        /*! \copydoc displaySamplePoints */ 
        bool getDisplaySamplePoints() const;
        /*! \copydoc drawErrorPolygons */ 
        void setDrawErrorPolygons(bool __value);
        /*! \copydoc drawErrorPolygons */ 
        bool getDrawErrorPolygons() const;
        /*! \copydoc drawErrorLines */ 
        void setDrawErrorLines(bool __value);
        /*! \copydoc drawErrorLines */ 
        bool getDrawErrorLines() const;
        /** \brief sets a functor to be used for calculating errors
         *
         * \see errorPlotFunction
         */
        virtual void setErrorPlotFunction (jkqtpPlotFunctionType && __value);
        /** \brief sets a functor to be used for calculating errors
         *
         * \see errorPlotFunction
         */
        virtual void setErrorPlotFunction (const jkqtpPlotFunctionType & __value);
        /*! \copydoc errorPlotFunction */ \
        virtual jkqtpPlotFunctionType getErrorPlotFunction () const;
        /** \brief sets a functor to be used for calculating errors
         *
         * \see errorSimplePlotFunction
         */
        virtual void setErrorPlotFunction (jkqtpSimplePlotFunctionType && __value);
        /** \brief sets a functor to be used for calculating errors
         *
         * \see errorSimplePlotFunction
         */
        virtual void setErrorPlotFunction (const jkqtpSimplePlotFunctionType & __value);
        /*! \copydoc errorSimplePlotFunction */ \
        virtual jkqtpSimplePlotFunctionType getErrorSimplePlotFunction () const;
        /*! \copydoc errorParams */ 
        virtual void setErrorParams(void* __value);
        /*! \copydoc errorParams */ 
        void *getErrorParams() const;
        /** \brief sets the error params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        void setErrorParams(const QVector<double>& errorParams);

        /*! \copydoc parameterColumn */ 
        void setParameterColumn(int __value);
        /*! \copydoc parameterColumn */ 
        int getParameterColumn() const;
        /*! \copydoc parameterColumn */ 
        void setParameterColumn (size_t __value);
        /*! \copydoc errorParameterColumn */ 
        void setErrorParameterColumn(int __value);
        /*! \copydoc errorParameterColumn */ 
        int getErrorParameterColumn() const;
        /*! \copydoc errorParameterColumn */ 
        void setErrorParameterColumn (size_t __value);

        /*! \copydoc errorColor */
        virtual void setErrorLineColor(const QColor & __value);
        /*! \copydoc errorColor */ 
        virtual QColor getErrorLineColor() const;
        /*! \copydoc errorFillColor */ 
        virtual void setErrorFillColor(const QColor & __value);
        /*! \copydoc errorFillColor */ 
        virtual QColor getErrorFillColor() const;
        /*! \copydoc errorFillStyle */ 
        virtual void setErrorFillStyle(Qt::BrushStyle  __value);
        /*! \copydoc errorFillStyle */ 
        virtual Qt::BrushStyle getErrorFillStyle() const;
        /*! \copydoc errorStyle */ 
        virtual void setErrorLineStyle(Qt::PenStyle  __value);
        /*! \copydoc errorStyle */ 
        virtual Qt::PenStyle getErrorLineStyle() const;
        /*! \copydoc errorLineWidth */ 
        virtual void setErrorLineWidth(double __value);
        /*! \copydoc errorLineWidth */ 
        virtual double getErrorLineWidth() const;

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


        /** \brief sets function to the given special function */
        void setSpecialFunction(SpecialFunction function);
        /** \brief returns, which special function is set (or if any is set) */
        SpecialFunction getFunctionType() const;
    protected:


        struct doublePair {
            double x;
            double f;
            doublePair* next;
        };
        /** \brief a linked list holding the datapoints \f$ \left(x, y=f(x, \vec{p})\right) \f$ to be plotted */
        doublePair* data;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true);
        /** \brief ensure that current function parameters for plotFunction (which may stem from different sources, as direct data, a datastore column ...) are stored in iparams and ierrorparams */
        virtual void collectParameters();
        /** \brief refine datapoints on the function graph between two evaluations \a a and \a b */
        void refine(doublePair* a, doublePair* b, unsigned int degree=0);

        /** \brief if set, the values from this datatsore column are used for the parameters \c p1 , \c p2 , \c p3 , ...  of the plot function */
        int parameterColumn;
        /** \brief if set, the values from this datatsore column are used for the parameters \c p1 , \c p2 , \c p3 , ...  of the error plot function */
        int errorParameterColumn;

        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief indicates whether to fill the space between the curve and the x-axis */
        bool fillCurve;
        /** \brief the function to be plotted */
        jkqtpPlotFunctionType plotFunction;
        /** \brief a simple function to be plotted, simplified form without parameters */
        jkqtpSimplePlotFunctionType simplePlotFunction;
        /** \brief indicates whether a special function is set (and if so, which one), or a user-supplied function */
        SpecialFunction functionType;
        /** \brief pointer to the parameters supplied to the plotting funtion */
        void* params;
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
        /** \brief switch on or off [default: on] the plot refinement algorithm */
        bool plotRefinement;
        /** \brief if true [default: off] display the points where the function has been sampled */
        bool displaySamplePoints;
        /** \brief indicates whether an error polygon should be drawn */
        bool drawErrorPolygons;
        /** \brief indicates whether error lines should be drawn */
        bool drawErrorLines;
        /** \brief this function calculates the error at a given position */
        jkqtpPlotFunctionType errorPlotFunction;
        /** \brief this function calculates the error at a given position, simplified form without parameters */
        jkqtpSimplePlotFunctionType errorSimplePlotFunction;
        /** \brief parameters for errorFunction */
        void* errorParams;


        /** \brief color of the error graph */
        QColor errorColor;
        /** \brief color of the error graph fill */
        QColor errorFillColor;
        /** \brief linestyle of the error graph lines */
        Qt::PenStyle errorStyle;
        /** \brief width (pixels) of the error graph */
        double errorLineWidth;
        /** \brief fill style, if the error curve should be filled */
        Qt::BrushStyle errorFillStyle;


        QBrush getErrorBrush(JKQTPEnhancedPainter& painter) const;
        QPen getErrorLinePen(JKQTPEnhancedPainter &painter) const;
        /** \brief internal storage for the current function parameters for plotFunction (which may stem from different sources, as direct data, a datastore column ...) */
        QVector<double> iparams;
        /** \brief internal storage for the current error function parameters for errorPlotFunction (which may stem from different sources, as direct data, a datastore column ...) */
        QVector<double> ierrorparams;
};

/*! \brief This implements line plots where the data is taken from a user supplied function \f$ x=f(y) \f$
    \ingroup jkqtplotter_functiongraphs

    The following image shows some example graphs:

    \image html functionplot_fy.png

    \see \ref JKQTPlotterFunctionPlots , JKQTPXFunctionLineGraph, JKQTPXYFunctionLineGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPYFunctionLineGraph: public JKQTPXFunctionLineGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(const jkqtpSimplePlotFunctionType & f, const QString& title, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(const jkqtpSimplePlotFunctionType & f, const QString& title, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(jkqtpSimplePlotFunctionType && f, const QString& title, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(jkqtpSimplePlotFunctionType && f, const QString& title, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(SpecialFunction type, const QVector<double>& params, const QString& title, JKQTBasePlotter* parent);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(SpecialFunction type, const QVector<double>& params, const QString& title, JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
    protected:

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true) override;

};




#endif // jkqtpgraphsevaluatedfunction_H
