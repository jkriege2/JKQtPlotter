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
#include "jkqtcommon/jkqtp_imexport.h"
#include <QString>
#include <QElapsedTimer>
#include <QLocale>
#include <cfloat>
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
#include <limits>
#include <cstdint>
#include <stdexcept>
#include <cctype>
#include "jkqtpstringtools.h"
#include "jkqtptoolsdebugging.h"
#include "jkqtpcommonmathtools.h"
#include "jkqtpalgorithms.h"


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


/** \brief C++11 finally construct (executes a callable-object when the edestructor is executed)
 * \ingroup jkqtptools_codestructuring
 *
 * Typical usage:
 * \code
 *   {
 *     // the instruction 'painter.restore()' will be executed at the end
 *     // of the block, when __finalpaint is destroyed (see (*) below)
 *     JKQTPFinalAct __finalpaint([&painter]() { painter.restore(); });
 *
 *     // ...
 *     // do something ...
 *     // ...
 *
 *   } // (*) 'painter.restore()' is executed before the end of this block!
 * \endcode
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

/** \brief C++11 finally construct (executes a callable-object at the end of a scope)
 * \ingroup jkqtptools_codestructuring
 *
 * Typical usage:
 * \code
 *   {
 *     // the instruction 'painter.restore()' will be executed at the end
 *     // of the block, when __finalpaint is destroyed (see (*) below)
 *     auto __finalpaint=JKQTPFinally([&painter]() { painter.restore(); });
 *
 *     // ...
 *     // do something ...
 *     // ...
 *
 *   } // (*) 'painter.restore()' is executed before the end of this block!
 * \endcode
 *
 * \see JKQTPFinalAct
 */
template <class F>
inline JKQTPFinalAct<F> JKQTPFinally(const F& f) noexcept
{
    return JKQTPFinalAct<F>(f);
}

/** \brief C++11 finally construct (executes a callable-object at the end of a scope)
 * \ingroup jkqtptools_codestructuring
 *
 * Typical usage:
 * \code
 *   {
 *     // the instruction 'painter.restore()' will be executed at the end
 *     // of the block, when __finalpaint is destroyed (see (*) below)
 *     auto __finalpaint=JKQTPFinally([&painter]() { painter.restore(); });
 *
 *     // ...
 *     // do something ...
 *     // ...
 *
 *   } // (*) 'painter.restore()' is executed before the end of this block!
 * \endcode
 *
 * \see JKQTPFinalAct
 */
template <class F>
inline JKQTPFinalAct<F> JKQTPFinally(F&& f) noexcept
{
    return JKQTPFinalAct<F>(std::forward<F>(f));
}



/** \brief Styles in which to mark single positions during user actions in JKQTPlotter
 * \ingroup jkqtpplottersupprt

 */
enum JKQTPUserActionMarkerType {
    jkqtpuamtCircle=0, /*!< \brief a small circle around the position to mark */
    jkqtpuamtCrossHair=1, /*!< \brief a cross-hair to the position to mark */
    jkqtpuamtCircleAndCrossHair=2, /*!< \brief a cross-hair to the position to mark, with a circle-marker around the actual target (i.e. combines jkqtpuamtCircle and jkqtpuamtCrossHair) */
};


/** \brief convert a JKQTPUserActionMarkerType to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplottersupprt
 *
 *  \see String2JKQTPUserActionMarkerType(), JKQTPUserActionMarkerType
 */
JKQTP_LIB_EXPORT QString JKQTPUserActionMarkerType2String(JKQTPUserActionMarkerType act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPUserActionMarkerType2String() ) to JKQTPUserActionMarkerType
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPUserActionMarkerType2String(), JKQTPUserActionMarkerType
 */
JKQTP_LIB_EXPORT JKQTPUserActionMarkerType String2JKQTPUserActionMarkerType(const QString &act);



