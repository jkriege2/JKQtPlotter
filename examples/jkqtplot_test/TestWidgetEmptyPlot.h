#ifndef TestWidgetEmptyPlot_H
#define TestWidgetEmptyPlot_H
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
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetEmptyPlot : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetEmptyPlot(QWidget *parent = 0);
        
    Q_SIGNALS:
        
    public Q_SLOTS:


    protected:

        
};


#endif // TestWidgetEmptyPlot_H
