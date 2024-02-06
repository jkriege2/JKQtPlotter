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



#ifndef JKQTPSTRINGTOOLS_H_INCLUDED
#define JKQTPSTRINGTOOLS_H_INCLUDED
#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QString>
#include <QLocale>
#include <string>
#include <QtGlobal>
#include <limits>

class QGradient; // forward

/** \brief converts a QT::PenStyle into a string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_QPenStyle2String(Qt::PenStyle style);

/** \brief converts a QString into a Qt::PenStyle
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT Qt::PenStyle jkqtp_String2QPenStyle(const QString& style);

/** \brief converts a Qt::BrushStyle into a string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_QBrushStyle2String(Qt::BrushStyle style);

/** \brief converts a QString into a Qt::BrushStyle
 * \ingroup jkqtptools_string
 * \see jkqtp_String2QBrushStyleExt() for a more complex parser
 */
JKQTCOMMON_LIB_EXPORT Qt::BrushStyle jkqtp_String2QBrushStyle(const QString& style);

/** \brief converts a QString into a Qt::BrushStyle. commpared to jkqtp_String2QBrushStyle(), this method can parse
 *         more complex pattern/brush descriptions, such as for colors, gradients or images, which are output in \a color, \a gradient and \a image
 * \ingroup jkqtptools_string
 *
 * \param style the string to be parsed
 * \param[out] gradient output parameter for a parsed gradient
 * \param[out] image output parameter for a parsed image
 * \param[out] rotationAngleDeg output parameter for a parsed rotation angle of the pattern in degrees, where the direction equals the direction of a clock hand, i.e. 0=12o'clock, 180=6o'clock, ...
 *
 * \see jkqtp_String2QBrushStyle()
 */
JKQTCOMMON_LIB_EXPORT Qt::BrushStyle jkqtp_String2QBrushStyleExt(const QString& style, QGradient* gradient=nullptr, QPixmap* image=nullptr);


/** \brief converts a Unicode codepoint into a UTF8-sequence
 * \ingroup jkqtptools_string
 *
 * \see https://stackoverflow.com/questions/19968705/unsigned-integer-as-utf-8-value
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_UnicodeToUTF8(uint32_t codepoint);

/** \copydoc jkqtp_UnicodeToUTF8() */
inline QString jkqtp_UnicodeToUTF8Q(uint32_t codepoint) {
    return QString::fromStdString(jkqtp_UnicodeToUTF8(codepoint));
}

/** \brief convert a double to a string, using the loacle "C"
 * \ingroup jkqtptools_string
 */
inline QString JKQTPCDoubleToQString(double value) {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    return loc.toString(value, 'g', 18);
}

/** \brief convert a double to a string
 * \ingroup jkqtptools_string
 */
inline QString JKQTPDoubleToQString(double value, int prec = 10, char f = 'g', QChar decimalSeparator='.') {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QString res=loc.toString(value, f, prec);
    if (loc.decimalPoint()!=decimalSeparator) {
        res=res.replace(loc.decimalPoint(), decimalSeparator);
    }
    return res;
}


/** \brief convert a double to a string
 * \ingroup jkqtptools_string
 */
inline QString& JKQTPExtendString(QString& s, const QString& separator, const QString& extension) {
    if (s.size()>0) s+=separator;
    s+=extension;
    return s;
}

/** \brief convert a string to lower-case characters
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_tolower(const std::string& s);

/** \brief convert a string to a boolean
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT bool jkqtp_strtobool(const std::string& data);
/** \brief convert a string to upper-case
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_toupper(const std::string& s);

/** \brief std::string wrapper around sprintf()
 * \ingroup jkqtptools_string
 */
template <class T1>
inline std::string jkqtp_format(const std::string& templ, T1 d1) {
    char buffer[4096];
    snprintf(buffer, 4096, templ.c_str(), d1);
    return std::string(buffer);
};

/** \brief std::string wrapper around sprintf()
 * \ingroup jkqtptools_string
 */
template <class T1, class T2>
inline std::string jkqtp_format(const std::string& templ, T1 d1, T2 d2) {
    char buffer[4096];
    snprintf(buffer, 4096, templ.c_str(), d1, d2);
    return std::string(buffer);
};

