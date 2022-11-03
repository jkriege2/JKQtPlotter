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

#ifndef JKQTPCOORDINATEAXESSTYLE_H
#define JKQTPCOORDINATEAXESSTYLE_H

#include <QSettings>
#include <QString>
#include <QColor>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtplotter_configmacros.h"
#if __cplusplus >= 202002L
# include <version>
# ifdef __cpp_lib_format
#  include <format>
# endif
#endif

class JKQTBasePlotterStyle; // forward


/** \brief Support Class for JKQTPCoordinateAxis, and summarizes all properties that define the visual styling of a grid (minor or major), associated with a JKQTPCoordinateAxis
 *  \ingroup jkqtpplotter_styling_classes
 *
 * \see JKQTPCoordinateAxis, \ref jkqtpplotter_styling
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGridStyle {
    Q_GADGET
public:
    JKQTPGridStyle(bool isMajor=true);


    /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         */
    void loadSettings(const QSettings &settings, const QString& group=QString("grid/"), const JKQTPGridStyle &defaultStyle=JKQTPGridStyle());

    /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
    void saveSettings(QSettings& settings, const QString& group=QString("grid/")) const;

    /** \brief indicates whether to draw the major grid lines */
    bool enabled;
    /** \brief color of the grid*/
    QColor lineColor;
    /** \brief width of the grid lines (in pixel) */
    double lineWidth;
    /** \brief line stye of the grid lines */
    Qt::PenStyle lineStyle;
};



/** \brief Support Class for JKQTPCoordinateAxis, which summarizes all properties that define the visual styling of a JKQTPCoordinateAxis
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \image html jkqtpcoordinateaxis1.png
 *
 *  \see JKQTPCoordinateAxis, \ref jkqtpplotter_styling
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPCoordinateAxisStyle {
        Q_GADGET
    public:
        JKQTPCoordinateAxisStyle();

        explicit JKQTPCoordinateAxisStyle(const JKQTBasePlotterStyle& baseStyle);


        /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         */
        void loadSettings(const QSettings &settings, const QString& group=QString("axis/"), const JKQTPCoordinateAxisStyle &defaultStyle=JKQTPCoordinateAxisStyle());

        /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
        void saveSettings(QSettings& settings, const QString& group=QString("axis/")) const;

        /** \brief digits used for tick labels */
        int labelDigits;
        /** \brief when \c true, the digits of the labels are calculated automatically */
        bool autoLabelDigits;
        /** \brief if \c true, the plotter displays minor axis labels as number between 1 and 10 in some cases */
        bool minorTickLabelsEnabled;
        /** \brief indicates how to draw the labels */
        JKQTPCALabelType tickLabelType;

        /** \brief mode of the major ticks */
        JKQTPLabelTickMode tickMode;

        /** \brief position of the axis label */
        JKQTPLabelPosition labelPosition;
        /** \brief fontsize of the axis labels */
        double labelFontSize;
        /** \brief color of the axis label */
        QColor labelColor;
        /** \brief fontsize of the axis tick labels */
        double tickLabelFontSize;
        /** \brief fontsize of the minor axis tick labels */
        double minorTickLabelFontSize;
        /** \brief indicates whether to draw a thick axis line at x=0 (zero axis) */
        bool showZeroAxis;
        /** \brief indicates whether the minor tick labels should be full numbers, or just a number between 0..10 */
        bool minorTickLabelFullNumber;


        /** \brief draw mode of the main (left/bottom) axis */
        JKQTPCADrawMode drawMode1;
        /** \brief draw mode of the secondary (right/top) axis */
        JKQTPCADrawMode drawMode2;
        /** \brief draw mode of the zero axis
         *
         *  \image html JKQTPCoordinateAxisStyleDrawMode0.png
         *
         *  \see \ref JKQTPlotterGeometricCoordinateAxis0
         */
        JKQTPCADrawMode drawMode0;
        /** \brief color of minor ticks */
        QColor minorTickColor;
        /** \brief color of minor tick labels */
        QColor minorTickLabelColor;
        /** \brief line width of minor ticks in pt */
        double minorTickWidth;
        /** \brief color of axis ticks */
        QColor tickColor;
        /** \brief color of axis tick labels */
        QColor tickLabelColor;
        /** \brief line width of ticks in pt */
        double tickWidth;
        /** \brief line width of axis in pt */
        double lineWidth;
        /** \brief factor used to calculate the size of line arrows */
        double arrowSizeFactor;
        /** \brief line width of 0-line in pt */
        double lineWidthZeroAxis;


        /** \brief format string for time tick labels, see  QDateTime::toString() documentation for details on format strings */
        QString tickTimeFormat;
        /** \brief format string for date tick labels, see  QDateTime::toString() documentation for details on format strings */
        QString tickDateFormat;
        /** \brief format string for datetime tick labels,  see QDateTime::toString() documentation for details on format strings */
        QString tickDateTimeFormat;
        /** \brief format string for printf tick labels, see https://en.wikipedia.org/wiki/Printf_format_string documentation for details on format strings The first data parameter is the tick value as \c double an the second is tickUnitName as string. The following image shows an example for \c "y=%+.2f": \image html axisstyle/JKQTPCALTprintf.png */
        QString tickPrintfFormat;
