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

#include "jkqtplotter/jkqtpcoordinateaxes.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtcommon/jkqtpgeometrytools.h"
#include <QDebug>
#include <QDateTime>
#include <cfloat>
#include <QApplication>


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
    if (axisStyle.labelType==JKQTPCALTdate) {
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
    } else if (axisStyle.labelType==JKQTPCALTdatetime) {
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
    } else if (axisStyle.labelType==JKQTPCALTtime) {
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


QString JKQTPCoordinateAxis::floattolabel(double data) const {
    int past_comma=axisStyle.labelDigits;
    const bool remove_trail0=true;
    QLocale loc=QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);

    double belowIsZero=1e-300;
    if (!getLogAxis()) {
        belowIsZero=fabs(getMax()-getMin())*1e-6;
    }

    switch(axisStyle.labelType) {
        case JKQTPCALTdefault: {
                QString res=loc.toString(data, 'f', past_comma);
                if (remove_trail0 && res.contains(QLocale::system().decimalPoint())) {
                    while (res.size()>0 && res[res.size()-1]=='0') {
                        res=res.left(res.size()-1);
                    }
                    if (res.size()>0 && res[res.size()-1]==loc.decimalPoint()) res=res.left(res.size()-1);
                }
                return res;
            }; break;
        case JKQTPCALTexponent: {
                return QString(jkqtp_floattolatexstr(data, past_comma, remove_trail0, belowIsZero, pow(10, -past_comma), pow(10, past_comma+1)).c_str());
            }; break;
        case JKQTPCALTexponentCharacter: {
                return QString(jkqtp_floattounitstr(data, past_comma, remove_trail0).c_str());
            }; break;
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
        default:
            return QString();
            break;
    }

    /*
    axisStyle.tickTimeFormat
    axisStyle.tickDateFormat
    axisStyle.tickDateTimeFormat
     **/

    return QString();
}

QString JKQTPCoordinateAxis::floattolabel(double data, int past_comma) const {
    bool remove_trail0=true;
    QLocale loc=QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    if (axisStyle.labelType==JKQTPCALTdefault) {
        QString res=loc.toString(data, 'f', past_comma);//QString::number(data, 'f', past_comma);
        if (remove_trail0 && res.contains(QLocale::system().decimalPoint())) {
            while (res.size()>0 && res[res.size()-1]=='0') {
                res=res.left(res.size()-1);
            }
            if (res.size()>0 && res[res.size()-1]==loc.decimalPoint()) res=res.left(res.size()-1);
        }
        return res;
    } else if (axisStyle.labelType==JKQTPCALTexponent) return QString(jkqtp_floattolatexstr(data, past_comma, remove_trail0, 1e-300, pow(10, -past_comma), pow(10, past_comma+1)).c_str());
    else if (axisStyle.labelType==JKQTPCALTexponentCharacter) return QString(jkqtp_floattounitstr(data, past_comma, remove_trail0).c_str());
    return "";
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
            tickSpacing=1;
            tickSpacingLog=calcLogTickSpacing();
            tickStart=pow(logAxisBase, floor(log(axismin)/log(logAxisBase)));
        } else {
            if (autoAxisSpacing) {
                // autoscaling linear x-axis
                tickSpacingLog=10;
                tickSpacing=calcLinearTickSpacing();
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
            tickSpacingLog=10;
            tickSpacing=calcLinearTickSpacing();
        } else {
            // predefined scaling for linear x-axis
            tickSpacing=userTickSpacing;
            tickSpacingLog=userLogTickSpacing;
        }
        tickStart=floor(axismin/(tickSpacing))*tickSpacing;
    } else if (axisStyle.tickMode==JKQTPLTMPower) {
        tickSpacing=1;
        tickSpacingLog=calcLogTickSpacing();
        tickStart=pow(logAxisBase, floor(log(axismin)/log(logAxisBase)));
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

void JKQTPCoordinateAxis::setLabelType(JKQTPCALabelType __value) {
    this->axisStyle.labelType = __value;
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

void JKQTPCoordinateAxis::setMinorGridColor(const QColor& __value) {
    this->axisStyle.minorGridStyle.lineColor = __value;
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
    f.setFamily(getParent()->getCurrentPlotterStyle().defaultFontName);
    f.setPointSizeF(this->axisStyle.tickLabelFontSize*parent->getFontSizeMultiplier());
    QFontMetricsF fm(f);
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

QSizeF JKQTPVerticalAxis::getSize1(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode1==JKQTPCADMnone) return QSize(0,0);
    double ptwidth=axisStyle.axisLineOffset;
    double labwidth=0;
    if (JKQTPCADrawModeHasTicks(axisStyle.drawMode1)) ptwidth+=axisStyle.tickOutsideLength;
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
        ptwidth+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labwidth+=getMaxTickLabelSize(painter).width();
    }
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
        ptwidth+=axisStyle.labelDistance;
        // find out size of axis label
        labwidth+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(parent->pt2px(painter, ptwidth)+labwidth, getParentPlotWidth());
}

QSizeF JKQTPVerticalAxis::getSize2(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode2==JKQTPCADMnone) return QSize(0,0);
    double ptwidth=axisStyle.axisLineOffset;
    double labwidth=0;
    if (JKQTPCADrawModeHasTicks(axisStyle.drawMode2)) ptwidth+=axisStyle.tickOutsideLength;
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
        ptwidth+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labwidth+=getMaxTickLabelSize(painter).width();
    }
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
        ptwidth+=axisStyle.labelDistance;
        // find out size of axis label
        labwidth+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(parent->pt2px(painter, ptwidth)+labwidth, getParentPlotWidth());
}


