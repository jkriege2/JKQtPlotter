#include "mandelbrotmainwindow.h"
#include "ui_mandelbrotmainwindow.h"
#include <thread>
#include <future>

MandelbrotMainWindow::MandelbrotMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MandelbrotMainWindow)
{
    ui->setupUi(this);

    // format graph:
    // 1. set the graph scales manually
    ui->plot->setXY(-2,1,-1,1);
    ui->plot->setAbsoluteXY(-5,5,-5,5);
    // 2. set the asxpect ratio to width/height
    ui->plot->getPlotter()->setMaintainAspectRatio(true);
    ui->plot->getPlotter()->setAspectRatio(static_cast<double>(ui->plot->width())/static_cast<double>(ui->plot->height()));
    // 3. disable grids
    ui->plot->getXAxis()->setDrawGrid(false);
    ui->plot->getYAxis()->setDrawGrid(false);

    JKQTPDatastore* ds=ui->plot->getDatastore();

    mandelbrot_col=ds->addImageColumn(300,200, "mandelbrot_image_calculate");
    mandelbrot_col_display=ds->copyColumn(mandelbrot_col, "mandelbrot_image_display");
    calculateMandelSet(ui->plot->getXMin(), ui->plot->getXMax(), ui->plot->getYMin(), ui->plot->getYMax(), 300, 200, ui->spinMaxIterations->value());
    ui->plot->getDatastore()->copyColumnData(mandelbrot_col_display, mandelbrot_col);

    // 4. create a graph (JKQTPColumnMathImage) with the column created above as data
    //    The data is color-coded with the color-palette JKQTPMathImageMATLAB
    //    the converted range of data is determined automatically because setAutoImageRange(true)
    graph=new JKQTPColumnMathImage(ui->plot);
    graph->setTitle("");
    // image column with the data
    graph->setImageColumn(mandelbrot_col_display);
    // image color range is calculated manually!
    graph->setAutoImageRange(false);
    graph->setImageMin(1);
    graph->setImageMax(ui->spinMaxIterations->value());
    // set image size
    graph->setX(ui->plot->getXMin());
    graph->setY(ui->plot->getYMin());
    graph->setWidth(ui->plot->getXMax()-ui->plot->getXMin());
    graph->setHeight(ui->plot->getYMax()-ui->plot->getYMin());
    // add graph to plot
    ui->plot->addGraph(graph);


    ui->cmbColorPalette->setCurrentColorPalette(graph->getColorPalette());

    connect(ui->action_Reset_View, &QAction::triggered, this, &MandelbrotMainWindow::resetView);
    connect(ui->cmbColorPalette, &JKQTPMathImageColorPaletteComboBox::currentPaletteChanged, this, &MandelbrotMainWindow::paletteChanged);
    connect(ui->spinMaxIterations, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MandelbrotMainWindow::maxIterationsChanged);
    connect(ui->chkLogScaling, &QCheckBox::toggled, this, &MandelbrotMainWindow::logScalingChanged);
    connect(ui->plot, &JKQTPlotter::zoomChangedLocally, this, &MandelbrotMainWindow::plotZoomChangedLocally);
    connect(ui->plot, &JKQTPlotter::widgetResized, this, &MandelbrotMainWindow::plotResized);
}

MandelbrotMainWindow::~MandelbrotMainWindow()
{
    delete ui;
}

void MandelbrotMainWindow::paletteChanged(JKQTPMathImageColorPalette pal)
{
    graph->setColorPalette(pal);
    ui->plot->redrawPlot();
}

void MandelbrotMainWindow::maxIterationsChanged(int/* maxIter*/)
{
    graph->setAutoImageRange(false);
    graph->setImageMin(1);
    if (ui->chkLogScaling->isChecked()) {
        graph->setImageMax(log(ui->spinMaxIterations->value())/log(10.0));
    } else {
        graph->setImageMax(ui->spinMaxIterations->value());
    }
    plotZoomChangedLocally(ui->plot->getXMin(), ui->plot->getXMax(), ui->plot->getYMin(), ui->plot->getYMax(), ui->plot);
}

void MandelbrotMainWindow::logScalingChanged(bool en)
{
    if (en) {
        std::transform(ui->plot->getDatastore()->begin(mandelbrot_col), ui->plot->getDatastore()->end(mandelbrot_col), ui->plot->getDatastore()->begin(mandelbrot_col_display), [](double v)->double { return log(v)/log(10.0); });
    } else {
        std::copy(ui->plot->getDatastore()->begin(mandelbrot_col), ui->plot->getDatastore()->end(mandelbrot_col), ui->plot->getDatastore()->begin(mandelbrot_col_display));
    }
    graph->setAutoImageRange(false);
    graph->setImageMin(1);
    if (ui->chkLogScaling->isChecked()) {
        graph->setImageMax(log(ui->spinMaxIterations->value())/log(10.0));
    } else {
        graph->setImageMax(ui->spinMaxIterations->value());
    }
    ui->plot->redrawPlot();
}

void MandelbrotMainWindow::plotResized(int /*new_width*/, int /*new_height*/, JKQTPlotter* /*sender*/)
{
    ui->plot->getPlotter()->setAspectRatio(static_cast<double>(ui->plot->width())/static_cast<double>(ui->plot->height()));
    ui->plot->getPlotter()->setMaintainAspectRatio(true);
    qDebug()<<"plotResized: aspect="<<static_cast<double>(ui->plot->width())/static_cast<double>(ui->plot->height());
    plotZoomChangedLocally(ui->plot->getXMin(), ui->plot->getXMax(), ui->plot->getYMin(), ui->plot->getYMax(), ui->plot);
}

