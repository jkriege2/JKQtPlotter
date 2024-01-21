/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)


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



#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtcommon/jkqtpcsstools.h"
#include <QDateTime>
#include <cmath>
#include <QDebug>
#include <QSet>
#include <QGuiApplication>
#include <QtMath>
#include <QPalette>
#include <QLocale>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <sstream>
#include <locale>
#include <QCache>
#include <QSharedPointer>
#include <QMetaEnum>
#include <QGradient>
#include <QGradientStops>
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#else
#include<QRegExp>
#endif

std::string jkqtp_tolower(const std::string& s){
  std::string d;
  d="";
  std::locale loc;
  if (s.length()>0) {
    for (unsigned long i=0; i<s.length(); i++) {
        d+=std::tolower(s[i], loc);
    }
  }
  return d;
};

 bool jkqtp_strtobool(const std::string& data){
    std::string d=jkqtp_tolower(data);
  if (d=="true") return true;
  if (d=="t") return true;
  if (d=="1") return true;
  if (d=="j") return true;
  if (d=="y") return true;
  if (d=="yes") return true;
  if (d=="ja") return true;
  return false;
}
 std::string jkqtp_toupper(const std::string& s){
  std::string d;
  d="";
  std::locale loc;
  if (s.length()>0) {
    for (unsigned long i=0; i<s.length(); i++) {
        d+=std::toupper(s[i], loc);
    }
  }
  return d;
};



 std::string jkqtp_bytestostr(double bytes){
  double data=bytes;
  std::string form="%.0lf";
  std::string res=jkqtp_format(form,data);
  form="%.3lf";
  if (fabs(data)>=1024.0) res=jkqtp_format(form,data/1024.0)+" k";
  if (fabs(data)>=1024.0*1024.0) res=jkqtp_format(form,data/(1024.0*1024.0))+" M";
  if (fabs(data)>=1024.0*1024.0*1024.0) res=jkqtp_format(form,data/(1024.0*1024.0*1024.0))+" ";
  if (fabs(data)>=1024.0*1024.0*1024.0*1024.0) res=jkqtp_format(form,data/(1024.0*1024.0*1024.0*1024.0))+" G";
  if (fabs(data)>=1024.0*1024.0*1024.0*1024.0*1024.0) res=jkqtp_format(form,data/(1024.0*1024.0*1024.0*1024.0*1024.0))+" T";
  if (fabs(data)>=1024.0*1024.0*1024.0*1024.0*1024.0*1024.0) res=jkqtp_format(form,data/(1024.0*1024.0*1024.0*1024.0*1024.0*1024.0))+" E";
  if (fabs(data)==0) res="0 ";
  return res+"Bytes";
}

 std::string jkqtp_inttostr(long data){
  return jkqtp_format("%ld", data);
};

 std::string jkqtp_inttohex(long data){
  return jkqtp_format("%lX", data);
};

 std::string jkqtp_uinttostr(unsigned long data){
  std::ostringstream ost;
  ost<<data;
  return ost.str();
};

 std::string jkqtp_floattostr(double data, int past_comma, bool remove_trail0, double belowIsZero){
  if (belowIsZero>0) {
      if (fabs(data)<belowIsZero) return std::string("0");
  }

  std::string form="%."+jkqtp_inttostr(past_comma)+"lf";
  //std::string form="%lf";
  if (past_comma<=0) form="%lf";
  std::string r=jkqtp_format(form,data);
  //std::cout<<r<<std::endl;
  if (remove_trail0 && (jkqtp_tolower(r).find('e')==std::string::npos)) {
      if (data==0) return "0";
      //size_t cp=r.find(".");
      //if (cp<r.size()) return r;
      std::string re;
      size_t dpos=r.find('.');
      if (dpos==std::string::npos) {
          return r;
      } else {
          long i= static_cast<long>(r.size())-1;
          bool nonz=false;
          while (i>=0) {
              //std::cout<<i<<"\n";
              if (r[i]!='0') {
                  nonz=true;
              }
              if (nonz || (i<long(dpos))) {
                  if (re.size()==0 && r[i]=='.') {
                      // swallow decimal dot, if only 0 folowed
                  } else {
                      re=r[i]+re;
                  }
              }
              i--;
          }
          return re;
      }

  }
  return r;
}

 std::string jkqtp_floattounitstr(double dataa, const std::string& unitname){
  if (dataa==0) return jkqtp_floattostr(dataa)+unitname;
  std::string u="";
  double factor=1;
  double data=fabs(dataa);
  if (data>=1e3) { u="k"; factor=1e3; }
  if (data>=1e6) { u="M"; factor=1e6; }
  if (data>=1e9) { u="G"; factor=1e9; }
  if (data>=1e12) { u="T"; factor=1e12; }
  if (data>=1e15) { u="P"; factor=1e15; }
  if (data>=1e18) { u="E"; factor=1e18; }
  if (data<1) {u="m"; factor=1e-3; }
  if (data<1e-3) {u="u"; factor=1e-6; }
  if (data<1e-6) {u="n"; factor=1e-9; }
  if (data<1e-9) {u="p"; factor=1e-12; }
  if (data<1e-12) {u="f"; factor=1e-15; }
  if (data<1e-15) {u="a"; factor=1e-18; }

  return jkqtp_floattostr(dataa/factor)+u+unitname;
};


 std::string jkqtp_floattounitstr(double data, int past_comma, bool remove_trail0, double belowIsZero){
   if (fabs(data)<=belowIsZero) return "0";
   std::string form="%."+jkqtp_inttostr(past_comma)+"lf";
   std::string res=jkqtp_format(form,data);
   std::string unit="";
   if (fabs(data)>=1e3) {res=jkqtp_format(form,data/1e3); unit="k";}
   if (fabs(data)>=1e6) {res=jkqtp_format(form,data/1e6); unit="M";}
   if (fabs(data)>=1e9) {res=jkqtp_format(form,data/1e9); unit="G";}
   if (fabs(data)>=1e12) {res=jkqtp_format(form,data/1e12); unit="T";}
   if (fabs(data)>=1e15) {res=jkqtp_format(form,data/1e15); unit="P";}
   if (fabs(data)<1) {res=jkqtp_format(form,data/1e-3); unit="m";}
   if (fabs(data)<1e-3) {res=jkqtp_format(form,data/1e-6); unit="u";}
   if (fabs(data)<1e-6) {res=jkqtp_format(form,data/1e-9); unit="n";}
   if (fabs(data)<1e-9) {res=jkqtp_format(form,data/1e-12); unit="f";}
   if (fabs(data)==0) res=jkqtp_format(form,data);
   if (remove_trail0) {
       if (fabs(data)<=belowIsZero) return "0";
       if (res.find('.')==std::string::npos) return res;
       size_t i=res.size()-1;
       while (i>0 && res[i]=='0') {
           i--;
       }
       if (res[i]=='.') i--; // remove decimal divider
       return res.erase(i+1);
   }
   return res+unit;
 }


 std::string jkqtp_floattolatexunitstr(double data, int past_comma, bool remove_trail0, double belowIsZero){
   if (fabs(data)<=belowIsZero) return "0";
   std::string form="%."+jkqtp_inttostr(past_comma)+"lf";
   std::string res=jkqtp_format(form,data);
   std::string unit="";
   if (fabs(data)>=1e3) {res=jkqtp_format(form,data/1e3); unit="\\mathrm{k}";}
   if (fabs(data)>=1e6) {res=jkqtp_format(form,data/1e6); unit="\\;\\mathrm{M}";}
   if (fabs(data)>=1e9) {res=jkqtp_format(form,data/1e9); unit="\\;\\mathrm{G}";}
   if (fabs(data)>=1e12) {res=jkqtp_format(form,data/1e12); unit="\\;\\mathrm{T}";}
   if (fabs(data)>=1e15) {res=jkqtp_format(form,data/1e15); unit="\\;\\mathrm{P}";}
   if (fabs(data)<1) {res=jkqtp_format(form,data/1e-3); unit="\\;\\mathrm{m}";}
   if (fabs(data)<1e-3) {res=jkqtp_format(form,data/1e-6); unit="\\;\\mathrm{\\mu}";}
   if (fabs(data)<1e-6) {res=jkqtp_format(form,data/1e-9); unit="\\;\\mathrm{n}";}
   if (fabs(data)<1e-9) {res=jkqtp_format(form,data/1e-12); unit="\\;\\mathrm{f}";}
   if (fabs(data)==0) res=jkqtp_format(form,data);
   if (remove_trail0) {
       if (fabs(data)<=belowIsZero) return "0";
       if (res.find('.')==std::string::npos) return res+unit;
       size_t i=res.size()-1;
       while (i>0 && res[i]=='0') {
           i--;
       }
       if (res[i]=='.') i--; // remove decimal divider
       return res.erase(i+1)+unit;
   }
   return res+unit;
 }

 std::string jkqtp_floattolatexstr(double data, int past_comma, bool remove_trail0, double belowIsZero, double minNoExponent, double maxNoExponent, bool ensurePlusMinus, const std::string &multOperator){
   if ((belowIsZero>0) && (fabs(data)<belowIsZero)) {
       if (ensurePlusMinus) return "+\\rm{0}";
       else return "\\rm{0}";
   }
   if (fabs(data)<5.0*std::numeric_limits<double>::min()) {
       if (ensurePlusMinus) return "+\\rm{0}";
       else return "\\rm{0}";
   }

   double adata=fabs(data);
   std::string res=jkqtp_floattostr(data, past_comma, remove_trail0);

   long exp=static_cast<long>(floor(log(adata)/JKQTPSTATISTICS_LN10));
   if ((minNoExponent>fabs(data)) || (fabs(data)>maxNoExponent)) {
       std::string v=jkqtp_floattostr(data/pow(10.0, static_cast<double>(exp)), past_comma, remove_trail0);
       if (v!="1" && v!="10")  {
           res=v+std::string("{")+multOperator+std::string("}10^{")+jkqtp_inttostr(exp)+"}";
       } else {
           if (v=="10") exp=exp+1;
           res=std::string("10^{")+jkqtp_inttostr(exp)+"}";
       }
   }
   if (ensurePlusMinus && res.size()>0) {
       if (res[0]!='-' && res[0]!='+') {
           if (data<0) res="-"+res;
           else res="+"+res;
       }
   }
   return res;
 }

 std::string jkqtp_floattohtmlstr(double data, int past_comma, bool remove_trail0, double belowIsZero, double minNoExponent, double maxNoExponent){
   std::string result;
   if ((belowIsZero>0) && (fabs(data)<belowIsZero)) return "0";
   if (fabs(data)<5.0*std::numeric_limits<double>::min()) return "0";
   double adata=fabs(data);
   std::string res=jkqtp_floattostr(data, past_comma, remove_trail0);

   long exp=static_cast<long>(floor(log(adata)/JKQTPSTATISTICS_LN10));
   if ((minNoExponent<=fabs(data)) && (fabs(data)<maxNoExponent)) return res;
   //if ((-past_comma<exp) && (exp<past_comma)) result= res;
   else {
       std::string v=jkqtp_floattostr(data/pow(10.0, static_cast<double>(exp)), past_comma, remove_trail0);
       if (v!="1") result= v+std::string("&times;10<sup>")+jkqtp_inttostr(exp)+"</sup>";
       else result=std::string("10<sup>")+jkqtp_inttostr(exp)+"</sup>";
   }
   //std::cout<<"floattohtmlstr("<<data<<") = '"<<result<<"'\n";
   return result;
 }




