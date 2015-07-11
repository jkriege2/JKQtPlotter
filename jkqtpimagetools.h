/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate$  (revision $Rev$)

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


#ifndef JKQTPIMAGETOOLS_H
#define JKQTPIMAGETOOLS_H
#include <QIcon>
#include <QDebug>
#include <QImage>
#include <QComboBox>
#include <QStringList>
#include "jkqtp_imexport.h"
#include <cmath>
#include <cfloat>
#include <stdint.h>
#include <QColor>
#include "jkqtptools.h"

#ifndef NO_JKQTPLOTTER
#include "jkqtptools.h"

class JKQTPverticalIndependentAxis; // forward
class JKQTPhorizontalIndependentAxis; // forward
class JKQtBasePlotter; // forward
#endif

/*! \brief available palettes for coloring an image
    \ingroup jkqtplotter_imagelots
 */
enum JKQTPMathImageColorPalette {
    JKQTPMathImageGRAY=0, /*!< \image html GRAY.png */
    JKQTPMathImageINVERTEDGRAY, /*!< \image html GRAY.png */
    JKQTPMathImageRED, /*!< \image html RED.png */
    JKQTPMathImageINVERTEDRED, /*!< \image html RED.png */
    JKQTPMathImageGREEN, /*!< \image html GREEN.png */
    JKQTPMathImageINVERTEDGREEN, /*!< \image html GREEN.png */
    JKQTPMathImageBLUE, /*!< \image html BLUE.png */
    JKQTPMathImageINVERTEDBLUE, /*!< \image html BLUE.png */
    JKQTPMathImageCYAN, /*!< \image html CYAN.png */
    JKQTPMathImageINVERTED_CYAN, /*!< \image html INVERTED_CYAN.png */
    JKQTPMathImageYELLOW, /*!< \image html CYAN.png */
    JKQTPMathImageINVERTED_YELLOW, /*!< \image html INVERTED_CYAN.png */
    JKQTPMathImageMAGENTA, /*!< \image html CYAN.png */
    JKQTPMathImageINVERTED_MAGENTA, /*!< \image html INVERTED_CYAN.png */


    JKQTPMathImageMATLAB, /*!< \image html MATLAB.png */
    JKQTPMathImageINVERTED_MATLAB, /*!< \image html INVERTED_MATLAB.png */
    JKQTPMathImageRYGB, /*!< \image html RYGB.png */
    JKQTPMathImageINVERTED_RYGB, /*!< \image html INVERTED_RYGB.png */
    JKQTPMathImageHSV, /*!< \image html HSV.png */
    JKQTPMathImageINVERTED_HSV, /*!< \image html INVERTED_HSV.png */
    JKQTPMathImageRAINBOW, /*!< \image html RAINBOW.png */
    JKQTPMathImageINVERTED_RAINBOW, /*!< \image html INVERTED_RAINBOW.png */
    JKQTPMathImageHOT, /*!< \image html HOT.png */
    JKQTPMathImageINVERTED_HOT, /*!< \image html INVERTED_HOT.png */
    JKQTPMathImageOCEAN, /*!< \image html OCEAN.png */
    JKQTPMathImageINVERTED_OCEAN, /*!< \image html INVERTED_OCEAN.png */
    JKQTPMathImageTRAFFICLIGHT, /*!< \image html TRAFFICLIGHT.png */
    JKQTPMathImageINVERTED_TRAFFICLIGHT, /*!< \image html INVERTED_TRAFFICLIGHT.png */
    JKQTPMathImageBLUEMAGENTAYELLOW, /*!< \image html BLUEMAGENTAYELLOW.png */
    JKQTPMathImageINVERTED_BLUEMAGENTAYELLOW, /*!< \image html INVERTED_BLUEMAGENTAYELLOW.png */
    JKQTPMathImageBLUEYELLOW, /*!< \image html BLUEYELLOW.png */
    JKQTPMathImageINVERTED_BLUEYELLOW, /*!< \image html INVERTED_BLUEYELLOW.png */

    JKQTPMathImageBLUEWHITERED,
    JKQTPMathImageREDWHITEBLUE,

    JKQTPMathImageBLACKBLUEREDYELLOW,
    JKQTPMathImageGREENREDVIOLET,
    JKQTPMathImageBLACKBLUEVIOLETYELLOWWHITE,
    JKQTPMathImageWHITEYELLOWVIOLETBLUEBLACK,
    JKQTPMathImageBR_GR,
    JKQTPMathImagePU_OR,
    JKQTPMathImageGN_BU,
    JKQTPMathImageBU_GN,
    JKQTPMathImageYL_GN_BU,

