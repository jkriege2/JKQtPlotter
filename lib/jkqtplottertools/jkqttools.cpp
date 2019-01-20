/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

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



#include "jkqttools.h"

#include <QList>
#include <QApplication>
#include <QDesktopWidget>
#include <QLocale>
#include <QtCore>

void jksaveWidgetGeometry(QSettings& settings, QWidget* widget, QString prefix) {
    settings.setValue(prefix+"pos", widget->pos());
    settings.setValue(prefix+"size", widget->size());
}

void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, QPoint defaultPosition, QSize defaultSize, QString prefix) {
    QPoint pos = settings.value(prefix+"pos", defaultPosition).toPoint();
    QSize size = settings.value(prefix+"size", defaultSize).toSize();

    widget->resize(size.boundedTo(QApplication::desktop()->screenGeometry(widget).size()));
    if (pos.x()<0 || pos.x()>QApplication::desktop()->screenGeometry(widget).width()) pos.setX(0);
    if (pos.y()<0 || pos.y()>QApplication::desktop()->screenGeometry(widget).height()) pos.setY(0);
    widget->move(pos);
}

void jkloadWidgetGeometry(QSettings& settings, QWidget* widget, QString prefix) {
    jkloadWidgetGeometry(settings, widget, QPoint(10, 10), QSize(100, 100), prefix);
}

void jksaveSplitter(QSettings& settings, QSplitter* splitter, QString prefix) {
    /*QList<int> sizes=splitter->sizes();
    QString data="";
    for (int i=0; i<sizes.size(); i++) {
        if (!data.isEmpty()) data=data+",";
        data=data+QLocale::system().toString(sizes[i]);
    }
    settings.setValue(prefix+"splitter_sizes", data);*/
    settings.setValue(prefix+"splitter_sizes", splitter->saveState());
}

void jkloadSplitter(QSettings& settings, QSplitter* splitter, QString prefix) {
    /*QString data=settings.value(prefix+"splitter_sizes", "").toString();
    QList<int> sizes, s1;
    QStringList sl=data.split(",");
    for (int i=0; i<sl.size(); i++) {
        sizes.append(sl[i].toInt());
    }
    s1=splitter->sizes();
    for (int i=0; i<s1.count(); i++) {
        if (i<sizes.size()) s1[i]=sizes[i];
    }*/
    splitter->restoreState(settings.value(prefix+"splitter_sizes").toByteArray());
}

QString jkVariantListToString(const QList<QVariant>& data, QString separator) {
    QString r="";
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    for (int i=0; i<data.size(); i++) {
        if (i>0) r=r+separator;
        QVariant v=data[i];
        switch (v.type()) {
            case QVariant::Bool: r=r+loc.toString(v.toBool()); break;
            case QVariant::Char: r=r+loc.toString(v.toInt()); break;
            case QVariant::Date: r=r+loc.toString(v.toDate()); break;
            case QVariant::DateTime: r=r+loc.toString(v.toDateTime()); break;
            case QVariant::Double: r=r+loc.toString(v.toDouble()); break;
            case QVariant::Int: r=r+loc.toString(v.toInt()); break;
            case QVariant::LongLong: r=r+loc.toString(v.toLongLong()); break;
            case QVariant::String: r=r+QString("\"%1\"").arg(v.toString().replace("\"", "_").replace("\t", " ").replace("\r", "").replace("\n", " ").replace(",", " ").replace(";", " ")); break;
            case QVariant::Time: r=r+loc.toString(v.toTime()); break;
            case QVariant::UInt: r=r+loc.toString(v.toUInt()); break;
            case QVariant::ULongLong: r=r+loc.toString(v.toULongLong()); break;
            //case : r=r+loc.toString(v.); break;
            default: r=r+v.toString(); break;
        }
    }
    return r;
}

LIB_EXPORT QString jkqtp_filenameize(const QString& data) {
    QString r;
    QString data1=data.simplified();
    for (int i=0; i<data1.size(); i++) {
        QCharRef c=data1[i];
        if (c.isLetterOrNumber() || (c=='-') || (c=='_') || (c=='.')) {
            r+=c;
        } else {
            r+='_';
        }
    }
    return r;
}

QString jkqtp_toValidVariableName(const QString& input) {
    QString out="";
    for (int i=0; i<input.size(); i++) {
        if (input[i].isLetter()) out=out+input[i];
        if (input[i].isDigit()&&(out.size()>0)) out=out+input[i];
        if ((input[i]=='_')&&(out.size()>0)) out=out+input[i];
    }
    return out;
}
