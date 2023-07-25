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


#ifndef JKQTPBASICIMAGETOOLS_H
#define JKQTPBASICIMAGETOOLS_H
#include <QIcon>
#include <QDebug>
#include <QImage>
#include <QStringList>
#include <cmath>
#include <cfloat>
#include <stdint.h>
#include <QColor>
#include <mutex>
#include <vector>
#include "jkqtcommon/jkqtcommon_imexport.h"
#include "jkqtcommon/jkqtpmathtools.h"

/** \brief possible datatypes of the data array, plotted by this class.
    \ingroup jkqtplotter_imagelots_tools
 */
enum class JKQTPMathImageDataType {
    FloatArray, /*!< Data is of type \c float */
    DoubleArray, /*!< Data is of type \c double */
    UInt8Array, /*!< Data is of type \c uint8_t */
    UInt16Array, /*!< Data is of type \c uint16_t */
    UInt32Array, /*!< Data is of type \c uint32_t */
    UInt64Array, /*!< Data is of type \c uint8_t */
    Int8Array, /*!< Data is of type \c int8_t */
    Int16Array, /*!< Data is of type \c int16_t */
    Int32Array, /*!< Data is of type \c int32_t */
    Int64Array /*!< Data is of type \c int64_t */
};

/*! \brief retrieve an R/G/B/Alpha (\a ch == 0/1/2/3) value from the \c QRgb value \a rgb
    \ingroup jkqtplotter_imagelots_tools
 */
inline int JKQTPGetColorChannel(QRgb rgb, int ch)
{
    switch(ch) {
        case 0: return qRed(rgb);
        case 1: return qGreen(rgb);
        case 2: return qBlue(rgb);
        case 3: return qAlpha(rgb);
    }
    return qGray(rgb);
}

/*! \brief set the R/G/B/Alpha (\a ch == 0/1/2/3) value in the \c QRgb value \a rgb to \a val (0..255!)
    \ingroup jkqtplotter_imagelots_tools
 */
inline void JKQTPSetColorChannel(QRgb& rgb, int ch, int val)
{
    switch(ch) {
        case 0: rgb= qRgba(val, qGreen(rgb), qBlue(rgb), qAlpha(rgb)); break;
        case 1: rgb= qRgba(qRed(rgb), val, qBlue(rgb), qAlpha(rgb)); break;
        case 2: rgb= qRgba(qRed(rgb), qGreen(rgb), val, qAlpha(rgb)); break;
        case 3: rgb= qRgba(qRed(rgb), qGreen(rgb), qBlue(rgb), val); break;
    }
}

/*! \brief available palettes for coloring an image
    \ingroup jkqtplotter_imagelots_tools
 */
enum JKQTPMathImageColorPalette {
    JKQTPMathImageGRAY=0, /*!< \image html palettes/palette_gray.png */
    JKQTPMathImageINVERTEDGRAY, /*!< \image html palettes/palette_invgray.png */
    JKQTPMathImageRED, /*!< \image html palettes/palette_red.png */
    JKQTPMathImageINVERTEDRED, /*!< \image html palettes/palette_invred.png */
    JKQTPMathImageGREEN, /*!< \image html palettes/palette_green.png */
    JKQTPMathImageINVERTEDGREEN, /*!< \image html palettes/palette_invgreen.png */
    JKQTPMathImageBLUE, /*!< \image html palettes/palette_blue.png */
    JKQTPMathImageINVERTEDBLUE, /*!< \image html palettes/palette_invblue.png */
    JKQTPMathImageCYAN, /*!< \image html palettes/palette_cyan.png */
    JKQTPMathImageINVERTED_CYAN, /*!< \image html palettes/palette_invcyan.png */
    JKQTPMathImageYELLOW, /*!< \image html palettes/palette_yellow.png */
    JKQTPMathImageINVERTED_YELLOW, /*!< \image html palettes/palette_invyellow.png */
    JKQTPMathImageMAGENTA, /*!< \image html palettes/palette_magenta.png */
    JKQTPMathImageINVERTED_MAGENTA, /*!< \image html palettes/palette_invmagenta.png */


    JKQTPMathImageMATLAB, /*!< \image html palettes/palette_Matlab.png */
    JKQTPMathImageINVERTED_MATLAB, /*!< \image html palettes/palette_invMatlab.png */
    JKQTPMathImageRYGB, /*!< \image html palettes/palette_RYGB.png */
    JKQTPMathImageINVERTED_RYGB, /*!< \image html palettes/palette_invRYGB.png */
    JKQTPMathImageHSV, /*!< \image html palettes/palette_HSV.png */
    JKQTPMathImageINVERTED_HSV, /*!< \image html palettes/palette_invHSV.png */
    JKQTPMathImageRAINBOW, /*!< \image html palettes/palette_rainbow.png */
    JKQTPMathImageINVERTED_RAINBOW, /*!< \image html palettes/palette_invrainbow.png */
    JKQTPMathImageHOT, /*!< \image html palettes/palette_AFMhot.png */
    JKQTPMathImageINVERTED_HOT, /*!< \image html palettes/palette_invAFMhot.png */
    JKQTPMathImageOCEAN, /*!< \image html palettes/palette_ocean.png */
    JKQTPMathImageINVERTED_OCEAN, /*!< \image html palettes/palette_invocean.png */
    JKQTPMathImageTRAFFICLIGHT, /*!< \image html palettes/palette_trafficlight.png */
    JKQTPMathImageINVERTED_TRAFFICLIGHT, /*!< \image html palettes/palette_invtrafficlight.png */

    JKQTPMathImageBone, /*!< \image html palettes/palette_bone.png */
    JKQTPMathImageCool, /*!< \image html palettes/palette_cool.png */
    JKQTPMathImageCopper, /*!< \image html palettes/palette_copper.png */
    JKQTPMathImageAutumn, /*!< \image html palettes/palette_autumn.png */
    JKQTPMathImageSeismic, /*!< \image html palettes/palette_seismic.png */
    JKQTPMathImageSeismic_STEP, /*!< \image html palettes/palette_seismic_step.png */
    JKQTPMathImageTerrain, /*!< \image html palettes/palette_terrain.png */
    JKQTPMathImageTerrain_STEP, /*!< \image html palettes/palette_terrain_step.png */

    JKQTPMathImageViridis, /*!< \image html palettes/palette_viridis.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Viridis, /*!< \image html palettes/palette_invviridis.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageViridis_STEP, /*!< \image html palettes/palette_viridis_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Viridis_STEP, /*!< \image html palettes/palette_invviridis_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */

