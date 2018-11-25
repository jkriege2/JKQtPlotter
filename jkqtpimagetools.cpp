/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtpimagetools.h"
#include <QPainter>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#ifndef NO_JKQTPLOTTER
#include "jkqtpbaseelements.h"
#include "jkqtpbaseplotter.h"
#endif
#ifdef QT_XML_LIB
#  include <QtXml/QtXml>
#endif

#define PALETTE_ICON_WIDTH 64
#define PALETTE_IMAGEICON_HEIGHT 64

QList<int*> global_jkqtpimagetools_lutstore = QList<int*>();

bool JKQTPimagePlot_buildDefinedPaletteLessThan(const QPair<double, QRgb> &s1, const QPair<double, QRgb> &s2)
 {
     return s1.first<s2.first;
 }

void JKQTPimagePlot_buildDefinedPaletteLinInterpolate(int* lut, QList<QPair<double, QRgb> > items, int lut_size) {
    qSort(items.begin(), items.end(), JKQTPimagePlot_buildDefinedPaletteLessThan);

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
            double del=items[j+1].first-items[j].first;
            double rel=(v-items[j].first)/del;
            r=double(qRed(items[j].second))+rel*(double(qRed(items[j+1].second))-double(qRed(items[j].second)));
            g=double(qGreen(items[j].second))+rel*(double(qGreen(items[j+1].second))-double(qGreen(items[j].second)));
            b=double(qBlue(items[j].second))+rel*(double(qBlue(items[j+1].second))-double(qBlue(items[j].second)));
            while (j+1<items.size() && v>=items[j+1].first) j++;
        } else {
            r=double(qRed(items.last().second));
            g=double(qGreen(items.last().second));
            b=double(qBlue(items.last().second));
        }
        if (i<=lut_size) lut[i]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        i++;
    }
}

void JKQTPimagePlot_buildDefinedPaletteLinInterpolate(int* lut, int N, ...) {
    QList<QPair<double, QRgb> > items;

    va_list args;
    va_start(args, N);

    for (int i=0; i<N; i++) {
        double colval=va_arg(args, double);
        QRgb col=va_arg(args,QRgb);
        items<<qMakePair(colval, col);
    }

    va_end(args);

    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(lut, items);
}


void JKQTPimagePlot_buildDefinedPalette(int* lut, QList<QPair<double, QRgb> > items)  {
    qSort(items.begin(), items.end(), JKQTPimagePlot_buildDefinedPaletteLessThan);

    double dmin=items.first().first;
    double dmax=items.last().first;
    double delta=(dmax-dmin)/double(JKQTPimagePlot_LUTSIZE);
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
        if (i<=JKQTPimagePlot_LUTSIZE) lut[i]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        i++;
    }
}

void JKQTPimagePlot_buildDefinedPalette(int* lut, int N, ...) {
    QList<QPair<double, QRgb> > items;

    va_list args;
    va_start(args, N);

    for (int i=0; i<N; i++) {
        double colval=va_arg(args, double);
        QRgb col=va_arg(args,QRgb);
        items<<qMakePair(colval, col);
    }

    va_end(args);

    JKQTPimagePlot_buildDefinedPalette(lut, items);
}


double JKQTPimagePlot_QStringToDouble(QString value) {
    QString v=value;
    if (value.contains(',')) {
        v=value.replace(',', '.');
    }
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    return loc.toDouble(v) ;
}

static QStringList JKQTPimagePlot_palettesSearchPaths=QStringList();
static QMap<int, QList<QPair<double, QRgb> > > JKQTPimagePlot_lutsFromFiles;

template <class T1, class T2>
bool JKQTPimagePlot_QPairCompareFirst(const QPair<T1, T2> &s1, const QPair<T1, T2> &s2) {
    return s1.first < s2.first;
}

