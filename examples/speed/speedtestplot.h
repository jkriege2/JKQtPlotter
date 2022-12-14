#pragma once
#include <array>
#include <random>
#include <chrono>
#include <QList>
#include <QMenu>
#include <QActionGroup>
#include "jkqtplotter/jkqtplotter.h"


class JKQTPXYLineGraph; // forward

class SpeedTestPlot: public JKQTPlotter {
        Q_OBJECT
    protected:
        std::array<double, 10000> X, Y, Y2;
        double dx;
        double x0;
        size_t NDATA;
        std::chrono::system_clock::time_point t_lastplot;
        QAction* actAntiAliase;
        QAction* actTwoGraphs;
        QAction* actFixedXAxis;
        QAction* actLines;
        QAction* actSymbols;
        QAction* actUseNonvisibleLineCompression;
        QMenu* menuUseNonvisibleLineCompressionAgressiveness;
        QAction* actUseClipping;
        QAction* actAnimation;
        QAction* actStepAnimation;
        QMenu* menuSizes;
        JKQTPXYLineGraph* graph;
        JKQTPXYLineGraph* graph2;
        QList<double> calctimes;
    public:
        SpeedTestPlot();

        virtual ~SpeedTestPlot();
    protected Q_SLOTS:
        void plotNewData();
        void updateDataSize(size_t newSize, bool updatePlots=true);
        static double addOutlier(double prob, double height);

};
