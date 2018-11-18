#ifndef TestWidgetBarcharts_H
#define TestWidgetBarcharts_H
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




class TestWidgetBarcharts : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetBarcharts(QWidget *parent = 0);
        
    signals:
        
    public slots:


        void setBarchartLogLog(bool checked);
        void setBarchartAngele(int angel);
        void setBarchartAngele2(int angel);

    protected:


        JKQtPlotter* plotBarchart;
        JKQtPlotter* plotBarchart2;
        QCheckBox* chkBarLog;
        QSpinBox* spinBarLabelAngel;
        QSpinBox* spinBarLabelAngel2;



        
};


#endif // TestWidgetBarcharts_H