    JKQTPMathImageMagma, /*!< \image html palettes/palette_magma.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Magma, /*!< \image html palettes/palette_invmagma.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageMagma_STEP, /*!< \image html palettes/palette_magma_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Magma_STEP, /*!< \image html palettes/palette_invmagma_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageInferno, /*!< \image html palettes/palette_inferno.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Inferno, /*!< \image html palettes/palette_invinferno.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageInferno_STEP, /*!< \image html palettes/palette_inferno_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Inferno_STEP, /*!< \image html palettes/palette_invinferno_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImagePlasma, /*!< \image html palettes/palette_plasma.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Plasma, /*!< \image html palettes/palette_invplasma.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImagePlasma_STEP, /*!< \image html palettes/palette_plasma_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageINVERTED_Plasma_STEP, /*!< \image html palettes/palette_invplasma_step.png
                                \see from https://github.com/BIDS/colormap/blob/master/colormaps.py */
    JKQTPMathImageIBMColorBlindSafe, /*!< \image html palettes/palette_IBMColorBlindSafe.png
                                \note color-blind freindly!
                                \see from https://lospec.com/palette-list/ibm-color-blind-safe */
    JKQTPMathImageIBMColorBlindSafe_STEP, /*!< \image html palettes/palette_IBMColorBlindSafe_step.png
                                \note color-blind freindly!
                                \see from https://lospec.com/palette-list/ibm-color-blind-safe */
    JKQTPMathImageOkabeIto_STEP, /*!< \image html palettes/palette_OkabeIto_step.png
                                \note color-blind freindly!
                                \see from https://yoshke.org/blog/colorblind-friendly-diagrams and M. Okabe and K. Ito, How to make figures and presentations that are friendly to color blind people, University of Tokyo, 2002. */
    JKQTPMathImageOkabeItoDarker_STEP, /*!< darker version of JKQTPMathImageOkabeItoDarker_STEP \image html palettes/palette_OkabeItoDarker_step.png
                                \note color-blind freindly!
                                \see from https://yoshke.org/blog/colorblind-friendly-diagrams and M. Okabe and K. Ito, How to make figures and presentations that are friendly to color blind people, University of Tokyo, 2002. */
    JKQTPMathImageOkabeItoLighter_STEP, /*!< lighter version of JKQTPMathImageOkabeItoDarker_STEP \image html palettes/palette_OkabeItoLighter_step.png
                                \note color-blind freindly!
                                \see from https://yoshke.org/blog/colorblind-friendly-diagrams and M. Okabe and K. Ito, How to make figures and presentations that are friendly to color blind people, University of Tokyo, 2002. */
    JKQTPMathImageDefault_STEP, /*!< \image html palettes/palette_jkqtplotterdefault_step.png
                                     This is the color cycle used to color graphs in JKQTPlotter's default style.
                                 */
    JKQTPMathImageCubeHelixClassic, /*!< \image html palettes/palette_CubeHelixClassic.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */
    JKQTPMathImageCubeHelixClassic_STEP, /*!< \image html palettes/palette_CubeHelixClassic_step.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */
    JKQTPMathImageCubeHelix1, /*!< \image html palettes/palette_CubeHelix1.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */
    JKQTPMathImageCubeHelix1_STEP, /*!< \image html palettes/palette_CubeHelix1_step.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */
    JKQTPMathImageCubeHelix2, /*!< \image html palettes/palette_CubeHelix2.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */
    JKQTPMathImageCubeHelix2_STEP, /*!< \image html palettes/palette_CubeHelix2_step.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */
    JKQTPMathImageCubeHelix3, /*!< \image html palettes/palette_CubeHelix3.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */
    JKQTPMathImageCubeHelix3_STEP, /*!< \image html palettes/palette_CubeHelix3_step.png
                                     This palette was created using Green's CubeHelix method with the parameters shown in the paper: start=0.5, rotation=-1.5, gamma=1.0, saturation=1.2.
                                     \see JKQTPCreateGreensCubeHelixLUT()
                                 */


    JKQTPMathImageBLUEMAGENTAYELLOW, /*!< \image html palettes/palette_BlMaYe.png */
    JKQTPMathImageINVERTED_BLUEMAGENTAYELLOW, /*!< \image html palettes/palette_YeMaBl.png */
    JKQTPMathImageYELLOWMAGENTABLUE=JKQTPMathImageINVERTED_BLUEMAGENTAYELLOW, /*!< \image html palettes/palette_YeMaBl.png */
    JKQTPMathImageBLUEYELLOW, /*!< \image html palettes/palette_BlYe.png */
    JKQTPMathImageINVERTED_BLUEYELLOW, /*!< \image html palettes/palette_YeBl.png */
    JKQTPMathImageYELLOWBLUE=JKQTPMathImageINVERTED_BLUEYELLOW, /*!< \image html palettes/palette_YeBl.png */

    JKQTPMathImageBLUEWHITERED, /*!< \image html palettes/palette_bluewhitered.png */
    JKQTPMathImageREDWHITEBLUE, /*!< \image html palettes/palette_redwhiteblue.png */

    JKQTPMathImageBLACKBLUEREDYELLOW, /*!< \image html palettes/palette_BBlRdYe.png */
    JKQTPMathImageGREENREDVIOLET, /*!< \image html palettes/palette_GnRdVi.png */
    JKQTPMathImageBLACKBLUEWHITEYELLOWWHITE, /*!< \image html palettes/palette_BWprint.png */
    JKQTPMathImageWHITEYELLOWWHITEBLUEBLACK, /*!< \image html palettes/palette_invBWprint.png */
    JKQTPMathImageBR_GR, /*!< \image html palettes/palette_BrBG.png */
    JKQTPMathImageBrownGreen=JKQTPMathImageBR_GR, /*!< \image html palettes/palette_BrBG.png */
    JKQTPMathImagePU_OR, /*!< \image html palettes/palette_PuOr.png */
    JKQTPMathImageOrangeWhitePurple=JKQTPMathImagePU_OR, /*!< \image html palettes/palette_PuOr.png */
    JKQTPMathImageGN_BU, /*!< \image html palettes/palette_greenblue.png */
    JKQTPMathImageGreenBlue=JKQTPMathImageGN_BU, /*!< \image html palettes/palette_greenblue.png */
    JKQTPMathImageBU_GN, /*!< \image html palettes/palette_bluegreen.png */
    JKQTPMathImageBlueGreen=JKQTPMathImageBU_GN, /*!< \image html palettes/palette_bluegreen.png */
    JKQTPMathImageYL_GN_BU, /*!< \image html palettes/palette_YeGnBu.png */
    JKQTPMathImageYellowGreenBlue=JKQTPMathImageYL_GN_BU, /*!< \image html palettes/palette_YeGnBu.png */

    JKQTPMathImageBR_GR_STEP, /*!< \image html palettes/palette_stepsBrBG.png */
    JKQTPMathImagePU_OR_STEP, /*!< \image html palettes/palette_stepsPuOr.png */
    JKQTPMathImageGN_BU_STEP, /*!< \image html palettes/palette_stepsGnBl.png */
    JKQTPMathImageBU_GN_STEP, /*!< \image html palettes/palette_stepsBlGn.png */
    JKQTPMathImageYL_GN_BU_STEP, /*!< \image html palettes/palette_stepsYeGnBu.png */


