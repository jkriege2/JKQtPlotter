#ifndef TestWidgetParamScatterPlots_H
#define TestWidgetParamScatterPlots_H
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
#include "jkqtplotter/jkqtpgraphsgeometric.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpelementsoverlay.h"
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplottergui/jkqtpgraphsmodel.h"
#include "jkqtcommon/jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetParamScatterPlots : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetParamScatterPlots(QWidget *parent = 0);
        
    signals:
        
    public slots:

        void setPSPSymmErrors(bool checked);

    protected:

        JKQTPlotter* plotPSP;
        JKQTPXYParametrizedErrorScatterGraph* pltePSPCol;
        JKQTPXYParametrizedScatterGraph* pltePSPSize;
        JKQTPXYParametrizedScatterGraph* pltePSPSymb;


        
};


#endif // TestWidgetParamScatterPlots_H
