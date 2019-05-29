/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)



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

#ifndef JKQTPLOTTERSTYLE_H
#define JKQTPLOTTERSTYLE_H

#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtp_imexport.h"
#include <QColor>
#include <QPainter>
#include <QString>
#include <QHash>
#include <QPair>
#include <QMouseEvent>
#include <QSettings>


/** \brief Support Class for JKQTPlotter, which summarizes all proeprties that define the visual styling of a JKQTPlotter
 *  \ingroup jkqtpplotter_styling
 *
 *  \see JKQTPlotter, \ref jkqtpplotter_styling
 */
class JKQTP_LIB_EXPORT JKQTPlotterStyle {
    Q_GADGET
public:
    /** \brief initialize with default values */
    JKQTPlotterStyle();



    /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
     *
     *  \param settings QSettings-object to read from
     *  \param group Group in the QSettings-object to read from
     *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
     *                      By default, this is a default-constructed object
     */
    void loadSettings(const QSettings &settings, const QString& group=QString("plots/"), const JKQTPlotterStyle &defaultStyle=JKQTPlotterStyle());

    /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
     *
     *  \param settings QSettings-object to save to
     *  \param group Group in the QSettings-object to save to
     */
    void saveSettings(QSettings& settings, const QString& group=QString("plots/")) const;





    /** \brief maximum distance at which a tooltip for a point is still displayed, when the user action jkqtpmdaToolTipForClosestDataPoint is executed */
    int maxTooltipDistance;
    /** \brief sensitivity (i.e. max. distance to see as one point) for user actions */
    int userActionCatchSensitivity;

    /** \brief width/height of the icons in the plotter toolbar in pixel */
    int toolbarIconSize;
    /** \brief pen for user actions that overlay the plot (typically semi-transparent, like e.g. zoom-rectangles)  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    QPen userActionOverlayPen;
    /** \brief brush for user actions (typically semi-transparent, like e.g. zoom-rectangles)  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    QBrush userActionOverlayBrush;

    /** \brief pen for user actions that cover parts of the plot (typically opaque, like e.g. tooltips)  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    QPen userActionOpaquePen;
    /** \brief brush for user actions that cover parts of the plot (typically opaque, like e.g. tooltips)  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    QBrush userActionOpaqueBrush;

    /** \brief pen for location markers of user actions on the plot  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    QPen userActionMarkerPen;
    /** \brief brush for location markers of user actions on the plot  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    QBrush userActionMarkerBrush;
    /** \brief type of the location markers of user actions on the plot  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    JKQTPUserActionMarkerType userActionMarkerType;

    /** \brief font name to use for text of user actions  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    QString userActionFontName;
    /** \brief font size [pt] to use for text of user actions  \see \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
    double userActionFontSize;

    /** \brief size (diameter) of markers drawn by user actions */
    int userActionMarkerDiameter;

    /** \brief this string is used to generate the position output above the graph (\c %1 is replaced by the x-position, \c %2 by the y-position)
     *
     * By default simply <code>"(%1, %2)</code> is used to format this display (e.g. <code>(1.35, -4.56)</code>).
     *
     * \image html jkqtplotter_mousepositiondisplay.png
     *
     * \see mousePositionTemplate, \ref JKQTPlotterUserInteraction */
    QString mousePositionTemplate;

    /** \brief specifies whether to display the current position of the mouse in the top border of the plot (this may automatically extent the
      *        top border, so the position fits in. The default widget font is used for the output.
      *
      * \image html jkqtplotter_mousepositiondisplay.png
      *
      * \see mousePositionTemplate, \ref JKQTPlotterUserInteraction, \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEMOVE
      */
    bool displayMousePosition;

    /** \brief specifies whether to display a toolbar with buttons when the mouse is in the upper border of the plot
      *
      * \image html jkqtplotter_toolbar_alwayson.png
      *
      * \see toolbarAlwaysOn, \ref JKQTPlotterUserInteraction
      */
    bool toolbarEnabled;
    /** \brief specifies whether the toolbar is always visible or only when the mouse moves to the upper left area
      *
      * If toolbarAlwaysOn is set to \c true:
      *
      * \image html jkqtplotter_toolbar_alwayson.png
      *
      * If toolbarAlwaysOn is set to \c false, the toolbar only appears when the mouse is in the right location:
      *
      * \image html jkqtplotter_toolbar_hidden.png "Hidden Toolbar"
      * \image html jkqtplotter_toolbar_shown.png "Shown Toolbar"
      *
      * \see toolbarEnabled, \ref JKQTPlotterUserInteraction
      */
    bool toolbarAlwaysOn;

    /** \brief if set (\c true ), JKQTPlotter uses QPalette::Window and other colors from QPalette as background color for the plot
     *
     * \note setting this (default: \c true ) will overwrite some settings from JKQTBasePlotterStyle! */
    bool usePaletteColors;

    /** \brief lists all availble mouse drag action modes */
    JKQTPMouseDragActionsHashMap registeredMouseDragActionModes;

    /** \brief lists all availble mouse wheel action modes */
    JKQTPMouseWheelActionsHashMap registeredMouseWheelActions;

    /** \brief action to perform on a double-click of the mouse buttons (depending on the button and the modifiers) */
    JKQTPMouseDoubleClickActionsHashMap registeredMouseDoubleClickActions;


};


/** \brief returns the system-wide default JKQTPlotterStyle
 *  \ingroup jkqtpplotter_styling
 *
 * \see JKQTPSetSystemDefaultStyle(), JKQTPGetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), \ref jkqtpplotter_styling
 */
JKQTP_LIB_EXPORT JKQTPlotterStyle& JKQTPGetSystemDefaultStyle();

/** \brief replaces the system-wide default JKQTPlotterStyle with the given \a newStyle
 *  \ingroup jkqtpplotter_styling
 *
 * \see JKQTPGetSystemDefaultStyle(), JKQTPGetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), \ref jkqtpplotter_styling
 */
JKQTP_LIB_EXPORT void JKQTPSetSystemDefaultStyle(JKQTPlotterStyle& newStyle);


#endif // JKQTPLOTTERSTYLE_H
