#ifndef CONTOURPLOTANIMATOR_H
#define CONTOURPLOTANIMATOR_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <cmath>
#include "jkqtplotter/jkqtplotter.h"


class ContourPlotAnimator: public QObject {
        Q_OBJECT
    public:
        ContourPlotAnimator(JKQTPDatastore* ds_, JKQTPlotter* pplot_, int NX_, int NY_,double w_, double h_, double dx_, size_t cPotential_);

        void start(int delayMS);
    protected slots:
        void step();
    protected:
        double angle;
        JKQTPDatastore* ds;
        JKQTPlotter* pplot;
        int NX;
        int NY;
        double w;
        double h;
        double dx;
        size_t cPotential;
        QElapsedTimer timerFR;
};

#endif // CONTOURPLOTANIMATOR_H