QStringList JKQTPimagePlot_getPredefinedPalettes()  {
    static QStringList sl;

    if (JKQTPimagePlot_palettesSearchPaths.isEmpty()) {
        JKQTPimagePlot_palettesSearchPaths<<":/colorpalettes/";
        JKQTPimagePlot_palettesSearchPaths<<(QApplication::applicationDirPath()+"/colorpalettes/");
        JKQTPimagePlot_palettesSearchPaths<<(QApplication::applicationDirPath()+"/assets/colorpalettes/");
    }

    if (sl.size()<=0) {
        sl<<QObject::tr("gray");
        sl<<QObject::tr("inv. gray");
        sl<<QObject::tr("red");
        sl<<QObject::tr("inv. red");
        sl<<QObject::tr("green");
        sl<<QObject::tr("inv. green");
        sl<<QObject::tr("blue");
        sl<<QObject::tr("inv. blue");
        sl<<QObject::tr("cyan");
        sl<<QObject::tr("inv. cyan");
        sl<<QObject::tr("yellow");
        sl<<QObject::tr("inv. yellow");
        sl<<QObject::tr("magenta");
        sl<<QObject::tr("inv. magenta");

        sl<<QObject::tr("Matlab");
        sl<<QObject::tr("inv. Matlab");
        sl<<QObject::tr("RYGB");
        sl<<QObject::tr("inv. RYGB");
        sl<<QObject::tr("HSV");
        sl<<QObject::tr("inv. HSV");
        sl<<QObject::tr("rainbow");
        sl<<QObject::tr("inv. rainbow");
        sl<<QObject::tr("AFM hot");
        sl<<QObject::tr("inv. AFM hot");
        sl<<QObject::tr("ocean");
        sl<<QObject::tr("inv. ocean");
        sl<<QObject::tr("trafficlight");
        sl<<QObject::tr("inv. trafficl.");
        sl<<QObject::tr("BlMaYe");
        sl<<QObject::tr("YeMaBl");
        sl<<QObject::tr("BlYe");
        sl<<QObject::tr("YeBl");
        sl<<QObject::tr("blue-white-red");
        sl<<QObject::tr("red-white-blue");
        sl<<QObject::tr("BBlRdYe");
        sl<<QObject::tr("GnRdVi");
        sl<<QObject::tr("BW print");
        sl<<QObject::tr("inv. BW print");
        sl<<QObject::tr("BrBG");
        sl<<QObject::tr("PuOr");
        sl<<QObject::tr("green-blue");
        sl<<QObject::tr("blue-green");
        sl<<QObject::tr("YeGnBu");
        sl<<QObject::tr("steps: BrBG");
        sl<<QObject::tr("steps: PuOr");
        sl<<QObject::tr("steps: GnBl");
        sl<<QObject::tr("steps: BlGn");
        sl<<QObject::tr("steps: YeGnBu");

        sl<<QObject::tr("cyan-white");
        sl<<QObject::tr("white-cyan");
        sl<<QObject::tr("yellow-white");
        sl<<QObject::tr("white-yellow");
        sl<<QObject::tr("magenta-white");
        sl<<QObject::tr("white-magenta");

        sl<<QObject::tr("blue-green-red");
        sl<<QObject::tr("red-green-blue");
        sl<<QObject::tr("magenta-yellow");
        sl<<QObject::tr("yellow-magenta");
        sl<<QObject::tr("red-blue");
        sl<<QObject::tr("blue-red");
        //sl<<QObject::tr("");

#ifdef QT_XML_LIB
        int palID=sl.size();
#endif
        for (int i=0; i<JKQTPimagePlot_palettesSearchPaths.size(); i++) {
            QDir d(JKQTPimagePlot_palettesSearchPaths[i]);
            QStringList nameFilters;
            nameFilters<<"*.pal";
            nameFilters<<"*.rgb";
            nameFilters<<"*.csv";
#ifdef QT_XML_LIB
            nameFilters<<"*.xml";
#endif
            QFileInfoList l=d.entryInfoList(nameFilters);
            for (int j=0; j<l.size(); j++) {
                QString ext=l[j].suffix().toLower();
                if (ext=="rgb" || ext=="pal" || ext=="csv") {
                    QFile file(l[j].absoluteFilePath());
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
                                double r=JKQTPimagePlot_QStringToDouble(rx4.cap(3));
                                double g=JKQTPimagePlot_QStringToDouble(rx4.cap(4));
                                double b=JKQTPimagePlot_QStringToDouble(rx4.cap(5));
                                if (r>1.0 || g>1.0 || b>1.0) {
                                    rgb255=true;
                                }
                            } else if (rx3.indexIn(slt[i])>=0) {
                                has4=false;
                                double r=JKQTPimagePlot_QStringToDouble(rx3.cap(1));
                                double g=JKQTPimagePlot_QStringToDouble(rx3.cap(3));
                                double b=JKQTPimagePlot_QStringToDouble(rx3.cap(4));
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
                                x=JKQTPimagePlot_QStringToDouble(rx4.cap(1));
                                r=JKQTPimagePlot_QStringToDouble(rx4.cap(3));
                                g=JKQTPimagePlot_QStringToDouble(rx4.cap(4));
                                b=JKQTPimagePlot_QStringToDouble(rx4.cap(5));
                            } else if (!has4 && rx3.indexIn(slt[i])>=0) {
                                x=i;
                                r=JKQTPimagePlot_QStringToDouble(rx3.cap(1));
                                g=JKQTPimagePlot_QStringToDouble(rx3.cap(3));
                                b=JKQTPimagePlot_QStringToDouble(rx3.cap(4));
                                //qDebug()<<r<<g<<b;
                            } else {
                                ok=false;
                                break;
                            }
                            if (ok) {
                                if (!rgb255) {
                                    pal<<qMakePair(x, qRgb(qBound(0,(int)round(255*r), 255), qBound(0,(int)round(255*g), 255), qBound(0,(int)round(255*b), 255)));
                                } else {
                                    pal<<qMakePair(x, qRgb(qBound(0,(int)round(r), 255), qBound(0,(int)round(g), 255), qBound(0,(int)round(b), 255)));
                                }
                            }
                        }
                        //qDebug()<<"read: "<<l[j].fileName()<<": size="<<pal.size()<<" ok="<<ok<<" has4="<<has4<<" rgb255="<<rgb255;
                        if (ok && pal.size()>1) {
                            sl<<l[j].fileName();
                            qStableSort(pal.begin(), pal.end(), JKQTPimagePlot_QPairCompareFirst<double, QRgb>);
                            JKQTPimagePlot_lutsFromFiles[sl.size()-1]=pal;
                            //qDebug()<<"   added as "<<sl.size()-1;
#ifdef QT_XML_LIB
                            palID=l.size();
#endif
                        }

                    }
#ifdef QT_XML_LIB
                } else if (ext=="xml") {
                    QDomDocument doc("palette");
                    QFile file(l[j].absoluteFilePath());
                    if (file.open(QIODevice::ReadOnly)) {
                        if (doc.setContent(&file)) {
                            file.close();
                            QDomElement docElem = doc.documentElement();

                            QDomElement n = docElem.firstChildElement("ColorMap");
                            if (docElem.nodeName()=="ColorMap") n=docElem;
                            while(!n.isNull()) {
                                QString name=n.attribute("name", QString("palette %1").arg(palID+1));
                                QString colorspace=n.attribute("space", "RGB").toLower();
                                QList<QPair<double, QRgb> > pal;
                                if (colorspace=="rgb"){
                                    QDomElement e = n.firstChildElement("Point");
                                    while(!e.isNull()) {
                                        double x=JKQTPimagePlot_QStringToDouble(e.attribute("x", "0"));
                                        //double o=JKQTPimagePlot_QStringToDouble(e.attribute("o", "0"));
                                        double r=JKQTPimagePlot_QStringToDouble(e.attribute("r", "0"));
                                        double g=JKQTPimagePlot_QStringToDouble(e.attribute("g", "0"));
                                        double b=JKQTPimagePlot_QStringToDouble(e.attribute("b", "0"));
                                        pal<<qMakePair(x, qRgb(qBound(0,(int)round(255*r), 255), qBound(0,(int)round(255*g), 255), qBound(0,(int)round(255*b), 255)));
                                        e = e.nextSiblingElement("Point");
                                    }
                                    if (pal.size()>1) {
                                        sl<<name;
                                        qStableSort(pal.begin(), pal.end(), JKQTPimagePlot_QPairCompareFirst<double, QRgb>);
                                        JKQTPimagePlot_lutsFromFiles[sl.size()-1]=pal;
                                        palID=sl.size();
                                        //qDebug()<<"read "<<l[j].fileName()<<":   "<<name<<" ("<<colorspace<<") size="<<pal.size();
                                    }
                                }

                                n = n.nextSiblingElement("ColorMap");
                            }
                        }
                    }
#endif
                }
            }
        }
    }


    return sl;
}

