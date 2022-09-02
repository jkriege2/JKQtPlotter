/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

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


#include "jkqtpicons.h"

#include <QtCore>
#include <QPixmap>
#include <QIcon>
#include <QPainter>

QIcon jkqtp_makeQBrushStyleIcon(Qt::BrushStyle style) {
    QPixmap icon(22,22);
    icon.fill(Qt::lightGray);
    {
        QPainter p;
        p.begin(&icon);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);
        p.fillRect(0,0,icon.width(),icon.height(),QBrush(Qt::darkGray, style));
        p.end();

    }
    return QIcon(icon);
}

QIcon jkqtp_makeQColorIcon(QColor style) {
    QPixmap icon(22,22);
    icon.fill(Qt::white);
    {
        QPainter p;
        p.begin(&icon);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);
        p.fillRect(0,0,icon.width()/2,icon.height()/2,QBrush(QColor("black"), Qt::SolidPattern));
        p.fillRect(icon.width()/2+1,icon.height()/2+1,icon.width(),icon.height(),QBrush(QColor("black"), Qt::SolidPattern));
        p.setPen(QPen(Qt::NoPen));
        p.setBrush(QBrush(style, Qt::SolidPattern));
        p.drawEllipse(QRectF(1,1,icon.width()-2,icon.height()-2));
        p.end();

    }
    return QIcon(icon);
}

QIcon jkqtp_makeQPenStyleIcon(Qt::PenStyle style) {
    QPixmap icon(22,22);
    icon.fill(Qt::white);
    {
        QPainter p;
        p.begin(&icon);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);
        p.setPen(QPen(QColor(Qt::black), 1, style));
        p.drawLine(0,icon.height()/2,icon.width(),icon.height()/2);
        p.end();

    }
    return QIcon(icon);
}

QIcon JKQTPLineDecoratorStyle2Icon(JKQTPLineDecoratorStyle style)
{
    QPixmap icon(22,22);
    icon.fill(Qt::white);
    {
        QPainter p;
        p.begin(&icon);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);
        p.setPen(QPen(QColor(Qt::black), 1, Qt::SolidLine));
        p.setBrush(QBrush(QColor("silver"), Qt::SolidPattern));
        p.drawLine(2,icon.height()/2,icon.width(),icon.height()/2);
        JKQTPPlotLineDecorator(p, 2, icon.height()/2, 0, style, 12);
        p.end();

    }
    return QIcon(icon);
}

QIcon JKQTPGraphSymbols2Icon(JKQTPGraphSymbols style)
{
    QPixmap icon(22,22);
    icon.fill(Qt::white);
    {
        QPainter p;
        p.begin(&icon);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);
        p.setPen(QPen(QColor(Qt::black), 1, Qt::SolidLine));
        JKQTPPlotSymbol(p, icon.width()/2, icon.height()/2, style, 15, 1, QColor("black"), QColor("silver"),QGuiApplication::font().family());
        p.end();

    }
    return QIcon(icon);
}