QString jkqtp_QPenStyle2String(Qt::PenStyle style) {
    switch(style) {
        case Qt::DashLine:       return "dash";
        case Qt::DotLine:        return "dot";
        case Qt::DashDotLine:    return "dashdot";
        case Qt::DashDotDotLine: return "dashdotdot";
        case Qt::NoPen:          return "none";
        default:
        case Qt::SolidLine:      return "solid";
    }
}

Qt::PenStyle jkqtp_String2QPenStyle(const QString& style) {
    QString s=style.toLower().trimmed();
    if (s=="dash" || s=="--") return Qt::DashLine;
    if (s=="dot" || s=="." || s=="..") return Qt::DotLine;
    if (s=="dashdot" || s=="-.") return Qt::DashDotLine;
    if (s=="dashdotdot" || s=="-..") return Qt::DashDotDotLine;
    if (s=="solid" || s=="-") return Qt::SolidLine;
    if (s=="none" || s=="" || s=="n") return Qt::NoPen;
    return Qt::SolidLine;
}

QString jkqtp_QBrushStyle2String(Qt::BrushStyle style) {

    switch(style) {
        case Qt::NoBrush:       return "none";
        case Qt::Dense1Pattern:        return "d1";
        case Qt::Dense2Pattern:        return "d2";
        case Qt::Dense3Pattern:        return "d3";
        case Qt::Dense4Pattern:        return "d4";
        case Qt::Dense5Pattern:        return "d5";
        case Qt::Dense6Pattern:        return "d6";
        case Qt::Dense7Pattern:        return "d7";
        case Qt::HorPattern:           return "hor";
        case Qt::VerPattern:           return "ver";
        case Qt::CrossPattern:           return "cross";
        case Qt::BDiagPattern:           return "bdiag";
        case Qt::FDiagPattern:           return "vdiag";
        case Qt::DiagCrossPattern:           return "diagcross";
        /*case Qt::LinearGradientPattern:           return "lingrad";
        case Qt::RadialGradientPattern:           return "radgrad";
        case Qt::ConicalGradientPattern:           return "congrad";*/
        default:
        case Qt::SolidPattern:      return "solid";
    }
}

