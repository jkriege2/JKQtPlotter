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
    void addExportStepPlot(JKQTPlotter* plot);
    void addExportStepPlotFunctor(const std::function<JKQTPlotter*(void)>& fplot);
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
    struct Data {
        enum Type {
            FunctorType,
            PlotterType,
            PlotterFunctorType
        };
        Type type;
        std::function<void(void)> f;
        std::function<JKQTPlotter*(void)> plotf;
        JKQTPlotter* p;
        inline Data(const std::function<void(void)>& f_):
            type(FunctorType), f(f_), p(nullptr), plotf()
        {}
        inline Data(JKQTPlotter* p_):
            type(PlotterType), p(p_), f(), plotf()
        {}
        inline Data(std::function<JKQTPlotter*(void)> p_):
            type(PlotterFunctorType), plotf(p_), f(), p(nullptr)
        {}
    };

    QVector<Data> functors;
    void readCmdLine();
    QRect getBoundsWithoutColor(QImage qImage, const QColor &exclusionColor = Qt::white);

    void saveWidget(QWidget* w, int iVisible);;
};