/** \brief std::string wrapper around sprintf()
 * \ingroup jkqtptools_string
 */
template <class T1, class T2, class T3>
inline std::string jkqtp_format(const std::string& templ, T1 d1, T2 d2, T3 d3) {
    char buffer[4096];
    snprintf(buffer, 4096, templ.c_str(), d1, d2, d3);
    return std::string(buffer);
};

/** \brief std::string wrapper around sprintf()
 * \ingroup jkqtptools_string
 */
template <class T1, class T2, class T3, class T4>
inline std::string jkqtp_format(const std::string& templ, T1 d1, T2 d2, T3 d3, T4 d4) {
    char buffer[4096];
    snprintf(buffer, 4096, templ.c_str(), d1, d2, d3, d4);
    return std::string(buffer);
};

/** \brief convert a number of bytes to a string, formatting e.g. 1024 as 1kB, ...
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_bytestostr(double bytes);

/** \brief convert an integer to a string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_inttostr(long data);

/** \brief convert an integer to a hex string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_inttohex(long data);

/** \brief convert an unsigned int to a string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_uinttostr(unsigned long data);

/** \brief convert a double to a string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_floattostr(double data, int past_comma=-1, bool remove_trail0=false, double belowIsZero=1e-16);

/** \brief convert a float to a string using \a format (\c f|e|E|g|G ) with given number of decimals after comma \a past_comma and optional removal of trailing zeros behind decimal separator \a remove_trail0. Uses given QLocal \a loc .
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_floattoqstr(const QLocale & loc, double data, char format='f', int past_comma=-1, bool remove_trail0=false);

/** \brief convert a float to a string using \a format (\c f|e|E|g|G ) with given number of decimals after comma \a past_comma and optional removal of trailing zeros behind decimal separator \a remove_trail0. Uses system local, disables not use Group-Separator
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_floattoqstr(double data, char format='f', int past_comma=-1, bool remove_trail0=false);

/** \brief convert a boolean to a string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_booltostr(bool data);
/** \brief converts a RGBA color into a string
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 *
 * \param r red value of the color to convert
 * \param g green value of the color to convert
 * \param b blue value of the color to convert
 * \param a alpha value of the color to convert
 * \param useSpecialTransparencySyntax is set (\c true ), the function uses a special syntax to denote color and transparency: \c color,trans
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_rgbtostring(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255, bool useSpecialTransparencySyntax=true);

/** \brief list the (machine-readable) names of all predefined colors
 * \ingroup jkqtptools_string
 *
 */
JKQTCOMMON_LIB_EXPORT const QStringList& jkqtp_listNamedColors();
/** \brief converts a QColor into a string using the jkqtp_rgbtostring() method.
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_QColor2String(QColor color, bool useSpecialTransparencySyntax=true);

/** \brief converts a QString into a QColor, does not support the ,alpha%-notation, use jkqtp_String2QColor() for a full conversion!
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 *
 * \param color the color name to convert
 * \param namesOnly if \c true , the function only compares against the list of CSS colors; otherwise it passes the string also on to QColor()-constructor, which interprets e.g. \c #AABBCC
 * \param nameFound[out] optional return value that signifies whether a name was found
 */
JKQTCOMMON_LIB_EXPORT QColor jkqtp_lookupQColorName(const QString& color, bool namesOnly=false, bool* nameFound=nullptr);

