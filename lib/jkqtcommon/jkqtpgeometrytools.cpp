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

#include "jkqtcommon/jkqtpgeometrytools.h"
#include <QDebug>
#include <algorithm>



QPolygonF jkqtpRotateRect(QRectF r, double angle) {
    QPolygonF p;
    QTransform m;
    m.rotate(angle);
    p.append(m.map(r.bottomLeft()));
    p.append(m.map(r.bottomRight()));
    p.append(m.map(r.topRight()));
    p.append(m.map(r.topLeft()));
    return p;
}



QVector<QPointF> JKQTPSplitEllipseIntoPoints(double x, double y, double a, double b, double angle_start, double angle_end, double alpha, int controlPoints, QPointF* x_start, QPointF* x_end) {
    QVector<QPointF> result;
    const double start=angle_start*JKQTPSTATISTICS_PI/180.0;
    const double stop=angle_end*JKQTPSTATISTICS_PI/180.0;
    double step=(stop-start)/static_cast<double>(controlPoints);
    while (fabs(stop-start)/step<10) step=step/2.0;
    const double sina=sin(1.0*alpha/180.0*JKQTPSTATISTICS_PI);
    const double cosa=cos(1.0*alpha/180.0*JKQTPSTATISTICS_PI);
    QPointF xp(x+a*cos(start)*cosa-b*sin(start)*sina, y+a*cos(start)*sina+b*sin(start)*cosa);
    result.append(xp);
    if (x_start) *x_start = xp;
    double t=start+step;
    for (int i=1; i<controlPoints; i++) {
        double cost=cos(t);
        double sint=sin(t);
        xp=QPointF( x+a*cost*cosa-b*sint*sina,  y+a*cost*sina+b*sint*cosa);
        result.append(xp);
        //std::cout<<"t="<<t/JKQTPSTATISTICS_PI*180.0<<":  sin(al)="<<sina<<" cos(al)="<<cosa<<" sin(t)="<<sint<<" cos(t)="<<cost<<" a="<<a<<" b="<<b<<": ("<<x+a*cost*cosa-b*sint*sina<<", "<<y+a*cost*sina+b*sint*cosa<<") = ("<<xp.x()<<", "<<xp.y()<<") \n";
        t=t+step;
    }
    if (x_end) *x_end=xp;
    return result;
}

QVector<QPointF> JKQTPSplitEllipseIntoPoints(std::function<QPointF (QPointF)> fTransform, double x, double y, double a, double b, double angle_start, double angle_end, double alpha, QPointF *x_start, QPointF *x_end, QPointF *x_start_notrafo, QPointF *x_end_notrafo)
{
    const double sina=sin(1.0*alpha/180.0*JKQTPSTATISTICS_PI);
    const double cosa=cos(1.0*alpha/180.0*JKQTPSTATISTICS_PI);
    std::function<QPointF(double)> fell=[&](double t)->QPointF {
        return QPointF(x+a*cos(t)*cosa-b*sin(t)*sina, y+a*cos(t)*sina+b*sin(t)*cosa);
    };
    std::function<QPointF(double)> fxy = [&](double t) ->QPointF {
        return fTransform(fell(t));
    };
    JKQTPAdaptiveFunctionGraphEvaluator eval(fxy);

    const QVector<QPointF> points=eval.evaluate(angle_start*JKQTPSTATISTICS_PI/180.0, angle_end*JKQTPSTATISTICS_PI/180.0);
    if (points.size()>0) {
        if (x_start) *x_start=points.first();
        if (x_end) *x_end=points.last();
        if (x_start_notrafo) *x_start_notrafo=fell(angle_start*JKQTPSTATISTICS_PI/180.0);
        if (x_end_notrafo) *x_end_notrafo=fell(angle_end*JKQTPSTATISTICS_PI/180.0);
    }
    return points;
}




