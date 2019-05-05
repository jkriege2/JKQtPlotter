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



#include "jkqtplotter/jkqtpgraphsboxplot.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;



JKQTPGraphBoxplotStyleMixin::JKQTPGraphBoxplotStyleMixin()
{

    boxWidth=0.4;

    m_whiskerLinePen=QPen(getLineColor(), getLineWidth());
    whiskerLineWidth=getLineWidth();
    m_medianLinePen=QPen(getLineColor(), getLineWidth());
    medianLineWidth=getLineWidth();

}


void JKQTPGraphBoxplotStyleMixin::initBoxplotStyle(JKQTBasePlotter *parent, int &parentPlotStyle)
{
    initFillStyle(parent, parentPlotStyle);
    initLineStyle(parent, parentPlotStyle);
    initSymbolStyle(parent, parentPlotStyle);
    if (parent && parentPlotStyle>=0) { // get style settings from parent object
        parentPlotStyle=parent->getNextStyle();
        m_whiskerLinePen.setColor(parent->getPlotStyle(parentPlotStyle).color());
        m_whiskerLinePen.setStyle(parent->getPlotStyle(parentPlotStyle).style());
        whiskerLineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
        m_medianLinePen.setColor(parent->getPlotStyle(parentPlotStyle).color());
        m_medianLinePen.setStyle(parent->getPlotStyle(parentPlotStyle).style());
        medianLineWidth=parent->getPlotStyle(parentPlotStyle).widthF();
    }

    setWhiskerLineColor(getLineColor());
    setMedianLineColor(getLineColor());
}

void JKQTPGraphBoxplotStyleMixin::setBoxplotColor(QColor c, JKQTBasePlotter *parent)
{
    setLineColor(c);
    setFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
    c.setAlphaF(0.5);
    setHighlightingLineColor(c);
    setSymbolColor(c);
    setSymbolFillColor(JKQTPGetDerivedColor(parent->getCurrentPlotterStyle().graphFillColorDerivationMode, c));
    setWhiskerLineColor(getLineColor());
    setMedianLineColor(getLineColor());
}

void JKQTPGraphBoxplotStyleMixin::setBoxWidth(double __value)
{
    this->boxWidth = __value;
}

