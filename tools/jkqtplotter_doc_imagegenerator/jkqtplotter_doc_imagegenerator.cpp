/** \example jkqtplotter_doc_imagegenerator.cpp
 *
 * \ref JKQTPlotterDocImageRenderCmdLineTool

 */
 
#include <QApplication>
#include <QPixmap>
#include <QPainter>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QElapsedTimer>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <algorithm>
#include <QDateTime>
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#else
#include<QRegExp>
#endif
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/graphs/jkqtplines.h"
#include <iostream>


void startPainting(QImage& img, JKQTPEnhancedPainter& p, int iconsizex, int iconsizey, QColor backgroundColor) {
    img=QImage(QSize(iconsizex,iconsizey),QImage::Format_ARGB32_Premultiplied);
    img.fill(backgroundColor);
    p.begin(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
}

void startPainting(QImage& img, JKQTPEnhancedPainter& p, int iconsize, QColor backgroundColor) {
    startPainting(img, p, iconsize,iconsize,backgroundColor);
}


void stopPaintingAndSave(QImage& img, JKQTPEnhancedPainter& p, const QString& filename) {
    p.end();
    img.save(filename);
}

void doListSymbols(const QDir& outputDir, int iconsize, QColor backgroundColor) {
    for (uint64_t i=0; i<JKQTPSymbolCount; i++) {
        const JKQTPGraphSymbols s=static_cast<JKQTPGraphSymbols>(i);
        QImage  img;
        JKQTPEnhancedPainter p;
        startPainting(img, p, iconsize, backgroundColor);
        JKQTPPlotSymbol(p, iconsize/2,iconsize/2,s,iconsize-4,2,QColor("blue"), QColor("salmon").lighter(120),QGuiApplication::font().family());
        p.setPen(QPen(QColor("green"),0.5));
        //p.drawRect(2,2,iconsize-4,iconsize-4);
        stopPaintingAndSave(img, p, outputDir.absoluteFilePath(JKQTPGraphSymbols2String(s)+".png"));
    }
    QImage  img;
    JKQTPEnhancedPainter p;
    startPainting(img, p, iconsize, backgroundColor);
    JKQTPPlotSymbol(p, iconsize/2,iconsize/2,JKQTPCharacterSymbol+QChar('@').unicode(),iconsize-4,1,QColor("blue"), QColor("salmon").lighter(120),QGuiApplication::font().family());
    p.setPen(QPen(QColor("green"),0.5));
    //p.drawRect(2,2,iconsize-4,iconsize-4);
    stopPaintingAndSave(img, p, outputDir.absoluteFilePath("symbol_char_at.png"));
    startPainting(img, p, iconsize, backgroundColor);
    JKQTPPlotSymbol(p, iconsize/2,iconsize/2,JKQTPFilledCharacterSymbol+QChar('@').unicode(),iconsize-4,0.5,QColor("blue"), QColor("salmon").lighter(120),QGuiApplication::font().family());
    p.setPen(QPen(QColor("green"),0.5));
    //p.drawRect(2,2,iconsize-4,iconsize-4);
    stopPaintingAndSave(img, p, outputDir.absoluteFilePath("symbol_filled_char_at.png"));
    startPainting(img, p, iconsize, backgroundColor);
    JKQTPPlotSymbol(p, iconsize/2,iconsize/2,JKQTPCharacterSymbol+QChar(0x2663).unicode(),iconsize-4,1,QColor("blue"), QColor("salmon").lighter(120),QGuiApplication::font().family());
    p.setPen(QPen(QColor("green"),0.5));
    //p.drawRect(2,2,iconsize-4,iconsize-4);
    stopPaintingAndSave(img, p, outputDir.absoluteFilePath("symbol_char_club.png"));
    startPainting(img, p, iconsize, backgroundColor);
    JKQTPPlotSymbol(p, iconsize/2,iconsize/2,JKQTPFilledCharacterSymbol+QChar(0x2663).unicode(),iconsize-4,0.5,QColor("blue"), QColor("salmon").lighter(120),QGuiApplication::font().family());
    p.setPen(QPen(QColor("green"),0.5));
    //p.drawRect(2,2,iconsize-4,iconsize-4);
    stopPaintingAndSave(img, p, outputDir.absoluteFilePath("symbol_filled_char_club.png"));

    JKQTPCustomGraphSymbolFunctor f=[](QPainter& p) {
        p.drawEllipse(QPointF(-0.2, -0.2), 0.3, 0.3);
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(QPointF(0.2, -0.2), 0.3, 0.3);
        p.drawEllipse(QPointF(0, 0.2), 0.3, 0.3);
    };
    JKQTPGraphSymbols customs=JKQTPRegisterCustomGraphSymbol(f);
    startPainting(img, p, iconsize, backgroundColor);
    JKQTPPlotSymbol(p, iconsize/2,iconsize/2,customs,iconsize-4,2,QColor("blue"), QColor("salmon").lighter(120),QGuiApplication::font().family());
    p.setPen(QPen(QColor("green"),0.5));
    stopPaintingAndSave(img, p, outputDir.absoluteFilePath("symbol_custom.png"));
}

void doListLineDecorators(const QDir& outputDir, int iconsize, QColor backgroundColor) {
    for (uint64_t i=0; i<JKQTPLineDecoratorCount; i++) {
        const JKQTPLineDecoratorStyle s=static_cast<JKQTPLineDecoratorStyle>(i);
        QImage  img;
        JKQTPEnhancedPainter p;
        startPainting(img, p, iconsize*2,iconsize, backgroundColor);

        p.setPen(QPen(QColor("red"), 1));
        p.setBrush(p.pen().color().lighter());
        JKQTPPlotDecoratedLine(p, QLineF(5, iconsize-14, iconsize, 7), JKQTPNoDecorator, 0, s, JKQTPLineDecoratorStyleCalcDecoratorSize(p.pen().widthF(), 9));

        p.setPen(QPen(QColor("blue"), 2));
        p.setBrush(p.pen().color().lighter());
        JKQTPPlotDecoratedLine(p, QLineF(5, iconsize-5, iconsize*2-5, 17), JKQTPNoDecorator, 0, s, JKQTPLineDecoratorStyleCalcDecoratorSize(p.pen().widthF(), 9));

        stopPaintingAndSave(img, p, outputDir.absoluteFilePath(JKQTPLineDecoratorStyle2String(s)+".png"));
    }
}


void doListErrorIndicators(const QDir& outputDir, int iconsize, QColor backgroundColor) {
    JKQTBasePlotter plot(true);
    JKQTPDatastore* ds=plot.getDatastore();
    size_t cx=ds->addCopiedColumn(QVector<double>{-1.5,-0.5,0.5,1.5,2.5},"x");
    size_t cy=ds->addCopiedColumn(QVector<double>{-0.75,-0.3,-0.05,0.2,0.65},"y");
    size_t cex=ds->addCopiedColumn(QVector<double>{0,0.2,0.1,0.45,0},"ex");
    size_t cey=ds->addCopiedColumn(QVector<double>{0.05,0.1,0.3,0.3,0.05},"ey");

    plot.setXY(-0.8,2.2,-0.5,0.7);
    plot.setWidgetSize(150,50);
    plot.setShowKey(false);
    plot.setGrid(false);
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.getXAxis()->setDrawMode1(JKQTPCADMnone);
    plot.getXAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getYAxis()->setDrawMode1(JKQTPCADMnone);
    plot.getYAxis()->setDrawMode2(JKQTPCADMnone);

    JKQTPXYLineErrorGraph* g=new JKQTPXYLineErrorGraph(&plot);
    g->setDrawLine(false);
    g->setLineStyle(Qt::DotLine);
    g->setSymbolType(JKQTPCross);
    g->setSymbolSize(6);
    g->setXColumn(cx);
    g->setYColumn(cy);
    g->setXErrorColumn(cex);
    g->setYErrorColumn(cey);
    plot.addGraph(g);
    const  std::vector<std::tuple<QString, JKQTPErrorPlotstyle, JKQTPErrorPlotstyle, bool, bool>> plots=
    {
        {"JKQTPNoError", JKQTPNoError, JKQTPNoError, true, false},
        {"JKQTPErrorSimpleBars", JKQTPErrorSimpleBars, JKQTPErrorSimpleBars, false, false},
        {"JKQTPErrorLines", JKQTPNoError, JKQTPErrorLines, true, false},
        {"JKQTPErrorPolygons", JKQTPNoError, JKQTPErrorPolygons, true, false},
        {"JKQTPErrorEllipses", JKQTPErrorEllipses, JKQTPErrorEllipses, false, false},
        {"JKQTPErrorBoxes", JKQTPErrorBoxes, JKQTPErrorBoxes, false, false},
        {"JKQTPErrorArrows", JKQTPErrorArrows, JKQTPErrorArrows, false, false},
        {"JKQTPErrorInwardArrows", JKQTPErrorInwardArrows, JKQTPErrorInwardArrows, false, false},
        {"JKQTPErrorBars", JKQTPErrorBars, JKQTPErrorBars, false, false},
        {"JKQTPErrorSimpleBarsPolygons", JKQTPNoError, JKQTPErrorSimpleBarsPolygons, true, false},
        {"JKQTPErrorSimpleBarsLines", JKQTPNoError, JKQTPErrorSimpleBarsLines, true, false},
        {"JKQTPErrorBarsLines", JKQTPNoError, JKQTPErrorBarsLines, true, false},
        {"JKQTPErrorBarsPolygons", JKQTPNoError, JKQTPErrorBarsPolygons, true, false},
        {"JKQTPErrorHalfBarsOutwards", JKQTPErrorHalfBarsOutwards, JKQTPErrorHalfBarsOutwards, false,true},
        {"JKQTPErrorHalfBarsInwards", JKQTPErrorHalfBarsInwards, JKQTPErrorHalfBarsInwards, false,true},
        {"JKQTPErrorHalfBarsAbove", JKQTPErrorHalfBarsAbove, JKQTPErrorHalfBarsAbove, false,true},
        {"JKQTPErrorHalfBarsBelow", JKQTPErrorHalfBarsBelow, JKQTPErrorHalfBarsBelow, false,true},
        {"JKQTPErrorIndicatorArrows", JKQTPErrorIndicatorArrows, JKQTPErrorIndicatorArrows, false, false},
        {"JKQTPErrorIndicatorInwardArrows", JKQTPErrorIndicatorInwardArrows, JKQTPErrorIndicatorInwardArrows, false, false},
        {"JKQTPErrorIndicatorBar", JKQTPErrorIndicatorBar, JKQTPErrorIndicatorBar, false, false},
    };
    for (const auto& d: plots) {
        g->setDrawLine(std::get<3>(d));
        g->setXErrorStyle(std::get<1>(d));
        g->setYErrorStyle(std::get<2>(d));
        plot.getXAxis()->setShowZeroAxis(std::get<4>(d));
        plot.getYAxis()->setShowZeroAxis(std::get<4>(d));

        plot.saveAsPixelImage(outputDir.absoluteFilePath(std::get<0>(d)+".png"), false, "png");
    }
}

void doListJKQTPCALabelType(const QDir& outputDir, int iconsize, QColor backgroundColor) {
    JKQTBasePlotter plot(true);

    plot.setXY(0,1,0.9,110);
    plot.setWidgetSize(iconsize*4,4*iconsize);
    plot.setShowKey(false);
    plot.setGrid(true);
    plot.setExportBackgroundColor(QColor("lightgrey"));
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine);
    plot.getXAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getXAxis()->setDrawGrid(false);
    plot.getYAxis()->setDrawMode1(JKQTPCADMLineTicksTickLabels);
    plot.getYAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getYAxis()->setAxisLabel("");


    plot.getYAxis()->setLabelType(JKQTPCALTdefault);
    plot.setY(0,110);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTdefault.png"), "png");

    plot.setY(1e-6,1.1e3);
    plot.getYAxis()->setMinTicks(7);
    plot.getYAxis()->setLabelType(JKQTPCALTexponentCharacter);
    plot.getYAxis()->setLogAxis(true);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTexponentCharacter.png"), "png");

    plot.getYAxis()->setLogAxis(false);
    plot.setY(-1.3, 1.55);
    plot.getYAxis()->setMinTicks(5);
    plot.getYAxis()->setLabelType(JKQTPCALTintfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTintfrac.png"), "png");
    plot.getYAxis()->setLabelType(JKQTPCALTintsfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTintsfrac.png"), "png");
    plot.getYAxis()->setLabelType(JKQTPCALTintslashfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTintslashfrac.png"), "png");
    plot.getYAxis()->setLabelType(JKQTPCALTfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTfrac.png"), "png");
    plot.getYAxis()->setLabelType(JKQTPCALTsfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTsfrac.png"), "png");
    plot.getYAxis()->setLabelType(JKQTPCALTslashfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTslashfrac.png"), "png");

    plot.setY(0.000008,100000);
    plot.getYAxis()->setMinTicks(4);
    plot.getYAxis()->setLabelType(JKQTPCALTexponent);
    plot.getYAxis()->setLogAxis(true);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTexponent.png"), "png");

    plot.setY(0.000008,100000);
    plot.getYAxis()->setMinTicks(4);
    plot.getYAxis()->setLabelType(JKQTPCALTscientific);
    plot.getYAxis()->setLogAxis(true);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTscientific.png"), "png");

    plot.getYAxis()->setMinTicks(4);
    plot.setY(QDateTime(QDate(1999,10,1),QTime(0,0,0)).toMSecsSinceEpoch(),
              QDateTime(QDate(2000,2,1),QTime(0,0,0)).toMSecsSinceEpoch());
    plot.getYAxis()->setLabelType(JKQTPCALTdate);
    plot.getYAxis()->setLogAxis(false);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTdate.png"), "png");

    plot.setY(QDateTime(QDate(2000,2,1),QTime(12,0,0)).toMSecsSinceEpoch(),
              QDateTime(QDate(2000,2,1),QTime(24,0,0)).toMSecsSinceEpoch());
    plot.getYAxis()->setLabelType(JKQTPCALTtime);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*1.0,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTtime.png"), "png");

    plot.setY(QDateTime(QDate(2010,10,1),QTime(12,0,0)).toMSecsSinceEpoch(),
              QDateTime(QDate(2010,10,2),QTime(12,0,0)).toMSecsSinceEpoch());
    plot.getYAxis()->setLabelType(JKQTPCALTdatetime);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2.5,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTdatetime.png"), "png");


}

int main(int argc, char* argv[])
{
    // 1. create Qt Appcilation object and a QCommandLineParser to go with it
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("JKQTPlotter command line tool that renders diverse images for the documentation");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption outputDirectoryOption("outputdir", "write results into this directory.", "outputdir", app.applicationDirPath());
    parser.addOption(outputDirectoryOption);
    QCommandLineOption listsymbolsOption("listsymbols", "generate example images for all symbols.");
    parser.addOption(listsymbolsOption);
    QCommandLineOption listlinedecoratorsOption("listlinedecorators", "generate example images for all line-decorators.");
    parser.addOption(listlinedecoratorsOption);
    QCommandLineOption listerrorindicatorsOption("listerrorindicators", "generate example images for all error-indicators.");
    parser.addOption(listerrorindicatorsOption);
    QCommandLineOption listaxislabeltypeOption("listaxislabeltype", "generate example images for JKQTPCALabelType.");
    parser.addOption(listaxislabeltypeOption);
    QCommandLineOption iconsizeOption("iconsize", "typical size of the generatued images.", "iconsize", "24");
    parser.addOption(iconsizeOption);
    QCommandLineOption backgroundOption("background", "background color.", "background", "white");
    parser.addOption(backgroundOption);
    parser.process(app);

    const QDir outputDir(parser.value(outputDirectoryOption));
    const bool listsymbols=parser.isSet(listsymbolsOption);
    const bool listlinedecorators=parser.isSet(listlinedecoratorsOption);
    const bool listerrorindicators=parser.isSet(listerrorindicatorsOption);
    const bool listaxislabeltype=parser.isSet(listaxislabeltypeOption);
    const int iconsize=parser.value(iconsizeOption).toInt();
    const QColor backgroundColor = jkqtp_String2QColor(parser.value(backgroundOption));



    if (listsymbols) doListSymbols(outputDir, iconsize, backgroundColor);
    if (listlinedecorators) doListLineDecorators(outputDir, iconsize, backgroundColor);
    if (listsymbols) doListErrorIndicators(outputDir, iconsize, backgroundColor);
    if (listaxislabeltype) doListJKQTPCALabelType(outputDir, iconsize, backgroundColor);

    return EXIT_SUCCESS;
}
