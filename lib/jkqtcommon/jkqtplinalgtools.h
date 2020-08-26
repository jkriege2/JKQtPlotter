/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

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


#ifndef JKQTPLINALGTOOLS_H_INCLUDED
#define JKQTPLINALGTOOLS_H_INCLUDED

#include <stdint.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <vector>
#include <utility>
#include <cfloat>
#include <ostream>
#include <iomanip>
#include <sstream>
#include "jkqtcommon/jkqtcommon_imexport.h"
#include "jkqtcommon/jkqtparraytools.h"
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtcommon/jkqtpstringtools.h"

#ifdef _OPENMP
# include <omp.h>
#endif


#ifndef JKQTP_ALIGNMENT_BYTES
#define JKQTP_ALIGNMENT_BYTES 32
#endif


#ifdef JKQTP_STATISTICS_TOOLS_MAY_USE_EIGEN3
#  include <Eigen/Core>
#  include <Eigen/SVD>
#  include <Eigen/Jacobi>
#  include <Eigen/LU>
#  include <Eigen/QR>
#endif





/** \brief calculate the index of the entry in line \a l and column \a c
 *         in a row-major matrix with \a C columns
 *  \ingroup jkqtptools_math_linalg
 *
 * You can use this to access a matrix with L rows and C columns:
 * \code
 *     for (long l=0; l<L; l++) {
 *       for (long c=0; c<C; c++) {
 *         matrix[jkqtplinalgMatIndex(l,c,C)=0;
 *         if (l==c) matrix[jkqtplinalgMatIndex(l,c,C)=1;
 *       }
 *     }
 * \endcode
 */
#define jkqtplinalgMatIndex(l,c,C) ((l)*(C)+(c))


/** \brief print the given LxC matrix to std::cout
 *  \ingroup jkqtptools_math_linalg
 *
 *  \tparam type of the matrix cells (typically double or float)
 *  \param matrix the matrix to print out
 *  \param L number of lines/rows in the matrix
 *  \param C number of columns in the matrix
 *  \param width width (in characters) of each cell in the output (used for formatting)
 *  \param precision precision (in digits) for string-conversions in the output (used for formatting)
 *  \param mode if \c =='f' the mode \c std::fixed is used for output, otherwise \c std::scientific is used
*/
template <class T>
inline void jkqtplinalgPrintMatrix(T* matrix, long L, long C, int width=9, int precision=3, char mode='f') {
    for (long l=0; l<L; l++) {
        for (long c=0; c<C; c++) {
            if (c>0) std::cout<<", ";
            std::cout.precision(precision);
            std::cout.width(width);
            if (mode=='f') std::cout<<std::fixed<<std::right<<matrix[jkqtplinalgMatIndex(l,c,C)];
            else std::cout<<std::scientific<<std::right<<matrix[jkqtplinalgMatIndex(l,c,C)];
        }
        std::cout<<std::endl;
    }
}



/** \brief convert the given LxC matrix to std::string
 *  \ingroup jkqtptools_math_linalg
 *
 *  \tparam type of the matrix cells (typically double or float)
 *  \param matrix the matrix to convert
 *  \param L number of lines/rows in the matrix
 *  \param C number of columns in the matrix
 *  \param width width (in characters) of each cell in the output (used for formatting)
 *  \param precision precision (in digits) for string-conversions in the output (used for formatting)
 *  \param mode the (printf()) string conversion mode for output of the cell values
*/
template <class T>
inline std::string jkqtplinalgMatrixToString(T* matrix, long L, long C, int width=9, int precision=3, const std::string& mode=std::string("g")) {
    std::string format="%"+jkqtp_inttostr(width)+std::string(".")+jkqtp_inttostr(precision)+std::string("l")+mode;
    std::ostringstream ost;
    for (long l=0; l<L; l++) {
        for (long c=0; c<C; c++) {
            if (c>0) ost<<", ";
            char buf[500];
            sprintf(buf, format.c_str(), jkqtp_todouble(matrix[jkqtplinalgMatIndex(l,c,C)]));
            ost<<buf;
            /*ost.precision(precision);
            ost.width(width);
            if (mode=='f') ost<<std::fixed<<std::right<<matrix[jkqtplinalgMatIndex(l,c,C)];
            else ost<<std::scientific<<std::right<<matrix[jkqtplinalgMatIndex(l,c,C)];*/

        }
        ost<<std::endl;
    }
    return ost.str();
}

/** \brief maps the number range -1 ... +1 to a color-scale lightblue - white - lightred (used for coloring matrices!)
 *  \ingroup jkqtptools_math_linalg
 *
 *  \param val the value to convert
 *  \param[out] r returns the red value (0..255)
 *  \param[out] g returns the green value (0..255)
 *  \param[out] b returns the blue value (0..255)
 */
