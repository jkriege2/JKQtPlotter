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
#include "../../jkqtplotter.h"
#include "../../jkqtpgeoelements.h"
#include "../../jkqtpimageelements.h"
#include "../../jkqtpoverlayelements.h"
#include "../../jkqtpparsedfunctionelements.h"
#include "../../jkqtpbaseelements.h"
#include "../../jkqtpplotsmodel.h"
#include "../../jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetPeaksPlots : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetPeaksPlots(QWidget *parent = 0);
        
    signals:
        
    public slots:
        void setDrawBaseline(bool checked);
        void setYPeaks(bool checked);

    protected:

        JKQtPlotter* plotPeaks;
        JKQTPPeakStreamGraph* graphPeakStream1;
        JKQTPPeakStreamGraph* graphPeakStream2;

        
};


#endif // TestWidgetPeaksPlots_H
