/*
Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"
#include <cmath>
#include <QDebug>
#include <QSet>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QAction>
#include <QLocale>
#include <QDialog>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPrinter>
#include <QHeaderView>
#include <QPrintDialog>
#include <QDialogButtonBox>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <locale>

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

 std::string jkqtp_format(const std::string& templ, ...){
  va_list ap;
  char buffer[4096];
  va_start (ap, templ);
  vsprintf (buffer, templ.c_str(), ap);
  va_end (ap);
  std::string ret(buffer);
  return ret;
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
          long i=r.size()-1;
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

 std::string jkqtp_floattolatexstr(double data, int past_comma, bool remove_trail0, double belowIsZero, double minNoExponent, double maxNoExponent){
   if ((belowIsZero>0) && (fabs(data)<belowIsZero)) return "\\rm{0}";
   if (data==0) return "\\rm{0}";
   double adata=fabs(data);
   std::string res=jkqtp_floattostr(data, past_comma, remove_trail0);
   /*std::string form="%."+inttostr(past_comma)+"lf";
   std::string res=jkqtp_format(form,data);
   std::string s="";
   if (data<0) s="-";*/

   long exp=(long)floor(log(adata)/log(10.0));
   //std::cout<<"data="<<data<<"   res="<<res<<"   exp="<<exp<<"   past_comma="<<past_comma<<std::endl;
   //if (exp==0 || exp==-1 || exp==1) return res;
   if ((minNoExponent<=fabs(data)) && (fabs(data)<=maxNoExponent)) return res;
   //if ((-past_comma<exp) && (exp<past_comma)) return res;

   //std::cout<<"adata="<<adata<<"   log(adata)/log(10)="<<log(adata)/log(10.0)<<"   exp="<<exp<<"   adata/pow(10, exp)="<<adata/pow(10.0, (double)exp)<<"\n";
   std::string v=jkqtp_floattostr(data/pow(10.0, static_cast<double>(exp)), past_comma, remove_trail0);
   //std::cout<<"floattolatexstr: v="<<v<<"   exp="<<exp<<std::endl;
   if (v!="1" && v!="10")  return v+std::string("{\\times}10^{")+jkqtp_inttostr(exp)+"}";
   if (v=="10") exp=exp+1;
   return std::string("10^{")+jkqtp_inttostr(exp)+"}";
 }

 std::string jkqtp_floattohtmlstr(double data, int past_comma, bool remove_trail0, double belowIsZero, double minNoExponent, double maxNoExponent){
   std::string result;
   if ((belowIsZero>0) && (fabs(data)<belowIsZero)) return "0";
   if (data==0) return "0";
   double adata=fabs(data);
   std::string res=jkqtp_floattostr(data, past_comma, remove_trail0);

   long exp=(long)floor(log(adata)/log(10.0));
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



QString JKQTPCADrawMode2String(JKQTPCADrawMode pos) {
    switch(pos) {
        case JKQTPCADMcomplete: return "all";
        case JKQTPCADMticksAndLabels: return "ticks+labels";
        case JKQTPCADMticks: return "ticks";
        case JKQTPCADMline: return "line";
        case JKQTPCADMnone: return "none";
    }
    return "";
}

JKQTPCADrawMode String2JKQTPCADrawMode(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="all") return JKQTPCADMcomplete;
    if (s=="ticks+labels") return JKQTPCADMticksAndLabels;
    if (s=="ticks") return JKQTPCADMticks;
    if (s=="line") return JKQTPCADMline;
    if (s=="none") return JKQTPCADMnone;
    return JKQTPCADMnone;
}




QString JKQTPLabelTickMode2String(JKQTPLabelTickMode pos) {
    switch(pos) {
        case JKQTPLTMLinOrPower: return "lin_or_power";
        case JKQTPLTMLin: return "lin";
        case JKQTPLTMPower: return "power";
    }
    return "";
}