inline void jkqtplinalgPM1ToRWBColors(double val, uint8_t& r, uint8_t& g, uint8_t& b){
    r=255;
    g=255;
    b=255;
    const double fval=fabs(val);
    if (val<0 && val>=-1) {
        r=jkqtp_boundedRoundTo<uint8_t>(0,255.0-fval*127.0,255);
        g=jkqtp_boundedRoundTo<uint8_t>(0,255.0-fval*127.0,255);
    } else if (val>0 && val<=1) {
        b=jkqtp_boundedRoundTo<uint8_t>(0,255.0-fval*127.0,255);
        g=jkqtp_boundedRoundTo<uint8_t>(0,255.0-fval*127.0,255);
    } else if (val<-1) {
        r=127;
        g=127;
        b=255;
    } else if (val>1) {
        r=255;
        g=127;
        b=127;
    }
}

/** \brief maps the number range -1 ... +1 to a non-linear color-scale lightblue - white - lightred (used for coloring matrices!)
 *  \ingroup jkqtptools_math_linalg
 *
 *  \param val the value to convert
 *  \param[out] r returns the red value (0..255)
 *  \param[out] g returns the green value (0..255)
 *  \param[out] b returns the blue value (0..255)
 *  \param gamma a gamma-value for non-linear color scaling
*/
inline void jkqtplinalgPM1ToNonlinRWBColors(double val, uint8_t& r, uint8_t& g, uint8_t& b, double gamma=0.5){
    if (val<0) {
        jkqtplinalgPM1ToRWBColors(-1.0*pow(-val,gamma),r,g,b);
    } else {
        jkqtplinalgPM1ToRWBColors(pow(val,gamma),r,g,b);
    }
}
/** \brief convert the given LxC matrix to std::string, encoded as HTML table
 *  \ingroup jkqtptools_math_linalg
 *
 *
 *  \tparam type of the matrix cells (typically double or float)
 *  \param matrix the matrix to convert
 *  \param L number of lines/rows in the matrix
 *  \param C number of columns in the matrix
 *  \param width width (in characters) of each cell in the output (used for formatting)
 *  \param precision precision (in digits) for string-conversions in the output (used for formatting)
 *  \param mode the (printf()) string conversion mode for output of the cell values
 *  \param tableformat this is inserted into the \c <table...> tag (may contain HTML property definitions)
 *  \param prenumber this is inserted before each number (may contain HTML markup)
 *  \param postnumber this is inserted after each number (may contain HTML markup)
 *  \param colorcoding if \c true, teh cell backgrounds are color-coded
 *  \param zeroIsWhite if \c the color-coding is forced to white for 0 and then encodes in positive/negative direction with colors (red and blue)
 *  \param[out] colorlabel outputs a label explaining the auto-generated color-coding
 *  \param nonlinColors if \c true, a non-linear color-coding is used
 *  \param nonlinColorsGamma gamma-value for a non-linear color-coding
 *  \param colortableformat  lie \a tableformat, but for the legend table output in \a colorLabel
 *
 * \see jkqtplinalgPM1ToNonlinRWBColors() and jkqtplinalgPM1ToRWBColors()
*/
template <class T>
inline std::string jkqtplinalgMatrixToHTMLString(T* matrix, long L, long C, int width=9, int precision=3, const std::string& mode=std::string("g"), const std::string& tableformat=std::string(), const std::string& prenumber=std::string(), const std::string& postnumber=std::string(), bool colorcoding=false, bool zeroIsWhite=true, std::string* colorlabel=nullptr, bool nonlinColors=false, double nonlinColorsGamma=0.25, const std::string& colortableformat=std::string()) {
    std::ostringstream ost;
    ost<<"<table "<<tableformat<<">\n";
    std::string format="%"+jkqtp_inttostr(width)+std::string(".")+jkqtp_inttostr(precision)+std::string("l")+mode;
    double minv=0, maxv=0;
    if (colorcoding) {
        jkqtpstatMinMax(matrix, L*C, minv, maxv);
    }
    for (long l=0; l<L; l++) {
        ost<<"  <tr>";
        for (long c=0; c<C; c++) {
            const double val=matrix[jkqtplinalgMatIndex(l,c,C)];
            std::string cols="";
            if (colorcoding) {
                double valrel=0;
                uint8_t r=255,g=255,b=255;
                if (zeroIsWhite){
                    if (val<0) valrel=-1.0*fabs(val/minv);
                    if (val>0) valrel=fabs(val/maxv);
                } else {
                    valrel=((val-minv)/(maxv-minv)-0.5)*2.0;
                }
                if (nonlinColors) {
                    jkqtplinalgPM1ToNonlinRWBColors(valrel, r,g,b, nonlinColorsGamma);
                } else {
                    jkqtplinalgPM1ToRWBColors(valrel, r,g,b);
                }
                char buf[500];
                sprintf(buf, " bgcolor=\"#%02X%02X%02X\"", int(r), int(g), int(b));
                cols=buf;
            }
            ost<<"<td align=\"center\" valign=\"middle\" width=\""<<(100.0/double(C))<<"%\" "<<cols<<"><nobr>";
            ost.precision(precision);
            ost.width(width);
            char buf[500];
            sprintf(buf, format.c_str(), val);
            ost<<prenumber<<buf<<postnumber;
            ost<<"</nobr></td>";
        }
        ost<<"</tr>\n";
    }
    ost<<"</table>";

    if (colorcoding && colorlabel) {
        char buf[8192];
        uint8_t rm=255,gm=255,bm=255;
        uint8_t rmc=255,gmc=255,bmc=255;
        uint8_t rc=255,gc=255,bc=255;
        uint8_t rcp=255,gcp=255,bcp=255;
        uint8_t rp=255,gp=255,bp=255;
        double vm=minv;
        double vc=0;
        double vp=maxv;
        if (!zeroIsWhite) {
            vc=(maxv+minv)/2.0;
        }
        if (nonlinColors) {
            jkqtplinalgPM1ToNonlinRWBColors(-1, rm, gm, bm, nonlinColorsGamma);
            jkqtplinalgPM1ToNonlinRWBColors(-0.5, rmc, gmc, bmc, nonlinColorsGamma);
            jkqtplinalgPM1ToNonlinRWBColors(0, rc, gc, bc, nonlinColorsGamma);
            jkqtplinalgPM1ToNonlinRWBColors(0.5, rcp, gcp, bcp, nonlinColorsGamma);
            jkqtplinalgPM1ToNonlinRWBColors(1, rp, gp, bp, nonlinColorsGamma);
        } else {
            jkqtplinalgPM1ToRWBColors(-1, rm, gm, bm);
            jkqtplinalgPM1ToRWBColors(-0.5, rmc, gmc, bmc);
            jkqtplinalgPM1ToRWBColors(0, rc, gc, bc);
            jkqtplinalgPM1ToRWBColors(0.5, rcp, gcp, bcp);
            jkqtplinalgPM1ToRWBColors(1, rp, gp, bp);
        }
        sprintf(buf, "<table %s cellpadding=\"2\" cellspacing=\"0\" border=\"1\"><tr><td><table width=\"100%%\" cellpadding=\"3\" cellspacing=\"0\" border=\"0\"><tr>"
                     "<td bgcolor=\"#%02X%02X%02X\" width=\"20%%\"><nobr>&nbsp;%9.3lg&nbsp;</nobr></td>"
                     "<td bgcolor=\"#%02X%02X%02X\" width=\"20%%\"><nobr>&nbsp;&nbsp;&nbsp;&mdash;&nbsp;&nbsp;&nbsp;</nobr></td>"
                     "<td bgcolor=\"#%02X%02X%02X\" width=\"20%%\"><nobr>&nbsp;%9.3lg&nbsp;</nobr></td>"
                     "<td bgcolor=\"#%02X%02X%02X\" width=\"20%%\"><nobr>&nbsp;&nbsp;&nbsp;&mdash;&nbsp;&nbsp;&nbsp;</nobr></td>"
                     "<td bgcolor=\"#%02X%02X%02X\" width=\"20%%\"><nobr>&nbsp;%9.3lg&nbsp;</nobr></td>"
                     "</tr></table></td></tr></table>", colortableformat.c_str(), int(rm), int(gm), int(bm), vm, int(rmc), int(gmc), int(bmc), int(rc), int(gc), int(bc), vc, int(rcp), int(gcp), int(bcp), int(rp), int(gp), int(bp), vp);
        (*colorlabel)=std::string(buf);
    }
    return ost.str();
}

