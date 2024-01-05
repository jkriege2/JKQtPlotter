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




#ifndef JKQTMATHTEXTSYMBOLNODE_H
#define JKQTMATHTEXTSYMBOLNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>
#include <cstdint>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing one symbol (e.g. \c \\alpha , \c \\cdot ...) node in the syntax tree
 *  \ingroup jkqtmathtext_items
 *
 *  All supported symbols are stored in the static table symbols, which is filled by fillSymbolTable().
 *  The latter function is also the customization point for new symbols:
 *    - For each symbol, the instruction name is stored as key, e.g. for \c \\alpha the key \c "alpha" is stored.
 *    - for each instruction a SymbolFullProps describes the symbol and stores how to draw it.
 *    - Each symbol has global properties (GlobalSymbolFlags, e.g. how to treat it's with etz.) and a set of representations.
 *      Each representation is for one font-encoding (JKQTMathTextFontEncoding, i.e. Unicode, WinSymbol or Standard)
 *    - each encoding representation SymbolProps specifies a text to represent the symol in SymbolProps::symbol and
 *      can have additional flags, properties etc.
 *  .
 *
 *  Font Lookup for symbols works as follows in JKQTMathTextSymbolNode:
 *    - if a character is found in the current (or to be used) font, it is taken from there (the font is specified in
 *      SymbolFullProps::fontType)
 *    - if the character is not found, it is looked for in the fallback fonts MTEFallbackGreek and MTEFallbackSymbols
 *    - as a last resort, some symbols can be created otherwise, so if neither of the two options above
 *      contain the required symbol, the symbol might be synthesized otherwise, or a rectangle with the size of "X" is drawn instead
 *  .
 *
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextSymbolNode: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextSymbolNode(JKQTMathText* parent, const QString& name);
        virtual ~JKQTMathTextSymbolNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;

        /** \copydoc symbolName */ 
        QString getSymbolName() const;
        /** \brief return type for getSymbolSize(), extends JKQTMathTextNodeSize with information about x-correction for sub- and superscript */
        struct NodeSize : JKQTMathTextNodeSize {
            NodeSize();
            /** \brief x-correction as described for JKQTMathParser::intsubsuper_xcorrection_factor for placing sub-/superscript below/above the symbol, e.g. for integral signs, where the subscript is slightly shifted to the left (from center) and the superscript slightly to the right */
            double subSuperAboveBelowXCorrection;

            NodeSize& operator=(const JKQTMathTextNodeSize& other);
            NodeSize& operator=(const NodeSize& other);
            NodeSize(const JKQTMathTextNodeSize& other);
            NodeSize(const NodeSize& other);
        };

        /** \brief determine the size of the node, calls getSizeInternal() implementation of the actual type \see getSizeInternal()
         *
         * \param painter painter to use for determining the size
         * \param currentEv current environment object
         *
         * \return all important size properties, packed into a NodeSize struct
         */
        NodeSize getSymbolSize(QPainter& painter, JKQTMathTextEnvironment currentEv) const;
        /** \brief checks whether the given symbol name can be prepresented by this type of node */
        static bool hasSymbol(const QString& symbolName);
        /** \brief return a list of all defined symbols */
        static QStringList getSymbols();
        /** \brief checks whether the given symbol name codes for an instruction that has SymbolFullProps::SubSuperscriptBelowAboveSymbol , does not neccessarily return the same value as JKQTMathTextNode::isSubSuperscriptAboveBelowNode() */
        static bool isSubSuperscriptBelowAboveSymbol(const QString& symbolName);
        /** \brief checks whether the given symbol has global flags SymbolFullProps::ExtendWidthInMathmode or SymbolFullProps::SmallExtendWidthInMathmode defined */
        static bool isExtendedWidthSymbol(const QString& symbolName);
        /** \brief returns the length of the symbol string for the given symbol */
        static int getSymbolLength(const QString& symbolName);
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;


        /** \brief this string will be sent to the drawText method with properly set fonts */
        QString symbolName;

        /** \brief flags specifying additional symbol features */
        enum SymbolFlags: uint64_t {
            AsOutside= 0,                  /*!< \brief indicates that no properties are activated */
            ItalicOn=  1 << 0,             /*!< \brief make font italic in any case */
            ItalicOff= 1 << 1,             /*!< \brief make font non-italic (i.e. upright) in any case */
            BoldOn=    1 << 2,             /*!< \brief make font bold in any case */
            BoldOff=   1 << 3,             /*!< \brief make font non-bold (i,,e, normal weight) in any case */
            DrawLeftHBar= 1 << 4,          /*!< \brief indicates whether to draw a bar on the left half of the character, above the xHeight (like for \c \\hbar ), implements an italic-correction */
            DrawRightHBar= 1 << 5,         /*!< \brief indicates whether to draw a bar on the right half of the character, above the xHeight, implements an italic-correction */
            FlipSymbolUpDown=1 << 6,       /*!< \brief indicates to flip the given symbol upside-down */
            FlipSymbolLeftRight=1 << 7,    /*!< \brief indicates to flip the given symbol left-right */
            RotateSymbol90=1 << 8,         /*!< \brief indicates to rotate the symbol 90 degree */
            DrawSlash=1 << 9,              /*!< \brief indicates to overdraw a slash (e.g. to combine 0 and / to form \\varnothing ) */
            DrawBackSlash=1 << 10,         /*!< \brief indicates to overdraw a backslash */
            DrawVertLine=1 << 11,          /*!< \brief indicates to overdraw a centered vertical line (slightly tilted if italic) */
            HeightIsAscent= 1 << 12,       /*!< \brief if true, the height of the symbol equals the ascent of the font */
            Upright=ItalicOff,
            NormalWeight=BoldOff,
        };

        friend inline SymbolFlags operator~ (SymbolFlags a) { return (SymbolFlags)~static_cast<uint64_t>(a); }
        friend inline SymbolFlags operator| (SymbolFlags a, SymbolFlags b) { return static_cast<SymbolFlags>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b)); }
        friend inline SymbolFlags operator& (SymbolFlags a, SymbolFlags b) { return static_cast<SymbolFlags>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b)); }
        friend inline SymbolFlags operator^ (SymbolFlags a, SymbolFlags b) { return static_cast<SymbolFlags>(static_cast<uint64_t>(a) ^ static_cast<uint64_t>(b)); }
        friend inline SymbolFlags& operator|= (SymbolFlags& a, SymbolFlags b) { return reinterpret_cast<SymbolFlags&>(reinterpret_cast<uint64_t&>(a) |= static_cast<uint64_t>(b)); }
        friend inline SymbolFlags& operator&= (SymbolFlags& a, SymbolFlags b) { return reinterpret_cast<SymbolFlags&>(reinterpret_cast<uint64_t&>(a) &= static_cast<uint64_t>(b)); }
        friend inline SymbolFlags& operator^= (SymbolFlags& a, SymbolFlags b) { return reinterpret_cast<SymbolFlags&>(reinterpret_cast<uint64_t&>(a) ^= static_cast<uint64_t>(b)); }
        friend inline bool has(SymbolFlags a, SymbolFlags b) { return (a&b)==b; }

        /** \brief flags specifying additional symbol features */
        enum GlobalSymbolFlags: uint64_t {
            NoGLobalSymbolFlags= 0,        /*!< \brief indicates that no properties are activated */
            ExtendWidthInMathmode= 1 << 0, /*!< \brief this symbol has an extended width, when used within a moth-environment/in math-mode */
            SmallExtendWidthInMathmode= 1 << 1, /*!< \brief like ExtendWidthInMathmode but adds a smaller whitespace */
            MakeWhitespaceHalf= 1 << 2,    /*!< \brief symbol uses whitespaces in its text (SymbolProps::symbol). These should be typeset as half-spaces */
            IntLikeSymbolCorrection= 1 << 3, /*!< \brief symbols, like \c \\int,\\iint,... require a correction in x-direction for subsequent sub-/superscripts ... this flag marks such symbols */
            SubSuperscriptBelowAboveSymbol= 1 << 4, /*!< \brief symbols, like \c \\int,\\iint,... if appearing in math-mode cause typesetting following sub-/superscripts below/above the symbol, not besides it. */
            SubscriptCorrection= 1 << 5, /*!< \brief symbols, like \c \\nabla,... require a subscript correction in x-direction for subsequent subscripts ... this flag marks such symbols */
        };

        friend inline GlobalSymbolFlags operator~ (GlobalSymbolFlags a) { return (GlobalSymbolFlags)~static_cast<uint64_t>(a); }
        friend inline GlobalSymbolFlags operator| (GlobalSymbolFlags a, GlobalSymbolFlags b) { return static_cast<GlobalSymbolFlags>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b)); }
        friend inline GlobalSymbolFlags operator& (GlobalSymbolFlags a, GlobalSymbolFlags b) { return static_cast<GlobalSymbolFlags>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b)); }
        friend inline GlobalSymbolFlags operator^ (GlobalSymbolFlags a, GlobalSymbolFlags b) { return static_cast<GlobalSymbolFlags>(static_cast<uint64_t>(a) ^ static_cast<uint64_t>(b)); }
        friend inline GlobalSymbolFlags& operator|= (GlobalSymbolFlags& a, GlobalSymbolFlags b) { return reinterpret_cast<GlobalSymbolFlags&>(reinterpret_cast<uint64_t&>(a) |= static_cast<uint64_t>(b)); }
        friend inline GlobalSymbolFlags& operator&= (GlobalSymbolFlags& a, GlobalSymbolFlags b) { return reinterpret_cast<GlobalSymbolFlags&>(reinterpret_cast<uint64_t&>(a) &= static_cast<uint64_t>(b)); }
        friend inline GlobalSymbolFlags& operator^= (GlobalSymbolFlags& a, GlobalSymbolFlags b) { return reinterpret_cast<GlobalSymbolFlags&>(reinterpret_cast<uint64_t&>(a) ^= static_cast<uint64_t>(b)); }
        friend inline bool has(GlobalSymbolFlags a, GlobalSymbolFlags b) { return (a&b)==b; }

        /** \brief calculates the bounding rect of \a text using \a fm and taking the flags from \a globalFlags into account */
        static QRectF getBoundingRect(const QFontMetricsF& fm, const QString& text, GlobalSymbolFlags globalFlags);
        /** \brief calculates the tight bounding rect of \a text using \a fm and taking the flags from \a globalFlags into account */
        static QRectF getTightBoundingRect(const QFontMetricsF& fm, const QString& text, GlobalSymbolFlags globalFlags);
        /** \brief draw \a text at (0,0) using QPainter \a p and taking  the flags from \a globalFlags into account */
        static void drawText(QPainter &p, const QString &text, GlobalSymbolFlags globalFlags, SymbolFlags symflags);

        /** \brief properties of the symbol */
        struct SymbolProps {
            SymbolProps();
            /** \brief this constructor sets the given properties and sets html=symbol! replaces whitespaces in \a _ymbol with \c &thinsp; for html, iff MakeWhitespaceHalf is set in \a _flags*/
            SymbolProps(const QString& _symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolProps(SymbolProps&& other)=default;
            SymbolProps(const SymbolProps& other)=default;
            SymbolProps& operator=(SymbolProps&& other)=default;
            SymbolProps& operator=(const SymbolProps& other)=default;
            /** \brief the symbol or text used to render the symbol (i.e. not the name,
             *         e.g. if the instruction/symbol name \c \\int is used, this would be \c QChar(0xF2) ,
             *         for \c \\sin it would be \c QString("sin") ...)  */
            QString symbol;
            /** \brief if \a symbol is a single character, this returns it, otherwise (empty or multi-character) it returns QChar() */
            inline QChar getSymbolSingleChar() const {
                if (symbol.size()==1) return symbol[0];
                return QChar();
            }
            /** \brief flags switching special features */
            SymbolFlags flags;
            /** \brief magnification factor for the font size */
            double fontScalingFactor;
            /** \brief this corrects the y position of a symbol: draws at y <- y+ height*yfactor) */
            double yShiftFactor;
        };

        /** \brief properties of the symbol, extends SymbolProps with font... */
        struct SymbolFullProps  {
            /** \brief default constructor */
            SymbolFullProps();
            SymbolFullProps(SymbolFullProps&& other)=default;
            SymbolFullProps(const SymbolFullProps& other)=default;
            SymbolFullProps& operator=(SymbolFullProps&& other)=default;
            SymbolFullProps& operator=(const SymbolFullProps& other)=default;
            /** \brief typesets the symbol (described in \a props ) from the specified \a font, \a props is stored with encoding MTFEStandard, an optional HTML-string \a _html can be given */
            SymbolFullProps(const QString& font, const SymbolProps& props, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief typesets the \a symbol from the specified \a font, \a props is stored with encoding MTFEStandard, an optional HTML-string \a _html can be given */
            SymbolFullProps(const QString& font, const QString& symbol, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief \a props is stored with encoding MTFEStandard, an optional HTML-string \a _html can be given */
            SymbolFullProps(const SymbolProps& props, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief \a props is stored with encoding MTFEStandard, uses the specified \a _fontType for drawing, an optional HTML-string \a _html can be given */
            SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, const SymbolProps& props, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief takes a \a symbol from the specified \a _fontType assuming MTFEStandard encoding, an optional HTML-string \a _html can be given */
            SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, const QString& symbol, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief single-alternative symbol for encoding \a enc0 and using symbol description \a props0 for it. An optional HTML-string \a _html can be provided */
            SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps& props0, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief single-alternative symbol for encoding \a enc0 and using symbol description \a props0 for it, uses the specified \a _fontType for drawing. An optional HTML-string \a _html can be provided */
            SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps& props0, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief double-alternative symbol for encoding \a enc0 + \a enc1 and using symbol descriptions \a props0 + \a props1 for it. An optional HTML-string \a _html can be provided */
            SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps& props0, JKQTMathTextFontEncoding enc1, const SymbolProps& props1, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief double-alternative symbol for encoding \a enc0 + \a enc1 and using symbol descriptions \a props0 + \a props1 for it, uses the specified \a _fontType for drawing. An optional HTML-string \a _html can be provided */
            SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps& props0, JKQTMathTextFontEncoding enc1, const SymbolProps& props1, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief triple-alternative symbol for encoding \a enc0 + \a enc1 + \a enc2 and using symbol descriptions \a props0 + \a props1 + \a props2 for it. An optional HTML-string \a _html can be provided */
            SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps& props0, JKQTMathTextFontEncoding enc1, const SymbolProps& props1, JKQTMathTextFontEncoding enc2, const SymbolProps& props2, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief triple-alternative symbol for encoding \a enc0 + \a enc1 + \a enc2 and using symbol descriptions \a props0 + \a props1 + \a props2 for it, uses the specified \a _fontType for drawing. An optional HTML-string \a _html can be provided */
            SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps& props0, JKQTMathTextFontEncoding enc1, const SymbolProps& props1, JKQTMathTextFontEncoding enc2, const SymbolProps& props2, const QString& _html=QString(), SymbolFlags _htmlflags=AsOutside, double _htmlfontScalingFactor=1.0, double _htmlyShiftFactor=0.0);
            /** \brief font type to use for typesetting the symbol (default is MTcurrentFont, which just takes the font set in the surrounding environment) */
            JKQTMathTextEnvironmentFont fontType;
            /** \brief font to use for output when fontType==MTcustomFont */
            QString customFontFamily;
            /** \brief here the actual symbol properties for each JKQTMathTextFontEncoding are stored */
            QHash<JKQTMathTextFontEncoding, SymbolProps> props;
            /** \brief HTML entity/text to represent the symbol (empty html.symbol not available) */
            SymbolProps html;
            /** \brief specifies global symbol flags */
            GlobalSymbolFlags globalFlags;
            /** \brief accesses entries in props for a given \a key. This also looks for fallback options (e.g.  MTFEStandard when MTFEUnicode is not available). Returns \a defaultValue when nothing is found */
            SymbolProps getProps(JKQTMathTextFontEncoding key, const SymbolProps& defaultValue=SymbolProps(), JKQTMathTextFontEncoding* foundEncodingOut=nullptr) const;

            SymbolFullProps &add(JKQTMathTextFontEncoding enc, const SymbolProps& props);
            SymbolFullProps& addWinSymbol(const QString& _symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addMathOperatorWinSymbol(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addUprightWinSymbol(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addUnicode(const QString& _symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addUprightUnicode(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addMathOperatorUnicode(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addStd(const QString& _symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addMathOperatorStd(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addUprightStd(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addHtml(const QString& _symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addMathOperatorHtml(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addUprightHtml(const QString& _symbol, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
            SymbolFullProps& addGlobalFlags(GlobalSymbolFlags addglobalFlags);
            SymbolFullProps& setGlobalFlags(GlobalSymbolFlags addglobalFlags);

            /** \brief returns the QFont to use for drawing and the QString to draw with it */
            QPair<QFont, SymbolProps> getDrawingData(JKQTMathTextEnvironment currentEv, JKQTMathText *parent, QPainter& painter) const;
        };

        /** \brief creates a SymbolFullProps object for a symbol that can be typeset in any font, for SymbolFullProps::html the same text as a\a symbol is used */
        static SymbolFullProps SimpleTextSymbol(const QString& symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
        /** \brief creates a SymbolFullProps object for a symbol that can be typeset in any font, a special html-string is given */
        static SymbolFullProps SimpleTextSymbol(const QString& symbol, const QString& html);
        /** \brief creates a SymbolFullProps object for a symbol that can be typeset in any font, for SymbolFullProps::html the same text as a\a symbol is used */
        static SymbolFullProps SimpleUprightTextSymbol(const QString& symbol);
        /** \brief creates a SymbolFullProps object for a symbol that can be typeset in any font, a special html-string is given */
        static SymbolFullProps SimpleUprightTextSymbol(const QString& symbol, const QString& html);
        /** \brief constructs a SymbolProps for a math-operator like \c \\sin ..., i.e. ItalicOff, BoldOff, ExtendWidthInMathmode */
        static SymbolFullProps MathOperatorSymbol(const QString& op);
        /** \brief constructs a SymbolProps with explicit HTML for a math-operator like \c \\sin ..., i.e. ItalicOff, BoldOff, ExtendWidthInMathmode */
        static SymbolFullProps MathOperatorSymbol(const QString& op, const QString& ophtml);
        /** \brief constructs a SymbolProps for a math-operator like \c \\sin ..., i.e. ItalicOff, BoldOff, HeightIsAscent, ExtendWidthInMathmode */
        static SymbolFullProps MathOperatorText(const QString& op);
        /** \brief constructs a SymbolProps with explicit HTML for a math-operator like \c \\sin ..., i.e. ItalicOff, BoldOff, HeightIsAscent, ExtendWidthInMathmode */
        static SymbolFullProps MathOperatorText(const QString& op, const QString& ophtml);
        /** \brief constructs a SymbolProps for an italic greek letter with the symbol in unicode-encoding \a letterUnicode and in WinSymbol-encoding letterWinWsymbol */
        static SymbolFullProps MathGreekLetter_WinSymbol_Unicode_Html(const QString& letterWinSymbol, const QString& letterUnicode, const QString& html);
        /** \brief constructs a SymbolProps for an upright greek letter with the symbol in unicode-encoding \a letterUnicode and in WinSymbol-encoding letterWinWsymbol */
        static SymbolFullProps UprightGreekLetter_WinSymbol_Unicode_Html(const QString& letterWinSymbol, const QString& letterUnicode, const QString& html);
        /** \brief constructs a SymbolProps for a greek letter with the format from outside with the symbol in unicode-encoding \a letterUnicode and in WinSymbol-encoding letterWinWsymbol */
        static SymbolFullProps AsOutsiudeGreekLetter_WinSymbol_Unicode_Html(const QString& letterWinSymbol, const QString& letterUnicode, const QString& html);
        /** \brief insert GreekLetter_WinSymbol_Unicode_Html() as \a baseInstructionName and UprightGreekLetter_WinSymbol_Unicode_Html and "up"+\a letterWinSymbol into symbols */
        static void addGreekLetterVariants_WinSymbol_Unicode_Html(QHash<QString, JKQTMathTextSymbolNode::SymbolFullProps>& symbols,const QString& baseInstructionName, const QString& letterWinSymbol, const QString& letterUnicode, const QString& html);
        /** \brief constructs a SymbolProps for a symbol with encoding in Standard-fonts a */
        static SymbolFullProps StdSymbol(const QString& symbol, const QString& html);
        /** \brief constructs a SymbolProps for a symbol with encoding in UnicodeFull-fonts a */
        static SymbolFullProps UnicodeSymbol(const QString& symbol, const QString& html);
        /** \brief constructs a SymbolProps for a symbol with encoding in Standard-fonts a */
        static SymbolFullProps StdSymbol(const QString& symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
        /** \brief constructs a SymbolProps for a symbol with encoding in UnicodeFull-fonts a */
        static SymbolFullProps UnicodeSymbol(const QString& symbol, SymbolFlags _flags=AsOutside, double _fontScalingFactor=1.0, double _yShiftFactor=0.0);
        /** \brief constructs a SymbolProps for a symbol with encoding in Standard-fonts a */
        static SymbolFullProps UprightSymbolStd(const QString& symbol);
        /** \brief constructs a SymbolProps for a symbol with encoding in UnicodeFull-fonts a */
        static SymbolFullProps UprightSymbolUnicode(const QString& symbol);
        /** \brief constructs a SymbolProps for a symbol with encoding in Standard-fonts a */
        static SymbolFullProps UprightSymbolStd(const QString& symbol, const QString& html);
        /** \brief constructs a SymbolProps for a symbol with encoding in UnicodeFull-fonts a */
        static SymbolFullProps UprightSymbolUnicode(const QString& symbol, const QString& html);
        /** \brief constructs a SymbolProps for a math-operator symbol like \c \\pm ... in unicode-full-encoding, i.e. ItalicOff, BoldOff, ExtendWidthInMathmode */
        static SymbolFullProps MathOperatorSymbolUnicode(const QString& unicode);
        /** \brief constructs a SymbolProps for a narrow math-operator symbol like \c \\pm ... in unicode-full-encoding, i.e. ItalicOff, BoldOff, SmallExtendWidthInMathmode */
        static SymbolFullProps NarrowMathOperatorSymbolUnicode(const QString& unicode);
        /** \brief constructs a SymbolProps for a narrow math-operator symbol like \c \\pm ... in unicode-full-encoding, i.e. ItalicOff, BoldOff, SmallExtendWidthInMathmode */
        static SymbolFullProps NarrowMathOperatorSymbolStd(const QString& symbol);
        /** \brief constructs a SymbolProps for a narrow math-operator symbol like \c \\pm ... in unicode-full-encoding, i.e. ItalicOff, BoldOff, SmallExtendWidthInMathmode */
        static SymbolFullProps NarrowMathOperatorSymbolStd(const QString& symbol, const QString& symbolHTML);
        /** \brief constructs a SymbolProps for a narrow math-operator like \c \\sin ..., i.e. ItalicOff, BoldOff, HeightIsAscent, ExtendWidthInMathmode */
        static SymbolFullProps NarrowMathOperatorText(const QString& op);
        /** \brief constructs a SymbolProps with explicit HTML for a narrow math-operator like \c \\sin ..., i.e. ItalicOff, BoldOff, HeightIsAscent, ExtendWidthInMathmode */
        static SymbolFullProps NarrowMathOperatorText(const QString& op, const QString& ophtml);


        /** \brief symbols that can be generated in any standard-font */
        static const QHash<QString, SymbolFullProps>& symbols();

        /** \brief retrieve the properties to render the given symbol \a symName in the current environment \a currentEv */
        SymbolFullProps getSymbolProp(const QString& symName, const JKQTMathTextEnvironment& currentEv) const;
};

#endif // JKQTMATHTEXTSYMBOLNODE_H









