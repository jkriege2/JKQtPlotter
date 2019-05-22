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



#ifndef JKQTPCOMMONMATHTOOLS_H_INCLUDED
#define JKQTPCOMMONMATHTOOLS_H_INCLUDED
#include "jkqtcommon/jkqtp_imexport.h"
#include <cmath>
#include <QPoint>
#include <QPointF>
#include <vector>



/*! \brief \f$ \sqrt{\pi}=2.50662827463 \f$
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

/** \brief wandelt einen Datentyp in einen double um, wird von JKQTPDatastore zur Wandlung benutzt
 *  \ingroup jkqtptools_math_basic
 *
 * Diese Funktion nutzt per default static_cast<double>(), kann aber für spezielle Datentypen überschrieben werden, etwa für bool
*/
template<typename T>
inline constexpr double jkqtp_todouble(const T& d) {
    return static_cast<double>(d);
}


/** \brief wandelt einen boolean in einen double um, wird von JKQTPDatastore zur Wandlung benutzt,
 *         Spezialisierung für bool (true -> 1.0, false -> 0.0)
 *  \ingroup jkqtptools_math_basic */
template<>
inline constexpr double jkqtp_todouble(const bool& d) {
    return static_cast<double>((d)?1.0:0.0);
}


/** \brief round a double using round() and convert it to a specified type T (static_cast!)
 *  \ingroup jkqtptools_math_basic */
template<typename T>
inline T jkqtp_roundTo(const double& v) {
    return static_cast<T>(round(v));
}


/** \brief compare two floats \a a and \a b for euqality, where any difference smaller than \a epsilon is seen as equality
 *  \ingroup jkqtptools_math_basic */
inline bool jkqtp_approximatelyEqual(float a, float b, float epsilon=2.0*std::numeric_limits<float>::epsilon())
{
    return fabsf(a - b) <= epsilon;
}

/** \brief compare two doubles \a a and \a b for euqality, where any difference smaller than \a epsilon is seen as equality
 *  \ingroup jkqtptools_math_basic */
inline bool jkqtp_approximatelyEqual(double a, double b, double epsilon=2.0*std::numeric_limits<double>::epsilon())
{
    return fabs(a - b) <= epsilon;
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


/*! \brief calculates the sign of number \a x
    \ingroup jkqtptools_math_basic
*/
template <class T>
inline T jkqtp_sign(T x) {
    if (x<0) return -1;
    //else if (x==0) return 0;
    else return 1;
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


#endif // JKQTPCOMMONMATHTOOLS_H_INCLUDED
