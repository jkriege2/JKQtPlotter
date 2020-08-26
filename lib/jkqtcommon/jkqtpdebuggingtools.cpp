/*
Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

    

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



#include "jkqtcommon/jkqtpdebuggingtools.h"
#include <QDebug>
#include <QApplication>




int JKQTPAutoOutputTimer::global_indent=0;

JKQTPAutoOutputTimer::JKQTPAutoOutputTimer(const QString& _message) :
    QElapsedTimer(),message(_message),indent()
{
    this->indent=QString(global_indent, QLatin1Char(' '));
    global_indent+=4;
#if QT_VERSION >= 0x040800
    qDebug()<<this->indent<<"TIMER_START:  "<<message;
#else
    qDebug()<<this->indent<<"TIMER_START:  "<<message;
#endif
    start();
}

JKQTPAutoOutputTimer::~JKQTPAutoOutputTimer()
{
    #if QT_VERSION >= 0x040800
        qDebug()<<this->indent<<"TIMER_END:  "<<message<<"    DUR: "<<double(nsecsElapsed())/1.0e6<<"ms";
    #else
        qDebug()<<this->indent<<"TIMER_END:  "<<message<<"    DUR: "<<double(elapsed())/1.0e3<<"ms";
    #endif
    global_indent-=4;

}

void JKQTPAutoOutputTimer::write(const QString& message) const {
#if QT_VERSION >= 0x040800
    qDebug()<<this->indent<<"TIMER_MESSAGE:  "<<this->message<<" "<<message<<"    DUR: "<<double(nsecsElapsed())/1.0e6<<"ms";
#else
    qDebug()<<this->indent<<"TIMER_MESSAGE:  "<<this->message<<" "<<message<<"    DUR: "<<double(elapsed())/1.0e3<<"ms";
#endif
}
