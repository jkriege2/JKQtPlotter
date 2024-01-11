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
#include <QtMath>
#include <QFontMetrics>
#include <QFontMetricsF>
class JKQTMathText; // forward



/** \brief initialized Qt-ressources necessary for JKQTMathText
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT void initJKQTMathTextResources();


/*! \brief represents a font specifier for JKQTMathText. The font consists of two parts: the actual font and the font used for math output (which may be empty)
    \ingroup jkqtmathtext_tools

    \section JKQTMathTextFontSpecifier_specialSyntax Font Definition Syntax
    JKQTMathTextFontSpecifier::fromFontSpec() defines a special syntax that when parsed allows to seth the font,
    a math-mode alternative:
      - \c FONTNAME : set the text-mode font only
      - \c FONTNAME+MATH_FONTNAME : set the text- and math-mode fonts




    \section JKQTMathTextFontSpecifier_specialNames Special Font Names
    This object also implements replacing special font names with actual fonts. Supported special font names are:
      - \c default / \c app / \c application - the applications default font
      - \c application-sf - a font for "sans", based on the application font
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

    Also some sepcial fonts are defined:
      - xits XITS fonts
      - stix STIX fonts
      - asana ASANA fonts
      - fira Fira fonts
*/
struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextFontSpecifier {
    JKQTMathTextFontSpecifier();
    JKQTMathTextFontSpecifier(const QString& fontName, const QString& mathFontName);
    /** \brief construct a JKQTMathTextFontSpecifier, by parsing a \a fontSpec string as defined in the struct description, see \ref JKQTMathTextFontSpecifier_specialNames . */
    static JKQTMathTextFontSpecifier fromFontSpec(const QString& fontSpec);

    /** \brief initialises the object with values from parsing a \a fontSpec string as defined in the struct description, see \ref JKQTMathTextFontSpecifier_specialNames . */
    void setFontSpec(const QString& fontSpec);

    /** \brief returns the object's constents as a fontSpec string with the form \c "FONT_NAME[+MATH_FONT_NAME]". */
    QString getFontSpec() const;
    /** \copydoc m_fontName */
    QString fontName() const;
    /** \copydoc m_mathFontName */
    QString mathFontName() const;
    /** \copydoc m_fallbackSymbolFont */
    QString fallbackSymbolsFontName() const;

    /** \copydoc m_fontName */
    void setFontName(const QString& name);
    /** \copydoc m_mathFontName */
    void setmathFontName(const QString& name);
    /** \copydoc fallbackSymbolsFontName */
    void setFallbackSymbolsFontName(const QString& name);
    /** \brief finds actual fonts for some predefined special font names, as listed in \ref JKQTMathTextFontSpecifier_specialNames */
    static QString transformFontName(const QString& fontName, bool mathmode=false);
    /** \brief same as transformFontName(), but also finds the actual name for XITS, STIX, ASANA,... */
    static QString transformFontNameAndDecodeSpecialFonts(const QString& fontName, bool mathmode=false);
    /** \brief leiefert \c true, wenn ein fontName() verfügbar ist */
    bool hasFontName() const;
    /** \brief leiefert \c true, wenn ein mathFontName() verfügbar ist */
    bool hasMathFontName() const;
    /** \brief leiefert \c true, wenn ein fallbcakSymbolsFontName() verfügbar ist */
    bool hasFallbackSymbolFontName() const;

    /** \brief initialize with the font-families from the XITS package for text and math */
    static JKQTMathTextFontSpecifier getXITSFamilies();

    /** \brief initialize with the font-families from the XITS package for text and math */
    static JKQTMathTextFontSpecifier getASANAFamilies();

    /** \brief initialize with the font-families from the STIX package for text and math */
    static JKQTMathTextFontSpecifier getSTIXFamilies();

    /** \brief initialize with the font-families from the Fira (Math) package for text and math */
    static JKQTMathTextFontSpecifier getFIRAFamilies();

    /** \brief initialize with the default app font-families for "roman" text and math and tries to find a matching fallback-font. This may be used to initialize serif-fonts
     *
     *  This method encodes some pre-coded knowledge of suitable combinations of fonts for different systems.
     *  e.g. on newer windows systems, the font "Segoe UI" is used for the GUI... a suitable math
     *  font with symbols is "Segoe UI Symbol" ...
     */
    static JKQTMathTextFontSpecifier getAppFontFamilies();
    /** \brief initialize with the default app font-families for "sans-serif" text and math and tries to find a matching fallback-font. This may be used to initialize sans-serif-fonts
     *
     *  This method encodes some pre-coded knowledge of suitable combinations of fonts for different systems.
     *
     *  This function e.g. checks whether the  default app font is sans-serif and then looks for a serif
     *  font for the "sans" font class (so the two can be distinguished) and vice-versa.
     */
    static JKQTMathTextFontSpecifier getAppFontSFFamilies();
private:
    /** \brief specifies the main font name */
    QString m_fontName;
    /** \brief specifies the math font to use in addition to fontName */
    QString m_mathFontName;
    /** \brief specifies a font to be used for fallbackSymbols */
    QString m_fallbackSymbolFont;
    /** \brief if set \c true the fonts are transformed when fontname() or mathFontName() is called by calling transformFontNameAndDecodeSpecialFonts() */
    bool m_transformOnOutput;


};


