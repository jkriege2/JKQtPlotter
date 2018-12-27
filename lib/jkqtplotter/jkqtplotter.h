/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




/**
 * \defgroup jkqtplotter Qt based Scientific Plotter Class
 * \ingroup tools_qt
 *
 * A Qt based plotter for 2D scientific graphs.
 */

/** \file jkqtplotter.h
  * \ingroup jkqtplotter
  *
  * A Qt based plotter for 2D scientific graphs.
  */
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

#include <vector>
#include <cmath>
#include <iostream>

#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottergui/jkvanishqtoolbar.h"
#include <QKeyEvent>

#ifndef JKQTPLOTTER_H
#define JKQTPLOTTER_H

/** \brief initialized Qt-ressources necessary for JKQtPlotter */
LIB_EXPORT void initJKQtPlotterResources();


/** \brief class to plot function graphs in linear or (semi-)logarithmic scale
 * \ingroup jkqtplotter
 *
 * This class is an implementation of JKQtPlotterBase. It uses the tools from this base class
 * to display function graphs that use the internal datastore as data source. You can add graphs
 * to this component which are described by a JKQTPplotElement struct.
 */
class LIB_EXPORT JKQtPlotter: public QWidget {
        Q_OBJECT
    public:
        enum MouseActionModes {
            ZoomRectangle,
            RectangleEvents,
            /*PolygonEvents,*/
            CircleEvents,
            EllipseEvents,
            LineEvents,
            ScribbleEvents,
            NoMouseAction,
            ClickEvents
        };

        enum RightMouseButtonAction {
            RightMouseButtonNone=0,
            RightMouseButtonZoom=1,
            RightMouseButtonContextMenu=2
        };

        enum LeftDoubleClickAction {
            LeftDoubleClickDefault,
            LeftDoubleClickContextMenu,
            LeftDoubleClickSpecialContextMenu,
        };




        /** \brief class constructor
         */
        explicit JKQtPlotter(bool datastore_internal, QWidget* parent=nullptr, JKQTPdatastore* datast=nullptr);
        explicit JKQtPlotter(QWidget* parent=nullptr);

        /** \brief class destructor */
        virtual ~JKQtPlotter();

        /** reinitializes the toolbar, i.e. fills in QActions added to the QWidget since its creation/the last call to this function */
        virtual void updateToolbarActions();


        /** \brief set the width/height of the icons in the toolbar in pixels */
        inline void set_toolbarIconSize(int value) {
            toolbarIconSize=value;
            QSize s=QSize(toolbarIconSize, toolbarIconSize);
            toolbar->setIconSize(s);
        }

        /** \brief get the width/height of the icons in the toolbar in pixels */
        inline int get_toolbarIconSize() {
            return toolbarIconSize;
        }

        /** \brief returns the class internally used for plotting */
        JKQtBasePlotter* get_plotter() const { return plotter; }
        /** \brief returns the class internally used for plotting */
        const JKQtBasePlotter* get_constplotter() const { return const_cast<const JKQtBasePlotter*>(plotter); }

        JKQTPGET_SET_MACRO_I(bool, displayToolbar, updateToolbar())
        JKQTPGET_SET_MACRO_I(bool, toolbarAlwaysOn, updateToolbar())
        JKQTPGET_SET_MACRO(bool, displayMousePosition)
        JKQTPGET_SET_MACRO(bool, displayCustomMousePosition)
        JKQTPGET_SET_MACRO_I(QColor, userActionColor, update())
        JKQTPGET_SET_MACRO_I(QPainter::CompositionMode, userActionCompositionMode, update())
        JKQTPGET_SET_MACRO_I(MouseActionModes, mouseActionMode, updateCursor())

        /** \brief loads the plot properties from a QSettings object */
        virtual void loadSettings(QSettings& settings, QString group=QString("plots"));

        /** \brief saves the plot properties into a QSettings object.
         *
         * This method only saves those properties that differ from their default value.
         */
        virtual void saveSettings(QSettings& settings, QString group=QString("plots"));

        /** \brief returns the minimum size of the widget */
        QSize minimumSizeHint() const;

        /** \brief returns the size of the widget */
        QSize sizeHint() const;

