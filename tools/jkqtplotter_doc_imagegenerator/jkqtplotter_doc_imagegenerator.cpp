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
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"
#include "jkqtplotter/graphs/jkqtpgeoannotations.h"
#include "jkqtplotter/graphs/jkqtpboxplot.h"
#include "jkqtplotter/graphs/jkqtpfilledcurve.h"
#include "jkqtplotter/graphs/jkqtpimpulses.h"
#include <iostream>
#if __cplusplus >= 202002L
# include <version>
# ifdef __cpp_lib_format
#  include <format>
# endif
#endif

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

void doListAxisStyling(const QDir& outputDir, int iconsize, QColor backgroundColor) {
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
    plot.getYAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getYAxis()->setAxisLabel("");


    plot.getYAxis()->setTickLabelType(JKQTPCALTdefault);
    plot.setY(0,110);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTdefault.png"), "png");

    plot.setY(1e-6,1.1e3);
    plot.getYAxis()->setMinTicks(7);
    plot.getYAxis()->setTickLabelType(JKQTPCALTexponentCharacter);
    plot.getYAxis()->setLogAxis(true);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTexponentCharacter.png"), "png");

    plot.getYAxis()->setLogAxis(false);
    plot.getYAxis()->setDrawMode1(JKQTPCADMLineTicksTickLabels);
    plot.getYAxis()->setDrawMode2(JKQTPCADMLineTicks);
    plot.setY(-1.55, 1.55);
    plot.getYAxis()->setMinTicks(5);
    plot.getYAxis()->setTickLabelType(JKQTPCALTintfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTintfrac.png"), "png");
    plot.getYAxis()->setTickLabelType(JKQTPCALTintsfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTintsfrac.png"), "png");
    plot.getYAxis()->setTickLabelType(JKQTPCALTintslashfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*1.2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTintslashfrac.png"), "png");
    plot.getYAxis()->setTickLabelType(JKQTPCALTfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTfrac.png"), "png");
    plot.getYAxis()->setTickLabelType(JKQTPCALTsfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTsfrac.png"), "png");
    plot.getYAxis()->setTickLabelType(JKQTPCALTslashfrac);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTslashfrac.png"), "png");

    plot.getYAxis()->setLogAxis(true);
    plot.setY(0.000008,100000);
    plot.getYAxis()->setMinTicks(4);
    plot.getYAxis()->setTickLabelType(JKQTPCALTexponent);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTexponent.png"), "png");

    plot.getYAxis()->setLogAxis(true);
    plot.setY(0.000008,100000);
    plot.getYAxis()->setMinTicks(4);
    plot.getYAxis()->setTickLabelType(JKQTPCALTscientific);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*1.2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTscientific.png"), "png");

    plot.getYAxis()->setLogAxis(false);
    plot.getYAxis()->setMinTicks(4);
    plot.setY(QDateTime(QDate(1999,10,1),QTime(0,0,0)).toMSecsSinceEpoch(),
              QDateTime(QDate(2000,2,1),QTime(0,0,0)).toMSecsSinceEpoch());
    plot.getYAxis()->setTickLabelType(JKQTPCALTdate);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTdate.png"), "png");

    plot.setY(QDateTime(QDate(2000,2,1),QTime(12,0,0)).toMSecsSinceEpoch(),
              QDateTime(QDate(2000,2,1),QTime(24,0,0)).toMSecsSinceEpoch());
    plot.getYAxis()->setTickLabelType(JKQTPCALTtime);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*1.0,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTtime.png"), "png");

    plot.setY(QDateTime(QDate(2010,10,1),QTime(12,0,0)).toMSecsSinceEpoch(),
              QDateTime(QDate(2010,10,2),QTime(12,0,0)).toMSecsSinceEpoch());
    plot.getYAxis()->setTickLabelType(JKQTPCALTdatetime);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2.5,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTdatetime.png"), "png");

    plot.setY(-1,1);
    plot.getYAxis()->setTickLabelType(JKQTPCALTprintf);
    plot.getYAxis()->setTickPrintfFormat("y=%+.2f");
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2.5,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTprintf.png"), "png");
#ifdef __cpp_lib_format
    plot.setY(-1,1);
    plot.getYAxis()->setTickLabelType(JKQTPCALTformat);
    plot.getYAxis()->setTickFormatFormat("\\texttt{{ y={:*^+8.1f}}}");
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,iconsize*2.5,plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPCALTformat.png"), "png");
#endif



    plot.setXY(0,10,0,1);
    plot.setWidgetSize(iconsize*4,4*iconsize);
    plot.setShowKey(false);
    plot.setGrid(true);
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine);
    plot.getXAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getYAxis()->setDrawMode1(JKQTPCADMLine);
    plot.getYAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getXAxis()->setAxisLabel("axis label");

    int imgheight;
    plot.getYAxis()->setTickLabelType(JKQTPCALTdefault);
    plot.getXAxis()->setDrawMode1(JKQTPCADMnone);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMnone.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMLine.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMTicks);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMTicks.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMTickLabels);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMTickLabels.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMAxisLabel);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMAxisLabel.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLineTicksTickLabels);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMLineTicksTickLabels.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLineTicks);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMLineTicks.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMTicksTickLabelsAxisLabel);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMTicksTickLabelsAxisLabel.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMTicksTickLabels);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMTicksTickLabels.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMTickLabelsAxisLabel);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMTickLabelsAxisLabel.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMcomplete);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMcomplete.png"), "png");

    plot.getYAxis()->setDrawMode1(JKQTPCADMnone);
    plot.getYAxis()->setDrawMode2(JKQTPCADMnone);
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine|JKQTPCADMMaxArrow);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMMaxArrow.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine|JKQTPCADMMaxFilledArrow);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMMaxFilledArrow.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine|JKQTPCADMMinArrow);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMMinArrow.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine|JKQTPCADMMinFilledArrow);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMMinFilledArrow.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine|JKQTPCADMcompleteMaxArrow);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMcompleteMaxArrow.png"), "png");
    plot.getXAxis()->setDrawMode1(JKQTPCADMLine|JKQTPCADMcompleteMinMaxArrow);
    imgheight=iconsize; plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,plot.getHeight()-imgheight,plot.getWidth(),imgheight).save(outputDir.absoluteFilePath("JKQTPCADMcompleteMinMaxArrow.png"), "png");


    plot.setXY(0.1,100,0.1,10);
    plot.setWidgetSize(iconsize*4,4*iconsize);
    plot.setShowKey(false);
    plot.setGrid(true);
    plot.getXAxis()->setLogAxis(true);
    plot.getYAxis()->setLogAxis(false);
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.getXAxis()->setDrawMode1(JKQTPCADMcomplete);
    plot.getXAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getYAxis()->setDrawMode1(JKQTPCADMcomplete);
    plot.getYAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getXAxis()->setAxisLabel("log. axis");
    plot.getYAxis()->setAxisLabel("lin. axis");

    plot.getXAxis()->setTickMode(JKQTPLTMLinOrPower);
    plot.getYAxis()->setTickMode(JKQTPLTMLinOrPower);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,plot.getWidth(),plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPLTMLinOrPower.png"), "png");

    plot.getXAxis()->setTickMode(JKQTPLTMLin);
    plot.getYAxis()->setTickMode(JKQTPLTMLin);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,plot.getWidth(),plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPLTMLin.png"), "png");

    plot.getXAxis()->setTickMode(JKQTPLTMPower);
    plot.getYAxis()->setTickMode(JKQTPLTMPower);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,plot.getWidth(),plot.getHeight()).save(outputDir.absoluteFilePath("JKQTPLTMPower.png"), "png");



    plot.setXY(0.1,10.0*JKQTPSTATISTICS_PI,0,2.1*JKQTPSTATISTICS_PI);
    plot.setWidgetSize(iconsize*4,4*iconsize);
    plot.setShowKey(false);
    plot.setGrid(true);
    plot.getXAxis()->setLogAxis(true);
    plot.getYAxis()->setLogAxis(false);
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.getXAxis()->setDrawMode1(JKQTPCADMcomplete);
    plot.getXAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getYAxis()->setDrawMode1(JKQTPCADMcomplete);
    plot.getYAxis()->setDrawMode2(JKQTPCADMLine);
    plot.getXAxis()->setAxisLabel("log. axis");
    plot.getYAxis()->setAxisLabel("lin. axis");
    plot.getXAxis()->setMinTicks(5);
    plot.getYAxis()->setMinTicks(3);
    plot.getYAxis()->setTickLabelType(JKQTPCALTfrac);

    plot.getXAxis()->setTickMode(JKQTPLTMLinOrPower);
    plot.getYAxis()->setTickMode(JKQTPLTMLinOrPower);
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,plot.getWidth(),plot.getHeight()).save(outputDir.absoluteFilePath("axis_unit_scaling_none.png"), "png");
    plot.getXAxis()->setTickUnitFactor(JKQTPSTATISTICS_PI);
    plot.getXAxis()->setTickUnitName("\\;\\pi");
    plot.getYAxis()->setTickUnitFactor(JKQTPSTATISTICS_PI);
    plot.getYAxis()->setTickUnitName("\\;\\pi");
    plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,plot.getWidth(),plot.getHeight()).save(outputDir.absoluteFilePath("axis_unit_scaling_pi.png"), "png");
}

