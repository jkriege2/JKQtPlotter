#ifndef TestWidgetGeometry_H
#define TestWidgetGeometry_H
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
#include "jkqtplotter/gui/jkqtpgraphsmodel.h"
#include "jkqtplotter/jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetGeometry : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetGeometry(QWidget *parent = 0);
        
    signals:
        
    public slots:


    protected:


        
};


#endif // TestWidgetGeometry_H