namespace {
    static QMap<QString,Qt::BrushStyle> s_String2QBrushStyleMap = []() {
        QMap<QString,Qt::BrushStyle> m;
        m["none"]=Qt::NoBrush;
        m["d1"]=Qt::Dense1Pattern;
        m["d2"]=Qt::Dense2Pattern;
        m["d3"]=Qt::Dense3Pattern;
        m["d4"]=Qt::Dense4Pattern;
        m["d5"]=Qt::Dense5Pattern;
        m["d6"]=Qt::Dense6Pattern;
        m["d7"]=Qt::Dense7Pattern;
        m["hor"]=Qt::HorPattern;
        m["ver"]=Qt::VerPattern;
        m["cross"]=Qt::CrossPattern;
        m["bdiag"]=Qt::BDiagPattern;
        m["vdiag"]=Qt::FDiagPattern;
        m["diagcross"]=Qt::DiagCrossPattern;
        return m;
    }();

    static QMap<QString,QGradient::Preset> s_GradientPresets = []() {
        QMap<QString,QGradient::Preset> m;
        for (int i=1; i<QGradient::Preset::NumPresets; i++) {
            const QString id=QString(QMetaEnum::fromType<QGradient::Preset>().valueToKey(static_cast<QGradient::Preset>(i))).toLower().trimmed().simplified();
            if (id.size()>0) m[id]=static_cast<QGradient::Preset>(i);
        }
        return m;
    }();

}

Qt::BrushStyle jkqtp_String2QBrushStyle(const QString& style) {
    QString s=style.toLower().trimmed();
    return s_String2QBrushStyleMap.value(s, Qt::SolidPattern);
}


Qt::BrushStyle jkqtp_String2QBrushStyleExt(const QString &style, QColor *color, QGradient *gradient, QPixmap *image, double* rotationAngleDeg)
{
    const QString s=style.toLower().trimmed().simplified();
    QStringList caps;

    if (s.startsWith("linear-gradient(")) {
        try {
            const auto grad=JKQTPCSSParser::readGradient(s);
            if (gradient) *gradient=grad;
            return Qt::LinearGradientPattern;
        } catch(std::exception& E) {
            qWarning()<<"error converting '"<<style<<"' into a QGradient: "<<E.what();
            return Qt::SolidPattern;
       }
    } else if (s_GradientPresets.contains(s)) {
        QGradient g(s_GradientPresets[s]);
        g.setCoordinateMode(QGradient::ObjectBoundingMode);
        if (gradient) *gradient=g;
        if (g.type()==QGradient::Type::RadialGradient) return Qt::RadialGradientPattern;
        else if (g.type()==QGradient::Type::ConicalGradient) return Qt::ConicalGradientPattern;
        else return Qt::LinearGradientPattern;
    } else if (jkqtp_rxExactlyMatches(s, "\\s*image\\s*\\(\\s*[\\\"\\\']?(.*)[\\\"\\\']?\\s*\\)\\s*", &caps)) {
        if (image) *image=QPixmap(caps[1]);
        return Qt::TexturePattern;
    } else {
        // now we parse:
        //   pattern ::= none | d1 | d2 | ... | cross | bdiag | ...
        return jkqtp_String2QBrushStyle(s);
    }

}


std::string jkqtp_booltostr(bool data){
  if (data) return "true";
  return "false";
}



#ifdef rgb
#  undef rgb
#endif
#define rgb(r,g,b) (0xff000000 | (r << 16) |  (g << 8) | b)
#define rgba(r,g,b,a) ((a << 24) | (r << 16) |  (g << 8) | b)
#define gray(g) rgb(g,g,g)
#define gray_p(p) gray(static_cast<uint8_t>(g/100.0*255.0))

