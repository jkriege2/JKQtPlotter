#include "jkqtpcoordinateaxesstyle.h"
#include "jkqtpbaseplotterstyle.h"
#include <QApplication>


JKQTPGridStyle::JKQTPGridStyle(bool isMajor):
    enabled(isMajor),
    lineColor(QColor("gray")),
    lineWidth(isMajor?0.75:0.5),
    lineStyle(isMajor?Qt::DashLine:Qt::DotLine)
{

}

void JKQTPGridStyle::loadSettings(const QSettings &settings, const QString &group, const JKQTPGridStyle &defaultStyle)
{
    lineWidth = settings.value(group+"width", defaultStyle.lineWidth).toDouble();
    enabled = settings.value(group+"enabled", defaultStyle.enabled).toBool();
    lineColor=jkqtp_String2QColor(settings.value(group+"color", jkqtp_QColor2String(lineColor)).toString());
    lineStyle=jkqtp_String2QPenStyle(settings.value(group+"style", jkqtp_QPenStyle2String(lineStyle)).toString());
}

void JKQTPGridStyle::saveSettings(QSettings &settings, const QString &group) const
{
    settings.setValue(group+"enabled", enabled);
    settings.setValue(group+"color", jkqtp_QColor2String(lineColor));
    settings.setValue(group+"width", lineWidth);
    settings.setValue(group+"style", jkqtp_QPenStyle2String(lineStyle));
}


JKQTPCoordinateAxisStyle::JKQTPCoordinateAxisStyle():
    labelDigits(3),
    autoLabelDigits(true),
    minorTickLabelsEnabled(false),
    labelType(JKQTPCALTexponent),
    tickMode(JKQTPLTMLinOrPower),
    labelPosition(JKQTPLabelCenter),
    labelFontSize(10),
    tickLabelFontSize(10),
    minorTickLabelFontSize(8),
    showZeroAxis(true),
    minorTickLabelFullNumber(true),
    drawMode1(JKQTPCADMcomplete),
    drawMode2(JKQTPCADMLineTicks),
    minorTickWidth(1),
    tickWidth(1.5),
    lineWidth(1.5),
    lineWidthZeroAxis(1.5),
    tickTimeFormat(QLocale().timeFormat(QLocale::NarrowFormat)),
    tickDateFormat(QLocale().dateFormat(QLocale::NarrowFormat)),
    tickDateTimeFormat(QLocale().dateTimeFormat(QLocale::NarrowFormat)),
    minTicks(5),
    minorTicks(1),
    tickOutsideLength(3),
    minorTickOutsideLength(1.5),
    tickInsideLength(3),
    minorTickInsideLength(1.5),
    axisColor(QColor("black")),
    tickLabelDistance(3),
    labelDistance(5),
    tickLabelAngle(0),
    majorGridStyle(true),
    minorGridStyle(false),
    colorZeroAxis(QColor("black")),
    styleZeroAxis(Qt::SolidLine),
    axisLineOffset(0)
{

}

JKQTPCoordinateAxisStyle::JKQTPCoordinateAxisStyle(const JKQTBasePlotterStyle &baseStyle):
    JKQTPCoordinateAxisStyle()
{
    labelFontSize=baseStyle.defaultFontSize;
    tickLabelFontSize=baseStyle.defaultFontSize;
    minorTickLabelFontSize=baseStyle.defaultFontSize*0.8;

}

void JKQTPCoordinateAxisStyle::loadSettings(const QSettings &settings, const QString &group, const JKQTPCoordinateAxisStyle &defaultStyle)
{
    showZeroAxis = settings.value(group+"zero_line/enabled", defaultStyle.showZeroAxis).toBool();
    minorTickLabelsEnabled = settings.value(group+"minor_tick/labels_enabled", defaultStyle.minorTickLabelsEnabled).toBool();
    minorTickWidth = settings.value(group+"minor_tick/width", defaultStyle.minorTickWidth).toDouble();
    tickWidth = settings.value(group+"ticks/width", defaultStyle.tickWidth).toDouble();
    lineWidth = settings.value(group+"line_width", defaultStyle.lineWidth).toDouble();
    lineWidthZeroAxis = settings.value(group+"zero_line/line_width", defaultStyle.lineWidthZeroAxis).toDouble();
    labelFontSize = settings.value(group+"axis_label/font_size", defaultStyle.labelFontSize).toDouble();
    tickLabelFontSize = settings.value(group+"ticks/label_font_size", defaultStyle.tickLabelFontSize).toDouble();
    minorTickLabelFontSize = settings.value(group+"minor_tick/label_font_size", defaultStyle.minorTickLabelFontSize).toDouble();
    minorTickLabelFullNumber = settings.value(group+"minor_tick/label_full_number", defaultStyle.minorTickLabelFullNumber).toBool();
    tickTimeFormat = settings.value(group+"ticks/time_format", defaultStyle.tickTimeFormat).toString();
    tickDateFormat = settings.value(group+"ticks/date_format", defaultStyle.tickDateFormat).toString();
    tickDateTimeFormat = settings.value(group+"ticks/datetime_format", defaultStyle.tickDateTimeFormat).toString();
    minTicks = settings.value(group+"min_ticks", defaultStyle.minTicks).toUInt();
    minorTicks = settings.value(group+"minor_tick/count", defaultStyle.minorTicks).toUInt();
    tickOutsideLength = settings.value(group+"ticks/outside_length", defaultStyle.tickOutsideLength).toDouble();
    minorTickOutsideLength = settings.value(group+"minor_tick/outside_length", defaultStyle.minorTickOutsideLength).toDouble();
    tickInsideLength = settings.value(group+"ticks/inside_length", defaultStyle.tickInsideLength).toDouble();
    minorTickInsideLength = settings.value(group+"minor_tick/inside_length", defaultStyle.minorTickInsideLength).toDouble();
    tickLabelDistance = settings.value(group+"ticks/label_distance", defaultStyle.tickLabelDistance).toDouble();
    labelDistance = settings.value(group+"axis_label/distance", defaultStyle.labelDistance).toDouble();
    labelPosition=String2JKQTPLabelPosition(settings.value(group+"axis_label/position", JKQTPLabelPosition2String(labelPosition)).toString());
    labelType=String2JKQTPCALabelType(settings.value(group+"axis_label/type", JKQTPCALabelType2String(labelType)).toString());
    axisColor=jkqtp_String2QColor(settings.value(group+"color", jkqtp_QColor2String(axisColor)).toString());
    drawMode1=String2JKQTPCADrawMode(settings.value(group+"draw_mode1", JKQTPCADrawMode2String(drawMode1)).toString());
    drawMode2=String2JKQTPCADrawMode(settings.value(group+"draw_mode2", JKQTPCADrawMode2String(drawMode2)).toString());
    tickMode=String2JKQTPLabelTickMode(settings.value(group+"ticks/mode", JKQTPLabelTickMode2String(tickMode)).toString());
    colorZeroAxis=jkqtp_String2QColor(settings.value(group+"zero_line/color", jkqtp_QColor2String(colorZeroAxis)).toString());
    styleZeroAxis=jkqtp_String2QPenStyle(settings.value(group+"zero_line/style", jkqtp_QPenStyle2String(styleZeroAxis)).toString());
    axisLineOffset = settings.value(group+"axis_lines_offset", defaultStyle.axisLineOffset).toDouble();
    majorGridStyle.loadSettings(settings, group+"grid/", defaultStyle.majorGridStyle);
    minorGridStyle.loadSettings(settings, group+"minor_grid/", defaultStyle.minorGridStyle);
}

