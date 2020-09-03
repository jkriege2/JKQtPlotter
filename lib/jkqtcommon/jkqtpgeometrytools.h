/*
    Copyright (c) 2020-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef JKQTPGEOMETRYTOOLS_H_INCLUDED
#define JKQTPGEOMETRYTOOLS_H_INCLUDED
#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QPolygonF>
#include <QPolygon>
#include <QRectF>
#include <QRect>
#include <QLineF>
#include <QLine>
#include <QPainterPath>
#include <QVector>
#include <vector>
#include <forward_list>
#include <cmath>
#include <utility>
#include <QDebug>
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtcommon/jkqtpcodestructuring.h"


/** \brief rotate a rectangle by  given angle (rotates all points around the center of the rectangle and returns it as a QPolygonF)
 * \ingroup jkqtptools_drawing
 */
JKQTCOMMON_LIB_EXPORT QPolygonF jkqtpRotateRect(QRectF r, double angle);


/** \brief adaptive drawing of a function graph, specified by two function \f$ f_x(t) \f$ and \f$ f_y(t) \f$ evaluated over a parameter range \f$ t\in\left[t_\text{min}..t_\text{max}\tight] \f$ */
class JKQTCOMMON_LIB_EXPORT JKQTPAdaptiveFunctionGraphEvaluator {
public:

    /** \brief class constructor
     *
     *  \param fx function \f$ f_x(t) \f$
     *  \param fy function \f$ f_y(t) \f$
     *  \param minSamples the minimum number of points to evaluate the function at
     *  \param maxRefinementDegree the maximum number of recursive refinement steps
     *                             each step bisects the interval \f$ [a, b] \f$ into two halfes. So the maximum number
     *                             of points plotted at all are thus:
     *                               \f[ \mbox{minSamples} \cdot 2^{\mbox{maxRefinementDegree}} \f]
     *  \param slopeTolerance the tolerance for the difference of two subsequent slopes
     *  \param minPixelPerSample create one sample at least every \a minPixelPerSample pixels
     */
    JKQTPAdaptiveFunctionGraphEvaluator(const std::function<double(double)>& fx_, const std::function<double(double)>& fy_, unsigned int minSamples_=10, unsigned int maxRefinementDegree_=5, double slopeTolerance_=0.005, double minPixelPerSample_=32);

    /** \brief evaluate the function specified in the constructor over the given parameter range \a tmin ... \a tmax
     *
     *  \param tmin lower parameter range limit \f$ t_\text{min} \f$
     *  \param tmax upper parameter range limit \f$ t_\text{max} \f$
     */
    QVector<QPointF> evaluate(double tmin=0.0, double tmax=1.0) const;
protected:
    typedef std::forward_list<std::pair<double, QPointF>> InternalList;
    /** \brief refine (if necessary) the function graph between the two points \a a and \a b, working on the given list of data \a data */
    void refine(InternalList& data, InternalList::iterator a, InternalList::iterator b, unsigned int degree) const;

    /** \brief function \f$ f_x(t) \f$ */
    std::function<double(double)> fx;
    /** \brief function \f$ f_y(t) \f$ */
    std::function<double(double)> fy;
    /** \brief the minimum number of points to evaluate the function at */
    unsigned int minSamples;
    /** \brief the maximum number of recursive refinement steps
         *
         * each step bisects the interval \f$ [a, b] \f$ into two halfes. So the maximum number
         * of points plotted at all are thus:
         *  \f[ \mbox{minSamples} \cdot 2^{\mbox{maxRefinementDegree}} \f]
         */
    unsigned int maxRefinementDegree;
    /** \brief the tolerance for the difference of two subsequent slopes */
    double slopeTolerance;
    /** \brief create one sample at least every \a minPixelPerSample pixels */
    double minPixelPerSample;
};


/*! \brief represent an ellipse as a series of points on the ellipse
    \ingroup jkqtptools_drawing

    \return a QVector<QPointF> with points that may be used for drawing
    \param x center of ellipse (x-coordinate)
    \param y center of ellipse (y-coordinate)
    \param a half axis in x-direction
    \param b half axis in y-direction
    \param angle_start starting angle of ellipse section
    \param angle_end ending angle of ellipse section
    \param alpha rotation angle of ellipse
    \param controlPoints the number of points to use for drawing
    \param[out] x_start first point of ellipse
    \param[out] x_end last point of ellipse

    \note all angles are given in degrees [0..360]
*/
JKQTCOMMON_LIB_EXPORT QVector<QPointF> JKQTPSplitEllipseIntoPoints(double x, double y, double a, double b, double angle_start=0, double angle_end=360, double alpha=0, int controlPoints=180, QPointF* x_start=nullptr, QPointF* x_end=nullptr);


