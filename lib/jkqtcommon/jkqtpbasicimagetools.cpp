/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "jkqtcommon/jkqtpbasicimagetools.h"
#include <QPainter>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#ifdef QT_XML_LIB
#  include <QtXml/QtXml>
#endif

const int JKQTPImageTools::PALETTE_ICON_WIDTH = 64;
const int JKQTPImageTools::PALETTE_IMAGEICON_HEIGHT = 64;
const int JKQTPImageTools::LUTSIZE = 256;

QMap<int, JKQTPImageTools::LUTData > JKQTPImageTools::global_jkqtpimagetools_lutstore = JKQTPImageTools::getDefaultLUTs();
int JKQTPImageTools::global_next_userpalette = JKQTPMathImageFIRST_REGISTERED_USER_PALETTE;












QMap<int, JKQTPImageTools::LUTData > JKQTPImageTools::getDefaultLUTs() {
    QMap<int, JKQTPImageTools::LUTData > lutstore;

    {
        auto palette=JKQTPMathImageRED;
        QString palN="red";
        QString palNT=QObject::tr("red");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgb(static_cast<int>(255.0*v), 0, 0);
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageGREEN;
        QString palN="green";
        QString palNT=QObject::tr("green");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgb(0, static_cast<int>(255.0*v), 0);
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageBLUE;
        QString palN="blue";
        QString palNT=QObject::tr("blue");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgb(0, 0, static_cast<int>(255.0*v));
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageGRAY;
        QString palN="gray";
        QString palNT=QObject::tr("gray");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgb(static_cast<int>(255.0*v),
                             static_cast<int>(255.0*v),
                             static_cast<int>(255.0*v));
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageALPHA;
        QString palN="alpha";
        QString palNT=QObject::tr("alpha");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgba(255,255,255,
                              static_cast<int>(255.0*v));
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_ALPHA;
        QString palN="invAlpha";
        QString palNT=QObject::tr("inv. alpha");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgba(255,255,255,
                              static_cast<int>(255.0*v));
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageINVERTEDRED;
        QString palN="invred";
        QString palNT=QObject::tr("inv. red");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgb(static_cast<int>(255.0*(1.0-v)), 0, 0);
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageINVERTEDGREEN;
        QString palN="invgreen";
        QString palNT=QObject::tr("inv. green");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgb(0, static_cast<int>(255.0*(1.0-v)), 0);
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageINVERTEDBLUE;
        QString palN="invblue";
        QString palNT=QObject::tr("inv. blue");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=qRgb(0, 0, static_cast<int>(255.0*(1.0-v)));
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageINVERTEDGRAY;
        QString palN="invgray";
        QString palNT=QObject::tr("inv. gray");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=1.0-(l/static_cast<double>(JKQTPImageTools::LUTSIZE));
                plut[l]=qRgb(static_cast<int>(255.0*v),
                             static_cast<int>(255.0*v),
                             static_cast<int>(255.0*v));
            }
            plut[JKQTPImageTools::LUTSIZE+1]=plut[JKQTPImageTools::LUTSIZE];
        }
    }


    {
        auto palette=JKQTPMathImageMATLAB;
        QString palN="Matlab";
        QString palNT=QObject::tr("Matlab");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 382.5 - 1020.0 * std::abs(v - 0.75);
                if (r > 255.0)
                    r = 255.0;
                else if (r < 0.0)
                    r = 0.0;

                double g = 382.5 - 1020.0 * std::abs(v - 0.5);
                if (g > 255.0)
                    g = 255.0;
                else if (g < 0.0)
                    g = 0.0;

                double b = 382.5 - 1020.0 * std::abs(v - 0.25);
                if (b > 255.0)
                    b = 255.0;
                else if (b < 0.0)
                    b = 0.0;

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_MATLAB;
        QString palN="invMatlab";
        QString palNT=QObject::tr("inv. Matlab");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=JKQTPImageTools::LUTSIZE; l>=0; l--) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 382.5 - 1020.0 * std::abs(v - 0.75);
                if (r > 255.0)
                    r = 255.0;
                else if (r < 0.0)
                    r = 0.0;

                double g = 382.5 - 1020.0 * std::abs(v - 0.5);
                if (g > 255.0)
                    g = 255.0;
                else if (g < 0.0)
                    g = 0.0;

                double b = 382.5 - 1020.0 * std::abs(v - 0.25);
                if (b > 255.0)
                    b = 255.0;
                else if (b < 0.0)
                    b = 0.0;

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageRYGB;
        QString palN="RYGB";
        QString palNT=QObject::tr("RYGB");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 796.875*v - 199.21875;
                if (r > 255.0)
                    r = 255.0;
                else if (r < 0.0)
                    r = 0.0;

                double g = 255.0 * std::sin(M_PI*v);

                double b = 255.0 - 765.0 * v;
                if (b < 0.0)
                    b = 0.0;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_RYGB;
        QString palN="invRYGB";
        QString palNT=QObject::tr("inv. RYGB");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          if (plut!=nullptr) {
            for (int l=JKQTPImageTools::LUTSIZE; l>=0; l--) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 796.875*v - 199.21875;
                if (r > 255.0)
                    r = 255.0;
                else if (r < 0.0)
                    r = 0.0;

                double g = 255.0 * std::sin(M_PI*v);

                double b = 255.0 - 765.0 * v;
                if (b < 0.0)
                    b = 0.0;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageHSV;
        QString palN="HSV";
        QString palNT=QObject::tr("HSV");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                int h = static_cast<int>(floor(6*v));
                double f = 6*v-double(h);

                switch (h)
                {
                    case 0: plut[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                    case 1: plut[l]=qRgb(static_cast<int>(255.0*(1-f)), 255, 0); break;
                    case 2: plut[l]=qRgb(0, 255, static_cast<int>(255.0*f)); break;
                    case 3: plut[l]=qRgb(0, static_cast<int>(255.0*(1-f)), 255); break;
                    case 4: plut[l]=qRgb(static_cast<int>(255.0*f), 0, 255); break;
                    case 5: plut[l]=qRgb(255, 0, static_cast<int>(255.0*(1-f))); break;
                    case 6: plut[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                }
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_HSV;
        QString palN="invHSV";
        QString palNT=QObject::tr("inv. HSV");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                int h = static_cast<int>(floor(6.0-6.0*v));
                double f = 6.0-6.0*v-double(h);

                switch (h)
                {
                    case 0: plut[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                    case 1: plut[l]=qRgb(static_cast<int>(255.0*(1-f)), 255, 0); break;
                    case 2: plut[l]=qRgb(0, 255, static_cast<int>(255.0*f)); break;
                    case 3: plut[l]=qRgb(0, static_cast<int>(255.0*(1-f)), 255); break;
                    case 4: plut[l]=qRgb(static_cast<int>(255.0*f), 0, 255); break;
                    case 5: plut[l]=qRgb(255, 0, static_cast<int>(255.0*(1-f))); break;
                    case 6: plut[l]=qRgb(255, static_cast<int>(255.0*f), 0); break;
                }
            }
        }
    }


    {
        auto palette=JKQTPMathImageRAINBOW;
        QString palN="rainbow";
        QString palNT=QObject::tr("rainbow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();         //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 255.0*std::abs(2.0*v-0.5);
                if (r > 255.0)
                    r = 255.0;

                double g = 255.0*sin(M_PI*v);

                double b = 255.0*cos(0.5*M_PI*v);

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_RAINBOW;
        QString palN="invrainbow";
        QString palNT=QObject::tr("inv. rainbow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();         //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=JKQTPImageTools::LUTSIZE; l>=0; l--) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 255.0*std::abs(2.0*v-0.5);
                if (r > 255.0)
                    r = 255.0;

                double g = 255.0*sin(M_PI*v);

                double b = 255.0*cos(0.5*M_PI*v);

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageHOT;
        QString palN="AFMhot";
        QString palNT=QObject::tr("AFM hot");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 765.0*v;
                if (r > 255.0)
                    r = 255.0;

                double g = 765.0*v-255.0;
                if (g > 255.0)
                    g = 255.0;
                else if (g < 0.0)
                    g = 0.0;

                double b = 765.0*v-510.0;
                if (b < 0.0)
                    b = 0.0;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_HOT;
        QString palN="invAFMhot";
        QString palNT=QObject::tr("inv. AFM hot");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=JKQTPImageTools::LUTSIZE; l>=0; l--) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 765.0*v;
                if (r > 255.0)
                    r = 255.0;

                double g = 765.0*v-255.0;
                if (g > 255.0)
                    g = 255.0;
                else if (g < 0.0)
                    g = 0.0;

                double b = 765.0*v-510.0;
                if (b < 0.0)
                    b = 0.0;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageOCEAN;
        QString palN="ocean";
        QString palNT=QObject::tr("ocean");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 765.0*v-510.0;
                if (r < 0.0)
                    r = 0.0;

                double g = std::abs(382.5*v-127.5);

                double b = 255.0*v;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_OCEAN;
        QString palN="invocean";
        QString palNT=QObject::tr("inv. ocean");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=JKQTPImageTools::LUTSIZE; l>=0; l--) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 765.0*v-510.0;
                if (r < 0.0)
                    r = 0.0;

                double g = std::abs(382.5*v-127.5);

                double b = 255.0*v;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageBLUEMAGENTAYELLOW;
        QString palN="BlMaYe";
        QString palNT=QObject::tr("blue-magenta-yellow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = (v/0.32-0.78125);
                if (r < 0.0) r = 0.0;
                if (r > 1.0) r = 1.0;

                double g = 2.0*v-0.84;
                if (g < 0.0) g = 0.0;
                if (g > 1.0) g = 1.0;

                double b = 4.0*v;
                if (b>1 || b<0) b = -2.0*v+1.84;
                if (b>1 || b<0) b = v/0.08-11.5;
                if (b>1 || b<0) b=1;

                if (b < 0.0) b = 0.0;
                if (b > 1.0) b = 1.0;
                plut[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_BLUEMAGENTAYELLOW;
        QString palN="YeMaBl";
        QString palNT=QObject::tr("yellow-magenta-blue");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();            if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = (v/0.32-0.78125);
                if (r < 0.0) r = 0.0;
                if (r > 1.0) r = 1.0;

                double g = 2.0*v-0.84;
                if (g < 0.0) g = 0.0;
                if (g > 1.0) g = 1.0;

                double b = 4.0*v;
                if (b>1 || b<0) b = -2.0*v+1.84;
                if (b>1 || b<0) b = v/0.08-11.5;
                if (b>1 || b<0) b=1;

                if (b < 0.0) b = 0.0;
                if (b > 1.0) b = 1.0;
                plut[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageBLUEYELLOW;
        QString palN="BlYe";
        QString palNT=QObject::tr("blue-yellow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = sqrt(sqrt(v));
                if (r < 0.0) r = 0.0;
                if (r > 1.0) r = 1.0;

                double g = sin(M_PI/2.0*v);
                if (g < 0.0) g = 0.0;
                if (g > 1.0) g = 1.0;

                double b = cos(M_PI/2.0*v);
                if (b < 0.0) b = 0.0;
                if (b > 1.0) b = 1.0;

                plut[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_BLUEYELLOW;
        QString palN="YeBl";
        QString palNT=QObject::tr("yellow-blue");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = sqrt(sqrt(v));
                if (r < 0.0) r = 0.0;
                if (r > 1.0) r = 1.0;

                double g = sin(M_PI/2.0*v);
                if (g < 0.0) g = 0.0;
                if (g > 1.0) g = 1.0;

                double b = cos(M_PI/2.0*v);
                if (b < 0.0) b = 0.0;
                if (b > 1.0) b = 1.0;

                plut[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageCYAN;
        QString palN="cyan";
        QString palNT=QObject::tr("cyan");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = v*0.5;
                if (r < 0.0) r = 0.0;
                if (r > 1.0) r = 1.0;

                double g = v;
                if (g < 0.0) g = 0.0;
                if (g > 1.0) g = 1.0;

                double b = v;
                if (b < 0.0) b = 0.0;
                if (b > 1.0) b = 1.0;
                plut[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_CYAN;
        QString palN="invcyan";
        QString palNT=QObject::tr("inv. cyan");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = v*0.5;
                if (r < 0.0) r = 0.0;
                if (r > 1.0) r = 1.0;

                double g = v;
                if (g < 0.0) g = 0.0;
                if (g > 1.0) g = 1.0;

                double b = v;
                if (b < 0.0) b = 0.0;
                if (b > 1.0) b = 1.0;
                plut[l]=qRgb(static_cast<int>(255.0*r), static_cast<int>(255.0*g), static_cast<int>(255.0*b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageTRAFFICLIGHT;
        QString palN="trafficlight";
        QString palNT=QObject::tr("trafficlight");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=l/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = (v < 0.5) ? 128.0*sin(M_PI*(2.0*v-0.5))+128.0 : 255.0;
                if (r > 255.0)
                    r = 255.0;

                double g = (v < 0.5) ? 512.0*v+128.0 : 512.0-512.0*v;
                if (g > 255.0)
                    g = 255.0;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), 0);
            }
        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_TRAFFICLIGHT;
        QString palN="invtrafficlight";
        QString palNT=QObject::tr("inv. trafficlight");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = (v < 0.5) ? 128.0*sin(M_PI*(2.0*v-0.5))+128.0 : 255.0;
                if (r > 255.0)
                    r = 255.0;

                double g = (v < 0.5) ? 512.0*v+128.0 : 512.0-512.0*v;
                if (g > 255.0)
                    g = 255.0;
                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), 0);
            }
        }
    }


    {
        auto palette=JKQTPMathImageBLUEWHITERED;
        QString palN="bluewhitered";
        QString palNT=QObject::tr("blue-white-red");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(8.0, 0xFFB2182B);
        lst<<qMakePair<double, QRgb>(7.0, 0xFFD6604D);
        lst<<qMakePair<double, QRgb>(6.0, 0xFFF4A582);
        lst<<qMakePair<double, QRgb>(5.0, 0xFFFDDBC7);
        lst<<qMakePair<double, QRgb>(4.0, 0xFFD1E5F0);
        lst<<qMakePair<double, QRgb>(3.0, 0xFF92C5DE);
        lst<<qMakePair<double, QRgb>(2.0, 0xFF4393C3);
        lst<<qMakePair<double, QRgb>(1.0, 0xFF2166AC);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst, JKQTPImageTools::LUTSIZE+1);
    }


    {
        auto palette=JKQTPMathImageREDWHITEBLUE;
        QString palN="redwhiteblue";
        QString palNT=QObject::tr("red-white-blue");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);

        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, 0xFFB2182B);
        lst<<qMakePair<double, QRgb>(1.0, 0xFFD6604D);
        lst<<qMakePair<double, QRgb>(2.0, 0xFFF4A582);
        lst<<qMakePair<double, QRgb>(3.0, 0xFFFDDBC7);
        lst<<qMakePair<double, QRgb>(4.0, 0xFFD1E5F0);
        lst<<qMakePair<double, QRgb>(5.0, 0xFF92C5DE);
        lst<<qMakePair<double, QRgb>(6.0, 0xFF4393C3);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF2166AC);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst, JKQTPImageTools::LUTSIZE+1);

    }


    {
        auto palette=JKQTPMathImageBLACKBLUEREDYELLOW;
        QString palN="BBlRdYe";
        QString palNT=QObject::tr("black-blue-red-yellow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 255.0*qBound(0.0,sqrt(v),1.0);
                double g = 255.0*qBound(0.0,v*v*v,1.0);
                double b = 255.0*qBound(0.0,sin(2.0*M_PI*v),1.0);

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageGREENREDVIOLET;
        QString palN="GnRdVi";
        QString palNT=QObject::tr("green-red-violet");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 255.0*qBound(0.0,v,1.0);
                double g = 255.0*qBound(0.0,fabs(v-0.5),1.0);
                double b = 255.0*qBound(0.0,v*v*v*v,1.0);

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageBLACKBLUEWHITEYELLOWWHITE;
        QString palN="BWprint";
        QString palNT=QObject::tr("black-blue-white-yellow-white");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                double g = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                double b = 255.0*qBound(0.0,(v<0.25)?4*v:(v<0.42)?1.0:(v<0.92)?-2.0*v+1.84:v/0.08-11.5,1.0);

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageWHITEYELLOWWHITEBLUEBLACK;
        QString palN="invBWprint";
        QString palNT=QObject::tr("white-yellow-white-blue-black");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                double r = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                double g = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                double b = 255.0*qBound(0.0,(v<0.25)?4*v:(v<0.42)?1.0:(v<0.92)?-2.0*v+1.84:v/0.08-11.5,1.0);

                plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }


    {
        auto palette=JKQTPMathImageBR_GR;
        QString palN="BrBG";
        QString palNT=QObject::tr("BrBG");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, qRgb(0x8C, 0x51, 0x0A));
        lst<<qMakePair<double, QRgb>(1.0, qRgb(0xBF, 0x81, 0x2D));
        lst<<qMakePair<double, QRgb>(2.0, qRgb(0xDF, 0xC2, 0x7D));
        lst<<qMakePair<double, QRgb>(3.0, 0xFFF6E8C3);
        lst<<qMakePair<double, QRgb>(4.0, 0xFFC7EAE5);
        lst<<qMakePair<double, QRgb>(5.0, 0xFF80CDC1);
        lst<<qMakePair<double, QRgb>(6.0, 0xFF35978F);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF01665E);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageBR_GR_STEP;
        QString palN="stepsBrBG";
        QString palNT=QObject::tr("steps: BrBG");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);

        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, qRgb(0x8C, 0x51, 0x0A));
        lst<<qMakePair<double, QRgb>(1.0, qRgb(0xBF, 0x81, 0x2D));
        lst<<qMakePair<double, QRgb>(2.0, qRgb(0xDF, 0xC2, 0x7D));
        lst<<qMakePair<double, QRgb>(3.0, 0xFFF6E8C3);
        lst<<qMakePair<double, QRgb>(4.0, 0xFFC7EAE5);
        lst<<qMakePair<double, QRgb>(5.0, 0xFF80CDC1);
        lst<<qMakePair<double, QRgb>(6.0, 0xFF35978F);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF01665E);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUT(lst);

    }


    {
        auto palette=JKQTPMathImagePU_OR;
        QString palN="PuOr";
        QString palNT=QObject::tr("PuOr");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, 0xFFB35806);
        lst<<qMakePair<double, QRgb>(1.0, 0xFFE08214);
        lst<<qMakePair<double, QRgb>(2.0, 0xFFFDB863);
        lst<<qMakePair<double, QRgb>(3.0, 0xFFFEE0B6);
        lst<<qMakePair<double, QRgb>(4.0, 0xFFF7F7F7);
        lst<<qMakePair<double, QRgb>(5.0, 0xFFD8DAEB);
        lst<<qMakePair<double, QRgb>(6.0, 0xFFB2ABD2);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF8073AC);
        lst<<qMakePair<double, QRgb>(8.0, 0xFF542788);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImagePU_OR_STEP;
        QString palN="stepsPuOr";
        QString palNT=QObject::tr("steps: PuOr");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, 0xFFB35806);
        lst<<qMakePair<double, QRgb>(1.0, 0xFFE08214);
        lst<<qMakePair<double, QRgb>(2.0, 0xFFFDB863);
        lst<<qMakePair<double, QRgb>(3.0, 0xFFFEE0B6);
        lst<<qMakePair<double, QRgb>(4.0, 0xFFF7F7F7);
        lst<<qMakePair<double, QRgb>(5.0, 0xFFD8DAEB);
        lst<<qMakePair<double, QRgb>(6.0, 0xFFB2ABD2);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF8073AC);
        lst<<qMakePair<double, QRgb>(8.0, 0xFF542788);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUT(lst);
    }


    {
        auto palette=JKQTPMathImageYL_GN_BU;
        QString palN="YeGnBu";
        QString palNT=QObject::tr("YeGnBu");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, 0xFFFFFFD9);
        lst<<qMakePair<double, QRgb>(1.0, 0xFFEDF8B1);
        lst<<qMakePair<double, QRgb>(2.0, 0xFFC7E9B4);
        lst<<qMakePair<double, QRgb>(3.0, 0xFF7FCDBB);
        lst<<qMakePair<double, QRgb>(4.0, 0xFF41B6C4);
        lst<<qMakePair<double, QRgb>(5.0, 0xFF1D91C0);
        lst<<qMakePair<double, QRgb>(6.0, 0xFF225EA8);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF253494);
        lst<<qMakePair<double, QRgb>(8.0, 0xFF081D58);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageYL_GN_BU_STEP;
        QString palN="stepsYeGnBu";
        QString palNT=QObject::tr("steps: YeGnBu");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, 0xFFFFFFD9);
        lst<<qMakePair<double, QRgb>(1.0, 0xFFEDF8B1);
        lst<<qMakePair<double, QRgb>(2.0, 0xFFC7E9B4);
        lst<<qMakePair<double, QRgb>(3.0, 0xFF7FCDBB);
        lst<<qMakePair<double, QRgb>(4.0, 0xFF41B6C4);
        lst<<qMakePair<double, QRgb>(5.0, 0xFF1D91C0);
        lst<<qMakePair<double, QRgb>(6.0, 0xFF225EA8);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF253494);
        lst<<qMakePair<double, QRgb>(8.0, 0xFF081D58);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUT(lst);
    }


    {
        auto palette=JKQTPMathImageGN_BU;
        QString palN="greenblue";
        QString palNT=QObject::tr("green-blue");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, 0xFFF7FCF0);
        lst<<qMakePair<double, QRgb>(1.0, 0xFFE0F3DB);
        lst<<qMakePair<double, QRgb>(2.0, 0xFFCCEBC5);
        lst<<qMakePair<double, QRgb>(3.0, 0xFFA8DDB5);
        lst<<qMakePair<double, QRgb>(4.0, 0xFF7BCCC4);
        lst<<qMakePair<double, QRgb>(5.0, 0xFF4EB3D3);
        lst<<qMakePair<double, QRgb>(6.0, 0xFF2B8CBE);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF0868AC);
        lst<<qMakePair<double, QRgb>(8.0, 0xFF084081);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageGN_BU_STEP;
        QString palN="stepsGnBl";
        QString palNT=QObject::tr("steps: green-blue");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.0, 0xFFF7FCF0);
        lst<<qMakePair<double, QRgb>(1.0, 0xFFE0F3DB);
        lst<<qMakePair<double, QRgb>(2.0, 0xFFCCEBC5);
        lst<<qMakePair<double, QRgb>(3.0, 0xFFA8DDB5);
        lst<<qMakePair<double, QRgb>(4.0, 0xFF7BCCC4);
        lst<<qMakePair<double, QRgb>(5.0, 0xFF4EB3D3);
        lst<<qMakePair<double, QRgb>(6.0, 0xFF2B8CBE);
        lst<<qMakePair<double, QRgb>(7.0, 0xFF0868AC);
        lst<<qMakePair<double, QRgb>(8.0, 0xFF084081);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUT(lst);
    }


    {
        auto palette=JKQTPMathImageBU_GN;
        QString palN="bluegreen";
        QString palNT=QObject::tr("blue-green");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(8.0, 0xFFF7FCF0);
        lst<<qMakePair<double, QRgb>(7.0, 0xFFE0F3DB);
        lst<<qMakePair<double, QRgb>(6.0, 0xFFCCEBC5);
        lst<<qMakePair<double, QRgb>(5.0, 0xFFA8DDB5);
        lst<<qMakePair<double, QRgb>(4.0, 0xFF7BCCC4);
        lst<<qMakePair<double, QRgb>(3.0, 0xFF4EB3D3);
        lst<<qMakePair<double, QRgb>(2.0, 0xFF2B8CBE);
        lst<<qMakePair<double, QRgb>(1.0, 0xFF0868AC);
        lst<<qMakePair<double, QRgb>(0.0, 0xFF084081);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageBU_GN_STEP;
        QString palN="stepsBlGn";
        QString palNT=QObject::tr("steps: blue-green");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(8.0, 0xFFF7FCF0);
        lst<<qMakePair<double, QRgb>(7.0, 0xFFE0F3DB);
        lst<<qMakePair<double, QRgb>(6.0, 0xFFCCEBC5);
        lst<<qMakePair<double, QRgb>(5.0, 0xFFA8DDB5);
        lst<<qMakePair<double, QRgb>(4.0, 0xFF7BCCC4);
        lst<<qMakePair<double, QRgb>(3.0, 0xFF4EB3D3);
        lst<<qMakePair<double, QRgb>(2.0, 0xFF2B8CBE);
        lst<<qMakePair<double, QRgb>(1.0, 0xFF0868AC);
        lst<<qMakePair<double, QRgb>(0.0, 0xFF084081);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUT(lst);
    }


    {
        auto palette=JKQTPMathImageINVERTED_MAGENTA;
        QString palN="invmagenta";
        QString palNT=QObject::tr("inv. magenta");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,1,0,v).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageMAGENTA;
        QString palN="magenta";
        QString palNT=QObject::tr("magenta");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,1,0,v).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_YELLOW;
        QString palN="invyellow";
        QString palNT=QObject::tr("inv. yellow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,0,1,v).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageYELLOW;
        QString palN="yellow";
        QString palNT=QObject::tr("yellow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,0,1,v).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_MAGENTAWHITE;
        QString palN="whitemagenta";
        QString palNT=QObject::tr("white-magenta");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,v,0,0).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageMAGENTAWHITE;
        QString palN="magentawhite";
        QString palNT=QObject::tr("magenta-white");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,v,0,0).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_YELLOWWHITE;
        QString palN="whiteyellow";
        QString palNT=QObject::tr("white-yellow");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,0,v,0).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageYELLOWWHITE;
        QString palN="yellowwhite";
        QString palNT=QObject::tr("yellow-white");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(0,0,v,0).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageINVERTED_CYANWHITE;
        QString palN="whitecyan";
        QString palNT=QObject::tr("white-cyan");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(v,0,0,0).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageCYANWHITE;
        QString palN="cyanwhite";
        QString palNT=QObject::tr("cyan-white");
        lutstore[palette]=JKQTPImageTools::LUTData(QVector<QRgb>(JKQTPImageTools::LUTSIZE+1, 0), palN, palNT);
        QRgb* plut=lutstore[palette].lut.data();          //std::cout<<"!!! creating rainbow lut\n";
        if (plut!=nullptr) {
            for (int l=0; l<=JKQTPImageTools::LUTSIZE; l++) {
                double v=(JKQTPImageTools::LUTSIZE-l)/static_cast<double>(JKQTPImageTools::LUTSIZE);
                plut[l]=QColor::fromCmykF(v,0,0,0).rgba();
            }

        }
    }


    {
        auto palette=JKQTPMathImageBlueGreenRed;
        QString palN="bluegreenred";
        QString palNT=QObject::tr("blue-green-red");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(1.0, QColor("blue").rgb());
        lst<<qMakePair<double, QRgb>(2.0, QColor("green").rgb());
        lst<<qMakePair<double, QRgb>(3.0, QColor("red").rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageRedGreenBlue;
        QString palN="redgreenblue";
        QString palNT=QObject::tr("red-green-blue");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(1.0, QColor("red").rgb());
        lst<<qMakePair<double, QRgb>(2.0, QColor("green").rgb());
        lst<<qMakePair<double, QRgb>(3.0, QColor("blue").rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageMagentaYellow;
        QString palN="magentayellow";
        QString palNT=QObject::tr("magenta-yellow");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(1.0, QColor("magenta").rgb());
        lst<<qMakePair<double, QRgb>(2.0, QColor("yellow").rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageYellowMagenta;
        QString palN="yellowmagenta";
        QString palNT=QObject::tr("yellow-magenta");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(1.0, QColor("yellow").rgb());
        lst<<qMakePair<double, QRgb>(2.0, QColor("magenta").rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageRedBlue;
        QString palN="redblue";
        QString palNT=QObject::tr("red-blue");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(1.0, QColor("red").rgb());
        lst<<qMakePair<double, QRgb>(2.0, QColor("blue").rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }


    {
        auto palette=JKQTPMathImageBlueRed;
        QString palN="bluered";
        QString palNT=QObject::tr("blue-red");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(1.0, QColor("blue").rgb());
        lst<<qMakePair<double, QRgb>(2.0, QColor("red").rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }

    {
        auto palette=JKQTPMathImageSeismic;
        QString palN="seismic";
        QString palNT=QObject::tr("seismic");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0, QColor::fromRgbF(0.0, 0.0, 0.3).rgb());
        lst<<qMakePair<double, QRgb>(1, QColor::fromRgbF(0.0, 0.0, 1.0).rgb());
        lst<<qMakePair<double, QRgb>(2, QColor::fromRgbF(1.0, 1.0, 1.0).rgb());
        lst<<qMakePair<double, QRgb>(3, QColor::fromRgbF(1.0, 0.0, 0.0).rgb());
        lst<<qMakePair<double, QRgb>(4, QColor::fromRgbF(0.5, 0.0, 0.0).rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }

    {
        auto palette=JKQTPMathImageTerrain;
        QString palN="terrain";
        QString palNT=QObject::tr("terrain");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<QPair<double, QRgb> > lst;
        lst<<qMakePair<double, QRgb>(0.00, QColor::fromRgbF(0.2, 0.2, 0.6).rgb());
        lst<<qMakePair<double, QRgb>(0.15, QColor::fromRgbF(0.0, 0.6, 1.0).rgb());
        lst<<qMakePair<double, QRgb>(0.25, QColor::fromRgbF(0.0, 0.8, 0.4).rgb());
        lst<<qMakePair<double, QRgb>(0.50, QColor::fromRgbF(1.0, 1.0, 0.6).rgb());
        lst<<qMakePair<double, QRgb>(0.75, QColor::fromRgbF(0.5, 0.36, 0.33).rgb());
        lst<<qMakePair<double, QRgb>(1.00, QColor::fromRgbF(1.0, 1.0, 1.0).rgb());

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
    }

    {
        auto palette=JKQTPMathImageBone;
        QString palN="bone";
        QString palNT=QObject::tr("bone");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<JKQTPColorPaletteSingleColorLinSegment> lstR,lstG,lstB;
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        0.0,      0.0);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.746032, 0.652778, 0.652778);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        0.0,      0.0);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.365079, 0.319444, 0.319444);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.746032, 0.777778, 0.777778);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        0.0,      0.0);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.365079, 0.444444, 0.444444);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB);
    }

    {
        auto palette=JKQTPMathImageCool;
        QString palN="cool";
        QString palNT=QObject::tr("cool");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<JKQTPColorPaletteSingleColorLinSegment> lstR,lstG,lstB;
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        0.0,      0.0);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      1.0,      1.0);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.0,      0.0);

        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      1.0,      1.0);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB);
    }

    {
        auto palette=JKQTPMathImageCopper;
        QString palN="copper";
        QString palNT=QObject::tr("copper");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<JKQTPColorPaletteSingleColorLinSegment> lstR,lstG,lstB;
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        0.0,      0.0);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.809524, 1.000000, 1.000000);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.7812,   0.7812);

        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.4975,   0.4975);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB);
    }

    {
        auto palette=JKQTPMathImageAutumn;
        QString palN="autumn";
        QString palNT=QObject::tr("autumn");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        QList<JKQTPColorPaletteSingleColorLinSegment> lstR,lstG,lstB;
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        1.0,      1.0);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.0,      0.0);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB);
    }


    {
        // from https://github.com/BIDS/colormap/blob/master/colormaps.py
        // https://github.com/BIDS/colormap/blob/master/colormaps.py
        auto palette=JKQTPMathImageViridis;
        QString palN="viridis";
        QString palNT=QObject::tr("viridis");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        lutstore[palette].lut=LUTType{
                         QColor::fromRgbF(0.267004, 0.004874, 0.329415).rgb(),
                         QColor::fromRgbF(0.268510, 0.009605, 0.335427).rgb(),
                         QColor::fromRgbF(0.269944, 0.014625, 0.341379).rgb(),
                         QColor::fromRgbF(0.271305, 0.019942, 0.347269).rgb(),
                         QColor::fromRgbF(0.272594, 0.025563, 0.353093).rgb(),
                         QColor::fromRgbF(0.273809, 0.031497, 0.358853).rgb(),
                         QColor::fromRgbF(0.274952, 0.037752, 0.364543).rgb(),
                         QColor::fromRgbF(0.276022, 0.044167, 0.370164).rgb(),
                         QColor::fromRgbF(0.277018, 0.050344, 0.375715).rgb(),
                         QColor::fromRgbF(0.277941, 0.056324, 0.381191).rgb(),
                         QColor::fromRgbF(0.278791, 0.062145, 0.386592).rgb(),
                         QColor::fromRgbF(0.279566, 0.067836, 0.391917).rgb(),
                         QColor::fromRgbF(0.280267, 0.073417, 0.397163).rgb(),
                         QColor::fromRgbF(0.280894, 0.078907, 0.402329).rgb(),
                         QColor::fromRgbF(0.281446, 0.084320, 0.407414).rgb(),
                         QColor::fromRgbF(0.281924, 0.089666, 0.412415).rgb(),
                         QColor::fromRgbF(0.282327, 0.094955, 0.417331).rgb(),
                         QColor::fromRgbF(0.282656, 0.100196, 0.422160).rgb(),
                         QColor::fromRgbF(0.282910, 0.105393, 0.426902).rgb(),
                         QColor::fromRgbF(0.283091, 0.110553, 0.431554).rgb(),
                         QColor::fromRgbF(0.283197, 0.115680, 0.436115).rgb(),
                         QColor::fromRgbF(0.283229, 0.120777, 0.440584).rgb(),
                         QColor::fromRgbF(0.283187, 0.125848, 0.444960).rgb(),
                         QColor::fromRgbF(0.283072, 0.130895, 0.449241).rgb(),
                         QColor::fromRgbF(0.282884, 0.135920, 0.453427).rgb(),
                         QColor::fromRgbF(0.282623, 0.140926, 0.457517).rgb(),
                         QColor::fromRgbF(0.282290, 0.145912, 0.461510).rgb(),
                         QColor::fromRgbF(0.281887, 0.150881, 0.465405).rgb(),
                         QColor::fromRgbF(0.281412, 0.155834, 0.469201).rgb(),
                         QColor::fromRgbF(0.280868, 0.160771, 0.472899).rgb(),
                         QColor::fromRgbF(0.280255, 0.165693, 0.476498).rgb(),
                         QColor::fromRgbF(0.279574, 0.170599, 0.479997).rgb(),
                         QColor::fromRgbF(0.278826, 0.175490, 0.483397).rgb(),
                         QColor::fromRgbF(0.278012, 0.180367, 0.486697).rgb(),
                         QColor::fromRgbF(0.277134, 0.185228, 0.489898).rgb(),
                         QColor::fromRgbF(0.276194, 0.190074, 0.493001).rgb(),
                         QColor::fromRgbF(0.275191, 0.194905, 0.496005).rgb(),
                         QColor::fromRgbF(0.274128, 0.199721, 0.498911).rgb(),
                         QColor::fromRgbF(0.273006, 0.204520, 0.501721).rgb(),
                         QColor::fromRgbF(0.271828, 0.209303, 0.504434).rgb(),
                         QColor::fromRgbF(0.270595, 0.214069, 0.507052).rgb(),
                         QColor::fromRgbF(0.269308, 0.218818, 0.509577).rgb(),
                         QColor::fromRgbF(0.267968, 0.223549, 0.512008).rgb(),
                         QColor::fromRgbF(0.266580, 0.228262, 0.514349).rgb(),
                         QColor::fromRgbF(0.265145, 0.232956, 0.516599).rgb(),
                         QColor::fromRgbF(0.263663, 0.237631, 0.518762).rgb(),
                         QColor::fromRgbF(0.262138, 0.242286, 0.520837).rgb(),
                         QColor::fromRgbF(0.260571, 0.246922, 0.522828).rgb(),
                         QColor::fromRgbF(0.258965, 0.251537, 0.524736).rgb(),
                         QColor::fromRgbF(0.257322, 0.256130, 0.526563).rgb(),
                         QColor::fromRgbF(0.255645, 0.260703, 0.528312).rgb(),
                         QColor::fromRgbF(0.253935, 0.265254, 0.529983).rgb(),
                         QColor::fromRgbF(0.252194, 0.269783, 0.531579).rgb(),
                         QColor::fromRgbF(0.250425, 0.274290, 0.533103).rgb(),
                         QColor::fromRgbF(0.248629, 0.278775, 0.534556).rgb(),
                         QColor::fromRgbF(0.246811, 0.283237, 0.535941).rgb(),
                         QColor::fromRgbF(0.244972, 0.287675, 0.537260).rgb(),
                         QColor::fromRgbF(0.243113, 0.292092, 0.538516).rgb(),
                         QColor::fromRgbF(0.241237, 0.296485, 0.539709).rgb(),
                         QColor::fromRgbF(0.239346, 0.300855, 0.540844).rgb(),
                         QColor::fromRgbF(0.237441, 0.305202, 0.541921).rgb(),
                         QColor::fromRgbF(0.235526, 0.309527, 0.542944).rgb(),
                         QColor::fromRgbF(0.233603, 0.313828, 0.543914).rgb(),
                         QColor::fromRgbF(0.231674, 0.318106, 0.544834).rgb(),
                         QColor::fromRgbF(0.229739, 0.322361, 0.545706).rgb(),
                         QColor::fromRgbF(0.227802, 0.326594, 0.546532).rgb(),
                         QColor::fromRgbF(0.225863, 0.330805, 0.547314).rgb(),
                         QColor::fromRgbF(0.223925, 0.334994, 0.548053).rgb(),
                         QColor::fromRgbF(0.221989, 0.339161, 0.548752).rgb(),
                         QColor::fromRgbF(0.220057, 0.343307, 0.549413).rgb(),
                         QColor::fromRgbF(0.218130, 0.347432, 0.550038).rgb(),
                         QColor::fromRgbF(0.216210, 0.351535, 0.550627).rgb(),
                         QColor::fromRgbF(0.214298, 0.355619, 0.551184).rgb(),
                         QColor::fromRgbF(0.212395, 0.359683, 0.551710).rgb(),
                         QColor::fromRgbF(0.210503, 0.363727, 0.552206).rgb(),
                         QColor::fromRgbF(0.208623, 0.367752, 0.552675).rgb(),
                         QColor::fromRgbF(0.206756, 0.371758, 0.553117).rgb(),
                         QColor::fromRgbF(0.204903, 0.375746, 0.553533).rgb(),
                         QColor::fromRgbF(0.203063, 0.379716, 0.553925).rgb(),
                         QColor::fromRgbF(0.201239, 0.383670, 0.554294).rgb(),
                         QColor::fromRgbF(0.199430, 0.387607, 0.554642).rgb(),
                         QColor::fromRgbF(0.197636, 0.391528, 0.554969).rgb(),
                         QColor::fromRgbF(0.195860, 0.395433, 0.555276).rgb(),
                         QColor::fromRgbF(0.194100, 0.399323, 0.555565).rgb(),
                         QColor::fromRgbF(0.192357, 0.403199, 0.555836).rgb(),
                         QColor::fromRgbF(0.190631, 0.407061, 0.556089).rgb(),
                         QColor::fromRgbF(0.188923, 0.410910, 0.556326).rgb(),
                         QColor::fromRgbF(0.187231, 0.414746, 0.556547).rgb(),
                         QColor::fromRgbF(0.185556, 0.418570, 0.556753).rgb(),
                         QColor::fromRgbF(0.183898, 0.422383, 0.556944).rgb(),
                         QColor::fromRgbF(0.182256, 0.426184, 0.557120).rgb(),
                         QColor::fromRgbF(0.180629, 0.429975, 0.557282).rgb(),
                         QColor::fromRgbF(0.179019, 0.433756, 0.557430).rgb(),
                         QColor::fromRgbF(0.177423, 0.437527, 0.557565).rgb(),
                         QColor::fromRgbF(0.175841, 0.441290, 0.557685).rgb(),
                         QColor::fromRgbF(0.174274, 0.445044, 0.557792).rgb(),
                         QColor::fromRgbF(0.172719, 0.448791, 0.557885).rgb(),
                         QColor::fromRgbF(0.171176, 0.452530, 0.557965).rgb(),
                         QColor::fromRgbF(0.169646, 0.456262, 0.558030).rgb(),
                         QColor::fromRgbF(0.168126, 0.459988, 0.558082).rgb(),
                         QColor::fromRgbF(0.166617, 0.463708, 0.558119).rgb(),
                         QColor::fromRgbF(0.165117, 0.467423, 0.558141).rgb(),
                         QColor::fromRgbF(0.163625, 0.471133, 0.558148).rgb(),
                         QColor::fromRgbF(0.162142, 0.474838, 0.558140).rgb(),
                         QColor::fromRgbF(0.160665, 0.478540, 0.558115).rgb(),
                         QColor::fromRgbF(0.159194, 0.482237, 0.558073).rgb(),
                         QColor::fromRgbF(0.157729, 0.485932, 0.558013).rgb(),
                         QColor::fromRgbF(0.156270, 0.489624, 0.557936).rgb(),
                         QColor::fromRgbF(0.154815, 0.493313, 0.557840).rgb(),
                         QColor::fromRgbF(0.153364, 0.497000, 0.557724).rgb(),
                         QColor::fromRgbF(0.151918, 0.500685, 0.557587).rgb(),
                         QColor::fromRgbF(0.150476, 0.504369, 0.557430).rgb(),
                         QColor::fromRgbF(0.149039, 0.508051, 0.557250).rgb(),
                         QColor::fromRgbF(0.147607, 0.511733, 0.557049).rgb(),
                         QColor::fromRgbF(0.146180, 0.515413, 0.556823).rgb(),
                         QColor::fromRgbF(0.144759, 0.519093, 0.556572).rgb(),
                         QColor::fromRgbF(0.143343, 0.522773, 0.556295).rgb(),
                         QColor::fromRgbF(0.141935, 0.526453, 0.555991).rgb(),
                         QColor::fromRgbF(0.140536, 0.530132, 0.555659).rgb(),
                         QColor::fromRgbF(0.139147, 0.533812, 0.555298).rgb(),
                         QColor::fromRgbF(0.137770, 0.537492, 0.554906).rgb(),
                         QColor::fromRgbF(0.136408, 0.541173, 0.554483).rgb(),
                         QColor::fromRgbF(0.135066, 0.544853, 0.554029).rgb(),
                         QColor::fromRgbF(0.133743, 0.548535, 0.553541).rgb(),
                         QColor::fromRgbF(0.132444, 0.552216, 0.553018).rgb(),
                         QColor::fromRgbF(0.131172, 0.555899, 0.552459).rgb(),
                         QColor::fromRgbF(0.129933, 0.559582, 0.551864).rgb(),
                         QColor::fromRgbF(0.128729, 0.563265, 0.551229).rgb(),
                         QColor::fromRgbF(0.127568, 0.566949, 0.550556).rgb(),
                         QColor::fromRgbF(0.126453, 0.570633, 0.549841).rgb(),
                         QColor::fromRgbF(0.125394, 0.574318, 0.549086).rgb(),
                         QColor::fromRgbF(0.124395, 0.578002, 0.548287).rgb(),
                         QColor::fromRgbF(0.123463, 0.581687, 0.547445).rgb(),
                         QColor::fromRgbF(0.122606, 0.585371, 0.546557).rgb(),
                         QColor::fromRgbF(0.121831, 0.589055, 0.545623).rgb(),
                         QColor::fromRgbF(0.121148, 0.592739, 0.544641).rgb(),
                         QColor::fromRgbF(0.120565, 0.596422, 0.543611).rgb(),
                         QColor::fromRgbF(0.120092, 0.600104, 0.542530).rgb(),
                         QColor::fromRgbF(0.119738, 0.603785, 0.541400).rgb(),
                         QColor::fromRgbF(0.119512, 0.607464, 0.540218).rgb(),
                         QColor::fromRgbF(0.119423, 0.611141, 0.538982).rgb(),
                         QColor::fromRgbF(0.119483, 0.614817, 0.537692).rgb(),
                         QColor::fromRgbF(0.119699, 0.618490, 0.536347).rgb(),
                         QColor::fromRgbF(0.120081, 0.622161, 0.534946).rgb(),
                         QColor::fromRgbF(0.120638, 0.625828, 0.533488).rgb(),
                         QColor::fromRgbF(0.121380, 0.629492, 0.531973).rgb(),
                         QColor::fromRgbF(0.122312, 0.633153, 0.530398).rgb(),
                         QColor::fromRgbF(0.123444, 0.636809, 0.528763).rgb(),
                         QColor::fromRgbF(0.124780, 0.640461, 0.527068).rgb(),
                         QColor::fromRgbF(0.126326, 0.644107, 0.525311).rgb(),
                         QColor::fromRgbF(0.128087, 0.647749, 0.523491).rgb(),
                         QColor::fromRgbF(0.130067, 0.651384, 0.521608).rgb(),
                         QColor::fromRgbF(0.132268, 0.655014, 0.519661).rgb(),
                         QColor::fromRgbF(0.134692, 0.658636, 0.517649).rgb(),
                         QColor::fromRgbF(0.137339, 0.662252, 0.515571).rgb(),
                         QColor::fromRgbF(0.140210, 0.665859, 0.513427).rgb(),
                         QColor::fromRgbF(0.143303, 0.669459, 0.511215).rgb(),
                         QColor::fromRgbF(0.146616, 0.673050, 0.508936).rgb(),
                         QColor::fromRgbF(0.150148, 0.676631, 0.506589).rgb(),
                         QColor::fromRgbF(0.153894, 0.680203, 0.504172).rgb(),
                         QColor::fromRgbF(0.157851, 0.683765, 0.501686).rgb(),
                         QColor::fromRgbF(0.162016, 0.687316, 0.499129).rgb(),
                         QColor::fromRgbF(0.166383, 0.690856, 0.496502).rgb(),
                         QColor::fromRgbF(0.170948, 0.694384, 0.493803).rgb(),
                         QColor::fromRgbF(0.175707, 0.697900, 0.491033).rgb(),
                         QColor::fromRgbF(0.180653, 0.701402, 0.488189).rgb(),
                         QColor::fromRgbF(0.185783, 0.704891, 0.485273).rgb(),
                         QColor::fromRgbF(0.191090, 0.708366, 0.482284).rgb(),
                         QColor::fromRgbF(0.196571, 0.711827, 0.479221).rgb(),
                         QColor::fromRgbF(0.202219, 0.715272, 0.476084).rgb(),
                         QColor::fromRgbF(0.208030, 0.718701, 0.472873).rgb(),
                         QColor::fromRgbF(0.214000, 0.722114, 0.469588).rgb(),
                         QColor::fromRgbF(0.220124, 0.725509, 0.466226).rgb(),
                         QColor::fromRgbF(0.226397, 0.728888, 0.462789).rgb(),
                         QColor::fromRgbF(0.232815, 0.732247, 0.459277).rgb(),
                         QColor::fromRgbF(0.239374, 0.735588, 0.455688).rgb(),
                         QColor::fromRgbF(0.246070, 0.738910, 0.452024).rgb(),
                         QColor::fromRgbF(0.252899, 0.742211, 0.448284).rgb(),
                         QColor::fromRgbF(0.259857, 0.745492, 0.444467).rgb(),
                         QColor::fromRgbF(0.266941, 0.748751, 0.440573).rgb(),
                         QColor::fromRgbF(0.274149, 0.751988, 0.436601).rgb(),
                         QColor::fromRgbF(0.281477, 0.755203, 0.432552).rgb(),
                         QColor::fromRgbF(0.288921, 0.758394, 0.428426).rgb(),
                         QColor::fromRgbF(0.296479, 0.761561, 0.424223).rgb(),
                         QColor::fromRgbF(0.304148, 0.764704, 0.419943).rgb(),
                         QColor::fromRgbF(0.311925, 0.767822, 0.415586).rgb(),
                         QColor::fromRgbF(0.319809, 0.770914, 0.411152).rgb(),
                         QColor::fromRgbF(0.327796, 0.773980, 0.406640).rgb(),
                         QColor::fromRgbF(0.335885, 0.777018, 0.402049).rgb(),
                         QColor::fromRgbF(0.344074, 0.780029, 0.397381).rgb(),
                         QColor::fromRgbF(0.352360, 0.783011, 0.392636).rgb(),
                         QColor::fromRgbF(0.360741, 0.785964, 0.387814).rgb(),
                         QColor::fromRgbF(0.369214, 0.788888, 0.382914).rgb(),
                         QColor::fromRgbF(0.377779, 0.791781, 0.377939).rgb(),
                         QColor::fromRgbF(0.386433, 0.794644, 0.372886).rgb(),
                         QColor::fromRgbF(0.395174, 0.797475, 0.367757).rgb(),
                         QColor::fromRgbF(0.404001, 0.800275, 0.362552).rgb(),
                         QColor::fromRgbF(0.412913, 0.803041, 0.357269).rgb(),
                         QColor::fromRgbF(0.421908, 0.805774, 0.351910).rgb(),
                         QColor::fromRgbF(0.430983, 0.808473, 0.346476).rgb(),
                         QColor::fromRgbF(0.440137, 0.811138, 0.340967).rgb(),
                         QColor::fromRgbF(0.449368, 0.813768, 0.335384).rgb(),
                         QColor::fromRgbF(0.458674, 0.816363, 0.329727).rgb(),
                         QColor::fromRgbF(0.468053, 0.818921, 0.323998).rgb(),
                         QColor::fromRgbF(0.477504, 0.821444, 0.318195).rgb(),
                         QColor::fromRgbF(0.487026, 0.823929, 0.312321).rgb(),
                         QColor::fromRgbF(0.496615, 0.826376, 0.306377).rgb(),
                         QColor::fromRgbF(0.506271, 0.828786, 0.300362).rgb(),
                         QColor::fromRgbF(0.515992, 0.831158, 0.294279).rgb(),
                         QColor::fromRgbF(0.525776, 0.833491, 0.288127).rgb(),
                         QColor::fromRgbF(0.535621, 0.835785, 0.281908).rgb(),
                         QColor::fromRgbF(0.545524, 0.838039, 0.275626).rgb(),
                         QColor::fromRgbF(0.555484, 0.840254, 0.269281).rgb(),
                         QColor::fromRgbF(0.565498, 0.842430, 0.262877).rgb(),
                         QColor::fromRgbF(0.575563, 0.844566, 0.256415).rgb(),
                         QColor::fromRgbF(0.585678, 0.846661, 0.249897).rgb(),
                         QColor::fromRgbF(0.595839, 0.848717, 0.243329).rgb(),
                         QColor::fromRgbF(0.606045, 0.850733, 0.236712).rgb(),
                         QColor::fromRgbF(0.616293, 0.852709, 0.230052).rgb(),
                         QColor::fromRgbF(0.626579, 0.854645, 0.223353).rgb(),
                         QColor::fromRgbF(0.636902, 0.856542, 0.216620).rgb(),
                         QColor::fromRgbF(0.647257, 0.858400, 0.209861).rgb(),
                         QColor::fromRgbF(0.657642, 0.860219, 0.203082).rgb(),
                         QColor::fromRgbF(0.668054, 0.861999, 0.196293).rgb(),
                         QColor::fromRgbF(0.678489, 0.863742, 0.189503).rgb(),
                         QColor::fromRgbF(0.688944, 0.865448, 0.182725).rgb(),
                         QColor::fromRgbF(0.699415, 0.867117, 0.175971).rgb(),
                         QColor::fromRgbF(0.709898, 0.868751, 0.169257).rgb(),
                         QColor::fromRgbF(0.720391, 0.870350, 0.162603).rgb(),
                         QColor::fromRgbF(0.730889, 0.871916, 0.156029).rgb(),
                         QColor::fromRgbF(0.741388, 0.873449, 0.149561).rgb(),
                         QColor::fromRgbF(0.751884, 0.874951, 0.143228).rgb(),
                         QColor::fromRgbF(0.762373, 0.876424, 0.137064).rgb(),
                         QColor::fromRgbF(0.772852, 0.877868, 0.131109).rgb(),
                         QColor::fromRgbF(0.783315, 0.879285, 0.125405).rgb(),
                         QColor::fromRgbF(0.793760, 0.880678, 0.120005).rgb(),
                         QColor::fromRgbF(0.804182, 0.882046, 0.114965).rgb(),
                         QColor::fromRgbF(0.814576, 0.883393, 0.110347).rgb(),
                         QColor::fromRgbF(0.824940, 0.884720, 0.106217).rgb(),
                         QColor::fromRgbF(0.835270, 0.886029, 0.102646).rgb(),
                         QColor::fromRgbF(0.845561, 0.887322, 0.099702).rgb(),
                         QColor::fromRgbF(0.855810, 0.888601, 0.097452).rgb(),
                         QColor::fromRgbF(0.866013, 0.889868, 0.095953).rgb(),
                         QColor::fromRgbF(0.876168, 0.891125, 0.095250).rgb(),
                         QColor::fromRgbF(0.886271, 0.892374, 0.095374).rgb(),
                         QColor::fromRgbF(0.896320, 0.893616, 0.096335).rgb(),
                         QColor::fromRgbF(0.906311, 0.894855, 0.098125).rgb(),
                         QColor::fromRgbF(0.916242, 0.896091, 0.100717).rgb(),
                         QColor::fromRgbF(0.926106, 0.897330, 0.104071).rgb(),
                         QColor::fromRgbF(0.935904, 0.898570, 0.108131).rgb(),
                         QColor::fromRgbF(0.945636, 0.899815, 0.112838).rgb(),
                         QColor::fromRgbF(0.955300, 0.901065, 0.118128).rgb(),
                         QColor::fromRgbF(0.964894, 0.902323, 0.123941).rgb(),
                         QColor::fromRgbF(0.974417, 0.903590, 0.130215).rgb(),
                         QColor::fromRgbF(0.983868, 0.904867, 0.136897).rgb(),
                         QColor::fromRgbF(0.993248, 0.906157, 0.143936).rgb()};

    }

    {
        // from https://github.com/BIDS/colormap/blob/master/colormaps.py
        // https://github.com/BIDS/colormap/blob/master/colormaps.py
        auto palette=JKQTPMathImagePlasma;
        QString palN="plasma";
        QString palNT=QObject::tr("plamsa");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        lutstore[palette].lut=LUTType{
                        QColor::fromRgbF(0.050383, 0.029803, 0.527975).rgb(),
                        QColor::fromRgbF(0.063536, 0.028426, 0.533124).rgb(),
                        QColor::fromRgbF(0.075353, 0.027206, 0.538007).rgb(),
                        QColor::fromRgbF(0.086222, 0.026125, 0.542658).rgb(),
                        QColor::fromRgbF(0.096379, 0.025165, 0.547103).rgb(),
                        QColor::fromRgbF(0.105980, 0.024309, 0.551368).rgb(),
                        QColor::fromRgbF(0.115124, 0.023556, 0.555468).rgb(),
                        QColor::fromRgbF(0.123903, 0.022878, 0.559423).rgb(),
                        QColor::fromRgbF(0.132381, 0.022258, 0.563250).rgb(),
                        QColor::fromRgbF(0.140603, 0.021687, 0.566959).rgb(),
                        QColor::fromRgbF(0.148607, 0.021154, 0.570562).rgb(),
                        QColor::fromRgbF(0.156421, 0.020651, 0.574065).rgb(),
                        QColor::fromRgbF(0.164070, 0.020171, 0.577478).rgb(),
                        QColor::fromRgbF(0.171574, 0.019706, 0.580806).rgb(),
                        QColor::fromRgbF(0.178950, 0.019252, 0.584054).rgb(),
                        QColor::fromRgbF(0.186213, 0.018803, 0.587228).rgb(),
                        QColor::fromRgbF(0.193374, 0.018354, 0.590330).rgb(),
                        QColor::fromRgbF(0.200445, 0.017902, 0.593364).rgb(),
                        QColor::fromRgbF(0.207435, 0.017442, 0.596333).rgb(),
                        QColor::fromRgbF(0.214350, 0.016973, 0.599239).rgb(),
                        QColor::fromRgbF(0.221197, 0.016497, 0.602083).rgb(),
                        QColor::fromRgbF(0.227983, 0.016007, 0.604867).rgb(),
                        QColor::fromRgbF(0.234715, 0.015502, 0.607592).rgb(),
                        QColor::fromRgbF(0.241396, 0.014979, 0.610259).rgb(),
                        QColor::fromRgbF(0.248032, 0.014439, 0.612868).rgb(),
                        QColor::fromRgbF(0.254627, 0.013882, 0.615419).rgb(),
                        QColor::fromRgbF(0.261183, 0.013308, 0.617911).rgb(),
                        QColor::fromRgbF(0.267703, 0.012716, 0.620346).rgb(),
                        QColor::fromRgbF(0.274191, 0.012109, 0.622722).rgb(),
                        QColor::fromRgbF(0.280648, 0.011488, 0.625038).rgb(),
                        QColor::fromRgbF(0.287076, 0.010855, 0.627295).rgb(),
                        QColor::fromRgbF(0.293478, 0.010213, 0.629490).rgb(),
                        QColor::fromRgbF(0.299855, 0.009561, 0.631624).rgb(),
                        QColor::fromRgbF(0.306210, 0.008902, 0.633694).rgb(),
                        QColor::fromRgbF(0.312543, 0.008239, 0.635700).rgb(),
                        QColor::fromRgbF(0.318856, 0.007576, 0.637640).rgb(),
                        QColor::fromRgbF(0.325150, 0.006915, 0.639512).rgb(),
                        QColor::fromRgbF(0.331426, 0.006261, 0.641316).rgb(),
                        QColor::fromRgbF(0.337683, 0.005618, 0.643049).rgb(),
                        QColor::fromRgbF(0.343925, 0.004991, 0.644710).rgb(),
                        QColor::fromRgbF(0.350150, 0.004382, 0.646298).rgb(),
                        QColor::fromRgbF(0.356359, 0.003798, 0.647810).rgb(),
                        QColor::fromRgbF(0.362553, 0.003243, 0.649245).rgb(),
                        QColor::fromRgbF(0.368733, 0.002724, 0.650601).rgb(),
                        QColor::fromRgbF(0.374897, 0.002245, 0.651876).rgb(),
                        QColor::fromRgbF(0.381047, 0.001814, 0.653068).rgb(),
                        QColor::fromRgbF(0.387183, 0.001434, 0.654177).rgb(),
                        QColor::fromRgbF(0.393304, 0.001114, 0.655199).rgb(),
                        QColor::fromRgbF(0.399411, 0.000859, 0.656133).rgb(),
                        QColor::fromRgbF(0.405503, 0.000678, 0.656977).rgb(),
                        QColor::fromRgbF(0.411580, 0.000577, 0.657730).rgb(),
                        QColor::fromRgbF(0.417642, 0.000564, 0.658390).rgb(),
                        QColor::fromRgbF(0.423689, 0.000646, 0.658956).rgb(),
                        QColor::fromRgbF(0.429719, 0.000831, 0.659425).rgb(),
                        QColor::fromRgbF(0.435734, 0.001127, 0.659797).rgb(),
                        QColor::fromRgbF(0.441732, 0.001540, 0.660069).rgb(),
                        QColor::fromRgbF(0.447714, 0.002080, 0.660240).rgb(),
                        QColor::fromRgbF(0.453677, 0.002755, 0.660310).rgb(),
                        QColor::fromRgbF(0.459623, 0.003574, 0.660277).rgb(),
                        QColor::fromRgbF(0.465550, 0.004545, 0.660139).rgb(),
                        QColor::fromRgbF(0.471457, 0.005678, 0.659897).rgb(),
                        QColor::fromRgbF(0.477344, 0.006980, 0.659549).rgb(),
                        QColor::fromRgbF(0.483210, 0.008460, 0.659095).rgb(),
                        QColor::fromRgbF(0.489055, 0.010127, 0.658534).rgb(),
                        QColor::fromRgbF(0.494877, 0.011990, 0.657865).rgb(),
                        QColor::fromRgbF(0.500678, 0.014055, 0.657088).rgb(),
                        QColor::fromRgbF(0.506454, 0.016333, 0.656202).rgb(),
                        QColor::fromRgbF(0.512206, 0.018833, 0.655209).rgb(),
                        QColor::fromRgbF(0.517933, 0.021563, 0.654109).rgb(),
                        QColor::fromRgbF(0.523633, 0.024532, 0.652901).rgb(),
                        QColor::fromRgbF(0.529306, 0.027747, 0.651586).rgb(),
                        QColor::fromRgbF(0.534952, 0.031217, 0.650165).rgb(),
                        QColor::fromRgbF(0.540570, 0.034950, 0.648640).rgb(),
                        QColor::fromRgbF(0.546157, 0.038954, 0.647010).rgb(),
                        QColor::fromRgbF(0.551715, 0.043136, 0.645277).rgb(),
                        QColor::fromRgbF(0.557243, 0.047331, 0.643443).rgb(),
                        QColor::fromRgbF(0.562738, 0.051545, 0.641509).rgb(),
                        QColor::fromRgbF(0.568201, 0.055778, 0.639477).rgb(),
                        QColor::fromRgbF(0.573632, 0.060028, 0.637349).rgb(),
                        QColor::fromRgbF(0.579029, 0.064296, 0.635126).rgb(),
                        QColor::fromRgbF(0.584391, 0.068579, 0.632812).rgb(),
                        QColor::fromRgbF(0.589719, 0.072878, 0.630408).rgb(),
                        QColor::fromRgbF(0.595011, 0.077190, 0.627917).rgb(),
                        QColor::fromRgbF(0.600266, 0.081516, 0.625342).rgb(),
                        QColor::fromRgbF(0.605485, 0.085854, 0.622686).rgb(),
                        QColor::fromRgbF(0.610667, 0.090204, 0.619951).rgb(),
                        QColor::fromRgbF(0.615812, 0.094564, 0.617140).rgb(),
                        QColor::fromRgbF(0.620919, 0.098934, 0.614257).rgb(),
                        QColor::fromRgbF(0.625987, 0.103312, 0.611305).rgb(),
                        QColor::fromRgbF(0.631017, 0.107699, 0.608287).rgb(),
                        QColor::fromRgbF(0.636008, 0.112092, 0.605205).rgb(),
                        QColor::fromRgbF(0.640959, 0.116492, 0.602065).rgb(),
                        QColor::fromRgbF(0.645872, 0.120898, 0.598867).rgb(),
                        QColor::fromRgbF(0.650746, 0.125309, 0.595617).rgb(),
                        QColor::fromRgbF(0.655580, 0.129725, 0.592317).rgb(),
                        QColor::fromRgbF(0.660374, 0.134144, 0.588971).rgb(),
                        QColor::fromRgbF(0.665129, 0.138566, 0.585582).rgb(),
                        QColor::fromRgbF(0.669845, 0.142992, 0.582154).rgb(),
                        QColor::fromRgbF(0.674522, 0.147419, 0.578688).rgb(),
                        QColor::fromRgbF(0.679160, 0.151848, 0.575189).rgb(),
                        QColor::fromRgbF(0.683758, 0.156278, 0.571660).rgb(),
                        QColor::fromRgbF(0.688318, 0.160709, 0.568103).rgb(),
                        QColor::fromRgbF(0.692840, 0.165141, 0.564522).rgb(),
                        QColor::fromRgbF(0.697324, 0.169573, 0.560919).rgb(),
                        QColor::fromRgbF(0.701769, 0.174005, 0.557296).rgb(),
                        QColor::fromRgbF(0.706178, 0.178437, 0.553657).rgb(),
                        QColor::fromRgbF(0.710549, 0.182868, 0.550004).rgb(),
                        QColor::fromRgbF(0.714883, 0.187299, 0.546338).rgb(),
                        QColor::fromRgbF(0.719181, 0.191729, 0.542663).rgb(),
                        QColor::fromRgbF(0.723444, 0.196158, 0.538981).rgb(),
                        QColor::fromRgbF(0.727670, 0.200586, 0.535293).rgb(),
                        QColor::fromRgbF(0.731862, 0.205013, 0.531601).rgb(),
                        QColor::fromRgbF(0.736019, 0.209439, 0.527908).rgb(),
                        QColor::fromRgbF(0.740143, 0.213864, 0.524216).rgb(),
                        QColor::fromRgbF(0.744232, 0.218288, 0.520524).rgb(),
                        QColor::fromRgbF(0.748289, 0.222711, 0.516834).rgb(),
                        QColor::fromRgbF(0.752312, 0.227133, 0.513149).rgb(),
                        QColor::fromRgbF(0.756304, 0.231555, 0.509468).rgb(),
                        QColor::fromRgbF(0.760264, 0.235976, 0.505794).rgb(),
                        QColor::fromRgbF(0.764193, 0.240396, 0.502126).rgb(),
                        QColor::fromRgbF(0.768090, 0.244817, 0.498465).rgb(),
                        QColor::fromRgbF(0.771958, 0.249237, 0.494813).rgb(),
                        QColor::fromRgbF(0.775796, 0.253658, 0.491171).rgb(),
                        QColor::fromRgbF(0.779604, 0.258078, 0.487539).rgb(),
                        QColor::fromRgbF(0.783383, 0.262500, 0.483918).rgb(),
                        QColor::fromRgbF(0.787133, 0.266922, 0.480307).rgb(),
                        QColor::fromRgbF(0.790855, 0.271345, 0.476706).rgb(),
                        QColor::fromRgbF(0.794549, 0.275770, 0.473117).rgb(),
                        QColor::fromRgbF(0.798216, 0.280197, 0.469538).rgb(),
                        QColor::fromRgbF(0.801855, 0.284626, 0.465971).rgb(),
                        QColor::fromRgbF(0.805467, 0.289057, 0.462415).rgb(),
                        QColor::fromRgbF(0.809052, 0.293491, 0.458870).rgb(),
                        QColor::fromRgbF(0.812612, 0.297928, 0.455338).rgb(),
                        QColor::fromRgbF(0.816144, 0.302368, 0.451816).rgb(),
                        QColor::fromRgbF(0.819651, 0.306812, 0.448306).rgb(),
                        QColor::fromRgbF(0.823132, 0.311261, 0.444806).rgb(),
                        QColor::fromRgbF(0.826588, 0.315714, 0.441316).rgb(),
                        QColor::fromRgbF(0.830018, 0.320172, 0.437836).rgb(),
                        QColor::fromRgbF(0.833422, 0.324635, 0.434366).rgb(),
                        QColor::fromRgbF(0.836801, 0.329105, 0.430905).rgb(),
                        QColor::fromRgbF(0.840155, 0.333580, 0.427455).rgb(),
                        QColor::fromRgbF(0.843484, 0.338062, 0.424013).rgb(),
                        QColor::fromRgbF(0.846788, 0.342551, 0.420579).rgb(),
                        QColor::fromRgbF(0.850066, 0.347048, 0.417153).rgb(),
                        QColor::fromRgbF(0.853319, 0.351553, 0.413734).rgb(),
                        QColor::fromRgbF(0.856547, 0.356066, 0.410322).rgb(),
                        QColor::fromRgbF(0.859750, 0.360588, 0.406917).rgb(),
                        QColor::fromRgbF(0.862927, 0.365119, 0.403519).rgb(),
                        QColor::fromRgbF(0.866078, 0.369660, 0.400126).rgb(),
                        QColor::fromRgbF(0.869203, 0.374212, 0.396738).rgb(),
                        QColor::fromRgbF(0.872303, 0.378774, 0.393355).rgb(),
                        QColor::fromRgbF(0.875376, 0.383347, 0.389976).rgb(),
                        QColor::fromRgbF(0.878423, 0.387932, 0.386600).rgb(),
                        QColor::fromRgbF(0.881443, 0.392529, 0.383229).rgb(),
                        QColor::fromRgbF(0.884436, 0.397139, 0.379860).rgb(),
                        QColor::fromRgbF(0.887402, 0.401762, 0.376494).rgb(),
                        QColor::fromRgbF(0.890340, 0.406398, 0.373130).rgb(),
                        QColor::fromRgbF(0.893250, 0.411048, 0.369768).rgb(),
                        QColor::fromRgbF(0.896131, 0.415712, 0.366407).rgb(),
                        QColor::fromRgbF(0.898984, 0.420392, 0.363047).rgb(),
                        QColor::fromRgbF(0.901807, 0.425087, 0.359688).rgb(),
                        QColor::fromRgbF(0.904601, 0.429797, 0.356329).rgb(),
                        QColor::fromRgbF(0.907365, 0.434524, 0.352970).rgb(),
                        QColor::fromRgbF(0.910098, 0.439268, 0.349610).rgb(),
                        QColor::fromRgbF(0.912800, 0.444029, 0.346251).rgb(),
                        QColor::fromRgbF(0.915471, 0.448807, 0.342890).rgb(),
                        QColor::fromRgbF(0.918109, 0.453603, 0.339529).rgb(),
                        QColor::fromRgbF(0.920714, 0.458417, 0.336166).rgb(),
                        QColor::fromRgbF(0.923287, 0.463251, 0.332801).rgb(),
                        QColor::fromRgbF(0.925825, 0.468103, 0.329435).rgb(),
                        QColor::fromRgbF(0.928329, 0.472975, 0.326067).rgb(),
                        QColor::fromRgbF(0.930798, 0.477867, 0.322697).rgb(),
                        QColor::fromRgbF(0.933232, 0.482780, 0.319325).rgb(),
                        QColor::fromRgbF(0.935630, 0.487712, 0.315952).rgb(),
                        QColor::fromRgbF(0.937990, 0.492667, 0.312575).rgb(),
                        QColor::fromRgbF(0.940313, 0.497642, 0.309197).rgb(),
                        QColor::fromRgbF(0.942598, 0.502639, 0.305816).rgb(),
                        QColor::fromRgbF(0.944844, 0.507658, 0.302433).rgb(),
                        QColor::fromRgbF(0.947051, 0.512699, 0.299049).rgb(),
                        QColor::fromRgbF(0.949217, 0.517763, 0.295662).rgb(),
                        QColor::fromRgbF(0.951344, 0.522850, 0.292275).rgb(),
                        QColor::fromRgbF(0.953428, 0.527960, 0.288883).rgb(),
                        QColor::fromRgbF(0.955470, 0.533093, 0.285490).rgb(),
                        QColor::fromRgbF(0.957469, 0.538250, 0.282096).rgb(),
                        QColor::fromRgbF(0.959424, 0.543431, 0.278701).rgb(),
                        QColor::fromRgbF(0.961336, 0.548636, 0.275305).rgb(),
                        QColor::fromRgbF(0.963203, 0.553865, 0.271909).rgb(),
                        QColor::fromRgbF(0.965024, 0.559118, 0.268513).rgb(),
                        QColor::fromRgbF(0.966798, 0.564396, 0.265118).rgb(),
                        QColor::fromRgbF(0.968526, 0.569700, 0.261721).rgb(),
                        QColor::fromRgbF(0.970205, 0.575028, 0.258325).rgb(),
                        QColor::fromRgbF(0.971835, 0.580382, 0.254931).rgb(),
                        QColor::fromRgbF(0.973416, 0.585761, 0.251540).rgb(),
                        QColor::fromRgbF(0.974947, 0.591165, 0.248151).rgb(),
                        QColor::fromRgbF(0.976428, 0.596595, 0.244767).rgb(),
                        QColor::fromRgbF(0.977856, 0.602051, 0.241387).rgb(),
                        QColor::fromRgbF(0.979233, 0.607532, 0.238013).rgb(),
                        QColor::fromRgbF(0.980556, 0.613039, 0.234646).rgb(),
                        QColor::fromRgbF(0.981826, 0.618572, 0.231287).rgb(),
                        QColor::fromRgbF(0.983041, 0.624131, 0.227937).rgb(),
                        QColor::fromRgbF(0.984199, 0.629718, 0.224595).rgb(),
                        QColor::fromRgbF(0.985301, 0.635330, 0.221265).rgb(),
                        QColor::fromRgbF(0.986345, 0.640969, 0.217948).rgb(),
                        QColor::fromRgbF(0.987332, 0.646633, 0.214648).rgb(),
                        QColor::fromRgbF(0.988260, 0.652325, 0.211364).rgb(),
                        QColor::fromRgbF(0.989128, 0.658043, 0.208100).rgb(),
                        QColor::fromRgbF(0.989935, 0.663787, 0.204859).rgb(),
                        QColor::fromRgbF(0.990681, 0.669558, 0.201642).rgb(),
                        QColor::fromRgbF(0.991365, 0.675355, 0.198453).rgb(),
                        QColor::fromRgbF(0.991985, 0.681179, 0.195295).rgb(),
                        QColor::fromRgbF(0.992541, 0.687030, 0.192170).rgb(),
                        QColor::fromRgbF(0.993032, 0.692907, 0.189084).rgb(),
                        QColor::fromRgbF(0.993456, 0.698810, 0.186041).rgb(),
                        QColor::fromRgbF(0.993814, 0.704741, 0.183043).rgb(),
                        QColor::fromRgbF(0.994103, 0.710698, 0.180097).rgb(),
                        QColor::fromRgbF(0.994324, 0.716681, 0.177208).rgb(),
                        QColor::fromRgbF(0.994474, 0.722691, 0.174381).rgb(),
                        QColor::fromRgbF(0.994553, 0.728728, 0.171622).rgb(),
                        QColor::fromRgbF(0.994561, 0.734791, 0.168938).rgb(),
                        QColor::fromRgbF(0.994495, 0.740880, 0.166335).rgb(),
                        QColor::fromRgbF(0.994355, 0.746995, 0.163821).rgb(),
                        QColor::fromRgbF(0.994141, 0.753137, 0.161404).rgb(),
                        QColor::fromRgbF(0.993851, 0.759304, 0.159092).rgb(),
                        QColor::fromRgbF(0.993482, 0.765499, 0.156891).rgb(),
                        QColor::fromRgbF(0.993033, 0.771720, 0.154808).rgb(),
                        QColor::fromRgbF(0.992505, 0.777967, 0.152855).rgb(),
                        QColor::fromRgbF(0.991897, 0.784239, 0.151042).rgb(),
                        QColor::fromRgbF(0.991209, 0.790537, 0.149377).rgb(),
                        QColor::fromRgbF(0.990439, 0.796859, 0.147870).rgb(),
                        QColor::fromRgbF(0.989587, 0.803205, 0.146529).rgb(),
                        QColor::fromRgbF(0.988648, 0.809579, 0.145357).rgb(),
                        QColor::fromRgbF(0.987621, 0.815978, 0.144363).rgb(),
                        QColor::fromRgbF(0.986509, 0.822401, 0.143557).rgb(),
                        QColor::fromRgbF(0.985314, 0.828846, 0.142945).rgb(),
                        QColor::fromRgbF(0.984031, 0.835315, 0.142528).rgb(),
                        QColor::fromRgbF(0.982653, 0.841812, 0.142303).rgb(),
                        QColor::fromRgbF(0.981190, 0.848329, 0.142279).rgb(),
                        QColor::fromRgbF(0.979644, 0.854866, 0.142453).rgb(),
                        QColor::fromRgbF(0.977995, 0.861432, 0.142808).rgb(),
                        QColor::fromRgbF(0.976265, 0.868016, 0.143351).rgb(),
                        QColor::fromRgbF(0.974443, 0.874622, 0.144061).rgb(),
                        QColor::fromRgbF(0.972530, 0.881250, 0.144923).rgb(),
                        QColor::fromRgbF(0.970533, 0.887896, 0.145919).rgb(),
                        QColor::fromRgbF(0.968443, 0.894564, 0.147014).rgb(),
                        QColor::fromRgbF(0.966271, 0.901249, 0.148180).rgb(),
                        QColor::fromRgbF(0.964021, 0.907950, 0.149370).rgb(),
                        QColor::fromRgbF(0.961681, 0.914672, 0.150520).rgb(),
                        QColor::fromRgbF(0.959276, 0.921407, 0.151566).rgb(),
                        QColor::fromRgbF(0.956808, 0.928152, 0.152409).rgb(),
                        QColor::fromRgbF(0.954287, 0.934908, 0.152921).rgb(),
                        QColor::fromRgbF(0.951726, 0.941671, 0.152925).rgb(),
                        QColor::fromRgbF(0.949151, 0.948435, 0.152178).rgb(),
                        QColor::fromRgbF(0.946602, 0.955190, 0.150328).rgb(),
                        QColor::fromRgbF(0.944152, 0.961916, 0.146861).rgb(),
                        QColor::fromRgbF(0.941896, 0.968590, 0.140956).rgb(),
                        QColor::fromRgbF(0.940015, 0.975158, 0.131326).rgb()};

    }

        {
            // from https://github.com/BIDS/colormap/blob/master/colormaps.py
            // https://github.com/BIDS/colormap/blob/master/colormaps.py
            auto palette=JKQTPMathImageMagma;
            QString palN="magma";
            QString palNT=QObject::tr("magma");
            lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palette].lut=LUTType{
                        QColor::fromRgbF(0.001462, 0.000466, 0.013866).rgb(),
                        QColor::fromRgbF(0.002258, 0.001295, 0.018331).rgb(),
                        QColor::fromRgbF(0.003279, 0.002305, 0.023708).rgb(),
                        QColor::fromRgbF(0.004512, 0.003490, 0.029965).rgb(),
                        QColor::fromRgbF(0.005950, 0.004843, 0.037130).rgb(),
                        QColor::fromRgbF(0.007588, 0.006356, 0.044973).rgb(),
                        QColor::fromRgbF(0.009426, 0.008022, 0.052844).rgb(),
                        QColor::fromRgbF(0.011465, 0.009828, 0.060750).rgb(),
                        QColor::fromRgbF(0.013708, 0.011771, 0.068667).rgb(),
                        QColor::fromRgbF(0.016156, 0.013840, 0.076603).rgb(),
                        QColor::fromRgbF(0.018815, 0.016026, 0.084584).rgb(),
                        QColor::fromRgbF(0.021692, 0.018320, 0.092610).rgb(),
                        QColor::fromRgbF(0.024792, 0.020715, 0.100676).rgb(),
                        QColor::fromRgbF(0.028123, 0.023201, 0.108787).rgb(),
                        QColor::fromRgbF(0.031696, 0.025765, 0.116965).rgb(),
                        QColor::fromRgbF(0.035520, 0.028397, 0.125209).rgb(),
                        QColor::fromRgbF(0.039608, 0.031090, 0.133515).rgb(),
                        QColor::fromRgbF(0.043830, 0.033830, 0.141886).rgb(),
                        QColor::fromRgbF(0.048062, 0.036607, 0.150327).rgb(),
                        QColor::fromRgbF(0.052320, 0.039407, 0.158841).rgb(),
                        QColor::fromRgbF(0.056615, 0.042160, 0.167446).rgb(),
                        QColor::fromRgbF(0.060949, 0.044794, 0.176129).rgb(),
                        QColor::fromRgbF(0.065330, 0.047318, 0.184892).rgb(),
                        QColor::fromRgbF(0.069764, 0.049726, 0.193735).rgb(),
                        QColor::fromRgbF(0.074257, 0.052017, 0.202660).rgb(),
                        QColor::fromRgbF(0.078815, 0.054184, 0.211667).rgb(),
                        QColor::fromRgbF(0.083446, 0.056225, 0.220755).rgb(),
                        QColor::fromRgbF(0.088155, 0.058133, 0.229922).rgb(),
                        QColor::fromRgbF(0.092949, 0.059904, 0.239164).rgb(),
                        QColor::fromRgbF(0.097833, 0.061531, 0.248477).rgb(),
                        QColor::fromRgbF(0.102815, 0.063010, 0.257854).rgb(),
                        QColor::fromRgbF(0.107899, 0.064335, 0.267289).rgb(),
                        QColor::fromRgbF(0.113094, 0.065492, 0.276784).rgb(),
                        QColor::fromRgbF(0.118405, 0.066479, 0.286321).rgb(),
                        QColor::fromRgbF(0.123833, 0.067295, 0.295879).rgb(),
                        QColor::fromRgbF(0.129380, 0.067935, 0.305443).rgb(),
                        QColor::fromRgbF(0.135053, 0.068391, 0.315000).rgb(),
                        QColor::fromRgbF(0.140858, 0.068654, 0.324538).rgb(),
                        QColor::fromRgbF(0.146785, 0.068738, 0.334011).rgb(),
                        QColor::fromRgbF(0.152839, 0.068637, 0.343404).rgb(),
                        QColor::fromRgbF(0.159018, 0.068354, 0.352688).rgb(),
                        QColor::fromRgbF(0.165308, 0.067911, 0.361816).rgb(),
                        QColor::fromRgbF(0.171713, 0.067305, 0.370771).rgb(),
                        QColor::fromRgbF(0.178212, 0.066576, 0.379497).rgb(),
                        QColor::fromRgbF(0.184801, 0.065732, 0.387973).rgb(),
                        QColor::fromRgbF(0.191460, 0.064818, 0.396152).rgb(),
                        QColor::fromRgbF(0.198177, 0.063862, 0.404009).rgb(),
                        QColor::fromRgbF(0.204935, 0.062907, 0.411514).rgb(),
                        QColor::fromRgbF(0.211718, 0.061992, 0.418647).rgb(),
                        QColor::fromRgbF(0.218512, 0.061158, 0.425392).rgb(),
                        QColor::fromRgbF(0.225302, 0.060445, 0.431742).rgb(),
                        QColor::fromRgbF(0.232077, 0.059889, 0.437695).rgb(),
                        QColor::fromRgbF(0.238826, 0.059517, 0.443256).rgb(),
                        QColor::fromRgbF(0.245543, 0.059352, 0.448436).rgb(),
                        QColor::fromRgbF(0.252220, 0.059415, 0.453248).rgb(),
                        QColor::fromRgbF(0.258857, 0.059706, 0.457710).rgb(),
                        QColor::fromRgbF(0.265447, 0.060237, 0.461840).rgb(),
                        QColor::fromRgbF(0.271994, 0.060994, 0.465660).rgb(),
                        QColor::fromRgbF(0.278493, 0.061978, 0.469190).rgb(),
                        QColor::fromRgbF(0.284951, 0.063168, 0.472451).rgb(),
                        QColor::fromRgbF(0.291366, 0.064553, 0.475462).rgb(),
                        QColor::fromRgbF(0.297740, 0.066117, 0.478243).rgb(),
                        QColor::fromRgbF(0.304081, 0.067835, 0.480812).rgb(),
                        QColor::fromRgbF(0.310382, 0.069702, 0.483186).rgb(),
                        QColor::fromRgbF(0.316654, 0.071690, 0.485380).rgb(),
                        QColor::fromRgbF(0.322899, 0.073782, 0.487408).rgb(),
                        QColor::fromRgbF(0.329114, 0.075972, 0.489287).rgb(),
                        QColor::fromRgbF(0.335308, 0.078236, 0.491024).rgb(),
                        QColor::fromRgbF(0.341482, 0.080564, 0.492631).rgb(),
                        QColor::fromRgbF(0.347636, 0.082946, 0.494121).rgb(),
                        QColor::fromRgbF(0.353773, 0.085373, 0.495501).rgb(),
                        QColor::fromRgbF(0.359898, 0.087831, 0.496778).rgb(),
                        QColor::fromRgbF(0.366012, 0.090314, 0.497960).rgb(),
                        QColor::fromRgbF(0.372116, 0.092816, 0.499053).rgb(),
                        QColor::fromRgbF(0.378211, 0.095332, 0.500067).rgb(),
                        QColor::fromRgbF(0.384299, 0.097855, 0.501002).rgb(),
                        QColor::fromRgbF(0.390384, 0.100379, 0.501864).rgb(),
                        QColor::fromRgbF(0.396467, 0.102902, 0.502658).rgb(),
                        QColor::fromRgbF(0.402548, 0.105420, 0.503386).rgb(),
                        QColor::fromRgbF(0.408629, 0.107930, 0.504052).rgb(),
                        QColor::fromRgbF(0.414709, 0.110431, 0.504662).rgb(),
                        QColor::fromRgbF(0.420791, 0.112920, 0.505215).rgb(),
                        QColor::fromRgbF(0.426877, 0.115395, 0.505714).rgb(),
                        QColor::fromRgbF(0.432967, 0.117855, 0.506160).rgb(),
                        QColor::fromRgbF(0.439062, 0.120298, 0.506555).rgb(),
                        QColor::fromRgbF(0.445163, 0.122724, 0.506901).rgb(),
                        QColor::fromRgbF(0.451271, 0.125132, 0.507198).rgb(),
                        QColor::fromRgbF(0.457386, 0.127522, 0.507448).rgb(),
                        QColor::fromRgbF(0.463508, 0.129893, 0.507652).rgb(),
                        QColor::fromRgbF(0.469640, 0.132245, 0.507809).rgb(),
                        QColor::fromRgbF(0.475780, 0.134577, 0.507921).rgb(),
                        QColor::fromRgbF(0.481929, 0.136891, 0.507989).rgb(),
                        QColor::fromRgbF(0.488088, 0.139186, 0.508011).rgb(),
                        QColor::fromRgbF(0.494258, 0.141462, 0.507988).rgb(),
                        QColor::fromRgbF(0.500438, 0.143719, 0.507920).rgb(),
                        QColor::fromRgbF(0.506629, 0.145958, 0.507806).rgb(),
                        QColor::fromRgbF(0.512831, 0.148179, 0.507648).rgb(),
                        QColor::fromRgbF(0.519045, 0.150383, 0.507443).rgb(),
                        QColor::fromRgbF(0.525270, 0.152569, 0.507192).rgb(),
                        QColor::fromRgbF(0.531507, 0.154739, 0.506895).rgb(),
                        QColor::fromRgbF(0.537755, 0.156894, 0.506551).rgb(),
                        QColor::fromRgbF(0.544015, 0.159033, 0.506159).rgb(),
                        QColor::fromRgbF(0.550287, 0.161158, 0.505719).rgb(),
                        QColor::fromRgbF(0.556571, 0.163269, 0.505230).rgb(),
                        QColor::fromRgbF(0.562866, 0.165368, 0.504692).rgb(),
                        QColor::fromRgbF(0.569172, 0.167454, 0.504105).rgb(),
                        QColor::fromRgbF(0.575490, 0.169530, 0.503466).rgb(),
                        QColor::fromRgbF(0.581819, 0.171596, 0.502777).rgb(),
                        QColor::fromRgbF(0.588158, 0.173652, 0.502035).rgb(),
                        QColor::fromRgbF(0.594508, 0.175701, 0.501241).rgb(),
                        QColor::fromRgbF(0.600868, 0.177743, 0.500394).rgb(),
                        QColor::fromRgbF(0.607238, 0.179779, 0.499492).rgb(),
                        QColor::fromRgbF(0.613617, 0.181811, 0.498536).rgb(),
                        QColor::fromRgbF(0.620005, 0.183840, 0.497524).rgb(),
                        QColor::fromRgbF(0.626401, 0.185867, 0.496456).rgb(),
                        QColor::fromRgbF(0.632805, 0.187893, 0.495332).rgb(),
                        QColor::fromRgbF(0.639216, 0.189921, 0.494150).rgb(),
                        QColor::fromRgbF(0.645633, 0.191952, 0.492910).rgb(),
                        QColor::fromRgbF(0.652056, 0.193986, 0.491611).rgb(),
                        QColor::fromRgbF(0.658483, 0.196027, 0.490253).rgb(),
                        QColor::fromRgbF(0.664915, 0.198075, 0.488836).rgb(),
                        QColor::fromRgbF(0.671349, 0.200133, 0.487358).rgb(),
                        QColor::fromRgbF(0.677786, 0.202203, 0.485819).rgb(),
                        QColor::fromRgbF(0.684224, 0.204286, 0.484219).rgb(),
                        QColor::fromRgbF(0.690661, 0.206384, 0.482558).rgb(),
                        QColor::fromRgbF(0.697098, 0.208501, 0.480835).rgb(),
                        QColor::fromRgbF(0.703532, 0.210638, 0.479049).rgb(),
                        QColor::fromRgbF(0.709962, 0.212797, 0.477201).rgb(),
                        QColor::fromRgbF(0.716387, 0.214982, 0.475290).rgb(),
                        QColor::fromRgbF(0.722805, 0.217194, 0.473316).rgb(),
                        QColor::fromRgbF(0.729216, 0.219437, 0.471279).rgb(),
                        QColor::fromRgbF(0.735616, 0.221713, 0.469180).rgb(),
                        QColor::fromRgbF(0.742004, 0.224025, 0.467018).rgb(),
                        QColor::fromRgbF(0.748378, 0.226377, 0.464794).rgb(),
                        QColor::fromRgbF(0.754737, 0.228772, 0.462509).rgb(),
                        QColor::fromRgbF(0.761077, 0.231214, 0.460162).rgb(),
                        QColor::fromRgbF(0.767398, 0.233705, 0.457755).rgb(),
                        QColor::fromRgbF(0.773695, 0.236249, 0.455289).rgb(),
                        QColor::fromRgbF(0.779968, 0.238851, 0.452765).rgb(),
                        QColor::fromRgbF(0.786212, 0.241514, 0.450184).rgb(),
                        QColor::fromRgbF(0.792427, 0.244242, 0.447543).rgb(),
                        QColor::fromRgbF(0.798608, 0.247040, 0.444848).rgb(),
                        QColor::fromRgbF(0.804752, 0.249911, 0.442102).rgb(),
                        QColor::fromRgbF(0.810855, 0.252861, 0.439305).rgb(),
                        QColor::fromRgbF(0.816914, 0.255895, 0.436461).rgb(),
                        QColor::fromRgbF(0.822926, 0.259016, 0.433573).rgb(),
                        QColor::fromRgbF(0.828886, 0.262229, 0.430644).rgb(),
                        QColor::fromRgbF(0.834791, 0.265540, 0.427671).rgb(),
                        QColor::fromRgbF(0.840636, 0.268953, 0.424666).rgb(),
                        QColor::fromRgbF(0.846416, 0.272473, 0.421631).rgb(),
                        QColor::fromRgbF(0.852126, 0.276106, 0.418573).rgb(),
                        QColor::fromRgbF(0.857763, 0.279857, 0.415496).rgb(),
                        QColor::fromRgbF(0.863320, 0.283729, 0.412403).rgb(),
                        QColor::fromRgbF(0.868793, 0.287728, 0.409303).rgb(),
                        QColor::fromRgbF(0.874176, 0.291859, 0.406205).rgb(),
                        QColor::fromRgbF(0.879464, 0.296125, 0.403118).rgb(),
                        QColor::fromRgbF(0.884651, 0.300530, 0.400047).rgb(),
                        QColor::fromRgbF(0.889731, 0.305079, 0.397002).rgb(),
                        QColor::fromRgbF(0.894700, 0.309773, 0.393995).rgb(),
                        QColor::fromRgbF(0.899552, 0.314616, 0.391037).rgb(),
                        QColor::fromRgbF(0.904281, 0.319610, 0.388137).rgb(),
                        QColor::fromRgbF(0.908884, 0.324755, 0.385308).rgb(),
                        QColor::fromRgbF(0.913354, 0.330052, 0.382563).rgb(),
                        QColor::fromRgbF(0.917689, 0.335500, 0.379915).rgb(),
                        QColor::fromRgbF(0.921884, 0.341098, 0.377376).rgb(),
                        QColor::fromRgbF(0.925937, 0.346844, 0.374959).rgb(),
                        QColor::fromRgbF(0.929845, 0.352734, 0.372677).rgb(),
                        QColor::fromRgbF(0.933606, 0.358764, 0.370541).rgb(),
                        QColor::fromRgbF(0.937221, 0.364929, 0.368567).rgb(),
                        QColor::fromRgbF(0.940687, 0.371224, 0.366762).rgb(),
                        QColor::fromRgbF(0.944006, 0.377643, 0.365136).rgb(),
                        QColor::fromRgbF(0.947180, 0.384178, 0.363701).rgb(),
                        QColor::fromRgbF(0.950210, 0.390820, 0.362468).rgb(),
                        QColor::fromRgbF(0.953099, 0.397563, 0.361438).rgb(),
                        QColor::fromRgbF(0.955849, 0.404400, 0.360619).rgb(),
                        QColor::fromRgbF(0.958464, 0.411324, 0.360014).rgb(),
                        QColor::fromRgbF(0.960949, 0.418323, 0.359630).rgb(),
                        QColor::fromRgbF(0.963310, 0.425390, 0.359469).rgb(),
                        QColor::fromRgbF(0.965549, 0.432519, 0.359529).rgb(),
                        QColor::fromRgbF(0.967671, 0.439703, 0.359810).rgb(),
                        QColor::fromRgbF(0.969680, 0.446936, 0.360311).rgb(),
                        QColor::fromRgbF(0.971582, 0.454210, 0.361030).rgb(),
                        QColor::fromRgbF(0.973381, 0.461520, 0.361965).rgb(),
                        QColor::fromRgbF(0.975082, 0.468861, 0.363111).rgb(),
                        QColor::fromRgbF(0.976690, 0.476226, 0.364466).rgb(),
                        QColor::fromRgbF(0.978210, 0.483612, 0.366025).rgb(),
                        QColor::fromRgbF(0.979645, 0.491014, 0.367783).rgb(),
                        QColor::fromRgbF(0.981000, 0.498428, 0.369734).rgb(),
                        QColor::fromRgbF(0.982279, 0.505851, 0.371874).rgb(),
                        QColor::fromRgbF(0.983485, 0.513280, 0.374198).rgb(),
                        QColor::fromRgbF(0.984622, 0.520713, 0.376698).rgb(),
                        QColor::fromRgbF(0.985693, 0.528148, 0.379371).rgb(),
                        QColor::fromRgbF(0.986700, 0.535582, 0.382210).rgb(),
                        QColor::fromRgbF(0.987646, 0.543015, 0.385210).rgb(),
                        QColor::fromRgbF(0.988533, 0.550446, 0.388365).rgb(),
                        QColor::fromRgbF(0.989363, 0.557873, 0.391671).rgb(),
                        QColor::fromRgbF(0.990138, 0.565296, 0.395122).rgb(),
                        QColor::fromRgbF(0.990871, 0.572706, 0.398714).rgb(),
                        QColor::fromRgbF(0.991558, 0.580107, 0.402441).rgb(),
                        QColor::fromRgbF(0.992196, 0.587502, 0.406299).rgb(),
                        QColor::fromRgbF(0.992785, 0.594891, 0.410283).rgb(),
                        QColor::fromRgbF(0.993326, 0.602275, 0.414390).rgb(),
                        QColor::fromRgbF(0.993834, 0.609644, 0.418613).rgb(),
                        QColor::fromRgbF(0.994309, 0.616999, 0.422950).rgb(),
                        QColor::fromRgbF(0.994738, 0.624350, 0.427397).rgb(),
                        QColor::fromRgbF(0.995122, 0.631696, 0.431951).rgb(),
                        QColor::fromRgbF(0.995480, 0.639027, 0.436607).rgb(),
                        QColor::fromRgbF(0.995810, 0.646344, 0.441361).rgb(),
                        QColor::fromRgbF(0.996096, 0.653659, 0.446213).rgb(),
                        QColor::fromRgbF(0.996341, 0.660969, 0.451160).rgb(),
                        QColor::fromRgbF(0.996580, 0.668256, 0.456192).rgb(),
                        QColor::fromRgbF(0.996775, 0.675541, 0.461314).rgb(),
                        QColor::fromRgbF(0.996925, 0.682828, 0.466526).rgb(),
                        QColor::fromRgbF(0.997077, 0.690088, 0.471811).rgb(),
                        QColor::fromRgbF(0.997186, 0.697349, 0.477182).rgb(),
                        QColor::fromRgbF(0.997254, 0.704611, 0.482635).rgb(),
                        QColor::fromRgbF(0.997325, 0.711848, 0.488154).rgb(),
                        QColor::fromRgbF(0.997351, 0.719089, 0.493755).rgb(),
                        QColor::fromRgbF(0.997351, 0.726324, 0.499428).rgb(),
                        QColor::fromRgbF(0.997341, 0.733545, 0.505167).rgb(),
                        QColor::fromRgbF(0.997285, 0.740772, 0.510983).rgb(),
                        QColor::fromRgbF(0.997228, 0.747981, 0.516859).rgb(),
                        QColor::fromRgbF(0.997138, 0.755190, 0.522806).rgb(),
                        QColor::fromRgbF(0.997019, 0.762398, 0.528821).rgb(),
                        QColor::fromRgbF(0.996898, 0.769591, 0.534892).rgb(),
                        QColor::fromRgbF(0.996727, 0.776795, 0.541039).rgb(),
                        QColor::fromRgbF(0.996571, 0.783977, 0.547233).rgb(),
                        QColor::fromRgbF(0.996369, 0.791167, 0.553499).rgb(),
                        QColor::fromRgbF(0.996162, 0.798348, 0.559820).rgb(),
                        QColor::fromRgbF(0.995932, 0.805527, 0.566202).rgb(),
                        QColor::fromRgbF(0.995680, 0.812706, 0.572645).rgb(),
                        QColor::fromRgbF(0.995424, 0.819875, 0.579140).rgb(),
                        QColor::fromRgbF(0.995131, 0.827052, 0.585701).rgb(),
                        QColor::fromRgbF(0.994851, 0.834213, 0.592307).rgb(),
                        QColor::fromRgbF(0.994524, 0.841387, 0.598983).rgb(),
                        QColor::fromRgbF(0.994222, 0.848540, 0.605696).rgb(),
                        QColor::fromRgbF(0.993866, 0.855711, 0.612482).rgb(),
                        QColor::fromRgbF(0.993545, 0.862859, 0.619299).rgb(),
                        QColor::fromRgbF(0.993170, 0.870024, 0.626189).rgb(),
                        QColor::fromRgbF(0.992831, 0.877168, 0.633109).rgb(),
                        QColor::fromRgbF(0.992440, 0.884330, 0.640099).rgb(),
                        QColor::fromRgbF(0.992089, 0.891470, 0.647116).rgb(),
                        QColor::fromRgbF(0.991688, 0.898627, 0.654202).rgb(),
                        QColor::fromRgbF(0.991332, 0.905763, 0.661309).rgb(),
                        QColor::fromRgbF(0.990930, 0.912915, 0.668481).rgb(),
                        QColor::fromRgbF(0.990570, 0.920049, 0.675675).rgb(),
                        QColor::fromRgbF(0.990175, 0.927196, 0.682926).rgb(),
                        QColor::fromRgbF(0.989815, 0.934329, 0.690198).rgb(),
                        QColor::fromRgbF(0.989434, 0.941470, 0.697519).rgb(),
                        QColor::fromRgbF(0.989077, 0.948604, 0.704863).rgb(),
                        QColor::fromRgbF(0.988717, 0.955742, 0.712242).rgb(),
                        QColor::fromRgbF(0.988367, 0.962878, 0.719649).rgb(),
                        QColor::fromRgbF(0.988033, 0.970012, 0.727077).rgb(),
                        QColor::fromRgbF(0.987691, 0.977154, 0.734536).rgb(),
                        QColor::fromRgbF(0.987387, 0.984288, 0.742002).rgb(),
                        QColor::fromRgbF(0.987053, 0.991438, 0.749504).rgb()};

        }

        {
            // from https://github.com/BIDS/colormap/blob/master/colormaps.py
            // https://github.com/BIDS/colormap/blob/master/colormaps.py
            auto palette=JKQTPMathImageInferno;
            QString palN="inferno";
            QString palNT=QObject::tr("inferno");
            lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palette].lut=LUTType{
                        QColor::fromRgbF(0.001462, 0.000466, 0.013866).rgb(),
                        QColor::fromRgbF(0.002267, 0.001270, 0.018570).rgb(),
                        QColor::fromRgbF(0.003299, 0.002249, 0.024239).rgb(),
                        QColor::fromRgbF(0.004547, 0.003392, 0.030909).rgb(),
                        QColor::fromRgbF(0.006006, 0.004692, 0.038558).rgb(),
                        QColor::fromRgbF(0.007676, 0.006136, 0.046836).rgb(),
                        QColor::fromRgbF(0.009561, 0.007713, 0.055143).rgb(),
                        QColor::fromRgbF(0.011663, 0.009417, 0.063460).rgb(),
                        QColor::fromRgbF(0.013995, 0.011225, 0.071862).rgb(),
                        QColor::fromRgbF(0.016561, 0.013136, 0.080282).rgb(),
                        QColor::fromRgbF(0.019373, 0.015133, 0.088767).rgb(),
                        QColor::fromRgbF(0.022447, 0.017199, 0.097327).rgb(),
                        QColor::fromRgbF(0.025793, 0.019331, 0.105930).rgb(),
                        QColor::fromRgbF(0.029432, 0.021503, 0.114621).rgb(),
                        QColor::fromRgbF(0.033385, 0.023702, 0.123397).rgb(),
                        QColor::fromRgbF(0.037668, 0.025921, 0.132232).rgb(),
                        QColor::fromRgbF(0.042253, 0.028139, 0.141141).rgb(),
                        QColor::fromRgbF(0.046915, 0.030324, 0.150164).rgb(),
                        QColor::fromRgbF(0.051644, 0.032474, 0.159254).rgb(),
                        QColor::fromRgbF(0.056449, 0.034569, 0.168414).rgb(),
                        QColor::fromRgbF(0.061340, 0.036590, 0.177642).rgb(),
                        QColor::fromRgbF(0.066331, 0.038504, 0.186962).rgb(),
                        QColor::fromRgbF(0.071429, 0.040294, 0.196354).rgb(),
                        QColor::fromRgbF(0.076637, 0.041905, 0.205799).rgb(),
                        QColor::fromRgbF(0.081962, 0.043328, 0.215289).rgb(),
                        QColor::fromRgbF(0.087411, 0.044556, 0.224813).rgb(),
                        QColor::fromRgbF(0.092990, 0.045583, 0.234358).rgb(),
                        QColor::fromRgbF(0.098702, 0.046402, 0.243904).rgb(),
                        QColor::fromRgbF(0.104551, 0.047008, 0.253430).rgb(),
                        QColor::fromRgbF(0.110536, 0.047399, 0.262912).rgb(),
                        QColor::fromRgbF(0.116656, 0.047574, 0.272321).rgb(),
                        QColor::fromRgbF(0.122908, 0.047536, 0.281624).rgb(),
                        QColor::fromRgbF(0.129285, 0.047293, 0.290788).rgb(),
                        QColor::fromRgbF(0.135778, 0.046856, 0.299776).rgb(),
                        QColor::fromRgbF(0.142378, 0.046242, 0.308553).rgb(),
                        QColor::fromRgbF(0.149073, 0.045468, 0.317085).rgb(),
                        QColor::fromRgbF(0.155850, 0.044559, 0.325338).rgb(),
                        QColor::fromRgbF(0.162689, 0.043554, 0.333277).rgb(),
                        QColor::fromRgbF(0.169575, 0.042489, 0.340874).rgb(),
                        QColor::fromRgbF(0.176493, 0.041402, 0.348111).rgb(),
                        QColor::fromRgbF(0.183429, 0.040329, 0.354971).rgb(),
                        QColor::fromRgbF(0.190367, 0.039309, 0.361447).rgb(),
                        QColor::fromRgbF(0.197297, 0.038400, 0.367535).rgb(),
                        QColor::fromRgbF(0.204209, 0.037632, 0.373238).rgb(),
                        QColor::fromRgbF(0.211095, 0.037030, 0.378563).rgb(),
                        QColor::fromRgbF(0.217949, 0.036615, 0.383522).rgb(),
                        QColor::fromRgbF(0.224763, 0.036405, 0.388129).rgb(),
                        QColor::fromRgbF(0.231538, 0.036405, 0.392400).rgb(),
                        QColor::fromRgbF(0.238273, 0.036621, 0.396353).rgb(),
                        QColor::fromRgbF(0.244967, 0.037055, 0.400007).rgb(),
                        QColor::fromRgbF(0.251620, 0.037705, 0.403378).rgb(),
                        QColor::fromRgbF(0.258234, 0.038571, 0.406485).rgb(),
                        QColor::fromRgbF(0.264810, 0.039647, 0.409345).rgb(),
                        QColor::fromRgbF(0.271347, 0.040922, 0.411976).rgb(),
                        QColor::fromRgbF(0.277850, 0.042353, 0.414392).rgb(),
                        QColor::fromRgbF(0.284321, 0.043933, 0.416608).rgb(),
                        QColor::fromRgbF(0.290763, 0.045644, 0.418637).rgb(),
                        QColor::fromRgbF(0.297178, 0.047470, 0.420491).rgb(),
                        QColor::fromRgbF(0.303568, 0.049396, 0.422182).rgb(),
                        QColor::fromRgbF(0.309935, 0.051407, 0.423721).rgb(),
                        QColor::fromRgbF(0.316282, 0.053490, 0.425116).rgb(),
                        QColor::fromRgbF(0.322610, 0.055634, 0.426377).rgb(),
                        QColor::fromRgbF(0.328921, 0.057827, 0.427511).rgb(),
                        QColor::fromRgbF(0.335217, 0.060060, 0.428524).rgb(),
                        QColor::fromRgbF(0.341500, 0.062325, 0.429425).rgb(),
                        QColor::fromRgbF(0.347771, 0.064616, 0.430217).rgb(),
                        QColor::fromRgbF(0.354032, 0.066925, 0.430906).rgb(),
                        QColor::fromRgbF(0.360284, 0.069247, 0.431497).rgb(),
                        QColor::fromRgbF(0.366529, 0.071579, 0.431994).rgb(),
                        QColor::fromRgbF(0.372768, 0.073915, 0.432400).rgb(),
                        QColor::fromRgbF(0.379001, 0.076253, 0.432719).rgb(),
                        QColor::fromRgbF(0.385228, 0.078591, 0.432955).rgb(),
                        QColor::fromRgbF(0.391453, 0.080927, 0.433109).rgb(),
                        QColor::fromRgbF(0.397674, 0.083257, 0.433183).rgb(),
                        QColor::fromRgbF(0.403894, 0.085580, 0.433179).rgb(),
                        QColor::fromRgbF(0.410113, 0.087896, 0.433098).rgb(),
                        QColor::fromRgbF(0.416331, 0.090203, 0.432943).rgb(),
                        QColor::fromRgbF(0.422549, 0.092501, 0.432714).rgb(),
                        QColor::fromRgbF(0.428768, 0.094790, 0.432412).rgb(),
                        QColor::fromRgbF(0.434987, 0.097069, 0.432039).rgb(),
                        QColor::fromRgbF(0.441207, 0.099338, 0.431594).rgb(),
                        QColor::fromRgbF(0.447428, 0.101597, 0.431080).rgb(),
                        QColor::fromRgbF(0.453651, 0.103848, 0.430498).rgb(),
                        QColor::fromRgbF(0.459875, 0.106089, 0.429846).rgb(),
                        QColor::fromRgbF(0.466100, 0.108322, 0.429125).rgb(),
                        QColor::fromRgbF(0.472328, 0.110547, 0.428334).rgb(),
                        QColor::fromRgbF(0.478558, 0.112764, 0.427475).rgb(),
                        QColor::fromRgbF(0.484789, 0.114974, 0.426548).rgb(),
                        QColor::fromRgbF(0.491022, 0.117179, 0.425552).rgb(),
                        QColor::fromRgbF(0.497257, 0.119379, 0.424488).rgb(),
                        QColor::fromRgbF(0.503493, 0.121575, 0.423356).rgb(),
                        QColor::fromRgbF(0.509730, 0.123769, 0.422156).rgb(),
                        QColor::fromRgbF(0.515967, 0.125960, 0.420887).rgb(),
                        QColor::fromRgbF(0.522206, 0.128150, 0.419549).rgb(),
                        QColor::fromRgbF(0.528444, 0.130341, 0.418142).rgb(),
                        QColor::fromRgbF(0.534683, 0.132534, 0.416667).rgb(),
                        QColor::fromRgbF(0.540920, 0.134729, 0.415123).rgb(),
                        QColor::fromRgbF(0.547157, 0.136929, 0.413511).rgb(),
                        QColor::fromRgbF(0.553392, 0.139134, 0.411829).rgb(),
                        QColor::fromRgbF(0.559624, 0.141346, 0.410078).rgb(),
                        QColor::fromRgbF(0.565854, 0.143567, 0.408258).rgb(),
                        QColor::fromRgbF(0.572081, 0.145797, 0.406369).rgb(),
                        QColor::fromRgbF(0.578304, 0.148039, 0.404411).rgb(),
                        QColor::fromRgbF(0.584521, 0.150294, 0.402385).rgb(),
                        QColor::fromRgbF(0.590734, 0.152563, 0.400290).rgb(),
                        QColor::fromRgbF(0.596940, 0.154848, 0.398125).rgb(),
                        QColor::fromRgbF(0.603139, 0.157151, 0.395891).rgb(),
                        QColor::fromRgbF(0.609330, 0.159474, 0.393589).rgb(),
                        QColor::fromRgbF(0.615513, 0.161817, 0.391219).rgb(),
                        QColor::fromRgbF(0.621685, 0.164184, 0.388781).rgb(),
                        QColor::fromRgbF(0.627847, 0.166575, 0.386276).rgb(),
                        QColor::fromRgbF(0.633998, 0.168992, 0.383704).rgb(),
                        QColor::fromRgbF(0.640135, 0.171438, 0.381065).rgb(),
                        QColor::fromRgbF(0.646260, 0.173914, 0.378359).rgb(),
                        QColor::fromRgbF(0.652369, 0.176421, 0.375586).rgb(),
                        QColor::fromRgbF(0.658463, 0.178962, 0.372748).rgb(),
                        QColor::fromRgbF(0.664540, 0.181539, 0.369846).rgb(),
                        QColor::fromRgbF(0.670599, 0.184153, 0.366879).rgb(),
                        QColor::fromRgbF(0.676638, 0.186807, 0.363849).rgb(),
                        QColor::fromRgbF(0.682656, 0.189501, 0.360757).rgb(),
                        QColor::fromRgbF(0.688653, 0.192239, 0.357603).rgb(),
                        QColor::fromRgbF(0.694627, 0.195021, 0.354388).rgb(),
                        QColor::fromRgbF(0.700576, 0.197851, 0.351113).rgb(),
                        QColor::fromRgbF(0.706500, 0.200728, 0.347777).rgb(),
                        QColor::fromRgbF(0.712396, 0.203656, 0.344383).rgb(),
                        QColor::fromRgbF(0.718264, 0.206636, 0.340931).rgb(),
                        QColor::fromRgbF(0.724103, 0.209670, 0.337424).rgb(),
                        QColor::fromRgbF(0.729909, 0.212759, 0.333861).rgb(),
                        QColor::fromRgbF(0.735683, 0.215906, 0.330245).rgb(),
                        QColor::fromRgbF(0.741423, 0.219112, 0.326576).rgb(),
                        QColor::fromRgbF(0.747127, 0.222378, 0.322856).rgb(),
                        QColor::fromRgbF(0.752794, 0.225706, 0.319085).rgb(),
                        QColor::fromRgbF(0.758422, 0.229097, 0.315266).rgb(),
                        QColor::fromRgbF(0.764010, 0.232554, 0.311399).rgb(),
                        QColor::fromRgbF(0.769556, 0.236077, 0.307485).rgb(),
                        QColor::fromRgbF(0.775059, 0.239667, 0.303526).rgb(),
                        QColor::fromRgbF(0.780517, 0.243327, 0.299523).rgb(),
                        QColor::fromRgbF(0.785929, 0.247056, 0.295477).rgb(),
                        QColor::fromRgbF(0.791293, 0.250856, 0.291390).rgb(),
                        QColor::fromRgbF(0.796607, 0.254728, 0.287264).rgb(),
                        QColor::fromRgbF(0.801871, 0.258674, 0.283099).rgb(),
                        QColor::fromRgbF(0.807082, 0.262692, 0.278898).rgb(),
                        QColor::fromRgbF(0.812239, 0.266786, 0.274661).rgb(),
                        QColor::fromRgbF(0.817341, 0.270954, 0.270390).rgb(),
                        QColor::fromRgbF(0.822386, 0.275197, 0.266085).rgb(),
                        QColor::fromRgbF(0.827372, 0.279517, 0.261750).rgb(),
                        QColor::fromRgbF(0.832299, 0.283913, 0.257383).rgb(),
                        QColor::fromRgbF(0.837165, 0.288385, 0.252988).rgb(),
                        QColor::fromRgbF(0.841969, 0.292933, 0.248564).rgb(),
                        QColor::fromRgbF(0.846709, 0.297559, 0.244113).rgb(),
                        QColor::fromRgbF(0.851384, 0.302260, 0.239636).rgb(),
                        QColor::fromRgbF(0.855992, 0.307038, 0.235133).rgb(),
                        QColor::fromRgbF(0.860533, 0.311892, 0.230606).rgb(),
                        QColor::fromRgbF(0.865006, 0.316822, 0.226055).rgb(),
                        QColor::fromRgbF(0.869409, 0.321827, 0.221482).rgb(),
                        QColor::fromRgbF(0.873741, 0.326906, 0.216886).rgb(),
                        QColor::fromRgbF(0.878001, 0.332060, 0.212268).rgb(),
                        QColor::fromRgbF(0.882188, 0.337287, 0.207628).rgb(),
                        QColor::fromRgbF(0.886302, 0.342586, 0.202968).rgb(),
                        QColor::fromRgbF(0.890341, 0.347957, 0.198286).rgb(),
                        QColor::fromRgbF(0.894305, 0.353399, 0.193584).rgb(),
                        QColor::fromRgbF(0.898192, 0.358911, 0.188860).rgb(),
                        QColor::fromRgbF(0.902003, 0.364492, 0.184116).rgb(),
                        QColor::fromRgbF(0.905735, 0.370140, 0.179350).rgb(),
                        QColor::fromRgbF(0.909390, 0.375856, 0.174563).rgb(),
                        QColor::fromRgbF(0.912966, 0.381636, 0.169755).rgb(),
                        QColor::fromRgbF(0.916462, 0.387481, 0.164924).rgb(),
                        QColor::fromRgbF(0.919879, 0.393389, 0.160070).rgb(),
                        QColor::fromRgbF(0.923215, 0.399359, 0.155193).rgb(),
                        QColor::fromRgbF(0.926470, 0.405389, 0.150292).rgb(),
                        QColor::fromRgbF(0.929644, 0.411479, 0.145367).rgb(),
                        QColor::fromRgbF(0.932737, 0.417627, 0.140417).rgb(),
                        QColor::fromRgbF(0.935747, 0.423831, 0.135440).rgb(),
                        QColor::fromRgbF(0.938675, 0.430091, 0.130438).rgb(),
                        QColor::fromRgbF(0.941521, 0.436405, 0.125409).rgb(),
                        QColor::fromRgbF(0.944285, 0.442772, 0.120354).rgb(),
                        QColor::fromRgbF(0.946965, 0.449191, 0.115272).rgb(),
                        QColor::fromRgbF(0.949562, 0.455660, 0.110164).rgb(),
                        QColor::fromRgbF(0.952075, 0.462178, 0.105031).rgb(),
                        QColor::fromRgbF(0.954506, 0.468744, 0.099874).rgb(),
                        QColor::fromRgbF(0.956852, 0.475356, 0.094695).rgb(),
                        QColor::fromRgbF(0.959114, 0.482014, 0.089499).rgb(),
                        QColor::fromRgbF(0.961293, 0.488716, 0.084289).rgb(),
                        QColor::fromRgbF(0.963387, 0.495462, 0.079073).rgb(),
                        QColor::fromRgbF(0.965397, 0.502249, 0.073859).rgb(),
                        QColor::fromRgbF(0.967322, 0.509078, 0.068659).rgb(),
                        QColor::fromRgbF(0.969163, 0.515946, 0.063488).rgb(),
                        QColor::fromRgbF(0.970919, 0.522853, 0.058367).rgb(),
                        QColor::fromRgbF(0.972590, 0.529798, 0.053324).rgb(),
                        QColor::fromRgbF(0.974176, 0.536780, 0.048392).rgb(),
                        QColor::fromRgbF(0.975677, 0.543798, 0.043618).rgb(),
                        QColor::fromRgbF(0.977092, 0.550850, 0.039050).rgb(),
                        QColor::fromRgbF(0.978422, 0.557937, 0.034931).rgb(),
                        QColor::fromRgbF(0.979666, 0.565057, 0.031409).rgb(),
                        QColor::fromRgbF(0.980824, 0.572209, 0.028508).rgb(),
                        QColor::fromRgbF(0.981895, 0.579392, 0.026250).rgb(),
                        QColor::fromRgbF(0.982881, 0.586606, 0.024661).rgb(),
                        QColor::fromRgbF(0.983779, 0.593849, 0.023770).rgb(),
                        QColor::fromRgbF(0.984591, 0.601122, 0.023606).rgb(),
                        QColor::fromRgbF(0.985315, 0.608422, 0.024202).rgb(),
                        QColor::fromRgbF(0.985952, 0.615750, 0.025592).rgb(),
                        QColor::fromRgbF(0.986502, 0.623105, 0.027814).rgb(),
                        QColor::fromRgbF(0.986964, 0.630485, 0.030908).rgb(),
                        QColor::fromRgbF(0.987337, 0.637890, 0.034916).rgb(),
                        QColor::fromRgbF(0.987622, 0.645320, 0.039886).rgb(),
                        QColor::fromRgbF(0.987819, 0.652773, 0.045581).rgb(),
                        QColor::fromRgbF(0.987926, 0.660250, 0.051750).rgb(),
                        QColor::fromRgbF(0.987945, 0.667748, 0.058329).rgb(),
                        QColor::fromRgbF(0.987874, 0.675267, 0.065257).rgb(),
                        QColor::fromRgbF(0.987714, 0.682807, 0.072489).rgb(),
                        QColor::fromRgbF(0.987464, 0.690366, 0.079990).rgb(),
                        QColor::fromRgbF(0.987124, 0.697944, 0.087731).rgb(),
                        QColor::fromRgbF(0.986694, 0.705540, 0.095694).rgb(),
                        QColor::fromRgbF(0.986175, 0.713153, 0.103863).rgb(),
                        QColor::fromRgbF(0.985566, 0.720782, 0.112229).rgb(),
                        QColor::fromRgbF(0.984865, 0.728427, 0.120785).rgb(),
                        QColor::fromRgbF(0.984075, 0.736087, 0.129527).rgb(),
                        QColor::fromRgbF(0.983196, 0.743758, 0.138453).rgb(),
                        QColor::fromRgbF(0.982228, 0.751442, 0.147565).rgb(),
                        QColor::fromRgbF(0.981173, 0.759135, 0.156863).rgb(),
                        QColor::fromRgbF(0.980032, 0.766837, 0.166353).rgb(),
                        QColor::fromRgbF(0.978806, 0.774545, 0.176037).rgb(),
                        QColor::fromRgbF(0.977497, 0.782258, 0.185923).rgb(),
                        QColor::fromRgbF(0.976108, 0.789974, 0.196018).rgb(),
                        QColor::fromRgbF(0.974638, 0.797692, 0.206332).rgb(),
                        QColor::fromRgbF(0.973088, 0.805409, 0.216877).rgb(),
                        QColor::fromRgbF(0.971468, 0.813122, 0.227658).rgb(),
                        QColor::fromRgbF(0.969783, 0.820825, 0.238686).rgb(),
                        QColor::fromRgbF(0.968041, 0.828515, 0.249972).rgb(),
                        QColor::fromRgbF(0.966243, 0.836191, 0.261534).rgb(),
                        QColor::fromRgbF(0.964394, 0.843848, 0.273391).rgb(),
                        QColor::fromRgbF(0.962517, 0.851476, 0.285546).rgb(),
                        QColor::fromRgbF(0.960626, 0.859069, 0.298010).rgb(),
                        QColor::fromRgbF(0.958720, 0.866624, 0.310820).rgb(),
                        QColor::fromRgbF(0.956834, 0.874129, 0.323974).rgb(),
                        QColor::fromRgbF(0.954997, 0.881569, 0.337475).rgb(),
                        QColor::fromRgbF(0.953215, 0.888942, 0.351369).rgb(),
                        QColor::fromRgbF(0.951546, 0.896226, 0.365627).rgb(),
                        QColor::fromRgbF(0.950018, 0.903409, 0.380271).rgb(),
                        QColor::fromRgbF(0.948683, 0.910473, 0.395289).rgb(),
                        QColor::fromRgbF(0.947594, 0.917399, 0.410665).rgb(),
                        QColor::fromRgbF(0.946809, 0.924168, 0.426373).rgb(),
                        QColor::fromRgbF(0.946392, 0.930761, 0.442367).rgb(),
                        QColor::fromRgbF(0.946403, 0.937159, 0.458592).rgb(),
                        QColor::fromRgbF(0.946903, 0.943348, 0.474970).rgb(),
                        QColor::fromRgbF(0.947937, 0.949318, 0.491426).rgb(),
                        QColor::fromRgbF(0.949545, 0.955063, 0.507860).rgb(),
                        QColor::fromRgbF(0.951740, 0.960587, 0.524203).rgb(),
                        QColor::fromRgbF(0.954529, 0.965896, 0.540361).rgb(),
                        QColor::fromRgbF(0.957896, 0.971003, 0.556275).rgb(),
                        QColor::fromRgbF(0.961812, 0.975924, 0.571925).rgb(),
                        QColor::fromRgbF(0.966249, 0.980678, 0.587206).rgb(),
                        QColor::fromRgbF(0.971162, 0.985282, 0.602154).rgb(),
                        QColor::fromRgbF(0.976511, 0.989753, 0.616760).rgb(),
                        QColor::fromRgbF(0.982257, 0.994109, 0.631017).rgb(),
                        QColor::fromRgbF(0.988362, 0.998364, 0.644924).rgb()};

        }

    return lutstore;
}












