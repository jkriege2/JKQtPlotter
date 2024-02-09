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
    m_minLineWidth(0.001),
    m_minVecLen(0),
    m_maxVecLen(0)
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
            m_minVecLen=m_maxVecLen=0;
            // first determine (auto-scale) factor
            if (m_vectorLengthMode==AutoscaleLength || m_vectorLengthMode==IgnoreLength) {
                double avgVecLength=0;
                double NDatapoints=0;
                double xmin=0, xmax=0,ymin=0,ymax=0;
                std::vector<double> lengths;
                lengths.reserve(imax-imin);
                for (int iii=imin; iii<imax; iii++) {
                    const int i=qBound(imin, getDataIndex(iii), imax);
                    const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
                    const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
                    const QPointF vecv=getVectorDxDy(i);
                    if (JKQTPIsOKFloat(xv) && JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(vecv)) {
                        const double l=getVectorMagnitude(vecv);
                        lengths.push_back(l);
                        avgVecLength+=l;
                        if (NDatapoints==0) {
                            xmin=xmax=xv;
                            ymin=ymax=yv;
                            m_minVecLen=m_maxVecLen=l;
                        } else {
                            xmin=qMin(xmin,xv);
                            xmax=qMax(xmax,xv);
                            ymin=qMin(ymin,yv);
                            ymax=qMax(ymax,yv);
                            m_minVecLen=qMin(m_minVecLen,l);
                            m_maxVecLen=qMax(m_maxVecLen,l);
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
                        const double veclen=getVectorMagnitude(vec);
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
                    QPen plw=p;
                    if (m_vectorLineWidthMode==AutoscaleLineWidthFromLength) {
                        const double vec_origlen=getVectorMagnitude(vec_orig);
                        plw.setWidthF(m_minLineWidth+(vec_origlen-m_minVecLen)/(m_maxVecLen-m_minVecLen)*(lw-m_minLineWidth));
                        actualLW=plw.widthF();
                    }
                    plw.setColor(getLocalVectorColor(i,xv,yv,vec_orig.x(),vec_orig.y()));
                    painter.setPen(plw);
                    painter.setBrush(plw.color());
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

QColor JKQTPVectorFieldGraph::getLocalVectorColor(int /*i*/, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/) const
{
    return getLineColor();
}

JKQTPParametrizedVectorFieldGraph::JKQTPParametrizedVectorFieldGraph(JKQTBasePlotter *parent):
    JKQTPVectorFieldGraph(parent),
    JKQTPColorPaletteStyleAndToolsMixin(parent),
    m_colorColumn(-1),
    m_colorColumnContainsRGB(false),
    m_vectorColorMode(ColorFromMagnitude)
{
    palette=JKQTPMathImageMATLAB;
    if (parent) {
        palette=parent->getCurrentPlotterStyle().graphsStyle.defaultPalette;
    }

}

JKQTPParametrizedVectorFieldGraph::JKQTPParametrizedVectorFieldGraph(JKQTPlotter *parent):
    JKQTPParametrizedVectorFieldGraph(parent->getPlotter())
{

}

void JKQTPParametrizedVectorFieldGraph::drawKeyMarker(JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    if (m_vectorColorMode==DefaultColor) {
        JKQTPVectorFieldGraph::drawKeyMarker(painter,rect);
    } else {
        QColor color1=getKeyLabelColor();
        QColor color2=getKeyLabelColor();

        if (m_colorColumn>=0) {
            if (m_colorColumnContainsRGB) {
                color1=QColor("red");
                color2=QColor("blue");
            } else {
                QImage img;
                double colorval[]={0,1};
                JKQTPImageTools::array2image<double>(colorval, 2, 1, img, getColorPalette(), double(0.0), double(1.0));
                color1=img.pixel(0,0);
                color2=img.pixel(1,0);
            }
        }
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        QPen p=getKeyLinePen(painter, rect, parent);
        p.setColor(color1);
        painter.setPen(p);
        painter.setBrush(p.color());
        const QLineF l(rect.left(), rect.bottom(), rect.right(), (rect.top()+rect.bottom())/2.0);
        JKQTPPlotDecoratedLine(painter,l, getTailDecoratorStyle(), calcTailDecoratorSize(p.widthF()), getHeadDecoratorStyle(), calcHeadDecoratorSize(p.widthF()));

        p=getKeyLinePen(painter, rect, parent);
        p.setColor(color2);
        painter.setPen(p);
        painter.setBrush(p.color());
        const QLineF l2(rect.left(), (rect.top()+rect.bottom())/2.0, rect.right(), rect.top());
        JKQTPPlotDecoratedLine(painter,l2, getTailDecoratorStyle(), calcTailDecoratorSize(p.widthF()), getHeadDecoratorStyle(), calcHeadDecoratorSize(p.widthF()));
    }
}

void JKQTPParametrizedVectorFieldGraph::draw(JKQTPEnhancedPainter &painter)
{
    cbGetDataMinMax(m_intColMin, m_intColMax);
    JKQTPVectorFieldGraph::draw(painter);
}

void JKQTPParametrizedVectorFieldGraph::setParent(JKQTBasePlotter *parent)
{
    JKQTPVectorFieldGraph::setParent(parent);
    cbSetParent(parent);
}

void JKQTPParametrizedVectorFieldGraph::getOutsideSize(JKQTPEnhancedPainter &painter, int &leftSpace, int &rightSpace, int &topSpace, int &bottomSpace)
{
    JKQTPVectorFieldGraph::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar&& m_colorColumn>=0 && !m_colorColumnContainsRGB) cbGetOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPParametrizedVectorFieldGraph::drawOutside(JKQTPEnhancedPainter &painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace)
{
    JKQTPVectorFieldGraph::drawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar&& m_colorColumn>=0 && !m_colorColumnContainsRGB) cbDrawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPParametrizedVectorFieldGraph::cbGetDataMinMax(double &dmin, double &dmax)
{
    dmin=dmax=0;
    if (autoImageRange) {
        if (parent==nullptr) return;
        JKQTPDatastore* datastore=parent->getDatastore();
        if (datastore==nullptr) return;
        int imin=0, imax=0;
        if (getIndexRange(imin, imax)) {
            if (m_vectorColorMode==ColorFromCustomColumn) {
                if (m_colorColumn<0) return;
                bool first=true;
                for (int iii=imin; iii<imax; iii++) {
                    const int i=qBound(imin, getDataIndex(iii), imax);
                    const double xv=datastore->get(m_colorColumn,i);
                    if (JKQTPIsOKFloat(xv)) {
                        if (first) {
                            dmin=dmax=xv;
                            first=false;
                        } else {
                            dmin=qMin(xv, dmin);
                            dmax=qMax(xv, dmax);
                        }
                    }
                }
            } else if (m_vectorColorMode==ColorFromMagnitude) {
                bool first=true;
                for (int iii=imin; iii<imax; iii++) {
                    const int i=qBound(imin, getDataIndex(iii), imax);
                    const double vecLen=getVectorMagnitude(i);
                    if (JKQTPIsOKFloat(vecLen)) {
                        if (first) {
                            dmin=dmax=vecLen;
                            first=false;
                        } else {
                            dmin=qMin(vecLen, dmin);
                            dmax=qMax(vecLen, dmax);
                        }
                    }
                }
            } else if (m_vectorColorMode==ColorFromAngle) {
                bool first=true;
                for (int iii=imin; iii<imax; iii++) {
                    const int i=qBound(imin, getDataIndex(iii), imax);
                    const double vecAngle=getVectorAngle(i);
                    if (JKQTPIsOKFloat(vecAngle)) {
                        if (first) {
                            dmin=dmax=vecAngle;
                            first=false;
                        } else {
                            dmin=qMin(vecAngle, dmin);
                            dmax=qMax(vecAngle, dmax);
                        }
                    }
                }
            }
        }
    } else {
        dmin=imageMin;
        dmax=imageMax;
    }
}

bool JKQTPParametrizedVectorFieldGraph::usesColumn(int c) const
{
    return (c==m_colorColumn) || JKQTPVectorFieldGraph::usesColumn(c);
}

void JKQTPParametrizedVectorFieldGraph::setColorColumn(int __value)
{
    m_colorColumn = __value;
    if (__value<0) m_vectorColorMode=ColorFromMagnitude;
    else m_vectorColorMode=ColorFromCustomColumn;
}

int JKQTPParametrizedVectorFieldGraph::getColorColumn() const
{
    return  m_colorColumn;
}

void JKQTPParametrizedVectorFieldGraph::setColorColumn(size_t __value)
{
    m_colorColumn = static_cast<int>(__value);
    m_vectorColorMode=ColorFromCustomColumn;
}

void JKQTPParametrizedVectorFieldGraph::setColorColumnContainsRGB(bool __value)
{
    m_colorColumnContainsRGB=__value;
}

bool JKQTPParametrizedVectorFieldGraph::getColorColumnContainsRGB() const
{
    return m_colorColumnContainsRGB;
}

void JKQTPParametrizedVectorFieldGraph::setVectorColorMode(VectorColorMode __value)
{
    m_vectorColorMode=__value;
}

JKQTPParametrizedVectorFieldGraph::VectorColorMode JKQTPParametrizedVectorFieldGraph::getVectorColorMode() const
{
    return m_vectorColorMode;
}

QColor JKQTPParametrizedVectorFieldGraph::getLocalVectorColor(int i, double x, double y, double dx, double dy) const
{
    if (parent==nullptr) return getLineColor();
    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore==nullptr) return getLineColor();
    if (m_colorColumn<0 && m_vectorColorMode==ColorFromCustomColumn) return getLineColor();
    if (m_colorColumn>=0 && m_vectorColorMode==ColorFromCustomColumn && (i<0 || i>=(int64_t)datastore->getRows(m_colorColumn))) return getLineColor();

    double colValue=0;
    double colMin=m_intColMin;
    double colMax=m_intColMax;
    switch(m_vectorColorMode) {
    case ColorFromCustomColumn:
        colValue=datastore->get(m_colorColumn,i);
        if (m_intColMin==m_intColMax) {
            colMin=0;
            colMax=datastore->getRows(m_colorColumn)-1;
        }
        break;
    case ColorFromMagnitude:
        colValue=getVectorMagnitude(QPointF(dx,dy));
        break;
    case ColorFromAngle:
        colValue=getVectorAngle(QPointF(dx,dy));
        break;
    case DefaultColor:
        return getLineColor();
    }


    if (m_colorColumnContainsRGB && m_vectorColorMode==ColorFromCustomColumn) {
        return QRgb(round(colValue));
    } else {
        QImage img;
        JKQTPImageTools::array2image(&colValue, 1, 1, img, palette, colMin, colMax);
        return img.pixel(0,0);
    }

}
