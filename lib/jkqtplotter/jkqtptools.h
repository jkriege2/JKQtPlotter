/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)



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
#include "jkqtplotter/jkqtplotter_imexport.h"
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
#include <QColor>
#include <QFlags>
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon/jkqtpmathtools.h"
#include "jkqtmath/jkqtpalgorithms.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#if __cplusplus >= 202002L
# include <version>
#endif





class JKQTPEnhancedPainter; // forward
class JKQTBasePlotter; // forward declaration


/** \brief used to specify teh type of graph when calling getPlotStyle()
 * \ingroup jkqtpplottersupprt
 */
enum class JKQTPPlotStyleType {
    Default, /*!< general graphs */
    Barchart, /*!< barchart-type graphs */
    Boxplot, /*!< boxplot-type graphs */
    Filled, /*!< filled graphs  */
    Impulses, /*!< impulse-type graphs  */
    Geometric, /*!< geometric elements (e.g. rectangles, circles, lines, ...) */
    Annotation, /*!< annotation elements */
};

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
JKQTPLOTTER_LIB_EXPORT QString JKQTPUserActionMarkerType2String(JKQTPUserActionMarkerType act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPUserActionMarkerType2String() ) to JKQTPUserActionMarkerType
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPUserActionMarkerType2String(), JKQTPUserActionMarkerType
 */
JKQTPLOTTER_LIB_EXPORT JKQTPUserActionMarkerType String2JKQTPUserActionMarkerType(const QString &act);



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
JKQTPLOTTER_LIB_EXPORT QString JKQTPMouseDragActions2String(JKQTPMouseDragActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseDragActions2String() ) to JKQTPMouseDragActions
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPMouseDragActions2String(), JKQTPMouseDragActions
 */
JKQTPLOTTER_LIB_EXPORT JKQTPMouseDragActions String2JKQTPMouseDragActions(const QString &button);

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
JKQTPLOTTER_LIB_EXPORT QString JKQTPMouseDoubleClickActions2String(JKQTPMouseDoubleClickActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseDoubleClickActions2String() ) to JKQTPMouseDoubleClickActions
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPMouseDoubleClickActions2String(), JKQTPMouseDoubleClickActions
 */
JKQTPLOTTER_LIB_EXPORT JKQTPMouseDoubleClickActions String2JKQTPMouseDoubleClickActions(const QString &act);

/** \brief actions that can be bound to a mouse wheel event
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPMouseWheelActions {
    jkqtpmwaZoomByWheel=0, /*!< \brief use the mouse-wheel for zooming */
    jkqtpmwaPanByWheel, /*!< \brief use the mouse-wheel for panning the plot */
    jkqtpmwaZoomByWheelAndTrackpadPan, /*!< \brief use the mouse-wheel for zooming. In addition, this tries to recognize track-pad pan gestures and applies them. \note This is needed, because Qt converts track-pad zoom AND pan gestures to wheelEvents, but does not provide the source. Therefore a heuristics is required to interpret both! */
};

/** \brief convert a JKQTPMouseWheelActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplottersupprt
 *
 *  \see String2JKQTPMouseWheelActions(), JKQTPMouseWheelActions
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPMouseWheelActions2String(JKQTPMouseWheelActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseWheelActions2String() ) to JKQTPMouseWheelActions
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPMouseWheelActions2String(), JKQTPMouseWheelActions
 */
JKQTPLOTTER_LIB_EXPORT JKQTPMouseWheelActions String2JKQTPMouseWheelActions(const QString &act);

/** \brief Availble action this JKQtPlotter can perform when mouse events occur.
 *         This allows you to e.g. draw rectangles or lines over the plot and receive a signal, when the drawing finishes
 * \ingroup jkqtpplottersupprt

 */
enum JKQTPMouseMoveActions {
    jkqtpmmaToolTipForClosestDataPoint /*!< \brief shows a tooltip with data of the closest data-point in the plot \image html tooltiptool.gif */
};


/** \brief convert a JKQTPMouseDragActions to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplottersupprt
 *
 *  \see String2JKQTPMouseDragActions(), JKQTPMouseDragActions
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPMouseMoveActions2String(JKQTPMouseMoveActions act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPMouseDragActions2String() ) to JKQTPMouseDragActions
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPMouseDragActions2String(), JKQTPMouseDragActions
 */
JKQTPLOTTER_LIB_EXPORT JKQTPMouseMoveActions String2JKQTPMouseMoveActions(const QString &button);

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
JKQTPLOTTER_LIB_EXPORT QString JKQTPContextMenuModes2String(JKQTPContextMenuModes act);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPContextMenuModes2String() ) to JKQTPContextMenuModes
 *  \ingroup jkqtpplottersupprt
 *
 *  \see JKQTPContextMenuModes2String(), JKQTPContextMenuModes
 */
JKQTPLOTTER_LIB_EXPORT JKQTPContextMenuModes String2JKQTPContextMenuModes(const QString &act);

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


/** \brief data structure for storage of assigned JKQTPMouseMoveActions \see JKQTPMouseMoveActionsHashMapIterator
 *  \ingroup jkqtpplottersupprt */
typedef QHash<Qt::KeyboardModifiers, JKQTPMouseMoveActions> JKQTPMouseMoveActionsHashMap;
/** \brief iterator for JKQTPMouseMoveActionsHashMap \see JKQTPMouseMoveActionsHashMap
 *  \ingroup jkqtpplottersupprt */
typedef JKQTPMouseMoveActionsHashMap::const_iterator JKQTPMouseMoveActionsHashMapIterator;

