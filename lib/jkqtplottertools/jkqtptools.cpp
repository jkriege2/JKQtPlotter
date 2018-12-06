/*
Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

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





/** \file jkqtptools.cpp
  * \ingroup jkqtptools
  */

#include "jkqtplottertools/jkqtptools.h"
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

 bool jkqtp_strtobool(std::string data){
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

 long jkqtp_get_filesize(char *FileName) {
    struct stat file;
    if(!stat(FileName,&file)) {
         return file.st_size;
    }
    return 0;
}
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

 std::string jkqtp_floattounitstr(double dataa, std::string unitname){
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
   std::string v=jkqtp_floattostr(data/pow(10.0, (double)exp), past_comma, remove_trail0);
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
       std::string v=jkqtp_floattostr(data/pow(10.0, (double)exp), past_comma, remove_trail0);
       if (v!="1") result= v+std::string("&times;10<sup>")+jkqtp_inttostr(exp)+"</sup>";
       else result=std::string("10<sup>")+jkqtp_inttostr(exp)+"</sup>";
   }
   //std::cout<<"floattohtmlstr("<<data<<") = '"<<result<<"'\n";
   return result;
 }


void JKQTPplotSymbol(QPaintDevice& paintDevice, double x, double y, JKQTPgraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor) {
    JKQTPEnhancedPainter p(&paintDevice);
    JKQTPplotSymbol(p, x, y, symbol, size, symbolLineWidth, color, fillColor);
}

