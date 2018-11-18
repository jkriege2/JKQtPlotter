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
        JKQtPlotter* plotImg;

        
};


#endif // TestWidgetImages_H
