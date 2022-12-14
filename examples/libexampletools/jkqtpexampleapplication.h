#pragma once
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtpappsettingcontroller.h"
#include <QDir>
#include <functional>



class JKQTPExampleApplication: public QApplication {
    Q_OBJECT
public:
    JKQTPExampleApplication(int &argc, char **argv, bool waitForScreenshotReady=false);

    virtual ~JKQTPExampleApplication();
    void addExportStepFunctor(const std::function<void(void)>& f);
    int exec();
public Q_SLOTS:
    void notifyReadyForScreenshot();
protected:
    bool waitForScreenshotReady;
    bool readyForScreenshot;
    QDir screenshotDir;
    bool saveScreenshot;
    bool saveSmallScreenshot;
    bool saveScreenshotPlot;
    bool saveSmallScreenshotPlot;
    bool scaleDownFromHighDPI;
    bool iterateFunctorSteps;
    bool iterateFunctorStepsSupressInitial;
    QStringList screenshotBasename;
    QVector<std::function<void(void)>> functors;
    void readCmdLine();
    QRect getBoundsWithoutColor(QImage qImage, const QColor &exclusionColor = Qt::white);
};
