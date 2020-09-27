#include "jkqtpgraphsbasestyle.h"
#include <QPalette>
#include <QApplication>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpbaseplotterstyle.h"



JKQTGraphsSpecificStyleProperties::JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTBasePlotterStyle &/*parent*/):
    defaultLineWidth(2),
    defaultSymbolSize(10),
    defaultSymbolLineWidth(1),
    defaultErrorIndicatorWidth(1),
    defaultHeadDecoratorStyle(JKQTPLineDecoratorStyle::JKQTPDefaultLineDecorator),
    defaultHeadDecoratorSizeFactor(8.0),
    errorFillStyle(Qt::SolidPattern),
    graphColorDerivationMode(JKQTPFFCMSameColor),
    fillColorDerivationMode(JKQTPFFCMLighterColor),
    errorColorDerivationMode(JKQTPFFCMDarkerColor),
    errorFillColorDerivationMode(JKQTPFFCMLighterAndTransparentColor),
    symbolFillColorDerivationMode(JKQTPFFCMLighterColor)
{
    modifyForDefaultStyle(type);
}

JKQTGraphsSpecificStyleProperties::JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTGraphsSpecificStyleProperties &other):
    JKQTGraphsSpecificStyleProperties(other)
{
    modifyForDefaultStyle(type);
}


void JKQTGraphsSpecificStyleProperties::modifyForDefaultStyle(JKQTPPlotStyleType type)
{
    switch(type) {
    case JKQTPPlotStyleType::Default:
        break;
    case JKQTPPlotStyleType::Filled:
        fillColorDerivationMode=JKQTPFFCMLighterAndTransparentColor;
        break;
    case JKQTPPlotStyleType::Impulses:
        defaultLineWidth=3;
        break;
    case JKQTPPlotStyleType::Barchart:
        defaultLineWidth=0;
        graphColorDerivationMode=JKQTPFFCMLighterColor;
        fillColorDerivationMode=JKQTPFFCMLighterColor;
        break;
    case JKQTPPlotStyleType::Boxplot:
        defaultLineWidth=1;
        fillColorDerivationMode=JKQTPFFCMWhite;
        break;
    case JKQTPPlotStyleType::Annotation:
        defaultSymbolSize=16;
        break;
    case JKQTPPlotStyleType::Geometric:
        fillColorDerivationMode=JKQTPFFCMFullyTransparentColor;
        break;
    }
}

void JKQTGraphsSpecificStyleProperties::loadSettings(const QSettings &settings, const QString &group, const JKQTGraphsSpecificStyleProperties &defaultStyle)
{
    defaultLineWidth=settings.value(group+"linewidth", defaultStyle.defaultLineWidth).toDouble();
    defaultSymbolSize=settings.value(group+"symbol_size", defaultStyle.defaultSymbolSize).toDouble();
    defaultSymbolLineWidth=settings.value(group+"symbol_line_width", defaultStyle.defaultSymbolLineWidth).toDouble();
    defaultErrorIndicatorWidth=settings.value(group+"error_indicator_width", defaultStyle.defaultErrorIndicatorWidth).toDouble();
    defaultHeadDecoratorStyle=String2JKQTPLineDecoratorStyle(settings.value(group+"head_decorator_type", JKQTPLineDecoratorStyle2String(defaultStyle.defaultHeadDecoratorStyle)).toString());
    defaultHeadDecoratorSizeFactor=settings.value(group+"head_decorator_size_factor", defaultStyle.defaultHeadDecoratorSizeFactor).toDouble();
    errorFillStyle=jkqtp_String2QBrushStyle(settings.value(group+"error_fill_style", jkqtp_QBrushStyle2String(errorFillStyle)).toString());

    graphColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"graph_color_mode", JKQTPColorDerivationMode2String(defaultStyle.graphColorDerivationMode)).toString());
    fillColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"fill_color_mode", JKQTPColorDerivationMode2String(defaultStyle.fillColorDerivationMode)).toString());
    errorColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"error_color_mode", JKQTPColorDerivationMode2String(defaultStyle.errorColorDerivationMode)).toString());
    errorFillColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"error_fill_color_mode", JKQTPColorDerivationMode2String(defaultStyle.errorFillColorDerivationMode)).toString());
    symbolFillColorDerivationMode=String2JKQTPColorDerivationMode(settings.value(group+"symbol_fill_color_mode", JKQTPColorDerivationMode2String(defaultStyle.symbolFillColorDerivationMode)).toString());

}

