/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)
    with contributions from: Razi Alavizadeh

    

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


/*
  Name: jkqtmathtext.h
  Copyright: (c) 2010-2019
  Author: Jan krieger <jan@jkrieger.de>, http://www.jkrieger.de/
*/



#ifndef JKQTMATHTEXTTOOLS_H
#define JKQTMATHTEXTTOOLS_H

#include <QObject>
#include <QSettings>
#include <QPainter>
#include <QString>
#include <QSet>
#include <QFile>
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include <QWidget>
#include <QLabel>
#include <QHash>
#include <QPainterPath>

class JKQTMathText; // forward



/** \brief initialized Qt-ressources necessary for JKQTMathText
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT void initJKQTMathTextResources();


/*! \brief represents a font specifier for JKQTMathText. The font consists of two parts: the actual font and the font used for math output (which may be empty)
    \ingroup jkqtmathtext_tools

    \section JKQTMathTextFontSpecifier_specialNames Special FOnt Names
    This object also implements replacing special font names with actual fonts. Supported special font names are:
      - \c default / \c app / \c application - the applications default font
      - \c times / \c serif - a general serif font
      - \c sans-serif - a general sans-serif font
      - \c typewriter - a general typewrter/monospaced font
      - \c cursive
      - \c decorative
      - \c fantasy
      - \c monospace
      - \c system
    .

    If copiled with Qt>5.3 you can also use these:
      - \c fixed
      - \c smallest_readable
      - \c title
      - \c general
    .
*/
struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextFontSpecifier {
    JKQTMathTextFontSpecifier();
    JKQTMathTextFontSpecifier(const QString& fontName, const QString& mathFontName);
    /** \brief construct a JKQTMathTextFontSpecifier, by parsing a \a fontSpec string with the form \c "FONT_NAME[+MATH_FONT_NAME]". */
    static JKQTMathTextFontSpecifier fromFontSpec(const QString& fontSpec);

    /** \brief initialises the object with values from parsing a \a fontSpec string with the form \c "FONT_NAME[+MATH_FONT_NAME]". */
    void setFontSpec(const QString& fontSpec);

    /** \brief returns the object's constents as a fontSpec string with the form \c "FONT_NAME[+MATH_FONT_NAME]". */
    QString getFontSpec() const;
    /** \copydoc m_fontName */
    QString fontName() const;
    /** \copydoc m_mathFontName */
    QString mathFontName() const;

    /** \copydoc m_fontName */
    void setFontName(const QString& name);
    /** \copydoc m_mathFontName */
    void setmathFontName(const QString& name);
    /** \brief finds actual fonts for some predefined special font names, as listed in \ref JKQTMathTextFontSpecifier_specialNames */
    static QString transformFontName(const QString& fontName);
    /** \brief same as transformFontName(), but also finds the actual name for XITS, STIX, ASANA,... */
    static QString transformFontNameAndDecodeSpecialFonts(const QString& fontName);
    /** \brief leiefert \c true, wenn ein fontName() verfügbar ist */
    bool hasFontName() const;
    /** \brief leiefert \c true, wenn ein mathFontName() verfügbar ist */
    bool hasMathFontName() const;

    /** \brief initialize with the font-families from the XITS package for text and math */
    static JKQTMathTextFontSpecifier getXITSFamilies();

    /** \brief initialize with the font-families from the XITS package for text and math */
    static JKQTMathTextFontSpecifier getASANAFamilies();

    /** \brief initialize with the font-families from the STIX package for text and math */
    static JKQTMathTextFontSpecifier getSTIXFamilies();
private:
    /** \brief specifies the main font name */
    QString m_fontName;
    /** \brief specifies the math font to use in addition to fontName */
    QString m_mathFontName;


};


