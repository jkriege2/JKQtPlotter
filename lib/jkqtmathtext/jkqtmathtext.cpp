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
#include "jkqtmathtext/nodes/jkqtmathtextnodetools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <typeinfo>
#include <QApplication>
#include <QPainterPath>


const double JKQTMathText::ABS_MIN_LINEWIDTH=0.02;





// --------------------------------------------------------------------------------------------------
// -- implementation of the JKQTMathText methods
// --------------------------------------------------------------------------------------------------
JKQTMathText::JKQTMathText(QObject* parent):
    QObject(parent)
{
    //std::chrono::high_resolution_clock::time_point t0=std::chrono::high_resolution_clock::now();
    initJKQTMathTextResources();
    //qDebug()<<"init_resoucre: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();

    fontSize=10;
    fontSizeUnits=JKQTMathTextEnvironment::POINTS;
    fontColor=QColor("black");
    italic_correction_factor=0.4;
    brace_factor=1.04;
    brace_shrink_factor=0.45;
    brace_y_shift_factor=0.7;//-1;

    subsuper_size_factor=0.7;
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
    operatorsubsuper_distance_factor=0.25;
    operatorsubsuper_extraspace_factor=0.5;
    intsubsuper_xcorrection_factor=0.25;
    intsubbesides_xcorrection_xfactor=0.33;
    mathoperator_width_factor=1.5;
    bigmathoperator_font_factor=1.8;

    sqrt_width_Xfactor=0.8;
    sqrt_height_factor=1.2;
    sqrt_smallfont_factor=0.57;

    blackboardSimulated=true;


    static QString serifFont="serif";
    static QString sansFont="sans";
    static QString symbolFont="symbol";
    static QString scriptFont="script";
    static QString typewriterFont="typewriter";
    static QString decorativeFont="decorative";
    static QString blackboardFont="blackboard";
    static QString fracturFont="fraktur";
    static bool firstStart=true;

    if (firstStart) {
        //t0=std::chrono::high_resolution_clock::now();

        firstStart=false;
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

        checkForFonts(serifFont, QStringList {"Times New Roman", "Times", "FreeSerif", "DejaVu Serif"});
        //qDebug()<<"check 1st font: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms";

        checkForFonts(sansFont, QStringList {"Arial Unicode MS", "Arial Unicode", "Lucida Sans Unicode", "Arial", "Helvetica", "FreeSans", "DejaVu Sans", "Lucida Sans"});
        checkForFonts(symbolFont, QStringList {"SymbolStandard", "Symbol"});
        checkForFonts(typewriterFont, QStringList {"Courier New", "Courier", "Courier Std", "FreeMono", "CMU Typewriter Text", "UM Typewriter"});
        checkForFonts(blackboardFont, QStringList {"Double Stroke", "CloisterOpenFace BT", "GoudyHandtooled BT", "Castellar", "MathJax_AMS", "Castellar Standard", "MathJax_AMS Standard", "Colonna MT"});
        checkForFonts(decorativeFont, QStringList {"Lucida Calligraphy", "Cookie", "Segoe Print", "Comic Sans", "Comic Sans MS", "Gabriola", "Gabriola Standard", "Lucida Handwriting Kursiv", "Lucida Handwriting", "Pristina", "Pristina Standard", "MathJax_Caligraphics"});
        checkForFonts(scriptFont, QStringList {"Lucida Handwriting", "Dancing Script", "Amazone BT", "ScriptS", "ScriptC", "ScriptC Standard", "Script", "Brush Script MT", "Brush Script MT Kursiv", "MathJax_Script"});
        checkForFonts(fracturFont, QStringList {"Old English Text MT", "Old English Text MT Standard", "UnifrakturMaguntia Standard", "UnifrakturMaguntia", "MathJax_Fraktur", "UnifrakturCook Fett"});
        //qDebug()<<"check all font: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms";
    }

    //t0=std::chrono::high_resolution_clock::now();
    if (serifFont!="serif") addReplacementFont("serif", serifFont);
    if (sansFont!="sans") addReplacementFont("sans", sansFont);
    if (symbolFont!="symbol") addReplacementFont("symbol", symbolFont);
    if (scriptFont!="script") addReplacementFont("script", scriptFont);
    if (typewriterFont!="typewriter") addReplacementFont("typewriter", typewriterFont);
    if (decorativeFont!="decorative") addReplacementFont("decorative", decorativeFont);
    if (fracturFont!="fraktur") addReplacementFont("fraktur", fracturFont);
    if (blackboardFont!="blackboard") {
        addReplacementFont("blackboard", blackboardFont);
    }
    //qDebug()<<"add replacement fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    setFontSans(sansFont, estimateJKQTMathTextFontEncoding(sansFont));
    setFontMathSans(sansFont, estimateJKQTMathTextFontEncoding(sansFont));
    setFontTypewriter(typewriterFont, estimateJKQTMathTextFontEncoding(typewriterFont));
    setFontRoman(serifFont, estimateJKQTMathTextFontEncoding(serifFont));
    setFontMathRoman(serifFont, estimateJKQTMathTextFontEncoding(serifFont));
    setFontCaligraphic(decorativeFont, estimateJKQTMathTextFontEncoding(decorativeFont));
    if (blackboardFont!="blackboard") {
        setFontBlackboard(blackboardFont, estimateJKQTMathTextFontEncoding(blackboardFont));
    } else {
        setFontBlackboardSimulated(blackboardFont=="blackboard");
    }
    setFontScript(scriptFont, estimateJKQTMathTextFontEncoding(scriptFont));
    setFontFraktur(fracturFont, estimateJKQTMathTextFontEncoding(fracturFont));
    setFallbackFontSymbols(symbolFont, estimateJKQTMathTextFontEncoding(symbolFont));
    //qDebug()<<"set fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    useXITS();
    //qDebug()<<"useXITS: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    useUnparsed=false;

    parsedNode=nullptr;
    unparsedNode=nullptr;

    currentToken=MTTnone;
    currentTokenName="";
    currentTokenID=0;
    parseString="";
    parsingMathEnvironment=false;
}

JKQTMathText::~JKQTMathText() {
    if (parsedNode!=nullptr) delete parsedNode;
    parsedNode=nullptr;
    if (unparsedNode!=nullptr) delete unparsedNode;
    unparsedNode=nullptr;
}

void JKQTMathText::loadSettings(const QSettings& settings, const QString& group){
    fontSize=settings.value(group+"font_size", fontSize).toDouble();
    fontSizeUnits=JKQTMathTextEnvironment::String2FontSizeUnit(settings.value(group+"font_size_units", JKQTMathTextEnvironment::FontSizeUnit2String(fontSizeUnits)).toString());
    fontColor=jkqtp_String2QColor(settings.value(group+"font_color", jkqtp_QColor2String(fontColor)).toString());
    brace_factor=settings.value(group+"brace_factor", brace_factor).toDouble();
    brace_shrink_factor=settings.value(group+"brace_shrink_factor", brace_shrink_factor).toDouble();
    subsuper_size_factor=settings.value(group+"subsuper_size_factor", subsuper_size_factor).toDouble();
    subsuper_mode_selection_by_size_factor=settings.value(group+"subsuper_mode_selection_by_size_factor", subsuper_mode_selection_by_size_factor).toDouble();
    italic_correction_factor=settings.value(group+"italic_correction_factor", italic_correction_factor).toDouble();
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


    if (settings.value(group+"use_stix_fonts", false).toBool()) useSTIX();
    if (settings.value(group+"use_xits_fonts", false).toBool()) useXITS();
    if (settings.value(group+"use_asana_fonts", false).toBool()) useASANA();
}