/** \brief dot-product between two vectors \a vec1 and \a vec2, each with a length of \a N entries
  * \ingroup jkqtptools_math_linalg
  *
  *  \tparam T of the vector cells (typically double or float)
  *  \param vec1 first vector
  *  \param vec2 second vector
  *  \param N number of entries in \a vec1 and \a vec2
  */
template <class T>
inline T jkqtplinalgDotProduct(const T* vec1, const T* vec2, long N) {
    T res=0;
    for (long l=0; l<N; l++) {
        res=res+vec1[l]*vec2[l];
    }
    return res;
}


/** \brief transpose the given NxN matrix
 *  \ingroup jkqtptools_math_linalg
 *
 *  \tparam T of the matrix cells (typically double or float)
 *  \param matrix the matrix to transpose
 *  \param N number of rows and columns in the matrix
 *
 */
template <class T>
inline void jkqtplinalgTransposeMatrix(T* matrix, long N) {
    for (long l=0; l<N; l++) {
        for (long c=l+1; c<N; c++) {
            jkqtpArraySwap(matrix, jkqtplinalgMatIndex(l,c,N), jkqtplinalgMatIndex(c,l,N));
        }
    }
}


/** \brief transpose the given LxC matrix
 *  \ingroup jkqtptools_math_linalg
 *
 *  \tparam T of the matrix cells (typically double or float)
 *  \param matrix the matrix to transpose
 *  \param L number of rows in the matrix
 *  \param C number of columns in the matrix
 *
 *  \note  The output is interpreted as CxL matrix!!!
 */