/** \brief used to specify the font encoding used for drawing
 *  \ingroup jkqtmathtext_tools
*/
enum JKQTMathTextFontEncoding {
    MTFEwinSymbol,      /*!< \brief This assumes that symbols shall be taken from a MS Windows style Symbol font */
    MTFEunicode,        /*!< \brief This assumes that symbols shall be taken from a Unicode font  (e.g. the STIX fonts from <a href="http://www.stixfonts.org/">http://www.stixfonts.org/</a>)*/
    MTFEunicodeLimited, /*!< \brief This assumes that the fonts used are Unicode, but only offer a limited set of symbols.  Especially math symbols are missing from this encoding */
    MTFEStandard,       /*!< \brief the encoding of a standard TTF font (i.e. we can only expect letters,number and not many special characters) */
};

/** \brief convert MTfontEncoding to a string
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT QString JKQTMathTextFontEncoding2String(JKQTMathTextFontEncoding e);

/** \brief types of available braces
 *  \ingroup jkqtmathtext_tools
 */
enum JKQTMathTextBraceType {
    MTBTParenthesis=0,  /*!< \brief parantheses () \image html jkqtmathtext/jkqtmathtext_brace_round.png */
    MTBTSquareBracket,  /*!< \brief brackets [] \image html jkqtmathtext/jkqtmathtext_brace_rect.png */
    MTBTCurlyBracket,  /*!< \brief curly braces {} \image html jkqtmathtext/jkqtmathtext_brace_curly.png */
    MTBTAngleBracket,  /*!< \brief angle backets <> \image html jkqtmathtext/jkqtmathtext_brace_tri.png */
    MTBTCeilBracket,  /*!< \brief ceil brackets \image html jkqtmathtext/jkqtmathtext_brace_ceil.png */
    MTBTFloorBracket,  /*!< \brief floor brackets \image html jkqtmathtext/jkqtmathtext_brace_floor.png */
    MTBTDoubleLine,  /*!< \brief double-line brackets (norm ||...||) \image html jkqtmathtext/jkqtmathtext_brace_dblline.png */
    MTBTSingleLine,  /*!< \brief single-line brackets (abs |...|) \image html jkqtmathtext/jkqtmathtext_brace_oneline.png */
    MTBTTopCorner,  /*!< \brief top-corner brackets  \image html jkqtmathtext/jkqtmathtext_brace_topcorner.png */
    MTBTBottomCorner,  /*!< \brief bottom-corner brackets  \image html jkqtmathtext/jkqtmathtext_brace_bottomcorner.png */
    MTBTNone,  /*!< \brief no bracket */
    MTBTAny,  /*!< \brief any bracket, used by JKQTMathText::parseLatexString() */
    MTBTUnknown  /*!< \brief an unknown tokenName presented to TokenName2JKQTMathTextBraceType() */
};
/** \brief convert a JKQTMathTextBraceType into a string
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT QString JKQTMathTextBraceType2String(JKQTMathTextBraceType type);
/** \brief convert a string \a tokenName describing a LaTeX Token or Instruction into an opening or closing JKQTMathTextBraceType
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBraceType TokenName2JKQTMathTextBraceType(const QString& tokenName);
/** \brief convert a string \a tokenName describing a LaTeX Instruction into an opening JKQTMathTextBraceType
 *  \ingroup jkqtmathtext_tools
 *
 *  This returns a JKQTMathTextBraceType for which  isPrintableJKQTMathTextBraceType() is \c true, or MTBTUnknown,
 *  never MTBTNone or MTBTAny.
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBraceType InstructionName2OpeningJKQTMathTextBraceType(const QString& tokenName);
/** \brief convert a string \a tokenName describing a LaTeX Instruction into an opening or closing JKQTMathTextBraceType
 *  \ingroup jkqtmathtext_tools
 *
 *  This returns a JKQTMathTextBraceType for which  isPrintableJKQTMathTextBraceType() is \c true, or MTBTUnknown,
 *  never MTBTNone or MTBTAny.
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBraceType InstructionName2JKQTMathTextBraceType(const QString& tokenName);
/** \brief return \c true if \a type represents a printable type of brace (including MTBTNone), basically \c true
 *         for any JKQTMathTextBraceType that can be used as parameter to JKQTMathTextBraceNode
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT bool isPrintableJKQTMathTextBraceType(JKQTMathTextBraceType type);
/** \brief returns true, if the given token/instruction-Name \a token ("{", "(", ..., "lceil", ".", ...) matches the given \a type (returns true, when \a type == MTBTAny )
 *  \ingroup jkqtmathtext_tools
 *
 *  This accepts TokenName2JKQTMathTextBraceType(toke)==MTBTNone for any \a type, iff \a acceptMTBTNone \a ==true.
 *
 *  Optionally returns in \a tokenEqualsNone whether \a token was encoding for MTBTNone .
 */
