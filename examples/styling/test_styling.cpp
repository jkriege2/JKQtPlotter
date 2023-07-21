#include "test_styling.h"
#include "build_test_graphs.h"
#include "ui_test_styling.h"
#include "jkqtplotter/jkqtplotter.h"

#include <QDir>
#include <QTextStream>
#include <QFileDialog>

TestStyling::TestStyling(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestStyling)
{
    // set up the GUI
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,2);
    ui->splitter->setStretchFactor(1,1);

    plots=QVector<JKQTPlotter*>{ui->plot1, ui->plot2, ui->plot3, ui->plot4, ui->plot5, ui->plot6, ui->plot7, ui->plot8};
    for (auto p: plots) {
        bplots<<p->getPlotter();
    }

    // connect plots
    setupGridPrintingAndDatastores(bplots, QSize(4,2));


    // initialize a simple plot
    initPlot();

    // connect GUI to JKQTPlotter
    for  (auto plt: plots) {
        connect(ui->chkGrid, SIGNAL(toggled(bool)), plt, SLOT(setGrid(bool)));
        connect(ui->chkLogX, SIGNAL(toggled(bool)), plt->getXAxis(), SLOT(setLogAxis(bool)));
        connect(ui->chkLogY, SIGNAL(toggled(bool)), plt->getYAxis(), SLOT(setLogAxis(bool)));
        connect(ui->chkDebugBoxes, SIGNAL(toggled(bool)), plt->getPlotter(), SLOT(enableDebugShowRegionBoxes(bool)));
        connect(ui->chkDebugTextBoxes, SIGNAL(toggled(bool)), plt->getPlotter(), SLOT(enableDebugShowTextBoxes(bool)));
    }

    // store JKQTPlotter plotterStyle to a temporary INI-file
    // and then load that temporary file into the plainTextEdit,
    // using on_btnLoad_clicked(tmpfn). This ensures that the currently
    // set plotterStyle is visible in the plainTextEdit and can be edited
    // by the user
    QString tmpfn=QDir::tempPath()+"/jkqtplotter_config.tmp";
    {
        QSettings settings(tmpfn, QSettings::IniFormat);
        ui->plot1->saveCurrentPlotterStyle(settings);
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
        for (auto plt: plots) {
            plt->loadCurrentPlotterStyle(settings);
        }
        initPlot();
        ui->chkDebugBoxes->setChecked(ui->plot1->getPlotter()->isDebugShowRegionBoxesEnabled());
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
        QPixmap img=ui->plot1->grab();
        img.save(fn+".png", "png");
        ui->plot1->saveImage(fn+".export.png", false);
    }
}



void TestStyling::initPlot()
{

    for (auto plt: plots) {
        plt->setPlotUpdateEnabled(true);
        plt->redrawPlot();
        plt->setPlotUpdateEnabled(false);
    }

    buildPlots(bplots);

    for (auto plt: plots) {
        plt->setPlotUpdateEnabled(true);
        plt->redrawPlot();
    }

    if (!plotExtra) {
        plotExtra=new JKQTPlotter;
        plotExtra->setWindowTitle("2: symbols/lines");
        plotExtra->show();
        plotExtra->resize(width(),300);
        plotExtra->move(pos().x(), pos().y()+height()+32);
    }
    plotExtra->setPlotUpdateEnabled(false);

    buildLineSymbolSampler(plotExtra->getPlotter(), bplots[0]);

    plotExtra->setPlotUpdateEnabled(true);
    plotExtra->redrawPlot();
}
