#include <QApplication>
#include <QLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 1. create a window that conatins a line-edit to edit a function
    //    and a JKQtPlotter to display the function, combine everything in a layout
    QWidget mainWin;
    QLineEdit* edit=new QLineEdit(&mainWin);
    edit->setToolTip("enter a function in dependence of the variable <tt>x</tt> and press ENTER to update the graph");
    QCheckBox* check=new QCheckBox("display sample points");
    QDoubleSpinBox* spinP1=new QDoubleSpinBox(&mainWin);
    spinP1->setValue(8);
    spinP1->setRange(-10000,10000);
    spinP1->setToolTip("enter a Value for parameter <tt>p1</tt> and press ENTER to update the graph");
    QDoubleSpinBox* spinP2=new QDoubleSpinBox(&mainWin);
    spinP2->setValue(4);
    spinP2->setRange(-10000,10000);
    spinP2->setToolTip("enter a Value for parameter <tt>p1</tt> and press ENTER to update the graph");
    JKQtPlotter* plot=new JKQtPlotter(&mainWin);
    QFormLayout* flayout=new QFormLayout;
    QVBoxLayout* layout=new QVBoxLayout;
    mainWin.setLayout(layout);
    flayout->addRow("equation:", edit);
    flayout->addRow("p1 =", spinP1);
    flayout->addRow("p2 =", spinP2);
    flayout->addRow("", check);
    layout->addLayout(flayout);
    layout->addWidget(plot);

    // 2. now we add a JKQTPxParsedFunctionLineGraph object, which will draw the function from
    //    the line edit
    JKQTPxParsedFunctionLineGraph* parsedFunc=new JKQTPxParsedFunctionLineGraph(plot);
    plot->addGraph(parsedFunc);
    //    finally we connect the line edit with the graph, whenever RETURN is pressed,
    //    the graph is updated:
    auto updateGraphFunctor=
       [=]() {
            parsedFunc->set_title(QString("user function: \\verb{"+edit->text()+"}, p_1=%1, p_2=%2").arg(spinP1->value()).arg(spinP2->value()));
            parsedFunc->set_function(edit->text());
            parsedFunc->set_paramsV(spinP1->value(), spinP2->value());
            parsedFunc->set_displaySamplePoints(check->isChecked());
            plot->update_plot();
       };
    QObject::connect(edit, &QLineEdit::returnPressed, updateGraphFunctor);
    QObject::connect(edit, &QLineEdit::editingFinished, updateGraphFunctor);
    QObject::connect(check, &QCheckBox::toggled, updateGraphFunctor);
    QObject::connect(spinP1, &QDoubleSpinBox::editingFinished, updateGraphFunctor);
    QObject::connect(spinP2, &QDoubleSpinBox::editingFinished, updateGraphFunctor);
    edit->setText("sin(x*p1)*exp(-x/p2)");
    updateGraphFunctor();


    // 3. set some axis properties (we use LaTeX for nice equation rendering)
    plot->getXAxis()->set_axisLabel(QObject::tr("x-axis"));
    plot->getYAxis()->set_axisLabel(QObject::tr("y-axis"));


    // 4. scale the plot so the graph is contained
    plot->setXY(-10,10,-10,10);

    // show window and make it a decent size
    mainWin.show();
    mainWin.resize(600,400);

    return app.exec();
}
