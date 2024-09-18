/** \example ui_bind_scrollbar.cpp
 * Use QScrollBar together with JKQtPlotter
 *
 * \ref JKQTPlotterUIScrollbar
 */

#include <QApplication>
#include <QScrollBar>
#include <QWidget>
#include <QBoxLayout>
#include <QGridLayout>

#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtplines.h"
#include "jkqtpexampleapplication.h"


void createXScrollingWindow(JKQTPExampleApplication& app, QWidget& win) {
    QVBoxLayout* lay=new QVBoxLayout();
    win.setLayout(lay);

    JKQTPlotter* plot=new JKQTPlotter(&win);
    lay->addWidget(plot);
    // 3. add a QScrollBar below the plot
    QScrollBar* scroll=new QScrollBar(Qt::Horizontal, &win);
    scroll->setMinimum(0);
    scroll->setMaximum(100);
    scroll->setPageStep(10);
    lay->addWidget(scroll);

    // 4. create a graph
    JKQTPDatastore* ds=plot->getDatastore();
    const int NPOINTS=10000;
    JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(plot);
    const size_t colX=ds->addLinearColumn(NPOINTS, 0, 100, "x");
    graph->setXColumn(colX);
    graph->setYColumn(ds->addCalculatedColumnFromColumn(colX, [](double x) { return 10.0*sin(x*3.0)*fabs(cos((x/8.0))); }, "f(x)"));
    graph->setDrawLine(true);
    graph->setSymbolType(JKQTPNoSymbol);
    // ... add the graphs to the plot, so it is actually displayed
    plot->addGraph(graph);

    // 5. set absolute plot range
    plot->setAbsoluteXY(0,100,-10,10);
    // show everything in y-direction
    plot->setY(-10,10);
    // fix y-range, so no zoming occurs in y
    plot->getYAxis()->setRangeFixed(true);


    // 6. connect scrollbar to plot
    QObject::connect(scroll, &QScrollBar::valueChanged, [plot,scroll](int value) {
        const double scrollRange=scroll->maximum()-scroll->minimum()+scroll->pageStep();
        const double scrollPos=scroll->value();
        const double scrollPageSize=scroll->pageStep();
        const double scrollRelative=scrollPos/scrollRange;
        const double plotFullRange=plot->getAbsoluteXMax()-plot->getAbsoluteXMin();
        const double plotViewRange=scrollPageSize/scrollRange*plotFullRange;
        const double plotViewStart=plot->getAbsoluteXMin()+scrollRelative*plotFullRange;
        plot->blockSignals(true);
        plot->setX(plotViewStart, plotViewStart+plotViewRange);
        plot->blockSignals(false);
    });
    scroll->setValue(1); // ensure to call slot once!

    // 7. connect plot to QScrollBar
    QObject::connect(plot, &JKQTPlotter::zoomChangedLocally, [scroll](double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* plot) {
        const double plotFullRange=plot->getAbsoluteXMax()-plot->getAbsoluteXMin();
        const double plotViewRange=newxmax-newxmin;
        const double plotRelViewRange=(plotViewRange>=plotFullRange)?1.0:(plotViewRange/(plotFullRange-plotViewRange));
        const double plotRelViewStart=(newxmin-plot->getAbsoluteXMin())/plotFullRange;
        const double scrollRange=scroll->maximum()-scroll->minimum();
        scroll->blockSignals(true);
        scroll->setPageStep(plotRelViewRange*scrollRange);
        scroll->setValue(plotRelViewStart*scrollRange);
        scroll->blockSignals(false);
    });

    // 8. show plotter and make it a decent size
    win.show();
    win.resize(400/plot->devicePixelRatioF(),300/plot->devicePixelRatioF());
    win.setWindowTitle("Bind JKQTPlotter to hor. QScrollBar");
}





