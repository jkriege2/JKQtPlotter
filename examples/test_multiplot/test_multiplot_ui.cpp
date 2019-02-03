#include "test_multiplot_ui.h"
#include "ui_test_multiplot_ui.h"

TestMultiplotUI::TestMultiplotUI(JKQTPlotter *plotTop_, JKQTPlotter *plotBot_, JKQTPlotter *plotRight_, QGridLayout *plotLayout_, QWidget *parent) :
    QDialog(parent),
    plotTop(plotTop_),
    plotBot(plotBot_),
    plotRight(plotRight_),
    plotLayout(plotLayout_),
    ui(new Ui::TestMultiplotUI)
{
    ui->setupUi(this);
}

TestMultiplotUI::~TestMultiplotUI()
{
    delete ui;
}

void TestMultiplotUI::updatePlot()
{
    plotLayout->setRowStretch(0, ui->spinRow0Stretch->value());
    plotLayout->setRowStretch(1, ui->spinRow1Stretch->value());
    plotLayout->setColumnStretch(0, ui->spinCol0Stretch->value());
    plotLayout->setColumnStretch(1, ui->spinCol1Stretch->value());
    plotBot->getPlotter()->resetMasterSynchronization();
    plotRight->getPlotter()->resetMasterSynchronization();
    plotBot->synchronizeToMaster(plotTop, JKQTBasePlotter::sdXAxis, ui->chkSyncWidthTopBot->isChecked(), ui->chkSyncZoomTopBot->isChecked(), ui->chkSyncZoomTopBot->isChecked());
    plotRight->synchronizeToMaster(plotBot, JKQTBasePlotter::sdYAxis, ui->chkSyncHeightLeftRight->isChecked(), ui->chkSyncZoomLeftRight->isChecked(), ui->chkSyncZoomLeftRight->isChecked());

    plotTop->getPlotter()->setShowKey(false);
    plotBot->getYAxis()->setAxisLabel("");
}
