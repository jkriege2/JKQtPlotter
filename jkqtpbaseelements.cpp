/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate$  (revision $Rev$)

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



#include "jkqtpbaseelements.h"
#include "jkqtpbaseplotter.h"
#include <QDebug>
#include <QDateTime>
#include <cfloat>
#include <QApplication>


//#undef SHOW_JKQTPLOTTER_DEBUG
//#define SHOW_JKQTPLOTTER_DEBUG

JKQTPcoordinateAxis::JKQTPcoordinateAxis(JKQtBasePlotter* parent): QObject(parent) {
    this->parent=parent;

    autoLabelDigits=true;

    tickMode=def_tickMode=JKQTPLTMLinOrPower;

    axisPrefix="";
    scaleSign=1;
    doUpdateScaling=true;

    axismin=-10;
    axismax=10;
    axisabsoultemin=-DBL_MAX/100.0;
    axisabsoultemax=DBL_MAX/100.0;
    axisMinWidth=-1;

    width=20;

    scale=0;
    offset=0;
    inverted=false;

    tickSpacing=0;
    tickSpacingLog=10;
    tickStart=0;
    labelDigits=3;

    autoAxisSpacing=def_autoAxisSpacing=true;
    logAxis=def_logAxis=false;
    minorTickLabelsEnabled=def_minorTickLabelsEnabled=false;
    minorTickLabelFullNumber=def_minorTickLabelFullNumber=true;
    logAxisBase=def_logAxisBase=10;
    userTickSpacing=def_userTickSpacing=1;
    userLogTickSpacing=def_userLogTickSpacing=10;
    tickLabelAngle=def_tickLabelAngle=0;

    labelType=def_labelType=JKQTPCALTexponent;

    axisLabel="";

    QFont f=QApplication::font();

    labelPosition=def_labelPosition=JKQTPlabelCenter;
    labelFont=def_labelFont=f.family(); //"Arial";
    labelFontSize=def_labelFontSize=f.pointSizeF()+1; //12;
    tickLabelFont=def_tickLabelFont=f.family(); //"Arial";
    tickLabelFontSize=def_tickLabelFontSize=f.pointSizeF(); //10;
    minorTickLabelFontSize=def_minorTickLabelFontSize=f.pointSizeF()*0.75; //10;

    tickTimeFormat=def_tickTimeFormat=QLocale().timeFormat(QLocale::NarrowFormat);
    tickDateFormat=def_tickDateFormat=QLocale().dateFormat(QLocale::NarrowFormat);
    tickDateTimeFormat=def_tickDateTimeFormat=QLocale().dateTimeFormat(QLocale::NarrowFormat);

    minTicks=def_minTicks=5;
    minorTicks=def_minorTicks=1;
    tickOutsideLength=def_tickOutsideLength=3;
    minorTickOutsideLength=def_minorTickOutsideLength=1.5;
    tickInsideLength=def_tickInsideLength=3;
    minorTickInsideLength=def_minorTickInsideLength=1.5;
    tickLabelDistance=def_tickLabelDistance=3;
    labelDistance=def_labelDistance=5;
    axisColor=def_axisColor=QColor("black");
    showZeroAxis=def_showZeroAxis=true;
    drawMode1=def_drawMode1=JKQTPCADMcomplete;
    drawMode2=def_drawMode2=JKQTPCADMticks;
    minorTickWidth=def_minorTickWidth=1;
    tickWidth=def_tickWidth=1.5;
    lineWidth=def_lineWidth=1.5;
    lineWidthZeroAxis=def_lineWidthZeroAxis=1.5;

    gridColor=def_gridColor=QColor("gray");
    gridWidth=def_gridWidth=0.75;
    gridStyle=def_gridStyle=Qt::DashLine;
    drawGrid=def_drawGrid=true;
    minorGridColor=def_minorGridColor=QColor("gray");
    minorGridWidth=def_minorGridWidth=0.5;
    minorGridStyle=def_minorGridStyle=Qt::DotLine;
    drawMinorGrid=def_drawMinorGrid=false;

    paramsChanged=true;
}

JKQTPcoordinateAxis::~JKQTPcoordinateAxis() {
}

void JKQTPcoordinateAxis::set_parent(JKQtBasePlotter* parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPcoordinateAxis::update_plot() {
    //if (paramsChanged)  {
        calcPlotScaling();
        parent->update_plot();
    //}
}

JKQTmathText* JKQTPcoordinateAxis::get_parent_mathText() {
    return parent->get_mathText();
}



#define JKQTPPROPERTYloadFromAxis(settings, param) this->param=settings->param


void JKQTPcoordinateAxis::loadSettings(JKQTPcoordinateAxis* settings) {
    JKQTPPROPERTYloadFromAxis(settings, showZeroAxis);
    JKQTPPROPERTYloadFromAxis(settings, autoAxisSpacing);
    JKQTPPROPERTYloadFromAxis(settings, minorTickLabelsEnabled);
    JKQTPPROPERTYloadFromAxis(settings, logAxis);
    JKQTPPROPERTYloadFromAxis(settings, inverted);
    JKQTPPROPERTYloadFromAxis(settings, logAxisBase);
    JKQTPPROPERTYloadFromAxis(settings, userTickSpacing);
    JKQTPPROPERTYloadFromAxis(settings, userLogTickSpacing);
    JKQTPPROPERTYloadFromAxis(settings, minorTickWidth);
    JKQTPPROPERTYloadFromAxis(settings, tickWidth);
    JKQTPPROPERTYloadFromAxis(settings, lineWidth);
    JKQTPPROPERTYloadFromAxis(settings, lineWidthZeroAxis);

    JKQTPPROPERTYloadFromAxis(settings, axisLabel);

    JKQTPPROPERTYloadFromAxis(settings, labelFont);
    JKQTPPROPERTYloadFromAxis(settings, labelFontSize);
    JKQTPPROPERTYloadFromAxis(settings, tickLabelFont);
    JKQTPPROPERTYloadFromAxis(settings, tickLabelFontSize);
    JKQTPPROPERTYloadFromAxis(settings, minorTickLabelFontSize);
    JKQTPPROPERTYloadFromAxis(settings, minorTickLabelFullNumber);

    JKQTPPROPERTYloadFromAxis(settings, tickTimeFormat);
    JKQTPPROPERTYloadFromAxis(settings, tickDateFormat);
    JKQTPPROPERTYloadFromAxis(settings, tickDateTimeFormat);


    JKQTPPROPERTYloadFromAxis(settings, minTicks);
    JKQTPPROPERTYloadFromAxis(settings, minorTicks);
    JKQTPPROPERTYloadFromAxis(settings, tickOutsideLength);
    JKQTPPROPERTYloadFromAxis(settings, minorTickOutsideLength);
    JKQTPPROPERTYloadFromAxis(settings, tickInsideLength);
    JKQTPPROPERTYloadFromAxis(settings, minorTickInsideLength);
    JKQTPPROPERTYloadFromAxis(settings, tickLabelDistance);
    JKQTPPROPERTYloadFromAxis(settings, labelDistance);
    JKQTPPROPERTYloadFromAxis(settings, gridWidth);
    JKQTPPROPERTYloadFromAxis(settings, minorGridWidth);
    JKQTPPROPERTYloadFromAxis(settings, drawGrid);
    JKQTPPROPERTYloadFromAxis(settings, drawMinorGrid);

    JKQTPPROPERTYloadFromAxis(settings, labelPosition);
    JKQTPPROPERTYloadFromAxis(settings, labelType);
    JKQTPPROPERTYloadFromAxis(settings, axisColor);

    JKQTPPROPERTYloadFromAxis(settings, gridColor);
    JKQTPPROPERTYloadFromAxis(settings, minorGridColor);
    JKQTPPROPERTYloadFromAxis(settings, gridStyle);
    JKQTPPROPERTYloadFromAxis(settings, minorGridStyle);

    JKQTPPROPERTYloadFromAxis(settings, drawMode1);
    JKQTPPROPERTYloadFromAxis(settings, drawMode2);
    JKQTPPROPERTYloadFromAxis(settings, tickMode);
    paramsChanged=true;
    calcPlotScaling();
}

void JKQTPcoordinateAxis::loadSettings(QSettings& settings, QString group) {
    JKQTPPROPERTYload(settings, group+axisPrefix, showZeroAxis, "show_zero_line", toBool());
    JKQTPPROPERTYload(settings, group+axisPrefix, autoAxisSpacing, "auto_spacing", toBool());
    JKQTPPROPERTYload(settings, group+axisPrefix, logAxis, "log", toBool());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorTickLabelsEnabled, "minorTickLabelsEnabled", toBool());
    JKQTPPROPERTYload(settings, group+axisPrefix, inverted, "inverted", toBool());
    JKQTPPROPERTYload(settings, group+axisPrefix, logAxisBase, "log_base", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, userTickSpacing, "tick_spacing", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, userLogTickSpacing, "log_tick_spacing", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorTickWidth, "minor_tick_width", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickWidth, "tick_width", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, lineWidth, "line_width", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, lineWidthZeroAxis, "line_width_zeroaxis", toDouble());

    axisLabel=settings.value(group+axisPrefix+"axis_label", axisLabel).toString();

    JKQTPPROPERTYload(settings, group+axisPrefix, labelFont, "label_font", toString());
    JKQTPPROPERTYload(settings, group+axisPrefix, labelFontSize, "label_font_size", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickLabelFont, "tick_label_font", toString());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickLabelFontSize, "tick_label_font_size", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorTickLabelFontSize, "minor_tick_label_font_size", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorTickLabelFullNumber, "minor_tick_label_full_number", toBool());

    JKQTPPROPERTYload(settings, group+axisPrefix, tickTimeFormat, "tick_time_format", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickDateFormat, "tick_date_format", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickDateTimeFormat, "tick_datetime_format", toDouble());


    JKQTPPROPERTYload(settings, group+axisPrefix, minTicks, "min_ticks", toUInt());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorTicks, "minor_ticks", toUInt());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickOutsideLength, "tick_outside_length", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorTickOutsideLength, "minor_tick_outside_length", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickInsideLength, "tick_inside_length", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorTickInsideLength, "minor_tick_inside_length", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, tickLabelDistance, "tick_label_distance", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, labelDistance, "label_distance", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, gridWidth, "grid_width", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, minorGridWidth, "minor_grid_width", toDouble());
    JKQTPPROPERTYload(settings, group+axisPrefix, drawGrid, "draw_grid", toBool());
    JKQTPPROPERTYload(settings, group+axisPrefix, drawMinorGrid, "draw_minor_grid", toBool());

    labelPosition=String2JKQTPlabelPosition(settings.value(group+axisPrefix+"label_position", JKQTPlabelPosition2String(labelPosition)).toString());
    labelType=String2JKQTPCAlabelType(settings.value(group+axisPrefix+"label_type", JKQTPCAlabelType2String(labelType)).toString());
    axisColor=QColor(settings.value(group+axisPrefix+"color", QColor2String(axisColor)).toString());

    gridColor=QColor(settings.value(group+axisPrefix+"grid_color", QColor2String(gridColor)).toString());
    minorGridColor=QColor(settings.value(group+axisPrefix+"minor_grid_color", QColor2String(minorGridColor)).toString());
    gridStyle=String2QPenStyle(settings.value(group+axisPrefix+"grid_style", QPenStyle2String(gridStyle)).toString());
    minorGridStyle=String2QPenStyle(settings.value(group+axisPrefix+"minor_grid_style", QPenStyle2String(minorGridStyle)).toString());

    drawMode1=String2JKQTPCAdrawMode(settings.value(group+axisPrefix+"draw_mode1", JKQTPCAdrawMode2String(drawMode1)).toString());
    drawMode2=String2JKQTPCAdrawMode(settings.value(group+axisPrefix+"draw_mode2", JKQTPCAdrawMode2String(drawMode2)).toString());
    tickMode=String2JKQTPLabelTickMode(settings.value(group+axisPrefix+"tick_mode", JKQTPLabelTickMode2String(tickMode)).toString());
    paramsChanged=true;
    calcPlotScaling();
}