void JKQTPimagePlot_freeLUTs(QList<int *> &lutstore) {
    QList<int *> ls=lutstore;
    lutstore.clear();
    for (int i=0; i<ls.size(); i++) {
        if (ls[i]) free(ls[i]);
    }
}

int* JKQTPimagePlot_getCreateLUT(QList<int*>& lutstore, JKQTPMathImageColorPalette palette) {
    int* lut_used=nullptr;

    if (palette == JKQTPMathImageRED)
    {
        //int*& plut=lutstore[palette];
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgb(static_cast<int>(255.0*v), 0, 0);
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];

    }
    else if (palette == JKQTPMathImageGREEN)
    {
        //int*& plut=&lut_green;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgb(0, static_cast<int>(255.0*v), 0);
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];

    }
    else if (palette == JKQTPMathImageBLUE)
    {
        //int*& plut=&lut_blue;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgb(0, 0, static_cast<int>(255.0*v));
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];

    }
    else if (palette == JKQTPMathImageGRAY)
    {
        //int*& plut=&lut_gray;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgb(static_cast<int>(255.0*v),
                                    static_cast<int>(255.0*v),
                                    static_cast<int>(255.0*v));
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageALPHA)
    {
        //int*& plut=&lut_gray;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgba(255,255,255,
                                    static_cast<int>(255.0*v));
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageINVERTED_ALPHA)
    {
        //int*& plut=&lut_gray;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgba(255,255,255,
                                    static_cast<int>(255.0*v));
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageINVERTEDRED)
    {
        //int*& plut=&lut_invred;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgb(static_cast<int>(255.0*(1.0-v)), 0, 0);
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];

    }
    else if (palette == JKQTPMathImageINVERTEDGREEN)
    {
        //int*& plut=&lut_invgreen;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgb(0, static_cast<int>(255.0*(1.0-v)), 0);
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageINVERTEDBLUE)
    {
        //int*& plut=&lut_invblue;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    plut[l]=qRgb(0, 0, static_cast<int>(255.0*(1.0-v)));
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageINVERTEDGRAY)
    {
        //int*& plut=&lut_invgray;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=1.0-(l/(double)(JKQTPimagePlot_LUTSIZE));
                    plut[l]=qRgb(static_cast<int>(255.0*v),
                                    static_cast<int>(255.0*v),
                                    static_cast<int>(255.0*v));
                }
                plut[JKQTPimagePlot_LUTSIZE+1]=plut[JKQTPimagePlot_LUTSIZE];
            }
        }
        lut_used=lutstore[palette];
    }

    else if (palette == JKQTPMathImageMATLAB)
    {
        //int*& plut=&lut_matlab;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];
    }

        else if (palette == JKQTPMathImageINVERTED_MATLAB)
        {
            //int*& plut=&lut_matlab_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                if (plut!=nullptr) {
                    for (int l=JKQTPimagePlot_LUTSIZE; l>=0; l--) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
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
            lut_used=lutstore[palette];

    }
        else if (palette == JKQTPMathImageRYGB) //gnuplot: 30,-13,-23
        {
            //int*& plut=&lut_rygb;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
            lut_used=lutstore[palette];

    }
        else if (palette == JKQTPMathImageINVERTED_RYGB) //gnuplot: 30,-13,-23
        {
            //int*& plut=&lut_rygb_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                if (plut!=nullptr) {
                    for (int l=JKQTPimagePlot_LUTSIZE; l>=0; l--) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
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
            lut_used=lutstore[palette];

    }
    else if (palette == JKQTPMathImageHSV)
    {
        //int*& plut=&lut_hsv;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageINVERTED_HSV)
    {
        //int*& plut=&lut_invhsv;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];

    }
    else if (palette == JKQTPMathImageRAINBOW) //gnuplot: 33,13,10
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            //std::cout<<"!!! creating rainbow lut\n";
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    double r = 255.0*std::abs(2.0*v-0.5);
                    if (r > 255.0)
                        r = 255.0;

                    double g = 255.0*sin(M_PI*v);

                    double b = 255.0*cos(0.5*M_PI*v);

                    plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                }
            }
        }
        lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageINVERTED_RAINBOW) //gnuplot: 33,13,10
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            //std::cout<<"!!! creating rainbow lut\n";
            if (plut!=nullptr) {
                for (int l=JKQTPimagePlot_LUTSIZE; l>=0; l--) {
                    double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                    double r = 255.0*std::abs(2.0*v-0.5);
                    if (r > 255.0)
                        r = 255.0;

                    double g = 255.0*sin(M_PI*v);

                    double b = 255.0*cos(0.5*M_PI*v);

                    plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                }
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageHOT) //gnuplot: 21,22,23
    {
        //int*& plut=&lut_hot;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageINVERTED_HOT) //gnuplot: 21,22,23
    {
        //int*& plut=&lut_hot_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=JKQTPimagePlot_LUTSIZE; l>=0; l--) {
                    double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageOCEAN) //gnuplot: 23,28,3
    {
        //int*& plut=&lut_ocean;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
                    double r = 765.0*v-510.0;
                    if (r < 0.0)
                        r = 0.0;

                    double g = std::abs(382.5*v-127.5);

                    double b = 255.0*v;
                    plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                }
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageINVERTED_OCEAN) //gnuplot: 23,28,3
    {
        //int*& plut=&lut_ocean_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=JKQTPimagePlot_LUTSIZE; l>=0; l--) {
                    double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                    double r = 765.0*v-510.0;
                    if (r < 0.0)
                        r = 0.0;

                    double g = std::abs(382.5*v-127.5);

                    double b = 255.0*v;
                    plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                }
            }
        }
        lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageBLUEMAGENTAYELLOW) //gnuplot: 30,31,32
    {
        //int*& plut=&lut_bluemagentayellow;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];
    }
        else if (palette == JKQTPMathImageINVERTED_BLUEMAGENTAYELLOW) //gnuplot: 30,31,32
       {
           //int*& plut=&lut_bluemagentayellow_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
               if (plut!=nullptr) {
                   for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                       double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
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
           lut_used=lutstore[palette];
       }
    else if (palette == JKQTPMathImageBLUEYELLOW) //gnuplot: 8,9,10
    {
        //int*& plut=&lut_blueyellow;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImageINVERTED_BLUEYELLOW) //gnuplot: 8,9,10
    {
        //int*& plut=&lut_blueyellow_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImageCYAN)
    {
        //int*& plut=&lut_cyan;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];

    }
        else if (palette == JKQTPMathImageINVERTED_CYAN)
    {
        //int*& plut=&lut_cyan_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];

    }
        else if (palette == JKQTPMathImageTRAFFICLIGHT)
    {
        //int*& plut=&lut_trafficlight;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=l/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];

    }
        else if (palette == JKQTPMathImageINVERTED_TRAFFICLIGHT)
    {
        //int*& plut=&lut_trafficlight_inverted;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            if (plut!=nullptr) {
                for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                    double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
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
        lut_used=lutstore[palette];

    }
    else if (palette == JKQTPMathImageBLUEWHITERED) //gnuplot: 34,13,-34
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            //std::cout<<"!!! creating rainbow lut\n";
            if (plut!=nullptr) {
                QList<QPair<double, QRgb> > lst;
                lst<<qMakePair<double, QRgb>(8.0, 0xFFB2182B);
                lst<<qMakePair<double, QRgb>(7.0, 0xFFD6604D);
                lst<<qMakePair<double, QRgb>(6.0, 0xFFF4A582);
                lst<<qMakePair<double, QRgb>(5.0, 0xFFFDDBC7);
                lst<<qMakePair<double, QRgb>(4.0, 0xFFD1E5F0);
                lst<<qMakePair<double, QRgb>(3.0, 0xFF92C5DE);
                lst<<qMakePair<double, QRgb>(2.0, 0xFF4393C3);
                lst<<qMakePair<double, QRgb>(1.0, 0xFF2166AC);

                JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
            }
        }
        lut_used=lutstore[palette];
    }
    else if (palette == JKQTPMathImageREDWHITEBLUE) //gnuplot: https://github.com/aschn/gnuplot-colorbrewer/blob/master/diverging/RdBu.plt
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            //std::cout<<"!!! creating rainbow lut\n";
            if (plut!=nullptr) {
                QList<QPair<double, QRgb> > lst;
                lst<<qMakePair<double, QRgb>(0.0, 0xFFB2182B);
                lst<<qMakePair<double, QRgb>(1.0, 0xFFD6604D);
                lst<<qMakePair<double, QRgb>(2.0, 0xFFF4A582);
                lst<<qMakePair<double, QRgb>(3.0, 0xFFFDDBC7);
                lst<<qMakePair<double, QRgb>(4.0, 0xFFD1E5F0);
                lst<<qMakePair<double, QRgb>(5.0, 0xFF92C5DE);
                lst<<qMakePair<double, QRgb>(6.0, 0xFF4393C3);
                lst<<qMakePair<double, QRgb>(7.0, 0xFF2166AC);

                JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
            }
        }
        lut_used=lutstore[palette];



    } else if (palette == JKQTPMathImageBLACKBLUEREDYELLOW) //gnuplot: 7,5,15
        {
//                int*& plut=&lut_BLACKBLUEREDYELLOW;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        double r = 255.0*qBound(0.0,sqrt(v),1.0);
                        double g = 255.0*qBound(0.0,v*v*v,1.0);
                        double b = 255.0*qBound(0.0,sin(2.0*M_PI*v),1.0);

                        plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageGREENREDVIOLET) //gnuplot: 3,11,6
    {
            //int*& plut=&lut_GREENREDVIOLET;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        double r = 255.0*qBound(0.0,v,1.0);
                        double g = 255.0*qBound(0.0,fabs(v-0.5),1.0);
                        double b = 255.0*qBound(0.0,v*v*v*v,1.0);

                        plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageBLACKBLUEVIOLETYELLOWWHITE) //gnuplot: 30,31,32
    {
            //int*& plut=&lut_BLACKBLUEVIOLETYELLOWWHITE;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        double r = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                        double g = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                        double b = 255.0*qBound(0.0,(v<0.25)?4*v:(v<0.42)?1.0:(v<0.92)?-2.0*v+1.84:v/0.08-11.5,1.0);

                        plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageWHITEYELLOWVIOLETBLUEBLACK) //gnuplot: 30,31,32
    {
            //int*& plut=&lut_WHITEYELLOWVIOLETBLUEBLACK;
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                        double r = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                        double g = 255.0*qBound(0.0,v/0.32-0.78125,1.0);
                        double b = 255.0*qBound(0.0,(v<0.25)?4*v:(v<0.42)?1.0:(v<0.92)?-2.0*v+1.84:v/0.08-11.5,1.0);

                        plut[l]=qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
                    }
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageBR_GR) //https://github.com/aschn/gnuplot-colorbrewer/blob/master/diverging/BrBG.plt
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(0.0, qRgb(0x8C, 0x51, 0x0A));
                    lst<<qMakePair<double, QRgb>(1.0, qRgb(0xBF, 0x81, 0x2D));
                    lst<<qMakePair<double, QRgb>(2.0, qRgb(0xDF, 0xC2, 0x7D));
                    lst<<qMakePair<double, QRgb>(3.0, 0xFFF6E8C3);
                    lst<<qMakePair<double, QRgb>(4.0, 0xFFC7EAE5);
                    lst<<qMakePair<double, QRgb>(5.0, 0xFF80CDC1);
                    lst<<qMakePair<double, QRgb>(6.0, 0xFF35978F);
                    lst<<qMakePair<double, QRgb>(7.0, 0xFF01665E);

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageBR_GR_STEP) //https://github.com/aschn/gnuplot-colorbrewer/blob/master/diverging/BrBG.plt
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(0.0, qRgb(0x8C, 0x51, 0x0A));
                    lst<<qMakePair<double, QRgb>(1.0, qRgb(0xBF, 0x81, 0x2D));
                    lst<<qMakePair<double, QRgb>(2.0, qRgb(0xDF, 0xC2, 0x7D));
                    lst<<qMakePair<double, QRgb>(3.0, 0xFFF6E8C3);
                    lst<<qMakePair<double, QRgb>(4.0, 0xFFC7EAE5);
                    lst<<qMakePair<double, QRgb>(5.0, 0xFF80CDC1);
                    lst<<qMakePair<double, QRgb>(6.0, 0xFF35978F);
                    lst<<qMakePair<double, QRgb>(7.0, 0xFF01665E);

                    JKQTPimagePlot_buildDefinedPalette(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImagePU_OR) //https://github.com/aschn/gnuplot-colorbrewer/blob/master/diverging/PuOr.plt
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImagePU_OR_STEP) //https://github.com/aschn/gnuplot-colorbrewer/blob/master/diverging/PuOr.plt
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPalette(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageYL_GN_BU) //http://colorbrewer2.org/
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageYL_GN_BU_STEP) //http://colorbrewer2.org/
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPalette(plut, lst);
                }
            }
            lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImageGN_BU) //http://colorbrewer2.org/
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageGN_BU_STEP) //http://colorbrewer2.org/
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPalette(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageBU_GN) //http://colorbrewer2.org/
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageBU_GN_STEP) //http://colorbrewer2.org/
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
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

                    JKQTPimagePlot_buildDefinedPalette(plut, lst);
                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageINVERTED_MAGENTA) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,1,0,v).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageMAGENTA) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,1,0,v).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageINVERTED_YELLOW) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,0,1,v).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageYELLOW) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,0,1,v).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImageINVERTED_MAGENTAWHITE) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,v,0,0).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageMAGENTAWHITE) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,v,0,0).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageINVERTED_YELLOWWHITE) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,0,v,0).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageYELLOWWHITE) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(0,0,v,0).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImageINVERTED_CYANWHITE) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(v,0,0,0).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageCYANWHITE) // from CMYK model
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    for (int l=0; l<=JKQTPimagePlot_LUTSIZE; l++) {
                        double v=(JKQTPimagePlot_LUTSIZE-l)/(double)(JKQTPimagePlot_LUTSIZE);
                        plut[l]=QColor::fromCmykF(v,0,0,0).rgba();
                    }

                }
            }
            lut_used=lutstore[palette];
    } else if (palette == JKQTPMathImageBlueGreenRed)
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(1.0, QColor("blue").rgb());
                    lst<<qMakePair<double, QRgb>(2.0, QColor("green").rgb());
                    lst<<qMakePair<double, QRgb>(3.0, QColor("red").rgb());

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImageRedGreenBlue)
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(1.0, QColor("red").rgb());
                    lst<<qMakePair<double, QRgb>(2.0, QColor("green").rgb());
                    lst<<qMakePair<double, QRgb>(3.0, QColor("blue").rgb());

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];


    } else if (palette == JKQTPMathImagePREDEFINED_PALETTES_COUNT)
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(1.0, QColor("magenta").rgb());
                    lst<<qMakePair<double, QRgb>(2.0, QColor("yellow").rgb());

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImagePREDEFINED_PALETTES_COUNT+1)
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(1.0, QColor("yellow").rgb());
                    lst<<qMakePair<double, QRgb>(2.0, QColor("magenta").rgb());

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImagePREDEFINED_PALETTES_COUNT+2)
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(1.0, QColor("red").rgb());
                    lst<<qMakePair<double, QRgb>(2.0, QColor("blue").rgb());

                    JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
                }
            }
            lut_used=lutstore[palette];

    } else if (palette == JKQTPMathImagePREDEFINED_PALETTES_COUNT+3)
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            //std::cout<<"!!! creating rainbow lut\n";
            if (plut!=nullptr) {
                QList<QPair<double, QRgb> > lst;
                lst<<qMakePair<double, QRgb>(1.0, QColor("blue").rgb());
                lst<<qMakePair<double, QRgb>(2.0, QColor("red").rgb());

                JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, lst);
            }
        }
        lut_used=lutstore[palette];


            /*
    } else if (palette == JKQTPMathImagePU_OR) //https://github.com/aschn/gnuplot-colorbrewer/blob/master/diverging/PuOr.plt
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
                //std::cout<<"!!! creating rainbow lut\n";
                if (plut!=nullptr) {
                    QList<QPair<double, QRgb> > lst;
                    lst<<qMakePair<double, QRgb>(0.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(1.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(2.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(3.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(4.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(5.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(6.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(7.0, 0xFF);
                    lst<<qMakePair<double, QRgb>(8.0, 0xFF);

                    JKQTPimagePlot_buildDefinedPalette(plut, lst);
                }
            }
            lut_used=lutstore[palette];

  */

    } else if (JKQTPimagePlot_lutsFromFiles.contains(palette)) // LUTs read from files in JKQTPimagePlot_lutsFromFiles
    {
        if (lutstore[palette]==nullptr) {
            lutstore[palette]=(int*)malloc((JKQTPimagePlot_LUTSIZE+2)*sizeof(int));
            int* plut=lutstore[palette];
            //std::cout<<"!!! creating rainbow lut\n";
            if (plut!=nullptr) {
                if (JKQTPimagePlot_lutsFromFiles[palette].size()>220) JKQTPimagePlot_buildDefinedPalette(plut, JKQTPimagePlot_lutsFromFiles[palette]);
                else JKQTPimagePlot_buildDefinedPaletteLinInterpolate(plut, JKQTPimagePlot_lutsFromFiles[palette]);
            }
        }
        lut_used=lutstore[palette];
    }
    return lut_used;
}


