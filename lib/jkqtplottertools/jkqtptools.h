/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef JKQTPTOOLS_H_INCLUDED
#define JKQTPTOOLS_H_INCLUDED
#include "jkqtplottertools/jkqtp_imexport.h"
#include <QString>
#include <QElapsedTimer>
#include <QLocale>
#include <QPainter>
#include <cfloat>
#include <QPrinter>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <cctype>


#ifndef __WINDOWS__
# if defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#  define __WINDOWS__
# endif
#endif

#ifndef __LINUX__
# if defined(linux)
#  define __LINUX__
# endif
#endif

#undef JKTOOLS_TIMER_USE_TIME
#if defined(__WINDOWS__)
  #include<windows.h>
#elif defined(__LINUX__)
  #include <sys/time.h>
#else
  #define JKTOOLS_TIMER_USE_TIME
#endif





class JKQTPEnhancedPainter; // forward
class JKQTBasePlotter; // forward declaration



/** \brief C++11 finally construct
 * \ingroup jkqtptools
 *
 * \see JKQTPFinally()
 */
template <class F>
class JKQTPFinalAct
{
public:
    explicit JKQTPFinalAct(F f) noexcept
      : f_(std::move(f)), invoke_(true) {}

    JKQTPFinalAct(JKQTPFinalAct&& other) noexcept
     : f_(std::move(other.f_)),
       invoke_(other.invoke_)
    {
        other.invoke_ = false;
    }

    JKQTPFinalAct(const JKQTPFinalAct&) = delete;
    JKQTPFinalAct& operator=(const JKQTPFinalAct&) = delete;

    ~JKQTPFinalAct() noexcept
    {
        if (invoke_) f_();
    }

private:
    F f_;
    bool invoke_;
};

/** \brief C++11 finally construct
 * \ingroup jkqtptools
 * \see JKQTPFinalAct
 */
template <class F>
inline JKQTPFinalAct<F> JKQTPFinally(const F& f) noexcept
{
    return JKQTPFinalAct<F>(f);
}

/** \brief C++11 finally construct
 * \ingroup jkqtptools
 * \see JKQTPFinalAct
 */
template <class F>
inline JKQTPFinalAct<F> JKQTPFinally(F&& f) noexcept
{
    return JKQTPFinalAct<F>(std::forward<F>(f));
}

/** \brief check whether the dlotaing point number is OK (i.e. non-inf, non-NAN)
 * \ingroup jkqtptools
 */
template <typename T>
inline bool JKQTPIsOKFloat(T v) {
    return std::isfinite(v)&&(!std::isinf(v))&&(!std::isnan(v));
}

/** \brief Availble action this JKQtPlotter can perform when mouse events occur.
 *         This allows you to e.g. draw rectangles or lines over the plot and receive a signal, when the drawing finishes
 * \ingroup jkqtptools

 */
enum JKQTPMouseDragActions {
    jkqtpmdaPanPlotOnMove=0, /*!< \brief the user can drag the current plot window while keeping the left mouse-button pushed down (=panning), the new widow is applied/displayed whenever the mouse moves \image html drag_viewport.gif "Drag the Plot Viewport" */
    jkqtpmdaPanPlotOnRelease, /*!< \brief the user can drag the current plot window while keeping the left mouse-button pushed down (=panning), the new widow is applied/displayed when the left mouse button is released */
    jkqtpmdaZoomByRectangle, /*!< \brief draw a rectangle and when finish zoom to that rectangle */
    jkqtpmdaDrawRectangleForEvent, /*!< \brief draw a rectangle and when finished execute the signal JKQTPlotter::userRectangleFinished() \image html draw_rectangle.gif "Draw Rectangle User-Action" */
    jkqtpmdaDrawCircleForEvent, /*!< \brief draw a circle and when finished execute the signal JKQTPlotter::userCircleFinished() \image html draw_circle.gif "Draw Circle User-Action" */
    jkqtpmdaDrawEllipseForEvent, /*!< \brief draw an ellipse and when finished execute the signal JKQTPlotter::userEllipseFinished() \image html draw_ellipse.gif "Draw Ellipse User-Action"  */
    jkqtpmdaDrawLineForEvent, /*!< \brief draw a line and when finished execute the signal JKQTPlotter::userLineFinished() \image html draw_line.gif "Draw Lines User-Action" */
    jkqtpmdaScribbleForEvents, /*!< \brief let the user scribble on the plot (left mouse button is kept pressed) and call JKQTPlotter::userScribbleClick() for each new position  */
};

