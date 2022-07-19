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



#include "jkqtpstatbasics.h"



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

