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



#include "jkqtpstatkde.h"


double jkqtpstatKernel1DGaussian(double t) {
    return exp(-0.5*t*t)/JKQTPSTATISTICS_SQRT_2PI;
}


double jkqtpstatKernel1DCauchy(double t) {
    return 1.0/(JKQTPSTATISTICS_PI*(1.0+t*t));
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
    return (fabs(t)<1.0)?(JKQTPSTATISTICS_PI/4.0*cos(t*JKQTPSTATISTICS_PI/2.0)):0.0;
}


double jkqtpstatKernel2DGaussian(double tx, double ty)
{
    return exp(-0.5*(tx*tx+ty*ty))/(2.0*JKQTPSTATISTICS_PI);
}

double jkqtpstatKernel2DUniform(double tx, double ty) {
    return (fabs(tx)<1.0 && fabs(ty)<=1.0)?0.25:0.0;
}
