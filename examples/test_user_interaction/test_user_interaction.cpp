/** \example test_user_interaction.cpp
 * JKQTPlotter: Tutorial: User Interaction in JKQTPlotter
 *
 * \ref JKQTPlotterUserInteraction
 * \see \ref JKQTPLOTTER_USERINTERACTION
 */

#include "test_user_interaction.h"
#include <QMenuBar>

TestUserInteraction::TestUserInteraction(QWidget *parent) :
    QMainWindow(parent)
{   
    QWidget* w=new QWidget(this);
    setCentralWidget(w);

    // setup layouts for form
    layout=new QGridLayout;
    layForm=new QFormLayout;
    layChk=new QHBoxLayout;
    layout->addLayout(layChk,0,0);
    layout->addLayout(layForm,1,0);

    // generate a JKQTPlotter and initialize some plot data
    plot=new JKQTPlotter(this);
    layout->addWidget(plot,2,0);
    initPlot();

    // add some of the default QActions from the JKQTPlotter to the window menu
    // Some of the are also available in the context menu and toolbar of the JKQTPlotter
    // others are not
    QMenu* menuPlot=menuBar()->addMenu("Plot-Menu");
    menuPlot->addAction(plot->getPlotter()->getActionPrint());
    QMenu* menuPlotS=menuPlot->addMenu("Save ...");
    menuPlotS->addAction(plot->getPlotter()->getActionSaveData());
    menuPlotS->addAction(plot->getPlotter()->getActionSavePDF()); // not available from JKQTPlotter by default
    menuPlotS->addAction(plot->getPlotter()->getActionSavePlot());
    QMenu* menuPlotZ=menuPlot->addMenu("Zoom ...");
    menuPlotZ->addAction(plot->getPlotter()->getActionZoomAll());
    menuPlotZ->addAction(plot->getPlotter()->getActionZoomIn());
    menuPlotZ->addAction(plot->getPlotter()->getActionZoomOut());

    // add a checkbox to show and hide the position display label in the plot
    chkPositionDisplay=new QCheckBox(tr("show mouse cursor position"), this);
    chkPositionDisplay->setChecked(plot->isMousePositionShown());
    connect(chkPositionDisplay, SIGNAL(toggled(bool)), plot, SLOT(setMousePositionShown(bool)));
    layChk->addWidget(chkPositionDisplay);

    // add a checkbox to en-/disable the toolbar of the JKQTPlotter
    chkShowToolbar=new QCheckBox(tr("enable toolbar"), this);
    chkShowToolbar->setChecked(plot->isToolbarVisible());
    connect(chkShowToolbar, SIGNAL(toggled(bool)), plot, SLOT(setToolbarVisible(bool)));
    layChk->addWidget(chkShowToolbar);

    // add a checkbox to switch the toolbar between always visible and the hiding mode,
    // where it only appears, when the mouse hovers over its designated area
    chkToolbarAlwaysOn=new QCheckBox(tr("toolbar 'always on'"), this);
    chkToolbarAlwaysOn->setChecked(plot->isToolbarAlwaysOn());
    connect(chkToolbarAlwaysOn, SIGNAL(toggled(bool)), plot, SLOT(setToolbarAlwaysOn(bool)));
    layChk->addWidget(chkToolbarAlwaysOn);

    // add a checkbox to switch the grid on and off
    chkGrid=new QCheckBox(tr("show grid in plot"), this);
    chkGrid->setChecked(true);
    connect(chkGrid, SIGNAL(toggled(bool)), plot, SLOT(setGrid(bool)));
    layChk->addWidget(chkGrid);

    w->setLayout(layout);
    resize(800,600);
}

void TestUserInteraction::initPlot()
{
    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=plot->getDatastore();

    // 2. now we create data for a simple plot (a sine curve)
    QVector<double> X, Y1, Y2;
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*M_PI;
        X<<x;
        Y1<<sin(x);
        Y2<<sin(x)*exp(-0.2*x);
    }

    // 3. make data available to JKQTPlotter by adding it to the internal datastore.
    //    Note: In this step the data is copied (of not specified otherwise), so you can
    //          reuse X and Y afterwards!
    //    the variables columnX and columnY will contain the internal column ID of the newly
    //    created columns with names "x" and "y" and the (copied) data from X and Y.
    size_t columnX=ds->addCopiedColumn(X, "x");
    size_t columnY1=ds->addCopiedColumn(Y1, "y1");
    size_t columnY2=ds->addCopiedColumn(Y2, "y2");

    // 4. create a graph in the plot, which plots the dataset X/Y:
    JKQTPXYLineGraph* graph1=new JKQTPXYLineGraph(plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY1);
    graph1->setTitle(QObject::tr("sine graph"));
    plot->addGraph(graph1);

    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(plot);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setTitle(QObject::tr("sine*exp graph"));
    plot->addGraph(graph2);

    // 6. autoscale the plot so the graph is contained
    plot->zoomToFit();

    plot->getPlotter()->setPlotLabel("Sine Plots - User-Interaction Example");
}
