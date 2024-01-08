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





#ifndef JKQTMATHTEXT_H
#define JKQTMATHTEXT_H

#include <QFlags>
#include <QObject>
#include <QSettings>
#include <QPainter>
#include <QString>
#include <QSet>
#include <QFile>
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include <QHash>
#include <QPicture>
#include <QImage>
#include <QPixmap>
#include <type_traits>
#include <memory>
#include <mutex>


class JKQTMathTextNode; // forward
class JKQTMathTextParser; // forward
class JKQTMathTextVerticalListNode; // forward

/*! \brief this class parses a mathematical markup string and can then draw the contained text/equation onto a <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>
    \ingroup jkqtmathtext_render

    
    JKQTMathText is a self-contained mathematical markup renderer for Qt. It is used to renderer
    labels in JKQTPlotter/JKQTBasePlotter, but can be used independently. 
    The class does not depend on any library, except Qt. 
    
    The implementation is split over several classes:
      - JKQTMathText implements drawing, based on a memory representatioj of the markup. It also provides an interface for class users.
      - JKQTMathTextParser and its children like JKQTMathTextLatexParser parses mathamtical markup.
      - The nodes summarized in \ref jkqtmathtext_items  are used to build the memory representation of the markup.
    .
    
    In particular JKQTMathTextLatexParser actually parses e.g. a LaTeX string and draws it in pure C++. It does NOT rely
    on an installed LaTeX for the rendering! See \ref jkqtmathtext_supportedlatex for a description of the supported LaTeX subset.

    More generally these pages describe how the class renders math markup:
      - \ref jkqtmathtext_renderingmodel for a description of the rendering model of JKQTMathTextLatexParser
      - \ref jkqtmathtext_fonthandling for a description of the font handling in JKQTMathTextLatexParser
        (which is modelled after LaTeX, but with tweaks especially for use in the  context of GUI code)
      - \ref jkqtmathtext_supportedlatex for a description of the supported LaTeX subset
    .

    \section JKQTMathTextUsageThreadSafety Thread-Safety/Re-Entrancy Guarantees
    All functions in this class are reentrant. Therefore different instances off JKQTMathtext can be used in parallel in different threads.
    DO NOT however use the same instance from multiple threads, as the functions are not thread-safe!

    Also note that there are certain caches that reuse information (e.g. about fonts) from previous runs, which allow different instances
    (also over different threads) to profit from each other. On the other hand, this strategy requires a bit of overhead due to synchronization,
    but usually the speedup outweighs the overhead significantly!


    \section JKQTMathTextUsage Usage

    \subsection JKQTMathTextUsageParsing Parsing Functions
    The class provieds two flavours of a parsing function:
      - There is a function \c parse(markup, markupType) that accepts the markup, the type of markup (as enum DefaultParserTypes)  and parser options.
        This function determines the appropriate parser class, based on the markup type.
      - A templated function parse<TParser>(markup, options) that gets a parser class to be used on the markup.
    .

    \subsection JKQTMathTextUsageDirect Drawing Functions
    The class provides different variants of the drawing function draw() that paints using an
    externlly provided <a href="https://doc.qt.io/qt-6/qpainter.html">QPainter</a>. These variants
    either paint into a QRect, or starting from a single location (x,y).

    The QRect-variants can align the render result inside the rectangle, whereas the
    location-variants draw from a position that is on the left-hand side of the output's baseline:

    \image html jkqtmathtext/jkqtmathtext_node_geo.png

    Note that you first need to call one of the \ref JKQTMathTextUsageParsing so there is anything to render!



    \subsection JKQTMathTextUsageConvenience Convenience Functions

    Alternatively you can use these methods to directly generate a QPixmap or QPicture:
      - drawIntoPixmap()
      - drawIntoImage()
      - drawIntoPicture()
    .


    \subsection JKQTMathTextSizing Determining the size of an equation

    In addition there are also functions that allow to calculate the size of the equation,
    before drawing it (just like the functions in <a href="http://doc.qt.io/qt-6/qfontmetrics.html">QFontMetrics</a>
    or <a href="http://doc.qt.io/qt-6/qfontmetricsf.html">QFontMetricsF</a>):
      - getSizeDetail()
      - getSize()
      - getAscent(), getDescent()
    .


    \subsection JKQTMathTextUsageQLabel Usage within a QLabel class JKQTMathTextLabel

    Finally, there is also a QLabel-derived class JKQTMathTextLabel which can be used for drawing a LaTeX string onto a Qt form.

    \see JKQTMathTextLabel


    \subsection JKQTMathTextErrorHandling Error Handling

    The class is designed to be as robust as possible and will still return some output, even if the equation contains some errors.
    Nevertheless, several errors are detected while parsing. You can get a list of error messages using getErrorList() after calling parse().
    Also parse() will return \c false if an error occured while parsing.

    \subsection JKQTMathTextUsageExample Example Code
    This small piece of C++ code may serve as an example of the usage and capabilities of the class:
    \code
    // create a JKQTMathText object.
    JKQTMathText mathText;

    // configure its properties to influence the rendering (e.g. fonts to use, font size, ...)
    mathText.useXITS();
    mathText.setFontSize(20);

    // parse some LaTeX code (the Schroedinger's equation)
    mathText.parse("$\\left[-\\frac{\\hbar^2}{2m}\\frac{\\partial^2}{\\partial x^2}+V(x)\\right]\\Psi(x)=\\mathrm{i}\\hbar\\frac{\\partial}{\\partial t}\\Psi(x)$");

    // draw the result into a QPixmap
    QPixmap result=mathText.drawIntoPixmap();
    \endcode

    Alternatively you can also use this class with a <a href="https://doc.qt.io/qt-6/qpainter.html">QPainter</a>:

    \code
    // use the draw() methods to draw the equation using a QPainter (here onto a QPixmap)
    QPainter painter;

    // first we determine the size of the render output:
    const JKQTMathTextNodeSize size=getSizeDetail(painter);

    // now we can us that size information to render:
    mathText.draw(painter, Qt::AlignCenter, QRectF(0,0,pix.width(), pix.height()), false);
    \endcode




    \section JKQTMathTextToHTML Convert to HTML

    The method toHtml() may be used to get a HTML representation of the LaTeX string, if possible (only for simple LaTeX equations!). Whether
    the transformation was possible is returned as a call by value argument!

    
    \section JKQTMathTextExamples Example Projects

    Examples for the usage of this class can be found here: 
      - \ref JKQTMathTextSimpleExample
      - \ref JKQTMathTextRenderCmdLineTool
      - \ref JKQTMathTextTestApp
    .



 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathText : public QObject {
        Q_OBJECT
    public:
        friend class JKQTMathTextParser;
        friend class JKQTMathTextNode;

        /** \brief minimum linewidth allowed in a JKQTMathText (given in pt) */
        static const double ABS_MIN_LINEWIDTH;


        /** \brief class constructor with a paren QObject \a parent and calls useGuiFonts() if \a useFontsForGUI \c =true */
        JKQTMathText(QObject * parent = nullptr, bool useFontsForGUI=false);
        /** \brief class destructor */
        ~JKQTMathText();