bool JKQTPBuildColorPaletteLUTLessThan(const QPair<double, QRgb> &s1, const QPair<double, QRgb> &s2)
{
    return s1.first<s2.first;
}

bool JKQTPBuildColorPaletteLUTLinSegLessThan(const JKQTPColorPaletteSingleColorLinSegment &s1, const JKQTPColorPaletteSingleColorLinSegment &s2)
{
    return s1.position<s2.position;
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTLinInterpolate(QList<QPair<double, QRgb> > items, int lut_size) {
    std::sort(items.begin(), items.end(), JKQTPBuildColorPaletteLUTLessThan);
    return JKQTPBuildColorPaletteLUTLinInterpolateSorted(items, lut_size);
}



JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUT(QList<QPair<double, QRgb> > items, int lut_size)  {
    std::sort(items.begin(), items.end(), JKQTPBuildColorPaletteLUTLessThan);
    return JKQTPBuildColorPaletteLUTSorted(items, lut_size);
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTLinSegments( QList<JKQTPColorPaletteSingleColorLinSegment> itemsR,  QList<JKQTPColorPaletteSingleColorLinSegment> itemsG,  QList<JKQTPColorPaletteSingleColorLinSegment> itemsB, int lut_size)
{
    std::sort(itemsR.begin(), itemsR.end(), JKQTPBuildColorPaletteLUTLinSegLessThan);
    std::sort(itemsG.begin(), itemsG.end(), JKQTPBuildColorPaletteLUTLinSegLessThan);
    std::sort(itemsB.begin(), itemsB.end(), JKQTPBuildColorPaletteLUTLinSegLessThan);
    return JKQTPBuildColorPaletteLUTLinSegmentsSorted(itemsR, itemsG, itemsB, lut_size);
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTLinSegmentsSorted(const QList<JKQTPColorPaletteSingleColorLinSegment> &itemsR, const QList<JKQTPColorPaletteSingleColorLinSegment> &itemsG, const QList<JKQTPColorPaletteSingleColorLinSegment> &itemsB, int lut_size)
{
    JKQTPImageTools::LUTType lut(lut_size, qRgb(255,255,255));
    if (itemsR.size()<=0 && itemsG.size()<=0 && itemsB.size()<=0) return lut;

    auto buildChannel=[](JKQTPImageTools::LUTType& lut, const QList<JKQTPColorPaletteSingleColorLinSegment> &items, int channel) {
        if (items.size()==1) {
            for (int i=0; i<lut.size(); i++) {
                JKQTPSetColorChannel(lut[i], channel, items.first().colval_startnext);
            }
        } else {
            double dmin=items.first().position;
            double dmax=items.last().position;
            double delta=(dmax-dmin)/static_cast<double>(lut.size()-2);
            double v=dmin;
            int j1=0;
            for (int i=0; i<lut.size(); i++) {
                int j2=j1+1;

                const double local_delta=items[j2].position-items[j1].position;
                const double local_relV=(v-items[j1].position)/local_delta; // relative location of v between j1 and j2

                const int newCol=items[j1].colval_startnext+static_cast<int>(static_cast<double>(items[j2].colval_endprevious-items[j1].colval_startnext)*local_relV);

                JKQTPSetColorChannel(lut[i], channel, jkqtp_bounded<int>(0, newCol, 255));
                //qDebug()<<"i="<<i<<", v="<<v<<", j1="<<j1<<", local_relV="<<local_relV<<", newCol="<<newCol<<" => lut[i]=0x"<<QString::number(lut[i], 16);
                v+=delta;
                // advance over items, if v crosses items[j2].first
                while (v>=items[j1].position && v>=items[j2].position  && j1<items.size()-1 && j2<items.size()-1) {
                    j1++; j2++;
                }
            }
            JKQTPSetColorChannel(lut[lut.size()-1], channel, jkqtp_bounded<int>(0, items.last().colval_endprevious, 255));
        }
    };

    buildChannel(lut, itemsR, 0);
    buildChannel(lut, itemsG, 1);
    buildChannel(lut, itemsB, 2);

    return lut;
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTLinInterpolateSorted(const QList<QPair<double, QRgb> > &items, int lut_size)
{
    JKQTPImageTools::LUTType lut(lut_size, 0);
    if (items.size()<=1) return lut;
    double dmin=items.first().first;
    double dmax=items.last().first;
    double delta=(dmax-dmin)/static_cast<double>(lut_size-2);
    double v=dmin;
    int j1=0;
    for (int i=0; i<lut_size; i++) {
        int j2=j1+1;
        double r=0;
        double g=0;
        double b=0;
        const double local_delta=items[j2].first-items[j1].first;
        const double local_relV=(v-items[j1].first)/local_delta; // relative location of v between j1 and j2

        r=double(qRed(items[j1].second))+local_relV*(double(qRed(items[j2].second))-double(qRed(items[j1].second)));
        g=double(qGreen(items[j1].second))+local_relV*(double(qGreen(items[j2].second))-double(qGreen(items[j1].second)));
        b=double(qBlue(items[j1].second))+local_relV*(double(qBlue(items[j2].second))-double(qBlue(items[j1].second)));

        lut[i]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        v+=delta;
        // advance over items, if v crosses items[j2].first
        while (v>=items[j1].first && v>=items[j2].first  && j1<items.size()-1 && j2<items.size()-1) {
            j1++; j2++;
        }
    }
    lut[lut_size-1]=items.last().second;

    return lut;
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTSorted(const QList<QPair<double, QRgb> > &items, int lut_size)
{
    JKQTPImageTools::LUTType lut(lut_size+1, 0);
    double dmin=items.first().first;
    double dmax=items.last().first;
    double delta=(dmax-dmin)/double(lut_size);
    int i=0;
    int j=0;
    for (double v=dmin; v<=dmax; v=v+delta) {
        double r=0;
        double g=0;
        double b=0;
        if (j+1<items.size()) {
            r=qRed(items[j].second);
            g=qGreen(items[j].second);
            b=qBlue(items[j].second);
            while (j+1<items.size() && v>=items[j+1].first) j++;
        }
        if (i<=lut_size) lut[i]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        i++;
    }
    return lut;
}


JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTLinInterpolate(const QMap<double, QRgb> &items, int lut_size)
{
    QList<QPair<double, QRgb> > itemsi;
    for (auto it=items.begin(); it!=items.end(); ++it) {
        itemsi.append(qMakePair<double, QRgb>(it.key(), it.value()));
    }
    return JKQTPBuildColorPaletteLUTLinInterpolateSorted(itemsi, lut_size);
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUT(const QMap<double, QRgb> &items, int lut_size)
{
    QList<QPair<double, QRgb> > itemsi;
    for (auto it=items.begin(); it!=items.end(); ++it) {
        itemsi.append(qMakePair<double, QRgb>(it.key(), it.value()));
    }
    return JKQTPBuildColorPaletteLUTSorted(itemsi, lut_size);
}


double JKQTPImagePlot_QStringToDouble(QString value) {
    QString v=value;
    if (value.contains(',')) {
        v=value.replace(',', '.');
    }
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    return loc.toDouble(v) ;
}



template <class T1, class T2>
bool JKQTPImagePlot_QPairCompareFirst(const QPair<T1, T2> &s1, const QPair<T1, T2> &s2) {
    return s1.first < s2.first;
}

QStringList JKQTPImageTools::getPredefinedPalettes()  {
    static QStringList sl;

    if (sl.size()!=JKQTPImageTools::global_jkqtpimagetools_lutstore.size()) {
        sl.clear();
        for (auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.begin(); it!=JKQTPImageTools::global_jkqtpimagetools_lutstore.end(); ++it) {
            if (it.key()>=0 && it.key()<=JKQTPMathImageLAST_POSSIBLE_REGISTERED_USER_PALETTE) {
                if (it.value().nameT.size()!=0) sl<<it.value().nameT;
                else if (it.value().name.size()!=0) sl<<it.value().name;
                else sl<<QString(QObject::tr("Palette")+" #"+QString::number(it.key()));
            }
        }
    }
    return sl;
}


QStringList JKQTPImageTools::getPredefinedPalettesMachineReadable()  {
    static QStringList sl;

    if (sl.size()!=JKQTPImageTools::global_jkqtpimagetools_lutstore.size()) {
        sl.clear();
        for (auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.begin(); it!=JKQTPImageTools::global_jkqtpimagetools_lutstore.end(); ++it) {
            if (it.key()>=0) {
                if (it.value().name.size()!=0) sl<<it.value().name;
                else if (it.value().nameT.size()!=0) sl<<it.value().nameT;
                else sl<<QString("palette #"+QString::number(it.key()));
            }
        }
    }
    return sl;
}





QString JKQTPImageTools::JKQTPMathImageColorPalette2String(JKQTPMathImageColorPalette p)
{
    auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.find(p);
    if (it==JKQTPImageTools::global_jkqtpimagetools_lutstore.end()) return QString::number(static_cast<int>(p));
    else {
        if (it.value().name.size()>0) return it.value().name;
        else return QString::number(static_cast<int>(p));
    }
}

JKQTPMathImageColorPalette JKQTPImageTools::String2JKQTPMathImageColorPalette(const QString &p)
{
    for (auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.begin(); it!=JKQTPImageTools::global_jkqtpimagetools_lutstore.end(); ++it) {
        if (QString::compare(p, it.value().name, Qt::CaseInsensitive)==0) {
            return static_cast<JKQTPMathImageColorPalette>(it.key());
        }
    }

    for (auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.begin(); it!=JKQTPImageTools::global_jkqtpimagetools_lutstore.end(); ++it) {
        if (QString::compare(p, it.value().nameT, Qt::CaseInsensitive)==0) {
            return static_cast<JKQTPMathImageColorPalette>(it.key());
        }
    }

    bool ok=false;
    JKQTPMathImageColorPalette res = static_cast<JKQTPMathImageColorPalette>(p.toInt(&ok));
    if (!ok) return JKQTPMathImageMATLAB;
    else return res;
}













const JKQTPImageTools::LUTType& JKQTPImageTools::getLUTforPalette(QMap<int, JKQTPImageTools::LUTData >& lutstore, JKQTPMathImageColorPalette palette) {
    static JKQTPImageTools::LUTType empty(JKQTPImageTools::LUTSIZE, 0);

    auto it=lutstore.find(static_cast<int>(palette));
    if (it==lutstore.end()) {
        return empty;
    } else {
        return it.value().lut;
    }
}



QImage JKQTPImageTools::GetPaletteImage(int i, int width)
{
    return JKQTPImageTools::GetPaletteImage(i, width, 1);
}

QImage JKQTPImageTools::GetPaletteImage(int i, int width, int height)
{
    QImage img;
    QVector<double> pic(width*height,0);
    for (int j=0; j<width*height; j++) {
        pic[j]=j%width;
    }
    JKQTPImageTools::array2image<double>(pic.data(), width, height, img, static_cast<JKQTPMathImageColorPalette>(i), Qt::black, Qt::black);
    return img;
}

QImage JKQTPImageTools::GetPaletteImage(JKQTPMathImageColorPalette palette, int width)
{
    return JKQTPImageTools::GetPaletteImage(static_cast<int>(palette), width);
}



QImage JKQTPImageTools::GetPaletteImage(JKQTPMathImageColorPalette palette, int width, int height)
{
    return JKQTPImageTools::GetPaletteImage(static_cast<int>(palette), width, height);
}

QImage JKQTPImageTools::GetPaletteImage(const JKQTPImageTools::LUTType& lut, int width)
{
    QImage img;
    QVector<double> pic(width);
    for (int j=0; j<width; j++) {
        pic[j]=j;
    }
    JKQTPImageTools::array2image<double>(pic.data(), pic.size(), 1, img, lut, Qt::black, Qt::black);
    return img;
}



QIcon JKQTPImageTools::GetPaletteIcon(int i)  {
    QImage img=JKQTPImageTools::GetPaletteImage(i, JKQTPImageTools::PALETTE_ICON_WIDTH);
    QPixmap pix(JKQTPImageTools::PALETTE_ICON_WIDTH,8);
    QRect r(0,0,JKQTPImageTools::PALETTE_ICON_WIDTH-1,7);
    QPainter p(&pix);
    p.drawImage(r, img);
    p.setPen(QPen(QColor("black")));
    p.drawRect(r);
    p.end();
    return QIcon(pix);

}

QIcon JKQTPImageTools::GetPaletteIcon(JKQTPMathImageColorPalette palette)  {
    return JKQTPImageTools::GetPaletteIcon(static_cast<int>(palette));
}









int JKQTPImageTools::registerPalette(const QString &name, const JKQTPImageTools::LUTType &paletteLut, const QString &nameT)
{
    int id=JKQTPImageTools::global_next_userpalette++;
    JKQTPImageTools::global_jkqtpimagetools_lutstore[id].name=name;
    JKQTPImageTools::global_jkqtpimagetools_lutstore[id].nameT=((nameT.size()>0)?nameT:name);
    JKQTPImageTools::global_jkqtpimagetools_lutstore[id].lut=paletteLut;
    return id;
}

QVector<int> JKQTPImageTools::registerPalettesFromFile(const QString &filename, bool interpolatePalette)
{
    QVector<int> ids;
    QString ext=QFileInfo(filename).suffix().toLower();
    if (ext=="rgb" || ext=="pal" || ext=="csv") {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            QString txt=file.readAll();
            QStringList slt=txt.split('\n');
            bool has4=false;
            bool rgb255=false;
            QList<QPair<double, QRgb> > pal;
            QRegExp rx3("\\s*([0-9eE.+-]+)\\s*([,\\t ])\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*", Qt::CaseInsensitive);
            rx3.setMinimal(false);
            QRegExp rx4("\\s*([0-9eE.+-]+)\\s*([,\\t ])\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*", Qt::CaseInsensitive);
            rx4.setMinimal(false);
            // determine format
            for (int i=slt.size()-1; i>=0; i--) {
                slt[i]=slt[i].trimmed();
                if (rx4.indexIn(slt[i])>=0) {
                    has4=true;
                    double r=JKQTPImagePlot_QStringToDouble(rx4.cap(3));
                    double g=JKQTPImagePlot_QStringToDouble(rx4.cap(4));
                    double b=JKQTPImagePlot_QStringToDouble(rx4.cap(5));
                    if (r>1.0 || g>1.0 || b>1.0) {
                        rgb255=true;
                    }
                } else if (rx3.indexIn(slt[i])>=0) {
                    has4=false;
                    double r=JKQTPImagePlot_QStringToDouble(rx3.cap(1));
                    double g=JKQTPImagePlot_QStringToDouble(rx3.cap(3));
                    double b=JKQTPImagePlot_QStringToDouble(rx3.cap(4));
                    if (r>1.0 || g>1.0 || b>1.0) {
                        rgb255=true;
                    }
                } else {
                    slt.removeAt(i);
                }
            }
            // read data
            bool ok=true;
            for (int i=0; i<slt.size(); i++) {
                double x=0;
                double r=0, g=0, b=0;
                if (has4 && rx4.indexIn(slt[i])>=0) {
                    x=JKQTPImagePlot_QStringToDouble(rx4.cap(1));
                    r=JKQTPImagePlot_QStringToDouble(rx4.cap(3));
                    g=JKQTPImagePlot_QStringToDouble(rx4.cap(4));
                    b=JKQTPImagePlot_QStringToDouble(rx4.cap(5));
                } else if (!has4 && rx3.indexIn(slt[i])>=0) {
                    x=i;
                    r=JKQTPImagePlot_QStringToDouble(rx3.cap(1));
                    g=JKQTPImagePlot_QStringToDouble(rx3.cap(3));
                    b=JKQTPImagePlot_QStringToDouble(rx3.cap(4));
                    //qDebug()<<r<<g<<b;
                } else {
                    ok=false;
                    break;
                }
                if (ok) {
                    if (!rgb255) {
                        pal<<qMakePair(x, qRgb(qBound(0,static_cast<int>(round(255*r)), 255), qBound(0,static_cast<int>(round(255*g)), 255), qBound(0,static_cast<int>(round(255*b)), 255)));
                    } else {
                        pal<<qMakePair(x, qRgb(qBound(0,static_cast<int>(round(r)), 255), qBound(0,static_cast<int>(round(g)), 255), qBound(0,static_cast<int>(round(b)), 255)));
                    }
                }
            }
            if (ok && pal.size()>1) {
                if (interpolatePalette) ids<<registerPalette(QFileInfo(filename).fileName(), JKQTPBuildColorPaletteLUTLinInterpolate(pal));
                else ids<<registerPalette(QFileInfo(filename).fileName(), JKQTPBuildColorPaletteLUT(pal));
            }

        }
    }
#ifdef QT_XML_LIB
    else if (ext=="xml") {
        int cnt=1;
        QDomDocument doc("palette");
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            if (doc.setContent(&file)) {
                file.close();
                QDomElement docElem = doc.documentElement();

                QDomElement n = docElem.firstChildElement("ColorMap");
                if (docElem.nodeName()=="ColorMap") n=docElem;
                while(!n.isNull()) {
                    QString name=n.attribute("name", QString("%2_palette%1").arg(cnt).arg(QFileInfo(filename).fileName()));
                    QString nameT=n.attribute("name", QObject::tr("%2: Palette #%1").arg(cnt).arg(QFileInfo(filename).fileName()));
                    QString colorspace=n.attribute("space", "RGB").toLower();
                    QList<QPair<double, QRgb> > pal;
                    if (colorspace=="rgb"){
                        QDomElement e = n.firstChildElement("Point");
                        while(!e.isNull()) {
                            double x=JKQTPImagePlot_QStringToDouble(e.attribute("x", "0"));
                            double r=JKQTPImagePlot_QStringToDouble(e.attribute("r", "0"));
                            double g=JKQTPImagePlot_QStringToDouble(e.attribute("g", "0"));
                            double b=JKQTPImagePlot_QStringToDouble(e.attribute("b", "0"));
                            pal<<qMakePair(x, qRgb(qBound(0,static_cast<int>(round(255*r)), 255), qBound(0,static_cast<int>(round(255*g)), 255), qBound(0,static_cast<int>(round(255*b)), 255)));
                            e = e.nextSiblingElement("Point");
                        }
                        if (pal.size()>1) {
                            if (interpolatePalette) ids<<registerPalette(name, JKQTPBuildColorPaletteLUTLinInterpolate(pal), nameT);
                            else ids<<registerPalette(name, JKQTPBuildColorPaletteLUT(pal), nameT);
                            cnt++;
                        }
                    }

                    n = n.nextSiblingElement("ColorMap");
                }
            }
        }
    }
#endif
    return ids;
}


JKQTPImageTools::LUTData::LUTData():
    lut(), name(""), nameT("")
{

}


JKQTPImageTools::LUTData::LUTData(const JKQTPImageTools::LUTType &_lut, const QString &_name, const QString &_nameT):
    lut(_lut), name(_name), nameT(_nameT)
{

}

JKQTPImageTools::LUTData::LUTData(const QString &_name, const QString &_nameT):
    lut(), name(_name), nameT(_nameT)
{

}


JKQTPColorPaletteSingleColorLinSegment::JKQTPColorPaletteSingleColorLinSegment(double p, uint8_t y1, uint8_t y2):
    position(p), colval_endprevious(y1), colval_startnext(y2)
{

}

JKQTPColorPaletteSingleColorLinSegment JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(double p, double y1, double y2)
{
    return JKQTPColorPaletteSingleColorLinSegment(p, jkqtp_boundedRoundTo<uint8_t>(0, y1*255.0, 255), jkqtp_boundedRoundTo<uint8_t>(0, y2*255.0, 255));
}


JKQTPColorPaletteSingleColorLinSegment::JKQTPColorPaletteSingleColorLinSegment():
    position(0), colval_endprevious(0), colval_startnext(0)
{

}


JKQTPMathImageModifierMode StringToModifierMode(const QString &mode) {
    QString m=mode.toLower();
    if (m=="value" ) return JKQTPMathImageModifierMode::ModifyValue;
    if (m=="saturation" ) return JKQTPMathImageModifierMode::ModifySaturation;
    if (m=="alpha" ) return JKQTPMathImageModifierMode::ModifyAlpha;
    if (m=="luminance" ) return JKQTPMathImageModifierMode::ModifyLuminance;
    if (m=="hue" ) return JKQTPMathImageModifierMode::ModifyHue;

    return JKQTPMathImageModifierMode::ModifyNone;
}

QString ModifierModeToString(const JKQTPMathImageModifierMode &mode) {

    if (mode==JKQTPMathImageModifierMode::ModifyValue) return "value";
    if (mode==JKQTPMathImageModifierMode::ModifySaturation) return "saturation";
    if (mode==JKQTPMathImageModifierMode::ModifyAlpha) return "alpha";
    if (mode==JKQTPMathImageModifierMode::ModifyLuminance) return "luminance";
    if (mode==JKQTPMathImageModifierMode::ModifyHue) return "hue";
    return "none";
}

void JKQTPModifyImage(QImage &img, JKQTPMathImageModifierMode modifierMode, void *dataModifier, JKQTPMathImageDataType datatypeModifier, int Nx, int Ny, double internalModifierMin, double internalModifierMax)
{
    if (!dataModifier) return;
    //getModifierMinMax(internalModifierMin, internalModifierMax);
    if (modifierMode!=JKQTPMathImageModifierMode::ModifyNone) {
        JKQTPRGBMathImageRGBMode rgbModMode=JKQTPRGBMathImageModeRGBMode;
        int modChannel=3;
        if (modifierMode==JKQTPMathImageModifierMode::ModifyValue) {
            modChannel=2;
            rgbModMode=JKQTPRGBMathImageModeHSVMode;
        } else if (modifierMode==JKQTPMathImageModifierMode::ModifySaturation) {
            modChannel=1;
            rgbModMode=JKQTPRGBMathImageModeHSVMode;
        } else if (modifierMode==JKQTPMathImageModifierMode::ModifyAlpha) {
            modChannel=3;
            rgbModMode=JKQTPRGBMathImageModeRGBMode;
        } else if (modifierMode==JKQTPMathImageModifierMode::ModifyLuminance) {
            modChannel=2;
            rgbModMode=JKQTPRGBMathImageModeHSLMode;
        } else if (modifierMode==JKQTPMathImageModifierMode::ModifyHue) {
            modChannel=0;
            rgbModMode=JKQTPRGBMathImageModeHSLMode;
        }
        //qDebug()<<"mod: "<<modifierMode<<"  ch:"<<modChannel<<"  rgb:"<<rgbModMode;
        switch(datatypeModifier) {
            case JKQTPMathImageDataType::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
        }
    }

}
