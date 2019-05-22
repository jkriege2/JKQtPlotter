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



#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtcommon/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtpgraphsbase.h"


#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPPlotElement::JKQTPPlotElement(JKQTBasePlotter* parent):
    QObject(parent)
{
    title="";
    visible=true;
    highlighted=false;
    parentPlotStyle=-1;
    setParent(parent);
}

JKQTPPlotElement::JKQTPPlotElement(JKQTPlotter *parent):
    JKQTPPlotElement(parent->getPlotter())
{
}

JKQTPGraph::JKQTPGraph(JKQTBasePlotter* parent):
    JKQTPPlotElement(parent)
{
}

JKQTPGraph::JKQTPGraph(JKQTPlotter *parent):
    JKQTPPlotElement(parent)
{

}

QImage JKQTPPlotElement::generateKeyMarker(QSize size)
{
    QImage img(size.width(),size.height(),QImage::Format_ARGB32);
    if (parent) img.fill(Qt::transparent);//->getKeyBackgroundColor());
    {
        JKQTPEnhancedPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
        QRectF rect(0,0,size.width(),size.height());
        drawKeyMarker(painter, rect);
    }
    return img;
}

void JKQTPPlotElement::setTitle(const QString &__value)
{
    this->title = __value;
}

QString JKQTPPlotElement::getTitle() const
{
    return this->title;
}

void JKQTPPlotElement::setVisible(bool __value)
{
    this->visible = __value;
}

bool JKQTPPlotElement::isVisible() const
{
    return this->visible;
}

void JKQTPPlotElement::setHighlighted(bool __value)
{
    highlighted=__value;
}

bool JKQTPPlotElement::isHighlighted() const
{
    return highlighted;
}





void JKQTPPlotElement::setParent(JKQTBasePlotter* parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPPlotElement::setParent(JKQTPlotter *parent)
{
    setParent(parent->getPlotter());
}


bool JKQTPGraph::getDataMinMax(int column, double &minx, double &maxx, double &smallestGreaterZero)
{
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=static_cast<int>(datastore->getRows(column));

    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(column,static_cast<size_t>(i));
        if (start || xv>maxx) maxx=xv;
        if (start || xv<minx) minx=xv;
        double xvsgz;
        xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
        start=false;
    }
    return !start;
}

void JKQTPPlotElement::getOutsideSize(JKQTPEnhancedPainter& /*painter*/, int& leftSpace, int& rightSpace, int& topSpace, int& bottomspace) {
    leftSpace=0;
    rightSpace=0;
    topSpace=0;
    bottomspace=0;
}

void JKQTPPlotElement::drawOutside(JKQTPEnhancedPainter& /*painter*/, QRect /*leftSpace*/, QRect /*rightSpace*/, QRect /*topSpace*/, QRect /*bottomSpace*/) {

}


