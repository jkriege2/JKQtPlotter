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





#ifndef JKQTPICONS_H
#define JKQTPICONS_H

#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QSettings>
#include <QWidget>
#include <QSplitter>
#include <QIcon>
#include "jkqtcommon/jkqtpdrawingtools.h"

/** \brief make a QIcon representing a QColor
 * \ingroup jkqtptools_icons
 */
JKQTCOMMON_LIB_EXPORT QIcon jkqtp_makeQColorIcon(QColor style);

/** \brief make a QIcon representing a Qt::BrushStyle
 * \ingroup jkqtptools_icons
 */
JKQTCOMMON_LIB_EXPORT QIcon jkqtp_makeQBrushStyleIcon(Qt::BrushStyle style);


/** \brief make a QIcon representing a Qt::PenStyle
 * \ingroup jkqtptools_icons
 */
JKQTCOMMON_LIB_EXPORT QIcon jkqtp_makeQPenStyleIcon(Qt::PenStyle style);

/** \brief make a QIcon representing a JKQTPLineDecoratorStyle
 * \ingroup jkqtptools_icons
 */
JKQTCOMMON_LIB_EXPORT QIcon JKQTPLineDecoratorStyle2Icon(JKQTPLineDecoratorStyle style);

/** \brief make a QIcon representing a JKQTPGraphSymbols
 * \ingroup jkqtptools_icons
 */
JKQTCOMMON_LIB_EXPORT QIcon JKQTPGraphSymbols2Icon(JKQTPGraphSymbols style);

#endif // JKQTPICONS_H