void MandelbrotMainWindow::resetView()
{
    ui->plot->setXY(-2,1,-1,1);
    ui->plot->redrawPlot();
}

void MandelbrotMainWindow::plotZoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* /*sender*/)
{
    calculateMandelSet(newxmin, newxmax, newymin, newymax, ui->plot->getXAxis()->getParentPlotWidth(), ui->plot->getYAxis()->getParentPlotWidth(), ui->spinMaxIterations->value());
    ui->plot->getDatastore()->copyColumnData(mandelbrot_col_display, mandelbrot_col);
    if (ui->chkLogScaling->isChecked()) {
        std::transform(ui->plot->getDatastore()->begin(mandelbrot_col), ui->plot->getDatastore()->end(mandelbrot_col), ui->plot->getDatastore()->begin(mandelbrot_col), [](double v)->double { return log(v)/log(10.0); });
    }
    graph->setX(newxmin);
    graph->setY(newymin);
    graph->setWidth(newxmax-newxmin);
    graph->setHeight(newymax-newymin);
    // this call ensures correctly set NX and NY
    graph->setImageColumn(mandelbrot_col_display);
    ui->plot->redrawPlot();
    emit readyForScreenshot();
}


void MandelbrotMainWindow::calculateMandelSet(double rmin, double rmax, double imin, double imax, size_t width, size_t height, unsigned int max_iterations) {
    QElapsedTimer timer;
    timer.start();

    auto ds=ui->plot->getDatastore();

    // ensure the image column has the correct size
    ds->resizeImageColumn(mandelbrot_col, width, height);
    qDebug()<<"calculating for "<<width<<"x"<<height<<"pixels: real="<<rmin<<"..."<<rmax<<", imaginary="<<imin<<"..."<<imax;


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // iterate over all pixels, parallelized version

    // calculate the block size for parallel processing
    const size_t blocksize=std::max<size_t>(100,width*height/std::max<size_t>(2, std::thread::hardware_concurrency()-1));

    std::vector<std::thread> threads;
    for (size_t offset=0; offset<width*height; offset+=blocksize) {
        threads.push_back(std::thread([=](){
            // start iterating at begin+offset
            auto pix=ds->begin(mandelbrot_col)+static_cast<int>(offset);
            // stop iterating at begin+offset+blocksize, or at the end
            const auto pix_end=pix+static_cast<int>(blocksize);
            for (; pix!=pix_end; ++pix) {
                // calculate the pixels coordinate in the imaginary plane
                const double r0=static_cast<double>(pix.getImagePositionX())/static_cast<double>(width)*(rmax-rmin)+rmin;
                const double i0=static_cast<double>(pix.getImagePositionY())/static_cast<double>(height)*(imax-imin)+imin;
                //qDebug()<<pix.getImagePositionX()<<","<<pix.getImagePositionY()<<": "<<r0<<","<<i0;

                unsigned int iteration=0;
                double ri=0;
                double ii=0;
                // check for Mandelbrot series divergence at r0, i0, i.e. calculate
                // the series [r(i),i(i)]=fmanelbrot(r(i-1),i(i-1) | r0,i0) for every point in the plane [r0,i0]
                // starting from r(0)=i(0)=0. The number of iterations until |r(i),i(i)|>=2 gives the color of
                // the point.
                while(ri*ri+ii*ii<=2.0*2.0 && iteration<max_iterations) {
                    const double tmp=ri*ri-ii*ii+r0;
                    ii=2.0*ri*ii+i0;
                    ri=tmp;
                    iteration++;
                }
                *pix=iteration;
            }
        }));
    }
    qDebug()<<"   using "<<threads.size()<<" threads with blocksize="<<blocksize;
    for (auto& thread:threads) thread.join();
    threads.clear();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ALTERNATIVE: iterate over all pixels, serial code
    /*
    for (auto pix=ds->begin(mandelbrot_col); pix!= ds->end(mandelbrot_col); ++pix) {
        // calculate the pixels coordinate in the imaginary plane
        const double r0=static_cast<double>(pix.getImagePositionX())/static_cast<double>(width)*(rmax-rmin)+rmin;
        const double i0=static_cast<double>(pix.getImagePositionY())/static_cast<double>(height)*(imax-imin)+imin;
        //qDebug()<<pix.getImagePositionX()<<","<<pix.getImagePositionY()<<": "<<r0<<","<<i0;

        unsigned int iteration=0;
        double ri=0;
        double ii=0;
        // check for Mandelbrot series divergence at r0, i0, i.e. calculate
        // the series [r(i),i(i)]=fmanelbrot(r(i-1),i(i-1) | r0,i0) for every point in the plane [r0,i0]
        // starting from r(0)=i(0)=0. The number of iterations until |r(i),i(i)|>=2 gives the color of
        // the point.
        while(ri*ri+ii*ii<=2.0*2.0 && iteration<max_iterations) {
            const double tmp=ri*ri-ii*ii+r0;
            ii=2.0*ri*ii+i0;
            ri=tmp;
            iteration++;
        }
        *pix=iteration;
    }
    */

    qDebug()<<"finished calculating after "<<static_cast<double>(timer.nsecsElapsed())/1000000.0<<"ms";
}