QVector<QPolygonF> JKQTPUnifyLinesToPolygons(const QVector<QLineF> &lines, double distanceThreshold, int searchMaxSurroundingElements)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPUnifyLinesToPolygons(%1, %2, %3)").arg(lines.size()).arg(distanceThreshold).arg(searchMaxSurroundingElements));
#endif
    QList<QPolygonF> res;
    res.reserve(lines.size());

    // first simply convert all lines to polygons
    for (const QLineF& l: lines) {
        QPolygonF p;
        p<<l.p1()<<l.p2();
        res<<p;
    }
    //return res.toVector();
    // clean the resulting polygon
    for (QPolygonF& p: res) {
        p=JKQTPCleanPolygon(p, distanceThreshold);
    }

    int maxIterations=100;
    int iter=0;
    bool found=true;
    //qDebug()<<"   iter "<<-1<<" -> polygons start "<<res.size();
    while (found && iter<maxIterations) {
        found=false;
        int i=0;
        while (i<res.size()-1) {
            int j=i+1;
            while (j<res.size() && j<i+searchMaxSurroundingElements) {
                if (jkqtp_distance(res[i].first(),res[j].first())<=distanceThreshold) {
                    found=true;
                    for (int k=1; k<res[j].size(); k++) {
                        res[i].prepend(res[j].at(k));
                    }
                    res.removeAt(j);
                } else if (jkqtp_distance(res[i].first(),res[j].last())<=distanceThreshold) {
                    found=true;
                    for (int k=res[j].size()-2; k>=0; k--) {
                        res[i].prepend(res[j].at(k));
                    }
                    res.removeAt(j);
                } else if (jkqtp_distance(res[i].last(),res[j].first())<=distanceThreshold) {
                    found=true;
                    for (int k=1; k<res[j].size(); k++) {
                        res[i].append(res[j].at(k));
                    }
                    res.removeAt(j);
                } else if (jkqtp_distance(res[i].last(),res[j].last())<=distanceThreshold) {
                    found=true;
                    for (int k=res[j].size()-2; k>=0; k--) {
                        res[i].append(res[j].at(k));
                    }
                    res.removeAt(j);
                } else {
                    j++;
                }
            }
            res[i]=JKQTPCleanPolygon(res[i], distanceThreshold);
            i++;
        }
        //qDebug()<<"   iter "<<iter<<" -> polygons left "<<res.size();
        iter++;
    }

    return res.toVector();
}

QPolygonF JKQTPCleanPolygon(const QPolygonF &poly, double distanceThreshold)
{
    if (poly.size()<=2) return poly;
    QPolygonF p;
    QPointF p0=poly[0];
    p<<p0;
    QVector<QPointF> inbetween;
    int i=1;
    while (i<poly.size()) {
        if ((jkqtp_distance(poly[i], p0)<=distanceThreshold)) {
            inbetween<<poly[i];
        } else {
            QPointF pmean(0,0);
            if (inbetween.size()>0) {
                for (const QPointF& pi: inbetween) {
                    pmean=QPointF(pmean.x()+pi.x()/static_cast<double>(inbetween.size()), pmean.y()+pi.y()/static_cast<double>(inbetween.size()));
                }
            } else {
                pmean=poly[i];
            }
            p<<pmean;
            p0=pmean;
            inbetween.clear();
        }
        i++;
    }

    // maybe we have something left to add
    QPointF pmean(0,0);
    if (inbetween.size()>0) {
        for (const QPointF& pi: inbetween) {
            pmean=QPointF(pmean.x()+pi.x()/static_cast<double>(inbetween.size()), pmean.y()+pi.y()/static_cast<double>(inbetween.size()));
        }
    } else {
        pmean=p0;
    }

    if (jkqtp_distance(pmean, poly.last())>distanceThreshold) {
        p<<pmean<<poly.last();
    } else {
        if (p.last()!=poly.last()) p<<poly.last();
    }
    return p;
}


QVector<QPointF> JKQTPSplitLineIntoPoints(const QLineF &line, int controlPoints)
{
    QVector<QPointF> result;
    result.reserve(controlPoints);
    result.push_back(line.p1());
    for (int i=1; i<controlPoints; i++) {
        result.push_back(line.p1()+static_cast<double>(i)/static_cast<double>(controlPoints)*(line.p2()-line.p1()));
    }
    result.push_back(line.p2());
    return result;
}