/** \brief convert a JKQTPMouseDragActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtptools
 *
 *  \see String2JKQTPMouseDragActions(), JKQTPMouseDragActions
 */
JKQTP_LIB_EXPORT QString JKQTPMouseDragActions2String(JKQTPMouseDragActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseDragActions2String() ) to JKQTPMouseDragActions
 *  \ingroup jkqtptools
 *
 *  \see JKQTPMouseDragActions2String(), JKQTPMouseDragActions
 */
JKQTP_LIB_EXPORT JKQTPMouseDragActions String2JKQTPMouseDragActions(const QString &button);

/** \brief actions that can be bound to a double-click of the mouse
 * \ingroup jkqtptools
 */
enum JKQTPMouseDoubleClickActions {
    jkqtpdcaClickZoomsIn=0, /*!< \brief a double-click zooms into the plot at the current mouse location */
    jkqtpdcaClickZoomsOut, /*!< \brief a double-click zooms out of the plot at the current mouse location */
    jkqtpdcaClickOpensContextMenu, /*!< \brief a double-click opens the context menu */
    jkqtpdcaClickOpensSpecialContextMenu, /*!< \brief a double-click opens the special context menu \see setSpecialContextMenu() */
    jkqtpdcaClickMovesViewport, /*!< \brief a double-click centers the x/y-range around the clicked position */
};

/** \brief convert a JKQTPMouseDoubleClickActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtptools
 *
 *  \see String2JKQTPMouseDoubleClickActions(), JKQTPMouseDoubleClickActions
 */
JKQTP_LIB_EXPORT QString JKQTPMouseDoubleClickActions2String(JKQTPMouseDoubleClickActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseDoubleClickActions2String() ) to JKQTPMouseDoubleClickActions
 *  \ingroup jkqtptools
 *
 *  \see JKQTPMouseDoubleClickActions2String(), JKQTPMouseDoubleClickActions
 */
JKQTP_LIB_EXPORT JKQTPMouseDoubleClickActions String2JKQTPMouseDoubleClickActions(const QString &act);

/** \brief actions that can be bound to a mouse wheel event
 * \ingroup jkqtptools
 */
enum JKQTPMouseWheelActions {
    jkqtpmwaZoomByWheel=0, /*!< \brief use the mouse-wheel for zooming */
    jkqtpmwaPanByWheel, /*!< \brief use the mouse-wheel for panning the plot */
};

/** \brief convert a JKQTPMouseWheelActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtptools
 *
 *  \see String2JKQTPMouseWheelActions(), JKQTPMouseWheelActions
 */
JKQTP_LIB_EXPORT QString JKQTPMouseWheelActions2String(JKQTPMouseWheelActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseWheelActions2String() ) to JKQTPMouseWheelActions
 *  \ingroup jkqtptools
 *
 *  \see JKQTPMouseWheelActions2String(), JKQTPMouseWheelActions
 */
JKQTP_LIB_EXPORT JKQTPMouseWheelActions String2JKQTPMouseWheelActions(const QString &act);

/** \brief modes for the context menu
 * \ingroup jkqtptools
 */
enum JKQTPContextMenuModes {
    jkqtpcmmStandardContextMenu=0,  /*!< \brief only show the standard context menu \image html zoomin_mouse_contextmenu.gif "Zooming with the mouse" */
    jkqtpcmmSpecialContextMenu,  /*!< \brief only show the special context menu \see setSpecialContextMenu() */
    jkqtpcmmStandardAndSpecialContextMenu,  /*!< \brief show the standard context menu, with the special context menu incorporated \see setSpecialContextMenu() */
    jkqtpcmmNoContextMenu, /*!< \brief don't show a context menu at all */
};

/** \brief convert a JKQTPContextMenuModes to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtptools
 *
 *  \see String2JKQTPContextMenuModes(), JKQTPContextMenuModes
 */
JKQTP_LIB_EXPORT QString JKQTPContextMenuModes2String(JKQTPContextMenuModes act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPContextMenuModes2String() ) to JKQTPContextMenuModes
 *  \ingroup jkqtptools
 *
 *  \see JKQTPContextMenuModes2String(), JKQTPContextMenuModes
 */
JKQTP_LIB_EXPORT JKQTPContextMenuModes String2JKQTPContextMenuModes(const QString &act);