double JKQTPGraphBoxplotStyleMixin::getBoxWidth() const
{
    return this->boxWidth;
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineStyle(Qt::PenStyle __value)
{
    this->m_whiskerLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphBoxplotStyleMixin::getWhiskerLineStyle() const
{
    return this->m_whiskerLinePen.style();
}


void JKQTPGraphBoxplotStyleMixin::setWhiskerLineWidth(double __value)
{
    whiskerLineWidth=__value;
}

double JKQTPGraphBoxplotStyleMixin::getWhiskerLineWidth() const
{
    return whiskerLineWidth;
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineColor(QColor __value)
{
    m_whiskerLinePen.setColor(__value);
}

QColor JKQTPGraphBoxplotStyleMixin::getWhiskerLineColor() const
{
    return m_whiskerLinePen.color();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineDashOffset(qreal offset)
{
    m_whiskerLinePen.setDashOffset(offset);
}

qreal JKQTPGraphBoxplotStyleMixin::getWhiskerLineDashOffset() const
{
    return m_whiskerLinePen.dashOffset();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineDashPattern(const QVector<qreal> &pattern)
{
    m_whiskerLinePen.setDashPattern(pattern);
    m_whiskerLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphBoxplotStyleMixin::getWhiskerLineDashPattern() const
{
    return m_whiskerLinePen.dashPattern();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineJoinStyle(Qt::PenJoinStyle style)
{
    m_whiskerLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphBoxplotStyleMixin::getWhiskerLineJoinStyle() const
{
    return m_whiskerLinePen.joinStyle();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineCapStyle(Qt::PenCapStyle style)
{
    m_whiskerLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphBoxplotStyleMixin::getWhiskerLineCapStyle() const
{
    return m_whiskerLinePen.capStyle();
}

void JKQTPGraphBoxplotStyleMixin::setWhiskerLineBrush(const QBrush &style)
{
    m_whiskerLinePen.setBrush(style);
}

QBrush JKQTPGraphBoxplotStyleMixin::getWhiskerLineBrush() const
{
    return m_whiskerLinePen.brush();
}

QPen JKQTPGraphBoxplotStyleMixin::getWhiskerPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_whiskerLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*whiskerLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    return pw;
}





void JKQTPGraphBoxplotStyleMixin::setMedianLineStyle(Qt::PenStyle __value)
{
    this->m_medianLinePen.setStyle(__value);
}

Qt::PenStyle JKQTPGraphBoxplotStyleMixin::getMedianLineStyle() const
{
    return this->m_medianLinePen.style();
}


void JKQTPGraphBoxplotStyleMixin::setMedianLineWidth(double __value)
{
    medianLineWidth=__value;
}

double JKQTPGraphBoxplotStyleMixin::getMedianLineWidth() const
{
    return medianLineWidth;
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineColor(QColor __value)
{
    m_medianLinePen.setColor(__value);
}

QColor JKQTPGraphBoxplotStyleMixin::getMedianLineColor() const
{
    return m_medianLinePen.color();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineDashOffset(qreal offset)
{
    m_medianLinePen.setDashOffset(offset);
}

qreal JKQTPGraphBoxplotStyleMixin::getMedianLineDashOffset() const
{
    return m_medianLinePen.dashOffset();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineDashPattern(const QVector<qreal> &pattern)
{
    m_medianLinePen.setDashPattern(pattern);
    m_medianLinePen.setStyle(Qt::CustomDashLine);
}

QVector<qreal> JKQTPGraphBoxplotStyleMixin::getMedianLineDashPattern() const
{
    return m_medianLinePen.dashPattern();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineJoinStyle(Qt::PenJoinStyle style)
{
    m_medianLinePen.setJoinStyle(style);
}

Qt::PenJoinStyle JKQTPGraphBoxplotStyleMixin::getMedianLineJoinStyle() const
{
    return m_medianLinePen.joinStyle();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineCapStyle(Qt::PenCapStyle style)
{
    m_medianLinePen.setCapStyle(style);
}

Qt::PenCapStyle JKQTPGraphBoxplotStyleMixin::getMedianLineCapStyle() const
{
    return m_medianLinePen.capStyle();
}

void JKQTPGraphBoxplotStyleMixin::setMedianLineBrush(const QBrush &style)
{
    m_medianLinePen.setBrush(style);
}

QBrush JKQTPGraphBoxplotStyleMixin::getMedianLineBrush() const
{
    return m_medianLinePen.brush();
}

QPen JKQTPGraphBoxplotStyleMixin::getMedianPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    QPen pw=m_medianLinePen;
    pw.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter, parent->getLineWidthMultiplier()*medianLineWidth)));
    pw.setJoinStyle(Qt::MiterJoin);
    return pw;
}












JKQTPBoxplotVerticalGraph::JKQTPBoxplotVerticalGraph(JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    posColumn=-1;
    medianColumn=-1;
    meanColumn=-1;
    minColumn=-1;
    maxColumn=-1;
    percentile25Column=-1;
    percentile75Column=-1;

    sortData=Unsorted;

    initBoxplotStyle(parent, parentPlotStyle);
}


JKQTPBoxplotVerticalGraph::JKQTPBoxplotVerticalGraph(JKQTPlotter* parent):
    JKQTPBoxplotVerticalGraph(parent->getPlotter())
{

}


void JKQTPBoxplotVerticalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotVerticalGraph::draw");
#endif
    clearHitTestData();
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;    

    drawErrorsBefore(painter);


    QPen p=getLinePenForRects(painter, parent);
    QPen pw=getWhiskerPen(painter, parent);
    QPen pm=getMedianPen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);

    int imax=datastore->getColumn(posColumn).getRows();
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    reserveHitTestData(imax-imin);


    //bool first=false;

    // 1. step find width of boxplots:
    double boxwidth_real=0;
    bool bwfirst=true;
    intSortData();
    for (int iii=imin+1; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv0=transformX(datastore->get(posColumn,i-1));
        double xv=transformX(datastore->get(posColumn,i));
        if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xv0)) {
            if (bwfirst) {
                boxwidth_real=fabs(xv-xv0);
                bwfirst=false;
            } else {
                if (fabs(xv-xv0)>0) boxwidth_real=qMin(fabs(xv-xv0), boxwidth_real);
            }
        }
    }
    // 2. plot:
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        for (int i=imin; i<imax; i++) {
            const double xv=datastore->get(posColumn,static_cast<size_t>(i));
            const double p25v=datastore->get(percentile25Column,static_cast<size_t>(i));
            const double p75v=datastore->get(percentile75Column,static_cast<size_t>(i));
            const double minv=datastore->get(minColumn,static_cast<size_t>(i));
            const double maxv=datastore->get(maxColumn,static_cast<size_t>(i));
            const double medianv=datastore->get(medianColumn,static_cast<size_t>(i));
            const double meanv=datastore->get(meanColumn,static_cast<size_t>(i));

            QVector<QLineF> lines_p, lines_pw, lines_m;

            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (posColumn>=0 && JKQTPIsOKFloat(xv) ) {

                // collect single-value labels for hitTest()-data at the bottom of this loop!
                QStringList labelValues, labelNames;
                int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
                labelNames<<"pos";
                labelValues<<QString::fromStdString(jkqtp_floattolatexstr(xv, 3));
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(minv, 3)); labMin=labelValues.size()-1; }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p25v, 3)); labQ25=labelValues.size()-1; }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) { labelNames<<"\\median"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(medianv, 3)); labMedian=labelValues.size()-1; }
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(meanv, 3)); labMean=labelValues.size()-1; }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p75v, 3)); labQ75=labelValues.size()-1; }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(maxv, 3)); labMax=labelValues.size()-1; }

                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(p);
                painter.setBrush(b);
                //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
                const double x=transformX(xv);
                const double p25=transformY(p25v);
                const double p75=transformY(p75v);
                const double min=transformY(minv);
                const double max=transformY(maxv);
                const double median=transformY(medianv);
                const double mean=transformY(meanv);

                double xn=x+1;
                if (i+1<imax) xn=transformX(datastore->get(posColumn,static_cast<size_t>(i+1)));
                else if (i-1>=0) xn=transformX(datastore->get(posColumn,static_cast<size_t>(i-1)));
                else xn=x+1;


                double w=((boxwidth_real>0)?boxwidth_real:(fabs(xn-x)))*getBoxWidth();
                double minstop=p25;
                double maxstop=p75;
                if (percentile25Column<0 && medianColumn>=0) minstop=median;
                else if (percentile25Column<0 && meanColumn>=0) minstop=mean;
                else if (percentile25Column<0 && maxColumn>=0) minstop=max;
                if (percentile75Column<0 && medianColumn>=0) maxstop=median;
                else if (percentile75Column<0 && meanColumn>=0) maxstop=mean;
                else if (percentile75Column<0 && minColumn>=0) maxstop=min;
                double xma=x+w/2.0;
                double xmi=x-w/2.0;
                double xma4=x+w/4.0;
                double xmi4=x-w/4.0;

                if (imax<=0) {
                    xma=transformX(xv+getBoxWidth()/2.0);
                    xmi=transformX(xv-getBoxWidth()/2.0);
                    xma4=transformX(xv+getBoxWidth()/4.0);
                    xmi4=transformX(xv-getBoxWidth()/4.0);
                }

                if (minColumn>=0) {
                    lines_pw.append(QLineF(xmi4, min, xma4, min));
                    lines_pw.append(QLineF(x, min, x, minstop));
                }
                if (maxColumn>=0) {
                    lines_pw.append(QLineF(xmi4, max, xma4, max));
                    lines_pw.append(QLineF(x, max, x, maxstop));
                }

                if (percentile25Column>=0 && percentile75Column>=0) painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
                if (medianColumn>=0) lines_m.append(QLineF(xmi+p.widthF()/2.0, median, xma-p.widthF()/2.0, median));
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) {
                    plotStyledSymbol(parent, painter,x,mean);
                }


                painter.setPen(p);
                if (lines_p.size()>0) painter.drawLines(lines_p);
                painter.setPen(pw);
                if (lines_pw.size()>0) painter.drawLines(lines_pw);
                painter.setPen(pm);
                if (lines_m.size()>0) painter.drawLines(lines_m);


                // add hit-test graph points
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(xv, meanv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(xv, medianv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(xv, minv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(xv, maxv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(xv, p25v, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(xv, p75v, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                //first=true;
            }
        }
    }


    drawErrorsAfter(painter);
}


bool JKQTPBoxplotVerticalGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (posColumn<0) return false;
    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(posColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,i);
        if (JKQTPIsOKFloat(xv)) {
            double xn=xv+1;
            if (i+1<imax) xn=datastore->get(posColumn,i+1);
            else if (i-1>=0) xn=datastore->get(posColumn,i-1);
            else xn=xv+1;
            double delta=fabs(xn-xv);
            double w=delta*getBoxWidth();
            double xma=xv+w;
            double xmi=xv-w;
            if (start || xma>maxx) maxx=xma;
            if (start || xmi<minx) minx=xmi;
            double xvsgz;
            xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPBoxplotVerticalGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (percentile25Column<0 && percentile75Column<0 && medianColumn<0 && minColumn<0 && maxColumn<0) return false;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(medianColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double p25=datastore->get(percentile25Column,i);
        double p75=datastore->get(percentile75Column,i);
        double min=datastore->get(minColumn,i);
        double max=datastore->get(maxColumn,i);
        double median=datastore->get(medianColumn,i);
        if (JKQTPIsOKFloat(median)) {
            if (start || median>maxy) maxy=median;
            if (start || median<miny) miny=median;
            if (JKQTPIsOKFloat(p25) &&p25>maxy) maxy=p25;
            if (JKQTPIsOKFloat(p25) &&p25<miny) miny=p25;
            if (JKQTPIsOKFloat(p75) &&p75>maxy) maxy=p75;
            if (JKQTPIsOKFloat(p75) &&p75<miny) miny=p75;
            if (JKQTPIsOKFloat(min) &&min>maxy) maxy=min;
            if (JKQTPIsOKFloat(min) &&min<miny) miny=min;
            if (JKQTPIsOKFloat(max) &&max>maxy) maxy=max;
            if (JKQTPIsOKFloat(max) &&max<miny) miny=max;

            double xvsgz;
            xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPBoxplotVerticalGraph::usesColumn(int c) const
{
    return (c==meanColumn)||(c==posColumn)||(c==medianColumn)||(c==minColumn)||(c==maxColumn)||(c==percentile25Column)||(c==percentile75Column);
}

void JKQTPBoxplotVerticalGraph::setDataSortOrder(JKQTPBoxplotVerticalGraph::DataSortOrder __value)
{
    this->sortData = __value;
}

JKQTPBoxplotVerticalGraph::DataSortOrder JKQTPBoxplotVerticalGraph::getDataSortOrder() const
{
    return this->sortData;
}

void JKQTPBoxplotVerticalGraph::setDataSortOrder(int __value) {
    sortData=(DataSortOrder)__value;
    if (__value>0) sortData=Sorted;
}

void JKQTPBoxplotVerticalGraph::setPositionColumn(int __value)
{
    this->posColumn = __value;
}

int JKQTPBoxplotVerticalGraph::getPositionColumn() const
{
    return this->posColumn;
}

void JKQTPBoxplotVerticalGraph::setPositionColumn(size_t __value) {
    this->posColumn = static_cast<int>(__value);
}

void JKQTPBoxplotVerticalGraph::setMedianColumn(int __value)
{
    this->medianColumn = __value;
}

int JKQTPBoxplotVerticalGraph::getMedianColumn() const
{
    return this->medianColumn;
}

void JKQTPBoxplotVerticalGraph::setMedianColumn(size_t __value) {
    this->medianColumn = static_cast<int>(__value);
}

void JKQTPBoxplotVerticalGraph::setMeanColumn(int __value)
{
    this->meanColumn = __value;
}

int JKQTPBoxplotVerticalGraph::getMeanColumn() const
{
    return this->meanColumn;
}

void JKQTPBoxplotVerticalGraph::setMeanColumn(size_t __value) {
    this->meanColumn = static_cast<int>(__value);
}

void JKQTPBoxplotVerticalGraph::setMinColumn(int __value)
{
    this->minColumn = __value;
}

int JKQTPBoxplotVerticalGraph::getMinColumn() const
{
    return this->minColumn;
}

void JKQTPBoxplotVerticalGraph::setMinColumn(size_t __value) {
    this->minColumn = static_cast<int>(__value);
}

void JKQTPBoxplotVerticalGraph::setMaxColumn(int __value)
{
    this->maxColumn = __value;
}

int JKQTPBoxplotVerticalGraph::getMaxColumn() const
{
    return this->maxColumn;
}

void JKQTPBoxplotVerticalGraph::setMaxColumn(size_t __value) {
    this->maxColumn = static_cast<int>(__value);
}

void JKQTPBoxplotVerticalGraph::setPercentile25Column(int __value)
{
    this->percentile25Column = __value;
}

int JKQTPBoxplotVerticalGraph::getPercentile25Column() const
{
    return this->percentile25Column;
}

void JKQTPBoxplotVerticalGraph::setPercentile25Column(size_t __value) {
    this->percentile25Column = static_cast<int>(__value);
}

void JKQTPBoxplotVerticalGraph::setPercentile75Column(int __value)
{
    this->percentile75Column = __value;
}

int JKQTPBoxplotVerticalGraph::getPercentile75Column() const
{
    return this->percentile75Column;
}

void JKQTPBoxplotVerticalGraph::setPercentile75Column(size_t __value) {
    this->percentile75Column = static_cast<int>(__value);
}


void JKQTPBoxplotVerticalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePenForRects(painter, parent);
    QPen pw=getWhiskerPen(painter, parent);
    QPen pm=getMedianPen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);

    p.setWidthF(qMin(1.0, p.widthF()));
    pw.setWidthF(qMin(1.0, pw.widthF()));
    pm.setWidthF(qMin(1.0, pm.widthF()));

    double x=rect.left()+rect.width()/2.0;
    double xma=x+rect.width()/2.5;
    double xmi=x-rect.width()/2.5;
    double min=rect.bottom();
    double max=rect.top();
    double median=max+rect.height()/2.0;
    double w=rect.width()/1.8;
    double p25=max+0.75*rect.height();
    double p75=max+0.25*rect.height();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
    }
    
    painter.setPen(pm);
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.setPen(pw);
    painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
    painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
    painter.drawLine(QLineF(x, max, x, p75));
    painter.drawLine(QLineF(x, min, x, p25));
    
}

QColor JKQTPBoxplotVerticalGraph::getKeyLabelColor() const {
    return getLineColor();
}

void JKQTPBoxplotVerticalGraph::setColor(QColor c)
{
    setBoxplotColor(c, getParent());
}




void JKQTPBoxplotHorizontalGraph::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePenForRects(painter, parent);
    QPen pw=getWhiskerPen(painter, parent);
    QPen pm=getMedianPen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    p.setWidthF(qMin(1.0, p.widthF()));
    pw.setWidthF(qMin(1.0, pw.widthF()));
    pm.setWidthF(qMin(1.0, pm.widthF()));


    double y=rect.top()+rect.height()/2.0;
    double yma=y+rect.height()/2.5;
    double ymi=y-rect.height()/2.5;
    double min=rect.left();
    double max=rect.right();
    double median=max-rect.width()/2.0;
    double w=rect.height()/1.8;
    double p25=min+0.75*rect.width();
    double p75=min+0.25*rect.width();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(p75, ymi, fabs(p75-p25), fabs(yma-ymi)));
    }
    
    painter.setPen(pm);
    painter.drawLine(QLineF(median, ymi, median, yma));
    painter.setPen(pw);
    painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
    painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
    painter.drawLine(QLineF(max, y, p75, y));
    painter.drawLine(QLineF(min, y, p25, y));
    
}