/** \brief Availble action this JKQtPlotter can perform when mouse events occur.
 *         This allows you to e.g. draw rectangles or lines over the plot and receive a signal, when the drawing finishes
 * \ingroup jkqtpplottersupprt

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
    jkqtpmdaToolTipForClosestDataPoint, /*!< \brief shows a tooltip with data of the closest data-point in the plot \image html tooltiptool.gif */
    jkqtpmdaRuler, /*!< \brief shows a ruler over the plot, which measures delta X, delta Y and sqrt(dx^2+dy^2) \image html rulertool.gif */
};


/** \brief convert a JKQTPMouseDragActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplottersupprt
 *
 *  \see String2JKQTPMouseDragActions(), JKQTPMouseDragActions
 */
JKQTP_LIB_EXPORT QString JKQTPMouseDragActions2String(JKQTPMouseDragActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseDragActions2String() ) to JKQTPMouseDragActions
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPMouseDragActions2String(), JKQTPMouseDragActions
 */
JKQTP_LIB_EXPORT JKQTPMouseDragActions String2JKQTPMouseDragActions(const QString &button);

/** \brief actions that can be bound to a double-click of the mouse
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPMouseDoubleClickActions {
    jkqtpdcaClickZoomsIn=0, /*!< \brief a double-click zooms into the plot at the current mouse location */
    jkqtpdcaClickZoomsOut, /*!< \brief a double-click zooms out of the plot at the current mouse location */
    jkqtpdcaClickOpensContextMenu, /*!< \brief a double-click opens the context menu */
    jkqtpdcaClickOpensSpecialContextMenu, /*!< \brief a double-click opens the special context menu \see setSpecialContextMenu() */
    jkqtpdcaClickMovesViewport, /*!< \brief a double-click centers the x/y-range around the clicked position */
};

/** \brief convert a JKQTPMouseDoubleClickActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplottersupprt
 *
 *  \see String2JKQTPMouseDoubleClickActions(), JKQTPMouseDoubleClickActions
 */
JKQTP_LIB_EXPORT QString JKQTPMouseDoubleClickActions2String(JKQTPMouseDoubleClickActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseDoubleClickActions2String() ) to JKQTPMouseDoubleClickActions
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPMouseDoubleClickActions2String(), JKQTPMouseDoubleClickActions
 */
JKQTP_LIB_EXPORT JKQTPMouseDoubleClickActions String2JKQTPMouseDoubleClickActions(const QString &act);

/** \brief actions that can be bound to a mouse wheel event
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPMouseWheelActions {
    jkqtpmwaZoomByWheel=0, /*!< \brief use the mouse-wheel for zooming */
    jkqtpmwaPanByWheel, /*!< \brief use the mouse-wheel for panning the plot */
};

/** \brief convert a JKQTPMouseWheelActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplottersupprt
 *
 *  \see String2JKQTPMouseWheelActions(), JKQTPMouseWheelActions
 */
JKQTP_LIB_EXPORT QString JKQTPMouseWheelActions2String(JKQTPMouseWheelActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseWheelActions2String() ) to JKQTPMouseWheelActions
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPMouseWheelActions2String(), JKQTPMouseWheelActions
 */
JKQTP_LIB_EXPORT JKQTPMouseWheelActions String2JKQTPMouseWheelActions(const QString &act);

/** \brief modes for the context menu
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPContextMenuModes {
    jkqtpcmmStandardContextMenu=0,  /*!< \brief only show the standard context menu \image html zoomin_mouse_contextmenu.gif "Zooming with the mouse" */
    jkqtpcmmSpecialContextMenu,  /*!< \brief only show the special context menu \see setSpecialContextMenu() */
    jkqtpcmmStandardAndSpecialContextMenu,  /*!< \brief show the standard context menu, with the special context menu incorporated \see setSpecialContextMenu() */
    jkqtpcmmNoContextMenu, /*!< \brief don't show a context menu at all */
};

/** \brief convert a JKQTPContextMenuModes to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplottersupprt
 *
 *  \see String2JKQTPContextMenuModes(), JKQTPContextMenuModes
 */
