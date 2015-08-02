/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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


#ifndef JKQTPPARSEDFUNCTIONELEMENTS_H
#define JKQTPPARSEDFUNCTIONELEMENTS_H

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtptools.h"
#include "jkqtpmathparser.h"
#include "jkqtp_imexport.h"
#include "jkqtpelements.h"

// forward declarations
class JKQtBasePlotter;
class JKQtPlotter;


struct JKQTPxParsedFunctionLineGraphFunctionData {
    JKQTPMathParser* parser;
    JKQTPMathParser::jkmpNode* node;
    int varcount;
};


/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$ The function is defined as a string and parsed by JKMathParser
    \ingroup jkqtplotter_plots

    Additional function parameters may be given in the vector parameters. They are accessible in the function as \c p1 , \c p2 , \c p3 , ...
    Parameters may also be given from a data column. Then first the params from the column and the the parameters from the vector are numbered.
 */
class LIB_EXPORT JKQTPxParsedFunctionLineGraph: public JKQTPxFunctionLineGraph {
        Q_OBJECT
    public:


        /** \brief class constructor */
        JKQTPxParsedFunctionLineGraph(JKQtBasePlotter* parent=NULL);
        /** \brief class constructor */
        JKQTPxParsedFunctionLineGraph(JKQtPlotter* parent);


        /** \brief class destructor */
        virtual ~JKQTPxParsedFunctionLineGraph();

        JKQTPGET_SET_MACRO(QList<double>, parameters)
        JKQTPGET_SET_MACRO(QString, function)
        JKQTPGET_SET_MACRO(int, parameterColumn)

        JKQTPGET_SET_MACRO(QList<double>, errorParameters)
        JKQTPGET_SET_MACRO(QString, errorFunction)
        JKQTPGET_SET_MACRO(int, errorParameterColumn)
    protected:


        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        QString function;
        QList<double> parameters;
        JKQTPxParsedFunctionLineGraphFunctionData fdata;
        int parameterColumn;

        QString errorFunction;
        QList<double> errorParameters;
        JKQTPxParsedFunctionLineGraphFunctionData efdata;
        int errorParameterColumn;

        JKQTPGET_SET_MACRO_I(jkqtpPlotFunctionType, plotFunction, clearData())
        JKQTPGET_SET_MACRO_I(void*, params, clearData())
        JKQTPGET_SET_MACRO(jkqtpPlotFunctionType, errorPlotFunction)
        JKQTPGET_SET_MACRO(void*, errorParams)

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData(bool collectParams=true);
};
#endif // JKQTPPARSEDFUNCTIONELEMENTS_H