    JKQTPMathImageBR_GR_STEP,
    JKQTPMathImagePU_OR_STEP,
    JKQTPMathImageGN_BU_STEP,
    JKQTPMathImageBU_GN_STEP,
    JKQTPMathImageYL_GN_BU_STEP,


    JKQTPMathImageCYANWHITE,
    JKQTPMathImageINVERTED_CYANWHITE,
    JKQTPMathImageYELLOWWHITE,
    JKQTPMathImageINVERTED_YELLOWWHITE,
    JKQTPMathImageMAGENTAWHITE,
    JKQTPMathImageINVERTED_MAGENTAWHITE,
    JKQTPMathImageBlueGreenRed,
    JKQTPMathImageRedGreenBlue,

    JKQTPMathImagePREDEFINED_PALETTES_COUNT,

    JKQTPMathImageUSER_PALETTE=65000,

    JKQTPMathImageALPHA=JKQTPMathImageUSER_PALETTE-2,
    JKQTPMathImageINVERTED_ALPHA=JKQTPMathImageUSER_PALETTE-1
};









/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPMathImageColorPaletteComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPMathImageColorPaletteComboBox(QWidget* parent=NULL);
        JKQTPMathImageColorPalette colorPalette() const;
        JKQTPMathImageColorPalette currentColorPalette() const;
        JKQTPMathImageColorPalette getColorPalette() const;
        void setColorPalette(JKQTPMathImageColorPalette palette);
        void setCurrentColorPalette(JKQTPMathImageColorPalette palette);
    signals:
        void currentPaletteChanged(JKQTPMathImageColorPalette palette);
    protected slots:
        void intIndexChanged(int i);

};

/*! \brief modes available for image pixels that are above/below the pixel value range
    \ingroup jkqtplotter_imagelots
 */
enum JKQTPMathImageColorRangeFailAction {
    JKQTPMathImageLastPaletteColor=0, /*!< set to last color in the palette */
    JKQTPMathImageGivenColor=1, /*!< set to the provided min/max color */
    JKQTPMathImageTransparent=2 /*!< set transparent */
};


/*! \brief modes available for RGB images
    \ingroup jkqtplotter_imagelots
 */
enum JKQTPRGBMathImageRGBMode {
    JKQTPRGBMathImageModeRGBMode=0,
    JKQTPRGBMathImageModeHSVMode=1,
    JKQTPRGBMathImageModeHSLMode=2,
    JKQTPRGBMathImageModeCMYMode=3
};


/*! \brief returns a vector containing all elements of the given array
    \ingroup jkqtplotter_imagelots

*/
template <class T>
inline QVector<T> JKQTPimagePlot_arrayToVector(const T* input, long long N) {
    if (!input || N<=0) return QVector<double>();
    T dummy;
    QVector<T> out(N, dummy);
    memcpy(out.data(), input, N*sizeof(T));
    return out;
}

/*! \brief returns a vector containing all elements of the given array as doubles
    \ingroup jkqtplotter_imagelots

*/
template <class T>
inline QVector<double> JKQTPimagePlot_arrayToDVector(const T* input, int N) {
    if (!input || N<=0) return QVector<double>();
    QVector<double> out(N, 0.0);
    for (int i=0; i<N; i++) {
        out[i]=input[i];
    }
    return out;
}

/*! \brief returns a vector containing all elements of the given boolean array as doubles (true=1, false=0)
    \ingroup jkqtplotter_imagelots

*/
inline QVector<double> JKQTPimagePlot_BarrayToDVector(const bool* input, int N) {
    if (!input || N<=0) return QVector<double>();
    QVector<double> out(N, 0.0);
    for (int i=0; i<N; i++) {
        if (input[i]) out[i]=1.0;
    }
    return out;
}

/*! \brief fin the minimum pixel value in the given image \a dbl with width \a width and height \a height
    \ingroup jkqtplotter_imagelots

 */
template <class T>
inline double JKQTPimagePlot_getImageMin(T* dbl, int width, int height)
{
    if (!dbl || width<=0 || height<=0)
            return 0;

    double min = 0;
    double max = 0;
    bool first=true;
    for (int i=1; i<width*height; ++i)
    {
        register T v=dbl[i];
        if (!(std::isnan(v) || std::isinf(v))) {
            if (first) {
                min=max=v;
                first=false;
            } else {
                if (v < min)
                    min = v;
                else if (v > max)
                    max = v;
            }
        }
    }
    return min;
};