/** \brief Specifies how a fill-color is derived from a given color
 * \ingroup jkqtpplotter_styling_classes

 */
struct JKQTPLOTTER_LIB_EXPORT JKQTPColorDerivationMode {
public:
    enum PredefinedModes {
        JKQTPFFCMFullyTransparentColor, /*!< \brief fully transparent color */
        JKQTPFFCMNoColor=JKQTPFFCMFullyTransparentColor, /*!< \brief fully transparent color */
        JKQTPFFCMBlack, /*!< \brief fill with black */
        JKQTPFFCMGrey75, /*!< \brief fill with 75% grey */
        JKQTPFFCMGrey50, /*!< \brief fill with 50% grey */
        JKQTPFFCMGrey25, /*!< \brief fill with 25% grey */
        JKQTPFFCMWhite, /*!< \brief fill with white */
        JKQTPFFCMBlackTransparent, /*!< \brief fill with black, slightly transparent */
        JKQTPFFCMGrey75Transparent, /*!< \brief fill with 75% grey, slightly transparent */
        JKQTPFFCMGrey50Transparent, /*!< \brief fill with 50% grey, slightly transparent */
        JKQTPFFCMGrey25Transparent, /*!< \brief fill with 25% grey, slightly transparent */
        JKQTPFFCMWhiteTransparent, /*!< \brief fill with white, slightly transparent */
        JKQTPFFCMSameColor, /*!< \brief fill with the same color */
        JKQTPFFCMSameNonTransparentColor, /*!< \brief fill with the same color, but remove any transparency (if any) */
        JKQTPFFCMSameMoreTransparentColor, /*!< \brief fill with the a partly transparent color */
        JKQTPFFCMSameEvenMoreTransparentColor, /*!< \brief fill with the a more transparent color than JKQTPFFCMMoreTransparentColor */
        JKQTPFFCMSameLessTransparentColor, /*!< \brief fill with the a partly transparent color */
        JKQTPFFCMSameEvenLessTransparentColor, /*!< \brief fill with the a more transparent color than JKQTPFFCMLessTransparentColor*/
        JKQTPFFCMInvertedColor, /*!< \brief fill with the inverted color */
        JKQTPFFCMInvertedTransparentColor, /*!< \brief fill with the inverted color, slightly transparent */
        JKQTPFFCMInvertedNonTransparentColor, /*!< \brief fill with the inverted color, non-transparent (even if the basecolor had some transparency) */
        JKQTPFFCMLighterColor, /*!< \brief fill with the a lighter color */
        JKQTPFFCMEvenLighterColor, /*!< \brief fill with the an even lighter color  than JKQTPFFCMLighterColor */
        JKQTPFFCMDarkerColor, /*!< \brief fill with the a darker color */
        JKQTPFFCMEvenDarkerColor, /*!< \brief fill with the an even darker color than JKQTPFFCMDarkerColor */
        JKQTPFFCMLighterAndTransparentColor, /*!< \brief fill with the a lighter color, that is in addition a bit transparent */
        JKQTPFFCMEvenLighterAndTransparentColor, /*!< \brief fill with the an even lighter color  than JKQTPFFCMLighterColor, that is in addition a bit transparent */
        JKQTPFFCMDarkerAndTransparentColor, /*!< \brief fill with the a darker color, that is in addition a bit transparent */
        JKQTPFFCMEvenDarkerAndTransparentColor, /*!< \brief fill with the an even darker color than JKQTPFFCMDarkerColor, that is in addition a bit transparent */
        JKQTPFFCMLighterAndNonTransparentColor, /*!< \brief fill with the a lighter color, that is non-transparent (even if the basecolor had some transparency)  */
        JKQTPFFCMEvenLighterAndNonTransparentColor, /*!< \brief fill with the an even lighter color  than JKQTPFFCMLighterColor, that is non-transparent (even if the basecolor had some transparency)  */
        JKQTPFFCMDarkerAndNonTransparentColor, /*!< \brief fill with the a darker color, that is non-transparent (even if the basecolor had some transparency)  */
        JKQTPFFCMEvenDarkerAndNonTransparentColor, /*!< \brief fill with the an even darker color than JKQTPFFCMDarkerColor, that is non-transparent (even if the basecolor had some transparency)  */

        JKQTPFFCMmax=JKQTPFFCMEvenDarkerAndNonTransparentColor
    };
    /** \brief for compatibility: use one of several predefined settings to iitialize object */
    JKQTPColorDerivationMode(PredefinedModes mode=JKQTPFFCMSameColor);
    /** \brief initialize object from a string (calls fromString() ) */
    JKQTPColorDerivationMode(const QString& mode);

    enum ColorChangeMode {
        SameColor,
        ReplaceColorAndTransparency,
        ReplaceColorNotTransparency,
        DarkerColor,
        LighterColor,
        InvertColor
    };
    /** \brief specifies how the color should change */
    ColorChangeMode colorModification;
    /** \brief replacement color when colorModification==ColorChangeMode::ReplaceColor */
    QColor targetColor;
    /** \brief strength of colorModification-settings, such as ColorChangeMode::DarkerColor or ColorChangeMode::LighterColor */
    float colorModificationStrength;

    enum TransparencyChangeMode {
        SameTransparency,
        ReplaceTransparency,
        MoreTransparent,
        LessTransparent
    };