/** @name Parsing Math Markup */
/**@{*/
        /** \brief options for parse() */
        enum ParseOption {
            AddSpaceBeforeAndAfter = 0x01, /*!< \brief If set, a little bit of space is added before and after the text. */
            StartWithMathMode = 0x02, /*!< \brief if set, the parser assumes the LaTeX string is in math-mode (as if surrounded by \c $ ) */
            AllowLinebreaks = 0x04, /*!< \brief If set, linebreak (i.e. \c \\ or \c \\newline )  are allowed, otherwise a single line wihtout such linebreak commands is expected */
            DefaultParseOptions=AllowLinebreaks,
        };
        Q_DECLARE_FLAGS(ParseOptions, ParseOption)
        Q_FLAG(ParseOptions)
        /** \brief lists the parser classes that are available by default (convenience interface to the templated parse()-funktion */
        enum DefaultParserTypes {
            LatexParser, /*!< \brief use the LaTeX parser from JKQTMathTextLatexParser */
            DefaultParser=LatexParser
        };
        /** \brief parse the given math \a markup string with a parser derived from \a markupType.
         *
         *  \param markup the string of math markup
         *  \param markupType defines the language the \a markup is written in (and  is used to derive the parser to use)
         *  \param options Options for parsing, \see ParseOptions
         *
         *  \returns \c true on success.
         */
        bool parse(const QString &markup, DefaultParserTypes markupType=DefaultParser, ParseOptions options=DefaultParseOptions);
        /** \brief parse the given math \a markup string, using the given parser class \a TParser
         *
         *  \tparam TParser the  parser (deived from JKQTMathTextParser) to be used
         *  \param markup the string of math markup
         *  \param options Options for parsing, \see ParseOptions
         *
         *  \returns \c true on success.
         */
        template <class TParser>
        inline bool parse(const QString &markup, ParseOptions options=DefaultParseOptions) {
            static_assert(std::is_base_of<JKQTMathTextParser, TParser>::value, "in parse<TParser>() the type TParser has to be derived from JKQTMathTextParser to work!");
            std::unique_ptr<TParser> p=std::unique_ptr<TParser>(new TParser(this));
            if (parsedNode) delete parsedNode;
            parsedNode=nullptr;
                        clearErrorList();
            parsedNode=p->parse(markup, options);
            return parsedNode!=nullptr;
        }
        /** \brief returns the syntax tree of JKQTMathTextNode's that was created by the last parse() call */
        JKQTMathTextNode* getNodeTree() ;
        /** \copydoc parsedNode */
        const JKQTMathTextNode *getNodeTree() const;
/**@}*/


/** @name Determine the Size of the Output */
/**@{*/

        /** \brief get the size of the drawn representation. returns an invalid size if no text has been parsed. */
        QSizeF getSize(QPainter& painter);
        /** \brief get the rounded (using ceil) to an integer size of the drawn representation. returns \c QSize(0,0) if no text has been parsed. */
        QSize getIntSize(QPainter& painter);
        /** \brief return the descent, i.e. the distance from the baseline to the lowest part of the representation */
        double getDescent(QPainter& painter);
        /** \brief return the ascentt, i.e. the distance from the baseline to the highest part of the representation */
        double getAscent(QPainter& painter);
        /** \brief return the detailes sizes of the text */
        void getSizeDetail(QPainter& painter, double& width, double& ascent, double& descent, double& strikeoutPos);
        /** \brief return the detailes sizes of the text */
        JKQTMathTextNodeSize getSizeDetail(QPainter& painter);
/**@}*/


/** @name Rendering */
/**@{*/

        /** \brief draw a representation to the  object at the specified position \a x , \a y
         *
         *  \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to use for drawing
         *  \param x position of the left border of the text/expression to be drawn (see sketch below)
         *  \param drawBoxes if \c true boxes defining the size of each node are drawn, example output: \image html jkqtmathtext/jkqtmathtext_drawboxes.png
         *
         *  Here is an illustration of the geometry of the drawn text/expression:
         *  \image html jkqtmathtext/jkqtmathtext_node_geo.png
         */
        void draw(QPainter& painter, QPointF x, bool drawBoxes=false);
        /** \brief draw a representation to the  object at the specified position \a x , \a y
         *
         *  \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to use for drawing
         *  \param x x-position of the left border of the text/expression to be drawn (see sketch below)
         *  \param y y-position of the baseline of the text/expression to be drawn (see sketch below)
         *  \param drawBoxes if \c true boxes defining the size of each node are drawn, example output: \image html jkqtmathtext/jkqtmathtext_drawboxes.png
         *
         *  Here is an illustration of the geometry of the drawn text/expression:
         *  \image html jkqtmathtext/jkqtmathtext_node_geo.png
         */
        double draw(QPainter& painter, double x, double y, bool drawBoxes=false);
        /** \brief draw into a rectangle \a rect with alignment defined in \a flags (see below)
         *
         *  \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to use for drawing
         *  \param rect rectangle to draw the text/expression into (see sketch below)
         *  \param flags alignment within \a rect (see below), use e.g. <tt>Qt::AlignHCenter | Qt::AlignVCenter</tt> to center the expression inside \a rect
         *               The flags (dark-red is the rectangle \a rect) are interpreted in the following way:
         *               \image html jkqtmathtext/jkqtmathtext_draw_flags.png
         *  \param drawBoxes if \c true boxes defining the size of each node are drawn, example output: \image html jkqtmathtext/jkqtmathtext_drawboxes.png
         *
         *
         *
         *  \see drawIntoPixmap(), drawIntoPicture(), getSize(), getSizeDetail()
         */
        void draw(QPainter& painter, unsigned int flags, QRectF rect, bool drawBoxes=false);

        /** \brief render the last  parse result into a <a href="https://doc.qt.io/qt-6/qpixmap.html">QPixmap</a>
         *
         *  \param drawBoxes if \c true boxes defining the size of each node are drawn, example output: \image html jkqtmathtext/jkqtmathtext_drawboxes.png
         *  \param backgroundColor fill color for the returnes QPixmap
         *  \param sizeincrease margin around the tight size of the rendering result for the returned QPixmap
         *  \param devicePixelRatio the devicePixelRatio of the returned QPixmap
         */
        QPixmap drawIntoPixmap(bool drawBoxes=false, QColor backgroundColor=QColor(Qt::white), int sizeincrease=0, qreal devicePixelRatio=1.0);

        /** \brief render the last  parse result into a <a href="https://doc.qt.io/qt-6/qimage.html">QImage</a>
         *
         *  \param drawBoxes if \c true boxes defining the size of each node are drawn, example output: \image html jkqtmathtext/jkqtmathtext_drawboxes.png
         *  \param backgroundColor fill color for the returnes QPixmap
         *  \param sizeincrease margin around the tight size of the rendering result for the returned QPixmap
         *  \param devicePixelRatio the devicePixelRatio of the returned QImage
         *  \param resolution_dpi resolution in dots/inch
         */
        QImage drawIntoImage(bool drawBoxes=false, QColor backgroundColor=QColor(Qt::white), int sizeincrease=0, qreal devicePixelRatio=1.0, unsigned int resolution_dpi=96);

        /** \brief render the last  parse result into a <a href="https://doc.qt.io/qt-6/qpicture.html">QPicture</a>
         *
         *  \param drawBoxes if \c true boxes defining the size of each node are drawn, example output: \image html jkqtmathtext/jkqtmathtext_drawboxes.png
         */
        QPicture drawIntoPicture(bool drawBoxes=false);


        /** \brief convert LaTeX to HTML. returns \c ok=true on success and \c ok=false else. */
        QString toHtml(bool* ok=nullptr, double fontPointSize=10);
