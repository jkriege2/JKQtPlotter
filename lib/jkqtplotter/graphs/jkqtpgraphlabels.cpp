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



#include "jkqtplotter/graphs/jkqtpgraphlabels.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <QDebug>
#include <QDateTime>
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtptools.h"

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;



JKQTPXYGraphLabels::JKQTPXYGraphLabels(LabelContentsDefaultType lt, JKQTPlotter* parent):
    JKQTPXYGraphLabels(lt, parent->getPlotter())
{
}

JKQTPXYGraphLabels::JKQTPXYGraphLabels(const LabelGenerator &lgen, JKQTBasePlotter *parent):
    JKQTPXYGraphLabels(YValueLabel, parent)
{
    setCustomLabelGenerator(lgen);
}

JKQTPXYGraphLabels::JKQTPXYGraphLabels(const LabelGenerator &lgen, JKQTPlotter *parent):
    JKQTPXYGraphLabels(lgen, parent->getPlotter())
{

}

JKQTPXYGraphLabels::JKQTPXYGraphLabels(LabelContentsDefaultType lt, JKQTBasePlotter* parent):
    JKQTPXYGraph(parent), JKQTPGraphValueLabelStyleMixin(parent)
{
    sortData=JKQTPXYGraph::Unsorted;

    initValueLabelStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);

    switch(lt) {
    case XValueLabel:
        setDefaultXLabelGenerator();
        break;
    case YValueLabel:
        setDefaultYLabelGenerator();
        break;
    case XYValueLabel:
        setDefaultXYLabelGenerator();
        break;
    }
}

void JKQTPXYGraphLabels::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPXYGraphLabels::draw");
#endif
    if (parent==nullptr) return;
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        int imax=0;
        int imin=0;
        if (getIndexRange(imin, imax)) {
            for (int iii=imin; iii<imax; iii++) {
                const int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                const double x=transformX(xv);
                const double y=transformY(yv);
                if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv)  &&  JKQTPIsOKFloat(x) && JKQTPIsOKFloat(y)) {
                    const QString label=generateLabel(xv,yv,iii);
                    drawLabel(painter, QPointF(x,y), QPointF(xv,yv), label, parent);
                }
            }

        }
    }
    drawErrorsAfter(painter);
}

void JKQTPXYGraphLabels::drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setFont(QFont(QGuiApplication::font().family(), getTextFontSize()));
    painter.setPen(getTextColor());
    painter.drawText(rect,Qt::AlignHCenter|Qt::AlignVCenter, "12");
}

QColor JKQTPXYGraphLabels::getKeyLabelColor() const {
    return getTextColor();
}

void JKQTPXYGraphLabels::setColor(QColor c, bool setTextColor_, bool setFrameColor_)
{
    if (setTextColor_) setTextColor(c);
    if (setFrameColor_) setLineColor(c);
}

void JKQTPXYGraphLabels::setCustomLabelGenerator(const LabelGenerator &labgen)
{
    m_labelGenerator=labgen;
}

void JKQTPXYGraphLabels::setDefaultXLabelGenerator()
{
    m_labelGenerator=std::bind(std::mem_fn(&JKQTPXYGraphLabels::generateDefaultXLabel), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void JKQTPXYGraphLabels::setDefaultYLabelGenerator()
{
    m_labelGenerator=std::bind(std::mem_fn(&JKQTPXYGraphLabels::generateDefaultYLabel), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void JKQTPXYGraphLabels::setDefaultXYLabelGenerator()
{
    m_labelGenerator=std::bind(std::mem_fn(&JKQTPXYGraphLabels::generateDefaultXYLabel), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}


bool JKQTPXYGraphLabels::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    const bool baseResult=JKQTPXYGraph::getXMinMax(minx,maxx,smallestGreaterZero);
    if (baseResult) {
        /*
        // this does not work, as it depends on the widget size, when called!!!
        qDebug()<<"JKQTPXYGraphLabels::getXMinMax():";
        qDebug()<<"  minx="<<minx<<", maxx="<<maxx;
        const double delta=(isLabelPositioningGrowingInY())? 0.0: parent->pt2px(getLabelOffset()+getLabelXPadding()+getTextFontSize()*2.0,96.0);
        const double mi_x=transformX(minx);
        const double ma_x=transformX(maxx);
        qDebug()<<"  mi_x="<<mi_x<<", ma_x="<<ma_x<<", delta="<<delta;
        const double new_mi_x=backtransformX(qMin(mi_x,ma_x)-delta);
        const double new_ma_x=backtransformX(qMax(mi_x,ma_x)+delta);
        qDebug()<<"  new_mi_x="<<new_mi_x<<", new_ma_x="<<new_ma_x;
        minx=qMin(minx,qMin(new_mi_x,new_ma_x));
        maxx=qMax(maxx,qMax(new_mi_x,new_ma_x));
        qDebug()<<"->minx="<<minx<<", maxx="<<maxx;*/

        // ... so if required, we just extend the range by ~15%
        if (!isLabelPositioningGrowingInX()) {
            const double mi_x=transformY(minx);
            const double ma_x=transformY(maxx);
            const double delta=fabs(ma_x-mi_x)*0.15/2.0;
            const double new_mi=backtransformY(qMax(mi_x,ma_x)+delta);
            const double new_ma=backtransformY(qMin(mi_x,ma_x)-delta);
            minx=qMin(minx,qMin(new_mi,new_ma));
            maxx=qMax(maxx,qMax(new_mi,new_ma));
        }
    }
    return baseResult;
}

bool JKQTPXYGraphLabels::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    const bool baseResult=JKQTPXYGraph::getYMinMax(miny,maxy,smallestGreaterZero);
    if (baseResult) {
        /*
        qDebug()<<"JKQTPXYGraphLabels::getXMinMax():";
        qDebug()<<"  minx="<<miny<<", maxx="<<maxy;
        const double delta=(!isLabelPositioningGrowingInY())?0.0: parent->pt2px(getLabelOffset()+getLabelYPadding()*2.0+getTextFontSize(),96.0);
        const double mi_y=transformY(miny);
        const double ma_y=transformY(maxy);
        qDebug()<<"  mi_x="<<mi_y<<", ma_x="<<ma_y<<", delta="<<delta;
        const double new_mi_y=backtransformY(qMin(mi_y,ma_y)-delta);
        const double new_ma_y=backtransformY(qMax(mi_y,ma_y)+delta);
        qDebug()<<"  new_mi_x="<<new_mi_y<<", new_ma_x="<<new_ma_y;
        miny=qMin(miny,qMin(new_mi_y,new_ma_y));
        maxy=qMax(maxy,qMax(new_mi_y,new_ma_y));
        qDebug()<<"->minx="<<miny<<", maxx="<<maxy;
        */
        // ... so if required, we just extend the range by ~10%
        if (isLabelPositioningGrowingInY()) {
            const double mi_y=transformY(miny);
            const double ma_y=transformY(maxy);
            const double delta=fabs(ma_y-mi_y)*0.15/2.0;
            const double new_mi=backtransformY(qMax(mi_y,ma_y)+delta);
            const double new_ma=backtransformY(qMin(mi_y,ma_y)-delta);
            miny=qMin(miny,qMin(new_mi,new_ma));
            maxy=qMax(maxy,qMax(new_mi,new_ma));
        }

    }
    return baseResult;
}

QString JKQTPXYGraphLabels::generateLabel(double x, double y, int index) const
{
    if (m_labelGenerator) return m_labelGenerator(x,y,index);
    else {
        qWarning()<<"no label generator functor set in JKQTPXYGraphLabels";
        return "";
    }
}