    /** \brief specifies how the transparency of the color chould change */
    TransparencyChangeMode transparencyModification;
    /** \brief target transparency (0:non-transparennt .. 1:fully-transparent, i.e. 1-alphaF!!!) for TransparencyChangeMode::ReplaceTransparency */
    float targetTransparency;
    /** \brief strength of transparencyModification-settings, such as TransparencyChangeMode::MoreTransparent or TransparencyChangeMode::LessTransparent */
    float transparencyModficationStrength;

    /** \brief convert object contents into a string */
    QString toString() const;
    /** \brief read values from a string */
    static JKQTPColorDerivationMode fromString(const QString& mode);
    /** \brief apply the modifications encoded in this object to a given color */
    QColor apply(const QColor& basecolor) const;

    /** \brief compares two objects for equality */
    bool operator==(const JKQTPColorDerivationMode& other) const;

    /** \brief determines whether the current object equals a default-initialized object with the given parameter, i.e. this is equivalent to calling \code operato==(JKQTPColorDerivationMode(other)) \endcode */
    bool operator==(const PredefinedModes& other) const;

};

/** \brief use a JKQTPColorDerivationMode to derive a color from \a col as specified
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \param mode the mode of how to modify the given color \a basecolor
 *  \param basecolor the color in which to base the derivation
 *
 *  \see JKQTPColorDerivationMode
 */
JKQTPLOTTER_LIB_EXPORT QColor JKQTPGetDerivedColor(JKQTPColorDerivationMode mode, const QColor& basecolor);

/** \brief construct a QColor, based on the given \a color, but with alpha set to the specified value \a alphaF
 *  \ingroup jkqtpplotter_styling_classes
 *  \see QColorWithAlpha()
 */
inline QColor QColorWithAlphaF(const QColor& color, qreal alphaF) {
    QColor c=color;
    c.setAlphaF(alphaF);
    return c;
}

/** \brief construct a QColor, based on the given \a color, but with alpha set to the specified value \a alpha
 *  \ingroup jkqtpplotter_styling_classes
 *  \see QColorWithAlphaF()
 */
inline QColor QColorWithAlpha(const QColor& color, int alpha) {
    QColor c=color;
    c.setAlpha(alpha);
    return c;
}

/** \brief convert a JKQTPColorDerivationMode to a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a>
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see String2JKQTPColorDerivationMode(), JKQTPColorDerivationMode
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPColorDerivationMode2String(JKQTPColorDerivationMode mode);
/** \brief convert a <a href="http://doc.qt.io/qt-5/qstring.html">QString</a> (created by JKQTPColorDerivationMode2String() ) to JKQTPColorDerivationMode
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see JKQTPColorDerivationMode2String(), JKQTPColorDerivationMode
 */
JKQTPLOTTER_LIB_EXPORT JKQTPColorDerivationMode String2JKQTPColorDerivationMode(const QString &mode);


/** \brief drawing flags for a coordinate axis
 * \ingroup jkqtpplottersupprt
 *
 * \note This enum provides the elements for \ref JKQTPCADrawMode, which is actually used
 *       throughout the code.
 *
 * \see JKQTPCADrawMode, JKQTPCoordinateAxisStyle
*/
enum JKQTPCADrawModeElements {
    JKQTPCADMLine=0x01, /*!< \brief draw axis as thick line  \image html axisstyle/JKQTPCADMLine.png */
    JKQTPCADMTicks=0x02, /*!< \brief draw axis ticks  \image html axisstyle/JKQTPCADMTicks.png */
    JKQTPCADMTickLabels=0x04, /*!< \brief draw axis tick labels  \image html axisstyle/JKQTPCADMTickLabels.png */
    JKQTPCADMAxisLabel=0x08, /*!< \brief draw axis axis Label  \image html axisstyle/JKQTPCADMAxisLabel.png */

    JKQTPCADMMaxArrow=0x0100, /*!< \brief an open arrow at the max-end of the axis  \image html axisstyle/JKQTPCADMMaxArrow.png */
    JKQTPCADMMaxFilledArrow=0x0200, /*!< \brief draw a filled arrow at the max-end of the axis  \image html axisstyle/JKQTPCADMMaxFilledArrow.png */
    JKQTPCADMMinArrow=0x1000, /*!< \brief an open arrow at the end of the axis at the min-end of the axis  \image html axisstyle/JKQTPCADMMinArrow.png */
    JKQTPCADMMinFilledArrow=0x2000, /*!< \brief draw a filled arrow at the min-end of the axis  \image html axisstyle/JKQTPCADMMinFilledArrow.png */

