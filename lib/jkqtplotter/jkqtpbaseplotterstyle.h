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

#ifndef JKQTBASEPLOTTERSTYLE_H
#define JKQTBASEPLOTTERSTYLE_H

#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpcoordinateaxesstyle.h"
#include "jkqtplotter/jkqtpkeystyle.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpgraphsbasestyle.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtplotter_configmacros.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtcommon/jkqtpbasicimagetools.h"
#include <QColor>
#include <QPainter>
#include <QString>
#include <QHash>
#include <QPair>
#include <QVector>
#include <QSettings>


/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of a JKQTBasePlotter
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTBasePlotterStyle {
        Q_GADGET
    public:
        JKQTBasePlotterStyle();



        /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         */
        void loadSettings(const QSettings &settings, const QString& group=QString("plots/"), const JKQTBasePlotterStyle &defaultStyle=JKQTBasePlotterStyle());

        /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
        void saveSettings(QSettings& settings, const QString& group=QString("plots/")) const;

        /** \brief if set \c true (default: \c false ) the JKQTBasePlotter draws colored rectangles to indicate the different regions in the plot (border, axes, ...)
         *
         * \image html debugShowRegionBoxes.png
         *
         */
        bool debugShowRegionBoxes;
        /** \brief width of the lines that are drawn, when debugShowRegionBoxes \c ==true [in pixels] */
        double debugRegionLineWidth;
        /** \brief if set \c true (default: \c false ) the JKQTBasePlotter draws colored rectangles to indicate the outline etc. of text inside the plot
         *
         * \image html debugShowTextBoxes.png
         *
         */
        bool debugShowTextBoxes;
        /** \brief width of the lines that are drawn, when debugShowTextBoxes \c ==true [in pixels] */
        double debugTextBoxLineWidth;


        /** \brief the decimal separator used when exporting data to text files */
        QString CSVdecimalSeparator;
        /** \brief this string is used to introduce comments in text output when exporting data */
        QString CSVcommentInitializer;
        /** \brief free space between widget top border and plot top border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to internalPlotBorderTop
         *
         * \image html plot_widget_orientation.png
         *
         * \see JKQTBasePlotter::setBorder(), \ref jkqtplotter_base_plotsize
         */
        int plotBorderTop;
        /** \brief free space between widget left border and plot left border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to internalPlotBorderLeft
         *
         * \image html plot_widget_orientation.png
         *
         * \see JKQTBasePlotter::setBorder(), \ref jkqtplotter_base_plotsize
          */
        int plotBorderLeft;
        /** \brief free space between widget bottom border and plot bottom border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to internalPlotBorderBottom
         *
         * \image html plot_widget_orientation.png
         *
         * \see JKQTBasePlotter::setBorder(), \ref jkqtplotter_base_plotsize
         */
        int plotBorderBottom;
        /** \brief free space between widget right border and plot right border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to internalPlotBorderRight
         *
         * \image html plot_widget_orientation.png
         *
         * \see JKQTBasePlotter::setBorder(), \ref jkqtplotter_base_plotsize
         */
        int plotBorderRight;
        /** \brief color of the background of the plot (widget area) when drawing (to the screen) */
        QBrush widgetBackgroundBrush;
        /** \brief color of the background of the plot (widget area) when exporting*/
        QBrush exportBackgroundBrush;
        /** \brief color of the plot's background (i.e. of the area within the coordinate axes rectangle) */
        QBrush plotBackgroundBrush;
        /** \brief if \c plotFrameVisible==true, JKQTBasePlotter will draw a rectangle/frame around the plot in this color */
        QColor plotFrameColor;
        /** \brief if \c plotFrameVisible==true, JKQTBasePlotter will draw a rectangle/frame around the plot in this width [pt] */
        double plotFrameWidth;
        /** \brief if \c plotFrameVisible==true, JKQTBasePlotter will draw a rectangle/frame around the plot, rounding the corners with this redius (<=0 -> no rounded rectangle) [pt] */
        double plotFrameRounding;
        /** \brief if \c true, JKQTBasePlotter will draw a rectangle/frame around the plot */
        bool plotFrameVisible;
        /** \brief the plot label font name */
        QString plotLabelFontName;
        /** \brief the plot label font size [pt] */
        double plotLabelFontSize;
        /** \brief specifies whether to use antialiasing for plotting the coordinate system */
        bool useAntiAliasingForSystem;
        /** \brief specifies whether to use antialiasing when drawing any text
        * \note You can set this property \c false to increase plotting speed of complex plots (with many graphs inside). You can reach a
        *       roughly three-fold speed improvement!
        */
        bool useAntiAliasingForText;
        /** \brief default text color in the plot */
        QColor defaultTextColor;
        /** \brief default font size in the plot [pt] */
        double defaultFontSize;
        /** \brief default font name in the plot */
        QString defaultFontName;


        /** \brief style of the plot key/legend */
        JKQTPKeyStyle keyStyle;
        /** \brief style of the x-axis of the main coordinate system */
        JKQTPCoordinateAxisStyle xAxisStyle;
        /** \brief style of the y-axis of the main coordinate system */
        JKQTPCoordinateAxisStyle yAxisStyle;
        /** \brief separation between two seondary axes in pt */
        double secondaryAxisSeparation;
        /** \brief style of the coordinate axes used to display colorbars in the right */
        JKQTPColorbarCoordinateAxisStyle rightColorbarAxisStyle;
        /** \brief style of the coordinate axes used to display colorbars at the top */
        JKQTPColorbarCoordinateAxisStyle topColorbarAxisStyle;
        /** \brief style of the graphs */
        JKQTGraphsBaseStyle graphsStyle;


};

/** \brief returns the system-wide default JKQTPlotterStyle
 *  \ingroup jkqtpplotter_styling_classes
 *
 * \see JKQTPGetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), JKQTPSetSystemDefaultBaseStyle(), \ref jkqtpplotter_styling
 */
JKQTPLOTTER_LIB_EXPORT JKQTBasePlotterStyle& JKQTPGetSystemDefaultBaseStyle();

/** \brief replaces the system-wide default JKQTPlotterStyle with the given \a newStyle
 *  \ingroup jkqtpplotter_styling_classes
 *
 * \see JKQTPGetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), JKQTPGetSystemDefaultBaseStyle(), \ref jkqtpplotter_styling
 */
JKQTPLOTTER_LIB_EXPORT void JKQTPSetSystemDefaultBaseStyle(JKQTBasePlotterStyle& newStyle);

#endif // JKQTBASEPLOTTERSTYLE_H
