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


#ifndef JKQTPSTATPOLY_H_INCLUDED
#define JKQTPSTATPOLY_H_INCLUDED

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






/*! \brief fits (in a least-squares sense) a polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ of order P to a set of N data pairs \f$ (x_i,y_i) \f$
    \ingroup jkqtptools_math_statistics_poly

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam OutputItP output iterator for the polynomial coefficients
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param P degree of the polynomial (P>=N !!!)
    \param[out] firstRes Iterator (of type \a OutputItP ), which receives the (P+1)-entry vector with the polynomial coefficients \f$ p_i \f$

    This function uses jkqtpstatLinSolve() to solve the system of equations
      \f[ \begin{bmatrix} y_1\\ y_2\\ y_3 \\ \vdots \\ y_n \end{bmatrix}= \begin{bmatrix} 1 & x_1 & x_1^2 & \dots & x_1^P \\ 1 & x_2 & x_2^2 & \dots & x_2^P\\ 1 & x_3 & x_3^2 & \dots & x_3^P \\ \vdots & \vdots & \vdots & & \vdots \\ 1 & x_n & x_n^2 & \dots & x_n^P \end{bmatrix} \begin{bmatrix} p_0\\ p_1\\ p_2\\ \vdots \\ p_P \end{bmatrix}  \f]
      \f[ \vec{y}=V\vec{p}\ \ \ \ \ \Rightarrow\ \ \ \ \ \vec{p}=(V^TV)^{-1}V^T\vec{y} \f]

    \image html datastore_regression_polynom.png

    \see https://en.wikipedia.org/wiki/Polynomial_regression
*/
template <class InputItX, class InputItY, class OutputItP>
inline void jkqtpstatPolyFit(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t P, OutputItP firstRes) {
    {
        const int Nx=std::distance(firstX,lastX);
        const int Ny=std::distance(firstY,lastY);
        JKQTPASSERT(Nx>1 && Ny>1);
    }

    size_t N=0;

    std::vector<double> X,Y;
    auto itX=firstX;
    auto itY=firstY;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            X.push_back(fit_x);
            Y.push_back(fit_y);
            N++;
        }
    }

    // build Vandermonde matrix V
    std::vector<double> V;
    V.resize(N*(P+1));
    for (size_t l=0; l<N; l++) {
        V[jkqtplinalgMatIndex(l,0,P+1)]=1.0;
        double x=X[l];
        const double xx=x;
        for (size_t c=1; c<P+1; c++) {
            V[jkqtplinalgMatIndex(l,c,P+1)]=x;
            x=x*xx;
        }
    }
#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V = \n";
    jkqtplinalgPrintMatrix(V.data(),N,P+1);
    std::cout<<"\n";
#endif

    // calculate V^T
   std::vector<double> VT=V;
    jkqtplinalgTransposeMatrix(VT.data(), static_cast<long>(N), static_cast<long>(P+1));

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V^T = \n";
    jkqtplinalgPrintMatrix(VT.data(),P+1,N);
    std::cout<<"\n";
#endif

    // calculate V^T*V
    std::vector<double> VTV;
    VTV.resize((P+1)*(P+1));
    jkqtplinalgMatrixProduct(VT.data(), static_cast<long>(P+1), static_cast<long>(N), V.data(), static_cast<long>(N), static_cast<long>(P+1), VTV.data());

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V^T*V = \n";
    jkqtplinalgPrintMatrix(VTV.data(),P+1,P+1);
    std::cout<<"\n";
#endif

    // calculate V^T*y
    std::vector<double> VTY;
    VTY.resize(P+1);
    jkqtplinalgMatrixProduct(VT.data(), static_cast<long>(P+1), static_cast<long>(N), Y.data(), static_cast<long>(N), 1, VTY.data());

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"V^T*y = \n";
    jkqtplinalgPrintMatrix(VTY.data(),P+1,1);
    std::cout<<"\n";
#endif

    // solve V^T*y = V^T*V*p
    const bool ok=jkqtplinalgLinSolve(VTV.data(), VTY.data(), static_cast<long>(P+1));

    if (ok) {
        auto itR=firstRes;
        for (size_t p=0; p<P+1; p++) {
            *itR=VTY[p];
            ++itR;
        }
    } else {
        throw std::runtime_error("jkqtplinalgLinSolve() didn't return a result!");
    }

#ifdef STATISTICS_TOOLS_DEBUG_statisticsPolyFit
    std::cout<<"result_out = \n";
    jkqtplinalgPrintMatrix(result_out,P+1,1);
    std::cout<<"\n";
#endif

}





#endif // JKQTPSTATPOLY_H_INCLUDED


