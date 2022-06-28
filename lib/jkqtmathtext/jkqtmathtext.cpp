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
    decoration_height_factor=0.2;
    decoration_width_reduction_Xfactor=0.2;

    operatorsubsuper_size_factor=0.65;
    operatorsubsuper_distance_factor=0.25;
    operatorsubsuper_extraspace_factor=0.5;
    intsubsuper_xcorrection_factor=0.25;
    intsubbesides_xcorrection_xfactor=0.33;
    mathoperator_width_factor=1.5;
    bigmathoperator_font_factor=1.8;

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
        //qDebug()<<"fonts:\n"<<fonts;

        /*if (SCAN_FONTS_ON_STARTUP) {
            for (const QString& f: fonts) {
                QFont fnt(f);
                QFontInfo fi(fnt);
                if (typewriterFont=="typewriter" && fi.styleHint()==QFont::TypeWriter) {
                    typewriterFont=f;
                }
                if (decorativeFont=="decorative" && fi.styleHint()==QFont::Decorative) {
                    decorativeFont=f;
                }
                if (serifFont=="serif" && fi.styleHint()==QFont::Serif) {
                    serifFont=f;
                }
                if (sansFont=="sans" && fi.styleHint()==QFont::SansSerif) {
                    sansFont=f;
                }
                if (scriptFont=="script" && fi.styleHint()==QFont::Cursive) {
                    scriptFont=f;
                }
            }
        }*/
        //qDebug()<<"iterate "<<fonts.size()<<" fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();


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
    setFontSans(sansFont, MTFEStandard);
    setFontMathSans(sansFont, MTFEStandard);
    setFontTypewriter(typewriterFont, MTFEStandard);
    setFontRoman(serifFont, MTFEStandard);
    setFontMathRoman(serifFont, MTFEStandard);
    setFontCaligraphic(decorativeFont, MTFEStandard);
    setFontBlackboard(blackboardFont, MTFEStandard);
    setFontBlackboardSimulated(blackboardFont=="blackboard");
    setFontScript(scriptFont, MTFEStandard);
    setFontFraktur(fracturFont, MTFEStandard);
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
    underset_factor=settings.value(group+"undersetFactor", underset_factor).toDouble();
    brace_y_shift_factor=settings.value(group+"brace_y_shift_factor", brace_y_shift_factor).toDouble();
    decoration_height_factor=settings.value(group+"decoration_height_factor", decoration_height_factor).toDouble();
    decoration_width_reduction_Xfactor=settings.value(group+"decoration_width_reduction_xfactor", decoration_width_reduction_Xfactor).toDouble();
    operatorsubsuper_size_factor=settings.value(group+"operatorsubsuper_size_factor", operatorsubsuper_size_factor).toDouble();
    operatorsubsuper_distance_factor=settings.value(group+"operatorsubsuper_distance_factor", operatorsubsuper_distance_factor).toDouble();
    operatorsubsuper_extraspace_factor=settings.value(group+"operatorsubsuper_extraspace_factor", operatorsubsuper_extraspace_factor).toDouble();
    mathoperator_width_factor=settings.value(group+"mathoperator_width_factor", mathoperator_width_factor).toDouble();
    intsubsuper_xcorrection_factor=settings.value(group+"intsubsuper_xcorrection_factor", intsubsuper_xcorrection_factor).toDouble();
    intsubbesides_xcorrection_xfactor=settings.value(group+"intsubbesides_xcorrection_xfactor", intsubbesides_xcorrection_xfactor).toDouble();


    if (settings.value(group+"use_stix_fonts", false).toBool()) useSTIX();
    if (settings.value(group+"use_xits_fonts", false).toBool()) useXITS();
    if (settings.value(group+"use_asana_fonts", false).toBool()) useASANA();
}


