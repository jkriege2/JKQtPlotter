/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)
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



#ifndef JKQTMATHTEXT_H
#define JKQTMATHTEXT_H

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



/** \brief initialized Qt-ressources necessary for JKQTMathText
 *  \ingroup jkqtmathtext
 */
JKQTMATHTEXT_LIB_EXPORT void initJKQTMathTextResources();


/*! \brief represents a font specifier for JKQTMathText. The font consists of two parts: the actual font and the font used for math output (which may be empty)
    \ingroup jkqtmathtext

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


/*! \brief this class parses a LaTeX string and can then draw the contained text/equation onto a <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>
    \ingroup jkqtmathtext

    
    JKQTMathText is a self-contained LaTeX-renderer for Qt. It is used to renderer
    labels in JKQTPlotter/JKQTBasePlotter, but can be used independently. 
    The class does not depend on any library, except Qt. 
    In particular it actually parses a LaTeX string and draws it in pure C++. It does NOT rely
    on an installed LaTeX for the rendering!
    
    \section JKQTMathTextUsage Usage
    \subsection JKQTMathTextUsageDirect Direct Usage
    This small piece of C++ code may serve as an example of the usage and capabilities of the class:
    \code
    // create a JKQTMathText object.
    JKQTMathText mathText;

    // configure its properties to influence the rendering (e.g. fonts to use, font size, ...)
    mathText.useXITS();
    mathText.setFontSize(20);

    // parse some LaTeX code (the Schroedinger's equation)
    mathText.parse("$\\left[-\\frac{\\hbar^2}{2m}\\frac{\\partial^2}{\\partial x^2}+V(x)\\right]\\Psi(x)=\\mathrm{i}\\hbar\\frac{\\partial}{\\partial t}\\Psi(x)$");

    // use the draw() methods to draw the equation using a QPainter (here onto a QPixmap)
    QPainter painter;
    QPixmap pix(600,400);
    painter.begin(&pix);
    mathText.draw(painter, Qt::AlignCenter, QRectF(0,0,pix.width(), pix.height()), false);
    painter.end();
    \endcode
    
    \subsection JKQTMathTextSizing Determining the size of an equation
    
    In addition there are also functions that allow to calculate the size of the equation, before drawing it (just like the functions in <a href="http://doc.qt.io/qt-5/qfontmetrics.html">QFontMetrics</a> and  <a href="http://doc.qt.io/qt-5/qfontmetricsf.html">QFontMetricsF</a>):
      - getSizeDetail()
      - getSize()
      - getAscent(), getDescent()
    .
    
    \subsection JKQTMathTextErrorHandling Error Handling
    
    The class is designed to be as robust as possible and will still return some output, even if the equation contains some errors.
    Nevertheless, several errors are detected while parsing. You can get a list of error messages using getErrorList() after calling parse().
    Also parse() will return \c false if an error occured while parsing.

    
    \subsection JKQTMathTextUsageQLabel Usage within a QLabel class JKQTMathTextLabel
    
    Finally, there is also a QLabel-derived class JKQTMathTextLabel which can be used for drawing a LaTeX string onto a Qt form.
    
    \see JKQTMathTextLabel

    
    \section JKQTMathTextExamples Examples

    Examples for the usage of this class can be found here: 
      - \ref JKQTMathTextSimpleExample 
    .


    \section JKQTMathTextSuppoertedLaTeX Supported LaTeX Subset
    \subsection JKQTMathTextSuppoertedLaTeXSimple Simple Instructions
    The supported LaTeX subset is listes below. Please note that some commands are used differently than in actual LaTeX.
    For example \c \\bf is just a renamed form of \c \\textbf and used as \c \\bf{...} and NOT as \c {\\bf...} .

      - \c $ : enter/leave math mode
      - \c \\bf{...} \c \\textbf{...} \c \\mathbf{...}:  draw the contained text in bold face \image html jkqtmathtext_bold.png
      - \c \\it{...} \c \\textit{...} \c \\mathit{...} :  draw the contained text in italic face \image html jkqtmathtext_italic.png
      - \c \\rm{...} \c \\textrm{...} \c \\mathrm{...} \c \\mbox{...} :  draw the contained text in normal upright roman font face \image html jkqtmathtext_fonts.png
      - \c \\sf{...} \c \\textsf{...} \c \\mathsf{...} :  draw the contained text in normal upright sans-serif font face \image html jkqtmathtext_fonts.png
      - \c \\script{...} \c \\textscript{...} \c \\mathscript{...} :  draw the contained text in a script font face \image html jkqtmathtext_fonts.png
      - \c \\sc{...} : draw the text in small caps \image html jkqtmathtext_sc.png
      - \c \\ul{...} \c \\underline{...} \c \\underlined{...} : draw the text with underlining \image html jkqtmathtext_ul.png
      - \c \\ol{...} \c \\overline{...} \c \\overlined{...} : draw the text with overlining \image html jkqtmathtext_ol.png
      - \c \\tt{...} \c \\texttt{...} \c \\mathtt{...} : draw text in typewriter font \image html jkqtmathtext_fonts.png
      - \c \\textcolor{color}{...} \c \\color{color} \c \\mathcolor{color}{...} : draw colored text \image html jkqtmathtext_colored.png
      - \c \\boxed{...} : draw text with a box around it \image html jkqtmathtext_boxed.png
      - \c \\colorbox{color}{...} : draw a colored box around text \image html jkqtmathtext_colorbox.png
      - \c \\alpha ... : display the according greek letter \image html jkqtmathtext_greek.png
      - \c ^{...} \c _{...} : display the contents of braces in superscript/subscript \image html jkqtmathtext_supersub.png
        <br>Special subscript/superscript typesetting applies, when the sub/super follows \c \\sum \c \\Prod ...: \image html jkqtmathtext_specialsubsuper.png
      - \c \\{ / \\} : display opening/closing brace
      - \c \\_ : display underscore
      - \c \\sum \c \\prod \c \\int ... : plot special symbol. Note that depending on the fontEncoding the available
                                          symbols may differ (there are not all symbols defined in the MS Windows Symbol
                                          font!). Best coverage should be given by Unicode font encoding with a good
                                          unicode font installed!<br>\image html jkqtmathtext_symbols.png
      - <code>\\vec{x} \\dot{x} \\ddot{x} \\overline{x} \\underline{x} \\hat{x} \\tilde{x} \\uul{x} \\ool{x} \\bar{x} \\arrow{x} </code>: Decorations over/under symbols \image html jkqtmathtext_mathdeco.png
      - <code>\\verb{don't parse this _aaa\\LaTeX} </code>: interpret enclosed text as verbose \image html jkqtmathtext_verb.png
    .

    \subsection JKQTMathTextSuppoertedLaTeXBraces Braces ...
    Braces in math mode are adjusted in size, so they are a small bit (factor \c brace_factor ) larger than the contents.
    To enable this you have to write braces with \c \\left and \c \\right. These types of braces are defined (slight
    differences to LaTeX standard):
      - \c \\left( \c \\right) : default meaning (), \image html jkqtmathtext_brace_round.png
      - \c \\left[ \c \\right] : default meaning [], \image html jkqtmathtext_brace_rect.png
      - \c \\left\\{ \c \\right\\} : default meaning {}, \image html jkqtmathtext_brace_curly.png
      - \c \\left< \c \\right> : "averaging" braces , \image html jkqtmathtext_brace_tri.png
      - \c \c \\left\\lfloor \c \\right\\rfloor : floor braces , \image html jkqtmathtext_brace_floor.png
      - \c \\left~ \c \\right~ \c \\left\\lceil \c \\right\\rceil : ceil braces , \image html jkqtmathtext_brace_ceil.png
      - \c \\left| \c \\right| : absolute value braces | |, \image html jkqtmathtext_brace_oneline.png
      - \code \left\| \right\| \endcode : norm braces || ||, \image html jkqtmathtext_brace_dblline.png
      - You can use \c \\left. or \c \\right. to have only right or only left brace
    .

    \subsection JKQTMathTextSuppoertedLaTeXUnderOver Undersetting, Oversetting, Underbraces, Overbraces ...
    There are also instructions that allow to under/overset braces, arrows, ...:
      - <code>$\\underbrace{x+x+...+x}{k\\ \\mathrm{times}}$</code> \image html jkqtmathtext_brace_underbrace.png
      - <code>$\\overbrace{x+x+...+x}{k\\ \\mathrm{times}}$</code> \image html jkqtmathtext_brace_overbrace.png
      - <code>$\\overset{main}{over}$</code> \image html jkqtmathtext_brace_overset.png
      - <code>$\\underset{main}{under}$</code> \image html jkqtmathtext_brace_underset.png
    .

    \subsection JKQTMathTextSuppoertedLaTeXMatrix Matrix/Array Type Instructions
    Several Matrix/Array-typed LaTeX instructions are supported:
      - <code>$\\frac{...}{...}$</code>, <code>$\\tfrac{...}{...}$</code> (70% smaller font), <code>$\\dfrac{...}{...}$</code> \image html jkqtmathtext_brace_frac.png
      - <code>$\\sfrac{...}{...}$</code> \image html mathparser/MTFMsfrac.png
      - <code>$\\stfrac{...}{...}$</code> (70% smaller font) \image html mathparser/MTFMstfrac.png
      - <code>$\\stackrel{...}{...}$ $\\binom{...}{...}$</code> \image html jkqtmathtext_brace_stackrel.png
      - <code>$\\begin{cases} ... & ... \\\\ ... & ... \\end{cases}$</code> \image html jkqtmathtext_brace_begincases.png
      - <code>$\\begin{array} a & b & ...\\\\ c & d & ...\\end{array}$</code> <code>$\\begin{matrix} a & b & ...\\\\ c & d & ...\\end{matrix}$</code> \image html jkqtmathtext_array.png
      - <code>$\\begin{pmatrix} a & b & ...\\\\ c & d & ...\\end{pmatrix}$</code> \image html jkqtmathtext_pmatrix.png
      - <code>$\\begin{bmatrix} a & b & ...\\\\ c & d & ...\\end{bmatrix}$</code> \image html jkqtmathtext_bmatrix.png
      - <code>$\\begin{Bmatrix} a & b & ...\\\\ c & d & ...\\end{Bmatrix}$</code> \image html jkqtmathtext_bbmatrix.png
      - <code>$\\begin{vmatrix} a & b & ...\\\\ c & d & ...\\end{vmatrix}$</code> \image html jkqtmathtext_vmatrix.png
      - <code>$\\begin{Vmatrix} a & b & ...\\\\ c & d & ...\\end{Vmatrix}$</code> \image html jkqtmathtext_vvmatrix.png
    .

    \section JKQTMathTextSuppoertedFonts Font Handling
    
    Several fonts are defined as properties to the class:
      - A "roman" font used as the standard font ( setFontRoman()  )
      - A "sans-serif" font which may be activated with \c \\sf ... ( setFontSans()  )
      - A "typewriter" font which may be activated with \c \\tt ... ( setFontTypewriter() )
      - A "script" font which may be activated with \c \\script ... ( setFontScript() )
      - A greek font which is used to display greek letters \c \\alpha ... ( setSymbolfontGreek() )
      - A symbol font used to display special (math) symbols. ( setSymbolfontSymbol() )
      - A "roman" font used as the standard font in math mode ( setFontMathRoman() )
      - A "sans-serif" used as sans serif font in math mode ( setFontMathSans() )
      - A "blackboard" font used to display double stroked characters ( setFontBlackboard() )
      - A "caligraphic" font used to display caligraphic characters ( setFontCaligraphic() )
    .
    
    These fonts are generic font classes, which font is actually used can be configured in JKQTMathText class with the \c set...() functions mentioned above. You can also use these functions to set the fonts used for math rendering in math-mode:
      - useSTIX() use the STIX fonts from <a href="https://www.stixfonts.org/">https://www.stixfonts.org/</a> in math-mode<br>\image html jkqtmathparser_stix.png
      - useXITS() use the XITS fonts from <a href="https://github.com/alif-type/xits">https://github.com/alif-type/xits</a> in math-mode. These are included by default in this library and also activated by default.<br>\image html jkqtmathparser_xits.png
      - useASANA() use the ASANA fonts from <a href="https://ctan.org/tex-archive/fonts/Asana-Math/">https://ctan.org/tex-archive/fonts/Asana-Math/</a> in math-mode<br>\image html jkqtmathparser_asana.png
      - useAnyUnicode() use generic Unicode fonts, e.g. "Arial" and "Times New Roman" in math-mode. You should use fonts that contain as many of the mathematical symbols as possible to ensure good rendering results.<br>using "Times New Roman": \image html jkqtmathparser_timesnewroman.png
        <br>using "Arial": \image html jkqtmathparser_arial.png
        <br>using "Courier New": \image html jkqtmathparser_couriernew.png
        <br>using "Comic Sans MS": \image html jkqtmathparser_comicsans.png
    .


    Math-mode is activated by enclosing your equation in \c $...$ or \c \\[...\\] . This mode is optimized for mathematical equations. Here is an example of the difference:
      - <b>math-mode (XITS fonts are used, whitespaces are mostly not drawn directly, symbol spacing is different)</b> \c $...$: <br>\image html jkqtmathparser_schreq_mathmode.png
      - <b>normal mode (Times new Roman is used, whitespaces are evaluated directly)</b>: <br>\image html jkqtmathparser_schreq_normalmode.png
    .
    

    \section JKQTMathTextToHTML Convert to HTML
    
    The method toHtml() may be used to get a HTML representation of the LaTeX string, if possible (only for simple LaTeX equations!). Whether
    the transformation was possible is returned as a call by value argument!


 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathText : public QObject {
        Q_OBJECT
    public:
        /** \brief minimum linewidth allowed in a JKQTMathText (given in pt) */
        static const double ABS_MIN_LINEWIDTH;


        /** \brief class constructor */
        JKQTMathText(QObject * parent = nullptr);
        /** \brief class destructor */
        ~JKQTMathText();
        /** \brief load the object settings from the given QSettings object with the given name prefix */
        void loadSettings(const QSettings& settings, const QString& group=QString("mathtext/"));
        /** \brief store the object settings to the given QSettings object with the given name prefix */
        void saveSettings(QSettings& settings, const QString& group=QString("mathtext/")) const;
        /** \brief parse the given enhanced string. Returns \c true on success. */
        bool parse(QString text);
        /** \brief get the size of the drawn representation. returns an invalid size if no text has been parsed. */
        QSizeF getSize(QPainter& painter);
        /** \brief return the descent, i.e. the distance from the baseline to the lowest part of the representation */
        double getDescent(QPainter& painter);
        /** \brief return the ascentt, i.e. the distance from the baseline to the highest part of the representation */
        double getAscent(QPainter& painter);
        /** \brief return the detailes sizes of the text */
        void getSizeDetail(QPainter& painter, double& width, double& ascent, double& descent, double& strikeoutPos);
        /** \brief draw a representation to the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> object at the specified position */
        void draw(QPainter& painter, double x, double y, bool drawBoxes=false);
        /** \brief overloaded version of draw(QPainter& painter, double x, double y).
         *
         *  This version draws the text inside the given rectangle according to the specified flags.
         */
        void draw(QPainter& painter, unsigned int flags, QRectF rect, bool drawBoxes=false);


        /** \brief convert LaTeX to HTML. returns \c ok=true on success and \c ok=false else. */
        QString toHtml(bool* ok=nullptr, double fontPointSize=10);

        /*! \brief used to specify the font encoding used for drawing
        */
        enum MTfontEncoding {
            MTFEwinSymbol,      /*!< \brief This assumes that symbols shall be taken from a MS Windows style Symbol font */
            MTFEunicode,        /*!< \brief This assumes that symbols shall be taken from a Unicode font  (e.g. the STIX fonts from <a href="http://www.stixfonts.org/">http://www.stixfonts.org/</a>)*/
            MTFEunicodeLimited, /*!< \brief This assumes that the fonts used are Unicode, but only offer a limited set of symbols.  Especially math symbols are missing from this encoding */
            MTFEStandard,       /*!< \brief the encoding of a standard TTF font (i.e. we can only expect letters,number and not many special characters) */
        };

        /** \brief convert MTfontEncoding to a string */
        static QString encoding2String(MTfontEncoding e);


        /** \brief the available logical fonts (default is MTEroman)  */
        enum MTenvironmentFont {
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


        /** \copydoc fontColor */ 
        void setFontColor(const QColor & __value);
        /** \copydoc fontColor */ 
        QColor getFontColor() const;
        /** \copydoc fontSize */ 
        void setFontSize(double __value);
        /** \copydoc fontSize */ 
        double getFontSize() const;
        /** \brief add a font pair to the table with font replacements
         *
         * e.g. if it is known that a certain font is not good for rendering, you can add an alternative with this function.
         * These are automatically applied, when setting a new font name!
         *
         * \param nonUseFont the font not to use
         * \param useFont replacement font for nonUseFont
         *
         * The entry in the encodings for this font is kept empty (or even deleted), so the default encoding of the font to be replaced is used!
         */
        void addReplacementFont(const QString& nonUseFont, const QString& useFont);
        /** \brief add a font pair to the table with font replacements
         *
         * e.g. if it is known that a certain font is not good for rendering, you can add an alternative with this function.
         * These are automatically applied, when setting a new font name!
         *
         * \param nonUseFont the font not to use
         * \param useFont replacement font for nonUseFont
         * \param useFontEncoding encoding of the replacement font
         */
        void addReplacementFont(const QString& nonUseFont, const QString& useFont, MTfontEncoding useFontEncoding);
        /** \brief retrieves a replacement for the given font name \a nonUseFont, including its encoding. Returns the given default values \a defaultFont and/or \a defaultFontEncoding if one of the two is not found */
        QPair<QString, MTfontEncoding> getReplacementFont(const QString &nonUseFont, const QString &defaultFont, MTfontEncoding defaultFontEncoding) const;

        enum class FontSubclass {
            Text,
            Default=Text,
            Symbols,
            Greek,
        };

        /** \brief retrieve the font and encoding to be used for \a font, which might optionally be typeset inside a math environment, specified by in_math_environment, possibly for the given font subclass \a subclass */
        QPair<QString, MTfontEncoding> getFontData(MTenvironmentFont font, bool in_math_environment=false, FontSubclass subclass=FontSubclass::Default) const;

        /*! \brief calls setFontRoman(), or calls useXITS() if \a __value \c =="XITS".  calls useSTIX() if \a __value \c =="STIX", ...

            \see setFontRoman(), useXITS(), useSTIX() for more information */
        void setFontRomanOrSpecial(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /*! \brief calls setFontRoman(), or calls useXITS() if \a __value \c =="XITS".  calls useSTIX() if \a __value \c =="STIX", ...

            \see setFontRoman(), useXITS(), useSTIX() for more information */
        void setFontRomanOrSpecial(const JKQTMathTextFontSpecifier & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);

        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEroman   */
        void setFontRoman(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEroman   */
        QString getFontRoman() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEsans   */
        void setFontSans(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEsans   */
        QString getFontSans() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEtypewriter   */
        void setFontTypewriter(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEtypewriter   */
        QString getFontTypewriter() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEscript   */
        void setFontScript(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEscript   */
        QString getFontScript() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEfraktur   */
        void setFontFraktur(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEfraktur   */
        QString getFontFraktur() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEcaligraphic   */
        void setFontCaligraphic(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEcaligraphic   */
        QString getFontCaligraphic() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEblackboard   */
        void setFontBlackboard(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief blackboard font is simulated by using roman with outlines only  */
        void setFontBlackboardSimulated(bool doSimulate);
        /** \brief is blackboard font simulated by using roman with outlines only  */
        bool isFontBlackboardSimulated() const;
        /** \brief retrieves the font to be used for text in the logical font MTEblackboard   */
        QString getFontBlackboard() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for greek letters in the logical font \a font   */
        void setSymbolfontGreek(MTenvironmentFont font, const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for integrals in all logical fonts   */
        void setSymbolfontGreek(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for greek letters in the logical font \a font   */
        QString getSymbolfontGreek(MTenvironmentFont font) const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for symbols in the logical font \a font   */
        void setSymbolfontSymbol(MTenvironmentFont font, const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for integrals in all logical fonts   */
        void setSymbolfontSymbol(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for symbols in the logical font \a font   */
        QString getSymbolfontSymbol(MTenvironmentFont font) const;

        /** \brief retrieves the encoding used for the symbol font to be used for symbols in the logical font \a font   */
        MTfontEncoding getSymbolfontEncodingSymbol(MTenvironmentFont font) const;
        /** \brief retrieves the encoding used for the greek letter font to be used for symbols in the logical font \a font   */
        MTfontEncoding getSymbolfontEncodingGreek(MTenvironmentFont font) const;
        /** \brief retrieves the encoding used for the script font  */
        MTfontEncoding getFontEncodingScript() const;
        /** \brief retrieves the encoding used for the Fraktur font  */
        MTfontEncoding getFontEncodingFraktur() const;
        /** \brief retrieves the encoding used for the typewriter font  */
        MTfontEncoding getFontEncodingTypewriter() const;
        /** \brief retrieves the encoding used for the sans-serif font  */
        MTfontEncoding getFontEncodingSans() const;
        /** \brief retrieves the encoding used for the roman font  */
        MTfontEncoding getFontEncodingRoman() const;
        /** \brief retrieves the encoding used for the blackboard font  */
        MTfontEncoding getFontEncodingBlackboard() const;
        /** \brief retrieves the encoding used for the caligraphic font  */
        JKQTMathText::MTfontEncoding getFontEncodingCaligraphic() const;


        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEmathRoman   */
        void setFontMathRoman(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEroman   */
        QString getFontMathRoman() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEmathSans   */
        void setFontMathSans(const QString & fontName, MTfontEncoding encoding=MTfontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEsans   */
        QString getFontMathSans() const;
        /** \brief retrieves the encoding used for the math-mode sans-serif font  */
        MTfontEncoding getFontEncodingMathSans() const;
        /** \brief retrieves the encoding used for the math-mode roman font  */
        MTfontEncoding getFontEncodingMathRoman() const;

        /** \brief configures the class to use the STIX fonts in mathmode
         *
         * use STIX (1.x/2.x) fonts from <a href="https://www.stixfonts.org/">https://www.stixfonts.org/</a> in math-mode
         *
         * \image html jkqtmathparser_stix.png
         */
        bool useSTIX(bool mathModeOnly=true);

        /** \brief configures the class to use the XITS fonts in mathmode
         *
         * use XITS fonts from <a href="https://github.com/alif-type/xits">https://github.com/alif-type/xits</a> in math-mode.
         * These are included by default in this library and also activated by default.
         *
         * \image html jkqtmathparser_xits.png
         *
         * \note The XITS fonts can be compiled into JKQTPlotter, when the CMake-option \c is set to ON (default: ON).
         *       Then the XITS fonts are added as Qt-Ressources to the library binary.
         *       If this is not the case, you have to provide the XITS fonts on the target system by other means, if you want
         *       to use them.
         */
        bool useXITS(bool mathModeOnly=true);

        /** \brief configures the class to use the ASANA fonts in mathmode
         *
         * use the ASANA fonts from <a href="https://ctan.org/tex-archive/fonts/Asana-Math/">https://ctan.org/tex-archive/fonts/Asana-Math/</a> in math-mode
         *
         * \image html jkqtmathparser_asana.png
         */
        bool useASANA(bool mathModeOnly=true);

        /** \brief sets \a timesFont (with its encoding \a encodingTimes ) for serif-text and \a sansFont (with its encoding \a encodingSans ) for both mathmode and textmode fonts
         *
         * use generic Unicode fonts, e.g. "Arial" and "Times New Roman" in math-mode.
         * You should use fonts that contain as many of the mathematical symbols as possible to ensure good rendering results.
         *
         * <code>setAnyUnicode("Times New Roman", "Times New Roman")</code>:<br>\image html jkqtmathparser_timesnewroman.png  <br><br>
         * <code>setAnyUnicode("Arial", "Arial")</code>:<br>\image html jkqtmathparser_arial.png  <br><br>
         * <code>setAnyUnicode("Courier New", "Courier New")</code>:<br>\image html jkqtmathparser_couriernew.png  <br><br>
         * <code>setAnyUnicode("Comic Sans MS", "Comic Sans MS")</code>:<br>\image html jkqtmathparser_comicsans.png  <br><br>
         *
         */
        void useAnyUnicode(QString timesFont=QString(""), const QString& sansFont=QString(""), MTfontEncoding encodingTimes=MTfontEncoding::MTFEunicode, MTfontEncoding encodingSans=MTfontEncoding::MTFEunicode);



        /** \copydoc brace_factor */ 
        void setBraceFactor(double __value);
        /** \copydoc brace_factor */ 
        double getBraceFactor() const;
        /** \copydoc subsuper_size_factor */ 
        void setSubsuperSizeFactor(double __value);
        /** \copydoc subsuper_size_factor */ 
        double getSubsuperSizeFactor() const;
        /** \copydoc italic_correction_factor */ 
        void setItalicCorrectionFactor(double __value);
        /** \copydoc italic_correction_factor */ 
        double getItalicCorrectionFactor() const;
        /** \copydoc operatorsubsuper_size_factor */ 
        void setOperatorsubsuperSizeFactor(double __value);
        /** \copydoc operatorsubsuper_size_factor */ 
        double getOperatorsubsuperSizeFactor() const;
        /** \copydoc mathoperator_width_factor */ 
        void setMathoperatorWidthFactor(double __value);
        /** \copydoc mathoperator_width_factor */ 
        double getMathoperatorWidthFactor() const;
        /** \copydoc super_shift_factor */ 
        void setSuperShiftFactor(double __value);
        /** \copydoc super_shift_factor */ 
        double getSuperShiftFactor() const;
        /** \copydoc sub_shift_factor */ 
        void setSubShiftFactor(double __value);
        /** \copydoc sub_shift_factor */ 
        double getSubShiftFactor() const;
        /** \copydoc brace_shrink_factor */ 
        void setBraceShrinkFactor(double __value);
        /** \copydoc brace_shrink_factor */ 
        double getBraceShrinkFactor() const;
        /** \copydoc underbrace_factor */ 
        void setUnderbraceFactor(double __value);
        /** \copydoc underbrace_factor */ 
        double getUnderbraceFactor() const;
        /** \copydoc undersetFactor */ 
        void setUndersetFactor(double __value);
        /** \copydoc undersetFactor */ 
        double getUndersetFactor() const;
        /** \copydoc frac_factor */ 
        void setFracFactor(double __value);
        /** \copydoc frac_factor */ 
        double getFracFactor() const;
        /** \copydoc frac_shift_factor */ 
        void setFracShiftFactor(double __value);
        /** \copydoc frac_shift_factor */ 
        double getFracShiftFactor() const;
        /** \copydoc brace_y_shift_factor */ 
        void setBraceYShiftFactor(double __value);
        /** \copydoc brace_y_shift_factor */ 
        double getBraceYShiftFactor() const;
        /** \copydoc decoration_height_factor */ 
        void setDecorationHeightFactor(double __value);
        /** \copydoc decoration_height_factor */ 
        double getDecorationHeightFactor() const;
        /** \copydoc expensiveRendering */ 
        void setExpensiveRendering(bool __value);
        /** \copydoc expensiveRendering */ 
        bool getExpensiveRendering() const;
        /** \copydoc useUnparsed */ 
        void setUseUnparsed(bool __value);
        /** \copydoc useUnparsed */ 
        bool isUsingUnparsed() const;
        /** \copydoc error_list */ 
        QStringList getErrorList() const;

    protected:

        /** \brief describes the current drawing environment (base fontname ...) */
        struct MTenvironment {
            MTenvironment();
            /** \brief current font color */
            QColor color;
            /** \brief current font */
            MTenvironmentFont font;
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
            QString toHtmlStart(MTenvironment defaultEv) const;
            /** \brief generate a HTML postfix that formats the text in front of it according to the settings in this object
             *
             * \param defaultEv environment before applying the current object (to detect changes)
             * \see toHtmlAfter()
             */
            QString toHtmlAfter(MTenvironment defaultEv) const;
        };

        /** \brief beschreibt die Größe eines Knotens */
        struct JKQTMATHTEXT_LIB_EXPORT MTnodeSize {
            MTnodeSize();
            double width;
            double baselineHeight;
            double overallHeight;
            double strikeoutPos;
        };


    public:
        /** \brief subclass representing one node in the syntax tree
         *  \ingroup jkqtmathtext_items
         *
         * \image html jkqtmathtext_node_geo.png
         */
        class JKQTMATHTEXT_LIB_EXPORT MTnode {
            public:
                explicit MTnode(JKQTMathText* parent);
                MTnode(const MTnode&)=delete;
                MTnode& operator=(const MTnode&)=delete;
                virtual ~MTnode();
                /** \brief determine the size of the node, calls getSizeInternal() implementation of the actual type \see getSizeInternal()
                 *
                 * \param painter painter to use for determining the size
                 * \param currentEv current environment object
                 * \param[out] width width of the block/node
                 * \param[out] baselineHeight distance from the bottom of the block/node-box to the baseline
                 * \param[out] overallHeight overall height (bottom to top) of the node, the ascent is \c overallHeight-baselineHeight
                 * \param[out] strikeoutPos position of the strikeout-line
                 * \param[in] prevNodeSize optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
                 *
                 */
                void getSize(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr);
                /** \brief draw the contents at the designated position
                 *
                 * \param painter QPainter to use
                 * \param x x-position, where the drawing starts [Pixel]
                 * \param y Y-position of the baseline, where the drawing starts [Pixel]
                 * \param currentEv JKQTMathText::MTenvironment object describing the current drawing environment/settings
                 * \param[in] prevNodeSize optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
                 * \return the x position which to use for the next part of the text
                 */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr)=0;
                /** \brief convert node to HTML and returns \c true on success
                 * \param[out] html new HTML code is APPENDED to this string
                 * \param currentEv JKQTMathText::MTenvironment object describing the current drawing environment/settings
                 * \param defaultEv JKQTMathText::MTenvironment object describing the default drawing environment/settings when starting to interpret a node tree
                 * \return \c true on success
                 */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv);

                /** \brief returns the drawing of colored boxes (for DEBUGGING) around the actual output of the node is enabled */
                bool getDrawBoxes() const;
                /** \brief enables the drawing of colored boxes (for DEBUGGING) around the actual output of the node */
                virtual void setDrawBoxes(bool draw);
                /** \brief return the name of this class as a string */
                virtual QString getTypeName() const;
            protected:
                /** \brief determine the size of the node, overwrite this function in derived classes
                 *
                 * \param painter painter to use for determining the size
                 * \param currentEv current environment object
                 * \param[out] width width of the block/node
                 * \param[out] baselineHeight distance from the bottom of the block/node-box to the baseline
                 * \param[out] overallHeight overall height (bottom to top) of the node, the ascent is \c overallHeight-baselineHeight
                 * \param[out] strikeoutPos position of the strikeout-line
                 * \param[in] prevNodeSize optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
                 *
                 */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr)=0;

                /** \brief parent JKQTMathText object (required for several drawing operations */
                JKQTMathText* parent;
                /** \brief enables the drawing of colored boxes (for DEBUGGING) around the actual output of the node */
                bool drawBoxes;
                /** \brief draws colored boxes (for DEBUGGING) around the actual output of the node
                 *
                 * \param painter QPainter to use
                 * \param x x-position, where the drawing starts [Pixel]
                 * \param y Y-position of the baseline, where the drawing starts [Pixel]
                 * \param currentEv JKQTMathText::MTenvironment object describing the current drawing environment/settings
                 */
                void doDrawBoxes(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv);
        };

        /** \brief subclass representing one text node in the syntax tree
         *  \ingroup jkqtmathtext_items
         */
        class JKQTMATHTEXT_LIB_EXPORT MTtextNode: public MTnode {
            public:
                explicit MTtextNode(JKQTMathText* parent, const QString& text, bool addWhitespace, bool stripInnerWhitepace=false);
                virtual ~MTtextNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc text */ 
                QString getText() const;
                virtual QString getTypeName() const override ;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                QString text;
                /** \brief transforms the text before sizing/drawing (may e.g. exchange special letters for other unicode symbols etc.) */
                virtual QString textTransform(const QString& text, JKQTMathText::MTenvironment currentEv, bool forSize=false);
        };

        /** \brief subclass representing one text node in the syntax tree
         *  \ingroup jkqtmathtext_items
         */
        class JKQTMATHTEXT_LIB_EXPORT MTplainTextNode: public MTtextNode {
            public:
                explicit MTplainTextNode(JKQTMathText* parent, const QString& text, bool addWhitespace, bool stripInnerWhitepace=false);
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
            protected:
                /** \copydoc MTtextNode::textTransform() */
                virtual QString textTransform(const QString& text, JKQTMathText::MTenvironment currentEv, bool forSize=false) override;
        };
        /** \brief subclass representing one whitepsace node in the syntax tree
         *  \ingroup jkqtmathtext_items
         */
        class JKQTMATHTEXT_LIB_EXPORT MTwhitespaceNode: public MTtextNode {
            public:
                explicit MTwhitespaceNode(JKQTMathText* parent);
                virtual ~MTwhitespaceNode() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \brief convert node to HTML and returns \c true on success */
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
        };

        /** \brief subclass representing one symbol (e.g. \c \\alpha , \c \\cdot ...) node in the syntax tree
         *  \ingroup jkqtmathtext_items
         */
        class JKQTMATHTEXT_LIB_EXPORT MTsymbolNode: public MTnode {
            public:
                explicit MTsymbolNode(JKQTMathText* parent, const QString& name, bool addWhitespace);
                virtual ~MTsymbolNode() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc symbolName */ 
                QString getSymbolName() const;
                QString getSymbolfontName() const;
                bool getAddWhitespace() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;

                /** \brief this string will be sent to the drawText method with properly set fonts */
                QString symbolName;
                /** \brief add a whitespace to the symbol? */
                bool addWhitespace;
                struct SymbolProps {
                    /** \brief the symbol name supplied to the constructor */
                    QString symbol;
                    /** \brief font to use for output */
                    QString font;
                    /** \brief magnification factor for the font size */
                    double fontFactor;
                    /** \brief 0: leave italic setting as is, >0: set italic, <0 set italic to false */
                    char italic;
                    /** \brief 0: leave bold setting as is, >0: set bold, <0 set bold to false */
                    char bold;
                    /** \brief this corrects the y position of a symbol: draws at y <- y+ height*yfactor) */
                    double yfactor;
                    /** \brief indicates whether to draw a bar (like for \c \\hbar ) */
                    bool drawBar;
                    bool heightIsAscent;
                    bool exactAscent;
                    bool extendWidthInMathmode;
                };
                /** \brief retrieve the properties to render the given symbol \a symName in the current environment \a currentEv */
                SymbolProps getSymbolProp(const QString& symName, const MTenvironment& currentEv) const;
                /** \brief fill \a props for the symbol named \a n in the given environment \a currentEv and with the given \a mathFontFactor , returns \c true if the symbol can be drawn using Unicode font (or WinSymbol as Fallback)*/
                bool getSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps &props, const QString &n, const MTenvironment &currentEv, double mathFontFactor) const;
                /** \brief fill \a props for the greek letter symbol named \a n in the given environment \a currentEv and with the given \a mathFontFactor , returns \c true if the symbol can be drawn using Unicode font (or WinSymbol as Fallback) */
                bool getGreekSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps &props, const QString &n, const MTenvironment &currentEv, double mathFontFactor) const;
                /** \brief fill \a props for the symbol named \a n in the given environment \a currentEv and with the given \a mathFontFactor , returns \c true if the symbol can be drawn using WinSymbol font */
                bool getWinSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps &props, const QString &n, const MTenvironment &currentEv, double mathFontFactor) const;
                /** \brief fill \a props for the symbol named \a n  , returns \c true if the symbol can be drawn using any font, does not alter the font name!!! */
                bool getStandardTextSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps &props, const QString &n) const;
                /** \brief fill \a props for the symbol named \a n  , returns \c true if the symbol can be drawn using any unicode font, does not alter the font name!!! */
                bool getUnicodeBaseSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps &props, const QString &n) const;
                /** \brief fill \a props for the symbol named \a n  , returns \c true if the symbol can be drawn using a full unicode font, does not alter the font name!!! */
                bool getUnicodeFullSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps &props, const QString &n, double mathFontFactor) const;
        };

        /** \brief subclass representing a list of nodes in the syntax tree
         *  \ingroup jkqtmathtext_items
         */
        class JKQTMATHTEXT_LIB_EXPORT MTlistNode: public MTnode {
            public:
                explicit MTlistNode(JKQTMathText* parent);
                virtual ~MTlistNode() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                void addNode(MTnode* n) { nodes.append(n); }
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
                /** \copydoc nodes */ 
                QList<MTnode*> getNodes() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                QList<MTnode*> nodes;
                QSet<QString> subsupOperations;
        };

        /** \brief subclass representing an instruction node with exactly one argument in the syntax tree
         *  \ingroup jkqtmathtext_items
         */
        class JKQTMATHTEXT_LIB_EXPORT MTinstruction1Node: public MTnode {
            public:
                explicit MTinstruction1Node(JKQTMathText* parent, const QString& name, MTnode* child, const QStringList& parameters=QStringList());
                virtual ~MTinstruction1Node() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \brief convert node to HTML and returns \c true on success */
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
                /** \brief returns the child node  */
                MTnode* getChild() const;
                /** \copydoc name */ 
                QString getName() const;
                /** \copydoc parameters */ 
                QStringList getParameters() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                bool setupMTenvironment(JKQTMathText::MTenvironment &ev);

                MTnode* child;
                QString name;
                QStringList parameters;
        };



        /** \brief subclass representing an subscript node with exactly one argument in the syntax tree
         *  \ingroup jkqtmathtext_items
         *
         * \image html jkqtmathtext_subscriptnode_getSizeInternal.png
         */
        class JKQTMATHTEXT_LIB_EXPORT MTsubscriptNode: public MTnode {
            public:
                explicit MTsubscriptNode(JKQTMathText* parent, MTnode* child);
                virtual ~MTsubscriptNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;                /** \brief returns the child node  */
                 MTnode *getChild() const;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                MTnode* child;
        };

        /** \brief subclass representing an superscript node with exactly one argument in the syntax tree
         *  \ingroup jkqtmathtext_items
         *
         * \image html jkqtmathtext_subscriptnode_getSizeInternal.png
         *
         * \note a MTlistNode might modify the positioning slightly for special cases (e.g. \c \\int , \c \\sum ... or after braces)
         */
        class JKQTMATHTEXT_LIB_EXPORT MTsuperscriptNode: public MTnode {
            public:
                explicit MTsuperscriptNode(JKQTMathText* parent, MTnode* child);
                virtual ~MTsuperscriptNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \brief returns the child node  */
                MTnode* getChild() const;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                MTnode* child;
        };

        /** \brief subclass representing a brace node
         *  \ingroup jkqtmathtext_items
          */
        class JKQTMATHTEXT_LIB_EXPORT MTbraceNode: public MTnode {
            public:
                MTbraceNode(JKQTMathText* parent, const QString& openbrace, const QString& closebrace, MTnode* child, bool showRightBrace=true);
                virtual ~MTbraceNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \brief returns the child node  */
                inline MTnode* getChild() const {
                    return this->child; 
                }
                /** \copydoc openbrace */ 
                inline QString getOpenbrace() const { 
                    return this->openbrace; 
                }
                /** \copydoc closebrace */ 
                inline QString getClosebrace() const { 
                    return this->closebrace; 
                }
                /** \copydoc showRightBrace */ 
                inline bool getShowRightBrace() const { 
                    return this->showRightBrace; 
                }
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                MTnode* child;
                QString openbrace;
                QString closebrace;
                bool showRightBrace;

                void getBraceWidth(QPainter& painter, MTenvironment currentEv, double baselineHeight, double overallHeight, double& bracewidth, double& braceheight);
        };


        /** \brief subclass representing a sqrt node
         *  \ingroup jkqtmathtext_items
          */
        class JKQTMATHTEXT_LIB_EXPORT MTsqrtNode: public MTnode {
            public:
                MTsqrtNode(JKQTMathText* parent, MTnode* child, int degree=2);
                virtual ~MTsqrtNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
                virtual QString getTypeName() const override ;
                /** \brief returns the child node  */
                MTnode *getChild() const;
                /** \copydoc degree */ 
                int getDegree() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                MTnode* child;
                int degree;
        };

        enum MTfracMode {
            MTFMfrac,  /*!< \brief normal fraction \image html mathparser/MTFMfrac.png */
            MTFMdfrac,  /*!< \brief normal fraction, without scaling of under/over text \image html mathparser/MTFMdfrac.png */
            MTFMtfrac,  /*!< \brief text fraction (smaller than MTFMfrac) \image html mathparser/MTFMtfrac.png */
            MTFMsfrac,  /*!< \brief slanted fraction \image html mathparser/MTFMsfrac.png */
            MTFMstfrac,  /*!< \brief slanted text fraction \image html mathparser/MTFMstfrac.png */
            MTFMunderbrace,  /*!< \brief curly underbrace \image html mathparser/MTFMunderbrace.png */
            MTFMoverbrace,  /*!< \brief curly overbrace \image html mathparser/MTFMoverbrace.png */
            MTFMstackrel,  /*!< \brief binom/fraction without line \image html mathparser/MTFMstackrel.png */
            MTFMunderset,  /*!< \brief underset text \image html mathparser/MTFMunderset.png */
            MTFMoverset  /*!< \brief overset text \image html mathparser/MTFMoverset.png */
        };

        static QString fracModeToString(MTfracMode mode);

        /** \brief subclass representing a \\frac node
         *  \ingroup jkqtmathtext_items
          */
        class JKQTMATHTEXT_LIB_EXPORT MTfracNode: public MTnode {
            public:
                MTfracNode(JKQTMathText* parent, MTnode* child_top, MTnode* child_bottom, MTfracMode mode);
                virtual ~MTfracNode() override;
                virtual QString getTypeName() const override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
                /** \brief returns the 1st child node  */
                MTnode* getChild1() const;
                /** \brief returns the 2nd child node  */
                MTnode* getChild2() const;
                /** \copydoc mode */ 
                MTfracMode getMode() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                MTnode* child1;
                MTnode* child2;
                MTfracMode mode;
        };

        /** \brief subclass representing a \\begin{matrix} node
         *  \ingroup jkqtmathtext_items
          */
        class JKQTMATHTEXT_LIB_EXPORT MTmatrixNode: public MTnode {
            public:
                MTmatrixNode(JKQTMathText* parent, QVector<QVector<MTnode*> > children);
                virtual ~MTmatrixNode() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \brief returns the child nodes  */
                QVector<QVector<MTnode*> > getChildren() const;
                /** \copydoc columns */ 
                int getColumns() const;
                /** \copydoc lines */ 
                int getLines() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
                QVector<QVector<MTnode*> > children;
                int columns;
                int lines;
        };


        enum MTdecoration {
            MTDvec,  /*!< \brief vector arrow over block \image html mathparser/MTDvec.png */
            MTDhat,  /*!< \brief hat over block \image html mathparser/MTDhat.png */
            MTDdot,  /*!< \brief single dot over block \image html mathparser/MTDvec.png */
            MTDddot,  /*!< \brief double dot over block \image html mathparser/MTDddot.png */
            MTDbar,  /*!< \brief bar over block \image html mathparser/MTDbar.png */
            MTDarrow,  /*!< \brief arrow over block \image html mathparser/MTDarrow.png */
            MTDoverline,  /*!< \brief overline over block \image html mathparser/MTDoverline.png */
            MTDdoubleoverline,  /*!< \brief double overline over block \image html mathparser/MTDdoubleoverline.png */
            MTDunderline,  /*!< \brief underline under block \image html mathparser/MTDunderline.png */
            MTDdoubleunderline,  /*!< \brief double underline under block \image html mathparser/MTDdoubleunderline.png */
            MTDtilde  /*!< \brief tilde over block \image html mathparser/MTDtilde.png */
        };
        static QString decorationToString(MTdecoration mode);

        /** \brief subclass representing a decorated text m (e.g. \c \\vec \c \\hat ...) node
         *  \ingroup jkqtmathtext_items
         */
        class JKQTMATHTEXT_LIB_EXPORT MTdecoratedNode: public MTnode {
            public:
                MTdecoratedNode(JKQTMathText* parent, MTdecoration decoration, MTnode* child);
                virtual ~MTdecoratedNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /** \copydoc MTnode::setDrawBoxes() */
                virtual void setDrawBoxes(bool draw) override;
                virtual QString getTypeName() const override ;
                /** \brief returns the child node  */
                MTnode* getChild() const;
                /** \copydoc decoration */ 
                MTdecoration getDecoration() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                MTnode* child;
                MTdecoration decoration;
        };

    protected:
        /** \brief table with font replacements to use (e.g. if it is known that a certain font is not good for rendering, you can add
         *         an alternative using addReplacementFont(). These are automatically applied, when setting a new font name! */
        QMap<QString, QString> fontReplacements;
        /** \brief acompanies fontReplacements and collects the encodings of the replacement fonts, if no entry is present, the default encoding is used, as given to the setter! */
        QMap<QString, MTfontEncoding> fontEncodingReplacements;

        /** \brief font color */
        QColor fontColor;
        /** \brief base font size in points */
        double fontSize;


        /*! \brief summarizes all information available on a font for a specific MTenvironmentFont
            \see fontDefinitions */
        struct FontDefinition {
            FontDefinition();
            /** \brief name of the font */
            QString fontName;
            /** \brief specifies the encoding of the font (default is \c MTFEwinSymbol ) */
            MTfontEncoding fontEncoding;

            /** \brief symbol font used for greek symbols, or empty when \a fontName shall be used */
            QString symbolfontGreek;
            /** \brief specifies the encoding of symbolfontGreek */
            MTfontEncoding symbolfontGreekEncoding;
            /** \brief symbol font, used for math symbols, or empty when \a fontName shall be used */
            QString symbolfontSymbol;
            /** \brief specifies the encoding of symbolfontSymbol */
            MTfontEncoding symbolfontSymbolEncoding;
        };

        /** \brief stores information about the different fonts used by LaTeX markup */
        QHash<MTenvironmentFont, FontDefinition> fontDefinitions;
        /** \brief if enabled, the blackboard-characters are simulated by using font outlines only */
        bool blackboardSimulated;


        /** \brief resizing factor for braces in math mode */
        double brace_factor;
        /** \brief shrinking the width of braces in math mode 0: reduce to 0 pixel width, 1: leave unchanged*/
        double brace_shrink_factor;
        /** \brief resizing factor for font size in sub-/superscript */
        double subsuper_size_factor;
        /** \brief fraction of a whitespace by which to shift a sub-/superscript left/right when the previous text is italic */
        double italic_correction_factor;
        /** \brief like subsuper_size_factor, but for operators (\\sum, \\int) where the text is placed above/below the symbol */
        double operatorsubsuper_size_factor;
        /** \brief factor, used to extend the size of an operator in math mode */
        double mathoperator_width_factor;
        /** \brief relative shift of text in superscript to normal text:
         *         0= baseline kept, 1: baseline shifted to top of normal text */
        double super_shift_factor;
        /** \brief relative shift of text in subscript to normal text:
         *         0= baseline kept, 1: baseline shifted to bottom of normal text */
        double sub_shift_factor;


        /** \brief scaling factor for font of nominator and denominator of a fraction */
        double frac_factor;
        /** \brief shift of denominator/nummerator away from central line of a frac */
        double frac_shift_factor;
        /** \brief scaling factor for font of underbrace/overbrace text */
        double underbrace_factor;
        /** \brief scaling factor for font of underset/overset text */
        double undersetFactor;
        /** \brief fraction of the brace ascent that the brace is shifted downwards, when scaled */
        double brace_y_shift_factor;
        /** \brief size of the decorations (dot, tilde, ...), as fractio of the baselineheight */
        double decoration_height_factor;
        /** \brief switches on some options that will grant better rendering at the expense of higher time consumption */
        bool expensiveRendering;
        /** \brief a list that will be filled with error messages while parsing, if any error occur */
        QStringList error_list;
        /** \brief used by the parser. This is used to implement brace pairs with \\right. */
        bool showRightBrace;

        /** \brief the result of parsing the last string supplied to the object via parse() */
        MTnode* parsedNode;
        /** \brief a tree containing the unparsed text as a single node */
        MTnode* unparsedNode;
        /** \brief if true, the unparsedNode is drawn */
        bool useUnparsed;

        MTnode* getTree() const;

        /** \brief the token types that may arrise in the string */
        enum tokenType {
            MTTnone,
            MTTtext,
            MTTinstruction,
            MTTunderscore,
            MTThat,
            MTTdollar,
            MTTopenbrace,
            MTTclosebrace,
            MTTwhitespace,
            MTTampersand
        };

        /** \brief tokenizer for the LaTeX parser */
        tokenType getToken();
        /** \brief parse a LaTeX string */
        MTnode* parseLatexString(bool get, const QString& quitOnClosingBrace=QString(""), const QString& quitOnEnvironmentEnd=QString(""));
        /** \brief parse a LaTeX math environment */
        MTnode* parseMath(bool get);

        /** \brief used by the tokenizer. type of the current token */
        tokenType currentToken;
        /** \brief used by the tokenizer. Name of the current token, id applicable */
        QString currentTokenName;
        /** \brief used by the tokenizer. Points to the currently read character in parseString */
        int currentTokenID;
        /** \brief used by the tokenizer. The string to be parsed */
        QString parseString;
        /** \brief used by the parser. indicates whether we are in a math environment */
        bool parsingMathEnvironment;


    public:
        /** \copydoc parsedNode */ 
        MTnode *getParsedNode() const;

        struct JKQTMATHTEXT_LIB_EXPORT tbrData {
            explicit tbrData(const QFont& f, const QString& text, QPaintDevice *pd);
            QFontMetricsF fm;
            QString text;
            QRectF tbr;
            QFont f;
            int ldpiX, ldpiY, pdpiX, pdpiY;
            //QPaintDevice *pd;

            bool operator==(const tbrData& other) const;
        };
        struct JKQTMATHTEXT_LIB_EXPORT tbrDataH {
            explicit tbrDataH(const QFont& f, const QString& text, QPaintDevice *pd);
            QString text;
            QFont f;
            int ldpiX, ldpiY, pdpiX, pdpiY;

            bool operator==(const tbrDataH& other) const;
        };
        static QList<JKQTMathText::tbrData> tbrs;
        static QHash<JKQTMathText::tbrDataH, QRectF> tbrh;
        static QRectF getTightBoundingRect(const QFont &fm, const QString& text,  QPaintDevice *pd);

};


#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
inline size_t qHash(const JKQTMathText::tbrDataH& data, size_t /*seed=0*/) {
#else
inline uint qHash(const JKQTMathText::tbrDataH& data) {
#endif
  return qHash(data.f.family())+qHash(data.text);
}



/*! \brief A QLabel-derived class that draws an equation with LaTeX markup using JKQTMathText
    \ingroup jkqtmathtext

    \see JKQTMathText
*/
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextLabel: public QLabel {
        Q_OBJECT
    public:
        explicit JKQTMathTextLabel(QWidget* parent=nullptr);
        virtual ~JKQTMathTextLabel();

        /** \brief returns the internal JKQTMathText instance used for drawing
         *
         *  Use this function to set the font, font size and other properties of the used renderer.
         */
        JKQTMathText* getMathText() const;
        /** \brief set the equation to draw */
        void setMath(const QString& text, bool doRepaint=true);
    protected:
        JKQTMathText* m_mathText;
        QString lastText;
        QPixmap buffer;
        bool repaintDo;
        void internalPaint();

        void paintEvent(QPaintEvent * event);
};


#endif // JKQTMATHTEXT_H









