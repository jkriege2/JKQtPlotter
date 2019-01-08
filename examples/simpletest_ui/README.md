[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)

# JKQtPlotter

## Using a JKQtPlotter inside a Qt User Interface Designer (UI) File
This project (see `./examples/simpletest_ui/`) demonstrates how to create add a `JKQtPlotter` inside the Qt Form Editor (e.g. called from of Qt Creator) into a widget. 

### Instructions on how to use JKQtPlotter in the Qt Form Designer

For this to work you have to follow the steps shown below:

1. add a new UI-file to your project and open it in the Form Editor. Then right-click the form and select `Promote Widgets ...`:
   ![step1](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/uidesigner_step1.png)

2. In the dialog that opens, you have to define `JKQtPlotter` as a promotion to `QWidget` as shown below. Finally store the settings by clicking `Add` and closing the dialog with `Close`.
   ![step2](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/uidesigner_step2.png)

3. Now you can add a `QWidget`from the side-bar to the form and then promote it to `JKQtPlotter`, by selecting and right-clicking the `QWidget` and then selecting `Promote To | JKQtPlotter`:
   ![step3](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/doc/images/uidesigner_step3.png)
   
   
   
### QMake-Project of this example

The QMake project for such a project looks like this (see [`jkqtplotter_simpletest_ui.pro`](https://github.com/jkriege2/JKQtPlotter/blob/master/examples/simpletest_ui/jkqtplotter_simpletest_ui.pro):
```qmake
# source code for this simple demo
SOURCES =  jkqtplotter_simpletest_ui.cpp \
           formwithjkqtplotter.cpp

FORMS +=   formwithjkqtplotter.ui

HEADERS += formwithjkqtplotter.h

# configure Qt
CONFIG += qt
QT += core gui xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest_ui

# include JKQtPlotter source headers and link against library
DEPENDPATH += . ../../lib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")

win32-msvc*: DEFINES += _USE_MATH_DEFINES

# here you can activate some debug options
#DEFINES += SHOW_JKQTPLOTTER_DEBUG
#DEFINES += JKQTBP_AUTOTIMER
```

As you can see there is a main CPP-file (shown below) and a `formwithjkqtplotter.ui`-file with the `formwithjkqtplotter.cpp/.h`-files that are used to implement the functionality behind the form (see next chapter).

The soruce code of the main application is simply instanciating the Form class `FormWithJKQtPlotter` from `formwithjkqtplotter.ui/.h/.cpp`:
```c++
#include <QApplication>
#include "formwithjkqtplotter.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    FormWithJKQtPlotter plot;
    plot.show();
    plot.resize(600,400);

    return app.exec();
}
```

### Form Class `FormWithJKQtPlotter`

The Form was designed in the Qt Form Designer within Qt Creator, using the method described above (see `formwithjkqtplotter.ui`):
![jkqtplotter_simpletest_ui1](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_ui_widget.png)

In addition the example implements some simple functionality in the `formwithjkqtplotter.cpp/.h`-files. A single graph, which parses and plots a function (from a `QLineEdit`) is added to the plot in the constructor:
```c++
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
```

Then three slots react to user interactions. First two interactions set the x- or y-axis to linear or logarithmic, depending on the state of the two check-boxes. It also sets the scaling of the axes to meaningful default values:
```c++
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
```

A third slot is connected to the `clicked()`-event of the `QPushButton` labeled "REPLOT!". This slot reads the function from the `QLineEdit` and updates the plot with it:
```c++
void FormWithJKQtPlotter::on_btnReplot_clicked()
{
    graph->set_function(ui->edtEquation->text());
    graph->set_title(ui->edtEquation->text());
    ui->plot->update_plot();
}
```

```
The result looks like this:
![jkqtplotter_simpletest_ui1](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_ui.png)

If you set both axes to logarithmic and modify the plotted function a bit, you get:
![jkqtplotter_simpletest_ui1](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtplotter_simpletest_ui_loglog.png)



[Back to JKQTPlotter main page](https://github.com/jkriege2/JKQtPlotter/)