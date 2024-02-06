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


#ifndef jkqtpgraphsparsedfunction_H
#define jkqtpgraphsparsedfunction_H

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtmath/jkqtpmathparser.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/graphs/jkqtpevaluatedfunction.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;


/** \brief extends JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase with the capabilities to define functions from strings
 *         that are parsed by JKQTPMathParser
 *  \ingroup jkqtplotter_functiongraphs
 *
 *  \see JKQTPXParsedFunctionLineGraph and JKQTPYParsedFunctionLineGraph for a concrete implementation, see also JKQTPMathParser
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPParsedFunctionLineGraphBase: public JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase {
    Q_OBJECT
public:


    /** \brief class constructor */
    JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName, JKQTBasePlotter* parent=nullptr);
    /** \brief class constructor */
    JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName, JKQTPlotter* parent);

    /** \brief class constructor */
    JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName, const QString& function, JKQTBasePlotter* parent=nullptr);
    /** \brief class constructor */
    JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName, const QString& function, JKQTPlotter* parent);


    /** \brief class destructor */
    virtual ~JKQTPParsedFunctionLineGraphBase() override;

    /** \copydoc function */
    QString getFunction() const;

    /** \copydoc errorFunction */
    QString getErrorFunction() const;
    /** \copydoc dependentVariableName */
    QString getDependentVariableName() const;

public Q_SLOTS:
    /** \copydoc errorFunction */
    void setErrorFunction(const QString & __value);
    /** \copydoc function */
    void setFunction(const QString & __value);

protected:

    /** \brief INTERNAL data structure combining a JKQTPMathParser and a JKQTPMathParser::jkmpNode
      */
    struct ParsedFunctionLineGraphFunctionData {
        inline ParsedFunctionLineGraphFunctionData(): varcount(0) {};
        std::shared_ptr<JKQTPMathParser> parser;
        std::shared_ptr<JKQTPMathParser::jkmpNode> node;
        int varcount;
        QString dependentVariableName;
    };

    /** \brief nache of the dependent variable (e.g. x for a function f(x) ) */
    QString dependentVariableName;

    /** \brief the function to be evaluated for the plot.  Use \c x as the free variable, e.g. \c "x^2+2" */
    QString function;
    /** \brief parser data structure for function */
    ParsedFunctionLineGraphFunctionData fdata;

    /** \brief the function to be evaluated to add error indicators to the graph. This function is evaluated to an error for every x. Use \c x as the free variable, e.g. \c "x^2+2". */
    QString errorFunction;
    /** \brief parser data structure for errorFunction */
    ParsedFunctionLineGraphFunctionData efdata;

    /** \brief implements the actual plot function */
    static double evaluateParsedFunction(double x, ParsedFunctionLineGraphFunctionData* fdata) ;
};



/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$ The function is defined as a string and parsed by JKMathParser
    \ingroup jkqtplotter_functiongraphs

    Additional function parameters may be given in the vector parameters. They are accessible in the function as \c p1 , \c p2 , \c p3 , ...
    Parameters may also be given from a data column. Then first the params from the column and the the parameters from the vector are numbered.

    Use the variable \c x in an equation to refer to the free parameter of the curve.

    \see \ref JKQTPlotterParsedFunctionPlot, JKQTPMathParser
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXParsedFunctionLineGraph: public JKQTPParsedFunctionLineGraphBase {
    Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPXParsedFunctionLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXParsedFunctionLineGraph(JKQTPlotter* parent);

        /** \brief class constructor */
        JKQTPXParsedFunctionLineGraph(const QString& function, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXParsedFunctionLineGraph(const QString& function, JKQTPlotter* parent);


        /** \brief class destructor */
        virtual ~JKQTPXParsedFunctionLineGraph() override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;

    protected:


        /** \copydoc JKQTPEvaluatedFunctionGraphBase::buildPlotFunctorSpec() */
        virtual PlotFunctorSpec buildPlotFunctorSpec() override;

        /** \copydoc JKQTPEvaluatedFunctionWithErrorsGraphBase::buildPlotFunctorSpec() */
        virtual std::function<QPointF(double)> buildErrorFunctorSpec() override;
};




/*! \brief This implements line plots where the data is taken from a user supplied function \f$ x=f(y) \f$ The function is defined as a string and parsed by JKMathParser
    \ingroup jkqtplotter_functiongraphs

    Additional function parameters may be given in the vector parameters. They are accessible in the function as \c p1 , \c p2 , \c p3 , ...
    Parameters may also be given from a data column. Then first the params from the column and the the parameters from the vector are numbered.

    Use the variable \c y in an equation to refer to the free parameter of the curve (\c is also understood for convenience).

    \see \ref JKQTPlotterParsedFunctionPlot, JKQTPMathParser

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPYParsedFunctionLineGraph: public JKQTPParsedFunctionLineGraphBase {
        Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPYParsedFunctionLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPYParsedFunctionLineGraph(JKQTPlotter* parent);

        /** \brief class constructor */
        JKQTPYParsedFunctionLineGraph(const QString& function, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPYParsedFunctionLineGraph(const QString& function, JKQTPlotter* parent);


        /** \brief class destructor */
        virtual ~JKQTPYParsedFunctionLineGraph() override;

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;


    protected:

        /** \copydoc JKQTPEvaluatedFunctionGraphBase::buildPlotFunctorSpec() */
        virtual PlotFunctorSpec buildPlotFunctorSpec() override;

        /** \copydoc JKQTPEvaluatedFunctionWithErrorsGraphBase::buildPlotFunctorSpec() */
        virtual std::function<QPointF(double)> buildErrorFunctorSpec() override;
};
#endif // jkqtpgraphsparsedfunction_H
