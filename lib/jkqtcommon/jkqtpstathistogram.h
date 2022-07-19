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


#ifndef JKQTPSTATHISTOGRAM_H_INCLUDED
#define JKQTPSTATHISTOGRAM_H_INCLUDED

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




/*! \brief defines where the returned x-coordinates (in histogramXOut) lie inside a histogram bin
    \ingroup jkqtptools_math_statistics_1dhist
    \see jkqtpstatHistogram()
*/
enum class JKQTPStatHistogramBinXMode {
    XIsLeft, /*!< \brief x-location is the left edge of the bin */
    XIsMid, /*!< \brief x-location is the middle of the bin */
    XIsRight /*!< \brief x-location is the right edge of the bin */
};

/*! \brief calculate an autoranged 1-dimensional histogram from the given data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dhist

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a histogramXOut and \a histogramYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] histogramXOut output iterator that receives x-positions of the histogram bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] histogramYOut output iterator that receives counts/frequencies of the histogram bins
    \param bins number of bins in the output histogram
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \param binXMode defines where the returned x-coordinates (in histogramXOut) lie inside the histogram bin (see JKQTPStatHistogramBinXMode)

    \see jkqtpstatAddHHistogram1DAutoranged()
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatHistogram1DAutoranged(InputIt first, InputIt last, OutputIt histogramXOut, OutputIt histogramYOut, int bins=11, bool normalized=true, bool cummulative=false, JKQTPStatHistogramBinXMode binXMode=JKQTPStatHistogramBinXMode::XIsLeft) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double range=maxV-minV;
    const double binw=range/static_cast<double>(bins);

    // initialize the histogram
    for (int i=0; i<bins; i++)  {
        histX.push_back(minV+static_cast<double>(i)*binw);
        histY.push_back(0);
    }

    // calculate the histogram
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            size_t b=jkqtp_bounded<size_t>(0, static_cast<size_t>(floor((v-minV)/binw)), bins-1);
            histY[b]++;
        }
    }


    // output the histogram
    double xoffset=0;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsRight) xoffset=binw;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsMid) xoffset=binw/2.0;

    double NNorm=1;
    if (normalized) {
        NNorm=static_cast<double>(N);
    }
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *histogramXOut=histX[i]+xoffset;
        if (cummulative) h+=(histY[i]/NNorm);
        else h=histY[i]/NNorm;
        *histogramYOut=h;
        ++histogramXOut;
        ++histogramYOut;
    }
}

/*! \brief calculate an autoranged 1-dimensional histogram from the given data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_1dhist

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a histogramXOut and \a histogramYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param[out] histogramXOut output iterator that receives x-positions of the histogram bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] histogramYOut output iterator that receives counts/frequencies of the histogram bins
    \param binWidth width of the bins
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \param binXMode defines where the returned x-coordinates (in histogramXOut) lie inside the histogram bin (see JKQTPStatHistogramBinXMode)

    \see jkqtpstatAddHHistogram1DAutoranged()
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatHistogram1DAutoranged(InputIt first, InputIt last, OutputIt histogramXOut, OutputIt histogramYOut, double binWidth, bool normalized=true, bool cummulative=false, JKQTPStatHistogramBinXMode binXMode=JKQTPStatHistogramBinXMode::XIsLeft) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double range=maxV-minV;
    const double binw=binWidth;
    const int bins=static_cast<int>(ceil(range/binWidth));

    // initialize the histogram
    for (int i=0; i<bins; i++)  {
        histX.push_back(minV+static_cast<double>(i)*binw);
        histY.push_back(0);
    }

    // calculate the histogram
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            size_t b=jkqtp_bounded<size_t>(0, static_cast<size_t>(floor((v-minV)/binw)), bins-1);
            histY[b]++;
        }
    }


    // output the histogram
    double xoffset=0;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsRight) xoffset=binw;
    if (binXMode==JKQTPStatHistogramBinXMode::XIsMid) xoffset=binw/2.0;

    double NNorm=1;
    if (normalized) {
        NNorm=static_cast<double>(N);
    }
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *histogramXOut=histX[i]+xoffset;
        if (cummulative) h+=(histY[i]/NNorm);
        else h=histY[i]/NNorm;
        *histogramYOut=h;
        ++histogramXOut;
        ++histogramYOut;    }
}



/*! \brief calculate an autoranged 1-dimensional histogram from the given data range \a first ... \a last, bins defined the range \a binsFirst ... \a binsLast
    \ingroup jkqtptools_math_statistics_1dhist

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \tparam OutputIt standard output iterator type used for the outliers output \a histogramXOut and \a histogramYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of histogram bins
    \param binsLast iterator pointing behind the last item in the set of histogram bins
    \param[out] histogramXOut output iterator that receives x-positions of the histogram bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] histogramYOut output iterator that receives counts/frequencies of the histogram bins
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \param binXMode defines where the returned x-coordinates (in histogramXOut) lie inside the histogram bin (see JKQTPStatHistogramBinXMode)

    \see jkqtpstatAddHHistogram1D()
*/
template <class InputIt, class BinsInputIt, class OutputIt>
inline void jkqtpstatHistogram1D(InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, OutputIt histogramXOut, OutputIt histogramYOut, bool normalized=true, bool cummulative=false, JKQTPStatHistogramBinXMode binXMode=JKQTPStatHistogramBinXMode::XIsLeft) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;


    // initialize the histogram
    for (auto it=binsFirst; it!=binsLast; ++it)  {
        histX.push_back(jkqtp_todouble(*it));
        histY.push_back(0);
    }
    std::sort(histX.begin(), histX.end());

    // calculate the histogram
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            auto itb=std::lower_bound(histX.begin(), histX.end(), v);
            size_t bin=jkqtp_bounded<size_t>(0,static_cast<size_t>(std::abs(std::distance(histX.begin(), itb))), histY.size()-1);
            histY[bin]++;
        }
    }


    // output the histogram
    double NNorm=1;
    if (normalized) {
        NNorm=static_cast<double>(N);
    }
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        double xoffset=0;
        double binw=1;
        if (binXMode!=JKQTPStatHistogramBinXMode::XIsLeft) {
            if (i==0 && i+1<histX.size()) binw=histX[1]-histX[0];
            else if (i==histX.size()-1 && static_cast<int>(i)-1>0) binw=histX[histX.size()-1]-histX[histX.size()-2];
            else if (i<histX.size() && i+1<histX.size()) binw=histX[i+1]-histX[i];
            if (binXMode==JKQTPStatHistogramBinXMode::XIsRight) xoffset=binw;
            if (binXMode==JKQTPStatHistogramBinXMode::XIsMid) xoffset=binw/2.0;
        }

        *histogramXOut=histX[i]+xoffset;
        if (cummulative) h+=(histY[i]/NNorm);
        else h=histY[i]/NNorm;
        *histogramYOut=h;
        ++histogramXOut;
        ++histogramYOut;    }
}









