#include "TestWidgetImages.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#include "jkqtplotter/gui/jkqtpcomboboxes.h"
#include "jkqtplotter/graphs/jkqtpimageoverlays.h"
#include "jkqtplotter/graphs/jkqtpimagergb.h"







TestWidgetImages::TestWidgetImages(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout* layouti=new QVBoxLayout();
    QFormLayout* layWid=new QFormLayout;
    layouti->addLayout(layWid);
    setLayout(layouti);
    resize(1000, 800);
    plotImg=new JKQTPlotter(true, this);
    plotImg->setPlotUpdateEnabled(false);
    plotImg->getPlotter()->setPlotLabel(tr("\\textbf{image plot}"));
    plotImg->setObjectName("plotImg");
    plotImg->getPlotter()->setUserSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_img/");

    plotImg->getPlotter()->setMaintainAspectRatio(true);
    plotImg->getPlotter()->setAspectRatio(3.0/2.0);
    plotImg->getPlotter()->setMaintainAxisAspectRatio(true);
    plotImg->getPlotter()->setAxisAspectRatio(4.0);
    /*plotImg->getPlotter()->getXAxis()->setLinkedAxis(plotImg->getPlotter()->getYAxis());
    plotImg->getPlotter()->getXAxis()->setChangeWidthToAspectRatio(true);
    plotImg->getPlotter()->getXAxis()->setAspectRatio(1.0);*/


    layouti->addWidget(plotImg);
    plotImg->setXY(0,30,0,20);
    plotImg->setAbsoluteXY(0,30,0,20);
    img1=QImage(":/lena.png");
    JKQTPImage* pimg1=new JKQTPImage(0,0,10,20, &img1, plotImg->getPlotter());
    pimg1->setTitle("LENA");
    plotImg->getPlotter()->addGraph(pimg1);
    double* img2=(double*)calloc(100*100,sizeof(double));
    int32_t* img2M=(int32_t*)calloc(100*100,sizeof(int32_t));
    bool* bimg2=(bool*)calloc(100*100,sizeof(bool));
    for (int x=0; x<100; x++) {
        for (int y=0; y<100; y++) {
            img2[y*100+x]=sin(2*JKQTPSTATISTICS_PI*sqrt(1.0+x*x+y*y)/20);
            img2M[y*100+x]=abs(y-x);
            bimg2[y*100+x]=static_cast<double>(rand())/static_cast<double>(RAND_MAX)>0.9;
        }
    }
    pimg2=new JKQTPMathImage(10,0,10,10, JKQTPMathImageDataType::DoubleArray, img2, 100, 100, JKQTPMathImageGRAY, plotImg->getPlotter());
    pimg2->setImageMin(-1);
    pimg2->setImageMax(0.5);
    pimg2->setImageName("left image");
    pimg2->setTitle("left image");
    pimg2->setAutoImageRange(false);
    pimg2->setColorBarTopVisible(true);
    pimg2->setColorBarRightVisible(false);
    plotImg->getPlotter()->addGraph(pimg2);
    pimg3=new JKQTPMathImage(20,0,10,10, JKQTPMathImageDataType::DoubleArray, img2, 100, 100, JKQTPMathImageMATLAB, plotImg->getPlotter());
    pimg3->setColorBarTopVisible(true);
    pimg3->setColorBarRightVisible(true);
    pimg3->setImageMax(0.5);
    pimg3->setRangeMaxFailAction(JKQTPMathImageGivenColor);
    pimg3->setRangeMaxFailColor(QColor("salmon"));
    pimg3->setImageMin(-0.3);
    pimg3->setRangeMinFailAction(JKQTPMathImageTransparent);
    pimg3->setAutoImageRange(false);
    pimg3->setImageName("I(x,y)=\\sin\\left(\\frac{2\\pi\\cdot\\sqrt{1+x^2+y^2}}{20}\\right)");
    pimg3->setDataModifier(img2M, JKQTPMathImageDataType::Int32Array);
    pimg3->setModifierMode(JKQTPMathImageModifierMode::ModifyNone);
    pimg3->setTitle("right image");
    plotImg->getPlotter()->addGraph(pimg3);

    QColor ty=QColor("blue");
    ty.setAlphaF(0.1);
    JKQTPOverlayImage* oimg1=new JKQTPOverlayImage(10,0,10,10, bimg2, 100, 100, QColor("blue"), plotImg->getPlotter());
    oimg1->setTitle("overlay");
    oimg1->setFalseColor(ty);
    plotImg->getPlotter()->addGraph(oimg1);

    JKQTPOverlayImageEnhanced* oimg2=new JKQTPOverlayImageEnhanced(10,0,10,10, bimg2, 100, 100, QColor("red"), plotImg->getPlotter());
    oimg2->setTitle("enhanced overlay");
    oimg2->setDrawMode(JKQTPOverlayImageEnhanced::DrawAsSymbols);
    plotImg->getPlotter()->addGraph(oimg2);

    plotImg->setPlotUpdateEnabled(true);
    plotImg->zoomToFit();

    JKQTPMathImageColorPaletteComboBox* cmbPal=new JKQTPMathImageColorPaletteComboBox(this);
    layWid->addRow("left image palette", cmbPal);
    connect(cmbPal, SIGNAL(currentIndexChanged(int)), this, SLOT(setImgPalette1(int)));
    cmbPal=new JKQTPMathImageColorPaletteComboBox(this);
    layWid->addRow("right image palette", cmbPal);
    connect(cmbPal, SIGNAL(currentIndexChanged(int)), this, SLOT(setImgPalette(int)));
    QComboBox* cmb=new QComboBox(this);
    layWid->addRow("image modifier mode", cmb);
    cmb->addItem("none");
    cmb->addItem("value");
    cmb->addItem("lightness");
    cmb->addItem("alpha");
    connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setImgModifier(int)));
    cmb->setCurrentIndex(1);







}


void TestWidgetImages::setImgPalette(int index)
{
    pimg3->setColorPalette(index);
    plotImg->redrawPlot();
}

void TestWidgetImages::setImgPalette1(int index)
{
    pimg2->setColorPalette(index);
    plotImg->redrawPlot();
}

void TestWidgetImages::setImgModifier(int index)
{
    pimg3->setModifierMode(static_cast<JKQTPMathImageModifierMode>(index));
    plotImg->redrawPlot();
}
