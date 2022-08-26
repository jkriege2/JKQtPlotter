#pragma once
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include <QDir>



class JKQTPExampleApplication: public QApplication {
    Q_OBJECT
public:
    JKQTPExampleApplication(int &argc, char **argv);

    virtual ~JKQTPExampleApplication();

    int exec();

protected:
    QDir screenshotDir;
    bool saveScreenshot;
    bool saveSmallScreenshot;
    bool saveScreenshotPlot;
    bool saveSmallScreenshotPlot;
    QString screenshotBasename;
    void readCmdLine();
    QRect getBoundsWithoutColor(QImage qImage, const QColor &exclusionColor = Qt::white);
};
