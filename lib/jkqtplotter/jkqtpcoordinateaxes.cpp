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

#include "jkqtplotter/jkqtpcoordinateaxes.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtcommon/jkqtpgeometrytools.h"
#include "jkqtcommon/jkqtpmathtools.h"
#include <QDebug>
#include <QDateTime>
#include <cfloat>
#include <QApplication>
#if __cplusplus >= 202002L
# include <version>
# ifdef __cpp_lib_format
#  include <format>
# endif
#endif


//#undef SHOW_JKQTPLOTTER_DEBUG
//#define SHOW_JKQTPLOTTER_DEBUG

JKQTPCoordinateAxis::JKQTPCoordinateAxis(JKQTBasePlotter* _parent):
    QObject(_parent),
    paramsChanged(true),
    doUpdateScaling(true),
    tickLabels(),
    parent(_parent),
    axismin(-10),
    axismax(10),
    axisabsoultemin(-DBL_MAX/100.),
    axisabsoultemax(DBL_MAX/100.0),
    axisStyle(),
    axisMinWidth(-1),
    width(20),
    scale(0),
    offset(0),
    inverted(false),
    tickStart(1),
    autoAxisSpacing(true),
    logAxis(false),
    logAxisBase(10),
    userTickSpacing(1),
    userLogTickSpacing(10),
    tickSpacing(0),
    tickSpacingLog(10),
    axisLabel(),
    tickUnitFactor(1),
    tickUnitName(""),
    axisPrefix(),
    scaleSign(1)
{

}

JKQTPCoordinateAxis::~JKQTPCoordinateAxis() {

}

void JKQTPCoordinateAxis::setParent(JKQTBasePlotter* parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPCoordinateAxis::redrawPlot() {
    //if (paramsChanged)  {
        calcPlotScaling();
        parent->redrawPlot();
    //}
}

JKQTMathText* JKQTPCoordinateAxis::getParentMathText() {
    return parent->getMathText();
}

const JKQTMathText* JKQTPCoordinateAxis::getParentMathText() const {
    return parent->getMathText();
}


void JKQTPCoordinateAxis::clearAxisTickLabels() {
    tickLabels.clear();
    redrawPlot();
}

void JKQTPCoordinateAxis::addAxisTickLabel(double x, const QString& label) {
    tickLabels.append(qMakePair(x, label));
    redrawPlot();
}

void JKQTPCoordinateAxis::addAxisTickLabels(const QVector<double> &x, const QStringList &label) {
    for (int i=0; i<qMin(x.size(), label.size()); i++) {
        tickLabels.append(qMakePair(x[i], label[i]));
    }
    redrawPlot();
}

void JKQTPCoordinateAxis::addAxisTickLabels(const double *x, const QStringList &label) {
    for (int i=0; i<label.size(); i++) {
        tickLabels.append(qMakePair(x[i], label[i]));
    }
    redrawPlot();
}

void JKQTPCoordinateAxis::addAxisTickLabels(const QVector<double> &x, const QString *label) {
    for (int i=0; i<x.size(); i++) {
        tickLabels.append(qMakePair(x[i], label[i]));
    }
    redrawPlot();
}

void JKQTPCoordinateAxis::addAxisTickLabels(const double *x, const QString *label, int items) {
    for (int i=0; i<items; i++) {
        tickLabels.append(qMakePair(x[i], label[i]));
    }
    redrawPlot();
}

void JKQTPCoordinateAxis::loadSettings(const QSettings& settings, const QString& group) {
    autoAxisSpacing=settings.value(group+axisPrefix+"auto_spacing").toBool();
    logAxis=settings.value(group+axisPrefix+"log").toBool();
    inverted=settings.value(group+axisPrefix+"inverted").toBool();
    logAxisBase=settings.value(group+axisPrefix+"log_base").toDouble();
    userTickSpacing=settings.value(group+axisPrefix+"tick_spacing").toDouble();
    userLogTickSpacing=settings.value(group+axisPrefix+"log_tick_spacing").toDouble();
    axisLabel=settings.value(group+axisPrefix+"axis_label", axisLabel).toString();

    axisStyle.loadSettings(settings, group+axisPrefix);
    paramsChanged=true;
    calcPlotScaling();
}



void JKQTPCoordinateAxis::saveSettings(QSettings& settings, const QString& group) const {
    settings.setValue(group+axisPrefix+"auto_spacing", autoAxisSpacing);
    settings.setValue(group+axisPrefix+"log", logAxis);
    settings.setValue(group+axisPrefix+"log_base", logAxisBase);
    settings.setValue(group+axisPrefix+"tick_spacing", userTickSpacing);
    settings.setValue(group+axisPrefix+"log_tick_spacing", userLogTickSpacing);

    settings.setValue(group+axisPrefix+"label", axisLabel);



    axisStyle.saveSettings(settings, group+axisPrefix);
}

double JKQTPCoordinateAxis::calcLinearTickSpacing() {
    if (axisStyle.tickLabelType==JKQTPCALTdate) {
        QDateTime dt;
        dt.setMSecsSinceEpoch(0);
        //qDebug()<<"  dt="<<dt;
        dt=dt.addYears(1);
        //qDebug()<<dt;
        double deltaDT=dt.toMSecsSinceEpoch();
        //qDebug()<<dt<<dt.toMSecsSinceEpoch()<<deltaDT;
        int level=0;
        double TickSpacing=deltaDT;
        QVector<double> tickbase;
        tickbase<<1<< 2<< 5<<10<<20<<50<<100<<200<<500<<1000;
        //qDebug()<<"startexponent="<<startexponent<<"   width="<<width;
        unsigned int current_tickbase=tickbase.size();
        unsigned int tick_count=0;
        int cnt=0;
        do {
            if (current_tickbase==0) {
                level++;
                if (level==1) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addMonths(1);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<< 6<<12;
                } else if (level==2) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addDays(1);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<< 6<<12;
                } else if (level>=3) {
                    break;
                }
                current_tickbase=tickbase.size()-1;
            } else current_tickbase--;
            TickSpacing=tickbase[current_tickbase]*deltaDT;
            tick_count=(unsigned int)round(width/TickSpacing);
            //qDebug()<<"current_tickbase="<<tickbase[current_tickbase]<<"   level="<<level<<"   tTickSpacing="<<TickSpacing<<"   deltaDT="<<deltaDT<<"   tick_count="<<tick_count;
            cnt++;
        } while (level<=3 && tick_count<axisStyle.minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/static_cast<double>(axisStyle.minTicks);
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    } else if (axisStyle.tickLabelType==JKQTPCALTdatetime) {
        QDateTime dt;
        dt.setMSecsSinceEpoch(0);
        //qDebug()<<"  dt="<<dt;
        dt=dt.addYears(1);
        //qDebug()<<dt;
        double deltaDT=dt.toMSecsSinceEpoch();
        //qDebug()<<dt<<dt.toMSecsSinceEpoch()<<deltaDT;
        int level=0;
        double TickSpacing=deltaDT;
        QVector<double> tickbase;
        tickbase<<1<< 2<< 5<<10<<20<<50<<100<<200<<500<<1000;
        //qDebug()<<"startexponent="<<startexponent<<"   width="<<width;
        unsigned int current_tickbase=tickbase.size();
        unsigned int tick_count=0;
        int cnt=0;
        do {
            if (current_tickbase==0) {
                level++;
                if (level==1) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addMonths(1);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<< 6<<12;
                } else if (level==2) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addDays(1);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<< 6<<12;
                } else if (level==3) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addSecs(60*60);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<< 6<<12<<24;
                } else if (level==4) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addSecs(60);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<<5<<15<<20<<30<<45<<60<<120;
                } else if (level==5) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addSecs(1);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<<5<<15<<20<<30<<45<<60<<120;
                } else if (level>=6) {
                    break;
                }
                current_tickbase=tickbase.size()-1;
            } else current_tickbase--;
            TickSpacing=tickbase[current_tickbase]*deltaDT;
            tick_count=(unsigned int)round(width/TickSpacing);
            //qDebug()<<"current_tickbase="<<tickbase[current_tickbase]<<"   level="<<level<<"   tTickSpacing="<<TickSpacing<<"   deltaDT="<<deltaDT<<"   tick_count="<<tick_count;
            cnt++;
        } while (level<6 && tick_count<axisStyle.minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/static_cast<double>(axisStyle.minTicks);
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    } else if (axisStyle.tickLabelType==JKQTPCALTtime) {
        QDateTime dt;
        dt.setMSecsSinceEpoch(0);
        //qDebug()<<"  dt="<<dt;
        dt=dt.addSecs(60*60);
        //qDebug()<<dt;
        double deltaDT=dt.toMSecsSinceEpoch();
        //qDebug()<<dt<<dt.toMSecsSinceEpoch()<<deltaDT;
        int level=0;
        double TickSpacing=deltaDT;
        QVector<double> tickbase;
        tickbase<<1<< 2<< 5<<10<<20<<50<<100<<200<<500<<1000;
        //qDebug()<<"startexponent="<<startexponent<<"   width="<<width;
        unsigned int current_tickbase=tickbase.size();
        unsigned int tick_count=0;
        int cnt=0;
        do {
            if (current_tickbase==0) {
                level++;
                if (level==1) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addSecs(60);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<<5<<15<<20<<30<<45<<60<<120;
                } else if (level==2) {
                    dt.setMSecsSinceEpoch(0);
                    dt=dt.addSecs(1);
                    deltaDT=dt.toMSecsSinceEpoch();
                    tickbase.clear();
                    tickbase<<1<< 2<<5<<15<<20<<30<<45<<60<<120;
                } else if (level>=3) {
                    break;
                }
                current_tickbase=tickbase.size()-1;
            } else current_tickbase--;
            TickSpacing=tickbase[current_tickbase]*deltaDT;
            tick_count=(unsigned int)round(width/TickSpacing);
            //qDebug()<<"current_tickbase="<<tickbase[current_tickbase]<<"   level="<<level<<"   tTickSpacing="<<TickSpacing<<"   deltaDT="<<deltaDT<<"   tick_count="<<tick_count;
            cnt++;
        } while (level<3 && tick_count<axisStyle.minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/static_cast<double>(axisStyle.minTicks);
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    } else {
        double TickSpacing=1;
        int startexponent=static_cast<int>(floor(log10(width)))+3;
        double tickbase[]={1, 2, 2.5, 5, 10};
        int tb_max=4;
        //qDebug()<<"startexponent="<<startexponent<<"   width="<<width;
        int current_exponent=startexponent;
        unsigned int current_tickbase=tb_max+1;
        unsigned int tick_count=0;
        int cnt=0;
        do {
            if (current_tickbase==0) {
                current_tickbase=tb_max;
                current_exponent--;
            } else current_tickbase--;
            TickSpacing=tickbase[current_tickbase]*pow(10,current_exponent);
            tick_count=(unsigned int)round(width/TickSpacing);
            //qDebug()<<"current_tickbase="<<tickbase[current_tickbase]<<"   current_exponent="<<current_exponent<<"   tTickSpacing="<<TickSpacing<<"   tick_count="<<tick_count;
            cnt++;
        } while (tick_count<axisStyle.minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/static_cast<double>(axisStyle.minTicks);
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    }
}


double JKQTPCoordinateAxis::calcLogTickSpacing() {
      double TickSpacing=logAxisBase;
      if (fabs(log(axismax)-log(axismin))/log(logAxisBase)>2*axisStyle.minTicks) {
          double power=1;
          int cnt=0;
          while (fabs(log(axismax)-log(axismin))/log(logAxisBase)/power>2*axisStyle.minTicks && cnt<1000) {
#ifdef SHOW_JKQTPLOTTER_DEBUG
              //qDebug()<<cnt<<power<<log(width)/log(logAxisBase)/power<<2*axisStyle.minTicks;
#endif
              power++;
              cnt++;
          }
          if (cnt<1000) TickSpacing=pow(logAxisBase, power);
      }
#ifdef SHOW_JKQTPLOTTER_DEBUG
      //qDebug()<<"log-tick-spacing: "<<TickSpacing;
#endif
      return TickSpacing;

}