void JKQTPcoordinateAxis::saveSettings(QSettings& settings, QString group) {
    JKQTPPROPERTYsave(settings, group+axisPrefix, showZeroAxis, "show_zero_line");
    JKQTPPROPERTYsave(settings, group+axisPrefix, autoAxisSpacing, "auto_spacing");
    JKQTPPROPERTYsave(settings, group+axisPrefix, logAxis, "log");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorTickLabelsEnabled, "minorTickLabelsEnabled");
    JKQTPPROPERTYsave(settings, group+axisPrefix, logAxisBase, "log_base");
    JKQTPPROPERTYsave(settings, group+axisPrefix, userTickSpacing, "tick_spacing");
    JKQTPPROPERTYsave(settings, group+axisPrefix, userLogTickSpacing, "log_tick_spacing");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorTickWidth, "minor_tick_width");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickWidth, "tick_width");
    JKQTPPROPERTYsave(settings, group+axisPrefix, lineWidth, "line_width");
    JKQTPPROPERTYsave(settings, group+axisPrefix, lineWidthZeroAxis, "line_width_zeroaxis");

    settings.setValue(group+axisPrefix+"label", axisLabel);

    JKQTPPROPERTYsave(settings, group+axisPrefix, labelFont, "label_font");
    JKQTPPROPERTYsave(settings, group+axisPrefix, labelFontSize, "label_font_size");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickLabelFont, "tick_label_font");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickLabelFontSize, "tick_label_font_size");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorTickLabelFontSize, "minor_tick_label_font_size");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorTickLabelFullNumber, "minor_tick_label_full_number");

    JKQTPPROPERTYsave(settings, group+axisPrefix, tickTimeFormat, "tick_time_format");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickDateFormat, "tick_date_format");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickDateTimeFormat, "tick_datetime_format");



    JKQTPPROPERTYsave(settings, group+axisPrefix, minTicks, "min_ticks");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorTicks, "minor_ticks");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickOutsideLength, "tick_outside_length");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorTickOutsideLength, "minor_tick_outside_length");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickInsideLength, "tick_inside_length");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorTickInsideLength, "minor_tick_inside_length");
    JKQTPPROPERTYsave(settings, group+axisPrefix, tickLabelDistance, "tick_label_distance");
    JKQTPPROPERTYsave(settings, group+axisPrefix, labelDistance, "label_distance");
    JKQTPPROPERTYsave(settings, group+axisPrefix, gridWidth, "grid_width");
    JKQTPPROPERTYsave(settings, group+axisPrefix, minorGridWidth, "minor_grid_width");
    JKQTPPROPERTYsave(settings, group+axisPrefix, drawGrid, "draw_grid");
    JKQTPPROPERTYsave(settings, group+axisPrefix, drawMinorGrid, "draw_minor_grid");

    if (labelPosition!=def_labelPosition) settings.setValue(group+axisPrefix+"label_position", JKQTPlabelPosition2String(labelPosition));
    if (labelType!=def_labelType) settings.setValue(group+axisPrefix+"label_type", JKQTPCAlabelType2String(labelType));
    if (axisColor!=def_axisColor) settings.setValue(group+axisPrefix+"color", QColor2String(axisColor));
    if (drawMode1!=def_drawMode1) settings.setValue(group+axisPrefix+"draw_mode1", JKQTPCAdrawMode2String(drawMode1));
    if (drawMode2!=def_drawMode2) settings.setValue(group+axisPrefix+"draw_mode2", JKQTPCAdrawMode2String(drawMode2));
    if (tickMode!=def_tickMode) settings.setValue(group+axisPrefix+"tick_mode", JKQTPLabelTickMode2String(tickMode));
    if (gridColor!=def_gridColor) settings.setValue(group+axisPrefix+"grid_color", QColor2String(gridColor));
    if (minorGridColor!=def_minorGridColor) settings.setValue(group+axisPrefix+"minor_grid_style", QPenStyle2String(minorGridStyle));
    if (gridStyle!=def_gridStyle) settings.setValue(group+axisPrefix+"grid_color", QColor2String(gridColor));
    if (minorGridStyle!=def_minorGridStyle) settings.setValue(group+axisPrefix+"minor_grid_style", QPenStyle2String(minorGridStyle));
}

