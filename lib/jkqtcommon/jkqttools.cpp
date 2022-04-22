/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

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
#if (QT_VERSION>QT_VERSION_CHECK(5, 3, 0))
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
#if (QT_VERSION>=QT_VERSION_CHECK(5, 3, 0))
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
    for (int i=0; i<s1.count(); i++) {
        if (i<sizes.size()) s1[i]=sizes[i];
    }*/
    splitter->restoreState(settings.value(prefix+"splitter_sizes").toByteArray());
}

QString jkVariantListToString(const QList<QVariant>& data, const QString& separator) {
    QString r="";
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    for (int i=0; i<data.size(); i++) {
        if (i>0) r=r+separator;
        QVariant v=data[i];
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
        if (v.typeId()==QMetaType::Bool) r=r+loc.toString(v.toBool());
        else if (v.typeId()==QMetaType::Char) r=r+loc.toString(v.toInt());
        else if (v.typeId()==QMetaType::QDate) r=r+loc.toString(v.toDate());
        else if (v.typeId()==QMetaType::QDateTime) r=r+loc.toString(v.toDateTime());
        else if (v.typeId()==QMetaType::Double) r=r+loc.toString(v.toDouble());
        else if (v.typeId()==QMetaType::Int) r=r+loc.toString(v.toInt());
        else if (v.typeId()==QMetaType::LongLong) r=r+loc.toString(v.toLongLong());
        else if (v.typeId()==QMetaType::QString) r=r+QString("\"%1\"").arg(v.toString().replace("\"", "_").replace("\t", " ").replace("\r", "").replace("\n", " ").replace(",", " ").replace(";", " "));
        else if (v.typeId()==QMetaType::QTime) r=r+loc.toString(v.toTime());
        else if (v.typeId()==QMetaType::UInt) r=r+loc.toString(v.toUInt());
        else if (v.typeId()==QMetaType::ULongLong) r=r+loc.toString(v.toULongLong());
        else r=r+v.toString();

#else
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
#endif
    }
    return r;
}