static const struct RGBData {
    const char *name;
    unsigned int  value;
} rgbTbl[] = {
    { "aliceblue", rgb(240, 248, 255) },
    { "antiquewhite", rgb(250, 235, 215) },
    { "aqua", rgb( 0, 255, 255) },
    { "aquamarine", rgb(127, 255, 212) },
    { "azure", rgb(240, 255, 255) },
    { "beige", rgb(245, 245, 220) },
    { "bisque", rgb(255, 228, 196) },
    { "black", rgb( 0, 0, 0) },
    { "blanchedalmond", rgb(255, 235, 205) },
    { "blue", rgb( 0, 0, 255) },
    { "blueviolet", rgb(138, 43, 226) },
    { "brown", rgb(165, 42, 42) },
    { "burlywood", rgb(222, 184, 135) },
    { "cadetblue", rgb( 95, 158, 160) },
    { "chartreuse", rgb(127, 255, 0) },
    { "chocolate", rgb(210, 105, 30) },
    { "coral", rgb(255, 127, 80) },
    { "cornflowerblue", rgb(100, 149, 237) },
    { "cornsilk", rgb(255, 248, 220) },
    { "crimson", rgb(220, 20, 60) },
    { "cyan", rgb( 0, 255, 255) },
    { "darkblue", rgb( 0, 0, 139) },
    { "darkcyan", rgb( 0, 139, 139) },
    { "darkgoldenrod", rgb(184, 134, 11) },
    { "darkgray", rgb(169, 169, 169) },
    { "darkgreen", rgb( 0, 100, 0) },
    { "darkgrey", rgb(169, 169, 169) },
    { "darkkhaki", rgb(189, 183, 107) },
    { "darkmagenta", rgb(139, 0, 139) },
    { "darkolivegreen", rgb( 85, 107, 47) },
    { "darkorange", rgb(255, 140, 0) },
    { "darkorchid", rgb(153, 50, 204) },
    { "darkred", rgb(139, 0, 0) },
    { "darksalmon", rgb(233, 150, 122) },
    { "darkseagreen", rgb(143, 188, 143) },
    { "darkslateblue", rgb( 72, 61, 139) },
    { "darkslategray", rgb( 47, 79, 79) },
    { "darkslategrey", rgb( 47, 79, 79) },
    { "darkturquoise", rgb( 0, 206, 209) },
    { "darkviolet", rgb(148, 0, 211) },
    { "deeppink", rgb(255, 20, 147) },
    { "deepskyblue", rgb( 0, 191, 255) },
    { "dimgray", rgb(105, 105, 105) },
    { "dimgrey", rgb(105, 105, 105) },
    { "dodgerblue", rgb( 30, 144, 255) },
    { "firebrick", rgb(178, 34, 34) },
    { "floralwhite", rgb(255, 250, 240) },
    { "forestgreen", rgb( 34, 139, 34) },
    { "fuchsia", rgb(255, 0, 255) },
    { "gainsboro", rgb(220, 220, 220) },
    { "ghostwhite", rgb(248, 248, 255) },
    { "gold", rgb(255, 215, 0) },
    { "goldenrod", rgb(218, 165, 32) },
    { "gray10", gray(255*1/10) },
    { "gray20", gray(255*2/10) },
    { "gray30", gray(255*3/10) },
    { "gray40", gray(255*4/10) },
    { "gray50", gray(255*5/10) },
    { "gray60", gray(255*6/10) },
    { "gray70", gray(255*7/10) },
    { "gray80", gray(255*8/10) },
    { "gray90", gray(255*9/10) },
    { "gray", rgb(128, 128, 128) },
    { "green", rgb( 0, 128, 0) },
    { "greenyellow", rgb(173, 255, 47) },
    { "grey", rgb(128, 128, 128) },
    { "honeydew", rgb(240, 255, 240) },
    { "hotpink", rgb(255, 105, 180) },
    { "indianred", rgb(205, 92, 92) },
    { "indigo", rgb( 75, 0, 130) },
    { "ivory", rgb(255, 255, 240) },
    { "khaki", rgb(240, 230, 140) },
    { "lavender", rgb(230, 230, 250) },
    { "lavenderblush", rgb(255, 240, 245) },
    { "lawngreen", rgb(124, 252, 0) },
    { "lemonchiffon", rgb(255, 250, 205) },
    { "lightblue", rgb(173, 216, 230) },
    { "lightcoral", rgb(240, 128, 128) },
    { "lightcyan", rgb(224, 255, 255) },
    { "lightgoldenrodyellow", rgb(250, 250, 210) },
    { "lightgray", rgb(211, 211, 211) },
    { "lightgreen", rgb(144, 238, 144) },
    { "lightgrey", rgb(211, 211, 211) },
    { "lightpink", rgb(255, 182, 193) },
    { "lightsalmon", rgb(255, 160, 122) },
    { "lightseagreen", rgb( 32, 178, 170) },
    { "lightskyblue", rgb(135, 206, 250) },
    { "lightslategray", rgb(119, 136, 153) },
    { "lightslategrey", rgb(119, 136, 153) },
    { "lightsteelblue", rgb(176, 196, 222) },
    { "lightyellow", rgb(255, 255, 224) },
    { "lime", rgb( 0, 255, 0) },
    { "limegreen", rgb( 50, 205, 50) },
    { "linen", rgb(250, 240, 230) },
    { "magenta", rgb(255, 0, 255) },
    { "maroon", rgb(128, 0, 0) },
    { "mediumaquamarine", rgb(102, 205, 170) },
    { "mediumblue", rgb( 0, 0, 205) },
    { "mediumorchid", rgb(186, 85, 211) },
    { "mediumpurple", rgb(147, 112, 219) },
    { "mediumseagreen", rgb( 60, 179, 113) },
    { "mediumslateblue", rgb(123, 104, 238) },
    { "mediumspringgreen", rgb( 0, 250, 154) },
    { "mediumturquoise", rgb( 72, 209, 204) },
    { "mediumvioletred", rgb(199, 21, 133) },
    { "midnightblue", rgb( 25, 25, 112) },
    { "mintcream", rgb(245, 255, 250) },
    { "mistyrose", rgb(255, 228, 225) },
    { "moccasin", rgb(255, 228, 181) },
    { "navajowhite", rgb(255, 222, 173) },
    { "navy", rgb( 0, 0, 128) },
    { "oldlace", rgb(253, 245, 230) },
    { "olive", rgb(128, 128, 0) },
    { "olivedrab", rgb(107, 142, 35) },
    { "orange", rgb(255, 165, 0) },
    { "orangered", rgb(255, 69, 0) },
    { "orchid", rgb(218, 112, 214) },
    { "palegoldenrod", rgb(238, 232, 170) },
    { "palegreen", rgb(152, 251, 152) },
    { "paleturquoise", rgb(175, 238, 238) },
    { "palevioletred", rgb(219, 112, 147) },
    { "papayawhip", rgb(255, 239, 213) },
    { "peachpuff", rgb(255, 218, 185) },
    { "peru", rgb(205, 133, 63) },
    { "pink", rgb(255, 192, 203) },
    { "plum", rgb(221, 160, 221) },
    { "powderblue", rgb(176, 224, 230) },
    { "purple", rgb(128, 0, 128) },
    { "red", rgb(255, 0, 0) },
    { "rosybrown", rgb(188, 143, 143) },
    { "royalblue", rgb( 65, 105, 225) },
    { "saddlebrown", rgb(139, 69, 19) },
    { "salmon", rgb(250, 128, 114) },
    { "sandybrown", rgb(244, 164, 96) },
    { "seagreen", rgb( 46, 139, 87) },
    { "seashell", rgb(255, 245, 238) },
    { "sienna", rgb(160, 82, 45) },
    { "silver", rgb(192, 192, 192) },
    { "skyblue", rgb(135, 206, 235) },
    { "slateblue", rgb(106, 90, 205) },
    { "slategray", rgb(112, 128, 144) },
    { "slategrey", rgb(112, 128, 144) },
    { "snow", rgb(255, 250, 250) },
    { "springgreen", rgb( 0, 255, 127) },
    { "steelblue", rgb( 70, 130, 180) },
    { "tan", rgb(210, 180, 140) },
    { "teal", rgb( 0, 128, 128) },
    { "thistle", rgb(216, 191, 216) },
    { "tomato", rgb(255, 99, 71) },
    { "transparent", rgba(0,0,0,0) },
    { "turquoise", rgb( 64, 224, 208) },
    { "violet", rgb(238, 130, 238) },
    { "wheat", rgb(245, 222, 179) },
    { "white", rgb(255, 255, 255) },
    { "whitesmoke", rgb(245, 245, 245) },
    { "yellow", rgb(255, 255, 0) },
    { "yellowgreen", rgb(154, 205, 50) }
};