JKQTPLabelTickMode String2JKQTPLabelTickMode(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="lin_or_power") return JKQTPLTMLinOrPower;
    if (s=="lin") return JKQTPLTMLin;
    if (s=="power") return JKQTPLTMPower;
    return JKQTPLTMLinOrPower;
}



QString JKQTPCALabelType2String(JKQTPCALabelType pos) {
    switch(pos) {
        case JKQTPCALTdefault: return "default";
        case JKQTPCALTexponentCharacter: return "exponent_character";
        case JKQTPCALTexponent: return "exponent";
        case JKQTPCALTtime: return "time";
        case JKQTPCALTdate: return "date";
        case JKQTPCALTdatetime: return "datetime";
    }
    return "";
}

JKQTPCALabelType String2JKQTPCALabelType(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="default") return JKQTPCALTdefault;
    if (s=="exponent_character") return JKQTPCALTexponentCharacter;
    if (s=="exponent") return JKQTPCALTexponent;
    if (s=="time") return JKQTPCALTtime;
    if (s=="date") return JKQTPCALTdate;
    if (s=="datetime") return JKQTPCALTdatetime;
    return JKQTPCALTdefault;
}

QString JKQTPLabelPosition2String(JKQTPLabelPosition pos) {
    switch(pos) {
        case JKQTPLabelMin: return "min";
        case JKQTPLabelMax: return "max";
        case JKQTPLabelCenter: return "center";
    }
    return "";
}

JKQTPLabelPosition String2JKQTPLabelPosition(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="min") return JKQTPLabelMin;
    if (s=="max") return JKQTPLabelMax;
    if (s=="center") return JKQTPLabelCenter;
    return JKQTPLabelCenter;
}

QString JKQTPKeyPosition2String(JKQTPKeyPosition pos) {
    switch(pos) {
        case JKQTPKeyOutsideLeftBottom: return "outside_leftbottom";
        case JKQTPKeyOutsideTopLeft: return "outside_topleft";
        case JKQTPKeyOutsideTopRight: return "outside_topright";
        case JKQTPKeyOutsideLeftTop: return "outside_lefttop";

        case JKQTPKeyOutsideRightBottom: return "outside_rightbottom";
        case JKQTPKeyOutsideBottomLeft: return "outside_bottomleft";
        case JKQTPKeyOutsideBottomRight: return "outside_bottomright";
        case JKQTPKeyOutsideRightTop: return "outside_righttop";

        case JKQTPKeyInsideBottomRight: return "inside_bottomright";
        case JKQTPKeyInsideTopLeft: return "inside_topleft";
        case JKQTPKeyInsideTopRight: return "inside_topright";
        case JKQTPKeyInsideBottomLeft: return "inside_bottomleft";
    }
    return "";
}

JKQTPKeyPosition String2JKQTPKeyPosition(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="outside_bottom" || s=="outsidebottom" || s=="outside_leftbottom" || s=="outsideleftbottom" || s=="olb") return JKQTPKeyOutsideLeftBottom;
    if (s=="outside_left" || s=="outsideleft" || s=="outside_topleft" || s=="outsidetopleft" || s=="otl") return JKQTPKeyOutsideTopLeft;
    if (s=="outside_right" || s=="outsideright" || s=="outside_topright" || s=="outsidetopright" || s=="otr") return JKQTPKeyOutsideTopRight;
    if (s=="outside_top" || s=="outsidetop" || s=="outside_lefttop" || s=="outsidelefttop" || s=="olt") return JKQTPKeyOutsideLeftTop;

    if (s=="outside_rightbottom" || s=="outsiderightbottom" || s=="orb" ) return JKQTPKeyOutsideRightBottom;
    if (s=="outside_bottomleft" || s=="outsidebottomleft" || s=="obl" ) return JKQTPKeyOutsideBottomLeft;
    if (s=="outside_bottomright" || s=="outsidebottomright" || s=="obr" ) return JKQTPKeyOutsideBottomRight;
    if (s=="outside_righttop" || s=="outsiderighttop" || s=="ort" ) return JKQTPKeyOutsideRightTop;

    if (s=="inside_bottom" || s=="insidebottom" || s=="inside_bottomright" || s=="insidebottomright" || s=="ibr") return JKQTPKeyInsideBottomRight;
    if (s=="inside_top" || s=="insidetop" || s=="inside_left" || s=="insideleft" || s=="inside_topleft" || s=="insidetopleft" || s=="itl") return JKQTPKeyInsideTopLeft;
    if (s=="inside_right" || s=="insideright" || s=="inside_topright" || s=="insidetopright" || s=="itr") return JKQTPKeyInsideTopRight;
    if (s=="inside_bottomleft" || s=="insidebottomleft"  || s=="ibl") return JKQTPKeyInsideBottomLeft;
    return JKQTPKeyInsideTopRight;
}


