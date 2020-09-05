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



#include "jkqtplotter/graphs/jkqtpevaluatedfunction.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"





JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTBasePlotter* parent):
    JKQTPEvaluatedFunctionGraphBase(parent)
{
    functionType=SpecialFunction::UserFunction;
    drawLine=true;
    fillCurve=false;
    params=nullptr;

    initLineStyle(parent, parentPlotStyle);
    initFillStyle(parent, parentPlotStyle);

    drawErrorPolygons=false;
    drawErrorLines=false;
    errorParams=nullptr;
    errorColor=getLineColor().lighter();
    errorFillColor=getLineColor().lighter();
    errorStyle=Qt::SolidLine;
    errorLineWidth=1;
    errorFillStyle=Qt::SolidPattern;

    parameterColumn=-1;
    errorParameterColumn=-1;


    if (parent && parentPlotStyle>=0) { // get style settings from parent object
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        errorColor=parent->getPlotStyle(parentPlotStyle).errorColor();
        errorStyle=parent->getPlotStyle(parentPlotStyle).errorStyle();
        errorLineWidth=parent->getPlotStyle(parentPlotStyle).errorWidthF();
        errorFillStyle=parent->getPlotStyle(parentPlotStyle).errorFillStyle();
        errorFillColor=parent->getPlotStyle(parentPlotStyle).errorFillColor();
    }
    errorFillColor.setAlphaF(0.5);
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTPlotter* parent):
    JKQTPXFunctionLineGraph(parent->getPlotter())
{

}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title_, JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    title=title_;
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=f;
    functionType=SpecialFunction::UserFunction;
    data.clear();
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title_, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(f, title_, parent->getPlotter())
{
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    title=title_;
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(f);
    functionType=SpecialFunction::UserFunction;
    data.clear();
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(std::move(f), title_, parent->getPlotter())
{

}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTPXFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title_, JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    title=title_;
    functionType=type;
    setParams(params);
    data.clear();
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTPXFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(type, params, title, parent->getPlotter())
{

}


JKQTPXFunctionLineGraph::~JKQTPXFunctionLineGraph() {
    data.clear();
}


void JKQTPXFunctionLineGraph::setDrawLine(bool __value)
{
    this->drawLine = __value;
}

bool JKQTPXFunctionLineGraph::getDrawLine() const
{
    return this->drawLine;
}

void JKQTPXFunctionLineGraph::setPlotFunctionFunctor(const jkqtpPlotFunctionType &__value)
{
    simplePlotFunction=jkqtpSimplePlotFunctionType();
    plotFunction = __value;
    functionType=SpecialFunction::UserFunction;

    data.clear();
}

void JKQTPXFunctionLineGraph::setPlotFunctionFunctor(const jkqtpSimplePlotFunctionType &__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=__value;
    functionType=SpecialFunction::UserFunction;

    data.clear();
}

void JKQTPXFunctionLineGraph::setPlotFunctionFunctor(jkqtpPlotFunctionType &&__value)
{
    simplePlotFunction=jkqtpSimplePlotFunctionType();
    plotFunction = std::move(__value);
    functionType=SpecialFunction::UserFunction;
    data.clear();
}

void JKQTPXFunctionLineGraph::setPlotFunctionFunctor(jkqtpSimplePlotFunctionType &&__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(__value);
    functionType=SpecialFunction::UserFunction;

    data.clear();
}

jkqtpPlotFunctionType JKQTPXFunctionLineGraph::getPlotFunctionFunctor() const
{
    return plotFunction;
}

jkqtpSimplePlotFunctionType JKQTPXFunctionLineGraph::getSimplePlotFunction() const
{
    return simplePlotFunction;
}

void JKQTPXFunctionLineGraph::setParams(void *__value)
{
    if (this->params != __value) {
        this->params = __value;
        data.clear();
    }
}

void *JKQTPXFunctionLineGraph::getParams() const
{
    return this->params;
}


void JKQTPXFunctionLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    const double y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (drawLine) painter.setPen(p);
    painter.setBrush(b);
    if (fillCurve) painter.drawRect(rect);
    if (!fillCurve & drawLine) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));

}

