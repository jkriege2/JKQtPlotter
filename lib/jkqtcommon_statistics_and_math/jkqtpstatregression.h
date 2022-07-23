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


#ifndef JKQTPSTATREGRESSION_H_INCLUDED
#define JKQTPSTATREGRESSION_H_INCLUDED

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
#include "jkqtcommon_statistics_and_math/jkqtcommon_statistics_and_math_imexport.h"
#include "jkqtcommon_statistics_and_math/jkqtplinalgtools.h"
#include "jkqtcommon_statistics_and_math/jkqtparraytools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon_statistics_and_math/jkqtpstatbasics.h"
#include "jkqtcommon_statistics_and_math/jkqtpstatpoly.h"






/*! \brief calculate the linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is \f$ f(x)=a+b\cdot x \f$
           So this function solves the least-squares optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i\left(y_i-(a+b\cdot x_i)\right)^2 \f]
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used

    This function computes internally:
        \f[ a=\overline{y}-b\cdot\overline{x} \f]
        \f[ b=\frac{\sum x_iy_i-N\cdot\overline{x}\cdot\overline{y}}{\sum x_i^2-N\cdot(\overline{x})^2} \f]

    \image html datastore_regression_lin.png
*/
template <class InputItX, class InputItY>
inline void jkqtpstatLinearRegression(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false) {
    if (fixA&&fixB) return;
    const int Nx=std::distance(firstX,lastX);
    const int Ny=std::distance(firstY,lastY);

    JKQTPASSERT(Nx>1 && Ny>1);

    double sumx=0, sumy=0, sumxy=0, sumx2=0;
    size_t N=0;
    auto itX=firstX;
    auto itY=firstY;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            sumx=sumx+fit_x;
            sumy=sumy+fit_y;
            sumxy=sumxy+fit_x*fit_y;
            sumx2=sumx2+fit_x*fit_x;
            N++;
        }
    }
    const double NN=static_cast<double>(N);
    JKQTPASSERT_M(NN>1, "too few datapoints");
    if (!fixA && !fixB) {
        coeffB=(double(sumxy)-double(sumx)*double(sumy)/NN)/(double(sumx2)-double(sumx)*double(sumx)/NN);;
        coeffA=double(sumy)/NN-coeffB*double(sumx)/NN;
    } else if (fixA && !fixB) {
        coeffB=(double(sumy)/NN-coeffA)/(double(sumx)/NN);
    } else if (!fixA && fixB) {
        coeffA=double(sumy)/NN-coeffB*double(sumx)/NN;
    }
}


/*! \brief calculate the weighted linear regression coefficients for a given for a given data range \a firstX / \a firstY / \a firstW ... \a lastX / \a lastY / \a lastW  where the model is \f$ f(x)=a+b\cdot x \f$
           So this function solves the least-squares optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_iw_i^2\cdot\left(y_i-(a+b\cdot x_i)\right)^2 \f]
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.


    This function internally computes:
        \f[ a=\frac{\overline{y}-b\cdot\overline{x}}{\overline{w^2}} \f]
        \f[ b=\frac{\overline{w^2}\cdot\overline{x\cdot y}-\overline{x}\cdot\overline{y}}{\overline{x^2}\cdot\overline{w^2}-\overline{x}^2} \f]

    Here the averages are defined in terms of a weight vector \f$ w_i\f$:
        \f[ \overline{x}=\sum\limits_iw_i^2\cdot x_i \f]
        \f[ \overline{y}=\sum\limits_iw_i^2\cdot y_i \f]
        \f[ \overline{x\cdot y}=\sum\limits_iw_i^2\cdot x_i\cdot y_i \f]
        \f[ \overline{x^2}=\sum\limits_iw_i^2\cdot x_i^2 \f]
        \f[ \overline{w^2}=\sum\limits_iw_i^2 \f]

    \image html datastore_regression_linweight.png

*/
template <class InputItX, class InputItY, class InputItW>
inline void jkqtpstatLinearWeightedRegression(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {
    if (fixA&&fixB) return;
    const int Nx=std::distance(firstX,lastX);
    const int Ny=std::distance(firstY,lastY);
    const int Nw=std::distance(firstW,lastW);

    JKQTPASSERT(Nx>1 && Ny>1 && Nw>1);

    double  sumx=0, sumy=0, sumxy=0, sumx2=0, sumw2=0;
    size_t N=0;
    auto itX=firstX;
    auto itY=firstY;
    auto itW=firstW;
    for (; itX!=lastX && itY!=lastY && itW!=lastW; ++itX, ++itY, ++itW) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        const double fit_w2=jkqtp_sqr(fWeightDataToWi(jkqtp_todouble(*itW)));
        if (JKQTPIsOKFloat(fit_x)&&JKQTPIsOKFloat(fit_y)&&JKQTPIsOKFloat(fit_w2)) {
            sumx=sumx+fit_w2*fit_x;
            sumy=sumy+fit_w2*fit_y;
            sumxy=sumxy+fit_w2*fit_x*fit_y;
            sumx2=sumx2+fit_w2*fit_x*fit_x;
            sumw2=sumw2+fit_w2;
            N++;
        }
    }
    const double NN=static_cast<double>(N);
    JKQTPASSERT_M(NN>1, "too few datapoints");
    if (!fixA && !fixB) {
        coeffB=(double(sumxy)*double(sumw2)-double(sumx)*double(sumy))/(double(sumx2)*double(sumw2)-double(sumx)*double(sumx));
        coeffA=(double(sumy)-coeffB*double(sumx))/double(sumw2);
    } else if (fixA && !fixB) {
        coeffB=(double(sumy)-coeffA*double(sumw2))/double(sumx);
    } else if (!fixA && fixB) {
        coeffA=(double(sumy)-coeffB*double(sumx))/double(sumw2);
    }
}