void JKQTGraphsSpecificStyleProperties::saveSettings(QSettings &settings, const QString &group) const
{
    settings.setValue(group+"linewidth", defaultLineWidth);
    settings.setValue(group+"symbol_size", defaultSymbolSize);
    settings.setValue(group+"symbol_line_width", defaultSymbolLineWidth);
    settings.setValue(group+"error_indicator_width", defaultErrorIndicatorWidth);
    settings.setValue(group+"head_decorator_size_factor", defaultHeadDecoratorSizeFactor);
    settings.setValue(group+"head_decorator_type", JKQTPLineDecoratorStyle2String(defaultHeadDecoratorStyle));
    settings.setValue(group+"error_fill_style", jkqtp_QBrushStyle2String(errorFillStyle));

    settings.setValue(group+"graph_color_mode", JKQTPColorDerivationMode2String(graphColorDerivationMode));
    settings.setValue(group+"fill_color_mode", JKQTPColorDerivationMode2String(fillColorDerivationMode));
    settings.setValue(group+"error_color_mode", JKQTPColorDerivationMode2String(errorColorDerivationMode));
    settings.setValue(group+"error_fill_color_mode", JKQTPColorDerivationMode2String(errorFillColorDerivationMode));
    settings.setValue(group+"symbol_fill_color_mode", JKQTPColorDerivationMode2String(symbolFillColorDerivationMode));
}



JKQTGeometricSpecificStyleProperties::JKQTGeometricSpecificStyleProperties(const JKQTBasePlotterStyle& parent):
    JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType::Geometric, parent),
    defaultColor(parent.defaultTextColor),
    defaultLineStyle(Qt::SolidLine),
    defaultSymbol(JKQTPGraphSymbols::JKQTPCross),
    defaultFillStyle(Qt::SolidPattern)
{

}

JKQTGeometricSpecificStyleProperties::JKQTGeometricSpecificStyleProperties(const JKQTBasePlotterStyle& parent, const JKQTGraphsSpecificStyleProperties &other):
    JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType::Geometric, other),
    defaultColor(parent.defaultTextColor),
    defaultLineStyle(Qt::SolidLine),
    defaultSymbol(JKQTPGraphSymbols::JKQTPCross),
    defaultFillStyle(Qt::SolidPattern)
{

}

JKQTGeometricSpecificStyleProperties::JKQTGeometricSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTBasePlotterStyle &parent):
    JKQTGraphsSpecificStyleProperties(type, parent),
    defaultColor(parent.defaultTextColor),
    defaultLineStyle(Qt::SolidLine),
    defaultSymbol(JKQTPGraphSymbols::JKQTPCross),
    defaultFillStyle(Qt::SolidPattern)
{

}

JKQTGeometricSpecificStyleProperties::JKQTGeometricSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTGraphsSpecificStyleProperties &other, const JKQTBasePlotterStyle &parent):
    JKQTGraphsSpecificStyleProperties(type, other),
    defaultColor(parent.defaultTextColor),
    defaultLineStyle(Qt::SolidLine),
    defaultSymbol(JKQTPGraphSymbols::JKQTPCross),
    defaultFillStyle(Qt::SolidPattern)
{

}


void JKQTGeometricSpecificStyleProperties::loadSettings(const QSettings &settings, const QString &group, const JKQTGeometricSpecificStyleProperties &defaultStyle)
{
    JKQTGraphsSpecificStyleProperties::loadSettings(settings, group, defaultStyle);
    defaultColor=jkqtp_String2QColor(settings.value(group+"color", jkqtp_QColor2String(defaultColor)).toString());
    defaultLineStyle=jkqtp_String2QPenStyle(settings.value(group+"line_style", jkqtp_QPenStyle2String(defaultLineStyle)).toString());
    defaultSymbol=String2JKQTPGraphSymbols(settings.value(group+"symbol", JKQTPGraphSymbols2String(defaultSymbol)).toString());
    defaultFillStyle=jkqtp_String2QBrushStyle(settings.value(group+"fill_style", jkqtp_QBrushStyle2String(defaultFillStyle)).toString());
}

void JKQTGeometricSpecificStyleProperties::saveSettings(QSettings &settings, const QString &group) const
{
    JKQTGraphsSpecificStyleProperties::saveSettings(settings, group);
    settings.setValue(group+"color", jkqtp_QColor2String(defaultColor));
    settings.setValue(group+"line_style", jkqtp_QPenStyle2String(defaultLineStyle));
    settings.setValue(group+"symbol", JKQTPGraphSymbols2String(defaultSymbol));
    settings.setValue(group+"fill_style", jkqtp_QBrushStyle2String(defaultFillStyle));
}