template <class T>
inline void jkqtplinalgTransposeMatrix(T* matrix, long L, long C) {
    JKQTPArrayScopedPointer<T> t(jkqtpArrayDuplicate<T>(matrix, L*C));
    for (long l=0; l<L; l++) {
        for (long c=0; c<C; c++) {
            matrix[jkqtplinalgMatIndex(c,l,L)]=t[jkqtplinalgMatIndex(l,c,C)];
        }
    }
}

/** \brief swap two lines in a matrix
 *  \ingroup jkqtptools_math_linalg
 *
 *  \tparam T of the matrix cells (typically double or float)
 *  \param m the matrix to work on
 *  \param l1 the row to swap with row \a l2
 *  \param l2 the row to swap with row \a l1
 *  \param C number of columns in the matrix
*/
template <class T>
inline void jkqtplinalgMatrixSwapLines(T* m, long l1, long l2, long C) {
    for (long c=0; c<C; c++) {
        jkqtpArraySwap(m, jkqtplinalgMatIndex(l1, c, C), jkqtplinalgMatIndex(l2, c, C));
    }
}


/*! \brief matrix-matrix product
    \ingroup jkqtptools_math_linalg

   \tparam T of the matrix cells (typically double or float)
    \param M1 matrix 1, size: L1xC1
    \param L1 number of rows in the matrix \a M1
    \param C1 number of columns in the matrix \a M1
    \param M2 matrix 1, size: L2xC2
    \param L2 number of rows in the matrix \a M2
    \param C2 number of columns in the matrix \a M2
    \param[out] M output matrix M=M1*M2, size: L1xC2
*/
template <class T>
inline void jkqtplinalgMatrixProduct(const T* M1, long L1, long C1, const T* M2, long L2, long C2, T* M) {
    if (M1!=M &&M2!=M) {

        for (long l=0; l<L1; l++) {
            for (long c=0; c<C2; c++) {
                double s=T(0);
                for (long i=0; i<C1; i++) {
                    s = s + M1[jkqtplinalgMatIndex(l, i, C1)]*M2[jkqtplinalgMatIndex(i,c, C2)];
                }
                M[jkqtplinalgMatIndex(l, c, C2)]=s;
            }
        }
    } else if (M1==M && M2!=M) {
        JKQTPArrayScopedPointer<T> MM(jkqtpArrayDuplicate(M1, L1*C1));
        jkqtplinalgMatrixProduct(MM.data(),L1,C1,M2,L2,C2,M);
    } else if (M1!=M && M2==M) {
        JKQTPArrayScopedPointer<T> MM(jkqtpArrayDuplicate(M1, L1*C1));
        jkqtplinalgMatrixProduct(M1,L1,C1,MM.data(),L2,C2,M);
    } else if (M1==M && M2==M) {
        JKQTPArrayScopedPointer<T> MM(jkqtpArrayDuplicate(M1, L1*C1));
        jkqtplinalgMatrixProduct(MM.data(),L1,C1,MM.data(),L2,C2,M);
    }
}


/*! \brief matrix-matrix product of two NxN matrices
    \ingroup jkqtptools_math_linalg

    \param M1 matrix 1, size: NxN
    \param M2 matrix 1, size: NxN
    \param N number os rows/columns in the matrices \a M1, \a M2 and \a M
    \param[out] M output matrix M=M1*M2, size: NxN
*/
template <class T>
inline void jkqtplinalgMatrixProduct(const T* M1, const T* M2, long N, T* M) {
    jkqtplinalgMatrixProduct(M1,N,N,M2,N,N,M);
}