QImage JKQTPMathImageGetPaletteImage(int i, int width)
{
    return JKQTPMathImageGetPaletteImage(i, width, 1);
}

QImage JKQTPMathImageGetPaletteImage(int i, int width, int height)
{
    QImage img;
    double* pic=(double*)malloc(width*height*sizeof(double));
    for (int j=0; j<width*height; j++) {
        pic[j]=j%width;
    }
    JKQTPimagePlot_array2image<double>(pic, width, height, img, (JKQTPMathImageColorPalette)i, Qt::black, Qt::black);
    free(pic);
    return img;
}

QImage JKQTPMathImageGetPaletteImage(JKQTPMathImageColorPalette palette, int width)
{
    return JKQTPMathImageGetPaletteImage((int)palette, width);
}



QImage JKQTPMathImageGetPaletteImage(JKQTPMathImageColorPalette palette, int width, int height)
{
    return JKQTPMathImageGetPaletteImage((int)palette, width, height);
}

QImage JKQTPMathImageGetPaletteImage(int *lut, int lut_size, int width)
{
    QImage img;
    double* pic=(double*)malloc(width*sizeof(double));
    for (int j=0; j<width; j++) {
        pic[j]=j;
    }
    JKQTPimagePlot_array2image<double>(pic, width, 1, img, lut, lut_size, Qt::black, Qt::black);
    free(pic);
    return img;
}



