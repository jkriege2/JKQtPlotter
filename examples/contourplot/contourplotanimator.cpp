#include "contourplotanimator.h"


ContourPlotAnimator::ContourPlotAnimator(JKQTPDatastore *ds_, JKQTPlotter *pplot_, int NX_, int NY_, double w_, double h_, double dx_, size_t cPotential_):
    QObject(pplot_), angle(0), ds(ds_), pplot(pplot_), NX(NX_), NY(NY_), w(w_), h(h_), dx(dx_), cPotential(cPotential_)
{

}

void ContourPlotAnimator::start(int delayMS) {
    timerFR.start();
    QTimer::singleShot(delayMS, this,&ContourPlotAnimator::step);
}

void ContourPlotAnimator::step() {
    double x;
    double y=-h/2.0;
    const double eps0=8.854187e-12;
    double r=1e-6*(5.0+cos(angle/4.0))/6.0;
    double r2=1e-6*(5.0+sin(angle/4.0))/6.0;
    const double Q1=1.6e-19;           // charge of charged particle 1
    const double Q1_x0=-r*cos(angle);        // x-position of charged particle 1
    const double Q1_y0=-r*sin(angle);        // y-position of charged particle 1
    const double Q2=1.6e-19;           // charge of charged particle 2
    const double Q2_x0=r*cos(angle);         // x-position of charged particle 2
    const double Q2_y0=r*sin(angle);         // y-position of charged particle 2
    const double Q3=-1.6e-19;          // charge of charged particle 3
    const double Q3_x0=-r2*cos(angle);        // x-position of charged particle 3
    const double Q3_y0=r2*sin(angle);         // y-position of charged particle 3
    const double Q4=-1.6e-19;          // charge of charged particle 4
    const double Q4_x0=r2*cos(angle);         // x-position of charged particle 4
    const double Q4_y0=-r2*sin(angle);        // y-position of charged particle 4
    for (size_t iy=0; iy<static_cast<size_t>(NY); iy++ ) {
        x=-w/2.0;
        for (size_t ix=0; ix<static_cast<size_t>(NX); ix++ ) {
            const double r1=sqrt((x-Q1_x0)*(x-Q1_x0)+(y-Q1_y0)*(y-Q1_y0));
            const double r2=sqrt((x-Q2_x0)*(x-Q2_x0)+(y-Q2_y0)*(y-Q2_y0));
            const double r3=sqrt((x-Q3_x0)*(x-Q3_x0)+(y-Q3_y0)*(y-Q3_y0));
            const double r4=sqrt((x-Q4_x0)*(x-Q4_x0)+(y-Q4_y0)*(y-Q4_y0));
            ds->setPixel(cPotential, ix, iy, Q1/(4.0*JKQTPSTATISTICS_PI*eps0)/r1+Q2/(4.0*JKQTPSTATISTICS_PI*eps0)/r2+Q3/(4.0*JKQTPSTATISTICS_PI*eps0)/r3+Q4/(4.0*JKQTPSTATISTICS_PI*eps0)/r4);
            x+=dx;
        }
        y+=dx;
    }
    pplot->redrawPlot();
    qDebug()<<"animation step: angle="<<angle<<", framerate="<<1.0/(timerFR.elapsed()/1000.0)<<"fps <==> "<<timerFR.elapsed()<<"ms/frame";
    pplot->setWindowTitle("JKQTPColumnContourPlot ["+QString::number(1.0/(timerFR.elapsed()/1000.0))+"fps]");
    angle+=(2.0/180.0*JKQTPSTATISTICS_PI);
    timerFR.start();
    QTimer::singleShot(1, this,SLOT(step()));
}