JKQTCOMMON_LIB_EXPORT QString jkqtp_filenameize(const QString& data) {
    QString r;
    QString data1=data.simplified();
    for (int i=0; i<data1.size(); i++) {
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
        const auto c=data1[i];
#else
        QCharRef c=data1[i];
#endif
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

QString jkqtp_KeyboardModifiers2String(Qt::KeyboardModifiers modifiers, bool useNONE)
{
    if (modifiers==Qt::NoModifier) {
        if (useNONE) return "NONE";
        else return "";
    }
    QString ret="";
    auto append=[](QString& ret, const QString & appending, const QString& separator="+") {
        if (appending.size()<=0) return;
        if (ret.size()>0) ret+=separator;
        ret+=appending;
    };
    if ((modifiers&Qt::ShiftModifier)==Qt::ShiftModifier) append(ret, "SHIFT", "+");
    if ((modifiers&Qt::ControlModifier)==Qt::ControlModifier) append(ret, "CTRL", "+");
    if ((modifiers&Qt::AltModifier)==Qt::AltModifier) append(ret, "ALT", "+");
    if ((modifiers&Qt::MetaModifier)==Qt::MetaModifier) append(ret, "META", "+");
    if ((modifiers&Qt::KeypadModifier)==Qt::KeypadModifier) append(ret, "KEYPAD", "+");
    if ((modifiers&Qt::GroupSwitchModifier)==Qt::GroupSwitchModifier) append(ret, "GROUP", "+");
    return ret;
}

Qt::KeyboardModifiers jkqtp_String2KeyboardModifiers(const QString &modifiers)
{
    auto mods=modifiers.toUpper().split("+");
    Qt::KeyboardModifiers ret=Qt::NoModifier;

    for (const auto& m: mods) {
        if (m.trimmed()=="SHIFT") ret |= Qt::ShiftModifier;
        else if (m.trimmed()=="CTRL") ret |= Qt::ControlModifier;
        else if (m.trimmed()=="ALT") ret |= Qt::AltModifier;
        else if (m.trimmed()=="META") ret |= Qt::MetaModifier;
        else if (m.trimmed()=="KEYPAD") ret |= Qt::KeypadModifier;
        else if (m.trimmed()=="GROUP") ret |= Qt::GroupSwitchModifier;
    }

    return ret;
}

QString jkqtp_MouseButton2String(Qt::MouseButton button, bool useNONE)
{
    if (button==Qt::NoButton) {
        if (useNONE) return "NONE";
        else return "";
    }
    if (button==Qt::LeftButton) return "LEFT";
    if (button==Qt::RightButton) return "RIGHT";
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (button==Qt::MiddleButton) return "MIDDLE";
#else
    if (button==Qt::MidButton) return "MIDDLE";
#endif
    if (button==Qt::BackButton) return "BACK";
    if (button==Qt::ForwardButton) return "FORWARD";
    if (button==Qt::TaskButton) return "TASK";
    if (button==Qt::ExtraButton4) return "EXTRA4";
    if (button==Qt::ExtraButton5) return "EXTRA5";
    if (button==Qt::ExtraButton6) return "EXTRA6";
    if (button==Qt::ExtraButton7) return "EXTRA7";
    if (button==Qt::ExtraButton8) return "EXTRA8";
    if (button==Qt::ExtraButton9) return "EXTRA9";
    if (button==Qt::ExtraButton10) return "EXTRA10";
    if (button==Qt::ExtraButton11) return "EXTRA11";
    if (button==Qt::ExtraButton12) return "EXTRA12";
    if (button==Qt::ExtraButton13) return "EXTRA13";
    if (button==Qt::ExtraButton14) return "EXTRA14";
    if (button==Qt::ExtraButton15) return "EXTRA15";
    if (button==Qt::ExtraButton16) return "EXTRA16";
    if (button==Qt::ExtraButton17) return "EXTRA17";
    if (button==Qt::ExtraButton18) return "EXTRA18";
    if (button==Qt::ExtraButton19) return "EXTRA19";
    if (button==Qt::ExtraButton20) return "EXTRA20";
    if (button==Qt::ExtraButton21) return "EXTRA21";
    if (button==Qt::ExtraButton22) return "EXTRA22";
    if (button==Qt::ExtraButton23) return "EXTRA23";
    if (button==Qt::ExtraButton24) return "EXTRA24";
    return "UNKNOWN";
}

Qt::MouseButton jkqtp_String2MouseButton(const QString &button)
{
    auto but=button.toUpper().trimmed();
    if (but=="LEFT") return Qt::LeftButton;
    if (but=="RIGHT") return Qt::RightButton;
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
    if (but=="MIDDLE") return Qt::MiddleButton;
#else
    if (but=="MIDDLE") return Qt::MidButton;
#endif
    if (but=="BACK") return Qt::BackButton;
    if (but=="FORWARD") return Qt::ForwardButton;
    if (but=="TASK") return Qt::TaskButton;
    if (but=="EXTRA4") return Qt::ExtraButton4;
    if (but=="EXTRA5") return Qt::ExtraButton5;
    if (but=="EXTRA6") return Qt::ExtraButton6;
    if (but=="EXTRA7") return Qt::ExtraButton7;
    if (but=="EXTRA8") return Qt::ExtraButton8;
    if (but=="EXTRA9") return Qt::ExtraButton9;
    if (but=="EXTRA10") return Qt::ExtraButton10;
    if (but=="EXTRA11") return Qt::ExtraButton11;
    if (but=="EXTRA12") return Qt::ExtraButton12;
    if (but=="EXTRA13") return Qt::ExtraButton13;
    if (but=="EXTRA14") return Qt::ExtraButton14;
    if (but=="EXTRA15") return Qt::ExtraButton15;
    if (but=="EXTRA16") return Qt::ExtraButton16;
    if (but=="EXTRA17") return Qt::ExtraButton17;
    if (but=="EXTRA18") return Qt::ExtraButton18;
    if (but=="EXTRA19") return Qt::ExtraButton19;
    if (but=="EXTRA20") return Qt::ExtraButton20;
    if (but=="EXTRA21") return Qt::ExtraButton21;
    if (but=="EXTRA22") return Qt::ExtraButton22;
    if (but=="EXTRA23") return Qt::ExtraButton23;
    if (but=="EXTRA24") return Qt::ExtraButton24;
    return Qt::NoButton;
}

quint16 jkqtp_checksum(const void *data, size_t len)
{
#if QT_VERSION>=QT_VERSION_CHECK(6,0,0)
    return qChecksum(QByteArrayView(static_cast<const uint8_t*>(data), len));
#else
    return qChecksum(data, len);
#endif
}