void plotStyle(JKQTBasePlotter& plot) {

    // 0. get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot.getDatastore();

    // 1. clear old graphs/data
    plot.clearGraphs(true);
    ds->clear();

    // 2. now we create data for a simple plot (a sine curve)
    QVector<double> X, Y1, Y2, img, X3, Y3, Y3err, Xbar, Ybar, Ybar2, Y4, Y5, XImp, YImp;
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
        X<<x;
        Y1<<1.1+sin(x);
        Y2<<1.1+sin(x)*exp(-0.2*x);
        Y4<<4.5*exp(-x/10.0);
        Y5<<2.5*exp(-x/10.0)+1.0;
        if (i%5==0) {
            X3<<x;
            Y3<<(2.5+Y1.last());
            Y3err<<(double(i+5)/double(Ndata)*0.5);
        }
        if (i>0 && i%20==0) {
            Xbar<<x/2.0;
            Ybar<<(double(i+5)/double(Ndata)*(-1.5));
            Ybar2<<(double(i+5)/double(Ndata)*(-1.2));
        }
        if (i%8==0) {
            XImp<<15.0+x/3.0;
            YImp<<-1.0+0.5*sin(x);
        }
    }
    auto fgauss=[](double x, double y, double x0, double y0, double sx, double sy) {
        return exp(-2.0*(x-x0)*(x-x0)/sx/sx-2.0*(y-y0)*(y-y0)/sy/sy);
    };
    for (int i=0; i<Ndata; i++) {
        for (int j=0; j<Ndata; j++) {
            img.push_back(fgauss(j,i,Ndata/3,Ndata/3,Ndata/5,Ndata/3)+fgauss(j,i,Ndata*3/4,Ndata*2/3,Ndata/2,Ndata/4));
        }
    }

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    Note: In this step the data is copied (of not specified otherwise), so you can
    //          reuse X and Y afterwards!
    //    the variables columnX and columnY will contain the internal column ID of the newly
    //    created columns with names "x" and "y" and the (copied) data from X and Y.
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY1=ds->addCopiedColumn(Y1, "y1");
    size_t columnY2=ds->addCopiedColumn(Y2, "y2");
    size_t columnImg=ds->addCopiedColumn(img, "img");
    size_t columnX3=ds->addCopiedColumn(X3, "x3");
    size_t columnY3=ds->addCopiedColumn(Y3, "y3");
    size_t columnY3err=ds->addCopiedColumn(Y3err, "y3err");
    size_t columnXbar=ds->addCopiedColumn(Xbar, "xbar");
    size_t columnYbar=ds->addCopiedColumn(Ybar, "ybar");
    size_t columnYbar2=ds->addCopiedColumn(Ybar2, "Ybar2");
    size_t columnY4=ds->addCopiedColumn(Y4, "y4");
    size_t columnY5=ds->addCopiedColumn(Y5, "y5");
    size_t columnXImp=ds->addCopiedColumn(XImp, "ximp");
    size_t columnYImp=ds->addCopiedColumn(YImp, "yimp");

    // 4. create diverse graphs in the plot:
    JKQTPFilledCurveXGraph* graphf4=new JKQTPFilledCurveXGraph(&plot);
    graphf4->setXColumn(columnX);
    graphf4->setYColumn(columnY4);
    graphf4->setTitle(QObject::tr("filled exp-function"));
    plot.addGraph(graphf4);

    JKQTPFilledCurveXGraph* graphf5=new JKQTPFilledCurveXGraph(&plot);
    graphf5->setXColumn(columnX);
    graphf5->setYColumn(columnY5);
    graphf5->setTitle(QObject::tr("other filled exp-function"));
    plot.addGraph(graphf5);

    JKQTPColumnMathImage* graphI=new JKQTPColumnMathImage(&plot);
    graphI->setImageColumn(columnImg);
    graphI->setNx(Ndata);
    graphI->setNy(Ndata);
    graphI->setColorBarTopVisible(true);
    graphI->setColorBarRightVisible(true);
    graphI->setX(0.5);
    graphI->setY(-2.0);
    graphI->setWidth(5);
    graphI->setHeight(1);
    graphI->setTitle(QObject::tr("2{\\times}Gauss"));
    plot.addGraph(graphI);

    JKQTPXYLineGraph* graph1=new JKQTPXYLineGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY1);
    graph1->setTitle(QObject::tr("sine graph $f(x)=\\sin(x)$"));
    plot.addGraph(graph1);

    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(&plot);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setSymbolType(JKQTPNoSymbol);
    //graph2->setTitle(QObject::tr("sine{\\cdot}exp graph $f(x)=\\sin(x)\\cdot\\exp\\left(-\\frac{x}{5}\\right)$"));
    graph2->setTitle(QObject::tr("sine*exp graph"));
    plot.addGraph(graph2);

    JKQTPXYLineErrorGraph* graph3=new JKQTPXYLineErrorGraph(&plot);
    graph3->setXColumn(columnX3);
    graph3->setYColumn(columnY3);
    graph3->setYErrorColumn(columnY3err);
    graph3->setDrawLine(true);
    graph3->setYErrorStyle(JKQTPErrorBarsPolygons);
    graph3->setTitle(QObject::tr("sine \\pm errors graph"));
    plot.addGraph(graph3);

    JKQTPBarVerticalGraph* graphb=new JKQTPBarVerticalGraph(&plot);
    graphb->setXColumn(columnXbar);
    graphb->setYColumn(columnYbar);
    graphb->setTitle(QObject::tr("barchart"));
    plot.addGraph(graphb);

    JKQTPBarVerticalGraph* graphb2=new JKQTPBarVerticalGraph(&plot);
    graphb2->setXColumn(columnXbar);
    graphb2->setYColumn(columnYbar2);
    graphb2->setTitle(QObject::tr("2^{nd} barchart"));
    plot.addGraph(graphb2);
    graphb2->autoscaleBarWidthAndShiftSeparatedGroups();

    JKQTPImpulsesVerticalGraph* graphi2=new JKQTPImpulsesVerticalGraph(&plot);
    graphi2->setXColumn(columnXImp);
    graphi2->setYColumn(columnYImp);
    graphi2->setTitle(QObject::tr("impulses"));
    plot.addGraph(graphi2);

    JKQTPBoxplotHorizontalElement* graphBP=new JKQTPBoxplotHorizontalElement(&plot);
    graphBP->setData(-2, 7, 10,12,13,17,22);
    plot.addGraph(graphBP);

    JKQTPGeoSymbol* annotSym=new JKQTPGeoSymbol(&plot, 15, -3);
    plot.addGraph(annotSym);

    JKQTPGeoText* annotTxt=new JKQTPGeoText(&plot, 15.1, -3, "Annotation $\\int_{0}^{2\\pi}\\sin(x)\\;\\mathrm{d}x$");
    plot.addGraph(annotTxt);

    JKQTPGeoEllipse* geoEll=new JKQTPGeoEllipse(&plot, 5,-3,5,1);
    geoEll->setAngle(-25);
    plot.addGraph(geoEll);

    plot.addGraph(new JKQTPGeoArrow(&plot, 5,-3,10,-3.5));

    plot.addGraph(new JKQTPGeoLine(&plot, 5,-3,10,-4));

    // 6. autoscale the plot so the graph is contained
    plot.zoomToFit();

    plot.setPlotLabel("Sine Plots - User-Interaction Example");
    plot.getXAxis()->setAxisLabel("x-axis");
    plot.getYAxis()->setAxisLabel("y-axis");

    plot.redrawPlot();
}

