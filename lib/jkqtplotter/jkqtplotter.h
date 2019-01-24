/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

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



/** \file jkqtplotter.h
  * \ingroup jkqtpplotterclasses
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

/** \brief initialized Qt-ressources necessary for JKQTPlotter
 *  \ingroup jkqtpplotterclasses
*/
LIB_EXPORT void initJKQTPlotterResources();


/** \brief class to plot function graphs in linear or (semi-)logarithmic scale
 * \ingroup jkqtpplotterclasses
 *
 * This class is an implementation of JKQTPlotterBase. It uses the tools from this base class
 * to display function graphs that use the internal datastore as data source. You can add graphs
 * to this component which are described by a JKQTPPlotElement struct.
 *
 * \section JKQTPLOTTER_USERINTERACTION User-Interactions
 *
 * JKQTPlotter offers a lot of user-interaction features out of the box. These are detailed below.
 *
 * \subsection JKQTPLOTTER_CONTEXTMENU Conext Menu of JKQTPlotter
 *
 * The class JKQTPlotter has a context emnu that already offers a lot of functionality.
 *
 * \image html jkqtplotter_defaultcontextmenu.png
 *
 * It allows to:
 *   - copy or save the data from the internal JKQTPDatastore
 *   - copy or save the plot to an image file (PNG, PDF, ...), includes a softisticated export-preview dialog:
 *
 *     \image html jkqtplotter_exportpreviewdialog.png
 *
 *   - print the plot (includes a softisticated print-preview dialog):
 *
 *     \image html jkqtplotter_printpreview.png
 *     \image html jkqtplotter_printpreview_relsize_mullinewidth_mulfonts.png
 *
 *   - open a dialog with a table of the plot data:
 *
 *     \image html jkqtplotter_datatabledialog.png
 *
 *   - zoom into/out of the plot and determine an auto-zoom, which shows all of the plot data
 *   - switch the visibility of the different graphs in the plot
 * .
 *
 * \subsection JKQTPLOTTER_TOOLBAR Toolbar of JKQTPlotter
 *
 * In addition to the context-menu, a JKQtPlotter also also provides a toolbar at its top that offers
 * most of the functionality of the context menu. Usually this toolbar is enabled (see displayToolbar)
 * and is in a mode where it is hidden, until the mouse moves over an area at the top of the plot (see toolbarAlwaysOn):
 *
 * \image html jkqtplotter_toolbar_hidden.png "Hidden Toolbar"
 * \image html jkqtplotter_toolbar_shown.png "Shown Toolbar"
 *
 * If toolbarAlwaysOn is set to \c true (set_toolbarAlwaysOn() ), the toolbar is always displayed:
 *
 * \image html jkqtplotter_toolbar_alwayson.png
 *
 * \see displayToolbar, toolbarAlwaysOn, \ref JKQTPlotterUserInteractio
 *
 *
 *
 * \subsection JKQTPLOTTER_ACTIONS QActions from a JKQTPlotter
 * Often you want to allow the suer to operate a plot from a user-defined QToolBar or a QMenu/QMenuBar in your
 * application (e.g. provide zooming commands ...). there are generally two ways to achieve this:
 *   # Simply connect home-brewn QAction instances to the slots provided by JKQTPlotter and JKQTBasePlotter.
 *     This also allows you to connect different plot properties to edit widgets in your forms.
 *   # For several functions (especially those also present in JKQTPlotter's context-emun, you can also find
 *     readily available QAction instances. these are available from JKQTBasePlotter (e.g. by JKQTBasePlotter::get_actPrint() ...).
 *     From JKQTPlotter you therefor have to use: <code>get_plotter()->get_actPrint()</code>
 * .
 * \see \subpage JKQTPlotterUserInteraction
 *
 *
 *
 * \subsection JKQTPLOTTER_USERMOUSEINTERACTION User Mouse-Interaction in JKQTPlotter
 *
 *
 *
 *
 *
 *
 * \section JKQTPLOTTER_USEQTCREATOR  How to use JKQTPlotter in the Qt Form Designer
 *
 * As JKQTPlotter is a standard Qt widget, you can also use it in Qt UI-files designed with the Qt From Designer (e.g. from within QTCreator).
 * For this to work you have to use the "Promote QWidget"-feature of the form designer. The steps you need to take are detailed below:
 *   # add a new UI-file to your project and open it in the Form Editor. Then right-click the form and select `Promote Widgets ...`:<br>
 *    \image html uidesigner_step1.png
 *
 *   # In the dialog that opens, you have to define `JKQTPlotter` as a promotion to `QWidget` as shown below. Finally store the settings by clicking `Add` and closing the dialog with `Close`.<br>
 *    \image html uidesigner_step2.png
 *
 *   # Now you can add a `QWidget`from the side-bar to the form and then promote it to `JKQTPlotter`, by selecting and right-clicking the `QWidget` and then selecting `Promote To | JKQTPlotter`:<br>
 *    \image html uidesigner_step3.png
 * .
 *
 * \see \subpage JKQTPlotterQtCreator
 *
 */