QIcon JKQTPMathImageGetPaletteIcon(int i)  {
    QImage img=JKQTPMathImageGetPaletteImage(i, PALETTE_ICON_WIDTH);
    QPixmap pix(PALETTE_ICON_WIDTH,8);
    QRect r(0,0,PALETTE_ICON_WIDTH-1,7);
    QPainter p(&pix);
    p.drawImage(r, img);
    p.setPen(QPen(QColor("black")));
    p.drawRect(r);
    p.end();
    return QIcon(pix);

}

QIcon JKQTPMathImageGetPaletteIcon(JKQTPMathImageColorPalette palette)  {
    return JKQTPMathImageGetPaletteIcon((int)palette);
}





JKQTPMathImageColorPaletteComboBox::JKQTPMathImageColorPaletteComboBox(QWidget *parent):
    QComboBox(parent)
{
    setIconSize(QSize(PALETTE_ICON_WIDTH,16));
    QStringList pal=JKQTPimagePlot_getPredefinedPalettes();
    clear();
    for (int i=0; i<pal.size(); i++) {
        addItem(JKQTPMathImageGetPaletteIcon(i), pal[i]);
    }
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(intIndexChanged(int)));
}

JKQTPMathImageColorPalette JKQTPMathImageColorPaletteComboBox::colorPalette() const
{
    return JKQTPMathImageColorPalette(currentIndex());
}