double JKQTPcoordinateAxis::calcLinearTickSpacing() {
    if (labelType==JKQTPCALTdate) {
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
        } while (level<=3 && tick_count<minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/(double)minTicks;
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    } else if (labelType==JKQTPCALTdatetime) {
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
        } while (level<6 && tick_count<minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/(double)minTicks;
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    } else if (labelType==JKQTPCALTtime) {
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
        } while (level<3 && tick_count<minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/(double)minTicks;
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    } else {
        double TickSpacing=1;
        int startexponent=(int)floor(log10(width))+3;
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
        } while (tick_count<minTicks && cnt<1000);

        // the cnt variable exists for savety reasons to prevent an endless loop!
        if (cnt>=1000) TickSpacing=width/(double)minTicks;
        //qDebug()<<"TickSpacing="<<TickSpacing<<"   cnt="<<cnt;
        return TickSpacing;
    }
}


double JKQTPcoordinateAxis::calcLogTickSpacing() {
      double TickSpacing=logAxisBase;
      if (fabs(log(axismax)-log(axismin))/log(logAxisBase)>2*minTicks) {
          double power=1;
          int cnt=0;
          while (fabs(log(axismax)-log(axismin))/log(logAxisBase)/power>2*minTicks && cnt<1000) {
#ifdef SHOW_JKQTPLOTTER_DEBUG
              //qDebug()<<cnt<<power<<log(width)/log(logAxisBase)/power<<2*minTicks;
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


QString JKQTPcoordinateAxis::floattolabel(double data) {
    int past_comma=labelDigits;
    bool remove_trail0=true;
    QLocale loc=QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);

    switch(labelType) {
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
                return QString(jkqtp_floattolatexstr(data, past_comma, remove_trail0, 1e-300, pow(10, -past_comma), pow(10, past_comma+1)).c_str());
            }; break;
        case JKQTPCALTexponentCharacter: {
                return QString(jkqtp_floattounitstr(data, past_comma, remove_trail0).c_str());
            }; break;
        case JKQTPCALTdate: {
                QDateTime dt;
                dt.setMSecsSinceEpoch(uint(data));
                return dt.toString(tickDateFormat);
            }; break;
        case JKQTPCALTtime: {
                QDateTime dt;
                dt.setMSecsSinceEpoch(uint(data));
                return dt.toString(tickTimeFormat);
            }; break;
        case JKQTPCALTdatetime: {
                QDateTime dt;
                dt.setMSecsSinceEpoch(uint(data));
                return dt.toString(tickDateTimeFormat);
            }; break;
        default:
            return QString();
            break;
    }

    /*
    tickTimeFormat
    tickDateFormat
    tickDateTimeFormat
     **/

    return QString();
}

QString JKQTPcoordinateAxis::floattolabel(double data, int past_comma) {
    bool remove_trail0=true;
    QLocale loc=QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    if (labelType==JKQTPCALTdefault) {
        QString res=loc.toString(data, 'f', past_comma);//QString::number(data, 'f', past_comma);
        if (remove_trail0 && res.contains(QLocale::system().decimalPoint())) {
            while (res.size()>0 && res[res.size()-1]=='0') {
                res=res.left(res.size()-1);
            }
            if (res.size()>0 && res[res.size()-1]==loc.decimalPoint()) res=res.left(res.size()-1);
        }
        return res;
    } else if (labelType==JKQTPCALTexponent) return QString(jkqtp_floattolatexstr(data, past_comma, remove_trail0, 1e-300, pow(10, -past_comma), pow(10, past_comma+1)).c_str());
    else if (labelType==JKQTPCALTexponentCharacter) return QString(jkqtp_floattounitstr(data, past_comma, remove_trail0).c_str());
    return "";
}

int JKQTPcoordinateAxis::calcLinearUnitDigits() {
    if (!autoLabelDigits) return labelDigits;
    int unitdigits=-1;
    double minval=tickStart;
    bool equals=true;
    /*for (int i=0; i<20; i++) {
        equals=equals || (floattolabel((minval+(double)i*tickSpacing), unitdigits)== floattolabel((minval+(double)(i+1)*tickSpacing), unitdigits));
    }*/
    while ((unitdigits<20) && equals) {
        unitdigits++;
        equals=false;
        for (int i=-10; i<10; i++) {
            QString l1=floattolabel((minval+(double)i*tickSpacing), unitdigits+2);
            QString l2=floattolabel((minval+(double)(i+1.0)*tickSpacing), unitdigits+2);
            //qDebug()<<"unitdigits="<<unitdigits<<"   l1="<<l1<<"   l2="<<l2;
            equals=equals || (l1==l2);
        }
    }
    return unitdigits+2;
}


void JKQTPcoordinateAxis::calcPlotScaling(bool force) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQtBasePlotter[%1]::calcPlotScaling()").arg(objectName()));
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
        if (axismax<=0) axismax=axismin+pow(10.0, (int)log10(axismin));
        if (axismax<axismin) {
            std::swap(axismin, axismax);
        } else if (axismax==axismin) {
            axismax=axismin+pow(10.0, (int)log10(axismin));
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


    double plotOffset=get_parent_plotoffset();
#ifdef SHOW_JKQTPLOTTER_DEBUG
    //qDebug()<<get_parent()->objectName()<<":  inverted   = "<<inverted;
    //qDebug()<<get_parent()->objectName()<<":  plotOffset = "<<plotOffset;
    //qDebug()<<get_parent()->objectName()<<":  plotWidth  = "<<get_parent_plotwidth();
#endif
    if (isLogAxis()) {
        scale=get_parent_plotwidth()/(log(axismax)-log(axismin))*log(logAxisBase);
        if (scaleSign>0) {
            offset=plotOffset-log(axismin)/log(logAxisBase)*scale;
        } else {
            offset=plotOffset+log(axismax)/log(logAxisBase)*scale;
        }
    } else {
        scale=get_parent_plotwidth()/width;
        if (scaleSign>0) {
            offset=plotOffset-axismin*scale;
        } else {
            offset=plotOffset+axismax*scale;
        }

    }
#ifdef SHOW_JKQTPLOTTER_DEBUG
    //qDebug()<<get_parent()->objectName()<<":  offset     = "<<offset;
    //qDebug()<<get_parent()->objectName()<<":  scale      = "<<scale;
    //qDebug()<<get_parent()->objectName()<<":  scaleSign  = "<<scaleSign;
#endif
    // now we try to find a suitable tick and grid line spacing so that
    // between maxTicks and minTicks ticks fit into twidth
    // if autoXAxisSpacing/autoYAxisSpacing is false then the spacing properties
    // are calculated from the user-supplied parameters currently there is no
    // autoscaling for the logarithmic plots available
    if (tickMode==JKQTPLTMLinOrPower) {
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
    } else if (tickMode==JKQTPLTMLin) {
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
    } else if (tickMode==JKQTPLTMPower) {
        tickSpacing=1;
        tickSpacingLog=calcLogTickSpacing();
        tickStart=pow(logAxisBase, floor(log(axismin)/log(logAxisBase)));
    }


    labelDigits=calcLinearUnitDigits();
#ifdef SHOW_JKQTPLOTTER_DEBUG
    //qDebug()<<"    tickStart="<<tickStart<<"\n";
    //qDebug()<<"    tickSpacing="<<tickSpacing<<"\n";
    //qDebug()<<"    labelDigits="<<labelDigits<<"\n";
#endif
    paramsChanged=false;

}

void JKQTPcoordinateAxis::setRange(double aamin, double aamax) {
    double oldamin=axismin;
    double oldamax=axismax;
    double amin=aamin;
    double amax=aamax;
    if (axisMinWidth>0 && fabs(amax-amin)<axisMinWidth) {
        amax=amin+axisMinWidth;
    }

    axismin=amin;
    axismax=amax;

    if (amin<axisabsoultemin) axismin=axisabsoultemin;
    if (amax>axisabsoultemax) axismax=axisabsoultemax;

    if (axismin>axismax) {
        axismin=amax;
        axismax=amin;
    }
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
        update_plot();
    }
}

