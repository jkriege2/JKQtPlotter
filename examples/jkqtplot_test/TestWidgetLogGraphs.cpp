#include "TestWidgetLogGraphs.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>


#define N2 50



TestWidgetLogGraphs::TestWidgetLogGraphs(QWidget *parent) :
    QWidget(parent)
{

    JKQtPlotter* plotLOG=new JKQtPlotter(this);
    plotLOG->set_doDrawing(false);
    plotLOG->setObjectName("plotLOG");
    plotLOG->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plotLOG/");

    JKQTPdatastore* ds=plotLOG->get_plotter()->getDatastore();
    QVector<double> x,y;
    for (unsigned int i=0; i<N2; i++) {
        const double xx=static_cast<double>(i+1)/static_cast<double>(N2)*15.0;
        x<<xx;
        y<<pow(5,xx/5.0);
    }

    size_t cx2=ds->addCopiedColumn(x, "x2");
    size_t cy23=ds->addCopiedColumn(y, "y23");


    QVBoxLayout* layout=new QVBoxLayout();
    layout->addWidget(plotLOG);
    QFormLayout* layWid=new QFormLayout;
    layout->addLayout(layWid);
    setLayout(layout);
    resize(1000, 800);


    plotLOG->get_plotter()->addGraph(cx2, cy23, "data, $f(x)=5^{x/5}$", JKQTPlines);


    plotLOG->set_doDrawing(true);
    plotLOG->get_plotter()->zoomToFit(true, true);//, true);
    plotLOG->get_plotter()->set_keyAutosize(true);

    QCheckBox* chklogX=new QCheckBox("logarithmic X-axis", this);
    QCheckBox* chklogY=new QCheckBox("logarithmic Y-axis", this);
    connect(chklogX, SIGNAL(toggled(bool)), plotLOG->get_plotter()->get_xAxis(), SLOT(set_logAxis(bool)));
    connect(chklogY, SIGNAL(toggled(bool)), plotLOG->get_plotter()->get_yAxis(), SLOT(set_logAxis(bool)));
    layWid->addWidget(chklogX);
    layWid->addWidget(chklogY);


    QCheckBox* chkXMinorTicks=new QCheckBox("minor X-tick labels", this);
    QCheckBox* chkYMinorTicks=new QCheckBox("minor Y-tick labels", this);
    connect(chkXMinorTicks, SIGNAL(toggled(bool)), plotLOG->get_plotter()->get_xAxis(), SLOT(set_minorTickLabelsEnabled(bool)));
    connect(chkYMinorTicks, SIGNAL(toggled(bool)), plotLOG->get_plotter()->get_yAxis(), SLOT(set_minorTickLabelsEnabled(bool)));
    layWid->addWidget(chkXMinorTicks);
    layWid->addWidget(chkYMinorTicks);

    QCheckBox* chkFullNumberMinor=new QCheckBox("full minor numbers", this);
    connect(chkFullNumberMinor, SIGNAL(toggled(bool)), plotLOG->get_plotter()->get_xAxis(), SLOT(set_minorTickLabelFullNumber(bool)));
    connect(chkFullNumberMinor, SIGNAL(toggled(bool)), plotLOG->get_plotter()->get_yAxis(), SLOT(set_minorTickLabelFullNumber(bool)));
    layWid->addWidget(chkFullNumberMinor);

    QComboBox* cmbTickMode=new QComboBox(this);
    cmbTickMode->addItem("JKQTPLTMLinOrLog");
    cmbTickMode->addItem("JKQTPLTMLin");
    cmbTickMode->addItem("JKQTPLTMPower");
    connect(cmbTickMode, SIGNAL(currentIndexChanged(int)), plotLOG->get_plotter()->get_xAxis(), SLOT(set_tickMode(int)));
    layWid->addRow("X-Axis TickMode", cmbTickMode);

    cmbTickMode=new QComboBox(this);
    cmbTickMode->addItem("JKQTPLTMLinOrLog");
    cmbTickMode->addItem("JKQTPLTMLin");
    cmbTickMode->addItem("JKQTPLTMPower");
    connect(cmbTickMode, SIGNAL(currentIndexChanged(int)), plotLOG->get_plotter()->get_yAxis(), SLOT(set_tickMode(int)));
    layWid->addRow("Y-Axis TickMode", cmbTickMode);
    cmbTickMode->setCurrentIndex(0);
    plotLOG->get_plotter()->get_yAxis()->set_tickMode(JKQTPLTMLinOrPower);
    plotLOG->get_plotter()->get_yAxis()->set_minorTicks(1);

    plotLOG->get_plotter()->get_yAxis()->set_drawMode2(JKQTPCADMcomplete);
    plotLOG->get_plotter()->get_xAxis()->set_drawMode2(JKQTPCADMcomplete);



    QSpinBox* spinMinTicks=new QSpinBox(this);
    spinMinTicks->setRange(0,999);
    connect(spinMinTicks, SIGNAL(valueChanged(int)), plotLOG->get_plotter()->get_xAxis(), SLOT(set_minorTicks(int)));
    connect(spinMinTicks, SIGNAL(valueChanged(int)), plotLOG->get_plotter()->get_yAxis(), SLOT(set_minorTicks(int)));
    layWid->addRow("X/Y-axis minor tick count", spinMinTicks);
    spinMinTicks->setValue(1);

    QDoubleSpinBox* spinLOGAngle=new QDoubleSpinBox(this);
    spinLOGAngle->setRange(-90,90);
    spinLOGAngle->setSuffix(QLatin1String("°"));
    connect(spinLOGAngle, SIGNAL(valueChanged(double)), plotLOG->get_plotter()->get_xAxis(), SLOT(set_tickLabelAngle(double)));
    layWid->addRow("X-Axis tick label angle", spinLOGAngle);
    spinLOGAngle->setValue(0);

    spinLOGAngle=new QDoubleSpinBox(this);
    spinLOGAngle->setRange(-90,90);
    spinLOGAngle->setSuffix(QLatin1String("°"));
    connect(spinLOGAngle, SIGNAL(valueChanged(double)), plotLOG->get_plotter()->get_yAxis(), SLOT(set_tickLabelAngle(double)));
    layWid->addRow(tr("Y-axis label angle = "), spinLOGAngle);
    spinLOGAngle->setValue(0);


}

