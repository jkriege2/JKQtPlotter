#include "jkqtpbaseplotterstyle.h"
#include <QPalette>
#include <QApplication>
#include "jkqtplottertools/jkqtptools.h"

JKQTBasePlotterStyle::JKQTBasePlotterStyle():
    debugShowRegionBoxes(false),
    debugRegionLineWidth(3),
    CSVdecimalSeparator("."),
    CSVcommentInitializer("# "),
    plotBorderTop(5),
    plotBorderLeft(5),
    plotBorderBottom(5),
    plotBorderRight(5),
    defaultGraphWidth(2),
    defaultGraphSymbolSize(10),
    defaultGraphSymbolLineWidth(1),
    widgetBackgroundBrush(QApplication::palette().color(QPalette::Window)),
    exportBackgroundBrush(QColor("white")),
    plotBackgroundBrush(QColor("white")),
    plotFrameColor(QColor("black")),
    plotFrameWidth(2),
    plotFrameRounding(0),
    plotFrameVisible(false),
    plotLabelFontName(QApplication::font().family()+"+XITS"),
    plotLabelFontSize(12),
    useAntiAliasingForSystem(true),
    useAntiAliasingForGraphs(true),
    useAntiAliasingForText(true),
    defaultTextColor(QColor("black")),
    defaultFontSize(8),
    defaultFontName(QApplication::font().family()+"+XITS"),
    defaultPalette(JKQTPMathImageMATLAB),
    graphFillColorDerivationMode(JKQTPFFCMLighterColor),
    graphErrorColorDerivationMode(JKQTPFFCMDarkerColor),
    graphErrorFillColorDerivationMode(JKQTPFFCMEvenLighterColor),
    keyStyle(*this),
    xAxisStyle(*this),
    yAxisStyle(*this),
    rightColorbarAxisStyle(*this),
    topColorbarAxisStyle(*this)
{
    /*
    keyStyle=JKQTPKeyStyle(*this);
    xAxisStyle=JKQTPCoordinateAxisStyle(*this);
    yAxisStyle=JKQTPCoordinateAxisStyle(*this);
    rightColorbarAxisStyle=JKQTPCoordinateAxisStyle(*this);
    topColorbarAxisStyle=JKQTPCoordinateAxisStyle(*this);
    */

    defaultGraphColors<<QColor("red")<<QColor("green")<<QColor("blue")<<QColor("fuchsia")<<QColor("darkorange")
                      <<QColor("navy")<<QColor("firebrick")<<QColor("darkgreen")<<QColor("darkmagenta")
                      <<QColor("darkgreen")<<QColor("darkslateblue")<<QColor("maroon")<<QColor("indianred")
                     <<QColor("darkolivegreen")<<QColor("mediumpurple")<<QColor("darkcyan");
    defaultGraphPenStyles<<Qt::SolidLine<<Qt::DashLine<<Qt::DotLine<<Qt::DashDotLine<<Qt::DashDotDotLine;
    for (int i=2; i<=JKQTPMaxSymbolID; i++) defaultGraphSymbols.push_back(static_cast<JKQTPGraphSymbols>(i));
    defaultGraphFillStyles<<Qt::SolidPattern;
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
    plotLabelFontName=settings.value(group+"plot_label_font_name", defaultStyle.plotLabelFontName).toString();
    plotLabelFontSize=settings.value(group+"plot_label_font_size", defaultStyle.debugRegionLineWidth).toDouble();
    widgetBackgroundBrush=QBrush(jkqtp_String2QColor(settings.value(group+"widget_background_color", jkqtp_QColor2String(defaultStyle.widgetBackgroundBrush.color())).toString()));
    exportBackgroundBrush=QBrush(jkqtp_String2QColor(settings.value(group+"widget_background_color_for_export", jkqtp_QColor2String(defaultStyle.exportBackgroundBrush.color())).toString()));
    plotBackgroundBrush=QBrush(jkqtp_String2QColor(settings.value(group+"plot_background_color", jkqtp_QColor2String(defaultStyle.plotBackgroundBrush.color())).toString()));
    plotFrameColor=jkqtp_String2QColor(settings.value(group+"plot_frame_color", jkqtp_QColor2String(defaultStyle.plotFrameColor)).toString());
    plotFrameWidth=settings.value(group+"plot_frame_width", defaultStyle.plotFrameWidth).toDouble();
    plotFrameVisible=settings.value(group+"plot_frame_visible", defaultStyle.plotFrameVisible).toBool();
    plotFrameRounding=settings.value(group+"plot_frame_rounded", defaultStyle.plotFrameRounding).toDouble();
    useAntiAliasingForSystem=settings.value(group+"antialiase_system", defaultStyle.useAntiAliasingForSystem).toBool();
    useAntiAliasingForGraphs=settings.value(group+"antialiase_graphs", defaultStyle.useAntiAliasingForGraphs).toBool();
    useAntiAliasingForText=settings.value(group+"antialiase_text", defaultStyle.useAntiAliasingForText).toBool();
    defaultTextColor=jkqtp_String2QColor(settings.value(group+"text_default_color", jkqtp_QColor2String(defaultStyle.defaultTextColor)).toString());
    defaultFontSize=settings.value(group+"text_default_size", defaultStyle.defaultFontSize).toDouble();
    defaultFontName=settings.value(group+"text_default_font_name", defaultStyle.defaultFontName).toString();
    defaultPalette=String2JKQTPMathImageColorPalette(settings.value(group+"graph_default_palette", JKQTPMathImageColorPalette2String(defaultStyle.defaultPalette)).toString());
    defaultGraphWidth=settings.value(group+"graph_default_linewidth", defaultStyle.debugRegionLineWidth).toDouble();
    defaultGraphSymbolSize=settings.value(group+"graph_default_symbolsize", defaultStyle.defaultGraphSymbolSize).toDouble();
    defaultGraphSymbolLineWidth=settings.value(group+"graph_default_symbol_linewidtht", defaultStyle.defaultGraphSymbolLineWidth).toDouble();
    graphFillColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"graph_default_fill_color_mode", JKQTPColorDerivationMode2String(defaultStyle.graphFillColorDerivationMode)).toString());
    graphErrorFillColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"graph_default_error_fill_color_mode", JKQTPColorDerivationMode2String(defaultStyle.graphErrorFillColorDerivationMode)).toString());
    graphErrorColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"graph_default_error_color_mode", JKQTPColorDerivationMode2String(defaultStyle.graphErrorColorDerivationMode)).toString());

    keyStyle.loadSettings(settings, group+"key/", defaultStyle.keyStyle);
    xAxisStyle.loadSettings(settings, group+"axis_x/", defaultStyle.xAxisStyle);
    yAxisStyle.loadSettings(settings, group+"axis_y/", defaultStyle.yAxisStyle);
    rightColorbarAxisStyle.loadSettings(settings, group+"axis_colorbar_right/", defaultStyle.rightColorbarAxisStyle);
    topColorbarAxisStyle.loadSettings(settings, group+"axis_colorbar_top/", defaultStyle.topColorbarAxisStyle);


    auto readID=[](const QString& k, const QString& start) {
        if (k.startsWith(start)) {
            QString kk=k;
            kk.remove(0, start.size());
            QString num="";
            while (kk.front().isDigit()) {
                num+=kk.front();
                kk.remove(0, 1);
            }
            bool ok=false;
            int id=num.toInt(&ok);
            if (ok) {
                return id;
            }
        }
        return -1;
    };

    defaultGraphColors.clear();
    defaultGraphPenStyles.clear();
    defaultGraphSymbols.clear();
    defaultGraphFillStyles.clear();
    QStringList allk=settings.allKeys();
    for (auto& k: allk) {
        int id=readID(k, group+"default_graph_styles/color");
        if (id>=0) {
            defaultGraphColors.push_back(jkqtp_String2QColor(settings.value(group+"default_graph_styles/color"+QString::number(id), jkqtp_QColor2String(QColor("red"))).toString()));
        }

        id=readID(k, group+"default_graph_styles/line_style");
        if (id>=0) {
            defaultGraphPenStyles.push_back(jkqtp_String2QPenStyle(settings.value(group+"default_graph_styles/line_style"+QString::number(id), jkqtp_QPenStyle2String(Qt::SolidLine)).toString()));
        }

        id=readID(k, group+"default_graph_styles/symbol");
        if (id>=0) {
            defaultGraphSymbols.push_back(String2JKQTPGraphSymbols(settings.value(group+"default_graph_styles/symbol"+QString::number(id), JKQTPGraphSymbols2String(JKQTPCross)).toString()));
        }
        id=readID(k, group+"default_graph_styles/fill_style");
        if (id>=0) {
            defaultGraphFillStyles.push_back(jkqtp_String2QBrushStyle(settings.value(group+"default_graph_styles/fill_style"+QString::number(id), jkqtp_QBrushStyle2String(Qt::SolidPattern)).toString()));
        }
    }
    if (defaultGraphColors.size()==0) {
        for (int i=defaultGraphColors.size(); i<defaultStyle.defaultGraphColors.size(); i++) {
            defaultGraphColors.push_back(defaultStyle.defaultGraphColors[i]);
        }
    }
    if (defaultGraphPenStyles.size()==0) {
        for (int i=defaultGraphPenStyles.size(); i<defaultStyle.defaultGraphPenStyles.size(); i++) {
            defaultGraphPenStyles.push_back(defaultStyle.defaultGraphPenStyles[i]);
        }
    }
    if (defaultGraphSymbols.size()==0) {
        for (int i=defaultGraphSymbols.size(); i<defaultStyle.defaultGraphSymbols.size(); i++) {
            defaultGraphSymbols.push_back(defaultStyle.defaultGraphSymbols[i]);
        }
    }
    if (defaultGraphFillStyles.size()==0) {
        for (int i=defaultGraphFillStyles.size(); i<defaultStyle.defaultGraphFillStyles.size(); i++) {
            defaultGraphFillStyles.push_back(defaultStyle.defaultGraphFillStyles[i]);
        }
    }
}