JKQTP_LIB_EXPORT QString JKQTPContextMenuModes2String(JKQTPContextMenuModes act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPContextMenuModes2String() ) to JKQTPContextMenuModes
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPContextMenuModes2String(), JKQTPContextMenuModes
 */
JKQTP_LIB_EXPORT JKQTPContextMenuModes String2JKQTPContextMenuModes(const QString &act);

/** \brief data structure for storage of assigned JKQTPMouseDragActions \see JKQTPMouseDragActionsHashMapIterator
 *  \ingroup jkqtpplottersupprt */
typedef QHash<QPair<Qt::MouseButton,Qt::KeyboardModifiers>, JKQTPMouseDragActions> JKQTPMouseDragActionsHashMap;
/** \brief iterator for JKQTPMouseDragActionsHashMap \see JKQTPMouseDragActionsHashMap
 *  \ingroup jkqtpplottersupprt */
typedef JKQTPMouseDragActionsHashMap::const_iterator JKQTPMouseDragActionsHashMapIterator;

/** \brief data structure for storage of assigned JKQTPMouseWheelActions \see JKQTPMouseWheelActionsHashMapIterator
 *  \ingroup jkqtpplottersupprt */
typedef QHash<Qt::KeyboardModifiers, JKQTPMouseWheelActions> JKQTPMouseWheelActionsHashMap;
/** \brief iterator for JKQTPMouseWheelActionsHashMap \see JKQTPMouseWheelActionsHashMap
 *  \ingroup jkqtpplottersupprt */
typedef JKQTPMouseWheelActionsHashMap::const_iterator JKQTPMouseWheelActionsHashMapIterator;

/** \brief data structure for storage of assigned JKQTPMouseDoubleClickActions \see JKQTPMouseDoubleClickActionsHashMapIterator
 *  \ingroup jkqtpplottersupprt */
typedef QHash<QPair<Qt::MouseButton,Qt::KeyboardModifiers>, JKQTPMouseDoubleClickActions> JKQTPMouseDoubleClickActionsHashMap;
/** \brief iterator for JKQTPMouseDoubleClickActionsHashMap \see JKQTPMouseDoubleClickActionsHashMap
 *  \ingroup jkqtpplottersupprt */
typedef JKQTPMouseDoubleClickActionsHashMap::const_iterator JKQTPMouseDoubleClickActionsHashMapIterator;



/** \brief Specifies how a fill-color is derived from a given color
 * \ingroup jkqtpplotter_styling

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
 *  \ingroup jkqtpplotter_styling
 *
 *  \see JKQTPColorDerivationMode
 */
JKQTP_LIB_EXPORT QColor JKQTPGetDerivedColor(JKQTPColorDerivationMode mode, const QColor& col);

/** \brief construct a QColor, based on the given \a color, but with alpha set to the specified value \a alphaF
 *  \ingroup jkqtpplotter_styling
 *  \see QColorWithAlpha()
 */
inline QColor QColorWithAlphaF(const QColor& color, qreal alphaF) {
    QColor c=color;
    c.setAlphaF(alphaF);
    return c;
}

/** \brief construct a QColor, based on the given \a color, but with alpha set to the specified value \a alpha
 *  \ingroup jkqtpplotter_styling
 *  \see QColorWithAlphaF()
 */
inline QColor QColorWithAlpha(const QColor& color, int alpha) {
    QColor c=color;
    c.setAlpha(alpha);
    return c;
}

/** \brief convert a JKQTPColorDerivationMode to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplotter_styling
 *
 *  \see String2JKQTPColorDerivationMode(), JKQTPColorDerivationMode
 */
JKQTP_LIB_EXPORT QString JKQTPColorDerivationMode2String(JKQTPColorDerivationMode mode);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPColorDerivationMode2String() ) to JKQTPColorDerivationMode
 *  \ingroup jkqtpplotter_styling
 *
 *  \see JKQTPColorDerivationMode2String(), JKQTPColorDerivationMode
 */
JKQTP_LIB_EXPORT JKQTPColorDerivationMode String2JKQTPColorDerivationMode(const QString &mode);