QVector<QPointF> JKQTPSimplyfyLineSegemnts(const QVector<QPointF> &points, double maxConsecutiveAngleDegree)
{
    QVector<QPointF> result;
    if (points.size()>2) {
        result.push_back(points[0]);
        for (int i=1; i<points.size()-1; i++) {
            const QLineF l1(result.last(), points[i]);
            const QLineF l2(points[i], points[i+1]);
            if (fabs(l1.angle()-l2.angle())>maxConsecutiveAngleDegree && l1.length()>0 ) {
                result.push_back(points[i]);
            }
        }
        if (result.last()!=points.last()) result.push_back(points.last());
        return result;
    } else {
        return points;
    }
}

JKQTPAdaptiveFunctionGraphEvaluator::JKQTPAdaptiveFunctionGraphEvaluator(const std::function<double (double)> &fx_, const std::function<double (double)> &fy_, unsigned int minSamples_, unsigned int maxRefinementDegree_, double slopeTolerance_, double minPixelPerSample_):
    fx(fx_),
    fy(fy_),
    fxy([&](double t)->QPointF { return QPointF(fx(t), fy(t)); }),
    minSamples(minSamples_),
    maxRefinementDegree(maxRefinementDegree_),
    slopeTolerance(slopeTolerance_),
    minPixelPerSample(minPixelPerSample_)
{

}

JKQTPAdaptiveFunctionGraphEvaluator::JKQTPAdaptiveFunctionGraphEvaluator(const std::function<QPointF (double)> &fxy_, unsigned int minSamples_, unsigned int maxRefinementDegree_, double slopeTolerance_, double minPixelPerSample_):
    fxy(fxy_),
    minSamples(minSamples_),
    maxRefinementDegree(maxRefinementDegree_),
    slopeTolerance(slopeTolerance_),
    minPixelPerSample(minPixelPerSample_)
{

}

QVector<QPointF> JKQTPAdaptiveFunctionGraphEvaluator::evaluate(double tmin, double tmax) const
{
    InternalList intData;

    double delta_t0=(tmax-tmin)/static_cast<double>(minSamples);

    intData.push_front(std::pair<double, QPointF>(tmin, fxy(tmin)));
    InternalList::iterator a=intData.begin();
    //qDebug()<<"**************************************************";
    for (double t=tmin+delta_t0; t<tmax; t=t+delta_t0) {
        const double treal=t;
        intData.insert_after(a, std::pair<double, QPointF>(treal, fxy(treal)));
        InternalList::iterator b=a; b++;
        //qDebug()<<"t="<<t<<", dist(a,b)="<<std::distance(a,b);
        refine(intData, a, b, 0);
        //qDebug()<<"       after refine: dist(a,b)="<<std::distance(a,b);
        a=b;
    }
    intData.insert_after(a, std::pair<double, QPointF>(tmax, fxy(tmax)));
    auto b=a; b++;
    refine(intData, a, b, 0);
    // copy data to output data structure
    QVector<QPointF> result;
    result.reserve(std::distance(intData.begin(), intData.end()));
    for (const auto& d: intData) {
        result.push_back(d.second);
    }
    return result;
}

void JKQTPAdaptiveFunctionGraphEvaluator::refine(JKQTPAdaptiveFunctionGraphEvaluator::InternalList &intData, JKQTPAdaptiveFunctionGraphEvaluator::InternalList::iterator a, JKQTPAdaptiveFunctionGraphEvaluator::InternalList::iterator b, unsigned int degree) const
{
    if (degree>=maxRefinementDegree) return;
    const double ta=a->first;
    const double tb=b->first;
    const double tmid=ta+(tb-ta)*(0.5 +(static_cast<double>(rand())/static_cast<double>(RAND_MAX)-0.5)/5.0);
    const QPointF pa=a->second;
    const QPointF pb=b->second;
    const QPointF pmid(fxy(tmid));
    const double delta=QLineF(pa, pb).length();

    const double slope_a_mid=(pmid.y()-pa.y())/(pmid.x()-pa.x());
    const double slope_mid_b=(pb.y()-pmid.y())/(pb.x()-pmid.x());
    if (fabs(slope_mid_b-slope_a_mid)>slopeTolerance || delta>minPixelPerSample) {
        intData.insert_after(a, std::pair<double, QPointF>(tmid, pmid));
        InternalList::iterator abmid=a; abmid++;
        refine(intData, a, abmid, degree+1);
        refine(intData, abmid, b, degree+1);
    }
}