class LIB_EXPORT JKQTPlotter: public QWidget {
        Q_OBJECT
    public:
        /** \brief availble user-action mode this JKQtPlotter use when mouse events occur.
         *         This allows you to e.g. draw rectangles or lines over the plot and receive a signal, when the drawing finishes */
        enum MouseActionModes {
            ZoomRectangle, /*!< \brief draw a rectangle and when finish zoom to that rectangle */
            RectangleEvents, /*!< \brief draw a rectangle and when finished execute the signal userRectangleFinished() */
            CircleEvents, /*!< \brief draw a circle and when finished execute the signal userCircleFinished() */
            EllipseEvents, /*!< \brief draw an ellipse and when finished execute the signal userEllipseFinished()  */
            LineEvents, /*!< \brief draw a line and when finished execute the signal userLineFinished() */
            ScribbleEvents, /*!< \brief let the user scribble on the plot (left mouse button is kept pressed) and call userScribbleClick() for each new position  */
            NoMouseAction, /*!< \brief no action is to be performed */
            ClickEvents /*!< \brief sinply call userClickFinished() for every single-click of the mouse button */
        };

        /** \brief options of how to react to a right mouse button click */
        enum RightMouseButtonAction {
            RightMouseButtonNone=0, /*!< \brief do not perform any action on a right mouse button click */
            RightMouseButtonZoom=1, /*!< \brief use right mouse button for zoomin out */
            RightMouseButtonContextMenu=2 /*!< \brief show the context menu when clicking the right mouse button */
        };

        /** \brief options of how to react to a left mouse button double-click (single-click events are described by MouseActionModes) */
        enum LeftDoubleClickAction {
            LeftDoubleClickDefault, /*!< \brief */
            LeftDoubleClickContextMenu, /*!< \brief open the context menu when the left mouse button is double-clicked */
            LeftDoubleClickSpecialContextMenu, /*!< \brief open the special context menu when the left mouse button is double-clicked */
        };




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
        virtual void updateToolbarActions();


        /** \brief set the width/height of the icons in the toolbar in pt */
        void set_toolbarIconSize(int value);

        /** \brief get the width/height of the icons in the toolbar in pt */
        int get_toolbarIconSize();

        /** \brief returns the class internally used for plotting */
        JKQTBasePlotter* get_plotter() const { return plotter; }
        /** \brief returns the class internally used for plotting */
        const JKQTBasePlotter* get_constplotter() const { return const_cast<const JKQTBasePlotter*>(plotter); }

        /*! \brief returns the property displayToolbar.
            \details Description of the parameter displayToolbar is: <BLOCKQUOTE>\copydoc displayToolbar </BLOCKQUOTE>
            \see displayToolbar for more information */ 
        virtual bool get_displayToolbar() const;
        /*! \brief returns the property toolbarAlwaysOn.
            \details Description of the parameter toolbarAlwaysOn is: <BLOCKQUOTE>\copydoc toolbarAlwaysOn </BLOCKQUOTE>
            \see toolbarAlwaysOn for more information */ 
        virtual bool get_toolbarAlwaysOn() const;
        /*! \brief returns the property displayMousePosition.
            \details Description of the parameter displayMousePosition is: <BLOCKQUOTE>\copydoc displayMousePosition </BLOCKQUOTE>
            \see displayMousePosition for more information */ 
        virtual bool get_displayMousePosition() const;
        /*! \brief returns the property userActionColor.
            \details Description of the parameter userActionColor is: <BLOCKQUOTE>\copydoc userActionColor </BLOCKQUOTE>
            \see userActionColor for more information */ 
        virtual QColor get_userActionColor() const;