void plotStyleSymbols(JKQTBasePlotter& plotExtra, const JKQTBasePlotter& plot) {
    plotExtra.clearGraphs(true);
    plotExtra.setBackgroundColor(plot.getBackgroundColor());
    plotExtra.setExportBackgroundColor(plot.getExportBackgroundColor());
    plotExtra.setPlotBackgroundColor(plot.getPlotBackgroundColor());
    plotExtra.getXAxis()->setDrawMode1(JKQTPCADMnone);
    plotExtra.getXAxis()->setDrawMode2(JKQTPCADMnone);
    plotExtra.getYAxis()->setDrawMode1(JKQTPCADMnone);
    plotExtra.getYAxis()->setDrawMode2(JKQTPCADMnone);
    double x=1;
    double xlineend=6;
    double y=1;
    double xmax=15;
    int cnt=0;
    for (auto s: plot.getCurrentPlotterStyle().graphsStyle.defaultGraphSymbols) {
        plotExtra.addGraph(new JKQTPGeoSymbol(&plotExtra, x, y, s, 12 ));
        const auto col=plot.getCurrentPlotterStyle().graphsStyle.defaultGraphColors.value(0, Qt::black);
        dynamic_cast<JKQTPGeoSymbol*>(plotExtra.getGraph(cnt))->setSymbolColor(col);
        dynamic_cast<JKQTPGeoSymbol*>(plotExtra.getGraph(cnt))->setSymbolFillColor(col);
        x+=1;
        xmax=qMax(x, xmax);
        if (cnt>0 && cnt%25==0) {
            x=1;
            y+=1.0;
        }
        cnt++;
    }
    y=y+1;
    double ycoltest=y;
    for (auto s: plot.getCurrentPlotterStyle().graphsStyle.defaultGraphPenStyles) {
        JKQTPGeoLine* l;
        plotExtra.addGraph(l=new JKQTPGeoLine(&plotExtra, 1,y,xlineend,y)); l->setStyle(QColor("red"), 2, s);
        const auto col=plot.getCurrentPlotterStyle().graphsStyle.defaultGraphColors.value(0, Qt::black);
        dynamic_cast<JKQTPGeoLine*>(plotExtra.getGraph(cnt))->setLineColor(col);
        y+=0.75;
    }
    x=xlineend+1;
    const int cntFillTests=qMax(plot.getCurrentPlotterStyle().graphsStyle.defaultGraphColors.size(), plot.getCurrentPlotterStyle().graphsStyle.defaultGraphFillStyles.size());
    double dx=(xmax-xlineend-1.0)/static_cast<double>(cntFillTests);
    for (int i=0; i<cntFillTests; i++) {
        const auto col=plot.getCurrentPlotterStyle().graphsStyle.defaultGraphColors.value(i%plot.getCurrentPlotterStyle().graphsStyle.defaultGraphColors.size(), Qt::black);
        const auto fs=plot.getCurrentPlotterStyle().graphsStyle.defaultGraphFillStyles.value(i%plot.getCurrentPlotterStyle().graphsStyle.defaultGraphFillStyles.size(), Qt::SolidPattern);
        JKQTPGeoLine* l;
        plotExtra.addGraph(l=new JKQTPGeoLine(&plotExtra, x,ycoltest,x,y-0.5)); l->setStyle(col,5);
        JKQTPGeoRectangle* r;
        plotExtra.addGraph(r=new JKQTPGeoRectangle(&plotExtra, x+dx/2,(ycoltest+y-0.5)/2.0, dx*0.5, y-ycoltest-0.5));
        r->setStyle(col,1, Qt::SolidLine, JKQTPGetDerivedColor(plot.getCurrentPlotterStyle().graphsStyle.defaultGraphStyle.fillColorDerivationMode, col), fs);
        x+=dx;
    }
    plotExtra.zoomToFit();
    plotExtra.setX(0, plotExtra.getXMax()+2);
    plotExtra.setY(0, plotExtra.getYMax()+1);
    plotExtra.setGrid(false);
    plotExtra.setPlotLabel("default symbols/lines/colors");
    plotExtra.redrawPlot();
}


