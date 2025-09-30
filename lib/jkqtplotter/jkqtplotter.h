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

#ifndef JKQTPLOTTER_H
#define JKQTPLOTTER_H


#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QColor>
#include <QMap>
#include <QVector>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QToolBar>
#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolBar>
#include <QPointer>
#include <QTimer>
#include <QHash>
#include <vector>
#include <cmath>
#include <iostream>
#include <atomic>

#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotterstyle.h"
#include "jkqtplotter/jkqtpbaseplotterstyle.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/gui/jkvanishqtoolbar.h"
#include <QKeyEvent>

/** \brief initialized Qt-ressources necessary for JKQTPlotter
 *  \ingroup jkqtpplottersupprt
*/
JKQTPLOTTER_LIB_EXPORT void initJKQTPlotterResources();


/** \brief plotter widget for scientific plots (uses JKQTBasePlotter to do the actual drawing)
 * \ingroup jkqtpplotterclasses
 *
 * This class is a QWidget-wrapper around JKQTBasePlotter. It uses the tools from JKQTBasePlotter
 * to display scientific plots. This class mostly adds the Widget for the output and adds different
 * types of user interactions.
 *
 * <b>Please have a look at the documentation of JKQTBasePlotter for details on the management of graphs
 *    and the formating/styling of the plot and graphs!</b>
 *
 * The rest of this documentation ins split into sections that each treat a special topic, as outlines below:
 *
 * \tableofcontents
 *
 *
 *
 *
 * \section  JKQTPLOTTER_BASICUSAGE Basic Usage of JKQTPlotter
 *
 * \copydetails jkqtplotter_general_usage_jkqtplotter
 *
 *
 *
 *
 * \section JKQTPLOTTER_PERFORMACEWHILESETUP Performance Considerations when Setting up Plots
 *
 * \copydetails jkqtplotter_general_usage_speedplotsetup
 *
 *
 *
 * \section  JKQTPLOTTER_SYNCMULTIPLOT Synchronizing Several Plots
 *
 * Often a single plot is not sufficient, but several plots need to be aligned with respect to each other:
 *
 * \image html multiplot.png
 *
 * In the Qt Window this is achieved by placing several JKQTPlotter objects into a <a href="http://doc.qt.io/qt-5/qgridlayout.html">QGridLayout</a>.
 * In order to support this alignment, also when exporting/printing a plot, and when the user interactions with the plot (e.g. zooming),
 * JKQTPlotter offers an API which allows to :
 * <ul>
 *   <li> <b>declare (grid-like) relations between plots for export/printing</b>
 *     \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentPos() and \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_SYNC</li>
 *   <li> <b>synchronize axes/plots</b>
 *     \see synchronizeXToMaster(), synchronizeYToMaster(), resetMasterSynchronization() and \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT</li>
 * </ul>
 *
 *
 * \note This API adopts and extends the possibilities of JKQTBasePlotter. Please see the documentation there for details:
 *       <ul>
 *         <li> \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
 *             <ul>
 *               <li> \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_SYNC</li>
 *               <li> \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT</li>
 *             </ul>
 *       </ul>
 *
 *
 *
 * \section JKQTPLOTTER_USERINTERACTION User-Interactions/GUI Features
 *
 * JKQTPlotter offers a lot of user-interaction features out of the box. These are detailed below.
 *
 * \subsection JKQTPLOTTER_CONTEXTMENU Context Menu of JKQTPlotter
 *
 * The class JKQTPlotter has a context menu that already offers a lot of functionality.
 *
 * \image html jkqtplotter_defaultcontextmenu.png
 *
 * \image html contextmenu_graphvisibility.gif
 *
 * It allows to:
 * <ul>
 *   <li> copy or save the data from the internal JKQTPDatastore</li>
 *   <li> copy or save the plot to an image file (PNG, PDF, ...), includes a softisticated export-preview dialog:
 *
 *     \image html jkqtplotter_exportpreviewdialog.png
 *   </li>
 *   <li> print the plot (includes a softisticated print-preview dialog):
 *
 *     \image html jkqtplotter_printpreview.png "Print Preview Dialog"
 *
 *     \image html jkqtplotter_printpreview_relsize_mullinewidth_mulfonts.png "Print Preview Dialog with modified (relative) size, line-width and font-size"
 *   </li>
 *   <li> open a dialog with a table of the plot data:
 *
 *     \image html jkqtplotter_datatabledialog.png "Data Table Dialog (with buttons to save or copy the data)"
 *   </li>
 *   <li> zoom into/out of the plot and determine an auto-zoom, which shows all of the plot data</li>
 *   <li> switch the visibility of the different graphs in the plot</li>
 * </ul>
 *
 * You can activate this menu, by setting \c setContextMenuMode(jkqtpcmmStandardContextMenu); and binding a mouse-action (typically the right-click)
 * to the context menu by calling \c deregisterMouseDragAction(Qt::RightButton,Qt::NoModifier) which deregisters any user actions from the rhs single
 * mouse click. In that case, the context menu is opened by default (see setContextMenuMode() to choose the mode of the context menu).
 *
 *
 * In addition JKQTPlotter provides several ways to customize this menu:
 *   - You can add additional actions to the JKQTPlotter, by calling `JKQTPlotter::addAction()` (i.e. using the default Qt mechanism)
 *     and/or by adding actions to the internal JKQTBasePlotter, using `JKQTBasePlotter::registerAdditionalAction()`
 *   - You can modify the menu, when it is displayed, by connecting a slot to the signal `JKQTPlotter::contextMenuOPened(x,y,menu)`:
 *     \code
 *       connect(plot, SIGNAL(contextMenuOpened(double, double, QMenu*)), this, SLOT(contextMenuOpened(double, double, QMenu*)));
 *
 *       // ...
 *
 *       void TestUserInteraction::contextMenuOpened(double x, double y, QMenu *contextMenu)
 *       {
 *           contextMenu->addSeparator();
 *           QAction* act=contextMenu->addMenu(QString("contextMenuOpened(x=%1, y=%2)").arg(x).arg(y))->addAction("user-added action");
 *           connect(act, &QAction::triggered, [x,y]() { QMessageBox::warning(nullptr, tr("Plot Context Menu"),
 *                                                                            tr("Context Menu was opened at x/y=%1/%2!").arg(x).arg(y),
 *                                                                            QMessageBox::Ok,
 *                                                                            QMessageBox::Ok); });
 *           labMouseAction->setText(QString("contextMenuOpened(x=%1, y=%2)").arg(x).arg(y));
 *       }
 *     \endcode
 * .
 *
 * \subsection JKQTPLOTTER_SPECIALCONTEXTMENU Special (User-Defined) Context Menu of JKQTPlotter
 *
 * In addition to the standard context menu, JKQTPlotter can also be configures to display a special, user-defined context menu.
 * To do so, call `\c setContextMenuMode(jkqtpcmmSpecialContextMenu) and set your menu, by calling \c setSpecialContextMenu()
 * You can also combine the special menu and the standard menu, by calling \c setContextMenuMode(jkqtpcmmStandardAndSpecialContextMenu) .
 *
 * You can also use the signal contextMenuOpened() to modify the special context menu, but note that alterations to this menu are permanently
 * stored in the special menu object. See contextMenuOpened() for a detailed discussion and ways to circumvent this!
 *
 *
 * \subsection JKQTPLOTTER_TOOLBAR Toolbar of JKQTPlotter
 *
 * In addition to the context-menu, a JKQtPlotter also also provides a toolbar at its top that offers
 * most of the functionality of the context menu. Usually this toolbar is enabled (see toolbarEnabled)
 * and is in a mode where it is hidden, until the mouse moves over an area at the top of the plot (see toolbarAlwaysOn):
 *
 * \image html jkqtplotter_toolbar_hidden.png "Hidden Toolbar"
 * \image html jkqtplotter_toolbar_shown.png "Shown Toolbar"
 *
 * \image html jkqtvanishtoolbar.gif
 *
 * If toolbarAlwaysOn is set to \c true (setToolbarAlwaysOn() ), the toolbar is always displayed:
 *
 * \image html jkqtplotter_toolbar_alwayson.png
 *
 * \see toolbarEnabled, toolbarAlwaysOn, \ref JKQTPlotterUserInteraction
 *
 *
 *
 * \subsection JKQTPLOTTER_ACTIONS QActions from a JKQTPlotter
 * Often you want to allow the suer to operate a plot from a user-defined QToolBar or a QMenu/QMenuBar in your
 * application (e.g. provide zooming commands ...). there are generally two ways to achieve this:
 * <ol>
 *   <li> Simply connect home-brewn QAction instances to the slots provided by JKQTPlotter and JKQTBasePlotter.
 *        This also allows you to connect different plot properties to edit widgets in your forms.
 *        \code
 *          // add a checkbox to show and hide the position display label in the plot
 *          chkPositionDisplay=new QCheckBox(tr("show mouse cursor position"), this);
 *          chkPositionDisplay->setChecked(plot->isMousePositionShown());
 *          connect(chkPositionDisplay, SIGNAL(toggled(bool)), plot, SLOT(setMousePositionShown(bool)));
 *        \endcode
 *   </li>
 *   <li> For several functions (especially those also present in JKQTPlotter's context-emun, you can also find
 *        readily available QAction instances. these are available from JKQTBasePlotter (e.g. by JKQTBasePlotter::getActionPrint() ...).
 *        From JKQTPlotter you therefor have to use: <code>getPlotter()->getActionPrint()</code>
 *        \code
 *          // add some of the default QActions from the JKQTPlotter to the window menu
 *          // Some of the are also available in the context menu and toolbar of the JKQTPlotter
 *          // others are not
 *          QMenu* menuPlot=menuBar()->addMenu("Plot-Menu");
 *          menuPlot->addAction(plot->getPlotter()->getActionPrint());
 *          QMenu* menuPlotS=menuPlot->addMenu("Save ...");
 *          menuPlotS->addAction(plot->getPlotter()->getActionSaveData());
 *          menuPlotS->addAction(plot->getPlotter()->getActionSavePDF()); // not available from JKQTPlotter by default
 *          menuPlotS->addAction(plot->getPlotter()->getActionSavePlot());
 *        \endcode
 *   </li>
 * </ol>
 * \see \ref JKQTPlotterUserInteraction
 *
 *
 *
 * \subsection JKQTPLOTTER_USERMOUSEINTERACTION Mouse-Interaction in JKQTPlotter
 *
 * This section summarizes all user-interaction functions in JKQTPlotter that somehow relate to the mouse.
 * These are:
 *   - \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG
 *   - \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSECLICK
 *   - \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEWHEEL
 *   - \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEMOVE
 * .
 *
 * \note Zooming and Paning interactions apply to both axes when they are performed with the mouse
 *       inside the plot. They are limited to one of the two axes, when the mouse hovers over that axis
 *       (e.g. when you zoom by mouse-wheel while the mouse pointer is below the plot, over the x-axis,
 *       only the x-axis is affected by the operation).
 *
 * \subsubsection JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG Actions When Dragging the Mouse
 * JKQTPlotter offers several methods that allow to customize, how it reacts to mouse actions:
 *   - registerMouseDragAction() tells JKQTPlotter to perform a certain action (selected from JKQTPlotter::MouseActionMode)
 *     when a specified mouse button is pushed while a specified (or no) keyborad modifiers (e.g. CTRL,ALT,SHIFT...) is pressed.
 *     By default JKQTPlotter calls these two registers in its constructors:
 *     \code
 *          registerMouseDragAction(Qt::LeftButton, Qt::NoModifier, JKQTPlotter::MouseActionMode::jkqtpmdaZoomByRectangle);
 *          registerMouseDragAction(Qt::LeftButton, Qt::ControlModifier, JKQTPlotter::MouseActionMode::jkqtpmdaPanPlotOnMove);
 *     \endcode
 *     Therefore by default you can draw a zoom rectangle with the left mouse button without modifiers
 *     and you can pan/drag the plot with the left mouse-button while keeping CTRL pressed.
 *   - deregisterMouseDragAction() deletes a previously defined user-interaction
 *   - clearAllRegisteredMouseDragActions() deletes all previously specified user-actions
 * .
 *
 * Pressing the \c ESC key will stop the current JKQTPlotter::MouseActionMode.
 *
 * If e.g. the mode JKQTPlotter::MouseActionMode::jkqtpmdaZoomByRectangle is selected, while you drag the mouse, the
 * zoom rectangle is drawn over the plot. You can modify the style of drawing using these functions:
 *   - setUserActionOverlayPen() sets the pen for (semi-transparent) overlay shapes, e.g. zoom rectangles
 *   - setUserActionOverlayBrush() sets the brush for (semi-transparent) overlay shapes, e.g. zoom rectangles
 *   - setUserActionOpaquePen() sets the pen for opaque overlay shapes, e.g. tool-tips
 *   - setUserActionOpaqueBrush() sets the brush for opaque overlay shapes, e.g. tool-tips
 *   - setUserActionMarkerPen() sets the pen for marker overlay shapes, e.g. with tool-tips
 *   - setUserActionMarkerBrush() sets the brush for marker overlay shapes, e.g. with tool-tips
 *   - setUserActionMarkerDiameter() sets the size of user-action markers
 *   - setUserActionMarkerType() sets the type of user-action markers
 * .
 *
 * \image html zoomin_mouse_contextmenu.gif "Zooming with the mouse"
 *
 * \image html draw_rectangle.gif "Draw Rectangle User-Action"
 *
 * \image html drag_viewport.gif "Drag the Plot Viewport"
 *
 * \image html rulertool.gif "Measurement Ruler Tool"
 *
 * \image html tooltiptool.gif "Data Point Tooltip Tool"
 *
 *
 * \subsubsection JKQTPLOTTER_USERMOUSEINTERACTION_MOUSECLICK Actions After (Double-)Clicks on the Mouse Buttons
 * The right mouse button has a special role: If it is single-clicked and no JKQTPlotter::MouseActionMode is specified
 * for the vent, it opens the context menu, unless you call \c setContextMenuMode(jkqtpcmmNoContextMenu) .
 * You can also use setContextMenuMode() to specify which type of context menu is shown. See JKQTPContextMenuModes
 * for details on the available modes.
 *
 * For any mouse-click, one of the following signals is emitted:
 *   - plotMouseClicked() for any single-click (during the pressDown-Event!)
 *   - plotMouseDoubleClicked() for any double-click
 * .
 *
 * The reaction to double-clicks of the mouse buttons can be specified separately. The possible
 * actions are listed in JKQTPMouseDoubleClickActions. You can bind one of these actions
 * to a specific click with these functions:
 *   - registerMouseDoubleClickAction() tells JKQTPlotter to perform a certain action (selected from JKQTPMouseDoubleClickActions)
 *     when a specified mouse button is pushed while a specified (or no) keyborad modifiers (e.g. CTRL,ALT,SHIFT...) is pressed.
 *     By default JKQTPlotter calls this in its constructors:
 *     \code
 *          registerMouseDoubleClickAction(Qt::LeftButton, Qt::NoModifier, JKQTPMouseDoubleClickActions::jkqtpdcaClickMovesViewport);
 *     \endcode
 *     Therefore by default you can pan the plot/move the viewport by double clicking a location
 *   - deregisterMouseDoubleClickAction() deletes a previously defined user-interaction
 *   - clearAllRegisteredMouseDoubleClickAction() deletes all previously specified user-actions
 * .
 * The button to react to is specified as a parameter.
 *
 * \image html contextmenu_graphvisibility.gif
 *
 * \subsubsection JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEWHEEL Actions When a Mouse Wheel Event Occurs
 * The actions to be performed when the mouse wheel is operated are specified in JKQTPMouseWheelActions.
 * You can bind one of these actions to the mouse-wheel (under the condition that a specified Qt::KeyboardModifiers
 * is pressed) by using these functions:
 *   - registerMouseWheelAction() tells JKQTPlotter to perform a certain action (selected from JKQTPMouseWheelActions)
 *     when a specified mouse button is pushed while a specified (or no) keyborad modifiers (e.g. CTRL,ALT,SHIFT...) is pressed.
 *     By default JKQTPlotter calls this in its constructors:
 *     \code
 *          registerMouseWheelAction(Qt::NoModifier, JKQTPMouseWheelActions::jkqtpmwaZoomByWheel);
 *     \endcode
 *     Therefore by default you can zoom into and out of the plot, using the mouse wheel.
 *   - deregisterMouseWheelAction() deletes a previously defined user-interaction
 *   - clearAllMouseWheelActions() deletes all previously specified user-actions
 * .
 * In addition the signal void plotMouseWheelOperated() is emitted whenever a mouse-wheel event occurs.
 *
 *
 * \subsubsection JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEMOVE Mouse Move Signaling and Actions
 * In addition the signal plotMouseMove() is called whenever the mouse moves over the plot.
 * Additional signals may be emitted, depending on the currently active JKQTPlotter::MouseActionMode.
 *
 * Also the current mouse position is shown above the graph by default (can be switched on or off
 * using setMousePositionShown() ).
 *
 * \image html mousepositiondisplay.gif
 *
 * The actions to be performed when the mouse moves without a mouse-button pressed are specified in JKQTPMouseMoveActions.
 * You can bind one of these actions to the mouse (under the condition that a specified Qt::KeyboardModifiers
 * is pressed) by using these functions:
 *   - registerMouseMoveAction() tells JKQTPlotter to perform a certain action (selected from JKQTPMouseMoveActions)
 *     when a specified mouse button is pushed while a specified (or no) keyborad modifiers (e.g. CTRL,ALT,SHIFT...) is pressed.
 *   - deregisterMouseMoveAction() deletes a previously defined user-interaction
 *   - clearAllMouseMoveActions() deletes all previously specified user-actions
 * .
 *
 * \subsubsection JKQTPLOTTER_USERMOUSEINTERACTION_FASTRESIZING Fast Resizing of a Plot Widget
 * When the user resized the widget, the graph would in principle have to be readrawn for every intermediate step.
 * As this is very timeconsuming, JKQTPlotter uses a different approach: Actual Redrawing is delayed a few 100ms
 * and this delay is reset whenever the widget size changes. Only after the last delay expires, the plot is redrawn.
 * In between an image of the last state of the plot is simple stretched/compressed to fill the new/current
 * widget size. In sum this looks like this:
 *
 * \image html jkqtplotter_fastresizing.gif
 *
 *
 *
 *
 * \section JKQTPLOTTER_USEQTCREATOR  How to use JKQTPlotter in the Qt Form Designer
 *
 * \copydetails jkqtplotter_general_usage_qtcreator
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPlotter: public QWidget {
        Q_OBJECT
    public:
        /** \brief sets the global resize delay in milliseconds \a delayMS. After calling this function all plots will use the new delay. Setting the the delay to 0 disables the delayed resize feature. This function is thread-safe!
          *
          * \see jkqtp_RESIZE_DELAY, setGlobalResizeDelay(), getGlobalResizeDelay(), resizeTimer */
        static void setGlobalResizeDelay(int delayMS);
        /** \brief returns the currently set global resize delay in milliseconds \a delayMS. This function is thread-safe!
          *
          * \see jkqtp_RESIZE_DELAY, setGlobalResizeDelay(), getGlobalResizeDelay(), resizeTimer */
        static int getGlobalResizeDelay();

        /** \brief class constructor
         *
         * If \a datastore_internal \c ==false, you can supply an external JKQTPDatastore with the parameter \a datast
         */
        explicit JKQTPlotter(bool datastore_internal, QWidget* parent=nullptr, JKQTPDatastore* datast=nullptr);
        /** \brief class constructor for a JKQTPlotter using an external JKQTPDatastore \a dataset
         */
        explicit JKQTPlotter(JKQTPDatastore* datast, QWidget* parent=nullptr);
        /** \brief class constructor
         *
         * generated a new internal JKQTPDatastore
         */
        explicit JKQTPlotter(QWidget* parent=nullptr);

        /** \brief class destructor */
        virtual ~JKQTPlotter();

        /** reinitializes the toolbar, i.e. fills in QActions added to the QWidget since its creation/the last call to this function */
        void updateToolbarActions();


        /** \brief set the width/height of the icons in the toolbar in pt */
        void setToolbarIconSize(int value);

        /** \brief get the width/height of the icons in the toolbar in pt */
        int getToolbarIconSize() const;

        /** \brief returns the JKQTBasePlotter object internally used for plotting */
        JKQTBasePlotter* getPlotter()  { return plotter; }
        /** \brief returns the JKQTBasePlotter object internally used for plotting */
        const JKQTBasePlotter* getPlotter() const  { return plotter; }
        /** \brief returns the JKQTBasePlotter object internally used for plotting */
        const JKQTBasePlotter* getConstplotter() const { return const_cast<const JKQTBasePlotter*>(plotter); }


        /** \copydoc JKQTBasePlotter::getMainKey()  */
        inline JKQTPBaseKey* getMainKey()
        {
            return plotter->getMainKey();
        }

        /** \copydoc JKQTBasePlotter::getMainKey()  */
        inline const JKQTPBaseKey* getMainKey() const
        {
            return plotter->getMainKey();
        }

        /** \brief returns whether the toolbar is enabled
         *
         *  \copydetails JKQTPlotterStyle::toolbarEnabled
         *
         *  \see setToolbarEnabled(), JKQTPlotterStyle::toolbarEnabled
         */
        bool isToolbarEnabled() const;
        /** \brief returns whether the toolbar is always visible or only when the mouse moves to the upper left area
         *
         *  \copydetails JKQTPlotterStyle::toolbarAlwaysOn
         *
         *  \see setToolbarAlwaysOn(), JKQTPlotterStyle::toolbarAlwaysOn
         */
        bool isToolbarAlwaysOn() const;
        /** \brief specifies whether to display the current position of the mouse in the top border of the plot (this may automatically extent the
          *        top border, so the position fits in. The default widget font is used for the output.
          *
          *  \copydetails JKQTPlotterStyle::displayMousePosition
          *
          *  \see setMousePositionShown(), JKQTPlotterStyle::displayMousePosition, \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEMOVE */
        bool isMousePositionShown() const;

        /** \copydoc JKQTPlotterStyle::userActionOverlayPen
         *
         * \see setUserActionOverlayPen(), getUserActionOverlayPen(), JKQTPlotterStyle::userActionOverlayPen \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        QPen getUserActionOverlayPen() const;

        /** \copydoc JKQTPlotterStyle::userActionOverlayBrush
         *
         * \see setUserActionOverlayBrush(), getUserActionOverlayBrush(), JKQTPlotterStyle::userActionOverlayBrush \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        QBrush getUserActionOverlayBrush() const;

        /** \copydoc JKQTPlotterStyle::userActionOpaquePen
         *
         * \see setUserActionOpaquePen(), getUserActionOpaquePen(), JKQTPlotterStyle::userActionOpaquePen \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        QPen getUserActionOpaquePen() const;

        /** \copydoc JKQTPlotterStyle::userActionOpaqueBrush
         *
         * \see setUserActionOpaqueBrush(), getUserActionOpaqueBrush(), JKQTPlotterStyle::userActionOpaqueBrush \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        QBrush getUserActionOpaqueBrush() const;

        /** \copydoc JKQTPlotterStyle::userActionMarkerPen
         *
         * \see setUserActionMarkerPen(), getUserActionMarkerPen(), JKQTPlotterStyle::userActionMarkerPen \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        QPen getUserActionMarkerPen() const;

        /** \copydoc JKQTPlotterStyle::userActionMarkerBrush
         *
         * \see setUserActionMarkerBrush(), getUserActionMarkerBrush(), JKQTPlotterStyle::userActionMarkerBrush \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        QBrush getUserActionMarkerBrush() const;

        /** \copydoc JKQTPlotterStyle::maxTooltipDistance  */
        int getMaxTooltipDistance() const;

        /** \copydoc JKQTPlotterStyle::userActionCatchSensitivity  */
        int getUserActionCatchSensitivity() const;

        /** \copydoc JKQTPlotterStyle::userActionMarkerDiameter  */
        int getUserActionMarkerDiameter() const;

        /** \copydoc JKQTPlotterStyle::userActionMarkerType  */
        JKQTPUserActionMarkerType getUserActionMarkerType() const;


        /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object */
        void loadSettings(const QSettings &settings, const QString& group=QString("plots/"));

        /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         * This method only saves those properties that differ from their default value.
         */
        void saveSettings(QSettings& settings, const QString& group=QString("plots/")) const;

        /** \brief returns the minimum size of the widget */
        QSize minimumSizeHint() const override;

        /** \brief returns the size of the widget */
        QSize sizeHint() const override;

        /** \brief returns \c true, if the JKQTPlotter::resizeTimer is currently running and the widget is waiting for the resize-event to finish
          *
          * \see jkqtp_RESIZE_DELAY, setGlobalResizeDelay(), getGlobalResizeDelay(), resizeTimer */
        bool isResizeTimerRunning() const;




        /*! \brief synchronize the plot borders (and zooming) with a given plotter (master --> slave/this)

            This function allows two plotters to draw a graph with exactly the same height or width
            as in another graph. For example if you want to have two plotters which are positioned one
            above the other (and have the same widget widths, which could be guaranteed by a QLayout)
            you may want to make sure that their plotWidth s are always the same. In this case call
            \code plotter2->synchronizeToMaster(plotter1, sdXAxis, true) \endcode of the lower plotter \c plotter2 .
            Now whenever the size of plotter1 changes, also plotter2 is redrawn with the changed
            borders.

            \image html jkqtbaseplotter_synchronization.png "Different Synchronization Settings for synchronizeDirection=sdXAxis"

            \param master the plotter widget to synchronize to
            \param synchronizeDirection direction in which to synchronize
            \param synchronizeAxisLength do you want the axis length to be synchronized?
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master

            \see synchronizeXToMaster(), synchronizeYToMaster(), resetMasterSynchronization(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
        */
        void synchronizeToMaster(JKQTPlotter* master, JKQTBasePlotter::SynchronizationDirection synchronizeDirection, bool synchronizeAxisLength=true, bool synchronizeZoomingMasterToSlave=true, bool synchronizeZoomingSlaveToMaster=true);

        /*! \brief synchronize the plot x-axis width (and x-zooming) with a given master plotter (master --> slave/this)

            \param master the plotter widget to synchronize to
            \param synchronizeAxisLength do you want the axis length to be synchronized?
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master

            \image html jkqtbaseplotter_synchronization.png "Different Synchronization Settings for synchronizeDirection=sdXAxis"

            \note This is a short-cut to synchronizeToMaster() with \c synchronizeDirection=csXAxis
            \see synchronizeToMaster(), synchronizeYToMaster(), resetMasterSynchronization(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
        */
        void synchronizeXToMaster(JKQTPlotter* master, bool synchronizeAxisLength=true, bool synchronizeZoomingMasterToSlave=true, bool synchronizeZoomingSlaveToMaster=true);

        /*! \brief synchronize the plot y-axis height (and y-zooming) with a given master plotter (master --> slave/this)

            \param master the plotter widget to synchronize to
            \param synchronizeAxisLength do you want the axis length to be synchronized?
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master

            \note This function internally calls JKQTBasePlotter::synchronizeToMaster()
            \note This is a short-cut to synchronizeToMaster() with \c synchronizeDirection=csXAxis
            \see synchronizeToMaster(), synchronizeXToMaster(), resetMasterSynchronization(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT

        */
        void synchronizeYToMaster(JKQTPlotter* master, bool synchronizeAxisLength=true, bool synchronizeZoomingMasterToSlave=true, bool synchronizeZoomingSlaveToMaster=true);

        /** \brief switches any synchronization off, that has been created by synchronizeToMaster()
         *
         *  \note This function internally calls JKQTBasePlotter::synchronizeToMaster()
            \see synchronizeToMaster(), synchronizeXToMaster(), resetMasterSynchronization(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
         */
        void resetMasterSynchronization(JKQTBasePlotter::SynchronizationDirection synchronizeDirection=JKQTBasePlotter::sdXYAxes);


        /*! \brief enables grid-printing for this plot
         *
         * \note This function call forwards to JKQTBasePlotter::setGridPrinting()
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentPos(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
         */
        void setGridPrinting(bool enabled);

        /** \brief add a new plotter \a plotterOther for grid printing mode, at location \a x / \a y
         *
         * \note This function call forwards to JKQTBasePlotter::addGridPrintingPlotter()
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentPos(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
         */
        void addGridPrintingPlotter(size_t x, size_t y, JKQTPlotter* plotterOther) ;

        /** \brief clear all additional plotters for grid printing mode
         *
         * \note This function call forwards to JKQTBasePlotter::clearGridPrintingPlotters()
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentPos(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
         */
        void clearGridPrintingPlotters() ;
        /** \brief set the x-position of this JKQTPlotter in the grid-printing grid
         *
         * \note This function call forwards to JKQTBasePlotter::setGridPrintingCurrentX()
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentPos(), setGridPrintingCurrentY(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
         */
        void setGridPrintingCurrentX(size_t x);
        /** \brief set the y-position of this JKQTPlotter in the grid-printing grid
         *
         * \note This function call forwards to JKQTBasePlotter::setGridPrintingCurrentY()
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentPos(), setGridPrintingCurrentX(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
         */
        void setGridPrintingCurrentY(size_t y);
        /** \brief set the x- and y-positions of this JKQTPlotter in the grid-printing grid
         *
         * \note This function call forwards to JKQTBasePlotter::setGridPrintingCurrentPos()
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentX(), setGridPrintingCurrentY() \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
         */
        void setGridPrintingCurrentPos(size_t x, size_t y);



        /** \copydoc JKQTBasePlotter::getDatastore()  */
        inline JKQTPDatastore* getDatastore() { return plotter->getDatastore(); }

        /** \copydoc JKQTBasePlotter::getDatastore()  */
        inline const JKQTPDatastore* getDatastore() const { return plotter->getDatastore(); }

        /** \copydoc JKQTBasePlotter::useExternalDatastore()  */
        inline void useExternalDatastore(JKQTPDatastore* newStore) { plotter->useExternalDatastore(newStore); }

        /** \copydoc JKQTBasePlotter::useAsInternalDatastore() */
        inline void useAsInternalDatastore(JKQTPDatastore* newStore) { plotter->useAsInternalDatastore(newStore); }

        /** \copydoc JKQTBasePlotter::useInternalDatastore() */
        inline void useInternalDatastore() { plotter->useInternalDatastore(); }

        /** \copydoc JKQTBasePlotter::forceInternalDatastore() */
        inline void forceInternalDatastore() { plotter->forceInternalDatastore(); }

        /** \copydoc JKQTBasePlotter::setEmittingSignalsEnabled() */
        inline void setEmittingSignalsEnabled(bool sig) { plotter->setEmittingSignalsEnabled(sig); }
        /** \copydoc JKQTBasePlotter::isEmittingSignalsEnabled() */
        inline bool isEmittingSignalsEnabled() const { return plotter->isEmittingSignalsEnabled(); }

        /** \brief returns, whether automatic redrawing the plot is currently activated (e.g. you can deactivate this with setPlotUpdateEnabled() while performing major updates on the plot)
         *
         * \see setPlotUpdateEnabled()
         */
        bool isPlotUpdateEnabled() const;
        /** \brief sets whether automatic redrawing the plot is currently activated (e.g. you can sett his to \c false while performing major updates on the plot)
         *
         * \see JKQTPlotterUpdateGuard for a guard class coupling calls to setPlotUpdateEnabled() and redrawPlot() with the lifetime of a code block.
         *      See \ref jkqtplotter_general_usage_speedplotsetup for a detailed description.
         *      Also see isPlotUpdateEnabled().
         */
        void setPlotUpdateEnabled(bool enable);

        /** \brief registeres a certain mouse drag action \a action to be executed when a mouse drag operation is
         *         initialized with the given \a button and \a modifier */
        void registerMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier, JKQTPMouseDragActions action);
        /** \brief deregisteres the mouse drag action to be executed when a mouse drag operation is
         *         initialized with the given \a button and \a modifier */
        void deregisterMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier);
        /** \brief clear all registeres mouse drag actions */
        void clearAllRegisteredMouseDragActions();

        /** \brief registeres a certain mouse action \a action to be executed when a mouse double-click occurs
         *         with the given \a button and \a modifier */
        void registerMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier, JKQTPMouseDoubleClickActions action);
        /** \brief deregisteres the mouse action \a action to be executed when a mouse double-click occurs
         *         with the given \a button and \a modifier */
        void deregisterMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier);
        /** \brief clear all registered mouse double-click actions */
        void clearAllRegisteredMouseDoubleClickActions();

        /** \brief specifies the action to perform on a mouse wheel event when a given modifier is pressed \see deregisterMouseWheelAction(), clearAllMouseWheelActions(), \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void registerMouseWheelAction(Qt::KeyboardModifiers modifier, JKQTPMouseWheelActions action);
        /** \brief deletes all mouse-wheel actions registered for a given \a modifier \see registerMouseWheelAction(), clearAllMouseWheelActions(), \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void deregisterMouseWheelAction(Qt::KeyboardModifiers modifier);
        /** \brief deletes all mouse-wheel actions \see registerMouseWheelAction(), deregisterMouseWheelAction(), \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void clearAllMouseWheelActions();

        /** \brief specifies the action to perform on a mouse move event when a given modifier is pressed \see deregisterMouseMoveAction(), clearAllMouseMoveActions(), \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void registerMouseMoveAction(Qt::KeyboardModifiers modifier, JKQTPMouseMoveActions action);
        /** \brief deletes all mouse-move actions registered for a given \a modifier \see registerMouseMoveAction(), clearAllMouseMoveActions(), \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void deregisterMouseMoveAction(Qt::KeyboardModifiers modifier);
        /** \brief deletes all mouse-move actions \see registerMouseMoveAction(), deregisterMouseMoveAction(), \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void clearAllMouseMoveActions();

        /*! \brief returns the currently set special context menu object
         *
         *  \see \ref JKQTPLOTTER_SPECIALCONTEXTMENU, setSpecialContextMenu(), menuSpecialContextMenu, contextMenuOpened(), \ref JKQTPlotterUserInteraction
         */
        QMenu *getSpecialContextMenu() const;

        /*! \brief sets a QMenu object to be used as special context menu
         *
         *  \see \ref JKQTPLOTTER_SPECIALCONTEXTMENU, getSpecialContextMenu(), menuSpecialContextMenu, contextMenuOpened()
         */
        void setSpecialContextMenu(QMenu* menu);


        /** \brief x-position of the mouse (in plot coordinates) when a user mouse-action was started (e.g. drawing a rectangle)
         *
         * \see \ref JKQTPLOTTER_CONTEXTMENU , getMouseContextY(), getMouseLastClickX(), getMouseLastClickY()
         */
        double getMouseContextX() const;
        /** \brief y-position of the mouse (in plot coordinates) when a user mouse-action was started (e.g. drawing a rectangle)
         *
         * \see \ref JKQTPLOTTER_CONTEXTMENU , getMouseContextX(), getMouseLastClickX(), getMouseLastClickY()
         */
        double getMouseContextY() const;
        /** \brief x-position of the last mouse-click (in screen pixels)
         *
         * \see getMouseLastClickY(), getMouseContextX(), getMouseContextY()
         */
        int getMouseLastClickX() const;
        /** \brief y-position of the last mouse-click (in screen pixels)
         *
         * \see getMouseLastClickX(), getMouseContextX(), getMouseContextY()
         */
        int getMouseLastClickY() const;

        /** \copydoc JKQTBasePlotter::getXAxis()   */
        inline JKQTPHorizontalAxisBase* getXAxis(JKQTPCoordinateAxisRef axis=JKQTPPrimaryAxis) { return plotter->getXAxis(axis); }
        /** \copydoc JKQTBasePlotter::getYAxis()   */
        inline JKQTPVerticalAxisBase* getYAxis(JKQTPCoordinateAxisRef axis=JKQTPPrimaryAxis) { return plotter->getYAxis(axis); }
        /** \copydoc JKQTBasePlotter::getXAxis()   */
        inline const JKQTPHorizontalAxisBase* getXAxis(JKQTPCoordinateAxisRef axis=JKQTPPrimaryAxis) const { return plotter->getXAxis(axis); }
        /** \copydoc JKQTBasePlotter::getYAxis()   */
        inline const JKQTPVerticalAxisBase* getYAxis(JKQTPCoordinateAxisRef axis=JKQTPPrimaryAxis) const { return plotter->getYAxis(axis); }




        /** \copydoc JKQTBasePlotter::beginGraphs() */
        inline typename JKQTBasePlotter::GraphsConstIterator cbeginGraphs() const  {
            return plotter->cbeginGraphs();
        }
        /** \copydoc JKQTBasePlotter::endGraphs() */
        inline typename JKQTBasePlotter::GraphsConstIterator cendGraphs() const {
            return plotter->cendGraphs();
        }

        /** \copydoc JKQTBasePlotter::beginGraphs() */
        inline typename JKQTBasePlotter::GraphsConstIterator beginGraphs() const  {
            return plotter->beginGraphs();
        }
        /** \copydoc JKQTBasePlotter::endGraphs() */
        inline typename JKQTBasePlotter::GraphsConstIterator endGraphs() const {
            return plotter->endGraphs();
        }
        /** \copydoc JKQTBasePlotter::beginGraphs() */
        inline typename JKQTBasePlotter::GraphsIterator beginGraphs()  {
            return plotter->beginGraphs();
        }
        /** \copydoc JKQTBasePlotter::endGraphs() */
        inline typename JKQTBasePlotter::GraphsIterator endGraphs()  {
            return plotter->endGraphs();
        }
        /** \copydoc JKQTBasePlotter::modifyGraphs() */
        inline void modifyGraphs(const std::function<void(JKQTPPlotElement*)>& func) {
            plotter->modifyGraphs(func);
        }
        /** \copydoc JKQTBasePlotter::sortGraphs() */
        inline void sortGraphs(const std::function<bool(const JKQTPPlotElement* , const JKQTPPlotElement* )>& compareLess) {
            plotter->sortGraphs(compareLess);
        }
        /** \copydoc JKQTBasePlotter::getGraphs()   */
        inline const typename JKQTBasePlotter::GraphsList& getGraphs() const {
            return plotter->getGraphs();
        }
        /** \copydoc JKQTBasePlotter::getGraphs()   */
        inline typename JKQTBasePlotter::GraphsList& getGraphs()  {
            return plotter->getGraphs();
        }
        /** \copydoc JKQTBasePlotter::getGraph()   */
        inline JKQTPPlotElement* getGraph(size_t i) { return plotter->getGraph(i); }
        /** \copydoc JKQTBasePlotter::getGraph()   */
        inline const JKQTPPlotElement* getGraph(size_t i) const { return plotter->getGraph(i); }

        /** \copydoc JKQTBasePlotter::getGraphCount()   */
        inline size_t getGraphCount() { return plotter->getGraphCount(); }

        /** \copydoc JKQTBasePlotter::deleteGraph()   */
        inline void deleteGraph(size_t i, bool deletegraph=true) { plotter->deleteGraph(i, deletegraph); }

        /** \copydoc JKQTBasePlotter::containsGraph()   */
        inline bool containsGraph(JKQTPPlotElement* gr) { return plotter->containsGraph(gr); }

        /** \copydoc JKQTBasePlotter::deleteGraph()   */
        inline void deleteGraph(JKQTPPlotElement* gr, bool deletegraph=true) { plotter->deleteGraph(gr, deletegraph);  }

        /** \copydoc JKQTBasePlotter::clearGraphs()   */
        inline void clearGraphs(bool deleteGraphs=true) { plotter->clearGraphs(deleteGraphs); }

        /** \copydoc JKQTBasePlotter::addGraph()   */
        inline size_t addGraph(JKQTPPlotElement* gr) { return plotter->addGraph(gr); }
        /** \copydoc JKQTBasePlotter::addGraphOnTop()   */
        inline size_t addGraphOnTop(JKQTPPlotElement* gr) { return plotter->addGraphOnTop(gr); }
        /** \copydoc JKQTBasePlotter::addGraphAtBottom()   */
        inline size_t addGraphAtBottom(JKQTPPlotElement* gr) { return plotter->addGraphAtBottom(gr); }

        /** \copydoc JKQTBasePlotter::moveGraphTop()   */
        inline void moveGraphTop(JKQTPPlotElement* gr) {  plotter->moveGraphTop(gr); }

        /** \copydoc JKQTBasePlotter::moveGraphBottom()   */
        inline void moveGraphBottom(JKQTPPlotElement* gr) {  plotter->moveGraphBottom(gr); }

        /** \copydoc JKQTBasePlotter::moveGraphUp()   */
        inline void moveGraphUp(JKQTPPlotElement* gr) {  plotter->moveGraphUp(gr); }

        /** \copydoc JKQTBasePlotter::moveGraphDown()   */
        inline void moveGraphDown(JKQTPPlotElement* gr) {  plotter->moveGraphDown(gr); }

        /** \copydoc JKQTBasePlotter::moveGraphTop()   */
        inline void moveGraphTop(int idx) {  plotter->moveGraphTop(idx); }

        /** \copydoc JKQTBasePlotter::moveGraphBottom()   */
        inline void moveGraphBottom(int idx) {  plotter->moveGraphBottom(idx); }

        /** \copydoc JKQTBasePlotter::moveGraphUp()   */
        inline void moveGraphUp(int idx) {  plotter->moveGraphUp(idx); }

        /** \copydoc JKQTBasePlotter::moveGraphDown()   */
        inline void moveGraphDown(int idx) {  plotter->moveGraphDown(idx); }

        /** \copydoc JKQTBasePlotter::addGraphs()   */
        template <class TJKQTPGraphContainer>
        inline void addGraphs(const TJKQTPGraphContainer& gr, QVector<size_t>* graphIDsOut=nullptr) { plotter->addGraphs(gr, graphIDsOut); }


        /** \copydoc JKQTBasePlotter::getXMin()   */
        inline double getXMin() const {return plotter->getXMin(); }
        /** \copydoc JKQTBasePlotter::getXMax()   */
        inline double getXMax() const {return plotter->getXMax(); }
        /** \copydoc JKQTBasePlotter::getYMin()   */
        inline double getYMin() const {return plotter->getYMin(); }
        /** \copydoc JKQTBasePlotter::getYMax()   */
        inline double getYMax() const {return plotter->getYMax(); }
        /** \copydoc JKQTBasePlotter::getAbsoluteXMin()   */
        inline double getAbsoluteXMin() const {return plotter->getAbsoluteXMin(); }
        /** \copydoc JKQTBasePlotter::getAbsoluteXMax()   */
        inline double getAbsoluteXMax() const {return plotter->getAbsoluteXMax(); }
        /** \copydoc JKQTBasePlotter::getAbsoluteYMin()   */
        inline double getAbsoluteYMin() const {return plotter->getAbsoluteYMin(); }
        /** \copydoc JKQTBasePlotter::getAbsoluteYMax()   */
        inline double getAbsoluteYMax() const {return plotter->getAbsoluteYMax(); }

        /** \brief returns the current magnification factor */
        inline double getMagnification() const { return magnification; }

        /** \copydoc JKQTBasePlotter::getNextStyle()   */
        inline int getNextStyle() {
            return getPlotter()->getNextStyle();
        }

        /** \copydoc JKQTBasePlotter::getPlotStyle()   */
        inline JKQTBasePlotter::JKQTPPen getPlotStyle(int i) const {
            return getConstplotter()->getPlotStyle(i);
        }



        /** \brief returns the currently set mode for the context menu \see JKQTPContextMenuModes, \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_SPECIALCONTEXTMENU , \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        JKQTPContextMenuModes getContextMenuMode() const;

        /** \brief current style properties for this JKQTPlotter
         *
         * \see JKQTPSetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), \ref jkqtpplotter_styling
         */
        const JKQTPlotterStyle& getCurrentPlotterStyle() const;

        /** \brief replace the current style properties for this JKQTBasePlotter
         *
         * \see JKQTPSetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        void setCurrentPlotterStyle(const JKQTPlotterStyle& style);
        /** \brief replace the current style properties for this JKQTBasePlotter
         *
         * \see JKQTPSetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        void setCurrentPlotterStyle(const JKQTPlotterStyle& style, const JKQTBasePlotterStyle &baseStyle);
        /** \brief replace the current style properties for this JKQTBasePlotter with properties loaded from \a settings
         *
         * \param settings the QSettings object to read from
         * \param group group in \a settings to read from
         * \param alsoLoadBaseStyle if \c true, then also JKQTBasePlotter::loadCurrentPlotterStyle() of the internal JKQTBasePlotter is called
         *
         * \see JKQTPSetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        void loadCurrentPlotterStyle(const QSettings& settings, const QString& group="plots/", bool alsoLoadBaseStyle=true);
        /** \brief store the current style properties for this JKQTBasePlotter with properties loaded from \a settings
         *
         * \param settings the QSettings object to write to
         * \param group group in \a settings to write to
         * \param alsoSaveBaseStyle if \c true, then also JKQTBasePlotter::saveCurrentPlotterStyle() of the internal JKQTBasePlotter is called
         *
         * \see JKQTPSetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        void saveCurrentPlotterStyle(QSettings& settings, const QString& group="plots/", bool alsoSaveBaseStyle=true) const;

        /** \brief \copydoc actMouseMoveToolTip */
        QAction *getActMouseMoveToolTip();
        /** \brief \copydoc actMouseLeftAsRuler */
        QAction *getActMouseLeftAsRuler();
        /** \brief \copydoc actMouseLeftAsDefault */
        QAction *getActMouseLeftAsDefault();
        /** \brief \copydoc actMouseLeftAsZoomRect */
        QAction *getActMouseLeftAsZoomRect();
        /** \brief \copydoc actMouseLeftAsPanView */
        QAction *getActMouseLeftAsPanView();
        /** \brief \copydoc actMouseMoveToolTip */
        const QAction *getActMouseMoveToolTip() const;
        /** \brief \copydoc actMouseLeftAsRuler */
        const QAction *getActMouseLeftAsRuler() const;
        /** \brief \copydoc actMouseLeftAsDefault */
        const QAction *getActMouseLeftAsDefault() const;
        /** \brief \copydoc actMouseLeftAsZoomRect */
        const QAction *getActMouseLeftAsZoomRect() const;
        /** \brief \copydoc actMouseLeftAsPanView */
        const QAction *getActMouseLeftAsPanView() const;

        /** \copydoc JKQTBasePlotter::grabPixelImage()   */
        inline QImage grabPixelImage(QSize size=QSize(), bool showPreview=false) {
            return plotter->grabPixelImage(size,showPreview);
        }
        /** \copydoc JKQTBasePlotter::copyPixelImage()   */
        inline void copyPixelImage(bool showPreview=true) {
            plotter->copyPixelImage(showPreview);
        }

    public Q_SLOTS:
        /** \brief set the current plot magnification */
        void setMagnification(double m);
        /** \copydoc JKQTBasePlotter::zoom()
         *  \deprecated The function JKQTPlotter::zoom() is deprecated and will be removed in future version, use JKQTPlotter::setXY() instead!
         */
        inline void zoom(double xminn, double xmaxn, double yminn, double ymaxn, bool affectsSecondaryAxes=false) {
            plotter->zoom(xminn, xmaxn, yminn, ymaxn, affectsSecondaryAxes);
        }

        /** \copydoc JKQTBasePlotter::setGrid()   */
         inline void setGrid(bool val) {
            plotter->setGrid(val);
        }

        /** \copydoc JKQTBasePlotter::setGridColor()   */
        inline void setGridColor(QColor color) {
            plotter->setGridColor(color);
        }

        /** \brief sets the color of all minor grid lines
         * */
        inline void setMinorGridColor(QColor color) {
            plotter->setMinorGridColor(color);
        }

        /** \brief sets the width of all Major grid lines
         * */
        inline void setGridWidth(double __value) {
            plotter->setGridWidth(__value);
        }

        /** \brief sets the width of all minor grid lines
         * */
        inline void setMinorGridWidth(double __value) {
            plotter->setMinorGridWidth(__value);
        }

        /** \brief sets the style of all Major grid lines
         * */
        inline void setGridStyle(Qt::PenStyle __value) {
            plotter->setGridStyle(__value);
        }

        /** \brief sets the style of all minor grid lines
         * */
        inline void setMinorGridStyle(Qt::PenStyle __value) {
            plotter->setMinorGridStyle(__value);
        }

        /** \brief switches the visibility of the zero-axes associated with the x- and y-axis
         *
         * \param showX indicates whether to show the zero-axis associated with the x-axis (i.e. x==0 or the vertical zero-axis)
         * \param showY indicates whether to show the zero-axis associated with the y-axis (i.e. y==0 or the horizontal zero-axis)
         * */
        inline void setShowZeroAxes(bool showX, bool showY) {
            plotter->setShowZeroAxes(showX, showY);
        }
        /** \copydoc JKQTBasePlotter::setShowZeroAxes() */
        inline void setShowZeroAxes(bool showXY) {
            plotter->setShowZeroAxes(showXY);
        }

        /** \copydoc JKQTBasePlotter::saveImage() */
        inline bool saveImage(const QString& filename=QString(""), bool displayPreview=true) {
            return plotter->saveImage(filename, displayPreview);
        }

        /** \copydoc JKQTBasePlotter::saveAsPixelImage() */
        inline bool saveAsPixelImage(const QString& filename=QString(""), bool displayPreview=true, const QByteArray &outputFormat=QByteArray(), const QSize& outputSizeIncrease=QSize(0,0)) {
            return plotter->saveAsPixelImage(filename, displayPreview, outputFormat, outputSizeIncrease);
        }

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \copydoc JKQTBasePlotter::saveAsSVG() */
        inline bool saveAsSVG(const QString& filename=QString(""), bool displayPreview=true) { return plotter->saveAsSVG(filename, displayPreview); }
        /** \copydoc JKQTBasePlotter::saveAsPDF() */
        inline bool saveAsPDF(const QString& filename=QString(""), bool displayPreview=true) { return plotter->saveAsPDF(filename, displayPreview); }