JKQTPMathImageColorPalette JKQTPMathImageColorPaletteComboBox::currentColorPalette() const
{
    return colorPalette();

}

JKQTPMathImageColorPalette JKQTPMathImageColorPaletteComboBox::getColorPalette() const
{
    return colorPalette();
}

void JKQTPMathImageColorPaletteComboBox::setColorPalette(JKQTPMathImageColorPalette palette)
{
    setCurrentIndex(int(palette));
}

void JKQTPMathImageColorPaletteComboBox::setCurrentColorPalette(JKQTPMathImageColorPalette palette)
{
    setCurrentIndex(int(palette));
}

void JKQTPMathImageColorPaletteComboBox::intIndexChanged(int i)
{
    emit currentPaletteChanged(JKQTPMathImageColorPalette(i));
}



QImage JKQTPMathImageGetAlphaPaletteImage(int *lut, int lut_size, int width, int height)
{
    QImage img;
    if (!lut || lut_size<=0 || width<=0 || height<=0) return img;
    double* pic=(double*)malloc(width*sizeof(double));
    for (int j=0; j<width; j++) {
        pic[j]=j;
    }
    QVector<int> l1(lut_size, QColor("white").rgba());
    for (int i=0; i<l1.size(); i++) {
        l1[i]=qRgba(255,255,255, lut[i]);
    }
    QImage img1(8,8,QImage::Format_ARGB32);
    img1.fill(Qt::white);
    for (int y=0; y<img1.height()/2; y++) {
        QRgb* sl1=(QRgb*)img1.scanLine(y);
        QRgb* sl2=(QRgb*)img1.scanLine(y+img1.height()/2);
        for (int x=0; x<img1.width()/2; x++) {
            sl1[x]=QColor(Qt::black).rgba();
            sl2[x+img1.width()/2]=QColor(Qt::black).rgba();
        }
    }
    JKQTPimagePlot_array2image<double>(pic, width, 1, img, l1.data(), lut_size, Qt::black, Qt::black);
    free(pic);
    QImage img2(width, height, QImage::Format_ARGB32);
    QPainter p(&img2);
    QBrush b;
    b.setStyle(Qt::TexturePattern);
    b.setTextureImage(img1);
    p.setBrush(b);
    p.fillRect(QRect(0,0,width,height), b);
    p.drawImage(QRect(0,0,width,height), img);
    p.end();
    return img2;
}




#ifndef NO_JKQTPLOTTER



JKQTPColorPaletteTools::JKQTPColorPaletteTools(JKQtBasePlotter *parent)
{
    cbParent=parent;
    colorBarRightAxis=new JKQTPverticalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarRightAxis->loadSettings(parent->getYAxis());
    colorBarRightAxis->set_drawMode1(JKQTPCADMline);
    colorBarRightAxis->set_drawMode2(JKQTPCADMcomplete);
    colorBarRightAxis->set_axisLabel("");
    colorBarRightAxis->set_minTicks(3);
    colorBarRightAxis->set_showZeroAxis(false);
    colorBarRightAxis->set_minorTicks(0);
    colorBarRightAxis->set_tickOutsideLength(0);
    colorBarRightAxis->set_minorTickOutsideLength(0);
    colorBarTopAxis=new JKQTPhorizontalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarTopAxis->loadSettings(parent->getXAxis());
    colorBarTopAxis->set_drawMode1(JKQTPCADMline);
    colorBarTopAxis->set_drawMode2(JKQTPCADMcomplete);
    colorBarTopAxis->set_axisLabel("");
    colorBarTopAxis->set_minTicks(3);
    colorBarTopAxis->set_showZeroAxis(false);
    colorBarTopAxis->set_minorTicks(0);
    colorBarTopAxis->set_tickOutsideLength(0);
    colorBarTopAxis->set_minorTickOutsideLength(0);


    this->palette=JKQTPMathImageMATLAB;
    this->imageNameFontName=parent->get_keyFont();
    this->imageNameFontSize=parent->get_keyFontSize();
    this->imageName="";
    this->showColorBar=true;
    this->colorBarWidth=14;
    this->colorBarRelativeHeight=0.75;
    this->autoImageRange=true;
    this->imageMin=0;
    this->imageMax=1;
    this->colorBarOffset=4;
    this->rangeMinFailAction=JKQTPMathImageLastPaletteColor;
    this->rangeMaxFailAction=JKQTPMathImageLastPaletteColor;
    this->rangeMinFailColor=QColor("black");
    this->rangeMaxFailColor=QColor("black");
    this->nanColor=QColor("black");
    this->infColor=QColor("black");
    this->colorBarTopVisible=false;
    this->colorBarRightVisible=true;
}

JKQTPColorPaletteTools::~JKQTPColorPaletteTools()
{

}