static const int rgbTblSize = sizeof(rgbTbl) / sizeof(RGBData);

const QStringList& jkqtp_listNamedColors() {
    thread_local QStringList sl;
    if (sl.size()==0) {
        sl.reserve(rgbTblSize);
        for (int i=0; i<rgbTblSize; i++) {
            sl.push_back(rgbTbl[i].name);
        }
    }
    return sl;
}

QString jkqtp_rgbtostring(unsigned char r, unsigned char g, unsigned char b, unsigned char a, bool useSpecialTransparencySyntax) {
    if (a==255) {// only for non-transparent colors
        for (int i=0; i<rgbTblSize; i++) {
            if (rgb(r,g,b)==rgbTbl[i].value) {
                return rgbTbl[i].name;
            }
        }
        if (r==g && r==b) return QString("grey%1").arg(static_cast<int>(static_cast<double>(r)/255.0*100.0));
        return QString("#%1%2%3").arg(static_cast<int>(r), 2,16,QLatin1Char('0')).arg(static_cast<int>(g), 2,16,QLatin1Char('0')).arg(static_cast<int>(b), 2,16,QLatin1Char('0'));
    }
    // if we reach this, we have an unnamed transparent color
    if (useSpecialTransparencySyntax) {
        QString col=jkqtp_rgbtostring(r,g,b,255,false);
        return QString("%1,%2").arg(col).arg(static_cast<int>(a), 0, 10);
        //return QString("%1,%2%%").arg(col).arg(static_cast<int>(a)*100/255, 0, 10);
    } else {
        return QString("#%1%2%3%4").arg(static_cast<int>(r), 2,16,QLatin1Char('0')).arg(static_cast<int>(g), 2,16,QLatin1Char('0')).arg(static_cast<int>(b), 2,16,QLatin1Char('0')).arg(static_cast<int>(a), 2,16,QLatin1Char('0'));
    }
}


QString jkqtp_QColor2String(QColor color, bool useSpecialTransparencySyntax) {
    return jkqtp_rgbtostring(static_cast<unsigned char>((color).red()),
                                     static_cast<unsigned char>((color).green()),
                                     static_cast<unsigned char>((color).blue()),
                                     static_cast<unsigned char>((color).alpha()),
                                     useSpecialTransparencySyntax);
}


QColor jkqtp_lookupQColorName(const QString &color, bool namesOnly, bool *nameFound) {
    if (nameFound) *nameFound=true;
    const QString col=color.toLower().trimmed();
    if (col=="transparent") return QColor::fromRgb(0,0,0,0);
    if (col=="window") return QGuiApplication::palette().color(QPalette::Window);
    if (col=="windowtext") return QGuiApplication::palette().color(QPalette::WindowText);
    if (col=="button") return QGuiApplication::palette().color(QPalette::Button);
    if (col=="light") return QGuiApplication::palette().color(QPalette::Light);
    if (col=="midlight") return QGuiApplication::palette().color(QPalette::Midlight);
    if (col=="dark") return QGuiApplication::palette().color(QPalette::Dark);
    if (col=="mid") return QGuiApplication::palette().color(QPalette::Mid);
    if (col=="text") return QGuiApplication::palette().color(QPalette::Text);
    if (col=="brightttext") return QGuiApplication::palette().color(QPalette::BrightText);
    if (col=="base") return QGuiApplication::palette().color(QPalette::Base);
    if (col=="shadow") return QGuiApplication::palette().color(QPalette::Shadow);
    if (col=="highlight") return QGuiApplication::palette().color(QPalette::Highlight);
    if (col=="highlightedtext") return QGuiApplication::palette().color(QPalette::HighlightedText);
    if (col=="link") return QGuiApplication::palette().color(QPalette::Link);
    if (col=="linkvisited") return QGuiApplication::palette().color(QPalette::LinkVisited);
    if (col=="alternatebase") return QGuiApplication::palette().color(QPalette::AlternateBase);
    if (col=="norole") return QGuiApplication::palette().color(QPalette::NoRole);
    if (col=="tooltipbase") return QGuiApplication::palette().color(QPalette::ToolTipBase);
    if (col=="tooltiptext") return QGuiApplication::palette().color(QPalette::ToolTipText);
#if (QT_VERSION>=QT_VERSION_CHECK(5, 12, 0))
    if (col=="placeholdertext") return QGuiApplication::palette().color(QPalette::PlaceholderText);
#endif

    for (int i=0; i<rgbTblSize; i++) {
       if (col==rgbTbl[i].name) return QColor(rgbTbl[i].value);
    }
    if (!namesOnly) return QColor(color);
    if (nameFound) *nameFound=false;
    return Qt::black;
}