typedef QHash<QPair<Qt::MouseButton,Qt::KeyboardModifiers>, JKQTPMouseDragActions> JKQTPMouseDragActionsHashMap;
typedef JKQTPMouseDragActionsHashMap::const_iterator JKQTPMouseDragActionsHashMapIterator;

typedef QHash<Qt::KeyboardModifiers, JKQTPMouseWheelActions> JKQTPMouseWheelActionsHashMap;
typedef JKQTPMouseWheelActionsHashMap::const_iterator JKQTPMouseWheelActionsHashMapIterator;

typedef QHash<QPair<Qt::MouseButton,Qt::KeyboardModifiers>, JKQTPMouseDoubleClickActions> JKQTPMouseDoubleClickActionsHashMap;
typedef JKQTPMouseDoubleClickActionsHashMap::const_iterator JKQTPMouseDoubleClickActionsHashMapIterator;

/** \brief converts a QT::PenStyle into a string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString jkqtp_QPenStyle2String(Qt::PenStyle style);

/** \brief converts a QString into a Qt::PenStyle
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT Qt::PenStyle jkqtp_String2QPenStyle(const QString& style);
/** \brief converts a QT::BrushStyle into a string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString jkqtp_QBrushStyle2String(Qt::BrushStyle style);

/** \brief converts a QString into a Qt::BrushStyle
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT Qt::BrushStyle jkqtp_String2QBrushStyle(const QString& style);



/** \brief Specifies how a fill-color is derived from a given color
 * \ingroup jkqtptools

 */
enum JKQTPColorDerivationMode {
    JKQTPFFCMSameColor, /*!< \brief fill with the same color */
    JKQTPFFCMInvertedColor, /*!< \brief fill with the inverted color */
    JKQTPFFCMLighterColor, /*!< \brief fill with the a lighter color */
    JKQTPFFCMEvenLighterColor, /*!< \brief fill with the an even lighter color  than JKQTPFFCMLighterColor */
    JKQTPFFCMDarkerColor, /*!< \brief fill with the a darker color */
    JKQTPFFCMEvenDarkerColor, /*!< \brief fill with the an even darker color than JKQTPFFCMDarkerColor */
    JKQTPFFCMMoreTransparentColor, /*!< \brief fill with the a partly transparent color */
    JKQTPFFCMEvenMoreTransparentColor, /*!< \brief fill with the a more transparent color than JKQTPFFCMMoreTransparentColor */
    JKQTPFFCMLessTransparentColor, /*!< \brief fill with the a partly transparent color */
    JKQTPFFCMEvenLessTransparentColor, /*!< \brief fill with the a more transparent color than JKQTPFFCMLessTransparentColor*/
};

/** \brief use a JKQTPColorDerivationMode to derive a color from \a col as specified
 *  \ingroup jkqtptools
 *
 *  \see JKQTPColorDerivationMode
 */
JKQTP_LIB_EXPORT QColor JKQTPGetDerivedColor(JKQTPColorDerivationMode mode, const QColor& col);

/** \brief convert a JKQTPColorDerivationMode to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtptools
 *
 *  \see String2JKQTPColorDerivationMode(), JKQTPColorDerivationMode
 */
JKQTP_LIB_EXPORT QString JKQTPColorDerivationMode2String(JKQTPColorDerivationMode mode);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPColorDerivationMode2String() ) to JKQTPColorDerivationMode
 *  \ingroup jkqtptools
 *
 *  \see JKQTPColorDerivationMode2String(), JKQTPColorDerivationMode
 */
JKQTP_LIB_EXPORT JKQTPColorDerivationMode String2JKQTPColorDerivationMode(const QString &mode);


/** \brief display mode for an axis
 * \ingroup jkqtptools */
enum JKQTPCADrawMode {
    JKQTPCADMcomplete=0, /*!< \brief draw axis with ticks, ticklabels and axis label */
    JKQTPCADMLineTicksTickLabels, /*!< \brief draw axis with ticks, line and tick labels */
    JKQTPCADMLineTicks, /*!< \brief draw axis with ticks and line */
    JKQTPCADMLine, /*!< \brief draw axis as thick line */
    JKQTPCADMTicksTickLabelsAxisLabel, /*!< \brief draw axis with ticks, tick labels and axisLabel */
    JKQTPCADMTicksTickLabels, /*!< \brief draw axis with ticks and tick labels */
    JKQTPCADMTickLabelsAxisLabel, /*!< \brief draw axis tick labels and axisLabel */
    JKQTPCADMTickLabels, /*!< \brief draw axis tick labels */
    JKQTPCADMTicks, /*!< \brief draw axis with ticks */
    JKQTPCADMnone /*!< \brief draw no axis */
};