QString JKQTPCoordinateAxis::floattostringWithFormat(const QLocale & loc, double data, char format, int past_comma, bool remove_trail0) const
{
    QString res=loc.toString(data, format, past_comma);
    if (remove_trail0 && res.contains(QLocale::system().decimalPoint())) {
        QString expo="";
        const int expIdx=res.toLower().indexOf('e');
        if (expIdx>=0) {
            expo=res.mid(expIdx);
            res=res.left(expIdx);
        }
        while (res.size()>0 && res[res.size()-1]=='0') {
            res=res.left(res.size()-1);
        }
        if (res.size()>0 && res[res.size()-1]==loc.decimalPoint()) res=res.left(res.size()-1);
        res=res+expo;
    }
    return res;

}
QString JKQTPCoordinateAxis::floattostringWithFormat(double data, char format, int past_comma, bool remove_trail0) const
{
    QLocale loc=QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    return floattostringWithFormat(loc, data, format, past_comma, remove_trail0);
}

QString JKQTPCoordinateAxis::floattolabel(double data) const {
    const int past_comma=axisStyle.labelDigits;
    return floattolabel(data, past_comma);
}

QString JKQTPCoordinateAxis::floattolabel(double data, int past_comma) const {
    const bool remove_trail0=true;
    QLocale loc= QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);

    double belowIsZero=1e-300;
    if (!getLogAxis()) {
        belowIsZero=fabs(getMax()-getMin())*1e-6;
    }

    data=data/tickUnitFactor;

    auto addTickUnit=[&](const QString& s) {
        if (tickUnitName.isEmpty()) return s;
        bool ok=false;
        const double d=s.toDouble(&ok);
        if (s=="0" || (ok && fabs(d)<1e-300)) return s;
        if (s=="1" || (ok && d==1.0)) return tickUnitName;
        return s+tickUnitName;
    };

    switch(axisStyle.tickLabelType) {
        case JKQTPCALTcount:
            return "";
        case JKQTPCALTdefault:
            return addTickUnit(floattostringWithFormat(loc, data, 'f', past_comma, remove_trail0));
        case JKQTPCALTscientific:
            return addTickUnit(floattostringWithFormat(loc, data, 'e', past_comma, remove_trail0));
        case JKQTPCALTexponent:
            return addTickUnit(QString(jkqtp_floattolatexstr(data, past_comma, remove_trail0, belowIsZero, pow(10, -past_comma), pow(10, past_comma+1)).c_str()));
        case JKQTPCALTexponentCharacter:
            return addTickUnit(QString(jkqtp_floattolatexunitstr(data, past_comma, remove_trail0).c_str()));
        case JKQTPCALTintfrac:
        case JKQTPCALTintsfrac:
        case JKQTPCALTintslashfrac:
        case JKQTPCALTfrac:
        case JKQTPCALTsfrac:
        case JKQTPCALTslashfrac: {
            uint64_t num=0;
            uint64_t denom=0;
            uint64_t intpart=0;
            int sign=+1;
            const double powfac=pow(10,past_comma);
            const double rounded=round(data*powfac)/powfac;
            jkqtp_estimateFraction(rounded, sign, intpart, num, denom);
            //std::cout<<"\n"<<data<<" => "<<rounded<<", "<<sign<<"*( "<<intpart<<" + "<<num<<"/"<<denom<<" )\n";
            if (axisStyle.tickLabelType==JKQTPCALTfrac || axisStyle.tickLabelType==JKQTPCALTsfrac || axisStyle.tickLabelType==JKQTPCALTslashfrac) {
                if (intpart>0) {
                    num=num+denom*intpart;
                    intpart=0;
                }
            }
            //std::cout<<data<<" => "<<rounded<<", "<<sign<<"*( "<<intpart<<" + "<<num<<"/"<<denom<<" )\n";
            QString res;
            if (rounded==0.0 || (intpart==0 && num==0)) res= "0";
            else {
                if (sign<0) res+="-";
                if (intpart!=0) res+=QString::number(intpart);
                if (num!=0) {
                    if (denom==1) res+=QString::number(num);
                    else {
                        if (axisStyle.tickLabelType==JKQTPCALTfrac || (axisStyle.tickLabelType==JKQTPCALTintfrac && intpart==0)) res+=QString("\\frac{%1}{%2}").arg(addTickUnit(QString::number(num))).arg(denom);
                        else if (axisStyle.tickLabelType==JKQTPCALTintfrac) res=addTickUnit(res+QString("\\frac{%1}{%2}").arg(num).arg(denom));
                        else if (axisStyle.tickLabelType==JKQTPCALTsfrac || (axisStyle.tickLabelType==JKQTPCALTintsfrac && intpart==0)) res+=QString("\\sfrac{%1}{%2}").arg(addTickUnit(QString::number(num))).arg(denom);
                        else if (axisStyle.tickLabelType==JKQTPCALTintsfrac) res=addTickUnit(res+QString("\\sfrac{%1}{%2}").arg(num).arg(denom));
                        else {
                            if (res.size()>0 && res[res.size()-1].isDigit()) {
                                if (sign<0) res+="-";
                                else res+="+";
                            }
                            if (axisStyle.tickLabelType==JKQTPCALTintslashfrac) {
                                res+=QString("%1/%2").arg(num).arg(denom);
                                if (intpart!=0 && !tickUnitName.isEmpty()) res="("+res+")";
                            }
                        }
                    }
                } else {
                    res=addTickUnit(res);
                }
            }
            //std::cout<<data<<" => "<<rounded<<", "<<res.toStdString()<<"\n";
            return res;
        }
        case JKQTPCALTdate: {
                QDateTime dt;
                dt.setMSecsSinceEpoch(uint64_t(data));
                return dt.toString(axisStyle.tickDateFormat);
            }; break;
        case JKQTPCALTtime: {
                QDateTime dt;
                dt.setMSecsSinceEpoch(uint64_t(data));
                return dt.toString(axisStyle.tickTimeFormat);
            }; break;
        case JKQTPCALTdatetime: {
                QDateTime dt;
                dt.setMSecsSinceEpoch(uint64_t(data));
                return dt.toString(axisStyle.tickDateTimeFormat);
            }; break;
        case JKQTPCALTprintf: {
                return QString::asprintf(axisStyle.tickPrintfFormat.toLatin1().data(), data, tickUnitName.toStdString().c_str());
            }; break;
#if __cplusplus >= 202002L
# ifdef __cpp_lib_format
    case JKQTPCALTformat: {
            return QString::fromStdString(std::vformat(axisStyle.tickFormatFormat.toStdString(), std::make_format_args(data, tickUnitName.toStdString())));
        }; break;
        /** \copydoc JKQTPCoordinateAxisStyle::tickFormatFormat */
        void setTickFormatFormat(const QString& __value);
# endif
#endif
    }
    return QString();
}

int JKQTPCoordinateAxis::calcLinearUnitDigits() {
    if (!axisStyle.autoLabelDigits) return axisStyle.labelDigits;
    int unitdigits=-1;
    double minval=tickStart;
    bool equals=true;
    /*for (int i=0; i<20; i++) {
        equals=equals || (floattolabel((minval+static_cast<double>(i)*tickSpacing), unitdigits)== floattolabel((minval+static_cast<double>(i+1)*tickSpacing), unitdigits));
    }*/
    while ((unitdigits<20) && equals) {
        unitdigits++;
        equals=false;
        for (int i=-10; i<10; i++) {
            QString l1=floattolabel((minval+static_cast<double>(i)*tickSpacing), unitdigits+2);
            QString l2=floattolabel((minval+static_cast<double>(i+1.0)*tickSpacing), unitdigits+2);
            //qDebug()<<"unitdigits="<<unitdigits<<"   l1="<<l1<<"   l2="<<l2;
            equals=equals || (l1==l2);
        }
    }
    return unitdigits+2;
}


void JKQTPCoordinateAxis::calcPlotScaling(bool force) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTBasePlotter[%1]::calcPlotScaling()").arg(objectName()));
#endif
    if (!doUpdateScaling) return;
    if (!force && !paramsChanged) return;
    if (axismin>axismax) {
        std::swap(axismin, axismax);
    } else if (axismax==axismin) {
        axismax=axismin+1.0;
    }
    if (isLogAxis()) {

        if (axismin<0) {
            if (axismax>0) axismin=axismax/(logAxisBase*logAxisBase);
            else axismin=1e-5;
        }
        if (axismax<=axismin) axismax=axismin*logAxisBase*logAxisBase;
        //if (axismax<0) axismax=pow(10.0, ceil(log10(axismax-axismin)+1.0));


        if (axismin<=0) axismin=1e-5;
        if (axismax<=0) axismax=axismin+pow(10.0, static_cast<int>(log10(axismin)));
        if (axismax<axismin) {
            std::swap(axismin, axismax);
        } else if (axismax==axismin) {
            axismax=axismin+pow(10.0, static_cast<int>(log10(axismin)));
        }
    }

    if (axismin>axismax) {
        std::swap(axismin, axismax);
    } else if (axismax==axismin) {
        axismax=axismin+1.0;
    }

    // this is the x- and y-range spanned by the plot
    width=fabs(axismax-axismin);
    //qDebug()<<"calcPlotScaling():\n";
    //qDebug()<<"    width="<<width<<"\n";


    double plotOffset=getParentPlotOffset();
#ifdef SHOW_JKQTPLOTTER_DEBUG
    //qDebug()<<getParent()->objectName()<<":  inverted   = "<<inverted;
    //qDebug()<<getParent()->objectName()<<":  plotOffset = "<<plotOffset;
    //qDebug()<<getParent()->objectName()<<":  plotWidth  = "<<getParentPlotWidth();
#endif
    if (isLogAxis()) {
        scale=getParentPlotWidth()/(log(axismax)-log(axismin))*log(logAxisBase);
        if (scaleSign>0) {
            offset=plotOffset-log(axismin)/log(logAxisBase)*scale;
        } else {
            offset=plotOffset+log(axismax)/log(logAxisBase)*scale;
        }
    } else {
        scale=getParentPlotWidth()/width;
        if (scaleSign>0) {
            offset=plotOffset-axismin*scale;
        } else {
            offset=plotOffset+axismax*scale;
        }

    }
#ifdef SHOW_JKQTPLOTTER_DEBUG
    //qDebug()<<getParent()->objectName()<<":  offset     = "<<offset;
    //qDebug()<<getParent()->objectName()<<":  scale      = "<<scale;
    //qDebug()<<getParent()->objectName()<<":  scaleSign  = "<<scaleSign;
#endif
    // now we try to find a suitable tick and grid line spacing so that
    // between maxTicks and axisStyle.minTicks ticks fit into twidth
    // if autoXAxisSpacing/autoYAxisSpacing is false then the spacing properties
    // are calculated from the user-supplied parameters currently there is no
    // autoscaling for the logarithmic plots available
    if (axisStyle.tickMode==JKQTPLTMLinOrPower) {
        if (logAxis) {
            tickSpacing=tickUnitFactor;
            tickSpacingLog=calcLogTickSpacing();
            tickStart=pow(logAxisBase, floor(log(axismin/tickUnitFactor)/log(logAxisBase)))*tickUnitFactor;
        } else {
            if (autoAxisSpacing) {
                // autoscaling linear x-axis
                tickSpacingLog=10.0;
                width/=tickUnitFactor;
                tickSpacing=calcLinearTickSpacing()*tickUnitFactor;
                width*=tickUnitFactor;
            } else {
                // predefined scaling for linear x-axis
                tickSpacing=userTickSpacing;
                tickSpacingLog=userLogTickSpacing;
            }
            tickStart=floor(axismin/(tickSpacing))*tickSpacing;
        }
    } else if (axisStyle.tickMode==JKQTPLTMLin) {
        if (autoAxisSpacing) {
            // autoscaling linear x-axis
            tickSpacingLog=10.0;
            width/=tickUnitFactor;
            tickSpacing=calcLinearTickSpacing()*tickUnitFactor;
            width*=tickUnitFactor;
        } else {
            // predefined scaling for linear x-axis
            tickSpacing=userTickSpacing;
            tickSpacingLog=userLogTickSpacing;
        }
        tickStart=floor(axismin/(tickSpacing))*tickSpacing;
    } else if (axisStyle.tickMode==JKQTPLTMPower) {
        tickSpacing=1.0*tickUnitFactor;
        tickSpacingLog=calcLogTickSpacing();
        tickStart=pow(logAxisBase, floor(log(axismin/tickUnitFactor)/log(logAxisBase)))*tickUnitFactor;
    }

    axisStyle.labelDigits=calcLinearUnitDigits();