    JKQTPCADMLineTicksTickLabels=JKQTPCADMLine|JKQTPCADMTicks|JKQTPCADMTickLabels, /*!< \brief draw axis with ticks, line and tick labels  \image html axisstyle/JKQTPCADMLineTicksTickLabels.png */
    JKQTPCADMLineTicks=JKQTPCADMLine|JKQTPCADMTicks, /*!< \brief draw axis with ticks and line  \image html axisstyle/JKQTPCADMLineTicks.png */
    JKQTPCADMTicksTickLabelsAxisLabel=JKQTPCADMTicks|JKQTPCADMTickLabels|JKQTPCADMAxisLabel, /*!< \brief draw axis with ticks, tick labels and axisLabel  \image html axisstyle/JKQTPCADMTicksTickLabelsAxisLabel.png */
    JKQTPCADMTicksTickLabels=JKQTPCADMTicks|JKQTPCADMTickLabels, /*!< \brief draw axis with ticks and tick labels  \image html axisstyle/JKQTPCADMTicksTickLabels.png */
    JKQTPCADMTickLabelsAxisLabel=JKQTPCADMTickLabels|JKQTPCADMAxisLabel, /*!< \brief draw axis tick labels and axisLabel  \image html axisstyle/JKQTPCADMTickLabelsAxisLabel.png */
    JKQTPCADMnone=0x0000, /*!< \brief draw no axis  \image html axisstyle/JKQTPCADMnone.png */
    JKQTPCADMcomplete=JKQTPCADMLine|JKQTPCADMTicks|JKQTPCADMTickLabels|JKQTPCADMAxisLabel, /*!< \brief draw axis withline,  ticks, ticklabels and axis label  \image html axisstyle/JKQTPCADMcomplete.png */
    JKQTPCADMcompleteMaxArrow=JKQTPCADMcomplete|JKQTPCADMMaxFilledArrow, /*!< \brief draw axis withline,  ticks, ticklabels and axis label and an arrow pointing to the max-side of the axis \image html axisstyle/JKQTPCADMcompleteMaxArrow.png */
    JKQTPCADMcompleteMinMaxArrow=JKQTPCADMcomplete|JKQTPCADMMaxFilledArrow|JKQTPCADMMinFilledArrow, /*!< \brief draw axis withline,  ticks, ticklabels and axis label and arrows pointing to the min and max side of the axis  \image html axisstyle/JKQTPCADMcompleteMinMaxArrow.png */
};
/** \brief drawing mode for a coordinate axis
 * \ingroup jkqtplotter_basegraphserrors
 *
 * \qFlagsNote{JKQTPCADrawMode,JKQTPCADrawModeElements}
 *
 * \see JKQTPCADrawModeElements, JKQTPCoordinateAxisStyle
 */
Q_DECLARE_FLAGS(JKQTPCADrawMode, JKQTPCADrawModeElements)
Q_DECLARE_OPERATORS_FOR_FLAGS(JKQTPCADrawMode)




/** \brief converts a JKQTPCADrawMode variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPCADrawMode2String(JKQTPCADrawMode pos);

/** \brief converts a string into a JKQTPCADrawMode
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT JKQTPCADrawMode String2JKQTPCADrawMode(const QString& pos);


/** \brief display mode for the axis labels
 * \ingroup jkqtpplottersupprt */
enum JKQTPCALabelType {
    JKQTPCALTdefault=0, /*!< \brief simply print the numbers \image html axisstyle/JKQTPCALTdefault.png */
    JKQTPCALTscientific, /*!< \brief print the numbers in scientific notation, e.g. \c "1.23e-4" \image html axisstyle/JKQTPCALTscientific.png */
    JKQTPCALTexponentCharacter, /*!< \brief print the numbers and show a unit character, i.e. 5&mu; for \f$ 5\cdot 10^{-6} \f$ , \c 3k for \f$ 3\cdot 10^3 \f$ ... \image html axisstyle/JKQTPCALTexponentCharacter.png */
    JKQTPCALTexponent, /*!< \brief show numbers in exponential for, e.g. \f$ 3\cdot 10^5 \f$ ... \image html axisstyle/JKQTPCALTexponent.png */
    JKQTPCALTprintf, /*!< \brief generate axis label from an arbitrary "printf" formatting string (see e.g. https://en.wikipedia.org/wiki/Printf_format_string ). The first data parameter is the tick value as \c double an the second is tickUnitName as string. The following image shows an example for \c "y=%+.2f": \image html axisstyle/JKQTPCALTprintf.png */
    JKQTPCALTdate, /*!< \brief show numbers as dates \image html axisstyle/JKQTPCALTdate.png */
    JKQTPCALTtime, /*!< \brief show numbers as times \image html axisstyle/JKQTPCALTtime.png*/
    JKQTPCALTdatetime, /*!< \brief show numbers as times \image html axisstyle/JKQTPCALTdatetime.png */
    JKQTPCALTfrac, /*!< \brief show numbers as fraction, the number is first rounded to the given precision and then a fraction is calculated and displayed like \f$ \frac{1}{2} \f$ \image html axisstyle/JKQTPCALTfrac.png */
    JKQTPCALTslashfrac, /*!< \brief show numbers as fraction, the number is first rounded to the given precision and then a fraction is calculated and displayed like \c 1/2 \image html axisstyle/JKQTPCALTslashfrac.png */
    JKQTPCALTsfrac, /*!< \brief show numbers as fraction, the number is first rounded to the given precision and then a fraction is calculated and displayed using \c \\sfrac{1}{2} \image html axisstyle/JKQTPCALTsfrac.png */
    JKQTPCALTintfrac, /*!< \brief show numbers as integral+fraction, the number is first rounded to the given precision and then a fraction is calculated and displayed like \f$ -3\frac{1}{2} \f$ \image html axisstyle/JKQTPCALTintfrac.png */
    JKQTPCALTintslashfrac, /*!< \brief show numbers as integral+fraction, the number is first rounded to the given precision and then a fraction is calculated and displayed like \c 1/2 \image html axisstyle/JKQTPCALTintslashfrac.png */
    JKQTPCALTintsfrac, /*!< \brief show numbers as integral+fraction, the number is first rounded to the given precision and then a fraction is calculated and displayed using \c \\sfrac{1}{2}  \image html axisstyle/JKQTPCALTintsfrac.png */
#if defined(__cpp_lib_format) || DOXYGEN
    JKQTPCALTformat, /*!< \brief generate axis label from an arbitrary "std::format" formatting string (see e.g. https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification ). The first data parameter is the tick value as \c double an the second is tickUnitName as string. The following image shows an example for \c "\\texttt{{ y={:*^+8.1f}}}": \image html axisstyle/JKQTPCALTformat.png
                                 \b NOte: This option is only available for C++20 and above, use the CMake option \c JKQtPlotter_ENABLED_CXX20=ON if your compiler supports this. */
#endif
    JKQTPCALTcount,
    JKQTPCALTmax=JKQTPCALTcount-1
};



