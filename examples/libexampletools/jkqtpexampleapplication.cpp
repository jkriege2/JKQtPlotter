#include "jkqtpexampleapplication.h"
#include <QElapsedTimer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QWindow>
#include <QWidget>
#include "jkqtplotter/jkqtplotter.h"


JKQTPExampleApplication::JKQTPExampleApplication(int &argc, char **argv):
    QApplication(argc, argv),
    saveScreenshot(false),
    saveSmallScreenshot(false),
    saveScreenshotPlot(false),
    saveSmallScreenshotPlot(false),
    scaleDownFromHighDPI(false),
    screenshotBasename("screenshot")
{
    screenshotDir=QDir::current();
    for (int i=0; i<argc; i++) {
        if (QString(argv[i])=="--disablehighdpi") {
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0) &&  QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false); // DPI support
            QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, false); //HiDPI pixmaps
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            QApplication::setAttribute(Qt::AA_Use96Dpi); // disable DPI support
#endif
        }
    }
}

JKQTPExampleApplication::~JKQTPExampleApplication()
{

}

void JKQTPExampleApplication::readCmdLine() {
    QCommandLineParser parser;
    parser.setApplicationDescription(QString("JKQTPlotter example program '%1'").arg(applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption outputDirectoryOption("screenshotdir", "write results into this directory.", "outputdir", applicationDirPath());
    parser.addOption(outputDirectoryOption);
    QCommandLineOption basenameOption("screenshotbasename", "basename for screenshots.", "basename", QFileInfo(applicationFilePath()).baseName());
    parser.addOption(basenameOption);
    QCommandLineOption screenshotOption(QStringList()<<"screenshot", "save screenshot(s) of the window(s).");
    parser.addOption(screenshotOption);
    QCommandLineOption smallscreenshotOption(QStringList()<<"smallscreenshot", "save small screenshot(s) of the window(s).");
    parser.addOption(smallscreenshotOption);
    QCommandLineOption screenshotPlotOption(QStringList()<<"screenshotplot", "save screenshot(s) of the plot(s).");
    parser.addOption(screenshotPlotOption);
    QCommandLineOption smallscreenshotPlotOption(QStringList()<<"smallscreenshotplot", "save screenshot(s) of the plot(s).");
    parser.addOption(smallscreenshotPlotOption);
    QCommandLineOption scaleDownFromHighDPIOption(QStringList()<<"scalescreenshotdownfromhighdpi", "if on high-dpi device, rescale to standard size.");
    parser.addOption(scaleDownFromHighDPIOption);
    QCommandLineOption disablehighdpiOption(QStringList()<<"disablehighdpi", "idisable high-dpi support.");
    parser.addOption(disablehighdpiOption);

    parser.process(*this);

    screenshotDir=QDir(parser.value(outputDirectoryOption));
    screenshotBasename=parser.value(basenameOption).split(',');
    saveScreenshot = parser.isSet(screenshotOption);
    saveSmallScreenshot = parser.isSet(smallscreenshotOption);
    saveScreenshotPlot = parser.isSet(screenshotPlotOption);
    saveSmallScreenshotPlot = parser.isSet(smallscreenshotPlotOption);
    scaleDownFromHighDPI = parser.isSet(scaleDownFromHighDPIOption);
}

QRect JKQTPExampleApplication::getBoundsWithoutColor(QImage qImage, const QColor &exclusionColor)
{
    QRect ofTheKing;

    int maxX = 0; int minX = qImage.width();
    int maxY = 0; int minY = qImage.height();

    for(int x=0; x < qImage.width(); x++)
        for(int y=0; y < qImage.height(); y++)
            if (QColor::fromRgb(qImage.pixel(x, y)) != exclusionColor)
            {
                if(x < minX) minX = x;
                if(x > maxX) maxX = x;
                if(y < minY) minY = y;
                if(y > maxY) maxY = y;
            }

    if (minX > maxX || minY > maxY)
        ofTheKing=QRect();
    else
        ofTheKing.setCoords(minX, minY, maxX+1, maxY+1);

    return ofTheKing;
 }

int JKQTPExampleApplication::exec()
{
    readCmdLine();
    if (saveScreenshot||saveSmallScreenshot) {
        QElapsedTimer timer;
        timer.start();
        while(timer.elapsed()<150) {
            QApplication::processEvents();
        }
        QWidgetList widgets=topLevelWidgets();
        int iVisible=0;
        for (int i=0; i<widgets.size(); i++) {
            QWidget* w=widgets[i];
            if (w->isVisible()) {
                JKQTPlotter* plot=dynamic_cast<JKQTPlotter*>(w);
                QString bn=screenshotBasename.value(iVisible, screenshotBasename.value(0));
                if (iVisible>0 && screenshotBasename.value(iVisible, "")=="") {
                    bn+=QString("_win%1").arg(iVisible, 2, 10, QLatin1Char('0'));
                }
                if (w) {
                    QPixmap pix_win=w->grab();
                    /*QPixmap pix;
                     if (screenshotIncludeWindowTitle) {
                        pix=w->screen()->grabWindow(0, w->frameGeometry().x(), w->frameGeometry().y(), w->frameGeometry().width(), w->frameGeometry().height());
                    } else {
                        pix=pix_win;
                    }*/
                    if (saveScreenshot) {
                        if (scaleDownFromHighDPI && pix_win.devicePixelRatio()>1.0) {
                            pix_win.scaled((QSizeF(pix_win.size())/pix_win.devicePixelRatio()).toSize()).save(screenshotDir.absoluteFilePath(bn+".png"));
                        } else {
                            pix_win.save(screenshotDir.absoluteFilePath(bn+".png"));
                        }
                    }
                    if (saveSmallScreenshot) {
                        QPixmap img=pix_win.scaledToWidth(150, Qt::SmoothTransformation);
                        img.save(screenshotDir.absoluteFilePath(bn+"_small.png"));
                    }
                }
                if (plot) {
                    QString bnp=bn+"_plot";
                    QImage gr=plot->grabPixelImage();

                    if (saveScreenshotPlot) {
                        QString fn=bn+"_small.png";
                        if (saveScreenshot) fn=bnp+"_small.png";
                        if (scaleDownFromHighDPI && gr.devicePixelRatio()>1.0) {
                            gr.scaled((QSizeF(gr.size())/gr.devicePixelRatio()).toSize()).save(screenshotDir.absoluteFilePath(fn));
                        } else {
                            gr.save(screenshotDir.absoluteFilePath(fn));
                        }
                    }
                    if (saveSmallScreenshotPlot) {
                        QString fn=bn+"_small.png";
                        if (saveSmallScreenshot) fn=bnp+"_small.png";
                        QImage img=gr.scaledToWidth(150, Qt::SmoothTransformation);
                        img.save(screenshotDir.absoluteFilePath(fn));
                    }
                }
                iVisible++;
            }
        }
        return 0;
    } else {
        return QApplication::exec();
    }
}