void JKQTPcoordinateAxis::setNoAbsoluteRange() {
    axisabsoultemin=-DBL_MAX;
    axisabsoultemax=DBL_MAX;
    setRange(axismin, axismax);
}

void JKQTPcoordinateAxis::set_logAxis(bool __value)
{
    this->logAxis = __value;
    this->paramsChanged=true;
    if (this->isLogAxis()) {
        if (axismin<0) axismin=pow(10.0, floor(log(axismax-axismin)/log(10.0)-1.0));
        if (axismax<0) axismax=pow(10.0, floor(log(axismax-axismin)/log(10.0)+1.0));
    }
    if (this->isLogAxis() && this->minorTicks==def_minorTicks) {
        this->minorTicks=9;
    }
    update_plot();
}

void JKQTPcoordinateAxis::set_tickMode(JKQTPLabelTickMode __value)
{
    this->tickMode = __value;
    this->paramsChanged=true;
    set_logAxis(this->logAxis);
}


void JKQTPcoordinateAxis::setAbsoluteRange(double amin, double amax) {
    axisabsoultemin=amin;
    axisabsoultemax=amax;

    if (axisabsoultemin>axisabsoultemax) {
        axisabsoultemin=amax;
        axisabsoultemax=amin;
    }

    if (axisabsoultemin==axisabsoultemax) {
        axisabsoultemax=axisabsoultemin+1;
    }
    setRange(axismin, axismax);
    /*paramsChanged=true;
    calcPlotScaling();
    update_plot();*/
}

double JKQTPcoordinateAxis::getNextLabelDistance(double x) {
    if  (tickMode==JKQTPLTMLinOrPower) {
        if (logAxis) {
            return x*tickSpacingLog;
        } else {
            return tickSpacing;
        }
    } else if  (tickMode==JKQTPLTMLin) {
        return tickSpacing;
    } else if  (tickMode==JKQTPLTMPower) {
        return x*tickSpacingLog;
    }
    return 0;
}

