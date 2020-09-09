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

#include "jkqtplotter/graphs/jkqtpparsedfunction.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtptools.h"
#include <stdlib.h>
#include <QDebug>
#include <QElapsedTimer>
#include <utility>

JKQTPParsedFunctionLineGraphBase::JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName_, const QString& function_, JKQTBasePlotter *parent):
    JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase(parent),
    dependentVariableName(dependentVariableName_),
    function(function_)
{
    fdata.parser=std::make_shared<JKQTPMathParser>();
    fdata.node=nullptr;
    fdata.varcount=0;


    efdata.parser=std::make_shared<JKQTPMathParser>();
    efdata.node=nullptr;
    efdata.varcount=0;
}

JKQTPParsedFunctionLineGraphBase::JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName, const QString& function, JKQTPlotter *parent):
    JKQTPParsedFunctionLineGraphBase(dependentVariableName, function, parent->getPlotter())
{
}

JKQTPParsedFunctionLineGraphBase::JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName, JKQTBasePlotter *parent):
    JKQTPParsedFunctionLineGraphBase(dependentVariableName, QString(), parent)
{
}

JKQTPParsedFunctionLineGraphBase::JKQTPParsedFunctionLineGraphBase(const QString& dependentVariableName, JKQTPlotter *parent):
    JKQTPParsedFunctionLineGraphBase(dependentVariableName, QString(), parent)
{
}


JKQTPParsedFunctionLineGraphBase::~JKQTPParsedFunctionLineGraphBase()
{
}

void JKQTPParsedFunctionLineGraphBase::setFunction(const QString &__value)
{
    this->function = __value;
}

QString JKQTPParsedFunctionLineGraphBase::getFunction() const
{
    return this->function;
}

void JKQTPParsedFunctionLineGraphBase::setErrorFunction(const QString &__value)
{
    this->errorFunction = __value;
}

QString JKQTPParsedFunctionLineGraphBase::getErrorFunction() const
{
    return this->errorFunction;
}


QString JKQTPParsedFunctionLineGraphBase::getDependentVariableName() const
{
    return dependentVariableName;
}


double JKQTPParsedFunctionLineGraphBase::evaluateParsedFunction(double t, ParsedFunctionLineGraphFunctionData *fdata) {
    JKQTPParsedFunctionLineGraphBase::ParsedFunctionLineGraphFunctionData* d=fdata;//static_cast<JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraphFunctionData*>(data);
    if (d && d->parser && d->node) {
        try {
            d->parser->addVariableDouble(d->dependentVariableName.toStdString(), t);
            JKQTPMathParser::jkmpResult r=d->node->evaluate();

            if (r.isValid) {
                if (r.type==JKQTPMathParser::jkmpBool) {
                    return r.boolean?1.0:0.0;
                } else if (r.type==JKQTPMathParser::jkmpDouble) {
                    return r.num;
                }
            }
        } catch(std::exception& E) {
            qDebug()<<QString("parser error: %1").arg(E.what());
            /*ok= QMessageBox::critical(this, tr("QuickFit-table"),
                                     tr("An error occured while parsing the expression '%1' in cell (row, column)=(%3, %4):\n%2\n\n\"OK\" will still go on evaluating\n\"Cancel\" will cancel evaluation for the rest of the cells.").arg(dlgMathExpression->getExpression()).arg(E.what()).arg(row).arg(column),
                                        QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Ok;*/

        }
    }
    return NAN;

}
















JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraph(JKQTBasePlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("x"), parent)
{
}

JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraph(JKQTPlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("x"), parent)
{
}

JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraph(const QString& function, JKQTBasePlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("x"), function, parent)
{
}

JKQTPXParsedFunctionLineGraph::JKQTPXParsedFunctionLineGraph(const QString& function, JKQTPlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("x"), function, parent)
{
}

JKQTPXParsedFunctionLineGraph::~JKQTPXParsedFunctionLineGraph()
{
}

void JKQTPXParsedFunctionLineGraph::draw(JKQTPEnhancedPainter &painter)
{
    drawXGraph(painter);
}


JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec JKQTPXParsedFunctionLineGraph::buildPlotFunctorSpec()
{
    JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec spec;

    if (parent==nullptr) return spec; // return an invalid PlotFunctorSpec

    for (int i=0; i<fdata.varcount; i++) {
        fdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    fdata.varcount=0;
    try {
        for (const auto& p: getInternalParams()) {
            fdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(fdata.varcount+1), p);
            fdata.varcount=fdata.varcount+1;
        }
        fdata.dependentVariableName=getDependentVariableName();
        fdata.parser->addVariableDouble(getDependentVariableName().toStdString(), 0.0);
        fdata.node=std::shared_ptr<JKQTPMathParser::jkmpNode>(fdata.parser->parse(function.toStdString()));
    } catch(std::exception& E) {
        qDebug()<<QString("parser error: %1").arg(E.what());
    }

    jkqtpSimplePlotFunctionType plotFunction=std::bind(&JKQTPXParsedFunctionLineGraph::evaluateParsedFunction, std::placeholders::_1, &fdata);
    // the actual function to use
    spec.func=std::bind([=](double x) -> QPointF { return QPointF(x, plotFunction(x)); }, std::placeholders::_1);

    // range over which to evaluate func
    spec.range_start=parent->getXMin();
    spec.range_end=parent->getXMax();

    return spec;
}

