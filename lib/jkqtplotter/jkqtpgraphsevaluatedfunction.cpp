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



#include "jkqtplotter/jkqtpgraphsevaluatedfunction.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtpgraphsevaluatedfunction.h"





JKQTPxFunctionLineGraph::JKQTPxFunctionLineGraph(JKQtBasePlotter* parent):
    JKQTPgraph(parent)
{
    functionType=SpecialFunction::UserFunction;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
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

    drawErrorPolygons=false;
    drawErrorLines=false;
    errorParams=nullptr;
    errorColor=color.lighter();
    errorFillColor=color.lighter();
    errorStyle=Qt::SolidLine;
    errorLineWidth=1;
    errorFillStyle=Qt::SolidPattern;

    parameterColumn=-1;
    errorParameterColumn=-1;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
        errorColor=color.lighter();
        errorFillColor=color.lighter();
        errorStyle=style;
    }
    fillColor.setAlphaF(0.5);
    errorFillColor.setAlphaF(0.5);
}

JKQTPxFunctionLineGraph::JKQTPxFunctionLineGraph(JKQtPlotter* parent):
    JKQTPgraph(parent)
{
    functionType=SpecialFunction::UserFunction;
    color=QColor("red");
    fillColor=color.lighter();
    style=Qt::SolidLine;
    lineWidth=2;
    fillStyle=Qt::SolidPattern;
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

    drawErrorPolygons=false;
    drawErrorLines=false;
    errorParams=nullptr;
    errorColor=color.lighter();
    errorFillColor=color.lighter();
    errorStyle=Qt::SolidLine;
    errorLineWidth=1;
    errorFillStyle=Qt::SolidPattern;

    parameterColumn=-1;
    errorParameterColumn=-1;


    if (parent) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        //std::cout<<"got style settings from parent: "<<parentPlotStyle<<std::endl;
        color=parent->getPlotStyle(parentPlotStyle).color();
        fillColor=color.lighter();
        style=parent->getPlotStyle(parentPlotStyle).style();
        errorColor=color.lighter();
        errorFillColor=color.lighter();
        errorStyle=style;
    }
    fillColor.setAlphaF(0.5);
    errorFillColor.setAlphaF(0.5);
}


JKQTPxFunctionLineGraph::~JKQTPxFunctionLineGraph() {
    clearData();
}

void JKQTPxFunctionLineGraph::clearData() {
    while (data!=nullptr) {
        doublePair* d=data;
        data=data->next;
        delete d;
    }
    data=nullptr;
}

void JKQTPxFunctionLineGraph::set_plotFunction(const jkqtpPlotFunctionType &__value)
{
    simplePlotFunction=jkqtpSimplePlotFunctionType();
    plotFunction = __value;
    functionType=SpecialFunction::UserFunction;

    clearData();
}

void JKQTPxFunctionLineGraph::set_plotFunction(const jkqtpSimplePlotFunctionType &__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=__value;
    functionType=SpecialFunction::UserFunction;

    clearData();
}

void JKQTPxFunctionLineGraph::set_plotFunction(jkqtpPlotFunctionType &&__value)
{
    simplePlotFunction=jkqtpSimplePlotFunctionType();
    plotFunction = std::move(__value);
    functionType=SpecialFunction::UserFunction;
    clearData();
}

void JKQTPxFunctionLineGraph::set_plotFunction(jkqtpSimplePlotFunctionType &&__value)
{
    plotFunction=jkqtpPlotFunctionType();
    simplePlotFunction=std::move(__value);
    functionType=SpecialFunction::UserFunction;

    clearData();
}

jkqtpPlotFunctionType JKQTPxFunctionLineGraph::get_plotFunction() const
{
    return plotFunction;
}

jkqtpSimplePlotFunctionType JKQTPxFunctionLineGraph::get_simplePlotFunction() const
{
    return simplePlotFunction;
}


void JKQTPxFunctionLineGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save();
    QPen p=painter.pen();
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);
    p.setColor(color);
    p.setStyle(style);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    const double y=rect.top()+rect.height()/2.0;
    painter.setPen(np);
    if (drawLine) painter.setPen(p);
    painter.setBrush(b);
    if (fillCurve) painter.drawRect(rect);
    if (!fillCurve & drawLine) painter.drawLine(QLineF(rect.left(), y, rect.right(), y));
    painter.restore();
}

QColor JKQTPxFunctionLineGraph::getKeyLabelColor() {
    return color;
}

bool JKQTPxFunctionLineGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    smallestGreaterZero=minx=maxx=0; return false;
}

bool JKQTPxFunctionLineGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    smallestGreaterZero=miny=maxy=0; return false;
}

void JKQTPxFunctionLineGraph::createPlotData(bool collectParams) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPxFunctionLineGraph[%1]::createPlotData()").arg(title));
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
    double delta0=(pxmax-pxmin)/(double)minSamples;
    //double logdelta0=(log(xmax)-log(xmin))/(double)minSamples;

    // initially sample function
    doublePair* d=new doublePair;
    d->x=xmin;
    d->f=func(xmin);
    d->next=nullptr;
    data=d;
    /*if (parent && parent->get_xAxis()->isLogAxis()) {
        for (double x=log(xmin)+logdelta0; x<log(xmax); x=x+logdelta0) {
            d->next = new doublePair;
            d->next->x=exp(x+((double)rand()/(double)RAND_MAX-0.5)*delta0/2.0);
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
           d->next->x=parent->p2x(x+((double)rand()/(double)RAND_MAX-0.5)*delta0/2.0);
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

void JKQTPxFunctionLineGraph::collectParameters()
{
    if (parent && parameterColumn>=0) {
        iparams.clear();
        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=datastore->getColumn(parameterColumn).getRows();

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
        JKQTPdatastore* datastore=parent->getDatastore();
        int imin=0;
        int imax=datastore->getColumn(errorParameterColumn).getRows();

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

void JKQTPxFunctionLineGraph::refine(doublePair* a, doublePair* b, unsigned int degree) {
    if (degree>=maxRefinementDegree) return;
    double ax=transformX(a->x);
    double af=transformX(a->f);
    double bx=transformX(b->x);
    double bf=transformX(b->f);

    double delta=bx - ax;
    //double logdelta=log(bx) - log(ax);
    double xmid=ax+(delta)/2.0;
    /*if (parent && parent->get_xAxis()->isLogAxis()) {
        xmid=log(a->x)+(logdelta)/2.0;
        xmid=xmid+((double)rand()/(double)RAND_MAX-0.5)*delta/5.0;
        xmid=exp(xmid);
    } else {*/
        xmid=xmid+((double)rand()/(double)RAND_MAX-0.5)*delta/5.0; // shake by 10%
    //}
    double realxmid=parent->p2x(xmid);
    double realfmid;
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


void JKQTPxFunctionLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPxFunctionLineGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //qDebug()<<"start plot\n";
    createPlotData();
    //qDebug()<<"plot data created\n";

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QPen ep=painter.pen();
    ep.setColor(errorColor);
    ep.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, errorLineWidth*parent->get_lineWidthMultiplier())));
    ep.setStyle(errorStyle);
    ep.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    QBrush eb=painter.brush();
    eb.setColor(errorFillColor);
    eb.setStyle(errorFillStyle);


//    double xold=-1;
//    double yold=-1;
//    double ypeold=-1;
//    double ymeold=-1;

//    double x0=transformX(0);
//    if (parent->get_xAxis()->isLogAxis()) x0=transformX(parent->get_xAxis()->getMin());
    double y0=transformY(0);
    if (parent->get_yAxis()->isLogAxis()) y0=transformY(parent->get_yAxis()->getMin());
    bool first=false;
    doublePair* d=data;
    //QPainterPath pa, pfill;
    //QPainterPath pel, pef;
    QPolygonF filledPolygon, linePolygon, errorLineTop, errorLineBottom;
    QList<QPointF> epTop, epBottom;
    double yami=qMin(transformY(parent->get_yAxis()->getMin()),transformY(parent->get_yAxis()->getMax()));
    double yama=qMax(transformY(parent->get_yAxis()->getMin()),transformY(parent->get_yAxis()->getMax()));
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
        painter.save();
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
        painter.restore();
    }
    if (fillCurve) {
        painter.save();
        painter.setBrush(b);
        painter.setPen(np);
        painter.drawPolygon(filledPolygon, Qt::OddEvenFill);
        painter.restore();
    }
    if (drawLine) {
        painter.save();
        painter.setPen(p);
        painter.drawPolyline(linePolygon);
        painter.restore();
    }

    if (drawErrorLines && (static_cast<bool>(errorPlotFunction))) {
        painter.save();
        painter.setPen(ep);
        painter.drawPolyline(errorLineTop);
        painter.drawPolyline(errorLineBottom);
        painter.restore();

    }


    QColor c=color;
    c.setHsv(fmod(color.hue()+90, 360), color.saturation(), color.value());
    d=data;
    if (displaySamplePoints) {
        painter.save();
        while (d!=nullptr) {
            double xv=d->x;
            double yv=d->f;
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
                double x=transformX(xv);
                double y=transformY(yv);
                JKQTPplotSymbol(painter, x, y, JKQTPcross, 6,1*parent->get_lineWidthMultiplier(), c, QColor(Qt::transparent));
            }
            d=d->next;
        }
        painter.restore();
    }
    painter.restore();
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}















