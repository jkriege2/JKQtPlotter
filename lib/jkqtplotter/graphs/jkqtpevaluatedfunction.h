/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#ifndef jkqtpgraphsevaluatedfunction_H
#define jkqtpgraphsevaluatedfunction_H



#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpgeometrytools.h"
#include "jkqtplotter/graphs/jkqtpevaluatedfunctionbase.h"
#include <functional>


/** \brief This class extends JKQTPEvaluatedFunctionWithErrorsGraphBase with functions to draw the graphs and
 *         set the drawing style
 *  \ingroup jkqtplotter_functiongraphs
 *
 *  \note Since this class is meant as a base for both f(x)- and f(y)-functions, it cannot
 *        implememt JKQTPGraph::draw(). Therefore it provides two implementations drawXGraph()
 *        and drawYGraph() and the user has to decide in the concrete graph class, which one to call
 *        (or whether to do the drawing completely different).
 *
 *  \see e.g. JKQTPXFunctionLineGraph for a concrete implementation
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase: public JKQTPEvaluatedFunctionWithErrorsGraphBase, public JKQTPGraphLineAndFillStyleMixin {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase(JKQTPlotter* parent);

        /** \brief class destructor */
        virtual ~JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase() override;

        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;



        /** \copydoc drawErrorPolygons */
        bool getDrawErrorPolygons() const;
        /** \copydoc drawErrorLines */
        bool getDrawErrorLines() const;
        /** \copydoc errorColor */
        virtual QColor getErrorLineColor() const;
        /** \copydoc errorFillColor */
        virtual QColor getErrorFillColor() const;
        /** \copydoc errorFillStyle */
        virtual Qt::BrushStyle getErrorFillStyle() const;
        /** \copydoc errorStyle */
        virtual Qt::PenStyle getErrorLineStyle() const;
        /** \copydoc errorLineWidth */
        virtual double getErrorLineWidth() const;





    public Q_SLOTS:
        /** \brief set color, fill color and error color at the same time  */
        void setColor(QColor c);


        /** \copydoc drawErrorPolygons */
        void setDrawErrorPolygons(bool __value);
        /** \copydoc drawErrorLines */
        void setDrawErrorLines(bool __value);
        /** \copydoc errorColor */
        virtual void setErrorLineColor(const QColor & __value);
        /** \copydoc errorFillColor */
        virtual void setErrorFillColor(const QColor & __value);
        /** \copydoc errorFillStyle */
        virtual void setErrorFillStyle(Qt::BrushStyle  __value);
        /** \copydoc errorStyle */
        virtual void setErrorLineStyle(Qt::PenStyle  __value);
        /** \copydoc errorLineWidth */
        virtual void setErrorLineWidth(double __value);

    protected:
        /** \brief plots the graph to the plotter object specified as parent */
        void drawXGraph(JKQTPEnhancedPainter& painter);
        /** \brief plots the graph to the plotter object specified as parent */
        void drawYGraph(JKQTPEnhancedPainter& painter);




        /** \brief indicates whether an error polygon should be drawn */
        bool drawErrorPolygons;
        /** \brief indicates whether error lines should be drawn */
        bool drawErrorLines;



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

};



/*! \brief type of functions that may be plotted by JKQTPXFunctionLineGraph and JKQTPYFunctionLineGraph
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ y=f(x, \vec{p}) \f$ that may be plottet by JKQTPXFunctionLineGraph
    and JKQTPYFunctionLineGraph. It is possible to supply parameters \f$ \vec{p} \f$ to the function that
    influence its result. Parameters are given as a pointer to some memory location. The function has to
    know on its own how to interpret these.
*/
typedef std::function<double(double, const QVector<double>&)> jkqtpPlotFunctionType;

/*! \brief simplified type of functions (without parameters) that may be plotted by JKQTPXFunctionLineGraph and JKQTPYFunctionLineGraph
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ y=f(x) \f$ that may be plottet by JKQTPXFunctionLineGraph
    and JKQTPYFunctionLineGraph.
*/
typedef std::function<double(double)> jkqtpSimplePlotFunctionType;