/** \brief used to specify the font encoding used for drawing
 *  \ingroup jkqtmathtext_tools
*/
enum JKQTMathTextFontEncoding {
    MTFEWinSymbol=0,      /*!< \brief This assumes that symbols shall be taken from a MS Windows style Symbol font */
    MTFEUnicode=1,        /*!< \brief This assumes that symbols shall be taken from a Unicode font, which ideally offers full symbol support  (e.g. the XITS fonts, STIX fonts from <a href="http://www.stixfonts.org/">http://www.stixfonts.org/</a>)*/
    MTFELatin1=2,         /*!< \brief the encoding of a standard Latin1 TTF font (i.e. we can only expect letters,number and not many special characters) */
    MTFEStandard=MTFELatin1
};

/** \brief this function tries to determine the JKQTMathTextFontEncoding of a given font (HEURISTICS!!!)
 *  \ingroup jkqtmathtext_tools
*/
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextFontEncoding estimateJKQTMathTextFontEncoding(QFont font);


/** \brief used to specify how blackboard-fonts are drawn
 *  \ingroup jkqtmathtext_tools
 *
 *  \see JKQTMathTextBlackboradDrawingMode2String(), String2JKQTMathTextBlackboradDrawingMode()
*/
enum JKQTMathTextBlackboradDrawingMode {
    MTBBDMfontDirectly=0,      /*!< \brief draw using the font specified by JKQTMathText::setFontBlackboard() \image html jkqtmathtext/jkqtmathtext_bb_font_directly.png */
    MTBBDMsimulate,      /*!< \brief simulate a blackboard font (i.e. draw the characters' outline only), based on the font specified by JKQTMathText::setFontBlackboard() (e.g. Arial or another sans-serif font is a good choice) \image html jkqtmathtext/jkqtmathtext_bb_simulate.png */
    MTBBDMunicodeCharactersOrFontDirectly,  /*!< \brief use the currently set font and look for special unicode-characters in it, uses the fallbackSymbolFont as fallback, use MTBBDMfontDirectly for characters that are not available \image html jkqtmathtext/jkqtmathtext_bb_unicode_or_font_directly.png */
    MTBBDMunicodeCharactersOrSimulate,  /*!< \brief use the currently set font and look for special unicode-characters in it, uses the fallbackSymbolFont as fallback, use MTBBDMsimulate for characters that are not available \image html jkqtmathtext/jkqtmathtext_bb_unicode_or_simulate.png  */
    MTBBDMdefault=MTBBDMunicodeCharactersOrFontDirectly /*!< \brief default drawing mode, same as MTBBDMunicodeCharactersOrFontDirectly */
};

/** \brief this converts a JKQTMathTextBlackboradDrawingMode into a string
 *  \ingroup jkqtmathtext_tools
 *  \see String2JKQTMathTextBlackboradDrawingMode(), JKQTMathTextBlackboradDrawingMode
*/
JKQTMATHTEXT_LIB_EXPORT QString JKQTMathTextBlackboradDrawingMode2String(JKQTMathTextBlackboradDrawingMode mode);
/** \brief this converts a QString into a JKQTMathTextBlackboradDrawingMode
 *  \ingroup jkqtmathtext_tools
 *  \see JKQTMathTextBlackboradDrawingMode2String(), JKQTMathTextBlackboradDrawingMode
*/
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBlackboradDrawingMode String2JKQTMathTextBlackboradDrawingMode(QString mode);



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
    MTBTTopCorner,  /*!< \brief top-corner brackets  \image html jkqtmathtext/jkqtmathtext_brace_ucorner.png */
    MTBTBottomCorner,  /*!< \brief bottom-corner brackets  \image html jkqtmathtext/jkqtmathtext_brace_lcorner.png */
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
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBraceType TokenName2JKQTMathTextBraceType(const QString& tokenName, bool *isOpening=nullptr);
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
    MTEFallbackSymbols, /*!< \brief symbol font */

    MTECurrentFont,  /*!< \brief internal enum value that specifies that the currently set font shall be used \internal */
    MTECustomFont,  /*!< \brief internal enum value that specifies that a custom font specified elsewhere shall be used \internal */
};