    JKQTPMathImageCYANWHITE, /*!< \image html palettes/palette_cyanwhite.png */
    JKQTPMathImageINVERTED_CYANWHITE, /*!< \image html palettes/palette_whitecyan.png */
    JKQTPMathImageWHITECYAN=JKQTPMathImageINVERTED_CYANWHITE, /*!< \image html palettes/palette_whitecyan.png */
    JKQTPMathImageYELLOWWHITE, /*!< \image html palettes/palette_yellowwhite.png */
    JKQTPMathImageINVERTED_YELLOWWHITE, /*!< \image html palettes/palette_whiteyellow.png */
    JKQTPMathImageWHITEYELLOW=JKQTPMathImageINVERTED_YELLOWWHITE, /*!< \image html palettes/palette_whiteyellow.png */
    JKQTPMathImageMAGENTAWHITE, /*!< \image html palettes/palette_magentawhite.png */
    JKQTPMathImageINVERTED_MAGENTAWHITE, /*!< \image html palettes/palette_whitemagenta.png */
    JKQTPMathImageWHITEMAGENTA=JKQTPMathImageINVERTED_MAGENTAWHITE, /*!< \image html palettes/palette_whitemagenta.png */
    JKQTPMathImageBlueGreenRed, /*!< \image html palettes/palette_bluegreenred.png */
    JKQTPMathImageRedGreenBlue, /*!< \image html palettes/palette_redgreenblue.png */
    JKQTPMathImageMagentaYellow, /*!< \image html palettes/palette_magentayellow.png */
    JKQTPMathImageYellowMagenta, /*!< \image html palettes/palette_yellowmagenta.png */
    JKQTPMathImageRedBlue, /*!< \image html palettes/palette_redblue.png */
    JKQTPMathImageBlueRed, /*!< \image html palettes/palette_bluered.png */

    JKQTPMathImagePREDEFINED_PALETTES_COUNT, /*!< \brief the number of predefined palettes */

    JKQTPMathImageUSER_PALETTE=65000, /*!< \brief special value for JKQTPImageTools::array2image(), which signals the usage of a provided user-defined palette */

    JKQTPMathImageALPHA=JKQTPMathImageUSER_PALETTE-2, /*!< \brief special palette with increasing alpha values */
    JKQTPMathImageINVERTED_ALPHA=JKQTPMathImageUSER_PALETTE-1, /*!< \brief special palette with decreasing alpha values */

    JKQTPMathImageFIRST_REGISTERED_USER_PALETTE=JKQTPMathImagePREDEFINED_PALETTES_COUNT,  /*!< \brief the ID of the first user-defined paletted, registered with JKQTPImageTools::registerPalette() or JKQTPImageTools::registerPalettesFromFile() */
    JKQTPMathImageLAST_POSSIBLE_REGISTERED_USER_PALETTE=JKQTPMathImageUSER_PALETTE-10,  /*!< \brief the ID of the first user-defined paletted, registered with JKQTPImageTools::registerPalette() or JKQTPImageTools::registerPalettesFromFile() */
};



/*! \brief modes available for image pixels that are above/below the pixel value range
    \ingroup jkqtplotter_imagelots_tools
 */
enum JKQTPMathImageColorRangeFailAction {
    JKQTPMathImageLastPaletteColor=0, /*!< set to last color in the palette */
    JKQTPMathImageGivenColor=1, /*!< set to the provided min/max color */
    JKQTPMathImageTransparent=2 /*!< set transparent */
};

/*! \brief tool structure that summarizes several static properties of JKQTPlotters palette system,
           also provides functions to work with palettes and register user-defined palettes.
    \ingroup jkqtplotter_imagelots_tools

    \see \ref JKQTPlotterImagePlot
*/
struct JKQTPImageTools {

        /*! \brief Datatype to store lookup-tables used to map data values (scales to 0..size-1) onto RGB-colors */
        typedef QVector<QRgb> LUTType;

        /*! \brief Width of the Palette-Icons, generated e.g. by JKQTPImageTools::GetPaletteIcon() */
        static JKQTCOMMON_LIB_EXPORT const int PALETTE_ICON_WIDTH;
        /*! \brief Height of the Palette-Icons, generated e.g. by JKQTPImageTools::GetPaletteIcon() */
        static JKQTCOMMON_LIB_EXPORT const int PALETTE_IMAGEICON_HEIGHT;

        /*! \brief size of the lookup tables used by JKQTFPimagePlot_array2image() */
        static JKQTCOMMON_LIB_EXPORT const int LUTSIZE;

        /*! \brief number of entries in an auto-generated steps-palette */
        static JKQTCOMMON_LIB_EXPORT const int NDEFAULTSTEPS;

        /*! \brief loads all palettes defined in the given palette files \a filename into global_jkqtpimagetools_userluts
                   and assigns a new LUT-ID >=JKQTPMathImageFIRST_REGISTERED_USER_PALETTE to each palette
                   All new IDs are returned as a list


            The file might either be a palette XML-file or a CSV-file:

            \b CSV-files need to have one of the following two formats (RED/GREEN/BLUE: 0..255):
            \verbatim
            scalar, red, green, blue
            scalar, red, green, blue
            ...
            \endverbatim
            or simply
            \verbatim
            red, green, blue
            red, green, blue
            ...
            \endverbatim
            The parser will also allow tabs and whitespaces as column separators.


            \b CML-files need to have one of the following two formats (RED/GREEN/BLUE/OPACITY: 0..1):
            \verbatim
                <ColorMap name="PALETTENAME" space="RGB">
                  <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
                  <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
                  ...
                </ColorMap>
            \endverbatim
            optionally several \code <ColorMap>...</ColorMap> \endcode definitions may be put below an arbitrarily named
            document node, e.g.:
            \verbatim
                <ColorMaps>
                    <ColorMap name="PALETTENAME" space="RGB">
                      <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
                      <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
                      ...
                    </ColorMap>
                    <ColorMap name="PALETTENAME" space="RGB">
                      <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
                      <Point x="scalar" r="RED" g="GREEN" b="BLUE"/>
                      ...
                    </ColorMap>
                    ...
                </ColorMaps>
            \endverbatim

            \note In both cases, the palette will be formed without interpolation, i.e. using JKQTPBuildColorPaletteLUT() .
                  If in addition \a interpolatePalette is set to \c true, the function JKQTPBuildColorPaletteLUTLinInterpolate()
                  is used instead.

            \see \ref JKQTPlotterImagePlot , JKQTPBuildColorPaletteLUT()
            */
        static JKQTCOMMON_LIB_EXPORT QVector<int> registerPalettesFromFile(const QString& filename, bool interpolatePalette=false);

        /*! \brief registers a new LUT defined by \a paletteLut and with the given \a name (computer-readable) for later use, optionally stores also the human-readable and localized name \a nameT
                   and assigns a new LUT-ID >=JKQTPMathImageFIRST_REGISTERED_USER_PALETTE to the palette and returns it

            \see \ref JKQTPlotterImagePlot, JKQTPBuildColorPaletteLUTLinInterpolate(), JKQTPBuildColorPaletteLUT(), JKQTPBuildColorPaletteLUTLinInterpolateSorted(), JKQTPBuildColorPaletteLUTSorted(), JKQTPBuildColorPaletteLUTLinInterpolate(), JKQTPBuildColorPaletteLUT()
            */
        static JKQTCOMMON_LIB_EXPORT int registerPalette(const QString& name, const LUTType &paletteLut, const QString &nameT=QString());