QString JKQTPPlotElement::formatHitTestDefaultLabel(double x, double y, int index, JKQTPDatastore* datastore) const {
    const JKQTPXGraphErrorData* errgx=dynamic_cast<const JKQTPXGraphErrorData*>(this);
    QString xerrstr;
    // retrieve x-error data
    if (errgx && datastore) {
        if (errgx->getXErrorColumn()>=0) {
            if (errgx->getXErrorColumnLower()>=0) {
                xerrstr=QString("\\:+%1\\:-%2")
                            .arg(QString::fromStdString(jkqtp_floattolatexstr(datastore->get(errgx->getXErrorColumn(),static_cast<size_t>(index)), 3)))
                            .arg(QString::fromStdString(jkqtp_floattolatexstr(datastore->get(errgx->getXErrorColumnLower(),static_cast<size_t>(index)), 3)));
            } else {
                xerrstr=QString("{\\:}{\\pm}%1")
                            .arg(QString::fromStdString(jkqtp_floattolatexstr(datastore->get(errgx->getXErrorColumn(),static_cast<size_t>(index)), 3)));
            }
        }
    }

    // retrieve y-error data
    const JKQTPYGraphErrorData* errgy=dynamic_cast<const JKQTPYGraphErrorData*>(this);
    QString yerrstr;
    if (errgy && datastore) {
        if (errgy->getYErrorColumn()>=0) {
            if (errgy->getYErrorColumnLower()>=0) {
                yerrstr=QString("\\:+%1\\:-%2")
                            .arg(QString::fromStdString(jkqtp_floattolatexstr(datastore->get(errgy->getYErrorColumn(),static_cast<size_t>(index)), 3)))
                            .arg(QString::fromStdString(jkqtp_floattolatexstr(datastore->get(errgy->getYErrorColumnLower(),static_cast<size_t>(index)), 3)));
            } else {
                yerrstr=QString("{\\:}{\\pm}%1")
                            .arg(QString::fromStdString(jkqtp_floattolatexstr(datastore->get(errgy->getYErrorColumn(),static_cast<size_t>(index)), 3)));
            }
        }
    }
    return QString("\\ensuremath{\\left[{\\:}%1%3{\\;},{\\;}%2%4{\\:}\\right]}").arg(QString::fromStdString(jkqtp_floattolatexstr(x, 3))).arg(QString::fromStdString(jkqtp_floattolatexstr(y, 3))).arg(xerrstr).arg(yerrstr);

}

double JKQTPPlotElement::hitTest(const QPointF & posSystem, QPointF* closestSpotSystem, QString* label, HitTestMode mode) const
{
    if (parent==nullptr) return JKQTP_NAN;

    int closest=-1;
    double closedist=JKQTP_NAN;
    double closedistsec=JKQTP_NAN;
    QPointF closestPos;
    QPointF posF=transform(posSystem);
    for (int i=0; i<m_hitTestData.count(); i++) {
        const QPointF x=m_hitTestData[i].pos;
        const QPointF xpix = transform(x);
        if (JKQTPIsOKFloat(xpix.x())&&JKQTPIsOKFloat(xpix.y())) {
            double d=0, dsecondary=0;
            switch (mode) {
                case HitTestXY: d=sqrt(jkqtp_sqr(xpix.x()-posF.x())+jkqtp_sqr(xpix.y()-posF.y())); dsecondary=0; break;
                case HitTestXOnly: d=fabs(xpix.x()-posF.x()); dsecondary=fabs(xpix.y()-posF.y()); break;
                case HitTestYOnly: d=fabs(xpix.y()-posF.y()); dsecondary=fabs(xpix.x()-posF.x()); break;
            }
            if (closest<0 || d<closedist || (jkqtp_approximatelyEqual(d,closedist) && dsecondary<closedistsec)) {
                closest=i;
                closedist=d;
                closedistsec=dsecondary;
                closestPos=x;
            }
        }
    }
    if (closest>=0) {
        if (closestSpotSystem) *closestSpotSystem=closestPos;
        if (label) *label=m_hitTestData[closest].label;
        return closedist;
    } else {
        return JKQTP_NAN;
    }
}

double JKQTPPlotElement::transformX(double x) const {
    return parent->getXAxis()->x2p(x);
}

double JKQTPPlotElement::transformY(double y) const {
    return parent->getYAxis()->x2p(y);
}

double JKQTPPlotElement::backtransformX(double x) const {
    return parent->getXAxis()->p2x(x);
}

double JKQTPPlotElement::backtransformY(double y) const {
    return parent->getYAxis()->p2x(y);
}


bool JKQTPGraph::usesColumn(int /*column*/) const
{
    return false;
}

void JKQTPGraph::drawErrorsBefore(JKQTPEnhancedPainter &)
{

}

void JKQTPGraph::drawErrorsAfter(JKQTPEnhancedPainter &)
{

}