#ifdef SHOW_JKQTPLOTTER_DEBUG
    //qDebug()<<"    tickStart="<<tickStart<<"\n";
    //qDebug()<<"    tickSpacing="<<tickSpacing<<"\n";
    //qDebug()<<"    labelDigits="<<axisStyle.labelDigits<<"\n";
#endif
    paramsChanged=false;

}

bool JKQTPCoordinateAxis::isLogAxis() const {
    return logAxis;// || (axisStyle.tickMode==JKQTPLTMPower);
}

bool JKQTPCoordinateAxis::isLinearAxis() const
{
    return !logAxis;
}

const JKQTPCoordinateAxisStyle &JKQTPCoordinateAxis::getCurrentAxisStyle() const
{
    return axisStyle;
}

void JKQTPCoordinateAxis::setCurrentAxisStyle(const JKQTPCoordinateAxisStyle &style)
{
    axisStyle=style;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::loadCurrentAxisStyle(const QSettings &settings, const QString &group)
{
    axisStyle.loadSettings(settings, group);
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::saveCurrentAxisStyle(QSettings &settings, const QString &group) const
{
    axisStyle.saveSettings(settings, group);
}

void JKQTPCoordinateAxis::setRange(double aamin, double aamax) {
    const double oldamin=axismin;
    const double oldamax=axismax;
    double amin=std::min(aamin, aamax);
    double amax=std::max(aamin, aamax);
    if (axisMinWidth>0 && fabs(amax-amin)<axisMinWidth) {
        amax=amin+axisMinWidth;
    }

    axismin=amin;
    axismax=amax;

    if (amin<axisabsoultemin) axismin=axisabsoultemin;
    if (amax>axisabsoultemax) axismax=axisabsoultemax;


    if (isLogAxis()) {
        if (axismin<=0) axismin=1e-306;
        if (axismax<=0) axismax=1e-306;
        if (fabs(axismin-axismax)<1e-306) {
            axismax=10;
            axismin=0.1;
        }
    }

    if (axisMinWidth>0 && fabs(axismax-axismin)<axisMinWidth) {
        axismax=axismin+axisMinWidth;
    }
    if (fabs(axismax-axismin)<1e-308) {
        axismax=axismin+1;
    }
    if (oldamin!=axismin || oldamax!=axismax) {
        paramsChanged=true;
        calcPlotScaling();
        redrawPlot();
    }
}

void JKQTPCoordinateAxis::setNoAbsoluteRange() {
    axisabsoultemin=-DBL_MAX;
    axisabsoultemax=DBL_MAX;
    setRange(axismin, axismax);
}

void JKQTPCoordinateAxis::setTickSpacing(double __value) {
    this->tickSpacing = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setAxisMinWidth(double __value) {
    this->axisMinWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setAutoAxisSpacing(bool __value) {
    this->autoAxisSpacing = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickLabelsEnabled(bool __value) {
    this->axisStyle.minorTickLabelsEnabled = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setLogAxis(bool __value)
{
    this->logAxis = __value;
    this->paramsChanged=true;
    if (this->isLogAxis()) {
        if (axismin<0) axismin=pow(10.0, floor(log(axismax-axismin)/JKQTPSTATISTICS_LN10-1.0));
        if (axismax<0) axismax=pow(10.0, floor(log(axismax-axismin)/JKQTPSTATISTICS_LN10+1.0));
    }
    redrawPlot();
}

void JKQTPCoordinateAxis::setLogAxisBase(double __value) {
    this->logAxisBase = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setUserTickSpacing(double __value) {
    this->userTickSpacing = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setUserLogTickSpacing(double __value) {
    this->userLogTickSpacing = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickLabelType(JKQTPCALabelType __value) {
    this->axisStyle.tickLabelType = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickMode(JKQTPLabelTickMode __value)
{
    this->axisStyle.tickMode = __value;
    this->paramsChanged=true;
    setLogAxis(this->logAxis);
}

void JKQTPCoordinateAxis::setTickMode(int __value) {
    setTickMode(JKQTPLabelTickMode(__value));
}

void JKQTPCoordinateAxis::setAxisLabel(const QString& __value) {
    this->axisLabel = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickUnitName(const QString &__value)
{
    this->tickUnitName = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickUnitFactor(double __value)
{
    this->tickUnitFactor = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickUnit(double factor, const QString &name)
{
    this->tickUnitFactor = factor;
    this->tickUnitName = name;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickUnitPi()
{
    setTickUnit(JKQTPSTATISTICS_PI, "\\;\\pi");
}

void JKQTPCoordinateAxis::resetTickUnit()
{
    setTickUnit(1, "");
}

void JKQTPCoordinateAxis::setLabelPosition(JKQTPLabelPosition __value) {
    this->axisStyle.labelPosition = __value;
    this->paramsChanged=true;
    redrawPlot();
}


void JKQTPCoordinateAxis::setLabelFontSize(double __value) {
    this->axisStyle.labelFontSize = __value;
    this->paramsChanged=true;
    redrawPlot();
}


void JKQTPCoordinateAxis::setTickTimeFormat(const QString& __value) {
    this->axisStyle.tickTimeFormat = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickDateFormat(const QString& __value) {
    this->axisStyle.tickDateFormat = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickDateTimeFormat(const QString& __value) {
    this->axisStyle.tickDateTimeFormat = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickPrintfFormat(const QString& __value) {
    this->axisStyle.tickPrintfFormat = __value;
    this->paramsChanged=true;
    redrawPlot();
}
#if __cplusplus >= 202002L
# ifdef __cpp_lib_format
void JKQTPCoordinateAxis::setTickFormatFormat(const QString &__value)
{
    this->axisStyle.tickFormatFormat = __value;
    this->paramsChanged=true;
    redrawPlot();

}
# endif
#endif



void JKQTPCoordinateAxis::setTickLabelFontSize(double __value) {
    this->axisStyle.tickLabelFontSize = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickLabelFontSize(double __value) {
    this->axisStyle.minorTickLabelFontSize = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickLabelFullNumber(bool __value) {
    this->axisStyle.minorTickLabelFullNumber = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinTicks(unsigned int __value) {
    this->axisStyle.minTicks = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTicks(unsigned int __value) {
    this->axisStyle.minorTicks = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTicks(int __value) {
    this->axisStyle.minorTicks = qMax(int(0), __value);
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickOutsideLength(double __value) {
    this->axisStyle.tickOutsideLength = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickOutsideLength(double __value) {
    this->axisStyle.minorTickOutsideLength = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickInsideLength(double __value) {
    this->axisStyle.tickInsideLength = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickInsideLength(double __value) {
    this->axisStyle.minorTickInsideLength = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setAxisColor(const QColor& __value) {
    this->axisStyle.axisColor = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setShowZeroAxis(bool __value) {
    this->axisStyle.showZeroAxis = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setInverted(bool __value) {
    this->inverted = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setGridColor(const QColor& __value) {
    this->axisStyle.majorGridStyle.lineColor = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setGridColor(const QColor &__value, double alpha)
{
    this->axisStyle.majorGridStyle.lineColor = __value;
    this->axisStyle.majorGridStyle.lineColor.setAlphaF(alpha);
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorGridColor(const QColor& __value) {
    this->axisStyle.minorGridStyle.lineColor = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorGridColor(const QColor& __value, double alpha) {
    this->axisStyle.minorGridStyle.lineColor = __value;
    this->axisStyle.minorGridStyle.lineColor.setAlphaF(alpha);
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setGridWidth(double __value) {
    this->axisStyle.majorGridStyle.lineWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setGridStyle(Qt::PenStyle __value) {
    this->axisStyle.majorGridStyle.lineStyle = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorGridWidth(double __value) {
    this->axisStyle.minorGridStyle.lineWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorGridStyle(Qt::PenStyle __value) {
    this->axisStyle.minorGridStyle.lineStyle = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setDrawMode1(JKQTPCADrawMode __value) {
    this->axisStyle.drawMode1 = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setDrawMode0(JKQTPCADrawMode __value)
{
    this->axisStyle.drawMode0 = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setDrawMode2(JKQTPCADrawMode __value) {
    this->axisStyle.drawMode2 = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickWidth(double __value) {
    this->axisStyle.minorTickWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickWidth(double __value) {
    this->axisStyle.tickWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setLineWidth(double __value) {
    this->axisStyle.lineWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setLineWidthZeroAxis(double __value) {
    this->axisStyle.lineWidthZeroAxis = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickLabelDistance(double __value) {
    this->axisStyle.tickLabelDistance = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setLabelDistance(double __value) {
    this->axisStyle.labelDistance = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setLabelDigits(int __value) {
    this->axisStyle.labelDigits = __value;
    this->paramsChanged=true;
    this->axisStyle.autoLabelDigits=false;
    redrawPlot();
}

void JKQTPCoordinateAxis::setDrawGrid(bool __value) {
    this->axisStyle.majorGridStyle.enabled = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setDrawMinorGrid(bool __value) {
    this->axisStyle.minorGridStyle.enabled = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickLabelAngle(double __value) {
    this->axisStyle.tickLabelAngle = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setArrowSizeFactor(double f)  {
    axisStyle.arrowSizeFactor=f;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setLabelColor(QColor c)  {
    axisStyle.labelColor=c;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickColor(QColor c)  {
    axisStyle.minorTickColor=c;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setMinorTickLabelColor(QColor c)  {
    axisStyle.minorTickLabelColor=c;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickColor(QColor c)  {
    axisStyle.tickColor=c;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPCoordinateAxis::setTickLabelColor(QColor c)  {
    axisStyle.tickLabelColor=c=c;
    this->paramsChanged=true;
    redrawPlot();
}

std::pair<QSizeF,QSizeF> JKQTPCoordinateAxis::getSize0(JKQTPEnhancedPainter &/*painter*/)
{
    return std::pair<QSizeF,QSizeF>(QSizeF(0,0),QSizeF(0,0));
}


void JKQTPCoordinateAxis::setAbsoluteRange(double amin, double amax) {
    axisabsoultemin=std::min(amin, amax);
    axisabsoultemax=std::max(amin, amax);

    if (axisabsoultemin==axisabsoultemax) {
        axisabsoultemax=axisabsoultemin+1;
    }

    // ensure that the actual axis range is within the absolute range
    setRange(axismin, axismax);
}

double JKQTPCoordinateAxis::getNextLabelDistance(double x) {
    if  (axisStyle.tickMode==JKQTPLTMLinOrPower) {
        if (logAxis) {
            return x*tickSpacingLog;
        } else {
            return tickSpacing;
        }
    } else if  (axisStyle.tickMode==JKQTPLTMLin) {
        return tickSpacing;
    } else if  (axisStyle.tickMode==JKQTPLTMPower) {
        return x*tickSpacingLog;
    }
    return 0;
}

bool JKQTPCoordinateAxis::getNextLabel(double& x, QString& label, bool init) {
    //qDebug()<<"start JKQTPCoordinateAxis::getNextLabel("<<x<<label<<init<<")";
    label="";
    if (tickLabels.size()<=0) {
        if (init) {
            x=tickStart;
            label=floattolabel(x);
            return true;
        } else {
            if  (axisStyle.tickMode==JKQTPLTMLinOrPower) {
                if (logAxis) {
                    x*=tickSpacingLog;
                } else {
                    x=x+tickSpacing;
                }
            } else if  (axisStyle.tickMode==JKQTPLTMLin) {
                x=x+tickSpacing;
            } else if  (axisStyle.tickMode==JKQTPLTMPower) {
                x*=tickSpacingLog;
            }
        }
        if (x>=axismin && x<=axismax) {
            label=floattolabel(x);
        }
        return (x<=axismax);
    } else {
        if (init) {
            if (tickLabels.size()>0) {
                x=tickLabels[0].first;
                label=tickLabels[0].second;
            }
            return (tickLabels.size()>0);
        } else {
            for (int i=0; i<tickLabels.size(); i++) {
                if (fabs(x-tickLabels[i].first)<1e-30 && i+1<tickLabels.size()) {
                    label=tickLabels[i+1].second;
                    x=tickLabels[i+1].first;
                    return (x>=axismin && x<=axismax);
                }
            }
        }
    }
    return false;
}

QSizeF JKQTPCoordinateAxis::getMaxTickLabelSize(JKQTPEnhancedPainter& painter, double* ascent, double* descent) {
    double w=0, h=0, a=0;
    calcPlotScaling();

    double x=tickStart;
    QString label="";
    getNextLabel(x, label, true);
    QFont f;
    f.setFamily(JKQTMathTextFontSpecifier::fromFontSpec(getParent()->getCurrentPlotterStyle().defaultFontName).fontName());
    f.setPointSizeF(this->axisStyle.tickLabelFontSize*parent->getFontSizeMultiplier());
    bool first=true;
    int cnt=0;
    while (getNextLabel(x, label, first) && cnt<50) {
        double width, ascent, descent, strikeoutPos;
        parent->getTextSizeDetail(f, label, painter, width, ascent, descent, strikeoutPos);
        if (width>w) w=width;
        if (ascent+descent>h) h=ascent+descent;
        if (ascent>a) a=ascent;
        first=false;
        cnt++;
    }
    //qDebug()<<"getMaxTickLabelSize() = ["<<w<<", "<<h<<"]\n";
    if (ascent) *ascent=a;
    if (descent) *descent=h-a;
    if (axisStyle.tickLabelAngle!=0) {
        QRectF r(0,0,w,h);
        return jkqtpRotateRect(r, axisStyle.tickLabelAngle).boundingRect().size();
    } else {
        return QSizeF(w, h);
    }
}

void JKQTPCoordinateAxis::drawAxisLine(JKQTPEnhancedPainter& painter, const QLineF& l, JKQTPCADrawMode drawMode) const {
    const double arrowSize=axisStyle.getArrowSize(painter, parent);
    JKQTPLineDecoratorStyle st1=JKQTPNoDecorator, st2=JKQTPNoDecorator;
    if (drawMode.testFlag(JKQTPCADMMinArrow)) st1=JKQTPArrow;
    if (drawMode.testFlag(JKQTPCADMMinFilledArrow)) st1=JKQTPFilledArrow;
    if (drawMode.testFlag(JKQTPCADMMaxArrow)) st2=JKQTPArrow;
    if (drawMode.testFlag(JKQTPCADMMaxFilledArrow)) st2=JKQTPFilledArrow;
    JKQTPPlotDecoratedLine(painter, l, st1, arrowSize,
                                       st2, arrowSize);
}

double JKQTPCoordinateAxis::getZeroAxisPos(bool *drawZeroAxis) {
    double zeroaxispos=0;
    if (axisStyle.drawMode0!=JKQTPCADMnone) {
        zeroaxispos=qBound(getParentOtheraxisOffset(), parentOtherAxisX2P(0), getParentOtheraxisOffset()+getParentOtheraxisWidth());
        if(drawZeroAxis) *drawZeroAxis=true;
    }
    return zeroaxispos;
}







JKQTPVerticalAxis::JKQTPVerticalAxis(JKQTBasePlotter* parent):
    JKQTPCoordinateAxis(parent)
{
    //axisPrefix="yaxis_";
    scaleSign=-1;
    if (parent) {
        axisStyle=parent->getCurrentPlotterStyle().yAxisStyle;
    } else {
        axisStyle=JKQTPGetSystemDefaultBaseStyle().yAxisStyle;
    }
}

double JKQTPVerticalAxis::getParentPlotWidth() const {
    return parent->getPlotHeight();
}

double JKQTPVerticalAxis::getParentPlotOffset() const {
    return parent->getInternalPlotBorderTop();
}

std::pair<QSizeF,QSizeF> JKQTPVerticalAxis::getSize0(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode0==JKQTPCADMnone) return std::pair<QSizeF,QSizeF>(QSizeF(0,0),QSizeF(0,0));
    double ptwidth=0;
    double ptwidth_r=0;
    const double arrowSize=((axisStyle.drawMode0&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))?(axisStyle.getArrowSize(painter, parent)/2.0):0.0;
    double labwidth=0;
    if (axisStyle.drawMode0.testFlag(JKQTPCADMTicks)) { ptwidth+=axisStyle.tickOutsideLength; ptwidth_r+=axisStyle.tickInsideLength; }
    if (axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels)) {
        ptwidth+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labwidth+=getMaxTickLabelSize(painter).width();
    }
    if (axisStyle.drawMode0.testFlag(JKQTPCADMAxisLabel)) {
        ptwidth+=axisStyle.labelDistance;
        // find out size of axis label
        labwidth+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    const double zaxispos=getZeroAxisPos();
    const double bottom=x2p(axismin);
    const double bottom_offset=(bottom-zaxispos);
    const double top=x2p(axismax);
    const double top_offset=(zaxispos-top);
    const QSizeF s1(qMax(0.0,qMax(parent->pt2px(painter, ptwidth)+labwidth, arrowSize)-bottom_offset), getParentPlotWidth());
    const QSizeF s2(qMax(0.0,qMax(parent->pt2px(painter, ptwidth_r), arrowSize)-top_offset), getParentPlotWidth());
    //qDebug()<<"JKQTPVerticalAxis::getSize0():  s1="<<s1<<", s2="<<s2<<", bottom_offset="<<bottom_offset<<", top_offset="<<top_offset<<" top="<<top<<", bottom="<<bottom<<", zaxispos="<<zaxispos;
    return std::pair<QSizeF,QSizeF>(s1,s2);
}


QSizeF JKQTPVerticalAxis::getSize1(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode1==JKQTPCADMnone) return QSize(0,0);
    double ptwidth=axisStyle.axisLineOffset;
    const double arrowSize=((axisStyle.drawMode1&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))?(axisStyle.getArrowSize(painter, parent)/2.0):0.0;
    double labwidth=0;
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) ptwidth+=axisStyle.tickOutsideLength;
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)) {
        ptwidth+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labwidth+=getMaxTickLabelSize(painter).width();
    }
    if (axisStyle.drawMode1.testFlag(JKQTPCADMAxisLabel)) {
        ptwidth+=axisStyle.labelDistance;
        // find out size of axis label
        labwidth+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(qMax(parent->pt2px(painter, ptwidth)+labwidth, arrowSize), getParentPlotWidth()).expandedTo(getSize0(painter).first);
}

QSizeF JKQTPVerticalAxis::getSize2(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode2==JKQTPCADMnone) return QSize(0,0);
    double ptwidth=axisStyle.axisLineOffset;
    const double arrowSize=((axisStyle.drawMode2&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))?(axisStyle.getArrowSize(painter, parent)/2.0):0.0;
    double labwidth=0;
    if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) ptwidth+=axisStyle.tickOutsideLength;
    if (axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)) {
        ptwidth+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labwidth+=getMaxTickLabelSize(painter).width();
    }
    if (axisStyle.drawMode2.testFlag(JKQTPCADMAxisLabel)) {
        ptwidth+=axisStyle.labelDistance;
        // find out size of axis label
        labwidth+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(qMax(arrowSize, parent->pt2px(painter, ptwidth)+labwidth), getParentPlotWidth()).expandedTo(getSize0(painter).second);
}


double JKQTPVerticalAxis::getParentOtheraxisOffset() const {
    return parent->x2p(parent->getXMin());
}

double JKQTPVerticalAxis::parentOtherAxisX2P(double x) const
{
    return parent->x2p(x);
}

double JKQTPVerticalAxis::getParentOtheraxisWidth() const {
    return fabs(parent->x2p(parent->getXMax())-parent->x2p(parent->getXMin()));
}

bool JKQTPVerticalAxis::getParentOtheraxisInverted() const
{
    return parent->getXAxis()->getInverted();
}

void JKQTPVerticalAxis::drawGrids(JKQTPEnhancedPainter& painter) {
    //qDebug()<<"  start JKQTPVerticalAxis::drawGrids(";
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen pg=painter.pen();
    pg.setColor(axisStyle.majorGridStyle.lineColor);
    pg.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.majorGridStyle.lineWidth*parent->getLineWidthMultiplier())));
    pg.setStyle(axisStyle.majorGridStyle.lineStyle);
    QPen pmg=painter.pen();
    pmg.setColor(axisStyle.minorGridStyle.lineColor);
    pmg.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH,parent->pt2px(painter,  axisStyle.minorGridStyle.lineWidth*parent->getLineWidthMultiplier())));
    pmg.setStyle(axisStyle.minorGridStyle.lineStyle);
    //double top=x2p(axismax);
    //double bottom=x2p(axismin);
    double left=0;
    double right=0;
    if (getParentOtheraxisInverted()) {
        left=getParentOtheraxisOffset()-getParentOtheraxisWidth();//;
        right=getParentOtheraxisOffset();//;
    } else {
        left=getParentOtheraxisOffset();//;
        right=getParentOtheraxisOffset()+getParentOtheraxisWidth();//;
    }

    double x=tickStart;
    QString label="";
    bool first=true;
    // loop through all labels, as they are at the major ticks, do not draw more than 200 labels (prevent hang ups)
    int cnt=0;
    QVector<QLineF> lines_pg;
    QVector<QLineF> lines_pmg;
    while (getNextLabel(x, label, first) && (cnt<200)) {
        double w=getNextLabelDistance(x);
        double mtdist=w/static_cast<double>(axisStyle.minorTicks+1);

        double xx=x2p(x);
        //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   axisStyle.minorTicks="<<minorTicks;

        //painter.setPen(pg);
        //qDebug()<<"      1";
        if (axisStyle.majorGridStyle.enabled && (x<axismax) && (x>axismin) && (fabs(left-right)>0)) {
            QLineF l(left, xx, right, xx);
            if (l.length()>0) lines_pg.append(l);//painter.drawLine(l);
        }
        //qDebug()<<"      2";
        //painter.setPen(pmg);
        //qDebug()<<"      3";
        if ((tickLabels.size()<=0) && axisStyle.minorGridStyle.enabled && (axisStyle.minorTicks>0) && (fabs(right-left)>0)) {
            //qDebug()<<"      3.1";
            double mx=x+mtdist;
            //qDebug()<<"      3.2";
            for (int i=0; i<static_cast<int>(axisStyle.minorTicks); i++) {
                //qDebug()<<"      3.2.1";
                double mxx=x2p(mx);
                //qDebug()<<"      3.2.2";
                if ((mx<axismax) && (mx>axismin) && (fabs(right-left)>0))  {
                    QLineF l(left, mxx, right, mxx);
                    if (l.length()>0) lines_pmg.append(l);//painter.drawLine(l);
                }
                //qDebug()<<"      3.2.3";
                mx=mx+mtdist;
                //qDebug()<<"      3.2.4";
            }
        }
        first=false;
        cnt++;
    }
    painter.setPen(pg);
    painter.drawLines(lines_pg);
    painter.setPen(pmg);
    painter.drawLines(lines_pmg);

    //qDebug()<<"    end JKQTPVerticalAxis::drawGrids(";
}

void JKQTPVerticalAxis::drawTickLabel1(JKQTPEnhancedPainter &painter, double xx, double yy, double labelOffset, const QString& label, double fontSize, bool isMinor)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.tickLabelColor);
    if (isMinor) getParentMathText()->setFontColor(axisStyle.minorTickLabelColor);

    getParentMathText()->parse(label);
    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    double lx=xx-parent->pt2px(painter, labelOffset)-width;
    if (axisStyle.tickLabelAngle==90) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(lx+width-1.25*strikeoutPos, yy-width/2.0);
        painter.rotate(axisStyle.tickLabelAngle);
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else if (axisStyle.tickLabelAngle==-90) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(lx+width-0.25*strikeoutPos, yy+width/2.0);
        painter.rotate(axisStyle.tickLabelAngle);
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else if (axisStyle.tickLabelAngle!=0) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        if (axisStyle.tickLabelAngle>0) {
            painter.translate(lx-strikeoutPos+(width)*(1.0-cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), yy+strikeoutPos-width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI));
            painter.rotate(axisStyle.tickLabelAngle);
        } else {
            painter.translate(lx+strikeoutPos+(width-strikeoutPos)*(1.0-cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), yy+strikeoutPos+width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI));
            painter.rotate(axisStyle.tickLabelAngle);
        }
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else {
        getParentMathText()->draw(painter, lx, yy+strikeoutPos, parent->getCurrentPlotterStyle().debugShowTextBoxes);
    }
}

void JKQTPVerticalAxis::drawTickLabel2(JKQTPEnhancedPainter &painter, double xx, double yy, double labelOffset, const QString &label, double fontSize, bool isMinor)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.tickLabelColor);
    if (isMinor) getParentMathText()->setFontColor(axisStyle.minorTickLabelColor);

    getParentMathText()->parse(label);
    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);

                                                                                            double lx=xx+parent->pt2px(painter, labelOffset);
    if (axisStyle.tickLabelAngle==90) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(lx+descent, yy-width/2.0);
        painter.rotate(axisStyle.tickLabelAngle);
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else if (axisStyle.tickLabelAngle==-90) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(lx+ascent, yy+width/2.0);
        painter.rotate(axisStyle.tickLabelAngle);
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else if (axisStyle.tickLabelAngle!=0) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        double shiftx=0;
        if (axisStyle.tickLabelAngle>0) {
            shiftx=strikeoutPos*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI);
        } else {

        }
        painter.translate(lx+shiftx, yy+strikeoutPos);
        painter.rotate(-axisStyle.tickLabelAngle);
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else {
        getParentMathText()->draw(painter, lx, yy+strikeoutPos, parent->getCurrentPlotterStyle().debugShowTextBoxes);
    }
}


void JKQTPVerticalAxis::drawAxisLabel1(JKQTPEnhancedPainter &painter, double left, double bottom, QSizeF labelMax, JKQTPCADrawMode drawMode) {
    double labelOffset=parent->pt2px(painter, axisStyle.labelDistance);
    if (drawMode.testFlag(JKQTPCADMTicks)) labelOffset+=parent->pt2px(painter, axisStyle.tickOutsideLength);
    if (drawMode.testFlag(JKQTPCADMTickLabels)) {
        labelOffset+=parent->pt2px(painter, axisStyle.tickLabelDistance);
        labelOffset+=labelMax.width();//+labelMax.height();
    }


    getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.labelColor);
    getParentMathText()->parse(axisLabel);
    const JKQTMathTextNodeSize labelsize=getParentMathText()->getSizeDetail(painter);


    QRectF rect(0,0, getParentPlotWidth(), labelsize.overallHeight);//plotBorderLeft-30);
    painter.save(); auto __finalpaintiner=JKQTPFinally([&painter]() {painter.restore();});
    painter.translate(QPointF(left-labelOffset-rect.height(), bottom));
    painter.rotate(-90);
    //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
    //painter.drawRect(rect);
    //painter.drawEllipse(-4, -4, 8, 8);
    switch(axisStyle.labelPosition) {
        case JKQTPLabelMax:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelMin:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelCenter:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
    }
    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("magenta");
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
        painter.setPen(p);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        painter.drawRect(rect);
    }
    painter.resetTransform();
}

void JKQTPVerticalAxis::drawAxisLabel2(JKQTPEnhancedPainter &painter, double right, double bottom, QSizeF labelMax, JKQTPCADrawMode drawMode) {
    double labelOffset=parent->pt2px(painter, axisStyle.labelDistance);
    if (drawMode.testFlag(JKQTPCADMTicks)) labelOffset+=parent->pt2px(painter, axisStyle.tickOutsideLength);
    if (drawMode.testFlag(JKQTPCADMTickLabels)) {
        labelOffset+=parent->pt2px(painter, axisStyle.tickLabelDistance);
        labelOffset+=labelMax.width();//+labelMax.height();
    }


    getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.labelColor);
    getParentMathText()->parse(axisLabel);
    const JKQTMathTextNodeSize labelsize=getParentMathText()->getSizeDetail(painter);


    QRectF rect(0,0, getParentPlotWidth(), labelsize.overallHeight);//plotBorderLeft-30);
    painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
    painter.translate(QPointF(right+labelOffset, bottom));
    painter.rotate(-90);

    //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
    //painter.drawRect(rect);
    //painter.drawEllipse(-4, -4, 8, 8);
    switch(axisStyle.labelPosition) {
        case JKQTPLabelMax:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelMin:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelCenter:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
    }
    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("magenta");
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
        painter.setPen(p);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        painter.drawRect(rect);
    }
    painter.resetTransform();
}

void JKQTPVerticalAxis::drawAxes(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPEnhancedPainter[%1]::drawAxes()").arg(objectName()));
#endif

    // make shure all parameters are calculated correctly
    calcPlotScaling();

    // determine pixel coordinates of important positions
    double top=x2p(axismax);
    double bottom=x2p(axismin);
    double left=0;
    double right=0;
    if (inverted) {
        qSwap(top, bottom);
    }
    if (getParentOtheraxisInverted()) {
        left=getParentOtheraxisOffset()-getParentOtheraxisWidth();//;
        right=getParentOtheraxisOffset();//;
    } else {
        left=getParentOtheraxisOffset();//;
        right=getParentOtheraxisOffset()+getParentOtheraxisWidth();//;
    }
    // move axes outside plot rectangle, if required
    left-=parent->pt2px(painter, axisStyle.axisLineOffset);
    right+=parent->pt2px(painter, axisStyle.axisLineOffset);


    double ticklabelOffset1PT=axisStyle.tickLabelDistance; // offset of tick labels from axis 1
    double minorticklabelOffset1PT=axisStyle.tickLabelDistance; // offset ofminor tick labels from axis 1
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) {
        ticklabelOffset1PT+=axisStyle.tickOutsideLength;
        minorticklabelOffset1PT+=axisStyle.minorTickOutsideLength;
    }
    double ticklabelOffset2PT=axisStyle.tickLabelDistance; // offset of tick labels from axis 2
    double minorticklabelOffset2PT=axisStyle.tickLabelDistance; // offset ofminor tick labels from axis 2
    if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) {
        ticklabelOffset2PT+=axisStyle.tickOutsideLength;
        minorticklabelOffset2PT+=axisStyle.minorTickOutsideLength;
    }
    double ticklabelOffset0PT=axisStyle.tickLabelDistance; // offset of tick labels from axis 0
    double minorticklabelOffset0PT=axisStyle.tickLabelDistance; // offset ofminor tick labels from axis 0
    if (axisStyle.drawMode0.testFlag(JKQTPCADMTicks)) {
        ticklabelOffset0PT+=axisStyle.tickOutsideLength;
        minorticklabelOffset0PT+=axisStyle.minorTickOutsideLength;
    }

    //qDebug()<<"JKQTPVerticalAxis:";
    //qDebug()<<"    left="<<left;
    //qDebug()<<"    right="<<right;
    //qDebug()<<"    top="<<top;
    //qDebug()<<"    bottom="<<bottom;
    //qDebug()<<"    scale="<<scale;
    //qDebug()<<"    offset="<<offset;
    //qDebug()<<"    x2p(0)="<<x2p(0);
    //qDebug()<<"    tickStart="<<tickStart;
    //qDebug()<<"    drawMode1="<<JKQTPCADrawMode2String(axisStyle.drawMode1);
    //qDebug()<<"    drawMode2="<<JKQTPCADrawMode2String(axisStyle.drawMode2);
    //qDebug()<<"    tickInsideLength="<<axisStyle.tickInsideLength;
    //qDebug()<<"    tickOutsideLength="<<axisStyle.tickOutsideLength;
    //qDebug()<<"    minorTickInsideLength="<<axisStyle.minorTickInsideLength;
    //qDebug()<<"    minorTickOutsideLength="<<axisStyle.minorTickOutsideLength;

    if (std::isnan(left) || std::isnan(right) || std::isnan(top) || std::isnan(bottom)) return;
    if (std::isinf(left) || std::isinf(right) || std::isinf(top) || std::isinf(bottom)) return;
    //qDebug()<<"  start JKQTPVerticalAxis::drawAxes(";
    //qDebug()<<"drawing vertical axis ... range = ["<<axismin<<" .. "<<axismax<<"]\n";

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    const QPen pmain=axisStyle.getAxisPen(painter, parent);
    const QPen ptick=axisStyle.getTickPen(painter, parent);
    const QPen pmtick=axisStyle.getMinorTickPen(painter, parent);

    getParentMathText()->setFontSize(this->axisStyle.tickLabelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);


    // plot thick axis at y==0
    if (axisStyle.showZeroAxis && (0>=axismin) && (0<=axismax)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): 0Axis").arg(objectName()));
        #endif
        const QPen pmain1=axisStyle.getZeroAxisPen(painter, parent);
        painter.setPen(pmain1);
        const QLineF l(left, x2p(0), right, x2p(0));
        if (l.length()>0) painter.drawLine(l);
    }

	// axis 0 properties
    bool  drawZeroAxis=false;
    const double zeroaxispos=getZeroAxisPos(&drawZeroAxis);

    painter.setPen(pmain);
    painter.setBrush(pmain.color());

    // draw thick axis lines, left and/or right with optional arrows, but wihtou ticks
    if ( axisStyle.drawMode1.testFlag(JKQTPCADMLine)) {
        const QLineF l(left, x2p(axismin), left, x2p(axismax));
        drawAxisLine(painter, l, axisStyle.drawMode1);
    }
    if (axisStyle.drawMode2.testFlag(JKQTPCADMLine)) {
        const QLineF l(right, x2p(axismin), right, x2p(axismax));
        drawAxisLine(painter, l, axisStyle.drawMode1);
    }
    if (axisStyle.drawMode0.testFlag(JKQTPCADMLine)) {
        const QLineF l(zeroaxispos, x2p(axismin), zeroaxispos, x2p(axismax));
        drawAxisLine(painter, l, axisStyle.drawMode0);
    }
    painter.setBrush(Qt::NoBrush);

    // plot minor and major ticks + tick labels
    const QSizeF labelMax=getMaxTickLabelSize(painter);
    double y=tickStart;
    QString label="";
    bool first=true;
    // loop through all labels, as they are at the major ticks
    int cnt=0;
    const double arrowFreeSpace=axisStyle.getArrowSize(painter, parent)*1.2;

    QVector<QLineF> lines_ptick, lines_ptick2, lines_ptick0;
    QVector<QLineF> lines_pmtick, lines_pmtick2, lines_pmtick0;

    {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaatii(QString("JKQTPEnhancedPainter[%1]::drawAxes(): calcLabels").arg(objectName()));
        #endif
        while (getNextLabel(y, label, first) && cnt<200) {
            const double mtdist=getNextLabelDistance(y)/static_cast<double>(axisStyle.minorTicks+1); // distance of minor ticks
            const double yy=x2p(y); // y tick position
            //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist;
            if (!(((axisStyle.drawMode1&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))||((axisStyle.drawMode2&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))||((axisStyle.drawMode0&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0)))
                    || (yy>top+arrowFreeSpace && yy<bottom-arrowFreeSpace)) {

                // calculate tick lines for axis 1
                if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)&&(y<=axismax) && (y>=axismin))  {
                    const QLineF l(left-parent->pt2px(painter, axisStyle.tickOutsideLength), yy, left+parent->pt2px(painter, axisStyle.tickInsideLength), yy);
                    if (l.length()>0) lines_ptick.append(l);
                }

                // calculate tick lines for axis 2
                if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks)&&(y<=axismax) && (y>=axismin))  {
                    const QLineF l(right+parent->pt2px(painter, axisStyle.tickOutsideLength), yy, right-parent->pt2px(painter, axisStyle.tickInsideLength), yy);
                    if (l.length()>0) lines_ptick2.append(l);
                }

                // calculate tick lines for axis 0
                if (axisStyle.drawMode0.testFlag(JKQTPCADMTicks)&&(y<=axismax) && (y>=axismin))  {
                    const QLineF l(zeroaxispos-parent->pt2px(painter, axisStyle.tickOutsideLength), yy, zeroaxispos+parent->pt2px(painter, axisStyle.tickInsideLength), yy);
                    if (l.length()>0) lines_ptick0.append(l);
                }

                // calculate minor ticks and draw minor tick labels
                if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)||axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode2.testFlag(JKQTPCADMTicks)||axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode0.testFlag(JKQTPCADMTicks)||axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels)) {
                    // minor ticks only if no user-defined tickLabels were set and if minor ticks are actiavted (i.e. minorTicks>0)
                    if ((tickLabels.size()<=0) && (axisStyle.minorTicks>0)) {
                        double my=y+mtdist; // location of first minor tick
                        int minTickCnt=axisStyle.minorTicks; // number of minor ticks
                        if (isLogAxis()) {
                            if (mtdist>y) {
                                my=mtdist;
                            } else {
                                minTickCnt--;
                                my=2.0*mtdist;
                            }

                        }
                        for (int i=0; i<minTickCnt; i++) {
                            const double my_pix=x2p(my);
                            //if (logAxis) qDebug()<<i<<"  =>  "<<mx<<mxx<<apprLogTicks<<w<<minorTicks;
                            if (my<=axismax && my>=axismin) {

                                // calculate minor tick lines, axis 1
                                if (axisStyle.minorTicks>0&&axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) {
                                    const QLineF l(left-parent->pt2px(painter, axisStyle.minorTickOutsideLength), my_pix, left+parent->pt2px(painter, axisStyle.minorTickInsideLength), my_pix);
                                    if (l.length()>0) lines_pmtick.append(l);
                                }
                                // calculate minor tick lines, axis 2
                                if (axisStyle.minorTicks>0&&axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) {
                                    const QLineF l(right+parent->pt2px(painter, axisStyle.minorTickOutsideLength), my_pix, right-parent->pt2px(painter, axisStyle.minorTickInsideLength), my_pix);
                                    if (l.length()>0) lines_pmtick.append(l);
                                }
                                // calculate minor tick lines, axis 0
                                if (axisStyle.minorTicks>0&&axisStyle.drawMode0.testFlag(JKQTPCADMTicks)) {
                                    const QLineF l(zeroaxispos-parent->pt2px(painter, axisStyle.minorTickOutsideLength), my_pix, zeroaxispos+parent->pt2px(painter, axisStyle.minorTickInsideLength), my_pix);
                                    if (l.length()>0) lines_pmtick.append(l);
                                }

                                // draw minor tick label, axis 1
                                if (axisStyle.minorTickLabelsEnabled&&axisStyle.minorTicks>0&&(axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels))) {
                                    double val= my/pow(logAxisBase,floor(log(my)/log(logAxisBase)));
                                    if (axisStyle.minorTickLabelFullNumber) val=my;
                                    const QString minorlabel=floattolabel(val);
                                    if (axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter, zeroaxispos, my_pix, minorticklabelOffset0PT, minorlabel, axisStyle.minorTickLabelFontSize, true);
                                    if (axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter, left, my_pix, minorticklabelOffset1PT, minorlabel, axisStyle.minorTickLabelFontSize, true);
                                    if (axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)) drawTickLabel2(painter, right, my_pix, minorticklabelOffset2PT, minorlabel, axisStyle.minorTickLabelFontSize, true);
                                }
                            }
                            my=my+mtdist;
                        }
                    }
                }

                if (!label.isEmpty() && (y<=axismax && y>=axismin)) {
                    if (axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter, zeroaxispos, yy, ticklabelOffset0PT, label, this->axisStyle.tickLabelFontSize, false);
                    if (axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter, left, yy, ticklabelOffset1PT, label, this->axisStyle.tickLabelFontSize, false);
                    if (axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)) drawTickLabel2(painter, right, yy, ticklabelOffset2PT, label, this->axisStyle.tickLabelFontSize, false);
                }
            }
            first=false;
            cnt++;
        }
    }
    {
    #ifdef JKQTBP_AUTOTIMER
        JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): drawLines").arg(objectName()));
    #endif
        if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) {
            painter.setPen(ptick);
            painter.drawLines(lines_ptick);
            painter.setPen(pmtick);
            painter.drawLines(lines_pmtick);
        }
        if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) {
            painter.setPen(ptick);
            painter.drawLines(lines_ptick2);
            painter.setPen(pmtick);
            painter.drawLines(lines_pmtick2);
        }
        if (axisStyle.drawMode0.testFlag(JKQTPCADMTicks)) {
            painter.setPen(ptick);
            painter.drawLines(lines_ptick0);
            painter.setPen(pmtick);
            painter.drawLines(lines_pmtick0);
        }
    }

    // plot axis label
    if (!axisLabel.isEmpty() && axisStyle.drawMode1.testFlag(JKQTPCADMAxisLabel)) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): axisLabel1").arg(objectName()));
#endif
        drawAxisLabel1(painter, left, bottom, labelMax, axisStyle.drawMode1);
    }
    if (!axisLabel.isEmpty() && axisStyle.drawMode0.testFlag(JKQTPCADMAxisLabel)) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): axisLabel0").arg(objectName()));
#endif
        drawAxisLabel1(painter, zeroaxispos, bottom, labelMax,axisStyle.drawMode0);
    }
    if (!axisLabel.isEmpty() && axisStyle.drawMode2.testFlag(JKQTPCADMAxisLabel)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): axisLabel2").arg(objectName()));
        #endif
        drawAxisLabel2(painter, right, bottom, labelMax,axisStyle.drawMode2);
    }


    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("cyan");
        p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth);
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        painter.setPen(p);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        QSizeF s1, s2;
        s1=getSize1(painter);
        s2=getSize2(painter);
        painter.drawRect(QRectF(QPointF(left-s1.width(), top), s1));
        painter.drawRect(QRectF(QPointF(right, top), s2));

    }

    //qDebug()<<"    end JKQTPVerticalAxis::drawAxes(";
}





JKQTPVerticalIndependentAxis::JKQTPVerticalIndependentAxis(double _axisOffset, double _axisWidth, double _otherAxisOffset, double _otherAxisWidth, JKQTBasePlotter* parent):
    JKQTPVerticalAxis(parent),
    axisOffset(_axisOffset),
    axisWidth(_axisWidth),
    otherAxisWidth(_otherAxisOffset),
    otherAxisOffset(_otherAxisWidth),
    otherAxisInverted(false)
{
    if (parent) {
        axisStyle=parent->getCurrentPlotterStyle().rightColorbarAxisStyle;
    } else {
        axisStyle=JKQTPGetSystemDefaultBaseStyle().rightColorbarAxisStyle;
    }

}

void JKQTPVerticalIndependentAxis::setAxisOffset(double __value) {
    this->axisOffset = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPVerticalIndependentAxis::setAxisWidth(double __value) {
    this->axisWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPVerticalIndependentAxis::setOtherAxisOffset(double __value) {
    this->otherAxisOffset = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPVerticalIndependentAxis::setOtherAxisWidth(double __value) {
    this->otherAxisWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPVerticalIndependentAxis::setOtherAxisInverted(bool __value) {
    this->otherAxisInverted = __value;
    this->paramsChanged=true;
    redrawPlot();
}

double JKQTPVerticalIndependentAxis::getParentPlotWidth() const {
    return axisWidth;
}

double JKQTPVerticalIndependentAxis::getParentPlotOffset() const {
    return axisOffset;
}

double JKQTPVerticalIndependentAxis::getParentOtheraxisWidth() const {
    return otherAxisWidth;
}

bool JKQTPVerticalIndependentAxis::getParentOtheraxisInverted() const {
    return otherAxisInverted;
}

double JKQTPVerticalIndependentAxis::getParentOtheraxisOffset() const {
    return otherAxisOffset;
}


double JKQTPVerticalIndependentAxis::parentOtherAxisX2P(double x) const
{
    return qQNaN();
}








JKQTPHorizontalAxis::JKQTPHorizontalAxis(JKQTBasePlotter* parent):
    JKQTPCoordinateAxis(parent)
{
    //axisPrefix="xaxis_";
    scaleSign=1;
    if (parent) {
        axisStyle=parent->getCurrentPlotterStyle().xAxisStyle;
    } else {
        axisStyle=JKQTPGetSystemDefaultBaseStyle().xAxisStyle;
    }

}

double JKQTPHorizontalAxis::getParentPlotWidth() const {
    return parent->getPlotWidth();
}

double JKQTPHorizontalAxis::getParentPlotOffset() const {
    return parent->getInternalPlotBorderLeft();
}

std::pair<QSizeF,QSizeF> JKQTPHorizontalAxis::getSize0(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode0==JKQTPCADMnone) return std::pair<QSizeF,QSizeF>(QSizeF(0,0),QSizeF(0,0));
    double ptheight=0;
    double ptheight_t=0;
    const double arrowSize=((axisStyle.drawMode1&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))?(axisStyle.getArrowSize(painter, parent)/2.0):0.0;
    double labheight=0;
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) { ptheight+=axisStyle.tickOutsideLength; ptheight_t+=axisStyle.tickOutsideLength; }
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)) {
        ptheight+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labheight+=getMaxTickLabelSize(painter).height();
    }
    if (axisStyle.drawMode1.testFlag(JKQTPCADMAxisLabel)) {
        ptheight+=axisStyle.labelDistance;
        // find out size of axis label
        labheight+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    const double zaxispos=getZeroAxisPos();
    const double left=x2p(axismin);
    const double left_offset=zaxispos-left;
    const double right=x2p(axismax);
    const double right_offset=right-zaxispos;

    return std::pair<QSizeF,QSizeF>(
                QSizeF(getParentPlotWidth(), qMax(0.0,qMax(arrowSize,parent->pt2px(painter, ptheight)+labheight)-left_offset)),
                QSizeF(getParentPlotWidth(), qMax(0.0,qMax(arrowSize,parent->pt2px(painter, ptheight_t))-right_offset))
                );
}

QSizeF JKQTPHorizontalAxis::getSize1(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode1==JKQTPCADMnone) return QSize(0,0);
    double ptheight=axisStyle.axisLineOffset;
    const double arrowSize=((axisStyle.drawMode1&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))?(axisStyle.getArrowSize(painter, parent)/2.0):0.0;
    double labheight=0;
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) ptheight+=axisStyle.tickOutsideLength;
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)) {
        ptheight+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labheight+=getMaxTickLabelSize(painter).height();
    }
    if (axisStyle.drawMode1.testFlag(JKQTPCADMAxisLabel)) {
        ptheight+=axisStyle.labelDistance;
        // find out size of axis label
        labheight+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(getParentPlotWidth(), qMax(arrowSize,parent->pt2px(painter, ptheight)+labheight)).expandedTo(getSize0(painter).first);
}

QSizeF JKQTPHorizontalAxis::getSize2(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode2==JKQTPCADMnone) return QSize(0,0);
    double ptheight=axisStyle.axisLineOffset;
    const double arrowSize=((axisStyle.drawMode2&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))?(axisStyle.getArrowSize(painter, parent)/2.0):0.0;
    double labheight=0;
    if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) ptheight+=axisStyle.tickOutsideLength;
    if (axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)) {
        ptheight+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labheight+=getMaxTickLabelSize(painter).height();
    }
    if (axisStyle.drawMode2.testFlag(JKQTPCADMAxisLabel)) {
        ptheight+=axisStyle.labelDistance;
        // find out size of axis label
        labheight+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(getParentPlotWidth(), qMax(arrowSize,parent->pt2px(painter, ptheight)+labheight)).expandedTo(getSize0(painter).second);
}


double JKQTPHorizontalAxis::getParentOtheraxisOffset() const {
    return parent->y2p(parent->getYMax());
}

double JKQTPHorizontalAxis::parentOtherAxisX2P(double x) const
{
    return parent->y2p(x);
}
double JKQTPHorizontalAxis::getParentOtheraxisWidth() const {
    return fabs(parent->y2p(parent->getYMax())-parent->y2p(parent->getYMin()));
}

bool JKQTPHorizontalAxis::getParentOtheraxisInverted() const
{
    return parent->getYAxis()->getInverted();
}

void JKQTPHorizontalAxis::drawGrids(JKQTPEnhancedPainter& painter) {
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    //double right=x2p(axismax);
    //double left=x2p(axismin);
    double bottom=0;
    double top=0;
    if (getParentOtheraxisInverted()) {
        top=getParentOtheraxisOffset()-getParentOtheraxisWidth();//;
        bottom=getParentOtheraxisOffset();//;
    } else {
        top=getParentOtheraxisOffset();//;
        bottom=getParentOtheraxisOffset()+getParentOtheraxisWidth();//;
    }
    QPen pg=painter.pen();
    pg.setColor(axisStyle.majorGridStyle.lineColor);
    pg.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.majorGridStyle.lineWidth*parent->getFontSizeMultiplier())));
    pg.setStyle(axisStyle.majorGridStyle.lineStyle);
    QPen pmg=painter.pen();
    pmg.setColor(axisStyle.minorGridStyle.lineColor);
    pmg.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.minorGridStyle.lineWidth*parent->getLineWidthMultiplier())));
    pmg.setStyle(axisStyle.minorGridStyle.lineStyle);

    double x=tickStart;
    QString label="";
    bool first=true;
    // loop through all labels, as they are at the major ticks
    int cnt=0;
    QVector<QLineF> lines_pg;
    QVector<QLineF> lines_pmg;

    while (getNextLabel(x, label, first) && cnt<200) {
        double mtdist=getNextLabelDistance(x)/static_cast<double>(axisStyle.minorTicks+1);

        double xx=x2p(x);
        //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   axisStyle.minorTicks="<<minorTicks;

        //painter.setPen(pg);
        if (axisStyle.majorGridStyle.enabled && (x<axismax) && (x>axismin))  {
            QLineF l(xx, bottom, xx, top);
            if (l.length()>0) lines_pg.append(l);//painter.drawLine(l);
        }
        //painter.setPen(pmg);
        if ((tickLabels.size()<=0) && axisStyle.minorGridStyle.enabled && (axisStyle.minorTicks>0)) {
            double mx=x+mtdist;
            for (int i=0; i<static_cast<int>(axisStyle.minorTicks); i++) {
                double mxx=x2p(mx);
                if ((mx<axismax) && (mx>axismin))  {
                    QLineF l( mxx, bottom, mxx, top);
                    if (l.length()>0) lines_pmg.append(l);//painter.drawLine(l);
                }
                mx=mx+mtdist;
            }
        }
        first=false;
        cnt++;
    }
    painter.setPen(pg);
    painter.drawLines(lines_pg);
    painter.setPen(pmg);
    painter.drawLines(lines_pmg);

}


