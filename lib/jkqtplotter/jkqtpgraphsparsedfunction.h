/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtpmathparser.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpgraphsevaluatedfunction.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;



/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$ The function is defined as a string and parsed by JKMathParser
    \ingroup jkqtplotter_parsedFgraphs

    Additional function parameters may be given in the vector parameters. They are accessible in the function as \c p1 , \c p2 , \c p3 , ...
    Parameters may also be given from a data column. Then first the params from the column and the the parameters from the vector are numbered.

    Use the variable \c x in an equation to refer to the free parameter of the curve.
 */
class JKQTP_LIB_EXPORT JKQTPXParsedFunctionLineGraph: public JKQTPXFunctionLineGraph {
    Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPXParsedFunctionLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPXParsedFunctionLineGraph(JKQTPlotter* parent);


        /** \brief class destructor */
        virtual ~JKQTPXParsedFunctionLineGraph() override;

        /*! \copydoc function
            \see see function for details */ 
        inline virtual void setFunction(const QString & __value)  
        {
            this->function = __value;
        } 
        /*! \copydoc function
            \see see function for details */ 
        inline virtual QString getFunction() const  
        {
            return this->function; 
        }

        /*! \copydoc errorFunction
            \see see errorFunction for details */ 
        inline virtual void setErrorFunction(const QString & __value)  
        {
            this->errorFunction = __value;
        } 
        /*! \copydoc errorFunction
            \see see errorFunction for details */ 
        inline virtual QString getErrorFunction() const  
        {
            return this->errorFunction; 
        }

        /** \brief INTERNAL data structure
         *  \internal
         */
        struct JKQTPXParsedFunctionLineGraphFunctionData {
            JKQTPMathParser* parser;
            JKQTPMathParser::jkmpNode* node;
            int varcount;
        };

    protected:
        /** \brief the function to be evaluated for the plot.  Use \c x as the free variable, e.g. \c "x^2+2" */
        QString function;
        JKQTPXParsedFunctionLineGraphFunctionData fdata;

        /** \brief the function to be evaluated to add error indicators to the graph. This function is evaluated to an error for every x. Use \c x as the free variable, e.g. \c "x^2+2". */
        QString errorFunction;
        JKQTPXParsedFunctionLineGraphFunctionData efdata;

        // hide functions that should not be used in this class!
        using JKQTPXFunctionLineGraph::setPlotFunction;
        using JKQTPXFunctionLineGraph::setParams;
        using JKQTPXFunctionLineGraph::setErrorPlotFunction;
        using JKQTPXFunctionLineGraph::setErrorParams;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData(bool collectParams=true) override;

        /** \brief implements the actual plot function */
        static double JKQTPXParsedFunctionLineGraphFunction(double x, void *data) ;
};




/*! \brief This implements line plots where the data is taken from a user supplied function \f$ x=f(y) \f$ The function is defined as a string and parsed by JKMathParser
    \ingroup jkqtplotter_parsedFgraphs

    Additional function parameters may be given in the vector parameters. They are accessible in the function as \c p1 , \c p2 , \c p3 , ...
    Parameters may also be given from a data column. Then first the params from the column and the the parameters from the vector are numbered.

    Use the variable \c y in an equation to refer to the free parameter of the curve (\c is also understood for convenience).
 */
class JKQTP_LIB_EXPORT JKQTPYParsedFunctionLineGraph: public JKQTPYFunctionLineGraph {
        Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPYParsedFunctionLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPYParsedFunctionLineGraph(JKQTPlotter* parent);


        /** \brief class destructor */
        virtual ~JKQTPYParsedFunctionLineGraph() override;

        /*! \copydoc function
            \see see function for details */ 
        inline virtual void setFunction(const QString & __value)  
        {
            this->function = __value;
        } 
        /*! \copydoc function
            \see see function for details */ 
        inline virtual QString getFunction() const  
        {
            return this->function; 
        }

        /*! \copydoc errorFunction
            \see see errorFunction for details */ 
        inline virtual void setErrorFunction(const QString & __value)  
        {
            this->errorFunction = __value;
        } 
        /*! \copydoc errorFunction
            \see see errorFunction for details */ 
        inline virtual QString getErrorFunction() const  
        {
            return this->errorFunction; 
        }

        /** \brief INTERNAL data structure
         *  \internal
         */
        struct JKQTPYParsedFunctionLineGraphFunctionData {
            JKQTPMathParser* parser;
            JKQTPMathParser::jkmpNode* node;
            int varcount;
        };

    protected:
        /** \brief the function to be evaluated for the plot.  Use \c x as the free variable, e.g. \c "x^2+2" */
        QString function;
        JKQTPYParsedFunctionLineGraphFunctionData fdata;

        /** \brief the function to be evaluated to add error indicators to the graph. This function is evaluated to an error for every x. Use \c x as the free variable, e.g. \c "x^2+2". */
        QString errorFunction;
        JKQTPYParsedFunctionLineGraphFunctionData efdata;

        // hide functions that should not be used in this class!
        using JKQTPXFunctionLineGraph::setPlotFunction;
        using JKQTPXFunctionLineGraph::setParams;
        using JKQTPXFunctionLineGraph::setErrorPlotFunction;
        using JKQTPXFunctionLineGraph::setErrorParams;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData(bool collectParams=true) override;
        /** \brief implements the actual plot function */
        static double JKQTPYParsedFunctionLineGraphFunction(double x, void *data);
};
#endif // jkqtpgraphsparsedfunction_H