double JKQTPVerticalAxis::getParentOtheraxisOffset() const {
    return parent->x2p(parent->getXMin());
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

void JKQTPVerticalAxis::drawTickLabel1(JKQTPEnhancedPainter &painter, double xx, double yy, const QString& label, double fontSize)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);

    getParentMathText()->parse(label);
    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
        double lx=xx-parent->pt2px(painter, axisStyle.minorTickOutsideLength+axisStyle.tickLabelDistance)-width;
        if (axisStyle.tickLabelAngle==90) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(lx+width-1.25*strikeoutPos, yy-width/2.0);
            painter.rotate(axisStyle.tickLabelAngle);
            getParentMathText()->draw(painter, 0,0);

        } else if (axisStyle.tickLabelAngle==-90) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(lx+width-0.25*strikeoutPos, yy+width/2.0);
            painter.rotate(axisStyle.tickLabelAngle);
            getParentMathText()->draw(painter, 0,0);

        } else if (axisStyle.tickLabelAngle!=0) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            if (axisStyle.tickLabelAngle>0) {
                painter.translate(lx-strikeoutPos+(width)*(1.0-cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), yy+strikeoutPos-width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI));
                painter.rotate(axisStyle.tickLabelAngle);
            } else {
                painter.translate(lx+strikeoutPos+(width-strikeoutPos)*(1.0-cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), yy+strikeoutPos+width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI));
                painter.rotate(axisStyle.tickLabelAngle);
            }
            getParentMathText()->draw(painter, 0,0);

        } else {
            getParentMathText()->draw(painter, lx, yy+strikeoutPos);
        }
    }
}