QString JKQTPKeyLayout2String(JKQTPKeyLayout pos) {
    switch(pos) {
        case JKQTPKeyLayoutOneColumn: return "one_column";
        case JKQTPKeyLayoutOneRow: return "one_row";
        case JKQTPKeyLayoutMultiColumn: return "multi_column";
    }
    return "";
}

JKQTPKeyLayout String2JKQTPKeyLayout(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="one_column" || s=="onecolumn" || s=="one") return JKQTPKeyLayoutOneColumn;
    if (s=="one_row" || s=="onerow") return JKQTPKeyLayoutOneRow;
    if (s=="multi_column" || s=="multicolumn" || s=="multi") return JKQTPKeyLayoutMultiColumn;
    return JKQTPKeyLayoutOneColumn;
}


QString JKQTPErrorPlotstyle2String(JKQTPErrorPlotstyle pos) {
    switch(pos) {
        case JKQTPErrorBoxes: return "error_box";
        case JKQTPErrorEllipses: return "error_ell";
        case JKQTPErrorLines: return "error_lines";
        case JKQTPErrorBars: return "error_bars";
        case JKQTPErrorSimpleBars: return "error_simplebars";
        case JKQTPErrorPolygons: return "error_polygons";
        case JKQTPErrorBarsLines: return "error_bars_lines";
        case JKQTPErrorBarsPolygons: return "error_bars_polygons";
        case JKQTPErrorSimpleBarsLines: return "error_simplebars_lines";
        case JKQTPErrorSimpleBarsPolygons: return "error_simplebars_polygons";
        case JKQTPNoError: return "error_none";
    }
    return "";
}

JKQTPErrorPlotstyle String2JKQTPErrorPlotstyle(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="error_lines") return JKQTPErrorLines;
    if (s=="error_box") return JKQTPErrorBoxes;
    if (s=="error_ell") return JKQTPErrorEllipses;
    if (s=="error_bars") return JKQTPErrorBars;
    if (s=="error_simplebars") return JKQTPErrorSimpleBars;
    if (s=="error_polygons") return JKQTPErrorPolygons;
    if (s=="error_bars_lines") return JKQTPErrorBarsLines;
    if (s=="error_bars_polygons") return JKQTPErrorBarsPolygons;
    if (s=="error_simplebars_lines") return JKQTPErrorSimpleBarsLines;
    if (s=="error_simplebars_polygons") return JKQTPErrorSimpleBarsPolygons;
    return JKQTPNoError;
}





QString JKQTPGraphSymbols2String(JKQTPGraphSymbols pos) {
    switch(pos) {
        case JKQTPDot: return "symbol_dot";
        case JKQTPCross: return "symbol_cross";
        case JKQTPPlus: return "symbol_plus";
        case JKQTPCircle: return "symbol_circle";
        case JKQTPFilledCircle: return "symbol_filled_circle";
        case JKQTPRect: return "symbol_rect";
        case JKQTPFilledRect: return "symbol_filled_rect";
        case JKQTPTriangle: return "symbol_triangle";
        case JKQTPFilledTriangle: return "symbol_filled_triangle";
        case JKQTPDownTriangle: return "symbol_down_triangle";
        case JKQTPFilledDownTriangle: return "symbol_filled_down_triangle";
        case JKQTPTarget: return "symbol_target";
        case JKQTPstar: return "symbol_star";
        case JKQTPFilledStar: return "symbol_filled_star";
        case JKQTPDiamond: return "symbol_diamond";
        case JKQTPFilledDiamond: return "symbol_filled_diamond";
        case JKQTPPentagon: return "symbol_pentagon";
        case JKQTPAsterisc: return "symbol_asterisc";
        case JKQTPFilledPentagon: return "symbol_filled_pentagon";
        case JKQTPRectCross: return "symbol_rect_cross";
        case JKQTPRectPlus: return "symbol_rect_plus";
        case JKQTPNoSymbol: return "none";
    }
    return "";
}