void JKQTPHorizontalAxis::drawTickLabel1(JKQTPEnhancedPainter &painter, double xx, double yy, double labelOffset, const QString &label, double fontSize, double ascentMax, double /*descentMax*/, bool isMinor)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.tickLabelColor);
    if (isMinor) getParentMathText()->setFontColor(axisStyle.minorTickLabelColor);

    getParentMathText()->parse(label);

    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    const double ly0=yy+parent->pt2px(painter, labelOffset);
    const double ly=ly0+ascentMax;
    if (axisStyle.tickLabelAngle!=0) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        if (axisStyle.tickLabelAngle>0) {
            painter.translate(xx-fabs(ascent*sin(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)/2.0), ly0+fabs(ascent*cos(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)));
            painter.rotate(axisStyle.tickLabelAngle);
        } else {
            painter.translate(xx+fabs(ascent*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)/2.0)-width*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), ly0+ascent*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI))+fabs(width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)));
            painter.rotate(axisStyle.tickLabelAngle);
        }
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else {
        getParentMathText()->draw(painter, xx-width/2.0, ly, parent->getCurrentPlotterStyle().debugShowTextBoxes);
    }
}

void JKQTPHorizontalAxis::drawTickLabel2(JKQTPEnhancedPainter &painter, double xx, double yy, double labelOffset, const QString &label, double fontSize, double /*ascentMax*/, double descentMax, bool isMinor)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.tickLabelColor);
    if (isMinor) getParentMathText()->setFontColor(axisStyle.minorTickLabelColor);

    getParentMathText()->parse(label);
    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    const double ly0=yy-parent->pt2px(painter, labelOffset);
    const double ly=ly0-descentMax;
    if (axisStyle.tickLabelAngle!=0) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        if (axisStyle.tickLabelAngle>0) {
            painter.translate(xx+fabs(descent*sin(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)/2.0), ly0-fabs(descent*cos(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)));
            painter.rotate(-axisStyle.tickLabelAngle);
        } else {
            painter.translate(xx-fabs(descent*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)/2.0)-width*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), ly0-descent*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI))-fabs(width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)));
            painter.rotate(-axisStyle.tickLabelAngle);
        }
        getParentMathText()->draw(painter, 0,0, parent->getCurrentPlotterStyle().debugShowTextBoxes);

    } else {
        getParentMathText()->draw(painter, xx-width/2.0, ly, parent->getCurrentPlotterStyle().debugShowTextBoxes);
    }
}


