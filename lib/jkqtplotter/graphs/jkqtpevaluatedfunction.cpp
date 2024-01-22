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



#include "jkqtplotter/graphs/jkqtpevaluatedfunction.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include <stdexcept>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"



JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase(JKQTBasePlotter* parent):
    JKQTPEvaluatedFunctionWithErrorsGraphBase(parent),
    drawErrorPolygons(false),
    drawErrorLines(false)
{
    initLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
    initFillStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
    setFillCurve(false);

    errorColor=JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.errorColorDerivationMode, getLineColor());
    errorFillColor=JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.errorFillColorDerivationMode, errorColor);
    errorStyle=Qt::SolidLine;
    errorLineWidth=parent->getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.defaultErrorIndicatorWidth;
    errorFillStyle=Qt::SolidPattern;


    if (parent && parentPlotStyle>=0) { // get style settings from parent object
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        const JKQTBasePlotter::JKQTPPen pen=parent->getPlotStyle(parentPlotStyle);
        errorColor=pen.errorColor();
        errorStyle=pen.errorStyle();
        errorLineWidth=pen.errorWidthF();
        errorFillStyle=pen.errorFillStyle();
        errorFillColor=pen.errorFillColor();
    }
    errorFillColor.setAlphaF(0.5);
}

JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase(JKQTPlotter* parent):
    JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase(parent->getPlotter())
{

}

JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::~JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase()
{

}


void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& r) {
    QRectF rect=r;
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    p.setWidthF(getKeyLineWidthPx(painter, rect, parent));
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    const double y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    rect.setWidth(rect.width()-p.widthF());
    rect.setHeight(rect.height()-p.widthF());
    rect.setX(rect.x()+p.widthF()/2.0);
    rect.setY(rect.y()+p.widthF()/2.0);
    if (getDrawLine()) painter.setPen(p);
    painter.setBrush(b);
    if (getFillCurve()) painter.drawRect(rect);
    if (!getFillCurve() && getDrawLine()) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));

}

QColor JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getKeyLabelColor() const {
    return getLineColor();
}


QBrush JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getErrorBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(errorFillColor);
    b.setStyle(errorFillStyle);
    return b;
}

QPen JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getErrorLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(errorColor);
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*errorLineWidth)));
    p.setStyle(errorStyle);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}


void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setErrorLineColor(const QColor &__value)
{
    this->errorColor = __value;
}