QVector<QPointF> JKQTPSplitLineIntoPoints(const QLineF &line, std::function<QPointF (QPointF)> fTransform)
{
    std::function<QPointF(double)> fxy = [&line, &fTransform] (double t) ->QPointF {
        return fTransform(line.p1()+t*(line.p2()-line.p1()));
    };
    JKQTPAdaptiveFunctionGraphEvaluator eval(fxy);

    return eval.evaluate(0.0,1.0);
}


QVector<QPointF> JKQTPSplitPolylineIntoPoints(const QVector<QPointF> &line, std::function<QPointF (QPointF)> fTransform)
{
    QVector<QPointF> result;
    if (line.size()==2) return JKQTPSplitLineIntoPoints(QLineF(line[0], line[1]), fTransform);
    if (line.size()>2) {
        for (int i=1; i<line.size(); i++) {
            const QVector<QPointF> ps=JKQTPSplitLineIntoPoints(QLineF(line[i-1], line[i]), fTransform);
            result.reserve(result.size()+ps.size());
            for (auto& p: ps) {
                if (result.size()==0 || result.last()!=p) result.push_back(p);
            }
            //qDebug()<<"line: "<<QLineF(line[i-1], line[i])<<"  --> ps.size()="<<ps.size()<<", result.size()="<<result.size();
        }
    }
    return result;
}



bool JKQTPClipLine(QLineF &line, const QRectF &clippingRect)
{
    //qDebug()<<"line="<<line<<", clippingRect="<<clippingRect;
    const qreal p1 = -(line.x2() - line.x1());
    const qreal p2 = -p1;
    const qreal p3 = -(line.y2() - line.y1());
    const qreal p4 = -p3;

    const qreal q1 = line.x1() - std::min(clippingRect.left(),clippingRect.right());
    const qreal q2 = std::max(clippingRect.left(),clippingRect.right()) - line.x1();
    const qreal q3 = line.y1() - std::min(clippingRect.bottom(),clippingRect.top());
    const qreal q4 = std::max(clippingRect.bottom(),clippingRect.top()) - line.y1();

    //qDebug()<<"  p1="<<p1<<", p2="<<p2<<", p3="<<p3<<", p4="<<p4;
    //qDebug()<<"  q1="<<q1<<", q2="<<q2<<", q3="<<p3<<", q4="<<p4;

    std::vector<qreal> posarr, negarr;
    posarr.reserve(5); negarr.reserve(5);
    posarr.push_back(1);
    negarr.push_back(0);


    if ((p1 == 0 && q1 < 0) || (p2 == 0 && q2 < 0) || (p3 == 0 && q3 < 0) || (p4 == 0 && q4 < 0)) {
        //qDebug()<<"  --> parallel line";
        line=QLineF();
        return false;
    }
    if (p1 != 0) {
        const qreal r1 = q1 / p1;
        const qreal r2 = q2 / p2;
        if (p1 < 0) {
            negarr.push_back(r1); // for negative p1, add it to negative array
            posarr.push_back(r2); // and add p2 to positive array
        } else {
            negarr.push_back(r2);
            posarr.push_back(r1);
        }
    }
    if (p3 != 0) {
        const qreal r3 = q3 / p3;
        const qreal r4 = q4 / p4;
        if (p3 < 0) {
            negarr.push_back(r3);
            posarr.push_back(r4);
        } else {
            negarr.push_back(r4);
            posarr.push_back(r3);
        }
    }

    const qreal rn1 = *std::max_element(negarr.begin(), negarr.end()); // maximum of negative array
    const qreal rn2 = *std::min_element(posarr.begin(), posarr.end()); // minimum of positive array

    //qDebug()<<"  rn1="<<rn1<<", rn2="<<rn2<<"   negarr="<<negarr<<", posarr="<<posarr;

    if (rn1 > rn2)  { // reject
        //qDebug()<<"  --> rejected line";
        line=QLineF();
        return false;
    }

    const qreal xn1 = line.x1() + p2 * rn1;
    const qreal yn1 = line.y1() + p4 * rn1; // computing new points

    const qreal xn2 = line.x1() + p2 * rn2;
    const qreal yn2 = line.y1() + p4 * rn2;

    line=QLineF(xn1, yn1, xn2, yn2); // the drawing the new line
    //qDebug()<<"  --> clipped line: "<<line;
    return true;
}

