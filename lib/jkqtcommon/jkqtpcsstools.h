
/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef JKQTPCSSTOOLS_H_INCLUDED
#define JKQTPCSSTOOLS_H_INCLUDED
#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QString>
#include <QColor>
#include <QGradient>
#include <QStack>
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtcommon/jkqtpexpected.h"


/*! \brief A simple parser for certain CSS subsets
    \ingroup jkqtptools_math_parser

 \section jkqtpcss_ebnf EBNF definition of the parsed expressions

<pre>
<lingradient>         -> <b>linear-gradient(</b> <angle>? <b>,</b> <gradientstops> <b>)</b>
<angle>               -> <numberwithunit>
                       | <b>to</b> (<b>left</b>|<b>right</b>|<b>bottom</b>|<b>top</b>) (<b>left</b>|<b>right</b>|<b>bottom</b>|<b>top</b>)?
<numberwithunit>      -> <number> [<b>A</b>-<b>Z</b><b>a</b>-<b>z</b><b>%</b>]*
<gradientstops>       -> <gradientstop> | <gradientstop> <b>,</b> <gradientstops>
<gradientstop>        -> <color> (<number><b>%</b>)?
<color>               -> NAME
                       | <b>#</b><hexnumber>
                       | (<b>rgb</b> | <b>rgba</a> | <b>hsl</a> | <b>hsv</a> | <b>gray</a> | <b>grey</a> | <b>red</a> | <b>green</a> | <b>blue</a>) <b>(</b> (<numberwithunit> [<b>,/</b>]? ){1-4} <b>)</b>

<number>              -> floating-point-number, i.e.  "[+-]?\d+\.?\d*"
<hexnumber>           -> RGB | RRGGBB | RGBA | RRGGBBAA
</pre>


 */
class JKQTCOMMON_LIB_EXPORT JKQTPCSSParser
{
public:

    /** \brief encodes a number with its unit, e.g. \c 100% or \c 45deg or ... */
    struct NumberWithUnit {
        inline NumberWithUnit(double val=0.0, const QString& unit_=QString()): unit(unit_), number(val) {};
        /** \bref unit as string */
        QString unit;
        /** \brief the number value ittself */
        double number;
        /** \brief partly evaluates the unit and transforms the number accordingly.
         *
         *  This will do these transformations:
         *    - unit == \c % --> returns number/100.0;
         *    - unit == angle unit : returns number in degrees:
         *      - unit == \c rad --> returns number/M_PI*180.0
         *      - unit == \c turn --> returns number*360.0
         *      - unit == \c grad --> returns number/400.0*360.0
         *    .
         *  .
         */
        inline double normNumber() const {
            const QString nu=normUnit();
            if (nu=="%") return number/100.0;
            if (nu=="rad") return number/JKQTPSTATISTICS_PI*180.0;
            if (nu=="turn") return number*360.0;
            if (nu=="grad") return number/400.0*360.0;
            return number;
        }
        /** \brief returns number as a normalized number for RGB, output is in the range 0..1
         *
         *  This interprets %-values and all other values as from the range 0..255 and noralizes both to 0..1
         */
        inline double normRGBNumber() const {
            const QString nu=normUnit();
            if (nu=="%") return jkqtp_bounded(0.0,number/100.0, 1.0);
            return jkqtp_bounded(0.0,number/255.0, 1.0);
        }
        /** \brief returns number as a normalized number for Hue, output is in the range 0..1
         *
         *  This interprets %-values and all other values as from the range 0..360 and noralizes both to 0..1
         */
        inline double normHueNumber() const {
            const QString nu=normUnit();
            if (nu=="%") return jkqtp_bounded(0.0,number/100.0, 1.0);
            if (nu=="rad") return fmod(number/(JKQTPSTATISTICS_PI*2.0),1.0);
            if (nu=="turn") return fmod(number,1.0);
            if (nu=="grad") return fmod(number/400.0,1.0);
            return fmod(number/360.0, 1.0);
        }
        /** \brief returns a normalized version of the unit, i.e. all lower-case, timmed and simplified */
        inline QString normUnit() const { return unit.simplified().trimmed().toLower(); }