#endif

        /** \copydoc JKQTBasePlotter::saveAsCSV() */
        inline void saveAsCSV(const QString& filename=QString("")) { plotter->saveAsCSV(filename); }
        /** \copydoc JKQTBasePlotter::saveAsSemicolonSV() */
        inline void saveAsSemicolonSV(const QString& filename=QString("")) { plotter->saveAsSemicolonSV(filename); }
        /** \copydoc JKQTBasePlotter::saveAsTabSV() */
        inline void saveAsTabSV(const QString& filename=QString("")) { plotter->saveAsTabSV(filename); }
        /** \copydoc JKQTBasePlotter::saveAsDIF() */
        inline void saveAsDIF(const QString& filename=QString("")) { plotter->saveAsDIF(filename); }
        /** \copydoc JKQTBasePlotter::saveAsSYLK() */
        inline void saveAsSYLK(const QString& filename=QString("")) { plotter->saveAsSYLK(filename); }
        /** \copydoc JKQTBasePlotter::saveAsMatlab() */
        inline void saveAsMatlab(const QString& filename=QString("")) { plotter->saveAsMatlab(filename); }
        /** \copydoc JKQTBasePlotter::saveAsGerExcelCSV() */
        inline void saveAsGerExcelCSV(const QString& filename=QString("")) { plotter->saveAsGerExcelCSV(filename); }


        /** \copydoc JKQTBasePlotter::saveData() */
        inline void saveData(const QString& filename=QString(""), const QString& format=QString("")) {
            plotter->saveData(filename, format);
        }

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \copydoc JKQTBasePlotter::print() */
        inline void print(QPrinter* printer=nullptr) {
            plotter->print(printer);
        }
