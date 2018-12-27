/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
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
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtpmathparser.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpgraphsevaluatedfunction.h"

// forward declarations
class JKQtBasePlotter;
class JKQtPlotter;



/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$ The function is defined as a string and parsed by JKMathParser
    \ingroup jkqtplotter_plots

    Additional function parameters may be given in the vector parameters. They are accessible in the function as \c p1 , \c p2 , \c p3 , ...
    Parameters may also be given from a data column. Then first the params from the column and the the parameters from the vector are numbered.

    Use the variable \c x in an equation to refer to the free parameter of the curve.
 */
class LIB_EXPORT JKQTPxParsedFunctionLineGraph: public JKQTPxFunctionLineGraph {
    Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPxParsedFunctionLineGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPxParsedFunctionLineGraph(JKQtPlotter* parent);


        /** \brief class destructor */
        virtual ~JKQTPxParsedFunctionLineGraph() override;

        JKQTPGET_SET_MACRO(QString, function)

        JKQTPGET_SET_MACRO(QString, errorFunction)

        /** \brief INTERNAL data structure
         *  \internal
         */
        struct JKQTPxParsedFunctionLineGraphFunctionData {
            JKQTPMathParser* parser;
            JKQTPMathParser::jkmpNode* node;
            int varcount;
        };

    protected:
        /** \brief the function to be evaluated for the plot.  Use \c x as the free variable, e.g. \c "x^2+2" */
        QString function;
        JKQTPxParsedFunctionLineGraphFunctionData fdata;

        /** \brief the function to be evaluated to add error indicators to the graph. This function is evaluated to an error for every x. Use \c x as the free variable, e.g. \c "x^2+2". */
        QString errorFunction;
        JKQTPxParsedFunctionLineGraphFunctionData efdata;

        // hide functions that should not be used in this class!
        using JKQTPxFunctionLineGraph::set_plotFunction;
        using JKQTPxFunctionLineGraph::set_params;
        using JKQTPxFunctionLineGraph::set_errorPlotFunction;
        using JKQTPxFunctionLineGraph::set_errorParams;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData(bool collectParams=true) override;

        /** \brief implements the actual plot function */
        static double JKQTPxParsedFunctionLineGraphFunction(double x, void *data) ;
};




/*! \brief This implements line plots where the data is taken from a user supplied function \f$ x=f(y) \f$ The function is defined as a string and parsed by JKMathParser
    \ingroup jkqtplotter_plots

    Additional function parameters may be given in the vector parameters. They are accessible in the function as \c p1 , \c p2 , \c p3 , ...
    Parameters may also be given from a data column. Then first the params from the column and the the parameters from the vector are numbered.

    Use the variable \c y in an equation to refer to the free parameter of the curve (\c is also understood for convenience).
 */
class LIB_EXPORT JKQTPyParsedFunctionLineGraph: public JKQTPyFunctionLineGraph {
        Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPyParsedFunctionLineGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPyParsedFunctionLineGraph(JKQtPlotter* parent);


        /** \brief class destructor */
        virtual ~JKQTPyParsedFunctionLineGraph() override;

        JKQTPGET_SET_MACRO(QString, function)

        JKQTPGET_SET_MACRO(QString, errorFunction)

        /** \brief INTERNAL data structure
         *  \internal
         */
        struct JKQTPyParsedFunctionLineGraphFunctionData {
            JKQTPMathParser* parser;
            JKQTPMathParser::jkmpNode* node;
            int varcount;
        };

    protected:
        /** \brief the function to be evaluated for the plot.  Use \c x as the free variable, e.g. \c "x^2+2" */
        QString function;
        JKQTPyParsedFunctionLineGraphFunctionData fdata;

        /** \brief the function to be evaluated to add error indicators to the graph. This function is evaluated to an error for every x. Use \c x as the free variable, e.g. \c "x^2+2". */
        QString errorFunction;
        JKQTPyParsedFunctionLineGraphFunctionData efdata;

        // hide functions that should not be used in this class!
        using JKQTPxFunctionLineGraph::set_plotFunction;
        using JKQTPxFunctionLineGraph::set_params;
        using JKQTPxFunctionLineGraph::set_errorPlotFunction;
        using JKQTPxFunctionLineGraph::set_errorParams;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData(bool collectParams=true) override;
        /** \brief implements the actual plot function */
        static double JKQTPyParsedFunctionLineGraphFunction(double x, void *data);
};
#endif // jkqtpgraphsparsedfunction_H