QColor JKQTPXFunctionLineGraph::getKeyLabelColor() const {
    return getLineColor();
}

bool JKQTPXFunctionLineGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    smallestGreaterZero=minx=maxx=0; return false;
}

bool JKQTPXFunctionLineGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    smallestGreaterZero=miny=maxy=0; return false;
}

void JKQTPXFunctionLineGraph::createPlotData(bool collectParams) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPXFunctionLineGraph[%1]::createPlotData()").arg(title));
#endif
    data.clear();
    if (collectParams) collectParameters();

    if (parent==nullptr) return;
    if (!plotFunction && !simplePlotFunction) return;

    jkqtpSimplePlotFunctionType func;
    if (plotFunction) func=std::bind(plotFunction, std::placeholders::_1, params);
    else if (simplePlotFunction) func=simplePlotFunction;

    const double xmin=parent->getXMin();
    const double xmax=parent->getXMax();

    std::function<QPointF(double)> fTransformedFunc= std::bind([&](const JKQTPPlotElement* plot, double x) -> QPointF { return plot->transform(x, func(x)); }, this, std::placeholders::_1);

    JKQTPAdaptiveFunctionGraphEvaluator evaluator(fTransformedFunc, minSamples, maxRefinementDegree, slopeTolerance, minPixelPerSample);
    data=evaluator.evaluate(xmin, xmax);
    data=JKQTPSimplyfyLineSegemnts(data, dataCleanupMaxAllowedAngleDegree);
}

void JKQTPXFunctionLineGraph::collectParameters()
{
    if (parent && parameterColumn>=0) {
        iparams.clear();
        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=static_cast<int>(datastore->getRows(parameterColumn));

        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(parameterColumn,i);
            iparams<<xv;
        }
        //qDebug()<<"iparams_beforeclean:";
        //for (int i=0; i<iparams.size(); i++) qDebug()<<iparams[i];
        int i=iparams.size()-1;
        while (i>=0 && !JKQTPIsOKFloat(iparams[i])) {
            iparams.remove(i,1);
            i--;
        }

        //qDebug()<<"iparams:";
        //for (i=0; i<iparams.size(); i++) qDebug()<<iparams[i];

        params=&iparams;
    }
    if (parent && errorParameterColumn>=0) {
        ierrorparams.clear();
        JKQTPDatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax= static_cast<int>(datastore->getRows(errorParameterColumn));

        for (int i=imin; i<imax; i++) {
            double xv=datastore->get(errorParameterColumn,i);
            ierrorparams<<xv;
        }
        int i=ierrorparams.size()-1;
        while (i>=0 && !JKQTPIsOKFloat(ierrorparams[i])) {
            ierrorparams.remove(i,1);
            i--;
        }

        errorParams=&ierrorparams;
    }
}



void JKQTPXFunctionLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPXFunctionLineGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //qDebug()<<"start plot\n";
    createPlotData();
    //qDebug()<<"plot data created\n";

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        QPen np(Qt::NoPen);

        QPen ep=painter.pen();
        ep.setColor(errorColor);
        ep.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, errorLineWidth*parent->getLineWidthMultiplier())));
        ep.setStyle(errorStyle);
        ep.setJoinStyle(Qt::RoundJoin);

        QBrush b=getFillBrush(painter, parent);

        QBrush eb=painter.brush();
        eb.setColor(errorFillColor);
        eb.setStyle(errorFillStyle);


        double y0=transformY(0);
        if (parent->getYAxis()->isLogAxis()) y0=transformY(parent->getYAxis()->getMin());
        bool first=false;
        QPolygonF filledPolygon, linePolygon, errorLineTop, errorLineBottom;
        QList<QPointF> epTop, epBottom;
        double yami=qMin(transformY(parent->getYAxis()->getMin()),transformY(parent->getYAxis()->getMax()));
        double yama=qMax(transformY(parent->getYAxis()->getMin()),transformY(parent->getYAxis()->getMax()));
        double dypix=fabs(yama-yami);
        yami=yami-2.0*dypix;
        yama=yama+2.0*dypix;
        for (auto it=data.begin(); it!=data.end(); ++it) {
            const QPointF& d=*it;
            double x=d.x();
            double y=d.y();

            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
                double xv=backtransformX(x);
                double yv=backtransformY(y);
                double ype=0, yme=0;
                if ((drawErrorLines || drawErrorPolygons) && (static_cast<bool>(errorPlotFunction))) {
                    double e=errorPlotFunction(xv, errorParams);
                    ype=transformY(yv+e);
                    yme=transformY(yv-e);
                    ype=qBound(yami, ype, yama);
                    yme=qBound(yami, yme, yama);
                }

                y=qBound(yami, y, yama);

                if (fillCurve) {
                    if (!first) filledPolygon<<QPointF(x, y0);
                    filledPolygon<<QPointF(x, y);
                    if (it+1==data.end()) filledPolygon<<QPointF(x, y0);
                }

                if (drawErrorPolygons && (static_cast<bool>(errorPlotFunction))) {
                    epTop<<QPointF(x, ype);
                    epBottom<<QPointF(x, yme);
                }

                if (drawLine) {
                    linePolygon<<QPointF(x, y);
                }

                if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
                    errorLineTop<<QPointF(x, ype);
                    errorLineBottom<<QPointF(x, yme);
                }

                first=true;
            }
        }
        if (drawErrorPolygons) {
            painter.save(); auto __finalpainterrpoly=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(eb);
            painter.setPen(np);
            QPolygonF poly;
            for (int i=0; i<epTop.size(); i++) {
                poly<<epTop[i];
            }
            for (int i=epBottom.size()-1; i>=0; i--) {
                poly<<epBottom[i];
            }
            painter.drawPolygon(poly, Qt::OddEvenFill);

        }
        if (fillCurve) {
            painter.save(); auto __finalpaintfillc=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(b);
            painter.setPen(np);
            painter.drawPolygon(filledPolygon, Qt::OddEvenFill);
        }
        if (drawLine) {
            painter.save(); auto __finalpaintline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            painter.drawPolyline(linePolygon);
        }

        if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
            painter.save(); auto __finalpainterrline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(ep);
            painter.drawPolyline(errorLineTop);
            painter.drawPolyline(errorLineBottom);
        }


        if (displaySamplePoints) drawSamplePoints(painter, getLineColor());
    }
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}















JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{

}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTPlotter *parent):
    JKQTPYFunctionLineGraph(parent->getPlotter())
{

}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title, JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(f, title, parent)
{

}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(f, title, parent)
{

}


JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title, JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(std::move(f), title, parent)
{

}


JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(std::move(f), title, parent)
{

}


JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTPYFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title_, JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(type, params, title_, parent)
{
}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTPYFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title_, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(type, params, title_, parent->getPlotter())
{

}

void JKQTPYFunctionLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPYFunctionLineGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //std::cout<<"start plot\n";
    createPlotData();
    //std::cout<<"plot data created\n";

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePen(painter, parent);
        QPen np(Qt::NoPen);

        QPen ep=painter.pen();
        ep.setColor(errorColor);
        ep.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, errorLineWidth*parent->getLineWidthMultiplier())));
        ep.setStyle(errorStyle);
        ep.setJoinStyle(Qt::RoundJoin);

        QBrush b=getFillBrush(painter, parent);

        QBrush eb=painter.brush();
        eb.setColor(errorFillColor);
        eb.setStyle(errorFillStyle);


        double x0=transformX(0);
        if (parent->getXAxis()->isLogAxis()) x0=transformX(parent->getXAxis()->getMin());
        bool first=false;
        QPolygonF filledPolygon, linePolygon, errorLineTop, errorLineBottom;
        QList<QPointF> epTop, epBottom;
        double xami=qMin(transformY(parent->getXAxis()->getMin()),transformY(parent->getXAxis()->getMax()));
        double xama=qMax(transformY(parent->getXAxis()->getMin()),transformY(parent->getXAxis()->getMax()));
        double dxpix=fabs(xama-xami);
        xami=xami-2.0*dxpix;
        xama=xama+2.0*dxpix;
        for (auto it=data.begin(); it!=data.end(); ++it) {
            const QPointF& d=*it;
            double x=d.x();
            double y=d.y();

            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
                double xv=backtransformX(x);
                double yv=backtransformY(y);
                double xpe=0, xme=0;
                if ((drawErrorLines || drawErrorPolygons) && (static_cast<bool>(errorPlotFunction))) {
                    double e=errorPlotFunction(yv, errorParams);
                    xpe=transformX(xv+e);
                    xme=transformX(xv-e);
                    xpe=qBound(xami, xpe, xama);
                    xme=qBound(xami, xme, xama);
                }

                x=qBound(xami, x, xama);

                if (fillCurve) {
                    if (!first) filledPolygon<<QPointF(x0, y);
                    filledPolygon<<QPointF(x, y);
                    if (it+1==data.end()) filledPolygon<<QPointF(x0, y);
                }

                if (drawErrorPolygons && (static_cast<bool>(errorPlotFunction))) {
                    epTop<<QPointF(xpe, y);
                    epBottom<<QPointF(xme, y);
                }

                if (drawLine) {
                    linePolygon<<QPointF(x, y);
                }

                if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
                    errorLineTop<<QPointF(xpe, y);
                    errorLineBottom<<QPointF(xme, y);
                }

                first=true;
            }
        }
        if (drawErrorPolygons) {
            painter.save(); auto __finalpainterrpoly=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(eb);
            painter.setPen(np);
            QPolygonF poly;
            for (int i=0; i<epTop.size(); i++) {
                poly<<epTop[i];
            }
            for (int i=epBottom.size()-1; i>=0; i--) {
                poly<<epBottom[i];
            }
            painter.drawPolygon(poly, Qt::OddEvenFill);
        }
        if (fillCurve) {
            painter.save(); auto __finalpaintfillc=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(b);
            painter.setPen(np);
            painter.drawPolygon(filledPolygon, Qt::OddEvenFill);
        }
        if (drawLine) {
            painter.save(); auto __finalpaintline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            painter.drawPolyline(linePolygon);
        }

        if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
            painter.save(); auto __finalpainterrline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(ep);
            painter.drawPolyline(errorLineTop);
            painter.drawPolyline(errorLineBottom);
        }


        if (displaySamplePoints) drawSamplePoints(painter, getLineColor());
    }
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}


void JKQTPYFunctionLineGraph::createPlotData(bool collectParams) {
    data.clear();
    if (collectParams) collectParameters();

    if (parent==nullptr) return;
    if (!plotFunction && !simplePlotFunction) return;

    jkqtpSimplePlotFunctionType func;
    if (plotFunction) func=std::bind(plotFunction, std::placeholders::_1, params);
    else if (simplePlotFunction) func=simplePlotFunction;

    const double ymin=parent->getYMin();
    const double ymax=parent->getYMax();

    std::function<QPointF(double)> fTransformedFunc= std::bind([&](const JKQTPPlotElement* plot, double y) -> QPointF { return plot->transform(func(y), y); }, this, std::placeholders::_1);

    JKQTPAdaptiveFunctionGraphEvaluator evaluator(fTransformedFunc, minSamples, maxRefinementDegree, slopeTolerance, minPixelPerSample);
    data=evaluator.evaluate(ymin, ymax);
    data=JKQTPSimplyfyLineSegemnts(data, dataCleanupMaxAllowedAngleDegree);

}




