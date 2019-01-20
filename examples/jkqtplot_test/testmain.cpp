#include "testmain.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#include "TestWidgetBarcharts.h"
#include "TestWidgetGraphs.h"
#include "TestWidgetLogGraphs.h"
#include "TestWidgetImages.h"
#include "TestWidgetRGBImages.h"
#include "TestWidgetGeometry.h"
#include "TestWidgetFunctionPlots.h"
#include "TestWidgetEmptyPlot.h"
#include "TestWidgetPeaksPlots.h"
#include "TestWidgetContourPlots.h"
#include "TestWidgetParamScatterPlots.h"

#ifdef Q_WS_WIN
#include "jkqtpemfengineadapter.h"
#endif




TestMain::TestMain(QWidget *parent) :
    QTabWidget(parent)
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    #ifdef Q_WS_WIN
        JKQTBasePlotter::registerPaintDeviceAdapter(new JKQTPEmfEngineAdapter());
    #endif
#endif




    // draw to screen
    addTab(new TestWidgetGraphs(this), QString("graphs"));
    addTab(new TestWidgetLogGraphs(this), QString("log graphs"));
    addTab(new TestWidgetImages(this), QString("images"));
    addTab(new TestWidgetRGBImages(this), QString("RGB images"));
    addTab(new TestWidgetGeometry(this), QString("geometry"));
    addTab(new TestWidgetFunctionPlots(this), QString("functionplots"));
    addTab(new TestWidgetEmptyPlot(this), QString("empty plot"));
    addTab(new TestWidgetPeaksPlots(this), QString("peaks plot"));
    addTab(new TestWidgetContourPlots(this), QString("contour plot"));
    addTab(new TestWidgetParamScatterPlots(this), QString("parametrized scatter plot"));
    addTab(new TestWidgetBarcharts(this), QString("barcharts"));
    setCurrentIndex(0);


    showMaximized();

}