std::function<QPointF (double)> JKQTPXParsedFunctionLineGraph::buildErrorFunctorSpec()
{
    std::function<QPointF (double)> spec;

    for (int i=0; i<efdata.varcount; i++) {
        efdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    efdata.varcount=0;
    try {
        for (const auto& p: getInternalErrorParams()) {
            efdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(efdata.varcount+1), p);
            efdata.varcount=efdata.varcount+1;
        }
        efdata.dependentVariableName=getDependentVariableName();
        efdata.parser->addVariableDouble(getDependentVariableName().toStdString(), 0.0);
        efdata.node=std::shared_ptr<JKQTPMathParser::jkmpNode>(efdata.parser->parse(errorFunction.toStdString()));
    } catch(std::exception& /*E*/) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    jkqtpSimplePlotFunctionType errorPlotFunction=std::bind(&JKQTPXParsedFunctionLineGraph::evaluateParsedFunction, std::placeholders::_1, &efdata);
    // the actual function to use
    spec=std::bind([=](double x) -> QPointF { return QPointF(0, errorPlotFunction(x)); }, std::placeholders::_1);

    return spec;
}























JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraph(JKQTBasePlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("y"), parent)
{
}

JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraph(JKQTPlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("y"), parent)
{
}

JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraph(const QString& function, JKQTBasePlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("y"), function, parent)
{
}

JKQTPYParsedFunctionLineGraph::JKQTPYParsedFunctionLineGraph(const QString& function, JKQTPlotter *parent):
    JKQTPParsedFunctionLineGraphBase(QString("y"), function, parent)
{
}

JKQTPYParsedFunctionLineGraph::~JKQTPYParsedFunctionLineGraph()
{
}

void JKQTPYParsedFunctionLineGraph::draw(JKQTPEnhancedPainter &painter)
{
    drawYGraph(painter);
}


JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec JKQTPYParsedFunctionLineGraph::buildPlotFunctorSpec()
{
    JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec spec;

    if (parent==nullptr) return spec; // return an invalid PlotFunctorSpec

    for (int i=0; i<fdata.varcount; i++) {
        fdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    fdata.varcount=0;
    try {
        for (const auto& p: getInternalParams()) {
            fdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(fdata.varcount+1), p);
            fdata.varcount=fdata.varcount+1;
        }
        fdata.dependentVariableName=getDependentVariableName();
        fdata.parser->addVariableDouble(getDependentVariableName().toStdString(), 0.0);
        fdata.node=std::shared_ptr<JKQTPMathParser::jkmpNode>(fdata.parser->parse(function.toStdString()));
    } catch(std::exception& E) {
        qDebug()<<QString("parser error: %1").arg(E.what());
    }

    jkqtpSimplePlotFunctionType plotFunction=std::bind(&JKQTPXParsedFunctionLineGraph::evaluateParsedFunction, std::placeholders::_1, &fdata);
    // the actual function to use
    spec.func=std::bind([=](double y) -> QPointF { return QPointF(plotFunction(y), y); }, std::placeholders::_1);

    // range over which to evaluate func
    spec.range_start=parent->getYMin();
    spec.range_end=parent->getYMax();

    return spec;
}

std::function<QPointF (double)> JKQTPYParsedFunctionLineGraph::buildErrorFunctorSpec()
{
    std::function<QPointF (double)> spec;

    for (int i=0; i<efdata.varcount; i++) {
        efdata.parser->deleteVariable(std::string("p")+jkqtp_inttostr(i+1));
    }
    efdata.varcount=0;
    try {
        for (const auto& p: getInternalErrorParams()) {
            efdata.parser->addVariableDouble(std::string("p")+jkqtp_inttostr(efdata.varcount+1), p);
            efdata.varcount=efdata.varcount+1;
        }
        efdata.dependentVariableName=getDependentVariableName();
        efdata.parser->addVariableDouble(getDependentVariableName().toStdString(), 0.0);
        efdata.node=std::shared_ptr<JKQTPMathParser::jkmpNode>(efdata.parser->parse(errorFunction.toStdString()));
    } catch(std::exception& /*E*/) {
        //qDebug()<<QString("parser error: %1").arg(E.what());
    }

    jkqtpSimplePlotFunctionType errorPlotFunction=std::bind(&JKQTPXParsedFunctionLineGraph::evaluateParsedFunction, std::placeholders::_1, &efdata);
    // the actual function to use
    spec=std::bind([=](double y) -> QPointF { return QPointF(errorPlotFunction(y), 0); }, std::placeholders::_1);

    return spec;
}
