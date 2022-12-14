#ifndef TestWidgetContourPlots_H
#define TestWidgetContourPlots_H
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
#include "jkqtplotter/graphs/jkqtpcontour.h"
#include "jkqtplotter/graphs/jkqtpparsedfunction.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/gui/jkqtpgraphsmodel.h"
#include "jkqtplotter/jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetContourPlots : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetContourPlots(QWidget *parent = 0);
        
    Q_SIGNALS:
        
    public Q_SLOTS:


    protected:


        
};


#endif // TestWidgetContourPlots_H
