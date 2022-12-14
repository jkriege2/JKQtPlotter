#ifndef TestWidgetPeaksPlots_H
#define TestWidgetPeaksPlots_H
#include <QElapsedTimer>
#include <QCheckBox>
#include <QLineEdit>
#include <QTimer>
#include <QImage>
#include <QVBoxLayout>
#include <QWidget>
#include <QTabWidget>
#include <QFormLayout>
#include <QCheckBox>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtppeakstream.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetPeaksPlots : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetPeaksPlots(QWidget *parent = nullptr);
        
    Q_SIGNALS:
        
    public Q_SLOTS:
        void setDrawBaseline(bool checked);
        void setYPeaks(bool checked);

    protected:

        JKQTPlotter* plotPeaks;
        JKQTPPeakStreamGraph* graphPeakStream1;
        JKQTPPeakStreamGraph* graphPeakStream2;

        
};


#endif // TestWidgetPeaksPlots_H
