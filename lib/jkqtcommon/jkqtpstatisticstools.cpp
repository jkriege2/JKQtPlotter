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



#include "jkqtpstatisticstools.h"


double jkqtpstatKernel1DGaussian(double t) {
    return exp(-0.5*t*t)/JKQTPSTATISTICS_SQRT_2PI;
}


double jkqtpstatKernel1DCauchy(double t) {
    return 1.0/(M_PI*(1.0+t*t));
}



double jkqtpstatKernel1DPicard(double t) {
    return exp(-0.5*fabs(t))/2.0;
}


double jkqtpstatKernel1DEpanechnikov(double t) {
    return (fabs(t)<1.0)?(0.75*(1.0-t*t)):0.0;
}


double jkqtpstatKernel1DUniform(double t) {
    return (fabs(t)<=1.0)?0.5:0.0;
}


double jkqtpstatKernel1DTriangle(double t) {
    return (fabs(t)<=1.0)?(1.0-fabs(t)):0.0;
}



double jkqtpstatKernel1DQuartic(double t) {
    return (fabs(t)<=1.0)?(15.0/16.0*jkqtp_sqr(1.0-t*t)):0.0;
}


double jkqtpstatKernel1DTriweight(double t) {
    return (fabs(t)<1.0)?(35.0/32.0*jkqtp_cube(1.0-t*t)):0.0;
}



double jkqtpstatKernel1DTricube(double t) {
    return (fabs(t)<1.0)?(70.0/81.0*jkqtp_cube(1.0-jkqtp_cube(fabs(t)))):0.0;
}


double jkqtpstatKernel1DCosine(double t) {
    return (fabs(t)<1.0)?(M_PI/4.0*cos(t*M_PI/2.0)):0.0;
}


double jkqtpstatKernel2DGaussian(double tx, double ty)
{
    return exp(-0.5*(tx*tx+ty*ty))/(2.0*M_PI);
}

double jkqtpstatKernel2DUniform(double tx, double ty) {
    return (fabs(tx)<1.0 && fabs(ty)<=1.0)?0.25:0.0;
}

JKQTPStat5NumberStatistics::JKQTPStat5NumberStatistics():
    minimum(JKQTP_DOUBLE_NAN),
    minimumQuantile(0),
    quantile1(JKQTP_DOUBLE_NAN),
    quantile1Spec(0.25),
    median(JKQTP_DOUBLE_NAN),
    quantile2(JKQTP_DOUBLE_NAN),
    quantile2Spec(0.75),
    maximum(JKQTP_DOUBLE_NAN),
    maximumQuantile(1),
    N(0)
{}

double JKQTPStat5NumberStatistics::IQR() const {
    return quantile2-quantile1;
}

double JKQTPStat5NumberStatistics::IQRSignificanceEstimate() const {
    return 2.0*(1.58*(IQR()))/sqrt(static_cast<double>(N));
}

std::function<double (double, double, double)> jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType type) {
    switch(type) {
        case JKQTPStatRegressionModelType::Linear: return [](double x, double a, double b)->double { return a+b*x; };
        case JKQTPStatRegressionModelType::PowerLaw: return [](double x, double a, double b)->double { return a*pow(x,b); };
        case JKQTPStatRegressionModelType::Exponential: return [](double x, double a, double b)->double { return a*exp(b*x); };
        case JKQTPStatRegressionModelType::Logarithm: return [](double x, double a, double b)->double { return a+b*log(x); };
    }
    throw std::runtime_error("unknown JKQTPStatRegressionModelType in jkqtpStatGenerateRegressionModel()");
}

