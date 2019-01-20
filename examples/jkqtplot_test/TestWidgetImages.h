#ifndef TestWidgetImages_H
#define TestWidgetImages_H
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




class TestWidgetImages : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetImages(QWidget *parent = 0);
        
    signals:
        
    public slots:


        void setImgPalette(int index);
        void setImgPalette1(int index);
        void setImgModifier(int index);

    protected:
        QImage img1;


        JKQTPMathImage* pimg2;
        JKQTPMathImage* pimg3;
        JKQTPLotter* plotImg;

        
};


#endif // TestWidgetImages_H