/** \brief describes the current drawing environment (base fontname ...)
 *  \ingroup jkqtmathtext_tools
 */
struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextEnvironment {
    /** \brief units for the property JKQTMathTextEnvironment::fontSize (Points/PT or Pixels) */
    enum FontSizeUnit {
        POINTS,
        PIXELS
    };
    /** \brief convert a FontSizeUnit to a string \see FontSizeUnit,String2FontSizeUnit() */
    static QString FontSizeUnit2String(FontSizeUnit unit);
    /** \brief convert a string into a FontSizeUnit \see FontSizeUnit,FontSizeUnit2String()  */
    static FontSizeUnit String2FontSizeUnit(QString unit);

    JKQTMathTextEnvironment();
    /** \brief current font color */
    QColor color;
    /** \brief current font */
    JKQTMathTextEnvironmentFont font;
    /** \brief custom font, when font==MTECustomFont */
    QString customFontName;
    /** \brief current font size the unit is determined by fontSizeUnit */
    double fontSize;
    /** \brief the unit of the font size fontSize */
    FontSizeUnit fontSizeUnit;

    /** \brief is the text currently bold? */
    bool bold;
    /** \brief is the text currently italic? */
    bool italic;
    /** \brief is the text currently in small caps? */
    QFont::Capitalization capitalization;
    /** \brief is the text currently underlined? */
    bool underlined;
    /** \brief is the text currently overlined? */
    bool overline;
    /** \brief is the text currently stroke through? */
    bool strike;
    /** \brief is the text currently are we inside a math environment? */
    bool insideMath;
    /** \brief if \a insideMath \c ==true and this is \c  true (the default), then digits are forced to be typeset in upright, otherwise they are typeset as defined by the other properties */
    bool insideMathForceDigitsUpright;
    /** \brief if \c true the commands like \c \\frac{}{} are executes as \c \\tfrac{}{} . This implements \c \\textstyle or if \c false \c \\displaystyle
     *
     *  \image html jkqtmathtext/jkqtmathtext_mathstyle.png
     */
    bool insideMathUseTextStyle;
    /** \brief sets  insideMath \c =true and insideMathForceDigitsUpright \c =true and \c insideMathUseTextStyle=!displaystyle*/
    void beginMathMode(bool displaystyle=true);
    /** \brief sets  insideMath \c =false  */
    void endMathMode();
    /** \brief determines whether to use displaystyle or textstyle for math-instructions like \c \\frace{}{} or \c \\int_x
     *
     *  \image html jkqtmathtext/jkqtmathtext_mathstyle.png
     */
    bool isMathDisplayStyle() const;
    /** \brief determines whether to use displaystyle or textstyle for math-instructions like \c \\frace{}{} or \c \\int_x
     *
     *  \image html jkqtmathtext/jkqtmathtext_mathstyle.png
     */
    bool isMathTextStyle() const;


    /** \brief build a <a href="https://doc.qt.io/qt-5/qfont.html">QFont</a> object from the settings in this object */
    QFont getFont(const JKQTMathText *parent) const;
    /** \brief return a copy of this object with the font exchanged for \a font */
    JKQTMathTextEnvironment exchangedFontFor(JKQTMathTextEnvironmentFont font) const;
    /** \brief return a copy of this object with the font exchanged for the matching roman font */
    JKQTMathTextEnvironment exchangedFontForRoman() const;
    /** \brief return the encoding of the given Font */
    JKQTMathTextFontEncoding getFontEncoding(JKQTMathText *parent) const;
    /** \brief generate a HTML prefix that formats the text after it according to the settings in this object
     *
     * \param defaultEv environment before applying the current object (to detect changes)
     * \param parentMathText the JKQTMathText object currently in use (used to e.g. look up font names)
     *
     * \see toHtmlAfter()
     */
    QString toHtmlStart(JKQTMathTextEnvironment defaultEv, JKQTMathText *parentMathText) const;
    /** \brief generate a HTML postfix that formats the text in front of it according to the settings in this object
     *
     * \param defaultEv environment before applying the current object (to detect changes)
     * \param parentMathText the JKQTMathText object currently in use (used to e.g. look up font names)
     *
     * \see toHtmlAfter()
     */
    QString toHtmlAfter(JKQTMathTextEnvironment defaultEv, JKQTMathText *parentMathText) const;
};