/*! \brief fin the maximum pixel value in the given image \a dbl with width \a width and height \a height
    \ingroup jkqtplotter_imagelots

 */
template <class T>
inline double JKQTPimagePlot_getImageMax(T* dbl, int width, int height)
{
    if (!dbl || width<=0 || height<=0)
            return 0;

    double min = 0;
    double max = 0;
    bool first=true;
    for (int i=1; i<width*height; ++i)
    {
        register T v=dbl[i];
        if (!(std::isnan(v) || std::isinf(v))) {
            if (first) {
                min=max=v;
                first=false;
            } else {
                if (v < min)
                    min = v;
                else if (v > max)
                    max = v;
            }
        }
    }
    return max;
};









/*! \brief convert a 2D image (as 1D array) into a QImage and puts the image values into one color channel (set by \a channel).The other color channels are not changed! So a repeated call to this function for the SAME QImage will
           result in a step-by-step buildup of an image.
    \ingroup jkqtplotter_imagelots

    \note  All calls (except channel>=3, i.e. alpha) set alpha to 255. Only the call
           with channel==3 (alpha) sets alpha to the desired value. Calls with channel==4 (saturation), channel==5 (value)
           leave alpha as it is.
*/
template <class T>
inline void JKQTPimagePlot_array2RGBimage(T* dbl_in, int width, int height, QImage &img, int channel, T minColor, T maxColor, JKQTPRGBMathImageRGBMode rgbMode=JKQTPRGBMathImageModeRGBMode, bool logScale=false, double logBase=10.0)
{
    if (!dbl_in || width<=0 || height<=0)
        return;

    double min = *dbl_in;
    double max = *dbl_in;
    bool first=true;
    if (minColor == maxColor) {
        for (int i=1; i<width*height; ++i)
        {
            register T v=dbl_in[i];
            if (std::isfinite(v)) {
                if (first) {
                    min=max=v;
                    first=false;
                } else {
                    if (v < min)
                        min = v;
                    else if (v > max)
                        max = v;
                }
            }
        }
    } else {
        min = minColor;
        max = maxColor;
    }


    T* dbl=dbl_in;
    if (logScale) {
        double logB=log10(logBase);
        dbl=(T*)malloc(width*height*sizeof(T));
        //memcpy(dbl, dbl_in, width*height*sizeof(T));
        for (int i=0; i<width*height; i++) {
            dbl[i]=log10(dbl_in[i])/logB;
        }
        min=log10(min)/logB;
        max=log10(max)/logB;
    }
    double delta=max-min;

    if (min != max) {
        if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
            //qDebug()<<"RGBMode";
            if (channel==0) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const register QRgb l=line[i];
                        //if (j==5) qDebug()<<"r: "<<v<<qGreen(l)<<qBlue(l)<<qAlpha(255);
                        line[i]=qRgb(v,qGreen(l),qBlue(l));
                    }
                }
            } else if (channel==1) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const register QRgb l=line[i];
                        //if (j==5) qDebug()<<"g: "<<qRed(l)<<v<<qBlue(l)<<qAlpha(255);
                        line[i]=qRgb(qRed(l),v,qBlue(l));
                    }
                }
            } else if (channel==2) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const register QRgb l=line[i];
                        //if (j==5) qDebug()<<"b: "<<qRed(l)<<qGreen(l)<<v<<qAlpha(255);
                        line[i]=qRgb(qRed(l),qGreen(l),v);
                    }
                }
            }
        } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
            //qDebug()<<"RGBMode";
            if (channel==0) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const register QRgb l=line[i];
                        //if (j==5) qDebug()<<"r: "<<v<<qGreen(l)<<qBlue(l)<<qAlpha(255);
                        line[i]=QColor::fromCmyk(v,QColor(l).magenta(),QColor(l).yellow(),QColor(l).black()).rgba();
                    }
                }
            } else if (channel==1) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const register QRgb l=line[i];
                        //if (j==5) qDebug()<<"g: "<<qRed(l)<<v<<qBlue(l)<<qAlpha(255);
                        line[i]=QColor::fromCmyk(QColor(l).cyan(),v,QColor(l).yellow(),QColor(l).black()).rgba();
                    }
                }
            } else if (channel==2) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const register QRgb l=line[i];
                        //if (j==5) qDebug()<<"b: "<<qRed(l)<<qGreen(l)<<v<<qAlpha(255);
                        line[i]=QColor::fromCmyk(QColor(l).cyan(),QColor(l).magenta(),v,QColor(l).black()).rgba();
                    }
                }
            }
        } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
            if (channel==0) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = trunc(double(dbl[j*width+i]-min)*220.0/delta);
                        v = (v < 0) ? 0 : ( (v > 360) ? 360 : v);
                        QColor l=QColor::fromRgb(line[i]);
                        //if (i<10 && j==5) qDebug()<<"hi: "<<l.name()<<dbl[j*width+i]<<min<<max;
                        l.setHsv(v, l.saturation(), l.value());
                        //if (i<10 && j==5) qDebug()<<"ho: "<<l.name();
                        line[i]=l.rgb();
                    }
                }
            } else if (channel==1) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        QColor l=QColor::fromRgb(line[i]);
                        //if (i<10 && j==5) qDebug()<<"si: "<<l.name()<<dbl[j*width+i]<<min<<max;
                        l.setHsv(l.hue(), v, l.value());
                        //if (i<10 && j==5) qDebug()<<"so: "<<l.name();
                        line[i]=l.rgb();
                    }
                }
            } else if (channel==2) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        QColor l=QColor::fromRgb(line[i]);
                        //if (i<10 && j==5) qDebug()<<"vi: "<<l.name()<<dbl[j*width+i]<<min<<max;
                        l.setHsv(l.hue(), l.saturation(), v);
                        //if (i<10 && j==5) qDebug()<<"vo: "<<l.name();
                        line[i]=l.rgb();
                    }
                }

            }
        } else if (rgbMode==JKQTPRGBMathImageModeHSLMode) {
            if (channel==0) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                        v = (v < 0) ? 0 : ( (v > 360) ? 360 : v);
                        QColor l=line[i];
                        l.setHsl(v, l.saturation(), l.lightness());
                        line[i]=l.rgb();
                    }
                }
            } else if (channel==1) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        QColor l=line[i];
                        l.setHsl(l.hue(), v, l.lightness());
                        line[i]=l.rgb();
                    }
                }
            } else if (channel==2) {
                for (register int j=0; j<height; ++j) {
                    QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                    for (register int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        QColor l=line[i];
                        l.setHsl(l.hue(), l.saturation(), v);
                        line[i]=l.rgb();
                    }
                }

            }
        }
        if (channel==3) {
           for (register int j=0; j<height; ++j) {
               QRgb* line=(QRgb*)(img.scanLine(height-1-j));
               for (register int i=0; i<width; ++i) {
                   int v = (dbl[j*width+i]-min)*255/delta;
                   v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                   const register QRgb l=line[i];
                   line[i]=qRgba(qRed(l),qGreen(l),qBlue(l),v);
               }
           }
        } else if (channel==4) {
            for (register int j=0; j<height; ++j) {
                QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                for (register int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    QColor c=QColor::fromRgba(line[i]);
                    c.setHsv(c.hue(), v, c.value(), c.alpha());
                    line[i]=c.rgba();
                }
            }
        } else if (channel==5) {
            for (register int j=0; j<height; ++j) {
                QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                for (register int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    QColor c=QColor::fromRgba(line[i]);
                    c.setHsv(c.hue(), c.saturation(), v, c.alpha());
                    line[i]=c.rgba();
                }
            }
       }
     }

    if (logScale) free(dbl);
}


