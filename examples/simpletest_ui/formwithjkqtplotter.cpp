#include "formwithjkqtplotter.h"
#include "ui_formwithjkqtplotter.h"

FormWithJKQtPlotter::FormWithJKQtPlotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWithJKQtPlotter)
{
    ui->setupUi(this);
    graph=new JKQTPxParsedFunctionLineGraph(ui->plot);
    graph->set_function(ui->edtEquation->text());
    graph->set_title(ui->edtEquation->text());
    ui->plot->addGraph(graph);
    ui->plot->setXY(-10,10,-10,10);
}

FormWithJKQtPlotter::~FormWithJKQtPlotter()
{
    delete ui;
}

void FormWithJKQtPlotter::on_chkLogX_toggled(bool checked)
{
    ui->plot->get_xAxis()->set_logAxis(checked);
    if (checked) {
        ui->plot->setX(1e-3,10);
    } else {
        ui->plot->setX(-10,10);
    }
}

void FormWithJKQtPlotter::on_chkLogY_toggled(bool checked)
{
    ui->plot->get_yAxis()->set_logAxis(checked);
    if (checked) {
        ui->plot->setY(1e-3,10);
    } else {
        ui->plot->setY(-10,10);
    }
}

void FormWithJKQtPlotter::on_btnReplot_clicked()
{
    graph->set_function(ui->edtEquation->text());
    graph->set_title(ui->edtEquation->text());
    ui->plot->update_plot();
}