/*! \brief calculate the (robust) iteratively reweighted least-squares (IRLS) estimate for the parameters of the model \f$ f(x)=a+b\cdot x \f$
           for a given data range \a firstX / \a firstY ... \a lastX / \a lastY
           So this function finds an outlier-robust solution to the optimization problem:
           \f[ (a^\ast,b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i|a+b\cdot x_i-y_i|^p \f]
    \ingroup jkqtptools_math_statistics_regression

    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    This is a simple form of the IRLS algorithm to estimate the parameters a and b in a linear model \f$ f(x)=a+b\cdot x \f$.
    This algorithm solves the optimization problem for a \f$ L_p\f$-norm:
      \f[ (a^\ast,b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i|a+b\cdot x_i-y_i|^p \f]
    by iteratively optimization weights \f$ \vec{w} \f$ and solving a weighted least squares problem in each iteration:
      \f[ (a_n,b_n)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i|a+b\cdot x_i-y_i|^{(p-2)}\cdot|a+b\cdot x_i-y_i|^2 \f]


    The IRLS-algorithm works as follows:
      - calculate initial \f$ a_0\f$ and \f$ b_0\f$ with unweighted regression from x and y
      - perform a number of iterations (parameter \a iterations ). In each iteration \f$ n\f$:
          - calculate the error vector \f$\vec{e}\f$: \f[ e_i = a+b\cdot x_i -y_i \f]
          - estimate new weights \f$\vec{w}\f$: \f[ w_i=|e_i|^{(p-2)/2} \f]
          - calculate new estimates \f$ a_n\f$ and \f$ b_n\f$ with weighted regression from \f$ \vec{x}\f$ and \f$ \vec{y}\f$ and \f$ \vec{w}\f$
        .
      - return the last estimates \f$ a_n\f$ and \f$ b_n\f$
    .

    \image html irls.png

    \image html datastore_regression_linrobust_p.png

    \see https://en.wikipedia.org/wiki/Iteratively_reweighted_least_squares, C. Sidney Burrus: "Iterative Reweighted Least Squares", <a href="http://cnx.org/content/m45285/latest/">http://cnx.org/content/m45285/latest/</a>
*/
template <class InputItX, class InputItY>
inline void jkqtpstatRobustIRLSLinearRegression(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100) {
    if (fixA&&fixB) return;
    const int Nx=std::distance(firstX,lastX);
    const int Ny=std::distance(firstY,lastY);
    const int N=std::min(Nx,Ny);

    JKQTPASSERT(Nx>1 && Ny>1);

    std::vector<double> weights;
    std::fill_n(std::back_inserter(weights), N, 1.0);

    double alast=coeffA, blast=coeffB;
    jkqtpstatLinearWeightedRegression(firstX, lastX, firstY, lastY, weights.begin(), weights.end(), alast, blast, fixA, fixB, &jkqtp_identity<double>);
    for (int it=0; it<iterations-1; it++) {
        // calculate weights
        auto itX=firstX;
        auto itY=firstY;
        for (double& w: weights) {
            const double fit_x=*itX;
            const double fit_y=*itY;
            const double e=alast+blast*fit_x-fit_y;
            w=pow(std::max<double>(JKQTP_EPSILON*100.0, fabs(e)), (p-2.0)/2.0);
            ++itX;
            ++itY;
        }
        // solve weighted linear least squares
        jkqtpstatLinearWeightedRegression(firstX, lastX, firstY, lastY, weights.begin(), weights.end(), alast, blast, fixA, fixB, &jkqtp_identity<double>);
    }
    coeffA=alast;
    coeffB=blast;
}







