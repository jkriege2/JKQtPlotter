/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    last modification: $LastChangedDate$  (revision $Rev$)

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




/**
 * \defgroup jkqttools Tool Functions and Definitions for Qt
 * \ingroup tools_qt
 */

/** \file jkqtptools.h
  * \ingroup tools
  */


#ifndef JKQTTOOLS_H
#define JKQTTOOLS_H

#include "jkqtplottertools/jkqtp_imexport.h"
#include <QSettings>
#include <QWidget>
#include <QSplitter>

/*! \brief store the geometry of a given widget to a QSettings
    \ingroup tools_qt

    This method stores the widget size and position.

    \param settings the QSettings object to write to
    \param widget the widget to store
    \param prefix this prefix is prepended to the QSettings keys
*/
LIB_EXPORT void jksaveWidgetGeometry(QSettings& settings, QWidget* widget, QString prefix=QString(""));

/*! \brief load the geometry of a given widget to a QSettings
    \ingroup tools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] widget the widget to change
    \param prefix this prefix is prepended to the QSettings keys
*/
LIB_EXPORT void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, QString prefix=QString(""));


/*! \brief load the geometry of a given widget to a QSettings
    \ingroup tools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] widget the widget to change
    \param defaultPosition default position of the widget
    \param defaultSize default size of the widget
    \param prefix this prefix is prepended to the QSettings keys
*/
LIB_EXPORT void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, QPoint defaultPosition, QSize defaultSize, QString prefix=QString(""));

/*! \brief store the geometry of a given widget to a QSettings
    \ingroup tools_qt

    This method stores the widget size and position.

    \param settings the QSettings object to write to
    \param widget the widget to store
    \param prefix this prefix is prepended to the QSettings keys
*/
LIB_EXPORT void jksaveSplitter(QSettings& settings, QSplitter* splitter, QString prefix=QString(""));

/*! \brief load the geometry of a given widget to a QSettings
    \ingroup tools_qt

    This method loads the widget size and position.

    \param settings the QSettings object to write to
    \param[out] plitter the plitter to change
    \param prefix this prefix is prepended to the QSettings keys
*/
LIB_EXPORT void jkloadSplitter(QSettings& settings, QSplitter* splitter, QString prefix=QString(""));


/*! \brief convert a QList<QVariant> to a string
    \ingroup tools_qt

*/
LIB_EXPORT QString jkVariantListToString(const QList<QVariant>& data, QString separator=QString(", "));


/*! \brief filename-ize a string, i.e. replace every non-number and non-character (and also not <code> _ -</code>) character to \c _
    \ingroup tools_qt */
LIB_EXPORT QString jkqtp_filenameize(const QString& data);

/** \brief create a valid variable name from the string, i.e. a string with only characters and digits and \c '_'. ALso the first character has to be a charcter.
    \ingroup tools_qt */
LIB_EXPORT QString jkqtp_toValidVariableName(const QString& input);
#endif // JKQTTOOLS_H
