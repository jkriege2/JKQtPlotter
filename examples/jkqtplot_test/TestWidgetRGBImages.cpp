#include "TestWidgetRGBImages.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>






TestWidgetRGBImages::TestWidgetRGBImages(QWidget *parent) :
    QWidget(parent)
{
    // plotter for images
    QWidget* winrgb=new QWidget(this);
    QVBoxLayout* layoutrgb=new QVBoxLayout();
    QFormLayout* frm=new QFormLayout();
    winrgb->setLayout(layoutrgb);
    winrgb->resize(1000, 800);
    plotImgRGB=new JKQTPlotter(true, winrgb);
    plotImgRGB->setPlotUpdateEnabled(false);
    plotImgRGB->getPlotter()->setPlotLabel(tr("\\textbf{RGB image plot}"));
    plotImgRGB->setObjectName("plotImgRGB");
    plotImgRGB->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_img_rgb/");

    /*plotImgRGB->getPlotter()->setMaintainAspectRatio(true);
    plotImgRGB->getPlotter()->setAspectRatio(1.0);
    plotImgRGB->getPlotter()->setMaintainAxisAspectRatio(true);
    plotImgRGB->getPlotter()->setAxisAspectRatio(1.0);*/

    QCheckBox* chkColorbars=new QCheckBox(tr("color bars share space"), winrgb);
    chkColorbars->setChecked(true);
    connect(chkColorbars, SIGNAL(toggled(bool)), this, SLOT(setRGBColorBars(bool)));
    layoutrgb->addWidget(chkColorbars);
    layoutrgb->addLayout(frm);
    layoutrgb->addWidget(plotImgRGB);
    plotImgRGB->setXY(0,100,0,100);
    plotImgRGB->setAbsoluteXY(0,100,0,100);

    rgb_rimg2=(double*)calloc(100*100,sizeof(double));
    rgb_bimg2=(double*)calloc(100*100,sizeof(double));
    rgb_gimg2=(double*)calloc(100*100,sizeof(double));
    rgb_aimg2=(double*)calloc(100*100,sizeof(double));
    for (int x=0; x<100; x++) {
        for (int y=0; y<100; y++) {
            rgb_rimg2[y*100+x]=sin(2*M_PI*sqrt(1.0+x*x+y*y)/20);
            rgb_bimg2[y*100+x]=sin(2*M_PI*sqrt(1.0+x*x+y*y)/30);
            rgb_gimg2[y*100+x]=cos(2*M_PI*sqrt(1.0+x*x+y*y)/30);
            rgb_aimg2[y*100+x]=double(x)/100.0;
        }
    }
    rgbimg=new JKQTPRGBMathImage(0,0,10,10, JKQTPMathImageDataType::DoubleArray, rgb_rimg2, 100, 100, plotImgRGB->getPlotter());
    rgbimg->setImageMin(-1);
    rgbimg->setImageMax(0.5);
    rgbimg->setImageMinB(-1);
    rgbimg->setImageMaxB(0.5);
    rgbimg->setImageName("R image");
    rgbimg->setImageNameG("G image");
    rgbimg->setImageNameB("B image");
    rgbimg->setData(rgb_rimg2, rgb_gimg2, rgb_bimg2, 100, 100, JKQTPMathImageDataType::DoubleArray);
    rgbimg->setDataModifier(rgb_aimg2, JKQTPMathImageDataType::DoubleArray);
    rgbimg->setModifierMode(JKQTPMathImageModifierMode::ModifyNone);
    plotImgRGB->getPlotter()->addGraph(rgbimg);
    rgbimg->setAutoImageRange(false);
    rgbimg->setColorBarTopVisible(true);
    rgbimg->setColorBarRightVisible(true);
    rgbimg->setColorbarsSideBySide(chkColorbars->isChecked());
    rgbimg2=new JKQTPColumnRGBMathImage(10,0,10,10,100,100, plotImgRGB->getPlotter());
    rgbimg2->setImageMin(-1);
    rgbimg2->setImageMax(0.5);
    rgbimg2->setImageMinB(-1);
    rgbimg2->setImageMaxB(0.5);
    rgbimg2->setImageName("R image");
    rgbimg2->setImageNameG("G image");
    rgbimg2->setImageNameB("B image");
    rgbimg2->setNx(100);
    rgbimg2->setNy(100);
    rgbimg2->setImageRColumn(rgb_colr=plotImgRGB->getDatastore()->addCopiedColumn(rgb_bimg2, 100*100, "R"));
    rgbimg2->setImageGColumn(rgb_colg=plotImgRGB->getDatastore()->addCopiedColumn(rgb_gimg2, 100*100, "G"));
    rgbimg2->setImageBColumn(rgb_colb=plotImgRGB->getDatastore()->addCopiedColumn(rgb_rimg2, 100*100, "B"));
    rgbimg2->setModifierColumn(rgb_cola=plotImgRGB->getDatastore()->addCopiedColumn(rgb_aimg2, 100*100, "alpha"));
    rgbimg2->setModifierMode(JKQTPMathImageModifierMode::ModifyNone);
    //rgbimg2->setData(rgb_rimg2, rgb_gimg2, rgb_bimg2, 100, 100, JKQTPMathImageDataType::DoubleArray);
    plotImgRGB->getPlotter()->addGraph(rgbimg2);
    rgbimg2->setAutoImageRange(true);
    rgbimg2->setColorBarTopVisible(true);
    rgbimg2->setColorBarRightVisible(true);
    rgbimg2->setColorbarsSideBySide(chkColorbars->isChecked());
    plotImgRGB->setAbsoluteXY(0,20,0,10);
    plotImgRGB->setPlotUpdateEnabled(true);
    //plotImgRGB->zoomToFit();
    plotImgRGB->setXY(0,20,0,10);

    QComboBox* cmb=new QComboBox(winrgb);
    frm->addRow(tr("mode 1:"), cmb);
    cmb->addItem("RGB");
    cmb->addItem("HSV");
    cmb->addItem("HSL");
    cmb->addItem("CMY");
    cmb->setCurrentIndex(0);
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setRGBMode1(int)));

    cmb=new QComboBox(winrgb);
    frm->addRow(tr("mode 2:"), cmb);
    cmb->addItem("RGB");
    cmb->addItem("HSV");
    cmb->addItem("HSL");
    cmb->addItem("CMY");
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setRGBMode2(int)));
    cmb->setCurrentIndex(1);

    cmb=new QComboBox(winrgb);
    frm->addRow(tr("modifier channel mode:"), cmb);
    cmb->addItem("none");
    cmb->addItem("value");
    cmb->addItem("lightness");
    cmb->addItem("alpha");
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setRGBModifier(int)));
    cmb->setCurrentIndex(0);

    QHBoxLayout* hblrgb=new QHBoxLayout();
    frm->addRow(tr("use channels:"), hblrgb);

    chkRGBUseR=new QCheckBox(tr("red"), winrgb);
    hblrgb->addWidget(chkRGBUseR);
    chkRGBUseR->setChecked(true);
    connect(chkRGBUseR, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));

    chkRGBUseG=new QCheckBox(tr("green"), winrgb);
    hblrgb->addWidget(chkRGBUseG);
    chkRGBUseG->setChecked(true);
    connect(chkRGBUseG, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));

    chkRGBUseB=new QCheckBox(tr("blue"), winrgb);
    hblrgb->addWidget(chkRGBUseB);
    chkRGBUseB->setChecked(true);
    connect(chkRGBUseB, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));

    chkRGBUseA=new QCheckBox(tr("alpha"), winrgb);
    hblrgb->addWidget(chkRGBUseA);
    chkRGBUseB->setChecked(false);
    connect(chkRGBUseB, SIGNAL(toggled(bool)), this, SLOT(updateRGBChannels()));





}