        /*! \brief synchronize the plot borders with a given plotter

            This function allows two plotters to draw a graph with exactly the same height or width
            as in another graph. For example if you want to have two plotters which are positioned one
            above the other (and have the same widget widths, which could be guaranteed by a QLayout)
            you may want to make sure that their plotWidth s are always the same. In this case call
            \code plotter2->synchronizeToMaster(plotter1, true, false) \endcode of the lower plotter \c plotter2 .
            Now whenever the size of plotter1 changes, also plotter2 is redrawn with the changed
            borders.

            \param master the plotter widget to synchronize to
            \param synchronizeWidth do you want the plot width to be synchronized?
            \param synchronizeHeight do you want the plot height to be synchronized?
        */
        void synchronizeToMaster(JKQtPlotter* master, bool synchronizeWidth, bool synchronizeHeight);

        /** \brief switches any synchronization off, that has been created by synchronizeToMaster() */
        void resetMasterSynchronization();


        /** \brief returns a pointer to the datastore used by this object */
        inline JKQTPdatastore* getDatastore() { return plotter->getDatastore(); }

        /** \brief tells the plotter object to use the given external datastore.
         *
         * If the current datastore is internally managed, this method will free that object and use the supplied datastore
         * with external management. If the current datastore is already external, this method will simply replace it by the
         * new one.
         */
        inline void useExternalDatastore(JKQTPdatastore* newStore) { plotter->useExternalDatastore(newStore); }

        /** \brief tells the plotter object to use the given external datastore and treat it as an internal one (i.e. free it
         *         when the plotter object ist destroyed.
         */
        inline void useAsInternalDatastore(JKQTPdatastore* newStore) { plotter->useAsInternalDatastore(newStore); }

        /** \brief tells the plotter object to use an internal datastore. A new internal datastore object is generated only if
         *         the current datastore is not internal.
         */
        inline void useInternalDatastore() { plotter->useInternalDatastore(); }

        /** \brief tells the plotter object to use an internal datastore (just like useInternalDatastore() ), but forces the
         *         generation of a new datastore, even if the current one is already internal (the current one will be freed in
         *         the lather case */
        inline void forceInternalDatastore() { plotter->forceInternalDatastore(); }

        inline void set_emitSignals(bool sig) { plotter->set_emitSignals(sig); }
        inline bool get_emitSignals() { return plotter->get_emitSignals(); }

        JKQTPGetMacro(bool, doDrawing)
        void set_doDrawing(bool enable);

        bool get_zoomByMouseRectangle() const;
        void set_zoomByMouseRectangle(bool zomByrectangle);

        //GET_SET_MACRO(bool, zoomByDoubleAndRightMouseClick);
        JKQTPGET_SET_MACRO(RightMouseButtonAction, rightMouseButtonAction)
        JKQTPGET_SET_MACRO(LeftDoubleClickAction, leftDoubleClickAction)
        JKQTPGET_MACRO(QMenu*, menuSpecialContextMenu)
        void set_menuSpecialContextMenu(QMenu* menu);

        JKQTPGET_SET_MACRO(bool, zoomByMouseWheel)

        JKQTPGetMacro(double, mouseContextX)
        JKQTPGetMacro(double, mouseContextY)
        JKQTPGetMacro(int, mouseLastClickX)
        JKQTPGetMacro(int, mouseLastClickY)

        inline JKQTPhorizontalAxis* get_xAxis() { return plotter->get_xAxis(); }
        inline JKQTPverticalAxis* get_yAxis() { return plotter->get_yAxis(); }
        inline const JKQTPhorizontalAxis* get_xAxis() const { return plotter->get_xAxis(); }
        inline const JKQTPverticalAxis* get_yAxis() const { return plotter->get_yAxis(); }

       /** \brief returns description of i'th graph */
        inline JKQTPplotElement* getGraph(size_t i) { return plotter->getGraph(i); }

        /** \brief returns the number of graphs */
        inline size_t getGraphCount() { return plotter->getGraphCount(); }

        /** \brief remove the i-th graph */
        inline void deleteGraph(size_t i, bool deletegraph=true) { plotter->deleteGraph(i, deletegraph); }

        /** \brief returns \c true, if the given graph is present */
        inline bool containsGraph(JKQTPplotElement* gr) { return plotter->containsGraph(gr); }