        inline bool operator==(const NumberWithUnit& other) const {
            return number==other.number && unit==other.unit;
        }
    };



protected:
    /** @name Tokenizer */
    /**@{*/

    struct Token {
        /** \brief the possible Token that can be recognized by the tokenizer in JKQTPCSSParser::getToken()  */
        enum TokenType {
            END,                /*!< \brief end token */
            NAME,               /*!< \brief a name (consisting of characters) of a variable or function */
            NUMBER,             /*!< \brief a number, possibly with a unit(string) */
            HEXSTRING,          /*!< \brief a string in HEX notation (i.e. 0-9a-fA-F) */
            LBRACKET,           /*!< \brief left brackets '(' */
            RBRACKET,           /*!< \brief right brackets ')' */
            COMMA,              /*!< \brief a comma ',' */
            SLASH,              /*!< \brief a slash '/' */
        };

        static QString toString(TokenType type);

        Token();
        Token(TokenType type);
        Token(double num, const QString& unit_=QString());
        Token(const QString& str, TokenType type=NAME);
        TokenType type;
        /** \brief the string value of the current token (when applicable) during the parsing step */
        QString StringValue;

        /** \brief the string value of the current token (when applicable) during the parsing step */
        double NumberValue;

        /** \brief checks whether the current token is of the given type (does not compare any other field) */
        inline bool is(TokenType othertype) const {
            return (type==othertype);
        }
        /** \brief checks whether the current token is of type TokenType::NAME and the String value equals \a name (case-insensitive commparison, trimmer, simplified) */
        inline bool isNormString(const QString& name) const {
            return (type==TokenType::NAME) && (getNormString()==name.toLower().simplified().trimmed());
        }
        /** \brief checks whether the current token is of type TokenType::NAME and the String value equals any of the entries in \a names (case-insensitive commparison, trimmer, simplified) */
        inline bool isNormStringAnyOf(const QStringList& names) const {
            if (type==TokenType::NAME) {
                const QString ns=getNormString();
                for (const auto&name: names) {
                    if (ns==name.toLower().simplified().trimmed()) return true;
                }
            }
            return false;
        }
        /** \brief normlizes the StringValue (i.e. lower-case, trimmer, simmplified) */
        inline QString getNormString() const {
            return StringValue.toLower().simplified().trimmed();
        }
        /** \brief converts the TOken information to a string */
        QString toString() const;
    };



    /** \brief Tokenizer: look at the next token from the input, but  don't set it as CurrentToken and don't move the read pointer pos */
    Token peekNextToken(int* endPos=nullptr);
    /** \brief Tokenizer: extract the next token from the input */
    Token getToken();

    /** \brief the current token while parsing a string */
    Token CurrentToken;


    /** \brief this stream is used to read in the program. An object is created and assigned
         * (and destroyed) by the parse()-function */
    QString text;
    /** \brief current reading position in text */
    int pos;

    /** \brief indicates whether pos points to the end of text */
    inline bool textAtEnd() const {
        return pos<0 || pos>=text.size()-1;
    }

    inline bool getCh(QChar& ch) {
        if (pos<0 || pos>=text.size()) return false;
        ch=text[pos];
        pos++;
        return true;
    }

    inline bool peekCh(QChar& ch) {
        if (pos<0 || pos>=text.size()) return false;
        ch=text[pos];
        return true;
    }

    inline void putBackCh() {
        pos--;
    }


    /**@}*/

    /** @name Error Handling  */
    /**@{*/