/** \brief mode of the axis ticks
 * \ingroup jkqtpplottersupprt */
enum JKQTPLabelTickMode {
    JKQTPLTMLinOrPower=0, /*!< \brief linear, or log, depending on whether the axis is log \image html axisstyle/JKQTPLTMLinOrPower.png */
    JKQTPLTMLin, /*!< \brief always linear (even for log-axes) \image html axisstyle/JKQTPLTMLin.png */
    JKQTPLTMPower, /*!< \brief powers (of the log-base) \image html axisstyle/JKQTPLTMPower.png */

    JKQTPLTMmax=JKQTPLTMPower
};

/** \brief converts a JKQTPLabelTickMode variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPLabelTickMode2String(JKQTPLabelTickMode pos);

/** \brief converts a string into a JKQTPLabelTickMode
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT JKQTPLabelTickMode String2JKQTPLabelTickMode(const QString& pos);


/** \brief converts a JKQTPCALabelType variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPCALabelType2String(JKQTPCALabelType pos);

/** \brief converts a string into a JKQTPCALabelType
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT JKQTPCALabelType String2JKQTPCALabelType(const QString& pos);

/** \brief position of the axis labels
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPLabelPosition {
    JKQTPLabelMin=0,            /*!< \brief the axis label is near the min value of the axis (left/bottom)
                                     \image html labelstyles/JKQTPLabelMin.png
                                     \image html labelstyles/JKQTPLabelMin_zeroaxis.png */
    JKQTPLabelMax,             /*!< \brief the axis label is near the max value of the axis (right/top)
                                     \image html labelstyles/JKQTPLabelMax.png
                                     \image html labelstyles/JKQTPLabelMax_zeroaxis.png */
    JKQTPLabelCenter,          /*!< \brief the label is at the center of the axis
                                     \image html labelstyles/JKQTPLabelCenter.png
                                     \image html labelstyles/JKQTPLabelCenter_zeroaxis.png */
    JKQTPLabelMinBesides,      /*!< \brief the axis label is near the min value of the axis (left/bottom), but besides the axis tip
                                     \image html labelstyles/JKQTPLabelMinBesides.png
                                     \image html labelstyles/JKQTPLabelMinBesides_zeroaxis.png */
    JKQTPLabelMaxBesides,      /*!< \brief the axis label is near the max value of the axis (right/top), but besides the axis tip
                                     \image html labelstyles/JKQTPLabelMaxBesides.png
                                     \image html labelstyles/JKQTPLabelMaxBesides_zeroaxis.png */
    JKQTPLabelPositionMax=JKQTPLabelMaxBesides
};


/** \brief converts a JKQTPLabelPosition variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPLabelPosition2String(JKQTPLabelPosition pos);

/** \brief converts a string into a JKQTPLabelPosition
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT JKQTPLabelPosition String2JKQTPLabelPosition(const QString& pos);

/** \brief position of the key
 * \ingroup jkqtpplottersupprt
 *
 * \see JKQTPKeyPosition, JKQTPKeyStyle, JKQTPKeyBase, JKQTPGetTypicalKeyPositions()
 */
enum JKQTPKeyPositions {
    JKQTPKeyInside       =0x0001,      /*!< \brief key is drawn inside the plot rectangle */
    JKQTPKeyOutsideTop   =0x0010,      /*!< \brief key is drawn outside and above of the plot rectangle */
    JKQTPKeyOutsideBottom=0x0020,      /*!< \brief key is drawn outside and below of the plot rectangle */
    JKQTPKeyOutsideLeft  =0x0040,      /*!< \brief key is drawn outside and on the left of the plot rectangle */
    JKQTPKeyOutsideRight =0x0080,      /*!< \brief key is drawn outside and on the rightof  the plot rectangle */
    JKQTPKeyLeft         =0x0100,      /*!< \brief key is drawn to the left */
    JKQTPKeyHCenter      =0x0200,      /*!< \brief key is drawn to the horizontal center */
    JKQTPKeyRight        =0x0400,      /*!< \brief key is drawn to the right */
    JKQTPKeyTop          =0x1000,      /*!< \brief key is drawn to the top */
    JKQTPKeyVCenter      =0x2000,      /*!< \brief key is drawn to the vertical center */
    JKQTPKeyBottom       =0x4000,      /*!< \brief key is drawn to the bottom */