        /** \brief remove the given graph, if it is contained */
        inline void deleteGraph(JKQTPplotElement* gr, bool deletegraph=true) { plotter->deleteGraph(gr, deletegraph);  };

        /** \brief remove all plots
         *
         *  \param deleteGraphs if set \c true (default) the graph objects will also be deleted
         */
        inline void clearGraphs(bool deleteGraphs=true) { plotter->clearGraphs(deleteGraphs); }

        /** \brief add a new graph, returns it's position in the graphs list */
        inline size_t addGraph(JKQTPplotElement* gr) { return plotter->addGraph(gr); }

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        inline size_t moveGraphTop(JKQTPplotElement* gr) { return plotter->moveGraphTop(gr); }

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        inline size_t moveGraphBottom(JKQTPplotElement* gr) { return plotter->moveGraphBottom(gr); }

        /** \brief add a new graphs from a QVector<JKQTPplotElement*>, QList<JKQTPplotElement*>, std::vector<JKQTPplotElement*> ... or any standard-iterateable container with JKQTPplotElement*-items */
        template <class TJKQTPgraphContainer>
        inline void addGraphs(const TJKQTPgraphContainer& gr) { plotter->addGraphs(gr); }

        /** \brief sets minimum and maximum x-value to plot */
        inline void setX(double xminn, double xmaxx) { plotter->setX(xminn, xmaxx); }

        /** \brief sets minimum and maximum y-value to plot */
        inline void setY(double yminn, double ymaxx) { plotter->setY(yminn, ymaxx); }

        /** \brief sets minimum and maximum x- and y-values to plot */
        inline void setXY(double xminn, double xmaxx, double yminn, double ymaxx) { plotter->setXY(xminn, xmaxx, yminn, ymaxx); }

        /** \brief returns the current x-axis min */
        inline double getXMin() const {return plotter->getXMin(); }

        /** \brief returns the current x-axis max */
        inline double getXMax() const {return plotter->getXMax(); }

        /** \brief returns the current y-axis min */
        inline double getYMin() const {return plotter->getYMin(); }

        /** \brief returns the current y-axis max */
        inline double getYMax() const {return plotter->getYMax(); }




        /** \brief sets absolute minimum and maximum x-value to plot */
        inline void setAbsoluteX(double xminn, double xmaxx) { plotter->setAbsoluteX(xminn, xmaxx); }

        /** \brief sets absolute minimum and maximum y-value to plot */
        inline void setAbsoluteY(double yminn, double ymaxx) { plotter->setAbsoluteY(yminn, ymaxx); }

        /** \brief sets absolute minimum and maximum x- and y-values to plot */
        inline void setAbsoluteXY(double xminn, double xmaxx, double yminn, double ymaxx) { plotter->setAbsoluteXY(xminn, xmaxx, yminn, ymaxx); }

        /** \brief returns the absolute x-axis min */
        inline double getAbsoluteXMin() const {return plotter->getAbsoluteXMin(); }
        /** \brief returns the absolute x-axis max */
        inline double getAbsoluteXMax() const {return plotter->getAbsoluteXMax(); }

        /** \brief returns the absolute y-axis min */
        inline double getAbsoluteYMin() const {return plotter->getAbsoluteYMin(); }

        /** \brief returns the absolute y-axis max */
        inline double getAbsoluteYMax() const {return plotter->getAbsoluteYMax(); }

        inline double getMagnification() const { return magnification; }

        /** \brief gets the next unused style id, i.e. the smalles number >=0 which is not contained in usedStyles */
        inline int getNextStyle() {
            return get_plotter()->getNextStyle();
        }

        /** \brief returns a QPen object for the i-th plot style */
        inline JKQtBasePlotter::JKQTPPen getPlotStyle(int i) const {
            return get_constplotter()->getPlotStyle(i);
        }

        /** \brief font face for key labels */
        inline QString get_keyFont() const {
            return get_constplotter()->get_keyFont();
        }