QColor JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getErrorLineColor() const
{
    return this->errorColor;
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setErrorFillColor(const QColor &__value)
{
    this->errorFillColor = __value;
}

QColor JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getErrorFillColor() const
{
    return this->errorFillColor;
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setErrorFillStyle(Qt::BrushStyle __value)
{
    this->errorFillStyle = __value;
}

Qt::BrushStyle JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getErrorFillStyle() const
{
    return this->errorFillStyle;
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setErrorLineStyle(Qt::PenStyle __value)
{
    this->errorStyle = __value;
}

Qt::PenStyle JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getErrorLineStyle() const
{
    return this->errorStyle;
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setErrorLineWidth(double __value)
{
    this->errorLineWidth = __value;
}

double JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getErrorLineWidth() const
{
    return this->errorLineWidth;
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setColor(QColor c)
{
    setLineColor(c);
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setDrawErrorPolygons(bool __value)
{
    this->drawErrorPolygons = __value;
}

bool JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getDrawErrorPolygons() const
{
    return this->drawErrorPolygons;
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::setDrawErrorLines(bool __value)
{
    this->drawErrorLines = __value;
}

bool JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::getDrawErrorLines() const
{
    return this->drawErrorLines;
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::drawXGraph(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::drawXGraph");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //qDebug()<<"start plot\n";
    createPlotData();
    //qDebug()<<"plot data created\n";

    auto errorPlotFunction=buildErrorFunctorSpec();

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
        if (getYAxis()->isLogAxis()) y0=transformY(getYAxis()->getMin());
        bool first=false;
        QPolygonF filledPolygon, linePolygon, errorLineTop, errorLineBottom;
        QList<QPointF> epTop, epBottom;
        double yami=qMin(transformY(getYAxis()->getMin()),transformY(getYAxis()->getMax()));
        double yama=qMax(transformY(getYAxis()->getMin()),transformY(getYAxis()->getMax()));
        double dypix=fabs(yama-yami);
        yami=yami-2.0*dypix;
        yama=yama+2.0*dypix;
        for (auto it=data.begin(); it!=data.end(); ++it) {
            const QPointF& d=*it;
            double x=d.x();
            double y=d.y();

            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
                const double xv=backtransformX(x);
                const double yv=backtransformY(y);
                double ype=0, yme=0;
                if ((drawErrorLines || drawErrorPolygons) && (static_cast<bool>(errorPlotFunction))) {
                    const double e=errorPlotFunction(xv).y();
                    ype=transformY(yv+e);
                    yme=transformY(yv-e);
                    ype=qBound(yami, ype, yama);
                    yme=qBound(yami, yme, yama);
                }

                y=qBound(yami, y, yama);

                if (getFillCurve()) {
                    if (!first) filledPolygon<<QPointF(x, y0);
                    filledPolygon<<QPointF(x, y);
                    if (it+1==data.end()) filledPolygon<<QPointF(x, y0);
                }

                if (drawErrorPolygons && (static_cast<bool>(errorPlotFunction))) {
                    epTop<<QPointF(x, ype);
                    epBottom<<QPointF(x, yme);
                }

                if (getDrawLine()) {
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
        if (getFillCurve()) {
            painter.save(); auto __finalpaintfillc=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(b);
            painter.setPen(np);
            painter.drawPolygon(filledPolygon, Qt::OddEvenFill);
        }
        if (getDrawLine()) {
            painter.save(); auto __finalpaintline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            painter.drawPolylineFast(linePolygon);
        }

        if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
            painter.save(); auto __finalpainterrline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(ep);
            painter.drawPolylineFast(errorLineTop);
            painter.drawPolylineFast(errorLineBottom);
        }


        if (displaySamplePoints) drawSamplePoints(painter, getLineColor());
    }
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}

void JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase::drawYGraph(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPYFunctionLineGraph::drawYGraph");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //std::cout<<"start plot\n";
    createPlotData();
    //std::cout<<"plot data created\n";

    auto errorPlotFunction=buildErrorFunctorSpec();

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
        if (getXAxis()->isLogAxis()) x0=transformX(getXAxis()->getMin());
        bool first=false;
        QPolygonF filledPolygon, linePolygon, errorLineTop, errorLineBottom;
        QList<QPointF> epTop, epBottom;
        double xami=qMin(transformY(getXAxis()->getMin()),transformY(getXAxis()->getMax()));
        double xama=qMax(transformY(getXAxis()->getMin()),transformY(getXAxis()->getMax()));
        double dxpix=fabs(xama-xami);
        xami=xami-2.0*dxpix;
        xama=xama+2.0*dxpix;
        for (auto it=data.begin(); it!=data.end(); ++it) {
            const QPointF& d=*it;
            double x=d.x();
            double y=d.y();

            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
                const double xv=backtransformX(x);
                const double yv=backtransformY(y);
                double xpe=0, xme=0;
                if ((drawErrorLines || drawErrorPolygons) && (static_cast<bool>(errorPlotFunction))) {
                    const double e=errorPlotFunction(yv).x();
                    xpe=transformX(xv+e);
                    xme=transformX(xv-e);
                    xpe=qBound(xami, xpe, xama);
                    xme=qBound(xami, xme, xama);
                }

                x=qBound(xami, x, xama);

                if (getFillCurve()) {
                    if (!first) filledPolygon<<QPointF(x0, y);
                    filledPolygon<<QPointF(x, y);
                    if (it+1==data.end()) filledPolygon<<QPointF(x0, y);
                }

                if (drawErrorPolygons && (static_cast<bool>(errorPlotFunction))) {
                    epTop<<QPointF(xpe, y);
                    epBottom<<QPointF(xme, y);
                }

                if (getDrawLine()) {
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
        if (getFillCurve()) {
            painter.save(); auto __finalpaintfillc=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(b);
            painter.setPen(np);
            painter.drawPolygon(filledPolygon, Qt::OddEvenFill);
        }
        if (getDrawLine()) {
            painter.save(); auto __finalpaintline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            painter.drawPolylineFast(linePolygon);
        }

        if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
            painter.save(); auto __finalpainterrline=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(ep);
            painter.drawPolylineFast(errorLineTop);
            painter.drawPolylineFast(errorLineBottom);
        }


        if (displaySamplePoints) drawSamplePoints(painter, getLineColor());
    }
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}












JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(JKQTBasePlotter* parent):
    JKQTPEvaluatedFunctionWithErrorsGraphDrawingBase(parent)
{

}

JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(JKQTPlotter* parent):
    JKQTPFunctorLineGraphBase(parent->getPlotter())
{

}

JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(const jkqtpSimplePlotFunctionType &f, const QString &title_, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(parent)
{
    title=title_;
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=f;
    data.clear();
}

JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(const jkqtpSimplePlotFunctionType &f, const QString &title_, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(f, title_, parent->getPlotter())
{
}

JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(parent)
{
    title=title_;
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(f);
    data.clear();
}

JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(std::move(f), title_, parent->getPlotter())
{

}

JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(JKQTPFunctorLineGraphBase::SpecialFunction type, const QVector<double> &params, const QString &title_, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(parent)
{
    title=title_;
    setSpecialFunction(type);
    setParams(params);
    data.clear();
}

JKQTPFunctorLineGraphBase::JKQTPFunctorLineGraphBase(JKQTPFunctorLineGraphBase::SpecialFunction type, const QVector<double> &params, const QString &title, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(type, params, title, parent->getPlotter())
{

}


JKQTPFunctorLineGraphBase::~JKQTPFunctorLineGraphBase() {

}

void JKQTPFunctorLineGraphBase::setSpecialFunction(JKQTPFunctorLineGraphBase::SpecialFunction function)
{
    if (function==JKQTPFunctorLineGraphBase::Polynomial) {
        setPlotFunctionFunctor([](double x, const QVector<double>& param) {
            double res=0;
            if (param.size()>0) {
                res=param.value(0,0);
                double xx=x;
                for (int i=1; i<param.size(); i++) {
                    res=res+param.value(i,0)*xx;
                    xx=xx*x;
                }
            }

            return res;
        });
    }
    else if (function==JKQTPFunctorLineGraphBase::Exponential) setPlotFunctionFunctor([](double x, const QVector<double>& param) {
            double res=0;
            if (param.size()>=3) {
                res=param.value(0,0)+param.value(1,0)*exp(x/param.value(2,0));
            } else if (param.size()>=2) {
                res=param.value(0,0)*exp(x/param.value(1,0));
            }
            return res;
        });
    else if (function==JKQTPFunctorLineGraphBase::PowerLaw) setPlotFunctionFunctor([](double x, const QVector<double>& param) {
            double res=0;
            if (param.size()>=3) {
                res=param.value(0,0)+param.value(1,0)*pow(x, param.value(2,1));
            } else if (param.size()>=2) {
                res=param.value(0,0)*pow(x, param.value(1,1));
            } else if (param.size()>=1) {
                res=pow(x, param.value(0,1));
            }
            return res;
        });
    else throw std::runtime_error("unknown special function type");
}


void JKQTPFunctorLineGraphBase::setErrorPlotFunction(const jkqtpPlotFunctionType &__value)
{
    errorSimplePlotFunction=jkqtpSimplePlotFunctionType();
    errorPlotFunction=__value;
    data.clear();
}

void JKQTPFunctorLineGraphBase::setErrorPlotFunction(jkqtpPlotFunctionType &&__value)
{
    errorSimplePlotFunction=jkqtpSimplePlotFunctionType();
    errorPlotFunction = std::move(__value);
    data.clear();
}
jkqtpPlotFunctionType JKQTPFunctorLineGraphBase::getErrorPlotFunction() const
{
    return errorPlotFunction;
}

void JKQTPFunctorLineGraphBase::setErrorPlotFunction(const jkqtpSimplePlotFunctionType &__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction=__value;
    data.clear();
}

void JKQTPFunctorLineGraphBase::setErrorPlotFunction(jkqtpSimplePlotFunctionType &&__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction = std::move(__value);
    data.clear();
}

jkqtpSimplePlotFunctionType JKQTPFunctorLineGraphBase::getErrorSimplePlotFunction() const
{
    return errorSimplePlotFunction;
}

void JKQTPFunctorLineGraphBase::setPlotFunctionFunctor(const jkqtpPlotFunctionType &__value)
{
    simplePlotFunction=jkqtpSimplePlotFunctionType();
    plotFunction = __value;
    data.clear();
}

void JKQTPFunctorLineGraphBase::setPlotFunctionFunctor(const jkqtpSimplePlotFunctionType &__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=__value;
    data.clear();
}

void JKQTPFunctorLineGraphBase::setPlotFunctionFunctor(jkqtpPlotFunctionType &&__value)
{
    simplePlotFunction=jkqtpSimplePlotFunctionType();
    plotFunction = std::move(__value);
    data.clear();
}

void JKQTPFunctorLineGraphBase::setPlotFunctionFunctor(jkqtpSimplePlotFunctionType &&__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(__value);
    data.clear();
}

jkqtpPlotFunctionType JKQTPFunctorLineGraphBase::getPlotFunctionFunctor() const
{
    return plotFunction;
}

jkqtpSimplePlotFunctionType JKQTPFunctorLineGraphBase::getSimplePlotFunction() const
{
    return simplePlotFunction;
}

bool JKQTPFunctorLineGraphBase::isSimplePlotFunction() const
{
    return !static_cast<bool>(plotFunction) && static_cast<bool>(simplePlotFunction);
}






JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTBasePlotter* parent):
    JKQTPFunctorLineGraphBase(parent)
{

}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTPlotter* parent):
    JKQTPFunctorLineGraphBase(parent)
{

}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title_, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(f, title_, parent)
{
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title_, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(f, title_, parent)
{
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(std::move(f), title_, parent)
{
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(std::move(f), title_, parent)
{

}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTPXFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title_, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(type, params, title_, parent)
{
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(JKQTPXFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(type, params, title, parent)
{

}


JKQTPXFunctionLineGraph::~JKQTPXFunctionLineGraph() {

}

void JKQTPXFunctionLineGraph::draw(JKQTPEnhancedPainter &painter)
{
    drawXGraph(painter);
}


JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec JKQTPXFunctionLineGraph::buildPlotFunctorSpec()
{
    JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec spec;

    if (parent==nullptr) return spec; // return an invalid PlotFunctorSpec
    if (!plotFunction && !simplePlotFunction) return spec; // return an invalid PlotFunctorSpec

    // range over which to evaluate func
    spec.range_start=parent->getXMin();
    spec.range_end=parent->getXMax();

    // the actual function to use
    if (plotFunction) spec.func=std::bind([&](double x, const QVector<double>& p) -> QPointF { return QPointF(x, plotFunction(x, p)); }, std::placeholders::_1, getInternalParams());
    else if (simplePlotFunction) spec.func=[&](double x) -> QPointF { return QPointF(x, simplePlotFunction(x)); };

    return spec;
}

std::function<QPointF (double)> JKQTPXFunctionLineGraph::buildErrorFunctorSpec()
{
    std::function<QPointF (double)> spec;
    if (parent==nullptr) return spec; // return an invalid PlotFunctorSpec
    if (!plotFunction && !simplePlotFunction) return spec; // return an invalid PlotFunctorSpec

    // the actual function to use
    if (errorPlotFunction) spec=std::bind([&](double x, const QVector<double>& p) -> QPointF { return QPointF(0, errorPlotFunction(x, p)); }, std::placeholders::_1, getInternalParams());
    else if (errorSimplePlotFunction) spec=[&](double x) -> QPointF { return QPointF(0, errorSimplePlotFunction(x)); };

    return spec;
}

bool JKQTPXFunctionLineGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    return false;
}

bool JKQTPXFunctionLineGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    return false;
}














JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(parent)
{

}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTPlotter *parent):
    JKQTPYFunctionLineGraph(parent->getPlotter())
{

}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(f, title, parent)
{

}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(f, title, parent)
{

}


JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(std::move(f), title, parent)
{

}


JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(std::move(f), title, parent)
{

}


JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTPYFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title_, JKQTBasePlotter *parent):
    JKQTPFunctorLineGraphBase(type, params, title_, parent)
{
}

JKQTPYFunctionLineGraph::JKQTPYFunctionLineGraph(JKQTPYFunctionLineGraph::SpecialFunction type, const QVector<double> &params, const QString &title_, JKQTPlotter *parent):
    JKQTPFunctorLineGraphBase(type, params, title_, parent)
{

}

void JKQTPYFunctionLineGraph::draw(JKQTPEnhancedPainter &painter)
{
    drawYGraph(painter);
}

JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec JKQTPYFunctionLineGraph::buildPlotFunctorSpec()
{
    JKQTPEvaluatedFunctionGraphBase::PlotFunctorSpec spec;

    if (parent==nullptr) return spec; // return an invalid PlotFunctorSpec
    if (!plotFunction && !simplePlotFunction) return spec; // return an invalid PlotFunctorSpec

    // range over which to evaluate func
    spec.range_start=parent->getYMin();
    spec.range_end=parent->getYMax();

    // the actual function to use
    if (plotFunction) spec.func=std::bind([&](double y, const QVector<double>& p) -> QPointF { return QPointF(plotFunction(y, p), y); }, std::placeholders::_1, getInternalParams());
    else if (simplePlotFunction) spec.func=[&](double y) -> QPointF { return QPointF(simplePlotFunction(y), y); };

    return spec;
}

std::function<QPointF (double)> JKQTPYFunctionLineGraph::buildErrorFunctorSpec()
{
    std::function<QPointF (double)> spec;
    if (parent==nullptr) return spec; // return an invalid PlotFunctorSpec
    if (!plotFunction && !simplePlotFunction) return spec; // return an invalid PlotFunctorSpec

    // the actual function to use
    if (errorPlotFunction) spec=std::bind([&](double y, const QVector<double>& p) -> QPointF { return QPointF(errorPlotFunction(y, p), 0); }, std::placeholders::_1, getInternalParams());
    else if (errorSimplePlotFunction) spec=[&](double y) -> QPointF { return QPointF(errorSimplePlotFunction(y), 0); };

    return spec;
}

bool JKQTPYFunctionLineGraph::getXMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    return false;
}

bool JKQTPYFunctionLineGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    return false;
}