template <typename T>
inline QList<T> JKQTPimagePlot_makeQList(const T& defaultVal, int N=1) {
    QList<T> l;
    for (int i=0; i<N; i++) l<<defaultVal;
    return l;
}



/*! \brief size of the lookup tables used by JKQTFPimagePlot_array2image()
    \ingroup jkqtplotter_imagelots
*/
#define JKQTPimagePlot_LUTSIZE 256

void LIB_EXPORT JKQTPimagePlot_buildDefinedPaletteLinInterpolate(int* lut, QList<QPair<double, QRgb> > items, int lut_size=JKQTPimagePlot_LUTSIZE);

void LIB_EXPORT JKQTPimagePlot_buildDefinedPaletteLinInterpolate(int* lut, int N, ...);

void LIB_EXPORT JKQTPimagePlot_buildDefinedPalette(int* lut, QList<QPair<double, QRgb> > items);

void LIB_EXPORT JKQTPimagePlot_buildDefinedPalette(int* lut, int N, ...);

QStringList LIB_EXPORT JKQTPimagePlot_getPredefinedPalettes();



int*  LIB_EXPORT JKQTPimagePlot_getCreateLUT(QList<int *> &lutstore, JKQTPMathImageColorPalette palette);
void LIB_EXPORT JKQTPimagePlot_freeLUTs(QList<int *> &lutstore);