        /** \brief font size for key labels [in points] */
        inline double get_keyFontSize() const {
            return get_constplotter()->get_keyFontSize();
        }
    public slots:
        /** \brief set the plot magnification */
        void setMagnification(double m);
        /** \brief sets x/ymin and x/ymax to the supplied values and replots the graph (zoom operation!) */
        inline void zoom(double nxmin, double nxmax, double nymin, double nymax) {
            plotter->zoom(nxmin, nxmax, nymin, nymax);
        }

        /** \brief sets whether to plot grid lines or not */
        inline void setGrid(bool val) {
            plotter->setGrid(val);
        }

        /** \brief save the current plot as an image file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed.
        *          The image format is extracted from the file extension (jpeg, tiff, png, pdf, ...) */
        inline void saveImage(QString filename=QString(""), bool displayPreview=true) {
            plotter->saveImage(filename, displayPreview);
        }

        /** \brief save the data used for the current plot. The file format is extracted from the file extension (csv, ...)
         *
         * The parameter \a format specifies the export format. if it is empty the format will be choosen according to the file extension, or
         * if \a filename is also empty the format will be choosen according to what is selected in the file selection dialog.
         *
         * If \a format is \c "slk" the output will be in SYLK format, if \a format is \c "csv" or \a "dat" the output will be comma separated values
         * and if \a format is \c "txt" the output will be tab separated values.
         */
        inline void saveData(QString filename=QString(""), QString format=QString("")) {
            plotter->saveData(filename, format);
        }

        /** \brief print the current plot, if printer is \c nullptr a printer selection dialog is displayed */
        inline void print(QPrinter* printer=nullptr) {
            plotter->print(printer);
        }

        /** \brief copy displayed data to cpliboard */
        inline void copyData() {
            plotter->copyData();
        }

        /** \brief copy displayed data to cpliboard in Matlab syntax */
        inline void copyDataMatlab() {
            plotter->copyDataMatlab();
        }
        /** \brief this method zooms the graph so that all plotted datapoints are visible.
         *
         * \param zoomX if set \c true (default) zooms the x axis
         * \param zoomY if set \c true (default) zooms the y axis
         * \param includeX0 if this is \c true zoomToFit() will ensure that \f$ x=0 \f$ is visible in the plot (only for non-logx plots, default: false)
         * \param includeY0 if this is \c true zoomToFit() will ensure that \f$ y=0 \f$ is visible in the plot (only for non-logy plots, default: false)
         * \param scaleX the plot will have a width of \f$ \mbox{Xscale}\cdot\Delta x \f$ where \f$ \Delta x \f$ is the actual x-axis data range
         *               For logx plots we actually use this on the logarithmized data! (default: 1.05)
         * \param scaleY the plot will have a height of \f$ \mbox{Yscale}\cdot\Delta < \f$ where \f$ \Delta < \f$ is the actual <-axis data range
         *               For log< plots we actually use this on the logarithmized data! (default: 1.05)
         *
         */
        inline void zoomToFit(bool zoomX=true, bool zoomY=true, bool includeX0=false, bool includeY0=false, double scaleX=1.05, double scaleY=1.05) {
            plotter->zoomToFit(zoomX, zoomY, includeX0, includeY0, scaleX, scaleY);
        }

        /** \brief zooms into the graph (the same as turning the mouse wheel) by the given factor */
        inline void zoomIn(double factor=2.0) { plotter->zoomIn(factor); };
        /** \brief zooms out of the graph (the same as turning the mouse wheel) by the given factor */
        inline void zoomOut(double factor=2.0) { plotter->zoomOut(factor); };

        /** \brief update the plot */
        void update_plot();

        /** \brief update the plot and replot immediately */
        void replot_plot();

        /** \brief update overlays only */
        void replot_overlays();

        /** \brief update overlays only */
        void update_overlays();


        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x-axis to the other x-axis */
        void synchronizeXAxis(double newxmin, double newxmax, double newymin, double newymax, JKQtPlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local y-axis to the other y-axis */
        void synchronizeYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQtPlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x- and y-axis to the other x- and y-axis */
        void synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQtPlotter* sender);


        /** \brief popuplate the given toolbar with all actions shown in a toolbar from this class ... */
        virtual void populateToolbar(QToolBar* toolbar) const;

        void setMousePositionLabel(const QString& text);