        /*! \brief convert a 2D image (as 1D array) into a QImage with given palette (see JKQTFPColorPalette)

            \param dbl_in pointer to a 1D array of template type \c T representing the image to plot. This array has to be of size \a width * \a height
            \param width width of the array in \a dbl
            \param height height of the array in \a dbl
            \param[out] img the QImage object to draw to (should be initialized as \c QImage::Format_ARGB32 )
            \param palette the color palette to use for the display
            \param minColor lower boundary of color range in \a dbl pixels, if \a minColor == \a maxColor then this function will extract the image min and image max.
            \param maxColor upper boundary of color range in \a dbl pixels, if \a minColor == \a maxColor then this function will extract the image min and image max.
            \param paletteMinFail specifies what shell happen, when a value in \a dbl is below \a minColor
            \param paletteMaxFail specifies what shell happen, when a value in \a dbl is above \a maxColor
            \param minFailColor color to use for pixels that are below \a minColor for some settings of \a paletteMinFail
            \param maxFailColor color to use for pixels that are below \a maxColor for some settings of \a paletteMaxFail
            \param nanColor color to use for pixels that are not-a-number
            \param infColor color to use for pixels that are infinity
            \param logScale create a log-scaled image
            \param logBase base for the logarithm used when \c logScale==true
            \param lutUser user define LUT, used if \a palette \c ==JKQTPMathImageUSER_PALETTE
                           \note There is a variant of this function that is called with a userLUT directly, instead of \a palette
        */
        template <class T>
        static inline void array2image(const T* dbl_in, int width, int height, QImage &img, JKQTPMathImageColorPalette palette, double minColor, double maxColor, JKQTPMathImageColorRangeFailAction paletteMinFail=JKQTPMathImageLastPaletteColor, JKQTPMathImageColorRangeFailAction paletteMaxFail=JKQTPMathImageLastPaletteColor, QColor minFailColor=QColor("black"), QColor maxFailColor=QColor("black"), QColor nanColor=QColor("black"), QColor infColor=QColor("black"), bool logScale=false, double logBase=10.0, const LUTType& lutUser=LUTType())
        {
            if (!dbl_in || width<=0 || height<=0)
                    return;

            double min = *dbl_in;
            double max = *dbl_in;
            if (jkqtp_approximatelyEqual(minColor, maxColor, JKQTP_DOUBLE_EPSILON)) {
                bool first=true;
                for (int i=1; i<width*height; ++i)
                {
                    T v=dbl_in[i];
                    if (!(std::isnan(static_cast<long double>(v)) || std::isinf(static_cast<long double>(v)))) {
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

            const T* dbl=dbl_in;
            QVector<T> dbl1;
            if (logScale) {
                double logB=log10(logBase);
                dbl1=QVector<T>(width*height, 0);
                for (int i=0; i<width*height; i++) {
                    dbl1[i]=log10(dbl_in[i])/logB;
                }
                dbl=dbl1.data();
                min=log10(min)/logB;
                max=log10(max)/logB;
            }
            double delta=max-min;


            // either use lutUser (if palette==JKQTPMathImageUSER_PALETTE) or else read the LUT from the internal storage
            const LUTType& lut_used=(palette==JKQTPMathImageUSER_PALETTE)?lutUser:getLUTforPalette(JKQTPImageTools::global_jkqtpimagetools_lutstore, palette);

            img = QImage(width, height, QImage::Format_ARGB32);
            if (jkqtp_approximatelyEqual(min, max, JKQTP_DOUBLE_EPSILON)) {
                    img.fill(lut_used[0]);
            } else {
                const int lutSize=lut_used.size();
                if (lutSize>0) {
                    // LUT found: collor the image accordingly
                    for (int j=0; j<height; ++j) {
                        QRgb* line=reinterpret_cast<QRgb*>(img.scanLine(height-1-j));
                        for (int i=0; i<width; ++i) {
                            double val=dbl[j*width+i];
                            if (std::isnan(val)) {
                                line[i]=nanColor.rgba();
                            } else if (std::isinf(val)) {
                                line[i]=infColor.rgba();
                            } else {
                                const int v = static_cast<int>((val-min)/delta*static_cast<double>(lutSize));
                                const int vv = qBound<int>(0, v, lutSize-1);
                                line[i]=lut_used[vv];
                                if ((v<0)&&(paletteMinFail==JKQTPMathImageGivenColor)) {
                                    line[i]=minFailColor.rgba();
                                } else if ((v>=lutSize)&&(paletteMaxFail==JKQTPMathImageGivenColor)) {
                                    line[i]=maxFailColor.rgba();
                                } else if ((v<0)&&(paletteMinFail==JKQTPMathImageTransparent)) {
                                    line[i]=QColor(Qt::transparent).rgba();
                                } else if ((v>=lutSize)&&(paletteMaxFail==JKQTPMathImageTransparent)) {
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

        }

        /*! \brief convert a 2D image (as 1D array) into a QImage with given palette (see JKQTFPColorPalette)

            \param dbl_in pointer to a 1D array of template type \c T representing the image to plot. This array has to be of size \a width * \a height
            \param width width of the array in \a dbl
            \param height height of the array in \a dbl
            \param[out] img the QImage object to draw to (should be initialized as \c QImage::Format_ARGB32 )
            \param lutUser user-defined lookup-table
            \param minColor lower boundary of color range in \a dbl pixels, if \a minColor == \a maxColor then this function will extract the image min and image max.
            \param maxColor upper boundary of color range in \a dbl pixels, if \a minColor == \a maxColor then this function will extract the image min and image max.
            \param paletteMinFail specifies what shell happen, when a value in \a dbl is below \a minColor
            \param paletteMaxFail specifies what shell happen, when a value in \a dbl is above \a maxColor
            \param minFailColor color to use for pixels that are below \a minColor for some settings of \a paletteMinFail
            \param maxFailColor color to use for pixels that are below \a maxColor for some settings of \a paletteMaxFail
            \param nanColor color to use for pixels that are not-a-number
            \param infColor color to use for pixels that are infinity
            \param logScale create a log-scaled image
            \param logBase base for the logarithm used when \c logScale==true
        */
        template <class T>
        static inline void array2image(const T* dbl_in, int width, int height, QImage &img, const LUTType& lutUser, double minColor, double maxColor, JKQTPMathImageColorRangeFailAction paletteMinFail=JKQTPMathImageLastPaletteColor, JKQTPMathImageColorRangeFailAction paletteMaxFail=JKQTPMathImageLastPaletteColor, QColor minFailColor=QColor("black"), QColor maxFailColor=QColor("black"), QColor nanColor=QColor("black"), QColor infColor=QColor("black"), bool logScale=false, double logBase=10.0)
        {
            array2image(dbl_in,  width,  height, img,  JKQTPMathImageUSER_PALETTE, minColor, maxColor,  paletteMinFail,  paletteMaxFail,  minFailColor,  maxFailColor,  nanColor,  infColor,  logScale,  logBase, lutUser);
        }



        /** \brief return a list of all globally available LUTs, human-readable/localized form  */
        static QStringList JKQTCOMMON_LIB_EXPORT getPredefinedPalettes();

        /** \brief return a list of all globally available LUTs, machine-readable form  */
        static QStringList JKQTCOMMON_LIB_EXPORT getPredefinedPalettesMachineReadable();

        /*! \brief convert the palette \a p to a machine-readable string
            \see JKQTPImageTools::String2JKQTPMathImageColorPalette()
         */
        static JKQTCOMMON_LIB_EXPORT QString JKQTPMathImageColorPalette2String(JKQTPMathImageColorPalette p);
        /*! \brief convert the palette \a p to a human-readable (localized) string
            \see JKQTPImageTools::String2JKQTPMathImageColorPalette()
         */
        static JKQTCOMMON_LIB_EXPORT QString JKQTPMathImageColorPalette2StringHumanReadable(JKQTPMathImageColorPalette p);

        /*! \brief convert the palette name \a p to JKQTPMathImageColorPalette (compatible with JKQTPImageTools::String2JKQTPMathImageColorPalette() )
            \see JKQTPImageTools::JKQTPMathImageColorPalette2String()
         */
        static JKQTCOMMON_LIB_EXPORT JKQTPMathImageColorPalette String2JKQTPMathImageColorPalette(const QString& p);


        /** \brief generates a QImage with width \a width and height 1 for the i-th color palette (\a i is based on the list returned by JKQTPImagePlot_getPredefinedPalettes() )  */
        static QImage JKQTCOMMON_LIB_EXPORT GetPaletteImage(int i, int width);
        /** \brief generates a QImage with width \a width and height \a height for the i-th color palette (\a i is based on the list returned by JKQTPImagePlot_getPredefinedPalettes() )  */
        static QImage JKQTCOMMON_LIB_EXPORT GetPaletteImage(int i, int width, int height);
        /** \brief generates a QImage with width \a width and height 1 for a specific JKQTPMathImageColorPalette  */
        static QImage JKQTCOMMON_LIB_EXPORT GetPaletteImage(JKQTPMathImageColorPalette palette, int width);
        /** \brief generates a QImage with width \a width and height \a height for a specific JKQTPMathImageColorPalette  */
        static QImage JKQTCOMMON_LIB_EXPORT GetPaletteImage(JKQTPMathImageColorPalette palette, int width, int height);
        /** \brief generates a QImage with width \a width and height 1 for a lookup-table \a lut  */
        static QImage JKQTCOMMON_LIB_EXPORT GetPaletteImage(const LUTType& lut, int width);

        /** \brief generates a QIcon for the i-th color palette (\a i is based on the list returned by JKQTPImagePlot_getPredefinedPalettes() )  */
        static QIcon JKQTCOMMON_LIB_EXPORT GetPaletteIcon(int i) ;


        /** \brief generates a QIcon for a specific JKQTPMathImageColorPalette  */
        static QIcon JKQTCOMMON_LIB_EXPORT GetPaletteIcon(JKQTPMathImageColorPalette palette) ;
        /*! \brief create a LUT for a given JKQTPMathImageColorPalette, stored it in \a lutstore and return it */
        static const LUTType& JKQTCOMMON_LIB_EXPORT getLUTforPalette(JKQTPMathImageColorPalette palette);
        /*! \brief return the list of QColors making up a JKQTPMathImageColorPalette, stored it in \a lutstore and return it */
        static QVector<QColor> JKQTCOMMON_LIB_EXPORT getColorsforPalette(JKQTPMathImageColorPalette palette);

    private:

        /*! \brief internal datatype, representing a lookup-table and its metadata inside global_jkqtpimagetools_lutstore
            \internal
            */
        struct LUTData {
            LUTData();
            LUTData(const LUTType& _lut, const QString& _name, const QString& _nameT);
            LUTData(const QString& _name, const QString& _nameT);
            /** \brief the LUT itself */
            LUTType lut;
            /** \brief name for the LUT (machine-readable) */
            QString name;
            /** \brief name for the LUT (localized, human-readable) */
            QString nameT;
        };

        /*! \brief internal global storage object for lookup-tables
            \internal
            */
        static JKQTCOMMON_LIB_EXPORT QMap<int, LUTData > global_jkqtpimagetools_lutstore;

        /*! \brief storage for the next ID to assign to a user-defined palette, registered with registerPalette() or registerPalettesFromFile()
            \internal

            \see registerPalette() registerPalettesFromFile()
            */
        static JKQTCOMMON_LIB_EXPORT int global_next_userpalette;
        /** \brief Mutex to protect global_jkqtpimagetools_lutstore and global_next_userpalette */
        static JKQTCOMMON_LIB_EXPORT std::mutex lutMutex;


        /*! \brief returns data of the default LUTs, used to initialize global_jkqtpimagetools_lutstore
            \internal
            */
        static JKQTCOMMON_LIB_EXPORT QMap<int, LUTData > getDefaultLUTs();


        /*! \brief create a LUT for a given JKQTPMathImageColorPalette, store it in \a lutstore and return it
            \internal
            */
        static JKQTCOMMON_LIB_EXPORT const LUTType& getLUTforPalette(const QMap<int, LUTData > &lutcache, JKQTPMathImageColorPalette palette);

};








/*! \brief modes available for RGB images
    \ingroup jkqtplotter_imagelots_tools


    \see Examples: \ref JKQTPlotterRGBImagePlot

 */
enum JKQTPRGBMathImageRGBMode {
    JKQTPRGBMathImageModeRGBMode=0, /*!< image channels are mapped to the R, G and B channel (red-green-blue) */
    JKQTPRGBMathImageModeHSVMode=1, /*!< image channels are mapped to the H, S and V channel (hue-saturation-value) */
    JKQTPRGBMathImageModeHSLMode=2, /*!< image channels are mapped to the H, S and L channel (bue-saturation-luminance) */
    JKQTPRGBMathImageModeCMYMode=3 /*!< image channels are mapped to the C, M and Y channel (subtractive color model!!!) */
};


/*! \brief returns a vector containing all elements of the given array as doubles
    \ingroup jkqtplotter_imagelots_tools

*/
template <class T>
inline QVector<double> JKQTPImagePlot_arrayToDVector(const T* input, int N) {
    if (!input || N<=0) return QVector<double>();
    QVector<double> out(N, 0.0);
    for (int i=0; i<N; i++) {
        out[i]=input[i];
    }
    return out;
}

/*! \brief returns a vector containing all elements of the given boolean array as doubles (true=1, false=0)
    \ingroup jkqtplotter_imagelots_tools

*/
inline QVector<double> JKQTPImagePlot_BarrayToDVector(const bool* input, int N) {
    if (!input || N<=0) return QVector<double>();
    QVector<double> out(N, 0.0);
    for (int i=0; i<N; i++) {
        if (input[i]) out[i]=1.0;
    }
    return out;
}

/*! \brief fin the minimum pixel value in the given image \a dbl with width \a width and height \a height
    \ingroup jkqtplotter_imagelots_tools

 */
template <class T>
inline double JKQTPImagePlot_getImageMin(const T* dbl, int width, int height)
{
    if (!dbl || width<=0 || height<=0)
            return 0;

    double min = 0;
    double max = 0;
    bool first=true;
    for (int i=1; i<width*height; ++i)
    {
        T v=dbl[i];
        if (!(std::isnan(static_cast<long double>(v)) || std::isinf(static_cast<long double>(v)))) {
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
    \ingroup jkqtplotter_imagelots_tools

 */
template <class T>
inline double JKQTPImagePlot_getImageMax(const T* dbl, int width, int height)
{
    if (!dbl || width<=0 || height<=0)
            return 0;

    double min = 0;
    double max = 0;
    bool first=true;
    for (int i=1; i<width*height; ++i)
    {
        T v=dbl[i];
        if (!(std::isnan(static_cast<long double>(v)) || std::isinf(static_cast<long double>(v)))) {
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
    \ingroup jkqtplotter_imagelots_tools

    \note  All calls (except channel>=3, i.e. alpha) set alpha to 255. Only the call
           with channel==3 (alpha) sets alpha to the desired value. Calls with channel==4 (saturation), channel==5 (value)
           leave alpha as it is.
*/
template <class T>
inline void JKQTPImagePlot_array2RGBimage(const T* dbl_in, int width, int height, QImage &img, int channel, double minColor, double maxColor, JKQTPRGBMathImageRGBMode rgbMode=JKQTPRGBMathImageModeRGBMode, bool logScale=false, double logBase=10.0)
{
    if (!dbl_in || width<=0 || height<=0)
        return;

    double min = *dbl_in;
    double max = *dbl_in;
    bool first=true;
    if (jkqtp_approximatelyEqual(minColor, maxColor, JKQTP_DOUBLE_EPSILON)) {
        for (int i=1; i<width*height; ++i)
        {
            T v=dbl_in[i];
            if (std::isfinite(static_cast<long double>(v))) {
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


    const T* dbl=dbl_in;
    QVector<T> dbllog;
    if (logScale) {
        double logB=log10(logBase);
        dbllog.resize(static_cast<size_t>(width)*static_cast<size_t>(height));
        //memcpy(dbl, dbl_in, width*height*sizeof(T));
        for (int i=0; i<width*height; i++) {
            dbllog[i]=log10(dbl_in[i])/logB;
        }
        min=log10(min)/logB;
        max=log10(max)/logB;
        dbl=dbllog.data();
    }
    double delta=max-min;

    if (jkqtp_approximatelyUnequal(minColor, maxColor, JKQTP_DOUBLE_EPSILON)) {
        if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
            //qDebug()<<"RGBMode";
            if (channel==0) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const QRgb l=line[i];
                        //if (j==5) qDebug()<<"r: "<<v<<qGreen(l)<<qBlue(l)<<qAlpha(255);
                        line[i]=qRgb(v,qGreen(l),qBlue(l));
                    }
                }
            } else if (channel==1) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const QRgb l=line[i];
                        //if (j==5) qDebug()<<"g: "<<qRed(l)<<v<<qBlue(l)<<qAlpha(255);
                        line[i]=qRgb(qRed(l),v,qBlue(l));
                    }
                }
            } else if (channel==2) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const QRgb l=line[i];
                        //if (j==5) qDebug()<<"b: "<<qRed(l)<<qGreen(l)<<v<<qAlpha(255);
                        line[i]=qRgb(qRed(l),qGreen(l),v);
                    }
                }
            } else if (channel==3) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const QRgb l=line[i];
                        //if (j==5) qDebug()<<"b: "<<qRed(l)<<qGreen(l)<<v<<qAlpha(255);
                        line[i]=qRgba(qRed(l),qGreen(l),qBlue(l), v);
                    }
                }
            }
        } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
            //qDebug()<<"RGBMode";
            if (channel==0) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const QRgb l=line[i];
                        //if (j==5) qDebug()<<"r: "<<v<<qGreen(l)<<qBlue(l)<<qAlpha(255);
                        line[i]=QColor::fromCmyk(v,QColor(l).magenta(),QColor(l).yellow(),QColor(l).black()).rgba();
                    }
                }
            } else if (channel==1) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const QRgb l=line[i];
                        //if (j==5) qDebug()<<"g: "<<qRed(l)<<v<<qBlue(l)<<qAlpha(255);
                        line[i]=QColor::fromCmyk(QColor(l).cyan(),v,QColor(l).yellow(),QColor(l).black()).rgba();
                    }
                }
            } else if (channel==2) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        const QRgb l=line[i];
                        //if (j==5) qDebug()<<"b: "<<qRed(l)<<qGreen(l)<<v<<qAlpha(255);
                        line[i]=QColor::fromCmyk(QColor(l).cyan(),QColor(l).magenta(),v,QColor(l).black()).rgba();
                    }
                }
            }
        } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
            if (channel==0) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
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
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
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
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
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
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = trunc(double(dbl[j*width+i]-min)*255.0/delta);
                        v = (v < 0) ? 0 : ( (v > 360) ? 360 : v);
                        QColor l=line[i];
                        l.setHsl(v, l.saturation(), l.lightness());
                        line[i]=l.rgb();
                    }
                }
            } else if (channel==1) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
                        int v = (dbl[j*width+i]-min)*255/delta;
                        v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                        QColor l=line[i];
                        l.setHsl(l.hue(), v, l.lightness());
                        line[i]=l.rgb();
                    }
                }
            } else if (channel==2) {
                for (int j=0; j<height; ++j) {
                    QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                    for (int i=0; i<width; ++i) {
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
           for (int j=0; j<height; ++j) {
               QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
               for (int i=0; i<width; ++i) {
                   int v = (dbl[j*width+i]-min)*255/delta;
                   v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                   const QRgb l=line[i];
                   line[i]=qRgba(qRed(l),qGreen(l),qBlue(l),v);
               }
           }
        } else if (channel==4) {
            for (int j=0; j<height; ++j) {
                QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                for (int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    QColor c=QColor::fromRgba(line[i]);
                    c.setHsv(c.hue(), v, c.value(), c.alpha());
                    line[i]=c.rgba();
                }
            }
        } else if (channel==5) {
            for (int j=0; j<height; ++j) {
                QRgb* line=reinterpret_cast<QRgb *>(img.scanLine(height-1-j));
                for (int i=0; i<width; ++i) {
                    int v = (dbl[j*width+i]-min)*255/delta;
                    v = (v < 0) ? 0 : ( (v > 255) ? 255 : v);
                    QColor c=QColor::fromRgba(line[i]);
                    c.setHsv(c.hue(), c.saturation(), v, c.alpha());
                    line[i]=c.rgba();
                }
            }
       }
     }
}