/*! \brief performs a Gauss-Jordan eliminaion on a LxC matrix m
    \ingroup jkqtptools_math_linalg

    For a matrix equation \f[ A\cdot\vec{x}=\vec{b} \f] the input matrix is \f[ \left[A | \vec{b}\right] \f] for matrix inversion it is
    \f[ \left[A | I_L\right] \f] where \f$ I_L \f$ is the unit matrix with LxL entries.

    \tparam T of the matrix cells (typically double or float)
    \param m the matrix
    \param L number of rows in the matrix
    \param C number of columns in the matrix

    \see http://www.virtual-maxim.de/matrix-invertieren-in-c-plus-plus/
*/
template <class T>
inline bool jkqtplinalgGaussJordan(T* m, long L, long C) {

    const long N=L;

    //std::cout<<"\nstep 0:\n";
    //linalgPrintMatrix(m, N, C);

    // first we perform a Gauss-elimination, which transforms the  matrix in the left half of m into upper triangular form
    for (long k=0; k<N-1; k++) {
        //std::cout<<"\nstep G"<<k<<": pivot="<<m[jkqtpstatisticsMatIndex(k,k,C)]<<"\n";
        if (m[jkqtplinalgMatIndex(k,k,C)]==0.0) {
            // if pivot(m[k,k])==0, then swap this line with a line, which does not have a 0 in the k-th column
            for (long ks=k+1; ks<N; ks++) {
                if (m[jkqtplinalgMatIndex(ks,k,C)]!=0.0) {
                    jkqtplinalgMatrixSwapLines(m, ks, k, C);
                    break;
                } else if (ks==N-1) {
                    // if no such element is found, the matrix may not be inverted!
                    return false;
                }
            }
        }

        // now we can eliminate all entries i below the pivot line p, by subtracting
        // the pivot line, scaled by s, from every line, where
        // s=m[i,p]/m[p,p]
        for (long i=k+1; i<N; i++) {
            const T s=m[jkqtplinalgMatIndex(i,k,C)]/m[jkqtplinalgMatIndex(k,k,C)];
            for (long c=k; c<C; c++) {
                m[jkqtplinalgMatIndex(i,c,C)] -= m[jkqtplinalgMatIndex(k,c,C)]*s;
            }
        }

        //linalgPrintMatrix(m, N, C);
    }

    // now we can caluate the determinant of the left half-matrix, which can be used to determine, whether matrix
    // is invertible at all: if det(T)==0.0 -> matrix is not invertible
    // the determinant of an upper triangular marix equals the product of the diagonal elements
    T det=1.0;
    for (long k=0; k<N; k++) {
        det = det * m[jkqtplinalgMatIndex(k,k,C)];
    }
    //linalgPrintMatrix(m, N, C);
    //std::cout<<"\nstep 2:  det(M)="<<det<<"\n";
    if (fabs(det)<DBL_MIN*10.0) return false;


    // if the matrix may be inverted, we can go on with the JOrdan part of the algorithm:
    // we work the Nx(2N) matrix from bottom to top and transform the left side into a unit matrix
    //   - the last row is left unchanged
    //   - the last row is subtracted from every row i above, scaled by m[i,N]/m[N,N]
    // then we repeat this for the (N-1)*(N-1) left upper matrix, which has again full triangular form
    for (long k=N-1; k>0; k--) {
        //std::cout<<"\nstep J"<<k<<":\n";
        for (long i=k-1; i>=0; i--) {
            const T s=m[jkqtplinalgMatIndex(i,k,C)]/m[jkqtplinalgMatIndex(k,k,C)];
            for (long c=k; c<C; c++) {
                m[jkqtplinalgMatIndex(i,c,C)] -= m[jkqtplinalgMatIndex(k,c,C)]*s;
            }
        }
        //linalgPrintMatrix(m, N, C);
    }
    // finally each line is normalized to 1 by dividing by the diagonal entry in the left NxN matrix
    // and copy the result to matrix_out
    for (long k=0; k<N; k++) {

        const T s=m[jkqtplinalgMatIndex(k,k,C)];
        for (long c=N; c<C; c++) {
            m[jkqtplinalgMatIndex(k,c,C)] = m[jkqtplinalgMatIndex(k,c,C)] / s;
        }
        m[jkqtplinalgMatIndex(k,k,C)]=T(1);
    }


    return true;
}








