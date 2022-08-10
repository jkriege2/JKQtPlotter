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



#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>


void initJKQTMathTextResources()
{
    static bool initialized=false;
    if (!initialized) {
#ifdef JKQTMATHTEXT_COMPILED_WITH_XITS
        Q_INIT_RESOURCE(xits);
#endif
        initialized=true;
    }
}

JKQTMathTextFontSpecifier::JKQTMathTextFontSpecifier():
    m_fontName(""),
    m_mathFontName("")
{

}

JKQTMathTextFontSpecifier::JKQTMathTextFontSpecifier(const QString &_fontName, const QString &_mathFontName):
    m_fontName(_fontName),
    m_mathFontName(_mathFontName)
{

}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::fromFontSpec(const QString &fontSpec)
{
    JKQTMathTextFontSpecifier s;
    s.setFontSpec(fontSpec);
    return s;
}

void JKQTMathTextFontSpecifier::setFontSpec(const QString &fontSpec)
{
    QStringList splitspec=fontSpec.split('+');
    if (splitspec.size()==0) {
        m_fontName=m_mathFontName="";
    } else if (splitspec.size()==1) {
        m_fontName=splitspec[0];
        m_mathFontName="";
    } else if (splitspec.size()==2) {
        m_fontName=splitspec[0];
        m_mathFontName=splitspec[1];
    } else if (splitspec.size()>2) {
        m_fontName=splitspec.mid(0, splitspec.size()-1).join('+');
        m_mathFontName=splitspec.last();
    }
}

QString JKQTMathTextFontSpecifier::getFontSpec() const
{
    QString res=m_fontName;
    if (m_mathFontName.size()>0) res+="+"+m_mathFontName;
    return res;
}

QString JKQTMathTextFontSpecifier::fontName() const
{
    return transformFontName(m_fontName);
}

QString JKQTMathTextFontSpecifier::mathFontName() const
{
    return transformFontName(m_mathFontName);
}

QString JKQTMathTextFontSpecifier::transformFontName(const QString &fontName)
{
    const QString fnt=fontName.trimmed().toLower();
    QFont testFnt;
    if (fnt=="serif") {
        testFnt.setStyleHint(QFont::StyleHint::Serif);
        return testFnt.defaultFamily();
    }
    if (fnt=="sans-serif" || fnt=="sansserif" || fnt=="sans" || fnt=="sans serif") {
        testFnt.setStyleHint(QFont::StyleHint::SansSerif);
        return testFnt.defaultFamily();
    }
    if (fnt=="cursive") {
        testFnt.setStyleHint(QFont::StyleHint::Cursive);
        return testFnt.defaultFamily();
    }
    if (fnt=="typewriter") {
        testFnt.setStyleHint(QFont::StyleHint::TypeWriter);
        return testFnt.defaultFamily();
    }
    if (fnt=="monospace") {
        testFnt.setStyleHint(QFont::StyleHint::Monospace);
        return testFnt.defaultFamily();
    }
    if (fnt=="fantasy") {
        testFnt.setStyleHint(QFont::StyleHint::Fantasy);
        return testFnt.defaultFamily();
    }
    if (fnt=="system") {
        testFnt.setStyleHint(QFont::StyleHint::System);
        return testFnt.defaultFamily();
    }
    if (fnt=="decorative") {
        testFnt.setStyleHint(QFont::StyleHint::Decorative);
        return testFnt.defaultFamily();
    }
    if (fnt=="default" || fnt=="app" || fnt=="application") {
        return QGuiApplication::font().family();
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    if (fnt=="fixed") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::FixedFont).family();
    }
    if (fnt=="smallest_readable" || fnt=="smallestreadable" || fnt=="smallest readable" || fnt=="smallest") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::SmallestReadableFont).family();
    }
    if (fnt=="title") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::TitleFont).family();
    }
    if (fnt=="general") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::GeneralFont).family();
    }
#elif QT_VERSION >= QT_VERSION_CHECK(5,2,0)
    QFontDatabase fontDB;
    if (fnt=="fixed") {
        return fontDB.systemFont(QFontDatabase::SystemFont::FixedFont).family();
    }
    if (fnt=="smallest_readable" || fnt=="smallestreadable" || fnt=="smallest readable" || fnt=="smallest") {
        return fontDB.systemFont(QFontDatabase::SystemFont::SmallestReadableFont).family();
    }
    if (fnt=="title") {
        return fontDB.systemFont(QFontDatabase::SystemFont::TitleFont).family();
    }
    if (fnt=="general") {
        return fontDB.systemFont(QFontDatabase::SystemFont::GeneralFont).family();
    }
