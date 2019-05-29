/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

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
#include "jkqtcommon/jkqtp_imexport.h"
#include <QString>
#include <QLocale>
#include <QPainter>


/** \brief converts a QT::PenStyle into a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT QString jkqtp_QPenStyle2String(Qt::PenStyle style);

/** \brief converts a QString into a Qt::PenStyle
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT Qt::PenStyle jkqtp_String2QPenStyle(const QString& style);
/** \brief converts a QT::BrushStyle into a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT QString jkqtp_QBrushStyle2String(Qt::BrushStyle style);

/** \brief converts a QString into a Qt::BrushStyle
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT Qt::BrushStyle jkqtp_String2QBrushStyle(const QString& style);



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



/** \brief convert a string to lower-case characters
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_tolower(const std::string& s);

/** \brief convert a string to a boolean
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT bool jkqtp_strtobool(const std::string& data);
/** \brief convert a string to upper-case
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_toupper(const std::string& s);

/** \brief std::string wrapper around sprintf()
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_format(const std::string& templ, ...);

/** \brief convert a number of bytes to a string, formatting e.g. 1024 as 1kB, ...
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_bytestostr(double bytes);

/** \brief convert an integer to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_inttostr(long data);

/** \brief convert an integer to a hex string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_inttohex(long data);

/** \brief convert an unsigned int to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_uinttostr(unsigned long data);

/** \brief convert a double to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattostr(double data, int past_comma=-1, bool remove_trail0=false, double belowIsZero=1e-16);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000,"g") will result in "1kg"
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattounitstr(double dataa, const std::string& unitname);
/** \brief convert a boolean to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_booltostr(bool data);
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
JKQTP_LIB_EXPORT QString jkqtp_rgbtostring(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255, bool useSpecialTransparencySyntax=true);

/** \brief converts a QColor into a string using the jkqtp_rgbtostring() method.
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
JKQTP_LIB_EXPORT QString jkqtp_QColor2String(QColor color, bool useSpecialTransparencySyntax=true);

/** \brief converts a QString into a QColor, compatible with jkqtp_QColor2String(QColor color);
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
JKQTP_LIB_EXPORT QColor jkqtp_String2QColor(const QString& color);

/** \brief clean a string to be usable as a variable name, e.g. in an expression parser, or a C++-expression
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_to_valid_variable_name(const std::string& input);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattounitstr(double data, int past_comma=5, bool remove_trail0=false);
/** \brief convert a double to a string, encoding powers of ten as exponent in LaTeX notation (e.g. <code>-1.23\\cdot 10^{-5}</code>)
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattolatexstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
/** \brief convert a double to a string, encoding powers of ten as exponent with HTML tags
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattohtmlstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT QString jkqtp_floattounitqstr(double data, int past_comma=5, bool remove_trail0=false);
/** \brief convert a double to a string, encoding powers of ten as exponent in LaTeX notation (e.g. <code>-1.23\\cdot 10^{-5}</code>)
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT QString jkqtp_floattolatexqstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
/** \brief convert a double to a string, encoding powers of ten as exponent with HTML tags
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT QString jkqtp_floattohtmlqstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);

/** \brief convert a character to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_chartostr(char data);




#endif // JKQTPSTRINGTOOLS_H_INCLUDED
