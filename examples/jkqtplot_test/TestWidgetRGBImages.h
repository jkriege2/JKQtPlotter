#ifndef TestWidgetRGBImages_H
#define TestWidgetRGBImages_H
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
#include "jkqtplotter/jkqtpgraphsimagergb.h"
#include "jkqtplotter/jkqtpelementsoverlay.h"
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/gui/jkqtpgraphsmodel.h"
#include "jkqtplotter/jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestWidgetRGBImages : public QWidget
{
        Q_OBJECT
    public:
        explicit TestWidgetRGBImages(QWidget *parent = 0);
        
    signals:
        
    public slots:
        void setRGBColorBars(bool checked);

        void setRGBMode1(int mode);
        void setRGBMode2(int mode);

        void updateRGBChannels();
        void setRGBModifier(int item);


    protected:


        double* rgb_rimg2;
        double* rgb_gimg2;
        double* rgb_bimg2;
        double* rgb_aimg2;
        int rgb_colr, rgb_colb, rgb_colg, rgb_cola;


        JKQTPlotter* plotImgRGB;
        JKQTPRGBMathImage* rgbimg;
        JKQTPColumnRGBMathImage* rgbimg2;

        QCheckBox* chkRGBUseR;
        QCheckBox* chkRGBUseG;
        QCheckBox* chkRGBUseB;
        QCheckBox* chkRGBUseA;

};


#endif // TestWidgetRGBImages_H