        /*! \brief returns the property userActionCompositionMode. 
            \details Description of the parameter userActionCompositionMode is: <BLOCKQUOTE>\copydoc userActionCompositionMode </BLOCKQUOTE>
            \see userActionCompositionMode for more information */ 
        virtual QPainter::CompositionMode get_userActionCompositionMode() const;

        /*! \brief returns the current mouseActionMode.
            \details Description of the parameter mouseActionMode is: <BLOCKQUOTE> specifies the user-action mode this JKQtPlotter use when mouse events occur.
         *         This allows you to e.g. draw rectangles or lines over the plot and receive a signal, when the drawing finishes </BLOCKQUOTE>
            \see mouseActionMode for more information */ 
        virtual MouseActionModes get_mouseActionMode() const;

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
        void synchronizeToMaster(JKQTPlotter* master, bool synchronizeWidth, bool synchronizeHeight);

        /** \brief switches any synchronization off, that has been created by synchronizeToMaster() */
        void resetMasterSynchronization();


        /** \brief returns a pointer to the datastore used by this object */
        inline JKQTPDatastore* getDatastore() { return plotter->getDatastore(); }

        /** \brief tells the plotter object to use the given external datastore.
         *
         * If the current datastore is internally managed, this method will free that object and use the supplied datastore
         * with external management. If the current datastore is already external, this method will simply replace it by the
         * new one.
         */
        inline void useExternalDatastore(JKQTPDatastore* newStore) { plotter->useExternalDatastore(newStore); }

        /** \brief tells the plotter object to use the given external datastore and treat it as an internal one (i.e. free it
         *         when the plotter object ist destroyed.
         */
        inline void useAsInternalDatastore(JKQTPDatastore* newStore) { plotter->useAsInternalDatastore(newStore); }

        /** \brief tells the plotter object to use an internal datastore. A new internal datastore object is generated only if
         *         the current datastore is not internal.
         */
        inline void useInternalDatastore() { plotter->useInternalDatastore(); }

        /** \brief tells the plotter object to use an internal datastore (just like useInternalDatastore() ), but forces the
         *         generation of a new datastore, even if the current one is already internal (the current one will be freed in
         *         the lather case */
        inline void forceInternalDatastore() { plotter->forceInternalDatastore(); }

        /** \brief switch emitting of signals, such as zoomChangedLocally() ..., on (sig=true) or off (sig=false) */
        inline void set_emitSignals(bool sig) { plotter->set_emitSignals(sig); }
        /** \brief determine, whether emitting of signals, such as zoomChangedLocally() ..., is switched on or off */
        inline bool get_emitSignals() { return plotter; }

        /** \brief returns, whether updating the plot is currently activated (e.g. you can deactivate this with set_doDrawing() while performing major updates on the plot)
         *
         * \see set_doDrawing()
         */
        bool get_doDrawing() const;
        /** \brief sets whether updating the plot is currently activated (e.g. you can sett his to \c false while performing major updates on the plot)
         *
         * \see get_doDrawing();
         */
        void set_doDrawing(bool enable);

        /*! \brief returns the property rightMouseButtonAction.
            \details Description of the parameter rightMouseButtonAction is: <BLOCKQUOTE>\copydoc rightMouseButtonAction </BLOCKQUOTE>
            \see rightMouseButtonAction for more information */
        bool get_zoomByMouseRectangle() const;