/** \brief determines whether JKQTPCADrawMode has the line
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasLine(JKQTPCADrawMode pos);

/** \brief determines whether JKQTPCADrawMode has ticks
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasTicks(JKQTPCADrawMode pos);

/** \brief determines whether JKQTPCADrawMode has tick labels
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasTickLabels(JKQTPCADrawMode pos);

/** \brief determines whether JKQTPCADrawMode has the axis label
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasAxisLabel(JKQTPCADrawMode pos);



/** \brief converts a JKQTPCADrawMode variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPCADrawMode2String(JKQTPCADrawMode pos);

/** \brief converts a string into a JKQTPCADrawMode
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPCADrawMode String2JKQTPCADrawMode(const QString& pos);


/** \brief display mode for the axis labels
 * \ingroup jkqtptools */
enum JKQTPCALabelType {
    JKQTPCALTdefault, /*!< \brief simply print the numbers \image html JKQTPCALTdefault.png */
    JKQTPCALTexponentCharacter, /*!< \brief print the numbers and show a unit character, i.e. \c 5µ for \f$ 5\cdot 10^{-6} \f$ , \cd 3k for \f$ 3\cdot 10^3 \f$ ...  */
    JKQTPCALTexponent, /*!< \brief show numbers in exponential for, e.g. \f$ 3\cdot 10^5 \f$ ... \image html JKQTPCALTexponent.png */
    JKQTPCALTdate, /*!< \brief show numbers as dates \image html JKQTPCALTdate.png */
    JKQTPCALTtime, /*!< \brief show numbers as times \image html JKQTPCALTtime.png*/
    JKQTPCALTdatetime, /*!< \brief show numbers as times */
};



/** \brief mode of the axis ticks
 * \ingroup jkqtptools */
enum JKQTPLabelTickMode {
    JKQTPLTMLinOrPower=0, /*!< \brief linear, or log, depending on whether the axis is log */
    JKQTPLTMLin, /*!< \brief always linear (even for log-axes) */
    JKQTPLTMPower, /*!< \brief powers (of the log-base) */
};

/** \brief converts a JKQTPLabelTickMode variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPLabelTickMode2String(JKQTPLabelTickMode pos);

/** \brief converts a string into a JKQTPLabelTickMode
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPLabelTickMode String2JKQTPLabelTickMode(const QString& pos);


/** \brief converts a JKQTPCALabelType variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPCALabelType2String(JKQTPCALabelType pos);

/** \brief converts a string into a JKQTPCALabelType
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPCALabelType String2JKQTPCALabelType(const QString& pos);

/** \brief position of the axis labels
 * \ingroup jkqtptools
 */
enum JKQTPLabelPosition {
    JKQTPLabelMin=0,            /*!< \brief the axis label is near the min value of the axis (left/bottom) */
    JKQTPLabelMax,            /*!< \brief the axis label is near the max value of the axis (right/top) */
    JKQTPLabelCenter          /*!< \brief the label is at the center of the axis */
};


/** \brief converts a JKQTPLabelPosition variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPLabelPosition2String(JKQTPLabelPosition pos);

/** \brief converts a string into a JKQTPLabelPosition
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPLabelPosition String2JKQTPLabelPosition(const QString& pos);

/** \brief position of the key
 * \ingroup jkqtptools
 */
enum JKQTPKeyPosition {
    JKQTPKeyOutsideTopRight=0,            /*!< \brief the key is positioned on the right side of the graph */
    JKQTPKeyOutsideTopLeft,             /*!< \brief the key is positioned on the left side of the graph */
    JKQTPKeyOutsideLeftTop,              /*!< \brief the key is positioned above the graph */
    JKQTPKeyOutsideLeftBottom,           /*!< \brief the key is positioned below the graph */
    JKQTPKeyOutsideRightBottom,            /*!< \brief the key is positioned on the right side of the graph */
    JKQTPKeyOutsideRightTop,             /*!< \brief the key is positioned on the left side of the graph */
    JKQTPKeyOutsideBottomLeft,              /*!< \brief the key is positioned above the graph */
    JKQTPKeyOutsideBottomRight,           /*!< \brief the key is positioned below the graph */
    JKQTPKeyInsideTopRight,             /*!< \brief the key is positioned inside on the right side of the graph, but inside the graph*/
    JKQTPKeyInsideTopLeft,              /*!< \brief the key is positioned inside on the left side of the graph */
    JKQTPKeyInsideBottomLeft,               /*!< \brief the key is positioned inside on the upper bound of the graph */
    JKQTPKeyInsideBottomRight             /*!< \brief the key is positioned inside on the lower bound of the graph */
};