/** \brief beschreibt die Größe(n) eines Knotens
 *  \ingroup jkqtmathtext_tools
 */
struct JKQTMATHTEXT_LIB_EXPORT JKQTMathTextNodeSize {
    JKQTMathTextNodeSize(double width=0, double baselineHeight=0, double overallHeight=0, double strikeoutPos=0, double baselineXCorrection=0,double topXCorrection=0);
    /** \brief width of whole block */
    double width;
    /** \brief baselineHeight of whole block, i.e. the ascent */
    double baselineHeight;
    /** \brief overallHeight of whole block */
    double overallHeight;
    /** \brief strikeoutPos of whole block, i.e. distance of the strikeout position from the baseline */
    double strikeoutPos;
    /** \brief x-correction (<0 = move to the left) for subscripts, i.e. approximately at the height of the baseline
     *
     *  \image html jkqtmathtext/jkqtmathtext_doc_subsuper_italiccorrection.png
     *
     *  \image html jkqtmathtext/jkqtmathtext_doc_subsuper_italiccorrection_boxes.png
     */
    double baselineXCorrection;
    /** \brief x-correction (>0 = move to the right) for superscripts, i.e. approximately at the top  of the box */
    double topXCorrection;
    /** \brief calculate the descent (overallHeight-baselineHeight) */
    inline double getDescent() const { return overallHeight-baselineHeight; }
    /** \brief calculate the ascent (=baselineHeight), for convenience and naming */
    inline double getAscent() const { return baselineHeight; }
    /** \brief calculate the height (=baselineHeight), for convenience and naming */
    inline double getHeight() const { return overallHeight; }
    /** \brief calculate the overall size in floating-point precision */
    inline QSizeF getSize() const { return QSizeF(width, overallHeight); }
    /** \brief calculate the overall size in floating-point precision */
    inline QSize getIntSize() const { return QSize(qCeil(width+1.0), qCeil(overallHeight+1.0)); }
    /** \brief helper function, which generates a copy of this object, used to suppress slicing warning due to GSL E.63: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-slice */
    inline JKQTMathTextNodeSize sliceToNodeSize() const { return *this; }
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
};


/** \brief create a QPainterPath for drawing horizontal braces, use QPainter::fillPath() with a vanishing line-width to draw this
 *  \ingroup jkqtmathtext_tools
 *
 *  \image html jkqtmathtext/JKQTMathTextMakeHBracePath.png
 *
 *  \param x x-center-position of the brace
 *  \param ybrace y-center-position of the brace
 *  \param width with of the overall brace
 *  \param bw height of the brace
 *  \param lineWidth linewidth when drawing, used for correcting so the brace exactly fills the rectangle and not overshoots it
 *  \param cubicshrink
 *  \param cubiccontrolfac
 *  \param lineWidthShrinkFactor the width of the tips is lineWidth reduced by this factor
 *  \param lineWidthGrowFactor the width of the horizontal bars is increased by this factor from lineWidth
 */
JKQTMATHTEXT_LIB_EXPORT QPainterPath JKQTMathTextMakeHBracePath(double x, double ybrace, double width, double bw, double lineWidth, double cubicshrink=0.5, double cubiccontrolfac=0.3, double lineWidthShrinkFactor=0.6, double lineWidthGrowFactor=0.9);


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

/** \brief draw a given \a txt in the font \a f using additional informaion (but not currentEv::getFont() ) from \a currentEv at (\a x , \a y ) using the given \a painter
 *
 *  This function implements drawing of synthesized fonts, e.g. MTEblackboard when JKQTMathText::isFontBlackboardSimulated() is \c true .
 *
 *  example output:
 *    \image html jkqtmathtext/jkqtmathtext_bb_unicode_or_simulate.png
 */