        void openContextMenu();
        void openContextMenu(int x, int y);
        void openSpecialContextMenu();
        void openSpecialContextMenu(int x, int y);
    signals:
        /** \brief signal: emitted whenever the mouse moved over the plot */
        void plotMouseMove(double x, double y);
        /** \brief signal: emitted whenever the mouse was clicked over the plot */
        void plotMouseClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        /** \brief signal: emitted whenever the mouse was clicked over the plot */
        void plotMouseDoubleClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        /** \brief signal: emitted whenever a zoom rectangle is drawn (and it's size changes) */
        void plotNewZoomRectangle(double mouseDragRectXStart, double mouseDragRectXEnd, double mouseDragRectYStart, double mouseDragRectYEnd);
        /** \brief emitted when the plot scaling has been recalculated */
        void plotScalingRecalculated();
        /** \brief emitted before the plot scaling has been recalculated */
        void beforePlotScalingRecalculate();
        /** \brief emitted when a context-emnu was opened at the given position */
        void contextMenuOpened(double x, double y, QMenu* contextMenu);


        /** \brief signal: emitted whenever the user selects a new x-y zoom range (by mouse) */
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQtPlotter* sender);

        /** \brief emitted when the user draws a rectangle */
        void userClickFinished(double x, double y, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the user draws a rectangle */
        void userScribbleClick(double x, double y, Qt::KeyboardModifiers modifiers, bool first, bool last);
        /** \brief emitted when the user draws a rectangle */
        void userRectangleFinished(double x, double y, double width, double height, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the user draws a line */
        void userLineFinished(double x1, double y1, double x2, double y2, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the user draws a circle */
        void userCircleFinished(double x, double y, double radius, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the user draws an ellipse */
        void userEllipseFinished(double x, double y, double radiusX, double radiusY, Qt::KeyboardModifiers modifiers);


    protected:

        /** \brief you may overwrite this method to modify the given context emnu before it is displayed.
         *
         *  The plotter will fill the menu with the default items and then call this method. The default implementation does NOTHING.
         */
        virtual void modifyContextMenu(QMenu* menu);

        void init(bool datastore_internal, QWidget* parent, JKQTPdatastore* datast);

        MouseActionModes mouseActionMode;

        bool doDrawing;

        /** \brief JKQtPlotterBase used to plot */
        JKQtBasePlotter* plotter;


        /** \brief fill color of the zoom rectangle */
        JKQTPPROPERTY(QColor, userActionColor)

        /** \brief fill color of the zoom rectangle */
        JKQTPPROPERTY(QPainter::CompositionMode, userActionCompositionMode)

        /** \brief width/height of the icons in the plotter toolbar in pixels */
        JKQTPPROPERTY(int, toolbarIconSize)

        /** \brief this is set \c true if we are drawing a zoom rectangle */
        bool mouseDragingRectangle;

        /** \brief when zooming by moving the mouse this contains the x-coordinate the user clicked on */
        double mouseDragRectXStart;

        /** \brief when zooming by moving the mouse this contains the x-coordinate the mouse is currently
         *         pointing to
         */
        double mouseDragRectXEnd;

        /** \brief when zooming by moving the mouse this contains the y-coordinate the user clicked on */
        double mouseDragRectYStart;

        /** \brief when zooming by moving the mouse this contains the y-coordinate the mouse is currently
         *         pointing to
         */
        double mouseDragRectYEnd;

        /** \brief this stores the currently displayed plot */
        QImage image;

        /** \brief this stores the currently displayed plot */
        QImage imageNoOverlays;

        /** \brief this can be used when drawing a zoom rectangle to store an unchanged
         *         copy of the currently displayed image.
         */
        QImage oldImage;

        /** \brief indicates whether zooming in by double clicking and zooming out by right-clicking is activated */
        //bool zoomByDoubleAndRightMouseClick;
        RightMouseButtonAction rightMouseButtonAction;

        /** \brief indicates whether zooming using the mouse-wheel is activated */
        bool zoomByMouseWheel;

        /** \brief indicates the action to perform on a left mouse-button double-click */
        LeftDoubleClickAction leftDoubleClickAction;
        QMenu* menuSpecialContextMenu;



        /** \brief toolbar class used for user input */
        JKVanishQToolBar* toolbar;


        /** \brief paint the user action (rectangle, ellipse, ... */
        void paintUserAction();


        /** \brief event handler for a double click (zoom in on time axis) */
        void mouseDoubleClickEvent ( QMouseEvent * event );

        /*! \brief react on key presses.

            These shortcuts are defined:
              - ESC stops current zooming/drawing action
            .
         */
        void keyReleaseEvent(QKeyEvent* event);

        /** \brief event handler for a mouse move
         *
         * This implements two behaviours:
         *   - If no mouse button is pressed this simply emits a plotMouseMove() event which
         *     allows for some other widget to display the current position of the mouse inside
         *     the plot
         *   - If the left mouse button is pressed (private property mouseZooming is \c true )
         *     this displays a rectangle. If the mouse is release ( mouseReleaseEvent() ) the control
         *     zooms the specified region. So you can use the mouse to select a range to zoom into.
         *     This stores the current mouse position in mouseZoomingTEnd .
         * .
         */
        void mouseMoveEvent ( QMouseEvent * event );

        /** \brief event handler for a mouse down event
         *
         * If the left mouse button is pressed down this starts the drawing of a zoom rectangle with
         * the mouse. This method sets mouseZooming to \c true and stores the current time in the private
         * property mouseZoomingTStart. All this is only executed when the mouse is inside the coordinate
         * system.
         *
         * If the right mouse button is clicked this zooms out of the coordinate system by a factor of two.
         */
        void mousePressEvent ( QMouseEvent * event );

        /** \brief event handler for a mouse release event
         *
         * If the left mouse button is released (and mouseZooming is \c true ) this stops drawing a
         * zoom rectangle and emits a tzoomChangedLocally() event.
         */
        void mouseReleaseEvent ( QMouseEvent * event );

        /** \brief event handler for a turn of the mouse wheel
         *         turning forward the mous wheel will zoom out and turning it backwards will zoom
         *         in by a factor of \f$ 2^{\mbox{delta\_wheel}} \f$. */
        void wheelEvent(QWheelEvent * event);

        /** \brief this simply paints the stored image to the widget's surface */
        void paintEvent(QPaintEvent *event);

        /** \brief resizes the internal representation (image) of the graphs */
        void resizeEvent(QResizeEvent *event);


        void leaveEvent ( QEvent * event );

        /** \brief specifies whether to display a toolbar with buttons when the mouse is in the upper border of the plot */
        bool displayToolbar;
        /** ßbrief specifies whether the toolbar is always visible or only when the mouse moves to the upper left area */
        bool toolbarAlwaysOn;

        void updateToolbar();

        /** \brief specifies whether to write the current position of the mouse in the top border of the plot (this may automatically extent the
                   top border, so the position fits in. The default widget font is used for the output. */
        bool displayMousePosition;
        /** \brief this string is used to generate the position output above the graph */
        JKQTPPROPERTY(QString, mousePositionTemplate)
        /** \brief if set \c true and displayMousePosition is \c true, the mouse position is not automatically determined, but the text given to setMousePositionLabel() is used */
        bool displayCustomMousePosition;
        QString customMousePositiontext;

        /** \brief the master plotter, this plotter is connected to. */
        QPointer<JKQtPlotter> masterPlotter;

        /** \brief calculate the y-axis shift of the plot, so there is space for the potentially displayed mouse position label */
        int getPlotYOffset();

        double mousePosX;
        double mousePosY;

        double magnification;

        QSize minSize;

        QMenu* contextMenu;
        double mouseContextX;
        double mouseContextY;
        int mouseLastClickX;
        int mouseLastClickY;
        QList<QMenu*> contextSubMenus;
        void initContextMenu();

        void updateCursor();


        QTimer resizeTimer;
    protected slots:
        void delayedResizeEvent();

        /** \brief connected to plotScalingRecalculated() of the masterPlotter */
        void masterPlotScalingRecalculated();

        /** \brief called whenever the zoom changes in plotter */
        void pzoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQtBasePlotter* sender);

        /** \brief emitted before the plot scaling has been recalculated */
        void intBeforePlotScalingRecalculate();
        /** \brief called from a menu entry that encodes the graph ID */
        void reactGraphVisible(bool visible);

};


#endif // JKQTPLOTTER_H