/*! \brief for building palettes from linear segments of single colors using JKQTPBuildColorPaletteLUTLinSegmentsSorted() and JKQTPBuildColorPaletteLUTLinSegments()
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \see JKQTPBuildColorPaletteLUTLinSegmentsSorted() and JKQTPBuildColorPaletteLUTLinSegments(), \ref JKQTPlotterImagePlotUserPalette

*/
struct JKQTPColorPaletteSingleColorLinSegment {
    JKQTPColorPaletteSingleColorLinSegment(double p, uint8_t y1, uint8_t y2);
    static JKQTPColorPaletteSingleColorLinSegment makeDbl_0_1(double p, double y1, double y2);
    JKQTPColorPaletteSingleColorLinSegment();
    /** \brief scalar position of the element on the value axis */
    double position;
    /** \brief color-channel-value that ends the prevoius segment (ignored for the first entry in a table) */
    uint8_t colval_endprevious;
    /** \brief color-channel-value that starts the next segment (ignored for the last entry in a table) */
    uint8_t colval_startnext;
};

/*! \brief build an interpolated palette with \a lut_size entries from the linear segments defined for the color channels R, G and B in \a itemsR, \a itemG and \a itemB respectively
           \b NOTE: The entries in \a itemsR, \a itemsG, \a itemsB are assumed to be sorted by the position entry.
    \ingroup jkqtplotter_imagelots_tools_LUTS




    This is used to build a table of linear segments as a \c QList<JKQTPColorPaletteSingleColorLinSegment> :
    \verbatim
      i    position    colval1            colval2
     ~~~  ~~~~~~~~~~  ~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~
      0       0.0           0 (IGNORED)        0
      1       0.5         100                100
      2       0.8         255                255
      3       1.0         255                255 (IGNORED)
    \endverbatim

    This will build a graph:
    \verbatim
       colval
         ^
         |
    250  -                                        #**********#
         |                                     ***
    200  -                                  ***
         |                               ***
    150  -                             **
         |                           **
    100  -                     *****#
         |                 ****
     50  -            *****
         |       *****
      0  - #*****
         |
        ---|----|----|----|----|----|----|----|----|----|----|--> position
          0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0
    \endverbatim

    You can also build graphs with a jump at a certain position
    \verbatim
      i    position    colval1            colval2
     ~~~  ~~~~~~~~~~  ~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~
      0       0.0           0 (IGNORED)        0
      1       0.5         100                  0
      2       1.0         255                255 (IGNORED)
    \endverbatim

    This results in:

    \verbatim
       colval
         ^
         |
    250  -                                                 **#
         |                                               **
    200  -                                             **
         |                                           **
    150  -                                        ***
         |                                      **
    100  -                          #         **
         |                    ******        **
     50  -              ******            **
         |        ******                **
      0  - #******                   #**
         |
        ---|----|----|----|----|----|----|----|----|----|----|--> position
          0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0
    \endverbatim

    \see JKQTPBuildColorPaletteLUTLinSegments(), \ref JKQTPlotterImagePlotUserPalette , https://matplotlib.org/api/_as_gen/matplotlib.colors.LinearSegmentedColormap.html#matplotlib.colors.LinearSegmentedColormap
*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTLinSegmentsSorted(const QList<JKQTPColorPaletteSingleColorLinSegment>& itemsR, const QList<JKQTPColorPaletteSingleColorLinSegment>& itemsG, const QList<JKQTPColorPaletteSingleColorLinSegment>& itemsB, int lut_size=JKQTPImageTools::LUTSIZE);


/*! \brief version of qRgb() that does not require to add the alpha part
    \ingroup jkqtplotter_imagelots_tools_LUTS
*/