void JKQTPColorPaletteTools::cbGetOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) {
    Q_UNUSED(leftSpace);
    Q_UNUSED(bottomSpace);
    if (showColorBar) {
        double internalDataMin=getInternalDataMin();
        double internalDataMax=getInternalDataMax();
        cbGetDataMinMax(internalDataMin, internalDataMax);
        if (colorBarRightVisible) {
            rightSpace+=cbParent->pt2px(painter, colorBarWidth+colorBarOffset);
            colorBarRightAxis->setRange(internalDataMin, internalDataMax);
            colorBarRightAxis->set_axisWidth(colorBarRelativeHeight*cbParent->get_plotHeight());
            QSize s=colorBarRightAxis->getSize2(painter);
            /*cbParent->get_mathText()->set_fontSize(imageNameFontSize*cbParent->get_fontSizeMultiplier());
            cbParent->get_mathText()->set_fontRoman(imageNameFontName);
            cbParent->get_mathText()->parse(imageName);
            QSizeF names=cbParent->get_mathText()->getSize(painter);*/
            QSizeF names=cbParent->getTextSizeSize(imageNameFontName, imageNameFontSize*cbParent->get_fontSizeMultiplier(), imageName, painter);
            rightSpace+=cbParent->pt2px(painter, colorBarWidth+colorBarOffset)+qMax((double)s.width(), (double)names.width());
        }
        if (colorBarTopVisible) {
            //topSpace+=cbParent->pt2px(painter, colorBarWidth+colorBarOffset);
            colorBarTopAxis->setRange(internalDataMin, internalDataMax);
            colorBarTopAxis->set_axisWidth(colorBarRelativeHeight*cbParent->get_plotWidth());
            QSize s=colorBarTopAxis->getSize2(painter);
            /*cbParent->get_mathText()->set_fontSize(imageNameFontSize*cbParent->get_fontSizeMultiplier());
            cbParent->get_mathText()->set_fontRoman(imageNameFontName);
            cbParent->get_mathText()->parse(imageName);
            QSizeF names=cbParent->get_mathText()->getSize(painter);*/
            QSizeF names=cbParent->getTextSizeSize(imageNameFontName, imageNameFontSize*cbParent->get_fontSizeMultiplier(), imageName, painter);
            topSpace+=cbParent->pt2px(painter, colorBarWidth+colorBarOffset)+qMax((double)s.height(), (double)names.height());
        }
    }
}

void JKQTPColorPaletteTools::cbDrawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) {
    Q_UNUSED(leftSpace);
    Q_UNUSED(bottomSpace);
    if (showColorBar) {
        double internalDataMin=getInternalDataMin();
        double internalDataMax=getInternalDataMax();
        cbGetDataMinMax(internalDataMin, internalDataMax);
        if (colorBarRightVisible) {
            painter.save();

            uint8_t h=1;
            uint8_t d[200*h];//, dd[200*h];
            for (int i=0; i<200; i++) {
                for (int j=0; j<h; j++) {
                    d[i*h+j]=i;
                    //dd[i*h+j]=j;
                }
            }
            QImage b(h, 200, QImage::Format_ARGB32);
            JKQTPimagePlot_array2image<uint8_t>(d, h, 200, b, palette, 0, 199);


            /*cbParent->get_mathText()->set_fontSize(imageNameFontSize*cbParent->get_fontSizeMultiplier());
            cbParent->get_mathText()->set_fontRoman(imageNameFontName);
            cbParent->get_mathText()->parse(imageName);
            QSizeF names=cbParent->get_mathText()->getSize(painter);*/
            QSizeF names=cbParent->getTextSizeSize(imageNameFontName, imageNameFontSize*cbParent->get_fontSizeMultiplier(), imageName, painter);

            double icolorBarRelativeHeight=colorBarRelativeHeight;
            int barHeight=rightSpace.height()*icolorBarRelativeHeight;
            if (barHeight<1) barHeight=1;
            // find a height for the bar that allows to show the image name
            while ((barHeight>1) && ((rightSpace.height()-barHeight)/2<names.height())) {
                barHeight--;
            }

            //qDebug()<<"r: "<<imageName<<rightSpace;

            QRect cb(rightSpace.x()+cbParent->pt2px(painter, colorBarOffset), rightSpace.top()+(rightSpace.height()-barHeight)/2, cbParent->pt2px(painter, colorBarWidth), barHeight);
            painter.drawImage(cb, b.mirrored(true, false));
            QPen p=painter.pen();
            p.setColor(colorBarRightAxis->get_axisColor());
            p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, cbParent->pt2px(painter, colorBarRightAxis->get_lineWidth()*cbParent->get_lineWidthMultiplier())));
            painter.setPen(p);
            painter.drawRect(cb);

            colorBarRightAxis->setRange(internalDataMin, internalDataMax);
            colorBarRightAxis->set_axisWidth(cb.height());
            colorBarRightAxis->set_axisOffset(cb.top());
            colorBarRightAxis->set_otherAxisOffset(cb.left());
            colorBarRightAxis->set_otherAxisWidth(cb.width());
            colorBarRightAxis->drawAxes(painter);



            cbParent->get_mathText()->set_fontSize(imageNameFontSize*cbParent->get_fontSizeMultiplier());
            cbParent->get_mathText()->set_fontRoman(imageNameFontName);
            cbParent->get_mathText()->parse(imageName);
            cbParent->get_mathText()->draw(painter, Qt::AlignHCenter|Qt::AlignVCenter, QRect(rightSpace.x(), rightSpace.y(), rightSpace.width(), (rightSpace.height()-barHeight)/2));

            painter.restore();
        }
        if (colorBarTopVisible) {
            painter.save();

            /*uint8_t d[200];
            for (int i=0; i<200; i++) d[i]=i;
            QImage b(200,1, QImage::Format_ARGB32);
            JKQTPimagePlot_array2image<uint8_t>(d, 200, 1, b, palette, 0, 199);*/


            uint8_t h=1;

            uint8_t d[200*h];//, dd[200*h];
            for (int i=0; i<200; i++) {
                for (int j=0; j<h; j++) {
                    d[i*h+j]=i;
                    //dd[i*h+j]=j;
                }
            }
            QImage b(h,200, QImage::Format_ARGB32);
            JKQTPimagePlot_array2image<uint8_t>(d,h,200, b, palette, 0, 199);





            /*cbParent->get_mathText()->set_fontSize(imageNameFontSize*cbParent->get_fontSizeMultiplier());
            cbParent->get_mathText()->set_fontRoman(imageNameFontName);
            cbParent->get_mathText()->parse(imageName);
            QSizeF names=cbParent->get_mathText()->getSize(painter);*/
            QSizeF names=cbParent->getTextSizeSize(imageNameFontName, imageNameFontSize*cbParent->get_fontSizeMultiplier(), imageName, painter);

            double icolorBarRelativeHeight=colorBarRelativeHeight;
            int barWidth=topSpace.width()*icolorBarRelativeHeight;
            if (barWidth<1) barWidth=1;
            // find a height for the bar that allows to show the image name
            while ((barWidth>1) && ((topSpace.width()-barWidth)/2<names.width())) {
                barWidth--;
            }


            QRect cb(topSpace.x()+(topSpace.width()-barWidth)/2, topSpace.bottom()-cbParent->pt2px(painter, colorBarOffset+(colorBarWidth)), barWidth, cbParent->pt2px(painter, colorBarWidth));
            //qDebug()<<"t: "<<imageName<<topSpace<<topSpace.bottom()<<colorBarOffset<<"\n     "<<cb;
            QMatrix rm;
            rm.rotate(90);
            painter.drawImage(cb, b.transformed(rm));
            QPen p=painter.pen();
            p.setColor(colorBarTopAxis->get_axisColor());
            p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, cbParent->pt2px(painter, colorBarTopAxis->get_lineWidth()*cbParent->get_lineWidthMultiplier())));
            painter.setPen(p);
            painter.drawRect(cb);


            colorBarTopAxis->setRange(internalDataMin, internalDataMax);
            colorBarTopAxis->set_axisWidth(cb.width());
            colorBarTopAxis->set_axisOffset(cb.left());
            colorBarTopAxis->set_otherAxisOffset(cb.top());
            colorBarTopAxis->set_otherAxisWidth(cb.height());
            colorBarTopAxis->drawAxes(painter);



            cbParent->get_mathText()->set_fontSize(imageNameFontSize*cbParent->get_fontSizeMultiplier());
            cbParent->get_mathText()->set_fontRoman(imageNameFontName);
            cbParent->get_mathText()->parse(imageName);
            cbParent->get_mathText()->draw(painter, Qt::AlignHCenter|Qt::AlignVCenter, QRect(topSpace.right()-(topSpace.width()-barWidth)/2, topSpace.y(), (topSpace.width()-barWidth)/2, topSpace.height()));

            painter.restore();
        }
    }
}