/** \brief display mode for an axis
 * \ingroup jkqtpplottersupprt */
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
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasLine(JKQTPCADrawMode pos);

/** \brief determines whether JKQTPCADrawMode has ticks
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasTicks(JKQTPCADrawMode pos);

/** \brief determines whether JKQTPCADrawMode has tick labels
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasTickLabels(JKQTPCADrawMode pos);

/** \brief determines whether JKQTPCADrawMode has the axis label
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT bool JKQTPCADrawModeHasAxisLabel(JKQTPCADrawMode pos);



/** \brief converts a JKQTPCADrawMode variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT QString JKQTPCADrawMode2String(JKQTPCADrawMode pos);

/** \brief converts a string into a JKQTPCADrawMode
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT JKQTPCADrawMode String2JKQTPCADrawMode(const QString& pos);


/** \brief display mode for the axis labels
 * \ingroup jkqtpplottersupprt */
enum JKQTPCALabelType {
    JKQTPCALTdefault, /*!< \brief simply print the numbers \image html JKQTPCALTdefault.png */
    JKQTPCALTexponentCharacter, /*!< \brief print the numbers and show a unit character, i.e. \c 5µ for \f$ 5\cdot 10^{-6} \f$ , \c 3k for \f$ 3\cdot 10^3 \f$ ...  */
    JKQTPCALTexponent, /*!< \brief show numbers in exponential for, e.g. \f$ 3\cdot 10^5 \f$ ... \image html JKQTPCALTexponent.png */
    JKQTPCALTdate, /*!< \brief show numbers as dates \image html JKQTPCALTdate.png */
    JKQTPCALTtime, /*!< \brief show numbers as times \image html JKQTPCALTtime.png*/
    JKQTPCALTdatetime, /*!< \brief show numbers as times */
};



/** \brief mode of the axis ticks
 * \ingroup jkqtpplottersupprt */
enum JKQTPLabelTickMode {
    JKQTPLTMLinOrPower=0, /*!< \brief linear, or log, depending on whether the axis is log */
    JKQTPLTMLin, /*!< \brief always linear (even for log-axes) */
    JKQTPLTMPower, /*!< \brief powers (of the log-base) */
};

/** \brief converts a JKQTPLabelTickMode variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT QString JKQTPLabelTickMode2String(JKQTPLabelTickMode pos);

/** \brief converts a string into a JKQTPLabelTickMode
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT JKQTPLabelTickMode String2JKQTPLabelTickMode(const QString& pos);


/** \brief converts a JKQTPCALabelType variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT QString JKQTPCALabelType2String(JKQTPCALabelType pos);

/** \brief converts a string into a JKQTPCALabelType
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT JKQTPCALabelType String2JKQTPCALabelType(const QString& pos);

/** \brief position of the axis labels
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPLabelPosition {
    JKQTPLabelMin=0,            /*!< \brief the axis label is near the min value of the axis (left/bottom) */
    JKQTPLabelMax,            /*!< \brief the axis label is near the max value of the axis (right/top) */
    JKQTPLabelCenter          /*!< \brief the label is at the center of the axis */
};


/** \brief converts a JKQTPLabelPosition variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT QString JKQTPLabelPosition2String(JKQTPLabelPosition pos);

/** \brief converts a string into a JKQTPLabelPosition
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT JKQTPLabelPosition String2JKQTPLabelPosition(const QString& pos);

/** \brief position of the key
 * \ingroup jkqtpplottersupprt
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
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT QString JKQTPKeyPosition2String(JKQTPKeyPosition pos);

/** \brief converts a string into a JKQTPLabelPosition
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT JKQTPKeyPosition String2JKQTPKeyPosition(const QString& pos);

/** \brief layout of the key
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPKeyLayout {
    JKQTPKeyLayoutOneColumn,            /*!< \brief the key consists of one column */
    JKQTPKeyLayoutOneRow,            /*!< \brief the key consists of one row */
    JKQTPKeyLayoutMultiColumn,           /*!< \brief the key consists of multiple columns */
};