        /*! \brief sets the property rightMouseButtonAction to the specified \a __value. 
            \details Description of the parameter rightMouseButtonAction is: <BLOCKQUOTE>\copydoc rightMouseButtonAction </BLOCKQUOTE>
            \see rightMouseButtonAction for more information */ 
        void set_rightMouseButtonAction(const RightMouseButtonAction & __value);
        /*! \brief returns the property rightMouseButtonAction. 
            \details Description of the parameter rightMouseButtonAction is: <BLOCKQUOTE>\copydoc rightMouseButtonAction </BLOCKQUOTE>
            \see rightMouseButtonAction for more information */ 
        virtual RightMouseButtonAction get_rightMouseButtonAction() const;
        /*! \brief sets the property leftDoubleClickAction to the specified \a __value. 
            \details Description of the parameter leftDoubleClickAction is: <BLOCKQUOTE>\copydoc leftDoubleClickAction </BLOCKQUOTE>
            \see leftDoubleClickAction for more information */ 
        void set_leftDoubleClickAction(const LeftDoubleClickAction & __value);
        /*! \brief returns the property leftDoubleClickAction. 
            \details Description of the parameter leftDoubleClickAction is: <BLOCKQUOTE>\copydoc leftDoubleClickAction </BLOCKQUOTE>
            \see leftDoubleClickAction for more information */ 
        virtual LeftDoubleClickAction get_leftDoubleClickAction() const;
        /*! \brief returns the property menuSpecialContextMenu. \details Description of the parameter menuSpecialContextMenu is:  <BLOCKQUOTE>\copydoc menuSpecialContextMenu </BLOCKQUOTE>. \see menuSpecialContextMenu for more information */
        QMenu *get_menuSpecialContextMenu() const;

        /*! \brief set the property menuSpecialContextMenu. \details Description of the parameter menuSpecialContextMenu is:  <BLOCKQUOTE>\copydoc menuSpecialContextMenu </BLOCKQUOTE>. \see menuSpecialContextMenu for more information */
        void set_menuSpecialContextMenu(QMenu* menu);

        /*! \brief sets the property zoomByMouseWheel to the specified \a __value. 
            \details Description of the parameter zoomByMouseWheel is: <BLOCKQUOTE>\copydoc zoomByMouseWheel </BLOCKQUOTE>
            \see zoomByMouseWheel for more information */ 
        void set_zoomByMouseWheel(bool __value);
        /*! \brief returns the property zoomByMouseWheel. 
            \details Description of the parameter zoomByMouseWheel is: <BLOCKQUOTE>\copydoc zoomByMouseWheel </BLOCKQUOTE>
            \see zoomByMouseWheel for more information */ 
        virtual bool get_zoomByMouseWheel() const;

        /** \brief returns the property mouseContextX. 
            \details Description of the parameter mouseContextX is:  <BLOCKQUOTE>\copydoc mouseContextX </BLOCKQUOTE>.
            \see mouseContextX for more information */ 
        double get_mouseContextX() const;
        /** \brief returns the property mouseContextY. 
            \details Description of the parameter mouseContextY is:  <BLOCKQUOTE>\copydoc mouseContextY </BLOCKQUOTE>.
            \see mouseContextY for more information */ 
        double get_mouseContextY() const;
        /** \brief returns the property mouseLastClickX. 
            \details Description of the parameter mouseLastClickX is:  <BLOCKQUOTE>\copydoc mouseLastClickX </BLOCKQUOTE>.
            \see mouseLastClickX for more information */ 
        int get_mouseLastClickX() const;
        /** \brief returns the property mouseLastClickY. 
            \details Description of the parameter mouseLastClickY is:  <BLOCKQUOTE>\copydoc mouseLastClickY </BLOCKQUOTE>.
            \see mouseLastClickY for more information */ 
        int get_mouseLastClickY() const;

        /** \brief returns the coordinate axis object for the x-axis */
        inline JKQTPHorizontalAxis* get_xAxis() { return plotter->get_xAxis(); }
        /** \brief returns the coordinate axis object for the y-axis */
        inline JKQTPVerticalAxis* get_yAxis() { return plotter->get_yAxis(); }
        /** \brief returns the coordinate axis object for the x-axis as a const pointer */
        inline const JKQTPHorizontalAxis* get_xAxis() const { return plotter->get_xAxis(); }
        /** \brief returns the coordinate axis object for the y-axis as a const pointer */
        inline const JKQTPVerticalAxis* get_yAxis() const { return plotter->get_yAxis(); }

       /** \brief returns description of i'th graph */
        inline JKQTPPlotElement* getGraph(size_t i) { return plotter->getGraph(i); }