QStringList JKQTPColorPaletteTools::getPalettes()  {
    return JKQTPimagePlot_getPredefinedPalettes();
}

int JKQTPColorPaletteTools::getPalettesCount()
{
    return getPalettes().size();
}

QIcon JKQTPColorPaletteTools::getPaletteIcon(int i)  {
    QImage img=getPaletteImage(i, PALETTE_ICON_WIDTH);
    QPixmap pix(PALETTE_ICON_WIDTH,8);
    QRect r(0,0,PALETTE_ICON_WIDTH-1,7);
    QPainter p(&pix);
    p.drawImage(r, img);
    p.setPen(QPen(QColor("black")));
    p.drawRect(r);
    p.end();
    return QIcon(pix);

}

QIcon JKQTPColorPaletteTools::getPaletteIcon(JKQTPMathImageColorPalette palette)  {
    return getPaletteIcon((int)palette);
}

QImage JKQTPColorPaletteTools::getPaletteImage(int i, int width)
{
    QImage img;
    double* pic=(double*)malloc(width*sizeof(double));
    for (int j=0; j<width; j++) {
        pic[j]=j;
    }
    JKQTPimagePlot_array2image<double>(pic, width, 1, img, (JKQTPMathImageColorPalette)i, 0, width-1);
    free(pic);
    return img;
}

QImage JKQTPColorPaletteTools::getPaletteImage(JKQTPMathImageColorPalette palette, int width)
{
    return getPaletteImage((int)palette, width);
}

QIcon JKQTPColorPaletteTools::getPaletteKeyIcon(int i)
{
    QImage img=getPaletteKeyImage(i, PALETTE_ICON_WIDTH, PALETTE_IMAGEICON_HEIGHT);
    QPixmap pix(PALETTE_ICON_WIDTH,PALETTE_IMAGEICON_HEIGHT);
    QRect r(0,0,PALETTE_ICON_WIDTH-1,PALETTE_IMAGEICON_HEIGHT-1);
    QPainter p(&pix);
    p.drawImage(r, img);
    p.setPen(QPen(QColor("black")));
    p.drawRect(r);
    p.end();
    return QIcon(pix);
}

QIcon JKQTPColorPaletteTools::getPaletteKeyIcon(JKQTPMathImageColorPalette palette)
{
    return getPaletteIcon((int)palette);
}

QImage JKQTPColorPaletteTools::getPaletteKeyImage(int i, int width, int height)
{
    QImage img;

    const double x01=double(width)/3.0;
    const double y01=double(height)/3.0*2.0;
    const double w1x=double(width*width)/(5.0*5.0);
    const double w1y=double(height*height)/(2.0*2.0);
    const double x02=double(width)/3.0*2.0;
    const double y02=double(height)/4.0;
    const double w2x=double(width*width)/(8.0*8.0);
    const double w2y=double(height*height)/(8.0*8.0);

    double* pic=(double*)malloc(width*height*sizeof(double));
    double mmax=0;
    for (int j=0; j<width*height; j++) {
        const int x=j%width;
        const int y=j/width;
        pic[j]=exp(-0.5*(double((x-x01)*double(x-x01))/w1x+double((y-y01)*double(y-y01))/w1y))+0.7*exp(-0.5*(double((x-x02)*double(x-x02))/w2x+double((y-y02)*double(y-y02))/w2y));
        if (pic[j]>mmax) mmax=pic[j];
    }
    JKQTPimagePlot_array2image<double>(pic, width, height, img, (JKQTPMathImageColorPalette)i, 0, mmax);
    free(pic);
    return img;
}

QImage JKQTPColorPaletteTools::getPaletteKeyImage(JKQTPMathImageColorPalette palette, int width, int height)
{
    return getPaletteKeyImage((int)palette, width, height);
}


void JKQTPColorPaletteTools::set_palette(int pal) {
    palette=(JKQTPMathImageColorPalette)pal;
}

void JKQTPColorPaletteTools::cbSetParent(JKQtBasePlotter* parent) {
    cbParent=parent;
    colorBarRightAxis->set_parent(parent);
    colorBarTopAxis->set_parent(parent);

}

double JKQTPColorPaletteTools::getInternalDataMin() const
{
    return 0;
}

double JKQTPColorPaletteTools::getInternalDataMax() const
{
    return 0;
}

#endif