JKQTPyFunctionLineGraph::JKQTPyFunctionLineGraph(JKQtBasePlotter *parent):JKQTPxFunctionLineGraph(parent) {}

JKQTPyFunctionLineGraph::JKQTPyFunctionLineGraph(JKQtPlotter *parent):JKQTPxFunctionLineGraph(parent) {}

void JKQTPyFunctionLineGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPyFunctionLineGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPdatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    //std::cout<<"start plot\n";
    createPlotData();
    //std::cout<<"plot data created\n";

    drawErrorsBefore(painter);

    painter.save();

    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPen np(Qt::NoPen);

    QPen ep=painter.pen();
    ep.setColor(errorColor);
    ep.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, errorLineWidth*parent->get_lineWidthMultiplier())));
    ep.setStyle(errorStyle);
    ep.setJoinStyle(Qt::RoundJoin);

    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(fillStyle);

    QBrush eb=painter.brush();
    eb.setColor(errorFillColor);
    eb.setStyle(errorFillStyle);


    double xold=-1;
    double yold=-1;
    double xpeold=-1;
    double xmeold=-1;

    double x0=transformX(0);
    if (parent->get_xAxis()->isLogAxis()) x0=transformX(parent->get_xAxis()->getMin());
//    double y0=transformY(0);
//    if (parent->get_yAxis()->isLogAxis()) y0=transformY(parent->get_yAxis()->getMin());
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
                    painter.save();
                    painter.setBrush(b);
                    painter.setPen(np);
                    QPolygonF poly;
                    poly << QPointF(xl1, yl1) << QPointF(xl2, yl2) << QPointF(x0, yl2) << QPointF(x0, yl1);
                    painter.drawConvexPolygon(poly);
                    painter.restore();
                    /*pfill.lineTo(x, y);
                    if (d->next==nullptr) { // last datapoint
                        pfill.lineTo(x, y0);
                    }*/
                }

                if (drawErrorPolygons && (static_cast<bool>(errorPlotFunction))) {
                    painter.save();
                    painter.setBrush(eb);
                    painter.setPen(np);
                    QPolygonF poly;
                    poly << QPointF(xpeold, yold) << QPointF(xpe, y)<< QPointF(xme, y) << QPointF(xmeold, yold) ;
                    painter.drawConvexPolygon(poly);
                    painter.restore();
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
        painter.save();
        painter.setBrush(b);
        painter.setPen(np);
        painter.drawPath(pfill);
        painter.restore();
    }

    if (drawLine) {
        painter.setPen(p);
        painter.drawPath(pa);
        painter.restore();
    }*/

    QColor c=color;
    c.setHsv(fmod(color.hue()+90, 360), color.saturation(), color.value());
    d=data;
    if (displaySamplePoints) while (d!=nullptr) {
        double yv=d->x;
        double xv=d->f;
        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)) {
            double x=transformX(xv);
            double y=transformY(yv);
            JKQTPplotSymbol(painter, x, y, JKQTPcross, 6, 1*parent->get_lineWidthMultiplier(), c, QColor(Qt::transparent));
        }
        d=d->next;
    }
    painter.restore();
    drawErrorsAfter(painter);
    //std::cout<<"plot done\n";
}


void JKQTPyFunctionLineGraph::createPlotData(bool collectParams) {
    clearData();
    if (collectParams) collectParameters();

    if (parent==nullptr) return;
    if (!plotFunction && !simplePlotFunction) return;

    jkqtpSimplePlotFunctionType func;
    if (plotFunction) func=std::bind(plotFunction, std::placeholders::_1, params);
    else if (simplePlotFunction) func=simplePlotFunction;

    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    double delta0=(ymax-ymin)/(double)minSamples;

    // initially sample function
    doublePair* d=new doublePair;
    d->x=ymin;
    d->f=func(ymin);
    d->next=nullptr;
    data=d;
    for (double y=ymin+delta0; y<ymax; y=y+delta0) {
        d->next = new doublePair;
        d->next->x=y+((double)rand()/(double)RAND_MAX-0.5)*delta0/2.0;
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



QBrush JKQTPxFunctionLineGraph::getBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(fillColor);
    b.setStyle(fillStyle);
    return b;
}

QPen JKQTPxFunctionLineGraph::getLinePen(JKQTPEnhancedPainter &painter) const {
    QPen p;
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*lineWidth)));
    p.setStyle(style);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}