    JKQTPKeyOutsideTopRight=JKQTPKeyOutsideTop|JKQTPKeyBottom|JKQTPKeyRight,            /*!< \brief the key is positioned above the graph, on the right
                                                                                             \image html keypositions/JKQTPKeyOutsideTopRight.png */
    JKQTPKeyOutsideTopLeft=JKQTPKeyOutsideTop|JKQTPKeyBottom|JKQTPKeyLeft,             /*!< \brief the key is positioned above the graph, on the left
                                                                                             \image html keypositions/JKQTPKeyOutsideTopLeft.png */
    JKQTPKeyOutsideTopCenter=JKQTPKeyOutsideTop|JKQTPKeyBottom|JKQTPKeyHCenter,             /*!< \brief the key is positioned above the graph, in the center
                                                                                             \image html keypositions/JKQTPKeyOutsideTopCenter.png */
    JKQTPKeyOutsideBottomLeft=JKQTPKeyOutsideBottom|JKQTPKeyTop|JKQTPKeyLeft,            /*!< \brief the key is positioned below the graph, on the left
                                                                                             \image html keypositions/JKQTPKeyOutsideBottomLeft.png */
    JKQTPKeyOutsideBottomRight=JKQTPKeyOutsideBottom|JKQTPKeyTop|JKQTPKeyRight,            /*!< \brief the key is positioned below the graph, on the right
                                                                                             \image html keypositions/JKQTPKeyOutsideBottomRight.png */
    JKQTPKeyOutsideBottomCenter=JKQTPKeyOutsideBottom|JKQTPKeyTop|JKQTPKeyHCenter,            /*!< \brief the key is positioned below the graph, in the center
                                                                                             \image html keypositions/JKQTPKeyOutsideBottomCenter.png */
    JKQTPKeyOutsideLeftTop=JKQTPKeyOutsideLeft|JKQTPKeyTop|JKQTPKeyRight,              /*!< \brief the key is positioned on the left side of the graph, towards the top
                                                                                             \image html keypositions/JKQTPKeyOutsideLeftTop.png */
    JKQTPKeyOutsideLeftCenter=JKQTPKeyOutsideLeft|JKQTPKeyVCenter|JKQTPKeyRight,           /*!< \brief the key is positioned on the left side of the graph, in the center
                                                                                             \image html keypositions/JKQTPKeyOutsideLeftCenter.png */
    JKQTPKeyOutsideLeftBottom=JKQTPKeyOutsideLeft|JKQTPKeyBottom|JKQTPKeyRight,           /*!< \brief the key is positioned on the left side of the graph, towards the Bottom
                                                                                             \image html keypositions/JKQTPKeyOutsideLeftBottom.png */
    JKQTPKeyOutsideRightBottom=JKQTPKeyOutsideRight|JKQTPKeyBottom|JKQTPKeyLeft,            /*!< \brief the key is positioned on the right side of the graph, towards the bottom
                                                                                             \image html keypositions/JKQTPKeyOutsideRightBottom.png */
    JKQTPKeyOutsideRightCenter=JKQTPKeyOutsideRight|JKQTPKeyVCenter|JKQTPKeyLeft,            /*!< \brief the key is positioned on the right side of the graph, in the center
                                                                                             \image html keypositions/JKQTPKeyOutsideRightCenter.png */
    JKQTPKeyOutsideRightTop=JKQTPKeyOutsideRight|JKQTPKeyTop|JKQTPKeyLeft,             /*!< \brief the key is positioned on the left side of the graph, towards the top
                                                                                             \image html keypositions/JKQTPKeyOutsideRightTop.png */
    JKQTPKeyInsideTopRight=JKQTPKeyInside|JKQTPKeyTop|JKQTPKeyRight,             /*!< \brief the key is positioned inside on the top-right
                                                                                             \image html keypositions/JKQTPKeyInsideTopRight.png */
    JKQTPKeyInsideTopLeft=JKQTPKeyInside|JKQTPKeyTop|JKQTPKeyLeft,             /*!< \brief the key is positioned inside on the top-left
                                                                                             \image html keypositions/JKQTPKeyInsideTopLeft.png */
    JKQTPKeyInsideBottomLeft=JKQTPKeyInside|JKQTPKeyBottom|JKQTPKeyLeft,             /*!< \brief the key is positioned inside on the bottom-left
                                                                                             \image html keypositions/JKQTPKeyInsideBottomLeft.png */
    JKQTPKeyInsideBottomRight=JKQTPKeyInside|JKQTPKeyBottom|JKQTPKeyRight,             /*!< \brief the key is positioned inside on the bottom-right
                                                                                             \image html keypositions/JKQTPKeyInsideBottomRight.png */
    JKQTPKeyInsideTop=JKQTPKeyInside|JKQTPKeyTop|JKQTPKeyHCenter,             /*!< \brief the key is positioned inside on the top-center
                                                                                             \image html keypositions/JKQTPKeyInsideTop.png */
    JKQTPKeyInsideBottom=JKQTPKeyInside|JKQTPKeyBottom|JKQTPKeyHCenter,             /*!< \brief the key is positioned inside on the bottom-center
                                                                                             \image html keypositions/JKQTPKeyInsideBottom.png */
    JKQTPKeyInsideLeft=JKQTPKeyInside|JKQTPKeyVCenter|JKQTPKeyLeft,             /*!< \brief the key is positioned inside on the center-left
                                                                                             \image html keypositions/JKQTPKeyInsideLeft.png */
    JKQTPKeyInsideRight=JKQTPKeyInside|JKQTPKeyVCenter|JKQTPKeyRight,             /*!< \brief the key is positioned inside on the center-right
                                                                                             \image html keypositions/JKQTPKeyInsideRight.png */
    JKQTPKeyDefaultPosition=JKQTPKeyInsideTopRight

};
/** \brief position of the key
 * \ingroup jkqtpplottersupprt
 *
 * \qFlagsNote{JKQTPKeyPosition,JKQTPKeyPositions}
 *
 * \see JKQTPKeyPositions, JKQTPKeyStyle, JKQTPKeyBase, JKQTPGetTypicalKeyPositions()
 */
Q_DECLARE_FLAGS(JKQTPKeyPosition, JKQTPKeyPositions)
Q_DECLARE_OPERATORS_FOR_FLAGS(JKQTPKeyPosition)