void JKQTPVerticalAxis::drawTickLabel2(JKQTPEnhancedPainter &painter, double xx, double yy, const QString &label, double fontSize)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);

    getParentMathText()->parse(label);
    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);

    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
        double lx=xx+parent->pt2px(painter, axisStyle.tickOutsideLength+axisStyle.tickLabelDistance);
        if (axisStyle.tickLabelAngle==90) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(lx+descent, yy-width/2.0);
            painter.rotate(axisStyle.tickLabelAngle);
            getParentMathText()->draw(painter, 0,0);

        } else if (axisStyle.tickLabelAngle==-90) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(lx+ascent, yy+width/2.0);
            painter.rotate(axisStyle.tickLabelAngle);
            getParentMathText()->draw(painter, 0,0);

        } else if (axisStyle.tickLabelAngle!=0) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            double shiftx=0;
            if (axisStyle.tickLabelAngle>0) {
                shiftx=strikeoutPos*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI);
            } else {

            }
            painter.translate(lx+shiftx, yy+strikeoutPos);
            painter.rotate(-axisStyle.tickLabelAngle);
            getParentMathText()->draw(painter, 0,0);

        } else {
            getParentMathText()->draw(painter, lx, yy+strikeoutPos);
        }
    }
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
    //qDebug()<<"JKQTPVerticalAxis:";
    //qDebug()<<"    left="<<left;
    //qDebug()<<"    right="<<right;
    //qDebug()<<"    top="<<top;
    //qDebug()<<"    bottom="<<bottom;
    //qDebug()<<"    scale="<<scale;
    //qDebug()<<"    offset="<<offset;
    //qDebug()<<"    x2p(0)="<<x2p(0);
    //qDebug()<<"    tickStart="<<tickStart;

    if (std::isnan(left) || std::isnan(right) || std::isnan(top) || std::isnan(bottom)) return;
    if (std::isinf(left) || std::isinf(right) || std::isinf(top) || std::isinf(bottom)) return;
    //qDebug()<<"  start JKQTPVerticalAxis::drawAxes(";
    //qDebug()<<"drawing vertical axis ... range = ["<<axismin<<" .. "<<axismax<<"]\n";

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen pmain=painter.pen();
    pmain.setColor(axisStyle.axisColor);
    pmain.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.lineWidth*parent->getLineWidthMultiplier())));
    pmain.setStyle(Qt::SolidLine);

    QPen ptick=pmain;
    ptick.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.tickWidth*parent->getLineWidthMultiplier())));

    QPen pmtick=ptick;
    pmtick.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.minorTickWidth*parent->getLineWidthMultiplier())));

    getParentMathText()->setFontSize(this->axisStyle.tickLabelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);

    painter.setPen(pmain);

    // plot thick axis at y==0
    if (axisStyle.showZeroAxis && (0>axismin) && (0<axismax)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): 0Axis").arg(objectName()));
        #endif
        QPen pmain1=pmain;
        pmain1.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.lineWidthZeroAxis*parent->getLineWidthMultiplier())));
        pmain1.setColor(axisStyle.colorZeroAxis);
        pmain1.setStyle(axisStyle.styleZeroAxis);
        painter.setPen(pmain1);
        QLineF l(left-parent->pt2px(painter, axisStyle.tickOutsideLength), x2p(0), right+parent->pt2px(painter, axisStyle.tickOutsideLength), x2p(0));
        painter.drawLine(l);

        painter.setPen(pmain);
    }

    // draw thick axis lines, left and/or right
    if ( JKQTPCADrawModeHasLine(axisStyle.drawMode1)) {
        QLineF l(left, x2p(axismin), left, x2p(axismax));
        painter.drawLine(l);
    }
    if (JKQTPCADrawModeHasLine(axisStyle.drawMode2)) {
        QLineF l(right, x2p(axismin), right, x2p(axismax));
        painter.drawLine(l);
    }

    // plot minor and major ticks + tick labels
    QSizeF labelMax=getMaxTickLabelSize(painter);
    double x=tickStart;
    QString label="";
    bool first=true;
    // loop through all labels, as they are at the major ticks
    int cnt=0;

    QVector<QLineF> lines_ptick, lines_ptick2;
    QVector<QLineF> lines_pmtick, lines_pmtick2;

    {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaatii(QString("JKQTPEnhancedPainter[%1]::drawAxes(): calcLabels").arg(objectName()));
        #endif
        while (getNextLabel(x, label, first) && cnt<200) {
            double mtdist=getNextLabelDistance(x)/static_cast<double>(axisStyle.minorTicks+1);

            double xleft=-1000000;
            double xx=x2p(x);
            //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   axisStyle.minorTicks="<<minorTicks;

            if (JKQTPCADrawModeHasTicks(axisStyle.drawMode1)||JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
                //painter.setPen(ptick);
                if ((x<=axismax) && (x>=axismin))  {
                    QLineF l(left-parent->pt2px(painter, axisStyle.tickOutsideLength), xx, left+parent->pt2px(painter, axisStyle.tickInsideLength), xx);
                    if (l.length()>0) lines_ptick.append(l);//painter.drawLine(l);
                }
                //painter.setPen(pmtick);
                if ((tickLabels.size()<=0) && (axisStyle.minorTicks>0)) {
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
                        double mxx=x2p(mx);
                        //if (logAxis) qDebug()<<i<<"  =>  "<<mx<<mxx<<apprLogTicks<<w<<minorTicks;
                        if (mx<=axismax && mx>=axismin) {
                            QLineF l(xleft=left-parent->pt2px(painter, axisStyle.minorTickOutsideLength), mxx, left+parent->pt2px(painter, axisStyle.minorTickInsideLength), mxx);
                            if (l.length()>0) lines_pmtick.append(l);//painter.drawLine(l);
                            double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                            if (axisStyle.minorTickLabelsEnabled&&JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
                                if (axisStyle.minorTickLabelFullNumber) val=mx;
                                QString minorlabel=floattolabel(val);
                                drawTickLabel1(painter, left, mxx, minorlabel, axisStyle.minorTickLabelFontSize);
                                //drawTickLabel2(painter, right, xx, minorlabel, axisStyle.minorTickLabelFontSize);
                            }
                        }
                        mx=mx+mtdist;

                    }
                }
            }
            if (JKQTPCADrawModeHasTicks(axisStyle.drawMode2)||JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
                //painter.setPen(ptick);
                if (x<=axismax && x>=axismin) {
                    QLineF l(right-parent->pt2px(painter, axisStyle.tickInsideLength), xx, right+parent->pt2px(painter, axisStyle.tickOutsideLength), xx);
                    if (l.length()>0) lines_ptick2.append(l);//painter.drawLine(l);
                }
                //painter.setPen(pmtick);
                if ((tickLabels.size()<=0) && (axisStyle.minorTicks>0)) {
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
                        double mxx=x2p(mx);
                        if (mx<=axismax && mx>=axismin)  {
                            QLineF l(right-parent->pt2px(painter, axisStyle.minorTickInsideLength), mxx, xleft=(right+parent->pt2px(painter, axisStyle.minorTickOutsideLength)), mxx);
                            if (l.length()>0) lines_pmtick2.append(l);//painter.drawLine(l);
                            double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                            if (axisStyle.minorTickLabelsEnabled) {
                                if (axisStyle.minorTickLabelFullNumber&&JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) val=mx;
                                QString minorlabel=floattolabel(val);
                                //drawTickLabel1(painter, left, xx, minorlabel, axisStyle.minorTickLabelFontSize);
                                drawTickLabel2(painter, xleft, mxx, minorlabel, axisStyle.minorTickLabelFontSize);
                            }
                        }
                        mx=mx+mtdist;
                    }
                }
            }

            if ((label!="") && (x<=axismax && x>=axismin)) {


                if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) drawTickLabel1(painter, left, xx, label, this->axisStyle.tickLabelFontSize);
                if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) drawTickLabel2(painter, right, xx, label, this->axisStyle.tickLabelFontSize);

            }
            first=false;
            cnt++;
        }
    }
    {
    #ifdef JKQTBP_AUTOTIMER
        JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): drawLines").arg(objectName()));
    #endif
        painter.setPen(ptick);
        painter.drawLines(lines_ptick);
        painter.drawLines(lines_ptick2);
        painter.setPen(pmtick);
        painter.drawLines(lines_pmtick);
        painter.drawLines(lines_pmtick2);
    }

    // plot axis label
    if (!axisLabel.isEmpty() && JKQTPCADrawModeHasAxisLabel(axisStyle.drawMode1)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): axisLabel1").arg(objectName()));
        #endif
        getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
        getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
        getParentMathText()->parse(axisLabel);
        double width, ascent, descent, strikeoutPos;
        getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);


        QRectF rect(0,0, getParentPlotWidth(), ascent+descent);//plotBorderLeft-30);
        painter.save(); auto __finalpaintiner=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(QPointF(left-parent->pt2px(painter, axisStyle.tickOutsideLength+axisStyle.tickLabelDistance+axisStyle.labelDistance)-descent-labelMax.width()-labelMax.height(), bottom));
        painter.rotate(-90);
        //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(axisStyle.labelPosition) {
            case JKQTPLabelMax:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPLabelMin:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPLabelCenter:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
            painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
            QPen p("magenta");
            QColor col=p.color(); col.setAlphaF(0.8); p.setColor(col);
            p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
            painter.setPen(p);
            painter.setBrush(QBrush(QColor(Qt::transparent)));
            painter.drawRect(rect);

        }
        painter.resetTransform();

    }
    if (!axisLabel.isEmpty() && JKQTPCADrawModeHasAxisLabel(axisStyle.drawMode2)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPEnhancedPainter[%1]::drawAxes(): axisLabel2").arg(objectName()));
        #endif
        getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
        getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
        getParentMathText()->parse(axisLabel);


        QRectF rect(0,0, getParentPlotWidth(), getParentMathText()->getSize(painter).height());//plotBorderLeft-30);
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(QPointF(right+parent->pt2px(painter, axisStyle.tickOutsideLength+axisStyle.tickLabelDistance+axisStyle.labelDistance)+labelMax.width(), bottom));
        painter.rotate(-90);
        //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(axisStyle.labelPosition) {
            case JKQTPLabelMax:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPLabelMin:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPLabelCenter:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
            painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
            QPen p("magenta");
            QColor col=p.color(); col.setAlphaF(0.8); p.setColor(col);
            p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
            painter.setPen(p);
            painter.setBrush(QBrush(QColor(Qt::transparent)));
            painter.drawRect(rect);

        }
        //painter.resetTransform();

    }


    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("cyan");
        p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth);
        QColor col=p.color(); col.setAlphaF(0.8); p.setColor(col);
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

