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



#include "jkqtpparsedfunctionelements.h"
#include "jkqtpelements.h"
#include "jkqtpbaseplotter.h"
#include "jkqtptools.h"
#include <stdlib.h>
#include <QDebug>
#include <QElapsedTimer>



double JKQTPxParsedFunctionLineGraphFunction(double x, void* data) {
    JKQTPxParsedFunctionLineGraphFunctionData* d=(JKQTPxParsedFunctionLineGraphFunctionData*)data;
    if (d && d->parser && d->node) {
        try {
            d->parser->addVariableDouble("x", x);
            JKQTPMathParser::jkmpResult r=d->node->evaluate();

            if (r.isValid) {
                if (r.type==JKQTPMathParser::jkmpBool) {
                    return r.boolean?1.0:0.0;
                } else if (r.type==JKQTPMathParser::jkmpDouble) {
                    return r.num;
                }
            }
        } catch(std::exception& E) {
            //qDebug()<<QString("parser error: %1").arg(E.what());
           /*ok= QMessageBox::critical(this, tr("QuickFit-table"),
                                     tr("An error occured while parsing the expression '%1' in cell (row, column)=(%3, %4):\n%2\n\n\"OK\" will still go on evaluating\n\"Cancel\" will cancel evaluation for the rest of the cells.").arg(dlgMathExpression->getExpression()).arg(E.what()).arg(row).arg(column),
                                        QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Ok;*/

        }
    }
    return NAN;

}

JKQTPxParsedFunctionLineGraph::JKQTPxParsedFunctionLineGraph(JKQtBasePlotter *parent):
    JKQTPxFunctionLineGraph(parent)
{
    fdata.parser=new JKQTPMathParser();
    fdata.node=NULL;
    fdata.varcount=0;
    function="";
    parameterColumn=-1;
    set_params(&fdata);
    set_plotFunction(JKQTPxParsedFunctionLineGraphFunction);

    efdata.parser=new JKQTPMathParser();
    efdata.node=NULL;
    efdata.varcount=0;
    errorFunction="";
    errorParameterColumn=-1;
    set_errorParams(&efdata);
    set_errorPlotFunction(JKQTPxParsedFunctionLineGraphFunction);
}

JKQTPxParsedFunctionLineGraph::~JKQTPxParsedFunctionLineGraph()
{
    if (fdata.node) delete fdata.node;
    delete fdata.parser;
    if (efdata.node) delete efdata.node;
    delete efdata.parser;
}

void JKQTPxParsedFunctionLineGraph::createPlotData(bool /*collectParams*/)
{
    collectParameters();

    QElapsedTimer timer;
    timer.start();
    for (int i=0; i<fdata.varcount; i++) {
        fdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    fdata.varcount=0;
    try {
        if (parent && parameterColumn>=0) {

            JKQTPdatastore* datastore=parent->getDatastore();
            int imin=0;
            int imax=datastore->getColumn(parameterColumn).getRows();

            for (int i=imin; i<imax; i++) {
                double xv=datastore->get(parameterColumn,i);
                fdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(fdata.varcount+1), xv);
                fdata.varcount=fdata.varcount+1;

            }
        }
        for (int i=0; i<parameters.size(); i++) {
            fdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(fdata.varcount+1), parameters[i]);
            fdata.varcount=fdata.varcount+1;
        }
        fdata.parser->addVariableDouble(std::string("x"), 0.0);
        if (fdata.node) delete fdata.node;
        qint64 t=timer.elapsed();


        //qDebug()<<"createPlotData():   adding variables: "<<t<<"ms";
        fdata.node=fdata.parser->parse(function.toStdString());
        //qDebug()<<"createPlotData():   parsing: "<<timer.elapsed()-t<<"ms";
    } catch(std::exception& E) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    qint64 t0=timer.elapsed();
    for (int i=0; i<efdata.varcount; i++) {
        efdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    efdata.varcount=0;
    try {
        if (parent && errorParameterColumn>=0) {

            JKQTPdatastore* datastore=parent->getDatastore();
            int imin=0;
            int imax=datastore->getColumn(errorParameterColumn).getRows();

            for (int i=imin; i<imax; i++) {
                double xv=datastore->get(errorParameterColumn,i);
                efdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(efdata.varcount+1), xv);
                efdata.varcount=efdata.varcount+1;

            }
        }
        for (int i=0; i<errorParameters.size(); i++) {
            efdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(efdata.varcount+1), errorParameters[i]);
            efdata.varcount=efdata.varcount+1;
        }
        efdata.parser->addVariableDouble(std::string("x"), 0.0);
        if (efdata.node) delete efdata.node;
        qint64 t=timer.elapsed();
        //qDebug()<<"createPlotData():   adding variables: "<<t-t0<<"ms";
        efdata.node=efdata.parser->parse(errorFunction.toStdString());
        //qDebug()<<"createPlotData():   parsing: "<<timer.elapsed()-t<<"ms";
    } catch(std::exception& E) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    set_params(&fdata);
    set_plotFunction(JKQTPxParsedFunctionLineGraphFunction);
    set_errorParams(&efdata);
    set_errorPlotFunction(JKQTPxParsedFunctionLineGraphFunction);

    qint64 t=timer.elapsed();
    JKQTPxFunctionLineGraph::createPlotData(false);
    //qDebug()<<"createPlotData():   JKQTPxFunctionLineGraph::createPlotData():   "<<timer.elapsed()-t<<"ms";

    /*int count=0;
    doublePair* d=data;
    while (d!=NULL) {
        count++;
        d=d->next;
    }
    qDebug()<<"refined to "<<count<<" daatapoints";*/
}