/*! \brief when performing linear regression, different target functions can be fitted, if the input data is transformed accordingly. This library provides the options in this enum by default.
    \ingroup jkqtptools_math_statistics_regression
    */
enum class JKQTPStatRegressionModelType {
    Linear,       /*!< \brief linear model \f$ f(x)=a+b\cdot x \f$ */
    PowerLaw,     /*!< \brief power law model \f$ f(x)=a\cdot x^b \f$ */
    Exponential,  /*!< \brief exponential model \f$ f(x)=a\cdot \exp(b\cdot x) \f$ */
    Logarithm,   /*!< \brief exponential model \f$ f(x)=a+b\cdot \ln(x) \f$ */
};


/*! \brief Generates functors \c f(x,a,b) for the models from JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    */
JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT std::function<double(double, double, double)> jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType type);

/*! \brief Generates a LaTeX string for the models from JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    */
JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT QString jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType type, double a, double b);

/*! \brief Generates functors \c f(x) for the models from JKQTPStatRegressionModelType in \a type and binds the parameter values \a and \a b to the returned function
    \ingroup jkqtptools_math_statistics_regression
    */
JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT std::function<double(double)> jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType type, double a, double b);

/*! \brief Generates the transformation function for x-data (\c result.first ) and y-data (\c result.second ) for each regression model in JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    \internal
    */
JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT std::pair<std::function<double(double)>,std::function<double(double)> > jkqtpStatGenerateTransformation(JKQTPStatRegressionModelType type);


/*! \brief Generates the transformation function for a-parameter (offset, \c result.first : transform, \c result.second : back-transform) for each regression model in JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    \internal
    */
JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT std::pair<std::function<double(double)>,std::function<double(double)> > jkqtpStatGenerateParameterATransformation(JKQTPStatRegressionModelType type);


/*! \brief Generates the transformation function for b-parameter (slope, \c result.first : transform, \c result.second : back-transform) for each regression model in JKQTPStatRegressionModelType in \a type
    \ingroup jkqtptools_math_statistics_regression
    \internal
    */
JKQTCOMMON_STATISTICS_AND_MATH_LIB_EXPORT std::pair<std::function<double(double)>,std::function<double(double)> > jkqtpStatGenerateParameterBTransformation(JKQTPStatRegressionModelType type);


/*! \brief calculate the linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is defined by \a type
           So this function solves the least-squares optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i\left(y_i-f_{\text{type}}(x_i,a,b)\right)^2 \f]
           by reducing it to a linear fit by transforming x- and/or y-data
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used

    This function computes internally first transforms the data, as appropriate to fit the model defined by \a type and then calls jkqtpstatLinearRegression()
    to obtain the parameters. The output parameters are transformed, so they can be used with jkqtpStatGenerateRegressionModel() to generate a functor
    that evaluates the model

    \see JKQTPStatRegressionModelType, jkqtpStatGenerateRegressionModel(), jkqtpstatLinearRegression(), jkqtpStatGenerateTransformation()
*/
template <class InputItX, class InputItY>
inline void jkqtpstatRegression(JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false) {
    std::vector<double> x, y;
    auto trafo=jkqtpStatGenerateTransformation(type);
    auto aTrafo =jkqtpStatGenerateParameterATransformation(type);
    auto bTrafo =jkqtpStatGenerateParameterBTransformation(type);

    std::transform(firstX, lastX, std::back_inserter(x), trafo.first);
    std::transform(firstY, lastY, std::back_inserter(y), trafo.second);

    double a=aTrafo.first(coeffA);
    double b=bTrafo.first(coeffB);

    jkqtpstatLinearRegression(x.begin(), x.end(), y.begin(), y.end(), a, b, fixA, fixB);

    coeffA=aTrafo.second(a);
    coeffB=bTrafo.second(b);
}



