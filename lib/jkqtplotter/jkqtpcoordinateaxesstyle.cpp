#include "jkqtpcoordinateaxesstyle.h"
#include "jkqtpbaseplotterstyle.h"
#include "jkqtpbaseplotter.h"
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
    tickLabelType(JKQTPCALTexponent),
    tickMode(JKQTPLTMLinOrPower),
    labelPosition(JKQTPLabelCenter),
    labelFontSize(10),
    tickLabelFontSize(10),
    minorTickLabelFontSize(8),
    showZeroAxis(true),
    minorTickLabelFullNumber(true),
    drawMode1(JKQTPCADMcomplete),
    drawMode2(JKQTPCADMLineTicks),
    drawMode0(JKQTPCADMnone),
    minorTickColor(QColor("black")),
    minorTickLabelColor(QColor("black")),
    minorTickWidth(1),
    tickColor(QColor("black")),
    tickLabelColor(QColor("black")),
    tickWidth(1.5),
    lineWidth(1.5),
    arrowSizeFactor(8),
    lineWidthZeroAxis(1.5),
    tickTimeFormat(QLocale().timeFormat(QLocale::NarrowFormat)),
    tickDateFormat(QLocale().dateFormat(QLocale::NarrowFormat)),
    tickDateTimeFormat(QLocale().dateTimeFormat(QLocale::NarrowFormat)),
    tickPrintfFormat("%f"),
    minTicks(5),
    minorTicks(1),
    tickOutsideLength(3),
    minorTickOutsideLength(1.5),
    tickInsideLength(3),
    minorTickInsideLength(1.5),
    axisColor(QColor("black")),
    labelColor(QColor("black")),
    tickLabelDistance(4),
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
    arrowSizeFactor = settings.value(group+"arrow_size_factor", defaultStyle.arrowSizeFactor).toDouble();
    lineWidthZeroAxis = settings.value(group+"zero_line/line_width", defaultStyle.lineWidthZeroAxis).toDouble();
    labelFontSize = settings.value(group+"axis_label/font_size", defaultStyle.labelFontSize).toDouble();
    tickLabelFontSize = settings.value(group+"ticks/label_font_size", defaultStyle.tickLabelFontSize).toDouble();
    minorTickLabelFontSize = settings.value(group+"minor_tick/label_font_size", defaultStyle.minorTickLabelFontSize).toDouble();
    minorTickLabelFullNumber = settings.value(group+"minor_tick/label_full_number", defaultStyle.minorTickLabelFullNumber).toBool();
    tickTimeFormat = settings.value(group+"ticks/time_format", defaultStyle.tickTimeFormat).toString();
    tickDateFormat = settings.value(group+"ticks/date_format", defaultStyle.tickDateFormat).toString();
    tickDateTimeFormat = settings.value(group+"ticks/datetime_format", defaultStyle.tickDateTimeFormat).toString();
    tickPrintfFormat = settings.value(group+"ticks/printf_format", defaultStyle.tickPrintfFormat).toString();
    minTicks = settings.value(group+"min_ticks", defaultStyle.minTicks).toUInt();
    minorTicks = settings.value(group+"minor_tick/count", defaultStyle.minorTicks).toUInt();
    tickOutsideLength = settings.value(group+"ticks/outside_length", defaultStyle.tickOutsideLength).toDouble();
    minorTickOutsideLength = settings.value(group+"minor_tick/outside_length", defaultStyle.minorTickOutsideLength).toDouble();
    tickInsideLength = settings.value(group+"ticks/inside_length", defaultStyle.tickInsideLength).toDouble();
    minorTickInsideLength = settings.value(group+"minor_tick/inside_length", defaultStyle.minorTickInsideLength).toDouble();
    tickLabelDistance = settings.value(group+"ticks/label_distance", defaultStyle.tickLabelDistance).toDouble();
    labelDistance = settings.value(group+"axis_label/distance", defaultStyle.labelDistance).toDouble();
    labelPosition=String2JKQTPLabelPosition(settings.value(group+"axis_label/position", JKQTPLabelPosition2String(defaultStyle.labelPosition)).toString());
    tickLabelType=String2JKQTPCALabelType(settings.value(group+"ticks/type", JKQTPCALabelType2String(defaultStyle.tickLabelType)).toString());
    labelColor=jkqtp_String2QColor(settings.value(group+"axis_label/color", jkqtp_QColor2String(defaultStyle.labelColor)).toString());
    axisColor=jkqtp_String2QColor(settings.value(group+"color", jkqtp_QColor2String(defaultStyle.axisColor)).toString());
    tickColor=jkqtp_String2QColor(settings.value(group+"ticks/color", jkqtp_QColor2String(defaultStyle.tickColor)).toString());
    minorTickColor=jkqtp_String2QColor(settings.value(group+"minor_tick/color", jkqtp_QColor2String(defaultStyle.minorTickColor)).toString());
    tickLabelColor=jkqtp_String2QColor(settings.value(group+"ticks/label_color", jkqtp_QColor2String(defaultStyle.tickLabelColor)).toString());
    minorTickLabelColor=jkqtp_String2QColor(settings.value(group+"minor_tick/label_color", jkqtp_QColor2String(defaultStyle.minorTickLabelColor)).toString());
    drawMode1=String2JKQTPCADrawMode(settings.value(group+"draw_mode1", JKQTPCADrawMode2String(defaultStyle.drawMode1)).toString());
    drawMode2=String2JKQTPCADrawMode(settings.value(group+"draw_mode2", JKQTPCADrawMode2String(defaultStyle.drawMode2)).toString());
    drawMode0=String2JKQTPCADrawMode(settings.value(group+"draw_mode0", JKQTPCADrawMode2String(defaultStyle.drawMode0)).toString());
    tickMode=String2JKQTPLabelTickMode(settings.value(group+"ticks/mode", JKQTPLabelTickMode2String(defaultStyle.tickMode)).toString());
    colorZeroAxis=jkqtp_String2QColor(settings.value(group+"zero_line/color", jkqtp_QColor2String(defaultStyle.colorZeroAxis)).toString());
    styleZeroAxis=jkqtp_String2QPenStyle(settings.value(group+"zero_line/style", jkqtp_QPenStyle2String(defaultStyle.styleZeroAxis)).toString());
    axisLineOffset = settings.value(group+"axis_lines_offset", defaultStyle.axisLineOffset).toDouble();
    majorGridStyle.loadSettings(settings, group+"grid/", defaultStyle.majorGridStyle);
    minorGridStyle.loadSettings(settings, group+"minor_grid/", defaultStyle.minorGridStyle);
}