/*! \brief represent an ellipse as a series of points on the ellipse
    \ingroup jkqtptools_drawing

    \return a QVector<QPointF> with points that may be used for drawing
    \param fTransform a function that transforms a point in graph coordinate space into pixel coordinate space
    \param x center of ellipse (x-coordinate)
    \param y center of ellipse (y-coordinate)
    \param a half axis in x-direction
    \param b half axis in y-direction
    \param angle_start starting angle of ellipse section
    \param angle_end ending angle of ellipse section
    \param alpha rotation angle of ellipse
    \param[out] x_start first point of ellipse
    \param[out] x_end last point of ellipse

    \note all angles are given in degrees [0..360]
*/
JKQTCOMMON_LIB_EXPORT QVector<QPointF> JKQTPSplitEllipseIntoPoints(std::function<QPointF(QPointF)> fTransform, double x, double y, double a, double b, double angle_start=0, double angle_end=360, double alpha=0, QPointF* x_start=nullptr, QPointF* x_end=nullptr);



/*! \brief represent a line as a series of points on the ellipse
    \ingroup jkqtptools_drawing

    \return a QVector<QPointF> with points that may be used for drawing
    \param line the line to draw
    \param controlPoints the number of points to use for drawing

*/
JKQTCOMMON_LIB_EXPORT QVector<QPointF> JKQTPSplitLineIntoPoints(const QLineF& line, int controlPoints=180);

/*! \brief represent a line as a series of points on the ellipse
    \ingroup jkqtptools_drawing

    \return a QVector<QPointF> with points that may be used for drawing
    \param line the line to draw in graph coordinate space
    \param fTransform a function that transforms a point in graph coordinate space into pixel coordinate space

*/
JKQTCOMMON_LIB_EXPORT QVector<QPointF> JKQTPSplitLineIntoPoints(const QLineF& line, std::function<QPointF(QPointF)> fTransform);


/*! \brief represent a poly-line as a series of points on the ellipse
    \ingroup jkqtptools_drawing

    \return a QVector<QPointF> with points that may be used for drawing
    \param line the poly-line to draw in graph coordinate space
    \param fTransform a function that transforms a point in graph coordinate space into pixel coordinate space

*/
JKQTCOMMON_LIB_EXPORT QVector<QPointF> JKQTPSplitPolylineIntoPoints(const QVector<QPointF>& line, std::function<QPointF(QPointF)> fTransform);


/*! \brief takes a list of points and tries to reduce them. Three points are merged to two, if they form a straight line
    \ingroup jkqtptools_drawing

    \return a cleaned QVector<QPointF>
    \param points input poly-line
    \param maxConsecutiveAngleDegree is two consecutive line-segments differ by an angle smaller than this, they can be merged

    \note this implements an incomplete algorithm

*/
JKQTCOMMON_LIB_EXPORT QVector<QPointF> JKQTPSimplyfyLineSegemnts(const QVector<QPointF>& points, double maxConsecutiveAngleDegree=1.0);


/** \brief cleans a polygon by uniting all consecutive points that were closer than distanceThreshold are united
 *  \ingroup jkqtptools_drawing
 *
 *  \param poly polygon to clean
 *  \param distanceThreshold if two end-points are closer together as this value, they are united to a single point
 *  \return a cleaned polygon, where all consecutive points that were closer than distanceThreshold are united
 */
JKQTCOMMON_LIB_EXPORT QPolygonF JKQTPCleanPolygon(const QPolygonF& poly, double distanceThreshold=0.3);

/** \brief takes a list of QLineF objesct \a lines and tries to combine as many of them as possible to QPolygonF objects.
 *         <b>Note: This method implements an incomplete algorithm with \a searchMaxSurroundingElements>0, as solving
 *         the complete problem is very time-consuming (cubic runtime)</b>
 *  \ingroup jkqtptools_drawing
 *
 *  \param lines line segments to unify
 *  \param distanceThreshold if two end-points are closer together as this value, they are united to a single point
 *  \param searchMaxSurroundingElements limits the search for a connected polygon to at most this number of neighbors
 *  \return a vector of QPolygonF objects, which contain longer line-segments formed from \a lines
 */
JKQTCOMMON_LIB_EXPORT QVector<QPolygonF> JKQTPUnifyLinesToPolygons(const QVector<QLineF>& lines, double distanceThreshold=0.3, int searchMaxSurroundingElements=10);

/** \brief clip a QLineF \a line to the rectangle defines by \a clippingRect
 *  \ingroup jkqtptools_drawing
 *
 *  \param[in,out] line The line to clip, if clipping is possible this is modified to the clipped line.
 *                      If the line is outside \a clippingRect this is modified to \c line=QLineF() i.e. a null-line!
 *  \param clippingRect the rectangle to clip to
 *  \return \c true, if the line had at least some points within \a clippingRect, \c false otherwise.
 *          The clipped line (or a null-line) is returned in the by-refrence parameter \a line
 *
 *  \see This function uses the Linag-Barsky-Algorithm: https://en.wikipedia.org/wiki/Liang%E2%80%93Barsky_algorithm https://www.skytopia.com/project/articles/compsci/clipping.html
 */
JKQTCOMMON_LIB_EXPORT bool JKQTPClipLine(QLineF& line, const QRectF& clippingRect);





#endif // JKQTPGEOMETRYTOOLS_H_INCLUDED
