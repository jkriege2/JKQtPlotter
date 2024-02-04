#ifndef MULTITHREADED_THREAD_H
#define MULTITHREADED_THREAD_H

#include <QThread>
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/graphs/jkqtplines.h"
#include "jkqtcommon/jkqtpmathtools.h"
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QObject>
#include <QDir>
#include <QString>

class PlottingThread: public QThread {
    Q_OBJECT
public:
    inline PlottingThread(const QString& filenamepart, int plotindex, int NUM_GRAPHS, int NUM_DATAPOINTS, const QString& labeltemplate, QObject* parent):
        QThread(parent),
        m_plotindex(plotindex),
        m_runtimeNanoseconds(0),
        m_filename(),
        m_filenamepart(filenamepart),
        m_NUM_GRAPHS(NUM_GRAPHS),
        m_NUM_DATAPOINTS(NUM_DATAPOINTS),
        m_labeltemplate(labeltemplate)
    {}

    inline static QString plotAndSave(const QString& filenamepart, int plotIndex, int NUM_GRAPHS, int NUM_DATAPOINTS, const QString& labeltemplate, double* runtimeNanoseconds=nullptr) {
        QElapsedTimer timer;
        timer.start();
        const QString filename=QDir(QDir::tempPath()).absoluteFilePath(QString("testimg_%1_%2.png").arg(filenamepart).arg(plotIndex));
        JKQTBasePlotter plot(true);

        const size_t colX=plot.getDatastore()->addLinearColumn(NUM_DATAPOINTS, 0, 10, "x");
        QRandomGenerator rng;
        for (int i=0; i<NUM_GRAPHS; i++) {
            JKQTPXYLineGraph* g;
            plot.addGraph(g=new JKQTPXYLineGraph(&plot));
            g->setXColumn(colX);
            g->setYColumn(plot.getDatastore()->addCalculatedColumnFromColumn(colX, [&](double x) { return cos(x+double(i+plotIndex)/8.0*JKQTPSTATISTICS_PI)+rng.generateDouble()*0.2-0.1;}));
            g->setTitle(labeltemplate.arg(i+plotIndex+1));
            g->setDrawLine(true);
            g->setSymbolType(JKQTPNoSymbol);

        }
        plot.setPlotLabel(QString("Test Plot %1 (%2)").arg(plotIndex+1).arg(filenamepart));
        plot.getXAxis()->setAxisLabel("x-axis $x$");
        plot.getYAxis()->setAxisLabel("y-axis $f(x)$");
        plot.zoomToFit();
        plot.saveAsPixelImage(filename, false, "PNG");

        if (runtimeNanoseconds) *runtimeNanoseconds=timer.nsecsElapsed();
        return filename;
    }
    inline double getRuntimeNanosends() const {
        return m_runtimeNanoseconds;
    }
    inline QString getFilename() const {
        return m_filename;
    }
protected:
    inline virtual void run() {
        m_filename=plotAndSave(m_filenamepart, m_plotindex, m_NUM_GRAPHS, m_NUM_DATAPOINTS, m_labeltemplate, &m_runtimeNanoseconds);
    }

    int m_plotindex;
    double m_runtimeNanoseconds;
    QString m_filename;
    const QString m_filenamepart;
    const int m_NUM_GRAPHS;
    const int m_NUM_DATAPOINTS;
    const QString m_labeltemplate;
};

#endif // MULTITHREADED_THREAD_H
