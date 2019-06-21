/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

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


#ifndef JKQTPSTATKDE_H_INCLUDED
#define JKQTPSTATKDE_H_INCLUDED

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
#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtcommon/jkqtplinalgtools.h"
#include "jkqtcommon/jkqtparraytools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon/jkqtpstatbasics.h"





/*! \brief a 1D Gaussian kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t):=\frac{1}{\sqrt{2\pi}}\exp \left(-\frac{1}{2}t^2\right) \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DGaussian(double t);
/*! \brief a 1D Cauchy kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t):=\frac{1}{\pi(1+t^2)} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DCauchy(double t);

/*! \brief a 1D Picard kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t):=\frac{1}{2}\exp(-|t|) \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DPicard(double t);
/*! \brief a 1D Epanechnikov kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{3}{4} ( 1- t^2 ),  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DEpanechnikov(double t);
/*! \brief a 1D uniform kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}0.5,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DUniform(double t);
/*! \brief a 1D Epanechnikov kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}1-|t|,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DTriangle(double t);

/*! \brief a 1D quartic kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{15}{16}(1-t^2)^2,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DQuartic(double t);
/*! \brief a 1D triweight kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{35}{32}(1-t^2)^3,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DTriweight(double t);

/*! \brief a 1D tricube kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{70}{81}(1-|t|^3)^3,  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DTricube(double t);
/*! \brief a 1D cosine kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_1dkde_kernels

    \f[ k(t) :=\begin{cases}\frac{\pi}{4}\cos\left(\frac{\pi}{2}t\right),  & \text{if }t\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel1DCosine(double t);













/*! \brief a 1D Gaussian kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_2dkde_kernels

    \f[ k(t_x, t_y):=\frac{1}{2\pi}\exp \left(-\frac{t_x^2+t_y^2}{2}\right) \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel2DGaussian(double tx, double ty);

/*! \brief a 1D Gaussian kernel function, e.g. for Kernel Density Estimation
    \ingroup jkqtptools_math_statistics_2dkde_kernels

    \f[ k(t_x, t_y):=\begin{cases}\frac{1}{4},  & \text{if }t_x,t_y\in [-1;1]\\0, & \text{else}\end{cases} \f]
*/
JKQTP_LIB_EXPORT double jkqtpstatKernel2DUniform(double tx, double ty);



















/*! \brief estimates a bandwidth for a Kernel Density Estimator (KDE) of the given data \a first ... \a last
    \ingroup jkqtptools_math_statistics_1dkde

    evaluates \f[ h = \left(\frac{4\hat{\sigma}^5}{3n}\right)^{\frac{1}{5}} \approx 1.06 \hat{\sigma} n^{-1/5} \f]

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \return the estimated bandwidth

*/
template <class InputIt>
inline double jkqtpstatEstimateKDEBandwidth(InputIt first, InputIt last) {
    size_t N=0;
    const double sigma=jkqtpstatStdDev(first, last, nullptr, &N);
    return 1.06*sigma/pow(static_cast<double>(N), 1.0/5.0);
}




/*! \brief evaluates the Kernel Density Estimator (KDE) at a given position
    \ingroup jkqtptools_math_statistics_1dkde

    evaluates \f[ \tilde{f}(t):=\frac{1}{N\cdot\text{bandwidth}}\cdot\sum\limits_{i=0}^{N-1}K\left(\frac{t-x_i}{\text{bandwidth}}\right) \f]

    \tparam InputIt standard iterator type of \a first and \a last.
    \param t where to evaluate the kernel sum
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE

*/
template <class InputIt>
inline double jkqtpstatEvaluateKernelSum(double t, InputIt first, InputIt last, const std::function<double(double)>& kernel, double bandwidth) {
    double res=0;
    size_t cnt=0;
    for (auto it=first; it!=last; ++it)  {
        const double v=jkqtp_todouble(*it);
        if (JKQTPIsOKFloat(v)) {
            const double vx=(t-v)/bandwidth;
            res+=kernel(vx);
            cnt++;
        }
    }
    if (cnt==0) return 0.0;
    return res/static_cast<double>(cnt)/bandwidth;
}