/** \brief returns a list of typical positions of the main key
 * \ingroup jkqtpplottersupprt
 *
 * \qFlagsNote{JKQTPKeyPosition,JKQTPKeyPositions}
 *
 * \see JKQTPKeyPositions, JKQTPKeyStyle, JKQTPKeyBase
 */
JKQTPLOTTER_LIB_EXPORT QList<JKQTPKeyPosition> JKQTPGetTypicalKeyPositions();

/** \brief converts a JKQTPLabelPosition variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPKeyPosition2String(JKQTPKeyPosition pos);

/** \brief converts a string into a JKQTPLabelPosition
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT JKQTPKeyPosition String2JKQTPKeyPosition(const QString& pos);

/** \brief layout of the key
 * \ingroup jkqtpplottersupprt
 */
enum JKQTPKeyLayout {
    JKQTPKeyLayoutOneColumn=0,            /*!< \brief the key consists of one column. This may overflow the available space, use JKQTPKeyLayoutMultiColumn or JKQTPKeyLayoutMultiRow if you have many entries!
                                               \image html keylayouts/JKQTPKeyLayout_one_column.png
                                               \image html keylayouts/JKQTPKeyLayout_outsidebottom_one_column.png
                                               \image html keylayouts/JKQTPKeyLayout_outsideright_one_column.png */
    JKQTPKeyLayoutOneRow,                 /*!< \brief the key consists of one row. This may overflow the available space, use JKQTPKeyLayoutMultiColumn or JKQTPKeyLayoutMultiRow if you have many entries!
                                               \image html keylayouts/JKQTPKeyLayout_one_row.png
                                               \image html keylayouts/JKQTPKeyLayout_outsidebottom_one_row.png
                                               \image html keylayouts/JKQTPKeyLayout_outsideright_one_row.png */
    JKQTPKeyLayoutMultiColumn,            /*!< \brief the key items are distributed over multiple columns that fill the available space, (top->bottom, top->bottom, ...)
                                               \image html keylayouts/JKQTPKeyLayout_multi_column.png
                                               \image html keylayouts/JKQTPKeyLayout_outsidebottom_multi_column.png
                                               \image html keylayouts/JKQTPKeyLayout_outsideright_multi_column.png */
    JKQTPKeyLayoutMultiRow,               /*!< \brief the key items are distributed over multiple rows that fill the available space (left->right,left->right,...)
                                               \image html keylayouts/JKQTPKeyLayout_multi_row.png
                                               \image html keylayouts/JKQTPKeyLayout_outsidebottom_multi_row.png
                                               \image html keylayouts/JKQTPKeyLayout_outsideright_multi_row.png */

    JKQTPKeyLayoutMax=JKQTPKeyLayoutMultiRow
};


/** \brief converts a JKQTPKeyLayout variable into a human-readable string
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPKeyLayout2String(JKQTPKeyLayout pos);

/** \brief converts a String into a JKQTPKeyLayout
 * \ingroup jkqtpplottersupprt
 */
JKQTPLOTTER_LIB_EXPORT JKQTPKeyLayout String2JKQTPKeyLayout(const QString& pos);



/** \brief used to represent the position of other graphs in \ref jkqtplotter_base_saveprint
 * \ingroup jkqtpplottersupprt
 * \internal
 */
struct JKQTPLOTTER_LIB_EXPORT JKQTPGridPrintingItem {
    /** \brief logic x-position of a graph, relative to the current JKQtPlotter */
    size_t x;
    /** \brief logic y-position of a graph, relative to the current JKQtPlotter */
    size_t y;
    /** \brief JKQTBasePlotter */
    JKQTBasePlotter* plotter;
};




/** \brief specifies the plot styles for the error information, e.g. error bars, boxes, lines ...
 * \ingroup jkqtplotter_basegraphserrors
 *
 * \note This enum provides the elements for \ref JKQTPErrorPlotstyle, which is actually used
 *       throughout the code.
 *
 * \see JKQTPErrorPlotstyle, JKQTPXGraphErrorData, JKQTPYGraphErrorData
 */
enum JKQTPErrorPlotstyleElements {
    JKQTPNoError=0x00,                   /*!< \brief don't show error information \image html errorindicators/JKQTPNoError.png */
    JKQTPErrorSimpleBars=0x01,          /*!< \brief simplified error bars for each data point \image html errorindicators/JKQTPErrorSimpleBars.png */
    JKQTPErrorLines=0x04,               /*!< \brief a second and third graph line above and below the actual data which indicates the error value \image html errorindicators/JKQTPErrorLines.png */
    JKQTPErrorPolygons=0x08,            /*!< \brief line error lines, but with filled range in between \image html errorindicators/JKQTPErrorPolygons.png */
    JKQTPErrorEllipses=0x10,            /*!< \brief an ellipse spanned by the errors \image html errorindicators/JKQTPErrorEllipses.png */
    JKQTPErrorBoxes=0x20,               /*!< \brief a box spanned by the errors \image html errorindicators/JKQTPErrorBoxes.png */