inline QRgb jkqtp_qRgbOpaque(QRgb col) {
    return col | 0xFF000000;
}

/*! \brief for building palettes from list of colors and position values, using JKQTPBuildColorPaletteLUTLinInterpolateSorted(), ...
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \see JKQTPBuildColorPaletteLUTLinInterpolateSorted(), JKQTPBuildColorPaletteLUTSorted(), JKQTPBuildColorPaletteLUTColorsOnlySteps(), JKQTPBuildColorPaletteLUT(), JKQTPBuildColorPaletteLUTLinInterpolate()

*/
class JKQTCOMMON_LIB_EXPORT JKQTPPaletteList: public QList<QPair<double, QRgb> >
{
public:
    typedef QPair<double, QRgb> parameter_type;
    typedef QList<parameter_type> ListType;
    inline JKQTPPaletteList(): ListType() {};
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    inline JKQTPPaletteList(qsizetype size): ListType(size) {};
    inline JKQTPPaletteList(qsizetype size, parameter_type value): ListType(size, value) {};
#endif
    template <class T>
    inline JKQTPPaletteList(std::initializer_list<QPair<double, QRgb>> args): ListType(args) {};
    template <typename InputIterator, QtPrivate::IfIsInputIterator<InputIterator> = true>
    inline JKQTPPaletteList(InputIterator first, InputIterator last): ListType(first, last) {};
    inline JKQTPPaletteList(ListType &&other):ListType(std::forward<ListType>(other)) {};
    inline JKQTPPaletteList(const ListType &other):ListType(other) {};