/** \brief converts a JKQTPLabelPosition variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPKeyPosition2String(JKQTPKeyPosition pos);

/** \brief converts a string into a JKQTPLabelPosition
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPKeyPosition String2JKQTPKeyPosition(const QString& pos);

/** \brief layout of the key
 * \ingroup jkqtptools
 */
enum JKQTPKeyLayout {
    JKQTPKeyLayoutOneColumn,            /*!< \brief the key consists of one column */
    JKQTPKeyLayoutOneRow,            /*!< \brief the key consists of one row */
    JKQTPKeyLayoutMultiColumn,           /*!< \brief the key consists of multiple columns */
};


/** \brief converts a JKQTPKeyLayout variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPKeyLayout2String(JKQTPKeyLayout pos);

/** \brief converts a String into a JKQTPKeyLayout
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPKeyLayout String2JKQTPKeyLayout(const QString& pos);



/** \brief used to represent the position of other graphs in \ref jkqtplotter_base_saveprint
 * \ingroup jkqtptools
 * \internal
 */
struct JKQTPGridPrintingItem {
    /** \brief logic x-position of a graph, relative to the current JKQtPlotter */
    int x;
    /** \brief logic y-position of a graph, relative to the current JKQtPlotter */
    int y;
    /** \brief JKQTBasePlotter */
    JKQTBasePlotter* plotter;
};



/** \brief plot styles for the error information
 * \ingroup jkqtptools
 */
enum JKQTPErrorPlotstyle {
    JKQTPErrorEllipses=10,           /*!< \brief an ellipse spanned by the errors \image html JKQTPErrorEllipses.png */
    JKQTPErrorBoxes=9,               /*!< \brief a box spanned by the errors \image html JKQTPErrorBoxes.png */
    JKQTPErrorSimpleBarsPolygons=8,  /*!< \brief simplified error barsand polygons  for each data point \image html JKQTPErrorSimpleBarsPolygons.png */
    JKQTPErrorSimpleBarsLines=7,     /*!< \brief simplified error bars and line for each data point \image html JKQTPErrorSimpleBarsLines.png */
    JKQTPErrorSimpleBars=6,          /*!< \brief simplified error bars for each data point \image html JKQTPErrorSimpleBars.png */
    JKQTPErrorLines=5,               /*!< \brief a second and third graph line above and below the actual data which indicates the error value \image html JKQTPErrorLines.png */
    JKQTPErrorBars=4,                /*!< \brief error bars for each data point \image html JKQTPErrorBars.png */
    JKQTPErrorPolygons=3,            /*!< \brief line error lines, but with filled range in between \image html JKQTPErrorPolygons.png */
    JKQTPErrorBarsLines=2,           /*!< \brief error bars and lines for each data point \image html JKQTPErrorBarsLines.png */
    JKQTPErrorBarsPolygons=1,        /*!< \brief error bars and polygons for each data point \image html JKQTPErrorBarsPolygons.png */
    JKQTPNoError=0                   /*!< \brief don't show error information \image html JKQTPNoError.png */
};




/** \brief converts a JKQTPErrorPlotstyle variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPErrorPlotstyle2String(JKQTPErrorPlotstyle pos);

/** \brief converts a String into a JKQTPErrorPlotstyle
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPErrorPlotstyle String2JKQTPErrorPlotstyle(const QString& pos);

/** \brief plot styles for a graph
 * \ingroup jkqtptools
 */