QString jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType type, double a, double b) {
    switch(type) {
        case JKQTPStatRegressionModelType::Linear: return QString("f(x)=%1%2{\\cdot}x").arg(jkqtp_floattolatexqstr(a, 2, true, 1e-16,1e-2, 1e4,false)).arg(jkqtp_floattolatexqstr(b, 2, true, 1e-16,1e-2, 1e4,true));
        case JKQTPStatRegressionModelType::PowerLaw: return QString("f(x)=%1{\\cdot}x^{%2}").arg(jkqtp_floattolatexqstr(a, 3)).arg(jkqtp_floattolatexqstr(b, 3));
        case JKQTPStatRegressionModelType::Exponential: return QString("f(x)=%1{\\cdot}\\exp(%2{\\cdot}x)").arg(jkqtp_floattolatexqstr(a, 3)).arg(jkqtp_floattolatexqstr(b, 3));
        case JKQTPStatRegressionModelType::Logarithm: return QString("f(x)=%1%2{\\cdot}\\ln(x)").arg(jkqtp_floattolatexqstr(a, 2, true, 1e-16,1e-2, 1e4,false)).arg(jkqtp_floattolatexqstr(b, 2, true, 1e-16,1e-2, 1e4,true));
    }
    throw std::runtime_error("unknown JKQTPStatRegressionModelType in jkqtpstatRegressionModel2Latex()");
}

std::function<double (double)> jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType type, double a, double b) {
    auto res=jkqtpStatGenerateRegressionModel(type);
    return std::bind(res, std::placeholders::_1, a, b);
}

std::pair<std::function<double (double)>, std::function<double (double)> > jkqtpStatGenerateTransformation(JKQTPStatRegressionModelType type) {
    auto logF=[](double x)->double { return log(x); };
    auto expF=[](double x)->double { return exp(x); };
    auto idF=&jkqtp_identity<double>;
    switch(type) {
        case JKQTPStatRegressionModelType::Linear: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, idF);
        case JKQTPStatRegressionModelType::PowerLaw: return std::pair<std::function<double(double)>,std::function<double(double)> >(logF, logF);
        case JKQTPStatRegressionModelType::Exponential: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, logF);
        case JKQTPStatRegressionModelType::Logarithm: return std::pair<std::function<double(double)>,std::function<double(double)> >(logF, idF);
    }
    throw std::runtime_error("unknown JKQTPStatRegressionModelType in jkqtpStatGenerateTransformation()");
}

std::pair<std::function<double (double)>, std::function<double (double)> > jkqtpStatGenerateParameterATransformation(JKQTPStatRegressionModelType type) {
    auto logF=[](double x)->double { return log(x); };
    auto expF=[](double x)->double { return exp(x); };
    auto idF=&jkqtp_identity<double>;
    switch(type) {
        case JKQTPStatRegressionModelType::Linear: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, idF);
        case JKQTPStatRegressionModelType::PowerLaw: return std::pair<std::function<double(double)>,std::function<double(double)> >(logF, expF);
        case JKQTPStatRegressionModelType::Exponential: return std::pair<std::function<double(double)>,std::function<double(double)> >(logF, expF);
        case JKQTPStatRegressionModelType::Logarithm: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, idF);
    }
    throw std::runtime_error("unknown JKQTPStatRegressionModelType in jkqtpStatGenerateParameterATransformation()");
}

std::pair<std::function<double (double)>, std::function<double (double)> > jkqtpStatGenerateParameterBTransformation(JKQTPStatRegressionModelType type) {
    //auto logF=[](double x)->double { return log(x); };
    //auto expF=[](double x)->double { return exp(x); };
    auto idF=&jkqtp_identity<double>;
    switch(type) {
        case JKQTPStatRegressionModelType::Linear: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, idF);
        case JKQTPStatRegressionModelType::PowerLaw: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, idF);
        case JKQTPStatRegressionModelType::Exponential: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, idF);
        case JKQTPStatRegressionModelType::Logarithm: return std::pair<std::function<double(double)>,std::function<double(double)> >(idF, idF);
    }
    throw std::runtime_error("unknown JKQTPStatRegressionModelType in jkqtpStatGenerateParameterBTransformation()");
}