/** \brief converts a QString into a QColor, compatible with jkqtp_QColor2String(QColor color);
 * \ingroup jkqtptools_string
 *
 * This function converts a color name to a QColor. It extends the names by the following optional synatxes (basically the CSS-syntax with even more options):
 *   - This function allows to add the alpha-value as \c "<color_name>,<alpha>" as integer betwee 0 and 255
 *     or as \c "<color_name>,<transparency_percent>%" in the range of 0..100 % (i.e. (1-transparency_percent/100)*255).
 *   - Also \c "<color_name>,a<alpha_percent>%" in the range of 0..100 % (i.e. alpha_percent/100*255).
 *   - \c "gray<percent>" in the range of 0..100 generates a gray value with 0%=black and 100%=white. Also works for "blue"|"green"|"red"|...
 *   - You can use full CSS-color syntax with functions \c "rgb(R,G,B)" , \c "rgba(...)" , \c "hsl(...)" , \c "hsv(...)" , \c "gray(...)" , \c "green(...)" , \c "red(...)" , \c "blue(...)".
 *     The function also support %-values as parameters and whitespace, comma or slash as value separatos!
 *   - Finally the default Qt color definitions are supported, i.e. \c #RGB , \c #RRGGBB , \c #AARRGGBB , \c #RRRGGGBBB , \c #RRRRGGGGBBBB
 * .
 *
 *
 */
JKQTCOMMON_LIB_EXPORT QColor jkqtp_String2QColor(QString color);

/** \brief clean a string to be usable as a variable name, e.g. in an expression parser, or a C++-expression
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_to_valid_variable_name(const std::string& input);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_floattounitstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=std::numeric_limits<double>::min()*4);
/** \brief convert a double to a LaTeX-encoded string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_floattolatexunitstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=std::numeric_limits<double>::min()*4);
/** \brief convert a double to a string, encoding powers of ten as exponent in LaTeX notation (e.g. <code>-1.23\\cdot 10^{-5}</code>)
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_floattolatexstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4, bool ensurePlusMinus=false, const std::string& multOperator="\\times");
/** \brief convert a double to a string, encoding powers of ten as exponent with HTML tags
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_floattohtmlstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_floattounitqstr(double data, int past_comma=5, bool remove_trail0=false);
/** \brief convert a double to a string, encoding powers of ten as exponent in LaTeX notation (e.g. <code>-1.23\\cdot 10^{-5}</code>)
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_floattolatexqstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4, bool ensurePlusMinus=false);
/** \brief convert a double to a string, encoding powers of ten as exponent with HTML tags
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_floattohtmlqstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
/** \brief convert a double to a LaTeX-encoded string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitqstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_floattolatexunitqstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=std::numeric_limits<double>::min()*4);

/** \brief convert a character to a string
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT std::string jkqtp_chartostr(char data);

/** \brief replace all linebreaks by \c "\\n" , \c "\\r" ...
 * \ingroup jkqtptools_string
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_backslashEscape(const QString& txt);


/*! \brief convert a QList<QVariant> to a string
    \ingroup jkqtptools_string

*/
JKQTCOMMON_LIB_EXPORT QString jkVariantListToString(const QList<QVariant>& data, const QString& separator=QString(", "));


/*! \brief filename-ize a string, i.e. replace every non-number and non-character (and also not <code> _ -</code>) character to \c _
    \ingroup jkqtptools_string */
JKQTCOMMON_LIB_EXPORT QString jkqtp_filenameize(const QString& data);

/** \brief create a valid variable name from the string, i.e. a string with only characters and digits and \c '_'. ALso the first character has to be a charcter.
 *  \ingroup jkqtptools_string */
JKQTCOMMON_LIB_EXPORT QString jkqtp_toValidVariableName(const QString& input);

/** \brief convert a <a href="http://doc.qt.io/qt-5/qt.html#KeyboardModifier-enum">Qt::KeyboardModifiers</a> to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtptools_string
 *
 * \param modifiers the object to convert
 * \param useNONE if \c true the function will return \c "NONE" if \c modifiers==Qt::NoMofifiers. Otherwise the function will return an empty string ( jkqtp_String2KeyboardModifiers() can cope with both variants)
 *
 *  \see jkqtp_String2KeyboardModifiers()
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_KeyboardModifiers2String(Qt::KeyboardModifiers modifiers, bool useNONE=true);

/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by jkqtp_KeyboardModifiers2String() ) to <a href="http://doc.qt.io/qt-5/qt.html#KeyboardModifier-enum">Qt::KeyboardModifiers</a>
 *  \ingroup jkqtptools_string
 *
 *  \see jkqtp_KeyboardModifiers2String()
 */