QColor jkqtp_String2QColor(QString color)
{
    //qDebug()<<"jkqtp_String2QColor: "<<color;
    // simplify string an convert to lower-case
    color=color.trimmed().toLower();
    if (color.startsWith('"')||color.startsWith('\'')) color=color.mid(1);
    if (color.endsWith('"')||color.endsWith('\'')) color=color.left(color.size()-1);
    if (color.isEmpty()) return Qt::black;

    // check whether we have a names color
    {
        bool nameFound=false;
        const QColor col=jkqtp_lookupQColorName(color, true, &nameFound);
        if (nameFound) return col;
    }

    // declare som helper functors
    static auto valUnitToInt=[](const QString& v, const QString& unit="", int intMax=255) {
        if (v.isEmpty()) return -1;
        if (unit=="%") {
            return qBound<int>(0, QLocale::c().toDouble(v)/100.0*intMax, intMax);
        } else if (unit=="deg") {
            int vv=qBound<int>(0, QLocale::c().toDouble(v), intMax);;
            if (vv<0) vv=vv+static_cast<int>(qCeil(static_cast<double>(-vv)/360.0)*360.0);
            return vv;
        }
        return qBound<int>(0, QLocale::c().toDouble(v), intMax);
    };
    static auto valUnitToAlphaInt=[](const QString& v, const QString& unit="", int intMax=255) {
        if (v.isEmpty()) return -1;
        if (unit=="%") {
            return intMax-qBound<int>(0, QLocale::c().toDouble(v)/100.0*intMax, intMax);
        } else if (unit=="deg") {
            int vv=qBound<int>(0, QLocale::c().toDouble(v), intMax);;
            if (vv<0) vv=vv+static_cast<int>(qCeil(static_cast<double>(-vv)/360.0)*360.0);
            return vv;
        }
        return qBound<int>(0, QLocale::c().toDouble(v), intMax);
    };

    // now we check for diverse special syntaxes
    //      P: "color,NN%"     NN=TRANSPARENCY in percent
    //     AP: "color,aNN\%"   NN=ALPHA in percent
    //     NP: "color,[a]NN"   NN=ALPHA 0..255
    //   Frac: "grey25"
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    static QRegularExpression rxP("([^,\\(\\)]+)\\s*,\\s*t?\\s*(\\d+\\.?\\d+)\\%");
    static QRegularExpression rxFrac("([a-zA-Z]{3,})(\\d{1,3})\\%?");
    static QRegularExpression rxAP("([^,\\(\\)]+)\\s*,\\s*a\\s*(\\d+\\.?\\d+)\\%");
    static QRegularExpression rxNP("([^,\\(\\)]+)\\s*,\\s*a?\\s*([\\d]+)");
    const auto mP=rxP.match(color);

    if (mP.hasMatch() && mP.captured(0).size()==color.size()) {
        QColor col=jkqtp_String2QColor(mP.captured(1));
        const double a=QLocale::c().toDouble(mP.captured(2));
        col.setAlphaF(1.0-a/100.0);
        return col;
    }
    const auto mAP=rxAP.match(color);
    if (mAP.hasMatch() && mAP.captured(0).size()==color.size()) {
        QColor col=jkqtp_String2QColor(mAP.captured(1));
        const double a=QLocale::c().toDouble(mAP.captured(2));
        col.setAlphaF(a/100.0);
        return col;
    }
    const auto mNP=rxNP.match(color);
    if (mNP.hasMatch() && mNP.captured(0).size()==color.size()) {
        QColor col=jkqtp_String2QColor(mNP.captured(1));
        const double a=QLocale::c().toInt(mNP.captured(2));
        col.setAlphaF(a/255.0);
        return col;
    }
    const auto mFrac=rxFrac.match(color);
    if (mFrac.hasMatch() && mFrac.captured(0).size()==color.size()) {
        QColor col=jkqtp_lookupQColorName(mFrac.captured(1));
        const double a=static_cast<double>(QLocale::c().toInt(mFrac.captured(2)))/100.0;
        if (mFrac.captured(1)=="grey"||mFrac.captured(1)=="gray") col.setRgbF(a,a,a);
        else col.setRgbF(col.redF()*a, col.greenF()*a, col.blueF()*a);
        return col;
    }
#else
    QRegExp rxP("([^,\\(\\)]+)\\s*,\\s*t?\\s*(\\d+\\.?\\d+)\\%");
    QRegExp rxAP("([^,\\(\\)]+)\\s*,\\s*a\\s*(\\d+\\.?\\d+)\\%");
    QRegExp rxNP("([^,\\(\\)]+)\\s*,\\s*a?\\s*([\\d]+)");
    QRegExp rxFrac("([a-zA-Z]{3,})(\\d{1,3})\\%?");
    if (rxP.exactMatch(color)) {
        QColor col=jkqtp_lookupQColorName(rxP.cap(1));
        double a=QLocale::c().toDouble(rxP.cap(2));
        col.setAlphaF(1.0-a/100.0);
        return col;
    }
    if (rxAP.exactMatch(color)) {
        QColor col=jkqtp_lookupQColorName(rxAP.cap(1));
        double a=QLocale::c().toDouble(rxAP.cap(2));
        col.setAlphaF(a/100.0);
        return col;
    }
    if (rxNP.exactMatch(color)) {
        QColor col=jkqtp_lookupQColorName(rxNP.cap(1));
        double a=QLocale::c().toInt(rxNP.cap(2));
        col.setAlphaF(a/255.0);
        return col;
    }
    if (rxFrac.exactMatch(color)) {
        QColor col=jkqtp_lookupQColorName(rxFrac.cap(1));
        const double a=static_cast<double>(QLocale::c().toInt(rxFrac.cap(2)))/100.0;
        if (rxFrac.cap(1)=="grey"||rxFrac.cap(1)=="gray") col.setRgbF(a,a,a);
        else col.setRgbF(col.redF()*a, col.greenF()*a, col.blueF()*a);
        return col;
    }
#endif

    try {
        return JKQTPCSSParser::readColor(color);
    } catch(std::exception& E) {
        qWarning()<<"error converting '"<<color<<"' into a QColor: "<<E.what();
        return jkqtp_lookupQColorName(color);
    }
}

std::string jkqtp_to_valid_variable_name(const std::string& input) {
    std::string out="";
    for (size_t i=0; i<input.size(); i++) {
        if (isalpha(input[i])) {
            out=out+input[i];
        }
        if ((isdigit(input[i]))&&(out.size()>0)) out=out+input[i];
        if ((input[i]=='_')&&(out.size()>0)) out=out+input[i];
    }
    return out;
}




std::string jkqtp_chartostr(char data){
  std::ostringstream ost;
  ost<<data;
  return ost.str();
}

QString jkqtp_floattounitqstr(double data, int past_comma, bool remove_trail0)
{
    return QString::fromStdString(jkqtp_floattounitstr(data, past_comma, remove_trail0));
}

QString jkqtp_floattolatexqstr(double data, int past_comma, bool remove_trail0, double belowIsZero, double minNoExponent, double maxNoExponent, bool ensurePlusMinus)
{
    return QString::fromStdString(jkqtp_floattolatexstr(data, past_comma, remove_trail0, belowIsZero, minNoExponent, maxNoExponent, ensurePlusMinus));
}

QString jkqtp_floattohtmlqstr(double data, int past_comma, bool remove_trail0, double belowIsZero, double minNoExponent, double maxNoExponent)
{
    return QString::fromStdString(jkqtp_floattohtmlstr(data, past_comma, remove_trail0, belowIsZero, minNoExponent, maxNoExponent));
}