/** \brief extends JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase with the capabilities to define functions from C++-functors
 *         of type jkqtpSimplePlotFunctionType or jkqtpPlotFunctionType
 *  \ingroup jkqtplotter_functiongraphs
 *
 *  \see JKQTPXFunctionLineGraph and JKQTPYFunctionLineGraph for a concrete implementation
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPFunctorLineGraphBase: public JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase {
        Q_OBJECT
    public:
        enum SpecialFunction {
            Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x+p_2x^2+p_3x^3+... \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Line=Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Exponential,  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot\exp(x/p_2) \f$ or \f$ f(x)=p_0\cdot\exp(x/p_1) \f$ (depending on the number of parameters). The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            PowerLaw,  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot x^{p_3} \f$ or \f$ f(x)=p_0\cdot x^{p_1} \f$ or \f$ f(x)= x^{p_0} \f$ (depending on the number of parameters) The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
        };

        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(const jkqtpSimplePlotFunctionType & f, const QString& title, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(const jkqtpSimplePlotFunctionType & f, const QString& title, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(jkqtpSimplePlotFunctionType && f, const QString& title, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(jkqtpSimplePlotFunctionType && f, const QString& title, JKQTPlotter* parent);
        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(SpecialFunction type, const QVector<double>& params, const QString& title, JKQTBasePlotter* parent);
        /** \brief class constructor */
        JKQTPFunctorLineGraphBase(SpecialFunction type, const QVector<double>& params, const QString& title, JKQTPlotter* parent);

        /** \brief class destructor */
        virtual ~JKQTPFunctorLineGraphBase() override;

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
        /*! \copydoc plotFunction
         *
         * \see isSimplePlotFunction() */ \
        virtual jkqtpPlotFunctionType getPlotFunctionFunctor () const;
        /*! \copydoc simplePlotFunction
         *
         * \see isSimplePlotFunction() */ \
        virtual jkqtpSimplePlotFunctionType getSimplePlotFunction () const;

        /** \brief returns whether the plot function was defined as a jkqtpSimpleParametricCurveFunctionType (\c true ) or
         *         a jkqtpParametricCurveFunctionType (\c false ) */
        bool isSimplePlotFunction() const;


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
        /** \copydoc errorPlotFunction */ \
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
        /** \copydoc errorSimplePlotFunction */ \
        virtual jkqtpSimplePlotFunctionType getErrorSimplePlotFunction () const;



        /** \brief sets function to the given special function */
        void setSpecialFunction(SpecialFunction function);
    protected:
        /** \brief the function to be plotted */
        jkqtpPlotFunctionType plotFunction;
        /** \brief a simple function to be plotted, simplified form without parameters */
        jkqtpSimplePlotFunctionType simplePlotFunction;


        /** \brief this function calculates the error at a given position */
        jkqtpPlotFunctionType errorPlotFunction;
        /** \brief this function calculates the error at a given position, simplified form without parameters */
        jkqtpSimplePlotFunctionType errorSimplePlotFunction;
};


/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$
    \ingroup jkqtplotter_functiongraphs

    This class uses the intelligent plotting algorithm for functions, implemented in JKQTPAdaptiveFunctionGraphEvaluator.

    The following image shows some example graphs:

    \image html plot_functionplots.png

    \note Auto-scaling of the plot is deactivated (by overwriting getXMinMax() and getYMinMax() to return \c false ), as it depends on the axes (hen-egg-problem!)
          Set the plot-ranges by hand!!!

    \see \ref JKQTPlotterFunctionPlots, JKQTPAdaptiveFunctionGraphEvaluator, JKQTPYFunctionLineGraph, JKQTPXYFunctionLineGraph, jkqtpstatAddPolyFit(), jkqtpstatAddWeightedRegression(), jkqtpstatAddRobustIRLSRegression(), jkqtpstatAddRegression(), jkqtpstatAddLinearWeightedRegression(), jkqtpstatAddRobustIRLSLinearRegression(), jkqtpstatAddLinearRegression()
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph: public JKQTPFunctorLineGraphBase {
        Q_OBJECT
    public:


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

    protected:

        /** \copydoc JKQTPEvaluatedFunctionGraphBase::buildPlotFunctorSpec() */
        virtual PlotFunctorSpec buildPlotFunctorSpec() override;

        /** \copydoc JKQTPEvaluatedFunctionWithErrorsGraphBase::buildPlotFunctorSpec() */
        virtual std::function<QPointF(double)> buildErrorFunctorSpec() override;

        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


};

/*! \brief This implements line plots where the data is taken from a user supplied function \f$ x=f(y) \f$
    \ingroup jkqtplotter_functiongraphs

    The following image shows some example graphs:

    \image html functionplot_fy.png

    \note Auto-scaling of the plot is deactivated (by overwriting getXMinMax() and getYMinMax() to return \c false ), as it depends on the axes (hen-egg-problem!)
          Set the plot-ranges by hand!!!

    \see \ref JKQTPlotterFunctionPlots , JKQTPXFunctionLineGraph, JKQTPXYFunctionLineGraph
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPYFunctionLineGraph: public JKQTPFunctorLineGraphBase {
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

        /** \copydoc JKQTPEvaluatedFunctionGraphBase::buildPlotFunctorSpec() */
        virtual PlotFunctorSpec buildPlotFunctorSpec() override;

        /** \copydoc JKQTPEvaluatedFunctionWithErrorsGraphBase::buildPlotFunctorSpec() */
        virtual std::function<QPointF(double)> buildErrorFunctorSpec() override;


        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


};




#endif // jkqtpgraphsevaluatedfunction_H