/*! \brief invert the given NxN matrix using Gauss-Jordan elimination
    \ingroup jkqtptools_math_linalg

    \tparam T of the matrix cells (typically double or float)
    \param matrix the matrix to invert
    \param[out] matrix_out target for the inverted matrix
    \param N number of rows and columns in the matrix

    \return \c true on success and the inverted matrix in matrix_out.

    \note It is save to call \c jkqtpstatMatrixInversion(A,A,N) with the same argument for in and out matrix. Then the input will be overwritten with the new matrix!
    \note matrix and matrix_out have to be of size N*N. Matrices are interpreted as row-major!

    \see http://www.virtual-maxim.de/matrix-invertieren-in-c-plus-plus/
*/
template <class T>
inline bool jkqtplinalgMatrixInversion(const T* matrix, T* matrix_out, long N) {
#ifdef JKQTP_STATISTICS_TOOLS_MAY_USE_EIGEN3
    if (N==2) {
        Eigen::Map<const Eigen::Matrix<T,2,2,Eigen::RowMajor> > eA(matrix);
        Eigen::Map<Eigen::Matrix<T,2,2,Eigen::RowMajor> > eO(matrix_out);
        eO=eA.inverse();
        //std::cout<<"\n--------------------------------------\n2x2 input matrix\n"<<eA<<"\n--------------------------------------\n";
        return eO.allFinite();
    } else if (N==3) {
        Eigen::Map<const Eigen::Matrix<T,3,3,Eigen::RowMajor> > eA(matrix);
        Eigen::Map<Eigen::Matrix<T,3,3,Eigen::RowMajor> > eO(matrix_out);
        //std::cout<<"\n--------------------------------------\n3x3 input matrix\n"<<eA<<"\n--------------------------------------\n";
        eO=eA.inverse();
        return eO.allFinite();
    } else if (N==4) {
        Eigen::Map<const Eigen::Matrix<T,4,4,Eigen::RowMajor> > eA(matrix);
        Eigen::Map<Eigen::Matrix<T,4,4,Eigen::RowMajor> > eO(matrix_out);
        //std::cout<<"\n--------------------------------------\n4x4 input matrix\n"<<eA<<"\n--------------------------------------\n";
        eO=eA.inverse();
        return eO.allFinite();
    } else {
        Eigen::Map<const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > eA(matrix,N,N);
        Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > eO(matrix_out,N,N);
        eO=eA.inverse();
        //std::cout<<"\n--------------------------------------\nNxN input matrix\n"<<eA<<"\n--------------------------------------\n";
        return eO.allFinite();
    }
    return true;
#else
    // first build a N*(2N) matrix of the form
    //
    //    m11 m12 ...  | 1 0 0
    //    m21 m22 ...  | 0 1 0
    //    ... ... ...  | .....
    //
    const long msize=N*N*2;
    std::vector<T> m;
    m.resize(msize);
    for (long i=0; i<msize; i++) m[i]=T(0); // init with 0
    for (long l=0; l<N; l++) {
        for (long c=0; c<N; c++) { // init left half with matrix
            m[jkqtplinalgMatIndex(l,c,2*N)]=matrix[jkqtplinalgMatIndex(l,c,N)];
        }
        // init right half with unit matrix
        m[jkqtplinalgMatIndex(l,N+l,2*N)]=T(1);
    }


    bool ok=jkqtplinalgGaussJordan(m.data(), N, 2*N);

    if (ok) {
        // finally we copy the result to matrix_out
        for (long k=0; k<N; k++) {
            for (long c=N; c<2*N; c++) {
                matrix_out[jkqtplinalgMatIndex(k,c-N,N)] = m[jkqtplinalgMatIndex(k,c,2*N)];
            }
        }
    }

    return ok;
#endif
}





/*! \brief invert the given NxN matrix using Gauss-Jordan elimination
    \ingroup jkqtptools_math_linalg

    \tparam T of the matrix cells (typically double or float)
    \param[in,out] matrix the matrix to invert (at the same time the target)
    \param N number of rows and columns in the matrix
*/
template <class T>
inline bool jkqtplinalgMatrixInversion(T* matrix, long N) {
    return jkqtplinalgMatrixInversion(matrix, matrix, N);
}



/*! \brief solve a system of N linear equations \f$ A\cdot\vec{x}=B \f$ simultaneously for C columns in B
    \ingroup jkqtptools_math_linalg

    \param A an NxN matrix of coefficients
    \param B an NxC marix
    \param N number of equations
    \param C number of columns in B
    \param result_out a NxC matrix with the results after the inversion of the system of equations
    \return \c true on success

    \note This function uses the Gauss-Jordan algorithm
    \note It is save to call \c jkqtpstatLinSolve(A,B,N,C,B) with the same argument for B and result_out. Then the input will be overwritten with the new matrix!
*/
template <class T>
inline bool jkqtplinalgLinSolve(const T* A, const T* B, long N, long C, T* result_out) {
#if defined(JKQTP_STATISTICS_TOOLS_MAY_USE_EIGEN3) && (!defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_noeigen))
    if (N==2 && C==1) {
        Eigen::Map<const Eigen::Matrix<T,2,2,Eigen::RowMajor> > eA(A);
        Eigen::Matrix<T,2,1>  eB;
        Eigen::Map<Eigen::Matrix<T,2,1> > x(result_out);

        eB=Eigen::Map<const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> >(B,2,1);
#  ifdef STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_fullPivLu
        x=eA.fullPivLu().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_householderQr)
        x=eA.householderQr().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_fullPivHouseholderQr)
        x=eA.fullPivHouseholderQr().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_jacobiSvd)
        x=eA.jacobiSVD().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_colPivHouseholderQr)
        x=eA.colPivHouseholderQr().solve(eB);
