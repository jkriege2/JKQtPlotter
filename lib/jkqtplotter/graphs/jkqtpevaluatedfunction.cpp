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
    JKQTPGraph(parent)
{
    functionType=SpecialFunction::UserFunction;
    drawLine=true;
    fillCurve=false;
    params=nullptr;
    minSamples=10;
    maxRefinementDegree=7;
    slopeTolerance=0.005;
    minPixelPerSample=32;
    plotRefinement=true;
    displaySamplePoints=false;
    data=nullptr;

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
    clearData();
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(const jkqtpSimplePlotFunctionType &f, const QString &title_, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    title=title_;
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=f;
    functionType=SpecialFunction::UserFunction;
    clearData();
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTBasePlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    title=title_;
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(f);
    functionType=SpecialFunction::UserFunction;
    clearData();
}

JKQTPXFunctionLineGraph::JKQTPXFunctionLineGraph(jkqtpSimplePlotFunctionType &&f, const QString &title_, JKQTPlotter *parent):
    JKQTPXFunctionLineGraph(parent)
{
    title=title_;
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(f);
    functionType=SpecialFunction::UserFunction;
    clearData();
}


JKQTPXFunctionLineGraph::~JKQTPXFunctionLineGraph() {
    clearData();
}

void JKQTPXFunctionLineGraph::clearData() {
    while (data!=nullptr) {
        doublePair* d=data;
        data=data->next;
        delete d;
    }
    data=nullptr;
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

    clearData();
}

void JKQTPXFunctionLineGraph::setPlotFunctionFunctor(const jkqtpSimplePlotFunctionType &__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=__value;
    functionType=SpecialFunction::UserFunction;

    clearData();
}

void JKQTPXFunctionLineGraph::setPlotFunctionFunctor(jkqtpPlotFunctionType &&__value)
{
    simplePlotFunction=jkqtpSimplePlotFunctionType();
    plotFunction = std::move(__value);
    functionType=SpecialFunction::UserFunction;
    clearData();
}

void JKQTPXFunctionLineGraph::setPlotFunctionFunctor(jkqtpSimplePlotFunctionType &&__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(__value);
    functionType=SpecialFunction::UserFunction;

    clearData();
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
        clearData();
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
    clearData();
    if (collectParams) collectParameters();

    if (parent==nullptr) return;
    if (!plotFunction && !simplePlotFunction) return;

    jkqtpSimplePlotFunctionType func;
    if (plotFunction) func=std::bind(plotFunction, std::placeholders::_1, params);
    else if (simplePlotFunction) func=simplePlotFunction;

    double xmin=parent->getXMin();
    double xmax=parent->getXMax();
    double pxmin=transformX(xmin);
    double pxmax=transformX(xmax);
    double delta0=(pxmax-pxmin)/static_cast<double>(minSamples);
    //double logdelta0=(log(xmax)-log(xmin))/static_cast<double>(minSamples);

    // initially sample function
    doublePair* d=new doublePair;
    d->x=xmin;
    d->f=func(xmin);
    d->next=nullptr;
    data=d;
    /*if (parent && parent->getXAxis()->isLogAxis()) {
        for (double x=log(xmin)+logdelta0; x<log(xmax); x=x+logdelta0) {
            d->next = new doublePair;
            d->next->x=exp(x+(static_cast<double>(rand())/static_cast<double>(RAND_MAX)-0.5)*delta0/2.0);
            d->next->f=func(d->next->x,);
            d->next->next=nullptr;
            doublePair* dd=d;
            d=d->next;
            refine(dd, d);
        }
    } else {*/
    QVector<double>* dv=static_cast<QVector<double>*>(params);
    if (functionType==Polynomial && dv && dv->size()<=2) {
           // we only need the first and last datapoint
    } else {
       for (double x=pxmin+delta0; x<pxmax; x=x+delta0) {
           d->next = new doublePair;
           d->next->x=parent->p2x(x+(static_cast<double>(rand())/static_cast<double>(RAND_MAX)-0.5)*delta0/2.0);
           d->next->f=func(d->next->x);
           d->next->next=nullptr;
           doublePair* dd=d;
           d=d->next;
           refine(dd, d);
       }
    }

    //}
    d->next = new doublePair;
    d->next->x=xmax;
    d->next->f=func(xmax);
    d->next->next=nullptr;
    refine(d, d->next);

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

void JKQTPXFunctionLineGraph::refine(doublePair* a, doublePair* b, unsigned int degree) {
    if (degree>=maxRefinementDegree) return;
    double ax=transformX(a->x);
    double af=transformX(a->f);
    double bx=transformX(b->x);
    double bf=transformX(b->f);

    double delta=bx - ax;
    //double logdelta=log(bx) - log(ax);
    double xmid=ax+(delta)/2.0;
    /*if (parent && parent->getXAxis()->isLogAxis()) {
        xmid=log(a->x)+(logdelta)/2.0;
        xmid=xmid+(static_cast<double>(rand())/static_cast<double>(RAND_MAX)-0.5)*delta/5.0;
        xmid=exp(xmid);
    } else {*/
        xmid=xmid+(static_cast<double>(rand())/static_cast<double>(RAND_MAX)-0.5)*delta/5.0; // shake by 10%
    //}
    double realxmid=parent->p2x(xmid);
    double realfmid = 0.0;
    if (plotFunction) realfmid=plotFunction(realxmid, params);
    else if (simplePlotFunction) realfmid=simplePlotFunction(realxmid);
    double fmid=transformY(realfmid);
    double a1=(fmid - af)/(xmid - ax);
    double a2=(bf - fmid)/(bx - xmid);
    //std::cout<<std::string(degree*2, ' ')<<"refine( ["<<a->x<<", "<<a->f<<"], ["<<xmid<<", "<<fmid<<"],   ["<<b->x<<", "<<b->f<<"] ): a1="<<a1<<",  a2="<<a2<<"  acrit="<<abs(a2/a1)-1.0<<"\n";
    //std::cout<<std::string(degree*2, ' ')<<"refine(): a1="<<a1<<",  a2="<<a2<<"  acrit="<<fabs(a2-a1)<<"\n";
    if (fabs(a2-a1)>slopeTolerance || delta>minPixelPerSample) {
        doublePair* dmid = new doublePair;
        dmid->x=realxmid;
        dmid->f=realfmid;
        a->next=dmid;
        dmid->next=b;
        refine(a, dmid, degree+1);
        refine(dmid, b, degree+1);
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


    //    double xold=-1;
    //    double yold=-1;
    //    double ypeold=-1;
    //    double ymeold=-1;

    //    double x0=transformX(0);
    //    if (parent->getXAxis()->isLogAxis()) x0=transformX(parent->getXAxis()->getMin());
        double y0=transformY(0);
        if (parent->getYAxis()->isLogAxis()) y0=transformY(parent->getYAxis()->getMin());
        bool first=false;
        doublePair* d=data;
        //QPainterPath pa, pfill;
        //QPainterPath pel, pef;
        QPolygonF filledPolygon, linePolygon, errorLineTop, errorLineBottom;
        QList<QPointF> epTop, epBottom;
        double yami=qMin(transformY(parent->getYAxis()->getMin()),transformY(parent->getYAxis()->getMax()));
        double yama=qMax(transformY(parent->getYAxis()->getMin()),transformY(parent->getYAxis()->getMax()));
        double dypix=fabs(yama-yami);
        yami=yami-2*dypix;
        yama=yama+2*dypix;
        while (d!=nullptr) {

            double xv=d->x;
            double yv=d->f;
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=transformX(xv);
                double y=transformY(yv);
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
                    if (!d->next) filledPolygon<<QPointF(x, y0);
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

    //            xold=x;
    //            yold=y;
    //            ypeold=ype;
    //            ymeold=yme;
                first=true;
            }
            d=d->next;
        }
        if (drawErrorPolygons) {
            painter.save(); auto __finalpainterrpoly=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(eb);
            painter.setPen(np);
            QPolygonF poly;
            //poly << QPointF(xold, ypeold) << QPointF(x, ype)<< QPointF(x, yme) << QPointF(xold, ymeold) ;
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


        QColor c=getLineColor();
        c.setHsv(fmod(c.hue()+90, 360), c.saturation(), c.value());
        d=data;
        if (displaySamplePoints) {
            painter.save(); auto __finalpaintsamplepoints=JKQTPFinally([&painter]() {painter.restore();});
            while (d!=nullptr) {
                double xv=d->x;
                double yv=d->f;
                //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                    double x=transformX(xv);
                    double y=transformY(yv);
                    JKQTPPlotSymbol(painter, x, y, JKQTPCross, 6,1*parent->getLineWidthMultiplier(), c, QColor(Qt::transparent));
                }
                d=d->next;
            }

        }
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


        double xold=-1;
        double yold=-1;
        double xpeold=-1;
        double xmeold=-1;

        double x0=transformX(0);
        if (parent->getXAxis()->isLogAxis()) x0=transformX(parent->getXAxis()->getMin());
    //    double y0=transformY(0);
    //    if (parent->getYAxis()->isLogAxis()) y0=transformY(parent->getYAxis()->getMin());
        bool first=false;
        doublePair* d=data;

        while (d!=nullptr) {
            double yv=d->x;
            double xv=d->f;
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=transformX(xv);
                double y=transformY(yv);
                double xpe=0, xme=0;
                if ((drawErrorLines || drawErrorPolygons) && (static_cast<bool>(errorPlotFunction))) {
                    double e=errorPlotFunction(xv, errorParams);
                    xpe=transformX(xv+e);
                    xme=transformX(xv-e);
                }

                if (first) {
                    double xl1=xold;
                    double yl1=yold;
                    double xl2=x;
                    double yl2=y;

                    if (fillCurve) {
                        painter.save(); auto __finalpaintfillcurve=JKQTPFinally([&painter]() {painter.restore();});
                        painter.setBrush(b);
                        painter.setPen(np);
                        QPolygonF poly;
                        poly << QPointF(xl1, yl1) << QPointF(xl2, yl2) << QPointF(x0, yl2) << QPointF(x0, yl1);
                        painter.drawConvexPolygon(poly);

                        /*pfill.lineTo(x, y);
                        if (d->next==nullptr) { // last datapoint
                            pfill.lineTo(x, y0);
                        }*/
                    }

                    if (drawErrorPolygons && (static_cast<bool>(errorPlotFunction))) {
                        painter.save(); auto __finalpainterrorpoly=JKQTPFinally([&painter]() {painter.restore();});
                        painter.setBrush(eb);
                        painter.setPen(np);
                        QPolygonF poly;
                        poly << QPointF(xpeold, yold) << QPointF(xpe, y)<< QPointF(xme, y) << QPointF(xmeold, yold) ;
                        painter.drawConvexPolygon(poly);

                    }

                    if (drawLine) {
                        painter.setPen(p);
                        //pa.lineTo(x, y);
                        painter.drawLine(QLineF(xl1, yl1, xl2, yl2));
                    }

                    if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
                        painter.setPen(ep);
                        painter.drawLine(QLineF(xpeold, yold, xpe, y));
                        painter.drawLine(QLineF(xmeold, yold, xme, y));
                    }

                    //std::cout<<"line ("<<xl1<<", "<<yl1<<") -- ("<<xl2<<", "<<yl2<<")"<<std::endl;
                } /*else {
                    if (drawLine) {
                        pa.moveTo(x, y);
                    }
                    if (fillCurve) {
                        pfill.moveTo(x, y0);
                        pfill.lineTo(x, y);
                    }
                }*/
                xold=x;
                yold=y;
                xpeold=xpe;
                xmeold=xme;
                first=true;
            }
            d=d->next;
        }
        /*if (fillCurve) {
            pfill.closeSubpath();
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(b);
            painter.setPen(np);
            painter.drawPath(pfill);

        }

        if (drawLine) {
            painter.setPen(p);
            painter.drawPath(pa);

        }*/

        QColor c=getLineColor();
        c.setHsv(fmod(c.hue()+90, 360), c.saturation(), c.value());
        d=data;
        if (displaySamplePoints) while (d!=nullptr) {
            double yv=d->x;
            double xv=d->f;
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=transformX(xv);
                double y=transformY(yv);
                JKQTPPlotSymbol(painter, x, y, JKQTPCross, 6, 1*parent->getLineWidthMultiplier(), c, QColor(Qt::transparent));
            }
            d=d->next;
        }
    }
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}