QSizeF JKQTPHorizontalAxis::getSize1(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode1==JKQTPCADMnone) return QSize(0,0);
    double ptwidth=axisStyle.axisLineOffset;
    double labwidth=0;
    if (JKQTPCADrawModeHasTicks(axisStyle.drawMode1)) ptwidth+=axisStyle.tickOutsideLength;
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
        ptwidth+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labwidth+=getMaxTickLabelSize(painter).height();
    }
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
        ptwidth+=axisStyle.labelDistance;
        // find out size of axis label
        labwidth+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(getParentPlotWidth(), parent->pt2px(painter, ptwidth)+labwidth);
}

QSizeF JKQTPHorizontalAxis::getSize2(JKQTPEnhancedPainter& painter) {
    if (axisStyle.drawMode2==JKQTPCADMnone) return QSize(0,0);
    double ptwidth=axisStyle.axisLineOffset;
    double labwidth=0;
    if (JKQTPCADrawModeHasTicks(axisStyle.drawMode2)) ptwidth+=axisStyle.tickOutsideLength;
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
        ptwidth+=axisStyle.tickLabelDistance;
        // find out the maximum width over all visible plot labels
        labwidth+=getMaxTickLabelSize(painter).height();
    }
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
        ptwidth+=axisStyle.labelDistance;
        // find out size of axis label
        labwidth+=parent->getTextSizeSize(getParent()->getCurrentPlotterStyle().defaultFontName, axisStyle.labelFontSize*parent->getFontSizeMultiplier(), axisLabel, painter).height();
    }

    return QSizeF(getParentPlotWidth(), parent->pt2px(painter, ptwidth)+labwidth);
}