    JKQTPErrorDirectionBoth=0x000,     /*!< \brief do not draw half error-bars, but in both directions (default) \image html errorindicators/JKQTPErrorBars.png */
    JKQTPErrorDirectionOutwards=0x100, /*!< \brief used to specify the directon of half error bars: outwards pointing \image html errorindicators/JKQTPErrorHalfBarsOutwards.png */
    JKQTPErrorDirectionInwards=0x200,  /*!< \brief used to specify the directon of half error bars: inwards pointing \image html errorindicators/JKQTPErrorHalfBarsInwards.png */
    JKQTPErrorDirectionAbove=04200,    /*!< \brief used to specify the directon of half error bars: above pointing \image html errorindicators/JKQTPErrorHalfBarsAbove.png */
    JKQTPErrorDirectionBelow=0x800,     /*!< \brief used to specify the directon of half error bars: below pointing \image html errorindicators/JKQTPErrorHalfBarsBelow.png */
    JKQTPErrorIndicatorNone=0x0000,     /*!< \brief used to specify that error bars shall be un-decorated i.e. "simple error-bars" */
    JKQTPErrorIndicatorBar=0x1000,      /*!< \brief used to specify that error bars shall be decorated by bars \image html errorindicators/JKQTPErrorIndicatorBar.png */
    JKQTPErrorIndicatorArrows=0x2000,     /*!< \brief used to specify that error bars shall be decorated by arrows \image html errorindicators/JKQTPErrorIndicatorArrows.png */
    JKQTPErrorIndicatorInwardArrows=0x4000,     /*!< \brief used to specify that error bars shall be decorated by arrows \image html errorindicators/JKQTPErrorIndicatorInwardArrows.png */

    JKQTPErrorArrows=JKQTPErrorSimpleBars|JKQTPErrorIndicatorArrows,          /*!< \brief error bars decorated with arrows for each data point \image html errorindicators/JKQTPErrorArrows.png */
    JKQTPErrorInwardArrows=JKQTPErrorSimpleBars|JKQTPErrorIndicatorInwardArrows,          /*!< \brief error bars decorated with inwards-pointing arrows for each data point \image html errorindicators/JKQTPErrorInwardArrows.png */
    JKQTPErrorBars=JKQTPErrorSimpleBars|JKQTPErrorIndicatorBar,          /*!< \brief error bars for each data point \image html errorindicators/JKQTPErrorBars.png */
    JKQTPErrorSimpleBarsPolygons=JKQTPErrorSimpleBars|JKQTPErrorPolygons,  /*!< \brief simplified error barsand polygons  for each data point \image html errorindicators/JKQTPErrorSimpleBarsPolygons.png */
    JKQTPErrorSimpleBarsLines=JKQTPErrorSimpleBars|JKQTPErrorLines,     /*!< \brief simplified error bars and line for each data point \image html errorindicators/JKQTPErrorSimpleBarsLines.png */
    JKQTPErrorHalfBarsOutwards=JKQTPErrorBars|JKQTPErrorDirectionOutwards,   /*!< \brief half error bars for each data point, pointing outwards \image html errorindicators/JKQTPErrorHalfBarsOutwards.png */
    JKQTPErrorHalfBarsInwards=JKQTPErrorBars|JKQTPErrorDirectionInwards,    /*!< \brief half error bars for each data point, pointing inwards \image html errorindicators/JKQTPErrorHalfBarsInwards.png */
    JKQTPErrorHalfBarsAbove=JKQTPErrorBars|JKQTPErrorDirectionAbove,      /*!< \brief half error bars for each data point, pointing up \image html errorindicators/JKQTPErrorHalfBarsAbove.png */
    JKQTPErrorHalfBarsBelow=JKQTPErrorBars|JKQTPErrorDirectionBelow,      /*!< \brief half error bars for each data point, pointing down \image html errorindicators/JKQTPErrorHalfBarsBelow.png */
    JKQTPErrorBarsLines=JKQTPErrorBars|JKQTPErrorLines,           /*!< \brief error bars and lines for each data point \image html errorindicators/JKQTPErrorBarsLines.png */
    JKQTPErrorBarsPolygons=JKQTPErrorBars|JKQTPErrorPolygons,        /*!< \brief error bars and polygons for each data point \image html errorindicators/JKQTPErrorBarsPolygons.png */
};
/** \brief specifies the plot styles for the error information, e.g. error bars, boxes, lines ...
 * \ingroup jkqtplotter_basegraphserrors
 *
 * \qFlagsNote{JKQTPErrorPlotstyle,JKQTPErrorPlotstyleElements}
 *
 * \see JKQTPErrorPlotstyleElements, JKQTPXGraphErrorData, JKQTPYGraphErrorData
 */
Q_DECLARE_FLAGS(JKQTPErrorPlotstyle, JKQTPErrorPlotstyleElements)
Q_DECLARE_OPERATORS_FOR_FLAGS(JKQTPErrorPlotstyle)


/** \brief converts a JKQTPErrorPlotstyle variable into a human-readable string
 * \ingroup jkqtplotter_basegraphserrors
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPErrorPlotstyle2String(JKQTPErrorPlotstyle pos);

/** \brief converts a String into a JKQTPErrorPlotstyle
 * \ingroup jkqtplotter_basegraphserrors
 */
JKQTPLOTTER_LIB_EXPORT JKQTPErrorPlotstyle String2JKQTPErrorPlotstyle(const QString& pos);




/** \brief plot styles for the error information
 * \ingroup jkqtplotter_linesymbolgraphs_line
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
 * \ingroup jkqtplotter_linesymbolgraphs_line
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPSpecialLineType2String(JKQTPSpecialLineType pos);

/** \brief converts a String into a JKQTPSpecialLineType
 * \ingroup jkqtplotter_linesymbolgraphs_line
 */
JKQTPLOTTER_LIB_EXPORT JKQTPSpecialLineType String2JKQTPSpecialLineType(const QString& pos);




#endif // JKQTPTOOLS_H_INCLUDED
