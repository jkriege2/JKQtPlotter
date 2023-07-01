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



#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnodetools.h"
#include "jkqtmathtext/nodes/jkqtmathtexttextnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextbracenode.h"
#include "jkqtmathtext/nodes/jkqtmathtextdecoratednode.h"
#include "jkqtmathtext/nodes/jkqtmathtextfracnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextinstructionnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextlistnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextmatrixnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsqrtnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsubsupernode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsymbolnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextwhitespacenode.h"
#include "jkqtmathtext/nodes/jkqtmathtextboxinstructionnode.h"
#include "jkqtmathtext/nodes/jkqtmathtexthorizontallistnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextmodifyenvironmentnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextnoopnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextverbatimnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextverticallistnode.h"
#include "jkqtmathtext/parsers/jkqtmathtextlatexparser.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <mutex>
#include <typeinfo>
#include <QPainterPath>


// --------------------------------------------------------------------------------------------------
// -- instanciation of the static JKQTMathText members
// --------------------------------------------------------------------------------------------------
const double JKQTMathText::ABS_MIN_LINEWIDTH=0.02;
QString JKQTMathText::init_serifFont="serif";
QString JKQTMathText::init_sansFont="sans";
QString JKQTMathText::init_symbolFont="symbol";
QString JKQTMathText::init_scriptFont="script";
QString JKQTMathText::init_typewriterFont="typewriter";
QString JKQTMathText::init_caligraphicFont="decorative";
QString JKQTMathText::init_blackboardFont="blackboard";
QString JKQTMathText::init_fracturFont="fraktur";
bool JKQTMathText::s_firstStart=true;
std::mutex JKQTMathText::s_mutex;




// --------------------------------------------------------------------------------------------------
// -- implementation of the JKQTMathText methods
// --------------------------------------------------------------------------------------------------
JKQTMathText::JKQTMathText(QObject* parent, bool useFontsForGUI):
    QObject(parent)
{
    //std::chrono::high_resolution_clock::time_point t0=std::chrono::high_resolution_clock::now();
    initJKQTMathTextResources();
    //qDebug()<<"init_resoucre: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();

    fontSize=10;
    fontSizeUnits=JKQTMathTextEnvironment::POINTS;
    fontColor=QColor("black");
    brace_factor=1.04;
    brace_y_shift_factor=0.7;//-1;

    subsuper_size_factor=0.65;
    subsuper_mode_selection_by_size_factor=1.01;
    sub_shift_factor=0.4;
    super_shift_factor=0.7;
    special_sub_shift_factor=0.4;
    special_super_shift_factor=0.4;

    frac_factor=1.0;
    frac_nested_factor=0.7;
    frac_shift_factor=0.4;

    underbrace_factor=0.75;
    underbrace_separation_xfactor=0.25;
    underbrace_bracesize_xfactor=0.5;
    underset_factor=0.7;
    decoration_height_factor=0.3;
    decoration_width_reduction_Xfactor=0.2;
    decoration_separation_factor=0.1;

    operatorsubsuper_size_factor=0.65;
    operatorsubsuper_distance_factor=0.35;
    operatorsubsuper_extraspace_factor=0.5;
    intsubsuper_xcorrection_factor=0.25;
    intsubbesides_xcorrection_xfactor=0.33;
    mathoperator_width_factor=1.5;
    bigmathoperator_font_factor=1.8;

    sqrt_width_Xfactor=0.8;
    sqrt_height_factor=1.2;
    sqrt_smallfont_factor=0.57;

    matrix_linewidth_thin_factor=0.4;
    matrix_linewidth_heavy_factor=1.5;
    matrix_line_separation_factor=2.0;
    matrix_xSeparation_factor=0.75;
    matrix_ySeparation_factor=0.6;
    matrix_xPadding_factor=0.75;
    matrix_yPadding_factor=0.75;

    blackboradFontMode=MTBBDMdefault;



    std::lock_guard<std::mutex> lock(s_mutex);
    if (s_firstStart) {
        //t0=std::chrono::high_resolution_clock::now();

        s_firstStart=false;
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
        QFontDatabase fdb;
        const auto fonts=fdb.families();
#else
        const auto fonts=QFontDatabase::families();
#endif


        auto checkForFonts=[&fonts](QString& targetfont, const QStringList& fontoptions) {
            for (auto& f: fontoptions) {
                if (fonts.contains(f)) {
                    targetfont=f;
                    break;
                }
            }
        };

        checkForFonts(init_serifFont, QStringList {"Times New Roman", "Times", "FreeSerif", "DejaVu Serif"});
        //qDebug()<<"check 1st font: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms";

        checkForFonts(init_sansFont, QStringList {"Segoe UI", "Arial Unicode MS", "Arial Unicode", "Lucida Sans Unicode", "Arial", "Helvetica", "FreeSans", "DejaVu Sans", "Lucida Sans"});
        checkForFonts(init_symbolFont, QStringList {"Segoe UI Symbol", "XITS Math", "Cambria Math", "Fira Math", "Lucida Bright Math", "SymbolStandard", "Symbol"});
        checkForFonts(init_typewriterFont, QStringList {"Courier New", "Courier", "Courier Std", "FreeMono", "CMU Typewriter Text", "UM Typewriter"});
        checkForFonts(init_blackboardFont, QStringList {"Double Stroke", "CloisterOpenFace BT", "GoudyHandtooled BT", "Castellar", "MathJax_AMS", "Castellar Standard", "MathJax_AMS Standard", "Colonna MT"});
        checkForFonts(init_caligraphicFont, QStringList {"Lucida Calligraphy", "Cookie", "Segoe Print", "Comic Sans", "Comic Sans MS", "Gabriola", "Gabriola Standard", "Lucida Handwriting Kursiv", "Lucida Handwriting", "Pristina", "Pristina Standard", "MathJax_Caligraphics"});
        checkForFonts(init_scriptFont, QStringList {"Lucida Handwriting", "Dancing Script", "Amazone BT", "ScriptS", "ScriptC", "ScriptC Standard", "Script", "Brush Script MT", "Brush Script MT Kursiv", "MathJax_Script"});
        checkForFonts(init_fracturFont, QStringList {"Old English Text MT", "Old English Text MT Standard", "UnifrakturMaguntia Standard", "UnifrakturMaguntia", "MathJax_Fraktur", "UnifrakturCook Fett"});
        //qDebug()<<"check all font: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms";
    }

    //t0=std::chrono::high_resolution_clock::now();
    if (init_serifFont!="serif") addReplacementFont("serif", init_serifFont);
    if (init_sansFont!="sans") addReplacementFont("sans", init_sansFont);
    if (init_symbolFont!="symbol") addReplacementFont("symbol", init_symbolFont);
    if (init_scriptFont!="script") addReplacementFont("script", init_scriptFont);
    if (init_typewriterFont!="typewriter") addReplacementFont("typewriter", init_typewriterFont);
    if (init_caligraphicFont!="decorative") addReplacementFont("decorative", init_caligraphicFont);
    if (init_fracturFont!="fraktur") addReplacementFont("fraktur", init_fracturFont);
    if (init_blackboardFont!="blackboard") {
        addReplacementFont("blackboard", init_blackboardFont);
    }
    //qDebug()<<"add replacement fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    setFontSans(init_sansFont, estimateJKQTMathTextFontEncoding(init_sansFont));
    setFontMathSans(init_sansFont, estimateJKQTMathTextFontEncoding(init_sansFont));
    setFontTypewriter(init_typewriterFont, estimateJKQTMathTextFontEncoding(init_typewriterFont));
    setFontRoman(init_serifFont, estimateJKQTMathTextFontEncoding(init_serifFont));
    setFontMathRoman(init_serifFont, estimateJKQTMathTextFontEncoding(init_serifFont));
    setFontCaligraphic(init_caligraphicFont, estimateJKQTMathTextFontEncoding(init_caligraphicFont));
    if (init_blackboardFont!="blackboard") {
        setFontBlackboard(init_blackboardFont, estimateJKQTMathTextFontEncoding(init_blackboardFont));
        setFontBlackboradMode(MTBBDMunicodeCharactersOrFontDirectly);
    } else {
        setFontBlackboard(init_sansFont, estimateJKQTMathTextFontEncoding(init_sansFont));
        setFontBlackboradMode(MTBBDMunicodeCharactersOrSimulate);
    }
    setFontScript(init_scriptFont, estimateJKQTMathTextFontEncoding(init_scriptFont));
    setFontFraktur(init_fracturFont, estimateJKQTMathTextFontEncoding(init_fracturFont));
    setFallbackFontSymbols(init_symbolFont, estimateJKQTMathTextFontEncoding(init_symbolFont));
    //qDebug()<<"set fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    useXITS();
    //qDebug()<<"useXITS: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    if (useFontsForGUI) useGuiFonts();
    parsedNode=nullptr;

}