QString jkVariantListToString(const QList<QVariant>& data, const QString& separator) {
    QString r="";
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    for (int i=0; i<data.size(); i++) {
        if (i>0) r=r+separator;
        QVariant v=data[i];
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
        if (v.typeId()==QMetaType::Bool) r=r+loc.toString(v.toBool());
        else if (v.typeId()==QMetaType::Char) r=r+loc.toString(v.toInt());
        else if (v.typeId()==QMetaType::QDate) r=r+loc.toString(v.toDate());
        else if (v.typeId()==QMetaType::QDateTime) r=r+loc.toString(v.toDateTime());
        else if (v.typeId()==QMetaType::Double) r=r+loc.toString(v.toDouble());
        else if (v.typeId()==QMetaType::Int) r=r+loc.toString(v.toInt());
        else if (v.typeId()==QMetaType::LongLong) r=r+loc.toString(v.toLongLong());
        else if (v.typeId()==QMetaType::QString) r=r+QString("\"%1\"").arg(v.toString().replace("\"", "_").replace("\t", " ").replace("\r", "").replace("\n", " ").replace(",", " ").replace(";", " "));
        else if (v.typeId()==QMetaType::QTime) r=r+loc.toString(v.toTime());
        else if (v.typeId()==QMetaType::UInt) r=r+loc.toString(v.toUInt());
        else if (v.typeId()==QMetaType::ULongLong) r=r+loc.toString(v.toULongLong());
        else r=r+v.toString();

#else
        switch (v.type()) {

            case QVariant::Bool: r=r+loc.toString(v.toBool()); break;
            case QVariant::Char: r=r+loc.toString(v.toInt()); break;
            case QVariant::Date: r=r+loc.toString(v.toDate()); break;
            case QVariant::DateTime: r=r+loc.toString(v.toDateTime()); break;
            case QVariant::Double: r=r+loc.toString(v.toDouble()); break;
            case QVariant::Int: r=r+loc.toString(v.toInt()); break;
            case QVariant::LongLong: r=r+loc.toString(v.toLongLong()); break;
            case QVariant::String: r=r+QString("\"%1\"").arg(v.toString().replace("\"", "_").replace("\t", " ").replace("\r", "").replace("\n", " ").replace(",", " ").replace(";", " ")); break;
            case QVariant::Time: r=r+loc.toString(v.toTime()); break;
            case QVariant::UInt: r=r+loc.toString(v.toUInt()); break;
            case QVariant::ULongLong: r=r+loc.toString(v.toULongLong()); break;
            //case : r=r+loc.toString(v.); break;
            default: r=r+v.toString(); break;
        }
#endif
    }
    return r;
}

JKQTCOMMON_LIB_EXPORT QString jkqtp_filenameize(const QString& data) {
    QString r;
    QString data1=data.simplified();
    for (int i=0; i<data1.size(); i++) {
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
        const auto c=data1[i];
#else
        QCharRef c=data1[i];
#endif
        if (c.isLetterOrNumber() || (c=='-') || (c=='_') || (c=='.')) {
            r+=c;
        } else {
            r+='_';
        }
    }
    return r;
}

QString jkqtp_toValidVariableName(const QString& input) {
    QString out="";
    for (int i=0; i<input.size(); i++) {
        if (input[i].isLetter()) out=out+input[i];
        if (input[i].isDigit()&&(out.size()>0)) out=out+input[i];
        if ((input[i]=='_')&&(out.size()>0)) out=out+input[i];
    }
    return out;
}

QString jkqtp_KeyboardModifiers2String(Qt::KeyboardModifiers modifiers, bool useNONE)
{
    if (modifiers==Qt::NoModifier) {
        if (useNONE) return "NONE";
        else return "";
    }
    QString ret="";
    auto append=[](QString& ret, const QString & appending, const QString& separator="+") {
        if (appending.size()<=0) return;
        if (ret.size()>0) ret+=separator;
        ret+=appending;
    };
    if ((modifiers&Qt::ShiftModifier)==Qt::ShiftModifier) append(ret, "SHIFT", "+");
    if ((modifiers&Qt::ControlModifier)==Qt::ControlModifier) append(ret, "CTRL", "+");
    if ((modifiers&Qt::AltModifier)==Qt::AltModifier) append(ret, "ALT", "+");
    if ((modifiers&Qt::MetaModifier)==Qt::MetaModifier) append(ret, "META", "+");
    if ((modifiers&Qt::KeypadModifier)==Qt::KeypadModifier) append(ret, "KEYPAD", "+");
    if ((modifiers&Qt::GroupSwitchModifier)==Qt::GroupSwitchModifier) append(ret, "GROUP", "+");
    return ret;
}

Qt::KeyboardModifiers jkqtp_String2KeyboardModifiers(const QString &modifiers)
{
    auto mods=modifiers.toUpper().split("+");
    Qt::KeyboardModifiers ret=Qt::NoModifier;

    for (const auto& m: mods) {
        if (m.trimmed()=="SHIFT") ret |= Qt::ShiftModifier;
        else if (m.trimmed()=="CTRL") ret |= Qt::ControlModifier;
        else if (m.trimmed()=="ALT") ret |= Qt::AltModifier;
        else if (m.trimmed()=="META") ret |= Qt::MetaModifier;
        else if (m.trimmed()=="KEYPAD") ret |= Qt::KeypadModifier;
        else if (m.trimmed()=="GROUP") ret |= Qt::GroupSwitchModifier;
    }

    return ret;
}

QString jkqtp_MouseButton2String(Qt::MouseButton button, bool useNONE)
{
    if (button==Qt::NoButton) {
        if (useNONE) return "NONE";
        else return "";
    }
    if (button==Qt::LeftButton) return "LEFT";
    if (button==Qt::RightButton) return "RIGHT";
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (button==Qt::MiddleButton) return "MIDDLE";
#else
    if (button==Qt::MidButton) return "MIDDLE";
#endif
    if (button==Qt::BackButton) return "BACK";
    if (button==Qt::ForwardButton) return "FORWARD";
    if (button==Qt::TaskButton) return "TASK";
    if (button==Qt::ExtraButton4) return "EXTRA4";
    if (button==Qt::ExtraButton5) return "EXTRA5";
    if (button==Qt::ExtraButton6) return "EXTRA6";
    if (button==Qt::ExtraButton7) return "EXTRA7";
    if (button==Qt::ExtraButton8) return "EXTRA8";
    if (button==Qt::ExtraButton9) return "EXTRA9";
    if (button==Qt::ExtraButton10) return "EXTRA10";
    if (button==Qt::ExtraButton11) return "EXTRA11";
    if (button==Qt::ExtraButton12) return "EXTRA12";
    if (button==Qt::ExtraButton13) return "EXTRA13";
    if (button==Qt::ExtraButton14) return "EXTRA14";
    if (button==Qt::ExtraButton15) return "EXTRA15";
    if (button==Qt::ExtraButton16) return "EXTRA16";
    if (button==Qt::ExtraButton17) return "EXTRA17";
    if (button==Qt::ExtraButton18) return "EXTRA18";
    if (button==Qt::ExtraButton19) return "EXTRA19";
    if (button==Qt::ExtraButton20) return "EXTRA20";
    if (button==Qt::ExtraButton21) return "EXTRA21";
    if (button==Qt::ExtraButton22) return "EXTRA22";
    if (button==Qt::ExtraButton23) return "EXTRA23";
    if (button==Qt::ExtraButton24) return "EXTRA24";
    return "UNKNOWN";
}