/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param Nout number datapoints in the output KDE
    \param cummulative if \c true, a cummulative KDE is calculated

    This function performs <a href="https://en.wikipedia.org/wiki/Kernel_density_estimation">Kernel Density Estimation</a> for a given data array.
    Then the resulting density is evaluated on a regular grid spanning [min(X)...max(X)] with bins datapoints in between.

    \warning this functions is getting very slow for large dataset, as for each point in the resulting histogram N kernel functions have to be evaluated.

    \see https://en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatKDE1DAutoranged(InputIt first, InputIt last, OutputIt KDEXOut, OutputIt KDEYOut, int Nout=100, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double range=maxV-minV;
    const double binw=range/static_cast<double>(Nout);

    // calculate the KDE
    for (double xi=minV; xi<=maxV; xi+=binw)  {
        histX.push_back(xi);
        histY.push_back(jkqtpstatEvaluateKernelSum(xi, first, last, kernel, bandwidth));
    }
    if (histX.size()>0 && histX[histX.size()-1]<maxV) {
        histX.push_back(maxV);
        histY.push_back(jkqtpstatEvaluateKernelSum(maxV, first, last, kernel, bandwidth));
    }



    // output the KDE
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *KDEYOut=h;
        ++KDEXOut;
        ++KDEYOut;
    }
}


/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param binWidth width of the bins
    \param cummulative if \c true, a cummulative KDE is calculated

    This function performs <a href="https://en.wikipedia.org/wiki/Kernel_density_estimation">Kernel Density Estimation</a> for a given data array.
    Then the resulting density is evaluated on a regular grid spanning [min(X)...max(X)] with bins datapoints in between.

    \warning this functions is getting very slow for large dataset, as for each point in the resulting histogram N kernel functions have to be evaluated.

    \see https://en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatKDE1DAutoranged(InputIt first, InputIt last, OutputIt KDEXOut, OutputIt KDEYOut, double binWidth, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;

    const double binw=binWidth;

    // calculate the KDE
    for (double xi=minV; xi<=maxV; xi+=binw)  {
        histX.push_back(xi);
        histY.push_back(jkqtpstatEvaluateKernelSum(xi, first, last, kernel, bandwidth));
    }
    if (histX.size()>0 && histX[histX.size()-1]<maxV) {
        histX.push_back(maxV);
        histY.push_back(jkqtpstatEvaluateKernelSum(maxV, first, last, kernel, bandwidth));
    }


    // output the KDE
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *KDEYOut=h;
        ++KDEXOut;
        ++KDEYOut;
    }

}



/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, bins defined the range \a binsFirst ... \a binsLast
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of KDE bins
    \param binsLast iterator pointing behind the last item in the set of KDE bins
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated

    \see https://en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class BinsInputIt, class OutputIt>
inline void jkqtpstatKDE1D(InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, OutputIt KDEXOut, OutputIt KDEYOut, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;


    // initialize the KDE
    for (auto it=binsFirst; it!=binsLast; ++it)  {
        histX.push_back(jkqtp_todouble(*it));
    }
    std::sort(histX.begin(), histX.end());

    // calculate the KDE
    for (auto it=histX.begin(); it!=histX.end(); ++it) {
        histY.push_back(jkqtpstatEvaluateKernelSum(*it, first, last, kernel, bandwidth));
    }


    // output the KDE
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *KDEYOut=h;
        ++KDEXOut;
        ++KDEYOut;
    }

}