    using ListType::push_back;
    inline void push_back(QRgb rgb) {
        if (size()==0) push_back(ListType::value_type(0.0, rgb));
        else if (size()==1) push_back(ListType::value_type(at(0).first+1.0, rgb));
        else {
            double mi=0,ma=0, delta=0;
            getMinMaxPosition(mi,ma,&delta);
            push_back(ListType::value_type(ma+delta, rgb));
        }
    }
    inline void push_back(double pos, QRgb rgb) {
        push_back(ListType::value_type(pos, rgb));
    }
    using ListType::operator<<;
    inline JKQTPPaletteList& operator<<(QRgb rgb) {
        push_back(rgb);
        return *this;
    }

    /** \brief returns the minimum value of the double-component */
    inline double getMinPosition() const {
        double mi=0;
        bool first=true;
        for (const auto& c: *this) {
            if (first) {
                mi=c.first;
                first=false;
            } else {
                mi=qMin<int>(mi, c.first);
            }
        }
        return mi;
    }
    /** \brief returns the maximum value of the double-component */
    inline double getMaxPosition() const {
        double ma=0;
        bool first=true;
        for (const auto& c: *this) {
            if (first) {
                ma=c.first;
                first=false;
            } else {
                ma=qMax<int>(ma, c.first);
            }
        }
        return ma;
    }
    /** \brief returns the minimum and maximum value of the double-component, as well as the avg. increment between two such values */
    inline void getMinMaxPosition(double& mi, double& ma, double* avgDelta=nullptr) const {
        mi=ma=0;
        bool first=true;
        for (const auto& c: *this) {
            if (first) {
                mi=ma=c.first;
                first=false;
            } else {
                ma=qMax<int>(ma, c.first);
                mi=qMin<int>(mi, c.first);
            }
        }
        if (avgDelta) {
            if (size()<=1) *avgDelta=1;
            else *avgDelta=(ma-mi)/static_cast<double>(size()-1);
        }
    }

};