/** \brief converts a JKQTPKeyLayout variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT QString JKQTPKeyLayout2String(JKQTPKeyLayout pos);

/** \brief converts a String into a JKQTPKeyLayout
 * \ingroup jkqtpplottersupprt
 */
JKQTP_LIB_EXPORT JKQTPKeyLayout String2JKQTPKeyLayout(const QString& pos);



/** \brief used to represent the position of other graphs in \ref jkqtplotter_base_saveprint
 * \ingroup jkqtpplottersupprt
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
 * \ingroup jkqtplotter_basegraphserrors
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
 * \ingroup jkqtplotter_basegraphserrors
 */
JKQTP_LIB_EXPORT QString JKQTPErrorPlotstyle2String(JKQTPErrorPlotstyle pos);

/** \brief converts a String into a JKQTPErrorPlotstyle
 * \ingroup jkqtplotter_basegraphserrors
 */
JKQTP_LIB_EXPORT JKQTPErrorPlotstyle String2JKQTPErrorPlotstyle(const QString& pos);

/** \brief plot styles for a graph
 * \ingroup jkqtpplottersupprt
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
 * \ingroup jkqtptools_drawing
 */
enum JKQTPGraphSymbols {
    JKQTPNoSymbol=0,             /*!< \brief plots no symbol at all (usefull together with error bars) */
    JKQTPDot,                 /*!< \brief a small dot \image html symbols/symbol_dot.png */
    JKQTPCross,               /*!< \brief a X cross \image html symbols/symbol_cross.png */
    JKQTPPlus,                /*!< \brief a + cross \image html symbols/symbol_plus.png */
    JKQTPCircle,              /*!< \brief an unfilled circle \image html symbols/symbol_circle.png */
    JKQTPFilledCircle,        /*!< \brief a filled circle \image html symbols/symbol_filled_circle.png */
    JKQTPRect,                /*!< \brief an unfilled rectangle \image html symbols/symbol_rect.png */
    JKQTPFilledRect,          /*!< \brief a filled rectangle \image html symbols/symbol_filled_rect.png */
    JKQTPTriangle,            /*!< \brief an unfilled triangle (tip at top) \image html symbols/symbol_triangle.png */
    JKQTPFilledTriangle,      /*!< \brief a filled triangle (tip at top) \image html symbols/symbol_filled_triangle.png */
    JKQTPDiamond,                /*!< \brief an unfilled diamond \image html symbols/symbol_diamond.png */
    JKQTPFilledDiamond,          /*!< \brief a filled diamond \image html symbols/symbol_filled_diamond.png */
    JKQTPstar,                /*!< \brief an unfilled diamond \image html symbols/symbol_star.png */
    JKQTPFilledStar,          /*!< \brief a filled diamond \image html symbols/symbol_filled_star.png */
    JKQTPPentagon,                /*!< \brief an unfilled pentagon \image html symbols/symbol_pentagon.png */
    JKQTPFilledPentagon,          /*!< \brief a filled pentagon \image html symbols/symbol_filled_pentagon.png */
    JKQTPAsterisc,          /*!< \brief an asterisc star with 5 arms \image html symbols/symbol_asterisc.png */
    JKQTPHourglass,              /*!< \brief an hour glass symbol \image html symbols/symbol_hourglass.png */
    JKQTPFilledHourglass,              /*!< \brief a filled hour glass symbol \image html symbols/symbol_filled_hourglass.png */
    JKQTPCurvedTriangle,  /*!< \brief a  curved triangle\image html symbols/symbol_curved_triangle.png */
    JKQTPFilledCurvedTriangle,  /*!< \brief a filled curved triangle\image html symbols/symbol_filled_curved_triangle.png */
    JKQTPHexagon,                /*!< \brief an unfilled hexagon \image html symbols/symbol_hexagon.png */
    JKQTPFilledHexagon,          /*!< \brief a filled hexagon \image html symbols/symbol_filled_hexagon.png */