bool JKQTPcoordinateAxis::getNextLabel(double& x, QString& label, bool init) {
    //qDebug()<<"start JKQTPcoordinateAxis::getNextLabel("<<x<<label<<init<<")";
    label="";
    if (tickLabels.size()<=0) {
        if (init) {
            x=tickStart;
            label=floattolabel(x);
            return true;
        } else {
            if  (tickMode==JKQTPLTMLinOrPower) {
                if (logAxis) {
                    x*=tickSpacingLog;
                } else {
                    x=x+tickSpacing;
                }
            } else if  (tickMode==JKQTPLTMLin) {
                x=x+tickSpacing;
            } else if  (tickMode==JKQTPLTMPower) {
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
                if (x==tickLabels[i].first && i+1<tickLabels.size()) {
                    if (x>=axismin && x<=axismax) {
                        label=tickLabels[i+1].second;
                        x=tickLabels[i+1].first;
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

QSizeF JKQTPcoordinateAxis::getMaxTickLabelSize(JKQTPEnhancedPainter& painter, double* ascent, double* descent) {
    double w=0, h=0, a=0;
    calcPlotScaling();

    double x=tickStart;
    QString label="";
    getNextLabel(x, label, true);
    QFont f;
    f.setFamily(tickLabelFont);
    f.setPointSizeF(tickLabelFontSize*parent->get_fontSizeMultiplier());
    QFontMetricsF fm(f);
    //get_parent_mathText()->set_fontSize(f.pointSizeF());
    //get_parent_mathText()->set_fontRoman(f.family());
    bool first=true;
    int cnt=0;
    while (getNextLabel(x, label, first) && cnt<50) {
        double width, ascent, descent, strikeoutPos;
        /*if (label.contains("\\") || label.contains("$") || label.contains("^") || label.contains("_")) {
            get_parent_mathText()->parse(label);
            get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
        } else {
            width=fm.width(label);
            ascent=fm.ascent();
            descent=fm.descent();
            strikeoutPos=fm.strikeOutPos();
        }*/
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
    if (tickLabelAngle!=0) {
        QRectF r(0,0,w,h);
        return jkqtpRotateRect(r, tickLabelAngle).boundingRect().size();
    } else {
        return QSizeF(w, h);
    }
}









JKQTPverticalAxis::JKQTPverticalAxis(JKQtBasePlotter* parent):
    JKQTPcoordinateAxis(parent)
{
    //axisPrefix="yaxis_";
    scaleSign=-1;
}

double JKQTPverticalAxis::get_parent_plotwidth() const {
    return parent->get_plotHeight();
}

double JKQTPverticalAxis::get_parent_plotoffset() const {
    return parent->get_iplotBorderTop();
}

QSize JKQTPverticalAxis::getSize1(JKQTPEnhancedPainter& painter) {
    if (drawMode1==JKQTPCADMnone || drawMode1==JKQTPCADMline) return QSize(0, get_parent_plotwidth());
    if (drawMode1==JKQTPCADMticks) return QSize(parent->pt2px(painter, tickOutsideLength), get_parent_plotwidth());
    // find out the maximum width over all visible plot labels
    double tlwidth=getMaxTickLabelSize(painter).width();
    if (drawMode1==JKQTPCADMticksAndLabels) return QSize(parent->pt2px(painter, tickOutsideLength+tickLabelDistance)+tlwidth, get_parent_plotwidth());

    // find out size of axis label
    /*get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(labelFont);
    get_parent_mathText()->parse(axisLabel);
    double lwidth=get_parent_mathText()->getSize(painter).height();*/
    double lwidth=parent->getTextSizeSize(labelFont, labelFontSize*parent->get_fontSizeMultiplier(), axisLabel, painter).height();
    if (drawMode1==JKQTPCADMcomplete) return QSize(parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)+tlwidth+lwidth, get_parent_plotwidth());

    return QSize(0,0);
}

QSize JKQTPverticalAxis::getSize2(JKQTPEnhancedPainter& painter) {
    if (drawMode2==JKQTPCADMnone || drawMode2==JKQTPCADMline) return QSize(0, get_parent_plotwidth());
    if (drawMode2==JKQTPCADMticks) return QSize(parent->pt2px(painter, tickOutsideLength),get_parent_plotwidth());
    // find out the maximum width over all visible plot labels
    double tlwidth=getMaxTickLabelSize(painter).width();
    if (drawMode2==JKQTPCADMticksAndLabels) return QSize(parent->pt2px(painter, tickOutsideLength+tickLabelDistance)+tlwidth, get_parent_plotwidth());

    // find out size of axis label
    /*get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(labelFont);
    get_parent_mathText()->parse(axisLabel);
    double lwidth=get_parent_mathText()->getSize(painter).height();*/
    double lwidth=parent->getTextSizeSize(labelFont, labelFontSize*parent->get_fontSizeMultiplier(), axisLabel, painter).height();
    if (drawMode2==JKQTPCADMcomplete) return QSize(parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)+tlwidth+lwidth, get_parent_plotwidth());

    return QSize(0,0);
}


double JKQTPverticalAxis::get_parent_otheraxis_offset() const {
    return parent->x2p(parent->getXMin());
}


double JKQTPverticalAxis::get_parent_otheraxis_width() const {
    return fabs(parent->x2p(parent->getXMax())-parent->x2p(parent->getXMin()));
}

bool JKQTPverticalAxis::get_parent_otheraxis_inverted() const
{
    return parent->getXAxis()->get_inverted();
}

void JKQTPverticalAxis::drawGrids(JKQTPEnhancedPainter& painter) {
    //qDebug()<<"  start JKQTPverticalAxis::drawGrids(";
    painter.save();
    QPen pg=painter.pen();
    pg.setColor(gridColor);
    pg.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, gridWidth*parent->get_lineWidthMultiplier())));
    pg.setStyle(gridStyle);
    QPen pmg=painter.pen();
    pmg.setColor(minorGridColor);
    pmg.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH,parent->pt2px(painter,  minorGridWidth*parent->get_lineWidthMultiplier())));
    pmg.setStyle(minorGridStyle);
    //double top=x2p(axismax);
    //double bottom=x2p(axismin);
    double left=0;
    double right=0;
    if (get_parent_otheraxis_inverted()) {
        left=get_parent_otheraxis_offset()-get_parent_otheraxis_width();//;
        right=get_parent_otheraxis_offset();//;
    } else {
        left=get_parent_otheraxis_offset();//;
        right=get_parent_otheraxis_offset()+get_parent_otheraxis_width();//;
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
        double mtdist=w/(double)(minorTicks+1);

        double xx=x2p(x);
        //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   minorTicks="<<minorTicks;

        //painter.setPen(pg);
        //qDebug()<<"      1";
        if (drawGrid && (x<axismax) && (x>axismin) && (fabs(left-right)>0)) {
            QLineF l(left, xx, right, xx);
            if (l.length()>0) lines_pg.append(l);//painter.drawLine(l);
        }
        //qDebug()<<"      2";
        //painter.setPen(pmg);
        //qDebug()<<"      3";
        if ((tickLabels.size()<=0) && drawMinorGrid && (minorTicks>0) && (fabs(right-left)>0)) {
            //qDebug()<<"      3.1";
            double mx=x+mtdist;
            //qDebug()<<"      3.2";
            for (int i=0; i<(int)minorTicks; i++) {
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
    painter.restore();
    //qDebug()<<"    end JKQTPverticalAxis::drawGrids(";
}

void JKQTPverticalAxis::drawTickLabel1(JKQTPEnhancedPainter &painter, double xx, double yy, const QString& label, double fontSize)
{
    get_parent_mathText()->set_fontSize(fontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(tickLabelFont);
    get_parent_mathText()->set_fontColor(axisColor);

    get_parent_mathText()->parse(label);
    double width, ascent, descent, strikeoutPos;
    get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    if (drawMode1==JKQTPCADMticksAndLabels || drawMode1==JKQTPCADMcomplete) {
        double lx=xx-parent->pt2px(painter, minorTickOutsideLength+tickLabelDistance)-width;
        if (tickLabelAngle==90) {
            painter.save();
            painter.translate(lx+width-1.25*strikeoutPos, yy-width/2.0);
            painter.rotate(tickLabelAngle);
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else if (tickLabelAngle==-90) {
            painter.save();
            painter.translate(lx+width-0.25*strikeoutPos, yy+width/2.0);
            painter.rotate(tickLabelAngle);
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else if (tickLabelAngle!=0) {
            painter.save();
            if (tickLabelAngle>0) {
                painter.translate(lx-strikeoutPos+(width)*(1.0-cos(fabs(tickLabelAngle)/180.0*M_PI)), yy+strikeoutPos-width*sin(fabs(tickLabelAngle)/180.0*M_PI));
                painter.rotate(tickLabelAngle);
            } else {
                painter.translate(lx+strikeoutPos+(width-strikeoutPos)*(1.0-cos(fabs(tickLabelAngle)/180.0*M_PI)), yy+strikeoutPos+width*sin(fabs(tickLabelAngle)/180.0*M_PI));
                painter.rotate(tickLabelAngle);
            }
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else {
            get_parent_mathText()->draw(painter, lx, yy+strikeoutPos);
        }
    }
}

void JKQTPverticalAxis::drawTickLabel2(JKQTPEnhancedPainter &painter, double xx, double yy, const QString &label, double fontSize)
{
    get_parent_mathText()->set_fontSize(fontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(tickLabelFont);
    get_parent_mathText()->set_fontColor(axisColor);

    get_parent_mathText()->parse(label);
    double width, ascent, descent, strikeoutPos;
    get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);

    if (drawMode2==JKQTPCADMticksAndLabels || drawMode2==JKQTPCADMcomplete) {
        double lx=xx+parent->pt2px(painter, tickOutsideLength+tickLabelDistance);
        if (tickLabelAngle==90) {
            painter.save();
            painter.translate(lx+descent, yy-width/2.0);
            painter.rotate(tickLabelAngle);
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else if (tickLabelAngle==-90) {
            painter.save();
            painter.translate(lx+ascent, yy+width/2.0);
            painter.rotate(tickLabelAngle);
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else if (tickLabelAngle!=0) {
            painter.save();
            double shiftx=0;
            if (tickLabelAngle>0) {
                shiftx=strikeoutPos*sin(fabs(tickLabelAngle)/180.0*M_PI);
            } else {

            }
            painter.translate(lx+shiftx, yy+strikeoutPos);
            painter.rotate(-tickLabelAngle);
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else {
            get_parent_mathText()->draw(painter, lx, yy+strikeoutPos);
        }
    }
}

void JKQTPverticalAxis::drawAxes(JKQTPEnhancedPainter& painter) {

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
    if (get_parent_otheraxis_inverted()) {
        left=get_parent_otheraxis_offset()-get_parent_otheraxis_width();//;
        right=get_parent_otheraxis_offset();//;
    } else {
        left=get_parent_otheraxis_offset();//;
        right=get_parent_otheraxis_offset()+get_parent_otheraxis_width();//;
    }
    //qDebug()<<"JKQTPverticalAxis:";
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
    //qDebug()<<"  start JKQTPverticalAxis::drawAxes(";
    //qDebug()<<"drawing vertical axis ... range = ["<<axismin<<" .. "<<axismax<<"]\n";

    painter.save();
    QPen pmain=painter.pen();
    pmain.setColor(axisColor);
    pmain.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    pmain.setStyle(Qt::SolidLine);

    QPen ptick=pmain;
    ptick.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, tickWidth*parent->get_lineWidthMultiplier())));

    QPen pmtick=ptick;
    pmtick.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, minorTickWidth*parent->get_lineWidthMultiplier())));

    get_parent_mathText()->set_fontSize(tickLabelFontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(tickLabelFont);
    get_parent_mathText()->set_fontColor(axisColor);

    painter.setPen(pmain);

    // plot thick axis at y==0
    if (showZeroAxis && (0>axismin) && (0<axismax)) {
        QPen pmain1=pmain;
        pmain1.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidthZeroAxis*parent->get_lineWidthMultiplier())));
        painter.setPen(pmain1);
        QLineF l(left-parent->pt2px(painter, tickOutsideLength), x2p(0), right+parent->pt2px(painter, tickOutsideLength), x2p(0));
        painter.drawLine(l);

        painter.setPen(pmain);
    }

    // draw thick axis lines, left and/or right
    if (drawMode1!=JKQTPCADMnone) {
        QLineF l(left, x2p(axismin), left, x2p(axismax));
        painter.drawLine(l);
    }
    if (drawMode2!=JKQTPCADMnone) {
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


    while (getNextLabel(x, label, first) && cnt<200) {
        double mtdist=getNextLabelDistance(x)/(double)(minorTicks+1);

        double xleft=-1000000;
        double xx=x2p(x);
        //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   minorTicks="<<minorTicks;

        if (drawMode1==JKQTPCADMticks || drawMode1==JKQTPCADMticksAndLabels || drawMode1==JKQTPCADMcomplete) {
            //painter.setPen(ptick);
            if ((x<=axismax) && (x>=axismin))  {
                QLineF l(left-parent->pt2px(painter, tickOutsideLength), xx, left+parent->pt2px(painter, tickInsideLength), xx);
                if (l.length()>0) lines_ptick.append(l);//painter.drawLine(l);
            }
            //painter.setPen(pmtick);
            if ((tickLabels.size()<=0) && (minorTicks>0)) {
                double mx=x+mtdist;
                int minTickCnt=minorTicks;
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
                        QLineF l(xleft=left-parent->pt2px(painter, minorTickOutsideLength), mxx, left+parent->pt2px(painter, minorTickInsideLength), mxx);
                        if (l.length()>0) lines_pmtick.append(l);//painter.drawLine(l);
                        double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                        if (minorTickLabelsEnabled) {
                            if (minorTickLabelFullNumber) val=mx;
                            QString minorlabel=floattolabel(val);
                            drawTickLabel1(painter, left, mxx, minorlabel, minorTickLabelFontSize);
                            //drawTickLabel2(painter, right, xx, minorlabel, minorTickLabelFontSize);
                        }
                    }
                    mx=mx+mtdist;

                }
            }
        }
        if (drawMode2==JKQTPCADMticks || drawMode2==JKQTPCADMticksAndLabels || drawMode2==JKQTPCADMcomplete) {
            //painter.setPen(ptick);
            if (x<=axismax && x>=axismin) {
                QLineF l(right-parent->pt2px(painter, tickInsideLength), xx, right+parent->pt2px(painter, tickOutsideLength), xx);
                if (l.length()>0) lines_ptick2.append(l);//painter.drawLine(l);
            }
            //painter.setPen(pmtick);
            if ((tickLabels.size()<=0) && (minorTicks>0)) {
                double mx=x+mtdist;
                int minTickCnt=minorTicks;
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
                        QLineF l(right-parent->pt2px(painter, minorTickInsideLength), mxx, xleft=(right+parent->pt2px(painter, minorTickOutsideLength)), mxx);
                        if (l.length()>0) lines_pmtick2.append(l);//painter.drawLine(l);
                        double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                        if (minorTickLabelsEnabled) {
                            if (minorTickLabelFullNumber) val=mx;
                            QString minorlabel=floattolabel(val);
                            //drawTickLabel1(painter, left, xx, minorlabel, minorTickLabelFontSize);
                            drawTickLabel2(painter, xleft, mxx, minorlabel, minorTickLabelFontSize);
                        }
                    }
                    mx=mx+mtdist;
                }
            }
        }

        if ((label!="") && (x<=axismax && x>=axismin) && (drawMode1==JKQTPCADMticksAndLabels || drawMode1==JKQTPCADMcomplete || drawMode2==JKQTPCADMticksAndLabels || drawMode2==JKQTPCADMcomplete)) {


            drawTickLabel1(painter, left, xx, label, tickLabelFontSize);
            drawTickLabel2(painter, right, xx, label, tickLabelFontSize);

        }
        first=false;
        cnt++;
    }
    painter.setPen(ptick);
    painter.drawLines(lines_ptick);
    painter.drawLines(lines_ptick2);
    painter.setPen(pmtick);
    painter.drawLines(lines_pmtick);
    painter.drawLines(lines_pmtick2);

    // plot axis label
    if (!axisLabel.isEmpty() && (drawMode1==JKQTPCADMcomplete)) {
        get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
        get_parent_mathText()->set_fontRoman(labelFont);
        get_parent_mathText()->parse(axisLabel);
        double width, ascent, descent, strikeoutPos;
        get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);


        QRect rect(0,0, get_parent_plotwidth(), ascent+descent);//plotBorderLeft-30);
        painter.save();
        painter.translate(QPointF(left-parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)-descent-labelMax.width()-labelMax.height(), bottom));
        painter.rotate(-90);
        //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(labelPosition) {
            case JKQTPlabelMax:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPlabelMin:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPlabelCenter:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        painter.resetTransform();
        painter.restore();
    }
    if (!axisLabel.isEmpty() && (drawMode2==JKQTPCADMcomplete)) {
        get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
        get_parent_mathText()->set_fontRoman(labelFont);
        get_parent_mathText()->parse(axisLabel);


        QRect rect(0,0, get_parent_plotwidth(), get_parent_mathText()->getSize(painter).height());//plotBorderLeft-30);
        painter.save();
        painter.translate(QPointF(right+parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)+labelMax.width(), bottom));
        painter.rotate(-90);
        //JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(labelPosition) {
            case JKQTPlabelMax:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPlabelMin:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPlabelCenter:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        //painter.resetTransform();
        painter.restore();
    }
    painter.restore();
    //qDebug()<<"    end JKQTPverticalAxis::drawAxes(";
}