void JKQTPHorizontalAxis::drawAxisLabel1(JKQTPEnhancedPainter &painter, double left, double bottom, QSizeF labelMax, JKQTPCADrawMode drawMode) {
    double labelOffset=parent->pt2px(painter, axisStyle.labelDistance);
    if (drawMode.testFlag(JKQTPCADMTicks)) labelOffset+=parent->pt2px(painter, axisStyle.tickOutsideLength);
    if (drawMode.testFlag(JKQTPCADMTickLabels)) {
        labelOffset+=parent->pt2px(painter, axisStyle.tickLabelDistance);
        labelOffset+=labelMax.height();//+labelMax.height();
    }


    getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.labelColor);
    getParentMathText()->parse(axisLabel);
    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);


    QRectF rect(0,0, getParentPlotWidth(), ascent+descent);//plotBorderLeft-30);
    painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
    painter.translate(QPointF(left, bottom+labelOffset));
    //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
    //painter.drawRect(rect);
    //painter.drawEllipse(-4, -4, 8, 8);
    switch(axisStyle.labelPosition) {
        case JKQTPLabelMax:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelMin:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelCenter:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
    }
    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("magenta");
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
        painter.setPen(p);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        painter.drawRect(rect);

    }
}

void JKQTPHorizontalAxis::drawAxisLabel2(JKQTPEnhancedPainter &painter, double left, double top, QSizeF labelMax, JKQTPCADrawMode drawMode) {
    double labelOffset=parent->pt2px(painter, axisStyle.labelDistance);
    if (drawMode.testFlag(JKQTPCADMTicks)) labelOffset+=parent->pt2px(painter, axisStyle.tickOutsideLength);
    if (drawMode.testFlag(JKQTPCADMTickLabels)) {
        labelOffset+=parent->pt2px(painter, axisStyle.tickLabelDistance);
        labelOffset+=labelMax.width();//+labelMax.height();
    }


    getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.labelColor);
    getParentMathText()->parse(axisLabel);


    QRectF rect(0,0, getParentPlotWidth(), getParentMathText()->getSize(painter).height());//plotBorderLeft-30);
    painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
    painter.translate(QPointF(left, top-labelOffset-rect.height()));
    //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
    //painter.drawRect(rect);
    //painter.drawEllipse(-4, -4, 8, 8);
    switch(axisStyle.labelPosition) {
        case JKQTPLabelMax:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelMin:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
        case JKQTPLabelCenter:
            getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
            break;
    }
    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("magenta");
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
        painter.setPen(p);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        painter.drawRect(rect);

    }
    //painter.resetTransform();
}