QBrush JKQTPXFunctionLineGraph::getErrorBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(errorFillColor);
    b.setStyle(errorFillStyle);
    return b;
}

QPen JKQTPXFunctionLineGraph::getErrorLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(errorColor);
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*errorLineWidth)));
    p.setStyle(errorStyle);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}


void JKQTPXFunctionLineGraph::setParams(const QVector<double> &params)
{
    iparams=params;
    setParams(&iparams);
}

void JKQTPXFunctionLineGraph::setCopiedParams(const double *params, int N)
{
    QVector<double> v;
    for (int i=0; i<N; i++) { v<<params[i]; }
    setParams(v);
}

void JKQTPXFunctionLineGraph::setParamsV(double p1) {
    QVector<double> p;
    p<<p1;
    setParams(p);
}

void JKQTPXFunctionLineGraph::setParamsV(double p1, double p2) {
    QVector<double> p;
    p<<p1<<p2;
    setParams(p);
}

void JKQTPXFunctionLineGraph::setParamsV(double p1, double p2, double p3) {
    QVector<double> p;
    p<<p1<<p2<<p3;
    setParams(p);
}

void JKQTPXFunctionLineGraph::setParamsV(double p1, double p2, double p3, double p4) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4;
    setParams(p);
}

void JKQTPXFunctionLineGraph::setParamsV(double p1, double p2, double p3, double p4, double p5) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4<<p5;
    setParams(p);
}

void JKQTPXFunctionLineGraph::setErrorParams(const QVector<double> &errorParams)
{
    ierrorparams=errorParams;
    setErrorParams(&ierrorparams);
}

void JKQTPXFunctionLineGraph::setParameterColumn(int __value)
{
    this->parameterColumn = __value;
}

int JKQTPXFunctionLineGraph::getParameterColumn() const
{
    return this->parameterColumn;
}

void JKQTPXFunctionLineGraph::setParameterColumn(size_t __value) {
    this->parameterColumn = static_cast<int>(__value);
}

void JKQTPXFunctionLineGraph::setErrorParameterColumn(int __value)
{
    this->errorParameterColumn = __value;
}

int JKQTPXFunctionLineGraph::getErrorParameterColumn() const
{
    return this->errorParameterColumn;
}

void JKQTPXFunctionLineGraph::setErrorParameterColumn(size_t __value) {
    this->errorParameterColumn = static_cast<int>(__value);
}

void JKQTPXFunctionLineGraph::setErrorLineColor(const QColor &__value)
{
    this->errorColor = __value;
}

QColor JKQTPXFunctionLineGraph::getErrorLineColor() const
{
    return this->errorColor;
}

void JKQTPXFunctionLineGraph::setErrorFillColor(const QColor &__value)
{
    this->errorFillColor = __value;
}

QColor JKQTPXFunctionLineGraph::getErrorFillColor() const
{
    return this->errorFillColor;
}

void JKQTPXFunctionLineGraph::setErrorFillStyle(Qt::BrushStyle __value)
{
    this->errorFillStyle = __value;
}

Qt::BrushStyle JKQTPXFunctionLineGraph::getErrorFillStyle() const
{
    return this->errorFillStyle;
}

void JKQTPXFunctionLineGraph::setErrorLineStyle(Qt::PenStyle __value)
{
    this->errorStyle = __value;
}

Qt::PenStyle JKQTPXFunctionLineGraph::getErrorLineStyle() const
{
    return this->errorStyle;
}

void JKQTPXFunctionLineGraph::setErrorLineWidth(double __value)
{
    this->errorLineWidth = __value;
}

double JKQTPXFunctionLineGraph::getErrorLineWidth() const
{
    return this->errorLineWidth;
}