void JKQTPCoordinateAxisStyle::saveSettings(QSettings &settings, const QString &group) const
{
    settings.setValue(group+"color", jkqtp_QColor2String(axisColor));
    settings.setValue(group+"draw_mode1", JKQTPCADrawMode2String(drawMode1));
    settings.setValue(group+"draw_mode2", JKQTPCADrawMode2String(drawMode2));
    settings.setValue(group+"draw_mode0", JKQTPCADrawMode2String(drawMode0));
    settings.setValue(group+"line_width", lineWidth);
    settings.setValue(group+"arrow_size_factor", arrowSizeFactor);
    settings.setValue(group+"axis_lines_offset", axisLineOffset);
    settings.setValue(group+"min_ticks", minTicks);
    settings.setValue(group+"axis_label/distance", labelDistance);
    settings.setValue(group+"axis_label/font_size", labelFontSize);
    settings.setValue(group+"axis_label/color", jkqtp_QColor2String(labelColor));
    settings.setValue(group+"axis_label/position", JKQTPLabelPosition2String(labelPosition));
    settings.setValue(group+"minor_tick/labels_enabled", minorTickLabelsEnabled);
    settings.setValue(group+"minor_tick/inside_length", minorTickInsideLength);
    settings.setValue(group+"minor_tick/label_font_size", minorTickLabelFontSize);
    settings.setValue(group+"minor_tick/label_full_number", minorTickLabelFullNumber);
    settings.setValue(group+"minor_tick/label_color", jkqtp_QColor2String(minorTickLabelColor));
    settings.setValue(group+"minor_tick/outside_length", minorTickOutsideLength);
    settings.setValue(group+"minor_tick/width", minorTickWidth);
    settings.setValue(group+"minor_tick/count", minorTicks);
    settings.setValue(group+"minor_tick/color", jkqtp_QColor2String(minorTickColor));
    settings.setValue(group+"ticks/type", JKQTPCALabelType2String(tickLabelType));
    settings.setValue(group+"ticks/date_format", tickDateFormat);
    settings.setValue(group+"ticks/datetime_format", tickDateTimeFormat);
    settings.setValue(group+"ticks/printf_format", tickPrintfFormat);
    settings.setValue(group+"ticks/inside_length", tickInsideLength);
    settings.setValue(group+"ticks/label_distance", tickLabelDistance);
    settings.setValue(group+"ticks/label_font_size", tickLabelFontSize);
    settings.setValue(group+"ticks/label_color", jkqtp_QColor2String(tickLabelColor));
    settings.setValue(group+"ticks/mode", JKQTPLabelTickMode2String(tickMode));
    settings.setValue(group+"ticks/outside_length", tickOutsideLength);
    settings.setValue(group+"ticks/time_format", tickTimeFormat);
    settings.setValue(group+"ticks/width", tickWidth);
    settings.setValue(group+"ticks/color", jkqtp_QColor2String(tickColor));
    settings.setValue(group+"zero_line/enabled", showZeroAxis);
    settings.setValue(group+"zero_line/line_width", lineWidthZeroAxis);
    settings.setValue(group+"zero_line/color",  jkqtp_QColor2String(colorZeroAxis));
    settings.setValue(group+"zero_line/style", jkqtp_QPenStyle2String(styleZeroAxis));
    majorGridStyle.saveSettings(settings, group+"grid/");
    minorGridStyle.saveSettings(settings, group+"minor_grid/");
}