#endif

        /** \copydoc JKQTBasePlotter::copyData() */
        inline void copyData() {
            plotter->copyData();
        }

        /** \copydoc JKQTBasePlotter::copyDataMatlab() */
        inline void copyDataMatlab() {
            plotter->copyDataMatlab();
        }


        /** \copydoc JKQTBasePlotter::zoomToFit() */
        inline void zoomToFit(bool zoomX=true, bool zoomY=true, bool includeX0=false, bool includeY0=false, double scaleX=1.05, double scaleY=1.05) {
            plotter->zoomToFit(zoomX, zoomY, includeX0, includeY0, scaleX, scaleY);
        }

        /** \copydoc JKQTBasePlotter::zoomIn() */
        inline void zoomIn(double factor=2.0) { plotter->zoomIn(factor); }
        /** \copydoc JKQTBasePlotter::zoomOut() */
        inline void zoomOut(double factor=2.0) { plotter->zoomOut(factor); }

        /** \brief update the plot and the overlays */
        void redrawPlot();

        /** \brief allows to activate/deactivate toolbar buttons that can activate certain mouse drag actions
         *
         *  \see getActMouseLeftAsDefault(), getActMouseLeftAsRuler(), getActMouseLeftAsToolTip()
         */
        void setMouseActionToolbarActionsActive(bool __value);

        /** \brief returns whether the toolbar is enabled
         *
         *  \copydetails JKQTPlotterStyle::toolbarEnabled
         *
         *  \see isToolbarEnabled(), JKQTPlotterStyle::toolbarEnabled
         */
        void setToolbarEnabled(bool __value);
        /** \brief returns whether the toolbar is always visible or only when the mouse moves to the upper left area
         *
         *  \copydetails JKQTPlotterStyle::toolbarAlwaysOn
         *
         *  \see isToolbarAlwaysOn(), JKQTPlotterStyle::toolbarAlwaysOn
         */
        void setToolbarAlwaysOn(bool __value);
        /** \brief specifies whether to display the current position of the mouse in the top border of the plot (this may automatically extent the
          *        top border, so the position fits in. The default widget font is used for the output.
          *
          *  \copydetails JKQTPlotterStyle::displayMousePosition
          *
          *  \see isMousePositionShown(), JKQTPlotterStyle::displayMousePosition, \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEMOVE */
        void setMousePositionShown(bool __value);
        /** \copydoc JKQTPlotterStyle::userActionOverlayPen
         *
         * \see setUserActionOverlayPen(), getUserActionOverlayPen(), JKQTPlotterStyle::userActionOverlayPen \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        void setUserActionOverlayPen(const QPen & __value);
        /** \copydoc JKQTPlotterStyle::userActionOverlayPen
         *
         * \see setUserActionOverlayBrush(), getUserActionOverlayBrush(), JKQTPlotterStyle::userActionOverlayBrush \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        void setUserActionOverlayBrush(const QBrush & __value);
        /** \copydoc JKQTPlotterStyle::userActionOpaquePen
         *
         * \see setUserActionOpaquePen(), getUserActionOpaquePen(), JKQTPlotterStyle::userActionOpaquePen \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        void setUserActionOpaquePen(const QPen & __value);
        /** \copydoc JKQTPlotterStyle::userActionOpaquePen
         *
         * \see setUserActionOpaqueBrush(), getUserActionOpaqueBrush(), JKQTPlotterStyle::userActionOpaqueBrush \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        void setUserActionOpaqueBrush(const QBrush & __value);
        /** \copydoc JKQTPlotterStyle::userActionMarkerPen
         *
         * \see setUserActionMarkerPen(), getUserActionMarkerPen(), JKQTPlotterStyle::userActionMarkerPen \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        void setUserActionMarkerPen(const QPen & __value);
        /** \copydoc JKQTPlotterStyle::userActionMarkerPen
         *
         * \see setUserActionMarkerBrush(), getUserActionMarkerBrush(), JKQTPlotterStyle::userActionMarkerBrush \ref JKQTPLOTTER_USERMOUSEINTERACTION_MOUSEDRAG */
        void setUserActionMarkerBrush(const QBrush & __value);

        /** \copydoc JKQTPlotterStyle::maxTooltipDistance  */
        void setMaxTooltipDistance(int v);

        /** \copydoc JKQTPlotterStyle::userActionCatchSensitivity  */
        void setUserActionCatchSensitivity(int v);

        /** \copydoc JKQTPlotterStyle::userActionMarkerDiameter  */
        void setUserActionMarkerDiameter(int v);

        /** \copydoc JKQTPlotterStyle::userActionMarkerType  */
        void setUserActionMarkerType(JKQTPUserActionMarkerType v);

        /** \brief sets the mode if the standard context menu \see JKQTPContextMenuModes, \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_SPECIALCONTEXTMENU , \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void setContextMenuMode(JKQTPContextMenuModes mode);

        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x-axis to the other x-axis \see \ref JKQTBASEPLOTTER_SYNCMULTIPLOT */
        void synchronizeXAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local y-axis to the other y-axis \see \ref JKQTBASEPLOTTER_SYNCMULTIPLOT */
        void synchronizeYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x- and y-axis to the other x- and y-axis \see \ref JKQTBASEPLOTTER_SYNCMULTIPLOT */
        void synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);


        /** \brief popuplate the given toolbar \a toolbar with all actions shown in a toolbar from this class ...
         *
         * This function can be used to populate an external toolbar with actions for this JKQTPlotter.
         */
        void populateToolbar(QToolBar* toolbar) const;

        /** \brief open the context menu at the mouse position of the last click \see \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openContextMenu();
        /** \brief open the context menu at the mouse position \a x and \a y \see \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openContextMenu(int x, int y);

        /** \brief open the standard context menu at the mouse position of the last click \see \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openStandardContextMenu();
        /** \brief open the standard context menu at the mouse position \a x and \a y \see \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openStandardContextMenu(int x, int y);
        /** \brief open the special context menu at the mouse position of the last click \see \ref JKQTPLOTTER_SPECIALCONTEXTMENU, \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openSpecialContextMenu();
        /** \brief open the special context menu at the mouse position \a x and \a y \see \ref JKQTPLOTTER_SPECIALCONTEXTMENU, \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openSpecialContextMenu(int x, int y);

        /** \brief open the standard context menu with the special context menu integrated at the mouse position of the last click \see \ref JKQTPLOTTER_SPECIALCONTEXTMENU, \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openStandardAndSpecialContextMenu();
        /** \brief open the standard context menu with the special context menu integrated at the mouse position \a x and \a y \see \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_SPECIALCONTEXTMENU, \ref JKQTPLOTTER_USERMOUSEINTERACTION */
        void openStandardAndSpecialContextMenu(int x, int y);

        /** \copydoc JKQTBasePlotter::setAbsoluteX()  */
        inline void setAbsoluteX(double xminn, double xmaxx) { plotter->setAbsoluteX(xminn, xmaxx); }

        /** \copydoc JKQTBasePlotter::setAbsoluteY()  */
        inline void setAbsoluteY(double yminn, double ymaxx) { plotter->setAbsoluteY(yminn, ymaxx); }

        /** \copydoc JKQTBasePlotter::setAbsoluteXY()  */
        inline void setAbsoluteXY(double xminn, double xmaxx, double yminn, double ymaxx) { plotter->setAbsoluteXY(xminn, xmaxx, yminn, ymaxx); }

        /** \copydoc JKQTBasePlotter::setX()  */
        inline void setX(double xminn, double xmaxx) { plotter->setX(xminn, xmaxx); }

        /** \copydoc JKQTBasePlotter::setY()  */
        inline void setY(double yminn, double ymaxx) { plotter->setY(yminn, ymaxx); }

        /** \copydoc JKQTBasePlotter::setXY()  */
         inline void setXY(double xminn, double xmaxx, double yminn, double ymaxx, bool affectsSecondaryAxes=false) { plotter->setXY(xminn, xmaxx, yminn, ymaxx, affectsSecondaryAxes); }
    Q_SIGNALS:
        /** \brief emitted whenever the mouse moves
         *
         * \param x x-position of the mouse (in plot coordinates)
         * \param y y-position of the mouse (in plot coordinates)
         */
        void plotMouseMove(double x, double y);
        /** \brief emitted when a single-click event from the mouse occurs inside the plot
         *
         * \param x x-position of the mouse (in plot coordinates)
         * \param y y-position of the mouse (in plot coordinates)
         * \param modifiers key-modifiers when the click occured
         * \param button mouse-button that was used for the click
         */
        void plotMouseClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        /** \brief emitted when a double-click event from the mouse occurs inside the plot
         *
         * \param x x-position of the mouse (in plot coordinates)
         * \param y y-position of the mouse (in plot coordinates)
         * \param modifiers key-modifiers when the click occured
         * \param button mouse-button that was used for the click
         */
        void plotMouseDoubleClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        /** \brief emitted when a single-click event from the mouse occurs inside the plot
         *
         * \param x x-position of the mouse (in plot coordinates)
         * \param y y-position of the mouse (in plot coordinates)
         * \param modifiers key-modifiers when the click occured
         * \param deltaAngleX amount of rotation (in eighths of a degree) of the wheel in x-direction
         * \param deltaAngleY amount of rotation (in eighths of a degree) of the wheel in y-direction
         */
        void plotMouseWheelOperated(double x, double y, Qt::KeyboardModifiers modifiers, int deltaAngleX, int deltaAngleY);
        /** \brief emitted when the mouse action jkqtpmdaZoomByRectangle and the drawing of the new zoom rectangle is finished (=mouse key released)
         *
         * \param mouseDragRectXStart start of the selected x-range (in plot coordinates)
         * \param mouseDragRectXEnd end of the selected x-range (in plot coordinates)
         * \param mouseDragRectYStart start of the selected x-range (in plot coordinates)
         * \param mouseDragRectYEnd end of the selected x-range (in plot coordinates)
         * \param modifiers key-modifiers when the click occured
         */
        void plotNewZoomRectangle(double mouseDragRectXStart, double mouseDragRectXEnd, double mouseDragRectYStart, double mouseDragRectYEnd, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the plot scaling has been recalculated */
        void plotScalingRecalculated();
        /** \brief emitted before the plot scaling has been recalculated */
        void beforePlotScalingRecalculate();
        /** \brief emitted whenever a context menu is opened. You can modify the menu via the parameter \a contextMenu!
         *
         * \param x x-position of the context-menu (in plot coordinates)
         * \param y y-position of the context-menu (in plot coordinates)
         * \param contextMenu QMenu-object of the context menu. This object may be altered to display additional items ... Here is an example:
         *                    \code
         *                      contextMenu->addSeparator();
         *                      QAction* act=contextMenu->addMenu(QString("contextMenuOpened(x=%1, y=%2)").arg(x).arg(y))->addAction("user-added action");
         *                      connect(act, &QAction::triggered, [x,y]() { QMessageBox::warning(nullptr, tr("Plot Context Menu"),
         *                                                                                       tr("Context Menu was opened at x/y=%1/%2!").arg(x).arg(y),
         *                                                                                       QMessageBox::Ok,
         *                                                                                       QMessageBox::Ok); });
         *                    \endcode
         *                    <b>Please read the warning below!!!</b>
         *
         * \warning A note of care: This signal can be used to alter the context menu. It behaves differently, depending on
         *          whether you use the standard context menu, or the special context menu (setSpecialContextMenu()). If the
         *          standard menu is opened, your alterations are valid until it is shown the next time (i.e. the internal
         *          context menu object is deleted in between). If you show the special context menu only, your alterations
         *          are <b>permanently stored</b> in the menu object. I.e. if you use the code example above and open the menu
         *          several times, it will accumulate additional menu entries!
         *          If you must modify the special menu, just access actions and do not add or delete any in a slot bound to
         *          this signal! You can recognize the special menu with code like this:
         *          \code
         *                if (contextMenu == plot->getSpecialContextMenu()) {
         *                    //...
         *                    // do something with the special menu, do not add/remove entries
         *                } else {
         *                    //...
         *                    // do something with the standard menu, you are free to do anything!
         *                }
         *          \endcode
         *
         *  \see \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_SPECIALCONTEXTMENU, \ref JKQTPLOTTER_USERMOUSEINTERACTION
         */
        void contextMenuOpened(double x, double y, QMenu* contextMenu);


        /** \brief signal: emitted whenever the user selects a new x-y zoom range (in the major axes, e.g. by mouse, setX(), setY(), setXY(), zoomToFit()... )
         *
         * \param newxmin start of the selected x-range (in plot coordinates)
         * \param newxmax end of the selected x-range (in plot coordinates)
         * \param newymin start of the selected x-range (in plot coordinates)
         * \param newymax end of the selected x-range (in plot coordinates)
         * \param sender JKQTPlotter sending this event
         *
         * This signal is designed to be connected to these Q_SLOTS: synchronizeXAxis(), synchronizeYAxis(), synchronizeXYAxis()
         *
         * \see JKQTBasePlotter::zoomChangedLocally()
         */
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);

        /** \brief signal: emitted whenever the widget is resized
         *
         * \param new_width new width of the widget (in pixels)
         * \param new_height new height of the widget (in pixels)
         * \param sender JKQTPlotter sending this event
         *
         * This signal is designed to be connected to these Q_SLOTS: synchronizeXAxis(), synchronizeYAxis(), synchronizeXYAxis()
         */
        void widgetResized(int new_width, int new_height, JKQTPlotter* sender);

        /** \brief emitted when the mouse action JKQTPlotter::ScribbleEvents and a click event from the mouse occurs inside the plot,
         *         or the mouse moved while the left button is pressed down
         *
         * \param x x-position of the mouse (in plot coordinates)
         * \param y y-position of the mouse (in plot coordinates)
         * \param modifiers key-modifiers when the event was generated
         * \param first if \c true: this is the first event of a series that starts with pressing the mouse-button down, within the series, this is \c false
         * \param last if \c true: this is the last event of a series that ends when releasing the mouse-button, within the series, this is \c false
         */
        void userScribbleClick(double x, double y, Qt::KeyboardModifiers modifiers, bool first, bool last);
        /** \brief emitted when the mouse action JKQTPlotter::RectangleEvents and the drawing of the new rectangle is finished (=mouse key released)
         *
         * \param x x-coordinate of the bottom left corner of the rectangle (in plot coordinates)
         * \param y y-coordinate of the bottom left corner of the rectangle (in plot coordinates)
         * \param width width of the rectangle (in plot coordinates)
         * \param height height of the rectangle (in plot coordinates)
         * \param modifiers key-modifiers when the rectangle was finished
         */
        void userRectangleFinished(double x, double y, double width, double height, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the mouse action JKQTPlotter::LineEvents and the drawing of the new line is finished (=mouse key released)
         *
         * \param x1 x-coordinate of the start of the line (in plot coordinates)
         * \param y1 y-coordinate of the start of the line (in plot coordinates)
         * \param x2 x-coordinate of the end of the line (in plot coordinates)
         * \param y2 y-coordinate of the end of the line (in plot coordinates)
         * \param modifiers key-modifiers when the rectangle was finished
         */
        void userLineFinished(double x1, double y1, double x2, double y2, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the mouse action JKQTPlotter::CircleEvents and the drawing of the new circle is finished (=mouse key released)
         *
         * \param x x-coordinate of the center of the circle (in plot coordinates)
         * \param y y-coordinate of the center of the circle (in plot coordinates)
         * \param radius radius of the circle (in plot coordinates)
         * \param modifiers key-modifiers when the rectangle was finished
         */
        void userCircleFinished(double x, double y, double radius, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the mouse action JKQTPlotter::EllipseEvents and the drawing of the new ellipse is finished (=mouse key released)
         *
         * \param x x-coordinate of the center of the ellipse (in plot coordinates)
         * \param y y-coordinate of the center of the ellipse (in plot coordinates)
         * \param radiusX half-axis in x-direction of the ellipse (in plot coordinates)
         * \param radiusY half-axis in y-direction of the ellipse (in plot coordinates)
         * \param modifiers key-modifiers when the rectangle was finished
         */
        void userEllipseFinished(double x, double y, double radiusX, double radiusY, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when a tooltip for a datapoint is displayed
         *
         * \param x x-coordinate of the center of the marked datapoint (in plot coordinates)
         * \param y y-coordinate of the center of the marked datapoint (in plot coordinates)
         * \param entries contents of the tooltip
         * \param graphs graph objects that created the entries
         */
        void tooltipDisplayed(double x, double y, const QStringList& entries, const QList<JKQTPPlotElement*>& graphs);
        /** \brief emitted when a new ruler between two points is displayed
         *
         * \param x1 x-coordinate of the start of the line (in plot coordinates)
         * \param y1 y-coordinate of the start of the line (in plot coordinates)
         * \param x2 x-coordinate of the end of the line (in plot coordinates)
         * \param y2 y-coordinate of the end of the line (in plot coordinates)
         * \param modifiers key-modifiers when the rectangle was finished
         */
        void rulerDisplayed(double x1, double y1, double x2, double y2, Qt::KeyboardModifiers modifiers);


    protected:

        /** \brief ties a MouseActionMode to a mouse-button and a keyboard-modifier
         *  \internal
         *  \ingroup jkqtpplottersupprt
         */
        struct JKQTPLOTTER_LIB_EXPORT MouseDragAction {
                /** \brief constructs an invalid object */
                MouseDragAction();
                MouseDragAction(Qt::MouseButton _mouseButton, Qt::KeyboardModifiers _modifier, JKQTPMouseDragActions _mode);
                JKQTPMouseDragActions mode;
                Qt::KeyboardModifiers modifier;
                Qt::MouseButton mouseButton;
                bool isValid() const;
                void clear();
            private:
                bool valid;
        };

        /** \brief the currently executed MouseDragAction */
        MouseDragAction currentMouseDragAction;


        /** \brief the currently executed MouseMoveActions */
        QSet<JKQTPMouseMoveActions> currentMouseMoveAction;

        enum class WheelActionType {
            Zoom,
            Pan,
            None
        };

        /** \brief storage for last WheelActionType (used in wheelAction() to distinguish trackpad-pan ations in mode jkqtpmwaZoomByWheelAndTrackpadPan) */
        WheelActionType lastWheelActionType;
        /** \brief storage for timestammp of the last QWheelEvent (used in wheelAction() to distinguish trackpad-pan ations in mode jkqtpmwaZoomByWheelAndTrackpadPan) */
        quint64 lastWheelActionTimestamp;
        /** \brief max. time between two QWheelEvents that are recognized as one series */
        const quint64 maxWheelEventSeriesTimestampDifference;

        /** \brief searches JKQTPlotterStyle::registeredMouseActionModes for a matching action, returns in \a found whether an action was found */
        JKQTPMouseDragActionsHashMapIterator findMatchingMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifiers, bool *found=nullptr) const;

        /** \brief searches JKQTPlotterStyle::registeredMouseMoveActions for a matching action */
        JKQTPMouseMoveActionsHashMapIterator findMatchingMouseMoveAction(Qt::KeyboardModifiers modifiers, bool *found=nullptr) const;

        /** \brief searches JKQTPlotterStyle::registeredMouseWheelActions for a matching action */
        JKQTPMouseWheelActionsHashMapIterator findMatchingMouseWheelAction(Qt::KeyboardModifiers modifiers, bool *found=nullptr) const;

        /** \brief searches JKQTPlotterStyle::registeredMouseDoubleClickActions for a matching action */
        JKQTPMouseDoubleClickActionsHashMapIterator findMatchingMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifiers, bool *found=nullptr) const;

        /** \brief you may overwrite this method to modify the given context menu before it is displayed.
         *
         *  The plotter will fill the menu with the default items and then call this method. The default implementation does NOTHING.
         *
         *  \see \ref JKQTPLOTTER_CONTEXTMENU , \ref JKQTPLOTTER_USERMOUSEINTERACTION
         */
        void modifyContextMenu(QMenu* menu);

        /** \brief indicates whether the plot is updated automatically at the moment
         *
         * \see setPlotUpdateEnabled(), isPlotUpdateEnabled(),
         *      JKQTPlotterUpdateGuard for a guard class coupling calls to setPlotUpdateEnabled() and redrawPlot() with the lifetime of a code block.
         *      See \ref jkqtplotter_general_usage_speedplotsetup for a detailed description.
         */
        bool doDrawing;

        /** \brief JKQTBasePlotter used to plot */
        JKQTBasePlotter* plotter;

        /** \brief modifies the settings of \a plotter to match those of this object */
        void fixBasePlotterSettings();


        /** \brief this is set \c true if we are drawing a zoom rectangle */
        bool mouseDragingRectangle;

        /** \brief when draging the mouse this contains the x-coordinate the user clicked on (in plot coordinates) */
        double mouseDragRectXStart;

        /** \brief when draging the mouse this contains the x-coordinate the user clicked on (in pixels) */
        int mouseDragRectXStartPixel;

        /** \brief when draging the mouse this contains the x-coordinate the mouse is currently
         *         pointing to (in pixels) */
        int mouseDragRectXEndPixel;

        /** \brief when draging the mouse this contains the y-coordinate the mouse is currently
         *         pointing to (in pixels) */
        int mouseDragRectYEndPixel;

        /** \brief when draging the mouse this contains the x-coordinate the mouse is currently
         *         pointing to
         */
        double mouseDragRectXEnd;

        /** \brief when draging the mouse this contains the y-coordinate the user clicked on (in plot coordinates) */
        double mouseDragRectYStart;

        /** \brief when zooming by moving the mouse this contains the y-coordinate the user clicked on (in pixels) */
        int mouseDragRectYStartPixel;

        /** \brief when zooming by moving the mouse this contains the y-coordinate the mouse is currently
         *         pointing to
         */
        double mouseDragRectYEnd;

        /** \brief describes a marker to be drawn by paintUserAction() */
        struct MouseDragMarker{
            inline MouseDragMarker(const QPoint& pos_, const QString& label_, const QString& title_, const QColor& color_, const QImage& keymarker_=QImage(), JKQTPPlotElement* _graph=nullptr):
                pos(pos_), label(label_), title(title_), color(color_), keyMarker(keymarker_), graph(_graph)
            {}
            /** \brief position of the marker in screen pixels */
            QPoint pos;
            /** \brief marker label */
            QString label;
            /** \brief graph label */
            QString title;
            /** \brief color for the marker */
            QColor color;
            /** \brief key marker image */
            QImage keyMarker;
            /** \brief graph that created that marker */
            JKQTPPlotElement* graph;
        };
        /** \brief internal list of markers to be drawn by paintUserAction() */
        QList<MouseDragMarker> mouseDragMarkers;

        typedef QPixmap InternalBufferImageType;

        /** \brief this stores the currently displayed plot */
        InternalBufferImageType image;

        /** \brief this can be used when drawing a zoom rectangle to store an unchanged
         *         copy of the currently displayed image.
         */
        InternalBufferImageType oldImage;

        /** \brief constructs a new image for the internal double-buffering
         *  \internal
         */
        InternalBufferImageType createImageBuffer() const;

        /** \brief returns the required size of an image for the internal double-buffering
         *  \internal
         */
        QSize getImageBufferSize(float* scale_out=nullptr) const;


        /** \brief use this QMenu instance instead of the standard context menu of this widget
         *  \see \ref JKQTPLOTTER_SPECIALCONTEXTMENU
         */
        QMenu* menuSpecialContextMenu;



        /** \brief toolbar class used for user input */
        JKVanishQToolBar* toolbar;


        /** \brief paint the user action (rectangle, ellipse, ... */
        void paintUserAction();

        /** \brief tool function, which corrects the given rectangle (in pixels!) during a panning action. The correction is necesary towards getAbsoluteXY() to prevent an unwanted zooming in. */
        void correctZoomRectForPanning(QRectF& rect) const;


        /** \brief event handler for a double click
         *
         * \see registerMouseDoubleClickAction(), deregisterMouseDoubleClickAction()
         */
        void mouseDoubleClickEvent ( QMouseEvent * event ) override;

        /*! \brief react on key presses.

            These shortcuts are defined:
              - ESC stops current zooming/drawing action
            .
         */
        void keyReleaseEvent(QKeyEvent* event) override;

        /** \brief event handler for a mouse move
         *
         * This implements two behaviours:
         *   - if displayMousePosition is \c true , stores the current mouse position in mousePosX, mousePosY
         *   - if necessary, contributes to user-actions started by mousePressEvent()
         *   - emits plotMouseMove() if the mouse is inside the plot rectangle .
         *   - execute mouseMoveActions
         * .
         *
         * \see mousePosX, mousePosY
         * \see registerMouseWheelAction(), deregisterMouseWheelAction(), registeredMouseWheelActions
         */
        void mouseMoveEvent ( QMouseEvent * event ) override;

        /** \brief event handler for a mouse down event
         *
         * This event determines the action to be performed from registeredMouseActionModes
         * and then sets currentMouseDragAction accordingly and starts the mouse action.
         *
         * \see registerMouseDragAction(), deregisterMouseDragAction(), registeredJKQTPMouseDragActions
         */
        void mousePressEvent ( QMouseEvent * event ) override;

        /** \brief event handler for a mouse release event
         *
         * this finishes the action, started by mousePressEvent()
         */
        void mouseReleaseEvent ( QMouseEvent * event ) override;

        /** \brief event handler for a turn of the mouse wheel
         *
         * Executes the user action defined for the mouse wheel.
         *
         * \see registerMouseWheelAction(), deregisterMouseWheelAction(), registeredMouseWheelActions
         */
        void wheelEvent(QWheelEvent * event) override;

        /** \brief this simply paints the stored image to the widget's surface */
        void paintEvent(QPaintEvent *event) override;

        /** \brief resizes the internal representation (image) of the graphs */
        void resizeEvent(QResizeEvent *event) override;

        /** \brief called, when the mouse leaves the widget, hides the toolbar (if visible) */
        void leaveEvent ( QEvent * event ) override;


        /** \brief update settings of the toolbar */
        void updateToolbar();


        /** \brief the master plotter, this plotter is connected to in x-direction. */
        QPointer<JKQTPlotter> masterPlotterX;
        /** \brief the master plotter, this plotter is connected to in y-direction. */
        QPointer<JKQTPlotter> masterPlotterY;

        /** \brief calculate the y-axis shift of the plot, so there is space for the potentially displayed mouse position label */
        int getPlotYOffset() const;

        /** \brief x-position of the mouse during the last mouseMoveEvent() calls (in plot coordinates) */
        double mousePosX;
        /** \brief y-position of the mouse during the last mouseMoveEvent() calls (in plot coordinates) */
        double mousePosY;
        /** \brief magnification factor for the display of the plot */
        double magnification;
        /** \brief current minimal size of the JKQTPlotter widget to properly display the plot */
        QSize minSize;

        /** \brief the context menu object used by this JKQTPlotter
         *
         *  \note this might be \c ==nullptr occasionally, therefore you need to check it, before accessing it!
         *
         *  \see \ref JKQTPLOTTER_CONTEXTMENU , resetContextMenu(), initContextMenu(), contextMenuMode
         */
        QMenu* contextMenu;
        /** \brief current mode for the default context menu (i.e. the right-click context menu) \see \ref JKQTPLOTTER_CONTEXTMENU  */
        JKQTPContextMenuModes contextMenuMode;
        /** \brief x-position of the mouse (in plot coordinates) when a user mouse-action was started (e.g. drawing a rectangle) */
        double mouseContextX;
        /** \brief y-position of the mouse (in plot coordinates) when a user mouse-action was started (e.g. drawing a rectangle) */
        double mouseContextY;
        /** \brief x-position of the last mouse-click (in screen pixels) */
        int mouseLastClickX;
        /** \brief y-position of the last mouse-click (in screen pixels) */
        int mouseLastClickY;
        /** \brief internal storage for sub-menu entries of the internal contextMenu object, based on the actions returned by JKQTBasePlotter::getLstAdditionalPlotterActions()
         *  \internal
         */
        QList<QMenu*> contextSubMenus;
        /** \brief fills the member contextMenu with all default and additionally registered actions, also calls modifyContextMenu()
         *
         *  \note This function calls resetContextMenu() internally!
         *
         *  \see resetContextMenu(), contextMenuMode
         */
        void initContextMenu();
        /** \brief set the current mouse cursor shappe according to currentMouseDragAction */
        void updateCursor();

        /** \brief current style properties for this JKQTPlotter
         *
         * \see JKQTPSetSystemDefaultStyle(), JKQTPSetSystemDefaultStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        JKQTPlotterStyle plotterStyle;


        /** \brief timer used while the graph is resized to delay the redrawing with new size (in the meantime, an intermediate graphic is displayed)
         * \internal
         * \see delayedResizeEvent()
         *
         * \image html jkqtplotter_fastresizing.gif
         *
         * \see jkqtp_RESIZE_DELAY, setGlobalResizeDelay(), getGlobalResizeDelay(), resizeTimer
         */
        QTimer resizeTimer;

        /** \brief delay for resizing in milliseconds.  If set to 0, resize delay is disabled and resizeTimer is unused.
         *
         * \see jkqtp_RESIZE_DELAY, setGlobalResizeDelay(), getGlobalResizeDelay(), resizeTimer
         */
        static std::atomic<int> jkqtp_RESIZE_DELAY;

        /** \brief destroys the internal contextMenu and optionally creates a new one
         *
         *  \param createnew if \c ==true, contextMenu is reinitialized with a (shiny) new QMenu,
         *                   otherwise it is set to \c nullptr after destroying the old menu.
         *
         *  \see initContextMenu(), contextMenuMode
         */
        void resetContextMenu(bool createnew=true);

        /** \brief fills the inertnal mouseDragMarkers structure with data to display tooltips close to (x0, y0)
         *
         * if \a emitEvent is \c true, the signal is emitted before the function returns
         */
        void fillInternalStructForToolTipOfClosestDataPoint(double x0, double y0, bool emitEvent=true);
        /** \brief resets the currently activated mouse drag action, e.g. called by mouseReleaseEvent() */
        void resetCurrentMouseDragAction();

        /** \brief list of override mouse drag action modes, that override the settings ing plotterStyle.registeredMouseDragActionModes \see setOverrideMouseDragAction(), resetOverrideMouseDragAction(), JKQTPlotterStyle::registeredMouseDragActionModes */
        JKQTPMouseDragActionsHashMap registeredOverrideMouseDragActionModes;

        /** \brief sets an override mouse drag action for the given button/modifiers combination \see setOverrideMouseDragAction(), resetOverrideMouseDragAction(), registeredOverrideMouseDragActionModes */
        void setOverrideMouseDragAction(Qt::MouseButton button,Qt::KeyboardModifiers modifiers, JKQTPMouseDragActions action);
        /** \brief removes a previously set override mouse drag action for the given button/modifiers combination \see setOverrideMouseDragAction(), resetOverrideMouseDragAction(), registeredOverrideMouseDragActionModes */
        void resetOverrideMouseDragAction(Qt::MouseButton button,Qt::KeyboardModifiers modifiers);

        /** \brief action group, that groups the actMouseLeft... actions */
        QActionGroup* actgrpMouseLeft;
        /** \brief action that activates the default action, set in plotterStyle! */
        QAction* actMouseLeftAsDefault;
        /** \brief action that activates the ruler tool (override!) */
        QAction* actMouseLeftAsRuler;
        /** \brief action that activates the tooltip tool (override!)  */
        QAction* actMouseMoveToolTip;
        /** \brief action that activates the zoom rectangle tool (override!) */
        QAction* actMouseLeftAsZoomRect;
        /** \brief action that activates the pan view tool (override!)  */
        QAction* actMouseLeftAsPanView;

        virtual bool event(QEvent *event) override;
    protected Q_SLOTS:
        /** \brief while the window is resized, the plot is only redrawn after a restartable delay, implemented by this function and resizeTimer
        * \internal
        * \see resizeTimer
        *
        * \image html jkqtplotter_fastresizing.gif
        */
        void delayedResizeEvent();

        /** \brief connected to plotScalingRecalculated() of the masterPlotter */
        void masterPlotScalingRecalculated();

        /** \brief called whenever the zoom changes in plotter */
        void pzoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);

        /** \brief emitted before the plot scaling has been recalculated */
        void intBeforePlotScalingRecalculate();
        /** \brief called from a menu entry that encodes the graph ID */
        void reactGraphVisible(bool visible);

        /** \brief action that activates the pan viewport tool \see resetMouseLeftAction(), setMouseLeftActionAsToolTip() */
        void setMouseLeftActionAsPanView();
        /** \brief action that activates the zoom rectangle tool \see resetMouseLeftAction(), setMouseLeftActionAsToolTip() */
        void setMouseLeftActionAsZoomRect();
        /** \brief action that activates the ruler tool \see resetMouseLeftAction(), setMouseLeftActionAsToolTip() */
        void setMouseLeftActionAsRuler();
        /** \brief action that activates the tooltip tool, when dragging the mouse with the left button pressed \see resetMouseLeftAction() */
        void setMouseLeftDragActionAsToolTip();
        /** \brief resets any previously set override action for the left mouse-button, un-modified \see setMouseLeftActionAsRuler(), setMouseLeftActionAsToolTip() */
        void resetMouseLeftAction();
        /** \brief action that (de)activates the tooltip tool, when moving the mouse without any button pressed \see setMouseLeftActionAsRuler(), resetMouseLeftAction() */
        void setMouseMoveActionAsToolTip(bool enabled);

};