enum JKQTPGraphPlotstyle {
    JKQTPLines,                /*!< \brief plot y=f(x), connect the datapoints by straight lines */
    JKQTPFilledCurveX,         /*!< \brief plot y=f(x), as filled curve (filled until the y=0/x-axis) */
    JKQTPFilledCurveY,         /*!< \brief plot x=f(y), as filled curve (filled until the x=0/y-axis) */
    JKQTPPoints,               /*!< \brief plot y=f(x), plot each datapoint with a symbol */
    JKQTPLinesPoints,          /*!< \brief plot y=f(x), plot each datapoint with a symbol and connect them by straight lines */
    JKQTPImpulsesHorizontal,   /*!< \brief plot y=f(x), plot each datapoint as a line from (x,0) to (x,f(x))  */
    JKQTPImpulsesVertical,     /*!< \brief plot x=f(y), plot each datapoint as a line from (0,f(x)) to (x,f(x))  */
    JKQTPStepsX,               /*!< \brief plot y=f(x), as a step curve */
    JKQTPStepsY                /*!< \brief plot x=f(y), as a step curve */
};

/** \brief symbols that can be used to plot a datapoint for a graph
 * \ingroup jkqtptools
 */
enum JKQTPGraphSymbols {
    JKQTPNoSymbol=0,             /*!< \brief plots no symbol at all (usefull together with error bars) */
    JKQTPDot=1,                 /*!< \brief a small dot \image html symbols/symbol_symbol_dot.png */
    JKQTPCross=2,               /*!< \brief a X cross \image html symbols/symbol_symbol_cross.png */
    JKQTPPlus=3,                /*!< \brief a + cross \image html symbols/symbol_symbol_plus.png */
    JKQTPCircle=4,              /*!< \brief an unfilled circle \image html symbols/symbol_symbol_circle.png */
    JKQTPFilledCircle=5,        /*!< \brief a filled circle \image html symbols/symbol_symbol_filled_circle.png */
    JKQTPRect=6,                /*!< \brief an unfilled rectangle \image html symbols/symbol_symbol_rect.png */
    JKQTPFilledRect=7,          /*!< \brief a filled rectangle \image html symbols/symbol_symbol_filled_rect.png */
    JKQTPTriangle=8,            /*!< \brief an unfilled triangle (tip at top) \image html symbols/symbol_symbol_triangle.png */
    JKQTPFilledTriangle=9,      /*!< \brief a filled triangle (tip at top) \image html symbols/symbol_symbol_filled_triangle.png */
    JKQTPDownTriangle=10,        /*!< \brief an unfilled triangle (tip at bottom) \image html symbols/symbol_symbol_down_triangle.png */
    JKQTPFilledDownTriangle=11,  /*!< \brief a filled triangle (tip at bottom) \image html symbols/symbol_symbol_filled_down_triangle.png */
    JKQTPDiamond=12,                /*!< \brief an unfilled diamond \image html symbols/symbol_symbol_diamond.png */
    JKQTPFilledDiamond=13,          /*!< \brief a filled diamond \image html symbols/symbol_symbol_filled_diamond.png */
    JKQTPstar=14,                /*!< \brief an unfilled diamond \image html symbols/symbol_symbol_star.png */
    JKQTPFilledStar=15,          /*!< \brief a filled diamond \image html symbols/symbol_symbol_filled_star.png */
    JKQTPPentagon=16,                /*!< \brief an unfilled pentagon \image html symbols/symbol_symbol_pentagon.png */
    JKQTPFilledPentagon=17,          /*!< \brief a filled pentagon \image html symbols/symbol_symbol_filled_pentagon.png */
    JKQTPAsterisc=18,          /*!< \brief an asterisc star '*' \image html symbols/symbol_symbol_asterisc.png */

    JKQTPTarget=19,              /*!< \brief a target symbol (circle with cross) \image html symbols/symbol_symbol_target.png */
    JKQTPRectCross=20,              /*!< \brief a square symbol with a cross inside \image html symbols/symbol_symbol_rect_cross.png */
    JKQTPRectPlus=21,              /*!< \brief a square symbol with a plus inside \image html symbols/symbol_symbol_rect_plus.png */

    JKQTPMaxSymbolID=JKQTPRectPlus,
    JKQTPDefaultSymbol=JKQTPCross,
};

/** \brief converts a JKQTPGraphSymbols variable into a identifier string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPGraphSymbols2String(JKQTPGraphSymbols pos);
/** \brief converts a JKQTPGraphSymbols variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPGraphSymbols2NameString(JKQTPGraphSymbols pos);

/** \brief converts a String into a JKQTPGraphSymbols
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPGraphSymbols String2JKQTPGraphSymbols(const QString& pos);


/** \brief convert a double to a string, using the loacle "C"
 * \ingroup jkqtptools
 */
