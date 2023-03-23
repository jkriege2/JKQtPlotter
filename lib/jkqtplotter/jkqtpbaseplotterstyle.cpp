#include "jkqtpbaseplotterstyle.h"
#include <QPalette>
#include <QApplication>
#include "jkqtplotter/jkqtptools.h"

JKQTBasePlotterStyle::JKQTBasePlotterStyle():
    debugShowRegionBoxes(false),
    debugRegionLineWidth(3),
    debugShowTextBoxes(false),
    debugTextBoxLineWidth(1),
    CSVdecimalSeparator("."),
    CSVcommentInitializer("# "),
    plotBorderTop(5),
    plotBorderLeft(5),
    plotBorderBottom(5),
    plotBorderRight(5),
    widgetBackgroundBrush(QApplication::palette().color(QPalette::Window)),
    exportBackgroundBrush(QColor("white")),
    plotBackgroundBrush(QColor("white")),
    plotFrameColor(QColor("black")),
    plotFrameWidth(2),
    plotFrameRounding(0),
    plotFrameVisible(false),
    plotLabelFontName("GUI"),
    plotLabelFontSize(12),
    useAntiAliasingForSystem(true),
    useAntiAliasingForText(true),
    defaultTextColor(QColor("black")),
    defaultFontSize(8),
    defaultFontName("GUI"),
    keyStyle(*this),
    xAxisStyle(*this),
    yAxisStyle(*this),
    secondaryAxisSeparation(6),
    rightColorbarAxisStyle(*this),
    topColorbarAxisStyle(*this),
    graphsStyle(*this)
{
}

void JKQTBasePlotterStyle::loadSettings(const QSettings &settings, const QString &group, const JKQTBasePlotterStyle &defaultStyle)
{
    CSVdecimalSeparator=settings.value(group+"csv_decimal_separator", defaultStyle.CSVdecimalSeparator).toString();
    CSVcommentInitializer=settings.value(group+"csv_comment_initializer", defaultStyle.CSVcommentInitializer).toString();
    plotBorderLeft=settings.value(group+"plot_border_left", defaultStyle.plotBorderLeft).toInt();
    plotBorderRight=settings.value(group+"plot_border_right", defaultStyle.plotBorderRight).toInt();
    plotBorderTop=settings.value(group+"plot_border_top", defaultStyle.plotBorderTop).toInt();
    plotBorderBottom=settings.value(group+"plot_border_bottom", defaultStyle.plotBorderBottom).toInt();
    debugShowRegionBoxes=settings.value(group+"debug_show_region_boxes", defaultStyle.debugShowRegionBoxes).toBool();
    debugRegionLineWidth=settings.value(group+"debug_region_linewidth", defaultStyle.debugRegionLineWidth).toDouble();
    debugShowTextBoxes=settings.value(group+"debug_show_text_boxes", defaultStyle.debugShowTextBoxes).toBool();
    debugTextBoxLineWidth=settings.value(group+"debug_textbox_linewidth", defaultStyle.debugTextBoxLineWidth).toDouble();
    plotLabelFontName=settings.value(group+"plot_label_font_name", defaultStyle.plotLabelFontName).toString();
    plotLabelFontSize=settings.value(group+"plot_label_font_size", defaultStyle.plotLabelFontSize).toDouble();
    widgetBackgroundBrush=QBrush(jkqtp_String2QColor(settings.value(group+"widget_background_color", jkqtp_QColor2String(defaultStyle.widgetBackgroundBrush.color())).toString()));
    exportBackgroundBrush=QBrush(jkqtp_String2QColor(settings.value(group+"widget_background_color_for_export", jkqtp_QColor2String(defaultStyle.exportBackgroundBrush.color())).toString()));
    plotBackgroundBrush=QBrush(jkqtp_String2QColor(settings.value(group+"plot_background_color", jkqtp_QColor2String(defaultStyle.plotBackgroundBrush.color())).toString()));
    plotFrameColor=jkqtp_String2QColor(settings.value(group+"plot_frame_color", jkqtp_QColor2String(defaultStyle.plotFrameColor)).toString());
    plotFrameWidth=settings.value(group+"plot_frame_width", defaultStyle.plotFrameWidth).toDouble();
    plotFrameVisible=settings.value(group+"plot_frame_visible", defaultStyle.plotFrameVisible).toBool();
    plotFrameRounding=settings.value(group+"plot_frame_rounded", defaultStyle.plotFrameRounding).toDouble();
    useAntiAliasingForSystem=settings.value(group+"antialiase_system", defaultStyle.useAntiAliasingForSystem).toBool();
    useAntiAliasingForText=settings.value(group+"antialiase_text", defaultStyle.useAntiAliasingForText).toBool();
    defaultTextColor=jkqtp_String2QColor(settings.value(group+"text_default_color", jkqtp_QColor2String(defaultStyle.defaultTextColor)).toString());
    defaultFontSize=settings.value(group+"text_default_size", defaultStyle.defaultFontSize).toDouble();
    defaultFontName=settings.value(group+"text_default_font_name", defaultStyle.defaultFontName).toString();
    secondaryAxisSeparation=settings.value(group+"secondary_axis_separation", defaultStyle.secondaryAxisSeparation).toDouble();

    keyStyle.loadSettings(settings, group+"key/", defaultStyle.keyStyle);
    xAxisStyle.loadSettings(settings, group+"axis_x/", defaultStyle.xAxisStyle);
    yAxisStyle.loadSettings(settings, group+"axis_y/", defaultStyle.yAxisStyle);
    rightColorbarAxisStyle.loadSettings(settings, group+"axis_colorbar_right/", defaultStyle.rightColorbarAxisStyle);
    topColorbarAxisStyle.loadSettings(settings, group+"axis_colorbar_top/", defaultStyle.topColorbarAxisStyle);
    graphsStyle.loadSettings(settings, group+"graphs/", defaultStyle.graphsStyle, *this);


}

