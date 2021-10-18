/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)



    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#define COMPILING_THIS_JKMATHPARSER
#define _USE_MATH_DEFINES
#include <cmath>
#include "jkqtcommon/jkqtpmathparser.h" // class's header file
#include <iostream>
#include <float.h>
#include <ctime>
#include "jkqtcommon/jkqtpstringtools.h"


/******************************************************************************************
 * default-function implementations for math parser
 ******************************************************************************************/
namespace { // anonymous namespace to limit availability to this module (CPP-file)
    JKQTPMathParser::jkmpResult fFloatToStr(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      std::ostringstream ost;
      r.type=JKQTPMathParser::jkmpString;
      if (n!=1) p->jkmpError("floattostr accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("floattostr needs double argument");
      ost<<params[0].num;
      r.str=ost.str();
      return r;
    }


    JKQTPMathParser::jkmpResult fIntToStr(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      std::ostringstream ost;
      r.type=JKQTPMathParser::jkmpString;
      if (n!=1) p->jkmpError("inttostr accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("inttostr needs double argument");
      ost<<int64_t(params[0].num);
      r.str=ost.str();
      return r;
    }

    JKQTPMathParser::jkmpResult fBoolToStr(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpString;
      if (n!=1) p->jkmpError("booltostr accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpBool) p->jkmpError("floattostr needs bool argument");
      r.str=(r.boolean)?"true":"false";
      return r;
    }


    JKQTPMathParser::jkmpResult fSinc(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("sinc accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("sinc needs double argument");
      //r.num=sin(params[0].num)/params[0].num;

      double x=params[0].num;
      static double const    taylor_0_bound = 3*DBL_MIN ;
      static double const    taylor_2_bound = sqrt(taylor_0_bound);
      static double const    taylor_n_bound = sqrt(taylor_2_bound);

      double    result = 1;
      if    (fabs(x) >= taylor_n_bound)
      {
          result= sin(x)/x;
      }
      else
      {
          // approximation by taylor series in x at 0 up to order 0

          if    (fabs(x) >= taylor_0_bound)
          {
              double    x2 = x*x;

              // approximation by taylor series in x at 0 up to order 2
              result -= x2/static_cast<double>(6);

              if    (fabs(x) >= taylor_2_bound)
              {
                  // approximation by taylor series in x at 0 up to order 4
                  result += (x2*x2)/static_cast<double>(120);
              }
          }


      }

      r.num=result;

      return r;
    }

    JKQTPMathParser::jkmpResult fTanc(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("tanc accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("tanc needs double argument");
      r.num=(params[0].num==0)?1.0:tan(params[0].num)/params[0].num;
      return r;
    }

    JKQTPMathParser::jkmpResult fSin(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("sin accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("sin needs double argument");
      r.num=sin(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fCos(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("cos accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("cos needs double argument");
      r.num=cos(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fTan(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("tan accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("tan needs double argument");
      r.num=tan(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fExp(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("exp accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("exp needs double argument");
      r.num=exp(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fLog(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("log accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("log needs double argument");
      r.num=log(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fLog10(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("log10 accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("log10 needs double argument");
      r.num=log10(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fLog2(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("log2 accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("log2 needs double argument");
      r.num=log2(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fSqrt(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("sqrt accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("sqrt needs double argument");
      r.num=sqrt(params[0].num);
      return r;
    }
    JKQTPMathParser::jkmpResult fCbrt(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("cbrt accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("cbrt needs double argument");
      r.num=cbrt(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fSqr(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("sqr accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("sqr needs double argument");
      r.num=params[0].num*params[0].num;
      return r;
    }

    JKQTPMathParser::jkmpResult fAbs(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("abs accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("abs needs double argument");
      r.num=fabs(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fIf(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      if (n!=3) p->jkmpError("If accepts 3 argument");
      if (params[0].type!=JKQTPMathParser::jkmpBool) p->jkmpError("If needs bool as first argument");
      if (params[0].boolean) return params[1]; else return params[2];
    }

    JKQTPMathParser::jkmpResult fASin(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("asin accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("asin needs double argument");
      r.num=asin(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fACos(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("acos accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("acos needs double argument");
      r.num=acos(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fATan(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("atan accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("atan needs double argument");
      r.num=atan(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fATan2(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("atan2 accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("atan2 needs double argument");
      r.num=atan2(params[0].num, params[1].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fSinh(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("sinh accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("sinh needs double argument");
      r.num=sinh(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fCosh(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("cosh accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("cosh needs double argument");
      r.num=cosh(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fTanh(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("tanh accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("tanh needs double argument");
      r.num=tanh(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fErf(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("erf accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("erf needs double argument");
      r.num=erf(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fErfc(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("erfc accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("erfc needs double argument");
      r.num=erfc(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult flGamma(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("lgamma accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("lgamma needs double argument");
      r.num=lgamma(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult ftGamma(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("tgamma accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("tgamma needs double argument");
      r.num=tgamma(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fJ0(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("j0 accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("j0 needs double argument");
#if Q_CC_MSVC
	  r.num = _j0(params[0].num);
#else
	  r.num = j0(params[0].num);
#endif
      return r;
    }

    JKQTPMathParser::jkmpResult fJ1(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("j1 accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("j1 needs double argument");
#if Q_CC_MSVC
	  r.num=_j1(params[0].num);
#else
	  r.num=j1(params[0].num);
#endif
      return r;
    }

    JKQTPMathParser::jkmpResult fY0(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("y0 accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("y0 needs double argument");
#if Q_CC_MSVC
	  r.num=_y0(params[0].num);
#else
	  r.num=y0(params[0].num);
#endif
      return r;
    }

    JKQTPMathParser::jkmpResult fY1(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("y1 accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("y1 needs double argument");
#if Q_CC_MSVC
	  r.num=_y1(params[0].num);
#else
	  r.num=y1(params[0].num);
#endif
      return r;
    }

    JKQTPMathParser::jkmpResult fYn(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("yn accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("yn needs double argument");
#if Q_CC_MSVC
	  r.num=_yn(static_cast<int>(params[0].num), params[1].num);
#else
	  r.num=yn(static_cast<int>(params[0].num), params[1].num);
#endif
      return r;
    }

    JKQTPMathParser::jkmpResult fJn(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("jn accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("jn needs double argument");
#if Q_CC_MSVC
      r.num=_jn(static_cast<int>(params[0].num), params[1].num);
#else
	  r.num=jn(static_cast<int>(params[0].num), params[1].num);
#endif
      return r;
    }

    JKQTPMathParser::jkmpResult fSRand(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("srand accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("srand needs double argument");
      r.num=0;
      srand(static_cast<unsigned int>(params[0].num));
      return r;
    }

    JKQTPMathParser::jkmpResult fRand(JKQTPMathParser::jkmpResult* /*params*/, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=0) p->jkmpError("rand accepts 0 argument");
      r.num=double(rand())/double(RAND_MAX);
      return r;
    }

    JKQTPMathParser::jkmpResult fCeil(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("ceil accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("ceil needs double argument");
      r.num=ceil(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fFloor(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("floor accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("floor needs double argument");
      r.num=floor(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fTrunc(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("trunc accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("trunc needs double argument");
      r.num=trunc(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fRound(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("round accepts 1 argument");
      if (params[0].type!=JKQTPMathParser::jkmpDouble) p->jkmpError("round needs double argument");
      r.num=round(params[0].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fFMod(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("fmod accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("fmod needs double argument");
      r.num=fmod(static_cast<int>(params[0].num), params[1].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fMin(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("min accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("min needs double argument");
      r.num=fmin(static_cast<int>(params[0].num), params[1].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fMax(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("max accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("max needs double argument");
      r.num=fmax(static_cast<int>(params[0].num), params[1].num);
      return r;
    }

    JKQTPMathParser::jkmpResult QFSPIMLightsheetEvaluationItem_fGauss(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("gauss accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("gauss needs double argument");
      r.num=exp(-2*params[0].num*params[0].num/params[1].num/params[1].num);
      return r;
    }

    JKQTPMathParser::jkmpResult fSlit(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=2) p->jkmpError("slit accepts 2 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)||(params[1].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("slit needs double argument");
      r.num=((params[0].num>=-1.0*params[1].num/2)&&(params[0].num<=params[1].num/2))?1.0:0.0;
      return r;
    }

    JKQTPMathParser::jkmpResult fTheta(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("theta accepts 1 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("theta needs double argument");
      r.num=(params[0].num>=0)?1.0:0.0;
      return r;
    }

    JKQTPMathParser::jkmpResult fSigmoid(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("sigmoid accepts 1 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("sigmoid needs double argument");
      r.num=1.0/(1+exp(-1.0*params[0].num));
      return r;
    }

    JKQTPMathParser::jkmpResult fSign(JKQTPMathParser::jkmpResult* params, unsigned char n, JKQTPMathParser* p){
      JKQTPMathParser::jkmpResult r;
      r.type=JKQTPMathParser::jkmpDouble;
      if (n!=1) p->jkmpError("sign accepts 1 argument");
      if ((params[0].type!=JKQTPMathParser::jkmpDouble)) p->jkmpError("sign needs double argument");
      r.num=0.0;
      if (params[0].num<0) { r.num=-1; }
      else if (params[0].num>0) { r.num=+1; }
      return r;
    }



    inline std::string strip(const std::string& s) {
      std::string r;
      r.reserve(s.size());
      for (size_t i=0; i<s.size(); i++){
        if ((s[i]!=' ')&&(s[i]!='\t')&&(s[i]!='\r')&&(s[i]!='\n')) {
          r+=s[i];
        }
      }
      return r;
    }
}


void JKQTPMathParser::jkmpError(const std::string &st) {
    if (jkmathparser_exception_function!=nullptr) {
        jkmathparser_exception_function(st);
    } else {
        throw jkmpException(st);
    }
}

void JKQTPMathParser::setException_function(JKQTPMathParser::jkmpexceptionf exception_function) {
    jkmathparser_exception_function=exception_function;
}

void JKQTPMathParser::resetException_function() {
    jkmathparser_exception_function=nullptr;
}

std::string JKQTPMathParser::tokentostring(JKQTPMathParser::jkmpTokenType token) {
    switch(token) {
        case END: return "END";
        case PRINT: return "PRINT (;)";
        case PARAMETER_DIV: return "PARAMETER_DIV (,)";
        case STRING_DELIM: return "STRING_DELIM (' or \")";
        case NAME: return "NAME";
        case NUMBER: return "NUMBER";
        case PLUS: return "PLUS (+)";
        case MINUS: return "MINUS (-)";
        case MUL: return "MUL (*)";
        case DIV: return "DIV (/)";
        case MODULO: return "MODULO (%)";
        case ASSIGN: return "ASSIGN (=)";
        case LBRACKET: return "LBRACKET '('";
        case RBRACKET: return "RBRACKET ')'";
        case POWER: return "POWER (^)";
        case FACTORIAL_LOGIC_NOT: return "FACTORIAL_LOGIC_NOT (!)";
        case LOGIC_NOT: return "LOGIC_NOT (!/not)";
        case LOGIC_AND: return "LOGIC_AND (&&/and)";
        case LOGIC_OR: return "LOGIC_OR (||/or)";
        case LOGIC_XOR: return "LOGIC_XOR (xor)";
        case LOGIC_NOR: return "LOGIC_NOR (nor)";
        case LOGIC_NAND: return "LOGIC_NAND (nand)";
        case LOGIC_TRUE: return "LOGIC_TRUE (true)";
        case LOGIC_FALSE: return "LOGIC_FALSE (false)";
        case COMP_EQUALT: return "COMP_EQUALT (==)";
        case COMP_UNEQUAL: return "COMP_UNEQUAL (!=)";
        case COMP_GREATER: return "COMP_GREATER (>)";
        case COMP_SMALLER: return "COMP_SMALLER (<)";
        case COMP_GEQUAL: return "COMP_GEQUAL (>=)";
        case COMP_SEQUAL: return "COMP_SEQUAL (<=)";
    }
    return "unknown";
}

std::string JKQTPMathParser::currenttokentostring() {
    switch(CurrentToken) {
        case END: return "END";
        case PRINT: return "PRINT (;)";
        case PARAMETER_DIV: return "PARAMETER_DIV (,)";
        case STRING_DELIM: return "STRING_DELIM (' or \")";
        case NAME: return jkqtp_format("NAME (%s)", StringValue.c_str());
        case NUMBER: return jkqtp_format("NUMBER (%lf)", NumberValue);
        case PLUS: return "PLUS (+)";
        case MINUS: return "MINUS (-)";
        case MUL: return "MUL (*)";
        case DIV: return "DIV (/)";
        case MODULO: return "MODULO (%)";
        case ASSIGN: return "ASSIGN (=)";
        case LBRACKET: return "LBRACKET '('";
        case RBRACKET: return "RBRACKET ')'";
        case POWER: return "POWER (^)";
        case FACTORIAL_LOGIC_NOT: return "FACTORIAL_LOGIC_NOT (!)";
        case LOGIC_NOT: return "LOGIC_NOT (!/not)";
        case LOGIC_AND: return "LOGIC_AND (&/and)";
        case LOGIC_OR: return "LOGIC_OR (|/or)";
        case LOGIC_XOR: return "LOGIC_XOR (xor)";
        case LOGIC_NOR: return "LOGIC_NOR (nor)";
        case LOGIC_NAND: return "LOGIC_NAND (nand)";
        case LOGIC_TRUE: return "LOGIC_TRUE (true)";
        case LOGIC_FALSE: return "LOGIC_FALSE (false)";
        case COMP_EQUALT: return "COMP_EQUALT (==)";
        case COMP_UNEQUAL: return "COMP_UNEQUAL (!=)";
        case COMP_GREATER: return "COMP_GREATER (>)";
        case COMP_SMALLER: return "COMP_SMALLER (<)";
        case COMP_GEQUAL: return "COMP_GEQUAL (>=)";
        case COMP_SEQUAL: return "COMP_SEQUAL (<=)";
    }
    return "unknown";
}



/******************************************************************************************
 * JKQTPMathParser
 ******************************************************************************************/

// class constructor
JKQTPMathParser::JKQTPMathParser() {
    jkmathparser_exception_function=nullptr;
    data=nullptr;
    argc=0;
    argv=nullptr;
    addStandardFunctions();
    addStandardVariables();
}

void JKQTPMathParser::addStandardVariables(){
    addVariableDouble("pi", M_PI);
    addVariableDouble("e", M_E);
    addVariableDouble("sqrt2", sqrt(2));
    addVariableString("version", "0.2");
    addVariableDouble("log2e", M_LOG2E);
    addVariableDouble("log10e", M_LOG10E);
    addVariableDouble("ln2", M_LN2);
    addVariableDouble("ln10", M_LN10);
    addVariableDouble("h", 6.6260689633E-34);
    addVariableDouble("hbar", 1.05457162853E-34);
    addVariableDouble("epsilon0", 8.854187817E-12);
    addVariableDouble("mu0", 12.566370614E-7);
    addVariableDouble("c", 299792458);
    addVariableDouble("ce", 1.60217648740E-19);
    addVariableDouble("muB", 927.40091523E-26);
    addVariableDouble("muB_eV", 5.788381755579E-5);
    addVariableDouble("muN", 5.0507832413E-27);
    addVariableDouble("muN_eV", 3.152451232645E-8);
    addVariableDouble("me", 9.1093821545E-31);
    addVariableDouble("mp", 1.67262163783E-27);
    addVariableDouble("mn", 1.67492721184E-27);
    addVariableDouble("NA", 6.0221417930E23);
    addVariableDouble("kB", 1.380650424E-23);
    addVariableDouble("kB_eV", 8.61734315E-5);
}

void JKQTPMathParser::addStandardFunctions(){
    addFunction("sinc", fSinc);
    addFunction("asin", fASin);
    addFunction("acos", fACos);
    addFunction("atan", fATan);
    addFunction("atan2", fATan2);
    addFunction("sin", fSin);
    addFunction("cos", fCos);
    addFunction("tan", fTan);
    addFunction("sinh", fSinh);
    addFunction("cosh", fCosh);
    addFunction("tanh", fTanh);
    addFunction("log", fLog);
    addFunction("log2", fLog2);
    addFunction("log10", fLog10);
    addFunction("exp", fExp);
    addFunction("sqrt", fSqrt);
    addFunction("cbrt", fCbrt);
    addFunction("sqr", fSqr);
    addFunction("abs", fAbs);
    addFunction("if", fIf);
    addFunction("erf", fErf);
    addFunction("erfc", fErfc);
    addFunction("lgamma", flGamma);
    addFunction("tgamma", ftGamma);
    addFunction("j0", fJ0);
    addFunction("j1", fJ1);
    addFunction("jn", fJn);
    addFunction("y0", fY0);
    addFunction("y1", fY1);
    addFunction("yn", fYn);
    addFunction("rand", fRand);
    addFunction("srand", fSRand);
    addFunction("ceil", fCeil);
    addFunction("floor", fFloor);
    addFunction("trunc", fTrunc);
    addFunction("round", fRound);
    addFunction("fmod", fFMod);
    addFunction("min", fMin);
    addFunction("max", fMax);
    addFunction("inttostr", fIntToStr);
    addFunction("floattostr", fFloatToStr);
    addFunction("num2str", fFloatToStr);
    addFunction("booltostr", fBoolToStr);
    addFunction("bool2str", fBoolToStr);
    addFunction("gauss", QFSPIMLightsheetEvaluationItem_fGauss);
    addFunction("slit", fSlit);
    addFunction("theta", fTheta);
    addFunction("tanc", fTanc);
    addFunction("sigmoid", fSigmoid);
    addFunction("sign", fSign);
}

// class destructor
JKQTPMathParser::~JKQTPMathParser()
{
    clearFunctions();
    clearVariables();
}

void JKQTPMathParser::setData(void *__value)
{
    this->data = __value;
}

void *JKQTPMathParser::getData() const
{
    return this->data;
}

void JKQTPMathParser::addVariableDouble(const std::string& ni, double* v)
{
    std::string name=strip(ni);
    jkmpVariable nv;
    nv.type=JKQTPMathParser::jkmpDouble;
    nv.num=v;
    if (variableExists(name)) nv.internal=variables[name].internal; else nv.internal=false;
    variables[name]=nv;
}

void JKQTPMathParser::addVariableString(const std::string& ni, std::string* v)
{
    std::string name=strip(ni);
    jkmpVariable nv;
    nv.type=JKQTPMathParser::jkmpString;
    nv.str=v;
    if (variableExists(name)) nv.internal=variables[name].internal; else nv.internal=false;
    variables[name]=nv;
}

void JKQTPMathParser::addVariableBoolean(const std::string& ni, bool* v)
{
    std::string name=strip(ni);
    jkmpVariable nv;
    nv.type=JKQTPMathParser::jkmpBool;
    nv.boolean=v;
    if (variableExists(name)) nv.internal=variables[name].internal; else nv.internal=false;
    variables[name]=nv;
}

void JKQTPMathParser::addVariableDouble(const std::string& name, double v)
{
    jkmpVariable nv;
    nv.type=JKQTPMathParser::jkmpDouble;
    nv.num=new double;//static_cast<double*>(malloc(sizeof(double)));
    nv.internal=true;
    *(nv.num)=v;
//  std::cout<<*(nv.num)<<std::endl;
    variables[strip(name)]=nv;
}

void JKQTPMathParser::addVariableString(const std::string& name, const std::string& v)
{
    jkmpVariable nv;
    nv.type=JKQTPMathParser::jkmpString;
    nv.str=new std::string;
    nv.internal=true;
    *(nv.str)=v;
    variables[strip(name)]=nv;
}

void JKQTPMathParser::addVariableBoolean(const std::string& name, bool v)
{
    jkmpVariable nv;
    nv.type=JKQTPMathParser::jkmpBool;
    nv.boolean=new bool;
    nv.internal=true;
    *(nv.boolean)=v;
    variables[strip(name)]=nv;
}

void JKQTPMathParser::addVariable(const std::string& name, JKQTPMathParser::jkmpResult result)
{
    switch(result.type) {
        case jkmpDouble:
            addVariableDouble(name, result.num);
            break;
        case jkmpString:
            addVariableString(name, result.str);
            break;
        case jkmpBool:
            addVariableBoolean(name, result.boolean);
            break;
    }

}


void JKQTPMathParser::printVariables() {
    if (variables.size()>0) {
        for(std::map<std::string, jkmpVariable>::const_iterator i=variables.begin(); i!=variables.end(); ++i) {
            jkmpVariable v=variables[i->first];
            std::cout<<"'"<<i->first.c_str()<<"'"<<"\t\t";
            if (v.internal) std::cout<<"intern"; else std::cout<<"extern";
            std::cout<<"\t";
            if (v.type==JKQTPMathParser::jkmpBool) std::cout<<"bool\t";
            if (v.type==JKQTPMathParser::jkmpDouble) std::cout<<"double\t";
            if (v.type==JKQTPMathParser::jkmpString) std::cout<<"string\t";

            std::cout<<std::endl;

        }
    }
}

std::vector<std::pair<std::string, JKQTPMathParser::jkmpVariable> > JKQTPMathParser::getVariables()
{
    std::vector<std::pair<std::string, jkmpVariable> > result;
    if (variables.size()>0) {
        for(std::map<std::string, jkmpVariable>::const_iterator i=variables.begin(); i!=variables.end(); ++i) {
            jkmpVariable v=variables[i->first];
            result.push_back(make_pair(i->first, v));
        }
    }
    return result;
}


void JKQTPMathParser::deleteVariable(const std::string& name) {
    if (variableExists(name)) {
        jkmpVariable v=variables[name];
        if (v.internal) {
            if (v.type==JKQTPMathParser::jkmpDouble) { delete v.num; }
            else if (v.type==JKQTPMathParser::jkmpString) { delete v.str; }
            else if (v.type==JKQTPMathParser::jkmpBool) { delete v.boolean; }
        }
        variables.erase(variables.find(name));
    }
}

void JKQTPMathParser::clearFunctions() {functions.clear();}


void JKQTPMathParser::clearVariables(){
    if (variables.size()>0) {
        for(std::map<std::string, jkmpVariable>::const_iterator i=variables.begin(); i!=variables.end(); ++i) {
            jkmpVariable v=variables[i->first];
            if (v.internal) {
                if (v.type==JKQTPMathParser::jkmpDouble) { delete v.num; }
                else if (v.type==JKQTPMathParser::jkmpString) { delete v.str; }
                else if (v.type==JKQTPMathParser::jkmpBool) { delete v.boolean; }
            }

        }
        variables.clear();
    }
}

// gibt den aktuellen Wert einer Variablen zur�ck
JKQTPMathParser::jkmpResult JKQTPMathParser::getVariable(const std::string& name)
{
    if (variableExists(name)) {
        jkmpVariable v=variables[name];
        JKQTPMathParser::jkmpResult r;
        r.type=v.type;
        if (v.type==JKQTPMathParser::jkmpDouble) {
            r.num=*(v.num);
        } else if (v.type==JKQTPMathParser::jkmpString) {
            r.str=*(v.str);
        } else if (v.type==JKQTPMathParser::jkmpBool) {
            r.boolean=*(v.boolean);
        }
        return r;
    } else {
        // error
        //std::cout <<"error for:  '"<<name<<"'"<<std::endl;
        jkmpError(jkqtp_format("variable '%s' does not exist (getVariable)", name.c_str()));
    }
    JKQTPMathParser::jkmpResult res;
    res.isValid=false;
    return res;
}

JKQTPMathParser::jkmpResult JKQTPMathParser::getVariableOrInvalid(const std::string& name)
{
    if (variableExists(name)) {
        jkmpVariable v=variables[name];
        JKQTPMathParser::jkmpResult r;
        r.type=v.type;
        if (v.type==JKQTPMathParser::jkmpDouble) {
            r.num=*(v.num);
        } else if (v.type==JKQTPMathParser::jkmpString) {
            r.str=*(v.str);
        } else if (v.type==JKQTPMathParser::jkmpBool) {
            r.boolean=*(v.boolean);
        }
        return r;
    }
    JKQTPMathParser::jkmpResult res;
    res.isValid=false;
    return res;
}

JKQTPMathParser::jkmpVariable JKQTPMathParser::getVariableDef(const std::string& name)
{
  if (variableExists(name)) {
    return variables[name];
  } else {
    // error
    //std::cout <<"error for:  '"<<name<<"'"<<std::endl;
    jkmpError(jkqtp_format("variable '%s' does not exist (getVariableDef)", name.c_str()));
  }
  return jkmpVariable();
}

JKQTPMathParser::jkmpEvaluateFunc JKQTPMathParser::getFunctionDef(const std::string& name){
  if (functionExists(name)) {
    return functions[name].function;
  } else {
    // error
    //std::cout <<name<<std::endl;
    jkmpError(jkqtp_format("function '%s' does not exist (getFunctionDef)", name.c_str()));
  }
  return nullptr;
}

bool JKQTPMathParser::tempvariableExists(const std::string &name){
    if (tempvariables.size()<=0)  return false;
    for (int i=static_cast<int>(tempvariables.size())-1; i>=0; i--) {
        if (tempvariables[static_cast<size_t>(i)].name==name) return true;
    }
    return false;
}

bool JKQTPMathParser::variableExists(const std::string &name){ return tempvariableExists(name)||(variables.find(name)!=variables.end()); }

bool JKQTPMathParser::functionExists(const std::string &name){ return !(functions.find(name)==functions.end()); }

void JKQTPMathParser::addTempVariable(const std::string& name, JKQTPMathParser::jkmpResult value) {
    jkmpTempVariable v;
  v.name=name;
  v.type=value.type;
  v.internal=true;
  if (v.type==JKQTPMathParser::jkmpDouble) { v.num=new double; }
  else if (v.type==JKQTPMathParser::jkmpString) { v.str=new std::string; }
  else if (v.type==JKQTPMathParser::jkmpBool) { v.boolean=new bool; }
  tempvariables.push_back(v);
}

void JKQTPMathParser::setVariableDouble(const std::string& name, double value) {
    JKQTPMathParser::jkmpResult r;
    r.type=JKQTPMathParser::jkmpDouble;
    r.num=value;
    setVariable(name, r);
}

void JKQTPMathParser::setVariable(const std::string& name, JKQTPMathParser::jkmpResult value)
{
    bool nexist=!variableExists(name);
    jkmpVariable v=variables[name];
    v.type=value.type;
    if (value.type==JKQTPMathParser::jkmpDouble) {
    if (nexist) {v.num=new double; v.internal=true;}
    *(v.num)=value.num;
  } else if (value.type==JKQTPMathParser::jkmpString) {
    if (nexist) {v.str=new std::string; v.internal=true;}
    *(v.str)=value.str;
  } else if (value.type==JKQTPMathParser::jkmpBool) {
    if (nexist) {v.boolean=new bool; v.internal=true;}
    *(v.boolean)=value.boolean;
  }
  variables[name]=v;
}


// wertet eine Funktion aus
JKQTPMathParser::jkmpResult JKQTPMathParser::evaluateFunction(const std::string& name, JKQTPMathParser::jkmpResult* params, unsigned char n)
{
  if (functionExists(name)) {
    //std::cout <<"found"    ;
    return functions[name].function(params, n, this);
  } else {
    // error
    //jkmpError("function does not exist")
    jkmpError(jkqtp_format("function '%s' does not exist (evaluateFunction)", name.c_str()));
  }
  JKQTPMathParser::jkmpResult res;
  res.isValid=false;
  return res;

}

void JKQTPMathParser::addFunction(const std::string& name, jkmpEvaluateFunc function) {
  jkmpFunctionDescriptor f;
  f.function=function;
  f.name=name;
  functions[name]=f;
}


JKQTPMathParser::jkmpTokenType JKQTPMathParser::getToken(){
    char ch=0;
    while(program->get(ch) && isspace(ch)) {
        ;
    }


    switch (ch) {
        case 0:
        case -1:
            return CurrentToken=END;
        case ';':
            return CurrentToken=PRINT;
        case '*':
            return CurrentToken=MUL;
        case '/':
            return CurrentToken=DIV;
        case '%':
            return CurrentToken=MODULO;
        case '+':
            return CurrentToken=PLUS;
        case '-':
            return CurrentToken=MINUS;
        case '(':
            return CurrentToken=LBRACKET;
        case ')':
            return CurrentToken=RBRACKET;
        case ',':
            return CurrentToken=PARAMETER_DIV;
        case '"':
            return CurrentToken=STRING_DELIM;
        case '^':
            return CurrentToken=POWER;
        case '!':{
            char ch1=0;
            if (*program) program->get(ch1);
            if (ch1=='=') return CurrentToken=COMP_UNEQUAL;
            // else
            program->putback(ch1);
            return CurrentToken=FACTORIAL_LOGIC_NOT;
        }
        case '&':{
            char ch1=0;
            if (*program) program->get(ch1);
            if (ch1=='&') return CurrentToken=LOGIC_AND;
            // else
            program->putback(ch1);
            jkmpError("undefined operator '&'; Did you mean LOGICAL_AND ('&&' / 'and')?");
            break;
        }
        case '|':{
            char ch1=0;
            if (*program) program->get(ch1);
            if (ch1=='|') return CurrentToken=LOGIC_OR;
            // else
            program->putback(ch1);
            jkmpError("undefined operator '|'; Did you mean LOGICAL_OR ('||' / 'or')?");
            break;
        }
        case '=':{
            char ch1=0;
            if (*program) program->get(ch1);
            if (ch1=='=') return CurrentToken=COMP_EQUALT;
            // else
            program->putback(ch1);
            return CurrentToken=ASSIGN;
        }
        case '>':{
            char ch1=0;
            if (*program) program->get(ch1);
            if (ch1=='=') return CurrentToken=COMP_GEQUAL;
            // else
            program->putback(ch1);
            return CurrentToken=COMP_GREATER;
        }
        case '<':{
            char ch1=0;
            if (*program) program->get(ch1);
            if (ch1=='=') return CurrentToken=COMP_SEQUAL;
            // else
            program->putback(ch1);
            return CurrentToken=COMP_SMALLER;
        }
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':{
            program->putback(ch);
            (*program) >> NumberValue;
            return CurrentToken=NUMBER;
        }
        default:
            if (isalpha(ch) || (ch=='_')) { // try to recognize NAME, LOGIC_TRUE, LOGIC_FALSE, DIFF_LBRACKET
                StringValue=ch;
                while (program->get(ch) && (isalnum(ch) || (ch=='_') || (ch=='.'))) {
                    if (isalnum(ch) || (ch=='_') || (ch=='.')) {
                        StringValue.push_back(ch);
                    }
                }

                program->putback(ch); // now put the last thing read back int the stream, as it
                                      // could belong to the next token

                if (StringValue=="true") return CurrentToken=LOGIC_TRUE;
                if (StringValue=="false") return CurrentToken=LOGIC_FALSE;
                if (StringValue=="and") return CurrentToken=LOGIC_AND;
                if (StringValue=="or") return CurrentToken=LOGIC_OR;
                if (StringValue=="xor") return CurrentToken=LOGIC_XOR;
                if (StringValue=="not") return CurrentToken=LOGIC_NOT;
                if (StringValue=="nor") return CurrentToken=LOGIC_NOR;
                if (StringValue=="nand") return CurrentToken=LOGIC_NAND;

                return CurrentToken=NAME;
            }
            // the parser has found an unknown token. an exception will be thrown
            //std::cout<<StringValue<<",   "<<ch<<std::endl;
            jkmpError(jkqtp_format("unknown token currentCharacter='%s', currentString='%s'", jkqtp_chartostr(ch).c_str(), StringValue.c_str()));
    }
    return END;
}


JKQTPMathParser::jkmpNode* JKQTPMathParser::parse(const std::string& prog){
    program=new std::istringstream(prog);
    JKQTPMathParser::jkmpNode* res=nullptr;
    JKQTPMathParser::jkmpNodeList* resList=new JKQTPMathParser::jkmpNodeList(this);
    while(true) {
        getToken();
        if (CurrentToken == END) {
            break;
        }
        res= logicalExpression(false);
        resList->add(res);
    }
    delete program;
    if (resList->getCount()==1) {
        delete resList;
        return res;
    } else {
        return resList;
    }
}

JKQTPMathParser::jkmpResult JKQTPMathParser::evaluate(const std::string& prog) {
    JKQTPMathParser::jkmpNode* res=parse(prog);
    JKQTPMathParser::jkmpResult r=res->evaluate();
    delete res;
    return r;
}

JKQTPMathParser::jkmpNode* JKQTPMathParser::logicalExpression(bool get){
    JKQTPMathParser::jkmpNode* left=logicalTerm(get);

    for(;;) // forever, do until you find anything else than an expressions
        switch(CurrentToken) {
            case LOGIC_OR:
                left= new jkmpBinaryBoolNode(jkmpLOPor, left, logicalTerm(true), this, nullptr);
                break;
            case LOGIC_XOR:
                left= new jkmpBinaryBoolNode(jkmpLOPxor, left, logicalTerm(true), this, nullptr);
                break;
            case LOGIC_NOR:
                left= new jkmpBinaryBoolNode(jkmpLOPnor, left, logicalTerm(true), this, nullptr);
                break;
            default:
                return left;
        }
}

JKQTPMathParser::jkmpNode* JKQTPMathParser::logicalTerm(bool get){
    JKQTPMathParser::jkmpNode* left=compExpression(get);

    for(;;) // forever, do until you find anything else than an expressions
        switch(CurrentToken) {
            case LOGIC_AND:
                left= new jkmpBinaryBoolNode(jkmpLOPand, left, compExpression(true), this, nullptr);
                break;
            case LOGIC_NAND:
                left= new jkmpBinaryBoolNode(jkmpLOPnand, left, compExpression(true), this, nullptr);
                break;
            default:
                return left;
        }
}

JKQTPMathParser::jkmpNode* JKQTPMathParser::compExpression(bool get){
    JKQTPMathParser::jkmpNode* left=mathExpression(get);

    for(;;) // forever, do until you find anything else than an expressions
        switch(CurrentToken) {
            case COMP_EQUALT:
                left= new jkmpCompareNode(jkmpCOMPequal, left, mathExpression(true), this, nullptr);
                break;
            case COMP_UNEQUAL:
                left= new jkmpCompareNode(jkmpCOMPnequal, left, mathExpression(true), this, nullptr);
                break;
            case COMP_GREATER:
                left= new jkmpCompareNode(jkmpCOMPgreater, left, mathExpression(true), this, nullptr);
                break;
            case COMP_SMALLER:
                left= new jkmpCompareNode(jkmpCOMPlesser, left, mathExpression(true), this, nullptr);
                break;
            case COMP_GEQUAL:
                left= new jkmpCompareNode(jkmpCOMPgreaterequal, left, mathExpression(true), this, nullptr);
                break;
            case COMP_SEQUAL:
                left= new jkmpCompareNode(jkmpCOMPlesserequal, left, mathExpression(true), this, nullptr);
                break;
            default:
                return left;
        }
}


JKQTPMathParser::jkmpNode* JKQTPMathParser::mathExpression(bool get){
    JKQTPMathParser::jkmpNode* left=mathTerm(get);

    for(;;) // forever, do until you find anything else than an expressions
        switch(CurrentToken) {
            case PLUS:
                left= new jkmpBinaryArithmeticNode('+', left, mathTerm(true), this, nullptr);
                break;
            case MINUS:
                left= new jkmpBinaryArithmeticNode('-', left, mathTerm(true), this, nullptr);
                break;
            default:
                return left;
        }
}

JKQTPMathParser::jkmpNode* JKQTPMathParser::mathTerm(bool get){
    JKQTPMathParser::jkmpNode* left=primary(get);

    for(;;) // forever, do until you find anything else than a term
        switch(CurrentToken) {
            case MUL:
                left= new jkmpBinaryArithmeticNode('*', left, primary(true), this, nullptr);
                break;
            case DIV:
                left= new jkmpBinaryArithmeticNode('/', left, primary(true), this, nullptr);
                break;
            case MODULO:
                left= new jkmpBinaryArithmeticNode('%', left, primary(true), this, nullptr);
                break;
            default:
                return left;
        }
}

JKQTPMathParser::jkmpNode* JKQTPMathParser::primary(bool get){
    JKQTPMathParser::jkmpNode* res=nullptr;

    if (get) getToken();

    switch(CurrentToken) {
        case NUMBER: {
            JKQTPMathParser::jkmpResult val;
            val.type=JKQTPMathParser::jkmpDouble;
            val.num=NumberValue;
            getToken();
            res= new jkmpConstantNode(val, this, nullptr);
            break;
        }

        case NAME: {
            //JKQTPMathParser::jkmpNode* def=nullptr;
            std::string varname=StringValue;
        getToken();
            if (CurrentToken == ASSIGN) { // assign a variable name
                res=new jkmpVariableAssignNode(varname, logicalExpression(true), this, nullptr);
            } else if (CurrentToken == LBRACKET) { // function found
                JKQTPMathParser::jkmpNode** params=static_cast<JKQTPMathParser::jkmpNode**>(malloc(255*sizeof(JKQTPMathParser::jkmpNode*)));
                unsigned char num=0;


            getToken();
            while ((CurrentToken != RBRACKET)&&(CurrentToken!=END)) {
                JKQTPMathParser::jkmpNode* parameter=logicalExpression(num>0);
                params[num]=parameter;
                num++;
                if ((CurrentToken!=RBRACKET)&&(CurrentToken!=PARAMETER_DIV)&&(CurrentToken!=END))
                 jkmpError(jkqtp_format("')' or ',' expected, but '%s' found", currenttokentostring().c_str()));
            }

                    if ( CurrentToken != RBRACKET ) jkmpError(jkqtp_format("')' expected, but '%s' found", currenttokentostring().c_str()));;
                    JKQTPMathParser::jkmpNode** p=nullptr;
            if (num>0) {
               p=static_cast<JKQTPMathParser::jkmpNode**>(malloc(sizeof(JKQTPMathParser::jkmpNode*) * num));
               for (int i=0; i<num; i++) {
                 p[i]=params[i];
               }
            }
                    res=new jkmpFunctionNode(varname, p, num, this, nullptr);
                    free(params);
                    getToken();

        } else {
          res=new jkmpVariableNode(varname, this, nullptr);
            }
            //res= def;
            break;

        }

        case STRING_DELIM: {// found primary: "data" == string constant
            JKQTPMathParser::jkmpResult val;
            val.type=JKQTPMathParser::jkmpString;
            val.str=readDelim('"');
            res= new jkmpConstantNode(val, this, nullptr);
            getToken();
            break;
        }

        case MINUS:  // found primary:  - primary
            res= new jkmpUnaryNode('-', primary(true), this, nullptr);
            break;

        case LOGIC_NOT:
            res= new jkmpUnaryNode('!', primary(true), this, nullptr);
            break;

        case LBRACKET: { // found primary ( expression )
            JKQTPMathParser::jkmpNode* expr=logicalExpression(true);
            if (CurrentToken != RBRACKET) jkmpError(jkqtp_format("')' expected, but '%s' found", currenttokentostring().c_str()));
            getToken(); // swallow ")"
            res= expr;
            break;
        }
        case FACTORIAL_LOGIC_NOT:
    case LOGIC_TRUE: {// found 'true'
            JKQTPMathParser::jkmpResult val;
            val.type=JKQTPMathParser::jkmpBool;
            val.boolean=true;
            res= new jkmpConstantNode(val, this, nullptr);
            getToken();
            break;
   }
        case LOGIC_FALSE: {// found 'false'
            JKQTPMathParser::jkmpResult val;
            val.type=JKQTPMathParser::jkmpBool;
            val.boolean=false;
            res= new jkmpConstantNode(val, this, nullptr);
            getToken();
            break;
   }

        default:
        jkmpError("primary expected");

    }

    if (CurrentToken==POWER) {
        res=new jkmpBinaryArithmeticNode('^', res, primary(true), this, nullptr);
    }

    return res;

}

std::string JKQTPMathParser::readDelim(char delimiter){
    std::string res="";
    char ch=0;

    while(program->get(ch)) {
        if (ch==delimiter ) {
            const char ch1=static_cast<char>(program->peek());
            if (ch1==delimiter) {
                    program->get(ch);
                    res=res+delimiter;
            } else {
                break;
            }
        } else if ((*program) && (ch!=delimiter)) res=res+ch;
    }

    return res;
}









/******************************************************************************************
 * Klassenhierarchie, um Ausdr�cke darzustellen
 ******************************************************************************************/
JKQTPMathParser::jkmpUnaryNode::jkmpUnaryNode(char op, JKQTPMathParser::jkmpNode* c, JKQTPMathParser* p, JKQTPMathParser::jkmpNode* par){
  child=c;
  child->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKQTPMathParser::jkmpUnaryNode::~jkmpUnaryNode() {delete child;}

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpUnaryNode::evaluate(){
  JKQTPMathParser::jkmpResult c=child->evaluate();
  JKQTPMathParser::jkmpResult res;
  res=c;

  switch(operation) {
    case '!':
         if (c.type==JKQTPMathParser::jkmpBool) {
           res.boolean=!c.boolean;
           return res;
         } else parser->jkmpError("'!' only defined for bool");
         break;
    case '-':
         if (c.type==JKQTPMathParser::jkmpDouble) {
           res.num=-c.num;
           return res;
         } else parser->jkmpError("'-' only defined for double");
         break;
     default: parser->jkmpError("unknown unary operation");
  }
  res.isValid=false;
  return res;
}







JKQTPMathParser::jkmpBinaryArithmeticNode::jkmpBinaryArithmeticNode(char op, JKQTPMathParser::jkmpNode* l, JKQTPMathParser::jkmpNode* r, JKQTPMathParser* p, JKQTPMathParser::jkmpNode* par){
  left=l;
  right=r;
  left->setParent(this);
  right->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKQTPMathParser::jkmpBinaryArithmeticNode::~jkmpBinaryArithmeticNode() { delete left; delete right;}

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpBinaryArithmeticNode::evaluate(){
    JKQTPMathParser::jkmpResult l=left->evaluate();
  JKQTPMathParser::jkmpResult r=right->evaluate();
  JKQTPMathParser::jkmpResult res;

  switch(operation) {
    case '+':
         if (l.type==r.type) {
            if (l.type==JKQTPMathParser::jkmpDouble) {
               res.type=JKQTPMathParser::jkmpDouble;
               res.num=l.num+r.num;
               return res;
            }
            if (l.type==JKQTPMathParser::jkmpString) {
               res.type=JKQTPMathParser::jkmpString;
               res.str=l.str+r.str;
               return res;
            }
            parser->jkmpError("bool may not be added");
         } else parser->jkmpError("different Types while adding");
         break;
    case '-':
         if (l.type==r.type) {
            if (l.type==JKQTPMathParser::jkmpDouble) {
               res.type=JKQTPMathParser::jkmpDouble;
               res.num=l.num-r.num;
               return res;
            }
            parser->jkmpError("bool or string may not be subtractet");
         } else parser->jkmpError("different Types while subtracting");
         break;
    case '*':
         if (l.type==r.type) {
            if (l.type==JKQTPMathParser::jkmpDouble) {
               res.type=JKQTPMathParser::jkmpDouble;
               res.num=l.num*r.num;
               return res;
            }
            parser->jkmpError("bool or string may not be multiplied");
         } else parser->jkmpError("different Types while multiplying");
         break;
    case '/':
         if (l.type==r.type) {
            if (l.type==JKQTPMathParser::jkmpDouble) {
               res.type=JKQTPMathParser::jkmpDouble;
               res.num=l.num/r.num;
               return res;
            }
            parser->jkmpError("bool or string may not be divided");
         } else parser->jkmpError("different Types while dividing");
         break;
    case '%':
         if (l.type==r.type) {
            if (l.type==JKQTPMathParser::jkmpDouble) {
               if ((l.num==floor(l.num)) && (r.num==floor(r.num)))  {
                   res.type=JKQTPMathParser::jkmpDouble;
                   res.num=l.num/r.num;
                   return res;
               } else {
                   parser->jkmpError("modulo is only defined for integer arguments");
               }
            }
            parser->jkmpError("modulo is not defined for bool or string arguments");
         } else parser->jkmpError("different Types while calculating modulo");
         break;
    case '^':
         if (l.type==r.type) {
            if (l.type==JKQTPMathParser::jkmpDouble) {
               res.type=JKQTPMathParser::jkmpDouble;
               res.num=pow(l.num, r.num);
               return res;
            }
            parser->jkmpError("bool or string may not be taken to powers");
         } else parser->jkmpError("different types while taking to power");
         break;
     default: parser->jkmpError("unknown arithmetic operation");
  }
  res.isValid=false;
  return res;
}







JKQTPMathParser::jkmpCompareNode::jkmpCompareNode(char op, JKQTPMathParser::jkmpNode* l, JKQTPMathParser::jkmpNode* r, JKQTPMathParser* p, JKQTPMathParser::jkmpNode* par){
  left=l;
  right=r;
  left->setParent(this);
  right->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKQTPMathParser::jkmpCompareNode::~jkmpCompareNode() { delete left; delete right;}

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpCompareNode::evaluate(){
    JKQTPMathParser::jkmpResult l=left->evaluate();
  JKQTPMathParser::jkmpResult r=right->evaluate();
  JKQTPMathParser::jkmpResult res;
  res.type=JKQTPMathParser::jkmpBool;

  if (l.type!=r.type) parser->jkmpError("you can't compare different datatypes");

  switch(operation) {
    case jkmpCOMPequal:
        if (l.type==JKQTPMathParser::jkmpDouble) {
           res.boolean=(l.num==r.num);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpBool) {
           res.boolean=(l.boolean==r.boolean);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpString) {
           res.boolean=(l.str==r.str);
           return res;
        }
        break;
    case jkmpCOMPnequal:
        if (l.type==JKQTPMathParser::jkmpDouble) {
           res.boolean=(l.num!=r.num);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpBool) {
           res.boolean=(l.boolean!=r.boolean);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpString) {
           res.boolean=(l.str!=r.str);
           return res;
        }
        break;
    case jkmpCOMPgreater:
        if (l.type==JKQTPMathParser::jkmpDouble) {
           res.boolean=(l.num>r.num);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpBool) {
           res.boolean=(l.boolean>r.boolean);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpString) {
           res.boolean=(l.str>r.str);
           return res;
        }
        break;
    case jkmpCOMPlesser:
        if (l.type==JKQTPMathParser::jkmpDouble) {
           res.boolean=(l.num<r.num);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpBool) {
           res.boolean=(l.boolean<r.boolean);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpString) {
           res.boolean=(l.str<r.str);
           return res;
        }
        break;
    case jkmpCOMPgreaterequal:
        if (l.type==JKQTPMathParser::jkmpDouble) {
           res.boolean=(l.num>=r.num);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpBool) {
           res.boolean=(l.boolean>=r.boolean);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpString) {
           res.boolean=(l.str>=r.str);
           return res;
        }
        break;
    case jkmpCOMPlesserequal:
        if (l.type==JKQTPMathParser::jkmpDouble) {
           res.boolean=(l.num<=r.num);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpBool) {
           res.boolean=(l.boolean<=r.boolean);
           return res;
        }
        if (l.type==JKQTPMathParser::jkmpString) {
           res.boolean=(l.str<=r.str);
           return res;
        }
        break;
     default: parser->jkmpError("unknown compare operation");
  }
  res.isValid=false;
  return res;
}







JKQTPMathParser::jkmpBinaryBoolNode::jkmpBinaryBoolNode(char op, JKQTPMathParser::jkmpNode* l, JKQTPMathParser::jkmpNode* r, JKQTPMathParser* p, JKQTPMathParser::jkmpNode* par){
  left=l;
  right=r;
  left->setParent(this);
  right->setParent(this);
  setParser(p);
  setParent(par);
  operation=op;
}

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpBinaryBoolNode::evaluate(){
  JKQTPMathParser::jkmpResult l=left->evaluate();
  JKQTPMathParser::jkmpResult r=right->evaluate();
  JKQTPMathParser::jkmpResult res;

  if ((l.type!=JKQTPMathParser::jkmpBool)||(r.type!=JKQTPMathParser::jkmpBool)) parser->jkmpError("logical operations only for bool");

  switch(operation) {
    case jkmpLOPand:
        res.type=JKQTPMathParser::jkmpBool;
        res.boolean=l.boolean&&r.boolean;
        return res;
    case jkmpLOPor:
        res.type=JKQTPMathParser::jkmpBool;
        res.boolean=l.boolean||r.boolean;
        return res;
    case jkmpLOPnor:
        res.type=JKQTPMathParser::jkmpBool;
        res.boolean=!(l.boolean||r.boolean);
        return res;
    case jkmpLOPxor:
        res.type=JKQTPMathParser::jkmpBool;
        res.boolean=(l.boolean&& (!r.boolean))||(r.boolean&& (!l.boolean));
        return res;
    case jkmpLOPnand:
        res.type=JKQTPMathParser::jkmpBool;
        res.boolean=!(l.boolean&&r.boolean);
        return res;
     default: parser->jkmpError("unknown error");
  }
  res.isValid=false;
  return res;
}


JKQTPMathParser::jkmpBinaryBoolNode::~jkmpBinaryBoolNode() { delete left; delete right;}

JKQTPMathParser::jkmpVariableNode::jkmpVariableNode(const std::string& name, JKQTPMathParser* p, JKQTPMathParser::jkmpNode* par) {
  var=name;
  setParser(p);
  setParent(par);
};

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpVariableNode::evaluate() {
  return getParser()->getVariable(var);
};




void JKQTPMathParser::jkmpNodeList::add(JKQTPMathParser::jkmpNode* n){
  list.push_back(n);
}


JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpNodeList::evaluate(){
  int n=getCount();
  JKQTPMathParser::jkmpResult res;
  //std::cout<<"Nodelist.count()="<<n<<std::endl;
  if (n>0) {
     for (size_t i=0; i<static_cast<size_t>(n); i++) {
        res=list[i]->evaluate();
     }
     return res;
  } else parser->jkmpError("NodeList empty");
  res.isValid=false;
  return res;
}

int JKQTPMathParser::jkmpNodeList::getCount() {
    return static_cast<int>(list.size());
}

JKQTPMathParser::jkmpNodeList::jkmpNodeList(JKQTPMathParser *p) {
    setParser(p); setParent(nullptr);
}

JKQTPMathParser::jkmpNodeList::~jkmpNodeList() = default;


JKQTPMathParser::jkmpVariableAssignNode::~jkmpVariableAssignNode() {
    delete child;
}

JKQTPMathParser::jkmpVariableAssignNode::jkmpVariableAssignNode(const std::string& var, JKQTPMathParser::jkmpNode* c, JKQTPMathParser* p, JKQTPMathParser::jkmpNode* par){
  child=c;
  child->setParent(this);
  setParser(p);
  setParent(par);
  variable=var;
//  std::cout<<"assign: "<<var<<std::endl;
}

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpVariableAssignNode::evaluate(){
  JKQTPMathParser::jkmpResult res=child->evaluate();
//  std::cout<<"assign: "<<variable<<"    "<<res.num<<std::endl;
  getParser()->setVariable(variable, res);
  return res;
}

JKQTPMathParser::jkmpFunctionNode::jkmpFunctionNode(const std::string& name, JKQTPMathParser::jkmpNode** c, unsigned char num, JKQTPMathParser* p, JKQTPMathParser::jkmpNode* par) {
  child=c;
  n=num;
  fun=name;
  setParser(p);
  setParent(par);
  if (n>0) {
    for (int i=0; i<n; i++) {
      child[i]->setParent(this);
    }
  }
  function=p->getFunctionDef(name);
}

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpFunctionNode::evaluate() {
  JKQTPMathParser::jkmpResult data[255];
  if (n>0) {
    for (int i=0; i<n; i++) {
      data[i]=child[i]->evaluate();
    }
  }
//  JKQTPMathParser::jkmpResult r= getParser()->evaluateFunction(fun, data,n);
  return function(data,n, parser);
}


JKQTPMathParser::jkmpFunctionNode::~jkmpFunctionNode() {
  /*if (n>0) {
    for (int i=0; i<n; i++) {
      delete child[i];
    }
  }*/
    if ((child!=nullptr) && (n>0)) {
      for (int i=0; i<n; i++) {
        delete child[i];
      }
      free(child);
    }
}

JKQTPMathParser::jkmpResult::jkmpResult()
{
    isValid=true;
    type=jkmpDouble;   /*!< \brief type of the result */
    str="";       /*!< \brief contains result if \c type==jkmpString */
    num=0;            /*!< \brief contains result if \c type==jkmpDouble */
    boolean=false;          /*!< \brief contains result if \c type==jkmpBool */
}

std::string JKQTPMathParser::jkmpResult::toString() {
    switch(type) {
        case jkmpDouble: return jkqtp_floattostr(num);
        case jkmpString: return str;
        case jkmpBool: return jkqtp_booltostr(boolean);
    }
    return "";
}

std::string JKQTPMathParser::jkmpResult::toTypeString() {
    switch(type) {
        case jkmpDouble: return jkqtp_floattostr(num)+" [number]";
        case jkmpString: return str+" [string]";
        case jkmpBool: return jkqtp_booltostr(boolean)+" [bool]";
    }
    return "";
}

JKQTPMathParser::jkmpVariable::jkmpVariable()
{
    type=jkmpDouble;     /*!< \brief type of the variable */
    internal=false;           /*!< \brief this is an internal variable */
    str=nullptr;        /*!< \brief this points to the variable data if \c type==jkmpString */
    num=nullptr;             /*!< \brief this points to the variable data if \c type==jkmpDouble */
    boolean=nullptr;
}

void JKQTPMathParser::setArgCV(int argc, char **argv) {
    this->argc=argc;
    this->argv=argv;
}

std::string JKQTPMathParser::getArgCVParam(const std::string& name, const std::string& defaultResult) {
        if (!argv ) return defaultResult;
        if (argc<=1) return defaultResult;
    for (int i=1; i<argc; i++) {
        if (name==std::string(argv[i]) && i+1<argc) {
            return std::string(argv[i+1]);
        }
    }
    return defaultResult;
}

JKQTPMathParser::jkmpNode::~jkmpNode() = default;

JKQTPMathParser::jkmpConstantNode::jkmpConstantNode(JKQTPMathParser::jkmpResult d, JKQTPMathParser *p, JKQTPMathParser::jkmpNode *par) { data=d; setParser(p); setParent(par); }

JKQTPMathParser::jkmpResult JKQTPMathParser::jkmpConstantNode::evaluate() { return data; }

JKQTPMathParser::jkmpException::~jkmpException() = default;

JKQTPMathParser::jkmpException::jkmpException() {
    errormessage="unknown error";
}

JKQTPMathParser::jkmpException::jkmpException(const std::string &msg) {
    errormessage=msg;
}

std::string JKQTPMathParser::jkmpException::getMessage() const {
    return errormessage;
}

const char *JKQTPMathParser::jkmpException::what() const noexcept {
    return errormessage.c_str();
}

JKQTPMathParser *JKQTPMathParser::jkmpNode::getParser(){ return parser; }

void JKQTPMathParser::jkmpNode::setParser(JKQTPMathParser *mp){ parser=mp; }

JKQTPMathParser::jkmpNode *JKQTPMathParser::jkmpNode::getParent(){ return parent; }

void JKQTPMathParser::jkmpNode::setParent(JKQTPMathParser::jkmpNode *par) { parent=par; }
