#include "test_styling.h"
#include "ui_test_styling.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"
#include <QDir>
#include <QTextStream>
#include <QFileDialog>

TestStyling::TestStyling(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestStyling)
{
    // set up the GUI
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,1);

    // initialize a simple plot
    initPlot();

    // connect GUI to JKQTPlotter
    connect(ui->chkGrid, SIGNAL(toggled(bool)), ui->plot, SLOT(setGrid(bool)));
    connect(ui->chkLogX, SIGNAL(toggled(bool)), ui->plot->getXAxis(), SLOT(setLogAxis(bool)));
    connect(ui->chkLogY, SIGNAL(toggled(bool)), ui->plot->getYAxis(), SLOT(setLogAxis(bool)));
    connect(ui->chkDebugBoxes, SIGNAL(toggled(bool)), ui->plot->getPlotter(), SLOT(enableDebugShowRegionBoxes(bool)));

    // store JKQTPlotter plotterStyle to a temporary INI-file
    // and then load that temporary file into the plainTextEdit,
    // using on_btnLoad_clicked(tmpfn). This ensures that the currently
    // set plotterStyle is visible in the plainTextEdit and can be edited
    // by the user
    QString tmpfn=QDir::tempPath()+"/jkqtplotter_config.tmp";
    {
        QSettings settings(tmpfn, QSettings::IniFormat);
        ui->plot->saveCurrentPlotterStyle(settings);
    }
    on_btnLoad_clicked(tmpfn);
    QFile::remove(tmpfn);
}

TestStyling::~TestStyling()
{
    delete ui;
    if (!plotExtra.isNull()) {
        plotExtra->close();
        plotExtra.clear();
    }
}

void TestStyling::on_btnUpdate_clicked()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    // In this function, we store the data from the plainTextEdit into a temporary file
    // tmpfn and the use that file's contents to create a QSettings object, which is used
    // to read the plotterStyle into the JKQTPlotter-object ui->plot (using loadCurrentPlotterStyle(settings) )
    QString tmpfn=QDir::tempPath()+"/jkqtplotter_config.tmp";
    {
        QFile data(tmpfn);
        if (data.open(QFile::WriteOnly|QFile::Text)) {
          QTextStream out(&data);
          out << ui->plainTextEdit->toPlainText();
        }
    }
    {
        QSettings settings(tmpfn, QSettings::IniFormat);
        ui->plot->loadCurrentPlotterStyle(settings);
        initPlot();
        ui->chkDebugBoxes->setChecked(ui->plot->getPlotter()->isDebugShowRegionBoxesEnabled());
    }
    QFile::remove(tmpfn);
    QApplication::restoreOverrideCursor();
}

void TestStyling::on_btnLoad_clicked(const QString& filename)
{
    // loads a plotterTsyle INI file from harddisk into the plainTextEdit
    // and then calls on_btnUpdate_clicked() to apply it immediately to plot
    QString fn=filename;
    if (fn.size()==0) {
        fn=QFileDialog::getOpenFileName(this, tr("Open Styling INI-File"),
                                        "",
                                        tr("Styling INI (*.ini *.cfg *.txt)"));
    }
    if (fn.size()>0) {
        QFile data(fn);
        if (data.open(QFile::ReadOnly|QFile::Text)) {
          ui->plainTextEdit->setPlainText(data.readAll());
        }
    }
    on_btnUpdate_clicked();
}

void TestStyling::on_btnSave_clicked()
{
    // stores the contents of plainTextEdit into a file on the harddisk
    QString fn=QFileDialog::getSaveFileName(this, tr("Save Styling INI-File"),
                                        "",
                                        tr("Styling INI (*.ini *.cfg *.txt)"));
    if (fn.size()>0) {
        QFile data(fn);
        if (data.open(QFile::WriteOnly|QFile::Text)) {
          QTextStream out(&data);
          out << ui->plainTextEdit->toPlainText();
        }
        QPixmap img=ui->plot->grab();
        img.save(fn+".png", "png");
        ui->plot->saveImage(fn+".export.png", false);
    }
}