void JKQTMathText::saveSettings(QSettings& settings, const QString& group) const{
    settings.setValue(group+"font_size", fontSize);
    settings.setValue(group+"font_size_units", JKQTMathTextEnvironment::FontSizeUnit2String(fontSizeUnits));
    settings.setValue(group+"font_color", jkqtp_QColor2String(fontColor));
    settings.setValue(group+ "brace_factor", brace_factor);
    settings.setValue(group+ "brace_shrink_factor", brace_shrink_factor);
    settings.setValue(group+ "subsuper_size_factor", subsuper_size_factor);
    settings.setValue(group+ "subsuper_mode_selection_by_size_factor", subsuper_mode_selection_by_size_factor);
    settings.setValue(group+ "italic_correction_factor", italic_correction_factor);
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
}

bool JKQTMathText::useSTIX(bool mathModeOnly) {

    const JKQTMathTextFontSpecifier stixs=JKQTMathTextFontSpecifier::getSTIXFamilies();
    bool res=false;

    if (!mathModeOnly && !stixs.fontName().isEmpty()) {
        setFontRoman(stixs.fontName(), MTFEUnicode);
        res=true;
    }
    if (!stixs.mathFontName().isEmpty()) {
        setFontMathRoman(stixs.mathFontName(), MTFEUnicode);
        setFallbackFontSymbols(stixs.mathFontName(), MTFEUnicode);
        res=true;
    } else if (!stixs.fontName().isEmpty()) {
        setFontMathRoman(stixs.fontName(), MTFEUnicode);
        setFallbackFontSymbols(stixs.fontName(), MTFEUnicode);
        res=true;
    }

    return res;
}

bool JKQTMathText::useXITS(bool mathModeOnly)
{

    const JKQTMathTextFontSpecifier xits=JKQTMathTextFontSpecifier::getXITSFamilies();
    bool res=false;

    if (!mathModeOnly && !xits.fontName().isEmpty()) {
        setFontRoman(xits.fontName(), MTFEUnicode);
        setFallbackFontSymbols(xits.fontName(), MTFEUnicode);
        res=true;
    }
    if (!xits.mathFontName().isEmpty()) {
        setFontMathRoman(xits.mathFontName(), MTFEUnicode);
        setFallbackFontSymbols(xits.mathFontName(), MTFEUnicode);
        res=true;
    }

    return res;
}

bool JKQTMathText::useASANA(bool mathModeOnly)
{

    const JKQTMathTextFontSpecifier asana=JKQTMathTextFontSpecifier::getXITSFamilies();
    bool res=false;

    if (!mathModeOnly && !asana.fontName().isEmpty()) {
        setFontRoman(asana.fontName(), MTFEUnicode);
        setFallbackFontSymbols(asana.fontName(), MTFEUnicode);
        res=true;
    }
    if (!asana.mathFontName().isEmpty()) {
        setFontMathRoman(asana.mathFontName(), MTFEUnicode);
        setFallbackFontSymbols(asana.mathFontName(), MTFEUnicode);
        res=true;
    }

    return res;
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
        if (fontName.fontName().toUpper()=="XITS") useXITS(false);
        else if (fontName.fontName().toUpper()=="STIX") useSTIX(false);
        else if (fontName.fontName().toUpper()=="ASANA") useASANA(false);
        else {
            setFontRoman(fontName.fontName(), encoding);
            setFontMathRoman(fontName.fontName(), encoding);
        }
    } else {
        if (fontName.mathFontName().toUpper()=="XITS") useXITS(true);
        else if (fontName.mathFontName().toUpper()=="STIX") useSTIX(true);
        else if (fontName.mathFontName().toUpper()=="ASANA") useASANA(true);
        else setFontMathRoman(fontName.mathFontName(), encoding);
        setFontRoman(fontName.fontName(), encoding);
    }
}

void JKQTMathText::setFontRoman(const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEroman].fontName = f.first;
    fontDefinitions[MTEroman].fontEncoding = f.second;
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
    blackboardSimulated=false;
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEblackboard].fontName = f.first;
    fontDefinitions[MTEblackboard].fontEncoding = f.second;
}

void JKQTMathText::setFontBlackboardSimulated(bool doSimulate)
{
    blackboardSimulated=doSimulate;
}