JKQTAnnotationsSpecificStyleProperties::JKQTAnnotationsSpecificStyleProperties(const JKQTBasePlotterStyle& parent):
    JKQTGeometricSpecificStyleProperties(JKQTPPlotStyleType::Annotation, parent),
    defaultTextColor(parent.defaultTextColor),
    defaultFontSize(12),
    defaultFontName(parent.defaultFontName)
{

}

JKQTAnnotationsSpecificStyleProperties::JKQTAnnotationsSpecificStyleProperties(const JKQTBasePlotterStyle& parent, const JKQTGraphsSpecificStyleProperties &other):
    JKQTGeometricSpecificStyleProperties(JKQTPPlotStyleType::Annotation, other, parent),
    defaultTextColor(parent.defaultTextColor),
    defaultFontSize(12),
    defaultFontName(parent.defaultFontName)
{

}

void JKQTAnnotationsSpecificStyleProperties::loadSettings(const QSettings &settings, const QString &group, const JKQTAnnotationsSpecificStyleProperties &defaultStyle)
{
    JKQTGeometricSpecificStyleProperties::loadSettings(settings, group, defaultStyle);
    defaultTextColor=jkqtp_String2QColor(settings.value(group+"text_color", jkqtp_QColor2String(defaultStyle.defaultTextColor)).toString());
    defaultFontSize=settings.value(group+"font_size", defaultStyle.defaultFontSize).toDouble();
    defaultFontName=settings.value(group+"font_name", defaultStyle.defaultFontName).toString();
}

void JKQTAnnotationsSpecificStyleProperties::saveSettings(QSettings &settings, const QString &group) const
{
    JKQTGeometricSpecificStyleProperties::saveSettings(settings, group);
    settings.setValue(group+"text_color", jkqtp_QColor2String(defaultTextColor));
    settings.setValue(group+"font_size", defaultFontSize);
    settings.setValue(group+"font_name", defaultFontName);
}



JKQTGraphsBaseStyle::JKQTGraphsBaseStyle(const JKQTBasePlotterStyle& parent):
    useAntiAliasingForGraphs(true),
    defaultGraphStyle(JKQTPPlotStyleType::Default, parent),
    barchartStyle(JKQTPPlotStyleType::Barchart, parent),
    boxplotStyle(JKQTPPlotStyleType::Boxplot, parent),
    filledStyle(JKQTPPlotStyleType::Filled, parent),
    impulseStyle(JKQTPPlotStyleType::Impulses, parent),
    geometricStyle(parent),
    annotationStyle(parent),
    defaultPalette(JKQTPMathImageColorPalette::JKQTPMathImageMATLAB)
{
    defaultGraphColors<<QColor("red")<<QColor("green")<<QColor("blue")<<QColor("fuchsia")<<QColor("darkorange")
                       <<QColor("navy")<<QColor("firebrick")<<QColor("darkgreen")<<QColor("darkmagenta")
                       <<QColor("darkgreen")<<QColor("darkslateblue")<<QColor("maroon")<<QColor("indianred")
                       <<QColor("darkolivegreen")<<QColor("mediumpurple")<<QColor("darkcyan");
    defaultGraphPenStyles<<Qt::SolidLine<<Qt::DashLine<<Qt::DotLine<<Qt::DashDotLine<<Qt::DashDotDotLine;
    for (int i=2; i<=JKQTPMaxSymbolID; i++) defaultGraphSymbols.push_back(static_cast<JKQTPGraphSymbols>(i));
    defaultGraphFillStyles<<Qt::SolidPattern;

}