inline QString JKQTPCDoubleToQString(double value) {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    return loc.toString(value, 'g', 18);
}

/** \brief convert a double to a string
 * \ingroup jkqtptools
 */
inline QString JKQTPDoubleToQString(double value, int prec = 10, char f = 'g', QChar decimalSeparator='.') {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QString res=loc.toString(value, f, prec);
    if (loc.decimalPoint()!=decimalSeparator) {
        res=res.replace(loc.decimalPoint(), decimalSeparator);
    }
    return res;
}



/** \brief rotate a rectangle by  given angle (rotates all points around the center of the rectangle and returns it as a QPolygonF)
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QPolygonF jkqtpRotateRect(QRectF r, double angle);




/*! \brief swap two elements \a l and \a r in an array \a a
    \ingroup jkqtptools

*/
template <class T>
inline void jkqtpSwap(T* a, int l, int r){
    const T tmp=a[l];
    a[l]=a[r];
    a[r]=tmp;
}




/*! \brief QuickSort (recursive implementation)
    \ingroup jkqtptools

    implementation from http://www.linux-related.de/index.html?/coding/sort/sort_quick.htm
*/
template <class T>
inline void jkqtpQuicksort(T* a, int l, int r){
    if(r>l){
        int i=l-1;
        int j=r;

        for(;;){
            while(a[++i]<a[r]);
            while(a[--j]>a[r] && j>i);
            if(i>=j) break;
            jkqtpSwap<T>(a, i, j);
        }
        jkqtpSwap<T>(a, i, r);

        jkqtpQuicksort(a, l, i-1);
        jkqtpQuicksort(a, i+1, r);
    }
}


/*! \brief QuickSort (recursive implementation), sorts \a a2 alongside \a a, using \a a as sort criterion
    \ingroup jkqtptools

    implementation from http://www.linux-related.de/index.html?/coding/sort/sort_quick.htm
*/
template <class T, class T2>
inline void jkqtpQuicksort(T* a, T2* a2, int l, int r){
    if(r>l){
        int i=l-1;
        int j=r;

        for(;;){
            while(a[++i]<a[r]);
            while(a[--j]>a[r] && j>i);
            if(i>=j) break;
            jkqtpSwap(a, i, j);
            jkqtpSwap(a2, i, j);
        }
        jkqtpSwap(a, i, r);
        jkqtpSwap(a2, i, r);

        jkqtpQuicksort(a, a2, l, i-1);
        jkqtpQuicksort(a, a2, i+1, r);
    }
}




/*! \brief sort the given arrays, using \a input as sort criterion
    \ingroup jkqtptools

    \param input array to be sorted
    \param input2 array to be sorted
    \param N size of the array input
    \param output if \c !=nullptr data is written here (the memory location pointed at by \a output has to have at least the length \a N !!!),
                  otherwise the array input is sorted inplace.
    \param output2 if \c !=nullptr data is written here (the memory location pointed at by \a output has to have at least the length \a N !!!),
                  otherwise the array input is sorted inplace.

 */
template <class T, class T2>
inline void jkqtpSort(T* input, T2* input2, int N, T* output=nullptr, T2* output2=nullptr) {
    if ((!input)) return ;
    if (N<=0) return;
    T* data=input;
    if (output!=nullptr) {
        data=output;
        memcpy(output, input, N*sizeof(T));
    }
    T2* data2=input2;
    if (output2!=nullptr && input2!=nullptr) {
        data2=output2;
        memcpy(output2, input2, N*sizeof(T2));
    }
    jkqtpQuicksort(data, data2, 0, N-1);
}

/** \brief RAII construct that times its lifetime, outputting properly indented qDebug()-message
 * \ingroup jkqtptools_debugging
 */
class JKQTP_LIB_EXPORT JKQTPAutoOutputTimer : public QElapsedTimer
{
    public:
        explicit JKQTPAutoOutputTimer(const QString& message);
        ~JKQTPAutoOutputTimer();
        void write(const QString& message) const;
    protected:
        QString message;
        QString indent;
        static int global_indent;

};