    JKQTPRectCross,              /*!< \brief a square symbol with a cross inside \image html symbols/symbol_rect_cross.png */
    JKQTPRectPlus,              /*!< \brief a square symbol with a plus inside \image html symbols/symbol_rect_plus.png */
    JKQTPRectTriangle,              /*!< \brief a square symbol with a triangle inside \image html symbols/symbol_rect_triangle.png */
    JKQTPRectDownTriangle,              /*!< \brief a square symbol with a triangle (tip to the bottom) inside \image html symbols/symbol_rect_downtriangle.png */
    JKQTPRectLeftTriangle,              /*!< \brief a square symbol with a triangle (tip to the left) inside \image html symbols/symbol_rect_lefttriangle.png */
    JKQTPRectRightTriangle,              /*!< \brief a square symbol with a triangle (tip to the right) inside \image html symbols/symbol_rect_righttriangle.png */

    JKQTPCircleCross,              /*!< \brief a circle symbol with a cross inside \image html symbols/symbol_circle_cross.png */
    JKQTPCirclePlus,              /*!< \brief a circle symbol with a plus inside \image html symbols/symbol_circle_plus.png */
    JKQTPCirclePeace,          /*!< \brief a circled peace symbol \image html symbols/symbol_circle_peace.png */

    JKQTPDiamondPlus,              /*!< \brief a diamond symbol with a plus inside \image html symbols/symbol_diamond_plus.png */
    JKQTPDiamondCross,              /*!< \brief a diamond symbol with a cross inside \image html symbols/symbol_diamond_cross.png */

    JKQTPTripod,  /*!< \brief a tripod symbol \image html symbols/symbol_tripod.png */
    JKQTPDownTripod,  /*!< \brief a tripod symbol, pointing down \image html symbols/symbol_down_tripod.png */
    JKQTPLeftTripod,  /*!< \brief a tripod symbol, pointing to the left \image html symbols/symbol_left_tripod.png */
    JKQTPRightTripod,  /*!< \brief a tripod symbol, pointing to the right \image html symbols/symbol_right_tripod.png */
    JKQTPAsterisc6,          /*!< \brief an asterisc star with 6 arms \image html symbols/symbol_asterisc6.png */
    JKQTPAsterisc8,          /*!< \brief an asterisc star with 8 arms \image html symbols/symbol_asterisc8.png */
    JKQTPPeace,          /*!< \brief a peace symbol \image html symbols/symbol_peace.png */
    JKQTPTarget,              /*!< \brief a target symbol (circle with cross) \image html symbols/symbol_target.png */

    JKQTPDownTriangle,        /*!< \brief an unfilled triangle (tip at bottom) \image html symbols/symbol_down_triangle.png */
    JKQTPFilledDownTriangle,  /*!< \brief a filled triangle (tip at bottom) \image html symbols/symbol_filled_down_triangle.png */
    JKQTPLeftTriangle,        /*!< \brief an unfilled triangle (tip to the left) \image html symbols/symbol_left_triangle.png */
    JKQTPFilledLeftTriangle,  /*!< \brief a filled triangle (tip to the left) \image html symbols/symbol_filled_left_triangle.png */
    JKQTPRightTriangle,        /*!< \brief an unfilled triangle (tip to the right) \image html symbols/symbol_right_triangle.png */
    JKQTPFilledRightTriangle,  /*!< \brief a filled triangle (tip to the right) \image html symbols/symbol_filled_right_triangle.png */
    JKQTPDownCurvedTriangle,  /*!< \brief a  curved triangle, pointing down \image html symbols/symbol_down_curved_triangle.png */
    JKQTPFilledDownCurvedTriangle,  /*!< \brief a filled curved triangle, pointing down \image html symbols/symbol_filled_down_curved_triangle.png */
    JKQTPLeftCurvedTriangle,  /*!< \brief a  curved triangle, pointing to the left \image html symbols/symbol_left_curved_triangle.png */
    JKQTPFilledLeftCurvedTriangle,  /*!< \brief a filled curved triangle, pointing to the left \image html symbols/symbol_filled_left_curved_triangle.png */
    JKQTPRightCurvedTriangle,  /*!< \brief a  curved triangle, pointing to the right \image html symbols/symbol_right_curved_triangle.png */
    JKQTPFilledRightCurvedTriangle,  /*!< \brief a filled curved triangle, pointing to the right \image html symbols/symbol_filled_right_curved_triangle.png */
    JKQTPOctagon,                /*!< \brief an unfilled octagon \image html symbols/symbol_octagon.png */
    JKQTPFilledOctagon,          /*!< \brief a filled octagon \image html symbols/symbol_filled_octagon.png */
    JKQTPUpDownTriangle,              /*!< \brief a overlay of an up and a down triangle symbol \image html symbols/symbol_updowntriangle.png */
    JKQTPFilledUpDownTriangle,              /*!< \brief a filled version of the overlay of an up and a down triangle \image html symbols/symbol_filled_updowntriangle.png */
    JKQTPHorizontalHourglass,              /*!< \brief a horizontal hour glass symbol \image html symbols/symbol_horizontal_hourglass.png */
    JKQTPFilledHorizontalHourglass,              /*!< \brief a filled horizontal hour glass symbol \image html symbols/symbol_filled_horizontal_hourglass.png */

