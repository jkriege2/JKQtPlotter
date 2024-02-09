/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)



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



#include "jkqtplotter/graphs/jkqtpvectorfield.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <QDebug>
#include <QDateTime>
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtmath/jkqtpstatbasics.h"
#include "jkqtplotter/jkqtptools.h"

#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;



JKQTPVectorFieldGraph::JKQTPVectorFieldGraph(JKQTBasePlotter *parent):
    JKQTPXYAndVectorGraph(parent),
    m_vectorLengthMode(AutoscaleLength),
    m_autoscaleLengthFactor(0.8),
    m_lengthScaleFactor(1.0),
    m_anchorPoint(AnchorBottom),
    m_vectorLineWidthMode(DefaultVectorLineWidth),
    m_minLineWidth(0.001)
{
    initDecoratedLineStyle(parent, parentPlotStyle, JKQTPPlotStyleType::Default);
    setTailDecoratorStyle(JKQTPNoDecorator);
    setHeadDecoratorStyle(JKQTPFilledArrow);
    setHeadDecoratorSizeFactor(getHeadDecoratorSizeFactor()/1.6);
    setTailDecoratorSizeFactor(getTailDecoratorSizeFactor()/1.6);
}

JKQTPVectorFieldGraph::JKQTPVectorFieldGraph(JKQTPlotter *parent):
    JKQTPVectorFieldGraph(parent->getPlotter())
{

}

void JKQTPVectorFieldGraph::draw(JKQTPEnhancedPainter &painter)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTPVectorFieldGraph::draw");
#endif
    if (parent==nullptr) return;
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return;

    drawErrorsBefore(painter);
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        const QPen p=getLinePen(painter, parent);
        const double lw=getLineWidth();
        painter.setPen(p);
        painter.setBrush(p.color());

        int imax=0;
        int imin=0;
        double scale=1;
        if (getIndexRange(imin, imax)) {
            double minVecLen=0, maxVecLen=0;
            // first determine (auto-scale) factor
            if (m_vectorLengthMode==AutoscaleLength || m_vectorLengthMode==IgnoreLength) {
                double avgVecLength=0;
                double NDatapoints=0;
                double xmin=0, xmax=0,ymin=0,ymax=0;
                QVector<double> lengths;
                lengths.reserve(imax-imin);
                for (int iii=imin; iii<imax; iii++) {
                    const int i=qBound(imin, getDataIndex(iii), imax);
                    const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                    const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                    const QPointF vecv=getVectorDxDy(i);
                    if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(vecv)) {
                        const double l=sqrt(jkqtp_sqr(vecv.x())+jkqtp_sqr(vecv.y()));
                        lengths<<l;
                        avgVecLength+=l;
                        if (NDatapoints==0) {
                            xmin=xmax=xv;
                            ymin=ymax=yv;
                            minVecLen=maxVecLen=l;
                        } else {
                            xmin=qMin(xmin,xv);
                            xmax=qMax(xmax,xv);
                            ymin=qMin(ymin,yv);
                            ymax=qMax(ymax,yv);
                            minVecLen=qMin(minVecLen,l);
                            maxVecLen=qMax(maxVecLen,l);
                        }
                        NDatapoints++;
                    }
                }
                //avgVecLength/=NDatapoints;
                avgVecLength=jkqtpstatQuantile(lengths.begin(), lengths.end(), 0.9);
                const double plotsize=qMax(fabs(xmax-xmin),fabs(ymax-ymin));
                const double aproxNPerSide=sqrt(NDatapoints);                
                if (m_vectorLengthMode==IgnoreLength) scale=plotsize/aproxNPerSide*m_autoscaleLengthFactor; // we can assume that the elngths of all vectors have been normalized beforehand
                else scale=plotsize/aproxNPerSide/avgVecLength*m_autoscaleLengthFactor;
            } else {
                scale=m_lengthScaleFactor;
            }

            // now draw
            for (int iii=imin; iii<imax; iii++) {
                const int i=qBound(imin, getDataIndex(iii), imax);
                const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                const double x=transformX(xv);
                const double y=transformY(yv);
                const QPointF vec_orig=getVectorDxDy(i);
                const QPointF vecv=[&](QPointF vec) {
                    if (m_vectorLengthMode==IgnoreLength) {
                        const double veclen=sqrt(jkqtp_sqr(vec.x())+jkqtp_sqr(vec.y()));
                        if (qFuzzyIsNull(veclen)) vec=QPointF(0,0);
                        else vec/=veclen; // normalize vector
                    }
                    return vec;
                }(vec_orig);
                const QLineF l=[&]() {
                    switch (m_anchorPoint) {
                    case AnchorBottom: return QLineF(x,y,transformX(xv+scale*vecv.x()),transformY(yv+scale*vecv.y()));
                    case AnchorMid: return QLineF(transformX(xv-0.5*scale*vecv.x()),transformY(yv-0.5*scale*vecv.y()),transformX(xv+0.5*scale*vecv.x()),transformY(yv+0.5*scale*vecv.y()));
                    case AnchorTip: return QLineF(transformX(xv-scale*vecv.x()),transformY(yv-scale*vecv.y()), x,y);
                    }
                    return QLineF(JKQTP_NAN,JKQTP_NAN,JKQTP_NAN,JKQTP_NAN);
                }();
                if (JKQTPIsOKFloat(l) && l.length()>0) {
                    double actualLW=p.widthF();
                    if (m_vectorLineWidthMode==AutoscaleLineWidthFromLength) {
                        const double vec_origlen=sqrt(jkqtp_sqr(vec_orig.x())+jkqtp_sqr(vec_orig.y()));
                        QPen plw=p;
                        plw.setWidthF(m_minLineWidth+(vec_origlen-minVecLen)/(maxVecLen-minVecLen)*(lw-m_minLineWidth));
                        painter.setPen(plw);
                        actualLW=plw.widthF();
                    }

                    JKQTPPlotDecoratedLine(painter,l, getTailDecoratorStyle(), calcTailDecoratorSize(actualLW), getHeadDecoratorStyle(), calcHeadDecoratorSize(actualLW));
                }
            }

        }
    }
    drawErrorsAfter(painter);
}

void JKQTPVectorFieldGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    const QPen p=getKeyLinePen(painter, rect, parent);
    painter.setPen(p);
    painter.setBrush(p.color());
    const QLineF l(rect.left(), rect.bottom(), rect.right(), rect.top());
    JKQTPPlotDecoratedLine(painter,l, getTailDecoratorStyle(), calcTailDecoratorSize(p.widthF()), getHeadDecoratorStyle(), calcHeadDecoratorSize(p.widthF()));
}

QColor JKQTPVectorFieldGraph::getKeyLabelColor() const
{
    return getLineColor();
}

JKQTPVectorFieldGraph::VectorLengthMode JKQTPVectorFieldGraph::getVectorLengthMode() const
{
    return m_vectorLengthMode;
}

void JKQTPVectorFieldGraph::setVectorLengthMode(VectorLengthMode newMode)
{
    m_vectorLengthMode = newMode;
}

double JKQTPVectorFieldGraph::getAutoscaleLengthFactor() const
{
    return m_autoscaleLengthFactor;
}

void JKQTPVectorFieldGraph::setAutoscaleLengthFactor(double newAutoscaleLengthFactor)
{
    m_autoscaleLengthFactor=newAutoscaleLengthFactor;
}

double JKQTPVectorFieldGraph::getLengthScaleFactor() const
{
    return m_lengthScaleFactor;
}

void JKQTPVectorFieldGraph::setLengthScaleFactor(double newLengthScaleFactor)
{
    m_lengthScaleFactor = newLengthScaleFactor;
}

JKQTPVectorFieldGraph::VectorAnchorPoint JKQTPVectorFieldGraph::getAnchorPoint() const
{
    return m_anchorPoint;
}

void JKQTPVectorFieldGraph::setAnchorPoint(VectorAnchorPoint newAnchorPoint)
{
    m_anchorPoint = newAnchorPoint;
}

void JKQTPVectorFieldGraph::setVectorLineWidthMode(VectorLineWidthMode m)
{
    m_vectorLineWidthMode=m;
}

JKQTPVectorFieldGraph::VectorLineWidthMode JKQTPVectorFieldGraph::getVectorLineWidthMode() const
{
    return m_vectorLineWidthMode;
}

void JKQTPVectorFieldGraph::setMinLineWidth(double lw)
{
    m_minLineWidth=lw;
}

double JKQTPVectorFieldGraph::getMinLineWIdth() const
{
    return m_minLineWidth;
}