void JKQTPplotSymbol(JKQTPEnhancedPainter& painter, double x, double y, JKQTPgraphSymbols symbol, double symbolSize, double symbolLineWidth, QColor color, QColor fillColor) {
    painter.save();
    QPen p=painter.pen();
    p.setColor(color);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,  symbolLineWidth));
    p.setStyle(Qt::SolidLine);
    painter.setPen(p);
    QBrush b=painter.brush();
    b.setColor(fillColor);
    b.setStyle(Qt::SolidPattern);
    const double w=symbolSize;
    const double w2=w/2.0;
    const double w3=w/3.0;

    // calculate star cordinates as static values
    static int star_items=0;
    static double starcordsx[10];
    static double starcordsy[10];
    if (star_items==0) {
        star_items=5;
        double angle=360.0/double(star_items)/180.0*M_PI;
        for (int i=0; i<star_items; i++) {
            double a=((double)i+0.5)*angle;
            starcordsx[i*2]=sin(a);
            starcordsx[i*2+1]=0.5*sin(a+angle/2.0);
            starcordsy[i*2]=cos(a);
            starcordsy[i*2+1]=0.5*cos(a+angle/2.0);
        }
    }

    switch(symbol) {
        case JKQTPdot:
            painter.drawPoint(QPointF(x,y));
            break;
        case JKQTPcross:{
                /*QVector<QLineF> lines;
                lines<<QLineF(x-w2,y-w2,x+w2,y+w2);
                lines<<QLineF(x-w2,y+w2,x+w2,y-w2);
                painter.drawLines(lines);*/
                QPainterPath path;
                path.moveTo(x-w2,y-w2);
                path.lineTo(x+w2,y+w2);
                path.moveTo(x-w2,y+w2);
                path.lineTo(x+w2,y-w2);
                painter.drawPath(path);
            }
            break;
        case JKQTPplus:{
                QVector<QLineF> lines;
                lines<<QLineF(x,y-w2,x,y+w2);
                lines<<QLineF(x-w2,y,x+w2,y);
                painter.drawLines(lines);
            }
            break;
        case JKQTPcircle:{
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawEllipse(rectangle);
            }
            break;
        case JKQTPtarget:{
                QPainterPath path;
                QRectF rectangle3(x-w3, y-w3, 2.0*w3, 2.0*w3);
                path.addEllipse(rectangle3);
                path.moveTo(QPointF(x,y-w2));
                path.lineTo(QPointF(x,y+w2));
                path.moveTo(QPointF(x-w2,y));
                path.lineTo(QPointF(x+w2,y));
                painter.drawPath(path);
            }
            break;
        case JKQTPfilledCircle:{
                painter.setBrush(b);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawEllipse(rectangle);
            }
            break;
        case JKQTPrect:{
                painter.setBrush(QColor(Qt::transparent));
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPrectCross:{
                painter.setBrush(QColor(Qt::transparent));
                QPainterPath path;
                path.moveTo(x-w2,y-w2);
                path.lineTo(x+w2,y+w2);
                path.moveTo(x-w2,y+w2);
                path.lineTo(x+w2,y-w2);
                painter.drawPath(path);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPrectPlus:{
                painter.setBrush(QColor(Qt::transparent));
                QVector<QLineF> lines;
                lines<<QLineF(x,y-w2,x,y+w2);
                lines<<QLineF(x-w2,y,x+w2,y);
                painter.drawLines(lines);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPfilledRect:{
                painter.setBrush(b);
                QRectF rectangle(x-w2, y-w2, w, w);
                painter.drawRect(rectangle);
            }
            break;
        case JKQTPtriangle: {
            painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPfilledTriangle: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y+w2)<<QPointF(x+w2, y+w2)<<QPointF(x, y-w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPdownTriangle: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            poly<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2)<<QPointF(x, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPfilledDownTriangle: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x-w2, y-w2)<<QPointF(x+w2, y-w2)<<QPointF(x, y+w2);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPdiamond: {
            QPolygonF poly;
            painter.setBrush(QColor(Qt::transparent));
            poly<<QPointF(x, y-w2)<<QPointF(x+w2, y)<<QPointF(x, y+w2)<<QPointF(x-w2, y);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPfilledDiamond: {
            painter.setBrush(b);
            QPolygonF poly;
            poly<<QPointF(x, y-w2)<<QPointF(x+w2, y)<<QPointF(x, y+w2)<<QPointF(x-w2, y);
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPstar: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            for (int i=0; i<star_items*2; i++) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPfilledStar: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star_items*2; i++) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPpentagon: {
                painter.setBrush(QColor(Qt::transparent));
            QPolygonF poly;
            for (int i=0; i<star_items*2; i+=2) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        case JKQTPasterisc: {
            //QVector<QLineF> lines;
            QPainterPath path;
            for (int i=0; i<star_items*2; i+=2) {
                //lines<<QLineF(x+starcordsx[i]*w2, y+starcordsy[i]*w2, x, y);
                path.moveTo(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
                path.lineTo(x,y);
            }
            //painter.drawLines(lines);
            painter.drawPath(path);
            } break;
        case JKQTPfilledPentagon: {
            painter.setBrush(b);
            QPolygonF poly;
            for (int i=0; i<star_items*2; i+=2) {
                poly<<QPointF(x+starcordsx[i]*w2, y+starcordsy[i]*w2);
            }
            painter.drawConvexPolygon(poly);
            } break;
        default: break;
    }
    painter.restore();
};



QVector<QPointF> JKQTPdrawEllipse(double x, double y, double a, double b, double angle_start, double angle_end, double alpha, int controlPoints, QPointF* x_start, QPointF* x_end) {
    QVector<QPointF> result;
    double start=angle_start*M_PI/180.0;
    double stop=angle_end*M_PI/180.0;
    double step=(stop-start)/(double)controlPoints;
    while (fabs(stop-start)/step<10) step=step/2.0;
    double sina=sin(1.0*alpha/180.0*M_PI);
    double cosa=cos(1.0*alpha/180.0*M_PI);
    QPointF xp(x+a*cos(start)*cosa-b*sin(start)*sina, y+a*cos(start)*sina+b*sin(start)*cosa);
    result.append(xp);
    if (x_start) *x_start = xp;
    double t=start+step;
    for (int i=1; i<controlPoints; i++) {
        double cost=cos(t);
        double sint=sin(t);
        xp=QPointF( x+a*cost*cosa-b*sint*sina,  y+a*cost*sina+b*sint*cosa);
        result.append(xp);
        //std::cout<<"t="<<t/M_PI*180.0<<":  sin(al)="<<sina<<" cos(al)="<<cosa<<" sin(t)="<<sint<<" cos(t)="<<cost<<" a="<<a<<" b="<<b<<": ("<<x+a*cost*cosa-b*sint*sina<<", "<<y+a*cost*sina+b*sint*cosa<<") = ("<<xp.x()<<", "<<xp.y()<<") \n";
        t=t+step;
    }
    if (x_end) *x_end=xp;
    return result;
}




QString JKQTP_QPenStyle2String(Qt::PenStyle style) {
    switch(style) {
        case Qt::DashLine:       return "dash";
        case Qt::DotLine:        return "dot";
        case Qt::DashDotLine:    return "dashdot";
        case Qt::DashDotDotLine: return "dashdotdot";
        default:
        case Qt::SolidLine:      return "solid";
    }
}

Qt::PenStyle JKQTP_String2QPenStyle(QString style) {
    QString s=style.toLower().trimmed();
    if (s=="dash" || s=="--") return Qt::DashLine;
    if (s=="dot" || s=="." || s=="..") return Qt::DotLine;
    if (s=="dashdot" || s=="-.") return Qt::DashDotLine;
    if (s=="dashdotdot" || s=="-..") return Qt::DashDotDotLine;
    if (s=="solid" || s=="-") return Qt::SolidLine;
    return Qt::SolidLine;
}

QString JKQTP_QBrushStyle2String(Qt::BrushStyle style) {

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

Qt::BrushStyle JKQTP_String2QBrushStyle(QString style) {
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



QString JKQTPCAdrawMode2String(JKQTPCAdrawMode pos) {
    switch(pos) {
        case JKQTPCADMcomplete: return "all";
        case JKQTPCADMticksAndLabels: return "ticks+labels";
        case JKQTPCADMticks: return "ticks";
        case JKQTPCADMline: return "line";
        case JKQTPCADMnone: return "none";
    }
    return "";
}

JKQTPCAdrawMode String2JKQTPCAdrawMode(QString pos) {
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

JKQTPLabelTickMode String2JKQTPLabelTickMode(QString pos) {
    QString s=pos.trimmed().toLower();
    if (s=="lin_or_power") return JKQTPLTMLinOrPower;
    if (s=="lin") return JKQTPLTMLin;
    if (s=="power") return JKQTPLTMPower;
    return JKQTPLTMLinOrPower;
}



QString JKQTPCAlabelType2String(JKQTPCAlabelType pos) {
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

JKQTPCAlabelType String2JKQTPCAlabelType(QString pos) {
    QString s=pos.trimmed().toLower();
    if (s=="default") return JKQTPCALTdefault;
    if (s=="exponent_character") return JKQTPCALTexponentCharacter;
    if (s=="exponent") return JKQTPCALTexponent;
    if (s=="time") return JKQTPCALTtime;
    if (s=="date") return JKQTPCALTdate;
    if (s=="datetime") return JKQTPCALTdatetime;
    return JKQTPCALTdefault;
}

QString JKQTPlabelPosition2String(JKQTPlabelPosition pos) {
    switch(pos) {
        case JKQTPlabelMin: return "min";
        case JKQTPlabelMax: return "max";
        case JKQTPlabelCenter: return "center";
    }
    return "";
}

JKQTPlabelPosition String2JKQTPlabelPosition(QString pos) {
    QString s=pos.trimmed().toLower();
    if (s=="min") return JKQTPlabelMin;
    if (s=="max") return JKQTPlabelMax;
    if (s=="center") return JKQTPlabelCenter;
    return JKQTPlabelCenter;
}

QString JKQTPkeyPosition2String(JKQTPkeyPosition pos) {
    switch(pos) {
        case JKQTPkeyOutsideLeftBottom: return "outside_leftbottom";
        case JKQTPkeyOutsideTopLeft: return "outside_topleft";
        case JKQTPkeyOutsideTopRight: return "outside_topright";
        case JKQTPkeyOutsideLeftTop: return "outside_lefttop";

        case JKQTPkeyOutsideRightBottom: return "outside_rightbottom";
        case JKQTPkeyOutsideBottomLeft: return "outside_bottomleft";
        case JKQTPkeyOutsideBottomRight: return "outside_bottomright";
        case JKQTPkeyOutsideRightTop: return "outside_righttop";

        case JKQTPkeyInsideBottomRight: return "inside_bottomright";
        case JKQTPkeyInsideTopLeft: return "inside_topleft";
        case JKQTPkeyInsideTopRight: return "inside_topright";
        case JKQTPkeyInsideBottomLeft: return "inside_bottomleft";
    }
    return "";
}

JKQTPkeyPosition String2JKQTPkeyPosition(QString pos) {
    QString s=pos.trimmed().toLower();
    if (s=="outside_bottom" || s=="outsidebottom" || s=="outside_leftbottom" || s=="outsideleftbottom" || s=="olb") return JKQTPkeyOutsideLeftBottom;
    if (s=="outside_left" || s=="outsideleft" || s=="outside_topleft" || s=="outsidetopleft" || s=="otl") return JKQTPkeyOutsideTopLeft;
    if (s=="outside_right" || s=="outsideright" || s=="outside_topright" || s=="outsidetopright" || s=="otr") return JKQTPkeyOutsideTopRight;
    if (s=="outside_top" || s=="outsidetop" || s=="outside_lefttop" || s=="outsidelefttop" || s=="olt") return JKQTPkeyOutsideLeftTop;

    if (s=="outside_rightbottom" || s=="outsiderightbottom" || s=="orb" ) return JKQTPkeyOutsideRightBottom;
    if (s=="outside_bottomleft" || s=="outsidebottomleft" || s=="obl" ) return JKQTPkeyOutsideBottomLeft;
    if (s=="outside_bottomright" || s=="outsidebottomright" || s=="obr" ) return JKQTPkeyOutsideBottomRight;
    if (s=="outside_righttop" || s=="outsiderighttop" || s=="ort" ) return JKQTPkeyOutsideRightTop;

    if (s=="inside_bottom" || s=="insidebottom" || s=="inside_bottomright" || s=="insidebottomright" || s=="ibr") return JKQTPkeyInsideBottomRight;
    if (s=="inside_top" || s=="insidetop" || s=="inside_left" || s=="insideleft" || s=="inside_topleft" || s=="insidetopleft" || s=="itl") return JKQTPkeyInsideTopLeft;
    if (s=="inside_right" || s=="insideright" || s=="inside_topright" || s=="insidetopright" || s=="itr") return JKQTPkeyInsideTopRight;
    if (s=="inside_bottomleft" || s=="insidebottomleft"  || s=="ibl") return JKQTPkeyInsideBottomLeft;
    return JKQTPkeyInsideTopRight;
}


QString JKQTPkeyLayout2String(JKQTPkeyLayout pos) {
    switch(pos) {
        case JKQTPkeyLayoutOneColumn: return "one_column";
        case JKQTPkeyLayoutOneRow: return "one_row";
        case JKQTPkeyLayoutMultiColumn: return "multi_column";
    }
    return "";
}

JKQTPkeyLayout String2JKQTPkeyLayout(QString pos) {
    QString s=pos.trimmed().toLower();
    if (s=="one_column" || s=="onecolumn" || s=="one") return JKQTPkeyLayoutOneColumn;
    if (s=="one_row" || s=="onerow") return JKQTPkeyLayoutOneRow;
    if (s=="multi_column" || s=="multicolumn" || s=="multi") return JKQTPkeyLayoutMultiColumn;
    return JKQTPkeyLayoutOneColumn;
}


QString JKQTPerrorPlotstyle2String(JKQTPerrorPlotstyle pos) {
    switch(pos) {
        case JKQTPerrorBoxes: return "error_box";
        case JKQTPerrorEllipses: return "error_ell";
        case JKQTPerrorLines: return "error_lines";
        case JKQTPerrorBars: return "error_bars";
        case JKQTPerrorSimpleBars: return "error_simplebars";
        case JKQTPerrorPolygons: return "error_polygons";
        case JKQTPerrorBarsLines: return "error_bars_lines";
        case JKQTPerrorBarsPolygons: return "error_bars_polygons";
        case JKQTPerrorSimpleBarsLines: return "error_simplebars_lines";
        case JKQTPerrorSimpleBarsPolygons: return "error_simplebars_polygons";
        case JKQTPnoError: return "error_none";
    }
    return "";
}

JKQTPerrorPlotstyle String2JKQTPerrorPlotstyle(QString pos) {
    QString s=pos.trimmed().toLower();
    if (s=="error_lines") return JKQTPerrorLines;
    if (s=="error_box") return JKQTPerrorBoxes;
    if (s=="error_ell") return JKQTPerrorEllipses;
    if (s=="error_bars") return JKQTPerrorBars;
    if (s=="error_simplebars") return JKQTPerrorSimpleBars;
    if (s=="error_polygons") return JKQTPerrorPolygons;
    if (s=="error_bars_lines") return JKQTPerrorBarsLines;
    if (s=="error_bars_polygons") return JKQTPerrorBarsPolygons;
    if (s=="error_simplebars_lines") return JKQTPerrorSimpleBarsLines;
    if (s=="error_simplebars_polygons") return JKQTPerrorSimpleBarsPolygons;
    return JKQTPnoError;
}





QString JKQTPgraphSymbols2String(JKQTPgraphSymbols pos) {
    switch(pos) {
        case JKQTPdot: return "symbol_dot";
        case JKQTPcross: return "symbol_cross";
        case JKQTPplus: return "symbol_plus";
        case JKQTPcircle: return "symbol_circle";
        case JKQTPfilledCircle: return "symbol_filled_circle";
        case JKQTPrect: return "symbol_rect";
        case JKQTPfilledRect: return "symbol_filled_rect";
        case JKQTPtriangle: return "symbol_triangle";
        case JKQTPfilledTriangle: return "symbol_filled_triangle";
        case JKQTPdownTriangle: return "symbol_down_triangle";
        case JKQTPfilledDownTriangle: return "symbol_filled_down_triangle";
        case JKQTPtarget: return "symbol_target";
        case JKQTPstar: return "symbol_star";
        case JKQTPfilledStar: return "symbol_filled_star";
        case JKQTPdiamond: return "symbol_diamond";
        case JKQTPfilledDiamond: return "symbol_filled_diamond";
        case JKQTPpentagon: return "symbol_pentagon";
        case JKQTPasterisc: return "symbol_asterisc";
        case JKQTPfilledPentagon: return "symbol_filled_pentagon";
        case JKQTPrectCross: return "symbol_rect_cross";
        case JKQTPrectPlus: return "symbol_rect_plus";
        case JKQTPnoSymbol: return "none";
    }
    return "";
}

QString JKQTPgraphSymbols2NameString(JKQTPgraphSymbols pos) {
    switch(pos) {
        case JKQTPdot: return QObject::tr("dot");
        case JKQTPcross: return QObject::tr("cross");
        case JKQTPplus: return QObject::tr("plus");
        case JKQTPcircle: return QObject::tr("circle");
        case JKQTPfilledCircle: return QObject::tr("filled circle");
        case JKQTPrect: return QObject::tr("rectangle");
        case JKQTPfilledRect: return QObject::tr("filled rectangle");
        case JKQTPtriangle: return QObject::tr("triangle");
        case JKQTPfilledTriangle: return QObject::tr("filled triangle");
        case JKQTPdownTriangle: return QObject::tr("down triangle");
        case JKQTPfilledDownTriangle: return QObject::tr("filled down triangle");
        case JKQTPstar: return QObject::tr("star");
        case JKQTPfilledStar: return QObject::tr("filled star");
        case JKQTPdiamond: return QObject::tr("diamond");
        case JKQTPfilledDiamond: return QObject::tr("filled diamond");
        case JKQTPpentagon: return QObject::tr("pentagon");
        case JKQTPfilledPentagon: return QObject::tr("filled pentagon");
        case JKQTPtarget: return QObject::tr("target");
        case JKQTPasterisc: return QObject::tr("asterisc");
        case JKQTPrectCross: return QObject::tr("square with cross");
        case JKQTPrectPlus: return QObject::tr("square with plus");
        case JKQTPnoSymbol: return QObject::tr("none");
    }
    return "";
}
JKQTPgraphSymbols String2JKQTPgraphSymbols(QString pos)  {
    QString s=pos.trimmed().toLower();
    if (s=="symbol_dot"||s=="dot"||s==".") return JKQTPdot;
    if (s=="symbol_cross"||s=="cross"||s=="x") return JKQTPcross;
    if (s=="symbol_plus"||s=="plus"||s=="+") return JKQTPplus;
    if (s=="symbol_circle"||s=="circle"||s=="o") return JKQTPcircle;
    if (s=="symbol_filled_circle"||s=="filled_circle"||s=="fo") return JKQTPfilledCircle;
    if (s=="symbol_rect"||s=="rect"||s=="r") return JKQTPrect;
    if (s=="symbol_filled_rect"||s=="filled_rect"||s=="fr") return JKQTPfilledRect;
    if (s=="symbol_triangle"||s=="triangle"||s=="^") return JKQTPtriangle;
    if (s=="symbol_filled_triangle"||s=="filled_triangle"||s=="f^") return JKQTPfilledTriangle;
    if (s=="symbol_down_triangle"||s=="down_triangle"||s=="v") return JKQTPdownTriangle;
    if (s=="symbol_filles_down_triangle"||s=="filles_down_triangle"||s=="symbol_filled_down_triangle"||s=="filled_down_triangle"||s=="fv") return JKQTPfilledDownTriangle;
    if (s=="symbol_target"||s=="target"||s=="t") return JKQTPtarget;
    if (s=="symbol_star"||s=="star"||s=="s") return JKQTPstar;
    if (s=="symbol_filled_star"||s=="filled_star"||s=="fs") return JKQTPfilledStar;
    if (s=="symbol_diamond"||s=="diamond"||s=="d") return JKQTPdiamond;
    if (s=="symbol_filled_diamond"||s=="filled_diamond"||s=="fd") return JKQTPfilledDiamond;
    if (s=="symbol_pentagon"||s=="pentagon"||s=="p") return JKQTPpentagon;
    if (s=="symbol_filled_pentagon"||s=="filled_pentagon"||s=="fp") return JKQTPfilledPentagon;
    if (s=="symbol_asterisc"||s=="asterisc"||s=="*") return JKQTPasterisc;
    if (s=="symbol_rect_cross"||s=="rect_cross"||s=="rx") return JKQTPrectCross;
    if (s=="symbol_rect_plus"||s=="rect_plus"||s=="r+") return JKQTPrectPlus;
    return JKQTPnoSymbol;
}

JKQTPSymbolComboBox::JKQTPSymbolComboBox(QWidget *parent):
    QComboBox(parent)
{
    clear();
    setEditable(false);
    for (int i=0; i<=JKQTPmaxSymbolID; i++) {
        addSymbol(static_cast<JKQTPgraphSymbols>(i), JKQTPgraphSymbols2NameString(static_cast<JKQTPgraphSymbols>(i)));
    }
    setCurrentIndex(0);
}

JKQTPgraphSymbols JKQTPSymbolComboBox::getSymbol() const
{
    return String2JKQTPgraphSymbols(itemData(currentIndex()).toString());
}

void JKQTPSymbolComboBox::setSymbol(JKQTPgraphSymbols symbol)
{
    int i=findData(JKQTPgraphSymbols2String(symbol));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPSymbolComboBox::setCurrentSymbol(JKQTPgraphSymbols symbol)
{
    setSymbol(symbol);
}

void JKQTPSymbolComboBox::addSymbol(JKQTPgraphSymbols symbol, const QString &name)
{
    QPixmap pix(12,12);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPplotSymbol(p, 6,6,symbol,10,1,QColor("blue"), QColor("blue").lighter());
    p.end();
    addItem(QIcon(pix), name, JKQTPgraphSymbols2String(symbol));
}






JKQTPLinePlotStyleComboBox::JKQTPLinePlotStyleComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    defaultSymbol=JKQTPcross;
    refill();
    setCurrentIndex(0);
}

void JKQTPLinePlotStyleComboBox::setDefaultSymbol(JKQTPgraphSymbols symbol)
{
    defaultSymbol=symbol;
    refill();
}

void JKQTPLinePlotStyleComboBox::addUsedSymbol(JKQTPgraphSymbols symbol)
{
    if (!symbols.contains(symbol)) symbols.append(symbol);
    refill();
}

JKQTPgraphSymbols JKQTPLinePlotStyleComboBox::getSymbol() const
{
    bool ok=true;
    int idx=itemData(currentIndex()).toInt(&ok);
    if (idx==-2) return JKQTPnoSymbol;
    if (idx>=0&&ok) return symbols.value(idx, defaultSymbol);
    return defaultSymbol;
}

bool JKQTPLinePlotStyleComboBox::getDrawLine() const
{
    return currentIndex()%2==0;
}

void JKQTPLinePlotStyleComboBox::refill()
{
    QString txt=currentText();
    setUpdatesEnabled(false);
    clear();
    addSymbol(JKQTPnoSymbol, true, tr("line"), -2);
    addSymbol(defaultSymbol, false, JKQTPgraphSymbols2NameString(defaultSymbol), -1);
    addSymbol(defaultSymbol, true, JKQTPgraphSymbols2NameString(defaultSymbol)+tr("+line"), -1);
    for (int i=0; i<symbols.size(); i++) {
        if (symbols[i]!=defaultSymbol) {
            addSymbol(symbols[i], false, JKQTPgraphSymbols2NameString(symbols[i]), i);
            addSymbol(symbols[i], true, JKQTPgraphSymbols2NameString(symbols[i])+tr("+line"), i);
        }
    }
    addSymbol(JKQTPnoSymbol, false, "none", -2);
    int idx=findText(txt);
    if (idx>=0) setCurrentIndex(idx);
    else setCurrentIndex(0);
    setUpdatesEnabled(true);
}

void JKQTPLinePlotStyleComboBox::addSymbol(JKQTPgraphSymbols symbol, bool line, const QString &name, const QVariant &data)
{
    QPixmap pix(12,12);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPplotSymbol(p, 6,6,symbol,7,1,QColor("blue"), QColor("blue").lighter());
    p.setPen(QColor("blue"));
    if (line) p.drawLine(0,6,12,6);
    p.end();
    addItem(QIcon(pix), name, data);
}



JKQTPerrorPlotstyleComboBox::JKQTPerrorPlotstyleComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addSymbol(JKQTPnoError, tr("none"), QIcon(":/JKQTPlotter/jkqtp_enone.png"));
    addSymbol(JKQTPerrorLines, tr("error lines"), QIcon(":/JKQTPlotter/jkqtp_elines.png"));
    addSymbol(JKQTPerrorBars, tr("error bars"), QIcon(":/JKQTPlotter/jkqtp_ebars.png"));
    addSymbol(JKQTPerrorSimpleBars, tr("simple error bars"), QIcon(":/JKQTPlotter/jkqtp_esbars.png"));
    addSymbol(JKQTPerrorPolygons, tr("error polygons"), QIcon(":/JKQTPlotter/jkqtp_epoly.png"));
    addSymbol(JKQTPerrorBarsLines, tr("bars + lines"), QIcon(":/JKQTPlotter/jkqtp_elinesbars.png"));
    addSymbol(JKQTPerrorBarsPolygons, tr("bars + polygons"), QIcon(":/JKQTPlotter/jkqtp_epolybars.png"));
    addSymbol(JKQTPerrorSimpleBarsLines, tr("simple bars + lines"), QIcon(":/JKQTPlotter/jkqtp_elinessbars.png"));
    addSymbol(JKQTPerrorSimpleBarsPolygons, tr("simple bars + polygons"), QIcon(":/JKQTPlotter/jkqtp_epolysbars.png"));
    addSymbol(JKQTPerrorBoxes, tr("boxes"), QIcon(":/JKQTPlotter/jkqtp_eboxes.png"));
    addSymbol(JKQTPerrorEllipses, tr("ellipses"), QIcon(":/JKQTPlotter/jkqtp_eellipses.png"));
    setCurrentIndex(2);
}

JKQTPerrorPlotstyle JKQTPerrorPlotstyleComboBox::getErrorStyle() const
{
    return String2JKQTPerrorPlotstyle(itemData(currentIndex()).toString());
}

void JKQTPerrorPlotstyleComboBox::setSymbol(JKQTPerrorPlotstyle symbol)
{
    int i=findData(JKQTPerrorPlotstyle2String(symbol));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPerrorPlotstyleComboBox::setCurrentErrorStyle(JKQTPerrorPlotstyle symbol)
{
    int i=findData(JKQTPerrorPlotstyle2String(symbol));
    if (i>=0) setCurrentIndex(i);

}

void JKQTPerrorPlotstyleComboBox::addSymbol(JKQTPerrorPlotstyle symbol, const QString &name, const QIcon& icon)
{
    addItem(icon, name, JKQTPerrorPlotstyle2String(symbol));
}





JKQTPkeyPositionComboBox::JKQTPkeyPositionComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addPosition(JKQTPkeyInsideTopRight, tr("inside, top-right"), QIcon(":/JKQTPlotter/jkqtp_key_itr.png"));
    addPosition(JKQTPkeyInsideTopLeft, tr("inside, top-left"), QIcon(":/JKQTPlotter/jkqtp_key_itl.png"));
    addPosition(JKQTPkeyInsideBottomLeft, tr("inside, bottom-left"), QIcon(":/JKQTPlotter/jkqtp_key_ibl.png"));
    addPosition(JKQTPkeyInsideBottomRight, tr("inside, bottom-right"), QIcon(":/JKQTPlotter/jkqtp_key_ibr.png"));
    addPosition(JKQTPkeyOutsideTopRight, tr("outside, top-right"), QIcon(":/JKQTPlotter/jkqtp_key_otr.png"));
    addPosition(JKQTPkeyOutsideTopLeft, tr("outside, top-left"), QIcon(":/JKQTPlotter/jkqtp_key_otl.png"));
    addPosition(JKQTPkeyOutsideLeftTop, tr("outside, left-top"), QIcon(":/JKQTPlotter/jkqtp_key_olt.png"));
    addPosition(JKQTPkeyOutsideLeftBottom, tr("outside, left-bottom"), QIcon(":/JKQTPlotter/jkqtp_key_olb.png"));
    addPosition(JKQTPkeyOutsideBottomRight, tr("outside, bottom-right"), QIcon(":/JKQTPlotter/jkqtp_key_obr.png"));
    addPosition(JKQTPkeyOutsideBottomLeft, tr("outside, bottom-left"), QIcon(":/JKQTPlotter/jkqtp_key_obl.png"));
    addPosition(JKQTPkeyOutsideRightTop, tr("outside, right-top"), QIcon(":/JKQTPlotter/jkqtp_key_ort.png"));
    addPosition(JKQTPkeyOutsideRightBottom, tr("outside, right-bottom"), QIcon(":/JKQTPlotter/jkqtp_key_orb.png"));
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(posChanged(int)));
}

JKQTPkeyPosition JKQTPkeyPositionComboBox::getPosition() const
{
    return String2JKQTPkeyPosition(itemData(currentIndex()).toString());
}

void JKQTPkeyPositionComboBox::setPosition(JKQTPkeyPosition position)
{
    int i=findData(JKQTPkeyPosition2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPkeyPositionComboBox::addPosition(JKQTPkeyPosition position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPkeyPosition2String(position));
}

void JKQTPkeyPositionComboBox::posChanged(int index)
{
    emit currentPositionChanged(String2JKQTPkeyPosition(itemData(index).toString()));
}

JKQTPkeyLayoutComboBox::JKQTPkeyLayoutComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addKeyLayout(JKQTPkeyLayoutOneColumn, tr("one column"));
    addKeyLayout(JKQTPkeyLayoutOneRow, tr("one row"));
    addKeyLayout(JKQTPkeyLayoutMultiColumn, tr("multiple columns"));
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedP(int)));
}

JKQTPkeyLayout JKQTPkeyLayoutComboBox::getKeyLayout() const
{
    return String2JKQTPkeyLayout(itemData(currentIndex()).toString());
}

void JKQTPkeyLayoutComboBox::setKeyLayout(JKQTPkeyLayout position)
{
    int i=findData(JKQTPkeyLayout2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPkeyLayoutComboBox::addKeyLayout(JKQTPkeyLayout position, const QString &name)
{
    addItem(name, JKQTPkeyLayout2String(position));
}

void JKQTPkeyLayoutComboBox::currentIndexChangedP(int index)
{
    emit currentLayoutChanged(String2JKQTPkeyLayout(itemData(index).toString()));
}

JKQTPlabelPositionComboBox::JKQTPlabelPositionComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addPosition(JKQTPlabelCenter, tr("center"));
    addPosition(JKQTPlabelMin, tr("left"));
    addPosition(JKQTPlabelMax, tr("right"));
    setCurrentIndex(0);
}

JKQTPlabelPosition JKQTPlabelPositionComboBox::getPosition() const
{
    return String2JKQTPlabelPosition(itemData(currentIndex()).toString());
}

void JKQTPlabelPositionComboBox::setPosition(JKQTPlabelPosition position)
{
    int i=findData(JKQTPlabelPosition2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPlabelPositionComboBox::addPosition(JKQTPlabelPosition position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPlabelPosition2String(position));
}

JKQTPCAlabelTypeComboBox::JKQTPCAlabelTypeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addLabelType(JKQTPCALTexponent, tr("exponent"), QIcon(":/JKQTPlotter/jkqtp_ticks_exp.png"));
    addLabelType(JKQTPCALTdefault, tr("default"), QIcon(":/JKQTPlotter/jkqtp_ticks_default.png"));
    addLabelType(JKQTPCALTexponentCharacter, tr("character"), QIcon(":/JKQTPlotter/jkqtp_ticks_expchar.png"));
    addLabelType(JKQTPCALTtime, tr("time"), QIcon(":/JKQTPlotter/jkqtp_ticks_time.png"));
    addLabelType(JKQTPCALTdate, tr("date"), QIcon(":/JKQTPlotter/jkqtp_ticks_date.png"));
    addLabelType(JKQTPCALTdatetime, tr("datetime"), QIcon(":/JKQTPlotter/jkqtp_ticks_datetime.png"));
    setCurrentIndex(0);
}

JKQTPCAlabelType JKQTPCAlabelTypeComboBox::getLabelType() const
{
    return String2JKQTPCAlabelType(itemData(currentIndex()).toString());
}

void JKQTPCAlabelTypeComboBox::setLabelType(JKQTPCAlabelType position)
{
    int i=findData(JKQTPCAlabelType2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPCAlabelTypeComboBox::addLabelType(JKQTPCAlabelType position, const QString &name, const QIcon& icon)
{
    addItem(icon, name, JKQTPCAlabelType2String(position));
}

JKQTPCAdrawModeComboBox::JKQTPCAdrawModeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addDrawMode(JKQTPCADMcomplete, tr("complete"), QIcon(":/JKQTPlotter/jkqtp_axis_complete.png"));
    addDrawMode(JKQTPCADMticksAndLabels, tr("tick+lab."), QIcon(":/JKQTPlotter/jkqtp_axis_ticksandlabels.png"));
    addDrawMode(JKQTPCADMticks, tr("tick"), QIcon(":/JKQTPlotter/jkqtp_axis_ticks.png"));
    addDrawMode(JKQTPCADMline, tr("line"), QIcon(":/JKQTPlotter/jkqtp_axis_line.png"));
    addDrawMode(JKQTPCADMnone, tr("none"), QIcon(":/JKQTPlotter/jkqtp_axis_none.png"));
    setCurrentIndex(0);
}

JKQTPCAdrawMode JKQTPCAdrawModeComboBox::getDrawMode() const
{
    return String2JKQTPCAdrawMode(itemData(currentIndex()).toString());
}

void JKQTPCAdrawModeComboBox::setDrawMode(JKQTPCAdrawMode position)
{
    int i=findData(JKQTPCAdrawMode2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPCAdrawModeComboBox::addDrawMode(JKQTPCAdrawMode position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPCAdrawMode2String(position));
}


JKQTPEnhancedTableView::JKQTPEnhancedTableView(QWidget *parent):
    QTableView(parent)
{
    setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* act;

    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16.png"), tr("Copy Selection to Clipboard (for Excel ...)"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToExcel()));
    addAction(act);
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16_nohead.png"), tr("Copy Selection to clipboard (for Excel ...) without header row/column"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToExcelNoHead()));
    addAction(act);
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16.png"), tr("Copy Selection to Clipboard (as CSV)"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToCSV()));
    addAction(act);
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16_nohead.png"), tr("Copy Selection to clipboard (as CSV ...) without header row/column"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToCSVNoHead()));
    addAction(act);
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_printtable.png"), tr("Print Table"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(print()));
    addAction(act);
	printAction=act;
}

JKQTPEnhancedTableView::~JKQTPEnhancedTableView()
{
}


QString JKQTPEnhancedTableView::toHtml(int borderWidth, bool /*non_breaking*/, int fontSizePt) const
{
    if (!model()) return "";
    QString fsstyle="";
    if (fontSizePt>0) {
        fsstyle=QString(" font-size: %1pt;").arg(fontSizePt);
    }
    QString html=QString("<table border=\"%1\" cellspacing=\"0\" cellpadding=\"0\">").arg(borderWidth);
    for (int row=-1; row<model()->rowCount(); row++) {
        html+="<tr>";
        if (row==-1) {
            for (int col=-1; col<model()->columnCount(); col++) {
                html+=QString("<th style=\"white-space: nowrap;%1\"><nobr>").arg(fsstyle);
                if (col>=0) {
                    html+=model()->headerData(col, Qt::Horizontal).toString();
                }
                html+="</nobr></th>";
            }
        } else {
            for (int col=-1; col<model()->columnCount(); col++) {
                if (col==-1) {
                    html+=QString("<th style=\"white-space: nowrap;%1\"><nobr>").arg(fsstyle);
                    html+=model()->headerData(row, Qt::Vertical).toString();
                    html+="</nobr></th>";
                } else {
                    QModelIndex index=model()->index(row, col);
                    QVariant check=index.data(Qt::CheckStateRole);
                    QBrush back=index.data(Qt::BackgroundRole).value<QBrush>();
                    QString style=fsstyle+"white-space: nowrap; ";
                    //qDebug()<<"r="<<row<<"\tc="<<col<<"\tcolor="<<back.color().name();
                    if (back.color()!=QColor(0,0,0) && index.data(Qt::BackgroundRole).isValid()) style=QString("background: %1;").arg(back.color().name());
                    if (style.isEmpty()) html+=QString("<td style=\"%1\"><nobr>").arg(fsstyle);
                    else html+=QString("<td style=\"%1\"><nobr>").arg(style);
                    if (check.isValid()) {
                        if (check.toInt()!=0) {
                            html+= QString("|&times;|&nbsp;&nbsp;");
                        } else {
                            html+= QString("|&nbsp;|&nbsp;&nbsp;");
                        }
                    }
                    html+=index.data().toString();
                    html+="</nobr></td>";
                }
            }
        }
        html+="</tr>";

    }
    html+="</table>";
    return html;
}

void JKQTPEnhancedTableView::copySelectionToExcel(int copyrole, bool storeHead)
{
    if (!model()) return;
    if (!selectionModel()) return;
    QModelIndexList sel=selectionModel()->selectedIndexes();
    QLocale loc=QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    if (sel.size()==1) {
        QVariant vdata=sel[0].data(copyrole);
        QString txt="";
        switch (vdata.type()) {
            case QVariant::Int:
            case QVariant::LongLong:
            case QVariant::UInt:
            case QVariant::ULongLong:
            case QVariant::Bool:
                txt=vdata.toString();
                break;
            case QVariant::Double:
                txt=loc.toString(vdata.toDouble());
                break;
            case QVariant::PointF:
                txt=loc.toString(vdata.toPointF().x());
                break;
            default:
                txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                break;
        }
         QApplication::clipboard()->setText(txt);
    } else {
        QSet<int> rows, cols;
        int colmin=0;
        int rowmin=0;
        for (int i=0; i<sel.size(); i++) {
            int r=sel[i].row();
            int c=sel[i].column();
            rows.insert(r);
            cols.insert(c);
            if (i==0) {
                colmin=c;
                rowmin=r;
            } else {
                if (c<colmin) colmin=c;
                if (r<rowmin) rowmin=r;
            }
        }
        QList<int> rowlist=QList<int>::fromSet(rows);
        qSort(rowlist.begin(), rowlist.end());
        QList<int> collist=QList<int>::fromSet(cols);
        qSort(collist.begin(), collist.end());
        int rowcnt=rowlist.size();
        int colcnt=collist.size();
        QList<QStringList> data;

        // header row:
        //
        //  <EMPTY> | <HOR_HEDER1> | <HOR_HEADER2> | ...
        QStringList hrow;
        if (storeHead) {
            hrow.append(""); // empty header for first column (vertical headers!)
            for (int c=0; c<colcnt; c++) {
                hrow.append(QString("\"%1\"").arg(model()->headerData(collist[c], Qt::Horizontal).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " ")));
            }
            data.append(hrow);
        }

        // now add dta rows:
        //
        //               <~~~~~~~~~ colcnt times ~~~~~~~~~~>
        //  <VER_HEADER> | <EMPTY> | <EMPTY> | ... | <EMPTY>
        for (int r=0; r<rowcnt; r++) {
            QStringList row;
            if (storeHead) row.append(QString("\"%1\"").arg(model()->headerData(rowlist[r], Qt::Vertical).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "))); // vertical header
            for (int c=0; c<colcnt; c++) {
                row.append(""); // empty columns for data
            }
            data.append(row);
        }
        for (int i=0; i<sel.size(); i++) {
            int r=rowlist.indexOf(sel[i].row());
            int c=collist.indexOf(sel[i].column());
            QVariant vdata=sel[i].data(copyrole);
            QString txt="";
            switch (vdata.type()) {
                case QVariant::Int:
                case QVariant::LongLong:
                case QVariant::UInt:
                case QVariant::ULongLong:
                case QVariant::Bool:
                    txt=vdata.toString();
                    break;
                case QVariant::Double:
                    txt=loc.toString(vdata.toDouble());
                    break;
                case QVariant::PointF:
                    txt=loc.toString(vdata.toPointF().x());
                    break;
                default:
                    txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                    break;
            }
            int shift=0;
            if (storeHead) shift=1;
            if ((r>=0) && (c>=0) && (r<=data.size()) && (c<=colcnt))data[r+shift][c+shift]=txt;
        }

        QString result="";
        for (int r=0; r<data.size(); r++) {
            result+=data[r].join("\t")+"\n";
        }
        QApplication::clipboard()->setText(result);
    }
}

void JKQTPEnhancedTableView::copySelectionToExcelNoHead(int copyrole)
{
    copySelectionToExcel(copyrole, false);
}

void JKQTPEnhancedTableView::copySelectionToCSV(int copyrole, bool storeHead, const QString &separator, const QChar &decimalpoint)
{
    if (!model()) return;
    if (!selectionModel()) return;
    QModelIndexList sel=selectionModel()->selectedIndexes();
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    if (sel.size()==1) {
        QVariant vdata=sel[0].data(copyrole);
        QString txt="";
        switch (vdata.type()) {
            case QVariant::Int:
            case QVariant::LongLong:
            case QVariant::UInt:
            case QVariant::ULongLong:
            case QVariant::Bool:
                txt=vdata.toString();
                break;
            case QVariant::Double:
                txt=JKQTPDoubleToQString(vdata.toDouble(), 15, 'g', decimalpoint);
                break;
            case QVariant::PointF:
                txt=JKQTPDoubleToQString(vdata.toPointF().x(), 15, 'g', decimalpoint);
                break;
            default:
                txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                break;
        }
         QApplication::clipboard()->setText(txt);
    } else {
        QSet<int> rows, cols;
        int colmin=0;
        int rowmin=0;
        for (int i=0; i<sel.size(); i++) {
            int r=sel[i].row();
            int c=sel[i].column();
            rows.insert(r);
            cols.insert(c);
            if (i==0) {
                colmin=c;
                rowmin=r;
            } else {
                if (c<colmin) colmin=c;
                if (r<rowmin) rowmin=r;
            }
        }
        QList<int> rowlist=QList<int>::fromSet(rows);
        qSort(rowlist.begin(), rowlist.end());
        QList<int> collist=QList<int>::fromSet(cols);
        qSort(collist.begin(), collist.end());
        int rowcnt=rowlist.size();
        int colcnt=collist.size();
        QList<QStringList> data;

        // header row:
        //
        //  <EMPTY> | <HOR_HEDER1> | <HOR_HEADER2> | ...
        QStringList hrow;
        if (storeHead) {
            hrow.append(""); // empty header for first column (vertical headers!)
            for (int c=0; c<colcnt; c++) {
                hrow.append(QString("\"%1\"").arg(model()->headerData(collist[c], Qt::Horizontal).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " ")));
            }
            data.append(hrow);
        }

        // now add dta rows:
        //
        //               <~~~~~~~~~ colcnt times ~~~~~~~~~~>
        //  <VER_HEADER> | <EMPTY> | <EMPTY> | ... | <EMPTY>
        for (int r=0; r<rowcnt; r++) {
            QStringList row;
            if (storeHead) row.append(QString("\"%1\"").arg(model()->headerData(rowlist[r], Qt::Vertical).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "))); // vertical header
            for (int c=0; c<colcnt; c++) {
                row.append(""); // empty columns for data
            }
            data.append(row);
        }
        for (int i=0; i<sel.size(); i++) {
            int r=rowlist.indexOf(sel[i].row());
            int c=collist.indexOf(sel[i].column());
            QVariant vdata=sel[i].data(copyrole);
            QString txt="";
            switch (vdata.type()) {
                case QVariant::Int:
                case QVariant::LongLong:
                case QVariant::UInt:
                case QVariant::ULongLong:
                case QVariant::Bool:
                    txt=vdata.toString();
                    break;
                case QVariant::Double:
                    txt=JKQTPDoubleToQString(vdata.toDouble(), 15, 'g', decimalpoint);
                    break;
                case QVariant::PointF:
                    txt=JKQTPDoubleToQString(vdata.toPointF().x(), 15, 'g', decimalpoint);
                    break;
                default:
                    txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                    break;
            }
            int shift=0;
            if (storeHead) shift=1;
            if ((r>=0) && (c>=0) && (r<=data.size()) && (c<=colcnt))data[r+shift][c+shift]=txt;
        }

        QString result="";
        for (int r=0; r<data.size(); r++) {
            result+=data[r].join(separator)+"\n";
        }
        QApplication::clipboard()->setText(result);
    }
}

void JKQTPEnhancedTableView::copySelectionToCSVNoHead(int copyrole, const QString &separator, const QChar &decimalpoint)
{
     copySelectionToCSV(copyrole, false, separator, decimalpoint);
}

void JKQTPEnhancedTableView::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy)) {
        copySelectionToExcel(Qt::EditRole, false);
        event->accept();
    } else if (event->matches(QKeySequence::Print)) {
        print();
        event->accept();
    } else QTableView::keyPressEvent(event);
    emit keyPressed(event->key(), event->modifiers(), event->text());
}


void JKQTPEnhancedTableView::print()
{
    QPrinter* tablePrinter=getPrinter(nullptr);

    if (tablePrinter) {
        QDialog* dlg=new QDialog(this);
        dlg->setWindowTitle(tr("Table print options ..."));
        QGridLayout* lay=new QGridLayout();
        dlg->setLayout(lay);
        lay->addWidget(new QLabel(tr("<b>scaling:</b>")), 0,0);
        QCheckBox* chk1Wide=new QCheckBox(tr("one page wide"), dlg);
        QCheckBox* chk1High=new QCheckBox(tr("one page high"), dlg);
        lay->addWidget(chk1Wide, 0, 1);
        lay->addWidget(chk1High, 1, 1);

        QDialogButtonBox* btns=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,dlg);
        connect(btns, SIGNAL(accepted()), dlg, SLOT(accept()));
        connect(btns, SIGNAL(rejected()), dlg, SLOT(reject()));
        lay->addWidget(btns, 2, 0,1,2);

        if (dlg->exec()) {
            print(tablePrinter, chk1Wide->isChecked(), chk1High->isChecked());
        }
        delete dlg;
        delete tablePrinter;
    }
}



void JKQTPEnhancedTableView::print(QPrinter *printer, bool onePageWide, bool onePageHigh)
{
    QPrinter* p=printer;


     //p->setPageMargins(10,10,10,10,QPrinter::Millimeter);

     /*if (width()>height()) {
         p->setOrientation(QPrinter::Landscape);
     } else {
         p->setOrientation(QPrinter::Portrait);
     }*/

     clearSelection();


     /// PRINT HERE //////////////////////////////////////////////////////////////////////////////////
     // calculate the total width/height table would need without scaling
     const int rows = model()->rowCount();
     const int cols = model()->columnCount();
     double vhw=verticalHeader()->width()+8;
     double totalWidth = vhw;
     double minWidth=1e33;
     double maxWidth=0;
     for (int c = -1; c < cols; ++c)
     {
         double w=columnWidth(c);
         totalWidth += w;
         if (w<minWidth) minWidth=w;
         if (w>maxWidth) maxWidth=w;
     }
     double hhh=horizontalHeader()->height()+8;
     double totalHeight = hhh;
     double minHeight=1e33;
     double maxHeight=0;
     for (int r = 0; r < rows; ++r)
     {
        double h=rowHeight(r);
        totalHeight += h;
        if (h<minHeight) minHeight=h;
        if (h>maxHeight) maxHeight=h;
     }
     double scale=1.0;
     // adjust scale, so the widest/highest column fits on one page
     /*if (maxWidth*scale>p->pageRect().width()) scale=p->pageRect().width()/maxWidth;
     if (maxHeight*scale>p->pageRect().height()) scale=p->pageRect().height()/maxHeight;*/
     if (onePageWide) {
         if (totalWidth>p->pageRect().width()) scale=p->pageRect().width()/totalWidth;
     }
     if (onePageHigh) {
         if (totalHeight>p->pageRect().height()) scale=qMin(scale, p->pageRect().height()/totalHeight);
     }

     //qDebug()<<scale;

     // print scaled pixmap

     int pagesWide=1;
     int pagesHigh=1;
     QList<int> pageCols, pageRows;
     pageCols<<0;
     pageRows<<0;
     { // find number of pages needed
         double x=vhw, x0=vhw;
         if (!onePageWide) {
             for (int c=0; c<cols; c++) {
                 double cw=columnWidth(c);
                 if (x+cw>p->pageRect().width()/scale) {
                     pagesWide++;
                     x=0;
                     pageCols<<c;
                 } else {
                     x=x+cw;
                     x0=x0+cw;
                 }
             }
         }
         if (pageCols.size()>0 && cols>pageCols.last()) pageCols<<cols;
         if (pageCols.size()==1) pageCols<<cols;
         double y=hhh, y0=hhh;
         if (!onePageHigh) {
             for (int r=0; r<rows; r++) {
                 double rh=rowHeight(r);
                 if (y+rh>p->pageRect().height()/scale) {
                     pagesHigh++;
                     pageRows<<r;
                     y=hhh;
                 } else {
                     y=y+rh;
                     y0=y0+rh;
                 }
             }
         }
         if (pageRows.size()>0 && rows>pageRows.last()) pageRows<<rows;
         if (pageRows.size()==1) pageRows<<rows;
     }

     JKQTPEnhancedPainter painter(p);
     paint(painter, scale, -1, hhh, vhw, pageCols, pageRows, p);
     painter.end();




     /// PRINT DONE //////////////////////////////////////////////////////////////////////////////////
}

void JKQTPEnhancedTableView::paint(QPainter &painter, QRect pageRect)
{
    QRect pageRec=pageRect;
    if (pageRec.width()==0 || pageRec.height()==0) pageRec=QRect(QPoint(0,0), getTotalSize().toSize());
    painter.save();
    painter.translate(pageRec.topLeft());
    QSizeF size=getTotalSize();
    double vhw=verticalHeader()->width()+8;
    double hhh=horizontalHeader()->height()+8;
    double scaleX=size.width()/double(pageRec.width());
    double scaleY=size.height()/double(pageRec.height());
    double scale=qMin(scaleX, scaleY);
    QList<int>pageCols, pageRows;
    pageCols<<0<<model()->columnCount();
    pageRows<<0<<model()->rowCount();
    paint(painter, scale, -1, hhh, vhw, pageCols, pageRows);
    painter.restore();
}

QSizeF JKQTPEnhancedTableView::getTotalSize() const
{
    const int rows = model()->rowCount();
    const int cols = model()->columnCount();
    double vhw=verticalHeader()->width()+8;
    double totalWidth = vhw;
    for (int c = -1; c < cols; ++c)
    {
        double w=columnWidth(c);
        totalWidth += w;
    }
    double hhh=horizontalHeader()->height()+8;
    double totalHeight = hhh;
    for (int r = 0; r < rows; ++r)
    {
       double h=rowHeight(r);
       totalHeight += h;
    }

    return QSizeF((totalWidth), (totalHeight));
}

void JKQTPEnhancedTableView::paint(QPainter &painter, double scale, int page, double hhh, double vhw, const QList<int>& pageCols, const QList<int>& pageRows, QPrinter* p)
{
    painter.save();
    QStyleOptionViewItem option = viewOptions();
    painter.scale(scale, scale);
    QPen headerPen("black");
    headerPen.setWidth(2);
    QPen cellPen("gray");
    cellPen.setWidth(1);
    QFont headerFont=horizontalHeader()->font();
    headerFont.setBold(true);
    int pagesWide=pageCols.size()-1;
    int pagesHigh=pageRows.size()-1;
    //painter.translate(p->pageRect().topLeft());
    int pageCnt=0;
    for (int ph=0; ph<pageRows.size()-1; ph++) {
        for (int pw=0; pw<pageCols.size()-1; pw++) {
            if (page<0 || page==pageCnt) {
                //qDebug()<<"print page "<<ph<<"/"<<pageRows.size()<<pagesHigh<<"    "<<pw<<"/"<<pageCols.size()<<pagesWide;
                //painter.drawPicture(p->pageRect().topLeft(), pic);
                double y=0;
                if (ph==0) {
                    y=hhh;
                }
                //qDebug()<<"   rows = "<<pageRows[ph]<<"..."<<pageRows[ph+1]-1;
                //qDebug()<<"   cols = "<<pageCols[pw]<<"..."<<pageCols[pw+1]-1;
                for (int r=pageRows[ph]; r<pageRows[ph+1]; r++) {
                    double x=0;
                    int rh=rowHeight(r);
                    if (pw%pagesWide==0) {
                        x=0;
                        option.rect = QRect((int)x, (int)y, vhw, rh);
                        //verticalHeader()->itemDelegate()->paint(&painter, option, model()->index(r, c, QModelIndex()));
                        x=vhw;
                    }
                    for (int c=pageCols[pw]; c<pageCols[pw+1]; c++) {
                        double cw=columnWidth(c);
                        option.rect = QRect((int)x, (int)y, cw, rh);
                        itemDelegate()->paint(&painter, option, model()->index(r, c, QModelIndex()));
                        painter.setPen(cellPen);
                        painter.drawRect(option.rect);
                        x=x+cw;

                    }
                    y=y+rh;
                }

                if (ph==0) {
                    y=0;
                    int x=0;
                    if (pw%pagesWide==0) x=vhw;
                    painter.setPen(headerPen);
                    for (int c=pageCols[pw]; c<pageCols[pw+1]; c++) {
                        QRect rec=QRect(x, (int)y, columnWidth(c), hhh);
                        painter.fillRect(rec, QColor("lightgrey"));
                        painter.setFont(headerFont);
                        painter.setPen(headerPen);
                        painter.drawText(QRect(rec.x()+4, rec.y()+4, rec.width()-8, rec.height()-8), model()->headerData(c, Qt::Horizontal).toString());
                        painter.drawRect(rec);
                        //if (x==vhw &&) painter.drawLine(rec.topLeft(), QPoint(rec.left(), p->pageRect().height()));
                        x=x+columnWidth(c);
                    }

                }
                if (pw%pagesWide==0) {
                    y=0;
                    int x=0;
                    if (ph==0) y=hhh;
                    for (int r=pageRows[ph]; r<pageRows[ph+1]; r++) {
                        QRect rec=QRect(x, (int)y, vhw, rowHeight(r));
                        painter.fillRect(rec, QColor("lightgrey"));
                        painter.setPen(headerPen);
                        painter.setFont(headerFont);
                        painter.drawText(QRect(rec.x()+4, rec.y()+4, rec.width()-8, rec.height()-8), model()->headerData(r, Qt::Vertical).toString());
                        painter.drawRect(rec);
                        //if (x==vhw &&) painter.drawLine(rec.topLeft(), QPoint(rec.left(), p->pageRect().height()));
                        y=y+rowHeight(r);
                    }

                }


                if (p && pw<pagesWide-1) p->newPage();
            }
            pageCnt++;
        }
        if (p && ph<pagesHigh-1) p->newPage();
    }
    painter.restore();
}



QPrinter *JKQTPEnhancedTableView::getPrinter(QPrinter *printerIn, bool *localPrinter)
{
    QPrinter* p=printerIn;
    if (p==nullptr) {
        p=new QPrinter();
        if (localPrinter) *localPrinter=true;
    }

    QPrintDialog *dialog = new QPrintDialog(p, nullptr);
    dialog->setWindowTitle(tr("Print Table"));
    if (dialog->exec() != QDialog::Accepted) {
        if (localPrinter && *localPrinter) delete p;
        delete dialog;
        return printerIn;
    }
    p=dialog->printer();
    delete dialog;

    return p;
}


JKQTPLinePlotStyleWithSymbolSizeComboBox::JKQTPLinePlotStyleWithSymbolSizeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    defaultSymbol=JKQTPcross;
    defaultSize=10;
    refill();
    setCurrentIndex(0);
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::setDefaultSymbol(JKQTPgraphSymbols symbol, double size)
{
    defaultSymbol=symbol;
    defaultSize=size;
    refill();
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::addUsedSymbol(JKQTPgraphSymbols symbol, double symbolSize, bool line)
{
    styleData d;
    d.symbol=symbol;
    d.symbolSize=symbolSize;
    d.line=line;
    if (!symbols.contains(d)) symbols.append(d);
    refill();
}

JKQTPgraphSymbols JKQTPLinePlotStyleWithSymbolSizeComboBox::getSymbol() const
{
    QVariant idxV=itemData(currentIndex());
    bool ok=false;
    int idx=idxV.toInt(&ok);
    if (idxV.isValid() && ok) {
        if (idx>=0 && idx<symbols.size()) return symbols[idx].symbol;
        if (idx==-4) return JKQTPnoSymbol;
        if (idx==-3) return JKQTPnoSymbol;
        return defaultSymbol;
    }
    return JKQTPnoSymbol;
}

bool JKQTPLinePlotStyleWithSymbolSizeComboBox::getDrawLine() const
{
    QVariant idxV=itemData(currentIndex());
    bool ok=false;
    int idx=idxV.toInt(&ok);
    if (idxV.isValid() && ok) {
        if (idx==-3) return true;
        if (idx==-1) return true;
        if (idx>=0 && idx<symbols.size()) return symbols[idx].line;
        return false;
    }
    return false;
}

double JKQTPLinePlotStyleWithSymbolSizeComboBox::getSymbolSize() const
{
    QVariant idxV=itemData(currentIndex());
    bool ok=false;
    int idx=idxV.toInt(&ok);
    if (idxV.isValid() && ok) {
        if (idx>=0 && idx<symbols.size()) return symbols[idx].symbolSize;
        return defaultSize;
    }
    return defaultSize;
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::refill()
{
    QString txt=currentText();
    setUpdatesEnabled(false);
    clear();
    addSymbol(JKQTPnoSymbol, true, defaultSize, tr("line"), -3);
    addSymbol(defaultSymbol, false, defaultSize, JKQTPgraphSymbols2NameString(defaultSymbol), -2);
    addSymbol(defaultSymbol, true, defaultSize, JKQTPgraphSymbols2NameString(defaultSymbol)+tr("+line"), -1);
    for (int i=0; i<symbols.size(); i++) {
        if (symbols[i].symbol!=defaultSymbol||symbols[i].symbolSize!=defaultSize) {
            addSymbol(symbols[i].symbol, symbols[i].line, symbols[i].symbolSize, JKQTPgraphSymbols2NameString(symbols[i].symbol), i);
        }
    }
    addSymbol(JKQTPnoSymbol, false, defaultSize, "none", -4);
    int idx=findText(txt);
    if (idx>=0) setCurrentIndex(idx);
    else setCurrentIndex(0);
    setUpdatesEnabled(true);
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::addSymbol(JKQTPgraphSymbols symbol, bool line, double symbolSize, const QString &name, const QVariant &data)
{
    int pixSize=qMax(16.0, 1.2*symbolSize);
    QPixmap pix(pixSize, pixSize);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPplotSymbol(p, double(pixSize)/2.0,double(pixSize)/2.0,symbol,symbolSize,1,QColor("blue"), QColor("blue").lighter());
    p.setPen(QColor("blue"));
    if (line) p.drawLine(QLineF(0,double(pixSize)/2.0,pixSize,double(pixSize)/2.0));
    p.end();
    addItem(QIcon(pix), name, data);
}


bool JKQTPLinePlotStyleWithSymbolSizeComboBox::styleData::operator==(const JKQTPLinePlotStyleWithSymbolSizeComboBox::styleData &other) const
{
    return (symbol==other.symbol && symbolSize==other.symbolSize && line==other.line);
}





JKQTPEnhancedDoubleSpinBox::JKQTPEnhancedDoubleSpinBox(QWidget *parent):
    QDoubleSpinBox(parent)
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

JKQTPEnhancedDoubleSpinBox::~JKQTPEnhancedDoubleSpinBox()
{
    disconnect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

void JKQTPEnhancedDoubleSpinBox::intEditingFinished()
{
    emit editingFinished(value());
}


JKQTPEnhancedSpinBox::JKQTPEnhancedSpinBox(QWidget *parent):
    QSpinBox(parent)
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

JKQTPEnhancedSpinBox::~JKQTPEnhancedSpinBox()
{
    disconnect(this, SIGNAL(editingFinished()), this, SLOT(intEditingFinished()));
}

void JKQTPEnhancedSpinBox::intEditingFinished()
{
    emit editingFinished(value());
}


JKQTPEnhancedPainter::JKQTPEnhancedPainter(QPaintDevice *device):
    QPainter(device)
{
    initQEnhacedPainter();
}

JKQTPEnhancedPainter::JKQTPEnhancedPainter():
    QPainter()
{
    initQEnhacedPainter();
}

JKQTPEnhancedPainter::~JKQTPEnhancedPainter()
{

}

//void JKQTPEnhancedPainter::drawLines(const QLineF *lines, int lineCount)
//{

//}

//void JKQTPEnhancedPainter::drawLines(const QPointF *lines, int lineCount)
//{

//}

//void JKQTPEnhancedPainter::drawLines(const QLine *lines, int lineCount)
//{

//}

//void JKQTPEnhancedPainter::drawLines(const QPoint *lines, int lineCount)
//{

//}


void JKQTPEnhancedPainter::initQEnhacedPainter()
{

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
        return jkqtp_format("#%02X%02X%02X", (int)r, (int)g, (int)b);
    }
    // if we reach this, we have an unnamed transparent color
    return jkqtp_format("#%02X%02X%02X%02X", (int)r, (int)g, (int)b, (int)a);
}

std::string jkqtp_to_valid_variable_name(std::string input) {
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

JKQTPstepTypeComboBox::JKQTPstepTypeComboBox(QWidget *parent):
    QComboBox(parent)
{
    clear();
    setEditable(false);
    addStep(JKQTPstepType::JKQTPstepLeft, JKQTPstepType2String(JKQTPstepType::JKQTPstepLeft));
    addStep(JKQTPstepType::JKQTPstepCenter, JKQTPstepType2String(JKQTPstepType::JKQTPstepCenter));
    addStep(JKQTPstepType::JKQTPstepRight, JKQTPstepType2String(JKQTPstepType::JKQTPstepRight));
    setCurrentIndex(0);
}

JKQTPstepType JKQTPstepTypeComboBox::getStepType() const
{
    return String2JKQTPstepType(itemData(currentIndex()).toString());
}

void JKQTPstepTypeComboBox::setStepType(JKQTPstepType step)
{
    int i=findData(JKQTPstepType2String(step));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPstepTypeComboBox::setCurrentStepType(JKQTPstepType step)
{
    setStepType(step);
}

void JKQTPstepTypeComboBox::addStep(JKQTPstepType step, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPstepType2String(step));
}

QString JKQTPstepType2String(JKQTPstepType pos)
{
    switch(pos) {
        case JKQTPstepLeft: return "step_left";
        case JKQTPstepCenter: return "step_center";
        case JKQTPstepRight: return "step_right";
    }
    return "";
}

JKQTPstepType String2JKQTPstepType(QString pos)
{
    QString s=pos.trimmed().toLower();
    if (s=="step_left"||s=="left"||s=="l") return JKQTPstepLeft;
    if (s=="step_center"||s=="center"||s=="c") return JKQTPstepCenter;
    if (s=="step_right"||s=="right"||s=="r") return JKQTPstepRight;

    return JKQTPstepLeft;
}