#if __cplusplus >= 202002L || DOXYGEN
#if defined(__cpp_lib_format) || DOXYGEN
        /** \brief format string for std::format tick labels, (see e.g. https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification ). The first data parameter is the tick value as \c double an the second is tickUnitName as string. The following image shows an example for \c "\\texttt{{ y={:*^+8.1f}}}": \image html axisstyle/JKQTPCALTformat.png
              \note This option is only available for C++20 and above, use the CMake option \c JKQtPlotter_ENABLED_CXX20=ON if your compiler supports this.*/
        QString tickFormatFormat;
# endif
#endif



        /** \brief minimum number of axis ticks */
        unsigned int minTicks;
        /** \brief number of minor grid lines per axis tick interval
         *
         *  \image html docu_logaxis_set_minorticks.png
         **/
        unsigned int minorTicks;
        /** \brief length of an axis tick outside the plot border  in pt */
        double tickOutsideLength;
        /** \brief length of a minor axis tick outside the plot border in pt */
        double minorTickOutsideLength;
        /** \brief length of an axis tick inside the plot border  in pt */
        double tickInsideLength;
        /** \brief length of a minor axis tick inside the plot border in pt */
        double minorTickInsideLength;
        /** \brief color of the axis (labels, ticks, axis itself ...) */
        QColor axisColor;
        /** \brief distance between tick end and label start in pt */
        double tickLabelDistance;
        /** \brief distance between tick label and axis label in pt */
        double labelDistance;
        /** \brief rotation angle of tick labels [-180..180], i.e. given in degrees, default is 0 (horizontal) */
        double tickLabelAngle;

        /** \brief styling of the major/primary grid */
        JKQTPGridStyle majorGridStyle;
        /** \brief styling of the minor/secondary grid */
        JKQTPGridStyle minorGridStyle;

        /** \brief color of 0-line */
        QColor colorZeroAxis;
        /** \brief pen style of 0-line */
        Qt::PenStyle styleZeroAxis;
        /** \brief if non-zero, the line of the coordinate axis is moved outside the plot by this amount [pt]. This does not apply to the zero-axis! */
        double axisLineOffset;


        /** \brief returns a QPen that can be used for drawing zero axis lines */
        QPen getZeroAxisPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const;
        /** \brief returns a QPen that can be used for drawing axis lines */
        QPen getAxisPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter* parent) const;
        /** \brief returns a QPen that can be used for drawing axis tick lines */
        QPen getTickPen(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent) const;
        /** \brief returns a QPen that can be used for drawing minor axis tick lines */
        QPen getMinorTickPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter* parent) const;
        /** \brief calculates the size of an arrow in pixels */
        double getArrowSize(JKQTPEnhancedPainter &painter, JKQTBasePlotter* parent) const;
};


/** \brief Support Class for JKQTPCoordinateAxis, which summarizes all properties that define the visual styling of a JKQTPCoordinateAxis
 *         used for colorbars outside the plot
 *  \ingroup jkqtpplotter_styling_classes
 *
 * The major difference between this dervied version and the base JKQTPCoordinateAxisStyle are differently initialized members.
 * \see JKQTPCoordinateAxis, \ref jkqtpplotter_styling
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPColorbarCoordinateAxisStyle: public JKQTPCoordinateAxisStyle {
        Q_GADGET
    public:
        /** \copydoc JKQTPCoordinateAxisStyle::JKQTPCoordinateAxisStyle() */
        JKQTPColorbarCoordinateAxisStyle();

        /** \copydoc JKQTPCoordinateAxisStyle::JKQTPCoordinateAxisStyle(const JKQTBasePlotterStyle&) */
        explicit JKQTPColorbarCoordinateAxisStyle(const JKQTBasePlotterStyle& baseStyle);
    private:
        /** \brief modifies the members to match the requirements of colorbar axes */
        void initMembersForColorbars();

};

#endif // JKQTPCOORDINATEAXESSTYLE_H