#  else
        x=eA.fullPivLu().solve(eB);
#  endif
    } else if (N==3 && C==1) {
        Eigen::Map<const Eigen::Matrix<T,3,3,Eigen::RowMajor> > eA(A);
        Eigen::Matrix<T,3,1>  eB;
        Eigen::Map<Eigen::Matrix<T,3,1> > x(result_out);

        eB=Eigen::Map<const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> >(B,3,1);

#  ifdef STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_fullPivLu
        x=eA.fullPivLu().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_householderQr)
        x=eA.householderQr().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_fullPivHouseholderQr)
        x=eA.fullPivHouseholderQr().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_jacobiSvd)
        x=eA.jacobiSVD().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_colPivHouseholderQr)
        x=eA.colPivHouseholderQr().solve(eB);
#  else
        x=eA.fullPivLu().solve(eB);
#  endif
    } else {
        Eigen::Map<const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > eA(A,N,N);
        Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor>  eB(N,C);
        Eigen::Map<Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > x(result_out,N,C);

        eB=Eigen::Map<const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> >(B,N,C);

#  ifdef STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_fullPivLu
        x=eA.fullPivLu().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_householderQr)
        x=eA.householderQr().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_fullPivHouseholderQr)
        x=eA.fullPivHouseholderQr().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_jacobiSvd)
        x=eA.jacobiSVD().solve(eB);
#  elif defined(STATISTICS_TOOLS_linalgLinSolve_EIGENMETHOD_colPivHouseholderQr)
        x=eA.colPivHouseholderQr().solve(eB);
#  else
        x=eA.fullPivLu().solve(eB);
#  endif
    }
    return true;
#else
    // first build a N*(N+C) matrix of the form
    //
    //    <---- N ----> <---- C ---->
    //  ^ A11 A12 ...  | B11 B12 ...
    //  | A21 A22 ...  | B21 B22 ...
    //  N ... ... ...  | .....
    //  | ... ... ...  | .....
    //  v ... ... ...  | .....
    //
    const long msize=N*(N+C);
    JKQTPArrayScopedPointer<T> m(static_cast<T*>(malloc(msize*sizeof(T)))); // use scoped pointer to ensure, that m is free'd, when the function is ending
    for (long l=0; l<N; l++) {
        for (long c=0; c<N; c++) { // init left half with matrix A
            m[jkqtplinalgMatIndex(l,c,N+C)]=A[jkqtplinalgMatIndex(l,c,N)];
        }
        // init right half with B
        for (long c=0; c<C; c++) { // init left half with matrix B
            m[jkqtplinalgMatIndex(l,N+c,N+C)]=B[jkqtplinalgMatIndex(l,c,C)];
        }
    }


    bool ok=jkqtplinalgGaussJordan(m.data(), N, N+C);

    if (ok) {
        for (long k=0; k<N; k++) {
            for (long c=N; c<(N+C); c++) {
                if (!JKQTPIsOKFloat(m[jkqtplinalgMatIndex(k,c,N+C)])) {
                    ok=false;
                    break;
                }
            }
            if (!ok) break;
        }
        if (ok) {
            // finally we copy the result to matrix_out
            for (long k=0; k<N; k++) {
                for (long c=N; c<(N+C); c++) {
                    result_out[jkqtplinalgMatIndex(k,c-N,C)] = m[jkqtplinalgMatIndex(k,c,N+C)];
                }
            }
        }
    }

    return ok;
#endif
}

/*! \brief solve a system of N linear equations \f$ A\cdot\vec{x}=B \f$ simultaneously for C columns in B
    \ingroup jkqtptools_math_linalg

    \param A an NxN matrix of coefficients
    \param[in,out] B an NxC marix (also receives the result)
    \param N number of equations
    \param C number of columns in B
    \return \c true on success

    \note This function uses the Gauss-Jordan algorithm
    \note It is save to call \c jkqtpstatLinSolve(A,B,N,C,B) with the same argument for B and result_out. Then the input will be overwritten with the new matrix!
*/
template <class T>
inline bool jkqtplinalgLinSolve(const T* A, T* B, long N, long C) {
    return jkqtplinalgLinSolve(A,B,N,C,B);
}


