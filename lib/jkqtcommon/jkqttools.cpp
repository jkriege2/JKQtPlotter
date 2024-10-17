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



#include "jkqttools.h"

#include <QList>
#include <QApplication>
#include <QLocale>
#include <QtCore>
#if (QT_VERSION>QT_VERSION_CHECK(5, 14, 0))
#  include <QScreen>
#  include <QGuiApplication>
#else
#  include <QDesktopWidget>
#endif
#if QT_VERSION>=QT_VERSION_CHECK(6,0,0)
#  include <QByteArrayView>
#endif

void jksaveWidgetGeometry(QSettings& settings, QWidget* widget, const QString& prefix) {
    settings.setValue(prefix+"pos", widget->pos());
    settings.setValue(prefix+"size", widget->size());
}

void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, QPoint defaultPosition, QSize defaultSize, const QString& prefix) {
    QPoint pos = settings.value(prefix+"pos", defaultPosition).toPoint();
    const QSize size = settings.value(prefix+"size", defaultSize).toSize();
#if (QT_VERSION>=QT_VERSION_CHECK(5, 14, 0))
    const auto widgeo = widget->screen()->geometry();
#else
    const auto widgeo = QApplication::desktop()->screenGeometry(widget);
#endif
    widget->resize(size.boundedTo(widgeo.size()));
    if (pos.x()<0 || pos.x()>widgeo.width()) pos.setX(0);
    if (pos.y()<0 || pos.y()>widgeo.height()) pos.setY(0);
    widget->move(pos);
}

void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, const QString& prefix) {
    jkloadWidgetGeometry(settings, widget, QPoint(10, 10), QSize(100, 100), prefix);
}

void jksaveSplitter(QSettings& settings, QSplitter* splitter, const QString& prefix) {
    /*QList<int> sizes=splitter->sizes();
    QString data="";
    for (int i=0; i<sizes.size(); i++) {
        if (!data.isEmpty()) data=data+",";
        data=data+QLocale::system().toString(sizes[i]);
    }
    settings.setValue(prefix+"splitter_sizes", data);*/
    settings.setValue(prefix+"splitter_sizes", splitter->saveState());
}

void jkloadSplitter(QSettings& settings, QSplitter* splitter, const QString& prefix) {
    /*QString data=settings.value(prefix+"splitter_sizes", "").toString();
    QList<int> sizes, s1;
    QStringList sl=data.split(",");
    for (int i=0; i<sl.size(); i++) {
        sizes.append(sl[i].toInt());
    }
    s1=splitter->sizes();
    for (int i=0; i<s1.size(); i++) {
        if (i<sizes.size()) s1[i]=sizes[i];
    }*/
    splitter->restoreState(settings.value(prefix+"splitter_sizes").toByteArray());
}

quint16 jkqtp_checksum(const void *data, size_t len)
{
#if QT_VERSION>=QT_VERSION_CHECK(6,0,0)
    return qChecksum(QByteArrayView(static_cast<const uint8_t*>(data), len));
#else
    return qChecksum(static_cast<const char*>(data), len);
#endif
}