        /** \brief returns the number of graphs */
        inline size_t getGraphCount() { return plotter->getGraphCount(); }

        /** \brief remove the i-th graph */
        inline void deleteGraph(size_t i, bool deletegraph=true) { plotter->deleteGraph(i, deletegraph); }

        /** \brief returns \c true, if the given graph is present */
        inline bool containsGraph(JKQTPPlotElement* gr) { return plotter->containsGraph(gr); }

        /** \brief remove the given graph, if it is contained */
        inline void deleteGraph(JKQTPPlotElement* gr, bool deletegraph=true) { plotter->deleteGraph(gr, deletegraph);  }

        /** \brief remove all plots
         *
         *  \param deleteGraphs if set \c true (default) the graph objects will also be deleted
         */
        inline void clearGraphs(bool deleteGraphs=true) { plotter->clearGraphs(deleteGraphs); }

        /** \brief add a new graph, returns it's position in the graphs list */
        inline size_t addGraph(JKQTPPlotElement* gr) { return plotter->addGraph(gr); }

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        inline size_t moveGraphTop(JKQTPPlotElement* gr) { return plotter->moveGraphTop(gr); }

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        inline size_t moveGraphBottom(JKQTPPlotElement* gr) { return plotter->moveGraphBottom(gr); }

        /** \brief add a new graphs from a QVector<JKQTPPlotElement*>, QList<JKQTPPlotElement*>, std::vector<JKQTPPlotElement*> ... or any standard-iterateable container with JKQTPPlotElement*-items */
        template <class TJKQTPGraphContainer>
        inline void addGraphs(const TJKQTPGraphContainer& gr) { plotter->addGraphs(gr); }


        /** \brief returns the current x-axis min */
        inline double getXMin() const {return plotter->getXMin(); }

        /** \brief returns the current x-axis max */
        inline double getXMax() const {return plotter->getXMax(); }

        /** \brief returns the current y-axis min */
        inline double getYMin() const {return plotter->getYMin(); }

        /** \brief returns the current y-axis max */
        inline double getYMax() const {return plotter->getYMax(); }



        /** \brief returns the absolute x-axis min */
        inline double getAbsoluteXMin() const {return plotter->getAbsoluteXMin(); }
        /** \brief returns the absolute x-axis max */
        inline double getAbsoluteXMax() const {return plotter->getAbsoluteXMax(); }

        /** \brief returns the absolute y-axis min */
        inline double getAbsoluteYMin() const {return plotter->getAbsoluteYMin(); }

        /** \brief returns the absolute y-axis max */
        inline double getAbsoluteYMax() const {return plotter->getAbsoluteYMax(); }

        /** \brief returns the current magnification factor */
        inline double getMagnification() const { return magnification; }

        /** \brief gets the next unused style id, i.e. the smalles number >=0 which is not contained in usedStyles */
        inline int getNextStyle() {
            return get_plotter()->getNextStyle();
        }

