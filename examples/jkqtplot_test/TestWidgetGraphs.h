#ifndef TestWidgetGraphs_H
#define TestWidgetGraphs_H
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
#include "jkqtplottertools/jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetGraphs : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetGraphs(QWidget *parent = 0);
        
    signals:
        
    public slots:
        void moveovl();

        void setErrorLineStyle(int index);
        void setESSymbol(int index);


        void setSortOrder2(int index);

        void setKeyLayout2(JKQTPKeyLayout layout);
        void setKeyPos2(JKQTPKeyPosition layout);

    protected:
        double x1[N1], x2[N2], x2r[N2]/*, x3[N3]*/;
        double y11[N1], y12[N1], y13[N1], y21[N2], y21r[N2], y21re[N2], y22[N2], y23[N2], y24[N2];
        double xbox[N3], ymed[N3], ymean[N3], ymin[N3], ymax[N3], yp25[N3], yp75[N3];
        double b1[N3], b2[N3], b3[N3];


        JKQTPOverlayVerticalLine* ovl1;
        JKQTPlotter* plot;
        JKQTPlotter* plotLOG;
        JKQTPlotter* plotBot;
        JKQTPlotter* plotBot2;

        JKQTPPlotElement* plteErrors;
        JKQTPPlotElement* plteSymbols;
        JKQTPXYLineErrorGraph* pltePlot2;


        int ovlCnt;
        QElapsedTimer ovlTime;

        
};


#endif // TestWidgetGraphs_H