/** \brief Guard Class (RAII construct) for JKQTPlotter that disables replotting on construction and reenables it on destruction
 * \ingroup jkqtpplotterclasses
 *
 * This class can be used to forbid replotting (to improve performance) while setting up a plot.
 * The redraw is called on destruction of the guard,i.e. typically when leaving the block that
 * encapsulates the plot construction.
 *
 * \code
 *   {
 *     JKQTPlotterUpdateGuard guard(plotter);
 *
 *     // set up plot here, e.g.
 *     plotter->setX();
 *     plotter->setY(); ...
 *   } // Block ends and immediate plot updates are reenabled. Also JKQTPlotter::redrawPlot() is called.
 * \endcode
 *
 * \see JKQTPlotter::setPlotUpdateEnabled(), JKQTPlotter::redrawPlot()
 * \see JKQTPlotterUpdateGuard for a guard class coupling calls to setPlotUpdateEnabled() and redrawPlot() with the lifetime of a code block.
 *      See \ref jkqtplotter_general_usage_speedplotsetup for a detailed description.
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPlotterUpdateGuard {
public:
    JKQTPlotterUpdateGuard(JKQTPlotter* plot, bool forceRedraw=false);

    ~JKQTPlotterUpdateGuard();

    JKQTPlotterUpdateGuard(const JKQTPlotterUpdateGuard&)=delete;
    JKQTPlotterUpdateGuard& operator=(const JKQTPlotterUpdateGuard&)=delete;
    JKQTPlotterUpdateGuard(JKQTPlotterUpdateGuard&& other) noexcept;
    JKQTPlotterUpdateGuard& operator=(JKQTPlotterUpdateGuard&& other) noexcept;

    /** \brief release the lock, if \a forceRedraw equals \c true JKQTPlotter::redrawPlot() is called in any case,
     *         if it is \c false, JKQTPlotter::redrawPlot() is called only if JKQTPlotter::isPlotUpdateEnabled() was \c true
     *         on construction of the guard */
    void release(bool forceRedraw=false);