JKQTCOMMON_LIB_EXPORT Qt::KeyboardModifiers jkqtp_String2KeyboardModifiers(const QString& modifiers);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qt.html#MouseButton-enum">Qt::MouseButton</a> to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtptools_string
 *
 * \param button the object to convert
 * \param useNONE if \c true the function will return \c "NONE" if \c button==Qt::NoButton. Otherwise the function will return an empty string ( jkqtp_String2MouseButton() can cope with both variants)
 *
 *  \see jkqtp_MouseButton2String()
 */
JKQTCOMMON_LIB_EXPORT QString jkqtp_MouseButton2String(Qt::MouseButton button, bool useNONE=true);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by jkqtp_MouseButton2String() ) to <a href="http://doc.qt.io/qt-5/qt.html#MouseButton-enum">Qt::MouseButton</a>
 *  \ingroup jkqtptools_string
 *
 *  \see jkqtp_MouseButton2String()
 */
JKQTCOMMON_LIB_EXPORT Qt::MouseButton jkqtp_String2MouseButton(const QString &button);

/** \brief returns \c true, if \a text contains a match to the given regular expression \a regex,
 *         starts from \a offset and optionally returns the match in \a caps \c =[fullmatch, cap1,cap2,...]
 *  \ingroup jkqtptools_string
 *
 *  \note this function uses an internal cache, so the \a regex does not have
 *        to be compiled every time this is called (with the same \a regex ).
 *
 *  \see jkqtp_rxExactlyMatches(), jkqtp_rxIndexIn(), jkqtp_rxContains(), jkqtp_rxPartiallyMatchesAt()
 */
JKQTCOMMON_LIB_EXPORT bool jkqtp_rxContains(const QString& text, const QString &regex, qsizetype offset=0, QStringList* caps=nullptr);

/** \brief returns the next match (i.e. its index) of the given regular expression \a regex within \a text,
 *         starts from \a offset and optionally returns the match in \a caps \c =[fullmatch, cap1,cap2,...]
 *  \ingroup jkqtptools_string
 *
 *  \note this function uses an internal cache, so the \a regex does not have
 *        to be compiled every time this is called (with the same \a regex ).
 *
 *  \see jkqtp_rxExactlyMatches(), jkqtp_rxIndexIn(), jkqtp_rxContains(), jkqtp_rxPartiallyMatchesAt()
 */
JKQTCOMMON_LIB_EXPORT qsizetype jkqtp_rxIndexIn(const QString& text, const QString &regex, qsizetype offset=0, QStringList* caps=nullptr);

/** \brief returns \c true, if \a text exactly matches the given regular expression \a regex,
 *         starts from \a offset and optionally returns the match in \a caps \c =[fullmatch, cap1,cap2,...]
 *  \ingroup jkqtptools_string
 *
 *  \note this function uses an internal cache, so the \a regex does not have
 *        to be compiled every time this is called (with the same \a regex ).
 *
 *  \see jkqtp_rxExactlyMatches(), jkqtp_rxIndexIn(), jkqtp_rxContains(), jkqtp_rxPartiallyMatchesAt()
 */
JKQTCOMMON_LIB_EXPORT bool jkqtp_rxExactlyMatches(const QString& text, const QString &regex, QStringList* caps=nullptr);


/** \brief returns \c true, if \a text partially matches the given regular expression \a regex,
 *         starting from \a offset (and the match starts at \a offset !!!)
 *         and optionally returns the match in \a caps \c =[fullmatch, cap1,cap2,...]
 *  \ingroup jkqtptools_string
 *
 *  \note this function uses an internal cache, so the \a regex does not have
 *        to be compiled every time this is called (with the same \a regex ).
 *
 *  \see jkqtp_rxExactlyMatches(), jkqtp_rxIndexIn(), jkqtp_rxContains(), jkqtp_rxPartiallyMatchesAt()
 */
JKQTCOMMON_LIB_EXPORT bool jkqtp_rxPartiallyMatchesAt(const QString& text, const QString &regex, qsizetype offset=0, QStringList* caps=nullptr);


#endif // JKQTPSTRINGTOOLS_H_INCLUDED