QString JKQTPGraphSymbols2NameString(JKQTPGraphSymbols pos) {
    switch(pos) {
        case JKQTPDot: return QObject::tr("dot");
        case JKQTPCross: return QObject::tr("cross");
        case JKQTPPlus: return QObject::tr("plus");
        case JKQTPCircle: return QObject::tr("circle");
        case JKQTPFilledCircle: return QObject::tr("filled circle");
        case JKQTPRect: return QObject::tr("rectangle");
        case JKQTPFilledRect: return QObject::tr("filled rectangle");
        case JKQTPTriangle: return QObject::tr("triangle");
        case JKQTPFilledTriangle: return QObject::tr("filled triangle");
        case JKQTPDownTriangle: return QObject::tr("down triangle");
        case JKQTPFilledDownTriangle: return QObject::tr("filled down triangle");
        case JKQTPstar: return QObject::tr("star");
        case JKQTPFilledStar: return QObject::tr("filled star");
        case JKQTPDiamond: return QObject::tr("diamond");
        case JKQTPFilledDiamond: return QObject::tr("filled diamond");
        case JKQTPPentagon: return QObject::tr("pentagon");
        case JKQTPFilledPentagon: return QObject::tr("filled pentagon");
        case JKQTPTarget: return QObject::tr("target");
        case JKQTPAsterisc: return QObject::tr("asterisc");
        case JKQTPRectCross: return QObject::tr("square with cross");
        case JKQTPRectPlus: return QObject::tr("square with plus");
        case JKQTPNoSymbol: return QObject::tr("none");
    }
    return "";
}
JKQTPGraphSymbols String2JKQTPGraphSymbols(const QString& pos)  {
    QString s=pos.trimmed().toLower();
    if (s=="symbol_dot"||s=="dot"||s==".") return JKQTPDot;
    if (s=="symbol_cross"||s=="cross"||s=="x") return JKQTPCross;
    if (s=="symbol_plus"||s=="plus"||s=="+") return JKQTPPlus;
    if (s=="symbol_circle"||s=="circle"||s=="o") return JKQTPCircle;
    if (s=="symbol_filled_circle"||s=="filled_circle"||s=="fo") return JKQTPFilledCircle;
    if (s=="symbol_rect"||s=="rect"||s=="r") return JKQTPRect;
    if (s=="symbol_filled_rect"||s=="filled_rect"||s=="fr") return JKQTPFilledRect;
    if (s=="symbol_triangle"||s=="triangle"||s=="^") return JKQTPTriangle;
    if (s=="symbol_filled_triangle"||s=="filled_triangle"||s=="f^") return JKQTPFilledTriangle;
    if (s=="symbol_down_triangle"||s=="down_triangle"||s=="v") return JKQTPDownTriangle;
    if (s=="symbol_filles_down_triangle"||s=="filles_down_triangle"||s=="symbol_filled_down_triangle"||s=="filled_down_triangle"||s=="fv") return JKQTPFilledDownTriangle;
    if (s=="symbol_target"||s=="target"||s=="t") return JKQTPTarget;
    if (s=="symbol_star"||s=="star"||s=="s") return JKQTPstar;
    if (s=="symbol_filled_star"||s=="filled_star"||s=="fs") return JKQTPFilledStar;
    if (s=="symbol_diamond"||s=="diamond"||s=="d") return JKQTPDiamond;
    if (s=="symbol_filled_diamond"||s=="filled_diamond"||s=="fd") return JKQTPFilledDiamond;
    if (s=="symbol_pentagon"||s=="pentagon"||s=="p") return JKQTPPentagon;
    if (s=="symbol_filled_pentagon"||s=="filled_pentagon"||s=="fp") return JKQTPFilledPentagon;
    if (s=="symbol_asterisc"||s=="asterisc"||s=="*") return JKQTPAsterisc;
    if (s=="symbol_rect_cross"||s=="rect_cross"||s=="rx") return JKQTPRectCross;
    if (s=="symbol_rect_plus"||s=="rect_plus"||s=="r+") return JKQTPRectPlus;
    return JKQTPNoSymbol;
}





