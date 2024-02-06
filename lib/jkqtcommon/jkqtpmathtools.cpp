/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)


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



#include "jkqtcommon/jkqtpmathtools.h"
#include <cmath>



uint64_t jkqtp_gcd(uint64_t a, uint64_t b)
{
    if (a == 0)
        return b;
    else if (b == 0)
        return a;

    if (a < b)
        return jkqtp_gcd(a, b % a);
    else
        return jkqtp_gcd(b, a % b);
}

void jkqtp_estimateFraction(double input, int& sign, uint64_t &intpart, uint64_t &num, uint64_t &denom, unsigned int precision)
{
    // see https://stackoverflow.com/a/28662740
    //     https://stackoverflow.com/questions/4819075/how-to-output-fraction-instead-of-decimal-number
    //     https://stackoverflow.com/a/64828741
    //     https://stackoverflow.com/questions/26643695/converting-a-floating-point-decimal-value-to-a-fraction
    sign=+1;
    if (input<0.0) {
        sign=-1;
        input=fabs(input);
    }
    // now input is guaranteed positive and sign stores the sign +1 or -1
    const double integral = std::floor(input);
    const double frac = input - integral;

    const double prec = pow(10,precision); // This is the accuracy.

    uint64_t gcd_ = jkqtp_gcd(static_cast<uint64_t>(round(frac * prec)), static_cast<uint64_t>(prec));

    denom = static_cast<uint64_t>(prec / gcd_);
    num = static_cast<uint64_t>(round(frac * prec) / gcd_);

    intpart=static_cast<uint64_t>(integral);

}