        /** \brief returns a QPen object for the i-th plot style */
        inline JKQTBasePlotter::JKQTPPen getPlotStyle(int i) const {
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
        /** \brief set the current plot magnification */
        void setMagnification(double m);
        /** \brief sets x/ymin and x/ymax to the supplied values and replots the graph (zoom operation!) */
        inline void zoom(double nxmin, double nxmax, double nymin, double nymax) {
            plotter->zoom(nxmin, nxmax, nymin, nymax);
        }

        /** \brief sets whether to plot grid lines or not
         *
         * \image html jkqtplotter_gridvisible.png "Grid visible"
         * \image html jkqtplotter_gridinvisible.png "Grid invisible"
         * */
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
        inline void zoomIn(double factor=2.0) { plotter->zoomIn(factor); }
        /** \brief zooms out of the graph (the same as turning the mouse wheel) by the given factor */
        inline void zoomOut(double factor=2.0) { plotter->zoomOut(factor); }

        /** \brief update the plot */
        void update_plot();

        /** \brief update the plot and replot immediately */
        void replot_plot();

        /** \brief update overlays only */
        void replot_overlays();

        /** \brief update overlays only */
        void update_overlays();

        /*! \brief sets the property displayToolbar to the specified \a __value.
            \details Description of the parameter displayToolbar is: <BLOCKQUOTE>\copydoc displayToolbar </BLOCKQUOTE>
            \see displayToolbar for more information */
        void set_displayToolbar(bool __value);
        /*! \brief sets the property toolbarAlwaysOn to the specified \a __value.
            \details Description of the parameter toolbarAlwaysOn is: <BLOCKQUOTE>\copydoc toolbarAlwaysOn </BLOCKQUOTE>
            \see toolbarAlwaysOn for more information */
        void set_toolbarAlwaysOn(bool __value);
        /*! \brief sets the property displayMousePosition to the specified \a __value.
            \details Description of the parameter displayMousePosition is: <BLOCKQUOTE>\copydoc displayMousePosition </BLOCKQUOTE>
            \see displayMousePosition for more information */
        void set_displayMousePosition(bool __value);
        /*! \brief sets the property userActionColor to the specified \a __value.
            \details Description of the parameter userActionColor is: <BLOCKQUOTE>\copydoc userActionColor </BLOCKQUOTE>
            \see userActionColor for more information */
        void set_userActionColor(const QColor & __value);
        /*! \brief sets the property userActionCompositionMode to the specified \a __value.
            \details Description of the parameter userActionCompositionMode is: <BLOCKQUOTE>\copydoc userActionCompositionMode </BLOCKQUOTE>
            \see userActionCompositionMode for more information */
        void set_userActionCompositionMode(const QPainter::CompositionMode & __value);
        /*! \brief sets the current mouseActionMode .
            \details Description of the parameter mouseActionMode is: <BLOCKQUOTE> specifies the user-action mode this JKQtPlotter use when mouse events occur.
         *         This allows you to e.g. draw rectangles or lines over the plot and receive a signal, when the drawing finishes </BLOCKQUOTE>
            \see mouseActionMode for more information */
        void set_mouseActionMode(const MouseActionModes & __value);
        /*! \brief sets the property rightMouseButtonAction to the specified \a __value.
            \details Description of the parameter rightMouseButtonAction is: <BLOCKQUOTE>\copydoc rightMouseButtonAction </BLOCKQUOTE>
            \see rightMouseButtonAction for more information */
        void set_zoomByMouseRectangle(bool zomByrectangle);


        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x-axis to the other x-axis */
        void synchronizeXAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local y-axis to the other y-axis */
        void synchronizeYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x- and y-axis to the other x- and y-axis */
        void synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);


        /** \brief popuplate the given toolbar with all actions shown in a toolbar from this class ... */
        virtual void populateToolbar(QToolBar* toolbar) const;

        /** \brief open the context menu at the mouse position of the last click */
        void openContextMenu();
        /** \brief open the context menu at the mouse position \a x and \a y */
        void openContextMenu(int x, int y);
        /** \brief open the special context menu at the mouse position of the last click */
        void openSpecialContextMenu();
        /** \brief open the special context menu at the mouse position \a x and \a y */
        void openSpecialContextMenu(int x, int y);


        /** \brief sets absolute minimum and maximum x-value to plot */
        inline void setAbsoluteX(double xminn, double xmaxx) { plotter->setAbsoluteX(xminn, xmaxx); }

        /** \brief sets absolute minimum and maximum y-value to plot */
        inline void setAbsoluteY(double yminn, double ymaxx) { plotter->setAbsoluteY(yminn, ymaxx); }

        /** \brief sets absolute minimum and maximum x- and y-values to plot */
        inline void setAbsoluteXY(double xminn, double xmaxx, double yminn, double ymaxx) { plotter->setAbsoluteXY(xminn, xmaxx, yminn, ymaxx); }

        /** \brief sets minimum and maximum x-value to plot */
        inline void setX(double xminn, double xmaxx) { plotter->setX(xminn, xmaxx); }

        /** \brief sets minimum and maximum y-value to plot */
        inline void setY(double yminn, double ymaxx) { plotter->setY(yminn, ymaxx); }

