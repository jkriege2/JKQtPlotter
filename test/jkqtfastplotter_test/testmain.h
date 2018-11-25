#ifndef TESTMAIN_H
#define TESTMAIN_H

#include <QTabWidget>
#include <QGridLayout>
#include "jkqtfastplotter/jkqtfastplotter.h"
#include <QComboBox>

#define N1 20
#define XMAX 10.0
#define IMAGE_N 300


class TestMain : public QWidget {
        Q_OBJECT
    public:
        explicit TestMain(QWidget *parent = 0);
        virtual ~TestMain();
        
    signals:
        
    public slots:
        void enableRed(bool enabled);
        void enableGreen(bool enabled);
        void enableBlue(bool enabled);

    protected:
        double* x;
        double* y1;
        double* y2;
        double* y3;
        QVector<double> xx;
        QVector<double> yy;
        QImage img;
        double* image;
        bool* ovrl;

        double* imageRed;
        double* imageGreen;
        double* imageBlue;
        JKQTFPRGBImageOverlayPlot* prgb;
};

#endif // TESTMAIN_H
