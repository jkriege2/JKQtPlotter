/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#include "jkqtplotter/jkqtpgraphsparsedfunction.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include <stdlib.h>
#include <QDebug>
#include <QElapsedTimer>
#include <utility>


JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraph(JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    fdata.parser=new JKQTPMathParser();
    fdata.node=nullptr;
    fdata.varcount=0;
    function="";
    parameterColumn=-1;
    set_params(&fdata);
    set_plotFunction(&JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunction);

    efdata.parser=new JKQTPMathParser();
    efdata.node=nullptr;
    efdata.varcount=0;
    errorFunction="";
    errorParameterColumn=-1;
    set_errorParams(&efdata);
    set_errorPlotFunction(&JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunction);
}

JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraph(JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    fdata.parser=new JKQTPMathParser();
    fdata.node=nullptr;
    fdata.varcount=0;
    function="";
    parameterColumn=-1;
    set_params(&fdata);
    set_plotFunction(&JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunction);

    efdata.parser=new JKQTPMathParser();
    efdata.node=nullptr;
    efdata.varcount=0;
    errorFunction="";
    errorParameterColumn=-1;
    set_errorParams(&efdata);
    set_errorPlotFunction(&JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunction);
}

JKQTPXParsedFunctionLineGraph::~JKQTPXParsedFunctionLineGraph()
{
    if (fdata.node) delete fdata.node;
    delete fdata.parser;
    if (efdata.node) delete efdata.node;
    delete efdata.parser;
}


void JKQTPXParsedFunctionLineGraph::createPlotData(bool /*collectParams*/)
{
    collectParameters();

    //QElapsedTimer timer;
    //timer.start();
    for (int i=0; i<fdata.varcount; i++) {
        fdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    fdata.varcount=0;
    try {
        QVector<double>* parameters=static_cast<QVector<double>*>(params);
        if (parameters) {
            for (int i=0; i<parameters->size(); i++) {
                fdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(fdata.varcount+1), parameters->at(i));
                fdata.varcount=fdata.varcount+1;
            }
        }
        fdata.parser->addVariableDouble(std::string("x"), 0.0);
        if (fdata.node) delete fdata.node;
        //qint64 t=timer.elapsed();


        //qDebug()<<"createPlotData():   adding variables: "<<t<<"ms";
        fdata.node=fdata.parser->parse(function.toStdString());
        //qDebug()<<"createPlotData():   parsing: "<<timer.elapsed()-t<<"ms";
    } catch(std::exception& /*E*/) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    //qint64 t0=timer.elapsed();
    for (int i=0; i<efdata.varcount; i++) {
        efdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    efdata.varcount=0;
    try {
        QVector<double>* errorParameters=static_cast<QVector<double>*>(errorParams);
        if (errorParameters) {
            for (int i=0; i<errorParameters->size(); i++) {
                efdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(efdata.varcount+1), errorParameters->at(i));
                efdata.varcount=efdata.varcount+1;
            }
        }
        efdata.parser->addVariableDouble(std::string("x"), 0.0);
        if (efdata.node) delete efdata.node;
        //qint64 t=timer.elapsed();
        //qDebug()<<"createPlotData():   adding variables: "<<t-t0<<"ms";
        efdata.node=efdata.parser->parse(errorFunction.toStdString());
        //qDebug()<<"createPlotData():   parsing: "<<timer.elapsed()-t<<"ms";
    } catch(std::exception& /*E*/) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    set_params(&fdata);
    set_plotFunction(JKQTPXParsedFunctionLineGraphFunction);
    set_errorParams(&efdata);
    set_errorPlotFunction(JKQTPXParsedFunctionLineGraphFunction);

    //qint64 t=timer.elapsed();
    JKQTPXFunctionLineGraph::createPlotData(false);
    //qDebug()<<"createPlotData():   JKQTPXFunctionLineGraph::createPlotData():   "<<timer.elapsed()-t<<"ms";

    /*int count=0;
    doublePair* d=data;
    while (d!=nullptr) {
        count++;
        d=d->next;
    }
    qDebug()<<"refined to "<<count<<" daatapoints";*/
}


double JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunction(double x, void* data) {
    JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunctionData* d=(JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunctionData*)data;
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
        } catch(std::exception& /*E*/) {
            //qDebug()<<QString("parser error: %1").arg(E.what());
           /*ok= QMessageBox::critical(this, tr("QuickFit-table"),
                                     tr("An error occured while parsing the expression '%1' in cell (row, column)=(%3, %4):\n%2\n\n\"OK\" will still go on evaluating\n\"Cancel\" will cancel evaluation for the rest of the cells.").arg(dlgMathExpression->getExpression()).arg(E.what()).arg(row).arg(column),
                                        QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Ok;*/

        }
    }
    return NAN;

}

















JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraph(JKQTBasePlotter *parent):
    JKQTPYFunctionLineGraph(parent)
{
    fdata.parser=new JKQTPMathParser();
    fdata.node=nullptr;
    fdata.varcount=0;
    function="";
    parameterColumn=-1;
    set_params(&fdata);
    set_plotFunction(&JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraphFunction);

    efdata.parser=new JKQTPMathParser();
    efdata.node=nullptr;
    efdata.varcount=0;
    errorFunction="";
    errorParameterColumn=-1;
    set_errorParams(&efdata);
    set_errorPlotFunction(&JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraphFunction);
}

JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraph(JKQTPlotter *parent):
    JKQTPYFunctionLineGraph(parent)
{
    fdata.parser=new JKQTPMathParser();
    fdata.node=nullptr;
    fdata.varcount=0;
    function="";
    parameterColumn=-1;
    set_params(&fdata);
    set_plotFunction(&JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraphFunction);

    efdata.parser=new JKQTPMathParser();
    efdata.node=nullptr;
    efdata.varcount=0;
    errorFunction="";
    errorParameterColumn=-1;
    set_errorParams(&efdata);
    set_errorPlotFunction(&JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraphFunction);
}

JKQTPYParsedFunctionLineGraph::~JKQTPYParsedFunctionLineGraph()
{
    if (fdata.node) delete fdata.node;
    delete fdata.parser;
    if (efdata.node) delete efdata.node;
    delete efdata.parser;
}

void JKQTPYParsedFunctionLineGraph::createPlotData(bool /*collectParams*/)
{
    collectParameters();

    //QElapsedTimer timer;
    //timer.start();
    for (int i=0; i<fdata.varcount; i++) {
        fdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    fdata.varcount=0;
    try {
        QVector<double>* parameters=static_cast<QVector<double>*>(params);
        if (parameters) {
            for (int i=0; i<parameters->size(); i++) {
                fdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(fdata.varcount+1), parameters->at(i));
                fdata.varcount=fdata.varcount+1;
            }
        }
        fdata.parser->addVariableDouble(std::string("x"), 0.0);
        fdata.parser->addVariableDouble(std::string("y"), 0.0);
        if (fdata.node) delete fdata.node;
        //qint64 t=timer.elapsed();


        //qDebug()<<"createPlotData():   adding variables: "<<t<<"ms";
        fdata.node=fdata.parser->parse(function.toStdString());
        //qDebug()<<"createPlotData():   parsing: "<<timer.elapsed()-t<<"ms";
    } catch(std::exception& /*E*/) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    //qint64 t0=timer.elapsed();
    for (int i=0; i<efdata.varcount; i++) {
        efdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    efdata.varcount=0;
    try {
        QVector<double>* errorParameters=static_cast<QVector<double>*>(errorParams);
        if (errorParameters) {
            for (int i=0; i<errorParameters->size(); i++) {
                efdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(efdata.varcount+1), errorParameters->at(i));
                efdata.varcount=efdata.varcount+1;
            }
        }
        efdata.parser->addVariableDouble(std::string("x"), 0.0);
        efdata.parser->addVariableDouble(std::string("y"), 0.0);
        if (efdata.node) delete efdata.node;
        //qint64 t=timer.elapsed();
        //qDebug()<<"createPlotData():   adding variables: "<<t-t0<<"ms";
        efdata.node=efdata.parser->parse(errorFunction.toStdString());
        //qDebug()<<"createPlotData():   parsing: "<<timer.elapsed()-t<<"ms";
    } catch(std::exception& /*E*/) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    set_params(&fdata);
    set_plotFunction(JKQTPYParsedFunctionLineGraphFunction);
    set_errorParams(&efdata);
    set_errorPlotFunction(JKQTPYParsedFunctionLineGraphFunction);

    //qint64 t=timer.elapsed();
    JKQTPYFunctionLineGraph::createPlotData(false);
    //qDebug()<<"createPlotData():   JKQTPYFunctionLineGraph::createPlotData():   "<<timer.elapsed()-t<<"ms";

    /*int count=0;
    doublePair* d=data;
    while (d!=nullptr) {
        count++;
        d=d->next;
    }
    qDebug()<<"refined to "<<count<<" daatapoints";*/
}

double JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraphFunction(double x, void* data) {
    JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraphFunctionData* d=(JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraphFunctionData*)data;
    if (d && d->parser && d->node) {
        try {
            d->parser->addVariableDouble("x", x);
            d->parser->addVariableDouble("y", x);
            JKQTPMathParser::jkmpResult r=d->node->evaluate();

            if (r.isValid) {
                if (r.type==JKQTPMathParser::jkmpBool) {
                    return r.boolean?1.0:0.0;
                } else if (r.type==JKQTPMathParser::jkmpDouble) {
                    return r.num;
                }
            }
        } catch(std::exception& /*E*/) {
            //qDebug()<<QString("parser error: %1").arg(E.what());
           /*ok= QMessageBox::critical(this, tr("QuickFit-table"),
                                     tr("An error occured while parsing the expression '%1' in cell (row, column)=(%3, %4):\n%2\n\n\"OK\" will still go on evaluating\n\"Cancel\" will cancel evaluation for the rest of the cells.").arg(dlgMathExpression->getExpression()).arg(E.what()).arg(row).arg(column),
                                        QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Ok;*/

        }
    }
    return NAN;

}