/*! \brief calculate the robust linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is defined by \a type
           So this function solves the Lp-norm optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_i|y_i-f_{\text{type}}(x_i,a,b)|^p \f]
           by reducing it to a linear fit by transforming x- and/or y-data
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    This function computes internally first transforms the data, as appropriate to fit the model defined by \a type and then calls jkqtpstatRobustIRLSLinearRegression()
    to obtain the parameters. The output parameters are transformed, so they can be used with jkqtpStatGenerateRegressionModel() to generate a functor
    that evaluates the model

    \see JKQTPStatRegressionModelType, jkqtpStatGenerateRegressionModel(), jkqtpstatRobustIRLSLinearRegression(), jkqtpStatGenerateTransformation()
*/
template <class InputItX, class InputItY>
inline void jkqtpstatRobustIRLSRegression(JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100) {
    std::vector<double> x, y;
    auto trafo=jkqtpStatGenerateTransformation(type);
    auto aTrafo =jkqtpStatGenerateParameterATransformation(type);
    auto bTrafo =jkqtpStatGenerateParameterBTransformation(type);

    std::transform(firstX, lastX, std::back_inserter(x), trafo.first);
    std::transform(firstY, lastY, std::back_inserter(y), trafo.second);

    double a=aTrafo.first(coeffA);
    double b=bTrafo.first(coeffB);

    jkqtpstatRobustIRLSLinearRegression(x.begin(), x.end(), y.begin(), y.end(), a, b, fixA, fixB, p, iterations);

    coeffA=aTrafo.second(a);
    coeffB=bTrafo.second(b);
}




/*! \brief calculate the robust linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is defined by \a type
           So this function solves the Lp-norm optimization problem: \f[ (a^\ast, b^\ast)=\mathop{\mathrm{arg\;min}}\limits_{a,b}\sum\limits_iw_i^2\left(y_i-f_{\text{type}}(x_i,a,b)\right)^2 \f]
           by reducing it to a linear fit by transforming x- and/or y-data
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.

    This function computes internally first transforms the data, as appropriate to fit the model defined by \a type and then calls jkqtpstatLinearWeightedRegression()
    to obtain the parameters. The output parameters are transformed, so they can be used with jkqtpStatGenerateRegressionModel() to generate a functor
    that evaluates the model

    \see JKQTPStatRegressionModelType, jkqtpStatGenerateRegressionModel(), jkqtpstatLinearWeightedRegression(), jkqtpStatGenerateTransformation()
*/
template <class InputItX, class InputItY, class InputItW>
inline void jkqtpstatWeightedRegression(JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, double& coeffA, double& coeffB, bool fixA=false, bool fixB=false, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {
    std::vector<double> x, y;
    auto trafo=jkqtpStatGenerateTransformation(type);
    auto aTrafo =jkqtpStatGenerateParameterATransformation(type);
    auto bTrafo =jkqtpStatGenerateParameterBTransformation(type);

    std::transform(firstX, lastX, std::back_inserter(x), trafo.first);
    std::transform(firstY, lastY, std::back_inserter(y), trafo.second);

    double a=aTrafo.first(coeffA);
    double b=bTrafo.first(coeffB);

    jkqtpstatLinearWeightedRegression(x.begin(), x.end(), y.begin(), y.end(), firstW, lastW, a, b, fixA, fixB, fWeightDataToWi);

    coeffA=aTrafo.second(a);
    coeffB=bTrafo.second(b);
}




/*! \brief calculates the coefficient of determination \f$ R^2 \f$ for a set of measurements \f$ (x_i,y_i) \f$ with a fit function \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param f function \f$ f(x) \f$, result of a fit to the data
    \return coeffcicient of determination \f[ R^2=1-\frac{\sum_i\bigl[y_i-f(x_i)\bigr]^2}{\sum_i\bigl[y_i-\overline{y}\bigr]^2} \f] where \f[ \overline{y}=\frac{1}{N}\cdot\sum_iy_i \f]



    \see https://en.wikipedia.org/wiki/Coefficient_of_determination
*/
template <class InputItX, class InputItY>
inline double jkqtpstatCoefficientOfDetermination(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, std::function<double(double)> f) {

    auto itX=firstX;
    auto itY=firstY;

    const double yMean=jkqtpstatAverage(firstX,lastX);
    double SSres=0;
    double SStot=0;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            SStot+=jkqtp_sqr(fit_y-yMean);
            SSres+=jkqtp_sqr(fit_y-f(fit_x));
        }
    }

    return 1.0-SSres/SStot;
}