/*! \brief calculate a 2-dimensional histogram from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_2dhist

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam OutputIt standard output iterator type used for the outliers output \a histogramXOut and \a histogramYOut, use e.g. std::back_inserter
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param[out] histogramImgOut output iterator that receives counts of the histogram bins in row-major ordering
    \param xmin position of the first histogram bin in x-direction
    \param xmax position of the last histogram bin in x-direction
    \param ymin position of the first histogram bin in y-direction
    \param ymax position of the last histogram bin in y-direction
    \param xbins number of bins in x-direction (i.e. width of the output histogram \a histogramImgOut )
    \param ybins number of bins in y-direction (i.e. height of the output histogram \a histogramImgOut )
    \param normalized indicates whether the histogram has to be normalized

    \see jkqtpstatAddHHistogram1DAutoranged()
*/
template <class InputItX, class InputItY, class OutputIt>
inline void jkqtpstatHistogram2D(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, OutputIt histogramImgOut, double xmin, double xmax, double ymin, double ymax, size_t xbins=10, size_t ybins=10, bool normalized=true) {

    const double binwx=fabs(xmax-xmin)/static_cast<double>(xbins);
    const double binwy=fabs(ymax-ymin)/static_cast<double>(ybins);

    std::vector<double> hist;
    std::fill_n(std::back_inserter(hist), xbins*ybins, 0.0);

    // calculate the histogram
    auto itX=firstX;
    auto itY=firstY;
    size_t N=0;
    for (; (itX!=lastX) && (itY!=lastY); ++itX, ++itY)  {
        const double vx=jkqtp_todouble(*itX);
        const double vy=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(vx) && JKQTPIsOKFloat(vy)) {
            const size_t bx=jkqtp_bounded<size_t>(0, static_cast<size_t>(floor((vx-xmin)/binwx)), xbins-1);
            const size_t by=jkqtp_bounded<size_t>(0, static_cast<size_t>(floor((vy-ymin)/binwy)), ybins-1);
            hist[by*xbins+bx]++;
            N++;
        }
    }


    // output the histogram
    double NNorm=1;
    if (normalized) {
        NNorm=static_cast<double>(N);
    }
    std::transform(hist.begin(), hist.end(), histogramImgOut, [NNorm](double v) { return v/NNorm; });
}







#endif // JKQTPSTATHISTOGRAM_H_INCLUDED


