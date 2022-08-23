#pragma once
#include <array>
#include <random>
#include <chrono>
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
        QMenu* menuSizes;
        JKQTPXYLineGraph* graph;
        JKQTPXYLineGraph* graph2;
    public:
        SpeedTestPlot();

        virtual ~SpeedTestPlot();
    protected slots:
        void plotNewData();
        void updateDataSize(size_t newSize);

};
