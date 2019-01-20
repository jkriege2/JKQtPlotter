/** \example formwithjkqtplotter.cpp
 * JKQTPLotter: Examples: Using a JKQTPLotter inside a Qt User Interface Designer (UI) File
 *
 * \ref JKQTPLotterQtCreator
 */

#include "formwithjkqtplotter.h"
#include "ui_formwithjkqtplotter.h"

FormWithJKQTPLotter::FormWithJKQTPLotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWithJKQTPLotter)
{
    ui->setupUi(this);
    graph=new JKQTPXParsedFunctionLineGraph(ui->plot);
    graph->set_function(ui->edtEquation->text());
    graph->set_title(ui->edtEquation->text());
    ui->plot->addGraph(graph);
    ui->plot->setXY(-10,10,-10,10);
}

FormWithJKQTPLotter::~FormWithJKQTPLotter()
{
    delete ui;
}

void FormWithJKQTPLotter::on_chkLogX_toggled(bool checked)
{
    ui->plot->get_xAxis()->set_logAxis(checked);
    if (checked) {
        ui->plot->setX(1e-3,10);
    } else {
        ui->plot->setX(-10,10);
    }
}

void FormWithJKQTPLotter::on_chkLogY_toggled(bool checked)
{
    ui->plot->get_yAxis()->set_logAxis(checked);
    if (checked) {
        ui->plot->setY(1e-3,10);
    } else {
        ui->plot->setY(-10,10);
    }
}

void FormWithJKQTPLotter::on_btnReplot_clicked()
{
    graph->set_function(ui->edtEquation->text());
    graph->set_title(ui->edtEquation->text());
    ui->plot->update_plot();
}