#endif
    return fontName;
}

QString JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(const QString &fontName)
{
    const QString fnt=fontName.toLower().trimmed();
    if (fnt=="xits") {
        return getXITSFamilies().fontName();
    } else if (fnt=="asana") {
        return getASANAFamilies().fontName();
    } else if (fnt=="stix") {
        return getSTIXFamilies().fontName();
    }
    return transformFontName(fontName);
}

bool JKQTMathTextFontSpecifier::hasFontName() const
{
    return !m_fontName.isEmpty();
}

bool JKQTMathTextFontSpecifier::hasMathFontName() const
{
    return !m_mathFontName.isEmpty();
}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::getXITSFamilies()
{
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
    QFontDatabase fdb;
    const auto fontFamilies=fdb.families();
#else
    const auto fontFamilies=QFontDatabase::families();
#endif
    if (!fontFamilies.contains("XITS")) {
        if (QFile::exists(":/JKQTMathText/fonts/xits-bold.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-bold.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-bolditalic.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-bolditalic.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-italic.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-italic.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-math.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-math.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-mathbold.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-mathbold.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-regular.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-regular.otf"); }
    }

    static JKQTMathTextFontSpecifier fontSpec;
    if (fontSpec.m_fontName.isEmpty() && fontSpec.m_mathFontName.isEmpty()) {
        for (int i=0; i<fontFamilies.size(); i++) {
            if (fontFamilies.at(i).contains("XITS Math")) {
                fontSpec.m_mathFontName=fontFamilies.at(i);
            } else if (fontFamilies.at(i).contains("XITS")) {
                fontSpec.m_fontName=fontFamilies.at(i);
            }
            if (fontSpec.m_mathFontName.size()>0 && fontSpec.m_fontName.size()>0) {
                break;
            }
        }
        if (fontSpec.m_mathFontName.isEmpty() && !fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_mathFontName=fontSpec.m_fontName;
        } else if (!fontSpec.m_mathFontName.isEmpty() && fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_fontName=fontSpec.m_mathFontName;
        }
    }

    return fontSpec;
}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::getASANAFamilies()
{
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
    QFontDatabase fdb;
    const auto fontFamilies=fdb.families();
#else
    const auto fontFamilies=QFontDatabase::families();
#endif
    if (!fontFamilies.contains("Asana") && !fontFamilies.contains("Asana Math")) {
        if (QFile::exists(":/JKQTMathText/fonts/asana-math.otf")) { /*i=*/QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/asana-math.otf"); }
    }


    static JKQTMathTextFontSpecifier fontSpec;
    if (fontSpec.m_fontName.isEmpty() && fontSpec.m_mathFontName.isEmpty()) {
        for (int i=0; i<fontFamilies.size(); i++) {
            if (fontFamilies.at(i).contains("Asana Math")) {
                fontSpec.m_mathFontName=fontFamilies.at(i);
            } else if (fontFamilies.at(i).contains("Asana")) {
                fontSpec.m_fontName=fontFamilies.at(i);
            }
            if (fontSpec.m_mathFontName.size()>0 && fontSpec.m_fontName.size()>0) {
                break;
            }
        }
        if (fontSpec.m_mathFontName.isEmpty() && !fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_mathFontName=fontSpec.m_fontName;
        } else if (!fontSpec.m_mathFontName.isEmpty() && fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_fontName=fontSpec.m_mathFontName;
        }
    }


    return fontSpec;
}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::getSTIXFamilies()
{
    static QStringList mathNames{"STIX Two Math", "STIX Math", "STIX Two Math Standard", "STIX Math Standard"};
    static QStringList textNames{"STIX", "STIXGeneral", "STIX General"};

    static JKQTMathTextFontSpecifier fontSpec;
    if (fontSpec.m_fontName.isEmpty() && fontSpec.m_mathFontName.isEmpty()) {
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
        QFontDatabase fdb;
        const auto fontFamilies=fdb.families();
#else
        const auto fontFamilies=QFontDatabase::families();
#endif
        for (const QString& name:mathNames) {
            for (int i=0; i<fontFamilies.size(); i++) {
                if (fontFamilies.at(i).contains(name) ) {
                    fontSpec.m_mathFontName=fontFamilies.at(i);
                }
                if (fontSpec.m_mathFontName.size()>0) {
                    break;
                }
            }
            if (fontSpec.m_mathFontName.size()>0) {
                break;
            }
        }
        for (const QString& name:textNames) {
            for (int i=0; i<fontFamilies.size(); i++) {
                if (fontFamilies.at(i).contains(name) ) {
                    fontSpec.m_fontName=fontFamilies.at(i);
                }
                if (fontSpec.m_fontName.size()>0) {
                    break;
                }
            }
            if (fontSpec.m_fontName.size()>0) {
                break;
            }
        }
        if (fontSpec.m_mathFontName.isEmpty() && !fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_mathFontName=fontSpec.m_fontName;
        } else if (!fontSpec.m_mathFontName.isEmpty() && fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_fontName=fontSpec.m_mathFontName;
        }
    }
    return fontSpec;
}

QString JKQTMathTextFontEncoding2String(JKQTMathTextFontEncoding e)
{
    switch(e) {
        case MTFEUnicode: return "MTFEUnicode";
        case MTFEStandard: return "MTFELatin1";
        case MTFEWinSymbol: return "MTFEWinSymbol";
    }
    return "???";
}



QString JKQTMathTextBraceType2String(JKQTMathTextBraceType type) {
    switch(type) {
    case MTBTAngleBracket:
        return "angle_bracket";
    case MTBTSquareBracket:
        return "square_bracket";
    case MTBTCeilBracket:
        return "ceil_bracket";
    case MTBTCurlyBracket:
        return "curly_bracket";
    case MTBTDoubleLine:
        return "double_line";
    case MTBTFloorBracket:
        return "floor_bracket";
    case MTBTParenthesis:
        return "parenhesis";
    case MTBTSingleLine:
        return "single_line";
    case MTBTTopCorner:
        return "top_corner";
    case MTBTBottomCorner:
        return "bottom_corner";
    case MTBTAny:
        return "any";
    case MTBTNone:
        return "none";
    case MTBTUnknown:
        return "unknow";
    }
    return "???";
}

JKQTMathTextBraceType TokenName2JKQTMathTextBraceType(const QString &tokenName, bool* isOpening)
{
    if (tokenName=="(") { if (isOpening) *isOpening=true; return MTBTParenthesis; }
    if (tokenName==")") { if (isOpening) *isOpening=false; return MTBTParenthesis; }
    if (tokenName=="[") { if (isOpening) *isOpening=true; return MTBTSquareBracket; }
    if (tokenName=="]") { if (isOpening) *isOpening=false; return MTBTSquareBracket; }
    if (tokenName=="{") { if (isOpening) *isOpening=true; return MTBTCurlyBracket; }
    if (tokenName=="}") { if (isOpening) *isOpening=false; return MTBTCurlyBracket; }
    if (tokenName=="|") { if (isOpening) *isOpening=true; return MTBTSingleLine; }
    if (tokenName=="||" || tokenName=="#") { if (isOpening) *isOpening=true; return MTBTDoubleLine; }

    if (tokenName=="<" || tokenName=="langle") { if (isOpening) *isOpening=true; return MTBTAngleBracket; }
    if (tokenName==">" || tokenName=="rangle") { if (isOpening) *isOpening=false; return MTBTAngleBracket; }
    if (tokenName=="_" || tokenName=="lfloor") { if (isOpening) *isOpening=true; return MTBTFloorBracket; }
    if (tokenName=="_" || tokenName=="rfloor") { if (isOpening) *isOpening=false; return MTBTFloorBracket; }
    if (tokenName=="~" || tokenName=="lceil") { if (isOpening) *isOpening=true; return MTBTCeilBracket; }
    if (tokenName=="~" || tokenName=="rceil") { if (isOpening) *isOpening=false; return MTBTCeilBracket; }
    if (tokenName=="ulcorner" || tokenName=="tlcorner") { if (isOpening) *isOpening=true; return MTBTTopCorner; }
    if (tokenName=="urcorner" || tokenName=="trcorner") { if (isOpening) *isOpening=false; return MTBTTopCorner; }
    if (tokenName=="blcorner" || tokenName=="llcorner") { if (isOpening) *isOpening=true; return MTBTBottomCorner; }
    if (tokenName=="brcorner" || tokenName=="lrcorner") { if (isOpening) *isOpening=false; return MTBTBottomCorner; }
    if (isOpening) *isOpening=true;

    if (tokenName=="any") return MTBTAny;
    if (tokenName=="." || tokenName=="" || tokenName=="none") return MTBTNone;
    return MTBTUnknown;
}

JKQTMathTextBraceType InstructionName2OpeningJKQTMathTextBraceType(const QString &tokenName)
{
    if (tokenName=="{") return MTBTCurlyBracket;
    if (tokenName=="|") return MTBTDoubleLine;
    if (tokenName=="langle") return MTBTAngleBracket;
    if (tokenName=="lfloor") return MTBTFloorBracket;
    if (tokenName=="lceil") return MTBTCeilBracket;
    if (tokenName=="tlcorner"||tokenName=="ulcorner") return MTBTTopCorner;
    if (tokenName=="blcorner"||tokenName=="llcorner") return MTBTBottomCorner;
    return MTBTUnknown;
}

JKQTMathTextBraceType InstructionName2JKQTMathTextBraceType(const QString &tokenName)
{
    if (tokenName=="{" || tokenName=="}") return MTBTCurlyBracket;
    if (tokenName=="|") return MTBTDoubleLine;
    if (tokenName=="langle" || tokenName=="rangle") return MTBTAngleBracket;
    if (tokenName=="lfloor" || tokenName=="rfloor") return MTBTFloorBracket;
    if (tokenName=="lceil" || tokenName=="rceil") return MTBTCeilBracket;
    if (tokenName=="ulcorner" || tokenName=="urcorner"||tokenName=="tlcorner" || tokenName=="trcorner") return MTBTTopCorner;
    if (tokenName=="blcorner" || tokenName=="brcorner"||tokenName=="llcorner" || tokenName=="lrcorner") return MTBTBottomCorner;
    return MTBTUnknown;
}

bool TokenNameMatchesJKQTMathTextBraceType(const QString &token, JKQTMathTextBraceType type, bool acceptMTBTNone, bool* tokenEqualsNone)
{
    const JKQTMathTextBraceType bt=TokenName2JKQTMathTextBraceType(token);
    if (tokenEqualsNone) *tokenEqualsNone=(bt==MTBTNone);
    if (type==MTBTAny) return true;
    if (acceptMTBTNone && bt==MTBTNone) return true;
    return (bt==type);
}

bool InstructionNameMatchesJKQTMathTextBraceType(const QString &token, JKQTMathTextBraceType type, bool acceptMTBTNone, bool* tokenEqualsNone)
{
    const JKQTMathTextBraceType bt=InstructionName2JKQTMathTextBraceType(token);
    if (tokenEqualsNone) *tokenEqualsNone=(bt==MTBTNone);
    if (type==MTBTAny) return true;
    if (acceptMTBTNone && bt==MTBTNone) return true;
    return (bt==type);
}

QString JKQTMathTextEnvironment::FontSizeUnit2String(FontSizeUnit unit)
{
    switch(unit) {
    case PIXELS: return "pix";
    default:
    case POINTS: return "pt";
    }
}

JKQTMathTextEnvironment::FontSizeUnit JKQTMathTextEnvironment::String2FontSizeUnit(QString unit)
{
    unit=unit.toLower().trimmed();
    if (unit=="pt" || unit=="points" || unit=="point") return POINTS;
    if (unit=="pix" || unit=="pixel" || unit=="pixels" || unit=="px") return PIXELS;
    return POINTS;
}

JKQTMathTextEnvironment::JKQTMathTextEnvironment() {
    color=QColor("black");
    font=MTEroman;
    fontSize=10;
    fontSizeUnit=POINTS;
    bold=false;
    italic=false;
    smallCaps=false;
    underlined=false;
    overline=false;
    strike=false;
    insideMath=false;
    insideMathForceDigitsUpright=true;
}

void JKQTMathTextEnvironment::beginMathMode()
{
    insideMath=true;
    insideMathForceDigitsUpright=true;
    italic=true;
    smallCaps=false;
    underlined=false;
    overline=false;
    strike=false;
}

void JKQTMathTextEnvironment::endMathMode()
{
    insideMath=false;
    insideMathForceDigitsUpright=true;
    italic=false;
    smallCaps=false;
    underlined=false;
    overline=false;
    strike=false;
}

JKQTMathTextFontEncoding JKQTMathTextEnvironment::getFontEncoding(JKQTMathText* parent) const {
    switch (font) {
        case MTEsans: if (insideMath) {
                return parent->getFontEncodingMathSans();
            } else {
                return parent->getFontEncodingSans();
            }
            break;
        case MTEmathSans: return parent->getFontEncodingMathSans(); break;
        case MTEtypewriter: return parent->getFontEncodingTypewriter(); break;
        case MTEscript: return parent->getFontEncodingScript(); break;
        case MTEcaligraphic: return parent->getFontEncodingCaligraphic(); break;
        case MTEblackboard: return parent->getFontEncodingBlackboard(); break;
        case MTEfraktur: return parent->getFontEncodingFraktur(); break;
        case MTEmathRoman: return parent->getFontEncodingMathRoman(); break;
        case MTEroman: if (insideMath) {
                return parent->getFontEncodingMathRoman();
            } else {
                return parent->getFontEncodingRoman();
            }
            break;
        default:
            return MTFEStandard;
    }
    return MTFEStandard;
}

QFont JKQTMathTextEnvironment::getFont(JKQTMathText* parent) const {
    QFont f;
    switch (font) {
        case MTEsans: if (insideMath) {
                f.setFamily(parent->getFontMathSans());
            } else {
                f.setFamily(parent->getFontSans());
            }
            break;
        case MTEmathSans: f.setFamily(parent->getFontMathSans()); break;
        case MTEtypewriter: f.setFamily(parent->getFontTypewriter()); break;
        case MTEscript: f.setFamily(parent->getFontScript()); break;
        case MTEcaligraphic: f.setFamily(parent->getFontCaligraphic()); break;
        case MTEblackboard: f.setFamily(parent->getFontBlackboard()); break;
        case MTEfraktur: f.setFamily(parent->getFontFraktur()); break;
        case MTEmathRoman: f.setFamily(parent->getFontMathRoman()); break;
        case MTEFallbackSymbols: f.setFamily(parent->getFallbackFontSymbols()); break;
        case MTECustomFont: f.setFamily(customFontName); break;
        default:
        case MTEroman: if (insideMath) {
                f.setFamily(parent->getFontMathRoman());
            } else {
                f.setFamily(parent->getFontRoman());
            }
            break;
    }
    f.setBold(bold);
    f.setItalic(italic);
    f.setUnderline(underlined);
    f.setOverline(overline);
    f.setStrikeOut(strike);
    f.setCapitalization(QFont::MixedCase);
    if (smallCaps) f.setCapitalization(QFont::SmallCaps);
    if (fontSizeUnit==POINTS) f.setPointSizeF(fontSize);
    else if (fontSizeUnit==PIXELS) f.setPixelSize(static_cast<int>(fontSize));
    f.setStyleStrategy(QFont::NoFontMerging);
    return f;
}

JKQTMathTextEnvironment JKQTMathTextEnvironment::exchangedFontForRoman() const
{
    if (insideMath) return exchangedFontFor(MTEmathRoman);
    else return exchangedFontFor(MTEroman);
}

JKQTMathTextEnvironment JKQTMathTextEnvironment::exchangedFontFor(JKQTMathTextEnvironmentFont font) const
{
    JKQTMathTextEnvironment newEnv=*this;
    newEnv.font=font;
    return newEnv;
}

QString JKQTMathTextEnvironment::toHtmlStart(JKQTMathTextEnvironment defaultEv, JKQTMathText* parentMathText) const {
    QString s;
    if (fontSizeUnit==POINTS) s=s+"font-size: "+QLocale::c().toString(fontSize)+"pt; ";
    else if (fontSizeUnit==PIXELS) s=s+"font-size: "+QLocale::c().toString(fontSize)+"px; ";

    if (insideMath) {
        if (defaultEv.italic) {
            if (!italic) s=s+"font-style: italic; ";
            if (italic) s=s+"font-style: normal; ";
        } else {
            if (!italic) s=s+"font-style: italic; ";
        }
    } else {
        if (!defaultEv.italic && italic) s=s+"font-style: italic; ";
    }
    if (bold && !defaultEv.bold) s=s+"font-weight: bold";
    if (color!=defaultEv.color) s=s+"color: "+color.name();
    if (font!=defaultEv.font) s=s+"font-family: "+getFont(parentMathText).family();

    QStringList td;
    if (underlined && !defaultEv.underlined) td<<"underline";
    if (overline && !defaultEv.overline) td<<"overline";
    if (strike && !defaultEv.strike) td<<"line-through";
    if (td.size()>0) s=s+"text-decoration: "+td.join(", ");
    return "<span style=\""+s+"\">";
}

QString JKQTMathTextEnvironment::toHtmlAfter(JKQTMathTextEnvironment /*defaultEv*/, JKQTMathText */*parentMathText*/) const {
    return "</span>";
}

JKQTMathTextNodeSize::JKQTMathTextNodeSize():
    width(0),
    baselineHeight(0),
    overallHeight(0),
    strikeoutPos()
{

}


JKQTMathTextFontDefinition::JKQTMathTextFontDefinition():
    fontName("Times New Roman"), fontEncoding(MTFEStandard)
{

}




QPainterPath JKQTMathTextMakeDArrow(double x, double y, double width, double arrowW, bool left, bool right) {
    double x1=x;
    double x2=x+width;
    double dx=arrowW/4.0;
    double y1=y-dx;
    double y2=y+dx;
    double x3=x2-arrowW/2.0;
    double y3u=y-arrowW/2.0;
    double y3d=y+arrowW/2.0;
    double x3l=x+arrowW/2.0;

    QPainterPath path;
    path.moveTo(x1+dx, y1);
    path.lineTo(x2-dx, y1);
    path.moveTo(x1+dx, y2);
    path.lineTo(x2-dx, y2);
    if (right) {
        path.moveTo(x3, y3u);
        path.lineTo(x2, y);
        path.lineTo(x3, y3d);
    }
    if (left) {
        path.moveTo(x3l, y3u);
        path.lineTo(x1, y);
        path.lineTo(x3l, y3d);
    }
    return path;
}

QPainterPath JKQTMathTextMakeArrow(double x, double y, double width, double arrowW, bool left, bool right) {
    double x1=x;
    double x2=x+width;
    double x3=x2-arrowW/2.0;
    double y3u=y-arrowW/2.0;
    double y3d=y+arrowW/2.0;
    double x3l=x+arrowW/2.0;

    QPainterPath path;
    path.moveTo(x1, y);
    path.lineTo(x2, y);
    if (right) {
        path.moveTo(x3, y3u);
        path.lineTo(x2, y);
        path.lineTo(x3, y3d);
    }
    if (left) {
        path.moveTo(x3l, y3u);
        path.lineTo(x1, y);
        path.lineTo(x3l, y3d);
    }
    return path;
}

QPainterPath JKQTMathTextMakeHBracePath(double x, double ybrace, double width, double bw, double lineWidth, double cubicshrink, double cubiccontrolfac, double lineWidthShrinkFactor, double lineWidthGrowFactor) {
    const double thinLW=lineWidthShrinkFactor*lineWidth;
    const double thickLW=lineWidth*lineWidthGrowFactor;
    const double xleft=x-width/2.0;
    const double xleft_inner=xleft+thinLW;
    const double xleftflat_leftbottom=xleft+cubicshrink*bw;
    const double xleftflat_lefttop=xleft_inner+cubicshrink*bw;
    const double xright=x+width/2.0;
    const double xright_inner=xright-thinLW;
    const double xrightflat_rightbottom=xright-cubicshrink*bw;
    const double xrightflat_righttop=xright_inner-cubicshrink*bw;
    const double xleftflat_righttop=x-bw*cubicshrink;
    const double xlefttip=x-thinLW/2.0;
    const double xleftflat_rightbottom=xlefttip-bw*cubicshrink;
    const double xrightflat_lefttop=x+bw*cubicshrink;
    const double xrighttip=x+thinLW/2.0;
    const double xrightflat_leftbottom=xrighttip+bw*cubicshrink;

    const double ytop=ybrace-bw*cubicshrink;
    const double yctop=ybrace-thickLW/2.0;
    const double ycbottom=ybrace+thickLW/2.0;
    const double ybottom=ybrace+bw*cubicshrink;
    const double ybottomtip=ybottom-thickLW;

    const double dxyControl=bw*cubiccontrolfac;

    QPainterPath path;
    path.moveTo(xleft_inner, ytop);
    path.lineTo(xleft, ytop);  // top-left flat
    path.cubicTo(xleft, ytop+dxyControl, xleftflat_leftbottom-dxyControl, ycbottom, xleftflat_leftbottom, ycbottom);
    path.lineTo(xleftflat_rightbottom,ycbottom); // left arm, bottom
    path.cubicTo(xleftflat_rightbottom+dxyControl, ycbottom, xlefttip, ybottom-dxyControl, xlefttip, ybottom);
    path.lineTo(xrighttip, ybottom); // bottom flat
    path.cubicTo(xrighttip, ybottom-dxyControl,xrightflat_leftbottom-dxyControl, ycbottom, xrightflat_leftbottom, ycbottom);
    path.lineTo(xrightflat_rightbottom, ycbottom); // right arm, bottom
    path.cubicTo(xrightflat_rightbottom+dxyControl, ycbottom, xright, ytop+dxyControl, xright, ytop);

    path.lineTo(xright_inner,ytop); // top-right flat
    path.cubicTo(xright_inner, ytop+dxyControl, xrightflat_righttop+dxyControl, yctop, xrightflat_righttop, yctop);
    path.lineTo(xrightflat_lefttop, yctop); // right arm, top
    path.cubicTo(xrightflat_lefttop-dxyControl, yctop, x, yctop, x, ybottomtip); // center-tip
    path.cubicTo(x, yctop, xleftflat_righttop+dxyControl, yctop, xleftflat_righttop, yctop);
    path.lineTo(xleftflat_lefttop, yctop); // left arm, top
    path.cubicTo(xleftflat_lefttop-dxyControl, yctop, xleft_inner,ytop+dxyControl, xleft_inner, ytop);
    path.closeSubpath();
    return path;
}


JKQTMathTextTBRData::JKQTMathTextTBRData(const QFont &f, const QString &text,  QPaintDevice *pd):
    fm(f, pd)
{
    this->text=text;
    this->tbr=this->fm.tightBoundingRect(text);
    this->f=f;
    //this->pd=pd;
    if (pd) {
        ldpiX=pd->logicalDpiX();
        ldpiY=pd->logicalDpiY();
        pdpiX=pd->physicalDpiX();
        pdpiY=pd->physicalDpiY();
    } else {
        ldpiX=0;
        ldpiY=0;
        pdpiX=0;
        pdpiY=0;
    }
}

bool JKQTMathTextTBRData::operator==(const JKQTMathTextTBRData &other) const
{
    return ldpiX==other.ldpiX &&  ldpiY==other.ldpiY && text==other.text && f==other.f;
}



JKQTMathTextTBRDataH::JKQTMathTextTBRDataH(const QFont &f, const QString &text, QPaintDevice *pd)
{
    this->text=text;
    this->f=f;
    if (pd) {
        ldpiX=pd->logicalDpiX();
        ldpiY=pd->logicalDpiY();
        pdpiX=pd->physicalDpiX();
        pdpiY=pd->physicalDpiY();
    } else {
        ldpiX=0;
        ldpiY=0;
        pdpiX=0;
        pdpiY=0;
    }
}

bool JKQTMathTextTBRDataH::operator==(const JKQTMathTextTBRDataH &other) const
{
    return ldpiX==other.ldpiX &&  ldpiY==other.ldpiY && text==other.text && f==other.f;

}



QRectF JKQTMathTextGetTightBoundingRect(const QFont &fm, const QString &text, QPaintDevice *pd)
{
    static QList<JKQTMathTextTBRData> JKQTMathText_tbrs=QList<JKQTMathTextTBRData>();
    static QHash<JKQTMathTextTBRDataH, QRectF> JKQTMathText_tbrh=QHash<JKQTMathTextTBRDataH, QRectF>();

    JKQTMathTextTBRDataH  dh(fm, text, pd);
    if (pd) {
        if (JKQTMathText_tbrh.contains(dh)) return JKQTMathText_tbrh[dh];
        /*for (int i=0; i<tbrs.size(); i++) {
            if (tbrs[i].f==fm && tbrs[i].text==text && (tbrs[i].ldpiX==pd->logicalDpiX() && tbrs[i].ldpiY==pd->logicalDpiY() && tbrs[i].pdpiX==pd->physicalDpiX() && tbrs[i].pdpiY==pd->physicalDpiY())) {
                //qDebug()<<"   ### "<<fm<<pd<<tbrs[i].text<<tbrs[i].tbr;
                return tbrs[i].tbr;
            }
        }*/
    } else {
        //qDebug()<<"warning no pd";
    }
    JKQTMathTextTBRData d(fm, text, pd);
    JKQTMathText_tbrs.append(d);
    JKQTMathText_tbrh[dh]=d.tbr;
    //qDebug()<<"TBRs lits: "<<tbrs.size();
    //qDebug()<<"+++ "<<fm<<pd<<d.text<<d.tbr;
    return d.tbr;
}

QFont JKQTMathTextGetNonItalic(const QFont &font)
{
    QFont f=font;
    f.setItalic(false);
    return f;
}


bool isPrintableJKQTMathTextBraceType(JKQTMathTextBraceType type)
{
    return (type!=MTBTAny) && (type!=MTBTUnknown);
}

JKQTMathTextFontEncoding estimateJKQTMathTextFontEncoding(QFont font)
{
    font.setStyleStrategy(QFont::NoFontMerging);
    const QString fontFamily=font.family().toLower();
    if (fontFamily=="symbol") return JKQTMathTextFontEncoding::MTFEWinSymbol;
    if (fontFamily.startsWith("xits") || fontFamily.startsWith("stix")||fontFamily.startsWith("asana")) return JKQTMathTextFontEncoding::MTFEUnicode;
    const QFontMetricsF fm(font);
    if (fm.inFont(QChar(0x3B1))) return JKQTMathTextFontEncoding::MTFEUnicode; // griechisch alpha
    if (fm.inFont(QChar(0x2192))) return JKQTMathTextFontEncoding::MTFEUnicode; // pfeil nach rechts
    if (fm.inFont(QChar(0x2202))) return JKQTMathTextFontEncoding::MTFEUnicode; // partial
    if (fm.inFont(QChar(0x2208))) return JKQTMathTextFontEncoding::MTFEUnicode; // element
    return JKQTMathTextFontEncoding::MTFELatin1;
}

QString JKQTMathTextHorizontalAlignment2String(JKQTMathTextHorizontalAlignment type)
{
    switch(type) {
        case MTHALeft: return "left";
        case MTHARight: return "right";
        default:
        case MTHACentered: return "centered";
    }
}

JKQTMathTextHorizontalAlignment String2JKQTMathTextHorizontalAlignment(QString tokenName)
{
    tokenName=tokenName.toLower().trimmed();
    if (tokenName=="l" || tokenName=="left" || tokenName=="flushleft") return MTHALeft;
    if (tokenName=="r" || tokenName=="right" || tokenName=="flushright") return MTHARight;
    if (tokenName=="c" || tokenName=="center" || tokenName=="centered") return MTHACentered;
    return MTHACentered;
}

QString JKQTMathTextVerticalOrientation2String(JKQTMathTextVerticalOrientation mode)
{
    switch(mode) {
        case MTVOTop: return "top";
        case MTVOCentered: return "centered";
        case MTVOLastLine: return "last_line";
        case MTVOBottom: return "bottom";
        default:
        case MTVOFirstLine: return "first_line";
    }
}

JKQTMathTextVerticalOrientation String2JKQTMathTextVerticalOrientation(QString tokenName)
{
    tokenName=tokenName.toLower().trimmed();
    if (tokenName=="p" || tokenName=="first_line" || tokenName=="first-line" || tokenName=="firstline" || tokenName=="line1") return MTVOFirstLine;
    if (tokenName=="last_line" || tokenName=="last-line" || tokenName=="lastline" || tokenName=="linen") return MTVOLastLine;
    if (tokenName=="t" || tokenName=="top") return MTVOTop;
    if (tokenName=="b" || tokenName=="bottom") return MTVOBottom;
    if (tokenName=="c" || tokenName=="center" || tokenName=="centered") return MTVOCentered;
    return MTVOCentered;
}

QString JKQTMathTextBlackboradDrawingMode2String(JKQTMathTextBlackboradDrawingMode mode)
{
    switch(mode) {
        case MTBBDMfontDirectly: return "font_directly";
        case MTBBDMsimulate: return "simulate";
        case MTBBDMunicodeCharactersOrFontDirectly: return "unicode_or_font_directly";
        case MTBBDMunicodeCharactersOrSimulate: return "unicode_or_simulate";
    }
    return "font_directly";
}


JKQTMathTextBlackboradDrawingMode String2JKQTMathTextBlackboradDrawingMode(QString mode)
{
    mode=mode.toLower().simplified().trimmed();
    if (mode=="font_directly" || mode=="font" || mode=="directly") return MTBBDMfontDirectly;
    if (mode=="simulate") return MTBBDMsimulate;
    if (mode=="unicode_or_font_directly" || mode=="unicode_or_font" || mode=="unicode_or_directly") return MTBBDMunicodeCharactersOrFontDirectly;
    if (mode=="unicode_or_simulate") return MTBBDMunicodeCharactersOrSimulate;
    if (mode=="default") return MTBBDMdefault;
    return MTBBDMdefault;
}

void JKQTMathTextDrawStringSimBlackboard(QPainter &painter, const QFont &f, const QColor& color, double x, double y, const QString &txt)
{
    const QFontMetricsF fm(f, painter.device());
    const QPen p(color, fm.lineWidth()/4.0, Qt::SolidLine);
    painter.setPen(p);
    QPainterPath path;
    path.addText(QPointF(x, y), f, txt);
    path.addText(QPointF(x+fm.lineWidth()/2.0, y), f, txt);
    painter.drawPath(path);
}
