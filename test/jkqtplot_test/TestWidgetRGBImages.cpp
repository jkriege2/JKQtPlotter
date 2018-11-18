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
    QVBoxLayout* layoutrgb=new QVBoxLayout(this);
    QFormLayout* frm=new QFormLayout(nullptr);
    winrgb->setLayout(layoutrgb);
    winrgb->resize(1000, 800);
    plotImgRGB=new JKQtPlotter(true, winrgb);
    plotImgRGB->set_doDrawing(false);
    plotImgRGB->get_plotter()->set_plotLabel(tr("\\textbf{RGB image plot}"));
    plotImgRGB->setObjectName("plotImgRGB");
    plotImgRGB->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_img_rgb/");

    /*plotImgRGB->get_plotter()->set_maintainAspectRatio(true);
    plotImgRGB->get_plotter()->set_aspectRatio(1.0);
    plotImgRGB->get_plotter()->set_maintainAxisAspectRatio(true);
    plotImgRGB->get_plotter()->set_axisAspectRatio(1.0);*/

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
    rgbimg=new JKQTPRGBMathImage(0,0,10,10, JKQTPMathImageBase::DoubleArray, rgb_rimg2, 100, 100, plotImgRGB->get_plotter());
    rgbimg->set_imageMin(-1);
    rgbimg->set_imageMax(0.5);
    rgbimg->set_imageMinB(-1);
    rgbimg->set_imageMaxB(0.5);
    rgbimg->set_imageName("R image");
    rgbimg->set_imageNameG("G image");
    rgbimg->set_imageNameB("B image");
    rgbimg->set_data(rgb_rimg2, rgb_gimg2, rgb_bimg2, 100, 100, JKQTPMathImageBase::DoubleArray);
    rgbimg->set_dataModifier(rgb_aimg2, JKQTPMathImageBase::DoubleArray);
    rgbimg->set_modifierMode(JKQTPMathImageBase::ModifyNone);
    plotImgRGB->get_plotter()->addGraph(rgbimg);
    rgbimg->set_autoImageRange(false);
    rgbimg->set_colorBarTopVisible(true);
    rgbimg->set_colorBarRightVisible(true);
    rgbimg->set_colorbarsSideBySide(chkColorbars->isChecked());
    rgbimg2=new JKQTPColumnRGBMathImage(10,0,10,10,100,100, plotImgRGB->get_plotter());
    rgbimg2->set_imageMin(-1);
    rgbimg2->set_imageMax(0.5);
    rgbimg2->set_imageMinB(-1);
    rgbimg2->set_imageMaxB(0.5);
    rgbimg2->set_imageName("R image");
    rgbimg2->set_imageNameG("G image");
    rgbimg2->set_imageNameB("B image");
    rgbimg2->set_Nx(100);
    rgbimg2->set_Ny(100);
    rgbimg2->set_imageRColumn(rgb_colr=plotImgRGB->getDatastore()->addCopiedColumn(rgb_bimg2, 100*100, "R"));
    rgbimg2->set_imageGColumn(rgb_colg=plotImgRGB->getDatastore()->addCopiedColumn(rgb_gimg2, 100*100, "G"));
    rgbimg2->set_imageBColumn(rgb_colb=plotImgRGB->getDatastore()->addCopiedColumn(rgb_rimg2, 100*100, "B"));
    rgbimg2->set_modifierColumn(rgb_cola=plotImgRGB->getDatastore()->addCopiedColumn(rgb_aimg2, 100*100, "alpha"));
    rgbimg2->set_modifierMode(JKQTPMathImageBase::ModifyNone);
    //rgbimg2->set_data(rgb_rimg2, rgb_gimg2, rgb_bimg2, 100, 100, JKQTPMathImageBase::DoubleArray);
    plotImgRGB->get_plotter()->addGraph(rgbimg2);
    rgbimg2->set_autoImageRange(true);
    rgbimg2->set_colorBarTopVisible(true);
    rgbimg2->set_colorBarRightVisible(true);
    rgbimg2->set_colorbarsSideBySide(chkColorbars->isChecked());
    plotImgRGB->setAbsoluteXY(0,20,0,10);
    plotImgRGB->set_doDrawing(true);
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
    rgbimg->set_colorbarsSideBySide(checked);
    rgbimg2->set_colorbarsSideBySide(checked);
    plotImgRGB->update_plot();
}

void TestWidgetRGBImages::setRGBMode1(int mode)
{
    rgbimg->set_rgbMode((JKQTPRGBMathImageRGBMode)mode);
    plotImgRGB->update_plot();
}

void TestWidgetRGBImages::setRGBMode2(int mode)
{
    rgbimg2->set_rgbMode((JKQTPRGBMathImageRGBMode)mode);
    plotImgRGB->update_plot();
}

void TestWidgetRGBImages::updateRGBChannels()
{
    if (chkRGBUseR->isChecked()) {
        rgbimg->set_data(rgb_rimg2, 100, 100);
        rgbimg2->set_imageRColumn(rgb_colr);
    } else {
        rgbimg->set_data(nullptr, 100, 100);
        rgbimg2->set_imageRColumn(-1);
    }
    if (chkRGBUseG->isChecked()) {
        rgbimg->set_dataG(rgb_gimg2);
        rgbimg2->set_imageGColumn(rgb_colg);
    } else {
        rgbimg->set_dataG(nullptr);
        rgbimg2->set_imageGColumn(-1);
    }
    if (chkRGBUseB->isChecked()) {
        rgbimg->set_dataB(rgb_bimg2);
        rgbimg2->set_imageBColumn(rgb_colb);
    } else {
        rgbimg->set_dataB(nullptr);
        rgbimg2->set_imageBColumn(-1);
    }
    plotImgRGB->update_plot();
}

void TestWidgetRGBImages::setRGBModifier(int item)
{
    rgbimg->set_modifierMode(JKQTPMathImage::ModifierMode(item));
    rgbimg2->set_modifierMode(JKQTPMathImage::ModifierMode(item));
    plotImgRGB->update_plot();
}