JKQTMathText::~JKQTMathText() {
    if (parsedNode!=nullptr) delete parsedNode;
    parsedNode=nullptr;
}

void JKQTMathText::loadSettings(const QSettings& settings, const QString& group){
    fontSize=settings.value(group+"font_size", fontSize).toDouble();
    fontSizeUnits=JKQTMathTextEnvironment::String2FontSizeUnit(settings.value(group+"font_size_units", JKQTMathTextEnvironment::FontSizeUnit2String(fontSizeUnits)).toString());
    fontColor=jkqtp_String2QColor(settings.value(group+"font_color", jkqtp_QColor2String(fontColor)).toString());
    brace_factor=settings.value(group+"brace_factor", brace_factor).toDouble();
    subsuper_size_factor=settings.value(group+"subsuper_size_factor", subsuper_size_factor).toDouble();
    subsuper_mode_selection_by_size_factor=settings.value(group+"subsuper_mode_selection_by_size_factor", subsuper_mode_selection_by_size_factor).toDouble();
    super_shift_factor=settings.value(group+"super_shift_factor", super_shift_factor).toDouble();
    sub_shift_factor=settings.value(group+"sub_shift_factor", sub_shift_factor).toDouble();
    special_super_shift_factor=settings.value(group+"special_super_shift_factor", special_super_shift_factor).toDouble();
    special_sub_shift_factor=settings.value(group+"special_sub_shift_factor", special_sub_shift_factor).toDouble();
    frac_factor=settings.value(group+"frac_factor", frac_factor).toDouble();
    frac_shift_factor=settings.value(group+"frac_shift_factor", frac_shift_factor).toDouble();
    underbrace_factor=settings.value(group+"underbrace_factor", underbrace_factor).toDouble();
    underbrace_bracesize_xfactor=settings.value(group+"underbrace_bracesize_xfactor", underbrace_bracesize_xfactor).toDouble();
    underbrace_separation_xfactor=settings.value(group+"underbrace_separation_xfactor", underbrace_separation_xfactor).toDouble();
    underset_factor=settings.value(group+"underset_factor", underset_factor).toDouble();
    brace_y_shift_factor=settings.value(group+"brace_y_shift_factor", brace_y_shift_factor).toDouble();
    decoration_height_factor=settings.value(group+"decoration_height_factor", decoration_height_factor).toDouble();
    decoration_separation_factor=settings.value(group+"decoration_separation_factor", decoration_separation_factor).toDouble();
    decoration_width_reduction_Xfactor=settings.value(group+"decoration_width_reduction_xfactor", decoration_width_reduction_Xfactor).toDouble();
    operatorsubsuper_size_factor=settings.value(group+"operatorsubsuper_size_factor", operatorsubsuper_size_factor).toDouble();
    operatorsubsuper_distance_factor=settings.value(group+"operatorsubsuper_distance_factor", operatorsubsuper_distance_factor).toDouble();
    operatorsubsuper_extraspace_factor=settings.value(group+"operatorsubsuper_extraspace_factor", operatorsubsuper_extraspace_factor).toDouble();
    mathoperator_width_factor=settings.value(group+"mathoperator_width_factor", mathoperator_width_factor).toDouble();
    intsubsuper_xcorrection_factor=settings.value(group+"intsubsuper_xcorrection_factor", intsubsuper_xcorrection_factor).toDouble();
    intsubbesides_xcorrection_xfactor=settings.value(group+"intsubbesides_xcorrection_xfactor", intsubbesides_xcorrection_xfactor).toDouble();
    sqrt_width_Xfactor=settings.value(group+"sqrt_width_Xfactor", sqrt_width_Xfactor).toDouble();
    sqrt_height_factor=settings.value(group+"sqrt_height_factor", sqrt_height_factor).toDouble();
    sqrt_smallfont_factor=settings.value(group+"sqrt_smallfont_factor", sqrt_smallfont_factor).toDouble();
    bigmathoperator_font_factor=settings.value(group+"bigmathoperator_font_factor", bigmathoperator_font_factor).toDouble();
    frac_nested_factor=settings.value(group+"frac_nested_factor", frac_nested_factor).toDouble();

    matrix_linewidth_thin_factor=settings.value(group+"matrix_linewidth_thin_factor", matrix_linewidth_thin_factor).toDouble();
    matrix_linewidth_heavy_factor=settings.value(group+"matrix_linewidth_heavy_factor", matrix_linewidth_heavy_factor).toDouble();
    matrix_line_separation_factor=settings.value(group+"matrix_line_separation_factor", matrix_line_separation_factor).toDouble();
    matrix_xSeparation_factor=settings.value(group+"matrix_xSeparation_factor", matrix_xSeparation_factor).toDouble();
    matrix_ySeparation_factor=settings.value(group+"matrix_ySeparation_factor", matrix_ySeparation_factor).toDouble();
    matrix_xPadding_factor=settings.value(group+"matrix_xPadding_factor", matrix_xPadding_factor).toDouble();
    matrix_yPadding_factor=settings.value(group+"matrix_yPadding_factor", matrix_yPadding_factor).toDouble();

    if (settings.value(group+"use_stix_fonts", false).toBool()) useSTIX();
    if (settings.value(group+"use_xits_fonts", false).toBool()) useXITS();
    if (settings.value(group+"use_asana_fonts", false).toBool()) useASANA();
}