void JKQTGraphsBaseStyle::loadSettings(const QSettings &settings, const QString &group, const JKQTGraphsBaseStyle &defaultStyle, const JKQTBasePlotterStyle& parent)
{
    useAntiAliasingForGraphs=settings.value(group+"antialiase", defaultStyle.useAntiAliasingForGraphs).toBool();
    defaultPalette=JKQTPImageTools::String2JKQTPMathImageColorPalette(settings.value(group+"palette", JKQTPImageTools::JKQTPMathImageColorPalette2String(defaultStyle.defaultPalette)).toString());

    auto readID=[](const QString& k, const QString& start) {
        if (k.startsWith(start)) {
            QString kk=k;
            kk.remove(0, start.size());
            QString num="";
            while (!kk.isEmpty() && kk.at(0).isDigit()) {
                num+=kk.at(0);
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
        int id=readID(k, group+"auto_styles/color");
        if (id>=0) {
            defaultGraphColors.push_back(jkqtp_String2QColor(settings.value(group+"auto_styles/color"+QString::number(id), jkqtp_QColor2String(QColor("red"))).toString()));
        }

        id=readID(k, group+"auto_styles/line_style");
        if (id>=0) {
            defaultGraphPenStyles.push_back(jkqtp_String2QPenStyle(settings.value(group+"auto_styles/line_style"+QString::number(id), jkqtp_QPenStyle2String(Qt::SolidLine)).toString()));
        }

        id=readID(k, group+"auto_styles/symbol");
        if (id>=0) {
            defaultGraphSymbols.push_back(String2JKQTPGraphSymbols(settings.value(group+"auto_styles/symbol"+QString::number(id), JKQTPGraphSymbols2String(JKQTPCross)).toString()));
        }
        id=readID(k, group+"auto_styles/fill_style");
        if (id>=0) {
            defaultGraphFillStyles.push_back(jkqtp_String2QBrushStyle(settings.value(group+"auto_styles/fill_style"+QString::number(id), jkqtp_QBrushStyle2String(Qt::SolidPattern)).toString()));
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


    defaultGraphStyle.loadSettings(settings, group+"graphs_base/", JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType::Default, parent));
    barchartStyle.loadSettings(settings, group+"graphs_barchart/", JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType::Barchart, defaultGraphStyle));
    boxplotStyle.loadSettings(settings, group+"graphs_boxplot/", JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType::Boxplot, defaultGraphStyle));
    filledStyle.loadSettings(settings, group+"graphs_filled/", JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType::Filled, defaultGraphStyle));
    impulseStyle.loadSettings(settings, group+"graphs_impulses/",JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType::Impulses,  defaultGraphStyle));
    geometricStyle.loadSettings(settings, group+"graphs_geometric/", JKQTGeometricSpecificStyleProperties(parent, defaultGraphStyle));
    annotationStyle.loadSettings(settings, group+"graphs_annotation/", JKQTAnnotationsSpecificStyleProperties(parent, defaultGraphStyle));

}

void JKQTGraphsBaseStyle::saveSettings(QSettings &settings, const QString &group) const
{

    settings.setValue(group+"antialiase", useAntiAliasingForGraphs);
    settings.setValue(group+"palette", JKQTPImageTools::JKQTPMathImageColorPalette2String(defaultPalette));


    int cnt=0;
    for (auto& gs: defaultGraphColors) {
        settings.setValue(group+"auto_styles/color"+QString::number(cnt), jkqtp_QColor2String(gs));
        cnt++;
    }
    cnt=0;
    for (auto& gs: defaultGraphPenStyles) {
        settings.setValue(group+"auto_styles/line_style"+QString::number(cnt), jkqtp_QPenStyle2String(gs));
        cnt++;
    }
    cnt=0;
    for (auto& gs: defaultGraphSymbols) {
        settings.setValue(group+"auto_styles/symbol"+QString::number(cnt), JKQTPGraphSymbols2String(gs));
        cnt++;
    }
    cnt=0;
    for (auto& gs: defaultGraphFillStyles) {
        settings.setValue(group+"auto_styles/fill_style"+QString::number(cnt), jkqtp_QBrushStyle2String(gs));
        cnt++;
    }

    defaultGraphStyle.saveSettings(settings, group+"graphs_base/");
    barchartStyle.saveSettings(settings, group+"graphs_barchart/");
    boxplotStyle.saveSettings(settings, group+"graphs_boxplot/");
    filledStyle.saveSettings(settings, group+"graphs_filled/");
    impulseStyle.saveSettings(settings, group+"graphs_impulses/");
    geometricStyle.saveSettings(settings, group+"graphs_geometric/");
    annotationStyle.saveSettings(settings, group+"graphs_annotation/");
}

const JKQTGraphsSpecificStyleProperties &JKQTGraphsBaseStyle::getGraphStyleByType(JKQTPPlotStyleType type) const
{
    switch (type) {
    case JKQTPPlotStyleType::Default:
        return defaultGraphStyle;
    case JKQTPPlotStyleType::Barchart:
        return barchartStyle;
    case JKQTPPlotStyleType::Boxplot:
        return boxplotStyle;
    case JKQTPPlotStyleType::Filled:
        return filledStyle;
    case JKQTPPlotStyleType::Impulses:
        return impulseStyle;
    case JKQTPPlotStyleType::Geometric:
        return geometricStyle;
    case JKQTPPlotStyleType::Annotation:
        return annotationStyle;
    }
    return defaultGraphStyle;
}