void JKQTBasePlotterStyle::saveSettings(QSettings &settings, const QString &group) const
{

    settings.setValue(group+"csv_decimal_separator", CSVdecimalSeparator);
    settings.setValue(group+"csv_comment_initializer", CSVcommentInitializer);
    settings.setValue(group+"debug_show_region_boxes", debugShowRegionBoxes);
    settings.setValue(group+"debug_region_linewidth", debugRegionLineWidth);
    settings.setValue(group+"debug_textbox_linewidth", debugShowTextBoxes);
    settings.setValue(group+"debug_region_linewidth", debugTextBoxLineWidth);
    settings.setValue(group+"antialiase_system", useAntiAliasingForSystem);
    settings.setValue(group+"antialiase_text", useAntiAliasingForText);
    settings.setValue(group+"widget_background_color", jkqtp_QColor2String(widgetBackgroundBrush.color()));
    settings.setValue(group+"widget_background_color_for_export", jkqtp_QColor2String(exportBackgroundBrush.color()));
    settings.setValue(group+"plot_background_color", jkqtp_QColor2String(plotBackgroundBrush.color()));
    settings.setValue(group+"plot_border_left", plotBorderLeft);
    settings.setValue(group+"plot_border_right", plotBorderRight);
    settings.setValue(group+"plot_border_top", plotBorderTop);
    settings.setValue(group+"plot_border_bottom", plotBorderBottom);
    settings.setValue(group+"text_default_color", jkqtp_QColor2String(defaultTextColor));
    settings.setValue(group+"text_default_size", defaultFontSize);
    settings.setValue(group+"text_default_font_name", defaultFontName);
    settings.setValue(group+"plot_label_font_name", plotLabelFontName);
    settings.setValue(group+"plot_label_font_size", plotLabelFontSize);
    settings.setValue(group+"plot_frame_visible", plotFrameVisible);
    settings.setValue(group+"plot_frame_color", jkqtp_QColor2String(plotFrameColor));
    settings.setValue(group+"plot_frame_width", plotFrameWidth);
    settings.setValue(group+"plot_frame_rounded", plotFrameRounding);
    settings.setValue(group+"secondary_axis_separation", secondaryAxisSeparation);

    keyStyle.saveSettings(settings, group+"key/");
    xAxisStyle.saveSettings(settings, group+"axis_x/");
    yAxisStyle.saveSettings(settings, group+"axis_y/");
    rightColorbarAxisStyle.saveSettings(settings, group+"axis_colorbar_right/");
    topColorbarAxisStyle.saveSettings(settings, group+"axis_colorbar_top/");
    graphsStyle.saveSettings(settings, group+"graphs/");

}

JKQTBasePlotterStyle &JKQTPGetSystemDefaultBaseStyle()
{
    static JKQTBasePlotterStyle instance=JKQTBasePlotterStyle();
    return instance;
}

void JKQTPSetSystemDefaultBaseStyle(JKQTBasePlotterStyle &newStyle)
{
    JKQTPGetSystemDefaultBaseStyle().operator=(newStyle);
}
