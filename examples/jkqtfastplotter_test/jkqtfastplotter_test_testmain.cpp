/** \example jkqtfastplotter_test_testmain.cpp
 * Example of how to use JKQTFastPlotter
 * \see \ref JKQTFastPlotterTest
 */

#include "jkqtfastplotter_test_testmain.h"
#include <QCheckBox>
#include <QtGui>

TestMain::TestMain(QWidget *parent) :
    QWidget(parent)
{


    imageRed=(double*)calloc(IMAGE_N*IMAGE_N, sizeof(double));
    imageGreen=(double*)calloc(IMAGE_N*IMAGE_N, sizeof(double));
    imageBlue=(double*)calloc(IMAGE_N*IMAGE_N, sizeof(double));
    image=(double*)calloc(IMAGE_N*IMAGE_N, sizeof(double));
    ovrl=(bool*)calloc(IMAGE_N*IMAGE_N, sizeof(bool));
    x=(double*)calloc(N1, sizeof(double));
    y1=(double*)calloc(N1, sizeof(double));
    y2=(double*)calloc(N1, sizeof(double));
    y3=(double*)calloc(N1, sizeof(double));


    QGridLayout* gl=new QGridLayout();
    setLayout(gl);
    QTabWidget* t=new QTabWidget(this);
    gl->addWidget(t,0,0);

    QWidget* w=new QWidget(this);
    gl=new QGridLayout(w);
    w->setLayout(gl);

    JKQTFastPlotter* pl1=new JKQTFastPlotter(w);
    pl1->setMaintainAspectRatio(true);
    gl->addWidget(pl1, 0, 0);
    JKQTFastPlotter* pl2=new JKQTFastPlotter(w);
    pl2->setMaximumWidth(100);
    pl2->setSynchronizeY(pl1);
    pl2->setYAxisLabelVisible(false);
    pl2->setPlotBorderLeft(10);
    pl2->setPlotBorderRight(2);
    w->connect(pl1, SIGNAL(replotting()), pl2, SLOT(redrawPlot()));
    gl->addWidget(pl2, 0, 1);
    JKQTFastPlotter* pl3=new JKQTFastPlotter(w);
    pl3->setSynchronizeX(pl1);
    w->connect(pl1, SIGNAL(replotting()), pl3, SLOT(redrawPlot()));
    gl->addWidget(pl3, 1, 0);


    xx.clear();
    yy.clear();
    for (int i=0; i<N1; i++) {
        x[i]=(i+1)*XMAX/(double)N1;
        xx.push_back(x[i]);
        yy.push_back(sin(0.5*JKQTPSTATISTICS_PI*x[i])+2.0);
        std::cout<<xx[i]<<", "<<yy[i]<<std::endl;
        y1[i]=i*XMAX/(double)N1;
        y2[i]=log(x[i]);
        y3[i]=log10(x[i]);
    }
    JKQTFPVBarPlot* p1=new JKQTFPVBarPlot(pl1, N1, x, y1);
    JKQTFPLinePlot* p2=new JKQTFPLinePlot(pl1, N1, x, y2, QColor("blue"));
    JKQTFPLinePlot* p3=new JKQTFPLinePlot(pl1, N1, x, y3, QColor("darkgreen"));
    JKQTFPLinePlot* pv=new JKQTFPLinePlot(pl1, &xx, &yy, QColor("black"), Qt::SolidLine, 3);

    img=QImage(":/lena.png");
    JKQTFPQImagePlot* p4=new JKQTFPQImagePlot(pl2, &img, 0, 10, 0, 10);

    for (int x=0; x<IMAGE_N; x++) {
        for (int y=0; y<IMAGE_N; y++) {
            image[y*IMAGE_N+x]=IMAGE_N*(1.1+sin(sqrt((x-50)*(x-50)+(y-50)*(y-50))));
            if (x>50 && y>50) image[y*IMAGE_N+x]=0;
            if (x<50 && y<50) image[y*IMAGE_N+x]=50;
            if (abs(x-(IMAGE_N-y))<4) ovrl[y*IMAGE_N+x]=true; else  ovrl[y*IMAGE_N+x]=false;
        }
    }

    JKQTFPimagePlot* p5=new JKQTFPimagePlot(pl3, image, JKQTFP_double, IMAGE_N, IMAGE_N, 0, 10, 0, 10, JKQTFP_GRAY);
    QColor col=QColor("red");
    col.setAlpha(127);
    JKQTFPimageOverlayPlot* p5o=new JKQTFPimageOverlayPlot(pl3, ovrl, IMAGE_N, IMAGE_N, 0, 10, 0, 10, col);
    JKQTFPXRangePlot* p6=new JKQTFPXRangePlot(pl1, 2.25, 7.75);
    p6->setFillStyle(Qt::SolidPattern);
    JKQTFPQScaleBarXPlot* sb=new JKQTFPQScaleBarXPlot(pl1, 1, QString("%1 mm"));

    pl1->addPlot(p6);
    pl1->addPlot(p1);
    pl1->addPlot(p2);
    pl1->addPlot(p3);
    pl1->addPlot(pv);
    pl1->addPlot(sb);

    pl2->addPlot(p4);

    pl3->addPlot(p5);
    pl3->addPlot(p5o);
    pl3->setObjectName("pl3");

    QComboBox* spin=new QComboBox(w);
    spin->addItems(JKQTFPimagePlot_getPalettes());
    gl->addWidget(spin, 2,0);
    connect(spin, SIGNAL(currentIndexChanged(int)), p5, SLOT(setPalette(int)));
    QComboBox* scale=new QComboBox(w);
    scale->addItem("TopLeft");
    scale->addItem("TopRight");
    scale->addItem("BottomLeft");
    scale->addItem("BottomRight");
    gl->addWidget(scale, 3,0);
    connect(scale, SIGNAL(currentIndexChanged(int)), sb, SLOT(setPosition(int)));

    t->addTab(w, tr("Basic Test"));


    w=new QWidget(this);
    gl=new QGridLayout(w);
    w->setLayout(gl);
    JKQTFastPlotter* p21=new JKQTFastPlotter(w);
    gl->addWidget(p21, 0, 0, 1, 3);
    for (int x=0; x<IMAGE_N; x++) {
        for (int y=0; y<IMAGE_N; y++) {
            imageRed[y*IMAGE_N+x]=(fabs(x)+fabs(y))/2.0;
            imageGreen[y*IMAGE_N+x]=(fabs(double(IMAGE_N)-x)+fabs(y))/2.0;
            imageBlue[y*IMAGE_N+x]=sqrt(x*x+y*y);
        }
    }

    prgb=new JKQTFPRGBImageOverlayPlot(p21);
    prgb->setImage(imageRed, JKQTFP_double, imageGreen, JKQTFP_double, imageBlue, JKQTFP_double, IMAGE_N, IMAGE_N,0,10,0,10);
    p21->addPlot(prgb);

    QCheckBox* c=new QCheckBox(tr("red channel"), w);
    c->setChecked(true);
    connect(c, SIGNAL(toggled(bool)), this, SLOT(enableRed(bool)));
    gl->addWidget(c, 1,0);

    c=new QCheckBox(tr("green channel"), w);
    c->setChecked(true);
    connect(c, SIGNAL(toggled(bool)), this, SLOT(enableGreen(bool)));
    gl->addWidget(c, 1,1);

    c=new QCheckBox(tr("blue channel"), w);
    c->setChecked(true);
    connect(c, SIGNAL(toggled(bool)), this, SLOT(enableBlue(bool)));
    gl->addWidget(c, 1,2);


    t->addTab(w, tr("Overlay Test"));
    t->setCurrentIndex(1);

    resize(500,400);

}

TestMain::~TestMain() {
    free(imageRed);
    free(imageGreen);
    free(imageBlue);
    free(image);
    free(ovrl);
    free(x);
    free(y1);
    free(y2);
    free(y3);

}

void TestMain::enableRed(bool enabled) {
    if (enabled) {
        prgb->setImageRed(imageRed, JKQTFP_double);
    } else {
        prgb->setImageRed(NULL, JKQTFP_double);
    }
}

void TestMain::enableGreen(bool enabled) {
    if (enabled) {
        prgb->setImageGreen(imageGreen, JKQTFP_double);
    } else {
        prgb->setImageGreen(NULL, JKQTFP_double);
    }
}

void TestMain::enableBlue(bool enabled) {
    if (enabled) {
        prgb->setImageBlue(imageBlue, JKQTFP_double);
    } else {
        prgb->setImageBlue(NULL, JKQTFP_double);
    }
}