/**@}*/

/** @name Font Handling */
/**@{*/
        /** \brief options for the base font */
        enum BaseFontOption {
            BaseFontBold = 0x01, /*!< \brief output is bold (equivalent to placinf text in <code>\textbf{...}</code>) */
            BaseFontItalic = 0x02, /*!< \brief output is italic (equivalent to placinf text in <code>\textit{...}</code>) */
            BaseFontUnderlined = 0x04, /*!< \brief output is underlined (equivalent to placinf text in <code>\underlined{...}</code>) */
            BaseFontSmallCaps = 0x08, /*!< \brief output is small-caps (equivalent to placinf text in <code>\textsc{...}</code>) */
            BaseFontDefault=0x00, /*!< \brief base font is used as is, without modifications */
        };
        Q_DECLARE_FLAGS(BaseFontOptions, BaseFontOption)
        Q_FLAG(BaseFontOptions)
        /** \brief convert BaseFontOptions to a string (e.g. <code>BOLD+ITALIC</code> */
        static QString BaseFontOptions2String(BaseFontOptions opt);
        /** \brief convert a string (generated by BaseFontOptions2String() to BaseFontOptions */
        static BaseFontOptions String2BaseFontOptions(const QString& s);
        /** \copydoc fontColor */
        void setFontColor(const QColor & __value);
        /** \copydoc fontColor */ 
        QColor getFontColor() const;
        /** \copydoc setFontPointSize() */
        void setFontSize(double __value);
        /** \brief set the default font size in points
         *  \see getFontSize(), fontSize, fontSizeUnits */
        void setFontPointSize(double __value);
        /** \brief set the default font soze in pixels
         *  \see getFontSizePixels(), fontSize, fontSizeUnits */
        void setFontSizePixels(double __value);
        /** \brief returns the currently set default font size in points, if it was defined in points using setFontSize(), or -1 if it was set in pixels with setFontSizePixels()
         *  \see setFontSize(), fontSize, fontSizeUnits */
        double getFontPointSize() const;
        /** \copydoc getFontPointSize() */
        double getFontSize() const;
        /** \brief returns the currently set default font size in pixels, if it was defined in points using setFontSizePixels(), or -1 if it was set in points with setFontSize()
         *  \see setFontSizePixels(), fontSize, fontSizeUnits */
        double getFontSizePixels() const;
        /** \brief set (overwrite) the options applied to the base font
         *
         *  \see BaseFontOptions
         */
        void setFontOptions(BaseFontOptions opts);
        /** \brief en/disable the given base font option
         *
         *  \see BaseFontOptions
         */
        void setFontOption(BaseFontOption opt,bool enabled=true);
        /** \brief returns the options applied to the base font
         *
         *  \see BaseFontOptions
         */
        BaseFontOptions getFontOptions() const;


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
        void addReplacementFont(const QString& nonUseFont, const QString& useFont, JKQTMathTextFontEncoding useFontEncoding);
        /** \brief retrieves a replacement for the given font name \a nonUseFont, including its encoding. Returns the given default values \a defaultFont and/or \a defaultFontEncoding if one of the two is not found */
        QPair<QString, JKQTMathTextFontEncoding> getReplacementFont(const QString &nonUseFont, const QString &defaultFont, JKQTMathTextFontEncoding defaultFontEncoding) const;

        /** \brief retrieve the font and encoding to be used for \a font, which might optionally be typeset inside a math environment, specified by in_math_environment, possibly for the given font subclass \a subclass */
        QPair<QString, JKQTMathTextFontEncoding> getFontData(JKQTMathTextEnvironmentFont font, bool in_math_environment=false) const;

        /*! \brief set the font by parsing a special syntax defined in the description of JKQTMathTextFontSpecifier
         *
         *  This allows to set the text-mode and math-mode roman fonts.
         *
         *   \note This function is a shorthand for
         *   \code
         *     setFontRomanOrSpecial(JKQTMathTextFontSpecifier::fromFontSpec(fontName), encoding);
         *   \endcode
         *
         *  \see JKQTMathTextFontSpecifier, setFontRoman(), setFontMathRoman(),  \ref jkqtmathtext_fonthandling
         */
        void setFontRomanOrSpecial(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /*! \brief set the font by an instance of JKQTMathTextFontSpecifier
         *
         *  This allows to set the text-mode and math-mode roman fonts.
         *
         *  \see JKQTMathTextFontSpecifier, setFontRoman(), setFontMathRoman(),  \ref jkqtmathtext_fonthandling
         */
        void setFontRomanOrSpecial(const JKQTMathTextFontSpecifier & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /*! \brief set font(s) from parsing a font specifier
         *
         *  This function understands a font-specifier in the following forms:
         *    - \c fontRoman - set "roman" and "math-roman" to fontRoman "sans"/"math-sans" are reset to the default, when constructing the object
         *    - \c fontRoman/fontSans -  set "roman" and "math-roman" to fontRoman and "sans" and "math-sans" to fontSans
         *    - \c fontRoman/fontSans/fontMathRoman/fontMathSans -  sets all four majpr fonts
         *    - any above with addition:
         *        - \c +XITS use XITS for math/symbols, calls useXITS()
         *        - \c +ASANA use ASANA for math/symbols, calls useASANA()
         *        - \c +STIX use STIX for math/symbols, calls useSTIX()
         *        - \c +FIRA use Fira Math for math/symbols, calls useFiraMath()
         *        - \c +BOLD make font initially bold
         *        - \c +ITALIC make font initially italic
         *        - \c +UNDERLINED make font initially underlines
         *        - \c +SMALLCAPS make font initially smallcaps
         *      .
         *    - \c GUI use GUI-fonts, calls useGuiFonts()
         *  .
         *
         *  Font names can also be one of the special names, specified in the documentation of JKQTMathTextFontSpecifier and JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts()
         *
         */
        void setFontSpecial(const QString& fontSpec);

        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEroman
         *
         *  \see  \ref jkqtmathtext_fonthandling
          */
        void setFontRoman(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text and math, i.e. the logical font MTEroman and MTEmathRoman
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontRomanAndMath(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEroman   */
        QString getFontRoman() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEsans
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontSans(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEsans   */
        QString getFontSans() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEtypewriter
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontTypewriter(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEtypewriter   */
        QString getFontTypewriter() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEscript
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontScript(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEscript   */
        QString getFontScript() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEfraktur
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontFraktur(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEfraktur   */
        QString getFontFraktur() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEcaligraphic
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontCaligraphic(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEcaligraphic   */
        QString getFontCaligraphic() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEblackboard \see setFontBlackboardMode()
         *
         *
         */
        void setFontBlackboard(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \copydoc blackboradFontMode */
        JKQTMathTextBlackboradDrawingMode getFontBlackboradMode() const;
        /** \copydoc blackboradFontMode */
        void setFontBlackboradMode(JKQTMathTextBlackboradDrawingMode mode);

        /** \brief retrieves the font to be used for text in the logical font MTEblackboard \see blackboradFontMode  */
        QString getFontBlackboard() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for symbols in the logical font \a font
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFallbackFontSymbols(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for symbols in the logical font \a font   */
        QString getFallbackFontSymbols() const;

        /** \brief retrieves the encoding used for the symbol font to be used for symbols   */
        JKQTMathTextFontEncoding getFontEncodingFallbackFontSymbols() const;
        /** \brief retrieves the encoding used for the script font  */
        JKQTMathTextFontEncoding getFontEncodingScript() const;
        /** \brief retrieves the encoding used for the Fraktur font  */
        JKQTMathTextFontEncoding getFontEncodingFraktur() const;
        /** \brief retrieves the encoding used for the typewriter font  */
        JKQTMathTextFontEncoding getFontEncodingTypewriter() const;
        /** \brief retrieves the encoding used for the sans-serif font  */
        JKQTMathTextFontEncoding getFontEncodingSans() const;
        /** \brief retrieves the encoding used for the roman font  */
        JKQTMathTextFontEncoding getFontEncodingRoman() const;
        /** \brief retrieves the encoding used for the blackboard font  */
        JKQTMathTextFontEncoding getFontEncodingBlackboard() const;
        /** \brief retrieves the encoding used for the caligraphic font  */
        JKQTMathTextFontEncoding getFontEncodingCaligraphic() const;


        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEmathRoman
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontMathRoman(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEroman   */
        QString getFontMathRoman() const;
        /** \brief set the font \a fontName and it's encoding \a encoding to be used for text in the logical font MTEmathSans
         *
         *  \see  \ref jkqtmathtext_fonthandling
         */
        void setFontMathSans(const QString & fontName, JKQTMathTextFontEncoding encoding=JKQTMathTextFontEncoding::MTFEStandard);
        /** \brief retrieves the font to be used for text in the logical font MTEsans   */
        QString getFontMathSans() const;
        /** \brief retrieves the encoding used for the math-mode sans-serif font  */
        JKQTMathTextFontEncoding getFontEncodingMathSans() const;
        /** \brief retrieves the encoding used for the math-mode roman font  */
        JKQTMathTextFontEncoding getFontEncodingMathRoman() const;

        /** \brief configures the class to use the STIX fonts in mathmode
         *
         * use STIX (1.x/2.x) fonts from <a href="https://www.stixfonts.org/">https://www.stixfonts.org/</a> in math-mode
         * or in math and text-mode (if \a mathModeOnly \c ==false )
         *
         * If \c useAsFallbackSymbol is set \c ==true then the XITS fonts are also used as MTEFallbackSymbols
         *
         * \image html jkqtmathtext/jkqtmathtext_stix.png
         *
         * This function does not only use default font-names for STIX, but searches
         * the font database of the system with several different variants, using JKQTMathTextFontSpecifier::getSTIXFamilies().
         * It also sets the special math-variant of STIX for math mode and the normal variant for text-mode
         *
         * \see \ref jkqtmathtext_fonthandling
         */
        bool useSTIX(bool mathModeOnly=true, bool useAsFallbackSymbol=true);

        /** \brief configures the class to use the XITS fonts in mathmode
         *
         * use XITS fonts from <a href="https://github.com/alif-type/xits">https://github.com/alif-type/xits</a> in math-mode
         * or in math and text-mode (if \a mathModeOnly \c ==false ).
         *
         * If \c useAsFallbackSymbol is set \c ==true then the XITS fonts are also used as MTEFallbackSymbols
         *
         * \image html jkqtmathtext/jkqtmathtext_xits.png
         *
         * \note The XITS fonts can be compiled into JKQTPlotter, when the CMake-option \c JKQtPlotter_BUILD_INCLUDE_XITS_FONTS is set to ON (default: ON).
         *       Then the XITS fonts are added as Qt-Ressources to the library binary.
         *       If this is not the case, you have to provide the XITS fonts on the target system by other means, if you want
         *       to use them.
         *
         * This function does not only use default font-names for XITS, but searches
         * the font database of the system with several different variants, using JKQTMathTextFontSpecifier::getXITSFamilies().
         * It also sets the special math-variant of XITS for math mode and the normal variant for text-mode
         *
         * \see \ref jkqtmathtext_fonthandling
         */
        bool useXITS(bool mathModeOnly=true, bool useAsFallbackSymbol=true);

        /** \brief configures the class to use the Fira Math fonts in mathmode
         *
         * use Fira Math fonts from <a href="https://github.com/firamath/firamath">https://github.com/firamath/firamath</a> in math-mode
         * or in math and text-mode (if \a mathModeOnly \c ==false ).
         *
         * If \c useAsFallbackSymbol is set \c ==true then the Fira Math fonts are also used as MTEFallbackSymbols
         *
         * \image html jkqtmathtext/jkqtmathtext_usefira.png
         *
         * \note The Fira Math fonts can be compiled into JKQTPlotter, when the CMake-option \c JKQtPlotter_BUILD_INCLUDE_FIRAMATH_FONTS is set to ON (default: ON).
         *       Then the Fira Math fonts are added as Qt-Ressources to the library binary.
         *       If this is not the case, you have to provide the Fira Math fonts on the target system by other means, if you want
         *       to use them.
         *
         * This function does not only use default font-names for Fira Math, but searches
         * the font database of the system with several different variants, using JKQTMathTextFontSpecifier::getFIRAFamilies().
         * It also sets the special math-variant of Fira for math mode and the normal variant for text-mode
         *
         * \see \ref jkqtmathtext_fonthandling
         */
        bool useFiraMath(bool mathModeOnly=true, bool useAsFallbackSymbol=true);

        /** \brief configures the class to use the ASANA fonts in mathmode
         *
         * use the ASANA fonts from <a href="https://ctan.org/tex-archive/fonts/Asana-Math/">https://ctan.org/tex-archive/fonts/Asana-Math/</a> in math-mode
         * or in math and text-mode (if \a mathModeOnly \c ==false )
         *
         * If \c useAsFallbackSymbol is set \c ==true then the XITS fonts are also used as MTEFallbackSymbols
         *
         * \image html jkqtmathtext/jkqtmathtext_asana.png
         *
         * This function does not only use default font-names for ASANA, but searches
         * the font database of the system with several different variants, using JKQTMathTextFontSpecifier::getASANAFamilies().
         * It also sets the special math-variant of ASANA for math mode and the normal variant for text-mode
         *
         * \see \ref jkqtmathtext_fonthandling
         */
        bool useASANA(bool mathModeOnly=true, bool useAsFallbackSymbol=true);

        /** \brief configures the class to be used for GUI output, i.e. tries to find fonts that are harmonious for the
         *         default GUI font, sets the default GUI font for "roman" and finds an oppposite font for "sans", which means that
         *         "sans" may actually be a serif font,when the app font is sans serif!
         *
         *
         * \see \ref jkqtmathtext_fonthandling
         */
        bool useGuiFonts();

        /** \brief sets \a timesFont (with its encoding \a encodingTimes ) for serif-text and \a sansFont (with its encoding \a encodingSans ) for both mathmode and textmode fonts
         *
         * \note use generic Unicode fonts, e.g. "Arial" and "Times New Roman" in math-mode.
         *       You should use fonts that contain as many of the mathematical symbols as possible
         *       to ensure good rendering results.
         *
         *
         * <code>useAnyUnicode("Comic Sans MS", "Comic Sans MS")</code>:<br>\image html jkqtmathtext/jkqtmathtext_comicsans.png  <br><br>
         * <code>useAnyUnicodeForTextOnly("Comic Sans MS", "Comic Sans MS");</code>:<br/>\image html jkqtmathtext/jkqtmathtext_comicsans_textonly.png
         * <code>useAnyUnicodeForMathOnly("Comic Sans MS", "Comic Sans MS");</code>:<br/>\image html jkqtmathtext/jkqtmathtext_comicsans_mathonly.png
         *
         * \see useAnyUnicodeForMathOnly(), useAnyUnicodeForTextOnly(), \ref jkqtmathtext_fonthandling
         */
        void useAnyUnicode(QString timesFont, const QString& sansFont, JKQTMathTextFontEncoding encodingTimes=JKQTMathTextFontEncoding::MTFEUnicode, JKQTMathTextFontEncoding encodingSans=JKQTMathTextFontEncoding::MTFEUnicode);
        /** \brief sets \a timesFont (with its encoding \a encodingTimes ) for serif-text and \a sansFont (with its encoding \a encodingSans ) for mathmode fonts only
         *
         * \note use generic Unicode fonts, e.g. "Arial" and "Times New Roman" in math-mode.
         *       You should use fonts that contain as many of the mathematical symbols as possible to ensure good rendering results.
         *
         * <code>useAnyUnicode("Comic Sans MS", "Comic Sans MS")</code>:<br>\image html jkqtmathtext/jkqtmathtext_comicsans.png  <br><br>
         * <code>useAnyUnicodeForTextOnly("Comic Sans MS", "Comic Sans MS");</code>:<br/>\image html jkqtmathtext/jkqtmathtext_comicsans_textonly.png
         * <code>useAnyUnicodeForMathOnly("Comic Sans MS", "Comic Sans MS");</code>:<br/>\image html jkqtmathtext/jkqtmathtext_comicsans_mathonly.png
         *
         * \see useAnyUnicodeForTextOnly(), useAnyUnicode(), \ref jkqtmathtext_fonthandling
         */
        void useAnyUnicodeForMathOnly(QString timesFont, const QString& sansFont, JKQTMathTextFontEncoding encodingTimes=JKQTMathTextFontEncoding::MTFEUnicode, JKQTMathTextFontEncoding encodingSans=JKQTMathTextFontEncoding::MTFEUnicode);
        /** \brief sets \a timesFont (with its encoding \a encodingTimes ) for serif-text and \a sansFont (with its encoding \a encodingSans ) for both mathmode fonts only
         *
         * <code>useAnyUnicode("Comic Sans MS", "Comic Sans MS")</code>:<br>\image html jkqtmathtext/jkqtmathtext_comicsans.png  <br><br>
         * <code>useAnyUnicodeForTextOnly("Comic Sans MS", "Comic Sans MS");</code>:<br/>\image html jkqtmathtext/jkqtmathtext_comicsans_textonly.png
         * <code>useAnyUnicodeForMathOnly("Comic Sans MS", "Comic Sans MS");</code>:<br/>\image html jkqtmathtext/jkqtmathtext_comicsans_mathonly.png
         *
         *  \see useAnyUnicodeForMathOnly(), useAnyUnicode(), \ref jkqtmathtext_fonthandling
         */
        void useAnyUnicodeForTextOnly(QString timesFont, const QString& sansFont, JKQTMathTextFontEncoding encodingTimes=JKQTMathTextFontEncoding::MTFEUnicode, JKQTMathTextFontEncoding encodingSans=JKQTMathTextFontEncoding::MTFEUnicode);
/**@}*/


/** @name (Defining) Rendering Details */
/**@{*/
        /** \brief load the object settings from the given QSettings object with the given name prefix */
        void loadSettings(const QSettings& settings, const QString& group=QString("mathtext/"));
        /** \brief store the object settings to the given QSettings object with the given name prefix */
        void saveSettings(QSettings& settings, const QString& group=QString("mathtext/")) const;

        /** \copydoc brace_factor */ 
        void setBraceFactor(double __value);
        /** \copydoc brace_factor */ 
        double getBraceFactor() const;
        /** \copydoc subsuper_size_factor */
        void setSubsuperSizeFactor(double __value);
        /** \copydoc subsuper_size_factor */
        double getSubsuperSizeFactor() const;
        /** \copydoc subsuper_mode_selection_by_size_factor */
        void setSubsuperModeSelectionBySizeFactor(double __value);
        /** \copydoc subsuper_mode_selection_by_size_factor */
        double getSubsuperModeSelectionBySizeFactor() const;
        /** \copydoc operatorsubsuper_size_factor */ 
        void setOperatorsubsuperSizeFactor(double __value);
        /** \copydoc operatorsubsuper_size_factor */ 
        double getOperatorsubsuperSizeFactor() const;
        /** \copydoc operatorsubsuper_distance_factor */
        void setOperatorsubsuperDistanceFactor(double __value);
        /** \copydoc operatorsubsuper_distance_factor */
        double getOperatorsubsuperDistanceFactor() const;
        /** \copydoc operatorsubsuper_extraspace_factor */
        void setOperatorsubsuperExtraSpaceFactor(double __value);
        /** \copydoc operatorsubsuper_extraspace_factor */
        double getOperatorsubsuperExtraSpaceFactor() const;
        /** \copydoc mathoperator_width_factor */
        void setMathoperatorWidthFactor(double __value);
        /** \copydoc mathoperator_width_factor */
        double getMathoperatorWidthFactor() const;
        /** \copydoc intsubsuper_xcorrection_factor */
        void setIntSubSuperXCorrectionFactor(double __value);
        /** \copydoc intsubsuper_xcorrection_factor */
        double getIntSubSuperXCorrectionFactor() const;
        /** \copydoc intsubbesides_xcorrection_xfactor */
        void setIntSubBesidesXCorrectionXFactor(double __value);
        /** \copydoc intsubbesides_xcorrection_xfactor */
        double getIntSubBesidesXCorrectionXFactor() const;
        /** \copydoc bigmathoperator_font_factor */
        void setBigMathoperatorFontFactor(double __value);
        /** \copydoc bigmathoperator_font_factor */
        double getBigMathoperatorFontFactor() const;
        /** \copydoc super_shift_factor */
        void setSuperShiftFactor(double __value);
        /** \copydoc super_shift_factor */
        double getSuperShiftFactor() const;
        /** \copydoc sub_shift_factor */
        void setSubShiftFactor(double __value);
        /** \copydoc sub_shift_factor */
        double getSubShiftFactor() const;
        /** \copydoc special_super_shift_factor */
        void setSpecialSuperShiftFactor(double __value);
        /** \copydoc special_super_shift_factor */
        double getSpecialSuperShiftFactor() const;
        /** \copydoc special_sub_shift_factor */
        void setSpecialSubShiftFactor(double __value);
        /** \copydoc special_sub_shift_factor */
        double getSpecialSubShiftFactor() const;
        /** \copydoc underbrace_factor */ 
        void setUnderbraceFactor(double __value);
        /** \copydoc underbrace_factor */ 
        double getUnderbraceFactor() const;
                /** \copydoc underbrace_separation_xfactor */
        void setUnderbraceSeparationXFactor(double __value);
        /** \copydoc underbrace_separation_xfactor */
        double getUnderbraceSeparationXFactor() const;
        /** \copydoc underbrace_bracesize_xfactor */
        void setUnderbraceBraceSizeXFactor(double __value);
        /** \copydoc underbrace_bracesize_xfactor */
        double getUnderbraceBraceSizeXFactor() const;
        /** \copydoc underset_factor */
        void setUndersetFactor(double __value);
        /** \copydoc underset_factor */
        double getUndersetFactor() const;
        /** \copydoc frac_factor */
        void setFracFactor(double __value);
        /** \copydoc frac_factor */
        double getFracFactor() const;
        /** \copydoc frac_nested_factor */
        void setFracNestedFactor(double __value);
        /** \copydoc frac_nested_factor */
        double getFracNestedFactor() const;


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
        /** \copydoc decoration_separation_factor */
        void setDecorationSeparationXFactor(double __value);
        /** \copydoc decoration_separation_factor */
        double getDecorationSeparationFactor() const;
        /** \copydoc decoration_width_reduction_Xfactor */
                void setDecorationWidthReductionFactor(double __value);
        /** \copydoc decoration_width_reduction_Xfactor */
        double getDecorationWidthReductionXFactor() const;


        /** \copydoc sqrt_width_Xfactor */
        void setSqrtWidthXFactor(double __value);
        /** \copydoc sqrt_width_Xfactor */
        double getSqrtWidthXFactor() const;
        /** \copydoc sqrt_height_factor */
        void setSqrtHeightFactor(double __value);
        /** \copydoc sqrt_height_factor */
        double getSqrtHeightFactor() const;
        /** \copydoc sqrt_smallfont_factor */
        void setSqrtSmallFontFactor(double __value);
        /** \copydoc sqrt_smallfont_factor */
        double getSqrtSmallFontFactor() const;

        /** \copydoc matrix_linewidth_thin_factor */
        double getMatrixLinewidthThinFactor();
        /** \copydoc matrix_linewidth_thin_factor */
        void setMatrixLinewidthThinFactor(double factor);
        /** \copydoc matrix_linewidth_heavy_factor */
        double getMatrixLinewidthHeavyFactor();
        /** \copydoc matrix_linewidth_heavy_factor */
        void setMatrixLinewidthHeavyFactor(double factor);
        /** \copydoc matrix_line_separation_factor */
        double getMatrixLineSeparationFactor();
        /** \copydoc matrix_line_separation_factor */
        void setMatrixLineSeparationFactor(double factor);
        /** \copydoc matrix_xSeparation_factor */
        double getMatrixXSeparationFactor();
        /** \copydoc matrix_xSeparation_factor */
        void setMatrixXSeparationFactor(double factor);
        /** \copydoc matrix_ySeparation_factor */
        double getMatrixYSeparationFactor();
        /** \copydoc matrix_ySeparation_factor */
        void setMatrixYSeparationFactor(double factor);
        /** \copydoc matrix_xPadding_factor */
        double getMatrixXPaddingFactor();
        /** \copydoc matrix_xPadding_factor */
        void setMatrixXPaddingFactor(double factor);
        /** \copydoc matrix_yPadding_factor */
        double getMatrixYPaddingFactor();
        /** \copydoc matrix_yPadding_factor */
        void setMatrixYPaddingFactor(double factor);
/**@}*/


/** @name Error Handling */
/**@{*/
        /** \copydoc error_list */
        QStringList getErrorList() const;
        /** \brief returns \c true when errors were registered in the system \see error_list */
        bool hadErrors() const;
    protected:
        /** \copydoc error_list */
        void addToErrorList(const  QString& error);
        /** \brief clears all registered errors (see error_list)
         */
        void clearErrorList();

        /** \brief a list that will be filled with error messages while parsing, if any error occur
         *
         *  This list of errors is (mostly) filled during a call to parse(). During rendering (e.g. with draw() )
         *  only very few errors will be detected, as most errors are caused by wrong markup.
         *
         *  A call to parse() also clears this list.
         *
         *  \see getErrorList(), hadErrors() and addToErrorList()
         *
          */
        QStringList error_list;
/**@}*/

        /** \brief the syntax tree of JKQTMathTextNode's that was created by the last parse() call */
        JKQTMathTextNode* parsedNode;



        /** \brief table with font replacements to use (e.g. if it is known that a certain font is not good for rendering, you can add
         *         an alternative using addReplacementFont(). These are automatically applied, when setting a new font name! */
        QMap<QString, QString> fontReplacements;
        /** \brief acompanies fontReplacements and collects the encodings of the replacement fonts, if no entry is present, the default encoding is used, as given to the setter! */
        QMap<QString, JKQTMathTextFontEncoding> fontEncodingReplacements;

        /** \brief font color */
        QColor fontColor;
        /** \brief base font size in the units defined in fontSizeUnits \see fontSizeUnits */
        double fontSize;
        /** \brief unit of fontSize */
        JKQTMathTextEnvironment::FontSizeUnit fontSizeUnits;
        /** \brief additional options for the base fonts */
        BaseFontOptions fontOptions;
        /** \brief sets all properties of JKQTMathTextEnvironment \a ev from the fon settings stored in this class (e.g. fontSize, fontOptions, ...) */
        void modifyEnvironmentFromFontSettings(JKQTMathTextEnvironment& ev) const;


        /** \brief stores information about the different fonts used by LaTeX markup */
        QHash<JKQTMathTextEnvironmentFont, JKQTMathTextFontDefinition> fontDefinitions;

        /** \brief specifies how to draw blackboard font characters (i.e. \c \\mathbb{N} )
         *
         *  Blackboard fonts are not widely available on target systems (viable fonts are e.g.
         *  <code>"Double Stroke", "CloisterOpenFace BT", "GoudyHandtooled BT", "Castellar", "MathJax_AMS", "Castellar Standard", "MathJax_AMS Standard", "Colonna MT"</code>).
         *  But the most important blackboard characters are usually available in the higher unicode
         *  codepoints of Fonts specialized for math (e.g. XIST, STIX, ASANA).
         *
         *  Therefore JKQTMathText supports using these characters, or simulating a blackboard font in
         *  addition to using one of the fonts above. You can set that by setting
         *  JKQTMathText::setFontBlackboradMode() with one of the options from JKQTMathTextBlackboradDrawingMode:
         *    - MTBBDMfontDirectly: use a blackboard font specified by JKQTMathText::setFontBlackboard() \image html jkqtmathtext/jkqtmathtext_bb_font_directly.png using \c JKQTMathText::setFontBlackboard("Castellar")
         *    - MTBBDMsimulate: \image html jkqtmathtext/jkqtmathtext_bb_simulate.png using \c JKQTMathText::setFontBlackboard("Arial")
         *    - MTBBDMunicodeCharactersOrFontDirectly: \image html jkqtmathtext/jkqtmathtext_bb_unicode_or_font_directly.png using \c JKQTMathText::setFontBlackboard("Castellar")
         *    - MTBBDMunicodeCharactersOrSimulate: \image html jkqtmathtext/jkqtmathtext_bb_unicode_or_simulate.png using \c JKQTMathText::setFontBlackboard("Arial")
         *  .
         *
         *  \see JKQTMathTextBlackboradDrawingMode, setFontBlackboard(), setBlackboardFontMode(),  \ref jkqtmathtext_fonthandling
         */
        JKQTMathTextBlackboradDrawingMode blackboradFontMode;


        /** \brief for \c \\left / \c \\right braces: The brace is this factor higher than the child block it surrounds */
        double brace_factor;
        /** \brief resizing factor for font size in sub-/superscript */
        double subsuper_size_factor;
        /** \brief this factor is used to determine how to typeset sub-/superscript.
         *
         *  If the ascent for superscript of descent for subscript of the previous character is  \c >=subsuper_mode_selection_by_size_factor*ascent(currentFont)
         *  or \c <=subsuper_mode_selection_by_size_factor*descent(currentFont) respectively, the sub/superscript is typeset, aligned with the ascent or descent
         *  of the previous character. Otherwise it is aligned with the default method:
         *
         *  <b>Default mode:</b>
         *  \image html jkqtmathtext_superscriptnode_getSizeInternal.png
         *  \image html jkqtmathtext_subscriptnode_getSizeInternal.png
         *
         *  <b>Special mode:</b>
         *
         *
         *  This method fixes problems with characters that are significantyl larger that normal text character of the fonst, such as generated
         *  by \c \\sum,\\int,... .
         */
        double subsuper_mode_selection_by_size_factor;
        /** \brief like subsuper_size_factor, but for operators (\c \\sum , \c \\int , ...) where the text is placed above/below the symbol */
        double operatorsubsuper_size_factor;
        /** \brief for operators (\c \\sum , \c \\int , ...) where the text is placed above/below the symbol, this is the distance between the operator symbol and the sub-/super-text if multiplied by xHeight
         *
         *  \image html jkqtmathtext_subsuper_with_limits.png
         */
        double operatorsubsuper_distance_factor;
        /** \brief for operators (\c \\sum , \c \\int , ...) where the text is placed above/below the symbol, this is the additional width added to the width of maximum width of the operator, above and below
         *
         *  \image html jkqtmathtext_subsuper_with_limits.png
         */
        double operatorsubsuper_extraspace_factor;
        /** \brief for integrals (\c \\int , \c \\oint , ...) the sub-/superscripts above/below the symbol have to be shifted a bit to the left/right to accomodate the shape of the operator symbol (i.e. some free space at the top-left and bottom-right)
         *
         *  \image html jkqtmathtext/jkqtmathtext_doc_subsupershift_integral.png
         *
         *  \image html jkqtmathtext/jkqtmathtext_doc_subsupershift_integral_boxes.png
         *
         *  This factor is multiplied by the symbol width: xshift=intsubsuper_xcorrection_factor*symbolWidth
         *  Then the subscript below is placed at centerx(symbol)-xshift and the superscript at centerx(symbol)+shiftx.
         *  This is also used to correct a subset next to the symbol by shifting it to rightx(symbol)-xshift.
         *
         *  This correction is applied to \\int, \\iint, \\iiint, \\oint, ...
         */
        double intsubsuper_xcorrection_factor;
        /** \brief for integrals (\c \\int , \c \\oint , ...) the subscripts besides the symbol have to be shifted to the left a bit to the left to accomodate the shape of the operator symbol (i.e. some free space at the bottom-right)
         *
         *  \image html jkqtmathtext/jkqtmathtext_doc_subsupershift_integral.png
         *
         *  \image html jkqtmathtext/jkqtmathtext_doc_subsupershift_integral_boxes.png
         *
         *  This factor is multiplied by the width of an x: xshift=intsubbesides_xcorrection_xfactor*xWidth
         *  Then the subscript besides the symbol is shifted by xshift to the left
         *
         *  This correction is applied to \\int, \\iint, \\iiint, \\oint, ...
         */
        double intsubbesides_xcorrection_xfactor;
        /** \brief factor, used to extend the size of an operator in math mode
         *
         *  The next image demonstrates the effect of this property, which adds extra space
         *  around certain math operators in math mode:
         *
         *  \image html jkqtmathtext/jkqtmathtext_mathoperator_width_factor.png
         */
        double mathoperator_width_factor;
        /** \brief factor, used to increase the font size for big math operators, such as \c \\sum , \c \\prod , ...
         *
         */
        double bigmathoperator_font_factor;
        /** \brief relative shift of text in superscript to normal text, factor is multiplied by \c font.xHeight()
         *
         *  \image html jkqtmathtext_superscriptnode_getSizeInternal.png
         */
        double super_shift_factor;
        /** \brief relative shift of text in subscript to normal text, factor is multiplied by \c font.xHeight()
         *
         *  \image html jkqtmathtext_subscriptnode_getSizeInternal.png
         */
        double sub_shift_factor;
        /** \brief relative shift of text in superscript to normal text in special superscript mode (after large previous nodes):
         *         0= superscript descent coincides with the previous node's baselineHeight, 1: top of previous node and top of the superscript nodes coincide
         *
         *  \image html jkqtmathtext_specialsuperscriptnode_getSizeInternal.png
         */
        double special_super_shift_factor;
        /** \brief relative shift of text in subscript to normal text in special superscript mode (after large previous nodes):
         *         0=child's baseline at the descent of the previous node, 1: subscript-node starts at the descent of the previous node
         *
         *  \image html jkqtmathtext_specialsubscriptnode_getSizeInternal.png
         */
        double special_sub_shift_factor;


        /** \brief scaling factor for font size of nominator and denominator of a fraction
         *
         *  \image html jkqtmathtext/jkqtmathtext_frac_factor.png
         */
        double frac_factor;
        /** \brief scaling factor for font size of nominator and denominator of a nested fraction
         *
         *  \image html jkqtmathtext/jkqtmathtext_frac_factor.png
         */
        double frac_nested_factor;
        /** \brief shift of denominator/nummerator away from central line of a frac
         *
         *  \image html jkqtmathtext_fracnode_geo.png
         */
        double frac_shift_factor;
        /** \brief scaling factor for font of underbrace/overbrace text */
        double underbrace_factor;
        /** \brief scaling factor for font of underset/overset text */
        double underset_factor;
        /** \brief additional space between the main text to the curly brace and the brace to underbrace/overbrace, multiplied with height("x") */
        double underbrace_separation_xfactor;
        /** \brief height of the brace in underbrace/overbrace, multiplied with ascent */
        double underbrace_bracesize_xfactor;
        /** \brief fraction of the brace ascent that the brace is shifted downwards, when scaled */
        double brace_y_shift_factor;
        /** \brief size of the decorations (dot, tilde, ...), as fraction of the ascent
         *
         *  \image html jkqtmathtext/decoration_sizing.png
         */
        double decoration_height_factor;
        /** \brief separation between the text and the decorations (dot, tilde, ...), as fraction of the ascent
         *
         *  \image html jkqtmathtext/decoration_sizing.png
         */
        double decoration_separation_factor;
        /** \brief a decoration has a size, which is slightly smaller than the text- width. the width is reduced by \c decoration_width_reduction_Xfactor*width("X") and the position is centered around the child-box. Also an italic correction is applied:
         *
         *  \image html jkqtmathtext/decoration_sizing.png
         */
        double decoration_width_reduction_Xfactor;
        /** \brief scaling factor for the small font used to indicate the degree of the sqrt */
        double sqrt_smallfont_factor;
        /** \brief width of the sqrt-symbol, as factor to width("X") */
        double sqrt_width_Xfactor;
        /** \brief height-increase of the sqrt-symbol, as factor of the child's height */
        double sqrt_height_factor;

        /** \brief width of thin table lines, this factor is multiplied with the current \c font.linewidth()
         *
         *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
         */
        double matrix_linewidth_thin_factor;
        /** \brief width of heavy table lines, this factor is multiplied with the current \c font.linewidth()
         *
         *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
         */
        double matrix_linewidth_heavy_factor;
        /** \brief separation of double-lines in tables, this factor is multiplied with the \c font.linewidth()
         *
         *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
         */
        double matrix_line_separation_factor;
        /** \brief x-separation of  two columns, this factor is multiplied with the \c font.width("x")
         *
         *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
         */
        double matrix_xSeparation_factor;
        /** \brief y-separation of  two columns, this factor is multiplied with the \c font.width("x")
         *
         *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
         */
        double matrix_ySeparation_factor;
        /** \brief x-padding of  two columns, this factor is multiplied with the \c font.width("x")
         *
         *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
         */
        double matrix_xPadding_factor;
        /** \brief y-padding of  two columns, this factor is multiplied with the \c font.width("x")
         *
         *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
         */
        double matrix_yPadding_factor;

        /** \brief initial choice for "serif" font */
        static QString init_serifFont;
        /** \brief initial choice for "sans-erif" font */
        static QString init_sansFont;
        /** \brief initial choice for "fallback symbol" font */
        static QString init_symbolFont;
        /** \brief initial choice for "script" font */
        static QString init_scriptFont;
        /** \brief initial choice for "typewriter" font */
        static QString init_typewriterFont;
        /** \brief initial choice for "caligraphic" font */
        static QString init_caligraphicFont;
        /** \brief initial choice for "blackboard" font */
        static QString init_blackboardFont;
        /** \brief initial choice for "fraktur" font */
        static QString init_fracturFont;
        /** \brief used to check for the first construction of a JKQTMathText ... if \c true several static variables are initialized in the constructor */
        static bool s_firstStart;
        /** \brief mutex to protect the static members */
        static std::mutex s_mutex;
};


#endif // JKQTMATHTEXT_H