JKQTMATHTEXT_LIB_EXPORT bool TokenNameMatchesJKQTMathTextBraceType(const QString &token, JKQTMathTextBraceType type, bool acceptMTBTNone, bool *tokenEqualsNone=nullptr);
/** \brief returns true, if the given instruction-Name \a token ("|", "{", ..., "lceil", ".", ...) matches the given \a type (returns true, when \a type == MTBTAny )
 *  \ingroup jkqtmathtext_tools
 *
 *  This accepts TokenName2JKQTMathTextBraceType(toke)==MTBTNone for any \a type, iff \a acceptMTBTNone \a ==true.
 *
 *  Optionally returns in \a tokenEqualsNone whether \a token was encoding for MTBTNone .
 */
JKQTMATHTEXT_LIB_EXPORT bool InstructionNameMatchesJKQTMathTextBraceType(const QString &token, JKQTMathTextBraceType type, bool acceptMTBTNone, bool *tokenEqualsNone=nullptr);

/** \brief the available logical fonts (default is MTEroman)
 *  \ingroup jkqtmathtext_tools
 */
enum JKQTMathTextEnvironmentFont {
    MTEroman,       /*!< \brief roman font, e.g. <code>\\rm{}</code> */
    MTEsans,        /*!< \brief sans-serif font, e.g. <code>\\sf{}</code> */
    MTEmathRoman,   /*!< \brief math-mode roman font, e.g. <code>\\mathrm{}</code> */
    MTEmathSans,    /*!< \brief math-mode sans-serif font, e.g. <code>\\mathsf{}</code> */
    MTEtypewriter,  /*!< \brief typewriter font, e.g. <code>\\tt{},\\mathtt{}</code> */
    MTEscript,      /*!< \brief script font, e.g. <code>\\script{},\\mathscript{}</code> */
    MTEblackboard,  /*!< \brief blackboard font, e.g. <code>\\mathbb{}</code> */
    MTEcaligraphic, /*!< \brief caligraphic font, e.g. <code>\\mathcal{}</code> */
    MTEfraktur,     /*!< \brief fraktur font, e.g. <code>\\mathfrak{}</code> */

    MTenvironmentFontCount  /*!< \brief internal enum value that allows to iterate over MTenvironmentFont \internal */
};


/** \brief describes the current drawing environment (base fontname ...)
 *  \ingroup jkqtmathtext_tools
 */
struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextEnvironment {
    JKQTMathTextEnvironment();
    /** \brief current font color */
    QColor color;
    /** \brief current font */
    JKQTMathTextEnvironmentFont font;
    /** \brief current font size [pt] */
    double fontSize;
    /** \brief is the text currently bold? */
    bool bold;
    /** \brief is the text currently italic? */
    bool italic;
    /** \brief is the text currently in small caps? */
    bool smallCaps;
    /** \brief is the text currently underlined? */
    bool underlined;
    /** \brief is the text currently overlined? */
    bool overline;
    /** \brief is the text currently stroke through? */
    bool strike;
    /** \brief is the text currently are we inside a math environment? */
    bool insideMath;


    /** \brief build a QFont object from the settings in this object */
    QFont getFont(JKQTMathText* parent) const;
    /** \brief generate a HTML prefix that formats the text after it according to the settings in this object
     *
     * \param defaultEv environment before applying the current object (to detect changes)
     * \see toHtmlAfter()
     */
    QString toHtmlStart(JKQTMathTextEnvironment defaultEv) const;
    /** \brief generate a HTML postfix that formats the text in front of it according to the settings in this object
     *
     * \param defaultEv environment before applying the current object (to detect changes)
     * \see toHtmlAfter()
     */
    QString toHtmlAfter(JKQTMathTextEnvironment defaultEv) const;
};