QPolygonF jkqtpRotateRect(QRectF r, double angle) {
    QPolygonF p;
    QMatrix m;
    m.rotate(angle);
    p.append(m.map(r.bottomLeft()));
    p.append(m.map(r.bottomRight()));
    p.append(m.map(r.topRight()));
    p.append(m.map(r.topLeft()));
    return p;
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

std::string jkqtp_rgbtostring(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    if (a==255) {// only for non-transparent colors
        for (int i=0; i<rgbTblSize; i++) {
            if (rgb(r,g,b)==rgbTbl[i].value) {
                return rgbTbl[i].name;
            }
        }
        return jkqtp_format("#%02X%02X%02X", static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
    }
    // if we reach this, we have an unnamed transparent color
    return jkqtp_format("#%02X%02X%02X%02X", static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a));
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



int JKQTPAutoOutputTimer::global_indent=0;

JKQTPAutoOutputTimer::JKQTPAutoOutputTimer(const QString& message) :
    QElapsedTimer()
{
    this->indent=QString(global_indent, QLatin1Char(' '));
    global_indent+=4;
    this->message=message;
#if QT_VERSION >= 0x040800
    qDebug()<<this->indent<<"TIMER_START:  "<<message;
#else
    qDebug()<<this->indent<<"TIMER_START:  "<<message;
#endif
    start();
}

JKQTPAutoOutputTimer::~JKQTPAutoOutputTimer()
{
    #if QT_VERSION >= 0x040800
        qDebug()<<this->indent<<"TIMER_END:  "<<message<<"    DUR: "<<double(nsecsElapsed())/1.0e6<<"ms";
    #else
        qDebug()<<this->indent<<"TIMER_END:  "<<message<<"    DUR: "<<double(elapsed())/1.0e3<<"ms";
    #endif
    global_indent-=4;

}

void JKQTPAutoOutputTimer::write(const QString& message) const {
#if QT_VERSION >= 0x040800
    qDebug()<<this->indent<<"TIMER_MESSAGE:  "<<this->message<<" "<<message<<"    DUR: "<<double(nsecsElapsed())/1.0e6<<"ms";
#else
    qDebug()<<this->indent<<"TIMER_MESSAGE:  "<<this->message<<" "<<message<<"    DUR: "<<double(elapsed())/1.0e3<<"ms";
#endif
}

std::string jkqtp_chartostr(char data){
  std::ostringstream ost;
  ost<<data;
  return ost.str();
}


QString JKQTPStepType2String(JKQTPStepType pos)
{
    switch(pos) {
        case JKQTPStepLeft: return "step_left";
        case JKQTPStepCenter: return "step_center";
        case JKQTPStepRight: return "step_right";
    }
    return "";
}

JKQTPStepType String2JKQTPStepType(const QString& pos)
{
    QString s=pos.trimmed().toLower();
    if (s=="step_left"||s=="left"||s=="l") return JKQTPStepLeft;
    if (s=="step_center"||s=="center"||s=="c") return JKQTPStepCenter;
    if (s=="step_right"||s=="right"||s=="r") return JKQTPStepRight;

    return JKQTPStepLeft;
}