void createXYScrollingWindow(JKQTPExampleApplication& app, QWidget& win) {
    QGridLayout* lay=new QGridLayout();
    win.setLayout(lay);

    JKQTPlotter* plot=new JKQTPlotter(&win);
    lay->addWidget(plot, 0,1);
    // 3. add a QScrollBar below the plot and one on the left
    QScrollBar* scrollX=new QScrollBar(Qt::Horizontal, &win);
    scrollX->setMinimum(0);
    scrollX->setMaximum(100);
    scrollX->setPageStep(20);
    lay->addWidget(scrollX,1,1);
    QScrollBar* scrollY=new QScrollBar(Qt::Vertical, &win);
    scrollY->setMinimum(0);
    scrollY->setMaximum(100);
    scrollY->setPageStep(100);
    scrollY->setInvertedAppearance(true);
    lay->addWidget(scrollY,0,0);

    // 4. create a graph
    JKQTPDatastore* ds=plot->getDatastore();
    const int NPOINTS=10000;
    JKQTPXYLineGraph* graph=new JKQTPXYLineGraph(plot);
    const size_t colX=ds->addLinearColumn(NPOINTS, 0, 100, "x");
    graph->setXColumn(colX);
    graph->setYColumn(ds->addCalculatedColumnFromColumn(colX, [](double x) { return 10.0*sin(x*3.0)*fabs(cos((x/8.0))); }, "f(x)"));
    graph->setDrawLine(true);
    graph->setSymbolType(JKQTPNoSymbol);
    // ... add the graphs to the plot, so it is actually displayed
    plot->addGraph(graph);

    // 5. set absolute plot range
    plot->setAbsoluteXY(0,100,-10,10);
    plot->setXY(0,20,-10,0);


    // 6. connect scrollbars to plot
    QObject::connect(scrollX, &QScrollBar::valueChanged, [plot,scrollX](int value) {
        const double scrollRange=scrollX->maximum()-scrollX->minimum()+scrollX->pageStep();
        const double scrollPos=scrollX->value();
        const double scrollPageSize=scrollX->pageStep();
        const double scrollRelative=scrollPos/scrollRange;
        const double plotFullRange=plot->getAbsoluteXMax()-plot->getAbsoluteXMin();
        const double plotViewRange=scrollPageSize/scrollRange*plotFullRange;
        const double plotViewStart=plot->getAbsoluteXMin()+scrollRelative*plotFullRange;
        plot->blockSignals(true);
        plot->setX(plotViewStart, plotViewStart+plotViewRange);
        plot->blockSignals(false);
    });
    scrollX->setValue(0); // ensure to call slot once!
    QObject::connect(scrollY, &QScrollBar::valueChanged, [plot,scrollY](int value) {
        const double scrollRange=scrollY->maximum()-scrollY->minimum()+scrollY->pageStep();
        const double scrollPos=scrollY->value();
        const double scrollPageSize=scrollY->pageStep();
        const double scrollRelative=scrollPos/scrollRange;
        const double plotFullRange=plot->getAbsoluteYMax()-plot->getAbsoluteYMin();
        const double plotViewRange=scrollPageSize/scrollRange*plotFullRange;
        const double plotViewStart=plot->getAbsoluteYMin()+scrollRelative*plotFullRange;
        plot->blockSignals(true);
        plot->setY(plotViewStart, plotViewStart+plotViewRange);
        plot->blockSignals(false);
    });
    scrollY->setValue(50); // ensure to call slot once!

    // 7. connect plot to QScrollBars
    QObject::connect(plot, &JKQTPlotter::zoomChangedLocally, [scrollX](double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* plot) {
        const double plotFullRange=plot->getAbsoluteXMax()-plot->getAbsoluteXMin();
        const double plotViewRange=newxmax-newxmin;
        const double plotRelViewRange=(plotViewRange>=plotFullRange)?1.0:(plotViewRange/(plotFullRange-plotViewRange));
        const double plotRelViewStart=(newxmin-plot->getAbsoluteXMin())/plotFullRange;
        const double scrollRange=scrollX->maximum()-scrollX->minimum();
        scrollX->blockSignals(true);
        scrollX->setPageStep(jkqtp_bounded<int>(1,plotRelViewRange*scrollRange,scrollRange));
        scrollX->setValue(jkqtp_bounded<int>(scrollX->minimum(),plotRelViewStart*scrollRange,scrollX->maximum()));
        scrollX->blockSignals(false);
    });
    QObject::connect(plot, &JKQTPlotter::zoomChangedLocally, [scrollY](double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* plot) {
        const double plotFullRange=plot->getAbsoluteYMax()-plot->getAbsoluteYMin();
        const double plotViewRange=newymax-newymin;
        const double plotRelViewRange=(plotViewRange>=plotFullRange)?1.0:(plotViewRange/(plotFullRange-plotViewRange));
        const double plotRelViewStart=(newymin-plot->getAbsoluteYMin())/plotFullRange;
        const double scrollRange=scrollY->maximum()-scrollY->minimum();
        scrollY->blockSignals(true);
        scrollY->setPageStep(jkqtp_bounded<int>(1,plotRelViewRange*scrollRange,scrollRange));
        scrollY->setValue(jkqtp_bounded<int>(scrollY->minimum(),plotRelViewStart*scrollRange,scrollY->maximum()));
        scrollY->blockSignals(false);
    });

    // 8. show plotter and make it a decent size
    win.show();
    win.resize(400/plot->devicePixelRatioF(),300/plot->devicePixelRatioF());
    win.setWindowTitle("Bind JKQTPlotter to hor.+ver. QScrollBar");
}

int main(int argc, char* argv[])
{

    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. create a window containing a plotter and a combobox to select the color palette
    //    ... and get a pointer to the internal datastore (for convenience)
    QWidget winX, winXY;

    createXScrollingWindow(app, winX);
    createXYScrollingWindow(app, winXY);

    return app.exec();
}