    JKQTPSantaClauseHouse,              /*!< \brief a small house symbol ("Das is das haus vom Nicolaus") \image html symbols/symbol_santaclause.png */
    JKQTPFilledSantaClauseHouse,              /*!< \brief a filled small house symbol ("Das is das haus vom Nicolaus") \image html symbols/symbol_filled_santaclause.png */

    JKQTPMale,                /*!< \brief a male symbol \image html symbols/symbol_male.png */
    JKQTPFemale,                /*!< \brief a female symbol \image html symbols/symbol_female.png */

    JKQTPSymbolCount, /*!< \brief can be used to iterate over all symbols using: <code>for (int i=0; i<static_cast<int>(JKQTPSymbolCount); i++) { JKQTPGraphSymbols s=static_cast<JKQTPGraphSymbols>(i); ... }</code> */
    JKQTPMaxSymbolID=JKQTPSymbolCount-1, /*!< \brief points to the last available symbol, can be used to iterate over all symbols: <code>for (int i=0; i<=static_cast<int>(JKQTPMaxSymbolID); i++) { JKQTPGraphSymbols s=static_cast<JKQTPGraphSymbols>(i); ... }</code> */
    JKQTPDefaultSymbol=JKQTPCross, /*!< \brief a default symbol used for plotting */
};

/** \brief converts a JKQTPGraphSymbols variable into a identifier string
 * \ingroup jkqtptools_drawing
 */
JKQTP_LIB_EXPORT QString JKQTPGraphSymbols2String(JKQTPGraphSymbols pos);
/** \brief converts a JKQTPGraphSymbols variable into a human-readable string
 * \ingroup jkqtptools_drawing
 */
JKQTP_LIB_EXPORT QString JKQTPGraphSymbols2NameString(JKQTPGraphSymbols pos);

/** \brief converts a String into a JKQTPGraphSymbols
 * \ingroup jkqtptools_drawing
 */
JKQTP_LIB_EXPORT JKQTPGraphSymbols String2JKQTPGraphSymbols(const QString& pos);




/** \brief plot styles for the error information
 * \ingroup jkqtplotter_linesymbolgraphs_simple
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
 * \ingroup jkqtplotter_linesymbolgraphs_simple
 */
JKQTP_LIB_EXPORT QString JKQTPSpecialLineType2String(JKQTPSpecialLineType pos);

/** \brief converts a String into a JKQTPSpecialLineType
 * \ingroup jkqtplotter_linesymbolgraphs_simple
 */
JKQTP_LIB_EXPORT JKQTPSpecialLineType String2JKQTPSpecialLineType(const QString& pos);




#endif // JKQTPTOOLS_H_INCLUDED