void JKQTMathText::saveSettings(QSettings& settings, const QString& group) const{
    settings.setValue(group+"font_size", fontSize);
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
    settings.setValue(group+ "undersetFactor", underset_factor);
    settings.setValue(group+ "operatorsubsuper_size_factor", operatorsubsuper_size_factor);
    settings.setValue(group+ "operatorsubsuper_distance_factor", operatorsubsuper_distance_factor);
    settings.setValue(group+ "operatorsubsuper_extraspace_factor", operatorsubsuper_extraspace_factor);
    settings.setValue(group+ "mathoperator_width_factor", mathoperator_width_factor);
    settings.setValue(group+ "intsubsuper_xcorrection_factor", intsubsuper_xcorrection_factor);
    settings.setValue(group+ "intsubbesides_xcorrection_xfactor", intsubbesides_xcorrection_xfactor);
    settings.setValue(group+ "brace_y_shift_factor", brace_y_shift_factor);
    settings.setValue(group+ "decoration_height_factor", decoration_height_factor);
    settings.setValue(group+ "decoration_width_reduction_xfactor", decoration_width_reduction_Xfactor);
}

bool JKQTMathText::useSTIX(bool mathModeOnly) {

    const JKQTMathTextFontSpecifier xits=JKQTMathTextFontSpecifier::getSTIXFamilies();
    bool res=false;

    if (!mathModeOnly && !xits.fontName().isEmpty()) {
        setFontRoman(xits.fontName(), MTFEunicode);
        res=true;
    }
    if (!xits.mathFontName().isEmpty()) {
        setFontMathRoman(xits.mathFontName(), MTFEunicode);
        res=true;
    } else if (!xits.fontName().isEmpty()) {
        setFontMathRoman(xits.fontName(), MTFEunicode);
        res=true;
    }

    return res;
}

bool JKQTMathText::useXITS(bool mathModeOnly)
{

    const JKQTMathTextFontSpecifier xits=JKQTMathTextFontSpecifier::getXITSFamilies();
    bool res=false;

    if (!mathModeOnly && !xits.fontName().isEmpty()) {
        setFontRoman(xits.fontName(), MTFEunicode);
        setSymbolfontSymbol(xits.fontName(), MTFEunicode);
        setSymbolfontGreek(xits.fontName(), MTFEunicode);
        res=true;
    }
    if (!xits.mathFontName().isEmpty()) {
        setFontMathRoman(xits.mathFontName(), MTFEunicode);
        setSymbolfontSymbol(xits.fontName(), MTFEunicode);
        setSymbolfontGreek(xits.fontName(), MTFEunicode);
        res=true;
    }

    return res;
}

bool JKQTMathText::useASANA(bool mathModeOnly)
{

    const JKQTMathTextFontSpecifier asana=JKQTMathTextFontSpecifier::getXITSFamilies();
    bool res=false;

    if (!mathModeOnly && !asana.fontName().isEmpty()) {
        setFontRoman(asana.fontName(), MTFEunicode);
        res=true;
    }
    if (!asana.mathFontName().isEmpty()) {
        setFontMathRoman(asana.mathFontName(), MTFEunicode);
        res=true;
    }

    return res;
}

void JKQTMathText::useAnyUnicode(QString timesFont, const QString &sansFont, JKQTMathTextFontEncoding encodingTimes, JKQTMathTextFontEncoding encodingSans)
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

        JKQTMathTextEnvironment defaultev;
        defaultev.fontSize=fontPointSize;

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
    this->fontSize = __value;
}

