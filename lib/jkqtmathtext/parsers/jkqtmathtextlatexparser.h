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





#ifndef JKQTMATHTEXTLATEXPARSER_H
#define JKQTMATHTEXTLATEXPARSER_H

#include "jkqtmathtext/parsers/jkqtmathtextparser.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include <QHash>
#include <QSet>
#include <QMap>
#include <QString>


class JKQTMathTextNode; // forward
class JKQTMathTextVerticalListNode; // forward

/*! \brief a LaTeX parser for JKQTMathText
    \ingroup jkqtmathtext_parser

    
    JKQTMathText is a self-contained mathematical markup renderer for Qt. It is used to renderer
    labels in JKQTPlotter/JKQTBasePlotter, but can be used independently. 
    
    That class is responsible for rendering a memory representation of mathematical markup,
    but relies on the current class JKQTMathTextLatexParser to parse a LaTeX string into
    the mentioned memory representation.

    \see See \ref jkqtmathtext_supportedlatex for a description of the supported LaTeX subset
         and \ref jkqtmathtext_renderingmodel for a description of the rendering model.
    
    In particular JKQTMathTextLatexParser actually parses e.g. a LaTeX string and draws it in pure C++. It does NOT rely
    on an installed LaTeX for the rendering!


 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextLatexParser : public JKQTMathTextParser {
        Q_OBJECT
    public:

        /** \brief class constructor */
        JKQTMathTextLatexParser(JKQTMathText * parent = nullptr);
        /** \brief class destructor */
        ~JKQTMathTextLatexParser();

        virtual JKQTMathTextNode* parse(const QString &text, JKQTMathText::ParseOptions options=JKQTMathText::DefaultParseOptions) override;


    protected:

        /** \brief the token types that may arrise in the string */
        enum tokenType {
            MTTnone, /*!< \brief no token */
            MTTtext, /*!< \brief a piece of general text */
            MTTinstruction, /*!< \brief an instruction, started by \c "\", e.g. \c "\\textbf", ... */
            MTTinstructionNewline,   /*!< \brief a newline instruction \c "\\" */
            MTTinstructionVerbatim,  /*!< \brief a verbatim instruction, e.g. \c \\verb!verbatimtext! was found: currentTokenName will contain the text enclode by the verbatim delimiters */
            MTTinstructionVerbatimVisibleSpace,  /*!< \brief a verbatim instruction that generates visible whitespaces, e.g. \c \\begin{verbatim}...\\end{verbatim} was found: currentTokenName will contain the text enclode by the verbatim delimiters */
            MTTinstructionBegin, /*!< \brief a \c '\\begin{...}' instruction, currentTokenName is the name of the environment */
            MTTinstructionEnd, /*!< \brief a \c '\\end{...}' instruction, currentTokenName is the name of the environment */
            MTTunderscore,  /*!< \brief the character \c "_" */
            MTThat,  /*!< \brief the character \c "^" */
            MTTdollar,  /*!< \brief the character \c "$" */
            MTTopenbrace, /*!< \brief the character \c "{" */
            MTTclosebrace, /*!< \brief the character \c "}" */
            MTTopenbracket, /*!< \brief the character \c "[" */
            MTTclosebracket, /*!< \brief the character \c "]" */
            MTTwhitespace, /*!< \brief some whitespace */
            MTTampersand,  /*!< \brief the character \c "&" */
            MTThyphen,  /*!< \brief the single hyphen character \c "-" in text-mode \note MTTendash and MTTemdash take precedence over MTThypen  */
            MTTendash,  /*!< \brief the en-dash character sequence \c "--" in text-mode */
            MTTemdash,  /*!< \brief the em-dash character sequence \c "---" in text-mode */
            MTTtilde,   /*!< \brief the tilde character \c "~"  */

        };
        /** \brief convert a tokenType into a string, e.g. for debugging output */
        static QString tokenType2String(tokenType type);

        /** \brief tokenizer for the LaTeX parser */
        tokenType getToken();
        /** \brief returns some characters to the Tokenizer */
        void giveBackToTokenizer(size_t count);
        /** \brief parse a LaTeX string
         *
         *  \param get if \c true this calls getToken()
         *  \param quitOnClosingBrace if unequal MTBTAny, this returns if the given closing brace is found
         *  \param quitOnEnvironmentEnd wuit if \c \\end{quitOnEnvironmentEnd} is found
         *  \param quitOnClosingBracket if \c true, quits on encountering a MTTclosebracket token
         */
        JKQTMathTextNode* parseLatexString(bool get, JKQTMathTextBraceType quitOnClosingBrace=JKQTMathTextBraceType::MTBTAny, const QString& quitOnEnvironmentEnd=QString(""), bool quitOnClosingBracket=false);
        /** \brief parse a LaTeX string with linebreaks
         *
         *  \param get if \c true this calls getToken()
         *  \param quitOnEnvironmentEnd wuit if \c \\end{quitOnEnvironmentEnd} is found
         *  \param _alignment horizontal alignment of the JKQTMathTextVerticalListNode \see JKQTMathTextVerticalListNode::alignment and JKQTMathTextHorizontalAlignment
         *  \param _linespacingFactor line spacing factor of the lines \see JKQTMathTextVerticalListNode::linespacingFactor
         *  \param spacingMode_ spacing mode/algorithm for the lines \see JKQTMathTextLineSpacingMode and JKQTMathTextLineSpacingMode
         *  \param _verticalOrientation vertical orientation of the block of all lines, see JKQTMathTextVerticalListNode::verticalOrientation and JKQTMathTextVerticalOrientation
         *
         *  \returns JKQTMathTextVerticalListNode with the lines as children
         */
        JKQTMathTextVerticalListNode *parseMultilineLatexString(bool get, const QString& quitOnEnvironmentEnd=QString(""), JKQTMathTextHorizontalAlignment _alignment=MTHALeft, double _linespacingFactor=1.0, JKQTMathTextLineSpacingMode spacingMode_=MTSMDefaultSpacing, JKQTMathTextVerticalOrientation _verticalOrientation=MTVOFirstLine);
        /** \brief parses a list of string-arguments, i.e. \c {p1}{p2}{...}
         *
         *  \param get call getToken() at the start, otherwise it is expected that currentToken==MTTopenbrace
         *  \param Nparams the number of parameters to expect
         *  \param[out] foundError will be set to \c true if an error occured (unexpected token) or \c false otherwise
         *  \return the list of parameter strings with Nparam entries or an empty or partial list on error
         */
        QStringList parseStringParams(bool get, size_t Nparams, bool *foundError=nullptr);
        /** \brief parses a string, i.e. a sequence of text and whitespaces. returns after any other token was found */
        QString parseSingleString(bool get);
        /** \brief read all text without tokenizing, until the sequence \a endsequence is found.
         *
         *  \param get if \c true the functions begins by reading a new character, otherwise the current character is used as first character
         *  \param endsequence the sequence, ending the read
         *  \param removeFirstlineWhitespace if \c true the returned string does not contain the first whitespace-line and a possible trailing whitespace line
         *  \return the read string, excluding the  \a endsequence
         */
        QString readUntil(bool get, const QString& endsequence, bool removeFirstlineWhitespace=false);
        /** \brief parses a single instruction (including it's parameters)
         *
         *  \param[out] _foundError will be set to \c true if an error occured (unexpected token) or \c false otherwise
         *  \param[out] getNew returns \c true if the parser has to call getToken() to go on
         *  \return the instruction node or \c nullptr on error (then also \a _foundError is set \c true )
         *  \note This method expects the current token currentToken to be MTTinstruction
         */
        JKQTMathTextNode* parseInstruction(bool *_foundError=nullptr, bool* getNew=nullptr);
        /** \brief parse a LaTeX math environment */
        JKQTMathTextNode* parseMath(bool get);

        /** \brief used by the tokenizer. type of the current token */
        tokenType currentToken;
        /** \brief the JKQTMathTextBraceType associated with the last \c \\right command the parser encountered */
        JKQTMathTextBraceType lastRightBraceType;
        /**  \brief returns the number of \c \\hline , \c \\hdashline , ... commands in the last parseLatexString() call */
        QMap<QString,size_t> lastMatrixLineCommandCount;
        /** \brief used by the tokenizer. Name of the current token, id applicable */
        QString currentTokenName;
        /** \brief used by the tokenizer. Points to the currently read character in parseString */
        int currentTokenID;
        /** \brief used by the tokenizer. The string to be parsed */
        QString parseString;
        /** \brief used by the parser. indicates whether we are in a math environment */
        bool parsingMathEnvironment;
        /** \brief used by the parser. indicates whether to use textstyle or displaystyle in math-mode */
        bool parsinginMathTextStyle;

        /** \brief characters that initiate a new token */
        static QSet<QChar> TokenCharacters;
        /** \brief characters that require special treatment in math  mode */
        static QSet<QChar> mathEnvironmentSpecialChars;
        /** \brief characters that require special treatment in math  mode */
        static QSet<QChar> mathEnvironmentSpecialEndChars;
        /** \brief single character instructions */
        static QSet<QChar> SingleCharInstructions;
        /** \brief maps instructions for accents in text-mode (e.g. \c \\'a or \c \\ae ) to the corresponding unicode  character */
        static QHash<QString, QChar> accentLetters;
        /** \brief lists all lengths of keys in accentLetters that start with a backslash */
        static QSet<int> accentLetters_LenBackslash;
        /** \brief lists all lengths of keys in accentLetters that start with a curly brace */
        static QSet<int> accentLetters_LenCurly;
        /** \brief characters that have to be replaced by the correspcoting JKQTMathTextSymbolNode in math mode */
        static QSet<QString> mathEnvironmentSpecialText;
        /** \brief maps instructions for braces with fixed size, e.g  \c \\bigl to the magnification factor of the brace versus the base font */
        static QHash<QString,double> big_instructions_family;


        /** \brief fills all static data structures, if they are still empty */
        static void initStaticStructures();
};


#endif // JKQTMATHTEXTLATEXPARSER_H