        /** \brief sets minimum and maximum x- and y-values to plot */
        inline void setXY(double xminn, double xmaxx, double yminn, double ymaxx) { plotter->setXY(xminn, xmaxx, yminn, ymaxx); }

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
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);

        /** \brief emitted when the user clicks the plot */
        void userClickFinished(double x, double y, Qt::KeyboardModifiers modifiers);
        /** \brief emitted when the user scribbles */
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

        void init(bool datastore_internal, QWidget* parent, JKQTPDatastore* datast);

        /** \brief specifies the user-action mode this JKQtPlotter use when mouse events occur.
         *         This allows you to e.g. draw rectangles or lines over the plot and receive a signal, when the drawing finishes */
        MouseActionModes mouseActionMode;

        bool doDrawing;

        /** \brief JKQTPlotterBase used to plot */
        JKQTBasePlotter* plotter;


        /** \brief fill color of the zoom rectangle */
        QColor userActionColor;
        /*! \brief default value for property property varname. \see userActionColor for more information */ 
        QColor def_userActionColor;

        /** \brief fill color of the zoom rectangle */
        QPainter::CompositionMode userActionCompositionMode;
        /*! \brief default value for property property varname. \see userActionCompositionMode for more information */ 
        QPainter::CompositionMode def_userActionCompositionMode;

        /** \brief width/height of the icons in the plotter toolbar in pt */
        int toolbarIconSize;
        /*! \brief default value for property property varname. \see toolbarIconSize for more information */ 
        int def_toolbarIconSize;

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
        RightMouseButtonAction rightMouseButtonAction;

        /** \brief indicates whether zooming using the mouse-wheel is activated */
        bool zoomByMouseWheel;

        /** \brief indicates the action to perform on a left mouse-button double-click */
        LeftDoubleClickAction leftDoubleClickAction;

        /** \brief use this QMenu instance instead of the standard context emnu of this widget */
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

        /** \brief specifies whether to display a toolbar with buttons when the mouse is in the upper border of the plot
          *
          * \image html jkqtplotter_toolbar_alwayson.png
          *
          * \see toolbarAlwaysOn, \ref JKQTPlotterUserInteraction
          */
        bool displayToolbar;
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
          * \see displayToolbar, \ref JKQTPlotterUserInteraction
          */
        bool toolbarAlwaysOn;

        void updateToolbar();

        /** \brief specifies whether to display the current position of the mouse in the top border of the plot (this may automatically extent the
          *        top border, so the position fits in. The default widget font is used for the output.
          *
          * \image html jkqtplotter_mousepositiondisplay.png
          *
          * \see mousePositionTemplate, \ref JKQTPlotterUserInteraction
          */
        bool displayMousePosition;
        /** \brief this string is used to generate the position output above the graph (\c %1 is replaced by the x-position, \c %2 by the y-position)
         *
         * By default simply <code>"(%1, %2)</code> is used to format this display (e.g. <code>(1.35, -4.56)</code>).
         *
         * \image html jkqtplotter_mousepositiondisplay.png
         *
         * \see mousePositionTemplate, \ref JKQTPlotterUserInteraction */
        QString mousePositionTemplate;
        /*! \brief default value for property property varname. \see mousePositionTemplate for more information */ 
        QString def_mousePositionTemplate;

        /** \brief the master plotter, this plotter is connected to. */
        QPointer<JKQTPlotter> masterPlotter;

        /** \brief calculate the y-axis shift of the plot, so there is space for the potentially displayed mouse position label */
        int getPlotYOffset();

        double mousePosX;
        double mousePosY;

        double magnification;

        QSize minSize;

        QMenu* contextMenu;
        /** \brief x-position of the mouse (in plot coordinates) when a user mouse-action was started (e.g. drawing a rectangle) */
        double mouseContextX;
        /** \brief y-position of the mouse (in plot coordinates) when a user mouse-action was started (e.g. drawing a rectangle) */
        double mouseContextY;
        /** \brief x-position of the last mouse-click (in screen pixels) */
        int mouseLastClickX;
        /** \brief y-position of the last mouse-click (in screen pixels) */
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
        void pzoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);

        /** \brief emitted before the plot scaling has been recalculated */
        void intBeforePlotScalingRecalculate();
        /** \brief called from a menu entry that encodes the graph ID */
        void reactGraphVisible(bool visible);

};


#endif // JKQTPLOTTER_H