Qt::MouseButton jkqtp_String2MouseButton(const QString &button)
{
    auto but=button.toUpper().trimmed();
    if (but=="LEFT") return Qt::LeftButton;
    if (but=="RIGHT") return Qt::RightButton;
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (but=="MIDDLE") return Qt::MiddleButton;
#else
    if (but=="MIDDLE") return Qt::MidButton;
#endif
    if (but=="BACK") return Qt::BackButton;
    if (but=="FORWARD") return Qt::ForwardButton;
    if (but=="TASK") return Qt::TaskButton;
    if (but=="EXTRA4") return Qt::ExtraButton4;
    if (but=="EXTRA5") return Qt::ExtraButton5;
    if (but=="EXTRA6") return Qt::ExtraButton6;
    if (but=="EXTRA7") return Qt::ExtraButton7;
    if (but=="EXTRA8") return Qt::ExtraButton8;
    if (but=="EXTRA9") return Qt::ExtraButton9;
    if (but=="EXTRA10") return Qt::ExtraButton10;
    if (but=="EXTRA11") return Qt::ExtraButton11;
    if (but=="EXTRA12") return Qt::ExtraButton12;
    if (but=="EXTRA13") return Qt::ExtraButton13;
    if (but=="EXTRA14") return Qt::ExtraButton14;
    if (but=="EXTRA15") return Qt::ExtraButton15;
    if (but=="EXTRA16") return Qt::ExtraButton16;
    if (but=="EXTRA17") return Qt::ExtraButton17;
    if (but=="EXTRA18") return Qt::ExtraButton18;
    if (but=="EXTRA19") return Qt::ExtraButton19;
    if (but=="EXTRA20") return Qt::ExtraButton20;
    if (but=="EXTRA21") return Qt::ExtraButton21;
    if (but=="EXTRA22") return Qt::ExtraButton22;
    if (but=="EXTRA23") return Qt::ExtraButton23;
    if (but=="EXTRA24") return Qt::ExtraButton24;
    return Qt::NoButton;
}

std::string jkqtp_UnicodeToUTF8(uint32_t codepoint)
{
    std::string out;

    if (codepoint <= 0x7f)
        out.append(1, static_cast<char>(codepoint));
    else if (codepoint <= 0x7ff)
    {
        out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff)
    {
        out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else
    {
        out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    return out;
}

QString jkqtp_backslashEscape(const QString &txt)
{
    QString res;
    for (const QChar c: txt) {
        if (c=='\n') res+="\\n";
        else if (c=='\r') res+="\\r";
        else if (c=='\t') res+="\\t";
        else if (c=='\\') res+="\\\\";
        else if (c.unicode()<32) res+="\\x"+QString::number(c.unicode(), 16).toUpper();
        else res+=c;
    }
    return res;
}


namespace JKQTCommon_private {
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    thread_local QCache<QString, QRegularExpression> rxCache(500);
#else
    thread_local QCache<QString, QRegExp> rxCache(500);
#endif
}

bool jkqtp_rxContains(const QString& text, const QString &regex, qsizetype offset, QStringList* caps)
{
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegularExpression(regex));
    QRegularExpression* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegularExpression> tempRX;
    if (!rx) {
        tempRX.reset(new QRegularExpression(regex));
        rx=tempRX.data();
    }
    const auto m=rx->match(text, offset);
    if (caps) *caps=m.capturedTexts();
    return m.hasMatch();
#else
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegExp(regex));
    QRegExp* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegExp> tempRX;
    if (!rx) {
        tempRX.reset(new QRegExp(regex));
        rx=tempRX.data();
    }
    const bool res= rx->indexIn(text, offset)>=0;
    if (caps) *caps=rx->capturedTexts();
    return res;
#endif
}


qsizetype jkqtp_rxIndexIn(const QString& text, const QString &regex, qsizetype offset, QStringList* caps)
{
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegularExpression(regex));
    QRegularExpression* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegularExpression> tempRX;
    if (!rx) {
        tempRX.reset(new QRegularExpression(regex));
        rx=tempRX.data();
    }
    QRegularExpressionMatch rmatch;
    const qsizetype idx=text.indexOf(*rx, offset, &rmatch);
    if (caps) *caps=rmatch.capturedTexts();
    return idx;
#else
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegExp(regex));
    QRegExp* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegExp> tempRX;
    if (!rx) {
        tempRX.reset(new QRegExp(regex));
        rx=tempRX.data();
    }
    const qsizetype idx = rx->indexIn(text, offset);
    if (caps) *caps=rx->capturedTexts();
    return idx;
#endif
}


bool jkqtp_rxPartiallyMatchesAt(const QString& text, const QString &regex, qsizetype offset, QStringList* caps)
{
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegularExpression(regex));
    QRegularExpression* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegularExpression> tempRX;
    if (!rx) {
        tempRX.reset(new QRegularExpression(regex));
        rx=tempRX.data();
    }
    const auto m=rx->match(text, offset, QRegularExpression::NormalMatch, QRegularExpression::AnchorAtOffsetMatchOption);
    if (caps) *caps=m.capturedTexts();
    return m.hasMatch();
#else
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegExp(regex));
    QRegExp* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegExp> tempRX;
    if (!rx) {
        tempRX.reset(new QRegExp(regex));
        rx=tempRX.data();
    }
    const bool res= (rx->indexIn(text, offset)==offset);
    if (caps) *caps=rx->capturedTexts();
    return res;
#endif
}



bool jkqtp_rxExactlyMatches(const QString& text, const QString &regex, QStringList* caps)
{
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegularExpression(regex));
    QRegularExpression* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegularExpression> tempRX;
    if (!rx) {
        tempRX.reset(new QRegularExpression(regex));
        rx=tempRX.data();
    }
    const auto m=rx->match(text);
    if (caps) *caps=m.capturedTexts();
    return m.hasMatch() && m.captured(0).size()==text.size();
#else
    if (!JKQTCommon_private::rxCache.contains(regex)) JKQTCommon_private::rxCache.insert(regex, new QRegExp(regex));
    QRegExp* rx=JKQTCommon_private::rxCache.object(regex);
    QSharedPointer<QRegExp> tempRX;
    if (!rx) {
        tempRX.reset(new QRegExp(regex));
        rx=tempRX.data();
    }
    const bool res= rx->exactMatch(text);
    if (caps) *caps=rx->capturedTexts();
    return res;
#endif
}