void JKQTPHorizontalAxis::drawAxes(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPHorizontalAxis[%1]::drawAxes()").arg(objectName()));
#endif

    // make shure all parameters are calculated correctly
    calcPlotScaling();

    // determine pixel coordinates of important positions
    double right=x2p(axismax);
    double left=x2p(axismin);
    double bottom=0;
    double top=0;
    if (inverted) {
        qSwap(left, right);
    }
    if (getParentOtheraxisInverted()) {
        top=getParentOtheraxisOffset()-getParentOtheraxisWidth();//;
        bottom=getParentOtheraxisOffset();//;
    } else {
        top=getParentOtheraxisOffset();//;
        bottom=getParentOtheraxisOffset()+getParentOtheraxisWidth();//;
    }
    // move axes outside plot rectangle, if required
    top-=parent->pt2px(painter, axisStyle.axisLineOffset);
    bottom+=parent->pt2px(painter, axisStyle.axisLineOffset);

    double ticklabelOffset1PT=axisStyle.tickLabelDistance; // offset of tick labels from axis 1
    double minorticklabelOffset1PT=axisStyle.tickLabelDistance; // offset ofminor tick labels from axis 1
    if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) {
        ticklabelOffset1PT+=axisStyle.tickOutsideLength;
        minorticklabelOffset1PT+=axisStyle.minorTickOutsideLength;
    }
    double ticklabelOffset2PT=axisStyle.tickLabelDistance; // offset of tick labels from axis 2
    double minorticklabelOffset2PT=axisStyle.tickLabelDistance; // offset ofminor tick labels from axis 2
    if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) {
        ticklabelOffset2PT+=axisStyle.tickOutsideLength;
        minorticklabelOffset2PT+=axisStyle.minorTickOutsideLength;
    }
    double ticklabelOffset0PT=axisStyle.tickLabelDistance; // offset of tick labels from axis 0
    double minorticklabelOffset0PT=axisStyle.tickLabelDistance; // offset ofminor tick labels from axis 0
    if (axisStyle.drawMode0.testFlag(JKQTPCADMTicks)) {
        ticklabelOffset0PT+=axisStyle.tickOutsideLength;
        minorticklabelOffset0PT+=axisStyle.minorTickOutsideLength;
    }

    //qDebug()<<"    left="<<left<<"\n";
    //qDebug()<<"    right="<<right<<"\n";
    //qDebug()<<"    top="<<top<<"\n";
    //qDebug()<<"    bottom="<<bottom<<"\n";
    //qDebug()<<"    scale="<<scale<<"\n";
    //qDebug()<<"    offset="<<offset<<"\n";
    //qDebug()<<"    x2p(0)="<<x2p(0);
    //qDebug()<<"    tickStart="<<tickStart<<"\n";

    if (std::isnan(left) || std::isnan(right) || std::isnan(top) || std::isnan(bottom)) return;
    if (std::isinf(left) || std::isinf(right) || std::isinf(top) || std::isinf(bottom)) return;
    //qDebug()<<"drawing vertical axis ... range = ["<<axismin<<" .. "<<axismax<<"]\n";

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

    const QPen pmain=axisStyle.getAxisPen(painter, parent);
    const QPen ptick=axisStyle.getTickPen(painter, parent);
    const QPen pmtick=axisStyle.getMinorTickPen(painter, parent);

    getParentMathText()->setFontSize(this->axisStyle.tickLabelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);


    // plot thick axis at y==0
    if (axisStyle.showZeroAxis && (0>axismin) && (0<axismax)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaat(QString("JKQTPHorizontalAxis[%1]::drawAxes(): 0Axis").arg(objectName()));
        #endif
            const QPen pmain1=axisStyle.getZeroAxisPen(painter, parent);
            painter.setPen(pmain1);
        const QLineF l(x2p(0), bottom, x2p(0), top);
        if (l.length()>0) painter.drawLine(l);
    }

    painter.setPen(pmain);
    painter.setBrush(pmain.color());

    // axis 0 properties
    bool  drawZeroAxis=false;
    const double zeroaxispos=getZeroAxisPos(&drawZeroAxis);


    // draw thick axis lines, left and/or right
    if (axisStyle.drawMode1.testFlag(JKQTPCADMLine)) {
        const QLineF l(x2p(axismin), bottom, x2p(axismax), bottom);
        drawAxisLine(painter, l, axisStyle.drawMode1);
    }
    if (axisStyle.drawMode2.testFlag(JKQTPCADMLine)) {
        const QLineF l(x2p(axismin), top, x2p(axismax), top);
        drawAxisLine(painter, l, axisStyle.drawMode2);
    }
    if (axisStyle.drawMode0.testFlag(JKQTPCADMLine)) {
        const QLineF l(x2p(axismin), zeroaxispos, x2p(axismax), zeroaxispos);
        drawAxisLine(painter, l, axisStyle.drawMode0);
    }
    painter.setBrush(Qt::NoBrush);

    // plot minor and major ticks + tick labels
    double ascentMax, descentMax;
    QSizeF labelMax=getMaxTickLabelSize(painter, &ascentMax, &descentMax);
    double x=tickStart;
    QString label="";
    bool first=true;
    // loop through all labels, as they are at the major ticks
    int cnt=0;
    QVector<QLineF> lines_ptick, lines_ptick2, lines_ptick0;
    QVector<QLineF> lines_pmtick, lines_pmtick2, lines_pmtick0;

    const double arrowFreeSpace=axisStyle.getArrowSize(painter, parent)*1.2;

    {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): calcLabels").arg(objectName()));
        #endif

        while (getNextLabel(x, label, first) && cnt<200) {
            const double mtdist=getNextLabelDistance(x)/static_cast<double>(axisStyle.minorTicks+1);

            const double xx=x2p(x);
            //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   axisStyle.minorTicks="<<minorTicks;

            if (!(((axisStyle.drawMode1&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))||((axisStyle.drawMode2&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0))||((axisStyle.drawMode0&(JKQTPCADMMinArrow|JKQTPCADMMinFilledArrow|JKQTPCADMMaxArrow|JKQTPCADMMaxFilledArrow))!=int(0)))
                    || (xx>left+arrowFreeSpace && xx<right-arrowFreeSpace))
            {

                if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks) && x<=axismax && x>=axismin)  {
                    const QLineF l(xx, bottom-parent->pt2px(painter, axisStyle.tickInsideLength), xx, bottom+parent->pt2px(painter, axisStyle.tickOutsideLength));
                    if (l.length()>0) lines_ptick.append(l);//painter.drawLine(l);
                }
                if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks) && x<=axismax && x>=axismin)  {
                    const QLineF l(xx, top+parent->pt2px(painter, axisStyle.tickInsideLength), xx, top-parent->pt2px(painter, axisStyle.tickOutsideLength));
                    if (l.length()>0) lines_ptick2.append(l);//painter.drawLine(l);
                }
                if (axisStyle.drawMode0.testFlag(JKQTPCADMTicks) && x<=axismax && x>=axismin)  {
                    const QLineF l(xx, zeroaxispos-parent->pt2px(painter, axisStyle.tickInsideLength), xx, zeroaxispos+parent->pt2px(painter, axisStyle.tickOutsideLength));
                    if (l.length()>0) lines_ptick0.append(l);//painter.drawLine(l);
                }

                //painter.setPen(pmtick);
                if ((tickLabels.size()<=0) && (axisStyle.minorTicks>0) && (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)||axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode2.testFlag(JKQTPCADMTicks)||axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode0.testFlag(JKQTPCADMTicks)||axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels))) {
                    double mx=x+mtdist;
                    int minTickCnt=axisStyle.minorTicks;
                    if (isLogAxis()) {
                        if (mtdist>x) {
                            mx=mtdist;
                        } else {
                            minTickCnt--;
                            mx=2.0*mtdist;
                        }

                    }
                    for (int i=0; i<minTickCnt; i++) {
                        double mx_pix=x2p(mx);

                        if (mx<=axismax && mx>=axismin)  {
                            // calculate minor tick lines, axis 1
                            if (axisStyle.minorTicks>0&&axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) {
                                const QLineF l( mx_pix, bottom-parent->pt2px(painter, axisStyle.minorTickInsideLength), mx_pix, bottom+parent->pt2px(painter, axisStyle.minorTickOutsideLength));
                                if (l.length()>0) lines_pmtick.append(l);
                            }
                            // calculate minor tick lines, axis 0
                            if (axisStyle.minorTicks>0&&axisStyle.drawMode0.testFlag(JKQTPCADMTicks)) {
                                const QLineF l( mx_pix, zeroaxispos-parent->pt2px(painter, axisStyle.minorTickInsideLength), mx_pix, zeroaxispos+parent->pt2px(painter, axisStyle.minorTickOutsideLength));
                                if (l.length()>0) lines_pmtick.append(l);
                            }
                            // calculate minor tick lines, axis 2
                            if (axisStyle.minorTicks>0&&axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) {
                                const QLineF l( mx_pix, top+parent->pt2px(painter, axisStyle.minorTickInsideLength), mx_pix, top-parent->pt2px(painter, axisStyle.minorTickOutsideLength));
                                if (l.length()>0) lines_pmtick.append(l);
                            }

                            // draw minor tick label, axis 1
                            if (axisStyle.minorTickLabelsEnabled&&axisStyle.minorTicks>0&&(axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)||axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels))) {
                                double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));
                                if (axisStyle.minorTickLabelFullNumber) val=mx;
                                QString minorlabel=floattolabel(val);
                                if (axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter,  mx_pix, bottom, minorticklabelOffset1PT, minorlabel, axisStyle.minorTickLabelFontSize, ascentMax, descentMax, true);
                                if (axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter,  mx_pix, zeroaxispos, minorticklabelOffset0PT, minorlabel, axisStyle.minorTickLabelFontSize, ascentMax, descentMax, true);
                                if (axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)) drawTickLabel2(painter,  mx_pix, top, minorticklabelOffset2PT, minorlabel, axisStyle.minorTickLabelFontSize, ascentMax, descentMax, true);
                            }
                        }
                        mx=mx+mtdist;
                    }
                }

                if (!label.isEmpty() && (x<=axismax && x>=axismin)) {
                    if (axisStyle.drawMode1.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter, xx, bottom, ticklabelOffset1PT, label, this->axisStyle.tickLabelFontSize, ascentMax, descentMax, false);
                    if (axisStyle.drawMode0.testFlag(JKQTPCADMTickLabels)) drawTickLabel1(painter, xx, zeroaxispos, ticklabelOffset0PT, label, this->axisStyle.tickLabelFontSize, ascentMax, descentMax, false);
                    if (axisStyle.drawMode2.testFlag(JKQTPCADMTickLabels)) drawTickLabel2(painter, xx, top, ticklabelOffset2PT, label, this->axisStyle.tickLabelFontSize, ascentMax, descentMax, false);

                }
            }

            first=false;
            cnt++;
        }
    }
    {
    #ifdef JKQTBP_AUTOTIMER
        JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): drawLines").arg(objectName()));
    #endif
        if (axisStyle.drawMode1.testFlag(JKQTPCADMTicks)) {
            painter.setPen(ptick);
            painter.drawLines(lines_ptick);
            painter.setPen(pmtick);
            painter.drawLines(lines_pmtick);
        }
        if (axisStyle.drawMode2.testFlag(JKQTPCADMTicks)) {
            painter.setPen(ptick);
            painter.drawLines(lines_ptick2);
            painter.setPen(pmtick);
            painter.drawLines(lines_pmtick2);
        }
        if (axisStyle.drawMode0.testFlag(JKQTPCADMTicks)) {
            painter.setPen(ptick);
            painter.drawLines(lines_ptick0);
            painter.setPen(pmtick);
            painter.drawLines(lines_pmtick0);
        }
    }

    // plot axis label
    if (!axisLabel.isEmpty() && axisStyle.drawMode1.testFlag(JKQTPCADMAxisLabel)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): axisLabel1").arg(objectName()));
        #endif
        drawAxisLabel1(painter, left, bottom, labelMax, axisStyle.drawMode1);

    }
    if (!axisLabel.isEmpty() && axisStyle.drawMode0.testFlag(JKQTPCADMAxisLabel)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): axisLabel0").arg(objectName()));
        #endif
        drawAxisLabel1(painter, left, zeroaxispos, labelMax, axisStyle.drawMode0);

    }
    if (!axisLabel.isEmpty() && axisStyle.drawMode2.testFlag(JKQTPCADMAxisLabel)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): axisLabel2").arg(objectName()));
        #endif
        drawAxisLabel2(painter, left, top, labelMax, axisStyle.drawMode2);
    }

    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("cyan");
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth);
        painter.setPen(p);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        QSizeF s1,s2;
        s1=getSize1(painter);
        s2=getSize2(painter);
        painter.drawRect(QRectF(QPointF(left, bottom), s1));
        painter.drawRect(QRectF(QPointF(left, top-s2.height()), s2));

    }
}