bool JKQTMathText::isFontBlackboardSimulated() const
{
    return blackboardSimulated;
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

void JKQTMathText::setItalicCorrectionFactor(double __value)
{
    this->italic_correction_factor = __value;
}

double JKQTMathText::getItalicCorrectionFactor() const
{
    return this->italic_correction_factor;
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

void JKQTMathText::setBraceShrinkFactor(double __value)
{
    this->brace_shrink_factor = __value;
}

double JKQTMathText::getBraceShrinkFactor() const
{
    return this->brace_shrink_factor;
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

void JKQTMathText::setUseUnparsed(bool __value)
{
    this->useUnparsed = __value;
}

bool JKQTMathText::isUsingUnparsed() const
{
    return this->useUnparsed;
}

QStringList JKQTMathText::getErrorList() const {
    return this->error_list;
}

void JKQTMathText::addToErrorList(const QString &error)
{
    error_list.append(error);
}



JKQTMathText::tokenType JKQTMathText::getToken() {
    currentTokenID++;
    if (currentTokenID>parseString.size()-1) return currentToken=MTTnone;
    QChar c=parseString[currentTokenID];
    currentTokenName="";
    //----------------------------------------------------------
    // define some static sets for easy character lookup/identificattion
    static QSet<QChar> TokenCharacters;
    static QSet<QChar> mathEnvironmentSpecialChars, mathEnvironmentSpecialEndChars;
    static QSet<QChar> SingleCharInstructions;
    static QHash<QString, QChar> accentLetters;
    static QSet<int> accentLetters_LenBackslash;
    static QSet<int> accentLetters_LenCurly;
    if (TokenCharacters.size()==0) {
        mathEnvironmentSpecialChars<<'(' << '[' << '|' << ')' << ']' << '+' << '-' << '*' << '/' << '<' << '>' << '=';
        mathEnvironmentSpecialEndChars<<'(' << '&' << '[' << '|' << ')' << ']' << '\\' << '$' << '{' << '}' << '_' << '^' << '+' << '-' << '/' << '*' << '=' << '<' << '>';
        TokenCharacters<<'_'<<'^'<<'\\'<<'$'<<'&'<<'}'<<'{'<<'['<<']';
        SingleCharInstructions<<'|'<<';'<<':'<<'!'<<','<<'_'<<'\\'<<'$'<<'%'<<'&'<<'#'<<'}'<<'{'<<' '<<'['<<']';

        auto fAddUml=[](const QString& cmd, const QChar& letter, const QChar& ch) {
            QString i;
            if (cmd.size()>0 && !letter.isNull()) {
                i="\\"+cmd+letter;
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="{\\"+cmd+letter+"}";
                accentLetters[i]=ch; accentLetters_LenCurly.insert(i.size());
                i="\\"+cmd+"{"+letter+"}";
                accentLetters[i]=ch;  accentLetters_LenBackslash.insert(i.size());
            } else if  (cmd.size()>0 && letter.isNull()) {
                i="\\"+cmd+" ";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="\\"+cmd+"\t";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="\\"+cmd+"\n";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="\\"+cmd+"\\";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="{\\"+cmd+"}";
                accentLetters[i]=ch; accentLetters_LenCurly.insert(i.size());
            }
        };
        // instructions like \"{a}
        fAddUml("\"", 'A', QChar(0xC4));
        fAddUml("\"", 'E', QChar(0xCB));
        fAddUml("\"", 'I', QChar(0xCF));
        fAddUml("\"", 'O', QChar(0xD6));
        fAddUml("\"", 'U', QChar(0xDC));
        fAddUml("\"", 'Y', QChar(0x178));
        fAddUml("\"", 'a', QChar(0xE4));
        fAddUml("\"", 'e', QChar(0xEB));
        fAddUml("\"", 'i', QChar(0xEF));
        fAddUml("\"", 'o', QChar(0xF6));
        fAddUml("\"", 'u', QChar(0xFC));
        fAddUml("\"", 'y', QChar(0xFF));

        fAddUml("'", 'A', QChar(0xC1));
        fAddUml("'", 'E', QChar(0xC9));
        fAddUml("'", 'I', QChar(0xCD));
        fAddUml("'", 'O', QChar(0xD3));
        fAddUml("'", 'U', QChar(0xDA));
        fAddUml("'", 'Y', QChar(0xDD));
        fAddUml("'", 'a', QChar(0xE1));
        fAddUml("'", 'e', QChar(0xE9));
        fAddUml("'", 'i', QChar(0xED));
        fAddUml("'", 'o', QChar(0xF3));
        fAddUml("'", 'u', QChar(0xFA));
        fAddUml("'", 'y', QChar(0xFD));
        fAddUml("'", 'C', QChar(0x106));
        fAddUml("'", 'c', QChar(0x107));
        fAddUml("'", 'L', QChar(0x139));
        fAddUml("'", 'l', QChar(0x13A));
        fAddUml("'", 'N', QChar(0x143));
        fAddUml("'", 'n', QChar(0x144));
        fAddUml("'", 'R', QChar(0x154));
        fAddUml("'", 'r', QChar(0x155));
        fAddUml("'", 'S', QChar(0x15A));
        fAddUml("'", 's', QChar(0x15B));
        fAddUml("'", 'Z', QChar(0x179));
        fAddUml("'", 'z', QChar(0x17A));
        fAddUml("'", 'G', QChar(0x1F4));
        fAddUml("'", 'g', QChar(0x1F5));

        fAddUml("`", 'A', QChar(0xC0));
        fAddUml("`", 'E', QChar(0xC8));
        fAddUml("`", 'I', QChar(0xCC));
        fAddUml("`", 'O', QChar(0xD2));
        fAddUml("`", 'U', QChar(0xD9));
        fAddUml("`", 'a', QChar(0xE0));
        fAddUml("`", 'e', QChar(0xE8));
        fAddUml("`", 'i', QChar(0xEC));
        fAddUml("`", 'o', QChar(0xF2));
        fAddUml("`", 'u', QChar(0xF9));
        fAddUml("`", 'N', QChar(0x1F8));
        fAddUml("`", 'n', QChar(0x1F9));

        fAddUml("^", 'A', QChar(0xC2));
        fAddUml("^", 'E', QChar(0xCA));
        fAddUml("^", 'I', QChar(0xCE));
        fAddUml("^", 'O', QChar(0xD4));
        fAddUml("^", 'U', QChar(0xDB));
        fAddUml("^", 'a', QChar(0xE2));
        fAddUml("^", 'e', QChar(0xEA));
        fAddUml("^", 'i', QChar(0xEE));
        fAddUml("^", 'o', QChar(0xF4));
        fAddUml("^", 'u', QChar(0xFB));
        fAddUml("^", 'C', QChar(0x108));
        fAddUml("^", 'c', QChar(0x109));
        fAddUml("^", 'G', QChar(0x11C));
        fAddUml("^", 'g', QChar(0x11D));
        fAddUml("^", 'H', QChar(0x124));
        fAddUml("^", 'h', QChar(0x125));
        fAddUml("^", 'J', QChar(0x134));
        fAddUml("^", 'j', QChar(0x135));
        fAddUml("^", 'S', QChar(0x15C));
        fAddUml("^", 's', QChar(0x15D));
        fAddUml("^", 'W', QChar(0x174));
        fAddUml("^", 'w', QChar(0x175));
        fAddUml("^", 'Y', QChar(0x176));
        fAddUml("^", 'y', QChar(0x177));

        fAddUml("v", 'C', QChar(0x10C));
        fAddUml("v", 'c', QChar(0x10D));
        fAddUml("v", 'D', QChar(0x10E));
        fAddUml("v", 'd', QChar(0x10F));
        fAddUml("v", 'E', QChar(0x11A));
        fAddUml("v", 'e', QChar(0x11B));
        fAddUml("v", 'L', QChar(0x13D));
        fAddUml("v", 'l', QChar(0x13E));
        fAddUml("v", 'N', QChar(0x147));
        fAddUml("v", 'n', QChar(0x148));
        fAddUml("v", 'R', QChar(0x158));
        fAddUml("v", 'r', QChar(0x159));
        fAddUml("v", 'S', QChar(0x160));
        fAddUml("v", 's', QChar(0x161));
        fAddUml("v", 'T', QChar(0x164));
        fAddUml("v", 't', QChar(0x165));
        fAddUml("v", 'Z', QChar(0x17D));
        fAddUml("v", 'z', QChar(0x17E));
        fAddUml("v", 'A', QChar(0x1CD));
        fAddUml("v", 'a', QChar(0x1CE));
        fAddUml("v", 'I', QChar(0x1CF));
        fAddUml("v", 'i', QChar(0x1D0));
        fAddUml("v", 'O', QChar(0x1D1));
        fAddUml("v", 'o', QChar(0x1D2));
        fAddUml("v", 'U', QChar(0x1D3));
        fAddUml("v", 'u', QChar(0x1D4));
        fAddUml("v", 'G', QChar(0x1E6));
        fAddUml("v", 'g', QChar(0x1E7));
        fAddUml("v", 'K', QChar(0x1E8));
        fAddUml("v", 'k', QChar(0x1E9));
        fAddUml("v", 'j', QChar(0x1F0));
        fAddUml("v", 'H', QChar(0x21E));
        fAddUml("v", 'h', QChar(0x21F));

        fAddUml("~", 'A', QChar(0xC3));
        fAddUml("~", 'N', QChar(0xD1));
        fAddUml("~", 'O', QChar(0xD5));
        fAddUml("~", 'a', QChar(0xE3));
        fAddUml("~", 'n', QChar(0xF1));
        fAddUml("~", 'o', QChar(0xF5));
        fAddUml("~", 'I', QChar(0x128));
        fAddUml("~", 'i', QChar(0x129));
        fAddUml("~", 'U', QChar(0x168));
        fAddUml("~", 'u', QChar(0x169));

        fAddUml("r", 'A', QChar(0xC5));
        fAddUml("r", 'a', QChar(0xE5));
        fAddUml("r", 'U', QChar(0x16E));
        fAddUml("r", 'u', QChar(0x16F));


        fAddUml("=", 'A', QChar(0xC2));
        fAddUml("=", 'E', QChar(0xCA));
        fAddUml("=", 'I', QChar(0xCE));
        fAddUml("=", 'O', QChar(0xD4));
        fAddUml("=", 'U', QChar(0xDB));
        fAddUml("=", 'a', QChar(0xE2));
        fAddUml("=", 'e', QChar(0xEA));
        fAddUml("=", 'i', QChar(0xEE));
        fAddUml("=", 'o', QChar(0xF4));
        fAddUml("=", 'u', QChar(0xFB));
        fAddUml("=", 'Y', QChar(0x108));
        fAddUml("=", 'y', QChar(0x109));

        fAddUml(".", 'C', QChar(0x10A));
        fAddUml(".", 'c', QChar(0x10B));
        fAddUml(".", 'E', QChar(0x116));
        fAddUml(".", 'e', QChar(0x117));
        fAddUml(".", 'G', QChar(0x120));
        fAddUml(".", 'g', QChar(0x121));
        fAddUml(".", 'I', QChar(0x130));
        fAddUml(".", 'Z', QChar(0x17B));
        fAddUml(".", 'z', QChar(0x17C));
        fAddUml(".", 'A', QChar(0x226));
        fAddUml(".", 'a', QChar(0x227));
        fAddUml(".", 'O', QChar(0x22E));
        fAddUml(".", 'o', QChar(0x22F));
        fAddUml(".", 'B', QChar(0x1E02));
        fAddUml(".", 'b', QChar(0x1E03));
        fAddUml(".", 'D', QChar(0x1E0A));
        fAddUml(".", 'd', QChar(0x1E0B));
        fAddUml(".", 'F', QChar(0x1E1E));
        fAddUml(".", 'f', QChar(0x1E1F));
        fAddUml(".", 'H', QChar(0x1E22));
        fAddUml(".", 'h', QChar(0x1E23));
        fAddUml(".", 'M', QChar(0x1E40));
        fAddUml(".", 'm', QChar(0x1E41));
        fAddUml(".", 'N', QChar(0x1E44));
        fAddUml(".", 'n', QChar(0x1E45));
        fAddUml(".", 'P', QChar(0x1E56));
        fAddUml(".", 'p', QChar(0x1E57));
        fAddUml(".", 'R', QChar(0x1E58));
        fAddUml(".", 'r', QChar(0x1E59));
        fAddUml(".", 'S', QChar(0x1E60));
        fAddUml(".", 's', QChar(0x1E61));
        fAddUml(".", 'T', QChar(0x1E6A));
        fAddUml(".", 't', QChar(0x1E6B));
        fAddUml(".", 'W', QChar(0x1E86));
        fAddUml(".", 'w', QChar(0x1E87));
        fAddUml(".", 'X', QChar(0x1E8A));
        fAddUml(".", 'x', QChar(0x1E8B));
        fAddUml(".", 'Y', QChar(0x1E8E));
        fAddUml(".", 'y', QChar(0x1E8F));

        fAddUml("u", 'A', QChar(0x102));
        fAddUml("u", 'a', QChar(0x103));
        fAddUml("u", 'E', QChar(0x114));
        fAddUml("u", 'e', QChar(0x115));
        fAddUml("u", 'G', QChar(0x11E));
        fAddUml("u", 'g', QChar(0x11F));
        fAddUml("u", 'I', QChar(0x12C));
        fAddUml("u", 'i', QChar(0x12D));
        fAddUml("u", 'O', QChar(0x14E));
        fAddUml("u", 'o', QChar(0x14F));
        fAddUml("u", 'U', QChar(0x16C));
        fAddUml("u", 'u', QChar(0x16D));


        fAddUml("c", 'C', QChar(0xC7));
        fAddUml("c", 'c', QChar(0xE7));
        fAddUml("c", 'G', QChar(0x122));
        fAddUml("c", 'g', QChar(0x123));
        fAddUml("c", 'K', QChar(0x136));
        fAddUml("c", 'k', QChar(0x137));
        fAddUml("c", 'L', QChar(0x13B));
        fAddUml("c", 'l', QChar(0x13C));
        fAddUml("c", 'N', QChar(0x145));
        fAddUml("c", 'n', QChar(0x146));
        fAddUml("c", 'R', QChar(0x156));
        fAddUml("c", 'r', QChar(0x157));
        fAddUml("c", 'S', QChar(0x15E));
        fAddUml("c", 's', QChar(0x15F));
        fAddUml("c", 'T', QChar(0x162));
        fAddUml("c", 't', QChar(0x163));
        fAddUml("c", 'E', QChar(0x228));
        fAddUml("c", 'e', QChar(0x229));
        fAddUml("c", 'D', QChar(0x1E10));
        fAddUml("c", 'd', QChar(0x1E11));
        fAddUml("c", 'H', QChar(0x1E28));
        fAddUml("c", 'h', QChar(0x1E29));

        fAddUml("H", 'O', QChar(0x150));
        fAddUml("H", 'o', QChar(0x151));
        fAddUml("H", 'U', QChar(0x170));
        fAddUml("H", 'u', QChar(0x171));
        fAddUml("H", 'Y', QChar(0x4F2));
        fAddUml("H", 'y', QChar(0x4F3));

        fAddUml("k", 'A', QChar(0x104));
        fAddUml("k", 'a', QChar(0x105));
        fAddUml("k", 'E', QChar(0x118));
        fAddUml("k", 'e', QChar(0x119));
        fAddUml("k", 'I', QChar(0x12E));
        fAddUml("k", 'i', QChar(0x12F));
        fAddUml("k", 'U', QChar(0x172));
        fAddUml("k", 'u', QChar(0x173));
        fAddUml("k", 'O', QChar(0x1EA));
        fAddUml("k", 'u', QChar(0x1EB));

        // ligatures, instructions without {letter}
        fAddUml("ss", QChar(), QChar(0xDF));
        fAddUml("ae", QChar(), QChar(0xE6));
        fAddUml("AE", QChar(), QChar(0xC6));
        fAddUml("oe", QChar(), QChar(0x153));
        fAddUml("OE", QChar(), QChar(0x152));
        fAddUml("o", QChar(), QChar(0xF8));
        fAddUml("O", QChar(), QChar(0xD8));
        fAddUml("S", QChar(), QChar(0xA7));
        fAddUml("l", QChar(), QChar(0x142));
        fAddUml("L", QChar(), QChar(0x141));
        fAddUml("aa", QChar(), QChar(0xE5));
        fAddUml("AA", QChar(), QChar(0xC5));
    }

    //----------------------------------------------------------
    // read an instruction name
    if (c=='\\') {
        //----------------------------------------------------------
        // parsing accent instructions like \ss \"{a} ...
        if (!parsingMathEnvironment){
            for (int len: accentLetters_LenBackslash) {
                const QString acc=parseString.mid(currentTokenID, len);
                if (acc.size()==len && accentLetters.contains(acc)) {
                    currentTokenName=accentLetters[acc];
                    currentTokenID+=acc.trimmed().size()-1; // forward the instruction, omit trailing whitespace in instruction
                    if (acc.endsWith('\\')) currentTokenID--;
                    return currentToken=MTTtext;
                }
            }
        }
        currentTokenID++;
        if (currentTokenID>=parseString.size()-1) return currentToken=MTTnone;
        c=parseString[currentTokenID];
        //----------------------------------------------------------
        // parsing single-character instruction
        if (SingleCharInstructions.contains(c)) {
            currentTokenName=c; // parse one-symbol instructions like \\, \& ...
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTinstruction;
        //----------------------------------------------------------
        // letter-only instruction name
        } else {
            while (c.isLetter()&& (currentTokenID<parseString.size())) {
                currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
            }
            if (!c.isLetter()) currentTokenID--;
            currentTokenName=currentTokenName.trimmed();
        }
        //std::cout<<"found instruction node '"<<currentTokenName.toStdString()<<"'\n";
        if (currentTokenName.size()==0) error_list.append(tr("error @ ch. %1: parser encountered empty istruction").arg(currentTokenID));
        return currentToken=MTTinstruction;
    //----------------------------------------------------------
    // check for $ character
    } else if (c=='$') {
        //std::cout<<"found dollar\n";
        return currentToken=MTTdollar;
    //----------------------------------------------------------
    // check for & character
    } else if (c=='&') {
        //std::cout<<"found ampersand\n";
        return currentToken=MTTampersand;
    //----------------------------------------------------------
    // check for { character
    } else if (c=='{') {
        //----------------------------------------------------------
        // parsing accent instructions like {\ss}
        if (!parsingMathEnvironment){
            for (int len: accentLetters_LenCurly) {
                const QString acc=parseString.mid(currentTokenID, len);
                if (acc.size()==len && accentLetters.contains(acc)) {
                    currentTokenName=accentLetters[acc];
                    currentTokenID+=acc.trimmed().size()-1; // forward the instruction, omit trailing whitespace in instruction
                    if (acc.endsWith('\\')) currentTokenID--;
                    return currentToken=MTTtext;
                }
            }
        }
        return currentToken=MTTopenbrace;
    //----------------------------------------------------------
    // check for } character
    } else if (c=='}') {
        return currentToken=MTTclosebrace;
    //----------------------------------------------------------
    // check for [ character
    } else if (c=='[') {
        return currentToken=MTTopenbracket;
    //----------------------------------------------------------
    // check for ] character
    } else if (c==']') {
        return currentToken=MTTclosebracket;
    //----------------------------------------------------------
    // check for _ character
    } else if (c=='_') {
        return currentToken=MTTunderscore;
    //----------------------------------------------------------
    // check for ^ character
    } else if (c=='^') {
        return currentToken=MTThat;
    //----------------------------------------------------------
    // check for whitespace character
    } else if (c.isSpace()) {
        while (c.isSpace() &&(currentTokenID<parseString.size())) { // eat up whitespace
            currentTokenID++;
            if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
        }
        if (!c.isSpace()) currentTokenID--;
        //std::cout<<"found whitespace\n";
        return currentToken=MTTwhitespace;
    } else {
        //----------------------------------------------------------
        // no instruction or special character => parse text
        if (parsingMathEnvironment) {
            // inside math environments we split texts at every brace {[(|)]} so that
            // braces form their own JKQTMathTextTextNode and may be formated accordingly
            if (mathEnvironmentSpecialChars.contains(c)) {
                currentTokenName=c;
                //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
                return currentToken=MTTtext;
            }
            while (!mathEnvironmentSpecialEndChars.contains(c) && (currentTokenID<parseString.size())) {
                // add whitespaces only once
                if (c.isSpace()) {
                    if (!currentTokenName.isEmpty()) {
                        if (!currentTokenName[currentTokenName.size()-1].isSpace())
                            currentTokenName+=c;
                    }
                } else currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
            }
            if (mathEnvironmentSpecialEndChars.contains(c) || c.isSpace()) currentTokenID--;
            //currentTokenName=currentTokenName.trimmed();
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTtext;
        } else {
            while ((!c.isSpace()) && !TokenCharacters.contains(c) && (currentTokenID<parseString.size())) {
                // add whitespaces only once
                if (c.isSpace()) {
                    if (!currentTokenName.isEmpty()) {
                        if (!currentTokenName[currentTokenName.size()-1].isSpace())
                            currentTokenName+=c;
                    }
                } else currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
            }
            if (TokenCharacters.contains(c) || c.isSpace()) currentTokenID--;
            //currentTokenName=currentTokenName.trimmed();
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTtext;
        }
    }
    return currentToken=MTTnone;
}

JKQTMathTextNode* JKQTMathText::parseLatexString(bool get, JKQTMathTextBraceType quitOnClosingBrace, const QString& quitOnEnvironmentEnd, bool quitOnClosingBracket) {
    //std::cout<<"    entering parseLatexString()\n";
    JKQTMathTextListNode* nl=new JKQTMathTextListNode(this);
    if (get) getToken();

    //----------------------------------------------------------
    // initialize some static sets for easy and fast character lookup
    static QSet<QString> mathEnvironmentSpecialText;
    if (mathEnvironmentSpecialText.size()==0) {
        mathEnvironmentSpecialText<<"+"<<"-"<<"="<<"*"<<"<"<<">"<<"|"<<"/";
    }

    bool getNew=true;
    while (currentToken!=MTTnone) {
        getNew=true;
        if (currentToken==MTTtext) {
            QString text=currentTokenName;
            bool addWhite=false;
            if (!parsingMathEnvironment) {
                getToken();
                getNew=false;
                while (currentToken==MTTtext || currentToken==MTTwhitespace) {
                    if (currentToken==MTTtext) {
                        text+=currentTokenName;
                        getNew=true;
                    } else if (currentToken==MTTwhitespace) {
                        text+=" ";
                        getNew=true;
                    }
                    getToken();
                    getNew=false;
                }
                if (text.size()>0 && text[text.size()-1].isSpace()) {
                    addWhite=true;
                    text=text.left(text.size()-1);
                }
            }
            if (parsingMathEnvironment) {
                if (mathEnvironmentSpecialText.contains(text.trimmed()) && JKQTMathTextSymbolNode::hasSymbol(text.trimmed())) {
                    nl->addChild(new JKQTMathTextSymbolNode(this, text.trimmed()));
                } else {
                    nl->addChild(new JKQTMathTextTextNode(this, text, addWhite, parsingMathEnvironment));
                }
            } else {
                nl->addChild(new JKQTMathTextTextNode(this, text, addWhite, parsingMathEnvironment));
            }
        } else if (currentToken==MTTwhitespace) {
            if (!parsingMathEnvironment) nl->addChild(new JKQTMathTextWhitespaceNode(this));
        } else if (currentToken==MTTinstruction) {
            const QString currentInstructionName=currentTokenName;
            if (currentInstructionName=="\\") break; // break on linebrak character
            if (currentInstructionName=="limits") {
                if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(true);
            } else  if (currentInstructionName=="nolimits") {
                if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(false);
            } else if (currentInstructionName=="begin") {
                if (getToken()==MTTopenbrace && getToken()==MTTtext) {
                    QString envname=currentTokenName;
                    while (currentToken!=MTTclosebrace) getToken(); // find closing brace '}' after '\\begin{name'
                    if (envname=="matrix" || envname=="array" || envname=="aligned" || envname=="align" || envname=="cases" || envname=="pmatrix"|| envname=="bmatrix"|| envname=="Bmatrix"|| envname=="vmatrix"|| envname=="Vmatrix") {
                        QVector< QVector<JKQTMathTextNode*> > items;
                        //int lines=0;
                        //int cols=0;
                        bool first=true;
                        QVector<JKQTMathTextNode*> line;
                        //std::cout<<"found \\begin{matrix}\n";
                        while (first || currentToken==MTTampersand || (currentToken==MTTinstruction && currentTokenName=="\\")) {
                            JKQTMathTextNode* it=parseLatexString(true, MTBTAny, envname);
                            if (currentToken==MTTampersand) {
                                //std::cout<<"  appending item\n";
                                line.append(it);
                            } else {
                                line.append(it);
                                //std::cout<<"  appending item and line with "<<line.size()<<" items.\n";
                                items.append(line);
                                line.clear();
                            }
                            first=false;
                        }
                        //std::cout<<"  creating matrix-node with "<<items.size()<<" items.\n";
                        if (envname=="pmatrix") nl->addChild(new JKQTMathTextBraceNode(this, MTBTParenthesis, MTBTParenthesis, new JKQTMathTextMatrixNode(this, items)));
                        else if (envname=="cases") nl->addChild(new JKQTMathTextBraceNode(this, MTBTCurlyBracket, MTBTNone, new JKQTMathTextMatrixNode(this, items)));
                        else if (envname=="bmatrix") nl->addChild(new JKQTMathTextBraceNode(this, MTBTSquareBracket, MTBTSquareBracket, new JKQTMathTextMatrixNode(this, items)));
                        else if (envname=="Bmatrix") nl->addChild(new JKQTMathTextBraceNode(this, MTBTCurlyBracket, MTBTCurlyBracket, new JKQTMathTextMatrixNode(this, items)));
                        else if (envname=="vmatrix") nl->addChild(new JKQTMathTextBraceNode(this, MTBTSingleLine, MTBTSingleLine, new JKQTMathTextMatrixNode(this, items)));
                        else if (envname=="Vmatrix") nl->addChild(new JKQTMathTextBraceNode(this, MTBTDoubleLine, MTBTDoubleLine, new JKQTMathTextMatrixNode(this, items)));
                        else nl->addChild(new JKQTMathTextMatrixNode(this, items));
                        //std::cout<<"  creating matrix-node ... done!\n";
                    } else {
                        error_list.append(tr("error @ ch. %1: unknown environment '%2'").arg(currentTokenID).arg(envname));
                    }
                } else { // find next '}'
                    error_list.append(tr("error @ ch. %1: text after '\\begin{' expected!").arg(currentTokenID));
                    while (currentToken!=MTTclosebrace) getToken();
                    getNew=true;
                }
            } else if (currentInstructionName=="end") {
                if (getToken()==MTTopenbrace && getToken()==MTTtext) {
                    QString envname=currentTokenName;
                    while (currentToken!=MTTclosebrace) getToken(); // find closing brace '}' after '\\begin{name'
                    if (envname==quitOnEnvironmentEnd) {
                        break;
                    } else {
                        error_list.append(tr("error @ ch. %1: '\\end{%2}' widthout preceding '\\begin{%3}'").arg(currentTokenID).arg(envname).arg(envname));
                    }
                } else { // find next '}'
                    error_list.append(tr("error @ ch. %1: text after '\\begin{' expected!").arg(currentTokenID));
                    while (currentToken!=MTTclosebrace) getToken();
                    getNew=true;
                }
            } else if (currentInstructionName=="right") {
                getToken();
                if (currentToken==MTTtext) {
                    if (currentTokenName.size()>0) {
                        bool tokenWasNoBrace=false;
                        const QString firstTokenChar(currentTokenName[0]);
                        if (TokenNameMatchesJKQTMathTextBraceType(firstTokenChar, quitOnClosingBrace, true, &tokenWasNoBrace)) {
                            lastRightBraceType=TokenName2JKQTMathTextBraceType(firstTokenChar);
                            if (quitOnClosingBrace!=MTBTAny) currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                            break;
                        } else {
                            getNew=false;
                        }
                    }
                } else if (currentToken==MTTinstruction) {
                    if (InstructionNameMatchesJKQTMathTextBraceType(currentTokenName, quitOnClosingBrace, true)) {
                        lastRightBraceType=InstructionName2JKQTMathTextBraceType(currentTokenName);
                        break;
                    }
                } else if (currentToken==MTTclosebracket) {
                    if (quitOnClosingBrace==MTBTSquareBracket || quitOnClosingBrace==MTBTAny) {
                        lastRightBraceType=MTBTSquareBracket;
                        break;
                    }
                } else {
                    error_list.append(tr("error @ ch. %1: unexpected token after \\left").arg(currentTokenID));
                }
            } else if (currentInstructionName=="left") {
                getToken();
                if (currentToken==MTTtext) {
                    if (currentTokenName.size()>0) {
                        const QString firstTokenChar(currentTokenName[0]);
                        const JKQTMathTextBraceType bracetype=TokenName2JKQTMathTextBraceType(firstTokenChar);
                        if (bracetype==MTBTNone) {
                            currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                            JKQTMathTextNode* cn=parseLatexString(currentTokenName.size()<=0, MTBTAny);
                            nl->addChild(new JKQTMathTextBraceNode(this, MTBTNone, bracetype, cn));
                        } else if (isPrintableJKQTMathTextBraceType(bracetype)) {
                            currentTokenName=currentTokenName.right(currentTokenName.size()-1); // we already used the first character from the text token!
                            JKQTMathTextNode* c=parseLatexString(currentTokenName.size()<=0, bracetype);
                            nl->addChild(new JKQTMathTextBraceNode(this, bracetype, lastRightBraceType, c));
                        } else {
                            getNew=false;
                        }
                    }
                } else if (currentToken==MTTinstruction) {
                    const JKQTMathTextBraceType bracetypeopening=InstructionName2OpeningJKQTMathTextBraceType(currentTokenName);
                    if (bracetypeopening!=MTBTUnknown) {
                        JKQTMathTextNode* c=parseLatexString(true, bracetypeopening);
                        nl->addChild(new JKQTMathTextBraceNode(this, bracetypeopening, lastRightBraceType, c));
                    } else if (currentToken==MTTinstruction && TokenNameMatchesJKQTMathTextBraceType(currentTokenName, quitOnClosingBrace, true)) {
                        break;
                    }
                } else if (currentToken==MTTopenbracket) {
                    JKQTMathTextNode* c=parseLatexString(true, MTBTSquareBracket);
                    nl->addChild(new JKQTMathTextBraceNode(this, MTBTSquareBracket, lastRightBraceType, c));
                } else {
                    error_list.append(tr("error @ ch. %1: unexpected token after \\left").arg(currentTokenID));
                }
            } else {
                bool foundError=false;
                JKQTMathTextNode* node=parseInstruction(&foundError, &getNew);
                if (node) {
                    if (foundError) {
                        delete node;
                        node=nullptr;
                    } else {
                        nl->addChild(node);
                    }
                }
            }
        } else if (currentToken==MTTunderscore) {
            getToken();
            JKQTMathTextNode* child=nullptr;
            JKQTMathTextNode* child2=nullptr;
            if (currentToken==MTTinstruction) {
                child=parseInstruction(nullptr, &getNew);
            } else if (currentToken==MTTopenbrace) {
                child=parseLatexString(true);
            } else if (currentToken==MTTtext) {
                if (currentTokenName.size()<=1) {
                    child=new JKQTMathTextTextNode(this, currentTokenName, false, parsingMathEnvironment);
                } else {
                    child=new JKQTMathTextTextNode(this, QString(currentTokenName[0]), false, parsingMathEnvironment);
                    child2=new JKQTMathTextTextNode(this, currentTokenName.right(currentTokenName.size()-1), false, parsingMathEnvironment);
                }
            } else {
                getNew=false;
            }
            if (child!=nullptr) nl->addChild(new JKQTMathTextSubscriptNode(this, child));
            if (child2!=nullptr) nl->addChild(child2);
        } else if (currentToken==MTThat) {
            getToken();
            JKQTMathTextNode* child=nullptr;
            JKQTMathTextNode* child2=nullptr;
            if (currentToken==MTTinstruction) {
                child=parseInstruction(nullptr, &getNew);
            } else if (currentToken==MTTopenbrace) {
                child=parseLatexString(true);
            } else if (currentToken==MTTtext) {
                if (currentTokenName.size()<=1) {
                    child=new JKQTMathTextTextNode(this, currentTokenName, false, parsingMathEnvironment);
                } else {
                    child=new JKQTMathTextTextNode(this, QString(currentTokenName[0]), false, parsingMathEnvironment);
                    child2=new JKQTMathTextTextNode(this, currentTokenName.right(currentTokenName.size()-1), false, parsingMathEnvironment);
                }
            } else {
                getNew=false;
            }
            if (child!=nullptr) nl->addChild(new JKQTMathTextSuperscriptNode(this, child));
            if (child2!=nullptr) nl->addChild(child2);
        } else if (currentToken==MTTopenbrace) {
            nl->addChild(parseLatexString(true));
        } else if (currentToken==MTTclosebrace) {
            break;
        } else if (currentToken==MTTopenbracket) {
            nl->addChild(new JKQTMathTextTextNode(this, "[", false));
        } else if (currentToken==MTTclosebracket) {
            if (quitOnClosingBracket) break;
            else nl->addChild(new JKQTMathTextTextNode(this, "]", false));
        } else if (currentToken==MTTampersand) {
            break;
        } else if (currentToken==MTTdollar) {
            if (parsingMathEnvironment) { // reached end of math environment
                parsingMathEnvironment=false;
                break;
            } else { // starting math environment
                parsingMathEnvironment=true;
                nl->addChild(new JKQTMathTextModifiedTextPropsInstructionNode(this, "equation", parseLatexString(true)));
            }
        }
        if (getNew) getToken();
    }
    //std::cout<<"    leaving parseLatexString()\n";
    return simplifyJKQTMathTextNode(nl);
}

JKQTMathTextNode* JKQTMathText::parseInstruction(bool *_foundError, bool* getNew) {
    if (currentToken!=MTTinstruction) {
        if (_foundError) *_foundError=true;
        if (getNew) *getNew=false;
        error_list.append(tr("error @ ch. %1: expected instruction token").arg(currentTokenID));
        return nullptr;
    }
    bool foundError=false;
    const QString currentInstructionName=currentTokenName;
    JKQTMathTextNode* child=nullptr;
    if (getNew) *getNew=true;
    if (JKQTMathTextWhitespaceNode::supportsInstructionName(currentInstructionName)) {
        if (getNew) *getNew=true;
        child= new JKQTMathTextWhitespaceNode(currentInstructionName, this);
    } else if (JKQTMathTextSymbolNode::hasSymbol(currentInstructionName)) {
        child=new JKQTMathTextSymbolNode(this, currentInstructionName);
        if (JKQTMathTextSymbolNode::isSubSuperscriptBelowAboveSymbol(currentInstructionName) && parsingMathEnvironment) {
            child->setSubSuperscriptAboveBelowNode(true);
        }
        if (getNew) *getNew=true;
    } else if (JKQTMathTextModifiedTextPropsInstructionNode::supportsInstructionName(currentInstructionName)) {
        const size_t Nparams=JKQTMathTextModifiedTextPropsInstructionNode::countParametersOfInstruction(currentInstructionName);
        bool foundError=false;
        const QStringList params=parseStringParams(true, Nparams, &foundError);
        if (!foundError) {
            if (getToken()==MTTopenbrace) {
                const bool oldParseMath=parsingMathEnvironment;
                auto __finalpaint=JKQTPFinally(std::bind([&oldParseMath](bool& parsingMathEnvironment) { parsingMathEnvironment=oldParseMath; }, std::ref(parsingMathEnvironment)));
                JKQTMathTextModifiedTextPropsInstructionNode::modifyInMathEnvironment(currentInstructionName, parsingMathEnvironment, params);
                child=new JKQTMathTextModifiedTextPropsInstructionNode(this, currentInstructionName, parseLatexString(true), params);

            } else {
                foundError=true;
            }
        }
        if (foundError){
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(Nparams+1));
        }
    } else if (JKQTMathTextBoxInstructionNode::supportsInstructionName(currentInstructionName)) {
        const size_t Nparams=JKQTMathTextBoxInstructionNode::countParametersOfInstruction(currentInstructionName);
        bool foundError=false;
        const QStringList params=parseStringParams(true, Nparams, &foundError);
        if (!foundError) {
            if (getNew) *getNew=true;
            if (getToken()==MTTopenbrace) {
                const bool oldParseMath=parsingMathEnvironment;
                auto __finalpaint=JKQTPFinally(std::bind([&oldParseMath](bool& parsingMathEnvironment) { parsingMathEnvironment=oldParseMath; }, std::ref(parsingMathEnvironment)));
                JKQTMathTextBoxInstructionNode::modifyInMathEnvironment(currentInstructionName, parsingMathEnvironment, params);
                child=new JKQTMathTextBoxInstructionNode(this, currentInstructionName, parseLatexString(true), params);
            } else {
                foundError=true;
            }
        }
        if (foundError){
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(Nparams+1));
        }
    } else if (currentInstructionName=="sqrt") {
        getToken();
        if (currentToken==MTTopenbrace) {
            child=new JKQTMathTextSqrtNode(this, parseLatexString(true));
        } else if (currentToken==MTTopenbracket) {
            JKQTMathTextNode* n1=parseLatexString(true, MTBTAny, "", true);
            JKQTMathTextNode* n2=nullptr;
            if (getToken()==MTTopenbrace) n2=parseLatexString(true);
            else error_list.append(tr("error @ ch. %1: expected one argument in '{' braces after '%2' command with an optional argument in []").arg(currentTokenID).arg(currentInstructionName));

            if (n1 && n2) {
                child=new JKQTMathTextSqrtNode(this, n2, n1);
            } else {
                if (n1) delete n1;
                if (n2) delete n2;
                error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
            }
        } else {
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (currentInstructionName=="cbrt") {
        if (getToken()==MTTopenbrace) {
            child=new JKQTMathTextSqrtNode(this, parseLatexString(true), new JKQTMathTextTextNode(this, "3", false));
        } else {
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (currentInstructionName=="verb") {
        if (getToken()==MTTopenbrace) {
            QString text="";
            currentTokenID++;
            if (currentTokenID<=parseString.size()-1) {
                QChar c=parseString[currentTokenID];
                while (c!='}' && (currentTokenID<parseString.size())) {
                    text=text+c;
                    currentTokenID++;
                    if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
                }
                if (c!='}')  error_list.append(tr("error @ ch. %1: \verb{...} not closed by '}'").arg(currentTokenID).arg(currentInstructionName));
                child=new JKQTMathTextTextNode(this, text, false);
            }
        } else {
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (JKQTMathTextDecoratedNode::supportsInstructionName(currentInstructionName)) {
        if (getToken()==MTTopenbrace) {
            child=new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::InstructionName2DecorationType(currentInstructionName), parseLatexString(true));
        } else {
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (JKQTMathTextFracNode::supportsInstructionName(currentInstructionName)) {
        if (getToken()==MTTopenbrace) {
            JKQTMathTextNode* n1=parseLatexString(true);
            JKQTMathTextNode* n2=nullptr;
            if (getToken()==MTTopenbrace) n2=parseLatexString(true);
            if (n1 && n2) child=new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::InstructionName2FracType(currentInstructionName));
            else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));

        } else {
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (currentInstructionName=="binom") {
        if (getToken()==MTTopenbrace) {
            JKQTMathTextNode* n1=parseLatexString(true);
            JKQTMathTextNode* n2=nullptr;
            if (getToken()==MTTopenbrace) n2=parseLatexString(true);
            if (n1 && n2) child=new JKQTMathTextBraceNode(this, MTBTParenthesis, MTBTParenthesis, new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMstackrel));
            else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
        } else {
            error_list.append(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else {
        if (getNew) *getNew=true;
        foundError=true;
        error_list.append(tr("error @ ch. %1: unknown instruction \\%2 or unsupported instruction at this location").arg(currentTokenID).arg(currentInstructionName));
    }
    if (_foundError) *_foundError=foundError;
    return child;
}

QStringList JKQTMathText::parseStringParams(bool get, size_t Nparams, bool *foundError) {
    const bool old_parsingMathEnvironment=parsingMathEnvironment;
    auto reset_parsingMathEnvironment=JKQTPFinally([&]() { parsingMathEnvironment=old_parsingMathEnvironment; });
    if (*foundError) *foundError=false;
    if (Nparams<=0) return QStringList();
    else {
        QStringList params;
        for (size_t n=0; n<Nparams; n++) {
            if (n>0 || (n==0 && get)) getToken();
            if (currentToken==MTTopenbrace) {
                bool ok=true;
                QString thisparam="";
                while (ok) {
                    getToken();
                    if (currentToken==MTTtext) {
                        thisparam+=currentTokenName;
                    } else if (currentToken==MTTwhitespace) {
                        thisparam+=" ";
                    } else if (currentToken==MTTclosebrace) {
                        params.append(thisparam);
                        ok=false;
                    } else {
                        if (*foundError) *foundError=true;
                        return params;
                    }
                }
            } else {
                if (*foundError) *foundError=true;
                return params;
            }
        }
        return params;
    }
}


JKQTMathTextNode *JKQTMathText::getParsedNode() const {
    return this->parsedNode;
}





bool JKQTMathText::parse(const QString& text, bool addSpaceBeforeAndAfter){
    QString ntext;
    if (addSpaceBeforeAndAfter) ntext=QString("\\;")+text+QString("\\;");
    else ntext=text;
    if (parsedNode && parseString==ntext) return true;


    if (parsedNode!=nullptr) delete parsedNode;
    if (unparsedNode!=nullptr) delete unparsedNode;
    parseString=ntext;

    currentTokenID=-1;
    currentToken=MTTnone;
    currentTokenName="";
    parsingMathEnvironment=false;
    error_list.clear();
    parsedNode=parseLatexString(true);
    unparsedNode=new MTplainTextNode(this, text, false);
    return (parsedNode!=nullptr);
}


QSizeF JKQTMathText::getSize(QPainter& painter){
    if (getNodeTree()!=nullptr) {
        double w=0, a=0, d=0, s=0;
        getSizeDetail(painter, w, a, d, s);
        return QSizeF(w, a+d);
    }
    return QSizeF(0,0);
}

double JKQTMathText::getDescent(QPainter& painter) {
    double w=0, a=0, d=0, s=0;
    getSizeDetail(painter, w, a, d, s);
    return d;
}

double JKQTMathText::getAscent(QPainter& painter) {
    double w=0, a=0, d=0, s=0;
    getSizeDetail(painter, w, a, d, s);
    return a;
}

void JKQTMathText::getSizeDetail(QPainter& painter, double& width, double& ascent, double& descent, double& strikeoutPos) {
    width=0;
    ascent=0;
    descent=0;
    strikeoutPos=0;
    if (getNodeTree()!=nullptr) {
        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        ev.fontSizeUnit=fontSizeUnits;

        double overallHeight=0;        
        getNodeTree()->getSize(painter, ev, width, ascent, overallHeight, strikeoutPos);
        descent=overallHeight-ascent;
        ascent=ascent*1.1;
        descent=qMax(ascent*0.1, descent*1.1);
        strikeoutPos=strikeoutPos*1.1;
    }
}

void JKQTMathText::draw(QPainter &painter, QPointF x, bool drawBoxes)
{
    draw(painter, x.x(), x.y(), drawBoxes);
}

void JKQTMathText::draw(QPainter& painter, double x, double y, bool drawBoxes){
    if (getNodeTree()!=nullptr) {
        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        ev.fontSizeUnit=fontSizeUnits;
        QPen pp=painter.pen();
        QPen p=pp;
        p.setStyle(Qt::SolidLine);
        painter.setPen(p);
        getNodeTree()->setDrawBoxes(drawBoxes);
        painter.setPen(p);
        getNodeTree()->draw(painter, x, y, ev);
        painter.setPen(pp);
    }
}

void JKQTMathText::draw(QPainter& painter, unsigned int flags, QRectF rect, bool drawBoxes) {
    if (getNodeTree()!=nullptr) {
        QPen pp=painter.pen();
        QPen p=pp;
        p.setStyle(Qt::SolidLine);
        painter.setPen(p);
        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        ev.fontSizeUnit=fontSizeUnits;
        getNodeTree()->setDrawBoxes(drawBoxes);
        painter.setPen(p);

        double width=0;
        double baselineHeight=0;
        double overallHeight=0, strikeoutPos=0;
        getNodeTree()->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);

        // align left top
        double x=rect.left();
        double y=rect.top()+baselineHeight;

        // care for horizontal align
        if ((flags & Qt::AlignRight) != 0) x=x+rect.width()-width;
        else if ((flags & Qt::AlignHCenter) != 0) x=x+(rect.width()-width)/2.0;

        // care for vertical align
        if ((flags & Qt::AlignBottom) != 0) y=y+rect.height()-overallHeight;
        else if ((flags & Qt::AlignVCenter) != 0) y=y+(rect.height()-overallHeight)/2.0;

        // finally draw
        getNodeTree()->draw(painter, x, y, ev);
        painter.setPen(pp);
    }
}


JKQTMathTextNode *JKQTMathText::getNodeTree() const {
    if (useUnparsed) return unparsedNode;
    return parsedNode;
}