QBrush JKQTPxFunctionLineGraph::getErrorBrush(JKQTPEnhancedPainter& /*painter*/) const {
    QBrush b;
    b.setColor(errorFillColor);
    b.setStyle(errorFillStyle);
    return b;
}

QPen JKQTPxFunctionLineGraph::getErrorLinePen(JKQTPEnhancedPainter& painter) const {
    QPen p;
    p.setColor(errorColor);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->get_lineWidthMultiplier()*errorLineWidth)));
    p.setStyle(errorStyle);
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);

    return p;
}


void JKQTPxFunctionLineGraph::set_params(const QVector<double> &params)
{
    iparams=params;
    set_params(&iparams);
}

void JKQTPxFunctionLineGraph::set_copiedParams(const double *params, int N)
{
    QVector<double> v;
    for (int i=0; i<N; i++) { v<<params[i]; }
    set_params(v);
}

void JKQTPxFunctionLineGraph::set_paramsV(double p1) {
    QVector<double> p;
    p<<p1;
    set_params(p);
}

void JKQTPxFunctionLineGraph::set_paramsV(double p1, double p2) {
    QVector<double> p;
    p<<p1<<p2;
    set_params(p);
}

void JKQTPxFunctionLineGraph::set_paramsV(double p1, double p2, double p3) {
    QVector<double> p;
    p<<p1<<p2<<p3;
    set_params(p);
}

void JKQTPxFunctionLineGraph::set_paramsV(double p1, double p2, double p3, double p4) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4;
    set_params(p);
}

void JKQTPxFunctionLineGraph::set_paramsV(double p1, double p2, double p3, double p4, double p5) {
    QVector<double> p;
    p<<p1<<p2<<p3<<p4<<p5;
    set_params(p);
}

void JKQTPxFunctionLineGraph::set_errorParams(const QVector<double> &errorParams)
{
    ierrorparams=errorParams;
    set_errorParams(&ierrorparams);
}

void JKQTPxFunctionLineGraph::setSpecialFunction(JKQTPxFunctionLineGraph::SpecialFunction function)
{
    if (function==JKQTPxFunctionLineGraph::Polynomial) {
        set_plotFunction([](double x, void* param) {
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
    else if (function==JKQTPxFunctionLineGraph::Exponential) set_plotFunction([](double x, void* param) {
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
    else if (function==JKQTPxFunctionLineGraph::PowerLaw) set_plotFunction([](double x, void* param) {
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

JKQTPxFunctionLineGraph::SpecialFunction JKQTPxFunctionLineGraph::getFunctionType() const
{
    return functionType;
}

QVector<double> JKQTPxFunctionLineGraph::get_internalParams() const {
    return iparams;
}
QVector<double> JKQTPxFunctionLineGraph::get_internalErrorParams() const {
    return ierrorparams;
}

void JKQTPxFunctionLineGraph::set_errorPlotFunction(const jkqtpPlotFunctionType &__value)
{
    errorSimplePlotFunction=jkqtpSimplePlotFunctionType();
    errorPlotFunction=__value;
    clearData();
}

void JKQTPxFunctionLineGraph::set_errorPlotFunction(jkqtpPlotFunctionType &&__value)
{
    errorSimplePlotFunction=jkqtpSimplePlotFunctionType();
    errorPlotFunction = std::move(__value);
    clearData();
}
jkqtpPlotFunctionType JKQTPxFunctionLineGraph::get_errorPlotFunction() const
{
    return errorPlotFunction;
}

void JKQTPxFunctionLineGraph::set_errorPlotFunction(const jkqtpSimplePlotFunctionType &__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction=__value;
    clearData();
}

void JKQTPxFunctionLineGraph::set_errorPlotFunction(jkqtpSimplePlotFunctionType &&__value)
{
    errorPlotFunction=jkqtpPlotFunctionType();
    errorSimplePlotFunction = std::move(__value);
    clearData();
}
jkqtpSimplePlotFunctionType JKQTPxFunctionLineGraph::get_errorSimplePlotFunction() const
{
    return errorSimplePlotFunction;
}


bool JKQTPxFunctionLineGraph::usesColumn(int c) const
{
    return (c==parameterColumn)||(c==errorParameterColumn);
}