extern LIB_EXPORT QList<int*> global_jkqtpimagetools_lutstore;

/*! \brief convert a 2D image (as 1D array) into a QImage with given palette (see JKQTFPColorPalette)
    \ingroup jkqtplotter_imagelots

    This method uses lookup tables which are saved as static variables to convert the 2D array into
    an image. The luts are only created once, and stored then, so mor CPU time is saved. The precompiler define
    JKQTPimagePlot_LUTSIZE sets the size of the LUTs. Note that if you don't use a specific color palette,
    the according LUT won't be calculated and stored!

    \param dbl pointer to a 1D array of template type \c T representing the image to plot. This array has to be of size \a width * \a height
    \param width width of the array in \a dbl
    \param height height of the array in \a dbl
    \param[out] im the QImage object to draw to (should be initialized as \c QImage::Format_ARGB32 )
    \param palette the color palette to use for the display
    \param minColor lower boundary of color range in \a dbl pixels, if \a minColor == \a maxColor then this function will extract the image min and image max.
    \param maxColor upper boundary of color range in \a dbl pixels, if \a minColor == \a maxColor then this function will extract the image min and image max.
    \param paletteMinFail specifies what shell happen, when a value in \a dbl is below \a minColor
    \param paletteMaxFail specifies what shell happen, when a value in \a dbl is above \a maxColor
    \param minFailColor color to use for pixels that are below \a minColor for some settings of \a paletteMinFail
    \param maxFailColor color to use for pixels that are below \a maxColor for some settings of \a paletteMaxFail
    \param nanColor color to use for pixels that are not-a-number
    \param infColor color to use for pixels that are infinity
*/
template <class T>
inline void JKQTPimagePlot_array2image(T* dbl_in, int width, int height, QImage &img, JKQTPMathImageColorPalette palette, T minColor, T maxColor, JKQTPMathImageColorRangeFailAction paletteMinFail=JKQTPMathImageLastPaletteColor, JKQTPMathImageColorRangeFailAction paletteMaxFail=JKQTPMathImageLastPaletteColor, QColor minFailColor=QColor("black"), QColor maxFailColor=QColor("black"), QColor nanColor=QColor("black"), QColor infColor=QColor("black"), bool logScale=false, double logBase=10.0, int* lutUser=0, int lutUserSize=0)
{    
    if (!dbl_in || width<=0 || height<=0)
            return;

    double min = *dbl_in;
    double max = *dbl_in;
    if (minColor == maxColor) {
        bool first=true;
        for (int i=1; i<width*height; ++i)
        {
            register T v=dbl_in[i];
            if (!(std::isnan(v) || std::isinf(v))) {
                if (first) {
                    min=max=v;
                    first=false;
                } else {
                    if (v < min)
                        min = v;
                    else if (v > max)
                        max = v;
                }
            }
        }

    } else {
        min = minColor;
        max = maxColor;
    }

    T* dbl=dbl_in;
    if (logScale) {
        double logB=log10(logBase);
        dbl=(T*)malloc(width*height*sizeof(T));
        //memcpy(dbl, dbl_in, width*height*sizeof(T));
        for (int i=0; i<width*height; i++) {
            dbl[i]=log10(dbl_in[i])/logB;
        }
        min=log10(min)/logB;
        max=log10(max)/logB;
    }
    double delta=max-min;


    int* lut_used=NULL;
    int lutSize=JKQTPimagePlot_LUTSIZE;
    if (global_jkqtpimagetools_lutstore.size()<=0) global_jkqtpimagetools_lutstore=JKQTPimagePlot_makeQList<int*>(NULL, JKQTPimagePlot_getPredefinedPalettes().size()+2);


    img = QImage(width, height, QImage::Format_ARGB32);
    if (min == max)
            img.fill(0);
    else
    {

        if (palette==JKQTPMathImageUSER_PALETTE) {
            lut_used=lutUser;
            lutSize=lutUserSize;
            //qDebug()<<"user palette "<<lutUser<<lutUserSize;
        } else {
            lut_used=JKQTPimagePlot_getCreateLUT(global_jkqtpimagetools_lutstore, palette);
        }


        if (lut_used!=NULL && lutSize>0) {
            // LUT found: collor the image accordingly
            for (register int j=0; j<height; ++j) {
                QRgb* line=(QRgb*)(img.scanLine(height-1-j));
                for (register int i=0; i<width; ++i) {
                    register double val=dbl[j*width+i];
                    if (std::isnan(val)) {
                        line[i]=nanColor.rgba();
                    } else if (std::isinf(val)) {
                        line[i]=infColor.rgba();
                    } else {
                        register int v = (val-min)/delta*lutSize;
                        register int vv = (v < 0) ? 0 : ( (v > lutSize) ? (lutSize) : v);
                        line[i]=lut_used[vv];
                        if ((v<0)&&(paletteMinFail==JKQTPMathImageGivenColor)) {
                            line[i]=minFailColor.rgba();
                        } else if ((v>lutSize)&&(paletteMaxFail==JKQTPMathImageGivenColor)) {
                            line[i]=maxFailColor.rgba();
                        } else if ((v<0)&&(paletteMinFail==JKQTPMathImageTransparent)) {
                            line[i]=QColor(Qt::transparent).rgba();
                        } else if ((v>lutSize)&&(paletteMaxFail==JKQTPMathImageTransparent)) {
                            line[i]=QColor(Qt::transparent).rgba();
                        }
                    }
                }
            }
        } else {
            // no LUT found: paint a black image!
            img.fill(0);
        }
    }

    if (logScale) free(dbl);

};