/** \brief convert a string to lower-case characters
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_tolower(const std::string& s);

/** \brief convert a string to a boolean
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT bool jkqtp_strtobool(const std::string& data);
/** \brief convert a string to upper-case
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_toupper(const std::string& s);

/** \brief std::string wrapper around sprintf()
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_format(const std::string& templ, ...);

/** \brief convert a number of bytes to a string, formatting e.g. 1024 as 1kB, ...
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_bytestostr(double bytes);

/** \brief convert an integer to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_inttostr(long data);

/** \brief convert an integer to a hex string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_inttohex(long data);

/** \brief convert an unsigned int to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_uinttostr(unsigned long data);

/** \brief convert a double to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattostr(double data, int past_comma=-1, bool remove_trail0=false, double belowIsZero=1e-16);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000,"g") will result in "1kg"
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattounitstr(double dataa, const std::string& unitname);
/** \brief convert a boolean to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_booltostr(bool data);
/** \brief converts a RGBA color into a string
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 *
 * \param r red value of the color to convert
 * \param g green value of the color to convert
 * \param b blue value of the color to convert
 * \param a alpha value of the color to convert
 * \param useSpecialTransparencySyntax is set (\c true ), the function uses a special syntax to denote color and transparency: \c color,trans
 */
JKQTP_LIB_EXPORT QString jkqtp_rgbtostring(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255, bool useSpecialTransparencySyntax=true);

/** \brief converts a QColor into a string using the jkqtp_rgbtostring() method.
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
JKQTP_LIB_EXPORT QString jkqtp_QColor2String(QColor color, bool useSpecialTransparencySyntax=true);

/** \brief converts a QString into a QColor, compatible with jkqtp_QColor2String(QColor color);
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
JKQTP_LIB_EXPORT QColor jkqtp_String2QColor(const QString& color);

/** \brief clean a string to be usable as a variable name, e.g. in an expression parser, or a C++-expression
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_to_valid_variable_name(const std::string& input);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattounitstr(double data, int past_comma=5, bool remove_trail0=false);
/** \brief convert a double to a string, encoding powers of ten as exponent in LaTeX notation (e.g. <code>-1.23\cdot 10^{-5}</code>)
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattolatexstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
/** \brief convert a double to a string, encoding powers of ten as exponent with HTML tags
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_floattohtmlstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
/** \brief convert a character to a string
 * \ingroup jkqtptools_string
 */
JKQTP_LIB_EXPORT std::string jkqtp_chartostr(char data);

/** \brief wandelt einen Datentyp in einen double um, wird von JKQTPDatastore zur Wandlung benutzt
 *  \ingroup jkqtptools
 *
 * Diese Funktion nutzt per default static_cast<double>(), kann aber für spezielle Datentypen überschrieben werden, etwa für bool
*/
template<typename T>
inline constexpr double jkqtp_todouble(const T& d) {
    return static_cast<double>(d);
}


/** \brief wandelt einen boolean in einen double um, wird von JKQTPDatastore zur Wandlung benutzt,
 *         Spezialisierung für bool (true -> 1.0, false -> 0.0)
 *  \ingroup jkqtptools */
template<>
inline constexpr double jkqtp_todouble(const bool& d) {
    return static_cast<double>((d)?1.0:0.0);
}




/** \brief plot styles for the error information
 * \ingroup jkqtptools
 *
 * \see JKQTPSpecialLineTypeComboBox
 */
enum JKQTPSpecialLineType {
    JKQTPStepLeft=0,           /*!< \brief datapoint is on the left edge of the hor. step line \image html JKQTPStepLeft.png */
    JKQTPStepCenter=1,           /*!< \brief datapoint is centered on the hor. step line \image html JKQTPStepCenter.png */
    JKQTPStepRight=2,         /*!< \brief datapoint is on the right edge of the hor. step line \image html JKQTPStepRight.png */
    JKQTPStepAverage=3,         /*!< \brief horizontal line on the level of the average between two points + vertical lines to the points \image html JKQTPStepAverage.png */
    JKQTPDirectLine=4,         /*!< \brief no step, but connects the two datapoints with a straight line \image html JKQTPDirectLine.png */
};


/** \brief converts a JKQTPSpecialLineType variable into a human-readable string
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT QString JKQTPSpecialLineType2String(JKQTPSpecialLineType pos);

/** \brief converts a String into a JKQTPSpecialLineType
 * \ingroup jkqtptools
 */
JKQTP_LIB_EXPORT JKQTPSpecialLineType String2JKQTPSpecialLineType(const QString& pos);

/** \brief round a double using round() and convert it to a specified type T (static_cast!) */
template<typename T>
inline T jkqtp_roundTo(const double& v) {
    return static_cast<T>(round(v));
}

#endif // JKQTPTOOLS_H_INCLUDED
