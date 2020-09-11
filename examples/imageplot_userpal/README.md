# Example (JKQTPlotter): Image Plots with Custom Palettes {#JKQTPlotterImagePlotUserPalette}

This project (see `./examples/imageplot_userpal/`) demonstrates how to use user-defined color-palettes for image plots.
The source code of the main application is (see [`imageplot_userpal.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/imageplot_userpal/imageplot_userpal.cpp)).

# Build Palettes Programmatically 

JKQTPlotter comes with a large set of predefined color palettes, which are enumerated in `JKQTPMathImageColorPalette`.
In addition you can build your own palettes as simple lookup-tables of type `JKQTPImageTools::LUTType` (which is a `QVector<QRgb>`) and register them in the system (using `JKQTPImageTools::registerPalette()`). There are several options available for building such a palette:
1. you can simply define a palette from single colors:
```.cpp
    JKQTPImageTools::LUTType pal{
        QColor("blue").rgb(), 
        QColor("green").rgb(), 
        QColor("white").rgb(), 
        QColor("yellow").rgb(), 
        QColor("red").rgb(), 
        QColor("red").rgb()
    };
```
    Such a palette will have exactly as many entries, as you specified:  ![palsimple](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/imageplot_userpal_palsimple.png)
2. Alternatively you can build a palette from a set of colors with assocziated positions on the values axis (typically 0..1 as these are in any way later mapped to the actual data range):
```.cpp
    QList<QPair<double, QRgb> > palsteps2;
    palsteps2<<qMakePair<double, QRgb>(0.00, QColor("blue").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.05, QColor("green").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.45, QColor("white").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.55, QColor("yellow").rgba());
    palsteps2<<qMakePair<double, QRgb>(0.95, QColor("red").rgba());
    palsteps2<<qMakePair<double, QRgb>(1.00, QColor("red").rgba());
    
    pal=JKQTPBuildColorPaletteLUT(palsteps2, JKQTPImageTools::LUTSIZE);
```
    Such a palette will have `JKQTPImageTools::LUTSIZE (=255)` entries  and the colors are not spaced equally: ![palsteps2](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/imageplot_userpal_palsteps2.png)
3. The palettes so far had steps, but you can also give a series of nodes with positions (on the value axis) and RGB-colors there (e.g. `palsteps2` above) but then linearly interpolate between these by calling:
```.cpp
    pal=JKQTPBuildColorPaletteLUTLinInterpolate(palsteps2, JKQTPImageTools::LUTSIZE);
```
    The resulting LUT is then:   ![imageplot_userpal_2_linear](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/imageplot_userpal_2_linear.png)
4. Finally there is a second way of linear interpolation, where linear segments are given for the three color channels separately. To use such a definition, call `JKQTPBuildColorPaletteLUTLinSegments` instead (see documenttaion there).

For each of these options, you finally call `JKQTPImageTools::registerPalette()` to make them known to the system:
```.cpp
    int userpalette_id=JKQTPImageTools::registerPalette("userpal_computer_readable_name", pal, QObject::tr("User Palette Human-Readable Name"));
```
This function returns an integer, which you can cast to a `JKQTPMathImageColorPalette` to use your new palette:
```.cpp
    JKQTPColumnMathImage* graph=new JKQTPColumnMathImage(plot);
    // ...
    graph->setColorPalette(static_cast<JKQTPMathImageColorPalette>(userpalette_id));
```

# Load Palettes from Files

In addition to building palettes/LUTs programmatically, as shown above, you can simply load palettes from files using:
```.cpp
    JKQTPImageTools::registerPalettesFromFile(":/usercolorpalettes/palettes/All_idl_cmaps.xml");
```

This function may load different file formats (discriminated by the extension):
1. XML-files (extension `.xml`) may contain one or more color palettes and should be formatted as follows:
```.xml
    <ColorMap name="PALETTENAME" space="RGB">
      <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
      <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
      ...
    </ColorMap>
```
   or with several palettes in one file:
```.xml
    <ColorMaps>
        <ColorMap name="PALETTENAME" space="RGB">
          <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
          <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
          ...
        </ColorMap>
        <ColorMap name="PALETTENAME" space="RGB">
          <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
          <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
          ...
        </ColorMap>
        ...
    </ColorMaps>
```

2. CSV-files (extensions `.csv`, `.rgb`, `.pal`) are simply a list of 3 or 4 comma-separated values:
```
    red, green, blue
    red, green, blue
    ...
```
    or:
```
    scalar, red, green, blue
    scalar, red, green, blue
    ...
```
    
By default the palettes are simply read from the files as raw data. Alternatively you can linearly interpolate between the nodes in the file by calling
```.cpp
    JKQTPImageTools::registerPalettesFromFile(":/usercolorpalettes/palettes/All_idl_cmaps.xml", true);
```

Examples for such palette files can be found here: [/examples/imageplot_userpal/palettes/](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/imageplot_userpal/palettes/)

# Main Program of the Example (GUI)

The rest of the example program [`imageplot_userpal.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/imageplot_userpal/imageplot_userpal.cpp) just generates a 2D function plot as a color-map and displays it ...
```.cpp
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
        const double v=2.0*M_PI*NA*r/wavelength;
        if (ix==NX/2 && iy==NY/2) return 1.0;
        else return pow(2.0*j1(v)/v, 2);
    }, "imagedata");



	
    // 3. create a graph (JKQTPColumnMathImage) with the column created above as data
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
    cmbPalette->setCurrentColorPalette(graph->getPalette());
	
	
    // 4. add the graphs to the plot, so it is actually displayed
    plot->addGraph(graph);

	
    // 5. fix axis and plot aspect ratio to 1
    plot->getPlotter()->setMaintainAspectRatio(true);
    plot->getPlotter()->setMaintainAxisAspectRatio(true);

    // 6. autoscale the plot so the graph is contained
    plot->zoomToFit();
    
    
    // 8. show plotter and make it a decent size
    win.show();
    win.resize(500,550);
    win.setWindowTitle("JKQTPColumnMathImage, User Palettes");
```
...along with a `JKQTPMathImageColorPaletteComboBox` to select a colormap and redraw the plot:
```.cpp
    JKQTPMathImageColorPaletteComboBox* cmbPalette=new JKQTPMathImageColorPaletteComboBox(&win);
    // ... 
    plot->connect(cmbPalette, &JKQTPMathImageColorPaletteComboBox::currentPaletteChanged,[&](JKQTPMathImageColorPalette p) { graph->setColorPalette(p); plot->redrawPlot(); });
    cmbPalette->setCurrentColorPalette(graph->getPalette());
```

It also adds two `QPushButton`s that allow to save the current or all registered color-palettes to small PNG-Files:
```.cpp
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
```

The whole program looks like this:

![imageplot_userpal_program](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/imageplot_userpal_program.png)