void TestStyling::initPlot()
{

    ui->plot->setPlotUpdateEnabled(false);

    // 0. get a pointer to the internal datastore (for convenience)
    JKQTPDatastore* ds=ui->plot->getDatastore();

    // 1. clear old graphs/data
    ui->plot->clearGraphs(true);
    ds->clear();

    // 2. now we create data for a simple plot (a sine curve)
    QVector<double> X, Y1, Y2, img, X3, Y3, Y3err, Xbar, Ybar, Ybar2;
    const int Ndata=100;
    for (int i=0; i<Ndata; i++) {
        const double x=double(i)/double(Ndata)*8.0*M_PI;
        X<<x;
        Y1<<1.1+sin(x);
        Y2<<1.1+sin(x)*exp(-0.2*x);
        if (i%5==0) {
            X3<<x;
            Y3<<(1.5+Y1.last());
            Y3err<<(double(i+5)/double(Ndata)*0.5);
        }
        if (i>0 && i%20==0) {
            Xbar<<x;
            Ybar<<(double(i+5)/double(Ndata)*(-1.5));
            Ybar2<<(double(i+5)/double(Ndata)*(-1.2));
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

    // 4. create a graph in the plot, which plots the dataset X/Y:
    JKQTPColumnMathImage* graphI=new JKQTPColumnMathImage(ui->plot);
    graphI->setImageColumn(columnImg);
    graphI->setNx(Ndata);
    graphI->setNy(Ndata);
    graphI->setColorBarTopVisible(true);
    graphI->setColorBarRightVisible(true);
    graphI->setX(0.5);
    graphI->setY(-1.5);
    graphI->setWidth(5);
    graphI->setHeight(1);
    graphI->setTitle(QObject::tr("2{\\times}Gauss"));
    ui->plot->addGraph(graphI);

    JKQTPXYLineGraph* graph1=new JKQTPXYLineGraph(ui->plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY1);
    graph1->setTitle(QObject::tr("sine graph $f(x)=\\sin(x)$"));
    ui->plot->addGraph(graph1);

    JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(ui->plot);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setSymbolType(JKQTPNoSymbol);
    //graph2->setTitle(QObject::tr("sine{\\cdot}exp graph $f(x)=\\sin(x)\\cdot\\exp\\left(-\\frac{x}{5}\\right)$"));
    graph2->setTitle(QObject::tr("sine*exp graph"));
    ui->plot->addGraph(graph2);

    JKQTPXYLineErrorGraph* graph3=new JKQTPXYLineErrorGraph(ui->plot);
    graph3->setXColumn(columnX3);
    graph3->setYColumn(columnY3);
    graph3->setYErrorColumn(columnY3err);
    graph3->setDrawLine(false);
    graph3->setTitle(QObject::tr("sine \\pm errors graph"));
    ui->plot->addGraph(graph3);

    JKQTPBarVerticalGraph* graphb=new JKQTPBarVerticalGraph(ui->plot);
    graphb->setXColumn(columnXbar);
    graphb->setYColumn(columnYbar);
    graphb->setTitle(QObject::tr("barchart"));
    ui->plot->addGraph(graphb);

    JKQTPBarVerticalGraph* graphb2=new JKQTPBarVerticalGraph(ui->plot);
    graphb2->setXColumn(columnXbar);
    graphb2->setYColumn(columnYbar2);
    graphb2->setTitle(QObject::tr("2^{nd} barchart"));
    ui->plot->addGraph(graphb2);
    graphb2->autoscaleBarWidthAndShiftSeparatedGroups();


    // 6. autoscale the plot so the graph is contained
    ui->plot->zoomToFit();

    ui->plot->getPlotter()->setPlotLabel("Sine Plots - User-Interaction Example");
    ui->plot->getXAxis()->setAxisLabel("x-axis");
    ui->plot->getYAxis()->setAxisLabel("y-axis");

    ui->plot->setPlotUpdateEnabled(true);
    ui->plot->redrawPlot();

    if (!plotExtra) {
        plotExtra=new JKQTPlotter;
        plotExtra->show();
        plotExtra->resize(width(),300);
        plotExtra->move(pos().x(), pos().y()+height()+32);
    }
    plotExtra->setPlotUpdateEnabled(false);
    plotExtra->clearGraphs(true);
    plotExtra->getPlotter()->setBackgroundColor(ui->plot->getPlotter()->getBackgroundColor());
    plotExtra->getPlotter()->setExportBackgroundColor(ui->plot->getPlotter()->getExportBackgroundColor());
    plotExtra->getPlotter()->setPlotBackgroundColor(ui->plot->getPlotter()->getPlotBackgroundColor());
    plotExtra->getXAxis()->setDrawMode1(JKQTPCADMnone);
    plotExtra->getXAxis()->setDrawMode2(JKQTPCADMnone);
    plotExtra->getYAxis()->setDrawMode1(JKQTPCADMnone);
    plotExtra->getYAxis()->setDrawMode2(JKQTPCADMnone);
    double x=1;
    double xlineend=6;
    double y=1;
    double xmax=15;
    int cnt=0;
    for (auto s: ui->plot->getPlotter()->getCurrentPlotterStyle().defaultGraphSymbols) {
        plotExtra->addGraph(new JKQTPGeoSymbol(plotExtra.data(), x, y, s, 12, QColor("red"), QColor("salmon") ));
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
    for (auto s: ui->plot->getPlotter()->getCurrentPlotterStyle().defaultGraphPenStyles) {
        plotExtra->addGraph(new JKQTPGeoLine(plotExtra.data(), 1,y,xlineend,y,QColor("red"), 2, s));
        plotExtra->addGraph(new JKQTPGeoLine(plotExtra.data(), 1,y-0.25,xlineend,y-0.25,QColor("maroon"), 1, s));
        y+=0.75;
    }
    x=xlineend+1;
    double dx=(xmax-xlineend-1.0)/static_cast<double>(ui->plot->getPlotter()->getCurrentPlotterStyle().defaultGraphColors.size());
    for (auto s: ui->plot->getPlotter()->getCurrentPlotterStyle().defaultGraphColors) {
        plotExtra->addGraph(new JKQTPGeoLine(plotExtra.data(), x,ycoltest,x,y-0.5,s,5));
        plotExtra->addGraph(new JKQTPGeoRectangle(plotExtra.data(), x+dx/2,(ycoltest+y-0.5)/2.0, dx*0.5, y-ycoltest-0.5, s,1, Qt::SolidLine, JKQTPGetDerivedColor(ui->plot->getPlotter()->getCurrentPlotterStyle().graphFillColorDerivationMode, s)));
        x+=dx;
    }
    plotExtra->zoomToFit();
    plotExtra->setX(0, plotExtra->getXMax()+2);
    plotExtra->setY(0, plotExtra->getYMax()+1);
    plotExtra->setGrid(false);
    plotExtra->getPlotter()->setPlotLabel("default symbols/lines/colors");
    plotExtra->setPlotUpdateEnabled(true);
    plotExtra->redrawPlot();
}