QVector<QPointF> JKQTPPlotElement::transform(const QVector<QPointF> &x) const {
    QVector<QPointF> res;
    for (int i=0; i<x.size(); i++) {
        res.append(transform(x[i]));
    }
    return res;
}

QPainterPath JKQTPPlotElement::transformToLinePath(const QVector<QPointF> &x) const {
    QPainterPath res;
    if (x.size()>0) {
        res.moveTo(transform(x[0]));
        for (int i=1; i<x.size(); i++) {
            res.lineTo(transform(x[i]));
        }
    }
    return res;
}




JKQTPXYGraph::JKQTPXYGraph(JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    xColumn=-1;
    yColumn=-1;


}

JKQTPXYGraph::JKQTPXYGraph(JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    xColumn=-1;
    yColumn=-1;

}

bool JKQTPXYGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr)  return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv)) {
            if (start || xv>maxx) maxx=xv;
            if (start || xv<minx) minx=xv;
            double xvsgz;
            xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPXYGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);


    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(yv)) {
            if (start || yv>maxy) maxy=yv;
            if (start || yv<miny) miny=yv;
            double xvsgz;
            xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPXYGraph::usesColumn(int column) const
{
    return (column==xColumn)||(column==yColumn);
}

void JKQTPXYGraph::setXColumn(int __value)
{
    this->xColumn = __value;
}

int JKQTPXYGraph::getXColumn() const
{
    return this->xColumn;
}

void JKQTPXYGraph::setXColumn(size_t __value) {
    this->xColumn = static_cast<int>(__value);
}

void JKQTPXYGraph::setYColumn(int __value)
{
    this->yColumn = __value;
}

int JKQTPXYGraph::getYColumn() const
{
    return this->yColumn;
}

void JKQTPXYGraph::setYColumn(size_t __value) { this->yColumn = static_cast<int>(__value); }

void JKQTPXYGraph::setDataSortOrder(JKQTPXYGraph::DataSortOrder __value)
{
    this->sortData = __value;
}

JKQTPXYGraph::DataSortOrder JKQTPXYGraph::getDataSortOrder() const
{
    return this->sortData;
}

void JKQTPXYGraph::setDataSortOrder(int __value) {
    sortData=static_cast<DataSortOrder>(__value);
}

void JKQTPXYGraph::setXYColumns(size_t xCol, size_t yCol)
{
    setXColumn(xCol);
    setYColumn(yCol);
}

void JKQTPXYGraph::setXYColumns(int xCol, int yCol)
{
    setXColumn(xCol);
    setYColumn(yCol);
}

void JKQTPXYGraph::setXYColumns(std::pair<int, int> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}

void JKQTPXYGraph::setXYColumns(std::pair<size_t, size_t> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}

void JKQTPXYGraph::setXYColumns(QPair<int, int> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}

void JKQTPXYGraph::setXYColumns(QPair<size_t, size_t> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}


double JKQTPXYGraph::hitTest(const QPointF &posSystem, QPointF *closestSpotSystem, QString *label, HitTestMode mode) const
{
    if (parent==nullptr) return JKQTP_NAN;

    // check base-class implementation and use it, if it returns a vaid value
    const double baseclassResult=JKQTPPlotElement::hitTest(posSystem, closestSpotSystem, label, mode);
    if (JKQTPIsOKFloat(baseclassResult)) return baseclassResult;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    if (!getIndexRange(imin, imax)) return JKQTP_NAN;


    int closest=-1;
    double closedist=JKQTP_NAN;
    double closedistsec=JKQTP_NAN;
    QPointF closestPos;
    QPointF posF=transform(posSystem);
    for (int i=imin; i<imax; i++) {
        const QPointF x(datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i)), datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i)));
        const QPointF xpix = transform(x);
        if (JKQTPIsOKFloat(xpix.x())&&JKQTPIsOKFloat(xpix.y())) {
            double d=0, dsecondary=0;
            switch (mode) {
                case HitTestXY: d=sqrt(jkqtp_sqr(xpix.x()-posF.x())+jkqtp_sqr(xpix.y()-posF.y())); dsecondary=0; break;
                case HitTestXOnly: d=fabs(xpix.x()-posF.x()); dsecondary=fabs(xpix.y()-posF.y()); break;
                case HitTestYOnly: d=fabs(xpix.y()-posF.y()); dsecondary=fabs(xpix.x()-posF.x()); break;
            }
            if (closest<0 || d<closedist || (jkqtp_approximatelyEqual(d,closedist) && dsecondary<closedistsec)) {
                closest=i;
                closedist=d;
                closedistsec=dsecondary;
                closestPos=x;
                //qDebug()<<"hitTest("<<posSystem<<"[="<<posF<<"pix]...): found closest="<<closest<<", closedist="<<closedist<<", closedistsec="<<closedistsec<<", closestPos="<<closestPos;
            }
        }
    }
    if (closest>=0) {
        if (label) *label=formatHitTestDefaultLabel(closestPos.x(), closestPos.y(), closest);
        if (closestSpotSystem) *closestSpotSystem=closestPos;
        return closedist;
    } else {
        return JKQTP_NAN;
    }
}




JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(JKQTBasePlotter *parent):
    JKQTPGraph(parent)
{
    sortData=Unsorted;
    dataColumn=-1;
}


JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(JKQTPlotter *parent):
    JKQTPSingleColumnGraph(parent->getPlotter())
{
}


void JKQTPSingleColumnGraph::setDataColumn(int __value)
{
    this->dataColumn = __value;
}

int JKQTPSingleColumnGraph::getDataColumn() const
{
    return this->dataColumn;
}

void JKQTPSingleColumnGraph::setDataColumn(size_t __value) {
    this->dataColumn = static_cast<int>(__value);
}

void JKQTPSingleColumnGraph::setDataSortOrder(JKQTPSingleColumnGraph::DataSortOrder __value)
{
    this->sortData = __value;
}

JKQTPSingleColumnGraph::DataSortOrder JKQTPSingleColumnGraph::getDataSortOrder() const
{
    return this->sortData;
}

void JKQTPSingleColumnGraph::setDataSortOrder(int __value) {
    sortData=static_cast<DataSortOrder>(__value);
    if (__value>0) sortData=Sorted;
}

void JKQTPSingleColumnGraph::setDataDirection(JKQTPSingleColumnGraph::DataDirection __value)
{
    this->dataDirection = __value;
}

JKQTPSingleColumnGraph::DataDirection JKQTPSingleColumnGraph::getDataDirection() const
{
    return this->dataDirection;
}

bool JKQTPSingleColumnGraph::usesColumn(int c) const
{
    return c==dataColumn;
}


void JKQTPSingleColumnGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    QVector<double> datas;

    if (sortData==JKQTPSingleColumnGraph::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(dataColumn,static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());


    }
}

bool JKQTPSingleColumnGraph::getIndexRange(int &imin, int &imax) const
{
    if (parent==nullptr)  return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    imin=0;
    imax=static_cast<int>(datastore->getRows(static_cast<size_t>(dataColumn)));
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    return imin>=0 && imax>=0;
}















void JKQTPXYGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    QVector<double> datas;

    if (sortData==JKQTPXYLineGraph::SortedX) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());


    } else if (sortData==JKQTPXYLineGraph::SortedY) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());
    }
}

bool JKQTPXYGraph::getIndexRange(int& imin, int& imax) const
{
    if (parent==nullptr)  return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    imin=0;
    imax=static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    return imin>=0 && imax>=0;
}



JKQTPPlotObject::JKQTPPlotObject(JKQTBasePlotter *parent):
    JKQTPPlotElement(parent)
{

}

JKQTPPlotObject::JKQTPPlotObject(JKQTPlotter *parent):
    JKQTPPlotElement(parent)
{

}

JKQTPPlotObject::~JKQTPPlotObject()
= default;