/*! \brief calculates the weightedcoefficient of determination \f$ R^2 \f$ for a set of measurements \f$ (x_i,y_i,w_i) \f$ with a fit function \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param f function \f$ f(x) \f$, result of a fit to the data
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.
    \return weighted coeffcicient of determination \f[ R^2=1-\frac{\sum_iw_i^2\bigl[y_i-f(x_i)\bigr]^2}{\sum_iw_i^2\bigl[y_i-\overline{y}\bigr]^2} \f] where \f[ \overline{y}=\frac{1}{N}\cdot\sum_iw_iy_i \f]
            with \f[ \sum_iw_i=1 \f]



    \see https://en.wikipedia.org/wiki/Coefficient_of_determination
*/
template <class InputItX, class InputItY, class InputItW>
inline double jkqtpstatWeightedCoefficientOfDetermination(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, std::function<double(double)> f, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {

    auto itX=firstX;
    auto itY=firstY;
    auto itW=firstW;

    const double yMean=jkqtpstatWeightedAverage(firstX,lastX,firstW);
    double SSres=0;
    double SStot=0;
    for (; itX!=lastX && itY!=lastY && itW!=lastW; ++itX, ++itY, ++itW) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        const double fit_w2=jkqtp_sqr(fWeightDataToWi(jkqtp_todouble(*itW)));
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y) && JKQTPIsOKFloat(fit_w2)) {
            SSres+=(fit_w2*jkqtp_sqr(fit_y-f(fit_x)));
            SStot+=(fit_w2*jkqtp_sqr(fit_y-yMean));
        }
    }

    return 1.0-SSres/SStot;
}




/*! \brief calculates the sum of deviations \f$ \chi^2 \f$ for a set of measurements \f$ (x_i,y_i) \f$ with a fit function \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param f function \f$ f(x) \f$, result of a fit to the data
    \return sum of deviations  \f[ \chi^2=\sum_i\bigl[y_i-f(x_i)\bigr]^2 \f]



    \see https://en.wikipedia.org/wiki/Coefficient_of_determination
*/
template <class InputItX, class InputItY>
inline double jkqtpstatSumOfDeviations(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, std::function<double(double)> f) {

    auto itX=firstX;
    auto itY=firstY;

    double SSres=0;
    for (; itX!=lastX && itY!=lastY; ++itX, ++itY) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y)) {
            SSres+=jkqtp_sqr(fit_y-f(fit_x));
        }
    }

    return SSres;
}




/*! \brief calculates the weighted sum of deviations \f$ \chi^2 \f$ for a set of measurements \f$ (x_i,y_i,w_i) \f$ with a fit function \f$ f(x) \f$
    \ingroup jkqtptools_math_statistics_regression

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param f function \f$ f(x) \f$, result of a fit to the data
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.
    \return weighted sum of deviations  \f[ \chi^2=\sum_iw_i^2\cdot\bigl[y_i-f(x_i)\bigr]^2 \f]


    \see https://en.wikipedia.org/wiki/Reduced_chi-squared_statistic
*/
template <class InputItX, class InputItY, class InputItW>
inline double jkqtpstatWeightedSumOfDeviations(InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, std::function<double(double)> f, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {

    auto itX=firstX;
    auto itY=firstY;
    auto itW=firstW;

    double SSres=0;
    for (; itX!=lastX && itY!=lastY && itW!=lastW; ++itX, ++itY, ++itW) {
        const double fit_x=jkqtp_todouble(*itX);
        const double fit_y=jkqtp_todouble(*itY);
        const double fit_w2=jkqtp_sqr(fWeightDataToWi(jkqtp_todouble(*itW)));
        if (JKQTPIsOKFloat(fit_x) && JKQTPIsOKFloat(fit_y) && JKQTPIsOKFloat(fit_w2)) {
            SSres+=fit_w2*jkqtp_sqr(fit_y-f(fit_x));
        }
    }

    return SSres;
}







#endif // JKQTPSTATREGRESSION_H_INCLUDED


