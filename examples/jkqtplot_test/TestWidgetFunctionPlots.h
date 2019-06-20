#ifndef TestWidgetFunctionPlots_H
#define TestWidgetFunctionPlots_H
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
#include "jkqtplotter/graphs/jkqtpgeometric.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/overlays/jkqtpbasicoverlays.h"
#include "jkqtplotter/graphs/jkqtpparsedfunction.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/gui/jkqtpgraphsmodel.h"
#include "jkqtplotter/gui/jkqtpcomboboxes.h"
#include "jkqtplotter/jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetFunctionPlots : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetFunctionPlots(QWidget *parent = 0);
        
    signals:
        
    public slots:


        void setPFuncStyle();
        void setPFuncMinPoint(int value);
        void setPFuncMaxRefine(int value);
        void setPFunc(const QString& expression);
        void setPFuncE(const QString& expression);

    protected:

        JKQTPXParsedFunctionLineGraph* pfunc;
        JKQTPlotter* plotFuncPlt;
        JKQTPErrorPlotstyleComboBox* pfuncErrorStyle;
        QCheckBox* chkPFuncDrawSamples;
        QCheckBox* chkPFuncDrawLine;

        
};


#endif // TestWidgetFunctionPlots_H
