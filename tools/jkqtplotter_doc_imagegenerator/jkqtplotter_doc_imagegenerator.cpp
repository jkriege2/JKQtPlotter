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
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#else
#include<QRegExp>
#endif
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
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
    QCommandLineOption listsymbolsOption("listsymbols", "list all symbols in the given output file and generate images.");
    parser.addOption(listsymbolsOption);
    QCommandLineOption listlinedecoratorsOption("listlinedecorators", "list all line-endings in the given output file and generate images.");
    parser.addOption(listlinedecoratorsOption);
    QCommandLineOption iconsizeOption("iconsize", "typical size of the generatued images.", "iconsize", "24");
    parser.addOption(iconsizeOption);
    QCommandLineOption backgroundOption("background", "background color.", "background", "white");
    parser.addOption(backgroundOption);
    parser.process(app);

    const QDir outputDir(parser.value(outputDirectoryOption));
    const bool listsymbols=parser.isSet(listsymbolsOption);
    const bool listlinedecorators=parser.isSet(listlinedecoratorsOption);
    const int iconsize=parser.value(iconsizeOption).toInt();
    const QColor backgroundColor = jkqtp_String2QColor(parser.value(backgroundOption));



    if (listsymbols) {
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
    }

    if (listlinedecorators) {
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
    return EXIT_SUCCESS;
}