double JKQTPHorizontalAxis::getParentOtheraxisOffset() const {
    return parent->y2p(parent->getYMax());
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


void JKQTPHorizontalAxis::drawTickLabel1(JKQTPEnhancedPainter &painter, double xx, double yy, const QString &label, double fontSize, double ascentMax, double /*descentMax*/)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);

    getParentMathText()->parse(label);

    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
        double lx0=yy+parent->pt2px(painter, axisStyle.tickOutsideLength+axisStyle.tickLabelDistance);
        double lx=lx0+ascentMax;
        if (axisStyle.tickLabelAngle!=0) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            if (axisStyle.tickLabelAngle>0) {
                painter.translate(xx-fabs(ascent*sin(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)/2.0), lx0+fabs(ascent*cos(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)));
                painter.rotate(axisStyle.tickLabelAngle);
            } else {
                painter.translate(xx+fabs(ascent*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)/2.0)-width*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), lx0+ascent*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI))+fabs(width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)));
                painter.rotate(axisStyle.tickLabelAngle);
            }
            getParentMathText()->draw(painter, 0,0);

        } else {
            getParentMathText()->draw(painter, xx-width/2.0, lx);
        }
    }
}

void JKQTPHorizontalAxis::drawTickLabel2(JKQTPEnhancedPainter &painter, double xx, double yy, const QString &label, double fontSize, double /*ascentMax*/, double descentMax)
{
    getParentMathText()->setFontSize(fontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);

    getParentMathText()->parse(label);

    double width, ascent, descent, strikeoutPos;
    getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
        double lx0=yy-parent->pt2px(painter, axisStyle.tickOutsideLength+axisStyle.tickLabelDistance);
        double lx=lx0-descentMax;
        if (axisStyle.tickLabelAngle!=0) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            if (axisStyle.tickLabelAngle>0) {
                painter.translate(xx+fabs(descent*sin(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)/2.0), lx0-fabs(descent*cos(axisStyle.tickLabelAngle/180.0*JKQTPSTATISTICS_PI)));
                painter.rotate(-axisStyle.tickLabelAngle);
            } else {
                painter.translate(xx-fabs(descent*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)/2.0)-width*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)), lx0-descent*fabs(cos(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI))-fabs(width*sin(fabs(axisStyle.tickLabelAngle)/180.0*JKQTPSTATISTICS_PI)));
                painter.rotate(-axisStyle.tickLabelAngle);
            }
            getParentMathText()->draw(painter, 0,0);

        } else {
            getParentMathText()->draw(painter, xx-width/2.0, lx);
        }


    }
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

    QPen pmain=painter.pen();
    pmain.setColor(axisStyle.axisColor);
    pmain.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.lineWidth*parent->getLineWidthMultiplier())));
    pmain.setStyle(Qt::SolidLine);

    QPen ptick=pmain;
    ptick.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.tickWidth*parent->getLineWidthMultiplier())));

    QPen pmtick=ptick;
    pmtick.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.minorTickWidth*parent->getLineWidthMultiplier())));

    getParentMathText()->setFontSize(this->axisStyle.tickLabelFontSize*parent->getFontSizeMultiplier());
    getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
    getParentMathText()->setFontColor(axisStyle.axisColor);

    painter.setPen(pmain);

    // plot thick axis at y==0
    if (axisStyle.showZeroAxis && (0>axismin) && (0<axismax)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaat(QString("JKQTPHorizontalAxis[%1]::drawAxes(): 0Axis").arg(objectName()));
        #endif
        QPen pmain1=pmain;
        pmain1.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, axisStyle.lineWidthZeroAxis*parent->getLineWidthMultiplier())));
        pmain1.setColor(axisStyle.colorZeroAxis);
        pmain1.setStyle(axisStyle.styleZeroAxis);
        painter.setPen(pmain1);
        QLineF l(x2p(0), bottom+parent->pt2px(painter, axisStyle.tickOutsideLength), x2p(0), top-parent->pt2px(painter, axisStyle.tickOutsideLength));
        painter.drawLine(l);
        painter.setPen(pmain);
    }

    // draw thick axis lines, left and/or right
    if (JKQTPCADrawModeHasLine(axisStyle.drawMode1)) {
        QLineF l(x2p(axismin), bottom, x2p(axismax), bottom);
        painter.drawLine(l);
    }
    if (JKQTPCADrawModeHasLine(axisStyle.drawMode2)) {
        QLineF l(x2p(axismin), top, x2p(axismax), top);
        painter.drawLine(l);
    }

    // plot minor and major ticks + tick labels
    double ascentMax, descentMax;
    QSizeF labelMax=getMaxTickLabelSize(painter, &ascentMax, &descentMax);
    double x=tickStart;
    QString label="";
    bool first=true;
    // loop through all labels, as they are at the major ticks
    int cnt=0;
    QVector<QLineF> lines_ptick, lines_ptick2;
    QVector<QLineF> lines_pmtick, lines_pmtick2;


    {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): calcLabels").arg(objectName()));
        #endif

        while (getNextLabel(x, label, first) && cnt<200) {
            double mtdist=getNextLabelDistance(x)/static_cast<double>(axisStyle.minorTicks+1);

            double xleft=-1000000;
            double xx=x2p(x);
            //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   axisStyle.minorTicks="<<minorTicks;

            if (JKQTPCADrawModeHasTicks(axisStyle.drawMode1)||JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
                //painter.setPen(ptick);
                if (x<=axismax && x>=axismin)  {
                    QLineF l(xx, bottom-parent->pt2px(painter, axisStyle.tickInsideLength), xx, bottom+parent->pt2px(painter, axisStyle.tickOutsideLength));
                    if (l.length()>0) lines_ptick.append(l);//painter.drawLine(l);
                }
                //painter.setPen(pmtick);
                if ((tickLabels.size()<=0) && (axisStyle.minorTicks>0)) {
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
                        double mxx=x2p(mx);

                        if (mx<=axismax && mx>=axismin)  {
                            QLineF l( mxx, bottom-parent->pt2px(painter, axisStyle.minorTickInsideLength), mxx, xleft=(bottom+parent->pt2px(painter, axisStyle.minorTickOutsideLength)));
                            if (l.length()>0) lines_pmtick.append(l);//painter.drawLine(l);

                            double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                            if (axisStyle.minorTickLabelsEnabled && JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) {
                                if (axisStyle.minorTickLabelFullNumber) val=mx;
                                QString minorlabel=floattolabel(val);
                                drawTickLabel1(painter,  mxx, xleft, minorlabel, axisStyle.minorTickLabelFontSize, ascentMax, descentMax);
                            }


                        }
                        mx=mx+mtdist;
                    }
                }
            }
            if (JKQTPCADrawModeHasTicks(axisStyle.drawMode2)||JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
                painter.setPen(ptick);
                if (x<=axismax && x>=axismin)  {
                    QLineF l(xx, top-parent->pt2px(painter, axisStyle.tickOutsideLength), xx, top+parent->pt2px(painter, axisStyle.tickInsideLength));
                    if (l.length()>0) lines_ptick2.append(l);//painter.drawLine(l);
                }
                painter.setPen(pmtick);
                if ((tickLabels.size()<=0) && (axisStyle.minorTicks>0)) {
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
                        double mxx=x2p(mx);
                        if (mx<=axismax && mx>=axismin)  {
                            QLineF l( mxx, top-parent->pt2px(painter, axisStyle.minorTickOutsideLength), mxx, xleft=(top+parent->pt2px(painter, axisStyle.minorTickInsideLength)));
                            if (l.length()>0) lines_pmtick2.append(l);//painter.drawLine(l);

                            double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                            if (axisStyle.minorTickLabelsEnabled && JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) {
                                if (axisStyle.minorTickLabelFullNumber) val=mx;
                                QString minorlabel=floattolabel(val);
                                drawTickLabel2(painter,  mxx, xleft, minorlabel, axisStyle.minorTickLabelFontSize, ascentMax, descentMax);
                            }

                        }
                        mx=mx+mtdist;
                    }
                }
            }

            if ((label!="") && (x<=axismax && x>=axismin)) {


                if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode1)) drawTickLabel1(painter, xx, bottom, label, this->axisStyle.tickLabelFontSize, ascentMax, descentMax);
                if (JKQTPCADrawModeHasTickLabels(axisStyle.drawMode2)) drawTickLabel2(painter, xx, top, label, this->axisStyle.tickLabelFontSize, ascentMax, descentMax);

            }
            first=false;
            cnt++;
        }
    }
    {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): drawLines").arg(objectName()));
        #endif

        painter.setPen(ptick);
        painter.drawLines(lines_ptick);
        painter.drawLines(lines_ptick2);
        painter.setPen(pmtick);
        painter.drawLines(lines_pmtick);
        painter.drawLines(lines_pmtick2);
    }

    // plot axis label
    if (!axisLabel.isEmpty() && JKQTPCADrawModeHasAxisLabel(axisStyle.drawMode1)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): axisLabel1").arg(objectName()));
        #endif
        getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
        getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
        getParentMathText()->parse(axisLabel);
        double width, ascent, descent, strikeoutPos;
        getParentMathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);


        QRectF rect(0,0, getParentPlotWidth(), ascent+descent);//plotBorderLeft-30);
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(QPointF(left, bottom+parent->pt2px(painter, axisStyle.tickOutsideLength+axisStyle.tickLabelDistance+axisStyle.labelDistance)+labelMax.height()));
        //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(axisStyle.labelPosition) {
            case JKQTPLabelMax:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPLabelMin:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPLabelCenter:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
            painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
            QPen p("magenta");
            QColor col=p.color(); col.setAlphaF(0.8); p.setColor(col);
            p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
            painter.setPen(p);
            painter.setBrush(QBrush(QColor(Qt::transparent)));
            painter.drawRect(rect);

        }
        //painter.resetTransform();

    }
    if (!axisLabel.isEmpty() && JKQTPCADrawModeHasAxisLabel(axisStyle.drawMode2)) {
        #ifdef JKQTBP_AUTOTIMER
            JKQTPAutoOutputTimer jkaati(QString("JKQTPHorizontalAxis[%1]::drawAxes(): axisLabel2").arg(objectName()));
        #endif
        getParentMathText()->setFontSize(axisStyle.labelFontSize*parent->getFontSizeMultiplier());
        getParentMathText()->setFontRomanOrSpecial(getParent()->getCurrentPlotterStyle().defaultFontName);
        getParentMathText()->parse(axisLabel);


        QRectF rect(0,0, getParentPlotWidth(), getParentMathText()->getSize(painter).height());//plotBorderLeft-30);
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(QPointF(left, top-parent->pt2px(painter, axisStyle.tickOutsideLength+axisStyle.tickLabelDistance+axisStyle.labelDistance)-labelMax.height()-rect.height()));
        //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(axisStyle.labelPosition) {
            case JKQTPLabelMax:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPLabelMin:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPLabelCenter:
                getParentMathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
            painter.save(); auto __finalpaintinnerif=JKQTPFinally([&painter]() {painter.restore();});
            QPen p("magenta");
            QColor col=p.color(); col.setAlphaF(0.8); p.setColor(col);
            p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
            painter.setPen(p);
            painter.setBrush(QBrush(QColor(Qt::transparent)));
            painter.drawRect(rect);

        }
        //painter.resetTransform();

    }

    if (getParent()->getCurrentPlotterStyle().debugShowRegionBoxes) {
        painter.save(); auto __finalpaintif=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("cyan");
        QColor col=p.color(); col.setAlphaF(0.8); p.setColor(col);
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