void JKQTBasePlotterStyle::saveSettings(QSettings &settings, const QString &group) const
{
    settings.setValue(group+"csv_decimal_separator", CSVdecimalSeparator);
    settings.setValue(group+"csv_comment_initializer", CSVcommentInitializer);
    settings.setValue(group+"debug_show_region_boxes", debugShowRegionBoxes);
    settings.setValue(group+"debug_region_linewidth", debugRegionLineWidth);
    settings.setValue(group+"antialiase_system", useAntiAliasingForSystem);
    settings.setValue(group+"antialiase_graphs", useAntiAliasingForGraphs);
    settings.setValue(group+"antialiase_text", useAntiAliasingForText);
    settings.setValue(group+"widget_background_color", jkqtp_QColor2String(widgetBackgroundBrush.color()));
    settings.setValue(group+"widget_background_color_for_export", jkqtp_QColor2String(exportBackgroundBrush.color()));
    settings.setValue(group+"plot_background_color", jkqtp_QColor2String(plotBackgroundBrush.color()));
    settings.setValue(group+"plot_border_left", plotBorderLeft);
    settings.setValue(group+"plot_border_right", plotBorderRight);
    settings.setValue(group+"plot_border_top", plotBorderTop);
    settings.setValue(group+"plot_border_bottom", plotBorderBottom);
    settings.setValue(group+"graph_default_linewidth", defaultGraphWidth);
    settings.setValue(group+"graph_default_symbolsize", defaultGraphSymbolSize);
    settings.setValue(group+"graph_default_symbol_linewidtht", defaultGraphSymbolLineWidth);
    settings.setValue(group+"graph_default_palette", JKQTPMathImageColorPalette2String(defaultPalette));
    settings.setValue(group+"graph_default_fill_color_mode", JKQTPColorDerivationMode2String(graphFillColorDerivationMode));
    settings.setValue(group+"graph_default_error_color_mode", JKQTPColorDerivationMode2String(graphErrorColorDerivationMode));
    settings.setValue(group+"graph_default_error_fill_color_mode", JKQTPColorDerivationMode2String(graphErrorFillColorDerivationMode));
    settings.setValue(group+"text_default_color", jkqtp_QColor2String(defaultTextColor));
    settings.setValue(group+"text_default_size", defaultFontSize);
    settings.setValue(group+"text_default_font_name", defaultFontName);
    settings.setValue(group+"plot_label_font_name", plotLabelFontName);
    settings.setValue(group+"plot_label_font_size", plotLabelFontSize);
    settings.setValue(group+"plot_frame_visible", plotFrameVisible);
    settings.setValue(group+"plot_frame_color", jkqtp_QColor2String(plotFrameColor));
    settings.setValue(group+"plot_frame_width", plotFrameWidth);
    settings.setValue(group+"plot_frame_rounded", plotFrameRounding);

    keyStyle.saveSettings(settings, group+"key/");
    xAxisStyle.saveSettings(settings, group+"axis_x/");
    yAxisStyle.saveSettings(settings, group+"axis_y/");
    rightColorbarAxisStyle.saveSettings(settings, group+"axis_colorbar_right/");
    topColorbarAxisStyle.saveSettings(settings, group+"axis_colorbar_top/");

    int cnt=0;
    for (auto& gs: defaultGraphColors) {
        settings.setValue(group+"default_graph_styles/color"+QString::number(cnt), jkqtp_QColor2String(gs));
        cnt++;
    }
    cnt=0;
    for (auto& gs: defaultGraphPenStyles) {
        settings.setValue(group+"default_graph_styles/line_style"+QString::number(cnt), jkqtp_QPenStyle2String(gs));
        cnt++;
    }
    cnt=0;
    for (auto& gs: defaultGraphSymbols) {
        settings.setValue(group+"default_graph_styles/symbol"+QString::number(cnt), JKQTPGraphSymbols2String(gs));
        cnt++;
    }
    cnt=0;
    for (auto& gs: defaultGraphFillStyles) {
        settings.setValue(group+"default_graph_styles/fill_style"+QString::number(cnt), jkqtp_QBrushStyle2String(gs));
        cnt++;
    }
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