/*! \brief solve a system of N linear equations \f$ A\cdot\vec{x}=\vec{b} \f$
    \ingroup jkqtptools_math_linalg

    \param A an NxN matrix of coefficients
    \param b an N-entry vector
    \param N number of rows and columns in \a A
    \param[out] result_out a N-entry vector with the result
    \return \c true on success

    \note This function uses the Gauss-Jordan algorithm
    \note It is save to call \c jkqtpstatLinSolve(A,B,N,C,B) with the same argument for B and result_out. Then the input will be overwritten with the new matrix!
*/
template <class T>
inline bool jkqtplinalgLinSolve(const T* A, const T* b, long N, T* result_out) {
    return jkqtplinalgLinSolve(A, b, N, 1, result_out);
}

/*! \brief solve a system of N linear equations \f$ A\cdot\vec{x}=\vec{b} \f$
    \ingroup jkqtptools_math_linalg

    \param A an NxN matrix of coefficients
    \param[in,out] b an N-entry vector (also receives the result)
    \param N number of equations
    \param N number of rows and columns in \a A
    \return \c true on success

    \note This function uses the Gauss-Jordan algorithm
    \note It is save to call \c jkqtpstatLinSolve(A,B,N,C,B) with the same argument for B and result_out. Then the input will be overwritten with the new matrix!
*/
template <class T>
inline bool jkqtplinalgLinSolve(const T* A, T* b, long N) {
    return jkqtplinalgLinSolve(A,b,N,1,b);
}




/*! \brief determinant the given NxN matrix
    \ingroup jkqtptools_math_linalg

    \tparam T of the matrix cells (typically double or float)
    \param a the matrix  for which to calculate the determinant
    \param N number of rows and columns in the matrix
    \return the determinant of \a a

    \note for large matrices this algorithm is very slow, think about defining the macro JKQTP_STATISTICS_TOOLS_MAY_USE_EIGEN3 to use faster methods from the EIGEN library!
*/
template <class T>
inline T jkqtplinalgMatrixDeterminant(const T* a, long N) {
#ifdef JKQTP_STATISTICS_TOOLS_MAY_USE_EIGEN3
    if (N < 1) { /* Error */
        return NAN;
    } else if (N == 1) { /* Shouldn't get used */
        return a[jkqtplinalgMatIndex(0,0,N)];
    } else if (N == 2) {
        return a[jkqtplinalgMatIndex(0,0,N)] * a[jkqtplinalgMatIndex(1,1,N)] - a[jkqtplinalgMatIndex(1,0,N)] * a[jkqtplinalgMatIndex(0,1,N)];
    } else if (N==3){
        Eigen::Map<const Eigen::Matrix<T,3,3,Eigen::RowMajor> > eA(a);
        return eA.determinant();
    } else if (N==4){
        Eigen::Map<const Eigen::Matrix<T,4,4,Eigen::RowMajor> > eA(a);
        return eA.determinant();
    } else {
        Eigen::Map<const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> > eA(a,N,N);
        return eA.determinant();
    }
#else
    long i,j,j1,j2;
    T det = 0;

    if (N < 1) { /* Error */
        det=NAN;
    } else if (N == 1) { /* Shouldn't get used */
        det = a[jkqtplinalgMatIndex(0,0,N)];
    } else if (N == 2) {
        det = a[jkqtplinalgMatIndex(0,0,N)] * a[jkqtplinalgMatIndex(1,1,N)] - a[jkqtplinalgMatIndex(1,0,N)] * a[jkqtplinalgMatIndex(0,1,N)];
    } else {
        det = 0;
        for (j1=0;j1<N;j1++) {
            JKQTPArrayScopedPointer<T> m(static_cast<T*>(calloc((N-1)*(N-1),sizeof(T *))));

            for (i=1;i<N;i++) {
                j2 = 0;
                for (j=0;j<N;j++) {
                    if (j != j1){
                        m[jkqtplinalgMatIndex(i-1,j2,N-1)] = a[jkqtplinalgMatIndex(i,j,N)];
                        j2++;
                    }
                }
            }
            //printf("%d: %lf (%lf  %lf)\n",j1,pow(-1.0,1.0+double(j1)+1.0),a[jkqtplinalgMatIndex(0,j1,N)], jkqtplinalgMatrixDeterminant(m,N-1));
            det = det + double(((1+j1+1)%2==0)?1.0:-1.0)/* pow(-1.0,1.0+double(j1)+1.0)*/ * a[jkqtplinalgMatIndex(0,j1,N)] * jkqtplinalgMatrixDeterminant(m,N-1);

        }
    }

    //printf("\n\n  jkqtplinalgMatrixDeterminant(%d):\n",N);
    //linalgPrintMatrix(a,N,N);
    //printf("  jkqtplinalgMatrixDeterminant(%d) = %lf\n", N, det);
    return(det);
#endif
}




#endif // JKQTPLINALGTOOLS_H_INCLUDED


