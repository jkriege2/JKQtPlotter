/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include <algorithm>
#ifdef QT_XML_LIB
#  include <QtXml/QtXml>
#endif
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#else
#include<QRegExp>
#endif

const int JKQTPImageTools::PALETTE_ICON_WIDTH = 64;
const int JKQTPImageTools::PALETTE_IMAGEICON_HEIGHT = 64;
const int JKQTPImageTools::LUTSIZE = 256;
const int JKQTPImageTools::NDEFAULTSTEPS = 5;

QMap<int, JKQTPImageTools::LUTData > JKQTPImageTools::global_jkqtpimagetools_lutstore = JKQTPImageTools::getDefaultLUTs();
int JKQTPImageTools::global_next_userpalette = JKQTPMathImageFIRST_REGISTERED_USER_PALETTE;
std::mutex JKQTPImageTools::lutMutex;












QMap<int, JKQTPImageTools::LUTData > JKQTPImageTools::getDefaultLUTs() {
    QMap<int, JKQTPImageTools::LUTData > lutstore;


    {
        const auto palette=JKQTPMathImageDefault_STEP;
        const QString palN="jkqtplotterdefault_step";
        const QString palNT=QObject::tr("steps: JKQTPlotter Default");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        JKQTPPaletteList lst;
        lst<<jkqtp_qRgbOpaque(0xBB0000);
        lst<<jkqtp_qRgbOpaque(0x00C11D);
        lst<<jkqtp_qRgbOpaque(0x0039D6);
        lst<<jkqtp_qRgbOpaque(0xFFDD00);
        lst<<jkqtp_qRgbOpaque(0xC05FFF);
        lst<<jkqtp_qRgbOpaque(0xDE7704);
        lst<<jkqtp_qRgbOpaque(0x03039A);


        lutstore[palette].lut=JKQTPBuildColorPaletteLUTColorsOnlySteps(lst);

    }



    lutstore[JKQTPMathImageRED]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::black),QColor(Qt::red)}), "red", QObject::tr("red"));
    lutstore[JKQTPMathImageINVERTEDRED]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::red),QColor(Qt::black)}), "invred", QObject::tr("inv. red"));
    lutstore[JKQTPMathImageGREEN]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::black),QColor(Qt::green)}), "green", QObject::tr("green"));
    lutstore[JKQTPMathImageINVERTEDGREEN]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::green),QColor(Qt::black)}), "invgreen", QObject::tr("inv. green"));
    lutstore[JKQTPMathImageBLUE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::black),QColor(Qt::blue)}), "blue", QObject::tr("blue"));
    lutstore[JKQTPMathImageINVERTEDBLUE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::blue),QColor(Qt::black)}), "invblue", QObject::tr("inv. blue"));
    lutstore[JKQTPMathImageGRAY]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::black),QColor(Qt::white)}), "gray", QObject::tr("gray"));
    lutstore[JKQTPMathImageINVERTEDGRAY]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::white),QColor(Qt::black)}), "invgray", QObject::tr("inv. gray"));
    lutstore[JKQTPMathImageALPHA]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(255,255,255,0),QColor(255,255,255,255)}), "alpha", QObject::tr("alpha"));
    lutstore[JKQTPMathImageINVERTED_ALPHA]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(255,255,255,255),QColor(255,255,255,0)}), "invalpha", QObject::tr("inv. alpha"));

    lutstore[JKQTPMathImageCYAN]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::black),QColor(Qt::cyan)}), "cyan", QObject::tr("cyan"));
    lutstore[JKQTPMathImageINVERTED_CYAN]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::cyan),QColor(Qt::black)}), "invcyan", QObject::tr("inv. cyan"));
    lutstore[JKQTPMathImageCYANWHITE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::cyan),QColor(Qt::white)}), "cyanwhite", QObject::tr("cyan-white"));
    lutstore[JKQTPMathImageINVERTED_CYANWHITE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::white),QColor(Qt::cyan)}), "whitecyan", QObject::tr("white-cyan"));

    lutstore[JKQTPMathImageMAGENTA]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::black),QColor(Qt::magenta)}), "magenta", QObject::tr("magenta"));
    lutstore[JKQTPMathImageINVERTED_MAGENTA]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::magenta),QColor(Qt::black)}), "invmagenta", QObject::tr("inv. magenta"));
    lutstore[JKQTPMathImageMAGENTAWHITE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::magenta),QColor(Qt::white)}), "magentawhite", QObject::tr("magenta-white"));
    lutstore[JKQTPMathImageINVERTED_MAGENTAWHITE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::white),QColor(Qt::magenta)}), "whitemagenta", QObject::tr("white-magenta"));

    lutstore[JKQTPMathImageYELLOW]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::black),QColor(Qt::yellow)}), "yellow", QObject::tr("yellow"));
    lutstore[JKQTPMathImageINVERTED_YELLOW]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::yellow),QColor(Qt::black)}), "invyellow", QObject::tr("inv. yellow"));
    lutstore[JKQTPMathImageYELLOWWHITE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::yellow),QColor(Qt::white)}), "yellowwhite", QObject::tr("yellow-white"));
    lutstore[JKQTPMathImageINVERTED_YELLOWWHITE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolateSorted({QColor(Qt::white),QColor(Qt::yellow)}), "whiteyellow", QObject::tr("white-yellow"));



    {
        const auto fR= [](float v) -> float { return  (382.5 - 1020.0 * std::abs(v - 0.75))/255.0; };
        const auto fG= [](float v) -> float { return  (382.5 - 1020.0 * std::abs(v - 0.5))/255.0; };
        const auto fB= [](float v) -> float { return  (382.5 - 1020.0 * std::abs(v - 0.25))/255.0; };
        const auto& normLUT=lutstore[JKQTPMathImageMATLAB]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "Matlab", QObject::tr("Matlab"));
        lutstore[JKQTPMathImageINVERTED_MATLAB]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invMatlab", QObject::tr("inv. Matlab"));

    }

    {
        const auto fR= [](float v) -> float { return  (796.875*v - 199.21875)/255.0; };
        const auto fG= [](float v) -> float { return  std::sin(JKQTPSTATISTICS_PI*v); };
        const auto fB= [](float v) -> float { return  (255.0 - 765.0 * v)/255.0; };
        const auto& normLUT=lutstore[JKQTPMathImageRYGB]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "RYGB", QObject::tr("RYGB"));
        lutstore[JKQTPMathImageINVERTED_RYGB]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invRYGB", QObject::tr("inv. RYGB"));

    }

    {
        const auto& normLUT=lutstore[JKQTPMathImageHSV]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT([](float v)->QRgb {
            const int h = static_cast<int>(floor(6.0*v));
            const double f = 6*v-double(h);

            switch (h)
            {
                case 0: return qRgb(255, static_cast<int>(255.0*f), 0);
                case 1: return qRgb(static_cast<int>(255.0*(1-f)), 255, 0);
                case 2: return qRgb(0, 255, static_cast<int>(255.0*f));
                case 3: return qRgb(0, static_cast<int>(255.0*(1-f)), 255);
                case 4: return qRgb(static_cast<int>(255.0*f), 0, 255);
                case 5: return qRgb(255, 0, static_cast<int>(255.0*(1-f)));
                case 6: return qRgb(255, static_cast<int>(255.0*f), 0);
                default: return qRgb(0,0,0);
            }
        }), "HSV", QObject::tr("HSV"));
        lutstore[JKQTPMathImageINVERTED_HSV]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invHSV", QObject::tr("inv. HSV"));

    }

    {
        const auto fR= [](float v) -> float { return  std::abs(2.0*v-0.5); };
        const auto fG= [](float v) -> float { return  sin(JKQTPSTATISTICS_PI*v); };
        const auto fB= [](float v) -> float { return  cos(0.5*JKQTPSTATISTICS_PI*v); };
        const auto& normLUT=lutstore[JKQTPMathImageRAINBOW]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "rainbow", QObject::tr("rainbow"));
        lutstore[JKQTPMathImageINVERTED_RAINBOW]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invrainbow", QObject::tr("inv. rainbow"));

    }

    {
        const auto fR= [](float v) -> float { return  765.0*v/255.0; };
        const auto fG= [](float v) -> float { return  (765.0*v-255.0)/255.0; };
        const auto fB= [](float v) -> float { return  (765.0*v-510.0)/255.0; };
        const auto& normLUT=lutstore[JKQTPMathImageHOT]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "AFMhot", QObject::tr("AFM hot"));
        lutstore[JKQTPMathImageINVERTED_HOT]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invAFMhot", QObject::tr("inv. AFM hot"));

    }

    {
        const auto fR= [](float v) -> float { return  (765.0*v-510.0)/255.0; };
        const auto fG= [](float v) -> float { return  (std::abs(382.5*v-127.5))/255.0; };
        const auto fB= [](float v) -> float { return  v; };
        const auto& normLUT=lutstore[JKQTPMathImageOCEAN]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "ocean", QObject::tr("ocean"));
        lutstore[JKQTPMathImageINVERTED_OCEAN]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invocean", QObject::tr("inv. ocean"));

    }

    {
        const auto fR= [](float v) -> float { return  v/0.32-0.78125; };
        const auto fG= [](float v) -> float { return  2.0*v-0.84; };
        const auto fB= [](float v) -> float {
            double b = 4.0*v;
            if (b>1 || b<0) b = -2.0*v+1.84;
            if (b>1 || b<0) b = v/0.08-11.5;
            if (b>1 || b<0) b=1;
            return  b;
        };
        const auto& normLUT=lutstore[JKQTPMathImageBLUEMAGENTAYELLOW]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "BlMaYe", QObject::tr("blue-magenta-yellow"));
        lutstore[JKQTPMathImageINVERTED_BLUEMAGENTAYELLOW]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "YeMaBl", QObject::tr("yellow-magenta-blue"));

    }

    {
        const auto fR= [](float v) -> float { return  sqrt(sqrt(v)); };
        const auto fG= [](float v) -> float { return  sin(JKQTPSTATISTICS_PI/2.0*v); };
        const auto fB= [](float v) -> float { return cos(JKQTPSTATISTICS_PI/2.0*v); };
        const auto& normLUT=lutstore[JKQTPMathImageBLUEYELLOW]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "BlYe", QObject::tr("blue-yellow"));
        lutstore[JKQTPMathImageINVERTED_BLUEYELLOW]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "YeBl", QObject::tr("yellow-blue"));

    }

    {
        const auto fR= [](float v) -> float { return  ((v < 0.5) ? 128.0*sin(JKQTPSTATISTICS_PI*(2.0*v-0.5))+128.0 : 255.0)/255.0; };
        const auto fG= [](float v) -> float { return  ((v < 0.5) ? 512.0*v+128.0 : 512.0-512.0*v)/255.0; };
        const auto fB= [](float v) -> float { return  0.0; };
        const auto& normLUT=lutstore[JKQTPMathImageTRAFFICLIGHT]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "Trafficlight", QObject::tr("Trafficlight"));
        lutstore[JKQTPMathImageINVERTED_TRAFFICLIGHT]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invtrafficlight", QObject::tr("inv. Trafficlight"));

    }

    {
        const auto fR= [](float v) -> float { return sqrt(v); };
        const auto fG= [](float v) -> float { return v*v*v; };
        const auto fB= [](float v) -> float { return sin(2.0*JKQTPSTATISTICS_PI*v); };
        const auto& normLUT=lutstore[JKQTPMathImageBLACKBLUEREDYELLOW]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "BBlRdYe", QObject::tr("black-blue-red-yellow"));
        lutstore[JKQTPMathImageYELLOWREDBLUEBLACK]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "YeRdBlB", QObject::tr("yellow-red-blue-black"));
    }

    {
        const auto fR= [](float v) -> float { return v; };
        const auto fG= [](float v) -> float { return fabs(v-0.5); };
        const auto fB= [](float v) -> float { return v*v*v*v; };
        const auto& normLUT=lutstore[JKQTPMathImageGREENREDVIOLET]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "GnRdVi", QObject::tr("green-red-violet"));
        lutstore[JKQTPMathImageVIOLETREDGREEN]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "ViRdGn", QObject::tr("violet-red-green"));
    }

    {
        const auto fR= [](float v) -> float { return v/0.32-0.78125; };
        const auto fG= [](float v) -> float { return v/0.32-0.78125; };
        const auto fB= [](float v) -> float { return (v<0.25)?4*v:(v<0.42)?1.0:(v<0.92)?-2.0*v+1.84:v/0.08-11.5; };
        const auto& normLUT=lutstore[JKQTPMathImageBLACKBLUEWHITEYELLOWWHITE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUT(fR, fG, fB), "BWprint", QObject::tr("black-blue-white-yellow-white"));
        lutstore[JKQTPMathImageWHITEYELLOWWHITEBLUEBLACK]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invBWprint", QObject::tr("white-yellow-white-blue-black"));
    }

    {
        JKQTPPaletteList lst {  jkqtp_qRgbOpaque(0xB2182B),
                                jkqtp_qRgbOpaque(0xD6604D),
                                jkqtp_qRgbOpaque(0xF4A582),
                                jkqtp_qRgbOpaque(0xFDDBC7),
                                jkqtp_qRgbOpaque(0xD1E5F0),
                                jkqtp_qRgbOpaque(0x92C5DE),
                                jkqtp_qRgbOpaque(0x4393C3),
                                jkqtp_qRgbOpaque(0x2166AC)
                             };
        const auto& normLUT=lutstore[JKQTPMathImageREDWHITEBLUE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst), "redwhiteblue", QObject::tr("red-white-blue (diverging)"));
        const auto& normStepLUT=lutstore[JKQTPMathImageREDWHITEBLUE_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "stepsredwhiteblue", QObject::tr("steps: red-white-blue (diverging)"));
        lutstore[JKQTPMathImageBLUEWHITERED]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "bluewhitered", QObject::tr("blue-white-red (diverging)"));
        lutstore[JKQTPMathImageBLUEWHITERED_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsbluewhitered", QObject::tr("steps: blue-white-red (diverging)"));
    }

    {
        JKQTPPaletteList lst {  QColor(49,54,149),
                                QColor(253,254,194),
                                QColor(165,0,38)
                             };
        const auto& normLUT=lutstore[JKQTPMathImageREDYELLOWBLUE]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst), "redyellowblue", QObject::tr("red-yellow-blue (diverging)"));
        const auto& normStepLUT=lutstore[JKQTPMathImageREDYELLOWBLUE_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst,9), "stepsredyellowblue", QObject::tr("steps: red-yellow-blue (diverging)"));
        lutstore[JKQTPMathImageBLUEYELLOWRED]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "blueyellowred", QObject::tr("blue-yellow-red (diverging)"));
        lutstore[JKQTPMathImageBLUEYELLOWRED_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsblueyellowred", QObject::tr("steps: blue-yellow-red (diverging)"));
    }

    {
        JKQTPPaletteList lst {  QColor(49,54,149),
                                QColor(253,254,194),
                                QColor(0,104,55)
                             };
        const auto& normLUT=lutstore[JKQTPMathImageRD_Yn_GN]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst), "RdYnGn", QObject::tr("red-yellow-green (diverging)"));
        const auto& normStepLUT=lutstore[JKQTPMathImageRD_Yn_GN_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst,9), "stepsRdYnGn", QObject::tr("steps: red-yellow-green (diverging)"));
        lutstore[JKQTPMathImageGN_Yn_RD]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "GnYnRd", QObject::tr("green-yellow-red (diverging)"));
        lutstore[JKQTPMathImageGN_Yn_RD_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsGnYnRd", QObject::tr("steps: green-yellow-red (diverging)"));
    }

    {
        const JKQTPPaletteList lst=  {
            jkqtp_qRgbOpaque(0x8C510A),
            jkqtp_qRgbOpaque(0xBF812D),
            jkqtp_qRgbOpaque(0xDFC27D),
            jkqtp_qRgbOpaque(0xF6E8C3),
            jkqtp_qRgbOpaque(0xC7EAE5),
            jkqtp_qRgbOpaque(0x80CDC1),
            jkqtp_qRgbOpaque(0x35978F),
            jkqtp_qRgbOpaque(0x01665E)
        };
        const auto& normLUT=lutstore[JKQTPMathImageBR_GR]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "BrGr", QObject::tr("brown-white-green (diverging)"));
        lutstore[JKQTPMathImageBR_GR].legacyNames<<"BrBG";
        const auto& normStepLUT=lutstore[JKQTPMathImageBR_GR_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "stepsBrGr", QObject::tr("steps: brown-white-green (diverging)"));
        lutstore[JKQTPMathImageBR_GR_STEP].legacyNames<<"stepsBrBG";
        lutstore[JKQTPMathImageGR_BR]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "GrBr", QObject::tr("green-white-brown (diverging)"));
        lutstore[JKQTPMathImageGR_BR_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsGrBr", QObject::tr("steps: green-white-brown (diverging)"));
    }

    {
        const JKQTPPaletteList lst=  {
            jkqtp_qRgbOpaque(0xB35806),
            jkqtp_qRgbOpaque(0xE08214),
            jkqtp_qRgbOpaque(0xFDB863),
            jkqtp_qRgbOpaque(0xFEE0B6),
            jkqtp_qRgbOpaque(0xF7F7F7),
            jkqtp_qRgbOpaque(0xD8DAEB),
            jkqtp_qRgbOpaque(0xB2ABD2),
            jkqtp_qRgbOpaque(0x8073AC),
            jkqtp_qRgbOpaque(0x542788)
        };
        const auto& normLUT=lutstore[JKQTPMathImagePU_OR]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "PuOr", QObject::tr("purple-white-orange (diverging)"));
        const auto& normStepLUT=lutstore[JKQTPMathImagePU_OR_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "stepsPuOr", QObject::tr("steps: purple-white-orange (diverging)"));
        lutstore[JKQTPMathImageOR_PU]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "OrPu", QObject::tr("orange-white-purple (diverging)"));
        lutstore[JKQTPMathImageOR_PU_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsOrPu", QObject::tr("steps: orange-white-purple (diverging)"));
    }

    {
        const JKQTPPaletteList lst=  {
            jkqtp_qRgbOpaque(0xFFFFD9),
            jkqtp_qRgbOpaque(0xEDF8B1),
            jkqtp_qRgbOpaque(0xC7E9B4),
            jkqtp_qRgbOpaque(0x7FCDBB),
            jkqtp_qRgbOpaque(0x41B6C4),
            jkqtp_qRgbOpaque(0x1D91C0),
            jkqtp_qRgbOpaque(0x225EA8),
            jkqtp_qRgbOpaque(0x253494),
            jkqtp_qRgbOpaque(0x081D58)
        };
        const auto& normLUT=lutstore[JKQTPMathImageYL_GN_BU]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "YeGnBu", QObject::tr("yellow-green-blue"));
        const auto& normStepLUT=lutstore[JKQTPMathImageYL_GN_BU_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "stepsYeGnBu", QObject::tr("steps: yellow-green-blue"));
        lutstore[JKQTPMathImageBU_GN_YL]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "BlGnYe", QObject::tr("blue-green-yellow"));
        lutstore[JKQTPMathImageBU_GN_YL_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsBlGnYe", QObject::tr("steps: blue-green-yellow"));
    }


    {
        const JKQTPPaletteList lst=  {
            jkqtp_qRgbOpaque(0xF7FCF0),
            jkqtp_qRgbOpaque(0xE0F3DB),
            jkqtp_qRgbOpaque(0xCCEBC5),
            jkqtp_qRgbOpaque(0xA8DDB5),
            jkqtp_qRgbOpaque(0x7BCCC4),
            jkqtp_qRgbOpaque(0x4EB3D3),
            jkqtp_qRgbOpaque(0x2B8CBE),
            jkqtp_qRgbOpaque(0x0868AC),
            jkqtp_qRgbOpaque(0x084081)
        };
        const auto& normLUT=lutstore[JKQTPMathImageBU_GN]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "bluegreen", QObject::tr("blue-green-white"));
        const auto& normStepLUT=lutstore[JKQTPMathImageBU_GN_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "stepsBlGn", QObject::tr("steps: blue-green-white"));
        lutstore[JKQTPMathImageGN_BU]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "BlGnYe", QObject::tr("white-green-blue"));
        lutstore[JKQTPMathImageGN_BU_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsBlGnYe", QObject::tr("steps: white-green-blue"));
    }

    {
        const JKQTPPaletteList lst=  {
            QColor("blue"),
            QColor("green"),
            QColor("red")
        };
        const auto& normLUT=lutstore[JKQTPMathImageBlueGreenRed]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "BlGnRd", QObject::tr("blue-green-red"));
        lutstore[JKQTPMathImageBlueGreenRed].legacyNames<<"bluegreenred";
        lutstore[JKQTPMathImageRedGreenBlue]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "RdGnBu", QObject::tr("red-green-blue"));
        lutstore[JKQTPMathImageRedGreenBlue].legacyNames<<"redgreenblue";
    }

    {
        const JKQTPPaletteList lst=  {
            QColor("magenta"),
            QColor("yellow")
        };
        const auto& normLUT=lutstore[JKQTPMathImageMagentaYellow]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "magentayellow", QObject::tr("magenta-yellow"));
        lutstore[JKQTPMathImageYellowMagenta]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "yellowmagenta", QObject::tr("yellow-magenta"));
    }

    {
        const JKQTPPaletteList lst=  {
            QColor("red"),
            QColor("blue")
        };
        const auto& normLUT=lutstore[JKQTPMathImageRedBlue]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "redblue", QObject::tr("red-blue"));
        lutstore[JKQTPMathImageBlueRed]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "bluered", QObject::tr("blue-red"));
    }

    {
        const JKQTPPaletteList lst=  {
            QColor(142,1,82),
            QColor(246,246,246),
            QColor(39,100,25)
        };
        const auto& normLUT=lutstore[JKQTPMathImagePI_W_GR]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "PiWGr", QObject::tr("pink-white-green (diverging)"));
        const auto& normStepLUT=lutstore[JKQTPMathImagePI_W_GR_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst,9), "stepsPiWGr", QObject::tr("steps: pink-white-green (diverging)"));
        lutstore[JKQTPMathImageGR_W_PI]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "GrWPi", QObject::tr("green-wite-pink (diverging)"));
        lutstore[JKQTPMathImageGR_W_PI_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsGrWPi", QObject::tr("steps: green-wite-pink (diverging)"));
    }


    {
        const JKQTPPaletteList lst=  {
            QColor(103,0,31),
            QColor(254,254,254),
            QColor(26,26,26)
        };
        const auto& normLUT=lutstore[JKQTPMathImageRD_W_GY]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "RdWGy", QObject::tr("red-white-gray (diverging)"));
        const auto& normStepLUT=lutstore[JKQTPMathImageRD_W_GY_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst,9), "stepsRdWGy", QObject::tr("steps: red-white-gray (diverging)"));
        lutstore[JKQTPMathImageGY_W_RD]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "GyWRd", QObject::tr("gray-white-red (diverging)"));
        lutstore[JKQTPMathImageGY_W_RD_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsGyWRd", QObject::tr("steps: gray-wite-red (diverging)"));
    }

    {
        const JKQTPPaletteList lst=  {
            QColor(58,76,192),
            QColor(220,220,220),
            QColor(179,3,38)
        };
        const auto& normLUT=lutstore[JKQTPMathImageCoolwarm]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "coolwarm", QObject::tr("coolwarm (diverging)"));
        const auto& normStepLUT=lutstore[JKQTPMathImageCoolwarm_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst,9), "stepscoolwarm", QObject::tr("steps: coolwarm (diverging)"));
        lutstore[JKQTPMathImageINVERTED_Coolwarm]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invcoolwarm", QObject::tr("inv. coolwarm (diverging)"));
        lutstore[JKQTPMathImageINVERTED_Coolwarm_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsinvcoolwarm", QObject::tr("steps: inv. coolwarm (diverging)"));
    }

    {
        const JKQTPPaletteList lst=  {
            QColor::fromRgbF(0.0f, 0.0f, 0.3f),
            QColor::fromRgbF(0.0f, 0.0f, 1.0f),
            QColor::fromRgbF(1.0f, 1.0f, 1.0f),
            QColor::fromRgbF(1.0f, 0.0f, 0.0f),
            QColor::fromRgbF(0.5f, 0.0f, 0.0f)
        };
        const auto& normLUT=lutstore[JKQTPMathImageSeismic]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "seismic", QObject::tr("seismic"));
        const auto& normStepLUT=lutstore[JKQTPMathImageSeismic_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "stepsseismic", QObject::tr("steps: seismic"));
        lutstore[JKQTPMathImageSeismic_STEP].legacyNames<<"seismic_step";
        lutstore[JKQTPMathImageINVERTED_Seismic]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invseismic", QObject::tr("inv. seismic"));
        lutstore[JKQTPMathImageINVERTED_Seismic_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsinvseismic", QObject::tr("steps: inv. seismic"));
    }

    {
        JKQTPPaletteList lst;
        lst<<qMakePair<double, QRgb>(0.00, QColor::fromRgbF(0.2f, 0.2f, 0.6f).rgb());
        lst<<qMakePair<double, QRgb>(0.15, QColor::fromRgbF(0.0f, 0.6f, 1.0f).rgb());
        lst<<qMakePair<double, QRgb>(0.25, QColor::fromRgbF(0.0f, 0.8f, 0.4f).rgb());
        lst<<qMakePair<double, QRgb>(0.50, QColor::fromRgbF(1.0f, 1.0f, 0.6f).rgb());
        lst<<qMakePair<double, QRgb>(0.75, QColor::fromRgbF(0.5f, 0.36f, 0.33f).rgb());
        lst<<qMakePair<double, QRgb>(1.00, QColor::fromRgbF(1.0f, 1.0f, 1.0f).rgb());
        const auto& normLUT=lutstore[JKQTPMathImageTerrain]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "terrain", QObject::tr("terrain"));
        const auto& normStepLUT=lutstore[JKQTPMathImageTerrain_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "stepsterrain", QObject::tr("steps: terrain"));
        lutstore[JKQTPMathImageTerrain_STEP].legacyNames<<"terrain_step";
        lutstore[JKQTPMathImageINVERTED_Terrain]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invterrain", QObject::tr("inv. terrain"));
        lutstore[JKQTPMathImageINVERTED_Terrain_STEP]=JKQTPImageTools::LUTData(jkqtp_reversed(normStepLUT.lut), "stepsinvterrain", QObject::tr("steps: inv. terrain"));
    }


    {
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
        const auto& normLUT=lutstore[JKQTPMathImageBone]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB),  "bone", QObject::tr("bone"));
        lutstore[JKQTPMathImageINVERTED_Bone]=JKQTPImageTools::LUTData(jkqtp_reversed(normLUT.lut), "invbone", QObject::tr("inv. bone"));
    }


    {
        QList<JKQTPColorPaletteSingleColorLinSegment> lstR,lstG,lstB;
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        0.0,      0.0);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      1.0,      1.0);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.0,      0.0);

        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      1.0,      1.0);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        const auto& lutNorm=lutstore[JKQTPMathImageCool]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB), "cool", QObject::tr("cool"));
        lutstore[JKQTPMathImageINVERTED_Cool]=JKQTPImageTools::LUTData(jkqtp_reversed(lutNorm.lut), "invcool", QObject::tr("inv. cool"));
    }

    {
        QList<JKQTPColorPaletteSingleColorLinSegment> lstR,lstG,lstB;
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        0.0,      0.0);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.809524, 1.000000, 1.000000);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.7812,   0.7812);

        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.4975,   0.4975);

        const auto& lutNorm=lutstore[JKQTPMathImageCopper]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB), "copper", QObject::tr("copper"));
        lutstore[JKQTPMathImageINVERTED_Copper]=JKQTPImageTools::LUTData(jkqtp_reversed(lutNorm.lut), "invcopper", QObject::tr("inv. copper"));
    }

    {
        QList<JKQTPColorPaletteSingleColorLinSegment> lstR,lstG,lstB;
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0,        1.0,      1.0);
        lstR<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstG<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      1.0,      1.0);

        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(0.0,      0.0,      0.0);
        lstB<<JKQTPColorPaletteSingleColorLinSegment::makeDbl_0_1(1.0,      0.0,      0.0);

        const auto& lutNorm=lutstore[JKQTPMathImageAutumn]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinSegmentsSorted(lstR, lstG, lstB), "autumn", QObject::tr("autumn"));
        lutstore[JKQTPMathImageINVERTED_Autumn]=JKQTPImageTools::LUTData(jkqtp_reversed(lutNorm.lut), "invautumn", QObject::tr("inv. autumn"));
    }


    {
        // from https://github.com/BIDS/colormap/blob/master/colormaps.py
        // https://github.com/BIDS/colormap/blob/master/colormaps.py
        const auto palette=JKQTPMathImageViridis;
        const QString palN="viridis";
        const QString palNT=QObject::tr("viridis");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        lutstore[palette].lut=LUTType{
                         QColor::fromRgbF(0.267004f, 0.004874f, 0.329415f).rgb(),
                         QColor::fromRgbF(0.268510f, 0.009605f, 0.335427f).rgb(),
                         QColor::fromRgbF(0.269944f, 0.014625f, 0.341379f).rgb(),
                         QColor::fromRgbF(0.271305f, 0.019942f, 0.347269f).rgb(),
                         QColor::fromRgbF(0.272594f, 0.025563f, 0.353093f).rgb(),
                         QColor::fromRgbF(0.273809f, 0.031497f, 0.358853f).rgb(),
                         QColor::fromRgbF(0.274952f, 0.037752f, 0.364543f).rgb(),
                         QColor::fromRgbF(0.276022f, 0.044167f, 0.370164f).rgb(),
                         QColor::fromRgbF(0.277018f, 0.050344f, 0.375715f).rgb(),
                         QColor::fromRgbF(0.277941f, 0.056324f, 0.381191f).rgb(),
                         QColor::fromRgbF(0.278791f, 0.062145f, 0.386592f).rgb(),
                         QColor::fromRgbF(0.279566f, 0.067836f, 0.391917f).rgb(),
                         QColor::fromRgbF(0.280267f, 0.073417f, 0.397163f).rgb(),
                         QColor::fromRgbF(0.280894f, 0.078907f, 0.402329f).rgb(),
                         QColor::fromRgbF(0.281446f, 0.084320f, 0.407414f).rgb(),
                         QColor::fromRgbF(0.281924f, 0.089666f, 0.412415f).rgb(),
                         QColor::fromRgbF(0.282327f, 0.094955f, 0.417331f).rgb(),
                         QColor::fromRgbF(0.282656f, 0.100196f, 0.422160f).rgb(),
                         QColor::fromRgbF(0.282910f, 0.105393f, 0.426902f).rgb(),
                         QColor::fromRgbF(0.283091f, 0.110553f, 0.431554f).rgb(),
                         QColor::fromRgbF(0.283197f, 0.115680f, 0.436115f).rgb(),
                         QColor::fromRgbF(0.283229f, 0.120777f, 0.440584f).rgb(),
                         QColor::fromRgbF(0.283187f, 0.125848f, 0.444960f).rgb(),
                         QColor::fromRgbF(0.283072f, 0.130895f, 0.449241f).rgb(),
                         QColor::fromRgbF(0.282884f, 0.135920f, 0.453427f).rgb(),
                         QColor::fromRgbF(0.282623f, 0.140926f, 0.457517f).rgb(),
                         QColor::fromRgbF(0.282290f, 0.145912f, 0.461510f).rgb(),
                         QColor::fromRgbF(0.281887f, 0.150881f, 0.465405f).rgb(),
                         QColor::fromRgbF(0.281412f, 0.155834f, 0.469201f).rgb(),
                         QColor::fromRgbF(0.280868f, 0.160771f, 0.472899f).rgb(),
                         QColor::fromRgbF(0.280255f, 0.165693f, 0.476498f).rgb(),
                         QColor::fromRgbF(0.279574f, 0.170599f, 0.479997f).rgb(),
                         QColor::fromRgbF(0.278826f, 0.175490f, 0.483397f).rgb(),
                         QColor::fromRgbF(0.278012f, 0.180367f, 0.486697f).rgb(),
                         QColor::fromRgbF(0.277134f, 0.185228f, 0.489898f).rgb(),
                         QColor::fromRgbF(0.276194f, 0.190074f, 0.493001f).rgb(),
                         QColor::fromRgbF(0.275191f, 0.194905f, 0.496005f).rgb(),
                         QColor::fromRgbF(0.274128f, 0.199721f, 0.498911f).rgb(),
                         QColor::fromRgbF(0.273006f, 0.204520f, 0.501721f).rgb(),
                         QColor::fromRgbF(0.271828f, 0.209303f, 0.504434f).rgb(),
                         QColor::fromRgbF(0.270595f, 0.214069f, 0.507052f).rgb(),
                         QColor::fromRgbF(0.269308f, 0.218818f, 0.509577f).rgb(),
                         QColor::fromRgbF(0.267968f, 0.223549f, 0.512008f).rgb(),
                         QColor::fromRgbF(0.266580f, 0.228262f, 0.514349f).rgb(),
                         QColor::fromRgbF(0.265145f, 0.232956f, 0.516599f).rgb(),
                         QColor::fromRgbF(0.263663f, 0.237631f, 0.518762f).rgb(),
                         QColor::fromRgbF(0.262138f, 0.242286f, 0.520837f).rgb(),
                         QColor::fromRgbF(0.260571f, 0.246922f, 0.522828f).rgb(),
                         QColor::fromRgbF(0.258965f, 0.251537f, 0.524736f).rgb(),
                         QColor::fromRgbF(0.257322f, 0.256130f, 0.526563f).rgb(),
                         QColor::fromRgbF(0.255645f, 0.260703f, 0.528312f).rgb(),
                         QColor::fromRgbF(0.253935f, 0.265254f, 0.529983f).rgb(),
                         QColor::fromRgbF(0.252194f, 0.269783f, 0.531579f).rgb(),
                         QColor::fromRgbF(0.250425f, 0.274290f, 0.533103f).rgb(),
                         QColor::fromRgbF(0.248629f, 0.278775f, 0.534556f).rgb(),
                         QColor::fromRgbF(0.246811f, 0.283237f, 0.535941f).rgb(),
                         QColor::fromRgbF(0.244972f, 0.287675f, 0.537260f).rgb(),
                         QColor::fromRgbF(0.243113f, 0.292092f, 0.538516f).rgb(),
                         QColor::fromRgbF(0.241237f, 0.296485f, 0.539709f).rgb(),
                         QColor::fromRgbF(0.239346f, 0.300855f, 0.540844f).rgb(),
                         QColor::fromRgbF(0.237441f, 0.305202f, 0.541921f).rgb(),
                         QColor::fromRgbF(0.235526f, 0.309527f, 0.542944f).rgb(),
                         QColor::fromRgbF(0.233603f, 0.313828f, 0.543914f).rgb(),
                         QColor::fromRgbF(0.231674f, 0.318106f, 0.544834f).rgb(),
                         QColor::fromRgbF(0.229739f, 0.322361f, 0.545706f).rgb(),
                         QColor::fromRgbF(0.227802f, 0.326594f, 0.546532f).rgb(),
                         QColor::fromRgbF(0.225863f, 0.330805f, 0.547314f).rgb(),
                         QColor::fromRgbF(0.223925f, 0.334994f, 0.548053f).rgb(),
                         QColor::fromRgbF(0.221989f, 0.339161f, 0.548752f).rgb(),
                         QColor::fromRgbF(0.220057f, 0.343307f, 0.549413f).rgb(),
                         QColor::fromRgbF(0.218130f, 0.347432f, 0.550038f).rgb(),
                         QColor::fromRgbF(0.216210f, 0.351535f, 0.550627f).rgb(),
                         QColor::fromRgbF(0.214298f, 0.355619f, 0.551184f).rgb(),
                         QColor::fromRgbF(0.212395f, 0.359683f, 0.551710f).rgb(),
                         QColor::fromRgbF(0.210503f, 0.363727f, 0.552206f).rgb(),
                         QColor::fromRgbF(0.208623f, 0.367752f, 0.552675f).rgb(),
                         QColor::fromRgbF(0.206756f, 0.371758f, 0.553117f).rgb(),
                         QColor::fromRgbF(0.204903f, 0.375746f, 0.553533f).rgb(),
                         QColor::fromRgbF(0.203063f, 0.379716f, 0.553925f).rgb(),
                         QColor::fromRgbF(0.201239f, 0.383670f, 0.554294f).rgb(),
                         QColor::fromRgbF(0.199430f, 0.387607f, 0.554642f).rgb(),
                         QColor::fromRgbF(0.197636f, 0.391528f, 0.554969f).rgb(),
                         QColor::fromRgbF(0.195860f, 0.395433f, 0.555276f).rgb(),
                         QColor::fromRgbF(0.194100f, 0.399323f, 0.555565f).rgb(),
                         QColor::fromRgbF(0.192357f, 0.403199f, 0.555836f).rgb(),
                         QColor::fromRgbF(0.190631f, 0.407061f, 0.556089f).rgb(),
                         QColor::fromRgbF(0.188923f, 0.410910f, 0.556326f).rgb(),
                         QColor::fromRgbF(0.187231f, 0.414746f, 0.556547f).rgb(),
                         QColor::fromRgbF(0.185556f, 0.418570f, 0.556753f).rgb(),
                         QColor::fromRgbF(0.183898f, 0.422383f, 0.556944f).rgb(),
                         QColor::fromRgbF(0.182256f, 0.426184f, 0.557120f).rgb(),
                         QColor::fromRgbF(0.180629f, 0.429975f, 0.557282f).rgb(),
                         QColor::fromRgbF(0.179019f, 0.433756f, 0.557430f).rgb(),
                         QColor::fromRgbF(0.177423f, 0.437527f, 0.557565f).rgb(),
                         QColor::fromRgbF(0.175841f, 0.441290f, 0.557685f).rgb(),
                         QColor::fromRgbF(0.174274f, 0.445044f, 0.557792f).rgb(),
                         QColor::fromRgbF(0.172719f, 0.448791f, 0.557885f).rgb(),
                         QColor::fromRgbF(0.171176f, 0.452530f, 0.557965f).rgb(),
                         QColor::fromRgbF(0.169646f, 0.456262f, 0.558030f).rgb(),
                         QColor::fromRgbF(0.168126f, 0.459988f, 0.558082f).rgb(),
                         QColor::fromRgbF(0.166617f, 0.463708f, 0.558119f).rgb(),
                         QColor::fromRgbF(0.165117f, 0.467423f, 0.558141f).rgb(),
                         QColor::fromRgbF(0.163625f, 0.471133f, 0.558148f).rgb(),
                         QColor::fromRgbF(0.162142f, 0.474838f, 0.558140f).rgb(),
                         QColor::fromRgbF(0.160665f, 0.478540f, 0.558115f).rgb(),
                         QColor::fromRgbF(0.159194f, 0.482237f, 0.558073f).rgb(),
                         QColor::fromRgbF(0.157729f, 0.485932f, 0.558013f).rgb(),
                         QColor::fromRgbF(0.156270f, 0.489624f, 0.557936f).rgb(),
                         QColor::fromRgbF(0.154815f, 0.493313f, 0.557840f).rgb(),
                         QColor::fromRgbF(0.153364f, 0.497000f, 0.557724f).rgb(),
                         QColor::fromRgbF(0.151918f, 0.500685f, 0.557587f).rgb(),
                         QColor::fromRgbF(0.150476f, 0.504369f, 0.557430f).rgb(),
                         QColor::fromRgbF(0.149039f, 0.508051f, 0.557250f).rgb(),
                         QColor::fromRgbF(0.147607f, 0.511733f, 0.557049f).rgb(),
                         QColor::fromRgbF(0.146180f, 0.515413f, 0.556823f).rgb(),
                         QColor::fromRgbF(0.144759f, 0.519093f, 0.556572f).rgb(),
                         QColor::fromRgbF(0.143343f, 0.522773f, 0.556295f).rgb(),
                         QColor::fromRgbF(0.141935f, 0.526453f, 0.555991f).rgb(),
                         QColor::fromRgbF(0.140536f, 0.530132f, 0.555659f).rgb(),
                         QColor::fromRgbF(0.139147f, 0.533812f, 0.555298f).rgb(),
                         QColor::fromRgbF(0.137770f, 0.537492f, 0.554906f).rgb(),
                         QColor::fromRgbF(0.136408f, 0.541173f, 0.554483f).rgb(),
                         QColor::fromRgbF(0.135066f, 0.544853f, 0.554029f).rgb(),
                         QColor::fromRgbF(0.133743f, 0.548535f, 0.553541f).rgb(),
                         QColor::fromRgbF(0.132444f, 0.552216f, 0.553018f).rgb(),
                         QColor::fromRgbF(0.131172f, 0.555899f, 0.552459f).rgb(),
                         QColor::fromRgbF(0.129933f, 0.559582f, 0.551864f).rgb(),
                         QColor::fromRgbF(0.128729f, 0.563265f, 0.551229f).rgb(),
                         QColor::fromRgbF(0.127568f, 0.566949f, 0.550556f).rgb(),
                         QColor::fromRgbF(0.126453f, 0.570633f, 0.549841f).rgb(),
                         QColor::fromRgbF(0.125394f, 0.574318f, 0.549086f).rgb(),
                         QColor::fromRgbF(0.124395f, 0.578002f, 0.548287f).rgb(),
                         QColor::fromRgbF(0.123463f, 0.581687f, 0.547445f).rgb(),
                         QColor::fromRgbF(0.122606f, 0.585371f, 0.546557f).rgb(),
                         QColor::fromRgbF(0.121831f, 0.589055f, 0.545623f).rgb(),
                         QColor::fromRgbF(0.121148f, 0.592739f, 0.544641f).rgb(),
                         QColor::fromRgbF(0.120565f, 0.596422f, 0.543611f).rgb(),
                         QColor::fromRgbF(0.120092f, 0.600104f, 0.542530f).rgb(),
                         QColor::fromRgbF(0.119738f, 0.603785f, 0.541400f).rgb(),
                         QColor::fromRgbF(0.119512f, 0.607464f, 0.540218f).rgb(),
                         QColor::fromRgbF(0.119423f, 0.611141f, 0.538982f).rgb(),
                         QColor::fromRgbF(0.119483f, 0.614817f, 0.537692f).rgb(),
                         QColor::fromRgbF(0.119699f, 0.618490f, 0.536347f).rgb(),
                         QColor::fromRgbF(0.120081f, 0.622161f, 0.534946f).rgb(),
                         QColor::fromRgbF(0.120638f, 0.625828f, 0.533488f).rgb(),
                         QColor::fromRgbF(0.121380f, 0.629492f, 0.531973f).rgb(),
                         QColor::fromRgbF(0.122312f, 0.633153f, 0.530398f).rgb(),
                         QColor::fromRgbF(0.123444f, 0.636809f, 0.528763f).rgb(),
                         QColor::fromRgbF(0.124780f, 0.640461f, 0.527068f).rgb(),
                         QColor::fromRgbF(0.126326f, 0.644107f, 0.525311f).rgb(),
                         QColor::fromRgbF(0.128087f, 0.647749f, 0.523491f).rgb(),
                         QColor::fromRgbF(0.130067f, 0.651384f, 0.521608f).rgb(),
                         QColor::fromRgbF(0.132268f, 0.655014f, 0.519661f).rgb(),
                         QColor::fromRgbF(0.134692f, 0.658636f, 0.517649f).rgb(),
                         QColor::fromRgbF(0.137339f, 0.662252f, 0.515571f).rgb(),
                         QColor::fromRgbF(0.140210f, 0.665859f, 0.513427f).rgb(),
                         QColor::fromRgbF(0.143303f, 0.669459f, 0.511215f).rgb(),
                         QColor::fromRgbF(0.146616f, 0.673050f, 0.508936f).rgb(),
                         QColor::fromRgbF(0.150148f, 0.676631f, 0.506589f).rgb(),
                         QColor::fromRgbF(0.153894f, 0.680203f, 0.504172f).rgb(),
                         QColor::fromRgbF(0.157851f, 0.683765f, 0.501686f).rgb(),
                         QColor::fromRgbF(0.162016f, 0.687316f, 0.499129f).rgb(),
                         QColor::fromRgbF(0.166383f, 0.690856f, 0.496502f).rgb(),
                         QColor::fromRgbF(0.170948f, 0.694384f, 0.493803f).rgb(),
                         QColor::fromRgbF(0.175707f, 0.697900f, 0.491033f).rgb(),
                         QColor::fromRgbF(0.180653f, 0.701402f, 0.488189f).rgb(),
                         QColor::fromRgbF(0.185783f, 0.704891f, 0.485273f).rgb(),
                         QColor::fromRgbF(0.191090f, 0.708366f, 0.482284f).rgb(),
                         QColor::fromRgbF(0.196571f, 0.711827f, 0.479221f).rgb(),
                         QColor::fromRgbF(0.202219f, 0.715272f, 0.476084f).rgb(),
                         QColor::fromRgbF(0.208030f, 0.718701f, 0.472873f).rgb(),
                         QColor::fromRgbF(0.214000f, 0.722114f, 0.469588f).rgb(),
                         QColor::fromRgbF(0.220124f, 0.725509f, 0.466226f).rgb(),
                         QColor::fromRgbF(0.226397f, 0.728888f, 0.462789f).rgb(),
                         QColor::fromRgbF(0.232815f, 0.732247f, 0.459277f).rgb(),
                         QColor::fromRgbF(0.239374f, 0.735588f, 0.455688f).rgb(),
                         QColor::fromRgbF(0.246070f, 0.738910f, 0.452024f).rgb(),
                         QColor::fromRgbF(0.252899f, 0.742211f, 0.448284f).rgb(),
                         QColor::fromRgbF(0.259857f, 0.745492f, 0.444467f).rgb(),
                         QColor::fromRgbF(0.266941f, 0.748751f, 0.440573f).rgb(),
                         QColor::fromRgbF(0.274149f, 0.751988f, 0.436601f).rgb(),
                         QColor::fromRgbF(0.281477f, 0.755203f, 0.432552f).rgb(),
                         QColor::fromRgbF(0.288921f, 0.758394f, 0.428426f).rgb(),
                         QColor::fromRgbF(0.296479f, 0.761561f, 0.424223f).rgb(),
                         QColor::fromRgbF(0.304148f, 0.764704f, 0.419943f).rgb(),
                         QColor::fromRgbF(0.311925f, 0.767822f, 0.415586f).rgb(),
                         QColor::fromRgbF(0.319809f, 0.770914f, 0.411152f).rgb(),
                         QColor::fromRgbF(0.327796f, 0.773980f, 0.406640f).rgb(),
                         QColor::fromRgbF(0.335885f, 0.777018f, 0.402049f).rgb(),
                         QColor::fromRgbF(0.344074f, 0.780029f, 0.397381f).rgb(),
                         QColor::fromRgbF(0.352360f, 0.783011f, 0.392636f).rgb(),
                         QColor::fromRgbF(0.360741f, 0.785964f, 0.387814f).rgb(),
                         QColor::fromRgbF(0.369214f, 0.788888f, 0.382914f).rgb(),
                         QColor::fromRgbF(0.377779f, 0.791781f, 0.377939f).rgb(),
                         QColor::fromRgbF(0.386433f, 0.794644f, 0.372886f).rgb(),
                         QColor::fromRgbF(0.395174f, 0.797475f, 0.367757f).rgb(),
                         QColor::fromRgbF(0.404001f, 0.800275f, 0.362552f).rgb(),
                         QColor::fromRgbF(0.412913f, 0.803041f, 0.357269f).rgb(),
                         QColor::fromRgbF(0.421908f, 0.805774f, 0.351910f).rgb(),
                         QColor::fromRgbF(0.430983f, 0.808473f, 0.346476f).rgb(),
                         QColor::fromRgbF(0.440137f, 0.811138f, 0.340967f).rgb(),
                         QColor::fromRgbF(0.449368f, 0.813768f, 0.335384f).rgb(),
                         QColor::fromRgbF(0.458674f, 0.816363f, 0.329727f).rgb(),
                         QColor::fromRgbF(0.468053f, 0.818921f, 0.323998f).rgb(),
                         QColor::fromRgbF(0.477504f, 0.821444f, 0.318195f).rgb(),
                         QColor::fromRgbF(0.487026f, 0.823929f, 0.312321f).rgb(),
                         QColor::fromRgbF(0.496615f, 0.826376f, 0.306377f).rgb(),
                         QColor::fromRgbF(0.506271f, 0.828786f, 0.300362f).rgb(),
                         QColor::fromRgbF(0.515992f, 0.831158f, 0.294279f).rgb(),
                         QColor::fromRgbF(0.525776f, 0.833491f, 0.288127f).rgb(),
                         QColor::fromRgbF(0.535621f, 0.835785f, 0.281908f).rgb(),
                         QColor::fromRgbF(0.545524f, 0.838039f, 0.275626f).rgb(),
                         QColor::fromRgbF(0.555484f, 0.840254f, 0.269281f).rgb(),
                         QColor::fromRgbF(0.565498f, 0.842430f, 0.262877f).rgb(),
                         QColor::fromRgbF(0.575563f, 0.844566f, 0.256415f).rgb(),
                         QColor::fromRgbF(0.585678f, 0.846661f, 0.249897f).rgb(),
                         QColor::fromRgbF(0.595839f, 0.848717f, 0.243329f).rgb(),
                         QColor::fromRgbF(0.606045f, 0.850733f, 0.236712f).rgb(),
                         QColor::fromRgbF(0.616293f, 0.852709f, 0.230052f).rgb(),
                         QColor::fromRgbF(0.626579f, 0.854645f, 0.223353f).rgb(),
                         QColor::fromRgbF(0.636902f, 0.856542f, 0.216620f).rgb(),
                         QColor::fromRgbF(0.647257f, 0.858400f, 0.209861f).rgb(),
                         QColor::fromRgbF(0.657642f, 0.860219f, 0.203082f).rgb(),
                         QColor::fromRgbF(0.668054f, 0.861999f, 0.196293f).rgb(),
                         QColor::fromRgbF(0.678489f, 0.863742f, 0.189503f).rgb(),
                         QColor::fromRgbF(0.688944f, 0.865448f, 0.182725f).rgb(),
                         QColor::fromRgbF(0.699415f, 0.867117f, 0.175971f).rgb(),
                         QColor::fromRgbF(0.709898f, 0.868751f, 0.169257f).rgb(),
                         QColor::fromRgbF(0.720391f, 0.870350f, 0.162603f).rgb(),
                         QColor::fromRgbF(0.730889f, 0.871916f, 0.156029f).rgb(),
                         QColor::fromRgbF(0.741388f, 0.873449f, 0.149561f).rgb(),
                         QColor::fromRgbF(0.751884f, 0.874951f, 0.143228f).rgb(),
                         QColor::fromRgbF(0.762373f, 0.876424f, 0.137064f).rgb(),
                         QColor::fromRgbF(0.772852f, 0.877868f, 0.131109f).rgb(),
                         QColor::fromRgbF(0.783315f, 0.879285f, 0.125405f).rgb(),
                         QColor::fromRgbF(0.793760f, 0.880678f, 0.120005f).rgb(),
                         QColor::fromRgbF(0.804182f, 0.882046f, 0.114965f).rgb(),
                         QColor::fromRgbF(0.814576f, 0.883393f, 0.110347f).rgb(),
                         QColor::fromRgbF(0.824940f, 0.884720f, 0.106217f).rgb(),
                         QColor::fromRgbF(0.835270f, 0.886029f, 0.102646f).rgb(),
                         QColor::fromRgbF(0.845561f, 0.887322f, 0.099702f).rgb(),
                         QColor::fromRgbF(0.855810f, 0.888601f, 0.097452f).rgb(),
                         QColor::fromRgbF(0.866013f, 0.889868f, 0.095953f).rgb(),
                         QColor::fromRgbF(0.876168f, 0.891125f, 0.095250f).rgb(),
                         QColor::fromRgbF(0.886271f, 0.892374f, 0.095374f).rgb(),
                         QColor::fromRgbF(0.896320f, 0.893616f, 0.096335f).rgb(),
                         QColor::fromRgbF(0.906311f, 0.894855f, 0.098125f).rgb(),
                         QColor::fromRgbF(0.916242f, 0.896091f, 0.100717f).rgb(),
                         QColor::fromRgbF(0.926106f, 0.897330f, 0.104071f).rgb(),
                         QColor::fromRgbF(0.935904f, 0.898570f, 0.108131f).rgb(),
                         QColor::fromRgbF(0.945636f, 0.899815f, 0.112838f).rgb(),
                         QColor::fromRgbF(0.955300f, 0.901065f, 0.118128f).rgb(),
                         QColor::fromRgbF(0.964894f, 0.902323f, 0.123941f).rgb(),
                         QColor::fromRgbF(0.974417f, 0.903590f, 0.130215f).rgb(),
                         QColor::fromRgbF(0.983868f, 0.904867f, 0.136897f).rgb(),
                         QColor::fromRgbF(0.993248f, 0.906157f, 0.143936f).rgb()};

        {
            const auto palettei=JKQTPMathImageINVERTED_Viridis;
            const QString palN="invviridis";
            const QString palNT=QObject::tr("inv. viridis");
            lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palettei].lut=lutstore[palette].lut;
            std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
        }
        {
            const auto palettei=JKQTPMathImageViridis_STEP;
            const QString palN="viridis_step";
            const QString palNT=QObject::tr("steps: viridis");
            lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
        }
        {
            const auto palettei=JKQTPMathImageINVERTED_Viridis_STEP;
            const QString palN="invviridis_step";
            const QString palNT=QObject::tr("steps: inv. viridis");
            lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
            std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
        }
    }

    {
        // from https://github.com/BIDS/colormap/blob/master/colormaps.py
        // https://github.com/BIDS/colormap/blob/master/colormaps.py
        const auto palette=JKQTPMathImagePlasma;
        const QString palN="plasma";
        const QString palNT=QObject::tr("plamsa");
        lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
        lutstore[palette].lut=LUTType{
                        QColor::fromRgbF(0.050383f, 0.029803f, 0.527975f).rgb(),
                        QColor::fromRgbF(0.063536f, 0.028426f, 0.533124f).rgb(),
                        QColor::fromRgbF(0.075353f, 0.027206f, 0.538007f).rgb(),
                        QColor::fromRgbF(0.086222f, 0.026125f, 0.542658f).rgb(),
                        QColor::fromRgbF(0.096379f, 0.025165f, 0.547103f).rgb(),
                        QColor::fromRgbF(0.105980f, 0.024309f, 0.551368f).rgb(),
                        QColor::fromRgbF(0.115124f, 0.023556f, 0.555468f).rgb(),
                        QColor::fromRgbF(0.123903f, 0.022878f, 0.559423f).rgb(),
                        QColor::fromRgbF(0.132381f, 0.022258f, 0.563250f).rgb(),
                        QColor::fromRgbF(0.140603f, 0.021687f, 0.566959f).rgb(),
                        QColor::fromRgbF(0.148607f, 0.021154f, 0.570562f).rgb(),
                        QColor::fromRgbF(0.156421f, 0.020651f, 0.574065f).rgb(),
                        QColor::fromRgbF(0.164070f, 0.020171f, 0.577478f).rgb(),
                        QColor::fromRgbF(0.171574f, 0.019706f, 0.580806f).rgb(),
                        QColor::fromRgbF(0.178950f, 0.019252f, 0.584054f).rgb(),
                        QColor::fromRgbF(0.186213f, 0.018803f, 0.587228f).rgb(),
                        QColor::fromRgbF(0.193374f, 0.018354f, 0.590330f).rgb(),
                        QColor::fromRgbF(0.200445f, 0.017902f, 0.593364f).rgb(),
                        QColor::fromRgbF(0.207435f, 0.017442f, 0.596333f).rgb(),
                        QColor::fromRgbF(0.214350f, 0.016973f, 0.599239f).rgb(),
                        QColor::fromRgbF(0.221197f, 0.016497f, 0.602083f).rgb(),
                        QColor::fromRgbF(0.227983f, 0.016007f, 0.604867f).rgb(),
                        QColor::fromRgbF(0.234715f, 0.015502f, 0.607592f).rgb(),
                        QColor::fromRgbF(0.241396f, 0.014979f, 0.610259f).rgb(),
                        QColor::fromRgbF(0.248032f, 0.014439f, 0.612868f).rgb(),
                        QColor::fromRgbF(0.254627f, 0.013882f, 0.615419f).rgb(),
                        QColor::fromRgbF(0.261183f, 0.013308f, 0.617911f).rgb(),
                        QColor::fromRgbF(0.267703f, 0.012716f, 0.620346f).rgb(),
                        QColor::fromRgbF(0.274191f, 0.012109f, 0.622722f).rgb(),
                        QColor::fromRgbF(0.280648f, 0.011488f, 0.625038f).rgb(),
                        QColor::fromRgbF(0.287076f, 0.010855f, 0.627295f).rgb(),
                        QColor::fromRgbF(0.293478f, 0.010213f, 0.629490f).rgb(),
                        QColor::fromRgbF(0.299855f, 0.009561f, 0.631624f).rgb(),
                        QColor::fromRgbF(0.306210f, 0.008902f, 0.633694f).rgb(),
                        QColor::fromRgbF(0.312543f, 0.008239f, 0.635700f).rgb(),
                        QColor::fromRgbF(0.318856f, 0.007576f, 0.637640f).rgb(),
                        QColor::fromRgbF(0.325150f, 0.006915f, 0.639512f).rgb(),
                        QColor::fromRgbF(0.331426f, 0.006261f, 0.641316f).rgb(),
                        QColor::fromRgbF(0.337683f, 0.005618f, 0.643049f).rgb(),
                        QColor::fromRgbF(0.343925f, 0.004991f, 0.644710f).rgb(),
                        QColor::fromRgbF(0.350150f, 0.004382f, 0.646298f).rgb(),
                        QColor::fromRgbF(0.356359f, 0.003798f, 0.647810f).rgb(),
                        QColor::fromRgbF(0.362553f, 0.003243f, 0.649245f).rgb(),
                        QColor::fromRgbF(0.368733f, 0.002724f, 0.650601f).rgb(),
                        QColor::fromRgbF(0.374897f, 0.002245f, 0.651876f).rgb(),
                        QColor::fromRgbF(0.381047f, 0.001814f, 0.653068f).rgb(),
                        QColor::fromRgbF(0.387183f, 0.001434f, 0.654177f).rgb(),
                        QColor::fromRgbF(0.393304f, 0.001114f, 0.655199f).rgb(),
                        QColor::fromRgbF(0.399411f, 0.000859f, 0.656133f).rgb(),
                        QColor::fromRgbF(0.405503f, 0.000678f, 0.656977f).rgb(),
                        QColor::fromRgbF(0.411580f, 0.000577f, 0.657730f).rgb(),
                        QColor::fromRgbF(0.417642f, 0.000564f, 0.658390f).rgb(),
                        QColor::fromRgbF(0.423689f, 0.000646f, 0.658956f).rgb(),
                        QColor::fromRgbF(0.429719f, 0.000831f, 0.659425f).rgb(),
                        QColor::fromRgbF(0.435734f, 0.001127f, 0.659797f).rgb(),
                        QColor::fromRgbF(0.441732f, 0.001540f, 0.660069f).rgb(),
                        QColor::fromRgbF(0.447714f, 0.002080f, 0.660240f).rgb(),
                        QColor::fromRgbF(0.453677f, 0.002755f, 0.660310f).rgb(),
                        QColor::fromRgbF(0.459623f, 0.003574f, 0.660277f).rgb(),
                        QColor::fromRgbF(0.465550f, 0.004545f, 0.660139f).rgb(),
                        QColor::fromRgbF(0.471457f, 0.005678f, 0.659897f).rgb(),
                        QColor::fromRgbF(0.477344f, 0.006980f, 0.659549f).rgb(),
                        QColor::fromRgbF(0.483210f, 0.008460f, 0.659095f).rgb(),
                        QColor::fromRgbF(0.489055f, 0.010127f, 0.658534f).rgb(),
                        QColor::fromRgbF(0.494877f, 0.011990f, 0.657865f).rgb(),
                        QColor::fromRgbF(0.500678f, 0.014055f, 0.657088f).rgb(),
                        QColor::fromRgbF(0.506454f, 0.016333f, 0.656202f).rgb(),
                        QColor::fromRgbF(0.512206f, 0.018833f, 0.655209f).rgb(),
                        QColor::fromRgbF(0.517933f, 0.021563f, 0.654109f).rgb(),
                        QColor::fromRgbF(0.523633f, 0.024532f, 0.652901f).rgb(),
                        QColor::fromRgbF(0.529306f, 0.027747f, 0.651586f).rgb(),
                        QColor::fromRgbF(0.534952f, 0.031217f, 0.650165f).rgb(),
                        QColor::fromRgbF(0.540570f, 0.034950f, 0.648640f).rgb(),
                        QColor::fromRgbF(0.546157f, 0.038954f, 0.647010f).rgb(),
                        QColor::fromRgbF(0.551715f, 0.043136f, 0.645277f).rgb(),
                        QColor::fromRgbF(0.557243f, 0.047331f, 0.643443f).rgb(),
                        QColor::fromRgbF(0.562738f, 0.051545f, 0.641509f).rgb(),
                        QColor::fromRgbF(0.568201f, 0.055778f, 0.639477f).rgb(),
                        QColor::fromRgbF(0.573632f, 0.060028f, 0.637349f).rgb(),
                        QColor::fromRgbF(0.579029f, 0.064296f, 0.635126f).rgb(),
                        QColor::fromRgbF(0.584391f, 0.068579f, 0.632812f).rgb(),
                        QColor::fromRgbF(0.589719f, 0.072878f, 0.630408f).rgb(),
                        QColor::fromRgbF(0.595011f, 0.077190f, 0.627917f).rgb(),
                        QColor::fromRgbF(0.600266f, 0.081516f, 0.625342f).rgb(),
                        QColor::fromRgbF(0.605485f, 0.085854f, 0.622686f).rgb(),
                        QColor::fromRgbF(0.610667f, 0.090204f, 0.619951f).rgb(),
                        QColor::fromRgbF(0.615812f, 0.094564f, 0.617140f).rgb(),
                        QColor::fromRgbF(0.620919f, 0.098934f, 0.614257f).rgb(),
                        QColor::fromRgbF(0.625987f, 0.103312f, 0.611305f).rgb(),
                        QColor::fromRgbF(0.631017f, 0.107699f, 0.608287f).rgb(),
                        QColor::fromRgbF(0.636008f, 0.112092f, 0.605205f).rgb(),
                        QColor::fromRgbF(0.640959f, 0.116492f, 0.602065f).rgb(),
                        QColor::fromRgbF(0.645872f, 0.120898f, 0.598867f).rgb(),
                        QColor::fromRgbF(0.650746f, 0.125309f, 0.595617f).rgb(),
                        QColor::fromRgbF(0.655580f, 0.129725f, 0.592317f).rgb(),
                        QColor::fromRgbF(0.660374f, 0.134144f, 0.588971f).rgb(),
                        QColor::fromRgbF(0.665129f, 0.138566f, 0.585582f).rgb(),
                        QColor::fromRgbF(0.669845f, 0.142992f, 0.582154f).rgb(),
                        QColor::fromRgbF(0.674522f, 0.147419f, 0.578688f).rgb(),
                        QColor::fromRgbF(0.679160f, 0.151848f, 0.575189f).rgb(),
                        QColor::fromRgbF(0.683758f, 0.156278f, 0.571660f).rgb(),
                        QColor::fromRgbF(0.688318f, 0.160709f, 0.568103f).rgb(),
                        QColor::fromRgbF(0.692840f, 0.165141f, 0.564522f).rgb(),
                        QColor::fromRgbF(0.697324f, 0.169573f, 0.560919f).rgb(),
                        QColor::fromRgbF(0.701769f, 0.174005f, 0.557296f).rgb(),
                        QColor::fromRgbF(0.706178f, 0.178437f, 0.553657f).rgb(),
                        QColor::fromRgbF(0.710549f, 0.182868f, 0.550004f).rgb(),
                        QColor::fromRgbF(0.714883f, 0.187299f, 0.546338f).rgb(),
                        QColor::fromRgbF(0.719181f, 0.191729f, 0.542663f).rgb(),
                        QColor::fromRgbF(0.723444f, 0.196158f, 0.538981f).rgb(),
                        QColor::fromRgbF(0.727670f, 0.200586f, 0.535293f).rgb(),
                        QColor::fromRgbF(0.731862f, 0.205013f, 0.531601f).rgb(),
                        QColor::fromRgbF(0.736019f, 0.209439f, 0.527908f).rgb(),
                        QColor::fromRgbF(0.740143f, 0.213864f, 0.524216f).rgb(),
                        QColor::fromRgbF(0.744232f, 0.218288f, 0.520524f).rgb(),
                        QColor::fromRgbF(0.748289f, 0.222711f, 0.516834f).rgb(),
                        QColor::fromRgbF(0.752312f, 0.227133f, 0.513149f).rgb(),
                        QColor::fromRgbF(0.756304f, 0.231555f, 0.509468f).rgb(),
                        QColor::fromRgbF(0.760264f, 0.235976f, 0.505794f).rgb(),
                        QColor::fromRgbF(0.764193f, 0.240396f, 0.502126f).rgb(),
                        QColor::fromRgbF(0.768090f, 0.244817f, 0.498465f).rgb(),
                        QColor::fromRgbF(0.771958f, 0.249237f, 0.494813f).rgb(),
                        QColor::fromRgbF(0.775796f, 0.253658f, 0.491171f).rgb(),
                        QColor::fromRgbF(0.779604f, 0.258078f, 0.487539f).rgb(),
                        QColor::fromRgbF(0.783383f, 0.262500f, 0.483918f).rgb(),
                        QColor::fromRgbF(0.787133f, 0.266922f, 0.480307f).rgb(),
                        QColor::fromRgbF(0.790855f, 0.271345f, 0.476706f).rgb(),
                        QColor::fromRgbF(0.794549f, 0.275770f, 0.473117f).rgb(),
                        QColor::fromRgbF(0.798216f, 0.280197f, 0.469538f).rgb(),
                        QColor::fromRgbF(0.801855f, 0.284626f, 0.465971f).rgb(),
                        QColor::fromRgbF(0.805467f, 0.289057f, 0.462415f).rgb(),
                        QColor::fromRgbF(0.809052f, 0.293491f, 0.458870f).rgb(),
                        QColor::fromRgbF(0.812612f, 0.297928f, 0.455338f).rgb(),
                        QColor::fromRgbF(0.816144f, 0.302368f, 0.451816f).rgb(),
                        QColor::fromRgbF(0.819651f, 0.306812f, 0.448306f).rgb(),
                        QColor::fromRgbF(0.823132f, 0.311261f, 0.444806f).rgb(),
                        QColor::fromRgbF(0.826588f, 0.315714f, 0.441316f).rgb(),
                        QColor::fromRgbF(0.830018f, 0.320172f, 0.437836f).rgb(),
                        QColor::fromRgbF(0.833422f, 0.324635f, 0.434366f).rgb(),
                        QColor::fromRgbF(0.836801f, 0.329105f, 0.430905f).rgb(),
                        QColor::fromRgbF(0.840155f, 0.333580f, 0.427455f).rgb(),
                        QColor::fromRgbF(0.843484f, 0.338062f, 0.424013f).rgb(),
                        QColor::fromRgbF(0.846788f, 0.342551f, 0.420579f).rgb(),
                        QColor::fromRgbF(0.850066f, 0.347048f, 0.417153f).rgb(),
                        QColor::fromRgbF(0.853319f, 0.351553f, 0.413734f).rgb(),
                        QColor::fromRgbF(0.856547f, 0.356066f, 0.410322f).rgb(),
                        QColor::fromRgbF(0.859750f, 0.360588f, 0.406917f).rgb(),
                        QColor::fromRgbF(0.862927f, 0.365119f, 0.403519f).rgb(),
                        QColor::fromRgbF(0.866078f, 0.369660f, 0.400126f).rgb(),
                        QColor::fromRgbF(0.869203f, 0.374212f, 0.396738f).rgb(),
                        QColor::fromRgbF(0.872303f, 0.378774f, 0.393355f).rgb(),
                        QColor::fromRgbF(0.875376f, 0.383347f, 0.389976f).rgb(),
                        QColor::fromRgbF(0.878423f, 0.387932f, 0.386600f).rgb(),
                        QColor::fromRgbF(0.881443f, 0.392529f, 0.383229f).rgb(),
                        QColor::fromRgbF(0.884436f, 0.397139f, 0.379860f).rgb(),
                        QColor::fromRgbF(0.887402f, 0.401762f, 0.376494f).rgb(),
                        QColor::fromRgbF(0.890340f, 0.406398f, 0.373130f).rgb(),
                        QColor::fromRgbF(0.893250f, 0.411048f, 0.369768f).rgb(),
                        QColor::fromRgbF(0.896131f, 0.415712f, 0.366407f).rgb(),
                        QColor::fromRgbF(0.898984f, 0.420392f, 0.363047f).rgb(),
                        QColor::fromRgbF(0.901807f, 0.425087f, 0.359688f).rgb(),
                        QColor::fromRgbF(0.904601f, 0.429797f, 0.356329f).rgb(),
                        QColor::fromRgbF(0.907365f, 0.434524f, 0.352970f).rgb(),
                        QColor::fromRgbF(0.910098f, 0.439268f, 0.349610f).rgb(),
                        QColor::fromRgbF(0.912800f, 0.444029f, 0.346251f).rgb(),
                        QColor::fromRgbF(0.915471f, 0.448807f, 0.342890f).rgb(),
                        QColor::fromRgbF(0.918109f, 0.453603f, 0.339529f).rgb(),
                        QColor::fromRgbF(0.920714f, 0.458417f, 0.336166f).rgb(),
                        QColor::fromRgbF(0.923287f, 0.463251f, 0.332801f).rgb(),
                        QColor::fromRgbF(0.925825f, 0.468103f, 0.329435f).rgb(),
                        QColor::fromRgbF(0.928329f, 0.472975f, 0.326067f).rgb(),
                        QColor::fromRgbF(0.930798f, 0.477867f, 0.322697f).rgb(),
                        QColor::fromRgbF(0.933232f, 0.482780f, 0.319325f).rgb(),
                        QColor::fromRgbF(0.935630f, 0.487712f, 0.315952f).rgb(),
                        QColor::fromRgbF(0.937990f, 0.492667f, 0.312575f).rgb(),
                        QColor::fromRgbF(0.940313f, 0.497642f, 0.309197f).rgb(),
                        QColor::fromRgbF(0.942598f, 0.502639f, 0.305816f).rgb(),
                        QColor::fromRgbF(0.944844f, 0.507658f, 0.302433f).rgb(),
                        QColor::fromRgbF(0.947051f, 0.512699f, 0.299049f).rgb(),
                        QColor::fromRgbF(0.949217f, 0.517763f, 0.295662f).rgb(),
                        QColor::fromRgbF(0.951344f, 0.522850f, 0.292275f).rgb(),
                        QColor::fromRgbF(0.953428f, 0.527960f, 0.288883f).rgb(),
                        QColor::fromRgbF(0.955470f, 0.533093f, 0.285490f).rgb(),
                        QColor::fromRgbF(0.957469f, 0.538250f, 0.282096f).rgb(),
                        QColor::fromRgbF(0.959424f, 0.543431f, 0.278701f).rgb(),
                        QColor::fromRgbF(0.961336f, 0.548636f, 0.275305f).rgb(),
                        QColor::fromRgbF(0.963203f, 0.553865f, 0.271909f).rgb(),
                        QColor::fromRgbF(0.965024f, 0.559118f, 0.268513f).rgb(),
                        QColor::fromRgbF(0.966798f, 0.564396f, 0.265118f).rgb(),
                        QColor::fromRgbF(0.968526f, 0.569700f, 0.261721f).rgb(),
                        QColor::fromRgbF(0.970205f, 0.575028f, 0.258325f).rgb(),
                        QColor::fromRgbF(0.971835f, 0.580382f, 0.254931f).rgb(),
                        QColor::fromRgbF(0.973416f, 0.585761f, 0.251540f).rgb(),
                        QColor::fromRgbF(0.974947f, 0.591165f, 0.248151f).rgb(),
                        QColor::fromRgbF(0.976428f, 0.596595f, 0.244767f).rgb(),
                        QColor::fromRgbF(0.977856f, 0.602051f, 0.241387f).rgb(),
                        QColor::fromRgbF(0.979233f, 0.607532f, 0.238013f).rgb(),
                        QColor::fromRgbF(0.980556f, 0.613039f, 0.234646f).rgb(),
                        QColor::fromRgbF(0.981826f, 0.618572f, 0.231287f).rgb(),
                        QColor::fromRgbF(0.983041f, 0.624131f, 0.227937f).rgb(),
                        QColor::fromRgbF(0.984199f, 0.629718f, 0.224595f).rgb(),
                        QColor::fromRgbF(0.985301f, 0.635330f, 0.221265f).rgb(),
                        QColor::fromRgbF(0.986345f, 0.640969f, 0.217948f).rgb(),
                        QColor::fromRgbF(0.987332f, 0.646633f, 0.214648f).rgb(),
                        QColor::fromRgbF(0.988260f, 0.652325f, 0.211364f).rgb(),
                        QColor::fromRgbF(0.989128f, 0.658043f, 0.208100f).rgb(),
                        QColor::fromRgbF(0.989935f, 0.663787f, 0.204859f).rgb(),
                        QColor::fromRgbF(0.990681f, 0.669558f, 0.201642f).rgb(),
                        QColor::fromRgbF(0.991365f, 0.675355f, 0.198453f).rgb(),
                        QColor::fromRgbF(0.991985f, 0.681179f, 0.195295f).rgb(),
                        QColor::fromRgbF(0.992541f, 0.687030f, 0.192170f).rgb(),
                        QColor::fromRgbF(0.993032f, 0.692907f, 0.189084f).rgb(),
                        QColor::fromRgbF(0.993456f, 0.698810f, 0.186041f).rgb(),
                        QColor::fromRgbF(0.993814f, 0.704741f, 0.183043f).rgb(),
                        QColor::fromRgbF(0.994103f, 0.710698f, 0.180097f).rgb(),
                        QColor::fromRgbF(0.994324f, 0.716681f, 0.177208f).rgb(),
                        QColor::fromRgbF(0.994474f, 0.722691f, 0.174381f).rgb(),
                        QColor::fromRgbF(0.994553f, 0.728728f, 0.171622f).rgb(),
                        QColor::fromRgbF(0.994561f, 0.734791f, 0.168938f).rgb(),
                        QColor::fromRgbF(0.994495f, 0.740880f, 0.166335f).rgb(),
                        QColor::fromRgbF(0.994355f, 0.746995f, 0.163821f).rgb(),
                        QColor::fromRgbF(0.994141f, 0.753137f, 0.161404f).rgb(),
                        QColor::fromRgbF(0.993851f, 0.759304f, 0.159092f).rgb(),
                        QColor::fromRgbF(0.993482f, 0.765499f, 0.156891f).rgb(),
                        QColor::fromRgbF(0.993033f, 0.771720f, 0.154808f).rgb(),
                        QColor::fromRgbF(0.992505f, 0.777967f, 0.152855f).rgb(),
                        QColor::fromRgbF(0.991897f, 0.784239f, 0.151042f).rgb(),
                        QColor::fromRgbF(0.991209f, 0.790537f, 0.149377f).rgb(),
                        QColor::fromRgbF(0.990439f, 0.796859f, 0.147870f).rgb(),
                        QColor::fromRgbF(0.989587f, 0.803205f, 0.146529f).rgb(),
                        QColor::fromRgbF(0.988648f, 0.809579f, 0.145357f).rgb(),
                        QColor::fromRgbF(0.987621f, 0.815978f, 0.144363f).rgb(),
                        QColor::fromRgbF(0.986509f, 0.822401f, 0.143557f).rgb(),
                        QColor::fromRgbF(0.985314f, 0.828846f, 0.142945f).rgb(),
                        QColor::fromRgbF(0.984031f, 0.835315f, 0.142528f).rgb(),
                        QColor::fromRgbF(0.982653f, 0.841812f, 0.142303f).rgb(),
                        QColor::fromRgbF(0.981190f, 0.848329f, 0.142279f).rgb(),
                        QColor::fromRgbF(0.979644f, 0.854866f, 0.142453f).rgb(),
                        QColor::fromRgbF(0.977995f, 0.861432f, 0.142808f).rgb(),
                        QColor::fromRgbF(0.976265f, 0.868016f, 0.143351f).rgb(),
                        QColor::fromRgbF(0.974443f, 0.874622f, 0.144061f).rgb(),
                        QColor::fromRgbF(0.972530f, 0.881250f, 0.144923f).rgb(),
                        QColor::fromRgbF(0.970533f, 0.887896f, 0.145919f).rgb(),
                        QColor::fromRgbF(0.968443f, 0.894564f, 0.147014f).rgb(),
                        QColor::fromRgbF(0.966271f, 0.901249f, 0.148180f).rgb(),
                        QColor::fromRgbF(0.964021f, 0.907950f, 0.149370f).rgb(),
                        QColor::fromRgbF(0.961681f, 0.914672f, 0.150520f).rgb(),
                        QColor::fromRgbF(0.959276f, 0.921407f, 0.151566f).rgb(),
                        QColor::fromRgbF(0.956808f, 0.928152f, 0.152409f).rgb(),
                        QColor::fromRgbF(0.954287f, 0.934908f, 0.152921f).rgb(),
                        QColor::fromRgbF(0.951726f, 0.941671f, 0.152925f).rgb(),
                        QColor::fromRgbF(0.949151f, 0.948435f, 0.152178f).rgb(),
                        QColor::fromRgbF(0.946602f, 0.955190f, 0.150328f).rgb(),
                        QColor::fromRgbF(0.944152f, 0.961916f, 0.146861f).rgb(),
                        QColor::fromRgbF(0.941896f, 0.968590f, 0.140956f).rgb(),
                        QColor::fromRgbF(0.940015f, 0.975158f, 0.131326f).rgb()};

        {
            const auto palettei=JKQTPMathImageINVERTED_Plasma;
            const QString palN="invplasma";
            const QString palNT=QObject::tr("inv. plasma");
            lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palettei].lut=lutstore[palette].lut;
            std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
        }
        {
            const auto palettei=JKQTPMathImagePlasma_STEP;
            const QString palN="plasma_step";
            const QString palNT=QObject::tr("steps: plasma");
            lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
        }
        {
            const auto palettei=JKQTPMathImageINVERTED_Plasma_STEP;
            const QString palN="invplasma_step";
            const QString palNT=QObject::tr("steps: inv. plasma");
            lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
            std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
        }

    }

        {
            // from https://github.com/BIDS/colormap/blob/master/colormaps.py
            // https://github.com/BIDS/colormap/blob/master/colormaps.py
            const auto palette=JKQTPMathImageMagma;
            const QString palN="magma";
            const QString palNT=QObject::tr("magma");
            lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palette].lut=LUTType{
                        QColor::fromRgbF(0.001462f, 0.000466f, 0.013866f).rgb(),
                        QColor::fromRgbF(0.002258f, 0.001295f, 0.018331f).rgb(),
                        QColor::fromRgbF(0.003279f, 0.002305f, 0.023708f).rgb(),
                        QColor::fromRgbF(0.004512f, 0.003490f, 0.029965f).rgb(),
                        QColor::fromRgbF(0.005950f, 0.004843f, 0.037130f).rgb(),
                        QColor::fromRgbF(0.007588f, 0.006356f, 0.044973f).rgb(),
                        QColor::fromRgbF(0.009426f, 0.008022f, 0.052844f).rgb(),
                        QColor::fromRgbF(0.011465f, 0.009828f, 0.060750f).rgb(),
                        QColor::fromRgbF(0.013708f, 0.011771f, 0.068667f).rgb(),
                        QColor::fromRgbF(0.016156f, 0.013840f, 0.076603f).rgb(),
                        QColor::fromRgbF(0.018815f, 0.016026f, 0.084584f).rgb(),
                        QColor::fromRgbF(0.021692f, 0.018320f, 0.092610f).rgb(),
                        QColor::fromRgbF(0.024792f, 0.020715f, 0.100676f).rgb(),
                        QColor::fromRgbF(0.028123f, 0.023201f, 0.108787f).rgb(),
                        QColor::fromRgbF(0.031696f, 0.025765f, 0.116965f).rgb(),
                        QColor::fromRgbF(0.035520f, 0.028397f, 0.125209f).rgb(),
                        QColor::fromRgbF(0.039608f, 0.031090f, 0.133515f).rgb(),
                        QColor::fromRgbF(0.043830f, 0.033830f, 0.141886f).rgb(),
                        QColor::fromRgbF(0.048062f, 0.036607f, 0.150327f).rgb(),
                        QColor::fromRgbF(0.052320f, 0.039407f, 0.158841f).rgb(),
                        QColor::fromRgbF(0.056615f, 0.042160f, 0.167446f).rgb(),
                        QColor::fromRgbF(0.060949f, 0.044794f, 0.176129f).rgb(),
                        QColor::fromRgbF(0.065330f, 0.047318f, 0.184892f).rgb(),
                        QColor::fromRgbF(0.069764f, 0.049726f, 0.193735f).rgb(),
                        QColor::fromRgbF(0.074257f, 0.052017f, 0.202660f).rgb(),
                        QColor::fromRgbF(0.078815f, 0.054184f, 0.211667f).rgb(),
                        QColor::fromRgbF(0.083446f, 0.056225f, 0.220755f).rgb(),
                        QColor::fromRgbF(0.088155f, 0.058133f, 0.229922f).rgb(),
                        QColor::fromRgbF(0.092949f, 0.059904f, 0.239164f).rgb(),
                        QColor::fromRgbF(0.097833f, 0.061531f, 0.248477f).rgb(),
                        QColor::fromRgbF(0.102815f, 0.063010f, 0.257854f).rgb(),
                        QColor::fromRgbF(0.107899f, 0.064335f, 0.267289f).rgb(),
                        QColor::fromRgbF(0.113094f, 0.065492f, 0.276784f).rgb(),
                        QColor::fromRgbF(0.118405f, 0.066479f, 0.286321f).rgb(),
                        QColor::fromRgbF(0.123833f, 0.067295f, 0.295879f).rgb(),
                        QColor::fromRgbF(0.129380f, 0.067935f, 0.305443f).rgb(),
                        QColor::fromRgbF(0.135053f, 0.068391f, 0.315000f).rgb(),
                        QColor::fromRgbF(0.140858f, 0.068654f, 0.324538f).rgb(),
                        QColor::fromRgbF(0.146785f, 0.068738f, 0.334011f).rgb(),
                        QColor::fromRgbF(0.152839f, 0.068637f, 0.343404f).rgb(),
                        QColor::fromRgbF(0.159018f, 0.068354f, 0.352688f).rgb(),
                        QColor::fromRgbF(0.165308f, 0.067911f, 0.361816f).rgb(),
                        QColor::fromRgbF(0.171713f, 0.067305f, 0.370771f).rgb(),
                        QColor::fromRgbF(0.178212f, 0.066576f, 0.379497f).rgb(),
                        QColor::fromRgbF(0.184801f, 0.065732f, 0.387973f).rgb(),
                        QColor::fromRgbF(0.191460f, 0.064818f, 0.396152f).rgb(),
                        QColor::fromRgbF(0.198177f, 0.063862f, 0.404009f).rgb(),
                        QColor::fromRgbF(0.204935f, 0.062907f, 0.411514f).rgb(),
                        QColor::fromRgbF(0.211718f, 0.061992f, 0.418647f).rgb(),
                        QColor::fromRgbF(0.218512f, 0.061158f, 0.425392f).rgb(),
                        QColor::fromRgbF(0.225302f, 0.060445f, 0.431742f).rgb(),
                        QColor::fromRgbF(0.232077f, 0.059889f, 0.437695f).rgb(),
                        QColor::fromRgbF(0.238826f, 0.059517f, 0.443256f).rgb(),
                        QColor::fromRgbF(0.245543f, 0.059352f, 0.448436f).rgb(),
                        QColor::fromRgbF(0.252220f, 0.059415f, 0.453248f).rgb(),
                        QColor::fromRgbF(0.258857f, 0.059706f, 0.457710f).rgb(),
                        QColor::fromRgbF(0.265447f, 0.060237f, 0.461840f).rgb(),
                        QColor::fromRgbF(0.271994f, 0.060994f, 0.465660f).rgb(),
                        QColor::fromRgbF(0.278493f, 0.061978f, 0.469190f).rgb(),
                        QColor::fromRgbF(0.284951f, 0.063168f, 0.472451f).rgb(),
                        QColor::fromRgbF(0.291366f, 0.064553f, 0.475462f).rgb(),
                        QColor::fromRgbF(0.297740f, 0.066117f, 0.478243f).rgb(),
                        QColor::fromRgbF(0.304081f, 0.067835f, 0.480812f).rgb(),
                        QColor::fromRgbF(0.310382f, 0.069702f, 0.483186f).rgb(),
                        QColor::fromRgbF(0.316654f, 0.071690f, 0.485380f).rgb(),
                        QColor::fromRgbF(0.322899f, 0.073782f, 0.487408f).rgb(),
                        QColor::fromRgbF(0.329114f, 0.075972f, 0.489287f).rgb(),
                        QColor::fromRgbF(0.335308f, 0.078236f, 0.491024f).rgb(),
                        QColor::fromRgbF(0.341482f, 0.080564f, 0.492631f).rgb(),
                        QColor::fromRgbF(0.347636f, 0.082946f, 0.494121f).rgb(),
                        QColor::fromRgbF(0.353773f, 0.085373f, 0.495501f).rgb(),
                        QColor::fromRgbF(0.359898f, 0.087831f, 0.496778f).rgb(),
                        QColor::fromRgbF(0.366012f, 0.090314f, 0.497960f).rgb(),
                        QColor::fromRgbF(0.372116f, 0.092816f, 0.499053f).rgb(),
                        QColor::fromRgbF(0.378211f, 0.095332f, 0.500067f).rgb(),
                        QColor::fromRgbF(0.384299f, 0.097855f, 0.501002f).rgb(),
                        QColor::fromRgbF(0.390384f, 0.100379f, 0.501864f).rgb(),
                        QColor::fromRgbF(0.396467f, 0.102902f, 0.502658f).rgb(),
                        QColor::fromRgbF(0.402548f, 0.105420f, 0.503386f).rgb(),
                        QColor::fromRgbF(0.408629f, 0.107930f, 0.504052f).rgb(),
                        QColor::fromRgbF(0.414709f, 0.110431f, 0.504662f).rgb(),
                        QColor::fromRgbF(0.420791f, 0.112920f, 0.505215f).rgb(),
                        QColor::fromRgbF(0.426877f, 0.115395f, 0.505714f).rgb(),
                        QColor::fromRgbF(0.432967f, 0.117855f, 0.506160f).rgb(),
                        QColor::fromRgbF(0.439062f, 0.120298f, 0.506555f).rgb(),
                        QColor::fromRgbF(0.445163f, 0.122724f, 0.506901f).rgb(),
                        QColor::fromRgbF(0.451271f, 0.125132f, 0.507198f).rgb(),
                        QColor::fromRgbF(0.457386f, 0.127522f, 0.507448f).rgb(),
                        QColor::fromRgbF(0.463508f, 0.129893f, 0.507652f).rgb(),
                        QColor::fromRgbF(0.469640f, 0.132245f, 0.507809f).rgb(),
                        QColor::fromRgbF(0.475780f, 0.134577f, 0.507921f).rgb(),
                        QColor::fromRgbF(0.481929f, 0.136891f, 0.507989f).rgb(),
                        QColor::fromRgbF(0.488088f, 0.139186f, 0.508011f).rgb(),
                        QColor::fromRgbF(0.494258f, 0.141462f, 0.507988f).rgb(),
                        QColor::fromRgbF(0.500438f, 0.143719f, 0.507920f).rgb(),
                        QColor::fromRgbF(0.506629f, 0.145958f, 0.507806f).rgb(),
                        QColor::fromRgbF(0.512831f, 0.148179f, 0.507648f).rgb(),
                        QColor::fromRgbF(0.519045f, 0.150383f, 0.507443f).rgb(),
                        QColor::fromRgbF(0.525270f, 0.152569f, 0.507192f).rgb(),
                        QColor::fromRgbF(0.531507f, 0.154739f, 0.506895f).rgb(),
                        QColor::fromRgbF(0.537755f, 0.156894f, 0.506551f).rgb(),
                        QColor::fromRgbF(0.544015f, 0.159033f, 0.506159f).rgb(),
                        QColor::fromRgbF(0.550287f, 0.161158f, 0.505719f).rgb(),
                        QColor::fromRgbF(0.556571f, 0.163269f, 0.505230f).rgb(),
                        QColor::fromRgbF(0.562866f, 0.165368f, 0.504692f).rgb(),
                        QColor::fromRgbF(0.569172f, 0.167454f, 0.504105f).rgb(),
                        QColor::fromRgbF(0.575490f, 0.169530f, 0.503466f).rgb(),
                        QColor::fromRgbF(0.581819f, 0.171596f, 0.502777f).rgb(),
                        QColor::fromRgbF(0.588158f, 0.173652f, 0.502035f).rgb(),
                        QColor::fromRgbF(0.594508f, 0.175701f, 0.501241f).rgb(),
                        QColor::fromRgbF(0.600868f, 0.177743f, 0.500394f).rgb(),
                        QColor::fromRgbF(0.607238f, 0.179779f, 0.499492f).rgb(),
                        QColor::fromRgbF(0.613617f, 0.181811f, 0.498536f).rgb(),
                        QColor::fromRgbF(0.620005f, 0.183840f, 0.497524f).rgb(),
                        QColor::fromRgbF(0.626401f, 0.185867f, 0.496456f).rgb(),
                        QColor::fromRgbF(0.632805f, 0.187893f, 0.495332f).rgb(),
                        QColor::fromRgbF(0.639216f, 0.189921f, 0.494150f).rgb(),
                        QColor::fromRgbF(0.645633f, 0.191952f, 0.492910f).rgb(),
                        QColor::fromRgbF(0.652056f, 0.193986f, 0.491611f).rgb(),
                        QColor::fromRgbF(0.658483f, 0.196027f, 0.490253f).rgb(),
                        QColor::fromRgbF(0.664915f, 0.198075f, 0.488836f).rgb(),
                        QColor::fromRgbF(0.671349f, 0.200133f, 0.487358f).rgb(),
                        QColor::fromRgbF(0.677786f, 0.202203f, 0.485819f).rgb(),
                        QColor::fromRgbF(0.684224f, 0.204286f, 0.484219f).rgb(),
                        QColor::fromRgbF(0.690661f, 0.206384f, 0.482558f).rgb(),
                        QColor::fromRgbF(0.697098f, 0.208501f, 0.480835f).rgb(),
                        QColor::fromRgbF(0.703532f, 0.210638f, 0.479049f).rgb(),
                        QColor::fromRgbF(0.709962f, 0.212797f, 0.477201f).rgb(),
                        QColor::fromRgbF(0.716387f, 0.214982f, 0.475290f).rgb(),
                        QColor::fromRgbF(0.722805f, 0.217194f, 0.473316f).rgb(),
                        QColor::fromRgbF(0.729216f, 0.219437f, 0.471279f).rgb(),
                        QColor::fromRgbF(0.735616f, 0.221713f, 0.469180f).rgb(),
                        QColor::fromRgbF(0.742004f, 0.224025f, 0.467018f).rgb(),
                        QColor::fromRgbF(0.748378f, 0.226377f, 0.464794f).rgb(),
                        QColor::fromRgbF(0.754737f, 0.228772f, 0.462509f).rgb(),
                        QColor::fromRgbF(0.761077f, 0.231214f, 0.460162f).rgb(),
                        QColor::fromRgbF(0.767398f, 0.233705f, 0.457755f).rgb(),
                        QColor::fromRgbF(0.773695f, 0.236249f, 0.455289f).rgb(),
                        QColor::fromRgbF(0.779968f, 0.238851f, 0.452765f).rgb(),
                        QColor::fromRgbF(0.786212f, 0.241514f, 0.450184f).rgb(),
                        QColor::fromRgbF(0.792427f, 0.244242f, 0.447543f).rgb(),
                        QColor::fromRgbF(0.798608f, 0.247040f, 0.444848f).rgb(),
                        QColor::fromRgbF(0.804752f, 0.249911f, 0.442102f).rgb(),
                        QColor::fromRgbF(0.810855f, 0.252861f, 0.439305f).rgb(),
                        QColor::fromRgbF(0.816914f, 0.255895f, 0.436461f).rgb(),
                        QColor::fromRgbF(0.822926f, 0.259016f, 0.433573f).rgb(),
                        QColor::fromRgbF(0.828886f, 0.262229f, 0.430644f).rgb(),
                        QColor::fromRgbF(0.834791f, 0.265540f, 0.427671f).rgb(),
                        QColor::fromRgbF(0.840636f, 0.268953f, 0.424666f).rgb(),
                        QColor::fromRgbF(0.846416f, 0.272473f, 0.421631f).rgb(),
                        QColor::fromRgbF(0.852126f, 0.276106f, 0.418573f).rgb(),
                        QColor::fromRgbF(0.857763f, 0.279857f, 0.415496f).rgb(),
                        QColor::fromRgbF(0.863320f, 0.283729f, 0.412403f).rgb(),
                        QColor::fromRgbF(0.868793f, 0.287728f, 0.409303f).rgb(),
                        QColor::fromRgbF(0.874176f, 0.291859f, 0.406205f).rgb(),
                        QColor::fromRgbF(0.879464f, 0.296125f, 0.403118f).rgb(),
                        QColor::fromRgbF(0.884651f, 0.300530f, 0.400047f).rgb(),
                        QColor::fromRgbF(0.889731f, 0.305079f, 0.397002f).rgb(),
                        QColor::fromRgbF(0.894700f, 0.309773f, 0.393995f).rgb(),
                        QColor::fromRgbF(0.899552f, 0.314616f, 0.391037f).rgb(),
                        QColor::fromRgbF(0.904281f, 0.319610f, 0.388137f).rgb(),
                        QColor::fromRgbF(0.908884f, 0.324755f, 0.385308f).rgb(),
                        QColor::fromRgbF(0.913354f, 0.330052f, 0.382563f).rgb(),
                        QColor::fromRgbF(0.917689f, 0.335500f, 0.379915f).rgb(),
                        QColor::fromRgbF(0.921884f, 0.341098f, 0.377376f).rgb(),
                        QColor::fromRgbF(0.925937f, 0.346844f, 0.374959f).rgb(),
                        QColor::fromRgbF(0.929845f, 0.352734f, 0.372677f).rgb(),
                        QColor::fromRgbF(0.933606f, 0.358764f, 0.370541f).rgb(),
                        QColor::fromRgbF(0.937221f, 0.364929f, 0.368567f).rgb(),
                        QColor::fromRgbF(0.940687f, 0.371224f, 0.366762f).rgb(),
                        QColor::fromRgbF(0.944006f, 0.377643f, 0.365136f).rgb(),
                        QColor::fromRgbF(0.947180f, 0.384178f, 0.363701f).rgb(),
                        QColor::fromRgbF(0.950210f, 0.390820f, 0.362468f).rgb(),
                        QColor::fromRgbF(0.953099f, 0.397563f, 0.361438f).rgb(),
                        QColor::fromRgbF(0.955849f, 0.404400f, 0.360619f).rgb(),
                        QColor::fromRgbF(0.958464f, 0.411324f, 0.360014f).rgb(),
                        QColor::fromRgbF(0.960949f, 0.418323f, 0.359630f).rgb(),
                        QColor::fromRgbF(0.963310f, 0.425390f, 0.359469f).rgb(),
                        QColor::fromRgbF(0.965549f, 0.432519f, 0.359529f).rgb(),
                        QColor::fromRgbF(0.967671f, 0.439703f, 0.359810f).rgb(),
                        QColor::fromRgbF(0.969680f, 0.446936f, 0.360311f).rgb(),
                        QColor::fromRgbF(0.971582f, 0.454210f, 0.361030f).rgb(),
                        QColor::fromRgbF(0.973381f, 0.461520f, 0.361965f).rgb(),
                        QColor::fromRgbF(0.975082f, 0.468861f, 0.363111f).rgb(),
                        QColor::fromRgbF(0.976690f, 0.476226f, 0.364466f).rgb(),
                        QColor::fromRgbF(0.978210f, 0.483612f, 0.366025f).rgb(),
                        QColor::fromRgbF(0.979645f, 0.491014f, 0.367783f).rgb(),
                        QColor::fromRgbF(0.981000f, 0.498428f, 0.369734f).rgb(),
                        QColor::fromRgbF(0.982279f, 0.505851f, 0.371874f).rgb(),
                        QColor::fromRgbF(0.983485f, 0.513280f, 0.374198f).rgb(),
                        QColor::fromRgbF(0.984622f, 0.520713f, 0.376698f).rgb(),
                        QColor::fromRgbF(0.985693f, 0.528148f, 0.379371f).rgb(),
                        QColor::fromRgbF(0.986700f, 0.535582f, 0.382210f).rgb(),
                        QColor::fromRgbF(0.987646f, 0.543015f, 0.385210f).rgb(),
                        QColor::fromRgbF(0.988533f, 0.550446f, 0.388365f).rgb(),
                        QColor::fromRgbF(0.989363f, 0.557873f, 0.391671f).rgb(),
                        QColor::fromRgbF(0.990138f, 0.565296f, 0.395122f).rgb(),
                        QColor::fromRgbF(0.990871f, 0.572706f, 0.398714f).rgb(),
                        QColor::fromRgbF(0.991558f, 0.580107f, 0.402441f).rgb(),
                        QColor::fromRgbF(0.992196f, 0.587502f, 0.406299f).rgb(),
                        QColor::fromRgbF(0.992785f, 0.594891f, 0.410283f).rgb(),
                        QColor::fromRgbF(0.993326f, 0.602275f, 0.414390f).rgb(),
                        QColor::fromRgbF(0.993834f, 0.609644f, 0.418613f).rgb(),
                        QColor::fromRgbF(0.994309f, 0.616999f, 0.422950f).rgb(),
                        QColor::fromRgbF(0.994738f, 0.624350f, 0.427397f).rgb(),
                        QColor::fromRgbF(0.995122f, 0.631696f, 0.431951f).rgb(),
                        QColor::fromRgbF(0.995480f, 0.639027f, 0.436607f).rgb(),
                        QColor::fromRgbF(0.995810f, 0.646344f, 0.441361f).rgb(),
                        QColor::fromRgbF(0.996096f, 0.653659f, 0.446213f).rgb(),
                        QColor::fromRgbF(0.996341f, 0.660969f, 0.451160f).rgb(),
                        QColor::fromRgbF(0.996580f, 0.668256f, 0.456192f).rgb(),
                        QColor::fromRgbF(0.996775f, 0.675541f, 0.461314f).rgb(),
                        QColor::fromRgbF(0.996925f, 0.682828f, 0.466526f).rgb(),
                        QColor::fromRgbF(0.997077f, 0.690088f, 0.471811f).rgb(),
                        QColor::fromRgbF(0.997186f, 0.697349f, 0.477182f).rgb(),
                        QColor::fromRgbF(0.997254f, 0.704611f, 0.482635f).rgb(),
                        QColor::fromRgbF(0.997325f, 0.711848f, 0.488154f).rgb(),
                        QColor::fromRgbF(0.997351f, 0.719089f, 0.493755f).rgb(),
                        QColor::fromRgbF(0.997351f, 0.726324f, 0.499428f).rgb(),
                        QColor::fromRgbF(0.997341f, 0.733545f, 0.505167f).rgb(),
                        QColor::fromRgbF(0.997285f, 0.740772f, 0.510983f).rgb(),
                        QColor::fromRgbF(0.997228f, 0.747981f, 0.516859f).rgb(),
                        QColor::fromRgbF(0.997138f, 0.755190f, 0.522806f).rgb(),
                        QColor::fromRgbF(0.997019f, 0.762398f, 0.528821f).rgb(),
                        QColor::fromRgbF(0.996898f, 0.769591f, 0.534892f).rgb(),
                        QColor::fromRgbF(0.996727f, 0.776795f, 0.541039f).rgb(),
                        QColor::fromRgbF(0.996571f, 0.783977f, 0.547233f).rgb(),
                        QColor::fromRgbF(0.996369f, 0.791167f, 0.553499f).rgb(),
                        QColor::fromRgbF(0.996162f, 0.798348f, 0.559820f).rgb(),
                        QColor::fromRgbF(0.995932f, 0.805527f, 0.566202f).rgb(),
                        QColor::fromRgbF(0.995680f, 0.812706f, 0.572645f).rgb(),
                        QColor::fromRgbF(0.995424f, 0.819875f, 0.579140f).rgb(),
                        QColor::fromRgbF(0.995131f, 0.827052f, 0.585701f).rgb(),
                        QColor::fromRgbF(0.994851f, 0.834213f, 0.592307f).rgb(),
                        QColor::fromRgbF(0.994524f, 0.841387f, 0.598983f).rgb(),
                        QColor::fromRgbF(0.994222f, 0.848540f, 0.605696f).rgb(),
                        QColor::fromRgbF(0.993866f, 0.855711f, 0.612482f).rgb(),
                        QColor::fromRgbF(0.993545f, 0.862859f, 0.619299f).rgb(),
                        QColor::fromRgbF(0.993170f, 0.870024f, 0.626189f).rgb(),
                        QColor::fromRgbF(0.992831f, 0.877168f, 0.633109f).rgb(),
                        QColor::fromRgbF(0.992440f, 0.884330f, 0.640099f).rgb(),
                        QColor::fromRgbF(0.992089f, 0.891470f, 0.647116f).rgb(),
                        QColor::fromRgbF(0.991688f, 0.898627f, 0.654202f).rgb(),
                        QColor::fromRgbF(0.991332f, 0.905763f, 0.661309f).rgb(),
                        QColor::fromRgbF(0.990930f, 0.912915f, 0.668481f).rgb(),
                        QColor::fromRgbF(0.990570f, 0.920049f, 0.675675f).rgb(),
                        QColor::fromRgbF(0.990175f, 0.927196f, 0.682926f).rgb(),
                        QColor::fromRgbF(0.989815f, 0.934329f, 0.690198f).rgb(),
                        QColor::fromRgbF(0.989434f, 0.941470f, 0.697519f).rgb(),
                        QColor::fromRgbF(0.989077f, 0.948604f, 0.704863f).rgb(),
                        QColor::fromRgbF(0.988717f, 0.955742f, 0.712242f).rgb(),
                        QColor::fromRgbF(0.988367f, 0.962878f, 0.719649f).rgb(),
                        QColor::fromRgbF(0.988033f, 0.970012f, 0.727077f).rgb(),
                        QColor::fromRgbF(0.987691f, 0.977154f, 0.734536f).rgb(),
                        QColor::fromRgbF(0.987387f, 0.984288f, 0.742002f).rgb(),
                        QColor::fromRgbF(0.987053f, 0.991438f, 0.749504f).rgb()};

            {
                const auto palettei=JKQTPMathImageINVERTED_Magma;
                const QString palN="invmagma";
                const QString palNT=QObject::tr("inv. magma");
                lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
                lutstore[palettei].lut=lutstore[palette].lut;
                std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
            }
            {
                const auto palettei=JKQTPMathImageMagma_STEP;
                const QString palN="magma_step";
                const QString palNT=QObject::tr("steps: magma");
                lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
                lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
            }
            {
                const auto palettei=JKQTPMathImageINVERTED_Magma_STEP;
                const QString palN="invmagma_step";
                const QString palNT=QObject::tr("steps: inv. magma");
                lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
                lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
                std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
            }

        }

        {
            // from https://github.com/BIDS/colormap/blob/master/colormaps.py
            // https://github.com/BIDS/colormap/blob/master/colormaps.py
            const auto palette=JKQTPMathImageInferno;
            const QString palN="inferno";
            const QString palNT=QObject::tr("inferno");
            lutstore[palette]=JKQTPImageTools::LUTData( palN, palNT);
            lutstore[palette].lut=LUTType{
                        QColor::fromRgbF(0.001462f, 0.000466f, 0.013866f).rgb(),
                        QColor::fromRgbF(0.002267f, 0.001270f, 0.018570f).rgb(),
                        QColor::fromRgbF(0.003299f, 0.002249f, 0.024239f).rgb(),
                        QColor::fromRgbF(0.004547f, 0.003392f, 0.030909f).rgb(),
                        QColor::fromRgbF(0.006006f, 0.004692f, 0.038558f).rgb(),
                        QColor::fromRgbF(0.007676f, 0.006136f, 0.046836f).rgb(),
                        QColor::fromRgbF(0.009561f, 0.007713f, 0.055143f).rgb(),
                        QColor::fromRgbF(0.011663f, 0.009417f, 0.063460f).rgb(),
                        QColor::fromRgbF(0.013995f, 0.011225f, 0.071862f).rgb(),
                        QColor::fromRgbF(0.016561f, 0.013136f, 0.080282f).rgb(),
                        QColor::fromRgbF(0.019373f, 0.015133f, 0.088767f).rgb(),
                        QColor::fromRgbF(0.022447f, 0.017199f, 0.097327f).rgb(),
                        QColor::fromRgbF(0.025793f, 0.019331f, 0.105930f).rgb(),
                        QColor::fromRgbF(0.029432f, 0.021503f, 0.114621f).rgb(),
                        QColor::fromRgbF(0.033385f, 0.023702f, 0.123397f).rgb(),
                        QColor::fromRgbF(0.037668f, 0.025921f, 0.132232f).rgb(),
                        QColor::fromRgbF(0.042253f, 0.028139f, 0.141141f).rgb(),
                        QColor::fromRgbF(0.046915f, 0.030324f, 0.150164f).rgb(),
                        QColor::fromRgbF(0.051644f, 0.032474f, 0.159254f).rgb(),
                        QColor::fromRgbF(0.056449f, 0.034569f, 0.168414f).rgb(),
                        QColor::fromRgbF(0.061340f, 0.036590f, 0.177642f).rgb(),
                        QColor::fromRgbF(0.066331f, 0.038504f, 0.186962f).rgb(),
                        QColor::fromRgbF(0.071429f, 0.040294f, 0.196354f).rgb(),
                        QColor::fromRgbF(0.076637f, 0.041905f, 0.205799f).rgb(),
                        QColor::fromRgbF(0.081962f, 0.043328f, 0.215289f).rgb(),
                        QColor::fromRgbF(0.087411f, 0.044556f, 0.224813f).rgb(),
                        QColor::fromRgbF(0.092990f, 0.045583f, 0.234358f).rgb(),
                        QColor::fromRgbF(0.098702f, 0.046402f, 0.243904f).rgb(),
                        QColor::fromRgbF(0.104551f, 0.047008f, 0.253430f).rgb(),
                        QColor::fromRgbF(0.110536f, 0.047399f, 0.262912f).rgb(),
                        QColor::fromRgbF(0.116656f, 0.047574f, 0.272321f).rgb(),
                        QColor::fromRgbF(0.122908f, 0.047536f, 0.281624f).rgb(),
                        QColor::fromRgbF(0.129285f, 0.047293f, 0.290788f).rgb(),
                        QColor::fromRgbF(0.135778f, 0.046856f, 0.299776f).rgb(),
                        QColor::fromRgbF(0.142378f, 0.046242f, 0.308553f).rgb(),
                        QColor::fromRgbF(0.149073f, 0.045468f, 0.317085f).rgb(),
                        QColor::fromRgbF(0.155850f, 0.044559f, 0.325338f).rgb(),
                        QColor::fromRgbF(0.162689f, 0.043554f, 0.333277f).rgb(),
                        QColor::fromRgbF(0.169575f, 0.042489f, 0.340874f).rgb(),
                        QColor::fromRgbF(0.176493f, 0.041402f, 0.348111f).rgb(),
                        QColor::fromRgbF(0.183429f, 0.040329f, 0.354971f).rgb(),
                        QColor::fromRgbF(0.190367f, 0.039309f, 0.361447f).rgb(),
                        QColor::fromRgbF(0.197297f, 0.038400f, 0.367535f).rgb(),
                        QColor::fromRgbF(0.204209f, 0.037632f, 0.373238f).rgb(),
                        QColor::fromRgbF(0.211095f, 0.037030f, 0.378563f).rgb(),
                        QColor::fromRgbF(0.217949f, 0.036615f, 0.383522f).rgb(),
                        QColor::fromRgbF(0.224763f, 0.036405f, 0.388129f).rgb(),
                        QColor::fromRgbF(0.231538f, 0.036405f, 0.392400f).rgb(),
                        QColor::fromRgbF(0.238273f, 0.036621f, 0.396353f).rgb(),
                        QColor::fromRgbF(0.244967f, 0.037055f, 0.400007f).rgb(),
                        QColor::fromRgbF(0.251620f, 0.037705f, 0.403378f).rgb(),
                        QColor::fromRgbF(0.258234f, 0.038571f, 0.406485f).rgb(),
                        QColor::fromRgbF(0.264810f, 0.039647f, 0.409345f).rgb(),
                        QColor::fromRgbF(0.271347f, 0.040922f, 0.411976f).rgb(),
                        QColor::fromRgbF(0.277850f, 0.042353f, 0.414392f).rgb(),
                        QColor::fromRgbF(0.284321f, 0.043933f, 0.416608f).rgb(),
                        QColor::fromRgbF(0.290763f, 0.045644f, 0.418637f).rgb(),
                        QColor::fromRgbF(0.297178f, 0.047470f, 0.420491f).rgb(),
                        QColor::fromRgbF(0.303568f, 0.049396f, 0.422182f).rgb(),
                        QColor::fromRgbF(0.309935f, 0.051407f, 0.423721f).rgb(),
                        QColor::fromRgbF(0.316282f, 0.053490f, 0.425116f).rgb(),
                        QColor::fromRgbF(0.322610f, 0.055634f, 0.426377f).rgb(),
                        QColor::fromRgbF(0.328921f, 0.057827f, 0.427511f).rgb(),
                        QColor::fromRgbF(0.335217f, 0.060060f, 0.428524f).rgb(),
                        QColor::fromRgbF(0.341500f, 0.062325f, 0.429425f).rgb(),
                        QColor::fromRgbF(0.347771f, 0.064616f, 0.430217f).rgb(),
                        QColor::fromRgbF(0.354032f, 0.066925f, 0.430906f).rgb(),
                        QColor::fromRgbF(0.360284f, 0.069247f, 0.431497f).rgb(),
                        QColor::fromRgbF(0.366529f, 0.071579f, 0.431994f).rgb(),
                        QColor::fromRgbF(0.372768f, 0.073915f, 0.432400f).rgb(),
                        QColor::fromRgbF(0.379001f, 0.076253f, 0.432719f).rgb(),
                        QColor::fromRgbF(0.385228f, 0.078591f, 0.432955f).rgb(),
                        QColor::fromRgbF(0.391453f, 0.080927f, 0.433109f).rgb(),
                        QColor::fromRgbF(0.397674f, 0.083257f, 0.433183f).rgb(),
                        QColor::fromRgbF(0.403894f, 0.085580f, 0.433179f).rgb(),
                        QColor::fromRgbF(0.410113f, 0.087896f, 0.433098f).rgb(),
                        QColor::fromRgbF(0.416331f, 0.090203f, 0.432943f).rgb(),
                        QColor::fromRgbF(0.422549f, 0.092501f, 0.432714f).rgb(),
                        QColor::fromRgbF(0.428768f, 0.094790f, 0.432412f).rgb(),
                        QColor::fromRgbF(0.434987f, 0.097069f, 0.432039f).rgb(),
                        QColor::fromRgbF(0.441207f, 0.099338f, 0.431594f).rgb(),
                        QColor::fromRgbF(0.447428f, 0.101597f, 0.431080f).rgb(),
                        QColor::fromRgbF(0.453651f, 0.103848f, 0.430498f).rgb(),
                        QColor::fromRgbF(0.459875f, 0.106089f, 0.429846f).rgb(),
                        QColor::fromRgbF(0.466100f, 0.108322f, 0.429125f).rgb(),
                        QColor::fromRgbF(0.472328f, 0.110547f, 0.428334f).rgb(),
                        QColor::fromRgbF(0.478558f, 0.112764f, 0.427475f).rgb(),
                        QColor::fromRgbF(0.484789f, 0.114974f, 0.426548f).rgb(),
                        QColor::fromRgbF(0.491022f, 0.117179f, 0.425552f).rgb(),
                        QColor::fromRgbF(0.497257f, 0.119379f, 0.424488f).rgb(),
                        QColor::fromRgbF(0.503493f, 0.121575f, 0.423356f).rgb(),
                        QColor::fromRgbF(0.509730f, 0.123769f, 0.422156f).rgb(),
                        QColor::fromRgbF(0.515967f, 0.125960f, 0.420887f).rgb(),
                        QColor::fromRgbF(0.522206f, 0.128150f, 0.419549f).rgb(),
                        QColor::fromRgbF(0.528444f, 0.130341f, 0.418142f).rgb(),
                        QColor::fromRgbF(0.534683f, 0.132534f, 0.416667f).rgb(),
                        QColor::fromRgbF(0.540920f, 0.134729f, 0.415123f).rgb(),
                        QColor::fromRgbF(0.547157f, 0.136929f, 0.413511f).rgb(),
                        QColor::fromRgbF(0.553392f, 0.139134f, 0.411829f).rgb(),
                        QColor::fromRgbF(0.559624f, 0.141346f, 0.410078f).rgb(),
                        QColor::fromRgbF(0.565854f, 0.143567f, 0.408258f).rgb(),
                        QColor::fromRgbF(0.572081f, 0.145797f, 0.406369f).rgb(),
                        QColor::fromRgbF(0.578304f, 0.148039f, 0.404411f).rgb(),
                        QColor::fromRgbF(0.584521f, 0.150294f, 0.402385f).rgb(),
                        QColor::fromRgbF(0.590734f, 0.152563f, 0.400290f).rgb(),
                        QColor::fromRgbF(0.596940f, 0.154848f, 0.398125f).rgb(),
                        QColor::fromRgbF(0.603139f, 0.157151f, 0.395891f).rgb(),
                        QColor::fromRgbF(0.609330f, 0.159474f, 0.393589f).rgb(),
                        QColor::fromRgbF(0.615513f, 0.161817f, 0.391219f).rgb(),
                        QColor::fromRgbF(0.621685f, 0.164184f, 0.388781f).rgb(),
                        QColor::fromRgbF(0.627847f, 0.166575f, 0.386276f).rgb(),
                        QColor::fromRgbF(0.633998f, 0.168992f, 0.383704f).rgb(),
                        QColor::fromRgbF(0.640135f, 0.171438f, 0.381065f).rgb(),
                        QColor::fromRgbF(0.646260f, 0.173914f, 0.378359f).rgb(),
                        QColor::fromRgbF(0.652369f, 0.176421f, 0.375586f).rgb(),
                        QColor::fromRgbF(0.658463f, 0.178962f, 0.372748f).rgb(),
                        QColor::fromRgbF(0.664540f, 0.181539f, 0.369846f).rgb(),
                        QColor::fromRgbF(0.670599f, 0.184153f, 0.366879f).rgb(),
                        QColor::fromRgbF(0.676638f, 0.186807f, 0.363849f).rgb(),
                        QColor::fromRgbF(0.682656f, 0.189501f, 0.360757f).rgb(),
                        QColor::fromRgbF(0.688653f, 0.192239f, 0.357603f).rgb(),
                        QColor::fromRgbF(0.694627f, 0.195021f, 0.354388f).rgb(),
                        QColor::fromRgbF(0.700576f, 0.197851f, 0.351113f).rgb(),
                        QColor::fromRgbF(0.706500f, 0.200728f, 0.347777f).rgb(),
                        QColor::fromRgbF(0.712396f, 0.203656f, 0.344383f).rgb(),
                        QColor::fromRgbF(0.718264f, 0.206636f, 0.340931f).rgb(),
                        QColor::fromRgbF(0.724103f, 0.209670f, 0.337424f).rgb(),
                        QColor::fromRgbF(0.729909f, 0.212759f, 0.333861f).rgb(),
                        QColor::fromRgbF(0.735683f, 0.215906f, 0.330245f).rgb(),
                        QColor::fromRgbF(0.741423f, 0.219112f, 0.326576f).rgb(),
                        QColor::fromRgbF(0.747127f, 0.222378f, 0.322856f).rgb(),
                        QColor::fromRgbF(0.752794f, 0.225706f, 0.319085f).rgb(),
                        QColor::fromRgbF(0.758422f, 0.229097f, 0.315266f).rgb(),
                        QColor::fromRgbF(0.764010f, 0.232554f, 0.311399f).rgb(),
                        QColor::fromRgbF(0.769556f, 0.236077f, 0.307485f).rgb(),
                        QColor::fromRgbF(0.775059f, 0.239667f, 0.303526f).rgb(),
                        QColor::fromRgbF(0.780517f, 0.243327f, 0.299523f).rgb(),
                        QColor::fromRgbF(0.785929f, 0.247056f, 0.295477f).rgb(),
                        QColor::fromRgbF(0.791293f, 0.250856f, 0.291390f).rgb(),
                        QColor::fromRgbF(0.796607f, 0.254728f, 0.287264f).rgb(),
                        QColor::fromRgbF(0.801871f, 0.258674f, 0.283099f).rgb(),
                        QColor::fromRgbF(0.807082f, 0.262692f, 0.278898f).rgb(),
                        QColor::fromRgbF(0.812239f, 0.266786f, 0.274661f).rgb(),
                        QColor::fromRgbF(0.817341f, 0.270954f, 0.270390f).rgb(),
                        QColor::fromRgbF(0.822386f, 0.275197f, 0.266085f).rgb(),
                        QColor::fromRgbF(0.827372f, 0.279517f, 0.261750f).rgb(),
                        QColor::fromRgbF(0.832299f, 0.283913f, 0.257383f).rgb(),
                        QColor::fromRgbF(0.837165f, 0.288385f, 0.252988f).rgb(),
                        QColor::fromRgbF(0.841969f, 0.292933f, 0.248564f).rgb(),
                        QColor::fromRgbF(0.846709f, 0.297559f, 0.244113f).rgb(),
                        QColor::fromRgbF(0.851384f, 0.302260f, 0.239636f).rgb(),
                        QColor::fromRgbF(0.855992f, 0.307038f, 0.235133f).rgb(),
                        QColor::fromRgbF(0.860533f, 0.311892f, 0.230606f).rgb(),
                        QColor::fromRgbF(0.865006f, 0.316822f, 0.226055f).rgb(),
                        QColor::fromRgbF(0.869409f, 0.321827f, 0.221482f).rgb(),
                        QColor::fromRgbF(0.873741f, 0.326906f, 0.216886f).rgb(),
                        QColor::fromRgbF(0.878001f, 0.332060f, 0.212268f).rgb(),
                        QColor::fromRgbF(0.882188f, 0.337287f, 0.207628f).rgb(),
                        QColor::fromRgbF(0.886302f, 0.342586f, 0.202968f).rgb(),
                        QColor::fromRgbF(0.890341f, 0.347957f, 0.198286f).rgb(),
                        QColor::fromRgbF(0.894305f, 0.353399f, 0.193584f).rgb(),
                        QColor::fromRgbF(0.898192f, 0.358911f, 0.188860f).rgb(),
                        QColor::fromRgbF(0.902003f, 0.364492f, 0.184116f).rgb(),
                        QColor::fromRgbF(0.905735f, 0.370140f, 0.179350f).rgb(),
                        QColor::fromRgbF(0.909390f, 0.375856f, 0.174563f).rgb(),
                        QColor::fromRgbF(0.912966f, 0.381636f, 0.169755f).rgb(),
                        QColor::fromRgbF(0.916462f, 0.387481f, 0.164924f).rgb(),
                        QColor::fromRgbF(0.919879f, 0.393389f, 0.160070f).rgb(),
                        QColor::fromRgbF(0.923215f, 0.399359f, 0.155193f).rgb(),
                        QColor::fromRgbF(0.926470f, 0.405389f, 0.150292f).rgb(),
                        QColor::fromRgbF(0.929644f, 0.411479f, 0.145367f).rgb(),
                        QColor::fromRgbF(0.932737f, 0.417627f, 0.140417f).rgb(),
                        QColor::fromRgbF(0.935747f, 0.423831f, 0.135440f).rgb(),
                        QColor::fromRgbF(0.938675f, 0.430091f, 0.130438f).rgb(),
                        QColor::fromRgbF(0.941521f, 0.436405f, 0.125409f).rgb(),
                        QColor::fromRgbF(0.944285f, 0.442772f, 0.120354f).rgb(),
                        QColor::fromRgbF(0.946965f, 0.449191f, 0.115272f).rgb(),
                        QColor::fromRgbF(0.949562f, 0.455660f, 0.110164f).rgb(),
                        QColor::fromRgbF(0.952075f, 0.462178f, 0.105031f).rgb(),
                        QColor::fromRgbF(0.954506f, 0.468744f, 0.099874f).rgb(),
                        QColor::fromRgbF(0.956852f, 0.475356f, 0.094695f).rgb(),
                        QColor::fromRgbF(0.959114f, 0.482014f, 0.089499f).rgb(),
                        QColor::fromRgbF(0.961293f, 0.488716f, 0.084289f).rgb(),
                        QColor::fromRgbF(0.963387f, 0.495462f, 0.079073f).rgb(),
                        QColor::fromRgbF(0.965397f, 0.502249f, 0.073859f).rgb(),
                        QColor::fromRgbF(0.967322f, 0.509078f, 0.068659f).rgb(),
                        QColor::fromRgbF(0.969163f, 0.515946f, 0.063488f).rgb(),
                        QColor::fromRgbF(0.970919f, 0.522853f, 0.058367f).rgb(),
                        QColor::fromRgbF(0.972590f, 0.529798f, 0.053324f).rgb(),
                        QColor::fromRgbF(0.974176f, 0.536780f, 0.048392f).rgb(),
                        QColor::fromRgbF(0.975677f, 0.543798f, 0.043618f).rgb(),
                        QColor::fromRgbF(0.977092f, 0.550850f, 0.039050f).rgb(),
                        QColor::fromRgbF(0.978422f, 0.557937f, 0.034931f).rgb(),
                        QColor::fromRgbF(0.979666f, 0.565057f, 0.031409f).rgb(),
                        QColor::fromRgbF(0.980824f, 0.572209f, 0.028508f).rgb(),
                        QColor::fromRgbF(0.981895f, 0.579392f, 0.026250f).rgb(),
                        QColor::fromRgbF(0.982881f, 0.586606f, 0.024661f).rgb(),
                        QColor::fromRgbF(0.983779f, 0.593849f, 0.023770f).rgb(),
                        QColor::fromRgbF(0.984591f, 0.601122f, 0.023606f).rgb(),
                        QColor::fromRgbF(0.985315f, 0.608422f, 0.024202f).rgb(),
                        QColor::fromRgbF(0.985952f, 0.615750f, 0.025592f).rgb(),
                        QColor::fromRgbF(0.986502f, 0.623105f, 0.027814f).rgb(),
                        QColor::fromRgbF(0.986964f, 0.630485f, 0.030908f).rgb(),
                        QColor::fromRgbF(0.987337f, 0.637890f, 0.034916f).rgb(),
                        QColor::fromRgbF(0.987622f, 0.645320f, 0.039886f).rgb(),
                        QColor::fromRgbF(0.987819f, 0.652773f, 0.045581f).rgb(),
                        QColor::fromRgbF(0.987926f, 0.660250f, 0.051750f).rgb(),
                        QColor::fromRgbF(0.987945f, 0.667748f, 0.058329f).rgb(),
                        QColor::fromRgbF(0.987874f, 0.675267f, 0.065257f).rgb(),
                        QColor::fromRgbF(0.987714f, 0.682807f, 0.072489f).rgb(),
                        QColor::fromRgbF(0.987464f, 0.690366f, 0.079990f).rgb(),
                        QColor::fromRgbF(0.987124f, 0.697944f, 0.087731f).rgb(),
                        QColor::fromRgbF(0.986694f, 0.705540f, 0.095694f).rgb(),
                        QColor::fromRgbF(0.986175f, 0.713153f, 0.103863f).rgb(),
                        QColor::fromRgbF(0.985566f, 0.720782f, 0.112229f).rgb(),
                        QColor::fromRgbF(0.984865f, 0.728427f, 0.120785f).rgb(),
                        QColor::fromRgbF(0.984075f, 0.736087f, 0.129527f).rgb(),
                        QColor::fromRgbF(0.983196f, 0.743758f, 0.138453f).rgb(),
                        QColor::fromRgbF(0.982228f, 0.751442f, 0.147565f).rgb(),
                        QColor::fromRgbF(0.981173f, 0.759135f, 0.156863f).rgb(),
                        QColor::fromRgbF(0.980032f, 0.766837f, 0.166353f).rgb(),
                        QColor::fromRgbF(0.978806f, 0.774545f, 0.176037f).rgb(),
                        QColor::fromRgbF(0.977497f, 0.782258f, 0.185923f).rgb(),
                        QColor::fromRgbF(0.976108f, 0.789974f, 0.196018f).rgb(),
                        QColor::fromRgbF(0.974638f, 0.797692f, 0.206332f).rgb(),
                        QColor::fromRgbF(0.973088f, 0.805409f, 0.216877f).rgb(),
                        QColor::fromRgbF(0.971468f, 0.813122f, 0.227658f).rgb(),
                        QColor::fromRgbF(0.969783f, 0.820825f, 0.238686f).rgb(),
                        QColor::fromRgbF(0.968041f, 0.828515f, 0.249972f).rgb(),
                        QColor::fromRgbF(0.966243f, 0.836191f, 0.261534f).rgb(),
                        QColor::fromRgbF(0.964394f, 0.843848f, 0.273391f).rgb(),
                        QColor::fromRgbF(0.962517f, 0.851476f, 0.285546f).rgb(),
                        QColor::fromRgbF(0.960626f, 0.859069f, 0.298010f).rgb(),
                        QColor::fromRgbF(0.958720f, 0.866624f, 0.310820f).rgb(),
                        QColor::fromRgbF(0.956834f, 0.874129f, 0.323974f).rgb(),
                        QColor::fromRgbF(0.954997f, 0.881569f, 0.337475f).rgb(),
                        QColor::fromRgbF(0.953215f, 0.888942f, 0.351369f).rgb(),
                        QColor::fromRgbF(0.951546f, 0.896226f, 0.365627f).rgb(),
                        QColor::fromRgbF(0.950018f, 0.903409f, 0.380271f).rgb(),
                        QColor::fromRgbF(0.948683f, 0.910473f, 0.395289f).rgb(),
                        QColor::fromRgbF(0.947594f, 0.917399f, 0.410665f).rgb(),
                        QColor::fromRgbF(0.946809f, 0.924168f, 0.426373f).rgb(),
                        QColor::fromRgbF(0.946392f, 0.930761f, 0.442367f).rgb(),
                        QColor::fromRgbF(0.946403f, 0.937159f, 0.458592f).rgb(),
                        QColor::fromRgbF(0.946903f, 0.943348f, 0.474970f).rgb(),
                        QColor::fromRgbF(0.947937f, 0.949318f, 0.491426f).rgb(),
                        QColor::fromRgbF(0.949545f, 0.955063f, 0.507860f).rgb(),
                        QColor::fromRgbF(0.951740f, 0.960587f, 0.524203f).rgb(),
                        QColor::fromRgbF(0.954529f, 0.965896f, 0.540361f).rgb(),
                        QColor::fromRgbF(0.957896f, 0.971003f, 0.556275f).rgb(),
                        QColor::fromRgbF(0.961812f, 0.975924f, 0.571925f).rgb(),
                        QColor::fromRgbF(0.966249f, 0.980678f, 0.587206f).rgb(),
                        QColor::fromRgbF(0.971162f, 0.985282f, 0.602154f).rgb(),
                        QColor::fromRgbF(0.976511f, 0.989753f, 0.616760f).rgb(),
                        QColor::fromRgbF(0.982257f, 0.994109f, 0.631017f).rgb(),
                        QColor::fromRgbF(0.988362f, 0.998364f, 0.644924f).rgb()};

            {
                const auto palettei=JKQTPMathImageINVERTED_Inferno;
                const QString palN="invinferno";
                const QString palNT=QObject::tr("inv. inferno");
                lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
                lutstore[palettei].lut=lutstore[palette].lut;
                std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
            }
            {
                const auto palettei=JKQTPMathImageInferno_STEP;
                const QString palN="inferno_step";
                const QString palNT=QObject::tr("steps: inferno");
                lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
                lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
            }
            {
                const auto palettei=JKQTPMathImageINVERTED_Inferno_STEP;
                const QString palN="invinferno_step";
                const QString palNT=QObject::tr("steps: inv. inferno");
                lutstore[palettei]=JKQTPImageTools::LUTData( palN, palNT);
                lutstore[palettei].lut=JKQTPBuildColorPaletteLUTBySubsampling(lutstore[palette].lut, NDEFAULTSTEPS);
                std::reverse(lutstore[palettei].lut.begin(), lutstore[palettei].lut.end());
            }

        }

    {
        const auto palette=JKQTPMathImageIBMColorBlindSafe;
        const QString palN="IBMColorBlindSafe";
        const QString palNT=QObject::tr("IBMColorBlindSafe");
        lutstore[palette]=JKQTPImageTools::LUTData(palN, palNT);
        JKQTPPaletteList lst;
        lst<<jkqtp_qRgbOpaque(0x648FFF);
        lst<<jkqtp_qRgbOpaque(0x785EF0);
        lst<<jkqtp_qRgbOpaque(0xDC267F);
        lst<<jkqtp_qRgbOpaque(0xFE6100);
        lst<<jkqtp_qRgbOpaque(0xFFB000);

        lutstore[palette].lut=JKQTPBuildColorPaletteLUTLinInterpolate(lst);
        {
            const auto palettei=JKQTPMathImageIBMColorBlindSafe_STEP;
            lutstore[palettei]=JKQTPImageTools::LUTData("IBMColorBlindSafe_step", QObject::tr("steps: IBMColorBlindSafe"));
            lutstore[palettei].lut=JKQTPBuildColorPaletteLUTColorsOnlySteps(lst);
        }
    }




    {
        JKQTPPaletteList lst;
        lst<<jkqtp_qRgbOpaque(0x000000);
        lst<<jkqtp_qRgbOpaque(0xE69F00);
        lst<<jkqtp_qRgbOpaque(0x56B4E9);
        lst<<jkqtp_qRgbOpaque(0x009E73);
        lst<<jkqtp_qRgbOpaque(0xF0E442);
        lst<<jkqtp_qRgbOpaque(0x0072B2);
        lst<<jkqtp_qRgbOpaque(0xD55E00);
        lst<<jkqtp_qRgbOpaque(0xCC79A7);

        lutstore[JKQTPMathImageOkabeIto_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "OkabeIto_step", QObject::tr("steps: Okabe-Ito"));
        lutstore[JKQTPMathImageOkabeItoDarker_STEP]=JKQTPImageTools::LUTData(JKQTPModifyLUT(lutstore[JKQTPMathImageOkabeIto_STEP].lut, [](QRgb c) { return QColor(c).darker().rgb(); }), "OkabeItoDarker_step", QObject::tr("steps: Okabe-Ito, darkened"));
        lutstore[JKQTPMathImageOkabeItoLighter_STEP]=JKQTPImageTools::LUTData(JKQTPModifyLUT(lutstore[JKQTPMathImageOkabeIto_STEP].lut, [](QRgb c) { return QColor(c).lighter().rgb(); }), "OkabeItoLighter_step", QObject::tr("steps: Okabe-Ito, lighter"));

    }
    {
        JKQTPPaletteList lst;
        lst<<jkqtp_qRgbOpaque(0xEE6677);
        lst<<jkqtp_qRgbOpaque(0x228833);
        lst<<jkqtp_qRgbOpaque(0x4477AA);
        lst<<jkqtp_qRgbOpaque(0x009E73);
        lst<<jkqtp_qRgbOpaque(0xCCBB44);
        lst<<jkqtp_qRgbOpaque(0x66CCEE);
        lst<<jkqtp_qRgbOpaque(0xAA3377);
        lst<<jkqtp_qRgbOpaque(0xBBBBBB);

        lutstore[JKQTPMathImageTol_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "Tol_step", QObject::tr("steps: Tol"));

    }
    {
        JKQTPPaletteList lst;
        lst<<jkqtp_qRgbOpaque(0xBBCC33);
        lst<<jkqtp_qRgbOpaque(0xAAAA00);
        lst<<jkqtp_qRgbOpaque(0x77AADD);
        lst<<jkqtp_qRgbOpaque(0xEE8866);
        lst<<jkqtp_qRgbOpaque(0xEEDD88);
        lst<<jkqtp_qRgbOpaque(0xFFAABB);
        lst<<jkqtp_qRgbOpaque(0x99DDFF);
        lst<<jkqtp_qRgbOpaque(0x44BB99);
        lst<<jkqtp_qRgbOpaque(0xDDDDDD);

        lutstore[JKQTPMathImageTolLight_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "TolLight_step", QObject::tr("steps: Tol, Light"));

    }
    {
        JKQTPPaletteList lst;
        lst<<jkqtp_qRgbOpaque(0x88CCEE);
        lst<<jkqtp_qRgbOpaque(0x44AA99);
        lst<<jkqtp_qRgbOpaque(0x117733);
        lst<<jkqtp_qRgbOpaque(0x332288);
        lst<<jkqtp_qRgbOpaque(0xDDCC77);
        lst<<jkqtp_qRgbOpaque(0x999933);
        lst<<jkqtp_qRgbOpaque(0xCC6677);
        lst<<jkqtp_qRgbOpaque(0x882255);
        lst<<jkqtp_qRgbOpaque(0xAA4499);
        lst<<jkqtp_qRgbOpaque(0xDDDDDD);

        lutstore[JKQTPMathImageTolMuted_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "TolMuted_step", QObject::tr("steps: Tol, Muted"));

    }
    {
        JKQTPPaletteList lst;
        lst<<QColor::fromRgbF(0.0000, 0.4470, 0.7410);
        lst<<QColor::fromRgbF(0.8500, 0.3250, 0.0980);
        lst<<QColor::fromRgbF(0.9290, 0.6940, 0.1250);
        lst<<QColor::fromRgbF(0.4940, 0.1840, 0.5560);
        lst<<QColor::fromRgbF(0.4660, 0.6740, 0.1880);
        lst<<QColor::fromRgbF(0.3010, 0.7450, 0.9330);
        lst<<QColor::fromRgbF(0.6350, 0.0780, 0.1840);

        lutstore[JKQTPMathImageMatlab_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "Matlab_step", QObject::tr("steps: Matlab"));

    }
    {
        JKQTPPaletteList lst;
        lst<<jkqtp_qRgbOpaque(0x1F77B4);
        lst<<jkqtp_qRgbOpaque(0xFF7F0E);
        lst<<jkqtp_qRgbOpaque(0x2CA02C);
        lst<<jkqtp_qRgbOpaque(0xD62728);
        lst<<jkqtp_qRgbOpaque(0x9467BD);
        lst<<jkqtp_qRgbOpaque(0x8C564B);
        lst<<jkqtp_qRgbOpaque(0xE377C2);
        lst<<jkqtp_qRgbOpaque(0x7F7F7F);
        lst<<jkqtp_qRgbOpaque(0xBCBD22);
        lst<<jkqtp_qRgbOpaque(0x17BECF);
        lutstore[JKQTPMathImageMatplotlib_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "Matplotlib_step", QObject::tr("steps: Matplotlib"));
    }
    {
        JKQTPPaletteList lst;
        lst<<QColor::fromRgbF(0, 0, 1);
        lst<<QColor::fromRgbF(0, 0.5, 0);
        lst<<QColor::fromRgbF(1, 0, 0);
        lst<<QColor::fromRgbF(0, 0.75, 0.75);
        lst<<QColor::fromRgbF(0.75, 0, 0.75);
        lst<<QColor::fromRgbF(0.75, 0.75, 0);
        lst<<QColor::fromRgbF(0.25, 0.25, 0.25);

        lutstore[JKQTPMathImageMatlabLegacy_STEP]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTColorsOnlySteps(lst), "MatlabLegacy_step", QObject::tr("steps: Matlab, Legacy"));

    }


    {
        JKQTPPaletteList lst;
        JKQTPPaletteList lst_;
        for (int i=0; i<16; i++) {
            lst<<QColor("red")<<QColor(255,127,0)<<QColor(255,255,0)<<QColor("green")<<QColor("blue")<<QColor(170,0,255);
            if (i<8) lst_<<QColor("red")<<QColor(255,127,0)<<QColor(255,255,0)<<QColor("green")<<QColor("blue")<<QColor(170,0,255);
        }
        lutstore[JKQTPMathImagePrism16]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "prism16", QObject::tr("prism16 (cycling)"));
        lutstore[JKQTPMathImagePrism8]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst_),  "prism8", QObject::tr("prism8 (cycling)"));
    }

    {
        JKQTPPaletteList lst;
        JKQTPPaletteList lst_;
        for (int i=0; i<16; i++) {
            lst<<QColor("red")<<QColor("white")<<QColor("blue")<<QColor("black");
            if (i<8) lst_<<QColor("red")<<QColor("white")<<QColor("blue")<<QColor("black");
        }
        lutstore[JKQTPMathImageFlag16]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst),  "flag16", QObject::tr("flag16 (cycling)"));
        lutstore[JKQTPMathImageFlag8]=JKQTPImageTools::LUTData(JKQTPBuildColorPaletteLUTLinInterpolate(lst_),  "flag8", QObject::tr("flag8 (cycling)"));
    }

    lutstore[JKQTPMathImageCubeHelixClassic]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(0.5, -1.5, 1.0, LUTSIZE, 0.0, 1.0, 1.2, 1.2), "CubeHelixClassic", QObject::tr("CubeHelix Classic"));
    lutstore[JKQTPMathImageCubeHelixClassic_STEP]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(0.5, -1.5, 1.0, JKQTPImageTools::NDEFAULTSTEPS, 0.0, 1.0, 1.2, 1.2), "CubeHelixClassic_step", QObject::tr("steps: CubeHelix Classic"));
    lutstore[JKQTPMathImageCubeHelix1]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(1.5, -1.0, 1.0, LUTSIZE, 0.0, 1.0, 1.5, 1.5), "CubeHelix1", QObject::tr("CubeHelix 1"));
    lutstore[JKQTPMathImageCubeHelix1_STEP]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(1.5, -1.0, 1.0, JKQTPImageTools::NDEFAULTSTEPS, 0.0, 1.0, 1.5, 1.5), "CubeHelix1_step", QObject::tr("steps: CubeHelix 1"));
    lutstore[JKQTPMathImageCubeHelix2]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(2.0, -1.0, 1.0, LUTSIZE, 0.0, 1.0, 1.5, 1.5), "CubeHelix2", QObject::tr("CubeHelix 2"));
    lutstore[JKQTPMathImageCubeHelix2_STEP]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(2.0, -1.0, 1.0, JKQTPImageTools::NDEFAULTSTEPS, 0.0, 1.0, 1.5, 1.5), "CubeHelix2_step", QObject::tr("steps: CubeHelix 2"));
    lutstore[JKQTPMathImageCubeHelix3]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(2.0, -1.0, 1.0, LUTSIZE, 0.0, 1.0, 1.5, 1.5), "CubeHelix3", QObject::tr("CubeHelix 3"));
    lutstore[JKQTPMathImageCubeHelix3_STEP]=JKQTPImageTools::LUTData(JKQTPCreateGreensCubeHelixLUT(2.0, 1.0, 1.0, JKQTPImageTools::NDEFAULTSTEPS, 0.0, 1.0, 3.0, 3.0), "CubeHelix3_step", QObject::tr("steps: CubeHelix 3"));
    lutstore[JKQTPMathImageSeaborn_STEP]=JKQTPImageTools::LUTData("#4C72B0, #DD8452, #55A868, #C44E52, #8172B3, #937860, #DA8BC3, #8C8C8C, #CCB974, #64B5CD", "Seaborn_step", QObject::tr("steps: Seaborn"));
    lutstore[JKQTPMathImageSeabornPastel_STEP]=JKQTPImageTools::LUTData("#A1C9F4, #FFB482, #8DE5A1, #FF9F9B, #D0BBFF, #DEBB9B, #FAB0E4, #CFCFCF, #FFFEA3, #B9F2F0", "SeabornPastel_step", QObject::tr("steps: Seaborn Pastel"));

    lutstore[JKQTPMathImagePaired12_STEP]=JKQTPImageTools::LUTData("#a6cee3, #1f78b4, #b2df8a, #33a02c, #fb9a99, #e31a1c, #fdbf6f, #ff7f00, #cab2d6, #6a3d9a,  #ffff99, #b15928", "paired12_step", QObject::tr("steps:  ColorBrewer-Paired-12"));
    lutstore[JKQTPMathImagePaired10_STEP]=JKQTPImageTools::LUTData("#a6cee3, #1f78b4, #b2df8a, #33a02c, #fb9a99, #e31a1c, #fdbf6f, #ff7f00, #cab2d6, #6a3d9a", "paired10_step", QObject::tr("steps:  ColorBrewer-Paired-10"));
    lutstore[JKQTPMathImageSet3_STEP]=JKQTPImageTools::LUTData("#8dd3c7, #ffffb3, #bebada, #fb8072, #80b1d3, #fdb462, #b3de69, #fccde5, #d9d9d9, #bc80bd, #ccebc5, #ffed6f", "set3_step", QObject::tr("steps:  ColorBrewer-Set3"));
    lutstore[JKQTPMathImageAccent_STEP]=JKQTPImageTools::LUTData("#7fc97f, #beaed4, #fdc086, #ffff99, #386cb0, #f0027f, #bf5b17, #666666", "accent_step", QObject::tr("steps:  ColorBrewer-Accent"));
    lutstore[JKQTPMathImageDark2_STEP]=JKQTPImageTools::LUTData("#1b9e77, #d95f02, #7570b3, #e7298a, #66a61e, #e6ab02, #a6761d, #666666", "dark2_step", QObject::tr("steps:  ColorBrewer-Dark2"));
    lutstore[JKQTPMathImagePastel1_STEP]=JKQTPImageTools::LUTData("#fbb4ae, #b3cde3, #ccebc5, #decbe4, #fed9a6, #ffffcc, #e5d8bd, #fddaec", "pastel1_step", QObject::tr("steps:  ColorBrewer-Pastel1"));
    lutstore[JKQTPMathImagePastel2_STEP]=JKQTPImageTools::LUTData("#b3e2cd, #fdcdac, #cbd5e8, #f4cae4, #e6f5c9, #fff2ae, #f1e2cc, #cccccc", "pastel2_step", QObject::tr("steps:  ColorBrewer-Pastel2"));
    lutstore[JKQTPMathImageSet1_STEP]=JKQTPImageTools::LUTData("#e41a1c, #377eb8, #4daf4a, #984ea3, #ff7f00, #ffff33, #a65628, #f781bf", "set1_step", QObject::tr("steps:  ColorBrewer-Set1"));
    lutstore[JKQTPMathImageSet2_STEP]=JKQTPImageTools::LUTData("#66c2a5, #fc8d62, #8da0cb, #e78ac3, #a6d854, #ffd92f, #e5c494, #b3b3b3", "set2_step", QObject::tr("steps:  ColorBrewer-Set2"));


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

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTLinInterpolate(JKQTPPaletteList items, int lut_size) {
    std::sort(items.begin(), items.end(), JKQTPBuildColorPaletteLUTLessThan);
    return JKQTPBuildColorPaletteLUTLinInterpolateSorted(items, lut_size);
}



JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUT(JKQTPPaletteList items, int lut_size)  {
    std::sort(items.begin(), items.end(), JKQTPBuildColorPaletteLUTLessThan);
    return JKQTPBuildColorPaletteLUTSorted(items, lut_size);
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTColorsOnlySteps(JKQTPPaletteList items)  {
    std::sort(items.begin(), items.end(), JKQTPBuildColorPaletteLUTLessThan);
    JKQTPImageTools::LUTType lut;
    for (const auto& it: items) {
        lut.push_back(it.second);
    }

    return lut;
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

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTLinInterpolateSorted(const JKQTPPaletteList &items, int lut_size)
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

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTSorted(const JKQTPPaletteList &items, int lut_size)
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
    JKQTPPaletteList itemsi;
    for (auto it=items.begin(); it!=items.end(); ++it) {
        itemsi.append(QPair<double, QRgb>(it.key(), it.value()));
    }
    return JKQTPBuildColorPaletteLUTLinInterpolateSorted(itemsi, lut_size);
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUT(const QMap<double, QRgb> &items, int lut_size)
{
    JKQTPPaletteList itemsi;
    for (auto it=items.begin(); it!=items.end(); ++it) {
        itemsi.append(QPair<double, QRgb>(it.key(), it.value()));
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


JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUTBySubsampling(const JKQTPImageTools::LUTType &lut, int lut_size)
{
    JKQTPImageTools::LUTType out;
    out.reserve(lut_size);
    for (int i=0; i<lut_size; i++) {
        const int step=jkqtp_bounded<int>(0, i*(lut.size()-1)/lut_size, lut.size()-1);
        out.append(lut[step]);
    }
    return out;
}

template <class T1, class T2>
bool JKQTPImagePlot_QPairCompareFirst(const QPair<T1, T2> &s1, const QPair<T1, T2> &s2) {
    return s1.first < s2.first;
}

QStringList JKQTPImageTools::getPredefinedPalettes()  {
    std::lock_guard<std::mutex> lock(JKQTPImageTools::lutMutex);
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
    std::lock_guard<std::mutex> lock(JKQTPImageTools::lutMutex);
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
    std::lock_guard<std::mutex> lock(JKQTPImageTools::lutMutex);
    auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.find(p);
    if (it==JKQTPImageTools::global_jkqtpimagetools_lutstore.end()) return QString::number(static_cast<int>(p));
    else {
        if (it.value().name.size()>0) return it.value().name;
        else return QString::number(static_cast<int>(p));
    }
}

QString JKQTPImageTools::JKQTPMathImageColorPalette2StringHumanReadable(JKQTPMathImageColorPalette p)
{
    std::lock_guard<std::mutex> lock(JKQTPImageTools::lutMutex);
    auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.find(p);
    if (it==JKQTPImageTools::global_jkqtpimagetools_lutstore.end()) return QString::number(static_cast<int>(p));
    else {
        if (it.value().nameT.size()>0) return it.value().nameT;
        else if (it.value().name.size()>0) return it.value().name;
        else return QString::number(static_cast<int>(p));
    }
}

JKQTPMathImageColorPalette JKQTPImageTools::String2JKQTPMathImageColorPalette(const QString &p)
{
    std::lock_guard<std::mutex> lock(JKQTPImageTools::lutMutex);
    for (auto it=JKQTPImageTools::global_jkqtpimagetools_lutstore.begin(); it!=JKQTPImageTools::global_jkqtpimagetools_lutstore.end(); ++it) {
        if (QString::compare(p, it.value().name, Qt::CaseInsensitive)==0) {
            return static_cast<JKQTPMathImageColorPalette>(it.key());
        }
        for (const auto& ln: it.value().legacyNames) {
            if (QString::compare(p, ln, Qt::CaseInsensitive)==0) {
                return static_cast<JKQTPMathImageColorPalette>(it.key());
            }
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













const JKQTPImageTools::LUTType& JKQTPImageTools::getLUTforPalette(const QMap<int, JKQTPImageTools::LUTData >& lutstore, JKQTPMathImageColorPalette palette) {
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
    const QImage img=JKQTPImageTools::GetPaletteImage(i, JKQTPImageTools::PALETTE_ICON_WIDTH);
    QPixmap pix(JKQTPImageTools::PALETTE_ICON_WIDTH,8);
    const QRect r(0,0,JKQTPImageTools::PALETTE_ICON_WIDTH-1,7);
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

const JKQTPImageTools::LUTType &JKQTPImageTools::getLUTforPalette(JKQTPMathImageColorPalette palette)
{
    return getLUTforPalette(JKQTPImageTools::global_jkqtpimagetools_lutstore, palette);
}

QVector<QColor> JKQTPImageTools::getColorsforPalette(JKQTPMathImageColorPalette palette)
{
    QVector<QColor> cols;
    const auto& lut=JKQTPImageTools::getLUTforPalette(palette);
    cols.reserve(lut.size());
    std::for_each(lut.begin(), lut.end(), [&](QRgb c) { cols.push_back(c); });
    return cols;
}









int JKQTPImageTools::registerPalette(const QString &name, const JKQTPImageTools::LUTType &paletteLut, const QString &nameT)
{
    std::lock_guard<std::mutex> lock(JKQTPImageTools::lutMutex);
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
            JKQTPPaletteList pal;
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
            QRegularExpression rx3("\\s*([0-9eE.+-]+)\\s*([,\\t ])\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*", QRegularExpression::CaseInsensitiveOption|QRegularExpression::InvertedGreedinessOption);
            QRegularExpression rx4("\\s*([0-9eE.+-]+)\\s*([,\\t ])\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*", QRegularExpression::CaseInsensitiveOption|QRegularExpression::InvertedGreedinessOption);
#else
            QRegExp rx3("\\s*([0-9eE.+-]+)\\s*([,\\t ])\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*", Qt::CaseInsensitive);
            rx3.setMinimal(false);
            QRegExp rx4("\\s*([0-9eE.+-]+)\\s*([,\\t ])\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*\\2\\s*([0-9eE.+-]+)\\s*", Qt::CaseInsensitive);
            rx4.setMinimal(false);
#endif

            // determine format
            for (int i=slt.size()-1; i>=0; i--) {
                slt[i]=slt[i].trimmed();
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
                const auto m4=rx4.match(slt[i]);
                const auto m3=rx3.match(slt[i]);
                if (m4.hasMatch()) {
                    const double r=JKQTPImagePlot_QStringToDouble(m4.captured(3));
                    const double g=JKQTPImagePlot_QStringToDouble(m4.captured(4));
                    const double b=JKQTPImagePlot_QStringToDouble(m4.captured(5));
#else
                if (rx4.indexIn(slt[i])>=0) {
                    const double r=JKQTPImagePlot_QStringToDouble(rx4.cap(3));
                    const double g=JKQTPImagePlot_QStringToDouble(rx4.cap(4));
                    const double b=JKQTPImagePlot_QStringToDouble(rx4.cap(5));
#endif
                    has4=true;
                    if (r>1.0 || g>1.0 || b>1.0) {
                        rgb255=true;
                    }
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
                } else if (m3.hasMatch()) {
                    const double r=JKQTPImagePlot_QStringToDouble(m3.captured(1));
                    const double g=JKQTPImagePlot_QStringToDouble(m3.captured(3));
                    const double b=JKQTPImagePlot_QStringToDouble(m3.captured(4));
#else
                } else if (rx3.indexIn(slt[i])>=0) {
                    double r=JKQTPImagePlot_QStringToDouble(rx3.cap(1));
                    double g=JKQTPImagePlot_QStringToDouble(rx3.cap(3));
                    double b=JKQTPImagePlot_QStringToDouble(rx3.cap(4));
#endif
                    has4=false;
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
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
                const auto m4=rx4.match(slt[i]);
                const auto m3=rx3.match(slt[i]);
                if (has4 && m4.hasMatch()) {
                    x=JKQTPImagePlot_QStringToDouble(m4.captured(1));
                    r=JKQTPImagePlot_QStringToDouble(m4.captured(3));
                    g=JKQTPImagePlot_QStringToDouble(m4.captured(4));
                    b=JKQTPImagePlot_QStringToDouble(m4.captured(5));
                } else if (!has4 && m3.hasMatch()) {
                    x=i;
                    r=JKQTPImagePlot_QStringToDouble(m3.captured(1));
                    g=JKQTPImagePlot_QStringToDouble(m3.captured(3));
                    b=JKQTPImagePlot_QStringToDouble(m3.captured(4));
#else
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
#endif
                } else {
                    ok=false;
                    break;
                }
                if (ok) {
                    if (!rgb255) {
                        pal<<QPair<double,QRgb>(x, qRgb(qBound(0,static_cast<int>(round(255*r)), 255), qBound(0,static_cast<int>(round(255*g)), 255), qBound(0,static_cast<int>(round(255*b)), 255)));
                    } else {
                        pal<<QPair<double,QRgb>(x, qRgb(qBound(0,static_cast<int>(round(r)), 255), qBound(0,static_cast<int>(round(g)), 255), qBound(0,static_cast<int>(round(b)), 255)));
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
                    JKQTPPaletteList pal;
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

JKQTPImageTools::LUTData::LUTData(const QString &_lut, const QString &_name, const QString &_nameT):
    lut(), name(_name), nameT(_nameT)
{
    const auto lt=_lut.simplified().split(",");
    for (const auto& l: lt) {
        lut<<jkqtp_String2QColor(l).rgb();
    }
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

void JKQTPModifyImage(QImage &img, JKQTPMathImageModifierMode modifierMode, const void *dataModifier, JKQTPMathImageDataType datatypeModifier, int Nx, int Ny, double internalModifierMin, double internalModifierMax)
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
            case JKQTPMathImageDataType::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<const double*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<const float*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<const uint8_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<const uint16_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<const uint32_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<const uint64_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<const int8_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<const int16_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<const int32_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageDataType::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<const int64_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
        }
    }

}


JKQTPImageTools::LUTType JKQTPModifyLUT(JKQTPImageTools::LUTType lut, std::function<QRgb (int, QRgb)> f)
{
    int i=0;
    for (auto& c: lut) {
        c=f(i, c);
        i++;
    }
    return lut;
}

JKQTPImageTools::LUTType JKQTPModifyLUT(JKQTPImageTools::LUTType lut, std::function<QRgb (QRgb)> f)
{
    for (auto& c: lut) {
        c=f(c);
    }
    return lut;
}

JKQTPImageTools::LUTType JKQTPCreateGreensCubeHelixLUT(float start, float rotation, float gamma, int lutsize, float lambda_min, float lambda_max, float saturation_min, float saturation_max)
{
    JKQTPImageTools::LUTType res;
    for (int i=0; i<lutsize; i++) {
        const float lambda=lambda_min+static_cast<float>(i)/static_cast<float>(lutsize-1)*(lambda_max-lambda_min);
        const float sat=saturation_min+static_cast<float>(i)/static_cast<float>(lutsize-1)*(saturation_max-saturation_min);
        const float lambda_gamma=pow(lambda, gamma);
        const float phi=2.0*JKQTPSTATISTICS_PI*(start/3.0+rotation*lambda);
        const float cosphi=cos(phi);
        const float sinphi=sin(phi);
        const float amplitude=sat*lambda_gamma*(1-lambda_gamma)/2.0;
        const float R=lambda_gamma + amplitude*(-0.14861*cosphi+1.78277*sinphi);
        const float G=lambda_gamma + amplitude*(-0.29227*cosphi-0.90649*sinphi);
        const float B=lambda_gamma + amplitude*(1.972940*cosphi+0.0*sinphi);
        res<<QColor::fromRgbF(jkqtp_bounded<float>(0.0,R,1.0), jkqtp_bounded<float>(0.0,G,1.0), jkqtp_bounded<float>(0.0,B,1.0)).rgb();
    }
    return res;
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUT(const std::function<QRgb (float)> &palFunc, int lut_size, float vMin, float vMax)
{
    JKQTPImageTools::LUTType res;
    const float delta=(vMax-vMin)/static_cast<float>(lut_size-1);
    for (int i=0; i<lut_size; i++) {
        const float v=vMin+static_cast<float>(i)*delta;
        res<<palFunc(v);
    }
    return res;
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUT(const std::function<float (float)> &rFunc, const std::function<float (float)> &gFunc, const std::function<float (float)> &bFunc, int lut_size, float vMin, float vMax)
{
    return JKQTPBuildColorPaletteLUT([&rFunc, &gFunc, &bFunc](float v) { return QColor::fromRgbF(jkqtp_bounded<float>(0.0, rFunc(v), 1.0), jkqtp_bounded<float>(0.0, gFunc(v), 1.0), jkqtp_bounded<float>(0.0, bFunc(v), 1.0)).rgba(); }, lut_size, vMin, vMax);
}

JKQTPImageTools::LUTType JKQTPBuildColorPaletteLUT(const std::function<float (float)> &rFunc, const std::function<float (float)> &gFunc, const std::function<float (float)> &aFunc, const std::function<float (float)> &bFunc, int lut_size, float vMin, float vMax)
{
    return JKQTPBuildColorPaletteLUT([&rFunc, &gFunc, &bFunc, &aFunc](float v) { return QColor::fromRgbF(jkqtp_bounded<float>(0.0, rFunc(v), 1.0), jkqtp_bounded<float>(0.0, gFunc(v), 1.0), jkqtp_bounded<float>(0.0, bFunc(v), 1.0), jkqtp_bounded<float>(0.0, aFunc(v), 1.0)).rgba(); }, lut_size, vMin, vMax);
}