JKQTMATHTEXT_LIB_EXPORT void JKQTMathTextDrawStringSimBlackboard(QPainter& painter, const QFont& f, const QColor &color, double x, double y, const QString& txt);



/** \brief calculates the tight bounding rectangle around \a text
 *         (from <a href="https://doc.qt.io/qt/qfontmetricsf.html#tightBoundingRect">QFontMetricsF::tightBoundingRect()</a>),
 *         uses internal (thread-local) hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font the text should be set in
 *  \param text the text of which the properties are calculated
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT QRectF JKQTMathTextGetTightBoundingRect(const QFont &fm, const QString& text,  QPaintDevice *pd);

/** \brief calculates the bounding rectangle around \a text
 *         (from (using <a href="https://doc.qt.io/qt/qfontmetricsf.html#boundingRect">QFontMetricsF::boundingRect()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font the text should be set in
 *  \param text the text of which the properties are calculated
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT QRectF JKQTMathTextGetBoundingRect(const QFont &fm, const QString& text,  QPaintDevice *pd);

/** \brief calculates the horizontal advance of \a text
 *         (from <a href="https://doc.qt.io/qt/qfontmetricsf.html#horizontalAdvance">QFontMetricsF::horizontalAdvance()</a>
 *         or \c QFontMetricsF::width() if it is not yet available in your Qt version),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font the text should be set in
 *  \param text the text of which the properties are calculated
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetHorAdvance(const QFont &fm, const QString& text,  QPaintDevice *pd);

/** \brief calculates the left bearing of \a text
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#leftBearing">QFontMetricsF::leftBearing()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font the text should be set in
 *  \param text the character of which the properties are calculated
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetLeftBearing(const QFont &fm, const QChar& text,  QPaintDevice *pd);

/** \brief calculates the right bearing of \a text
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#rightBearing">QFontMetricsF::rightBearing()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font the text should be set in
 *  \param text the character of which the properties are calculated
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetRightBearing(const QFont &fm, const QChar& text,  QPaintDevice *pd);


/** \brief calculates the strikeout-pos of \a font
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#strikeoutPos">QFontMetricsF::strikeoutPos()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font for which to calculate
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetFontStrikoutPos(const QFont &fm,  QPaintDevice *pd);

/** \brief calculates the line width of \a font
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#lineWidth">QFontMetricsF::lineWidth()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font for which to calculate
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetFontLineWidth(const QFont &fm,  QPaintDevice *pd);

/** \brief calculates the ascent of \a font
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#ascent">QFontMetricsF::ascent()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font for which to calculate
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetFontAscent(const QFont &fm,  QPaintDevice *pd);

/** \brief calculates the descent of \a font
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#descent">QFontMetricsF::descent()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font for which to calculate
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetFontDescent(const QFont &fm,  QPaintDevice *pd);


/** \brief calculates the height of \a font
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#height">QFontMetricsF::height()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font for which to calculate
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetFontHeight(const QFont &fm,  QPaintDevice *pd);

/** \brief calculates the leading of \a font
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#leading">QFontMetricsF::leading()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font for which to calculate
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetFontLeading(const QFont &fm,  QPaintDevice *pd);

/** \brief calculates the line spacing of \a font
 *         (from <a href="https://doc.qt.io/qt-6/qfontmetricsf.html#lineSpacing">QFontMetricsF::lineSpacing()</a>),
 *         uses internal hashing to not redo a calculation that has already been performed
 *  \ingroup jkqtmathtext_tools
 *
 *  \param fm font for which to calculate
 *  \param pd (or \c nullptr) the currently used <a href="https://doc.qt.io/qt-6/qpaintdevice.html">QPaintDevice</a>
 *            (e.g. from <a href="https://doc.qt.io/qt/qpainter.html#device">QPainter::device()</a> )
 *
 *  \note This function is thread-safe and uses the same cache for all threads (so they profit from eachother)
 */
JKQTMATHTEXT_LIB_EXPORT qreal JKQTMathTextGetFontLineSpacing(const QFont &fm,  QPaintDevice *pd);


/** \brief returns a copy of \a f, but with the italic-property set to \c false
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT QFont JKQTMathTextGetNonItalic(const QFont& f);


/** \brief types of horizontal alignment
 *  \ingroup jkqtmathtext_tools
 *
 *  \image html jkqtmathtext_horizontalalignment.png
 *
 *  \see JKQTMathTextVerticalOrientation2String(), String2JKQTMathTextVerticalOrientation(), JKQTMathTextVerticalListNode
 */