void JKQTPYFunctionLineGraph::createPlotData(bool collectParams) {
    clearData();
    if (collectParams) collectParameters();

    if (parent==nullptr) return;
    if (!plotFunction && !simplePlotFunction) return;

    jkqtpSimplePlotFunctionType func;
    if (plotFunction) func=std::bind(plotFunction, std::placeholders::_1, params);
    else if (simplePlotFunction) func=simplePlotFunction;

    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    double delta0=(ymax-ymin)/static_cast<double>(minSamples);

    // initially sample function
    doublePair* d=new doublePair;
    d->x=ymin;
    d->f=func(ymin);
    d->next=nullptr;
    data=d;
    for (double y=ymin+delta0; y<ymax; y=y+delta0) {
        d->next = new doublePair;
        d->next->x=y+(static_cast<double>(rand())/static_cast<double>(RAND_MAX)-0.5)*delta0/2.0;
        d->next->f=func(d->next->x);
        d->next->next=nullptr;
        doublePair* dd=d;
        d=d->next;
        refine(dd, d);
    }
    d->next = new doublePair;
    d->next->x=ymax;
    d->next->f=func(ymax);
    d->next->next=nullptr;
    refine(d, d->next);

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

void JKQTPXFunctionLineGraph::setParameterColumn(size_t __value) { this->parameterColumn = static_cast<int>(__value); }

void JKQTPXFunctionLineGraph::setErrorParameterColumn(int __value)
{
    this->errorParameterColumn = __value;
}

int JKQTPXFunctionLineGraph::getErrorParameterColumn() const
{
    return this->errorParameterColumn;
}

void JKQTPXFunctionLineGraph::setErrorParameterColumn(size_t __value) { this->errorParameterColumn = static_cast<int>(__value); }

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

void JKQTPXFunctionLineGraph::setMinSamples(const unsigned int &__value)
{
    this->minSamples = __value;
}

unsigned int JKQTPXFunctionLineGraph::getMinSamples() const
{
    return this->minSamples;
}

void JKQTPXFunctionLineGraph::setMaxRefinementDegree(const unsigned int &__value)
{
    this->maxRefinementDegree = __value;
}

unsigned int JKQTPXFunctionLineGraph::getMaxRefinementDegree() const
{
    return this->maxRefinementDegree;
}

void JKQTPXFunctionLineGraph::setSlopeTolerance(double __value)
{
    this->slopeTolerance = __value;
}

double JKQTPXFunctionLineGraph::getSlopeTolerance() const
{
    return this->slopeTolerance;
}

void JKQTPXFunctionLineGraph::setMinPixelPerSample(double __value)
{
    this->minPixelPerSample = __value;
}

double JKQTPXFunctionLineGraph::getMinPixelPerSample() const
{
    return this->minPixelPerSample;
}

void JKQTPXFunctionLineGraph::setPlotRefinement(bool __value)
{
    this->plotRefinement = __value;
}

bool JKQTPXFunctionLineGraph::getPlotRefinement() const
{
    return this->plotRefinement;
}

void JKQTPXFunctionLineGraph::setDisplaySamplePoints(bool __value)
{
    this->displaySamplePoints = __value;
}

bool JKQTPXFunctionLineGraph::getDisplaySamplePoints() const
{
    return this->displaySamplePoints;
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
    clearData();
}

void JKQTPXFunctionLineGraph::setErrorPlotFunction(jkqtpPlotFunctionType &&__value)
{
    errorSimplePlotFunction=jkqtpSimplePlotFunctionType();
    errorPlotFunction = std::move(__value);
    clearData();
}
jkqtpPlotFunctionType JKQTPXFunctionLineGraph::getErrorPlotFunction() const
{
    return errorPlotFunction;
}

void JKQTPXFunctionLineGraph::setErrorPlotFunction(const jkqtpSimplePlotFunctionType &__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction=__value;
    clearData();
}

void JKQTPXFunctionLineGraph::setErrorPlotFunction(jkqtpSimplePlotFunctionType &&__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction = std::move(__value);
    clearData();
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

