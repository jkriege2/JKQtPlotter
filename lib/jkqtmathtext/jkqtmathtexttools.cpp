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
        case MTFEunicode: return "MTFEunicode";
        case MTFEStandard: return "MTFEStandard";
        case MTFEunicodeLimited: return "MTFEunicodeLimited";
        case MTFEwinSymbol: return "MTFEwinSymbol";
    }
    return "???";
}

JKQTMathTextEnvironment::JKQTMathTextEnvironment() {
    color=QColor("black");
    font=MTEroman;
    fontSize=10;
    bold=false;
    italic=false;
    smallCaps=false;
    underlined=false;
    overline=false;
    strike=false;
    insideMath=false;
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
    f.setPointSizeF(fontSize);
    f.setStyleStrategy(QFont::NoFontMerging);
    return f;
}

QString JKQTMathTextEnvironment::toHtmlStart(JKQTMathTextEnvironment defaultEv) const {
    QString s;
    s=s+"font-size: "+QLocale::c().toString(fontSize)+"pt; ";
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

    QStringList td;
    if (underlined && !defaultEv.underlined) td<<"underline";
    if (overline && !defaultEv.overline) td<<"overline";
    if (strike && !defaultEv.strike) td<<"line-through";
    if (td.size()>0) s=s+"text-decoration: "+td.join(", ");
    return "<span style=\""+s+"\">";
}

QString JKQTMathTextEnvironment::toHtmlAfter(JKQTMathTextEnvironment /*defaultEv*/) const {
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
    fontName("Times New Roman"), fontEncoding(MTFEStandard),
    symbolfontGreek("Symbol"), symbolfontGreekEncoding(MTFEwinSymbol),
    symbolfontSymbol("Symbol"), symbolfontSymbolEncoding(MTFEwinSymbol)
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

QPainterPath JKQTMathTextMakeHBracePath(double x, double ybrace, double width, double bw, double cubicshrink, double cubiccontrolfac) {
    double xl1=x-(width)*cubicshrink+bw*cubicshrink;
    double xr2=x+(width)*cubicshrink-bw*cubicshrink;
    double xl2=x-bw*cubicshrink;
    double xr1=x+bw*cubicshrink;

    QPainterPath path;
    path.moveTo(xl1-bw*cubicshrink, ybrace-bw*cubicshrink);
    path.cubicTo(xl1-bw*cubicshrink, ybrace-bw*cubicshrink+bw*cubiccontrolfac, xl1-bw*cubiccontrolfac, ybrace, xl1, ybrace);
    path.lineTo(xl2, ybrace);
    path.cubicTo(xl2+bw*cubiccontrolfac, ybrace, (xl2+xr1)/2.0, ybrace+bw*cubicshrink-bw*cubiccontrolfac, (xl2+xr1)/2.0, ybrace+bw*cubicshrink);
    path.cubicTo((xl2+xr1)/2.0, ybrace+bw*cubicshrink-bw*cubiccontrolfac, xr1-bw*cubiccontrolfac, ybrace, xr1, ybrace);
    path.lineTo(xr2, ybrace);
    path.cubicTo(xr2+bw*cubiccontrolfac, ybrace, xr2+bw*cubicshrink, ybrace-bw*cubicshrink+bw*cubiccontrolfac, xr2+bw*cubicshrink, ybrace-bw*cubicshrink);
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