void JKQTPCoordinateAxisStyle::saveSettings(QSettings &settings, const QString &group) const
{
    settings.setValue(group+"color", jkqtp_QColor2String(axisColor));
    settings.setValue(group+"draw_mode1", JKQTPCADrawMode2String(drawMode1));
    settings.setValue(group+"draw_mode2", JKQTPCADrawMode2String(drawMode2));
    settings.setValue(group+"line_width", lineWidth);
    settings.setValue(group+"axis_lines_offset", axisLineOffset);
    settings.setValue(group+"min_ticks", minTicks);
    settings.setValue(group+"axis_label/distance", labelDistance);
    settings.setValue(group+"axis_label/font_size", labelFontSize);
    settings.setValue(group+"axis_label/position", JKQTPLabelPosition2String(labelPosition));
    settings.setValue(group+"axis_label/type", JKQTPCALabelType2String(labelType));
    settings.setValue(group+"minor_tick/labels_enabled", minorTickLabelsEnabled);
    settings.setValue(group+"minor_tick/inside_length", minorTickInsideLength);
    settings.setValue(group+"minor_tick/label_font_size", minorTickLabelFontSize);
    settings.setValue(group+"minor_tick/label_full_number", minorTickLabelFullNumber);
    settings.setValue(group+"minor_tick/outside_length", minorTickOutsideLength);
    settings.setValue(group+"minor_tick/width", minorTickWidth);
    settings.setValue(group+"minor_tick/count", minorTicks);
    settings.setValue(group+"ticks/date_format", tickDateFormat);
    settings.setValue(group+"ticks/datetime_format", tickDateTimeFormat);
    settings.setValue(group+"ticks/inside_length", tickInsideLength);
    settings.setValue(group+"ticks/label_distance", tickLabelDistance);
    settings.setValue(group+"ticks/label_font_size", tickLabelFontSize);
    settings.setValue(group+"ticks/mode", JKQTPLabelTickMode2String(tickMode));
    settings.setValue(group+"ticks/outside_length", tickOutsideLength);
    settings.setValue(group+"ticks/time_format", tickTimeFormat);
    settings.setValue(group+"ticks/width", tickWidth);
    settings.setValue(group+"zero_line/enabled", showZeroAxis);
    settings.setValue(group+"zero_line/line_width", lineWidthZeroAxis);
    settings.setValue(group+"zero_line/color",  jkqtp_QColor2String(colorZeroAxis));
    settings.setValue(group+"zero_line/style", jkqtp_QPenStyle2String(styleZeroAxis));
    majorGridStyle.saveSettings(settings, group+"grid/");
    minorGridStyle.saveSettings(settings, group+"minor_grid/");
}


JKQTPColorbarCoordinateAxisStyle::JKQTPColorbarCoordinateAxisStyle():
    JKQTPCoordinateAxisStyle()
{
    initMembersForColorbars();
}


JKQTPColorbarCoordinateAxisStyle::JKQTPColorbarCoordinateAxisStyle(const JKQTBasePlotterStyle &baseStyle):
    JKQTPCoordinateAxisStyle(baseStyle)
{
    initMembersForColorbars();
}

void JKQTPColorbarCoordinateAxisStyle::initMembersForColorbars() {
    drawMode1=JKQTPCADMLine;
    drawMode2=JKQTPCADMcomplete;
    minTicks=5;
    minorTicks=0;
    tickOutsideLength=0;
    minorTickOutsideLength=0;
    showZeroAxis=false;
}
