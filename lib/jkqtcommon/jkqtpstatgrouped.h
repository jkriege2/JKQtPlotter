/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    last modification: $LastChangedDate$  (revision $Rev$)

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


#ifndef JKQTPSTATGROUPED_H_INCLUDED
#define JKQTPSTATGROUPED_H_INCLUDED

#include <stdint.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <limits>
#include <vector>
#include <utility>
#include <cfloat>
#include <ostream>
#include <iomanip>
#include <sstream>
#include "jkqtcommon/jkqtcommon_imexport.h"
#include "jkqtcommon/jkqtplinalgtools.h"
#include "jkqtcommon/jkqtparraytools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon/jkqtpstatbasics.h"

/*! \brief a functor \f$ f(x): \mathbb{R}\rightarrow\mathbb{R} \f$ which assignes a value \f$ x \f$ to a group center \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_grouped

    The simplest version are e.g.
      - jkqtpstatGroupingIdentity1D()
      - jkqtpstatGroupingRound1D()
      - jkqtpstatGroupingCustomRound1D()
    .

    \see jkqtpstatGroupData
*/
typedef std::function<double(double)> JKQTPStatGroupDefinitionFunctor1D;



/*! \brief use a column value as group ID directly
    \ingroup jkqtptools_math_statistics_grouped

    \see JKQTPStatGroupDefinitionFunctor1D
*/
JKQTCOMMON_LIB_EXPORT double jkqtpstatGroupingIdentity1D(double v);
/*! \brief use a rounded column value as group ID directly \f$ f(x)=\mbox{round}(x) \f$
    \ingroup jkqtptools_math_statistics_grouped

    \see JKQTPStatGroupDefinitionFunctor1D
*/
JKQTCOMMON_LIB_EXPORT double jkqtpstatGroupingRound1D(double v);
/*! \brief assign each value to groups \f$ \mbox{firstGroupCenter} \f$ , \f$ \mbox{firstGroupCenter}\pm\mbox{groupWidth}/2\f$ , \f$ \mbox{firstGroupCenter}\pm2\cdot\mbox{groupWidth}/2 \f$ , \f$ \mbox{firstGroupCenter}\pm3\cdot\mbox{groupWidth}/2 \f$ , ...
    \ingroup jkqtptools_math_statistics_grouped

    This is equivalent to \f$ \mbox{round}\left(\frac{x-\mbox{firstGroupCenter}}{\mbox{groupWidth}/2}\right) \f$

    \see JKQTPStatGroupDefinitionFunctor1D, jkqtpstatMakeGroupingCustomRound1D() for a factory-function that returns a functor of this function bound to specific arguments.
*/
JKQTCOMMON_LIB_EXPORT double jkqtpstatGroupingCustomRound1D(double v, double firstGroupCenter, double groupWidth);
/*! \brief generates a functor of jkqtpstatGroupingCustomRound1D() with the two paramaters \a firstGroupCenter and \a groupWidth fixed to the given values
    \ingroup jkqtptools_math_statistics_grouped

    This is equivalent to \c std::bind(&jkqtpstatGroupingCustomRound1D,std::placeholders::_1,firstGroupCenter,groupWidth);

    \see JKQTPStatGroupDefinitionFunctor1D, jkqtpstatGroupingCustomRound1D()
*/
JKQTCOMMON_LIB_EXPORT JKQTPStatGroupDefinitionFunctor1D jkqtpstatMakeGroupingCustomRound1D(double firstGroupCenter, double groupWidth);



