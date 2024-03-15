/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

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

#include "jkqtcommon/jkqtcommon_imexport.h"
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
JKQTCOMMON_LIB_EXPORT void jksaveWidgetGeometry(QSettings& settings, QWidget* widget, const QString& prefix=QString(""));

/*! \brief load the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] widget the widget to change
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTCOMMON_LIB_EXPORT void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, const QString& prefix=QString(""));


/*! \brief load the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] widget the widget to change
    \param defaultPosition default position of the widget
    \param defaultSize default size of the widget
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTCOMMON_LIB_EXPORT void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, QPoint defaultPosition, QSize defaultSize, const QString& prefix=QString(""));

/*! \brief store the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method stores the widget size and position.

    \param settings the QSettings object to write to
    \param widget the widget to store
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTCOMMON_LIB_EXPORT void jksaveSplitter(QSettings& settings, QSplitter* splitter, const QString& prefix=QString(""));

/*! \brief load the geometry of a given widget to a QSettings
    \ingroup jkqtpjkqtptools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] plitter the plitter to change
    \param prefix this prefix is prepended to the QSettings keys
*/
JKQTCOMMON_LIB_EXPORT void jkloadSplitter(QSettings& settings, QSplitter* splitter, const QString& prefix=QString(""));


/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by jkqtp_MouseButton2String() ) to <a href="http://doc.qt.io/qt-5/qt.html#MouseButton-enum">Qt::MouseButton</a>
 *  \ingroup jkqtptools_general
 *
 *  \see jkqtp_MouseButton2String()
 */
JKQTCOMMON_LIB_EXPORT quint16 jkqtp_checksum(const void* data, size_t len);

/** \brief Checks whether a given iterator is an input iterator (std::input_iterator_tag)
 *  \ingroup jkqtptools_general
 *
 **/
template <typename Iterator>
using JKQTPIsInputIteratorTrait = typename std::enable_if<std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>::value, bool>::type;


#endif // JKQTTOOLS_H