JKQTPHorizontalIndependentAxis::JKQTPHorizontalIndependentAxis(double _axisOffset, double _axisWidth, double _otherAxisOffset, double _otherAxisWidth, JKQTBasePlotter* parent):
    JKQTPHorizontalAxis(parent),
    axisOffset(_axisOffset),
    axisWidth(_axisWidth),
    otherAxisWidth(_otherAxisOffset),
    otherAxisOffset(_otherAxisWidth),
    otherAxisInverted(false)
{
    if (parent) {
        axisStyle=parent->getCurrentPlotterStyle().topColorbarAxisStyle;
    } else {
        axisStyle=JKQTPGetSystemDefaultBaseStyle().topColorbarAxisStyle;
    }

}

void JKQTPHorizontalIndependentAxis::setAxisOffset(double __value) {
    this->axisOffset = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPHorizontalIndependentAxis::setAxisWidth(double __value) {
    this->axisWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPHorizontalIndependentAxis::setOtherAxisOffset(double __value) {
    this->otherAxisOffset = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPHorizontalIndependentAxis::setOtherAxisWidth(double __value) {
    this->otherAxisWidth = __value;
    this->paramsChanged=true;
    redrawPlot();
}

void JKQTPHorizontalIndependentAxis::setOtherAxisInverted(bool __value) {
    this->otherAxisInverted = __value;
    this->paramsChanged=true;
    redrawPlot();
}

double JKQTPHorizontalIndependentAxis::getParentPlotWidth() const {
    return axisWidth;
}

double JKQTPHorizontalIndependentAxis::getParentPlotOffset() const {
    return axisOffset;
}

double JKQTPHorizontalIndependentAxis::getParentOtheraxisWidth() const {
    return otherAxisWidth;
}

bool JKQTPHorizontalIndependentAxis::getParentOtheraxisInverted() const {
    return otherAxisInverted;
}

double JKQTPHorizontalIndependentAxis::getParentOtheraxisOffset() const {
    return otherAxisOffset;
}

double JKQTPHorizontalIndependentAxis::parentOtherAxisX2P(double x) const
{
    return qQNaN();
}