JKQTPverticalIndependentAxis::JKQTPverticalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQtBasePlotter* parent):
    JKQTPverticalAxis(parent)
{
    this->axisOffset=axisOffset;
    this->axisWidth=axisWidth;
    this->otherAxisOffset=otherAxisOffset;
    this->otherAxisWidth=otherAxisWidth;
    this->otherAxisInverted=false;
}










JKQTPhorizontalAxis::JKQTPhorizontalAxis(JKQtBasePlotter* parent):
    JKQTPcoordinateAxis(parent)
{
    //axisPrefix="xaxis_";
    scaleSign=1;
}

double JKQTPhorizontalAxis::get_parent_plotwidth() const {
    return parent->get_plotWidth();
}

double JKQTPhorizontalAxis::get_parent_plotoffset() const {
    return parent->get_iplotBorderLeft();
}

QSize JKQTPhorizontalAxis::getSize1(JKQTPEnhancedPainter& painter) {
    if (drawMode1==JKQTPCADMnone || drawMode1==JKQTPCADMline) return QSize(get_parent_plotwidth(), 0);
    if (drawMode1==JKQTPCADMticks) return QSize(get_parent_plotwidth(), parent->pt2px(painter, tickOutsideLength));
    // find out the maximum width over all visible plot labels
    double tlwidth=getMaxTickLabelSize(painter).height();
    if (drawMode1==JKQTPCADMticksAndLabels) return QSize(get_parent_plotwidth(),parent->pt2px(painter, tickOutsideLength+tickLabelDistance)+tlwidth);

    // find out size of axis label
    /*get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(labelFont);
    get_parent_mathText()->parse(axisLabel);
    double lwidth=get_parent_mathText()->getSize(painter).height();*/
    double lwidth=parent->getTextSizeSize(labelFont, labelFontSize*parent->get_fontSizeMultiplier(), axisLabel, painter).height();

    if (drawMode1==JKQTPCADMcomplete) return QSize(get_parent_plotwidth(),parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)+tlwidth+lwidth);

    return QSize(0,0);
}

QSize JKQTPhorizontalAxis::getSize2(JKQTPEnhancedPainter& painter) {
    if (drawMode2==JKQTPCADMnone || drawMode2==JKQTPCADMline) return QSize(get_parent_plotwidth(), 0);
    if (drawMode2==JKQTPCADMticks) return QSize(get_parent_plotwidth(), parent->pt2px(painter, tickOutsideLength));
    // find out the maximum width over all visible plot labels
    double tlwidth=getMaxTickLabelSize(painter).height();
    if (drawMode2==JKQTPCADMticksAndLabels) return QSize(get_parent_plotwidth(),parent->pt2px(painter, tickOutsideLength+tickLabelDistance)+tlwidth);

    // find out size of axis label
    /*get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(labelFont);
    get_parent_mathText()->parse(axisLabel);
    double lwidth=get_parent_mathText()->getSize(painter).height();*/
    double lwidth=parent->getTextSizeSize(labelFont, labelFontSize*parent->get_fontSizeMultiplier(), axisLabel, painter).height();
    if (drawMode2==JKQTPCADMcomplete) return QSize(get_parent_plotwidth(),parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)+tlwidth+lwidth);

    return QSize(0,0);
}


double JKQTPhorizontalAxis::get_parent_otheraxis_offset() const {
    return parent->y2p(parent->getYMax());
}


double JKQTPhorizontalAxis::get_parent_otheraxis_width() const {
    return fabs(parent->y2p(parent->getYMax())-parent->y2p(parent->getYMin()));
}

bool JKQTPhorizontalAxis::get_parent_otheraxis_inverted() const
{
    return parent->getYAxis()->get_inverted();
}

