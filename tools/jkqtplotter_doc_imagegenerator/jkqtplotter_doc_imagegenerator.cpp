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
    QCommandLineOption iconsizeOption("iconsize", "typical size of the generatued images.", "iconsize", "24");
    parser.addOption(iconsizeOption);
    QCommandLineOption backgroundOption("background", "background color.", "background", "white");
    parser.addOption(backgroundOption);
    parser.process(app);

    const QDir outputDir(parser.value(outputDirectoryOption));
    const bool listsymbols=parser.isSet(listsymbolsOption);
    const int iconsize=parser.value(iconsizeOption).toInt();
    const QColor backgroundColor = jkqtp_String2QColor(parser.value(backgroundOption));

    if (listsymbols) {
        for (uint64_t i=0; i<JKQTPSymbolCount; i++) {
            const JKQTPGraphSymbols s=static_cast<JKQTPGraphSymbols>(i);
            QImage  img(QSize(iconsize,iconsize),QImage::Format_ARGB32_Premultiplied);
            img.fill(backgroundColor);
            JKQTPEnhancedPainter p;
            p.begin(&img);
            p.setRenderHint(QPainter::Antialiasing);
            p.setRenderHint(QPainter::TextAntialiasing);
            p.setRenderHint(QPainter::SmoothPixmapTransform);
            JKQTPPlotSymbol(p, iconsize/2,iconsize/2,s,iconsize-4,2,QColor("blue"), QColor("salmon").lighter(120),QGuiApplication::font().family());
            p.end();
            const  bool ok=img.save(outputDir.absoluteFilePath(JKQTPGraphSymbols2String(s)+".png"));
            std::cout<<"saving to "<<outputDir.absoluteFilePath(JKQTPGraphSymbols2String(s)+".png").toStdString()<<" ==> "<<std::boolalpha<<ok<<"\n";
        }
        QImage  img(QSize(iconsize,iconsize),QImage::Format_ARGB32_Premultiplied);
        img.fill(backgroundColor);
        JKQTPEnhancedPainter p;
        p.begin(&img);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        JKQTPPlotSymbol(p, iconsize/2,iconsize/2,JKQTPCharacterSymbol+QChar('@').unicode(),iconsize-4,2,QColor("blue"), QColor("blue").lighter(),QGuiApplication::font().family());
        p.end();
        img.save(outputDir.absoluteFilePath("symbol_char_at.png"));

    }

    return EXIT_SUCCESS;
}