/*! \brief calculate an autoranged 1-dimensional Kernel Density Estimation (KDE) from the given data range \a first ... \a last, evaluation positions are given by the range \a binXLeft ... \a binXRight (in steps of \a binxDelta )
    \ingroup jkqtptools_math_statistics_1dkde

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam OutputIt standard output iterator type used for the outliers output \a KDEXOut and \a KDEYOut, use e.g. std::back_inserter
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binXLeft first x-position, where to evaluate the KDE
    \param binXDelta distance between two x-positions at which the KDE is evaluated
    \param binXRight last x-position, where to evaluate the KDE
    \param[out] KDEXOut output iterator that receives x-positions of the KDE bins. Location of this value inside the bin range is defined by \a binXMode
    \param[out] KDEYOut output iterator that receives counts/frequencies of the KDE bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated

    \see https://en.wikipedia.org/wiki/Kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/
template <class InputIt, class OutputIt>
inline void jkqtpstatKDE1D(InputIt first, InputIt last, double binXLeft, double binXDelta, double binXRight, OutputIt KDEXOut, OutputIt KDEYOut, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false) {
    double minV=0, maxV=0;
    size_t N=0;
    jkqtpstatMinMax<InputIt>(first, last, minV, maxV, nullptr, nullptr, &N);

    std::vector<double> histX;
    std::vector<double> histY;


    // calculate the KDE
    for (double x=binXLeft; x<=binXRight; x+=binXDelta)  {
        histX.push_back(x);
        histY.push_back(jkqtpstatEvaluateKernelSum(x, first, last, kernel, bandwidth));
    }


    // output the KDE
    double h=0;
    for (size_t i=0; i<histX.size(); i++) {
        *KDEXOut=histX[i];
        if (cummulative) h+=histY[i];
        else h=histY[i];
        *KDEYOut=h;
        ++KDEXOut;
        ++KDEYOut;
    }

}









/*! \brief evaluates the Kernel Density Estimator (KDE) at a given position
    \ingroup jkqtptools_math_statistics_2dkde

    evaluates \f[ \tilde{f}(x,y):=\frac{1}{N\cdot\sqrt{\text{bandwidthx}}\cdot\sqrt{\text{bandwidthy}}}\cdot\sum\limits_{i=0}^{N-1}K\left(\frac{x-x_i}{\text{bandwidthx}},\frac{y-y_i}{\text{bandwidthy}}\right) \f]

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param x where to evaluate the kernel sum, x-coordinate
    \param y where to evaluate the kernel sum, y-coordinate
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param kernel the kernel function to use (e.g. jkqtpstatKernel2DGaussian() )
    \param bandwidthX x-bandwidth used for the KDE
    \param bandwidthY y-bandwidth used for the KDE

*/
template <class InputItX, class InputItY>
inline double jkqtpstatEvaluateKernelSum2D(double x, double y, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, const std::function<double(double,double)>& kernel, double bandwidthX, double bandwidthY) {
    double res=0;
    size_t cnt=0;
    auto itX=firstX;
    auto itY=firstY;
    for (; (itX!=lastX)&&(itY!=lastY); ++itX, ++itY)  {
        const double vx=jkqtp_todouble(*itX);
        const double vy=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(vx) && JKQTPIsOKFloat(vy)) {
            const double vvx=(x-vx)/bandwidthX;
            const double vvy=(y-vy)/bandwidthY;
            res+=kernel(vvx,vvy);
            cnt++;
        }
    }
    if (cnt==0) return 0.0;
    return res/static_cast<double>(cnt)/sqrt(bandwidthX*bandwidthY);
}




/*! \brief calculate an autoranged 2-dimensional Kernel Density Estimation (KDE) from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_2dkde

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
    \param kernel the kernel function to use (e.g. jkqtpstatKernel2DGaussian() )
    \param bandwidthX x-bandwidth used for the KDE
    \param bandwidthY y-bandwidth used for the KDE

    \see https://en.wikipedia.org/wiki/Multivariate_kernel_density_estimation, \ref JKQTPlotterBasicJKQTPDatastoreStatistics
*/

template <class InputItX, class InputItY, class OutputIt>
inline void jkqtpstatKDE2D(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, OutputIt histogramImgOut, double xmin, double xmax, double ymin, double ymax, size_t xbins, size_t ybins, const std::function<double(double,double)>& kernel=std::function<double(double,double)>(&jkqtpstatKernel2DGaussian), double bandwidthX=1.0, double bandwidthY=1.0) {

    const double binwx=fabs(xmax-xmin)/static_cast<double>(xbins);
    const double binwy=fabs(ymax-ymin)/static_cast<double>(ybins);

    double y=ymin;
    auto itOut=histogramImgOut;
    for (size_t by=0; by<ybins; by++) {
        double x=xmin;
        for (size_t bx=0; bx<xbins; bx++) {
            const double vv=jkqtpstatEvaluateKernelSum2D(x,y, firstX, lastX,firstY,lastY, kernel, bandwidthX,bandwidthY);
            *itOut=vv;
            //std::cout<<x<<","<<y<<","<<vv<<*itOut<<std::endl;
            x+=binwx;
            ++itOut;
        }
        y+=binwy;
    }
}




/*! \brief estimates a bandwidth for a 2-dimensional Kernel Density Estimator (KDE) of the given data \a first ... \a last using Scott's rule
    \ingroup jkqtptools_math_statistics_2dkde

    evaluates \f[ h =  \hat{\sigma} n^{-1/(d+4)},\ \ \ \ \ d=2 \f]

    \tparam InputIt standard iterator type of \a first and \a last.
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \return the estimated bandwidth

    \see https://en.wikipedia.org/wiki/Multivariate_kernel_density_estimation#Rule_of_thumb

*/
template <class InputIt>
inline double jkqtpstatEstimateKDEBandwidth2D(InputIt first, InputIt last) {
    size_t N=0;
    const double sigma=jkqtpstatStdDev(first, last, nullptr, &N);
    return sigma/pow(static_cast<double>(N), 1.0/(2.0+4.0));
}



#endif // JKQTPSTATKDE_H_INCLUDED