void JKQTPhorizontalAxis::drawGrids(JKQTPEnhancedPainter& painter) {
    painter.save();
    //double right=x2p(axismax);
    //double left=x2p(axismin);
    double bottom=0;
    double top=0;
    if (get_parent_otheraxis_inverted()) {
        top=get_parent_otheraxis_offset()-get_parent_otheraxis_width();//;
        bottom=get_parent_otheraxis_offset();//;
    } else {
        top=get_parent_otheraxis_offset();//;
        bottom=get_parent_otheraxis_offset()+get_parent_otheraxis_width();//;
    }
    QPen pg=painter.pen();
    pg.setColor(gridColor);
    pg.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, gridWidth*parent->get_fontSizeMultiplier())));
    pg.setStyle(gridStyle);
    QPen pmg=painter.pen();
    pmg.setColor(minorGridColor);
    pmg.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, minorGridWidth*parent->get_lineWidthMultiplier())));
    pmg.setStyle(minorGridStyle);

    double x=tickStart;
    QString label="";
    bool first=true;
    // loop through all labels, as they are at the major ticks
    int cnt=0;
    QVector<QLineF> lines_pg;
    QVector<QLineF> lines_pmg;

    while (getNextLabel(x, label, first) && cnt<200) {
        double mtdist=getNextLabelDistance(x)/(double)(minorTicks+1);

        double xx=x2p(x);
        //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   minorTicks="<<minorTicks;

        //painter.setPen(pg);
        if (drawGrid && (x<axismax) && (x>axismin))  {
            QLineF l(xx, bottom, xx, top);
            if (l.length()>0) lines_pg.append(l);//painter.drawLine(l);
        }
        //painter.setPen(pmg);
        if ((tickLabels.size()<=0) && drawMinorGrid && (minorTicks>0)) {
            double mx=x+mtdist;
            for (int i=0; i<(int)minorTicks; i++) {
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
    painter.restore();
}


void JKQTPhorizontalAxis::drawTickLabel1(JKQTPEnhancedPainter &painter, double xx, double yy, const QString &label, double fontSize, double ascentMax, double /*descentMax*/)
{
    get_parent_mathText()->set_fontSize(fontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(tickLabelFont);
    get_parent_mathText()->set_fontColor(axisColor);

    get_parent_mathText()->parse(label);

    double width, ascent, descent, strikeoutPos;
    get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    if (drawMode1==JKQTPCADMticksAndLabels || drawMode1==JKQTPCADMcomplete) {
        double lx0=yy+parent->pt2px(painter, tickOutsideLength+tickLabelDistance);
        double lx=lx0+ascentMax;
        if (tickLabelAngle!=0) {
            painter.save();
            if (tickLabelAngle>0) {
                painter.translate(xx-fabs(ascent*sin(tickLabelAngle/180.0*M_PI)/2.0), lx0+fabs(ascent*cos(tickLabelAngle/180.0*M_PI)));
                painter.rotate(tickLabelAngle);
            } else {
                painter.translate(xx+fabs(ascent*sin(fabs(tickLabelAngle)/180.0*M_PI)/2.0)-width*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI)), lx0+ascent*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI))+fabs(width*sin(fabs(tickLabelAngle)/180.0*M_PI)));
                painter.rotate(tickLabelAngle);
            }
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else {
            get_parent_mathText()->draw(painter, xx-width/2.0, lx);
        }
    }
}

void JKQTPhorizontalAxis::drawTickLabel2(JKQTPEnhancedPainter &painter, double xx, double yy, const QString &label, double fontSize, double /*ascentMax*/, double descentMax)
{
    get_parent_mathText()->set_fontSize(fontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(tickLabelFont);
    get_parent_mathText()->set_fontColor(axisColor);

    get_parent_mathText()->parse(label);

    double width, ascent, descent, strikeoutPos;
    get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
    if (drawMode2==JKQTPCADMticksAndLabels || drawMode2==JKQTPCADMcomplete) {
        double lx0=yy-parent->pt2px(painter, tickOutsideLength+tickLabelDistance);
        double lx=lx0-descentMax;
        if (tickLabelAngle!=0) {
            painter.save();
            if (tickLabelAngle>0) {
                painter.translate(xx+fabs(descent*sin(tickLabelAngle/180.0*M_PI)/2.0), lx0-fabs(descent*cos(tickLabelAngle/180.0*M_PI)));
                painter.rotate(-tickLabelAngle);
            } else {
                painter.translate(xx-fabs(descent*sin(fabs(tickLabelAngle)/180.0*M_PI)/2.0)-width*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI)), lx0-descent*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI))-fabs(width*sin(fabs(tickLabelAngle)/180.0*M_PI)));
                painter.rotate(-tickLabelAngle);
            }
            get_parent_mathText()->draw(painter, 0,0);
            painter.restore();
        } else {
            get_parent_mathText()->draw(painter, xx-width/2.0, lx);
        }


    }
}

