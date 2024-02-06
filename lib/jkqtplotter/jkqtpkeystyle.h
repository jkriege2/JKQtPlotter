/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)



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

#ifndef JKQTPKEYSTYLE_H
#define JKQTPKEYSTYLE_H

#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtplotter_configmacros.h"
#include <QColor>
#include <QString>
#include <QSettings>
#include <QPen>
#include <QBrush>

class JKQTBasePlotterStyle; // forward

/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of the key in a JKQTBasePlotter
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  With JKQTPKeyStyle you can style the appearance of a key/legend:
 *
 *  \image html plot_key.png
 *
 *  In addition it is possible to set:
 *    - its visiblility with JKQTPKeyStyle::visible
 *    - its location within or besides the plot area, using JKQTPKeyStyle::position
 *    - its overall layout (i.e. how the entries are distributed over the available space), using JKQTPKeyStyle::layout
 *  .
 *
 *  \see JKQTPBaseKey, JKQTBasePlotter, JKQTBasePlotterStyle, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPKeyStyle  {
        Q_GADGET
    public:
        JKQTPKeyStyle();
        explicit JKQTPKeyStyle(const JKQTBasePlotterStyle& baseStyle);


        /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         */
        void loadSettings(const QSettings &settings, const QString& group=QString("plots/key/"), const JKQTPKeyStyle &defaultStyle=JKQTPKeyStyle());

        /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
        void saveSettings(QSettings& settings, const QString& group=QString("plots/key/")) const;


        /** \brief indicates whether to plot a frame around the key */
        bool frameVisible;
        /** \brief color of the key frame line */
        QColor frameColor;
        /** \brief linestyle for the frame */
        Qt::PenStyle frameLineStyle;
        /** \brief width of the key frame line [pt] */
        double frameWidth;
        /** \brief rounding radius of the key frame rectangle (<=0 -> no rounded rectangle) [pt] */
        double frameRounding;
        /** \brief color of the key background */
        QBrush backgroundBrush;
        /** \brief indicates whether to plot a key */
        bool visible;
        /** \brief font size for key labels [in points] */
        double fontSize;
        /** \brief font name */
        QString fontName;
        /** \brief color of the key entries' text */
        QColor textColor;
        /** \brief length of the line samples in the key [in units of width of 'X' set in fontName, fontSize] */
        double sampleLineLength;
        /** \brief height of a line sample [in units of height of fontName, fontSize] */
        double sampleHeight;
        /** \brief x-distance between key frame and key content [in units of width of 'X' set in fontName, fontSize] */
        double xMargin;
        /** \brief y-distance between key frame and key content [in units of width of 'X' set in fontName, fontSize] */
        double yMargin;
        /** \brief x-offset of the key from the border of the plot [in units of width of 'X' set in fontName, fontSize] */
        double xOffset;
        /** \brief y-offset of the key from the border of the plot [in units of width of 'X' set in fontName, fontSize] */
        double yOffset;
        /** \brief distance between key line example and key text [in units of width of 'X' set in fontName, fontSize] */
        double xSeparation;
        /** \brief distance between two key entries [in units of height of fontName, fontSize] */
        double ySeparation;
        /** \brief distance between two columns of key entries [in units of width of 'X' set in fontName, fontSize] */
        double columnSeparation;

        /** \brief key position inside or besides the plot area, see JKQTPKeyPositions for details and examples */
        JKQTPKeyPosition position;
        /** \brief the key layout, i.e. how the entries are distributed over the available space, see JKQTPKeyLayout for details and examples */
        JKQTPKeyLayout layout;
};

#endif // JKQTPKEYSTYLE_H
