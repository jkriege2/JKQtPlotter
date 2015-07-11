#ifndef TESTMAIN_H
#define TESTMAIN_H
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
#include "../../jkqtptools.h"

#define N1 200
#define N2 50
#define N3 8




class TestMain : public QTabWidget
{
        Q_OBJECT
    public:
        explicit TestMain(QWidget *parent = 0);
        
    signals:
        
    public slots:
        void moveovl();
        void setRGBColorBars(bool checked);
        void setDrawBaseline(bool checked);
        void setYPeaks(bool checked);

        void setPFuncStyle();
        void setPFuncMinPoint(int value);
        void setPFuncMaxRefine(int value);
        void setPFunc(const QString& expression);
        void setPFuncE(const QString& expression);
        void setRGBMode1(int mode);
        void setRGBMode2(int mode);

        void updateRGBChannels();
        void setRGBModifier(int item);

        void setImgPalette(int index);
        void setImgPalette1(int index);
        void setImgModifier(int index);

        void setErrorStyle(int index);
        void setESSymbol(int index);

        void setBarchartLogLog(bool checked);
        void setBarchartAngele(int angel);
        void setBarchartAngele2(int angel);
        void setPSPSymmErrors(bool checked);

        void setSortOrder2(int index);

        void setKeyLayout2(JKQTPkeyLayout layout);
        void setKeyPos2(JKQTPkeyPosition layout);

    protected:
        double x1[N1], x2[N2], x2r[N2]/*, x3[N3]*/;
        double y11[N1], y12[N1], y13[N1], y21[N2], y21r[N2], y21re[N2], y22[N2], y23[N2], y24[N2];
        double xbox[N3], ymed[N3], ymean[N3], ymin[N3], ymax[N3], yp25[N3], yp75[N3];
        double b1[N3], b2[N3], b3[N3];
        QImage img1;

        double* rgb_rimg2;
        double* rgb_gimg2;
        double* rgb_bimg2;
        double* rgb_aimg2;
        int rgb_colr, rgb_colb, rgb_colg, rgb_cola;

        JKQTPoverlayVerticalLine* ovl1;
        JKQtPlotter* plot;
        JKQtPlotter* plotLOG;
        JKQtPlotter* plotBot;
        JKQtPlotter* plotBot2;

        JKQtPlotter* plotImgRGB;
        JKQtPlotter* plotPeaks;
        JKQTPRGBMathImage* rgbimg;
        JKQTPColumnRGBMathImage* rgbimg2;
        JKQTPgraph* plteErrors;
        JKQTPgraph* plteSymbols;
        JKQTPxyLineErrorGraph* pltePlot2;

        JKQTPPeakStreamGraph* graphPeakStream1;
        JKQTPPeakStreamGraph* graphPeakStream2;

        JKQTPxParsedFunctionLineGraph* pfunc;
        JKQtPlotter* plotFuncPlt;
        JKQTPerrorPlotstyleComboBox* pfuncErrorStyle;
        QCheckBox* chkPFuncDrawSamples;
        QCheckBox* chkPFuncDrawLine;
        QCheckBox* chkRGBUseR;
        QCheckBox* chkRGBUseG;
        QCheckBox* chkRGBUseB;
        QCheckBox* chkRGBUseA;

        JKQTPMathImage* pimg2;
        JKQTPMathImage* pimg3;
        JKQtPlotter* plotImg;

        JKQtPlotter* plotPSP;
        JKQTPxyParametrizedErrorScatterGraph* pltePSPCol;
        JKQTPxyParametrizedScatterGraph* pltePSPSize;
        JKQTPxyParametrizedScatterGraph* pltePSPSymb;


        JKQtPlotter* plotBarchart;
        JKQtPlotter* plotBarchart2;
        QCheckBox* chkBarLog;
        QSpinBox* spinBarLabelAngel;
        QSpinBox* spinBarLabelAngel2;

        int ovlCnt;
        QElapsedTimer ovlTime;

        
};


#endif // TESTMAIN_H
