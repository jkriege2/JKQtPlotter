/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)



    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "jkqtpkeystyle.h"
#include "jkqtpbaseplotterstyle.h"
#include <QApplication>

JKQTPKeyStyle::JKQTPKeyStyle():
    frameVisible(true),
    frameColor(QColor("black")),
    frameLineStyle(Qt::SolidLine),
    frameWidth(1),
    frameRounding(0),
    backgroundBrush(QColor("white")),
    visible(true),
    fontSize(QApplication::font().pointSizeF()),
    fontName("GUI"),
    textColor(QColor("black")),
    sampleLineLength(3),
    sampleHeight(1),
    xMargin(0.5),
    yMargin(0.5),
    xOffset(1),
    yOffset(1),
    xSeparation(0.85),
    ySeparation(0.35),
    columnSeparation(0.75),
    position(JKQTPKeyInsideTopRight),
    layout(JKQTPKeyLayoutOneColumn)
{



}

JKQTPKeyStyle::JKQTPKeyStyle(const JKQTBasePlotterStyle &baseStyle):
    JKQTPKeyStyle()
{
    fontSize=baseStyle.defaultFontSize;
    textColor=baseStyle.defaultTextColor;
    fontName=baseStyle.defaultFontName;
}

void JKQTPKeyStyle::loadSettings(const QSettings &settings, const QString &group, const JKQTPKeyStyle &defaultStyle)
{
    xOffset = settings.value(group+"xoffset", defaultStyle.xOffset).toDouble();
    yOffset = settings.value(group+"yoffset", defaultStyle.yOffset).toDouble();
    xMargin = settings.value(group+"xmargin", defaultStyle.xMargin).toDouble();
    yMargin = settings.value(group+"ymargin", defaultStyle.yMargin).toDouble();
    xSeparation = settings.value(group+"xseparation", defaultStyle.xSeparation).toDouble();
    ySeparation = settings.value(group+"yseparation", defaultStyle.ySeparation).toDouble();
    columnSeparation = settings.value(group+"column_separation", defaultStyle.columnSeparation).toDouble();
    frameColor = jkqtp_String2QColor(settings.value(group+"frame_color", jkqtp_QColor2String(defaultStyle.frameColor)).toString());
    frameLineStyle=jkqtp_String2QPenStyle(settings.value(group+"frame_linestyle", jkqtp_QPenStyle2String(frameLineStyle)).toString());
    textColor = jkqtp_String2QColor(settings.value(group+"text_color", jkqtp_QColor2String(defaultStyle.textColor)).toString());
    frameWidth = settings.value(group+"frame_width", defaultStyle.frameWidth).toDouble();
    frameRounding = settings.value(group+"frame_rounding", defaultStyle.frameRounding).toDouble();
    frameVisible = settings.value(group+"frame_visible", defaultStyle.frameVisible).toBool();
    backgroundBrush = QBrush(jkqtp_String2QColor(settings.value(group+"background_color", jkqtp_QColor2String(defaultStyle.backgroundBrush.color())).toString()));
    visible = settings.value(group+"visible", defaultStyle.visible).toBool();
    position = String2JKQTPKeyPosition(settings.value(group+"position", JKQTPKeyPosition2String(defaultStyle.position)).toString());
    layout =  String2JKQTPKeyLayout(settings.value(group+"layout", JKQTPKeyLayout2String(defaultStyle.layout)).toString());
    fontSize = settings.value(group+"fontsize", defaultStyle.fontSize).toDouble();
    fontName = settings.value(group+"fontname", defaultStyle.fontName).toString();
    sampleLineLength = settings.value(group+"sample_width", settings.value(group+"line_width", defaultStyle.sampleLineLength).toDouble()).toDouble();
    sampleHeight = settings.value(group+"sample_height", defaultStyle.sampleHeight).toDouble();
}

void JKQTPKeyStyle::saveSettings(QSettings &settings, const QString &group) const
{
    settings.setValue(group+"xoffset", xOffset);
    settings.setValue(group+"yoffset", yOffset);
    settings.setValue(group+"xmargin", xMargin);
    settings.setValue(group+"ymargin", yMargin);
    settings.setValue(group+"xseparation", xSeparation);
    settings.setValue(group+"yseparation", ySeparation);
    settings.setValue(group+"column_separation", columnSeparation);
    settings.setValue(group+"frame_visible", frameVisible);
    settings.setValue(group+"frame_color", jkqtp_QColor2String(frameColor));
    settings.setValue(group+"frame_linestyle", jkqtp_QPenStyle2String(frameLineStyle));
    settings.setValue(group+"frame_width", frameWidth);
    settings.setValue(group+"frame_rounding", frameRounding);
    settings.setValue(group+"background_color", jkqtp_QColor2String(backgroundBrush.color()));
    settings.setValue(group+"visible", visible);
    settings.setValue(group+"position", JKQTPKeyPosition2String(position));
    settings.setValue(group+"layout", JKQTPKeyLayout2String(layout));
    settings.setValue(group+"fontsize", fontSize);
    settings.setValue(group+"fontname", fontName);
    settings.setValue(group+"text_color", jkqtp_QColor2String(textColor));
    settings.setValue(group+"sample_width", sampleLineLength);
    settings.setValue(group+"sample_height", sampleHeight);
}