void doListStyles(const QDir& outputDir, const QDir& docoutputDir, int iconsize, QColor backgroundColor) {
    QDir dir(":/JKQTPlotter/styles/", "*.ini");
    QFile fhtml(docoutputDir.absoluteFilePath("styles.html"));
    fhtml.open(QFile::WriteOnly|QFile::Text);
    QTextStream shtml(&fhtml);
    shtml<<"<table class=\"doxtable\">\n  <tr><th>Style-file </th><th>Screenshot </th><th>Symbols </th></tr>\n";
    for (auto& f: dir.entryList()) {
        qDebug()<<"plotting example for style "<<f;

        JKQTBasePlotter plot(true);
        plot.setWidgetSize(iconsize,iconsize);
        plot.loadCurrentPlotterStyle(QSettings(dir.absoluteFilePath(f), QSettings::IniFormat));
        plotStyle(plot);
        plot.grabPixelImage(QSize(plot.getWidth(),plot.getHeight()), false).copy(0,0,plot.getWidth(),plot.getHeight()).save(outputDir.absoluteFilePath(f+".png"), "png");

        JKQTBasePlotter plotExtra(true);
        plotExtra.setWidgetSize(iconsize*2, iconsize/3);
        plotStyleSymbols(plotExtra, plot);
        plotExtra.grabPixelImage(QSize(plotExtra.getWidth(),plotExtra.getHeight()), false).copy(0,0,plotExtra.getWidth(),plotExtra.getHeight()).save(outputDir.absoluteFilePath(f+".symbols.png"), "png");

        shtml<<"  <tr>"
             <<"<td><a href=\"https://github.com/jkriege2/JKQtPlotter/tree/master/lib/jkqtplotter/resources/styles/"<<f<<"\"><code>"<<dir.absoluteFilePath(f)<<"</code></a></td>"
             <<"<td><div class=\"image\"><img src=\""<<f<<".png\" alt=\"\"/></div></td>"
             <<"<td><div class=\"image\"><img src=\""<<f<<".symbols.png\" alt=\"\"/></div></td>"
             <<"</tr>";
    }
    shtml<<"</table>";
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
    QCommandLineOption docoutputDirectoryOption("docoutputdir", "write doc-file results into this directory.", "docoutputdir", app.applicationDirPath());
    parser.addOption(docoutputDirectoryOption);
    QCommandLineOption listsymbolsOption("listsymbols", "generate example images for all symbols.");
    parser.addOption(listsymbolsOption);
    QCommandLineOption listlinedecoratorsOption("listlinedecorators", "generate example images for all line-decorators.");
    parser.addOption(listlinedecoratorsOption);
    QCommandLineOption listerrorindicatorsOption("listerrorindicators", "generate example images for all error-indicators.");
    parser.addOption(listerrorindicatorsOption);
    QCommandLineOption listaxisstylepropsOption("listaxisstyleprops", "generate example images for JKQTPCALabelType and other coordinate axis properties.");
    parser.addOption(listaxisstylepropsOption);
    QCommandLineOption liststylesOption("liststyles", "generate example images for all predefined style INI files.");
    parser.addOption(liststylesOption);
    QCommandLineOption iconsizeOption("iconsize", "typical size of the generatued images.", "iconsize", "24");
    parser.addOption(iconsizeOption);
    QCommandLineOption backgroundOption("background", "background color.", "background", "white");
    parser.addOption(backgroundOption);
    parser.process(app);

    const QDir outputDir(parser.value(outputDirectoryOption));
    const QDir docoutputDir(parser.value(docoutputDirectoryOption));
    const bool listsymbols=parser.isSet(listsymbolsOption);
    const bool listlinedecorators=parser.isSet(listlinedecoratorsOption);
    const bool listerrorindicators=parser.isSet(listerrorindicatorsOption);
    const bool listaxisstyleprops=parser.isSet(listaxisstylepropsOption);
    const bool liststyles=parser.isSet(liststylesOption);
    const int iconsize=parser.value(iconsizeOption).toInt();
    const QColor backgroundColor = jkqtp_String2QColor(parser.value(backgroundOption));



    if (listsymbols) doListSymbols(outputDir, iconsize, backgroundColor);
    if (listlinedecorators) doListLineDecorators(outputDir, iconsize, backgroundColor);
    if (listerrorindicators) doListErrorIndicators(outputDir, iconsize, backgroundColor);
    if (listaxisstyleprops) doListAxisStyling(outputDir, iconsize, backgroundColor);
    if (liststyles) doListStyles(outputDir, docoutputDir, iconsize, backgroundColor);

    return EXIT_SUCCESS;
}
