/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    last modification: $LastChangedDate$  (revision $Rev$)

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





#ifndef JKQTTOOLS_H
#define JKQTTOOLS_H

#include "jkqtcommon/jkqtp_imexport.h"
#include <QSettings>
#include <QWidget>
#include <QSplitter>

/*! \brief store the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method stores the widget size and position.

    \param settings the QSettings object to write to
    \param widget the widget to store
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTP_LIB_EXPORT void jksaveWidgetGeometry(QSettings& settings, QWidget* widget, const QString& prefix=QString(""));

/*! \brief load the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] widget the widget to change
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTP_LIB_EXPORT void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, const QString& prefix=QString(""));


/*! \brief load the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] widget the widget to change
    \param defaultPosition default position of the widget
    \param defaultSize default size of the widget
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTP_LIB_EXPORT void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, QPoint defaultPosition, QSize defaultSize, const QString& prefix=QString(""));

/*! \brief store the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method stores the widget size and position.

    \param settings the QSettings object to write to
    \param widget the widget to store
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTP_LIB_EXPORT void jksaveSplitter(QSettings& settings, QSplitter* splitter, const QString& prefix=QString(""));

/*! \brief load the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] plitter the plitter to change
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTP_LIB_EXPORT void jkloadSplitter(QSettings& settings, QSplitter* splitter, const QString& prefix=QString(""));


/*! \brief convert a QList<QVariant> to a string
    \ingroup jkqtpjkqtptools_qt

*/
JKQTP_LIB_EXPORT QString jkVariantListToString(const QList<QVariant>& data, const QString& separator=QString(", "));


/*! \brief filename-ize a string, i.e. replace every non-number and non-character (and also not <code> _ -</code>) character to \c _
    \ingroup tools */
JKQTP_LIB_EXPORT QString jkqtp_filenameize(const QString& data);

/** \brief create a valid variable name from the string, i.e. a string with only characters and digits and \c '_'. ALso the first character has to be a charcter.
 *  \ingroup tools */
JKQTP_LIB_EXPORT QString jkqtp_toValidVariableName(const QString& input);

/** \brief convert a <a href="http://doc.qt.io/qt-5/qt.html#KeyboardModifier-enum">Qt::KeyboardModifiers</a> to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup tools
 *
 * \param modifiers the object to convert
 * \param useNONE if \c true the function will return \C "NONE" if \c modifiers==Qt::NoMofifiers. Otherwise the function will return an empty string ( jkqtp_String2KeyboardModifiers() can cope with both variants)
 *
 *  \see jkqtp_String2KeyboardModifiers()
 */
JKQTP_LIB_EXPORT QString jkqtp_KeyboardModifiers2String(Qt::KeyboardModifiers modifiers, bool useNONE=true);

/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by jkqtp_KeyboardModifiers2String() ) to <a href="http://doc.qt.io/qt-5/qt.html#KeyboardModifier-enum">Qt::KeyboardModifiers</a>
 *  \ingroup tools
 *
 *  \see jkqtp_KeyboardModifiers2String()
 */
JKQTP_LIB_EXPORT Qt::KeyboardModifiers jkqtp_String2KeyboardModifiers(const QString& modifiers);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qt.html#MouseButton-enum">Qt::MouseButton</a> to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup tools
 *
 * \param button the object to convert
 * \param useNONE if \c true the function will return \C "NONE" if \c button==Qt::NoButton. Otherwise the function will return an empty string ( jkqtp_String2MouseButton() can cope with both variants)
 *
 *  \see jkqtp_MouseButton2String()
 */
JKQTP_LIB_EXPORT QString jkqtp_MouseButton2String(Qt::MouseButton button, bool useNONE=true);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by jkqtp_MouseButton2String() ) to <a href="http://doc.qt.io/qt-5/qt.html#MouseButton-enum">Qt::MouseButton</a>
 *  \ingroup tools
 *
 *  \see jkqtp_MouseButton2String()
 */
JKQTP_LIB_EXPORT Qt::MouseButton jkqtp_String2MouseButton(const QString &button);


#endif // JKQTTOOLS_H