QPen JKQTPCoordinateAxisStyle::getZeroAxisPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter *parent) const
{
    QPen pmain(colorZeroAxis);
    pmain.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidthZeroAxis*parent->getLineWidthMultiplier())));
    pmain.setStyle(styleZeroAxis);
    pmain.setCapStyle(Qt::SquareCap);
    return pmain;
}

QPen JKQTPCoordinateAxisStyle::getAxisPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter *parent) const {
    QPen pmain(axisColor);
    pmain.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));
    pmain.setStyle(Qt::SolidLine);
    pmain.setCapStyle(Qt::SquareCap);
    return pmain;
}

QPen JKQTPCoordinateAxisStyle::getTickPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const {
    QPen ptick=getAxisPen(painter, parent);
    ptick.setColor(tickColor);
    ptick.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, tickWidth*parent->getLineWidthMultiplier())));
    ptick.setCapStyle(Qt::FlatCap);
    return ptick;
}

QPen JKQTPCoordinateAxisStyle::getMinorTickPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter *parent) const {
    QPen pmtick=getTickPen(painter, parent);
    pmtick.setColor(minorTickColor);
    pmtick.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, minorTickWidth*parent->getLineWidthMultiplier())));
    pmtick.setCapStyle(Qt::FlatCap);
    return pmtick;
}

double JKQTPCoordinateAxisStyle::getArrowSize(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const
{
    const QPen paxis=getAxisPen(painter, parent);
    return JKQTPLineDecoratorStyleCalcDecoratorSize(paxis.widthF(), arrowSizeFactor);
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
    drawMode0=JKQTPCADMnone;
    drawMode1=JKQTPCADMLine;
    drawMode2=JKQTPCADMcomplete;
    minTicks=5;
    minorTicks=0;
    tickOutsideLength=0;
    minorTickOutsideLength=0;
    showZeroAxis=false;
}