/*! \brief build a linearly interpolated palette (as a look-up table) with \a lut_size entries by linearly interpolating between the nodes in \a items .
           \b NOTE: \a items is assumed to be sorted by the first component of the \c QPair<double,QRgb> entries!
    \ingroup jkqtplotter_imagelots_tools_LUTS

    The LUT is built following these rules:
      - the final LUT has \a lut_size entries
      - the first color in the lut is given by \c items.first().second
      - the last color in the lut is given by \c items.last().second
      - in between the colors are interpolated between the nodes in \a items and the color-nodes are
        distributed according to the first component of the \c QPair<double,QRgb> entries:<br>
        \image html JKQTPBuildColorPaletteLUTLinInterpolateSorted.png
    .

    \see JKQTPBuildColorPaletteLUTLinInterpolate(), \ref JKQTPlotterImagePlotUserPalette

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTLinInterpolateSorted(const JKQTPPaletteList& items, int lut_size=JKQTPImageTools::LUTSIZE);


/*! \brief build a palette (as a look-up table) with \a lut_size entries that step between the nodes provided in \a items.
           \b NOTE: \a items is assumed to be sorted by the first component of the \c QPair<double,QRgb> entries!
    \ingroup jkqtplotter_imagelots_tools_LUTS

    The LUT is built following these rules:
      - the final LUT has \a lut_size entries
      - the first color in the lut is given by \c items.first().second
      - the last color in the lut is given by \c items.last().second
      - in between the colors are stepped between the nodes in \a items and the color-nodes are
        distributed according to the first component of the \c QPair<double,QRgb> entries:<br>
        \image html JKQTPBuildColorPaletteLUTSorted.png
    .

    \see JKQTPBuildColorPaletteLUT(), \ref JKQTPlotterImagePlotUserPalette

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTSorted(const JKQTPPaletteList& items, int lut_size=JKQTPImageTools::LUTSIZE);

/*! \brief like JKQTPBuildColorPaletteLUTLinInterpolateSorted(), but sorts \a items before processing it!
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \copydetails JKQTPBuildColorPaletteLUTLinInterpolateSorted()
*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTLinInterpolate(JKQTPPaletteList items, int lut_size=JKQTPImageTools::LUTSIZE);


/*! \brief like JKQTPBuildColorPaletteLUTSorted(), but sorts \a items before processing it!
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \copydetails JKQTPBuildColorPaletteLUTSorted()
*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUT(JKQTPPaletteList items, int lut_size=JKQTPImageTools::LUTSIZE);

/*! \brief takes the QRgb-values in \a items and builds a stepped palette from them, ignoring the double-values. the palette will have items.size() entries.
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \note \a items wird zunächst nach dem \c double sortiert
*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTColorsOnlySteps(JKQTPPaletteList items);

/*! \brief like JKQTPBuildColorPaletteLUTLinSegmentsSorted(), but sorts \a itemsR, \a itemB, \a itemsG before processing them!
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \copydetails JKQTPBuildColorPaletteLUTLinSegmentsSorted()

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTLinSegments(QList<JKQTPColorPaletteSingleColorLinSegment> itemsR,  QList<JKQTPColorPaletteSingleColorLinSegment> itemsG,  QList<JKQTPColorPaletteSingleColorLinSegment> itemsB, int lut_size=JKQTPImageTools::LUTSIZE);


/*! \brief like JKQTPBuildColorPaletteLUTLinInterpolateSorted(), but accepts a \c QMap<double,QRgb> as parameter instead of \c QList<QPair<double,QRgb>>
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \copydetails JKQTPBuildColorPaletteLUTLinInterpolateSorted()

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTLinInterpolate(const QMap<double, QRgb>& items, int lut_size=JKQTPImageTools::LUTSIZE);


/*! \brief like JKQTPBuildColorPaletteLUTSorted(), but accepts a \c QMap<double,QRgb> as parameter instead of \c QList<QPair<double,QRgb>>
    \ingroup jkqtplotter_imagelots_tools_LUTS

     \copydetails JKQTPBuildColorPaletteLUTSorted()
*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUT(const QMap<double, QRgb>& items, int lut_size=JKQTPImageTools::LUTSIZE);


/*! \brief takes an existing JKQTPImageTools::LUTType and subsamples \a lut_size elements from it
    \ingroup jkqtplotter_imagelots_tools_LUTS

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPBuildColorPaletteLUTBySubsampling(const JKQTPImageTools::LUTType& items, int lut_size=JKQTPImageTools::NDEFAULTSTEPS);

/*! \brief modify each element of a given lut with a given functor that receives the color and an index
    \ingroup jkqtplotter_imagelots_tools_LUTS

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPModifyLUT(JKQTPImageTools::LUTType lut, std::function<QRgb(int, QRgb)> f);
/*! \brief modify each element of a given lut with a given functor that receives the color
    \ingroup jkqtplotter_imagelots_tools_LUTS

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPModifyLUT(JKQTPImageTools::LUTType lut, std::function<QRgb(QRgb)> f);
/*! \brief create color palettes/LUTs by using Green's CubeHelix method
    \ingroup jkqtplotter_imagelots_tools_LUTS

    \see see Green, D. A. 2011. A colour scheme for the display of astronomical intensity images. Bull. Astr. Soc. India 39, 289-295.
         and <a href="https://articles.adsabs.harvard.edu/pdf/2011BASI...39..289G">https://articles.adsabs.harvard.edu/pdf/2011BASI...39..289G</a>
         and <a href="https://jiffyclub.github.io/palettable/cubehelix/">https://jiffyclub.github.io/palettable/cubehelix/</a>

     \param start start-hue for the palette
     \param rotation rotation paremeter for the palette
     \param gamma optional gamma parameter
     \param lutsize number of entries in LUT
     \param lambda_min start value for the \f$ \lambda \f$ in Green's formula
     \param lambda_max end value for the \f$ \lambda \f$ in Green's formula
     \param saturation_min start value for the saturation in Green's formula, usually the saturation is a single value, then \a saturation_min == \a saturation_max!
     \param saturation_max end value for the saturation in Green's formula, usually the saturation is a single value, then \a saturation_min == \a saturation_max!
     \return the lut generated from the provided parameters with \a lutsize entries

*/
JKQTPImageTools::LUTType JKQTCOMMON_LIB_EXPORT JKQTPCreateGreensCubeHelixLUT(float start, float rotation, float gamma=1.0, int lutsize=JKQTPImageTools::LUTSIZE, float lambda_min=0.0, float lambda_max=1.0, float saturation_min=1.2, float saturation_max=1.2);


/** \brief describes how to modify a rendered image with a second data array \see ModifierModeToString(), StringToModifierMode(), JKQTPImageModifierModeComboBox
    \ingroup jkqtplotter_imagelots_tools*/
enum class JKQTPMathImageModifierMode {
    ModifyNone=0,  /*!< no modification \image html JKQTPMathImageBaseModifyNone.png */
    ModifyValue=1,  /*!< modify the VALUE-channel from the HSV color space \image html JKQTPMathImageBaseModifyValue.png */
    ModifySaturation=2,/*!< modify the SATURATION-channel from the HSV color space \image html JKQTPMathImageBaseModifySaturation.png */
    ModifyAlpha=3,/*!< modify the ALPHA/TRANSPARENCY-channel from the RGBA color space \image html JKQTPMathImageBaseModifyAlpha.png */
    ModifyTransparency=ModifyAlpha,/*!< \see ModifyAlpha */
    ModifyLuminance=4,/*!< modify the LUMINANCE-channel from the HSL color space \image html JKQTPMathImageBaseModifyLuminance.png */
    ModifyHue=5,/*!< modify the HUE-channel from the HSV color space \image html JKQTPMathImageBaseModifyHue.png */
};

/** \brief convert a ModifierMode to a string
    \ingroup jkqtplotter_imagelots_tools
  \see ModifierModeToString(), ModifierMode */
JKQTPMathImageModifierMode JKQTCOMMON_LIB_EXPORT StringToModifierMode(const QString& mode);

/** \brief convert a string to a ModifierMode
    \ingroup jkqtplotter_imagelots_tools
  \see StringToModifierMode(), ModifierMode */
QString JKQTCOMMON_LIB_EXPORT ModifierModeToString(const JKQTPMathImageModifierMode& mode);

/** \brief modify the given image \a img, using  modifier image \a dataModifier (of type \a datatypeModifier and size \a Nx * \a Ny), using values in the range \a internalModifierMin ... \a internalModifierMax )
    \ingroup jkqtplotter_imagelots_tools
 */
void JKQTCOMMON_LIB_EXPORT JKQTPModifyImage(QImage& img, JKQTPMathImageModifierMode modifierMode, const void* dataModifier, JKQTPMathImageDataType datatypeModifier, int Nx, int Ny, double internalModifierMin, double internalModifierMax);





#endif // JKQTPBASICIMAGETOOLS_H