template <class T>
inline void JKQTPimagePlot_array2image(T* dbl_in, int width, int height, QImage &img, int* lutUser, int lutUserSize, T minColor, T maxColor, JKQTPMathImageColorRangeFailAction paletteMinFail=JKQTPMathImageLastPaletteColor, JKQTPMathImageColorRangeFailAction paletteMaxFail=JKQTPMathImageLastPaletteColor, QColor minFailColor=QColor("black"), QColor maxFailColor=QColor("black"), QColor nanColor=QColor("black"), QColor infColor=QColor("black"), bool logScale=false, double logBase=10.0)
{
    JKQTPimagePlot_array2image(dbl_in,  width,  height, img,  JKQTPMathImageUSER_PALETTE, minColor, maxColor,  paletteMinFail,  paletteMaxFail,  minFailColor,  maxFailColor,  nanColor,  infColor,  logScale,  logBase, lutUser, lutUserSize);
}





QImage LIB_EXPORT JKQTPMathImageGetPaletteImage(int i, int width);
QImage LIB_EXPORT JKQTPMathImageGetPaletteImage(int i, int width, int height);
QImage LIB_EXPORT JKQTPMathImageGetPaletteImage(JKQTPMathImageColorPalette palette, int width);
QImage LIB_EXPORT JKQTPMathImageGetPaletteImage(JKQTPMathImageColorPalette palette, int width, int height);
QImage LIB_EXPORT JKQTPMathImageGetPaletteImage(int* lut, int lut_size, int width);
QImage LIB_EXPORT JKQTPMathImageGetAlphaPaletteImage(int* lut, int lut_size, int width, int height);

QIcon LIB_EXPORT JKQTPMathImageGetPaletteIcon(int i) ;

QIcon LIB_EXPORT JKQTPMathImageGetPaletteIcon(JKQTPMathImageColorPalette palette) ;



#ifndef NO_JKQTPLOTTER

