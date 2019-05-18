/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)
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
#include "jkqtcommon/jkqtptools.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include <QWidget>
#include <QLabel>
#include <QHash>



/** \brief initialized Qt-ressources necessary for JKQTMathText
 *  \ingroup jkqtmathtext
 */
JKQTP_LIB_EXPORT void initJKQTMathTextResources();

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
      - <code>\\verb{don't parse this _aaa\LaTeX} </code>: interpret enclosed text as verbose \image html jkqtmathtext_verb.png
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
      - \c \\left\| \c \\right\| : norm braces || ||, \image html jkqtmathtext_brace_dblline.png
      - You can use \c \\left. or \c \\right. to have only right or only left brace
    .

    \subsection JKQTMathTextSuppoertedLaTeXUnderOver Undersetting, Oversetting, Underbraces, Overbraces ...
    There are also instructions that allow to under/overset braces, arrows, ...:
      - <code>$\underbrace{x+x+...+x}{k\ \mathrm{times}}$</code> \image html jkqtmathtext_brace_underbrace.png
      - <code>$\overbrace{x+x+...+x}{k\ \mathrm{times}}$</code> \image html jkqtmathtext_brace_overbrace.png
      - <code>$\overset{main}{over}$</code> \image html jkqtmathtext_brace_overset.png
      - <code>$\underset{main}{under}$</code> \image html jkqtmathtext_brace_underset.png
    .

    \subsection JKQTMathTextSuppoertedLaTeXMatrix Matrix/Array Type Instructions
    Several Matrix/Array-typed LaTeX instructions are supported:
      - <code>$\frac{...}{...}$</code>, <code>$\tfrac{...}{...}$</code> (70% smaller font), <code>$\dfrac{...}{...}$</code> \image html jkqtmathtext_brace_frac.png
      - <code>$\sfrac{...}{...}$</code> \image html mathparser/MTFMsfrac.png
      - <code>$\stfrac{...}{...}$</code> (70% smaller font) \image html mathparser/MTFMstfrac.png
      - <code>$\stackrel{...}{...}$ $\binom{...}{...}$</code> \image html jkqtmathtext_brace_stackrel.png
      - <code>$\begin{cases} ... & ... \\ ... & ... \end{cases}$</code> \image html jkqtmathtext_brace_begincases.png
      - <code>$\begin{array} a & b & ...\\ c & d & ...\end{array}$</code> <code>$\begin{matrix} a & b & ...\\ c & d & ...\end{matrix}$</code> \image html jkqtmathtext_array.png
      - <code>$\begin{pmatrix} a & b & ...\\ c & d & ...\end{pmatrix}$</code> \image html jkqtmathtext_pmatrix.png
      - <code>$\begin{bmatrix} a & b & ...\\ c & d & ...\end{bmatrix}$</code> \image html jkqtmathtext_bmatrix.png
      - <code>$\begin{Bmatrix} a & b & ...\\ c & d & ...\end{Bmatrix}$</code> \image html jkqtmathtext_bbmatrix.png
      - <code>$\begin{vmatrix} a & b & ...\\ c & d & ...\end{vmatrix}$</code> \image html jkqtmathtext_vmatrix.png
      - <code>$\begin{Vmatrix} a & b & ...\\ c & d & ...\end{Vmatrix}$</code> \image html jkqtmathtext_vvmatrix.png
    .

    \section JKQTMathTextSuppoertedFonts Font Handling
    
    Several fonts are defined as properties to the class:
      - A "roman" font used as the standard font ( setFontRoman()  )
      - A "sans-serif" font which may be activated with \c \\sf ... ( setFontSans()  )
      - A "typewriter" font which may be activated with \c \\tt ... ( setFontTypewriter() )
      - A "script" font which may be activated with \c \\script ... ( setFontScript() )
      - A greek font which is used to display greek letters \c \\alpha ... ( setFontGreek() )
      - A symbol font used to display special (math) symbols.
      - A "roman" font used as the standard font in math mode
      - A "sans-serif" used as sans serif font in math mode
      - A "blackboard" font used to display double stroked characters (setFontBlackboard() )
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
class JKQTP_LIB_EXPORT JKQTMathText : public QObject {
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

        /** \brief configures the class to use the STIX fonts in mathmode
         *
         * use STIX (1.x/2.x) fonts from <a href="https://www.stixfonts.org/">https://www.stixfonts.org/</a> in math-mode
         *
         * \image html jkqtmathparser_stix.png
         */
        void useSTIX();

        /** \brief configures the class to use the XITS fonts in mathmode
         *
         * use XITS fonts from <a href="https://github.com/alif-type/xits">https://github.com/alif-type/xits</a> in math-mode.
         * These are included by default in this library and also activated by default.
         *
         * \image html jkqtmathparser_xits.png
         */
        void useXITS();

        /** \brief configures the class to use the ASANA fonts in mathmode
         *
         * use the ASANA fonts from <a href="https://ctan.org/tex-archive/fonts/Asana-Math/">https://ctan.org/tex-archive/fonts/Asana-Math/</a> in math-mode
         *
         * \image html jkqtmathparser_asana.png
         */
        void useASANA();

        /** \brief configures the class to use a unicode font for symbols in mathmode
         *
         * use generic Unicode fonts, e.g. "Arial" and "Times New Roman" in math-mode.
         * You should use fonts that contain as many of the mathematical symbols as possible to ensure good rendering results.
         *
         * <code>setAnyUnicode("Times New Roman", "Times New Roman")</code>:<br>\image html jkqtmathparser_timesnewroman.png
         * <code>setAnyUnicode("Arial", "Arial")</code>:<br>\image html jkqtmathparser_arial.png
         * <code>setAnyUnicode("Courier New", "Courier New")</code>:<br>\image html jkqtmathparser_couriernew.png
         * <code>setAnyUnicode("Comic Sans MS", "Comic Sans MS")</code>:<br>\image html jkqtmathparser_comicsans.png
         *
         * The parameter \a fullMathUnicodeFont specifies, whether the unicode fonts encode also math symbols, or are
         * limited to latin+greek+basic symbols.
         */
        void useAnyUnicode(QString timesFont=QString(""), const QString& sansFont=QString(""), bool fullMathUnicodeFont=false);

        void useLatexFonts(QString prefix=QString(""), const QString& postfix=QString(""));

        /** \brief convert LaTeX to HTML. returns \c ok=true on success and \c ok=false else. */
        QString toHtml(bool* ok=nullptr, double fontPointSize=10);

        /*! \brief used to specify the font encoding used for drawing

               - \c MTFEwinSymbol: This assumes that symbols shal be taken from a MS Windows style Symbol font
               - \c MTFElatex:     This assumes that symbols shal be taken from the CM (computer modern) fonts, used by LaTeX
               - \c MTFEunicode:   This assumes that symbols shall be taken from a Unicode font
                                   (e.g. the STIX fonts from <a href="http://www.stixfonts.org/">http://www.stixfonts.org/</a>)
               - \c MTFEunicodeLimited: This assumes that the fonts used are Unicode, but only offer a limited set of symbols.
                                        Especially math symbols are missing from this encoding
            .
        */
        enum MTfontEncoding {
            MTFEwinSymbol,
            MTFEunicode,
            MTFEunicodeLimited,
            MTFElatex
        };

        /*! \copydoc fontColor */ 
        inline void setFontColor(const QColor & __value)
        {
            this->fontColor = __value;
        } 
        /*! \copydoc fontColor */ 
        inline QColor getFontColor() const
        {
            return this->fontColor; 
        }
        /*! \copydoc fontSize */ 
        inline void setFontSize(double __value)
        {
            this->fontSize = __value;
        } 
        /*! \copydoc fontSize */ 
        inline double getFontSize() const
        {
            return this->fontSize; 
        }
        /** \brief add a font pair to the table with font replacements
         *
         * e.g. if it is known that a certain font is not good for rendering, you can add an alternative with this function.
         * These are automatically applied, when setting a new font name!
         *
         * \param nonUseFont the font not to use
         * \param useFont replacement font for nonUseFont
         */
        void addReplacementFont(const QString& nonUseFont, const QString& useFont);


        /*! \copydoc fontRoman */
        inline void setFontRoman(const QString & __value)
        {
            this->fontRoman = fontReplacements.value(__value, __value);
        }
        /*! \brief sets the property fontRoman ( \copybrief fontRoman ) to \a __value, or calls useXITS() if \a __value \c =="XITS".  calls useSTIX() if \a __value \c =="STIX", ...

            \see fontRoman for more information */
        void setFontRomanOrSpecial(const QString & __value);
        /*! \copydoc fontRoman */
        inline QString getFontRoman() const
        {
            return this->fontRoman; 
        }
        /*! \copydoc fontSans */ 
        inline void setFontSans(const QString & __value)
        {
            this->fontSans = fontReplacements.value(__value, __value);
        } 
        /*! \copydoc fontSans */ 
        inline QString getFontSans() const
        {
            return this->fontSans; 
        }
        /*! \copydoc fontTypewriter */ 
        inline void setFontTypewriter(const QString & __value)
        {
            this->fontTypewriter = __value;
        } 
        /*! \copydoc fontTypewriter */ 
        inline QString getFontTypewriter() const
        {
            return this->fontTypewriter; 
        }
        /*! \copydoc fontScript */ 
        inline void setFontScript(const QString & __value)
        {
            this->fontScript = __value;
        } 
        /*! \copydoc fontScript */ 
        inline QString getFontScript() const
        {
            return this->fontScript; 
        }
        /*! \copydoc fontGreek */ 
        inline void setFontGreek(const QString & __value)
        {
            this->fontGreek = __value;
        } 
        /*! \copydoc fontGreek */ 
        inline QString getFontGreek() const
        {
            return this->fontGreek; 
        }
        /*! \copydoc fontSymbol */ 
        inline void setFontSymbol(const QString & __value)
        {
            this->fontSymbol = __value;
        } 
        /*! \copydoc fontSymbol */ 
        inline QString getFontSymbol() const
        {
            return this->fontSymbol; 
        }
        /*! \copydoc fontBraces */ 
        inline void setFontBraces(const QString & __value)
        {
            this->fontBraces = __value;
        } 
        /*! \copydoc fontBraces */ 
        inline QString getFontBraces() const
        {
            return this->fontBraces; 
        }
        /*! \copydoc fontIntegrals */ 
        inline void setFontIntegrals(const QString & __value)
        {
            this->fontIntegrals = __value;
        } 
        /*! \copydoc fontIntegrals */ 
        inline QString getFontIntegrals() const
        {
            return this->fontIntegrals; 
        }
        /*! \copydoc fontCaligraphic */ 
        inline void setFontCaligraphic(const QString & __value)
        {
            this->fontCaligraphic = __value;
        } 
        /*! \copydoc fontCaligraphic */ 
        inline QString getFontCaligraphic() const
        {
            return this->fontCaligraphic; 
        }
        /*! \copydoc fontBlackboard */ 
        inline void setFontBlackboard(const QString & __value)
        {
            this->fontBlackboard = __value;
        } 
        /*! \copydoc fontBlackboard */ 
        inline QString getFontBlackboard() const
        {
            return this->fontBlackboard; 
        }
        /*! \copydoc fontLatexPrefix */ 
        inline void setFontLatexPrefix(const QString & __value)
        {
            this->fontLatexPrefix = __value;
        } 
        /*! \copydoc fontLatexPrefix */ 
        inline QString getFontLatexPrefix() const
        {
            return this->fontLatexPrefix; 
        }
        /*! \copydoc fontLatexPostfix */ 
        inline void setFontLatexPostfix(const QString & __value)
        {
            this->fontLatexPostfix = __value;
        } 
        /*! \copydoc fontLatexPostfix */ 
        inline QString getFontLatexPostfix() const
        {
            return this->fontLatexPostfix; 
        }
        /*! \copydoc fontEncoding */ 
        inline void setFontEncoding(const MTfontEncoding & __value)
        {
            this->fontEncoding = __value;
        } 
        /*! \copydoc fontEncoding */
        inline MTfontEncoding getFontEncoding() const
        {
            return this->fontEncoding;
        }
        /*! \copydoc useSTIXfonts */ 
        inline bool isUsingSTIXfonts() const { 
            return this->useSTIXfonts; 
        }
        /*! \copydoc useXITSfonts */ 
        inline bool isUsingXITSfonts() const { 
            return this->useXITSfonts; 
        }
        /*! \copydoc brace_factor */ 
        inline void setBraceFactor(double __value)
        {
            this->brace_factor = __value;
        } 
        /*! \copydoc brace_factor */ 
        inline double getBraceFactor() const
        {
            return this->brace_factor; 
        }
        /*! \copydoc subsuper_size_factor */ 
        inline void setSubsuperSizeFactor(double __value)
        {
            this->subsuper_size_factor = __value;
        } 
        /*! \copydoc subsuper_size_factor */ 
        inline double getSubsuperSizeFactor() const
        {
            return this->subsuper_size_factor; 
        }
        /*! \copydoc italic_correction_factor */ 
        inline void setItalicCorrectionFactor(double __value)
        {
            this->italic_correction_factor = __value;
        } 
        /*! \copydoc italic_correction_factor */ 
        inline double getItalicCorrectionFactor() const
        {
            return this->italic_correction_factor; 
        }
        /*! \copydoc operatorsubsuper_size_factor */ 
        inline void setOperatorsubsuperSizeFactor(double __value)
        {
            this->operatorsubsuper_size_factor = __value;
        } 
        /*! \copydoc operatorsubsuper_size_factor */ 
        inline double getOperatorsubsuperSizeFactor() const
        {
            return this->operatorsubsuper_size_factor; 
        }
        /*! \copydoc mathoperator_width_factor */ 
        inline void setMathoperatorWidthFactor(double __value)
        {
            this->mathoperator_width_factor = __value;
        } 
        /*! \copydoc mathoperator_width_factor */ 
        inline double getMathoperatorWidthFactor() const
        {
            return this->mathoperator_width_factor; 
        }
        /*! \copydoc super_shift_factor */ 
        inline void setSuperShiftFactor(double __value)
        {
            this->super_shift_factor = __value;
        } 
        /*! \copydoc super_shift_factor */ 
        inline double getSuperShiftFactor() const
        {
            return this->super_shift_factor; 
        }
        /*! \copydoc sub_shift_factor */ 
        inline void setSubShiftFactor(double __value)
        {
            this->sub_shift_factor = __value;
        } 
        /*! \copydoc sub_shift_factor */ 
        inline double getSubShiftFactor() const
        {
            return this->sub_shift_factor; 
        }
        /*! \copydoc brace_shrink_factor */ 
        inline void setBraceShrinkFactor(double __value)
        {
            this->brace_shrink_factor = __value;
        } 
        /*! \copydoc brace_shrink_factor */ 
        inline double getBraceShrinkFactor() const
        {
            return this->brace_shrink_factor; 
        }
        /*! \copydoc underbrace_factor */ 
        inline void setUnderbraceFactor(double __value)
        {
            this->underbrace_factor = __value;
        } 
        /*! \copydoc underbrace_factor */ 
        inline double getUnderbraceFactor() const
        {
            return this->underbrace_factor; 
        }
        /*! \copydoc undersetFactor */ 
        inline void setUndersetFactor(double __value)
        {
            this->undersetFactor = __value;
        } 
        /*! \copydoc undersetFactor */ 
        inline double getUndersetFactor() const
        {
            return this->undersetFactor; 
        }
        /*! \copydoc frac_factor */ 
        inline void setFracFactor(double __value)
        {
            this->frac_factor = __value;
        } 
        /*! \copydoc frac_factor */ 
        inline double getFracFactor() const
        {
            return this->frac_factor; 
        }
        /*! \copydoc frac_shift_factor */ 
        inline void setFracShiftFactor(double __value)
        {
            this->frac_shift_factor = __value;
        } 
        /*! \copydoc frac_shift_factor */ 
        inline double getFracShiftFactor() const
        {
            return this->frac_shift_factor; 
        }
        /*! \copydoc brace_y_shift_factor */ 
        inline void setBraceYShiftFactor(double __value)
        {
            this->brace_y_shift_factor = __value;
        } 
        /*! \copydoc brace_y_shift_factor */ 
        inline double getBraceYShiftFactor() const
        {
            return this->brace_y_shift_factor; 
        }
        /*! \copydoc decoration_height_factor */ 
        inline void setDecorationHeightFactor(double __value)
        {
            this->decoration_height_factor = __value;
        } 
        /*! \copydoc decoration_height_factor */ 
        inline double getDecorationHeightFactor() const
        {
            return this->decoration_height_factor; 
        }
        /*! \copydoc expensiveRendering */ 
        inline void setExpensiveRendering(bool __value)
        {
            this->expensiveRendering = __value;
        } 
        /*! \copydoc expensiveRendering */ 
        inline bool getExpensiveRendering() const
        {
            return this->expensiveRendering; 
        }
        /*! \copydoc useUnparsed */ 
        inline void setUseUnparsed(bool __value)
        {
            this->useUnparsed = __value;
        } 
        /*! \copydoc useUnparsed */ 
        inline bool isUsingUnparsed() const
        {
            return this->useUnparsed; 
        }
        /*! \copydoc error_list */ 
        inline QStringList getErrorList() const { 
            return this->error_list; 
        }

    protected:

        /** \brief the available fonts */
        enum MTenvironmentFont {
            MTEroman,
            MTEsans,
            MTEtypewriter,
            MTEscript,
            MTEblackboard,
            MTEcaligraphic
        };

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
        struct MTnodeSize {
            MTnodeSize();
            double width;
            double baselineHeight;
            double overallHeight;
            double strikeoutPos;
        };


    public:
        /** \brief subclass representing one node in the syntax tree
         *
         * \image html jkqtmathtext_node_geo.png
         */
        class MTnode {
            public:
                MTnode(JKQTMathText* parent);
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

        /** \brief subclass representing one text node in the syntax tree */
        class MTtextNode: public MTnode {
            public:
                MTtextNode(JKQTMathText* parent, const QString& text, bool addWhitespace, bool stripInnerWhitepace=false);
                virtual ~MTtextNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /*! \copydoc text */ 
                inline QString getText() const { 
                    return this->text; 
                }
                virtual QString getTypeName() const override ;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                QString text;
                /** \brief transforms the text before sizing/drawing (may e.g. exchange special letters for other unicode symbols etc.) */
                virtual QString textTransform(const QString& text, JKQTMathText::MTenvironment currentEv, bool forSize=false);
        };

        /** \brief subclass representing one text node in the syntax tree */
        class MTplainTextNode: public MTtextNode {
            public:
                MTplainTextNode(JKQTMathText* parent, const QString& text, bool addWhitespace, bool stripInnerWhitepace=false);
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
            protected:
                /** \copydoc MTtextNode::textTransform() */
                virtual QString textTransform(const QString& text, JKQTMathText::MTenvironment currentEv, bool forSize=false) override;
        };
        /** \brief subclass representing one whitepsace node in the syntax tree */
        class MTwhitespaceNode: public MTtextNode {
            public:
                MTwhitespaceNode(JKQTMathText* parent);
                virtual ~MTwhitespaceNode() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \brief convert node to HTML and returns \c true on success */
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
        };

        /** \brief subclass representing one symbol (e.g. \c \\alpha , \c \\cdot ...) node in the syntax tree */
        class MTsymbolNode: public MTnode {
            public:
                MTsymbolNode(JKQTMathText* parent, const QString& name, bool addWhitespace);
                virtual ~MTsymbolNode() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::toHtml() */
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /*! \copydoc symbolName */ 
                inline QString getSymbolName() const {
                    return this->symbolName;
                }
                QString getSymbolFontName() const;
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \brief this string will be sent to the drawText method with properly set fonts */
                QString symbol;
                /** \brief the symbol name supplied to the constructor */
                QString symbolName;
                /** \brief these fonts may be used for symbols */
                enum symbolFont { MTSFdefault, MTSFsymbol, MTSFgreek, MTSFbraces, MTSFintegrals, MTSFcaligraphic, MTSFblackboard };
                /** \brief changes the font name according to a given symbolFont value */
                QFont getFontName(symbolFont f, QFont& fi) const;
                /** \brief magnification factor for the font size */
                symbolFont font;
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

        /** \brief subclass representing a list of nodes in the syntax tree
         */
        class MTlistNode: public MTnode {
            public:
                MTlistNode(JKQTMathText* parent);
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
                /*! \copydoc nodes */ 
                inline QList<MTnode*> getNodes() const { 
                    return this->nodes; 
                }
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                QList<MTnode*> nodes;
                QSet<QString> subsupOperations;
        };

        /** \brief subclass representing an instruction node with exactly one argument in the syntax tree */
        class MTinstruction1Node: public MTnode {
            public:
                MTinstruction1Node(JKQTMathText* parent, const QString& name, MTnode* child, const QStringList& parameters=QStringList());
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
                /*! \brief returns the child node */
                inline MTnode* getChild() const {
                    return this->child; 
                }
                /*! \copydoc name */ 
                inline QString getName() const { 
                    return this->name; 
                }
                /*! \copydoc parameters */ 
                inline QStringList getParameters() const { 
                    return this->parameters; 
                }
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                bool setupMTenvironment(JKQTMathText::MTenvironment &ev);

                MTnode* child;
                QString name;
                QStringList parameters;
        };



        /** \brief subclass representing an subscript node with exactly one argument in the syntax tree
         *
         * \image html jkqtmathtext_subscriptnode_getSizeInternal.png
         */
        class MTsubscriptNode: public MTnode {
            public:
                MTsubscriptNode(JKQTMathText* parent, MTnode* child);
                virtual ~MTsubscriptNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;                /*! \brief returns the child node */
                 inline MTnode* getChild() const {
                    return this->child; 
                }
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
         *
         * \image html jkqtmathtext_subscriptnode_getSizeInternal.png
         *
         * \note a MTlistNode might modify the positioning slightly for special cases (e.g. \c \\int , \c \\sum ... or after braces)
         */
        class MTsuperscriptNode: public MTnode {
            public:
                MTsuperscriptNode(JKQTMathText* parent, MTnode* child);
                virtual ~MTsuperscriptNode() override;
                /** \copydoc MTnode::draw() */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                /*! \brief returns the child node */
                inline MTnode* getChild() const { 
                    return this->child; 
                }
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

        /** \brief subclass representing a brace node  */
        class MTbraceNode: public MTnode {
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
                /*! \brief returns the child node */
                inline MTnode* getChild() const {
                    return this->child; 
                }
                /*! \copydoc openbrace */ 
                inline QString getOpenbrace() const { 
                    return this->openbrace; 
                }
                /*! \copydoc closebrace */ 
                inline QString getClosebrace() const { 
                    return this->closebrace; 
                }
                /*! \copydoc showRightBrace */ 
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


        /** \brief subclass representing a sqrt node  */
        class MTsqrtNode: public MTnode {
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
                /*! \brief returns the child node */
                inline MTnode* getChild() const {
                    return this->child; 
                }
                /*! \copydoc degree */ 
                inline int getDegree() const { 
                    return this->degree; 
                }
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

        /** \brief subclass representing a \\frac node  */
        class MTfracNode: public MTnode {
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
                /*! \brief returns the 1st child node */
                inline MTnode* getChild1() const {
                    return this->child1; 
                }
                /*! \brief returns the 2nd child node */
                 inline MTnode* getChild2() const {
                    return this->child2; 
                }
                /*! \copydoc mode */ 
                inline MTfracMode getMode() const { 
                    return this->mode; 
                }
            protected:
                /** \copydoc MTnode::getSizeInternal() */
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize=nullptr) override;
                MTnode* child1;
                MTnode* child2;
                MTfracMode mode;
        };

        /** \brief subclass representing a \\begin{matrix} node  */
        class MTmatrixNode: public MTnode {
            public:
                MTmatrixNode(JKQTMathText* parent, QVector<QVector<MTnode*> > children);
                virtual ~MTmatrixNode() override;
                /** \copydoc MTnode::getTypeName() */
                virtual QString getTypeName() const override;
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv, const MTnodeSize* prevNodeSize=nullptr) override;
                virtual bool toHtml(QString& html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) override;
                /*! \brief returns the child nodes */
                inline QVector<QVector<MTnode*> > getChildren() const {
                    return this->children; 
                }
                /*! \copydoc columns */ 
                inline int getColumns() const { 
                    return this->columns; 
                }
                /*! \copydoc lines */ 
                inline int getLines() const { 
                    return this->lines; 
                }
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

        /** \brief subclass representing a decorated text m (e.g. \c \\vec \c \\hat ...) node  */
        class MTdecoratedNode: public MTnode {
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
                /*! \brief returns the child node */
                 inline MTnode* getChild() const {
                    return this->child; 
                }
                /*! \copydoc decoration */ 
                inline MTdecoration getDecoration() const { 
                    return this->decoration; 
                }
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

        /** \brief font color */
        QColor fontColor;
        /*! \brief default value for property fontColor.
            \see fontColor for more information */
        QColor default_fontColor;
        /** \brief base font size in points */
        double fontSize;
        /*! \brief default value for property fontSize.
            \see fontSize for more information */
        double default_fontSize;
        /** \brief roman font */
        QString fontRoman;
        /*! \brief default value for property fontRoman.
            \see fontRoman for more information */
        QString default_fontRoman;
        /** \brief sans-serif font */
        QString fontSans;
        /*! \brief default value for property fontSans.
            \see fontSans for more information */
        QString default_fontSans;
        /** \brief typewriter font */
        QString fontTypewriter;
        /*! \brief default value for property fontTypewriter.
            \see fontTypewriter for more information */
        QString default_fontTypewriter;
        /** \brief script font */
        QString fontScript;
        /*! \brief default value for property fontScript.
            \see fontScript for more information */
        QString default_fontScript;
        /** \brief greek font */
        QString fontGreek;
        /*! \brief default value for property fontGreek.
            \see fontGreek for more information */
        QString default_fontGreek;
        /** \brief symbol font, used for math symbols. */
        QString fontSymbol;
        /*! \brief default value for property fontSymbol.
            \see fontSymbol for more information */
        QString default_fontSymbol;
        /** \brief symbol font, used for braces in math mode. */
        QString fontBraces;
        /*! \brief default value for property fontBraces.
            \see fontBraces for more information */
        QString default_fontBraces;
        /** \brief symbol font, used for integrals in math mode. */
        QString fontIntegrals;
        /*! \brief default value for property fontIntegrals.
            \see fontIntegrals for more information */
        QString default_fontIntegrals;
        /** \brief font used for caligraphic symbols (escepcially in math environments) */
        QString fontCaligraphic;
        /*! \brief default value for property fontCaligraphic.
            \see fontCaligraphic for more information */
        QString default_fontCaligraphic;
        /** \brief font used for blackboard (double-stroke) symbols (escepcially in math environments) */
        QString fontBlackboard;
        /*! \brief default value for property fontBlackboard.
            \see fontBlackboard for more information */
        QString default_fontBlackboard;
        /** \brief prefix for LaTeX fonts */
        QString fontLatexPrefix;
        /*! \brief default value for property fontLatexPrefix.
            \see fontLatexPrefix for more information */
        QString default_fontLatexPrefix;
        /** \brief postfix for LaTeX fonts */
        QString fontLatexPostfix;
        /*! \brief default value for property fontLatexPostfix.
            \see fontLatexPostfix for more information */
        QString default_fontLatexPostfix;
        /** \brief specifies the encoding of special character fonts (default is \c MTFEwinSymbol ) */
        MTfontEncoding fontEncoding;
        /*! \brief default value for property fontEncoding.
            \see fontEncoding for more information */
        MTfontEncoding default_fontEncoding;
        /** \brief resizing factor for braces in math mode */
        double brace_factor;
        /*! \brief default value for property brace_factor.
            \see brace_factor for more information */
        double default_brace_factor;
        /** \brief shrinking the width of braces in math mode 0: reduce to 0 pixel width, 1: leave unchanged*/
        double brace_shrink_factor;
        /*! \brief default value for property brace_shrink_factor.
            \see brace_shrink_factor for more information */
        double default_brace_shrink_factor;
        /** \brief resizing factor for font size in sub-/superscript */
        double subsuper_size_factor;
        /*! \brief default value for property subsuper_size_factor.
            \see subsuper_size_factor for more information */
        double default_subsuper_size_factor;
        /** \brief fraction of a whitespace by which to shift a sub-/superscript left/right when the previous text is italic */
        double italic_correction_factor;
        /*! \brief default value for property italic_correction_factor.
            \see italic_correction_factor for more information */
        double default_italic_correction_factor;
        /** \brief like subsuper_size_factor, but for operators (\\sum, \\int) where the text is placed above/below the symbol */
        double operatorsubsuper_size_factor;
        /*! \brief default value for property operatorsubsuper_size_factor.
            \see operatorsubsuper_size_factor for more information */
        double default_operatorsubsuper_size_factor;
        /** \brief factor, used to extend the size of an operator in math mode */
        double mathoperator_width_factor;
        /*! \brief default value for property mathoperator_width_factor.
            \see mathoperator_width_factor for more information */
        double default_mathoperator_width_factor;
        /** \brief relative shift of text in superscript to normal text:
         *         0= baseline kept, 1: baseline shifted to top of normal text */
        double super_shift_factor;
        /*! \brief default value for property super_shift_factor.
            \see super_shift_factor for more information */
        double default_super_shift_factor;
        /** \brief relative shift of text in subscript to normal text:
         *         0= baseline kept, 1: baseline shifted to bottom of normal text */
        double sub_shift_factor;
        /*! \brief default value for property sub_shift_factor.
            \see sub_shift_factor for more information */
        double default_sub_shift_factor;
        /** \brief indicates whether to use STIX fonts or not */
        bool useSTIXfonts;
        /*! \brief default value for property useSTIXfonts.
            \see useSTIXfonts for more information */
        bool default_useSTIXfonts;
        /** \brief indicates whether to use XITS fonts or not */
        bool useXITSfonts;
        /** \brief the first call to useXITS() determines this name for the default XITS fonts */
        QString defaultXITSFontName;
        /** \brief the call to useXITS() determines this name for the default XITS Math fonts */
        QString defaultXITSMathFontName;
        /*! \brief default value for property useXITSfonts.
            \see useXITSfonts for more information */
        bool default_useXITSfonts;
        /** \brief indicates whether to use XITS fonts or not */
        bool useASANAfonts;
        /*! \brief default value for property useASANAfonts.
            \see useASANAfonts for more information */
        bool default_useASANAfonts;
        /** \brief scaling factor for font of nominator and denominator of a fraction */
        double frac_factor;
        /*! \brief default value for property frac_factor.
            \see frac_factor for more information */
        double default_frac_factor;
        /** \brief shift of denominator/nummerator away from central line of a frac */
        double frac_shift_factor;
        /*! \brief default value for property frac_shift_factor.
            \see frac_shift_factor for more information */
        double default_frac_shift_factor;
        /** \brief scaling factor for font of underbrace/overbrace text */
        double underbrace_factor;
        /*! \brief default value for property underbrace_factor.
            \see underbrace_factor for more information */
        double default_underbrace_factor;
        /** \brief scaling factor for font of underset/overset text */
        double undersetFactor;
        /*! \brief default value for property undersetFactor.
            \see undersetFactor for more information */
        double default_undersetFactor;
        /** \brief fraction of the brace ascent that the brace is shifted downwards, when scaled */
        double brace_y_shift_factor;
        /*! \brief default value for property brace_y_shift_factor.
            \see brace_y_shift_factor for more information */
        double default_brace_y_shift_factor;
        /** \brief size of the decorations (dot, tilde, ...), as fractio of the baselineheight */
        double decoration_height_factor;
        /*! \brief default value for property decoration_height_factor.
            \see decoration_height_factor for more information */
        double default_decoration_height_factor;
        /** \brief switches on some options that will grant better rendering at the expense of higher time consumption */
        bool expensiveRendering;
        /*! \brief default value for property expensiveRendering.
            \see expensiveRendering for more information */
        bool default_expensiveRendering;
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

        inline MTnode* getTree() const {
            if (useUnparsed) return unparsedNode;
            return parsedNode;
        }

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
        /*! \copydoc parsedNode */ 
        inline MTnode* getParsedNode() const { 
            return this->parsedNode; 
        }

        struct tbrData {
            explicit tbrData(const QFont& f, const QString& text, QPaintDevice *pd);
            QFontMetricsF fm;
            QString text;
            QRectF tbr;
            QFont f;
            int ldpiX, ldpiY, pdpiX, pdpiY;
            //QPaintDevice *pd;

            bool operator==(const tbrData& other) const;
        };
        struct tbrDataH {
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



inline uint qHash(const JKQTMathText::tbrDataH& data) {
    return qHash(data.f.family())+qHash(data.text);
}




/*! \brief A QLabel-derived class that draws an equation with LaTeX markup using JKQTMathText
    \ingroup jkqtmathtext

    \see JKQTMathText
*/
class JKQTP_LIB_EXPORT JKQTMathTextLabel: public QLabel {
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