double JKQTMathText::getFontSize() const
{
    return this->fontSize;
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

QPair<QString, JKQTMathTextFontEncoding> JKQTMathText::getFontData(JKQTMathTextEnvironmentFont font, bool in_math_environment, FontSubclass subclass) const
{
    if (in_math_environment) {
        if (font==MTEroman) font=MTEmathRoman;
        if (font==MTEsans) font=MTEmathSans;
    }
    const auto fd=fontDefinitions.value(font);
    if (subclass==FontSubclass::Greek) return QPair<QString, JKQTMathTextFontEncoding>(fd.symbolfontGreek, fd.symbolfontGreekEncoding);
    if (subclass==FontSubclass::Symbols) return QPair<QString, JKQTMathTextFontEncoding>(fd.symbolfontSymbol, fd.symbolfontSymbolEncoding);
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
void JKQTMathText::setSymbolfontGreek(JKQTMathTextEnvironmentFont font, const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[font].symbolfontGreek = f.first;
    fontDefinitions[font].symbolfontGreekEncoding = f.second;
}

void JKQTMathText::setSymbolfontGreek(const QString &fontName, JKQTMathTextFontEncoding encoding)
{
    for (int f=0; f<static_cast<int>(MTenvironmentFontCount); f++) {
        setSymbolfontGreek(static_cast<JKQTMathTextEnvironmentFont>(f), fontName, encoding);
    }
}

QString JKQTMathText::getSymbolfontGreek(JKQTMathTextEnvironmentFont font) const
{
    return fontDefinitions[font].symbolfontGreek;
}

JKQTMathTextFontEncoding JKQTMathText::getSymbolfontEncodingGreek(JKQTMathTextEnvironmentFont font) const
{
    return fontDefinitions[font].symbolfontGreekEncoding;
}

void JKQTMathText::setSymbolfontSymbol(JKQTMathTextEnvironmentFont font, const QString &__value, JKQTMathTextFontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[font].symbolfontSymbol = f.first;
    fontDefinitions[font].symbolfontSymbolEncoding = f.second;
}

void JKQTMathText::setSymbolfontSymbol(const QString &fontName, JKQTMathTextFontEncoding encoding)
{
    for (int f=0; f<static_cast<int>(MTenvironmentFontCount); f++) {
        setSymbolfontSymbol(static_cast<JKQTMathTextEnvironmentFont>(f), fontName, encoding);
    }
}

QString JKQTMathText::getSymbolfontSymbol(JKQTMathTextEnvironmentFont font) const
{
    return fontDefinitions[font].symbolfontSymbol;
}

JKQTMathTextFontEncoding JKQTMathText::getSymbolfontEncodingSymbol(JKQTMathTextEnvironmentFont font) const
{
    return fontDefinitions[font].symbolfontSymbolEncoding;
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

void JKQTMathText::setDecorationWidthReductionXFactor(double __value)
{
    decoration_width_reduction_Xfactor=__value;
}

double JKQTMathText::getDecorationWidthReductionXFactor() const
{
    return decoration_width_reduction_Xfactor;
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
    if (TokenCharacters.size()==0) {
        mathEnvironmentSpecialChars<<'(' << '[' << '|' << ')' << ']' << '+' << '-' << '*' << '/' << '<' << '>' << '=';
        mathEnvironmentSpecialEndChars<<'(' << '&' << '[' << '|' << ')' << ']' << '\\' << '$' << '{' << '}' << '_' << '^' << '+' << '-' << '/' << '*' << '=' << '<' << '>';
        TokenCharacters<<'_'<<'^'<<'\\'<<'$'<<'&'<<'}'<<'{'<<'['<<']';
        SingleCharInstructions<<'|'<<';'<<':'<<'!'<<','<<'_'<<'\\'<<'$'<<'%'<<'&'<<'#'<<'}'<<'{'<<' '<<'['<<']';
    }

    //----------------------------------------------------------
    // read an instruction name
    if (c=='\\') {
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
        //std::cout<<"found openbrace\n";
        return currentToken=MTTopenbrace;
    //----------------------------------------------------------
    // check for } character
    } else if (c=='}') {
        //std::cout<<"found closebrace\n";
        return currentToken=MTTclosebrace;
    //----------------------------------------------------------
    // check for [ character
    } else if (c=='[') {
        //std::cout<<"found openbracket\n";
        return currentToken=MTTopenbracket;
    //----------------------------------------------------------
    // check for ] character
    } else if (c==']') {
        //std::cout<<"found closebracket\n";
        return currentToken=MTTclosebracket;
    //----------------------------------------------------------
    // check for _ character
    } else if (c=='_') {
        //std::cout<<"found underscore\n";
        return currentToken=MTTunderscore;
    //----------------------------------------------------------
    // check for ^ character
    } else if (c=='^') {
        //std::cout<<"found hat\n";
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
            bool addWhite=(getToken()==MTTwhitespace) && (!parsingMathEnvironment);
            getNew=addWhite;
            if (parsingMathEnvironment) {
                if (mathEnvironmentSpecialText.contains(text.trimmed())) {
                    nl->addChild(new JKQTMathTextSymbolNode(this, text, addWhite));
                } else {
                    nl->addChild(new JKQTMathTextTextNode(this, text, addWhite, parsingMathEnvironment));
                }
            } else {
                nl->addChild(new JKQTMathTextTextNode(this, text, addWhite, parsingMathEnvironment));
            }
        } else if (currentToken==MTTinstruction) {
            const QString currentInstructionName=currentTokenName;
            if (currentInstructionName=="\\") break; // break on linebrak character
            getToken(); // look at next token
            if (currentToken==MTTopenbrace) {
                //std::cout<<"found '{' after '"<<name.toStdString()<<"'\n";
                if (currentInstructionName=="sqrt") {
                    nl->addChild(new JKQTMathTextSqrtNode(this, parseLatexString(true)));
                } else if (currentInstructionName=="cbrt") {
                    nl->addChild(new JKQTMathTextSqrtNode(this, parseLatexString(true), 3));
                } else if (currentInstructionName=="verb") {
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
                        nl->addChild(new JKQTMathTextTextNode(this, text, false));
                    }
                } else if (currentInstructionName=="frac") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="dfrac" || currentInstructionName=="cfrac") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMdfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="sfrac" || currentInstructionName=="slantfrac" || currentInstructionName=="xfrac") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMsfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="stfrac" || currentInstructionName=="nicefrac" || currentInstructionName=="slanttextfrac" || currentInstructionName=="xtfrac") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMstfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="tfrac") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMtfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="stackrel") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMstackrel));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="binom") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextBraceNode(this, MTBTParenthesis, MTBTParenthesis, new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMstackrel)));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="underbrace") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMunderbrace));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="underbracket") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMunderbracket));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="underset") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMunderset));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="overbrace") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMoverbrace));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="overbracket") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMoverbracket));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="overset") {
                    JKQTMathTextNode* n1=parseLatexString(true);
                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addChild(new JKQTMathTextFracNode(this, n1, n2, JKQTMathTextFracNode::MTFMoverset));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else if (currentInstructionName=="begin") {
                    if (getToken()==MTTtext) {
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
                    if (getToken()==MTTtext) {
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
                } else if (currentInstructionName=="vec") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDvec, parseLatexString(true)));
                } else if (currentInstructionName=="overline"||currentInstructionName=="oline"||currentInstructionName=="ol") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDoverline, parseLatexString(true)));
                } else if (currentInstructionName=="underline"||currentInstructionName=="uline"||currentInstructionName=="ul") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDunderline, parseLatexString(true)));
                } else if (currentInstructionName=="uuline"||currentInstructionName=="uul") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDdoubleunderline, parseLatexString(true)));
                } else if (currentInstructionName=="ooline"||currentInstructionName=="ool") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDdoubleoverline, parseLatexString(true)));
                } else if (currentInstructionName=="arrow"||currentInstructionName=="overrightarrow"||currentInstructionName=="overarrow") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDarrow, parseLatexString(true)));
                } else if (currentInstructionName=="hat" || currentInstructionName=="^") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDhat, parseLatexString(true)));
                } else if (currentInstructionName=="widehat") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDwidehat, parseLatexString(true)));
                } else if (currentInstructionName=="check" || currentInstructionName=="v") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDcheck, parseLatexString(true)));
                } else if (currentInstructionName=="widecheck") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDwidecheck, parseLatexString(true)));
                } else if (currentInstructionName=="bar") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDbar, parseLatexString(true)));
                } else if (currentInstructionName=="dot" || currentInstructionName==".") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDdot, parseLatexString(true)));
                } else if (currentInstructionName=="ocirc") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDocirc, parseLatexString(true)));
                } else if (currentInstructionName=="tilde" || currentInstructionName=="~") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDtilde, parseLatexString(true)));
                } else if (currentInstructionName=="breve" || currentInstructionName=="u") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDbreve, parseLatexString(true)));
                } else if (currentInstructionName=="widetilde") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDwidetilde, parseLatexString(true)));
                } else if (currentInstructionName=="ddot") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDddot, parseLatexString(true)));
                } else if (currentInstructionName=="cancel") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDcancel, parseLatexString(true)));
                } else if (currentInstructionName=="xcancel") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDxcancel, parseLatexString(true)));
                } else if (currentInstructionName=="bcancel") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDbcancel, parseLatexString(true)));
                } else if (currentInstructionName=="strike" || currentInstructionName=="st" || currentInstructionName=="sout") {
                    nl->addChild(new JKQTMathTextDecoratedNode(this, JKQTMathTextDecoratedNode::MTDstrike, parseLatexString(true)));
                } else {
                    if (currentInstructionName=="textcolor" || currentInstructionName=="mathcolor" || currentInstructionName=="color" || currentInstructionName=="colorbox") {
                        bool foundError=true;
                        QString col="";
                        if (getToken()==MTTtext) {
                            col=currentTokenName;
                            if (getToken()==MTTclosebrace) {
                                if (getToken()==MTTopenbrace) {
                                    foundError=false;
                                }
                            }
                        }
                        if (foundError) error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                        else nl->addChild(new JKQTMathTextInstruction1Node(this, currentInstructionName, parseLatexString(true), QStringList(col)));


                    } else {
                        nl->addChild(new JKQTMathTextInstruction1Node(this, currentInstructionName, parseLatexString(true)));
                    }
                }
                if (getNew) getToken();
                if (currentToken==MTTinstruction && currentTokenName=="limits") {
                    if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(true);
                    getNew=true;
                } else  if (currentToken==MTTinstruction && currentTokenName=="nolimits") {
                    if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(false);
                    getNew=true;
                } else {
                    getNew=false;
                }

            } else if (currentToken==MTTopenbracket && currentInstructionName!="left") {
                //std::cout<<"found '[' after '"<<name.toStdString()<<"'\n";
                if (currentInstructionName=="sqrt") {
                    JKQTMathTextNode* n1=parseLatexString(true, MTBTAny, "", true);
                    JKQTMathTextListNode* n1lst=dynamic_cast<JKQTMathTextListNode*>(n1);
                    JKQTMathTextTextNode* n1txt=dynamic_cast<JKQTMathTextTextNode*>(n1);
                    if (n1lst && n1lst->childCount()==1) {
                        n1txt=dynamic_cast<JKQTMathTextTextNode*>(n1lst->getChild(0));
                    }
                    int degree=2;
                    bool ok=false;
                    if (n1txt) degree=n1txt->getText().toInt(&ok);
                    if (!ok) {
                        degree=2;
                        error_list.append(tr("error @ ch. %1: an integer in [] after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                    }

                    JKQTMathTextNode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    else error_list.append(tr("error @ ch. %1: expected one argument in '{' braces after '%2' command with an optional argument in []").arg(currentTokenID).arg(currentInstructionName));

                    if (n1 && n2) nl->addChild(new JKQTMathTextSqrtNode(this, n2, degree));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
                } else {
                    nl->addChild(new JKQTMathTextTextNode(this, "[", false));
                }
            } else {
                bool subSuperscriptAboveBelowNode=false;
                //std::cout<<"did not find '{' after '"<<name.toStdString()<<"'\n";
                if (currentInstructionName=="right") {
                    if (currentToken==MTTtext) {
                        if (currentTokenName.size()>0) {
                            bool tokenWasNoBrace=false;
                            const QString firstTokenChar(currentTokenName[0]);
                            if (TokenNameMatchesJKQTMathTextBraceType(firstTokenChar, quitOnClosingBrace, true, &tokenWasNoBrace)) {
                                if (quitOnClosingBrace!=MTBTAny) currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                            } else {
                                getNew=false;
                            }
                        }
                    } else if (currentToken==MTTinstruction) {
                        if (InstructionNameMatchesJKQTMathTextBraceType(currentTokenName, quitOnClosingBrace, true)) {
                            currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                            break;
                        }
                    } else if (currentToken==MTTclosebracket) {
                        if (quitOnClosingBrace==MTBTSquareBracket || quitOnClosingBrace==MTBTAny) {
                            currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                            break;
                        }
                    } else {
                        getNew=false;
                    }
                } else if (currentInstructionName=="left") {
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
                                nl->addChild(new JKQTMathTextBraceNode(this, bracetype, bracetype, parseLatexString(currentTokenName.size()<=0, bracetype)));
                            } else {
                                getNew=false;
                            }
                        }
                    } else if (currentToken==MTTinstruction) {
                        const JKQTMathTextBraceType bracetypeopening=InstructionName2OpeningJKQTMathTextBraceType(currentTokenName);
                        if (bracetypeopening!=MTBTUnknown) {
                            nl->addChild(new JKQTMathTextBraceNode(this, bracetypeopening, bracetypeopening, parseLatexString(true, bracetypeopening)));
                        } else if (currentToken==MTTinstruction && TokenNameMatchesJKQTMathTextBraceType(currentTokenName, quitOnClosingBrace, true)) {
                            break;
                        }
                    } else if (currentToken==MTTopenbracket) {
                        nl->addChild(new JKQTMathTextBraceNode(this, MTBTSquareBracket, MTBTSquareBracket, parseLatexString(true, MTBTSquareBracket)));
                    } else {
                        error_list.append(tr("error @ ch. %1: unexpected token after \\left").arg(currentTokenID));
                    }

                } else {
                    nl->addChild(new JKQTMathTextSymbolNode(this, currentInstructionName, false));

                    static QSet<QString> subsupOperations= (QSet<QString>()<<"sum"<<"prod"<<"coprod"
                                                                           <<"bigcap"<<"bigcup"<<"bigvee"<<"bighat"
                                                                           <<"int"<<"iint"<<"iiint"<<"oint"<<"oiint"<<"oiiint"
                                                                           <<"mod"<<"median"<<"max"<<"min"<<"argmax"<<"argmin"<<"sup"<<"inf"
                                                                           <<"liminf"<<"limsup"<<"lim"<<"max"<<"min");
                    if (subsupOperations.contains(currentInstructionName) && parsingMathEnvironment) {
                        nl->getLastChild()->setSubSuperscriptAboveBelowNode(true);
                    }
                    if (currentToken==MTTinstruction && currentTokenName=="limits") {
                        if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(true);
                    } else  if (currentToken==MTTinstruction && currentTokenName=="nolimits") {
                        if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(false);
                    } else {
                        getNew=false;
                    }

                }
            }
        } else if (currentToken==MTTwhitespace) {
            if (!parsingMathEnvironment) nl->addChild(new JKQTMathTextWhitespaceNode(this));
        } else if (currentToken==MTTunderscore) {
            getToken();
            JKQTMathTextNode* child=nullptr;
            JKQTMathTextNode* child2=nullptr;
            if (currentToken==MTTinstruction) {
                QString name=currentTokenName;
                getToken(); // look at next token
                if (currentToken==MTTopenbrace) {
                    child=new JKQTMathTextInstruction1Node(this, name, parseLatexString(true));
                } else {
                    //bool addWhite=(currentToken==MTTwhitespace);
                    //getNew=addWhite;
                    //child=new JKQTMathTextSymbolNode(this, name, addWhite);
                    getNew=false;
                    child=new JKQTMathTextSymbolNode(this, name, false);
                }
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
                QString name=currentTokenName;
                getToken(); // look at next token
                if (currentToken==MTTopenbrace) {
                    child=new JKQTMathTextInstruction1Node(this, name, parseLatexString(true));
                } else {
                    //bool addWhite=(currentToken==MTTwhitespace);
                    //getNew=addWhite;
                    //child=new JKQTMathTextSymbolNode(this, name, addWhite);
                    getNew=false;
                    child=new JKQTMathTextSymbolNode(this, name, false);
                }
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
                nl->addChild(new JKQTMathTextInstruction1Node(this, "equation", parseLatexString(true)));
            }
        }
        if (getNew) getToken();
    }
    //std::cout<<"    leaving parseLatexString()\n";
    return simplifyJKQTMathTextNode(nl);
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

        double overallHeight=0;        
        getNodeTree()->getSize(painter, ev, width, ascent, overallHeight, strikeoutPos);
        descent=overallHeight-ascent;
        ascent=ascent*1.1;
        descent=qMax(ascent*0.1, descent*1.1);
        strikeoutPos=strikeoutPos*1.1;
    }
}

void JKQTMathText::draw(QPainter& painter, double x, double y, bool drawBoxes){
    if (getNodeTree()!=nullptr) {
        JKQTMathTextEnvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
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