bool JKQTPBoxplotHorizontalGraph::getXMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (percentile25Column<0 && percentile75Column<0 && medianColumn<0 && minColumn<0 && maxColumn<0) return false;

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(medianColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double p25=datastore->get(percentile25Column,i);
        double p75=datastore->get(percentile75Column,i);
        double min=datastore->get(minColumn,i);
        double max=datastore->get(maxColumn,i);
        double median=datastore->get(medianColumn,i);
        if (JKQTPIsOKFloat(median)) {
            if (start || median>maxy) maxy=median;
            if (start || median<miny) miny=median;
            if (JKQTPIsOKFloat(p25)&&p25>maxy) maxy=p25;
            if (JKQTPIsOKFloat(p25)&&p25<miny) miny=p25;
            if (JKQTPIsOKFloat(p75)&&p75>maxy) maxy=p75;
            if (JKQTPIsOKFloat(p75)&&p75<miny) miny=p75;
            if (JKQTPIsOKFloat(min)&&min>maxy) maxy=min;
            if (JKQTPIsOKFloat(min)&&min<miny) miny=min;
            if (JKQTPIsOKFloat(max)&&max>maxy) maxy=max;
            if (JKQTPIsOKFloat(max)&&max<miny) miny=max;
            double xvsgz;
            xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPBoxplotHorizontalGraph::getYMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (posColumn<0) return false;
    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(posColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(posColumn,i);
        double xn=xv+1;
        if (i+1<imax) xn=datastore->get(posColumn,i+1);
        else if (i-1>=0) xn=datastore->get(posColumn,i-1);
        else xn=xv+1;
        double delta=fabs(xn-xv);
        double w=delta*getBoxWidth();
        double xma=xv+w;
        double xmi=xv-w;
        if (JKQTPIsOKFloat(xma) && JKQTPIsOKFloat(xmi) ) {
            if (start || xma>maxx) maxx=xma;
            if (start || xmi<minx) minx=xmi;
            double xvsgz;
            xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
            xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

JKQTPBoxplotHorizontalGraph::JKQTPBoxplotHorizontalGraph(JKQTBasePlotter *parent):
    JKQTPBoxplotVerticalGraph(parent)
{
}

JKQTPBoxplotHorizontalGraph::JKQTPBoxplotHorizontalGraph(JKQTPlotter *parent):
    JKQTPBoxplotHorizontalGraph(parent->getPlotter())
{
}

void JKQTPBoxplotHorizontalGraph::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotHorizontalGraph::draw");
#endif
    if (parent==nullptr) return;
    JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);

    QPen p=getLinePenForRects(painter, parent);
    QPen pw=getWhiskerPen(painter, parent);
    QPen pm=getMedianPen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);


    int imax=datastore->getColumn(posColumn).getRows();
    int imin=0;
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    // 1. step find width of boxplots:
    double boxwidth_real=0;
    bool bwfirst=true;
    intSortData();
    for (int iii=imin+1; iii<imax; iii++) {
        int i=qBound(imin, getDataIndex(iii), imax);
        double xv0=transformY(datastore->get(posColumn,i-1));
        double xv=transformY(datastore->get(posColumn,i));
        if (posColumn>=0 && JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(xv0)) {
            if (bwfirst) {
                boxwidth_real=fabs(xv-xv0);
                bwfirst=false;
            } else {
                if (fabs(xv-xv0)>0) boxwidth_real=qMin(fabs(xv-xv0), boxwidth_real);
            }
        }
    }
    // 2. plot:
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        //bool first=false;
        for (int i=imin; i<imax; i++) {
            const double yv=datastore->get(posColumn,static_cast<size_t>(i));
            const double p25v=datastore->get(percentile25Column,static_cast<size_t>(i));
            const double p75v=datastore->get(percentile75Column,static_cast<size_t>(i));
            const double minv=datastore->get(minColumn,static_cast<size_t>(i));
            const double maxv=datastore->get(maxColumn,static_cast<size_t>(i));
            const double medianv=datastore->get(medianColumn,static_cast<size_t>(i));
            const double meanv=datastore->get(meanColumn,static_cast<size_t>(i));

            QVector<QLineF> lines_p, lines_pw, lines_m;
            //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
            if (posColumn>=0 && JKQTPIsOKFloat(yv) ) {


                // collect single-value labels for hitTest()-data at the bottom of this loop!
                QStringList labelValues, labelNames;
                int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
                labelNames<<"pos";
                labelValues<<QString::fromStdString(jkqtp_floattolatexstr(yv, 3));
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(minv, 3)); labMin=labelValues.size()-1; }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p25v, 3)); labQ25=labelValues.size()-1; }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) { labelNames<<"\\median"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(medianv, 3)); labMedian=labelValues.size()-1; }
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(meanv, 3)); labMean=labelValues.size()-1; }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p75v, 3)); labQ75=labelValues.size()-1; }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(maxv, 3)); labMax=labelValues.size()-1; }


                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setPen(p);
                painter.setBrush(b);


                const double y=transformY(yv);
                const double p25=transformX(p25v);
                const double p75=transformX(p75v);
                const double min=transformX(minv);
                const double max=transformX(maxv);
                const double median=transformX(medianv);
                const double mean=transformX(meanv);


                //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
                double minstop=p25;
                double maxstop=p75;
                if (percentile25Column<0 && medianColumn>=0) minstop=median;
                else if (percentile25Column<0 && maxColumn>=0) minstop=max;
                else if (percentile25Column<0 && meanColumn>=0) minstop=mean;
                if (percentile75Column<0 && medianColumn>=0) maxstop=median;
                else if (percentile75Column<0 && minColumn>=0) maxstop=min;
                else if (percentile75Column<0 && meanColumn>=0) maxstop=mean;

                double yn=y+1;
                if (i+1<imax) yn=transformY(datastore->get(posColumn,static_cast<size_t>(i+1)));
                else if (i-1>=0) yn=transformY(datastore->get(posColumn,static_cast<size_t>(i-1)));
                else yn=y+1;
                double delta=fabs(yn-y);
                double w=((boxwidth_real>0)?boxwidth_real:(delta))*getBoxWidth();
                double yma=y-w/2.0;
                double ymi=y+w/2.0;
                double yma4=y+w/4.0;
                double ymi4=y-w/4.0;

                if (imax<=1) {
                    ymi=transformY(yv+getBoxWidth()/2.0);
                    yma=transformY(yv-getBoxWidth()/2.0);
                    yma4=transformY(yv+getBoxWidth()/4.0);
                    ymi4=transformY(yv-getBoxWidth()/4.0);
                }
                if (minColumn>=0) {
                    lines_pw.append(QLineF(min, ymi4, min, yma4));
                    lines_pw.append(QLineF(min, y, minstop, y));
                }
                if (maxColumn>=0) {
                    lines_pw.append(QLineF(max, ymi4, max, yma4));
                    lines_pw.append(QLineF(max, y, maxstop, y));
                }
                if (percentile25Column>=0 && percentile75Column>=0) painter.drawRect(QRectF(p25, qMin(yma,ymi), fabs(p75-p25), fabs(yma-ymi)));
                if (medianColumn>=0) lines_m.append(QLineF(median, ymi-p.widthF()/2.0, median, yma+p.widthF()/2.0));

                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) {
                    plotStyledSymbol(parent, painter, mean, y);
                }

                //first=true;
                painter.setPen(p);
                if (lines_p.size()>0) painter.drawLines(lines_p);
                painter.setPen(pw);
                if (lines_pw.size()>0) painter.drawLines(lines_pw);
                painter.setPen(pm);
                if (lines_m.size()>0) painter.drawLines(lines_m);


                // add hit-test graph points
                if (meanColumn>=0 && JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(meanv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (medianColumn>=0 && JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(medianv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (minColumn>=0 && JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(minv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (maxColumn>=0 && JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(maxv, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile25Column>=0 && JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(p25v, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (percentile75Column>=0 && JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(p75v, yv, i, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }

        }

    }

    drawErrorsAfter(painter);
}





















JKQTPBoxplotVerticalElement::JKQTPBoxplotVerticalElement(JKQTBasePlotter* parent):
    JKQTPPlotObject(parent)
{
    pos=0;
    median=0;
    mean=0;
    min=-1;
    max=1;
    drawMean=true;
    drawMinMax=true;
    percentile25=-0.75;
    percentile75=0.75;

    initBoxplotStyle(parent, parentPlotStyle);

}

JKQTPBoxplotVerticalElement::JKQTPBoxplotVerticalElement(JKQTPlotter* parent):
    JKQTPBoxplotVerticalElement(parent->getPlotter())
{
}

void JKQTPBoxplotVerticalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotVerticalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePenForRects(painter, parent);
        QPen pw=getWhiskerPen(painter, parent);
        QPen pm=getMedianPen(painter, parent);
        QPen np(Qt::NoPen);
        QBrush b=getFillBrush(painter, parent);



        const double xv=pos;
        const double p25v=percentile25;
        const double p75v=percentile75;
        const double minv=min;
        const double maxv=max;
        const double medianv=median;
        const double meanv=mean;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(xv) ) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            const double x=transformX(xv);
            const double p25=transformY(p25v);
            const double p75=transformY(p75v);
            const double min=transformY(minv);
            const double max=transformY(maxv);
            const double median=transformY(medianv);

            // collect single-value labels for hitTest()-data at the bottom of this loop!
            QStringList labelValues, labelNames;
            int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
            labelNames<<"pos";
            labelValues<<QString::fromStdString(jkqtp_floattolatexstr(xv, 3));
            if (JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(minv, 3)); labMin=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p25v, 3)); labQ25=labelValues.size()-1; }
            if (JKQTPIsOKFloat(medianv)) { labelNames<<"\\median"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(medianv, 3)); labMedian=labelValues.size()-1; }
            if (JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(meanv, 3)); labMean=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p75v, 3)); labQ75=labelValues.size()-1; }
            if (JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(maxv, 3)); labMax=labelValues.size()-1; }

            double w=parent->pt2px(painter, getBoxWidth());
            double xma=x+w/2.0;
            double xmi=x-w/2.0;

            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            if (JKQTPIsOKFloat(p25v) && JKQTPIsOKFloat(p75v)) {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setBrush(b);
                painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
                if (JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(xv, p25v, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(xv, p75v, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        
            if (drawMedian && JKQTPIsOKFloat(medianv)) {
                painter.setPen(pm);
                painter.drawLine(QLineF(xmi, median, xma, median));
                if (JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(xv, medianv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
            if (drawMinMax) {
                painter.setPen(pw);
                if (JKQTPIsOKFloat(maxv)) painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
                if (JKQTPIsOKFloat(minv)) painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
                if (JKQTPIsOKFloat(maxv)) painter.drawLine(QLineF(x, max, x, p75));
                if (JKQTPIsOKFloat(minv)) painter.drawLine(QLineF(x, min, x, p25));
                if (JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(xv, minv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(xv, maxv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        

            if (drawMean && JKQTPIsOKFloat(meanv)) {
                double mean=transformY(meanv);
                plotStyledSymbol(parent, painter, x, mean);
                if (JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(xv, meanv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }


        }
    }

    //drawErrorsAfter(painter);
}


bool JKQTPBoxplotVerticalElement::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    double xv=pos;
    double w=0;
    double xma=xv+w;
    double xmi=xv-w;
    maxx=xma;
    minx=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPBoxplotVerticalElement::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false ;


        double p25=percentile25;
        double p75=percentile75;
        if (drawMedian) {
            maxy=median;
            miny=median;
        } else {
            maxy=p75;
            miny=p25;
        }
        if (p25>maxy) maxy=p25;
        if (p25<miny) miny=p25;
        if (p75>maxy) maxy=p75;
        if (p75<miny) miny=p75;
        if (min>maxy) maxy=min;
        if (drawMinMax && (min<miny)) miny=min;
        if (drawMinMax && (max>maxy)) maxy=max;
        if (drawMinMax && (max<miny)) miny=max;

        double xvsgz;
        xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
        xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
        return true;
}

void JKQTPBoxplotVerticalElement::setPos(double __value)
{
    this->pos = __value;
}

double JKQTPBoxplotVerticalElement::getPos() const
{
    return this->pos;
}

void JKQTPBoxplotVerticalElement::setMedian(double __value)
{
    if (this->median != __value) {
        this->median = __value;
        drawMedian=true;
    }
}

double JKQTPBoxplotVerticalElement::getMedian() const
{
    return this->median;
}

void JKQTPBoxplotVerticalElement::setMean(double __value)
{
    if (this->mean != __value) {
        this->mean = __value;
        drawMean=true;
    }
}

double JKQTPBoxplotVerticalElement::getMean() const
{
    return this->mean;
}

void JKQTPBoxplotVerticalElement::setMin(double __value)
{
    if (this->min != __value) {
        this->min = __value;
        drawMinMax=true;
    }
}

double JKQTPBoxplotVerticalElement::getMin() const
{
    return this->min;
}

void JKQTPBoxplotVerticalElement::setMax(double __value)
{
    if (this->max != __value) {
        this->max = __value;
        drawMinMax=true;
    }
}

double JKQTPBoxplotVerticalElement::getMax() const
{
    return this->max;
}

void JKQTPBoxplotVerticalElement::setPercentile25(double __value)
{
    this->percentile25 = __value;
}

double JKQTPBoxplotVerticalElement::getPercentile25() const
{
    return this->percentile25;
}

void JKQTPBoxplotVerticalElement::setPercentile75(double __value)
{
    this->percentile75 = __value;
}

double JKQTPBoxplotVerticalElement::getPercentile75() const
{
    return this->percentile75;
}

void JKQTPBoxplotVerticalElement::setDrawMean(bool __value)
{
    this->drawMean = __value;
}

bool JKQTPBoxplotVerticalElement::getDrawMean() const
{
    return this->drawMean;
}

void JKQTPBoxplotVerticalElement::setDrawMedian(bool __value)
{
    this->drawMedian = __value;
}

bool JKQTPBoxplotVerticalElement::getDrawMedian() const
{
    return this->drawMedian;
}

void JKQTPBoxplotVerticalElement::setDrawMinMax(bool __value)
{
    this->drawMinMax = __value;
}

bool JKQTPBoxplotVerticalElement::getDrawMinMax() const
{
    return this->drawMinMax;
}


void JKQTPBoxplotVerticalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePenForRects(painter, parent);
    QPen pw=getWhiskerPen(painter, parent);
    QPen pm=getMedianPen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    p.setWidthF(qMin(1.0, p.widthF()));
    pw.setWidthF(qMin(1.0, pw.widthF()));
    pm.setWidthF(qMin(1.0, pm.widthF()));


    double x=rect.left()+rect.width()/2.0;
    double xma=x+rect.width()/2.5;
    double xmi=x-rect.width()/2.5;
    double min=rect.bottom();
    double max=rect.top();
    double median=max+rect.height()/2.0;
    double w=rect.width()/1.8;
    double p25=max+0.75*rect.height();
    double p75=max+0.25*rect.height();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(xmi, p75, fabs(xma-xmi), fabs(p75-p25)));
    }
    
    painter.setPen(pm);
    painter.drawLine(QLineF(xmi, median, xma, median));
    painter.setPen(pw);
    painter.drawLine(QLineF(x-w/4.0, max, x+w/4.0, max));
    painter.drawLine(QLineF(x-w/4.0, min, x+w/4.0, min));
    painter.drawLine(QLineF(x, max, x, p75));
    painter.drawLine(QLineF(x, min, x, p25));
    
}

QColor JKQTPBoxplotVerticalElement::getKeyLabelColor() const {
    return getLineColor();
}




void JKQTPBoxplotHorizontalElement::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePenForRects(painter, parent);
    QPen pw=getWhiskerPen(painter, parent);
    QPen pm=getMedianPen(painter, parent);
    QPen np(Qt::NoPen);
    QBrush b=getFillBrush(painter, parent);
    p.setWidthF(qMin(1.0, p.widthF()));
    pw.setWidthF(qMin(1.0, pw.widthF()));
    pm.setWidthF(qMin(1.0, pm.widthF()));

    double y=rect.top()+rect.height()/2.0;
    double yma=y+rect.height()/2.5;
    double ymi=y-rect.height()/2.5;
    double min=rect.left();
    double max=rect.right();
    double median=max-rect.width()/2.0;
    double w=rect.height()/1.8;
    double p25=min+0.75*rect.width();
    double p75=min+0.25*rect.width();

    painter.setPen(p);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.drawRect(QRectF(p75, ymi, fabs(p75-p25), fabs(yma-ymi)));
    }
    
    painter.setPen(pm);
    painter.drawLine(QLineF(median, ymi, median, yma));
    painter.setPen(pw);
    painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
    painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
    painter.drawLine(QLineF(max, y, p75, y));
    painter.drawLine(QLineF(min, y, p25, y));
    
}

bool JKQTPBoxplotHorizontalElement::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;


    double p25=percentile25;
    double p75=percentile75;
    if (drawMedian) {
        maxx=median;
        minx=median;
    } else {
        maxx=p75;
        minx=p25;
    }
    if (p25>maxx) maxx=p25;
    if (p25<minx) minx=p25;
    if (p75>maxx) maxx=p75;
    if (p75<minx) minx=p75;
    if (min>maxx) maxx=min;
    if (drawMinMax && (min<minx)) minx=min;
    if (drawMinMax && (max>maxx)) maxx=max;
    if (drawMinMax && (max<minx)) minx=max;

    double xvsgz;
    xvsgz=median; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=p25; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=p75; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=min; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=max; SmallestGreaterZeroCompare_xvsgz();
    return true;
}

bool JKQTPBoxplotHorizontalElement::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    double xv=pos;
    double w=0;
    double xma=xv+w;
    double xmi=xv-w;
    maxy=xma;
    miny=xmi;
    double xvsgz;
    xvsgz=xmi; SmallestGreaterZeroCompare_xvsgz();
    xvsgz=xma; SmallestGreaterZeroCompare_xvsgz();
    return true;

}

JKQTPBoxplotHorizontalElement::JKQTPBoxplotHorizontalElement(JKQTBasePlotter *parent):
    JKQTPBoxplotVerticalElement(parent)
{
}

JKQTPBoxplotHorizontalElement::JKQTPBoxplotHorizontalElement(JKQTPlotter *parent):
    JKQTPBoxplotHorizontalElement(parent->getPlotter())
{
}

void JKQTPBoxplotHorizontalElement::draw(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPBoxplotHorizontalElement::draw");
#endif
    if (parent==nullptr) return;

    //drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        QPen p=getLinePenForRects(painter, parent);
        QPen pw=getWhiskerPen(painter, parent);
        QPen pm=getMedianPen(painter, parent);
        QPen np(Qt::NoPen);
        QBrush b=getFillBrush(painter, parent);
        const double yv=pos;
        const double p25v=percentile25;
        const double p75v=percentile75;
        const double minv=min;
        const double maxv=max;
        const double medianv=median;
        const double meanv=mean;

        //std::cout<<"(xv, yv) =    ( "<<xv<<", "<<yv<<" )\n";
        if (JKQTPIsOKFloat(yv) ) {

            //std::cout<<"boxplot(med="<<medianv<<", min="<<minv<<", max="<<maxv<<", p25="<<p25v<<", p75="<<p75v<<")\n";
            const double y=transformY(yv);
            const double p25=transformX(p25v);
            const double p75=transformX(p75v);
            const double min=transformX(minv);
            const double max=transformX(maxv);
            const double median=transformX(medianv);

            // collect single-value labels for hitTest()-data at the bottom of this loop!
            QStringList labelValues, labelNames;
            int labMedian=-1, labMean=-1, labMin=-1, labMax=-1, labQ25=-1, labQ75=-1;
            labelNames<<"pos";
            labelValues<<QString::fromStdString(jkqtp_floattolatexstr(yv, 3));
            if (JKQTPIsOKFloat(minv)) { labelNames<<"\\min"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(minv, 3)); labMin=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p25v)) { labelNames<<"q_{25}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p25v, 3)); labQ25=labelValues.size()-1; }
            if (JKQTPIsOKFloat(medianv)) { labelNames<<"\\median"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(medianv, 3)); labMedian=labelValues.size()-1; }
            if (JKQTPIsOKFloat(meanv)) { labelNames<<"\\mu"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(meanv, 3)); labMean=labelValues.size()-1; }
            if (JKQTPIsOKFloat(p75v)) { labelNames<<"q_{75}"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(p75v, 3)); labQ75=labelValues.size()-1; }
            if (JKQTPIsOKFloat(maxv)) { labelNames<<"\\max"; labelValues<<QString::fromStdString(jkqtp_floattolatexstr(maxv, 3)); labMax=labelValues.size()-1; }

            double w= parent->pt2px(painter, getBoxWidth());
            double yma=y+w/2.0;
            double ymi=y-w/2.0;

            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(p);
            if (JKQTPIsOKFloat(p25v) && JKQTPIsOKFloat(p75v)) {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                painter.setBrush(b);
                painter.drawRect(QRectF(p25, ymi, fabs(p75-p25), fabs(yma-ymi)));
                if (JKQTPIsOKFloat(p25v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ25]="\\ul{"+sl[labQ25]+"}";
                    sll[labQ25]="\\ul{"+sll[labQ25]+"}";
                    addHitTestData(p25v, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(p75v)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labQ75]="\\ul{"+sl[labQ75]+"}";
                    sll[labQ75]="\\ul{"+sll[labQ75]+"}";
                    addHitTestData(p75v, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        
            if (drawMedian && JKQTPIsOKFloat(medianv)) {
                painter.setPen(pm);
                painter.drawLine(QLineF(median, ymi, median, yma));
                if (JKQTPIsOKFloat(medianv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMedian]="\\ul{"+sl[labMedian]+"}";
                    sll[labMedian]="\\ul{"+sll[labMedian]+"}";
                    addHitTestData(medianv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }            }
            if (drawMinMax) {
                painter.setPen(pw);
                if (JKQTPIsOKFloat(maxv)) painter.drawLine(QLineF(max, y-w/4.0, max, y+w/4.0));
                if (JKQTPIsOKFloat(minv)) painter.drawLine(QLineF(min, y-w/4.0, min, y+w/4.0));
                if (JKQTPIsOKFloat(maxv)) painter.drawLine(QLineF(max, y, p75, y));
                if (JKQTPIsOKFloat(minv)) painter.drawLine(QLineF(min, y, p25, y));
                if (JKQTPIsOKFloat(minv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMin]="\\ul{"+sl[labMin]+"}";
                    sll[labMin]="\\ul{"+sll[labMin]+"}";
                    addHitTestData(minv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
                if (JKQTPIsOKFloat(maxv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMax]="\\ul{"+sl[labMax]+"}";
                    sll[labMax]="\\ul{"+sll[labMax]+"}";
                    addHitTestData(maxv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }
        

            if (drawMean && JKQTPIsOKFloat(meanv)) {
                double mean=transformY(meanv);
                plotStyledSymbol(parent, painter, mean, y);
                if (JKQTPIsOKFloat(meanv)) {
                    QStringList sl=labelValues, sll=labelNames;
                    sl[labMean]="\\ul{"+sl[labMean]+"}";
                    sll[labMean]="\\ul{"+sll[labMean]+"}";
                    addHitTestData(meanv, yv, "\\ensuremath{\\begin{bmatrix}"+sll.join("\\\\")+"\\end{bmatrix}\\;=\\;\\begin{bmatrix}"+sl.join("\\\\")+"\\end{bmatrix}}");
                }
            }

        }
    }

    //drawErrorsAfter(painter);
}





void JKQTPBoxplotVerticalGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=datastore->getColumn(posColumn).getRows();
    if (imax<imin) {
        int h=imin;
        imin=imax;
        imax=h;
    }
    if (imin<0) imin=0;
    if (imax<0) imax=0;

    QVector<double> datas;

    if (sortData==JKQTPBoxplotVerticalGraph::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(posColumn,i);
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpSort(datas.data(), sortedIndices.data(), datas.size());


    }
}
