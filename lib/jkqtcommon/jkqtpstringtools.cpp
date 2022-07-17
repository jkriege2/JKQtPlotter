/*
Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

    

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
#include <cmath>
#include <QDebug>
#include <QSet>
#include <QApplication>
#include <QClipboard>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QAction>
#include <QLocale>
#include <QDialog>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif
#include <QHeaderView>
#include <QDialogButtonBox>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <sstream>
#include <locale>
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


 std::string jkqtp_floattounitstr(double data, int past_comma, bool remove_trail0){
   if (data==0) return "0";
   std::string form="%."+jkqtp_inttostr(past_comma)+"lf";
   std::string res=jkqtp_format(form,data);
   if (fabs(data)>=1e3) res=jkqtp_format(form,data/1e3)+"k";
   if (fabs(data)>=1e6) res=jkqtp_format(form,data/1e6)+"M";
   if (fabs(data)>=1e9) res=jkqtp_format(form,data/1e9)+"G";
   if (fabs(data)>=1e12) res=jkqtp_format(form,data/1e12)+"T";
   if (fabs(data)>=1e15) res=jkqtp_format(form,data/1e15)+"P";
   if (fabs(data)<1) res=jkqtp_format(form,data/1e-3)+"m";
   if (fabs(data)<1e-3) res=jkqtp_format(form,data/1e-6)+"u";
   if (fabs(data)<1e-6) res=jkqtp_format(form,data/1e-9)+"n";
   if (fabs(data)<1e-9) res=jkqtp_format(form,data/1e-12)+"f";
   if (fabs(data)==0) res=jkqtp_format(form,data);
   if (remove_trail0) {
       if (data==0) return "0";
       if (res.find('.')==std::string::npos) return res;
       size_t i=res.size()-1;
       while (i>0 && res[i]=='0') {
           i--;
       }
       if (res[i]=='.') i--; // remove decimal divider
       return res.erase(i+1);
   }
   return res;
 }

 std::string jkqtp_floattolatexstr(double data, int past_comma, bool remove_trail0, double belowIsZero, double minNoExponent, double maxNoExponent, bool ensurePlusMinus){
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
           res=v+std::string("{\\times}10^{")+jkqtp_inttostr(exp)+"}";
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

Qt::BrushStyle jkqtp_String2QBrushStyle(const QString& style) {
    QString s=style.toLower().trimmed();
    if (s=="none") return Qt::NoBrush;
    if (s=="d1") return Qt::Dense1Pattern;
    if (s=="d2") return Qt::Dense2Pattern;
    if (s=="d3") return Qt::Dense3Pattern;
    if (s=="d4") return Qt::Dense4Pattern;
    if (s=="d5") return Qt::Dense5Pattern;
    if (s=="d6") return Qt::Dense6Pattern;
    if (s=="d7") return Qt::Dense7Pattern;
    if (s=="hor") return Qt::HorPattern;
    if (s=="ver") return Qt::VerPattern;
    if (s=="cross") return Qt::CrossPattern;
    if (s=="bdiag") return Qt::BDiagPattern;
    if (s=="vdiag") return Qt::FDiagPattern;
    if (s=="diagcross") return Qt::DiagCrossPattern;
    /*if (s=="lingrad") return Qt::LinearGradientPattern;
    if (s=="radgrad") return Qt::RadialGradientPattern;
    if (s=="congrad") return Qt::ConicalGradientPattern;*/
    return Qt::SolidPattern;
}


std::string jkqtp_booltostr(bool data){
  if (data) return "true";
  return "false";
}



#ifdef rgb
#  undef rgb
#endif
#define rgb(r,g,b) (0xff000000 | (r << 16) |  (g << 8) | b)

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
    { "transparent", 0 },
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
    static QStringList sl;
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


QColor jkqtp_lookupQColorName(const QString &color) {
   const QString col=color.toLower().trimmed();
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
   if (col=="window") return QGuiApplication::palette().color(QPalette::Window);
   if (col=="shadow") return QGuiApplication::palette().color(QPalette::Shadow);
   if (col=="highlight") return QGuiApplication::palette().color(QPalette::Highlight);
   if (col=="highlightedtext") return QGuiApplication::palette().color(QPalette::HighlightedText);
   if (col=="link") return QGuiApplication::palette().color(QPalette::Link);
   if (col=="linkvisited") return QGuiApplication::palette().color(QPalette::LinkVisited);
   if (col=="alternatebase") return QGuiApplication::palette().color(QPalette::AlternateBase);
   if (col=="norole") return QGuiApplication::palette().color(QPalette::NoRole);
   if (col=="tooltipbase") return QGuiApplication::palette().color(QPalette::ToolTipBase);
   if (col=="tooltiptext") return QGuiApplication::palette().color(QPalette::ToolTipText);
   if (col=="placeholdertext") return QGuiApplication::palette().color(QPalette::PlaceholderText);


   for (int i=0; i<rgbTblSize; i++) {
       if (col==rgbTbl[i].name) return QColor(rgbTbl[i].value);
   }
   return QColor(color);
}

QColor jkqtp_String2QColor(const QString &color)
{
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    QRegularExpression rxP("(.+)\\s*,\\s*t?\\s*(\\d+\\.?\\d+)\\%");
    QRegularExpression rxAP("(.+)\\s*,\\s*a\\s*(\\d+\\.?\\d+)\\%");
    QRegularExpression rxNP("(.+)\\s*,\\s*a?\\s*([\\d]+)");
    const auto mP=rxP.match(color);
    if (mP.hasMatch()) {
        QColor col=jkqtp_lookupQColorName(mP.captured(1));
        double a=QLocale::c().toDouble(mP.captured(2));
        col.setAlphaF(1.0-a/100.0);
        return col;
    }
    const auto mAP=rxAP.match(color);
    if (mAP.hasMatch()) {
        QColor col=jkqtp_lookupQColorName(mAP.captured(1));
        double a=QLocale::c().toDouble(mAP.captured(2));
        col.setAlphaF(a/100.0);
        return col;
    }
    const auto mNP=rxNP.match(color);
    if (mNP.hasMatch()) {
        QColor col=jkqtp_lookupQColorName(mNP.captured(1));
        double a=QLocale::c().toInt(mNP.captured(2));
        col.setAlphaF(a/255.0);
        return col;
    }
#else
    QRegExp rxP("(.+)\\s*,\\s*t?\\s*(\\d+\\.?\\d+)\\%");
    QRegExp rxAP("(.+)\\s*,\\s*a\\s*(\\d+\\.?\\d+)\\%");
    QRegExp rxNP("(.+)\\s*,\\s*a?\\s*([\\d]+)");
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
#endif
    return jkqtp_lookupQColorName(color);
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