void JKQTMathText::saveSettings(QSettings& settings, const QString& group) const{
    settings.setValue(group+"font_size", fontSize);
    settings.setValue(group+"font_size_units", JKQTMathTextEnvironment::FontSizeUnit2String(fontSizeUnits));
    settings.setValue(group+"font_color", jkqtp_QColor2String(fontColor));
    settings.setValue(group+ "brace_factor", brace_factor);
    settings.setValue(group+ "subsuper_size_factor", subsuper_size_factor);
    settings.setValue(group+ "subsuper_mode_selection_by_size_factor", subsuper_mode_selection_by_size_factor);
    settings.setValue(group+ "sub_shift_factor", sub_shift_factor);
    settings.setValue(group+ "super_shift_factor", super_shift_factor);
    settings.setValue(group+ "special_sub_shift_factor", special_sub_shift_factor);
    settings.setValue(group+ "special_super_shift_factor", special_super_shift_factor);
    settings.setValue(group+ "frac_factor", frac_factor);
    settings.setValue(group+ "frac_shift_factor", frac_shift_factor);
    settings.setValue(group+ "underbrace_factor", underbrace_factor);
    settings.setValue(group+ "underbrace_bracesize_xfactor", underbrace_bracesize_xfactor);
    settings.setValue(group+ "underbrace_separation_xfactor", underbrace_separation_xfactor);
    settings.setValue(group+ "underset_factor", underset_factor);
    settings.setValue(group+ "operatorsubsuper_size_factor", operatorsubsuper_size_factor);
    settings.setValue(group+ "operatorsubsuper_distance_factor", operatorsubsuper_distance_factor);
    settings.setValue(group+ "operatorsubsuper_extraspace_factor", operatorsubsuper_extraspace_factor);
    settings.setValue(group+ "mathoperator_width_factor", mathoperator_width_factor);
    settings.setValue(group+ "intsubsuper_xcorrection_factor", intsubsuper_xcorrection_factor);
    settings.setValue(group+ "intsubbesides_xcorrection_xfactor", intsubbesides_xcorrection_xfactor);
    settings.setValue(group+ "brace_y_shift_factor", brace_y_shift_factor);
    settings.setValue(group+ "decoration_height_factor", decoration_height_factor);
    settings.setValue(group+ "decoration_separation_factor", decoration_separation_factor);
    settings.setValue(group+ "decoration_width_reduction_xfactor", decoration_width_reduction_Xfactor);
    settings.setValue(group+ "sqrt_width_Xfactor", sqrt_width_Xfactor);
    settings.setValue(group+ "sqrt_height_factor", sqrt_height_factor);
    settings.setValue(group+ "sqrt_smallfont_factor", sqrt_smallfont_factor);
    settings.setValue(group+ "bigmathoperator_font_factor", bigmathoperator_font_factor);
    settings.setValue(group+ "frac_nested_factor", frac_nested_factor);
    settings.setValue(group+ "matrix_linewidth_thin_factor", matrix_linewidth_thin_factor);
    settings.setValue(group+ "matrix_linewidth_heavy_factor", matrix_linewidth_heavy_factor);
    settings.setValue(group+ "matrix_line_separation_factor", matrix_line_separation_factor);
    settings.setValue(group+ "matrix_xSeparation_factor", matrix_xSeparation_factor);
    settings.setValue(group+ "matrix_ySeparation_factor", matrix_ySeparation_factor);
    settings.setValue(group+ "matrix_xPadding_factor", matrix_xPadding_factor);
    settings.setValue(group+ "matrix_yPadding_factor", matrix_yPadding_factor);
}

bool JKQTMathText::parse(const QString &markup, DefaultParserTypes markupType, ParseOptions options)
{
    if (markupType==LatexParser) return parse<JKQTMathTextLatexParser>(markup, options);
    return false;
}

