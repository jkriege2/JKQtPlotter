#include "TestWidgetImages.h"
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QApplication>
#include "jkqtplottergui/jkqtpcomboboxes.h"






TestWidgetImages::TestWidgetImages(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout* layouti=new QVBoxLayout(this);
    QFormLayout* layWid=new QFormLayout;
    layouti->addLayout(layWid);
    setLayout(layouti);
    resize(1000, 800);
    plotImg=new JKQtPlotter(true, this);
    plotImg->set_doDrawing(false);
    plotImg->get_plotter()->set_plotLabel(tr("\\textbf{image plot}"));
    plotImg->setObjectName("plotImg");
    plotImg->get_plotter()->set_userSettigsFilename(QApplication::applicationDirPath()+"/usersettings.ini", "plot_img/");

    plotImg->get_plotter()->set_maintainAspectRatio(true);
    plotImg->get_plotter()->set_aspectRatio(3.0/2.0);
    plotImg->get_plotter()->set_maintainAxisAspectRatio(true);
    plotImg->get_plotter()->set_axisAspectRatio(4.0);
    /*plotImg->get_plotter()->getXAxis()->set_linkedAxis(plotImg->get_plotter()->getYAxis());
    plotImg->get_plotter()->getXAxis()->set_changeWidthToAspectRatio(true);
    plotImg->get_plotter()->getXAxis()->set_aspectRatio(1.0);*/


    layouti->addWidget(plotImg);
    plotImg->setXY(0,30,0,20);
    plotImg->setAbsoluteXY(0,30,0,20);
    img1=QImage(":/lena.png");
    JKQTPImage* pimg1=new JKQTPImage(0,0,10,20, &img1, plotImg->get_plotter());
    pimg1->set_title("LENA");
    plotImg->get_plotter()->addGraph(pimg1);
    double* img2=(double*)calloc(100*100,sizeof(double));
    int32_t* img2M=(int32_t*)calloc(100*100,sizeof(int32_t));
    bool* bimg2=(bool*)calloc(100*100,sizeof(bool));
    for (int x=0; x<100; x++) {
        for (int y=0; y<100; y++) {
            img2[y*100+x]=sin(2*M_PI*sqrt(1.0+x*x+y*y)/20);
            img2M[y*100+x]=abs(y-x);
            bimg2[y*100+x]=static_cast<double>(rand())/static_cast<double>(RAND_MAX)>0.9;
        }
    }
    pimg2=new JKQTPMathImage(10,0,10,10, JKQTPMathImageBase::DoubleArray, img2, 100, 100, JKQTPMathImageGRAY, plotImg->get_plotter());
    pimg2->set_imageMin(-1);
    pimg2->set_imageMax(0.5);
    pimg2->set_imageName("left image");
    pimg2->set_title("left image");
    pimg2->set_autoImageRange(false);
    pimg2->set_colorBarTopVisible(true);
    pimg2->set_colorBarRightVisible(false);
    plotImg->get_plotter()->addGraph(pimg2);
    pimg3=new JKQTPMathImage(20,0,10,10, JKQTPMathImageBase::DoubleArray, img2, 100, 100, JKQTPMathImageMATLAB, plotImg->get_plotter());
    pimg3->set_colorBarTopVisible(true);
    pimg3->set_colorBarRightVisible(true);
    pimg3->set_imageMax(0.5);
    pimg3->set_rangeMaxFailAction(JKQTPMathImageGivenColor);
    pimg3->set_rangeMaxFailColor(QColor("salmon"));
    pimg3->set_imageMin(-0.3);
    pimg3->set_rangeMinFailAction(JKQTPMathImageTransparent);
    pimg3->set_autoImageRange(false);
    pimg3->set_imageName("I(x,y)=\\sin\\left(\\frac{2\\pi\\cdot\\sqrt{1+x^2+y^2}}{20}\\right)");
    pimg3->set_dataModifier(img2M, JKQTPMathImageBase::Int32Array);
    pimg3->set_modifierMode(JKQTPMathImageBase::ModifyNone);
    pimg3->set_title("right image");
    plotImg->get_plotter()->addGraph(pimg3);

    QColor ty=QColor("blue");
    ty.setAlphaF(0.1);
    JKQTPOverlayImage* oimg1=new JKQTPOverlayImage(10,0,10,10, bimg2, 100, 100, QColor("blue"), plotImg->get_plotter());
    oimg1->set_title("overlay");
    oimg1->set_falseColor(ty);
    plotImg->get_plotter()->addGraph(oimg1);

    JKQTPOverlayImageEnhanced* oimg2=new JKQTPOverlayImageEnhanced(10,0,10,10, bimg2, 100, 100, QColor("red"), plotImg->get_plotter());
    oimg2->set_title("enhanced overlay");
    oimg2->set_drawAsRectangles(false);
    plotImg->get_plotter()->addGraph(oimg2);

    plotImg->set_doDrawing(true);
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
    pimg3->set_palette(index);
    plotImg->update_plot();
}

void TestWidgetImages::setImgPalette1(int index)
{
    pimg2->set_palette(index);
    plotImg->update_plot();
}

void TestWidgetImages::setImgModifier(int index)
{
    pimg3->set_modifierMode(JKQTPMathImage::ModifierMode(index));
    plotImg->update_plot();
}