private:
    JKQTPlotter* m_plot;
    bool m_oldEnabled;
    bool m_forceRedraw;
};




QT_BEGIN_NAMESPACE

/** \brief qHash-variant used by JKQTPlotter
 *  \internal
 *  \ingroup jkqtpplottersupprt
*/
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
inline size_t qHash(const QPair<Qt::MouseButton,Qt::KeyboardModifiers> &key, size_t /*seed=0*/) {
#else
inline uint qHash(const QPair<Qt::MouseButton,Qt::KeyboardModifiers> &key, uint /*seed=0*/) {
#endif
    std::size_t seed=0;
    jkqtp_hash_combine(seed, key.first);
    jkqtp_hash_combine(seed, key.second);
    return seed;
}

/** \brief qHash-variant used by JKQTPlotter
 *  \internal
 *  \ingroup jkqtpplottersupprt
*/
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
inline size_t qHash(const Qt::MouseButton &key, size_t /*seed=0*/) {
#else
inline uint qHash(const Qt::MouseButton &key, uint /*seed=0*/) {
#endif
    return qHash(static_cast<uint>(key),0);
}

/** \brief qHash-variant used by JKQTPlotter
 *  \internal
 *  \ingroup jkqtpplottersupprt
*/
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
inline size_t qHash(const Qt::KeyboardModifiers &key, size_t /*seed=0*/) {
#else
inline uint qHash(const Qt::KeyboardModifiers &key, uint /*seed=0*/) {
#endif
    return qHash(static_cast<uint>(key),0);
}

QT_END_NAMESPACE

#endif // JKQTPLOTTER_H