void JKQTPhorizontalAxis::drawAxes(JKQTPEnhancedPainter& painter) {

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
    if (get_parent_otheraxis_inverted()) {
        top=get_parent_otheraxis_offset()-get_parent_otheraxis_width();//;
        bottom=get_parent_otheraxis_offset();//;
    } else {
        top=get_parent_otheraxis_offset();//;
        bottom=get_parent_otheraxis_offset()+get_parent_otheraxis_width();//;
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

    painter.save();

    QPen pmain=painter.pen();
    pmain.setColor(axisColor);
    pmain.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->get_lineWidthMultiplier())));
    pmain.setStyle(Qt::SolidLine);

    QPen ptick=pmain;
    ptick.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, tickWidth*parent->get_lineWidthMultiplier())));

    QPen pmtick=ptick;
    pmtick.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, minorTickWidth*parent->get_lineWidthMultiplier())));

    get_parent_mathText()->set_fontSize(tickLabelFontSize*parent->get_fontSizeMultiplier());
    get_parent_mathText()->set_fontRoman(tickLabelFont);
    get_parent_mathText()->set_fontColor(axisColor);

    painter.setPen(pmain);

    // plot thick axis at y==0
    if (showZeroAxis && (0>axismin) && (0<axismax)) {
        QLineF l(x2p(0), bottom+parent->pt2px(painter, tickOutsideLength), x2p(0), top-parent->pt2px(painter, tickOutsideLength));
        painter.drawLine(l);
    }

    // draw thick axis lines, left and/or right
    if (drawMode1!=JKQTPCADMnone) {
        QLineF l(x2p(axismin), bottom, x2p(axismax), bottom);
        painter.drawLine(l);
    }
    if (drawMode2!=JKQTPCADMnone) {
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


    while (getNextLabel(x, label, first) && cnt<200) {
        double mtdist=getNextLabelDistance(x)/(double)(minorTicks+1);

        double xleft=-1000000;
        double xx=x2p(x);
        //qDebug()<<"   tick @ x="<<x<<"   label="<<label<<"   mtdist="<<mtdist<<"   minorTicks="<<minorTicks;

        if (drawMode1==JKQTPCADMticks || drawMode1==JKQTPCADMticksAndLabels || drawMode1==JKQTPCADMcomplete) {
            //painter.setPen(ptick);
            if (x<=axismax && x>=axismin)  {
                QLineF l(xx, bottom-parent->pt2px(painter, tickInsideLength), xx, bottom+parent->pt2px(painter, tickOutsideLength));
                if (l.length()>0) lines_ptick.append(l);//painter.drawLine(l);
            }
            //painter.setPen(pmtick);
            if ((tickLabels.size()<=0) && (minorTicks>0)) {
                double mx=x+mtdist;
                int minTickCnt=minorTicks;
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
                        QLineF l( mxx, bottom-parent->pt2px(painter, minorTickInsideLength), mxx, xleft=(bottom+parent->pt2px(painter, minorTickOutsideLength)));
                        if (l.length()>0) lines_pmtick.append(l);//painter.drawLine(l);

                        double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                        if (minorTickLabelsEnabled) {
                            if (minorTickLabelFullNumber) val=mx;
                            QString minorlabel=floattolabel(val);
                            drawTickLabel1(painter,  mxx, xleft, minorlabel, minorTickLabelFontSize, ascentMax, descentMax);
                            //drawTickLabel2(painter, right, xx, minorlabel, minorTickLabelFontSize, ascentMax, descentMax);
                        }

                        /*double val= mx/pow(10.0,floor(log10(mx)));
                        if (isLogAxis() && minorTickLabelsEnabled && apprLogTicks<=2 && val>1.05) {
                            painter.save();
                            QFont f;
                            f.setFamily(tickLabelFont);
                            f.setPointSizeF(minorTickLabelFontSize*parent->get_fontSizeMultiplier());
                            painter.setFont(f);
                            painter.setPen(axisColor);
                            QFontMetrics fm(f);
                            QString str=QString::number(val);
                            double wid=fm.width(str);
                            double heig=fm.ascent();
                            painter.drawText(QPointF(mxx-wid/2.0, xleft+parent->pt2px(painter, minorTickOutsideLength)/2.0+heig), str);
                            painter.restore();
                        }*/
                    }
                    mx=mx+mtdist;
                }
            }
        }
        if (drawMode2==JKQTPCADMticks || drawMode2==JKQTPCADMticksAndLabels || drawMode2==JKQTPCADMcomplete) {
            painter.setPen(ptick);
            if (x<=axismax && x>=axismin)  {
                QLineF l(xx, top-parent->pt2px(painter, tickOutsideLength), xx, top+parent->pt2px(painter, tickInsideLength));
                if (l.length()>0) lines_ptick2.append(l);//painter.drawLine(l);
            }
            painter.setPen(pmtick);
            if ((tickLabels.size()<=0) && (minorTicks>0)) {
                double mx=x+mtdist;
                int minTickCnt=minorTicks;
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
                        QLineF l( mxx, top-parent->pt2px(painter, minorTickOutsideLength), mxx, xleft=(top+parent->pt2px(painter, minorTickInsideLength)));
                        if (l.length()>0) lines_pmtick2.append(l);//painter.drawLine(l);

                        double val= mx/pow(logAxisBase,floor(log(mx)/log(logAxisBase)));

                        if (minorTickLabelsEnabled) {
                            if (minorTickLabelFullNumber) val=mx;
                            QString minorlabel=floattolabel(val);
                            drawTickLabel2(painter,  mxx, xleft, minorlabel, minorTickLabelFontSize, ascentMax, descentMax);
                            //drawTickLabel2(painter, right, xx, minorlabel, minorTickLabelFontSize, ascentMax, descentMax);
                        }

                        /*
                        double val= mx/pow(10.0,floor(log10(mx)));
                        if (isLogAxis() && minorTickLabelsEnabled && apprLogTicks<=2 && val>1.05) {
                            painter.save();
                            QFont f;
                            f.setFamily(tickLabelFont);
                            f.setPointSizeF(minorTickLabelFontSize*parent->get_fontSizeMultiplier());
                            painter.setFont(f);
                            painter.setPen(axisColor);
                            QFontMetrics fm(f);
                            QString str=QString::number(val);
                            double wid=fm.width(str);
                            double heig=fm.ascent();
                            painter.drawText(QPointF(mxx-wid/2.0, xleft-parent->pt2px(painter, minorTickOutsideLength)/2.0), str);
                            painter.restore();
                        }*/
                    }
                    mx=mx+mtdist;
                }
            }
        }

        if ((label!="") && (x<=axismax && x>=axismin) && (drawMode1==JKQTPCADMticksAndLabels || drawMode1==JKQTPCADMcomplete || drawMode2==JKQTPCADMticksAndLabels || drawMode2==JKQTPCADMcomplete)) {


            drawTickLabel1(painter, xx, bottom, label, tickLabelFontSize, ascentMax, descentMax);
            drawTickLabel2(painter, xx, top, label, tickLabelFontSize, ascentMax, descentMax);

            /*
            get_parent_mathText()->parse(label);
            double width, ascent, descent, strikeoutPos;
            get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);
            if (drawMode1==JKQTPCADMticksAndLabels || drawMode1==JKQTPCADMcomplete) {
                double lx0=bottom+parent->pt2px(painter, tickOutsideLength+tickLabelDistance);
                double lx=lx0+ascentMax;
                if (tickLabelAngle!=0) {
                    painter.save();
                    if (tickLabelAngle>0) {
                        painter.translate(xx-fabs(ascent*sin(tickLabelAngle/180.0*M_PI)/2.0), lx0+fabs(ascent*cos(tickLabelAngle/180.0*M_PI)));
                        painter.rotate(tickLabelAngle);
                    } else {
                        painter.translate(xx+fabs(ascent*sin(fabs(tickLabelAngle)/180.0*M_PI)/2.0)-width*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI)), lx0+ascent*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI))+fabs(width*sin(fabs(tickLabelAngle)/180.0*M_PI)));
                        painter.rotate(tickLabelAngle);
                    }
                    get_parent_mathText()->draw(painter, 0,0);
                    painter.restore();
                } else {
                    get_parent_mathText()->draw(painter, xx-width/2.0, lx);
                }
            }
            if (drawMode2==JKQTPCADMticksAndLabels || drawMode2==JKQTPCADMcomplete) {
                double lx0=top-parent->pt2px(painter, tickOutsideLength+tickLabelDistance);
                double lx=lx0-descentMax;
                if (tickLabelAngle!=0) {
                    painter.save();
                    if (tickLabelAngle>0) {
                        painter.translate(xx+fabs(descent*sin(tickLabelAngle/180.0*M_PI)/2.0), lx0-fabs(descent*cos(tickLabelAngle/180.0*M_PI)));
                        painter.rotate(-tickLabelAngle);
                    } else {
                        painter.translate(xx-fabs(descent*sin(fabs(tickLabelAngle)/180.0*M_PI)/2.0)-width*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI)), lx0-descent*fabs(cos(fabs(tickLabelAngle)/180.0*M_PI))-fabs(width*sin(fabs(tickLabelAngle)/180.0*M_PI)));
                        painter.rotate(-tickLabelAngle);
                    }
                    get_parent_mathText()->draw(painter, 0,0);
                    painter.restore();
                } else {
                    get_parent_mathText()->draw(painter, xx-width/2.0, lx);
                }


            }*/
        }
        first=false;
        cnt++;
    }
    painter.setPen(ptick);
    painter.drawLines(lines_ptick);
    painter.drawLines(lines_ptick2);
    painter.setPen(pmtick);
    painter.drawLines(lines_pmtick);
    painter.drawLines(lines_pmtick2);

    // plot axis label
    if (!axisLabel.isEmpty() && (drawMode1==JKQTPCADMcomplete)) {
        get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
        get_parent_mathText()->set_fontRoman(labelFont);
        get_parent_mathText()->parse(axisLabel);
        double width, ascent, descent, strikeoutPos;
        get_parent_mathText()->getSizeDetail(painter, width, ascent, descent, strikeoutPos);


        QRect rect(0,0, get_parent_plotwidth(), ascent+descent);//plotBorderLeft-30);
        painter.save();
        painter.translate(QPointF(left, bottom+parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)+labelMax.height()));
        JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(labelPosition) {
            case JKQTPlabelMax:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPlabelMin:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPlabelCenter:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        //painter.resetTransform();
        painter.restore();
    }
    if (!axisLabel.isEmpty() && (drawMode2==JKQTPCADMcomplete)) {
        get_parent_mathText()->set_fontSize(labelFontSize*parent->get_fontSizeMultiplier());
        get_parent_mathText()->set_fontRoman(labelFont);
        get_parent_mathText()->parse(axisLabel);


        QRect rect(0,0, get_parent_plotwidth(), get_parent_mathText()->getSize(painter).height());//plotBorderLeft-30);
        painter.save();
        painter.translate(QPointF(left, top-parent->pt2px(painter, tickOutsideLength+tickLabelDistance+labelDistance)-labelMax.height()-rect.height()));
        JKQTPEnhancedPainter::RenderHints h=painter.renderHints();
        //painter.drawRect(rect);
        //painter.drawEllipse(-4, -4, 8, 8);
        switch(labelPosition) {
            case JKQTPlabelMax:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignRight, rect);
                break;
            case JKQTPlabelMin:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignLeft, rect);
                break;
            case JKQTPlabelCenter:
                get_parent_mathText()->draw(painter, Qt::AlignBottom|Qt::AlignHCenter, rect);
                break;
        }
        //painter.resetTransform();
        painter.restore();
    }
#ifdef SHOW_JKQTPLOTTER_DEBUG
    QPen mp=QPen(QColor("blue"));
    mp.setWidthF(3);
    painter.setPen(mp);
    painter.drawLine(QLineF(get_parent_plotoffset(), top, get_parent_plotoffset()+get_parent_plotwidth(), bottom));
    painter.drawEllipse(QPointF(get_parent_plotoffset(), top), 5,5);
#endif
    painter.restore();
}








JKQTPhorizontalIndependentAxis::JKQTPhorizontalIndependentAxis(double axisOffset, double axisWidth, double otherAxisOffset, double otherAxisWidth, JKQtBasePlotter* parent):
    JKQTPhorizontalAxis(parent)
{
    this->axisOffset=axisOffset;
    this->axisWidth=axisWidth;
    this->otherAxisOffset=otherAxisOffset;
    this->otherAxisWidth=otherAxisWidth;
    this->otherAxisInverted=false;
}


