
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

#ifndef JKQTPEXPECTED_H_INCLUDED
#define JKQTPEXPECTED_H_INCLUDED

#include <stdexcept>


/** \brief tag type for an unexpected/error result in JKQTPExpected
 *  \ingroup jkqtptools_general
 *
 *  \see JKQTPExpected, JKQTPUnexpected
 */
struct JKQTPExpectedUnexpectedType {};
/** \brief tag for an unexpected/error result in JKQTPExpected
 *  \ingroup jkqtptools_general
 *
 *  \see JKQTPExpected, JKQTPExpectedUnexpectedType
 */
#define JKQTPUnexpected JKQTPExpectedUnexpectedType()

/** \brief an "expected" datatype, which can either represent a function result of type \c T or an error of type \c E
 *  \ingroup jkqtptools_general
 *
 *  Usage example:
 *  \code
 *    JKQTPExpected<double,QString> toInt(QString s) {
 *      bool ok=false;
 *      int i=s.toInt(&ok);
 *      if (ok) return {i}
 *      else return { JKQTPUnexpected, QString("cannot convert to int") };
 *    }
 *
 *    auto r=toInt("1.0a");
 *    if (r.hasValue()) qDebug()<<"SUCCESS! i="<<r.value();
 *    else qDebug()<<"FAILURE: "<<r.error();
 *  \endcode
 *
 *
 *  \see JKQTPExpectedUnexpectedType, JKQTPUnexpected
 */
template<class T, class E>
struct JKQTPExpected {
    JKQTPExpected()=delete;
    JKQTPExpected(const T& value_): m_hasValue(true), m_value(value_), m_error() {};
    JKQTPExpected(const JKQTPExpected<T,E>& other)=default;
    JKQTPExpected(JKQTPExpectedUnexpectedType, const E& error_): m_hasValue(false), m_value(), m_error(error_) {};

    const T& value() const {
        if (m_hasValue) return m_value;
        throw std::runtime_error("value not available");
    }
    const E& error() const {
        if (!m_hasValue) return m_error;
        throw std::runtime_error("error not available");
    }
    bool has_value() const { return m_hasValue; }
    operator bool() const { return m_hasValue; }
private:
    const bool m_hasValue;
    const T m_value;
    const E m_error;
};


#endif // JKQTPEXPECTED_H_INCLUDED
