/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef jkqtpmathtools_H_INCLUDED
#define jkqtpmathtools_H_INCLUDED
#include "jkqtcommon/jkqtp_imexport.h"
#include <cmath>
#include <limits>
#include <QPoint>
#include <QPointF>
#include <vector>



/*! \brief \f$ \sqrt{2\pi}=2.50662827463 \f$
    \ingroup jkqtptools_math_basic

*/
#define JKQTPSTATISTICS_SQRT_2PI 2.50662827463


/** \brief double-value NotANumber
 * \ingroup jkqtptools_math_basic
 */
#define JKQTP_DOUBLE_NAN (std::numeric_limits<double>::signaling_NaN())

/** \brief float-value NotANumber
 * \ingroup jkqtptools_math_basic
 */
#define JKQTP_FLOAT_NAN (std::numeric_limits<float>::signaling_NaN())

/** \brief double-value NotANumber
 * \ingroup jkqtptools_math_basic
 */
#define JKQTP_NAN JKQTP_DOUBLE_NAN

/** \brief double-value epsilon
 * \ingroup jkqtptools_math_basic
 */
#define JKQTP_DOUBLE_EPSILON (std::numeric_limits<double>::epsilon())

/** \brief float-value epsilon
 * \ingroup jkqtptools_math_basic
 */
#define JKQTP_FLOAT_EPSILON (std::numeric_limits<float>::epsilon())

/** \brief double-value NotANumber
 * \ingroup jkqtptools_math_basic
 */
#define JKQTP_EPSILON JKQTP_DOUBLE_EPSILON

/** \brief converts a boolean to a double, is used to convert boolean to double by JKQTPDatastore
 *  \ingroup jkqtptools_math_basic
 *
 * This function uses static_cast<double>() by default, but certain specializations (e.g. for bool) are
 * readily available.
 *
 * \callergraph
 */
template<typename T>
inline constexpr double jkqtp_todouble(const T& d) {
    return static_cast<double>(d);
}


/** \brief converts a boolean to a double, is used to convert boolean to double by JKQTPDatastore
 *  \ingroup jkqtptools_math_basic
 *
 * Specialisation of the generic template jkqtp_todouble() with (true -> 1.0, false -> 0.0)
 *
 * \callergraph
 */
template<>
inline constexpr double jkqtp_todouble(const bool& d) {
    return static_cast<double>((d)?1.0:0.0);
}


/** \brief round a double \a v using round() and convert it to a specified type T (static_cast!)
 *  \ingroup jkqtptools_math_basic
 *
 *  \tparam T a numeric datatype (int, double, ...)
 *  \param v the value to round and cast
 *
 *  this is equivalent to
 *  \code
 *     static_cast<T>(round(v));
 *  \endcode
 *
 * \callergraph
 */
template<typename T>
inline T jkqtp_roundTo(const double& v) {
    return static_cast<T>(round(v));
}


/** \brief round a double \a v using round() and convert it to a specified type T (static_cast!).
 *         Finally the value is bounded to the range \a min ... \a max
 *  \ingroup jkqtptools_math_basic
 *
 *  \tparam T a numeric datatype (int, double, ...)
 *  \param min minimum output value
 *  \param v the value to round and cast
 *  \param max maximum output value
 *
 *  this is equivalent to
 *  \code
 *     qBound(min, static_cast<T>(round(v)), max);
 *  \endcode
 */
template<typename T>
inline T jkqtp_boundedRoundTo(T min, const double& v, T max) {
    return qBound(min, static_cast<T>(round(v)), max);
}

/** \brief round a double \a v using round() and convert it to a specified type T (static_cast!).
 *         Finally the value is bounded to the range \c std::numeric_limits<T>::min() ... \c std::numeric_limits<T>::max()
 *  \ingroup jkqtptools_math_basic
 *
 *  \tparam T a numeric datatype (int, double, ...)
 *  \param v the value to round and cast
 *
 *  this is equivalent to
 *  \code
 *     jkqtp_boundedRoundTo<T>(std::numeric_limits<T>::min(), v, std::numeric_limits<T>::max())
 *  \endcode
 */
template<typename T>
inline T jkqtp_boundedRoundTo(const double& v) {
    return jkqtp_boundedRoundTo<T>(std::numeric_limits<T>::min(), v, std::numeric_limits<T>::max());
}

/** \brief bounds a value \a v to the given range \a min ... \a max
 *  \ingroup jkqtptools_math_basic
 *
 *  \tparam T a numeric datatype (int, double, ...)
 *  \param min minimum output value
 *  \param v the value to round and cast
 *  \param max maximum output value
 */