enum JKQTMathTextHorizontalAlignment {
    MTHALeft,  /*!< \brief align left */
    MTHACentered,  /*!< \brief align centered */
    MTHARight,  /*!< \brief align right */
};
/** \brief convert a JKQTMathTextHorizontalAlignment into a string
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT QString JKQTMathTextHorizontalAlignment2String(JKQTMathTextHorizontalAlignment type);
/** \brief convert a string \a tokenName into a JKQTMathTextHorizontalAlignment
 *  \ingroup jkqtmathtext_tools
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextHorizontalAlignment String2JKQTMathTextHorizontalAlignment(QString tokenName);

/** \brief type of ffractions represented by JKQTMathTextFracNode
 *  \ingroup jkqtmathtext_tools
 *
 *  \image html jkqtmathtext_verticalorientation.png
 *
 *  \see JKQTMathTextVerticalOrientation2String(), String2JKQTMathTextVerticalOrientation(), JKQTMathTextVerticalListNode, JKQTMathTextVerbatimNode
 */
enum JKQTMathTextVerticalOrientation {
    MTVOTop,  /*!< \brief baseline of the whole block is at the top of the first */
    MTVOFirstLine,  /*!< \brief baseline of the whole block is at the baseline of the first line */
    MTVOCentered,  /*!< \brief baseline of the whole block is at the center of all lines */
    MTVOLastLine,  /*!< \brief baseline of the whole block is at the baseline of the last line */
    MTVOBottom,  /*!< \brief baseline of the whole block is at the bottom of the last line */
};

/** \brief convert a JKQTMathTextVerticalOrientation into a QString
 *  \ingroup jkqtmathtext_tools
 *
 *  \see JKQTMathTextVerticalOrientation2String(), String2JKQTMathTextVerticalOrientation(), JKQTMathTextVerticalListNode, JKQTMathTextVerbatimNode
 */
JKQTMATHTEXT_LIB_EXPORT QString JKQTMathTextVerticalOrientation2String(JKQTMathTextVerticalOrientation mode);

/** \brief returns the JKQTMathTextVerticalOrientation corresponding to \a instructionName
 *  \ingroup jkqtmathtext_tools
 *
 *  \see JKQTMathTextVerticalOrientation2String(), String2JKQTMathTextVerticalOrientation(), JKQTMathTextVerticalListNode, JKQTMathTextVerbatimNode
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextVerticalOrientation String2JKQTMathTextVerticalOrientation(QString mode);


/** \brief defines, how lines are beeing spaced by the node
 *  \ingroup jkqtmathtext_tool
 *
 *  \image html jkqtmathtext_verticallist.png
 *
 *  \see JKQTMathTextLineSpacingMode2String(), String2JKQTMathTextLineSpacingMode(), JKQTMathTextVerticalListNode
 */
enum JKQTMathTextLineSpacingMode {
    MTSMDefaultSpacing, /*!< space the lines with equilibrated spacing, i.e. the baselines are at least \c QFontMetricsF::lineSpacing()*JKQTMathTextVerticalListNode::lineSpacingFactor apart, but even more, if the height of the text bloxk is larger than the the font's ascent+descent */
    MTSMMinimalSpacing /*!< space the lines as tight as possible, i.e. each line is separated by \c QFontMetricsF::leading()*JKQTMathTextVerticalListNode::lineSpacingFactor from the next line. This is a s compact as possible */
};
/** \brief convert a SpacingMode to a String
 *  \ingroup jkqtmathtext_tools
 *
 *  \see JKQTMathTextLineSpacingMode2String(), String2JKQTMathTextLineSpacingMode(), JKQTMathTextVerticalListNode
 */
JKQTMATHTEXT_LIB_EXPORT QString JKQTMathTextLineSpacingMode2String(JKQTMathTextLineSpacingMode mode);
/** \brief convert a String to a SpacingMode
 *  \ingroup jkqtmathtext_tools
 *
 *  \see JKQTMathTextLineSpacingMode2String(), String2JKQTMathTextLineSpacingMode(), JKQTMathTextVerticalListNode
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextLineSpacingMode String2JKQTMathTextLineSpacingMode(QString mode);

#endif // JKQTMATHTEXTTOOLS_H