/*! \brief groups data from an input range \a inFirstCat / \a inFirstValue ...  \a inLastCat / \a outFirstCat representing pairs \f$ (c_i,v_i) \f$ of a
           category value \f$ c_i \f$ and a group value \f$ v_i \f$ into groups \f$ V_j=\{v_{i}|c_i\equiv c_{\text{out},j}\} \f$ of data that were assigned
           to the same group, i.e. \f$ c_i\equiv c_{\text{out},j} \f$ . A functor \a groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \ingroup jkqtptools_math_statistics_grouped

    \tparam InputCatIt standard iterator type of \a inFirstCat and \a inLastCat
    \tparam InputValueIt standard iterator type of \a inFirstValue and \a inLastValue
    \param inFirstCat iterator pointing to the first item in the category dataset to use \f$ c_1 \f$
    \param inLastCat iterator pointing behind the last item in the category dataset to use \f$ c_N \f$
    \param inFirstValue iterator pointing to the first item in the category dataset to use \f$ v_1 \f$
    \param inLastValue iterator pointing behind the last item in the category dataset to use \f$ v_N \f$
    \param[out] groupeddata receives the grouped data, each key in the map represents one group, each map-value contains a vector with the value data
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .

    \note the contents of \a groupeddata is not cleared before usage, so you can also use this fucntion to append to a group!

    \see JKQTPStatGroupDefinitionFunctor1D, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline void jkqtpstatGroupData(InputCatIt inFirstCat, InputCatIt inLastCat, InputValueIt inFirstValue, InputValueIt inLastValue, std::map<double, std::vector<double> >& groupeddata, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D) {
    auto inCat=inFirstCat;
    auto inVal=inFirstValue;
    for (; inCat!=inLastCat && inVal!=inLastValue; ++inCat, ++inVal) {
        const double c=jkqtp_todouble(*inCat);
        const double v=jkqtp_todouble(*inVal);
        if (JKQTPIsOKFloat(c) && JKQTPIsOKFloat(v)) {
            const double g=groupDefFunc(c);
            groupeddata[g].push_back(v);
        }
    }
}

/*! \brief groups data from an input range \a inFirstCat / \a inFirstValue ...  \a inLastCat / \a outFirstCat representing pairs \f$ (c_i,v_i) \f$ of a
           category value \f$ c_i \f$ and a group value \f$ v_i \f$ into groups \f$ V_j=\{v_{i}|c_i\equiv c_{\text{out},j}\} \f$ of data that were assigned
           to the same group, i.e. \f$ c_i\equiv c_{\text{out},j} \f$ . A functor \a groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \ingroup jkqtptools_math_statistics_grouped

    \tparam InputCatIt standard iterator type of \a inFirstCat and \a inLastCat
    \tparam InputValueIt standard iterator type of \a inFirstValue and \a inLastValue
    \tparam OutputGroupIt standard output iterator type of \a outFirstCategory
    \param inFirstCat iterator pointing to the first item in the category dataset to use \f$ c_1 \f$
    \param inLastCat iterator pointing behind the last item in the category dataset to use \f$ c_N \f$
    \param inFirstValue iterator pointing to the first item in the category dataset to use \f$ v_1 \f$
    \param inLastValue iterator pointing behind the last item in the category dataset to use \f$ v_N \f$
    \param[out] outFirstCategory for each element in the range \a inFirstCat / \a inFirstValue ...  \a inLastCat / \a outFirstCat this receives the calculated category
    \param[out] groupeddata receives the grouped data, each key in the map represents one group, each map-value contains a vector with the value data
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .

    \note the contents of \a groupeddata is not cleared before usage, so you can also use this fucntion to append to a group!

    \see JKQTPStatGroupDefinitionFunctor1D, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt, class OutputGroupIt>
inline void jkqtpstatGroupData(InputCatIt inFirstCat, InputCatIt inLastCat, InputValueIt inFirstValue, InputValueIt inLastValue, OutputGroupIt outFirstCategory, std::map<double, std::vector<double> >& groupeddata, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D) {
    auto inCat=inFirstCat;
    auto inVal=inFirstValue;
    auto outCat=outFirstCategory;
    for (; inCat!=inLastCat && inVal!=inLastValue; ++inCat, ++inVal) {
        const double c=jkqtp_todouble(*inCat);
        const double v=jkqtp_todouble(*inVal);
        if (JKQTPIsOKFloat(c) && JKQTPIsOKFloat(v)) {
            const double g=groupDefFunc(c);
            groupeddata[g].push_back(v);
            *outCat=g;
        } else {
            *outCat=JKQTP_DOUBLE_NAN;
        }
        ++outCat;
    }
}


/*! \brief groups data from an input range \a inFirstCat / \a inFirstValue ...  \a inLastCat / \a outFirstCat representing pairs \f$ (c_i,v_i) \f$ of a
           category value \f$ c_i \f$ and a group value \f$ v_i \f$ into groups \f$ V_j=\{v_{i}|c_i\equiv c_{\text{out},j}\} \f$ of data that were assigned
           to the same group, i.e. \f$ c_i\equiv c_{\text{out},j} \f$ . A functor \a groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \ingroup jkqtptools_math_statistics_grouped

    \tparam InputCatIt standard iterator type of \a inFirstCat and \a inLastCat
    \tparam InputValueIt standard iterator type of \a inFirstValue and \a inLastValue
    \param inFirstCat iterator pointing to the first item in the category dataset to use \f$ c_1 \f$
    \param inLastCat iterator pointing behind the last item in the category dataset to use \f$ c_N \f$
    \param inFirstValue iterator pointing to the first item in the category dataset to use \f$ v_1 \f$
    \param inLastValue iterator pointing behind the last item in the category dataset to use \f$ v_N \f$
    \param[out] groupeddata receives the grouped data, each key in the map represents one group, each map-value contains two vecors with the category and value data respectively
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .

    \note the contents of \a groupeddata is not cleared before usage, so you can also use this fucntion to append to a group!

    \see JKQTPStatGroupDefinitionFunctor1D, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline void jkqtpstatGroupData(InputCatIt inFirstCat, InputCatIt inLastCat, InputValueIt inFirstValue, InputValueIt inLastValue, std::map<double, std::pair<std::vector<double>,std::vector<double> > >& groupeddata, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D) {
    auto inCat=inFirstCat;
    auto inVal=inFirstValue;
    for (; inCat!=inLastCat && inVal!=inLastValue; ++inCat, ++inVal) {
        const double c=jkqtp_todouble(*inCat);
        const double v=jkqtp_todouble(*inVal);
        if (JKQTPIsOKFloat(c) && JKQTPIsOKFloat(v)) {
            const double g=groupDefFunc(c);
            groupeddata[g].first.push_back(c);
            groupeddata[g].second.push_back(v);
        }
    }
}


/*! \brief groups data from an input range \a inFirstCat / \a inFirstValue ...  \a inLastCat / \a outFirstCat representing pairs \f$ (c_i,v_i) \f$ of a
           category value \f$ c_i \f$ and a group value \f$ v_i \f$ into groups \f$ V_j=\{v_{i}|c_i\equiv c_{\text{out},j}\} \f$ of data that were assigned
           to the same group, i.e. \f$ c_i\equiv c_{\text{out},j} \f$ . A functor \a groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \ingroup jkqtptools_math_statistics_grouped

    \tparam InputCatIt standard iterator type of \a inFirstCat and \a inLastCat
    \tparam InputValueIt standard iterator type of \a inFirstValue and \a inLastValue
    \tparam OutputGroupIt standard output iterator type of \a outFirstCategory
    \param inFirstCat iterator pointing to the first item in the category dataset to use \f$ c_1 \f$
    \param inLastCat iterator pointing behind the last item in the category dataset to use \f$ c_N \f$
    \param inFirstValue iterator pointing to the first item in the category dataset to use \f$ v_1 \f$
    \param inLastValue iterator pointing behind the last item in the category dataset to use \f$ v_N \f$
    \param[out] outFirstCategory for each element in the range \a inFirstCat / \a inFirstValue ...  \a inLastCat / \a outFirstCat this receives the calculated category
    \param[out] groupeddata receives the grouped data, each key in the map represents one group, each map-value contains two vecors with the category and value data respectively
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .

    \note the contents of \a groupeddata is not cleared before usage, so you can also use this fucntion to append to a group!

    \see JKQTPStatGroupDefinitionFunctor1D, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt, class OutputGroupIt>
inline void jkqtpstatGroupData(InputCatIt inFirstCat, InputCatIt inLastCat, InputValueIt inFirstValue, InputValueIt inLastValue, OutputGroupIt outFirstCategory, std::map<double, std::pair<std::vector<double>,std::vector<double> > >& groupeddata, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D) {
    auto inCat=inFirstCat;
    auto inVal=inFirstValue;
    auto outCat=outFirstCategory;
    for (; inCat!=inLastCat && inVal!=inLastValue; ++inCat, ++inVal) {
        const double c=jkqtp_todouble(*inCat);
        const double v=jkqtp_todouble(*inVal);
        if (JKQTPIsOKFloat(c) && JKQTPIsOKFloat(v)) {
            const double g=groupDefFunc(c);
            groupeddata[g].first.push_back(c);
            groupeddata[g].second.push_back(v);
            *outCat=g;
        } else {
            *outCat=JKQTP_DOUBLE_NAN;
        }
        ++outCat;
    }
}

#endif // JKQTPSTATGROUPED_H_INCLUDED