template<typename T>
inline T jkqtp_bounded(T min, T v, T max) {
    if (v<min) return min;
    if (v>max) return max;
    return v;
}

/** \brief compare two floats \a a and \a b for euqality, where any difference smaller than \a epsilon is seen as equality
 *  \ingroup jkqtptools_math_basic */
inline bool jkqtp_approximatelyEqual(float a, float b, float epsilon=2.0f*JKQTP_FLOAT_EPSILON)
{
    return fabsf(a - b) <= epsilon;
}

/** \brief compare two doubles \a a and \a b for euqality, where any difference smaller than \a epsilon is seen as equality
 *  \ingroup jkqtptools_math_basic */
inline bool jkqtp_approximatelyEqual(double a, double b, double epsilon=2.0*JKQTP_DOUBLE_EPSILON)
{
    return fabs(a - b) <= epsilon;
}

/** \brief returns the given value \a v (i.e. identity function)
 *  \ingroup jkqtptools_math_basic */
template<typename T>
inline T jkqtp_identity(const T& v) {
    return v;
}

/** \brief returns the quare of the value \a v, i.e. \c v*v
 *  \ingroup jkqtptools_math_basic */
template<typename T>
inline T jkqtp_sqr(const T& v) {
    return v*v;
}


/*! \brief 4-th power of a number
    \ingroup jkqtptools_math_basic

*/
template <class T>
inline T jkqtp_pow4(T x) {
    const T xx=x*x;
    return xx*xx;
}

/*! \brief cube of a number
    \ingroup jkqtptools_math_basic

*/
template <class T>
inline T jkqtp_cube(T x) {
    return x*x*x;
}


/*! \brief calculates the sign of number \a x (-1 for x<0 and +1 for x>=0)
    \ingroup jkqtptools_math_basic
*/
template <class T>
inline T jkqtp_sign(T x) {
    if (x<0) return -1;
    else return 1;
}

/** \brief returns the inversely proportional value 1/\a v of \a v
 *  \ingroup jkqtptools_math_basic */
template<typename T>
inline T jkqtp_inverseProp(const T& v) {
    return T(1.0)/v;
}

/** \brief returns the inversely proportional value 1/\a v of \a v and ensures that \f$ |v|\geq \mbox{absMinV} \f$
 *  \ingroup jkqtptools_math_basic */
template<typename T>
inline T jkqtp_inversePropSave(const T& v, const T& absMinV) {
    T vv=v;
    if (fabs(vv)<absMinV) vv=jkqtp_sign(v)*absMinV;
    return T(1.0)/vv;
}

/** \brief returns the inversely proportional value 1/\a v of \a v and ensures that \f$ |v|\geq \mbox{absMinV} \f$, uses \c absMinV=std::numeric_limits<T>::epsilon()*100.0
 *  \ingroup jkqtptools_math_basic */
template<typename T>
inline T jkqtp_inversePropSaveDefault(const T& v) {
    return jkqtp_inversePropSave<T>(v, std::numeric_limits<T>::epsilon()*100.0);
}


/** \brief calculate the distance between two QPointF points
 *  \ingroup jkqtptools_math_basic
 *
 */
inline double jkqtp_distance(const QPointF& p1, const QPointF& p2){
    return sqrt(jkqtp_sqr<double>(p1.x()-p2.x())+jkqtp_sqr<double>(p1.y()-p2.y()));
}

/** \brief calculate the distance between two QPoint points
 *  \ingroup jkqtptools_math_basic
 *
 */
inline double jkqtp_distance(const QPoint& p1, const QPoint& p2){
    return sqrt(jkqtp_sqr<double>(p1.x()-p2.x())+jkqtp_sqr<double>(p1.y()-p2.y()));
}

/** \brief check whether the dlotaing point number is OK (i.e. non-inf, non-NAN)
 * \ingroup jkqtptools_math_basic
 */
template <typename T>
inline bool JKQTPIsOKFloat(T v) {
    return std::isfinite(v)&&(!std::isinf(v))&&(!std::isnan(v));
}

/** \brief evaluates a gaussian propability density function
 * \ingroup jkqtptools_math_basic
 *
 * \f[ f(x,\mu, \sigma)=\frac{1}{\sqrt{2\pi\sigma^2}}\cdot\exp\left(-\frac{(x-\mu)^2}{2\sigma^2}\right)
 */
inline double jkqtp_gaussdist(double x, double mu=0.0, double sigma=1.0) {
    return exp(-0.5*jkqtp_sqr(x-mu)/jkqtp_sqr(sigma))/sqrt(2.0*M_PI*sigma*sigma);
}

#endif // jkqtpmathtools_H_INCLUDED