void JKQTPXFunctionLineGraph::setSpecialFunction(JKQTPXFunctionLineGraph::SpecialFunction function)
{
    if (function==JKQTPXFunctionLineGraph::Polynomial) {
        setPlotFunctionFunctor([](double x, void* param) {
            double res=0;
            QVector<double>* d=static_cast<QVector<double>*>(param);
            if (d && d->size()>0) {
                res=d->value(0,0);
                double xx=x;
                for (int i=1; i<d->size(); i++) {
                    res=res+d->value(i,0)*xx;
                    xx=xx*x;
                }
            }

            return res;
        });
    }
    else if (function==JKQTPXFunctionLineGraph::Exponential) setPlotFunctionFunctor([](double x, void* param) {
            double res=0;
            QVector<double>* d=static_cast<QVector<double>*>(param);
            if (d) {
                if (d->size()>=3) {
                    res=d->value(0,0)+d->value(1,0)*exp(x/d->value(2,0));
                } else if (d->size()>=2) {
                    res=d->value(0,0)*exp(x/d->value(1,0));
                }
            }
            return res;
        });
    else if (function==JKQTPXFunctionLineGraph::PowerLaw) setPlotFunctionFunctor([](double x, void* param) {
            double res=0;
            QVector<double>* d=static_cast<QVector<double>*>(param);
            if (d) {
                if (d->size()>=3) {
                    res=d->value(0,0)+d->value(1,0)*pow(x, d->value(2,1));
                } else if (d->size()>=2) {
                    res=d->value(0,0)*pow(x, d->value(1,1));
                } else if (d->size()>=1) {
                    res=pow(x, d->value(0,1));
                }

            }
            return res;
        });
    else throw std::runtime_error("unknown special function type");
}

JKQTPXFunctionLineGraph::SpecialFunction JKQTPXFunctionLineGraph::getFunctionType() const
{
    return functionType;
}

QVector<double> JKQTPXFunctionLineGraph::getInternalParams() const {
    return iparams;
}
QVector<double> JKQTPXFunctionLineGraph::getInternalErrorParams() const {
    return ierrorparams;
}

void JKQTPXFunctionLineGraph::setDrawErrorPolygons(bool __value)
{
    this->drawErrorPolygons = __value;
}

bool JKQTPXFunctionLineGraph::getDrawErrorPolygons() const
{
    return this->drawErrorPolygons;
}

void JKQTPXFunctionLineGraph::setDrawErrorLines(bool __value)
{
    this->drawErrorLines = __value;
}

bool JKQTPXFunctionLineGraph::getDrawErrorLines() const
{
    return this->drawErrorLines;
}

void JKQTPXFunctionLineGraph::setErrorPlotFunction(const jkqtpPlotFunctionType &__value)
{
    errorSimplePlotFunction=jkqtpSimplePlotFunctionType();
    errorPlotFunction=__value;
    data.clear();
}

void JKQTPXFunctionLineGraph::setErrorPlotFunction(jkqtpPlotFunctionType &&__value)
{
    errorSimplePlotFunction=jkqtpSimplePlotFunctionType();
    errorPlotFunction = std::move(__value);
    data.clear();
}
jkqtpPlotFunctionType JKQTPXFunctionLineGraph::getErrorPlotFunction() const
{
    return errorPlotFunction;
}

void JKQTPXFunctionLineGraph::setErrorPlotFunction(const jkqtpSimplePlotFunctionType &__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction=__value;
    data.clear();
}

void JKQTPXFunctionLineGraph::setErrorPlotFunction(jkqtpSimplePlotFunctionType &&__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction = std::move(__value);
    data.clear();
}
jkqtpSimplePlotFunctionType JKQTPXFunctionLineGraph::getErrorSimplePlotFunction() const
{
    return errorSimplePlotFunction;
}

void JKQTPXFunctionLineGraph::setErrorParams(void *__value)
{
    this->errorParams = __value;
}

void *JKQTPXFunctionLineGraph::getErrorParams() const
{
    return this->errorParams;
}


bool JKQTPXFunctionLineGraph::usesColumn(int c) const
{
    return (c==parameterColumn)||(c==errorParameterColumn);
}