    struct RawErrorTag_t {};
    static RawErrorTag_t RawErrorTag;
    struct GeneralError {
        inline GeneralError(const QString& err=QString("unspecified error"), int p=-1): error(QString("%1 at pos.%2").arg(err).arg(p)), pos(p) {}
        inline GeneralError(RawErrorTag_t, const QString& err, int p=-1): error(err), pos(p) { }
        inline GeneralError(const QString& context, const QString& err, int p=-1): error(QString("%1 in context %3 at pos.%2").arg(err).arg(p).arg(context)), pos(p) {}
        QString error;
        int pos;
    };
    /** \brief Exception for unexpected Token */
    struct UnexpectedTokenError: public GeneralError {
        inline UnexpectedTokenError(Token::TokenType expectedToken, Token::TokenType foundToken, int pos):
            GeneralError(RawErrorTag, QString("unexpected token at pos.%1 (found: %2, expected: %3)").arg(pos).arg(Token::toString(foundToken)).arg(Token::toString(expectedToken)), pos)
        {}
        inline UnexpectedTokenError(Token::TokenType expectedToken, const Token& foundToken, int pos):
            GeneralError(RawErrorTag, QString("unexpected token at pos.%1 (found: %2, expected: %3)").arg(pos).arg(foundToken.toString()).arg(Token::toString(expectedToken)), pos)
        {}
        inline UnexpectedTokenError(const QString& context, Token::TokenType expectedToken, const Token& foundToken, int pos):
            GeneralError(RawErrorTag, QString("unexpected token at pos.%1 in context %4 (found: %2, expected: %3)").arg(pos).arg(foundToken.toString()).arg(Token::toString(expectedToken)).arg(context), pos)
        {}
    };
    /** \brief Exception for unexpected Term */
    struct UnexpectedTermError: public GeneralError {
        inline UnexpectedTermError(const QString& expectedToken, const Token& foundToken, int pos):
            GeneralError(RawErrorTag, QString("unexpected term at pos.%1 (found: %2, expected: %3)").arg(pos).arg(foundToken.toString()).arg(expectedToken), pos)
        {}
        inline UnexpectedTermError(const QString& context, const QString& expectedToken, const Token& foundToken, int pos):
            GeneralError(RawErrorTag, QString("unexpected term at pos.%1 in context %4 (found: %2, expected: %3)").arg(pos).arg(foundToken.toString()).arg(expectedToken).arg(context), pos)
        {}
    };
    /** \brief Exception for wrong number of function arguments */
    struct WrongNumberOfArgumentError: public GeneralError {
        inline WrongNumberOfArgumentError(const QString& func, int numArgs, int minArgs, int maxArgs, int pos):
            GeneralError(RawErrorTag, QString("wrong number of function arguments for %2() found at pos.%1 (found: %3, expected: %4...%5)").arg(pos).arg(func).arg(numArgs).arg(minArgs).arg(maxArgs), pos)
        {}
    };
    /** \brief Exception when a string cannot be converted properly */
    struct UnconvertobleError: public GeneralError {
        inline UnconvertobleError(const QString& str, const QString& target, int pos):
            GeneralError(RawErrorTag, QString("Could not convert '%2' to %3 at pos.%1").arg(pos).arg(str).arg(target), pos)
        {}
    };
    /**@}*/


    /** @name Parser */
    /**@{*/

    /** \brief  parses a number with unit */
    JKQTPExpected<JKQTPCSSParser::NumberWithUnit, JKQTPCSSParser::GeneralError> parseNumberWithUnit(bool get);
    /** \brief  parses a color definition <color> */
    JKQTPExpected<QColor, JKQTPCSSParser::GeneralError> parseColor(bool get);
    /** \brief  parses a color definition <gradient> */
    JKQTPExpected<QGradient, JKQTPCSSParser::GeneralError> parseGradient(bool get);

    /**@}*/




    /** \brief class constructor
         *
         *  \note This also registers all standatd functions and constants by calling     addStandardFunctions() and addStandardVariables()
         */
    JKQTPCSSParser(const QString& text);

public:






    /** \brief  parses the given expression*/
    static NumberWithUnit readNumberWithUnit(const QString& prog);
    /** \brief  parses the given expression, which should represent a color (incl. color-functions like \c rgb() ) */
    static QColor readColor(const QString& prog);
    /** \brief  parses the given expression, which should represent a QGradient */
    static QGradient readGradient(const QString& prog);

};


#endif // JKQTPCSSTOOLS_H_INCLUDED