void TestWidgetRGBImages::setRGBColorBars(bool checked)
{
    rgbimg->setColorbarsSideBySide(checked);
    rgbimg2->setColorbarsSideBySide(checked);
    plotImgRGB->redrawPlot();
}

void TestWidgetRGBImages::setRGBMode1(int mode)
{
    rgbimg->setRgbMode((JKQTPRGBMathImageRGBMode)mode);
    plotImgRGB->redrawPlot();
}

void TestWidgetRGBImages::setRGBMode2(int mode)
{
    rgbimg2->setRgbMode((JKQTPRGBMathImageRGBMode)mode);
    plotImgRGB->redrawPlot();
}

void TestWidgetRGBImages::updateRGBChannels()
{
    if (chkRGBUseR->isChecked()) {
        rgbimg->setData(rgb_rimg2, 100, 100);
        rgbimg2->setImageRColumn(rgb_colr);
    } else {
        rgbimg->setData(nullptr, 100, 100);
        rgbimg2->setImageRColumn(-1);
    }
    if (chkRGBUseG->isChecked()) {
        rgbimg->setDataG(rgb_gimg2);
        rgbimg2->setImageGColumn(rgb_colg);
    } else {
        rgbimg->setDataG(nullptr);
        rgbimg2->setImageGColumn(-1);
    }
    if (chkRGBUseB->isChecked()) {
        rgbimg->setDataB(rgb_bimg2);
        rgbimg2->setImageBColumn(rgb_colb);
    } else {
        rgbimg->setDataB(nullptr);
        rgbimg2->setImageBColumn(-1);
    }
    plotImgRGB->redrawPlot();
}

void TestWidgetRGBImages::setRGBModifier(int item)
{
    rgbimg->setModifierMode(static_cast<JKQTPMathImageModifierMode>(item));
    rgbimg2->setModifierMode(static_cast<JKQTPMathImageModifierMode>(item));
    plotImgRGB->redrawPlot();
}