/*! \brief if a class is derived from this class, it may use color bars
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPColorPaletteTools {
    public:

        JKQTPColorPaletteTools(JKQtBasePlotter *parent);
        virtual ~JKQTPColorPaletteTools();

        /*! \brief get list with all available palettes */
        static QStringList getPalettes() ;
        /*! \brief get list with all available palettes */
        static int getPalettesCount() ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteImage(int i, int width) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteImage(JKQTPMathImageColorPalette palette, int width) ;

        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(int i) ;
        /*! \brief get QIcon representing the given palette */
        static QIcon getPaletteKeyIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(int i, int width, int height) ;
        /*! \brief get QIcon representing the given palette */
        static QImage getPaletteKeyImage(JKQTPMathImageColorPalette palette, int width, int height) ;



        void set_palette(int pal);

        /*! \brief if the graph plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement drawOutside()
         */
        void cbGetOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace);

        /*! \brief plots outside the actual plot field of view (e.g. color bars, scale bars, ...)

            \note If you want to draw outside, then you'll also have to implement getOutsideSize(), so enough space is reserved

            The four value supplied tell the method where to draw (inside one of the rectangles).
         */
        void cbDrawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace);


        void cbSetParent(JKQtBasePlotter* parent);


        /** \brief determine min/max data value of the image */
        virtual void cbGetDataMinMax(double& imin, double& imax)=0;


        virtual double getInternalDataMin() const ;
        virtual double getInternalDataMax() const ;


    protected:
        JKQtBasePlotter* cbParent;


        /** \brief top color bar visible */
        bool colorBarTopVisible;
        /** \brief right color bar visible */
        bool colorBarRightVisible;
        /** \brief name of the image displayed above color bar (may contain LaTeX markup!) */
        QString imageName;
        /** \brief font name when displaying imageName */
        QString imageNameFontName;
        /** \brief font size in points when displaying imageName */
        double imageNameFontSize;
        /** \brief palette for plotting an image */
        JKQTPMathImageColorPalette palette;
        /** \brief indicate whether to display a color bar */
        bool showColorBar;
        /** \brief width of the color bar */
        int colorBarWidth;
        /** \brief height of the color bar, as multiple of plotHeight */
        double colorBarRelativeHeight;
        /** \brief indicates whether to estimate min/max of the image automatically */
        bool autoImageRange;
        /** \brief image value range minimum */
        double imageMin;
        /** \brief image value range maximum */
        double imageMax;
        /** \brief offset between outside space border and color bar */
        int colorBarOffset;
        /** \brief which action to take if a color is below \a imageMin and \a autoImageRange \c ==false */
        JKQTPMathImageColorRangeFailAction rangeMinFailAction;
        /** \brief which action to take if a color is above \a imageMax and \a autoImageRange \c ==false */
        JKQTPMathImageColorRangeFailAction rangeMaxFailAction;
        /** \brief color to use for some settings of \a rangeMinFailAction */
        QColor rangeMinFailColor;
        /** \brief color to use for some settings of \a rangeMaxFailAction */
        QColor rangeMaxFailColor;
        /** \brief color to use for a not-a-number value */
        QColor nanColor;
        /** \brief color to use for an infinity value */
        QColor infColor;


        /** \brief object used for color bar axes
         *
         *  \note this axis has some kind of a special role. It is used to format color bar axes
         */
        JKQTPverticalIndependentAxis* colorBarRightAxis;
        JKQTPhorizontalIndependentAxis* colorBarTopAxis;



    public:

        JKQTPGET_SET_MACRO(JKQTPMathImageColorPalette, palette)
        JKQTPGET_SET_MACRO(JKQTPMathImageColorRangeFailAction, rangeMinFailAction)
        JKQTPGET_SET_MACRO(JKQTPMathImageColorRangeFailAction, rangeMaxFailAction)
        JKQTPGET_SET_MACRO(QColor, rangeMinFailColor)
        JKQTPGET_SET_MACRO(QColor, rangeMaxFailColor)
        JKQTPGET_SET_MACRO(QColor, nanColor)
        JKQTPGET_SET_MACRO(QColor, infColor)
        JKQTPGET_SET_MACRO(bool, showColorBar)
        JKQTPGET_SET_MACRO(int, colorBarWidth)
        JKQTPGET_SET_MACRO(int, colorBarOffset)
        JKQTPGET_SET_MACRO(double, colorBarRelativeHeight)
        JKQTPGET_SET_MACRO(double, imageMin)
        JKQTPGET_SET_MACRO(double, imageMax)
        JKQTPGET_SET_MACRO(bool, autoImageRange)
        JKQTPGET_SET_MACRO(QString, imageName)
        JKQTPGET_SET_MACRO(QString, imageNameFontName)
        JKQTPGET_SET_MACRO(double, imageNameFontSize)
        JKQTPGET_MACRO(JKQTPverticalIndependentAxis*, colorBarRightAxis)
        JKQTPGET_MACRO(JKQTPhorizontalIndependentAxis*, colorBarTopAxis)
        JKQTPGET_SET_MACRO(bool, colorBarTopVisible)
        JKQTPGET_SET_MACRO(bool, colorBarRightVisible)

};
#endif


#endif // JKQTPIMAGETOOLS_H