/** \brief beschreibt die Größe eines Knotens
 *  \ingroup jkqtmathtext_tools
 */
struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextNodeSize {
    JKQTMathTextNodeSize();
    double width;
    double baselineHeight;
    double overallHeight;
    double strikeoutPos;
};

/** \brief summarizes all information available on a font for a specific MTenvironmentFont
 *  \ingroup jkqtmathtext_tools
 *  \see fontDefinitions
 */
struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextFontDefinition {
    JKQTMathTextFontDefinition();
    /** \brief name of the font */
    QString fontName;
    /** \brief specifies the encoding of the font (default is \c MTFEwinSymbol ) */
    JKQTMathTextFontEncoding fontEncoding;

    /** \brief symbol font used for greek symbols, or empty when \a fontName shall be used */
    QString symbolfontGreek;
    /** \brief specifies the encoding of symbolfontGreek */
    JKQTMathTextFontEncoding symbolfontGreekEncoding;
    /** \brief symbol font, used for math symbols, or empty when \a fontName shall be used */
    QString symbolfontSymbol;
    /** \brief specifies the encoding of symbolfontSymbol */
    JKQTMathTextFontEncoding symbolfontSymbolEncoding;
};


/** \brief create a QPainterPath for drawing horizontal braces
 *  \ingroup jkqtmathtext_tools
 *
 *  \image html jkqtmathtext/JKQTMathTextMakeHBracePath.png
 */
JKQTMATHTEXT_LIB_EXPORT QPainterPath JKQTMathTextMakeHBracePath(double x, double ybrace, double width, double bw, double cubicshrink=0.5, double cubiccontrolfac=0.3);


/** \brief create a QPainterPath for drawing horizontal arrows
 *  \ingroup jkqtmathtext_tools
 *
 *  \image html jkqtmathtext/JKQTMathTextMakeArrow.png
 */
JKQTMATHTEXT_LIB_EXPORT QPainterPath JKQTMathTextMakeArrow(double x, double y, double width, double arrowW, bool left=false, bool right=true);


/** \brief create a QPainterPath for drawing horizontal double arrows
 *  \ingroup jkqtmathtext_tools
 *
 *  \image html jkqtmathtext/JKQTMathTextMakeDArrow.png
 */
JKQTMATHTEXT_LIB_EXPORT QPainterPath JKQTMathTextMakeDArrow(double x, double y, double width, double arrowW, bool left=false, bool right=true);


struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextTBRData {
    explicit JKQTMathTextTBRData(const QFont& f, const QString& text, QPaintDevice *pd);
    QFontMetricsF fm;
    QString text;
    QRectF tbr;
    QFont f;
    int ldpiX, ldpiY, pdpiX, pdpiY;
    //QPaintDevice *pd;

    bool operator==(const JKQTMathTextTBRData& other) const;
};

struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextTBRDataH {
    explicit JKQTMathTextTBRDataH(const QFont& f, const QString& text, QPaintDevice *pd);
    QString text;
    QFont f;
    int ldpiX, ldpiY, pdpiX, pdpiY;

    bool operator==(const JKQTMathTextTBRDataH& other) const;
};


#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
inline size_t qHash(const JKQTMathTextTBRDataH& data, size_t /*seed=0*/) {
#else
inline uint qHash(const JKQTMathTextTBRDataH& data) {
#endif
  return qHash(data.f.family())+qHash(data.text);
}


/** \brief calculates the tight bounding rectangle around \a text, uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT QRectF JKQTMathTextGetTightBoundingRect(const QFont &fm, const QString& text,  QPaintDevice *pd);
/** \brief returns a copy of \a f, but with the italic-property set to \c false
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT QFont JKQTMathTextGetNonItalic(const QFont& f);


#endif // JKQTMATHTEXTTOOLS_H