bool JKQTMathText::useSTIX(bool mathModeOnly, bool useAsFallbackSymbol) {

    const JKQTMathTextFontSpecifier stixs=JKQTMathTextFontSpecifier::getSTIXFamilies();
    bool res=false;

    if (!mathModeOnly && !stixs.fontName().isEmpty()) {
        setFontRoman(stixs.fontName(), MTFEUnicode);
        res=true;
    }
    if (!stixs.mathFontName().isEmpty()) {
        setFontMathRoman(stixs.mathFontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(stixs.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    } else if (!stixs.fontName().isEmpty()) {
        setFontMathRoman(stixs.fontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(stixs.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    }

    return res;
}

bool JKQTMathText::useXITS(bool mathModeOnly, bool useAsFallbackSymbol)
{
    const JKQTMathTextFontSpecifier xits=JKQTMathTextFontSpecifier::getXITSFamilies();
    bool res=false;

    if (!mathModeOnly && !xits.fontName().isEmpty()) {
        setFontRoman(xits.fontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(xits.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    }
    if (!xits.mathFontName().isEmpty()) {
        setFontMathRoman(xits.mathFontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(xits.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    }

    return res;
}

bool JKQTMathText::useFiraMath(bool mathModeOnly, bool useAsFallbackSymbol)
{
    const JKQTMathTextFontSpecifier fira=JKQTMathTextFontSpecifier::getFIRAFamilies();
    bool res=false;
    qDebug()<<"useFiraMath("<<mathModeOnly<<", "<<useAsFallbackSymbol<<")";

    if (!mathModeOnly && !fira.fontName().isEmpty()) {
        setFontRoman(fira.fontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(fira.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    }
    if (!fira.mathFontName().isEmpty()) {
        setFontMathRoman(fira.mathFontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(fira.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    }

    //std::cout<<"useFiraMath("<<mathModeOnly<<", "<<useAsFallbackSymbol<<") roman="<<getFontRoman().toStdString()<<", mathRoman="<<getFontMathRoman().toStdString()<<", symbol="<<getFallbackFontSymbols().toStdString();
    return res;
}

bool JKQTMathText::useASANA(bool mathModeOnly, bool useAsFallbackSymbol)
{

    const JKQTMathTextFontSpecifier asana=JKQTMathTextFontSpecifier::getASANAFamilies();
    bool res=false;

    if (!mathModeOnly && !asana.fontName().isEmpty()) {
        setFontRoman(asana.fontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(asana.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    }
    if (!asana.mathFontName().isEmpty()) {
        setFontMathRoman(asana.mathFontName(), MTFEUnicode);
        if (useAsFallbackSymbol) setFallbackFontSymbols(asana.fallbackSymbolsFontName(), MTFEUnicode);
        res=true;
    }

    return res;
}

bool JKQTMathText::useGuiFonts()
{
    const JKQTMathTextFontSpecifier rm=JKQTMathTextFontSpecifier::getAppFontFamilies();
    const JKQTMathTextFontSpecifier sf=JKQTMathTextFontSpecifier::getAppFontSFFamilies();

    if (rm.hasFontName()) setFontRoman(rm.fontName(), MTFEUnicode);
    if (rm.hasMathFontName()) setFontMathRoman(rm.mathFontName(), MTFEUnicode);
    if (rm.hasFallbackSymbolFontName()) setFallbackFontSymbols(rm.fallbackSymbolsFontName(), MTFEUnicode);
    if (sf.hasFontName()) setFontSans(sf.fontName(), MTFEUnicode);
    if (sf.hasMathFontName()) setFontMathSans(sf.mathFontName(), MTFEUnicode);
    return true;
}

void JKQTMathText::useAnyUnicode(QString timesFont, const QString &sansFont, JKQTMathTextFontEncoding encodingTimes, JKQTMathTextFontEncoding encodingSans)
{
    if (!timesFont.isEmpty()) {
        setFontRoman(timesFont, encodingTimes);
        setFontMathRoman(timesFont, encodingTimes);
    }
    if (!sansFont.isEmpty()) {
        setFontSans(sansFont, encodingSans);
        setFontMathSans(sansFont, encodingSans);
    }
}

void JKQTMathText::useAnyUnicodeForMathOnly(QString timesFont, const QString &sansFont, JKQTMathTextFontEncoding encodingTimes, JKQTMathTextFontEncoding encodingSans)
{
    if (!timesFont.isEmpty()) { setFontMathRoman(timesFont, encodingTimes); }
    if (!sansFont.isEmpty()) { setFontMathSans(sansFont, encodingSans); }
}

void JKQTMathText::useAnyUnicodeForTextOnly(QString timesFont, const QString &sansFont, JKQTMathTextFontEncoding encodingTimes, JKQTMathTextFontEncoding encodingSans)
{
    if (!timesFont.isEmpty()) { setFontRoman(timesFont, encodingTimes); }
    if (!sansFont.isEmpty()) { setFontSans(sansFont, encodingSans); }
}


QString JKQTMathText::toHtml(bool *ok, double fontPointSize) {
    QString s;
    bool okk=false;
    if (getNodeTree()!=nullptr) {
        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontPointSize;
        ev.fontSizeUnit=JKQTMathTextEnvironment::POINTS;

        JKQTMathTextEnvironment defaultev;
        defaultev.fontSize=fontPointSize;
        defaultev.fontSizeUnit=JKQTMathTextEnvironment::POINTS;

        okk=getNodeTree()->toHtml(s, ev, defaultev);
    }
    if (ok) *ok=okk;
    return s;
}


void JKQTMathText::setFontColor(const QColor &__value)
{
    this->fontColor = __value;
}

QColor JKQTMathText::getFontColor() const
{
    return this->fontColor;
}

void JKQTMathText::setFontSize(double __value)
{
    setFontPointSize(__value);
}

void JKQTMathText::setFontPointSize(double __value)
{
    fontSize = __value;
    fontSizeUnits=JKQTMathTextEnvironment::POINTS;
}

void JKQTMathText::setFontSizePixels(double __value)
{
    fontSize = __value;
    fontSizeUnits=JKQTMathTextEnvironment::PIXELS;
}

double JKQTMathText::getFontSize() const
{
    return getFontPointSize();
}

double JKQTMathText::getFontPointSize() const
{
    if (fontSizeUnits==JKQTMathTextEnvironment::POINTS) return fontSize;
    else return -1;
}

double JKQTMathText::getFontSizePixels() const
{
    if (fontSizeUnits==JKQTMathTextEnvironment::PIXELS) return fontSize;
    else return -1;
}

void JKQTMathText::addReplacementFont(const QString &nonUseFont, const QString &useFont, JKQTMathTextFontEncoding useFontEncoding) {
    fontReplacements.insert(nonUseFont, useFont);
    fontEncodingReplacements.insert(nonUseFont, useFontEncoding);
}

void JKQTMathText::addReplacementFont(const QString &nonUseFont, const QString &useFont) {
    fontReplacements.insert(nonUseFont, useFont);
    auto it=fontEncodingReplacements.find(nonUseFont);
    if (it!=fontEncodingReplacements.end()) fontEncodingReplacements.erase(it);
}

QPair<QString,JKQTMathTextFontEncoding> JKQTMathText::getReplacementFont(const QString &nonUseFont, const QString &defaultFont, JKQTMathTextFontEncoding defaultFontEncoding) const {
    QPair<QString,JKQTMathTextFontEncoding> res(defaultFont, defaultFontEncoding);
    for (auto it=fontReplacements.begin(); it!=fontReplacements.end(); ++it) {
        if (it.key().toLower()==nonUseFont.toLower()) {
            res.first=it.value();
            res.second=fontEncodingReplacements.value(res.first, res.second);
            return res;
        }
    }
    return res;
}

QPair<QString, JKQTMathTextFontEncoding> JKQTMathText::getFontData(JKQTMathTextEnvironmentFont font, bool in_math_environment) const
{
    if (in_math_environment) {
        if (font==MTEroman) font=MTEmathRoman;
        if (font==MTEsans) font=MTEmathSans;
    }
    const auto fd=fontDefinitions.value(font);
    return QPair<QString, JKQTMathTextFontEncoding>(fd.fontName, fd.fontEncoding);
}


void JKQTMathText::setFontRomanOrSpecial(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    setFontRomanOrSpecial(JKQTMathTextFontSpecifier::fromFontSpec(__value), encoding);
}

void JKQTMathText::setFontRomanOrSpecial(const JKQTMathTextFontSpecifier &fontName, JKQTMathTextFontEncoding encoding)
{
    if (!fontName.hasMathFontName()) {
        setFontRoman(fontName.fontName(), encoding);
        setFontMathRoman(fontName.fontName(), encoding);
    } else {
        setFontRoman(fontName.fontName(), encoding);
        setFontMathRoman(fontName.mathFontName(), encoding);
    }
    if (fontName.hasFallbackSymbolFontName()) setFallbackFontSymbols(fontName.fallbackSymbolsFontName(), encoding);
}

void JKQTMathText::setFontSpecial(const QString &fontSpec)
{
    if (fontSpec.trimmed().size()==0) return;
    QString beforePlus=fontSpec;
    QString afterPlus="";
    const int Iplus=fontSpec.lastIndexOf('+');
    if (Iplus>=0 && Iplus<fontSpec.size()) {
        beforePlus=fontSpec.left(Iplus);
        afterPlus=fontSpec.mid(Iplus+1);
    }
    if (beforePlus.toUpper()=="GUI") useGuiFonts();
    else {
        const QStringList splitSlash=beforePlus.split('/');
        if (splitSlash.size()==1) {
            setFontRoman(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[0]), MTFEUnicode);
            setFontMathRoman(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[0]), MTFEUnicode);
            setFontSans(init_sansFont, estimateJKQTMathTextFontEncoding(init_sansFont));
            setFontMathSans(init_sansFont, estimateJKQTMathTextFontEncoding(init_sansFont));
        } else if (splitSlash.size()==2) {
            setFontRoman(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[0]), MTFEUnicode);
            setFontMathRoman(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[0]), MTFEUnicode);
            setFontSans(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[1]), MTFEUnicode);
            setFontMathSans(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[1]), MTFEUnicode);
        } else if (splitSlash.size()==4) {
            setFontRoman(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[0]), MTFEUnicode);
            setFontMathRoman(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[2]), MTFEUnicode);
            setFontSans(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[1]), MTFEUnicode);
            setFontMathSans(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(splitSlash[3]), MTFEUnicode);
        } else {
            qDebug()<<"JKQTMathText::setFontSpecial(): undecodable fontSpec '"<<fontSpec<<"'";
        }
    }
    if (afterPlus.toUpper()=="XITS") useXITS();
    if (afterPlus.toUpper()=="STIX") useSTIX();
    if (afterPlus.toUpper()=="ASANA") useASANA();
    if (afterPlus.toUpper()=="FIRA") useFiraMath();
}

void JKQTMathText::setFontRoman(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEroman].fontName = f.first;
    fontDefinitions[MTEroman].fontEncoding = f.second;
}

void JKQTMathText::setFontRomanAndMath(const QString &fontName, JKQTMathTextFontEncoding encoding)
{
    setFontRoman(fontName, encoding);
    setFontMathRoman(fontName, encoding);
}

QString JKQTMathText::getFontRoman() const
{
    return fontDefinitions[MTEroman].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingRoman() const
{
    return fontDefinitions[MTEroman].fontEncoding;
}

void JKQTMathText::setFontSans(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEsans].fontName = f.first;
    fontDefinitions[MTEsans].fontEncoding = f.second;
}

QString JKQTMathText::getFontSans() const
{
    return fontDefinitions[MTEsans].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingSans() const
{
    return fontDefinitions[MTEsans].fontEncoding;
}

void JKQTMathText::setFontTypewriter(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEtypewriter].fontName = f.first;
    fontDefinitions[MTEtypewriter].fontEncoding = f.second;
}

QString JKQTMathText::getFontTypewriter() const
{
    return fontDefinitions[MTEtypewriter].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingTypewriter() const
{
    return fontDefinitions[MTEtypewriter].fontEncoding;
}

void JKQTMathText::setFontScript(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEscript].fontName = f.first;
    fontDefinitions[MTEscript].fontEncoding = f.second;
}

QString JKQTMathText::getFontScript() const
{
    return fontDefinitions[MTEscript].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingScript() const
{
    return fontDefinitions[MTEscript].fontEncoding;
}

void JKQTMathText::setFontFraktur(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEfraktur].fontName = f.first;
    fontDefinitions[MTEfraktur].fontEncoding = f.second;
}

QString JKQTMathText::getFontFraktur() const
{
    return fontDefinitions[MTEfraktur].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingFraktur() const
{
    return fontDefinitions[MTEfraktur].fontEncoding;
}

void JKQTMathText::setFallbackFontSymbols(const QString &fontName, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(fontName, fontName, encoding);
    fontDefinitions[MTEFallbackSymbols].fontName = f.first;
    fontDefinitions[MTEFallbackSymbols].fontEncoding = f.second;
}


QString JKQTMathText::getFallbackFontSymbols() const
{
    return fontDefinitions[MTEFallbackSymbols].fontName;
}


JKQTMathTextFontEncoding JKQTMathText::getFontEncodingFallbackFontSymbols() const
{
    return fontDefinitions[MTEFallbackSymbols].fontEncoding;
}

void JKQTMathText::setFontCaligraphic(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEcaligraphic].fontName = f.first;
    fontDefinitions[MTEcaligraphic].fontEncoding = f.second;
}

QString JKQTMathText::getFontCaligraphic() const
{
    return fontDefinitions[MTEcaligraphic].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingCaligraphic() const
{
    return fontDefinitions[MTEcaligraphic].fontEncoding;
}

void JKQTMathText::setFontMathRoman(const QString &fontName, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(fontName, fontName, encoding);
    fontDefinitions[MTEmathRoman].fontName = f.first;
    fontDefinitions[MTEmathRoman].fontEncoding = f.second;
}

QString JKQTMathText::getFontMathRoman() const
{
    return fontDefinitions[MTEmathRoman].fontName;
}

void JKQTMathText::setFontMathSans(const QString &fontName, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(fontName, fontName, encoding);
    fontDefinitions[MTEmathSans].fontName = f.first;
    fontDefinitions[MTEmathSans].fontEncoding = f.second;
}

QString JKQTMathText::getFontMathSans() const
{
    return fontDefinitions[MTEmathSans].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingMathSans() const
{
    return fontDefinitions[MTEmathSans].fontEncoding;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingMathRoman() const
{
    return fontDefinitions[MTEmathRoman].fontEncoding;
}

void JKQTMathText::setFontBlackboard(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEblackboard].fontName = f.first;
    fontDefinitions[MTEblackboard].fontEncoding = f.second;
}

JKQTMathTextBlackboradDrawingMode JKQTMathText::getFontBlackboradMode() const
{
    return blackboradFontMode;
}

void JKQTMathText::setFontBlackboradMode(JKQTMathTextBlackboradDrawingMode mode)
{
    blackboradFontMode=mode;
}

QString JKQTMathText::getFontBlackboard() const
{
    return fontDefinitions[MTEblackboard].fontName;
}

JKQTMathTextFontEncoding JKQTMathText::getFontEncodingBlackboard() const
{
    return fontDefinitions[MTEblackboard].fontEncoding;
}


void JKQTMathText::setBraceFactor(double __value)
{
    this->brace_factor = __value;
}

double JKQTMathText::getBraceFactor() const
{
    return this->brace_factor;
}

void JKQTMathText::setSubsuperSizeFactor(double __value)
{
    this->subsuper_size_factor = __value;
}

double JKQTMathText::getSubsuperSizeFactor() const
{
    return this->subsuper_size_factor;
}

void JKQTMathText::setSubsuperModeSelectionBySizeFactor(double __value)
{
    subsuper_mode_selection_by_size_factor=__value;
}

double JKQTMathText::getSubsuperModeSelectionBySizeFactor() const
{
    return subsuper_mode_selection_by_size_factor;
}

void JKQTMathText::setOperatorsubsuperSizeFactor(double __value)
{
    this->operatorsubsuper_size_factor = __value;
}

double JKQTMathText::getOperatorsubsuperSizeFactor() const
{
    return this->operatorsubsuper_size_factor;
}

void JKQTMathText::setOperatorsubsuperDistanceFactor(double __value)
{
    this->operatorsubsuper_distance_factor = __value;
}

double JKQTMathText::getOperatorsubsuperDistanceFactor() const
{
    return this->operatorsubsuper_distance_factor;
}

void JKQTMathText::setOperatorsubsuperExtraSpaceFactor(double __value)
{
    operatorsubsuper_extraspace_factor=__value;
}

double JKQTMathText::getOperatorsubsuperExtraSpaceFactor() const
{
    return operatorsubsuper_extraspace_factor;
}

void JKQTMathText::setMathoperatorWidthFactor(double __value)
{
    this->mathoperator_width_factor = __value;
}

double JKQTMathText::getMathoperatorWidthFactor() const
{
    return this->mathoperator_width_factor;
}

void JKQTMathText::setIntSubSuperXCorrectionFactor(double __value)
{
    intsubsuper_xcorrection_factor=__value;
}

double JKQTMathText::getIntSubSuperXCorrectionFactor() const
{
    return intsubsuper_xcorrection_factor;
}

void JKQTMathText::setIntSubBesidesXCorrectionXFactor(double __value)
{
    intsubbesides_xcorrection_xfactor=__value;
}

double JKQTMathText::getIntSubBesidesXCorrectionXFactor() const
{
    return intsubbesides_xcorrection_xfactor;
}

void JKQTMathText::setBigMathoperatorFontFactor(double __value)
{
    bigmathoperator_font_factor=__value;
}

double JKQTMathText::getBigMathoperatorFontFactor() const
{
    return bigmathoperator_font_factor;
}

void JKQTMathText::setSuperShiftFactor(double __value)
{
    this->super_shift_factor = __value;
}

double JKQTMathText::getSuperShiftFactor() const
{
    return this->super_shift_factor;
}

void JKQTMathText::setSubShiftFactor(double __value)
{
    this->sub_shift_factor = __value;
}

double JKQTMathText::getSubShiftFactor() const
{
    return this->sub_shift_factor;
}

double JKQTMathText::getSpecialSuperShiftFactor() const
{
    return special_super_shift_factor;
}

void JKQTMathText::setSpecialSuperShiftFactor(double __value)
{
    special_super_shift_factor=__value;
}

void JKQTMathText::setSpecialSubShiftFactor(double __value)
{
    special_sub_shift_factor=__value;
}

double JKQTMathText::getSpecialSubShiftFactor() const
{
    return special_sub_shift_factor;
}

void JKQTMathText::setUnderbraceFactor(double __value)
{
    this->underbrace_factor = __value;
}

double JKQTMathText::getUnderbraceFactor() const
{
    return this->underbrace_factor;
}

void JKQTMathText::setUnderbraceSeparationXFactor(double __value)
{
    underbrace_separation_xfactor=__value;
}

double JKQTMathText::getUnderbraceSeparationXFactor() const
{
    return underbrace_separation_xfactor;
}

void JKQTMathText::setUnderbraceBraceSizeXFactor(double __value)
{
    underbrace_bracesize_xfactor=__value;
}

double JKQTMathText::getUnderbraceBraceSizeXFactor() const
{
    return underbrace_bracesize_xfactor;
}

void JKQTMathText::setUndersetFactor(double __value)
{
    this->underset_factor = __value;
}

double JKQTMathText::getUndersetFactor() const
{
    return this->underset_factor;
}

void JKQTMathText::setFracFactor(double __value)
{
    this->frac_factor = __value;
}

double JKQTMathText::getFracFactor() const
{
    return this->frac_factor;
}

void JKQTMathText::setFracNestedFactor(double __value)
{
    frac_nested_factor=__value;
}

double JKQTMathText::getFracNestedFactor() const
{
    return frac_nested_factor;
}

void JKQTMathText::setFracShiftFactor(double __value)
{
    this->frac_shift_factor = __value;
}

double JKQTMathText::getFracShiftFactor() const
{
    return this->frac_shift_factor;
}

void JKQTMathText::setBraceYShiftFactor(double __value)
{
    this->brace_y_shift_factor = __value;
}

double JKQTMathText::getBraceYShiftFactor() const
{
    return this->brace_y_shift_factor;
}

void JKQTMathText::setDecorationHeightFactor(double __value)
{
    this->decoration_height_factor = __value;
}

double JKQTMathText::getDecorationHeightFactor() const
{
    return this->decoration_height_factor;
}

void JKQTMathText::setDecorationSeparationXFactor(double __value)
{
    decoration_separation_factor=__value;
}

double JKQTMathText::getDecorationSeparationFactor() const
{
    return decoration_separation_factor;
}

void JKQTMathText::setDecorationWidthReductionFactor(double __value)
{
    decoration_width_reduction_Xfactor=__value;
}

double JKQTMathText::getDecorationWidthReductionXFactor() const
{
    return decoration_width_reduction_Xfactor;
}

void JKQTMathText::setSqrtWidthXFactor(double __value)
{
    sqrt_width_Xfactor=__value;
}

double JKQTMathText::getSqrtWidthXFactor() const
{
    return sqrt_width_Xfactor;
}

void JKQTMathText::setSqrtHeightFactor(double __value)
{
    sqrt_height_factor=__value;
}

double JKQTMathText::getSqrtHeightFactor() const
{
    return sqrt_height_factor;
}

void JKQTMathText::setSqrtSmallFontFactor(double __value)
{
    sqrt_smallfont_factor=__value;
}

double JKQTMathText::getSqrtSmallFontFactor() const
{
    return sqrt_smallfont_factor;
}

double JKQTMathText::getMatrixLinewidthThinFactor()
{
    return matrix_linewidth_thin_factor;
}

void JKQTMathText::setMatrixLinewidthThinFactor(double factor)
{
    matrix_linewidth_thin_factor=factor;
}

double JKQTMathText::getMatrixLinewidthHeavyFactor()
{
    return matrix_linewidth_heavy_factor;
}

void JKQTMathText::setMatrixLinewidthHeavyFactor(double factor)
{
    matrix_linewidth_heavy_factor=factor;
}

double JKQTMathText::getMatrixLineSeparationFactor()
{
    return matrix_line_separation_factor;
}

void JKQTMathText::setMatrixLineSeparationFactor(double factor)
{
    matrix_line_separation_factor=factor;
}

double JKQTMathText::getMatrixXSeparationFactor()
{
    return matrix_xSeparation_factor;
}

void JKQTMathText::setMatrixXSeparationFactor(double factor)
{
    matrix_xSeparation_factor=factor;
}

double JKQTMathText::getMatrixYSeparationFactor()
{
    return matrix_ySeparation_factor;
}

void JKQTMathText::setMatrixYSeparationFactor(double factor)
{
    matrix_ySeparation_factor=factor;
}

double JKQTMathText::getMatrixXPaddingFactor()
{
    return matrix_xPadding_factor;
}

void JKQTMathText::setMatrixXPaddingFactor(double factor)
{
    matrix_xPadding_factor=factor;
}

double JKQTMathText::getMatrixYPaddingFactor()
{
    return matrix_yPadding_factor;
}

void JKQTMathText::setMatrixYPaddingFactor(double factor)
{
    matrix_yPadding_factor=factor;
}

QStringList JKQTMathText::getErrorList() const {
    return this->error_list;
}

bool JKQTMathText::hadErrors() const
{
    return error_list.size()>0;
}

void JKQTMathText::addToErrorList(const QString &error)
{
    error_list.append(error);
}


void JKQTMathText::clearErrorList()
{
    error_list.clear();
}

const JKQTMathTextNode *JKQTMathText::getNodeTree() const {
    return this->parsedNode;
}






QSizeF JKQTMathText::getSize(QPainter& painter){
    if (getNodeTree()!=nullptr) {
        const JKQTMathTextNodeSize s=getSizeDetail(painter);
        return s.getSize();
    }
    return QSizeF(0,0);
}

QSize JKQTMathText::getIntSize(QPainter &painter)
{
    if (getNodeTree()!=nullptr) {
        const JKQTMathTextNodeSize s=getSizeDetail(painter);
        return s.getIntSize();
    }
    return QSize(0,0);
}

double JKQTMathText::getDescent(QPainter& painter) {
    const JKQTMathTextNodeSize s=getSizeDetail(painter);
    return s.getDescent();
}

double JKQTMathText::getAscent(QPainter& painter) {
    const JKQTMathTextNodeSize s=getSizeDetail(painter);
    return s.baselineHeight;
}

void JKQTMathText::getSizeDetail(QPainter& painter, double& width, double& ascent, double& descent, double& strikeoutPos) {
    JKQTMathTextNodeSize s=getSizeDetail(painter);
    width=s.width;
    ascent=s.baselineHeight;
    descent=s.getDescent();
    strikeoutPos=s.strikeoutPos;
}

JKQTMathTextNodeSize JKQTMathText::getSizeDetail(QPainter &painter)
{
    JKQTMathTextNodeSize s;
    if (getNodeTree()!=nullptr) {
        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        ev.fontSizeUnit=fontSizeUnits;

        s=getNodeTree()->getSize(painter, ev);
    }
    return s;
}

void JKQTMathText::draw(QPainter &painter, QPointF x, bool drawBoxes)
{
    draw(painter, x.x(), x.y(), drawBoxes);
}

double JKQTMathText::draw(QPainter& painter, double x, double y, bool drawBoxes){
    if (getNodeTree()!=nullptr) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(fontColor);
        painter.setBrush(Qt::NoBrush);
        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        ev.fontSizeUnit=fontSizeUnits;
        getNodeTree()->setDrawBoxes(drawBoxes);
        const double xend=getNodeTree()->draw(painter, x, y, ev);
        return xend;
    }
    return x;
}

void JKQTMathText::draw(QPainter& painter, unsigned int flags, QRectF rect, bool drawBoxes) {
    if (getNodeTree()!=nullptr) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(fontColor);
        painter.setBrush(Qt::NoBrush);

        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        ev.fontSizeUnit=fontSizeUnits;
        getNodeTree()->setDrawBoxes(drawBoxes);

        const JKQTMathTextNodeSize size= getSizeDetail(painter);

        // align left top
        double x=rect.left();
        double y=rect.top()+size.baselineHeight;

        // care for horizontal align
        if ((flags & Qt::AlignRight) != 0) x=x+rect.width()-size.width;
        else if ((flags & Qt::AlignHCenter) != 0) x=x+(rect.width()-size.width)/2.0;

        // care for vertical align
        if ((flags & Qt::AlignBottom) != 0) y=y+rect.height()-size.overallHeight;
        else if ((flags & Qt::AlignVCenter) != 0) y=y+(rect.height()-size.overallHeight)/2.0;

        // finally draw
        const double xend=getNodeTree()->draw(painter, x, y, ev);
    }
}

QPixmap JKQTMathText::drawIntoPixmap(bool drawBoxes, QColor backgroundColor, int sizeincrease, qreal devicePixelRatio)
{
    // 1. generate dummy QPixmap that is needed to use a QPainter
    //    we need the dummy, because we first need to determine the size of the render output
    //    for which we need a QPainter.
    QPixmap pix(1,1);
    pix.setDevicePixelRatio(devicePixelRatio);
    {
        QPainter painter;

        // 2. now we determine the size and additional parameters,
        //    such as the ascent(or "baseline height")
        painter.begin(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if (QT_VERSION>=QT_VERSION_CHECK(6, 1, 0))
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
        const JKQTMathTextNodeSize size=getSizeDetail(painter);
        const QSize pixsize=size.getIntSize()+QSize(2*sizeincrease,2*sizeincrease);
        painter.end();

        // 3. finally we can generate a QPixmap with the appropriate
        //    size to contain the full rendering. We fill it with the
        //    color white and finally paint the math markup/LaTeX string
        pix=QPixmap(pixsize);
        pix.setDevicePixelRatio(devicePixelRatio);
        pix.fill(backgroundColor);
        painter.begin(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if (QT_VERSION>=QT_VERSION_CHECK(6, 1, 0))
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
        draw(painter, Qt::AlignVCenter|Qt::AlignHCenter, QRect(QPoint(0,0),pixsize), drawBoxes);
        painter.end();
    }
    return pix;
}

QImage JKQTMathText::drawIntoImage(bool drawBoxes, QColor backgroundColor, int sizeincrease, qreal devicePixelRatio, unsigned int resolution_dpi)
{
    // 1. generate dummy QPixmap that is needed to use a QPainter
    //    we need the dummy, because we first need to determine the size of the render output
    //    for which we need a QPainter.
    QImage img(1,1,QImage::Format_ARGB32_Premultiplied);
    img.setDevicePixelRatio(devicePixelRatio);
    img.setDotsPerMeterX(resolution_dpi*(10000/254));
    img.setDotsPerMeterY(resolution_dpi*(10000/254));
    {
        QPainter painter;

        // 2. now we determine the size and additional parameters,
        //    such as the ascent(or "baseline height")
        painter.begin(&img);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if (QT_VERSION>=QT_VERSION_CHECK(6, 1, 0))
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
        const JKQTMathTextNodeSize size=getSizeDetail(painter);
        const QSize pixsize=size.getIntSize()+QSize(2*sizeincrease,2*sizeincrease);
        painter.end();

        // 3. finally we can generate a QPixmap with the appropriate
        //    size to contain the full rendering. We fill it with the
        //    color white and finally paint the math markup/LaTeX string
        img=QImage(pixsize*devicePixelRatio,QImage::Format_ARGB32_Premultiplied);
        img.setDevicePixelRatio(devicePixelRatio);
        img.setDotsPerMeterX(resolution_dpi*(10000/254));
        img.setDotsPerMeterY(resolution_dpi*(10000/254));
        img.fill(backgroundColor);
        painter.begin(&img);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if (QT_VERSION>=QT_VERSION_CHECK(6, 1, 0))
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
        draw(painter, Qt::AlignVCenter|Qt::AlignHCenter, QRect(QPoint(0,0),pixsize), drawBoxes);
        painter.end();
    }
    return img;
}

QPicture JKQTMathText::drawIntoPicture(bool drawBoxes)
{
    // 1. generate dummy QPixmap that is needed to use a QPainter
    //    we need the dummy, because we first need to determine the size of the render output
    //    for which we need a QPainter.
    QPicture pic;
    {
        QPainter painter;

        // 2. now we determine the size and additional parameters,
        //    such as the ascent(or "baseline height")
        painter.begin(&pic);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if (QT_VERSION>=QT_VERSION_CHECK(6, 1, 0))
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
        const JKQTMathTextNodeSize size=getSizeDetail(painter);
        painter.end();

        // 3. finally we can generate a QPixmap with the appropriate
        //    size to contain the full rendering. We fill it with the
        //    color white and finally paint the math markup/LaTeX string
        painter.begin(&pic);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if (QT_VERSION>=QT_VERSION_CHECK(6, 1, 0))
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
        draw(painter, 0, size.baselineHeight, drawBoxes);
        painter.end();
    }
    return pic;
}


JKQTMathTextNode *JKQTMathText::getNodeTree() {
    return parsedNode;
}
