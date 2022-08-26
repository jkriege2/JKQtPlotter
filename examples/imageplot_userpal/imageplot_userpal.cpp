/** \example imageplot_userpal.cpp
 * Shows how to plot colored math images/matrices with JKQTPlotter, using user-defined image palettes
 *
 * \ref JKQTPlotterImagePlot
 */

#include "jkqtpexampleapplication.h"
#include <QApplication>
#include <cmath>
#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/gui/jkqtpcomboboxes.h"
#include "jkqtcommon/jkqtpbasicimagetools.h"


int main(int argc, char* argv[])
{
        
    JKQTPAppSettingController highDPIController(argc, argv);
    JKQTPExampleApplication app(argc, argv);


    // 0. tell the library where to find additional palettes.
    //    this needs to be done BEFORE first using JKQTPlotter
    // 0.1 first we want to build a user-defined palette with five colors
    //     from a simple list of these colors:
    JKQTPImageTools::LUTType pal{
        QColor("blue").rgb(),
        QColor("green").rgb(),
        QColor("white").rgb(),
        QColor("yellow").rgb(),
        QColor("red").rgb(),
        QColor("red").rgb()
    };
    JKQTPImageTools::registerPalette("userpal_list", pal, QObject::tr("User Palette simple list"));

    // 0.2 first we want to build a user-defined palette with five colors
    //     the function JKQTPBuildColorPaletteLUT builds a full-sized palette
    //     with steps from the given 5 colors.
    //     Note that you need to double the last color in order to define its range's
    //     beginning and end
    QList<QPair<double, QRgb> > palsteps1;
    palsteps1<<qMakePair<double, QRgb>(0.0, QColor("blue").rgba());
    palsteps1<<qMakePair<double, QRgb>(0.2, QColor("green").rgba());
    palsteps1<<qMakePair<double, QRgb>(0.4, QColor("white").rgba());
    palsteps1<<qMakePair<double, QRgb>(0.6, QColor("yellow").rgba());
    palsteps1<<qMakePair<double, QRgb>(0.8, QColor("red").rgba());
    palsteps1<<qMakePair<double, QRgb>(1.0, QColor("red").rgba());
    JKQTPImageTools::registerPalette("userpal_1_steps", JKQTPBuildColorPaletteLUT(palsteps1), QObject::tr("User Palette 1, steps"));


    //     With the double value (first argument), you can determine where the next color
    //     band starts, with respect to the other colors. As an example, we make the central
    //     white band narrow, as well as the bands at the borders:
    QList<QPair<double, QRgb> > palsteps2;
    palsteps2<<qMakePair<double, QRgb>(0.00, QColor("blue").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.05, QColor("green").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.45, QColor("white").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.55, QColor("yellow").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.95, QColor("red").rgba());
    palsteps2<<qMakePair<double, QRgb>(1.00, QColor("red").rgba());
    int userpalette_id=JKQTPImageTools::registerPalette("userpal_2_steps", JKQTPBuildColorPaletteLUT(palsteps2), QObject::tr("User Palette 2, steps"));


    // 0.2 If we use JKQTPBuildColorPaletteLUTLinInterpolate() instead of JKQTPBuildColorPaletteLUT(),
    //     the palettes will be smooth (linearly interpolated between the single given colors:
    JKQTPImageTools::registerPalette("userpal_1_linear", JKQTPBuildColorPaletteLUTLinInterpolate(palsteps1), QObject::tr("User Palette 1, linear"));
    JKQTPImageTools::registerPalette("userpal_2_linear", JKQTPBuildColorPaletteLUTLinInterpolate(palsteps2), QObject::tr("User Palette 2, linear"));

    // 0.3 Now we load a whole set of additional palettes from an XML-file:
    JKQTPImageTools::registerPalettesFromFile(":/usercolorpalettes/palettes/All_idl_cmaps.xml");
    JKQTPImageTools::registerPalettesFromFile(":/usercolorpalettes/palettes/All_mpl_cmaps.xml");
    JKQTPImageTools::registerPalettesFromFile(":/usercolorpalettes/palettes/CoolWarmUChar33.csv");
    JKQTPImageTools::registerPalettesFromFile(":/usercolorpalettes/palettes/CoolWarmUChar257.csv");
    JKQTPImageTools::registerPalettesFromFile(":/usercolorpalettes/palettes/NSW_Discrete_Z_ColorMap.xml");


    // 1. create a window containing a plotter and a combobox to select the color palette
    //    ... and get a pointer to the internal datastore (for convenience)
    QWidget win;
    QVBoxLayout* lay=new QVBoxLayout();
    win.setLayout(lay);
    JKQTPMathImageColorPaletteComboBox* cmbPalette=new JKQTPMathImageColorPaletteComboBox(&win);
    lay->addWidget(cmbPalette);
    JKQTPlotter* plot=new JKQTPlotter(&win);
    lay->addWidget(plot);
    JKQTPDatastore* ds=plot->getDatastore();

    // 2. now we create data for the charts (taken from https://commons.wikimedia.org/wiki/File:Energiemix_Deutschland.svg)
    const int NX=100; // image dimension in x-direction [pixels]
    const int NY=NX; // image dimension in x-direction [pixels]
    const double dx=0.6e-2; // size of a pixel in x-direction [micrometers]
    const double dy=0.6e-2; // size of a pixel in x-direction [micrometers]

    // 2.1 Parameters for airy disk plot (see https://en.wikipedia.org/wiki/Airy_disk)
    double NA=1.1; // numerical aperture of lens
    double wavelength=488e-3; // wavelength of the light [micrometers]

    // 2.2 calculate image of airy disk in a row-major array and immediately store the values
    //     in a new image column cAiryDisk
    size_t cAiryDisk=ds->addCalculatedImageColumn(NX, NY, [&](size_t ix, size_t iy)->double {
        double x=static_cast<double>(static_cast<int>(ix)-NX/2)*dx;
        double y=static_cast<double>(static_cast<int>(iy)-NY/2)*dy;
        const double r=sqrt(x*x+y*y);
        const double v=2.0*JKQTPSTATISTICS_PI*NA*r/wavelength;
        if (ix==NX/2 && iy==NY/2) return 1.0;
        else return pow(2.0*jkqtp_j1(v)/v, 2);
    }, "imagedata");



	
    // 3. create a grapJKQTPSTATISTICS_PIKQTPColumnMathImage) with the column created above as data
    //    The data is color-coded with the color-palette JKQTPMathImageMATLAB
    //    the converted range of data is determined automatically because setAutoImageRange(true)
    JKQTPColumnMathImage* graph=new JKQTPColumnMathImage(plot);
    graph->setTitle("default MATLAB palette");
    // set the image column with the data
    graph->setImageColumn(cAiryDisk);
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->setX(0);
    graph->setY(0);
    // width and height of the image in plot-axis-coordinates
    graph->setWidth(1);
    graph->setHeight(1);
    // color-map is taken from cmbPalette
    plot->connect(cmbPalette, &JKQTPMathImageColorPaletteComboBox::currentPaletteChanged,[&](JKQTPMathImageColorPalette p) { graph->setColorPalette(p); plot->redrawPlot(); });
    graph->setColorPalette(static_cast<JKQTPMathImageColorPalette>(userpalette_id));
    cmbPalette->setCurrentColorPalette(graph->getColorPalette());
	
	
    // 4. add the graphs to the plot, so it is actually displayed
    plot->addGraph(graph);

	
    // 5. fix axis and plot aspect ratio to 1
    plot->getPlotter()->setMaintainAspectRatio(true);
    plot->getPlotter()->setMaintainAxisAspectRatio(true);

    // 6. autoscale the plot so the graph is contained
    plot->zoomToFit();


    // 7. Finally we add two buttons that save the current palette to a PNG-file and all loaded palettes:
    QPushButton* btnSavePal=new QPushButton(QObject::tr("save current palette"), &win);
    btnSavePal->connect(btnSavePal, &QPushButton::clicked, [&]() {
        auto img=JKQTPImageTools::GetPaletteImage(cmbPalette->currentColorPalette(), JKQTPImageTools::LUTSIZE, 16);
        img.save(JKQTPImageTools::JKQTPMathImageColorPalette2String(cmbPalette->currentColorPalette())+".png");
    });
    lay->addWidget(btnSavePal);
    QPushButton* btnSaveAllPal=new QPushButton(QObject::tr("save all palettes"), &win);
    btnSavePal->connect(btnSaveAllPal, &QPushButton::clicked, [&]() {
        for (auto pn: JKQTPImageTools::getPredefinedPalettes()) {
            auto p=JKQTPImageTools::String2JKQTPMathImageColorPalette(pn);
            auto img=JKQTPImageTools::GetPaletteImage(p, JKQTPImageTools::LUTSIZE, 16);
            img.save("palette_"+JKQTPImageTools::JKQTPMathImageColorPalette2String(p)+".png");
        }
    });
    lay->addWidget(btnSaveAllPal);



    // 8. show plotter and make it a decent size
    win.show();
    win.resize(500,550);
    win.setWindowTitle("JKQTPColumnMathImage, USer Palettes");


    return app.exec();
}
